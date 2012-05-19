//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2011 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id$
//============================================================================

#include <fstream>
#include <cmath>

#include "FrameBuffer.hxx"
#include "bspf.hxx"
#include "PNGLibrary.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
PNGLibrary::PNGLibrary(const string& filename)
  : myFilename(filename)
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
PNGLibrary::~PNGLibrary()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool PNGLibrary::readImage(const FrameBuffer& fb, FBSurface& surface)
    throw(const char*)
{
  #define readImageERROR(s) { err_message = s; goto done; }

  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_bytep* row_pointers = NULL;
  png_uint_32 iwidth, iheight, ipitch;
  uInt8* buffer = NULL;
  int bit_depth, color_type, interlace_type;
  const char* err_message = NULL;

  ifstream* in = new ifstream(myFilename.c_str(), ios_base::binary);
  if(!in || !in->is_open())
    readImageERROR("No image found");

  // Create the PNG loading context structure
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                 png_user_error, png_user_warn);
  if(png_ptr == NULL)
    readImageERROR("Couldn't allocate memory for PNG file");

  // Allocate/initialize the memory for image information.  REQUIRED.
	info_ptr = png_create_info_struct(png_ptr);
  if(info_ptr == NULL)
    readImageERROR("Couldn't create image information for PNG file");

  // Set up the input control
  png_set_read_fn(png_ptr, in, png_read_data);

  // Read PNG header info
  png_read_info(png_ptr, info_ptr);
  png_get_IHDR(png_ptr, info_ptr, &iwidth, &iheight, &bit_depth,
    &color_type, &interlace_type, NULL, NULL);

  // Tell libpng to strip 16 bit/color files down to 8 bits/color
  png_set_strip_16(png_ptr);

  // Extract multiple pixels with bit depths of 1, 2, and 4 from a single
  // byte into separate bytes (useful for paletted and grayscale images).
  png_set_packing(png_ptr);

  // Only normal RBG(A) images are supported (without the alpha channel)
  if(color_type == PNG_COLOR_TYPE_RGBA)
  {
    png_set_strip_alpha(png_ptr);
  }
  else if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
  {
    readImageERROR("Greyscale PNG images not supported");
  }
  else if(color_type == PNG_COLOR_TYPE_PALETTE)
  {
    readImageERROR("Paletted PNG images not supported");
  }
  else if(color_type != PNG_COLOR_TYPE_RGB)
  {
    readImageERROR("Unknown format in PNG image");
  }

  // Create space for the entire image (3 bytes per pixel in RGB format)
  ipitch = iwidth * 3;
  buffer = new uInt8[ipitch * iheight];
  if(buffer == NULL)
    readImageERROR("Not enough memory to read PNG file");

  // The PNG read function expects an array of rows, not a single 1-D array
  row_pointers = new png_bytep[iheight];
  for(uInt32 irow = 0, offset = 0; irow < iheight; ++irow, offset += ipitch)
    row_pointers[irow] = (png_bytep) (uInt8*)buffer + offset;

  // Read the entire image in one go
  png_read_image(png_ptr, row_pointers);
  delete[] row_pointers;

  // We're finished reading
  png_read_end(png_ptr, info_ptr);

  // Scale image to surface dimensions
  scaleImagetoSurface(iwidth, iheight, buffer, fb, surface);

  // Cleanup
done:
  if(png_ptr)
    png_destroy_read_struct(&png_ptr, info_ptr ? &info_ptr : (png_infopp)0, (png_infopp)0);
  if(in)
    in->close();
  delete[] buffer;

  if(err_message)
    throw err_message;
  else
    return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PNGLibrary::scaleImagetoSurface(uInt32 iwidth, uInt32 iheight, uInt8* buffer,
                 const FrameBuffer& fb, FBSurface& surface)
{
  // Figure out the original zoom level of the snapshot
  // All snapshots generated by Stella are at most some multiple of 320
  // pixels wide
  // The only complication is when the aspect ratio is changed, the width
  // can range from 256 (80%) to 320 (100%)
  // The following calculation will work up to approx. 16x zoom level,
  // but since Stella only generates snapshots at up to 10x, we should
  // be fine for a while ...
  uInt32 izoom = uInt32(ceil(iwidth/320.0)),
         szoom = surface.getWidth()/320;

  uInt32 sw = iwidth / izoom * szoom,
         sh = iheight / izoom * szoom;
  sw = BSPF_min(sw, surface.getWidth());
  sh = BSPF_min(sh, surface.getHeight());
  surface.setWidth(sw);
  surface.setHeight(sh);

  // Decompress the image, and scale it correctly
  uInt32 ipitch = iwidth * 3;  // bytes per line of the actual PNG image
  uInt32* line  = new uInt32[ipitch];

  uInt32 buf_offset = ipitch * izoom;
  uInt32 i_offset = 3 * izoom;

  // We can only scan at most the height of the image to the constraints of
  // the surface height (some multiple of 256)
  iheight = BSPF_min(iheight, izoom * 256);

  // Grab each non-duplicate row of data from the image
  for(uInt32 irow = 0, srow = 0; irow < iheight; irow += izoom, buffer += buf_offset)
  {
    // Scale the image data into the temporary line buffer
    uInt8*  i_ptr = buffer;
    uInt32* l_ptr = line;
    for(uInt32 icol = 0; icol < iwidth; icol += izoom, i_ptr += i_offset)
    {
      uInt32 pixel = fb.mapRGB(*i_ptr, *(i_ptr+1), *(i_ptr+2));
      uInt32 xstride = szoom;
      while(xstride--)
        *l_ptr++ = pixel;
    }

    // Then fill the surface with those bytes
    uInt32 ystride = szoom;
    while(ystride--)
      surface.drawPixels(line, 0, srow++, sw);
  }
  delete[] line;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PNGLibrary::png_read_data(png_structp ctx, png_bytep area, png_size_t size)
{
  ifstream* stream = (ifstream *) png_get_io_ptr(ctx);
  stream->read((char *)area, size);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PNGLibrary::png_write_data(png_structp ctx, png_bytep area, png_size_t size)
{
  ofstream* stream = (ofstream *) png_get_io_ptr(ctx);
  stream->write((const char *)area, size);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PNGLibrary::png_io_flush(png_structp ctx)
{
  ofstream* stream = (ofstream *) png_get_io_ptr(ctx);
  stream->flush();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PNGLibrary::png_user_warn(png_structp ctx, png_const_charp str)
{
  cout << "PNGLibrary warning: " << str << endl;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PNGLibrary::png_user_error(png_structp ctx, png_const_charp str)
{
  cout << "PNGLibrary error: " << str << endl;
}