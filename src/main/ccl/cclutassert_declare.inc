%#ifndef CCLUTASSERT_DECLARE_INC
%#def CCLUTASSERT_DECLARE_INC
;**********************************************************************************************************************************
;
; Declares the cclutAssert* subroutines
;
;**********************************************************************************************************************************

declare cclut::useCclStringLogic = i2 with protect, noconstant(FALSE)

;**********************************************************************************************************************************
;** Equal
;**********************************************************************************************************************************
declare cclutAssertF8Equal(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
declare cclutAssertI4Equal(cclutLineNum = i4, cclutCtx = vc, cclutVal = i4, cclutCompVal = i4) = i2 with protect
declare cclutAssertI2Equal(cclutLineNum = i4, cclutCtx = vc, cclutVal = i2, cclutCompVal = i2) = i2 with protect
declare cclutAssertVCEqual(cclutLineNum = i4, cclutCtx = vc, cclutVal = vc, cclutCompVal = vc) = i2 with protect
declare cclutAssertDateTimeEqual(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
 
;**********************************************************************************************************************************
;** AlmostEqual
;**********************************************************************************************************************************
declare cclutAssertF8AlmostEqual(
    cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8, cclutTolerance = f8) = i2 with protect
declare cclutAssertF8NotAlmostEqual(
    cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8, cclutTolerance = f8) = i2 with protect
 
;**********************************************************************************************************************************
;** NotEqual
;**********************************************************************************************************************************
declare cclutAssertF8NotEqual(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
declare cclutAssertI4NotEqual(cclutLineNum = i4, cclutCtx = vc, cclutVal = i4, cclutCompVal = i4) = i2 with protect
declare cclutAssertI2NotEqual(cclutLineNum = i4, cclutCtx = vc, cclutVal = i2, cclutCompVal = i2) = i2 with protect
declare cclutAssertVCNotEqual(cclutLineNum = i4, cclutCtx = vc, cclutVal = vc, cclutCompVal = vc) = i2 with protect
declare cclutAssertDateTimeNotEqual(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
 
;**********************************************************************************************************************************
;** LessThan
;**********************************************************************************************************************************
declare cclutAssertF8LessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
declare cclutAssertI4LessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = i4, cclutCompVal = i4) = i2 with protect
declare cclutAssertI2LessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = i2, cclutCompVal = i2) = i2 with protect
declare cclutAssertVCLessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = vc, cclutCompVal = vc) = i2 with protect
declare cclutAssertDateTimeLessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
 
;**********************************************************************************************************************************
;** NotLessThan
;**********************************************************************************************************************************
declare cclutAssertF8NotLessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
declare cclutAssertI4NotLessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = i4, cclutCompVal = i4) = i2 with protect
declare cclutAssertI2NotLessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = i2, cclutCompVal = i2) = i2 with protect
declare cclutAssertVCNotLessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = vc, cclutCompVal = vc) = i2 with protect
declare cclutAssertDateTimeNotLessThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
 
;**********************************************************************************************************************************
;** GreaterThan
;**********************************************************************************************************************************
declare cclutAssertF8GreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
declare cclutAssertI4GreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = i4, cclutCompVal = i4) = i2 with protect
declare cclutAssertI2GreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = i2, cclutCompVal = i2) = i2 with protect
declare cclutAssertVCGreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = vc, cclutCompVal = vc) = i2 with protect
declare cclutAssertDateTimeGreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
 
;**********************************************************************************************************************************
;** NotGreaterThan
;**********************************************************************************************************************************
declare cclutAssertF8NotGreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
declare cclutAssertI4NotGreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = i4, cclutCompVal = i4) = i2 with protect
declare cclutAssertI2NotGreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = i2, cclutCompVal = i2) = i2 with protect
declare cclutAssertVCNotGreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = vc, cclutCompVal = vc) = i2 with protect
declare cclutAssertDateTimeNotGreaterThan(cclutLineNum = i4, cclutCtx = vc, cclutVal = f8, cclutCompVal = f8) = i2 with protect
 
;**********************************************************************************************************************************
;** Contains
;**********************************************************************************************************************************
declare cclutAssertContains(cclutLineNum = i4, cclutCtx = vc, cclutSubstring = vc, cclutString = vc) = i2 with protect
declare cclutAssertNotContains(cclutLineNum = i4, cclutCtx = vc, cclutSubstring = vc, cclutString = vc) = i2 with protect
 
;**********************************************************************************************************************************
;** StartsWith
;**********************************************************************************************************************************
declare cclutAssertStartsWith(cclutLineNum = i4, cclutCtx = vc, cclutSubstring = vc, cclutString = vc) = i2 with protect
declare cclutAssertNotStartsWith(cclutLineNum = i4, cclutCtx = vc, cclutSubstring = vc, cclutString = vc) = i2 with protect
 
;**********************************************************************************************************************************
;** EndsWith
;**********************************************************************************************************************************
declare cclutAssertEndsWith(cclutLineNum = i4, cclutCtx = vc, cclutSubstring = vc, cclutString = vc) = i2 with protect
declare cclutAssertNotEndsWith(cclutLineNum = i4, cclutCtx = vc, cclutSubstring = vc, cclutString = vc) = i2 with protect
 
;**********************************************************************************************************************************
;** Operartor
;**********************************************************************************************************************************
declare cclutAssertF8Operator(cclutLineNum = i4, cclutCtx = vc, cclutExp1 = f8, cclutOp = vc, cclutExp2 = f8) = i2 with protect
declare cclutAssertF8NotOperator(cclutLineNum = i4, cclutCtx = vc, cclutExp1 = f8, cclutOp = vc, cclutExp2 = f8) = i2 with protect
declare cclutAssertI4Operator(cclutLineNum = i4, cclutCtx = vc, cclutExp1 = i4, cclutOp = vc, cclutExp2 = i4) = i2 with protect
declare cclutAssertI4NotOperator(cclutLineNum = i4, cclutCtx = vc, cclutExp1 = i4, cclutOp = vc, cclutExp2 = i4) = i2 with protect
declare cclutAssertI2Operator(cclutLineNum = i4, cclutCtx = vc, cclutExp1 = i2, cclutOp = vc, cclutExp2 = i2) = i2 with protect
declare cclutAssertI2NotOperator(cclutLineNum = i4, cclutCtx = vc, cclutExp1 = i2, cclutOp = vc, cclutExp2 = i2) = i2 with protect
declare cclutAssertVCOperator(cclutLineNum = i4, cclutCtx = vc, cclutExp1 = vc, cclutOp = vc, cclutExp2 = vc) = i2 with protect
declare cclutAssertVCNotOperator(cclutLineNum = i4, cclutCtx = vc, cclutExp1 = vc, cclutOp = vc, cclutExp2 = vc) = i2 with protect
declare cclutAssertDateTimeOperator(
    cclutLineNum = i4, cclutCtx = vc, cclutExp1 = f8, cclutOp = vc, cclutExp2 = f8) = i2 with protect
declare cclutAssertDateTimeNotOperator(
    cclutLineNum = i4, cclutCtx = vc, cclutExp1 = f8, cclutOp = vc, cclutExp2 = f8) = i2 with protect

%#endif CCLUTASSERT_DECLARE_INC
