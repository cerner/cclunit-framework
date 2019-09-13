/**
  Unit Tests with failed cclutAssert* calls for confirming that each assert function fails when expected.
*/

declare testFailureOfF8EqualAssert(null) = null
declare testFailureOfI4EqualAssert(null) = null
declare testFailureOfI2EqualAssert(null) = null
declare testFailureOfVCEqualAssert(null) = null
declare testFailureOfDateTimeEqualAssert(null) = null
declare testFailureOfF8AlmostEqualAssert(null) = null
declare testFailureOfF8NotAlmostEqualAssert(null) = null
declare testFailureOfF8NotEqualAssert(null) = null
declare testFailureOfI4NotEqualAssert(null) = null
declare testFailureOfI2NotEqualAssert(null) = null
declare testFailureOfVCNotEqualAssert(null) = null
declare testFailureOfDateTimeNotEqualAssert(null) = null
declare testFailureOfF8LessThanAssert(null) = null
declare testFailureOfI4LessThanAssert(null) = null
declare testFailureOfI2LessThanAssert(null) = null
declare testFailureOfVCLessThanAssert(null) = null
declare testFailureOfDateTimeLessThanAssert(null) = null
declare testFailureOfF8NotLessThanAssert(null) = null
declare testFailureOfI4NotLessThanAssert(null) = null
declare testFailureOfI2NotLessThanAssert(null) = null
declare testFailureOfVCNotLessThanAssert(null) = null
declare testFailureOfDateTimeNotLessThanAssert(null) = null
declare testFailureOfF8GreaterThanAssert(null) = null
declare testFailureOfI4GreaterThanAssert(null) = null
declare testFailureOfI2GreaterThanAssert(null) = null
declare testFailureOfVCGreaterThanAssert(null) = null
declare testFailureOfDateTimeGreaterThanAssert(null) = null
declare testFailureOfF8NotGreaterThanAssert(null) = null
declare testFailureOfI4NotGreaterThanAssert(null) = null
declare testFailureOfI2NotGreaterThanAssert(null) = null
declare testFailureOfVCNotGreaterThanAssert(null) = null
declare testFailureOfDateTimeNotGreaterThanAssert(null) = null
declare testFailureOfContainsAssert(null) = null
declare testFailureOfNotContainsAssert(null) = null
declare testFailureOfStartsWithAssert(null) = null
declare testFailureOfNotStartsWithAssert(null) = null
declare testFailureOfEndsWithAssert(null) = null
declare testFailureOfNotEndsWithAssert(null) = null
declare testFailureOfF8OperatorAssert(null) = null
declare testFailureOfF8NotOperatorAssert(null) = null
declare testFailureOfI4OperatorAssert(null) = null
declare testFailureOfI4NotOperatorAssert(null) = null
declare testFailureOfI2OperatorAssert(null) = null
declare testFailureOfI2NotOperatorAssert(null) = null
declare testFailureOfVcOperatorAssert(null) = null
declare testFailureOfVcNotOperatorAssert(null) = null
declare testFailureOfDateTimeOperatorAssert(null) = null
declare testFailureOfDateTimeNotOperatorAssert(null) = null
declare testFailureOfVCAssertsWithCclStringLogic(null) = null

