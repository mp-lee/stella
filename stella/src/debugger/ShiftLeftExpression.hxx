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
// $Id: ShiftLeftExpression.hxx,v 1.1 2005-07-13 04:49:18 urchlay Exp $
//============================================================================

#ifndef SHIFTLEFT_EXPRESSION_HXX
#define SHIFTLEFT_EXPRESSION_HXX

#include "Expression.hxx"

/**
  @author  B. Watson
  @version $Id: ShiftLeftExpression.hxx,v 1.1 2005-07-13 04:49:18 urchlay Exp $
*/
class ShiftLeftExpression : public Expression
{
  public:
    ShiftLeftExpression(Expression *left, Expression *right);
    int evaluate() { return myLHS->evaluate() << myRHS->evaluate(); }
};

#endif

