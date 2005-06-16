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
// Copyright (c) 1995-2005 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: ConsoleFont.hxx,v 1.2 2005-06-16 00:55:59 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef CONSOLE_FONT_DATA_HXX
#define CONSOLE_FONT_DATA_HXX

#include "bspf.hxx"

namespace GUI {

/* Font information:
   name: win_crox0c
   facename: -Cronyx-Fixed-Medium-R-Normal--10-100-75-75-C-60-Windows-1251
   w x h: 6x10
   size: 96
   ascent: 8
   descent: 2
   first char: 32 (0x20)
   last char: 127 (0x7f)
   default char: 32 (0x20)
   proportional: no
   Copyright (C) 1994 Cronyx Ltd.
*/

/* Font character bitmap data. */
static const uInt16 _console_font_bits[] = {

/* Character 32 (0x20):
   width 6
   +------+
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,

/* Character 33 (0x21):
   width 6
   +------+
   |      |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |      |
   |   *  |
   |      |
   |      |
   +------+
*/
0x0000,
0x1000,
0x1000,
0x1000,
0x1000,
0x1000,
0x0000,
0x1000,
0x0000,
0x0000,

/* Character 34 (0x22):
   width 6
   +------+
   |      |
   |  * * |
   |  * * |
   |  * * |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x2800,
0x2800,
0x2800,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,

/* Character 35 (0x23):
   width 6
   +------+
   |      |
   |  * * |
   |  * * |
   | *****|
   |  * * |
   | *****|
   |  * * |
   |  * * |
   |      |
   |      |
   +------+
*/
0x0000,
0x2800,
0x2800,
0x7c00,
0x2800,
0x7c00,
0x2800,
0x2800,
0x0000,
0x0000,

/* Character 36 (0x24):
   width 6
   +------+
   |      |
   |   *  |
   |  *** |
   | * *  |
   |  *** |
   |   * *|
   |  *** |
   |   *  |
   |      |
   |      |
   +------+
*/
0x0000,
0x1000,
0x3800,
0x5000,
0x3800,
0x1400,
0x3800,
0x1000,
0x0000,
0x0000,

/* Character 37 (0x25):
   width 6
   +------+
   |      |
   |  *  *|
   | * * *|
   |  * * |
   |   *  |
   |  * * |
   | * * *|
   | *  * |
   |      |
   |      |
   +------+
*/
0x0000,
0x2400,
0x5400,
0x2800,
0x1000,
0x2800,
0x5400,
0x4800,
0x0000,
0x0000,

/* Character 38 (0x26):
   width 6
   +------+
   |      |
   |  *   |
   | * *  |
   | * *  |
   |  *   |
   | * * *|
   | *  * |
   |  ** *|
   |      |
   |      |
   +------+
*/
0x0000,
0x2000,
0x5000,
0x5000,
0x2000,
0x5400,
0x4800,
0x3400,
0x0000,
0x0000,

/* Character 39 (0x27):
   width 6
   +------+
   |      |
   |   ** |
   |   *  |
   |  *   |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x1800,
0x1000,
0x2000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,

/* Character 40 (0x28):
   width 6
   +------+
   |      |
   |    * |
   |   *  |
   |  *   |
   |  *   |
   |  *   |
   |   *  |
   |    * |
   |      |
   |      |
   +------+
*/
0x0000,
0x0800,
0x1000,
0x2000,
0x2000,
0x2000,
0x1000,
0x0800,
0x0000,
0x0000,

/* Character 41 (0x29):
   width 6
   +------+
   |      |
   |  *   |
   |   *  |
   |    * |
   |    * |
   |    * |
   |   *  |
   |  *   |
   |      |
   |      |
   +------+
*/
0x0000,
0x2000,
0x1000,
0x0800,
0x0800,
0x0800,
0x1000,
0x2000,
0x0000,
0x0000,

/* Character 42 (0x2a):
   width 6
   +------+
   |      |
   |      |
   | *   *|
   |  * * |
   | *****|
   |  * * |
   | *   *|
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x4400,
0x2800,
0x7c00,
0x2800,
0x4400,
0x0000,
0x0000,
0x0000,

/* Character 43 (0x2b):
   width 6
   +------+
   |      |
   |      |
   |   *  |
   |   *  |
   | *****|
   |   *  |
   |   *  |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x1000,
0x1000,
0x7c00,
0x1000,
0x1000,
0x0000,
0x0000,
0x0000,

/* Character 44 (0x2c):
   width 6
   +------+
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |   ** |
   |   *  |
   |  *   |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x1800,
0x1000,
0x2000,
0x0000,

/* Character 45 (0x2d):
   width 6
   +------+
   |      |
   |      |
   |      |
   |      |
   | *****|
   |      |
   |      |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x0000,
0x7c00,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,

/* Character 46 (0x2e):
   width 6
   +------+
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |   *  |
   |  *** |
   |   *  |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x1000,
0x3800,
0x1000,
0x0000,

/* Character 47 (0x2f):
   width 6
   +------+
   |      |
   |     *|
   |     *|
   |    * |
   |   *  |
   |  *   |
   | *    |
   | *    |
   |      |
   |      |
   +------+
*/
0x0000,
0x0400,
0x0400,
0x0800,
0x1000,
0x2000,
0x4000,
0x4000,
0x0000,
0x0000,

/* Character 48 (0x30):
   width 6
   +------+
   |      |
   |   *  |
   |  * * |
   | *   *|
   | *   *|
   | *   *|
   |  * * |
   |   *  |
   |      |
   |      |
   +------+
*/
0x0000,
0x1000,
0x2800,
0x4400,
0x4400,
0x4400,
0x2800,
0x1000,
0x0000,
0x0000,

/* Character 49 (0x31):
   width 6
   +------+
   |      |
   |   *  |
   |  **  |
   | * *  |
   |   *  |
   |   *  |
   |   *  |
   | *****|
   |      |
   |      |
   +------+
*/
0x0000,
0x1000,
0x3000,
0x5000,
0x1000,
0x1000,
0x1000,
0x7c00,
0x0000,
0x0000,

/* Character 50 (0x32):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   |     *|
   |   ** |
   |  *   |
   | *    |
   | *****|
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x0400,
0x1800,
0x2000,
0x4000,
0x7c00,
0x0000,
0x0000,

/* Character 51 (0x33):
   width 6
   +------+
   |      |
   | *****|
   |     *|
   |    * |
   |   ** |
   |     *|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x7c00,
0x0400,
0x0800,
0x1800,
0x0400,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 52 (0x34):
   width 6
   +------+
   |      |
   |    * |
   |   ** |
   |  * * |
   | *  * |
   | *****|
   |    * |
   |    * |
   |      |
   |      |
   +------+
*/
0x0000,
0x0800,
0x1800,
0x2800,
0x4800,
0x7c00,
0x0800,
0x0800,
0x0000,
0x0000,

/* Character 53 (0x35):
   width 6
   +------+
   |      |
   | *****|
   | *    |
   | * ** |
   | **  *|
   |     *|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x7c00,
0x4000,
0x5800,
0x6400,
0x0400,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 54 (0x36):
   width 6
   +------+
   |      |
   |   ** |
   |  *   |
   | *    |
   | * ** |
   | **  *|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x1800,
0x2000,
0x4000,
0x5800,
0x6400,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 55 (0x37):
   width 6
   +------+
   |      |
   | *****|
   |     *|
   |    * |
   |    * |
   |   *  |
   |  *   |
   |  *   |
   |      |
   |      |
   +------+
*/
0x0000,
0x7c00,
0x0400,
0x0800,
0x0800,
0x1000,
0x2000,
0x2000,
0x0000,
0x0000,

/* Character 56 (0x38):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   | *   *|
   |  *** |
   | *   *|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x4400,
0x3800,
0x4400,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 57 (0x39):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   | *  **|
   |  ** *|
   |     *|
   |    * |
   |  **  |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x4c00,
0x3400,
0x0400,
0x0800,
0x3000,
0x0000,
0x0000,

/* Character 58 (0x3a):
   width 6
   +------+
   |      |
   |      |
   |   *  |
   |  *** |
   |   *  |
   |      |
   |   *  |
   |  *** |
   |   *  |
   |      |
   +------+
*/
0x0000,
0x0000,
0x1000,
0x3800,
0x1000,
0x0000,
0x1000,
0x3800,
0x1000,
0x0000,

/* Character 59 (0x3b):
   width 6
   +------+
   |      |
   |      |
   |   *  |
   |  *** |
   |   *  |
   |      |
   |   ** |
   |   *  |
   |  *   |
   |      |
   +------+
*/
0x0000,
0x0000,
0x1000,
0x3800,
0x1000,
0x0000,
0x1800,
0x1000,
0x2000,
0x0000,

/* Character 60 (0x3c):
   width 6
   +------+
   |      |
   |     *|
   |    * |
   |   *  |
   |  *   |
   |   *  |
   |    * |
   |     *|
   |      |
   |      |
   +------+
*/
0x0000,
0x0400,
0x0800,
0x1000,
0x2000,
0x1000,
0x0800,
0x0400,
0x0000,
0x0000,

/* Character 61 (0x3d):
   width 6
   +------+
   |      |
   |      |
   |      |
   | *****|
   |      |
   | *****|
   |      |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x7c00,
0x0000,
0x7c00,
0x0000,
0x0000,
0x0000,
0x0000,

/* Character 62 (0x3e):
   width 6
   +------+
   |      |
   | *    |
   |  *   |
   |   *  |
   |    * |
   |   *  |
   |  *   |
   | *    |
   |      |
   |      |
   +------+
*/
0x0000,
0x4000,
0x2000,
0x1000,
0x0800,
0x1000,
0x2000,
0x4000,
0x0000,
0x0000,

/* Character 63 (0x3f):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   |    * |
   |   *  |
   |   *  |
   |      |
   |   *  |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x0800,
0x1000,
0x1000,
0x0000,
0x1000,
0x0000,
0x0000,

/* Character 64 (0x40):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   | *  **|
   | * * *|
   | * ** |
   | *    |
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x4c00,
0x5400,
0x5800,
0x4000,
0x3800,
0x0000,
0x0000,

/* Character 65 (0x41):
   width 6
   +------+
   |      |
   |   *  |
   |  * * |
   | *   *|
   | *   *|
   | *****|
   | *   *|
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x1000,
0x2800,
0x4400,
0x4400,
0x7c00,
0x4400,
0x4400,
0x0000,
0x0000,

/* Character 66 (0x42):
   width 6
   +------+
   |      |
   | **** |
   |  *  *|
   |  *  *|
   |  *** |
   |  *  *|
   |  *  *|
   | **** |
   |      |
   |      |
   +------+
*/
0x0000,
0x7800,
0x2400,
0x2400,
0x3800,
0x2400,
0x2400,
0x7800,
0x0000,
0x0000,

/* Character 67 (0x43):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   | *    |
   | *    |
   | *    |
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x4000,
0x4000,
0x4000,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 68 (0x44):
   width 6
   +------+
   |      |
   | **** |
   |  *  *|
   |  *  *|
   |  *  *|
   |  *  *|
   |  *  *|
   | **** |
   |      |
   |      |
   +------+
*/
0x0000,
0x7800,
0x2400,
0x2400,
0x2400,
0x2400,
0x2400,
0x7800,
0x0000,
0x0000,

/* Character 69 (0x45):
   width 6
   +------+
   |      |
   | *****|
   | *    |
   | *    |
   | **** |
   | *    |
   | *    |
   | *****|
   |      |
   |      |
   +------+
*/
0x0000,
0x7c00,
0x4000,
0x4000,
0x7800,
0x4000,
0x4000,
0x7c00,
0x0000,
0x0000,

/* Character 70 (0x46):
   width 6
   +------+
   |      |
   | *****|
   | *    |
   | *    |
   | **** |
   | *    |
   | *    |
   | *    |
   |      |
   |      |
   +------+
*/
0x0000,
0x7c00,
0x4000,
0x4000,
0x7800,
0x4000,
0x4000,
0x4000,
0x0000,
0x0000,

/* Character 71 (0x47):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   | *    |
   | *    |
   | *  **|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x4000,
0x4000,
0x4c00,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 72 (0x48):
   width 6
   +------+
   |      |
   | *   *|
   | *   *|
   | *   *|
   | *****|
   | *   *|
   | *   *|
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x4400,
0x4400,
0x4400,
0x7c00,
0x4400,
0x4400,
0x4400,
0x0000,
0x0000,

/* Character 73 (0x49):
   width 6
   +------+
   |      |
   |  *** |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x1000,
0x1000,
0x1000,
0x1000,
0x1000,
0x3800,
0x0000,
0x0000,

/* Character 74 (0x4a):
   width 6
   +------+
   |      |
   |   ***|
   |     *|
   |     *|
   |     *|
   |     *|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x1c00,
0x0400,
0x0400,
0x0400,
0x0400,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 75 (0x4b):
   width 6
   +------+
   |      |
   | *   *|
   | *  * |
   | * *  |
   | **   |
   | * *  |
   | *  * |
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x4400,
0x4800,
0x5000,
0x6000,
0x5000,
0x4800,
0x4400,
0x0000,
0x0000,

/* Character 76 (0x4c):
   width 6
   +------+
   |      |
   | *    |
   | *    |
   | *    |
   | *    |
   | *    |
   | *    |
   | *****|
   |      |
   |      |
   +------+
*/
0x0000,
0x4000,
0x4000,
0x4000,
0x4000,
0x4000,
0x4000,
0x7c00,
0x0000,
0x0000,

/* Character 77 (0x4d):
   width 6
   +------+
   |      |
   | *   *|
   | *   *|
   | ** **|
   | * * *|
   | *   *|
   | *   *|
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x4400,
0x4400,
0x6c00,
0x5400,
0x4400,
0x4400,
0x4400,
0x0000,
0x0000,

/* Character 78 (0x4e):
   width 6
   +------+
   |      |
   | *   *|
   | *   *|
   | **  *|
   | * * *|
   | *  **|
   | *   *|
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x4400,
0x4400,
0x6400,
0x5400,
0x4c00,
0x4400,
0x4400,
0x0000,
0x0000,

/* Character 79 (0x4f):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   | *   *|
   | *   *|
   | *   *|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x4400,
0x4400,
0x4400,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 80 (0x50):
   width 6
   +------+
   |      |
   | **** |
   | *   *|
   | *   *|
   | **** |
   | *    |
   | *    |
   | *    |
   |      |
   |      |
   +------+
*/
0x0000,
0x7800,
0x4400,
0x4400,
0x7800,
0x4000,
0x4000,
0x4000,
0x0000,
0x0000,

/* Character 81 (0x51):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   | *   *|
   | *   *|
   | *   *|
   | * * *|
   |  *** |
   |     *|
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x4400,
0x4400,
0x4400,
0x5400,
0x3800,
0x0400,
0x0000,

/* Character 82 (0x52):
   width 6
   +------+
   |      |
   | **** |
   | *   *|
   | *   *|
   | **** |
   | * *  |
   | *  * |
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x7800,
0x4400,
0x4400,
0x7800,
0x5000,
0x4800,
0x4400,
0x0000,
0x0000,

/* Character 83 (0x53):
   width 6
   +------+
   |      |
   |  *** |
   | *   *|
   | *    |
   |  *** |
   |     *|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x4400,
0x4000,
0x3800,
0x0400,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 84 (0x54):
   width 6
   +------+
   |      |
   | *****|
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |      |
   |      |
   +------+
*/
0x0000,
0x7c00,
0x1000,
0x1000,
0x1000,
0x1000,
0x1000,
0x1000,
0x0000,
0x0000,

/* Character 85 (0x55):
   width 6
   +------+
   |      |
   | *   *|
   | *   *|
   | *   *|
   | *   *|
   | *   *|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x4400,
0x4400,
0x4400,
0x4400,
0x4400,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 86 (0x56):
   width 6
   +------+
   |      |
   | *   *|
   | *   *|
   | *   *|
   |  * * |
   |  * * |
   |  * * |
   |   *  |
   |      |
   |      |
   +------+
*/
0x0000,
0x4400,
0x4400,
0x4400,
0x2800,
0x2800,
0x2800,
0x1000,
0x0000,
0x0000,

/* Character 87 (0x57):
   width 6
   +------+
   |      |
   | *   *|
   | *   *|
   | *   *|
   | * * *|
   | * * *|
   | ** **|
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x4400,
0x4400,
0x4400,
0x5400,
0x5400,
0x6c00,
0x4400,
0x0000,
0x0000,

/* Character 88 (0x58):
   width 6
   +------+
   |      |
   | *   *|
   | *   *|
   |  * * |
   |   *  |
   |  * * |
   | *   *|
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x4400,
0x4400,
0x2800,
0x1000,
0x2800,
0x4400,
0x4400,
0x0000,
0x0000,

/* Character 89 (0x59):
   width 6
   +------+
   |      |
   | *   *|
   | *   *|
   |  * * |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |      |
   |      |
   +------+
*/
0x0000,
0x4400,
0x4400,
0x2800,
0x1000,
0x1000,
0x1000,
0x1000,
0x0000,
0x0000,

/* Character 90 (0x5a):
   width 6
   +------+
   |      |
   | *****|
   |     *|
   |    * |
   |   *  |
   |  *   |
   | *    |
   | *****|
   |      |
   |      |
   +------+
*/
0x0000,
0x7c00,
0x0400,
0x0800,
0x1000,
0x2000,
0x4000,
0x7c00,
0x0000,
0x0000,

/* Character 91 (0x5b):
   width 6
   +------+
   |      |
   |  *** |
   |  *   |
   |  *   |
   |  *   |
   |  *   |
   |  *   |
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x2000,
0x2000,
0x2000,
0x2000,
0x2000,
0x3800,
0x0000,
0x0000,

/* Character 92 (0x5c):
   width 6
   +------+
   |      |
   | *    |
   | *    |
   |  *   |
   |   *  |
   |    * |
   |     *|
   |     *|
   |      |
   |      |
   +------+
*/
0x0000,
0x4000,
0x4000,
0x2000,
0x1000,
0x0800,
0x0400,
0x0400,
0x0000,
0x0000,

/* Character 93 (0x5d):
   width 6
   +------+
   |      |
   |  *** |
   |    * |
   |    * |
   |    * |
   |    * |
   |    * |
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3800,
0x0800,
0x0800,
0x0800,
0x0800,
0x0800,
0x3800,
0x0000,
0x0000,

/* Character 94 (0x5e):
   width 6
   +------+
   |      |
   |   *  |
   |  * * |
   | *   *|
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x1000,
0x2800,
0x4400,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,

/* Character 95 (0x5f):
   width 6
   +------+
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   | *****|
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x7c00,
0x0000,

/* Character 96 (0x60):
   width 6
   +------+
   |      |
   |  **  |
   |   *  |
   |    * |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x3000,
0x1000,
0x0800,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,

/* Character 97 (0x61):
   width 6
   +------+
   |      |
   |      |
   |      |
   |  *** |
   |     *|
   |  ****|
   | *   *|
   |  ****|
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x3800,
0x0400,
0x3c00,
0x4400,
0x3c00,
0x0000,
0x0000,

/* Character 98 (0x62):
   width 6
   +------+
   |      |
   | *    |
   | *    |
   | * ** |
   | **  *|
   | *   *|
   | **  *|
   | * ** |
   |      |
   |      |
   +------+
*/
0x0000,
0x4000,
0x4000,
0x5800,
0x6400,
0x4400,
0x6400,
0x5800,
0x0000,
0x0000,

/* Character 99 (0x63):
   width 6
   +------+
   |      |
   |      |
   |      |
   |  *** |
   | *   *|
   | *    |
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x3800,
0x4400,
0x4000,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 100 (0x64):
   width 6
   +------+
   |      |
   |     *|
   |     *|
   |  ** *|
   | *  **|
   | *   *|
   | *  **|
   |  ** *|
   |      |
   |      |
   +------+
*/
0x0000,
0x0400,
0x0400,
0x3400,
0x4c00,
0x4400,
0x4c00,
0x3400,
0x0000,
0x0000,

/* Character 101 (0x65):
   width 6
   +------+
   |      |
   |      |
   |      |
   |  *** |
   | *   *|
   | *****|
   | *    |
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x3800,
0x4400,
0x7c00,
0x4000,
0x3800,
0x0000,
0x0000,

/* Character 102 (0x66):
   width 6
   +------+
   |      |
   |   ** |
   |  *  *|
   |  *   |
   | **** |
   |  *   |
   |  *   |
   |  *   |
   |      |
   |      |
   +------+
*/
0x0000,
0x1800,
0x2400,
0x2000,
0x7800,
0x2000,
0x2000,
0x2000,
0x0000,
0x0000,

/* Character 103 (0x67):
   width 6
   +------+
   |      |
   |      |
   |      |
   |  ** *|
   | *  * |
   |  **  |
   | *    |
   |  *** |
   | *   *|
   |  *** |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x3400,
0x4800,
0x3000,
0x4000,
0x3800,
0x4400,
0x3800,

/* Character 104 (0x68):
   width 6
   +------+
   |      |
   | *    |
   | *    |
   | * ** |
   | **  *|
   | *   *|
   | *   *|
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x4000,
0x4000,
0x5800,
0x6400,
0x4400,
0x4400,
0x4400,
0x0000,
0x0000,

/* Character 105 (0x69):
   width 6
   +------+
   |      |
   |   *  |
   |      |
   |  **  |
   |   *  |
   |   *  |
   |   *  |
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x1000,
0x0000,
0x3000,
0x1000,
0x1000,
0x1000,
0x3800,
0x0000,
0x0000,

/* Character 106 (0x6a):
   width 6
   +------+
   |      |
   |    * |
   |      |
   |   ** |
   |    * |
   |    * |
   |    * |
   | *  * |
   | *  * |
   |  **  |
   +------+
*/
0x0000,
0x0800,
0x0000,
0x1800,
0x0800,
0x0800,
0x0800,
0x4800,
0x4800,
0x3000,

/* Character 107 (0x6b):
   width 6
   +------+
   |      |
   | *    |
   | *    |
   | *   *|
   | *  * |
   | ***  |
   | *  * |
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x4000,
0x4000,
0x4400,
0x4800,
0x7000,
0x4800,
0x4400,
0x0000,
0x0000,

/* Character 108 (0x6c):
   width 6
   +------+
   |      |
   |  **  |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x3000,
0x1000,
0x1000,
0x1000,
0x1000,
0x1000,
0x3800,
0x0000,
0x0000,

/* Character 109 (0x6d):
   width 6
   +------+
   |      |
   |      |
   |      |
   | ** * |
   | * * *|
   | * * *|
   | * * *|
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x6800,
0x5400,
0x5400,
0x5400,
0x4400,
0x0000,
0x0000,

/* Character 110 (0x6e):
   width 6
   +------+
   |      |
   |      |
   |      |
   | * ** |
   | **  *|
   | *   *|
   | *   *|
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x5800,
0x6400,
0x4400,
0x4400,
0x4400,
0x0000,
0x0000,

/* Character 111 (0x6f):
   width 6
   +------+
   |      |
   |      |
   |      |
   |  *** |
   | *   *|
   | *   *|
   | *   *|
   |  *** |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x3800,
0x4400,
0x4400,
0x4400,
0x3800,
0x0000,
0x0000,

/* Character 112 (0x70):
   width 6
   +------+
   |      |
   |      |
   |      |
   | * ** |
   | **  *|
   | **  *|
   | * ** |
   | *    |
   | *    |
   | *    |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x5800,
0x6400,
0x6400,
0x5800,
0x4000,
0x4000,
0x4000,

/* Character 113 (0x71):
   width 6
   +------+
   |      |
   |      |
   |      |
   |  ** *|
   | *  **|
   | *  **|
   |  ** *|
   |     *|
   |     *|
   |     *|
   +------+
*/
0x0000,
0x0000,
0x0000,
0x3400,
0x4c00,
0x4c00,
0x3400,
0x0400,
0x0400,
0x0400,

/* Character 114 (0x72):
   width 6
   +------+
   |      |
   |      |
   |      |
   | * ** |
   | **  *|
   | *    |
   | *    |
   | *    |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x5800,
0x6400,
0x4000,
0x4000,
0x4000,
0x0000,
0x0000,

/* Character 115 (0x73):
   width 6
   +------+
   |      |
   |      |
   |      |
   |  *** |
   | *    |
   |  *** |
   |     *|
   | **** |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x3800,
0x4000,
0x3800,
0x0400,
0x7800,
0x0000,
0x0000,

/* Character 116 (0x74):
   width 6
   +------+
   |      |
   |  *   |
   |  *   |
   | **** |
   |  *   |
   |  *   |
   |  *  *|
   |   ** |
   |      |
   |      |
   +------+
*/
0x0000,
0x2000,
0x2000,
0x7800,
0x2000,
0x2000,
0x2400,
0x1800,
0x0000,
0x0000,

/* Character 117 (0x75):
   width 6
   +------+
   |      |
   |      |
   |      |
   | *   *|
   | *   *|
   | *   *|
   | *  **|
   |  ** *|
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x4400,
0x4400,
0x4400,
0x4c00,
0x3400,
0x0000,
0x0000,

/* Character 118 (0x76):
   width 6
   +------+
   |      |
   |      |
   |      |
   | *   *|
   | *   *|
   |  * * |
   |  * * |
   |   *  |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x4400,
0x4400,
0x2800,
0x2800,
0x1000,
0x0000,
0x0000,

/* Character 119 (0x77):
   width 6
   +------+
   |      |
   |      |
   |      |
   | *   *|
   | *   *|
   | * * *|
   | * * *|
   |  * * |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x4400,
0x4400,
0x5400,
0x5400,
0x2800,
0x0000,
0x0000,

/* Character 120 (0x78):
   width 6
   +------+
   |      |
   |      |
   |      |
   | *   *|
   |  * * |
   |   *  |
   |  * * |
   | *   *|
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x4400,
0x2800,
0x1000,
0x2800,
0x4400,
0x0000,
0x0000,

/* Character 121 (0x79):
   width 6
   +------+
   |      |
   |      |
   |      |
   | *   *|
   | *   *|
   | *  **|
   |  ** *|
   |     *|
   | *   *|
   |  *** |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x4400,
0x4400,
0x4c00,
0x3400,
0x0400,
0x4400,
0x3800,

/* Character 122 (0x7a):
   width 6
   +------+
   |      |
   |      |
   |      |
   | *****|
   |    * |
   |   *  |
   |  *   |
   | *****|
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x7c00,
0x0800,
0x1000,
0x2000,
0x7c00,
0x0000,
0x0000,

/* Character 123 (0x7b):
   width 6
   +------+
   |      |
   |    **|
   |   *  |
   |    * |
   |  **  |
   |    * |
   |   *  |
   |    **|
   |      |
   |      |
   +------+
*/
0x0000,
0x0c00,
0x1000,
0x0800,
0x3000,
0x0800,
0x1000,
0x0c00,
0x0000,
0x0000,

/* Character 124 (0x7c):
   width 6
   +------+
   |      |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |   *  |
   |      |
   |      |
   +------+
*/
0x0000,
0x1000,
0x1000,
0x1000,
0x1000,
0x1000,
0x1000,
0x1000,
0x0000,
0x0000,

/* Character 125 (0x7d):
   width 6
   +------+
   |      |
   | **   |
   |   *  |
   |  *   |
   |   ** |
   |  *   |
   |   *  |
   | **   |
   |      |
   |      |
   +------+
*/
0x0000,
0x6000,
0x1000,
0x2000,
0x1800,
0x2000,
0x1000,
0x6000,
0x0000,
0x0000,

/* Character 126 (0x7e):
   width 6
   +------+
   |      |
   |  *  *|
   | * * *|
   | *  * |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x2400,
0x5400,
0x4800,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,

/* Character 127 (0x7f):
   width 6
   +------+
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   |      |
   +------+
*/
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
};

static const FontDesc consoleDesc = {
  "win_crox0c",                               /* font name */
  6,                                          /* max width in pixels */
  10,                                         /* height in pixels */
  8,                                          /* ascent (baseline) height */
  32,                                         /* first character in bitmap */
  96,                                         /* font size in glyphs */
  _console_font_bits,                         /* 16-bit right-padded bitmap data */
  0,  /* no encode table*/                    /* offsets into bitmap data */
  0,  /* fixed width*/                        /* character widths or NULL if fixed */
  32,                                         /* default char (not glyph index) */
  sizeof(_console_font_bits)/sizeof(uInt16)   /* # words of bitmap_t bits */
};

} // namespace GUI

#endif