;**********************************************************************************************************************************
;** Equal
;**********************************************************************************************************************************
subroutine testFailureOfF8EqualAssert(null)
  declare expected = f8 with protect, noconstant(0.0)
  declare actual = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = 6.7
  set actual = 2.4
  set assertReturn = cclutAssertF8Equal(CURREF, "Test Assert Failure 1", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8EqualAssert
 
subroutine testFailureOfI4EqualAssert(null)
  declare expected = i4 with protect, noconstant(0)
  declare actual = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = 9827
  set actual = 243
  set assertReturn = cclutAssertI4Equal(CURREF, "Test Assert Failure 2", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI4EqualAssert
 
subroutine testFailureOfI2EqualAssert(null)
  declare expected = i2 with protect, noconstant(0)
  declare actual = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = TRUE
  set actual = FALSE
  set assertReturn = cclutAssertI2Equal(CURREF, "Test Assert Failure 3", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI2EqualAssert
 
subroutine testFailureOfVCEqualAssert(null)
  declare expected = vc with protect, noconstant("")
  declare actual = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = "Fantastic"
  set actual = "Terrible"
  set assertReturn = cclutAssertVCEqual(CURREF, "Test Assert Failure 4", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set assertReturn = cclutAssertVCEqual(CURREF, "Test Assert Failure 4a", "", " ")
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set assertReturn = cclutAssertVCEqual(CURREF, "Test Assert Failure 4b", "", notrim(" "))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set assertReturn = cclutAssertVCEqual(CURREF, "Test Assert Failure 4c", "", trim(" "))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set expected = "hw"
  set actual = notrim("hw ")
  set assertReturn = cclutAssertVCEqual(CURREF, "Test Assert Failure 4d", expected, notrim(actual))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfVCEqualAssert
 
subroutine testFailureOfDateTimeEqualAssert(null)
  declare expected = f8 with protect, noconstant(0.0)
  declare actual = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = cnvtdatetime("30-NOV-2017 15:55:00")
  set actual = cnvtdatetime("01-JAN-2001 00:01:02")
  set assertReturn = cclutAssertDateTimeEqual(CURREF, "Test Assert Failure 5", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfDateTimeEqualAssert


;**********************************************************************************************************************************
;** AlmostEqual
;**********************************************************************************************************************************
subroutine testFailureOfF8AlmostEqualAssert(null)
  declare value1 = f8 with protect, noconstant(0.0) 
  declare value2 = f8 with protect, noconstant(0.0) 
  declare tolerance = f8 with protect, noconstant(0.0) 
  declare assertReturn = i2 with protect, noconstant(99) 
  
  set value1 = 2.6
  set value2 = 2.4
  set tolerance = 0.03
  set assertReturn = cclutAssertF8AlmostEqual(CURREF, "Test Assert Failure 6", value1, value2, tolerance)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8AlmostEqualAssert

subroutine testFailureOfF8NotAlmostEqualAssert(null)
  declare value1 = f8 with protect, noconstant(0.0) 
  declare value2 = f8 with protect, noconstant(0.0) 
  declare tolerance = f8 with protect, noconstant(0.0) 
  declare assertReturn = i2 with protect, noconstant(99) 
  
  set value1 = 2.88
  set value2 = 2.9
  set tolerance = 0.05
  set assertReturn = cclutAssertF8NotAlmostEqual(CURREF, "Test Assert Failure 7", value1, value2, tolerance)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8NotAlmostEqualAssert


;**********************************************************************************************************************************
;** NotEqual
;**********************************************************************************************************************************
subroutine testFailureOfF8NotEqualAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 6.7
  set value2 = 6.7
  set assertReturn = cclutAssertF8NotEqual(CURREF, "Test Assert Failure 8", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8NotEqualAssert

subroutine testFailureOfI4NotEqualAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 9827
  set value2 = 9827
  set assertReturn = cclutAssertI4NotEqual(CURREF, "Test Assert Failure 9", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI4NotEqualAssert

subroutine testFailureOfI2NotEqualAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 12
  set value2 = 12
  set assertReturn = cclutAssertI2NotEqual(CURREF, "Test Assert Failure 10", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI2NotEqualAssert

subroutine testFailureOfVCNotEqualAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  declare cclut::useCclStringLogic = i2 with protect, noconstant(TRUE)
  
  set value1 = notrim("Fantastic   ")
  set value2 = "Fantastic"
  set assertReturn = cclutAssertVCNotEqual(CURREF, "Test Assert Failure 11", notrim(value1), value2)
  call cclutAssertI2Equal(CURREF, "Space padded strings are equal to their non-padded strings", FALSE, assertReturn)
end ;testFailureOfVCNotEqualAssert

subroutine testFailureOfDateTimeNotEqualAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime(curdate, 0)
  set value2 = cnvtdatetime(curdate, 0)
  set assertReturn = cclutAssertDateTimeNotEqual(CURREF, "Test Assert Failure 12", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfDateTimeNotEqualAssert
 


;**********************************************************************************************************************************
;** LessThan
;**********************************************************************************************************************************
subroutine testFailureOfF8LessThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 2.4
  set value2 = 2.4
  set assertReturn = cclutAssertF8LessThan(CURREF, "Test Assert Failure 13", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8LessThanAssert

subroutine testFailureOfI4LessThanAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 827
  set value2 = 243
  set assertReturn = cclutAssertI4LessThan(CURREF, "Test Assert Failure 14", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI4LessThanAssert

subroutine testFailureOfI2LessThanAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 57
  set value2 = 14
  set assertReturn = cclutAssertI2LessThan(CURREF, "Test Assert Failure 15", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI2LessThanAssert

subroutine testFailureOfVCLessThanAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)

  set value1 = "Wow"
  set value2 = "Terrible"
  set assertReturn = cclutAssertVCLessThan(CURREF, "Test Assert Failure 16", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set value1 = "hw "
  set value2 = "hw"
  set assertReturn = cclutAssertVCLessThan(CURREF, "Test Assert Failure 16a", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfVCLessThanAssert

subroutine testFailureOfDateTimeLessThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime("01-JAN-2001 00:01:02")
  set value2 = cnvtdatetime("01-JAN-2001 00:01:02")
  set assertReturn = cclutAssertDateTimeLessThan(CURREF, "Test Assert Failure 17", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfDateTimeLessThanAssert


;**********************************************************************************************************************************
;** NotLessThan
;**********************************************************************************************************************************
subroutine testFailureOfF8NotLessThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 1.7
  set value2 = 2.4
  set assertReturn = cclutAssertF8NotLessThan(CURREF, "Test Assert Failure 18", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8NotLessThanAssert

subroutine testFailureOfI4NotLessThanAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 127
  set value2 = 243
  set assertReturn = cclutAssertI4NotLessThan(CURREF, "Test Assert Failure 19", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI4NotLessThanAssert

subroutine testFailureOfI2NotLessThanAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 7
  set value2 = 14
  set assertReturn = cclutAssertI2NotLessThan(CURREF, "Test Assert Failure 20", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI2NotLessThanAssert

subroutine testFailureOfVCNotLessThanAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = "Fantastic"
  set value2 = "Terrible"
  set assertReturn = cclutAssertVCNotLessThan(CURREF, "Test Assert Failure 21", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set value1 = "hw"
  set value2 = notrim("hw ")
  set assertReturn = cclutAssertVCNotLessThan(CURREF, "Test Assert Failure 21a", value1, notrim(value2))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfVCNotLessThanAssert

subroutine testFailureOfDateTimeNotLessThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime(curdate, curtime)
  set value2 = cnvtdatetime(curdate+1, curtime)
  set assertReturn = cclutAssertDateTimeNotLessThan(CURREF, "Test Assert Failure 22", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfDateTimeNotLessThanAssert


;**********************************************************************************************************************************
;** GreaterThan
;**********************************************************************************************************************************
subroutine testFailureOfF8GreaterThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 1.7
  set value2 = 2.4
  set assertReturn = cclutAssertF8GreaterThan(CURREF, "Test Assert Failure 23", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8GreaterThanAssert

subroutine testFailureOfI4GreaterThanAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 827
  set value2 = 827
  set assertReturn = cclutAssertI4GreaterThan(CURREF, "Test Assert Failure 24", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI4GreaterThanAssert

subroutine testFailureOfI2GreaterThanAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)

  set value1 = 57
  set value2 = 114
  set assertReturn = cclutAssertI2GreaterThan(CURREF, "Test Assert Failure 25", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI2GreaterThanAssert

subroutine testFailureOfVCGreaterThanAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = "Dove"
  set value2 = "Terrible"
  set assertReturn = cclutAssertVCGreaterThan(CURREF, "Test Assert Failure 26", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set value1 = "hw"
  set value2 = "hw "
  set assertReturn = cclutAssertVCGreaterThan(CURREF, "Test Assert Failure 26a", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfVCGreaterThanAssert

subroutine testFailureOfDateTimeGreaterThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime("01-JAN-2001 00:01:02")
  set value2 = cnvtdatetime("01-JAN-2001 00:01:02")
  set assertReturn = cclutAssertDateTimeGreaterThan(CURREF, "Test Assert Failure 27", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfDateTimeGreaterThanAssert


;**********************************************************************************************************************************
;** NotGreaterThan
;**********************************************************************************************************************************
subroutine testFailureOfF8NotGreaterThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 981.7
  set value2 = 2.4
  set assertReturn = cclutAssertF8NotGreaterThan(CURREF, "Test Assert Failure 28", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8NotGreaterThanAssert

subroutine testFailureOfI4NotGreaterThanAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 593
  set value2 = 243
  set assertReturn = cclutAssertI4NotGreaterThan(CURREF, "Test Assert Failure 29", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI4NotGreaterThanAssert

subroutine testFailureOfI2NotGreaterThanAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 87
  set value2 = 14
  set assertReturn = cclutAssertI2NotGreaterThan(CURREF, "Test Assert Failure 30", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI2NotGreaterThanAssert

subroutine testFailureOfVCNotGreaterThanAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = "Wave"
  set value2 = "Terrible"
  set assertReturn = cclutAssertVCNotGreaterThan(CURREF, "Test Assert Failure 31", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set value1 = notrim("hw ")
  set value2 = "hw"
  set assertReturn = cclutAssertVCNotGreaterThan(CURREF, "Test Assert Failure 31a", notrim(value1), value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfVCNotGreaterThanAssert

subroutine testFailureOfDateTimeNotGreaterThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime(curdate+3, curtime)
  set value2 = cnvtdatetime(curdate-1, curtime)
  set assertReturn = cclutAssertDateTimeNotGreaterThan(CURREF, "Test Assert Failure 32", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfDateTimeNotGreaterThanAssert


;**********************************************************************************************************************************
;** Contains
;**********************************************************************************************************************************
subroutine testFailureOfContainsAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = "Good"
  set bigStr = "This is good for me!"
  set assertReturn = cclutAssertContains(CURREF, "Test Assert Failure 33", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfContainsAssert

subroutine testFailureOfNotContainsAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = "better"
  set bigStr = "Good, better, worse"
  set assertReturn = cclutAssertNotContains(CURREF, "Test Assert Failure 34", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfContainsAssert

;**********************************************************************************************************************************
;** StartsWith
;**********************************************************************************************************************************
subroutine testFailureOfStartsWithAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = "his"
  set bigStr = "This is good for me!"
  set assertReturn = cclutAssertStartsWith(CURREF, "Test Assert Failure 35", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfStartsWithAssert

subroutine testFailureOfNotStartsWithAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = " This"
  set bigStr = " This is bad for me!"
  set assertReturn = cclutAssertNotStartsWith(CURREF, "Test Assert Failure 36", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfNotStartsWithAssert


;**********************************************************************************************************************************
;** EndsWith
;**********************************************************************************************************************************
subroutine testFailureOfEndsWithAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = " e!"
  set bigStr = " This is good for me!"
  set assertReturn = cclutAssertEndsWith(CURREF, "Test Assert Failure 37", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfEndsWithAssert

subroutine testFailureOfNotEndsWithAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = " pepper"
  set bigStr = "Salt and pepper"
  set assertReturn = cclutAssertNotEndsWith(CURREF, "Test Assert Failure 38", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfNotEndsWithAssert


;**********************************************************************************************************************************
;** Operartor
;**********************************************************************************************************************************
subroutine testFailureOfF8OperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set assertReturn = cclutAssertF8Operator(CURREF, "Test Assert Failure 39", 3.2, ">", 3.21)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8OperatorAssert

subroutine testFailureOfF8NotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set assertReturn = cclutAssertF8NotOperator(CURREF, "Test Assert Failure 40", 3.21, ">", 3.2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfF8NotOperatorAssert

subroutine testFailureOfI4OperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set assertReturn = cclutAssertI4Operator(CURREF, "Test Assert Failure 41", 4, "<=", 3)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI4OperatorAssert

subroutine testFailureOfI4NotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set assertReturn = cclutAssertI4NotOperator(CURREF, "Test Assert Failure 42", 6, "!=", 5)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI4NotOperatorAssert

subroutine testFailureOfI2OperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set assertReturn = cclutAssertI2Operator(CURREF, "Test Assert Failure 43", TRUE, "<", FALSE)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI2OperatorAssert

subroutine testFailureOfI2NotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set assertReturn = cclutAssertI2NotOperator(CURREF, "Test Assert Failure 44", TRUE, "not !=", TRUE)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfI2NotOperatorAssert

subroutine testFailureOfVcOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  declare str = vc with protect, noconstant("")
  declare op = vc with protect, noconstant("")
  declare opval = vc with protect, noconstant("")
  
  set str = "41276 check    this out. "
  set op = "regexplike"
  set opval = "41[0-9]{4} check *this out\. "
  
  set assertReturn = cclutAssertVCOperator(CURREF, "Test Assert Failure 45", str, op, opval)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfVcOperatorAssert

subroutine testFailureOfVcNotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  declare str = vc with protect, noconstant("")
  declare op = vc with protect, noconstant("")
  declare opval = vc with protect, noconstant("")
  
  set str = "41276 check    this out. "
  set op = "regexplike"
  set opval = "41[0-9]{4} check *this out\. "
  
  set assertReturn = cclutAssertVCNotOperator(CURREF, "Test Assert Failure 46", str, op, opval)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfVcNotOperatorAssert

subroutine testFailureOfDateTimeOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  declare datetime1 = dq8 with protect, noconstant(cnvtdatetime(curdate, 120101))
  declare datetime2 = dq8 with protect, noconstant(cnvtdatetime(curdate, 120102))
  
  set assertReturn = cclutAssertDateTimeOperator(CURREF, "Test Assert Failure 47", datetime1, "<", datetime2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfDateTimeOperatorAssert

subroutine testFailureOfDateTimeNotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  declare datetime1 = dq8 with protect, noconstant(cnvtdatetime(curdate, 120101))
  declare datetime2 = dq8 with protect, noconstant(cnvtdatetime(curdate, 120102))
 
  set assertReturn = cclutAssertDateTimeNotOperator(CURREF, "Test Assert Failure 48", datetime2, "<", datetime1)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfDateTimeNotOperatorAssert

subroutine testFailureOfVCAssertsWithCclStringLogic(null)
  declare cclut::useCclStringLogic = i2 with protect, noconstant(TRUE)
  declare assertReturn = i2 with protect, noconstant(99)

  set assertReturn = cclutAssertVCEqual(CURREF, "Test Assert Failure 49a", concat("hw", char(32)), concat("hw", char(0)))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set assertReturn = cclutAssertVCLessThan(CURREF, "Test Assert Failure 49b", "hw", concat("hw", char(0)))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set assertReturn = cclutAssertVCLessThan(CURREF, "Test Assert Failure 49c", "hw", concat("hw", char(32)))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set assertReturn = cclutAssertVCGreaterThan(CURREF, "Test Assert Failure 49d", concat("hw", char(0)), "hw")
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set assertReturn = cclutAssertVCGreaterThan(CURREF, "Test Assert Failure 49e", concat("hw", char(32)), "hw")
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set assertReturn = cclutAssertVCNotEqual(CURREF, "Test Assert Failure 50a", "hw", concat("hw", char(0)))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)

  set assertReturn = cclutAssertVCNotEqual(CURREF, "Test Assert Failure 50b", "hw", concat("hw", char(32)))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value", FALSE, assertReturn)
end ;testFailureOfVCAssertsWithCclStringLogic
