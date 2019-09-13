/**
  Unit Tests to confirm the cclutAssert* functions are working as expected. Since an assert failure will fail the build, they 
  cannot be tested directly. They are checked by executing a test case with failed asserts via cclut and then examining
  the results.
  .
*/

%i cclsource:cclut_get_file_as_string.inc

declare NULL_MARKER = vc with protect, constant(char(128))

declare testing::checkLines(lines = vc(ref)) = null with protect
declare testSuccessOfF8EqualAssert(null) = null with protect
declare testSuccessOfI4EqualAssert(null) = null with protect
declare testSuccessOfI2EqualAssert(null) = null with protect
declare testSuccessOfVCEqualAssert(null) = null with protect
declare testSuccessOfDateTimeEqualAssert(null) = null with protect
declare testSuccessOfF8AlmostEqualAssert(null) = null with protect
declare testSuccessOfF8NotAlmostEqualAssert(null) = null with protect
declare testSuccessOfF8NotEqualAssert(null) = null with protect
declare testSuccessOfI4NotEqualAssert(null) = null with protect
declare testSuccessOfI2NotEqualAssert(null) = null with protect
declare testSuccessOfVCNotEqualAssert(null) = null with protect
declare testSuccessOfDateTimeNotEqualAssert(null) = null with protect
declare testSuccessOfF8LessThanAssert(null) = null with protect
declare testSuccessOfI4LessThanAssert(null) = null with protect
declare testSuccessOfI2LessThanAssert(null) = null with protect
declare testSuccessOfVCLessThanAssert(null) = null with protect
declare testSuccessOfDateTimeLessThanAssert(null) = null with protect
declare testSuccessOfF8NotLessThanAssert(null) = null with protect
declare testSuccessOfI4NotLessThanAssert(null) = null with protect
declare testSuccessOfI2NotLessThanAssert(null) = null with protect
declare testSuccessOfVCNotLessThanAssert(null) = null with protect
declare testSuccessOfDateTimeNotLessThanAssert(null) = null with protect
declare testSuccessOfF8GreaterThanAssert(null) = null with protect
declare testSuccessOfI4GreaterThanAssert(null) = null with protect
declare testSuccessOfI2GreaterThanAssert(null) = null with protect
declare testSuccessOfVCGreaterThanAssert(null) = null with protect
declare testSuccessOfDateTimeGreaterThanAssert(null) = null with protect
declare testSuccessOfF8NotGreaterThanAssert(null) = null with protect
declare testSuccessOfI4NotGreaterThanAssert(null) = null with protect
declare testSuccessOfI2NotGreaterThanAssert(null) = null with protect
declare testSuccessOfVCNotGreaterThanAssert(null) = null with protect
declare testSuccessOfDateTimeNotGreaterThanAssert(null) = null with protect
declare testSuccessOfContainsAssert(null) = null with protect
declare testSuccessOfNotContainsAssert(null) = null with protect
declare testSuccessOfStartsWithAssert(null) = null with protect
declare testSuccessOfNotStartsWithAssert(null) = null with protect
declare testSuccessOfEndsWithAssert(null) = null with protect
declare testSuccessOfNotEndsWithAssert(null) = null with protect
declare testSuccessOfF8OperatorAssert(null) = null with protect
declare testSuccessOfF8NotOperatorAssert(null) = null with protect
declare testSuccessOfI4OperatorAssert(null) = null with protect
declare testSuccessOfI4NotOperatorAssert(null) = null with protect
declare testSuccessOfI2OperatorAssert(null) = null with protect
declare testSuccessOfI2NotOperatorAssert(null) = null with protect
declare testSuccessOfVcOperatorAssert(null) = null with protect
declare testSuccessOfVcNotOperatorAssert(null) = null with protect
declare testSuccessOfDateTimeOperatorAssert(null) = null with protect
declare testSuccessOfDateTimeNotOperatorAssert(null) = null with protect
declare testAssertFailures(null) = null with protect
declare testParameterNameConflicts(null) = null with protect
declare testStringLogicOverride(null) = null with protect


declare testing::ASSERT_TYPE_EQUAL = i4 with protect, constant(0)
declare testing::ASSERT_TYPE_DATE = i4 with protect, constant(1)
declare testing::ASSERT_TYPE_MATCH = i4 with protect, constant(2)

declare testing::NL = vc with protect, constant(char(10))

subroutine testing::checkLines(lines)
  declare output = vc with protect, noconstant("")
  declare startPos = i4 with protect, noconstant(0)
  declare nullPos = i4 with protect, noconstant(0)
  declare lineNumberN = i4 with protect, noconstant(0)
  declare lineNumberStr = vc with protect, noconstant("")
  declare outputLine = vc with protect, noconstant("")
  declare expectedLine = vc with protect, noconstant("")
  declare expectedLineType = i4 with protect, noconstant(0)
  declare matches = i2 with protect, noconstant(FALSE)

  set output = trim(cclut::getFileAsString("ut_cclut_output.dat"))
  set startPos = 1
  while (startPos > 0)
    set nullPos = findstring(testing::NL, output, startPos)
    if (nullPos > 0)
      set outputLine = substring(startPos, nullPos-startPos, output)
      set startPos = nullPos + 1
    else
      set outputLine = substring(startPos, 1+textlen(output)-startPos, output)
      set startPos = 0
    endif
    set lineNumberN = lineNumberN + 1
    set lineNumberStr = trim(cnvtstring(lineNumberN), 3)
    set expectedLine = lines->line[lineNumberN].text
    set expectedLineType = lines->line[lineNumberN].type
    case (expectedLineType)
      of testing::ASSERT_TYPE_EQUAL:
        call cclutAssertVCEqual(CURREF, build2("validate output line ", lineNumberStr), outputLine, expectedLine)
      of testing::ASSERT_TYPE_DATE:
        call cclutAssertStartsWith(CURREF, build2("validate output line ", lineNumberStr), expectedLine, outputLine)
        set matches = operator(outputLine, "regexplike", 
            "[0-9]{2}-(JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC)-[0-9]{4} [0-9]{2}:[0-9]{2}:[0-9]{2}\.[0-9]{2}")
        call cclutAssertI4Equal(CURREF, build2("validate output line ", lineNumberStr), matches, TRUE)
      of testing::ASSERT_TYPE_MATCH:
        set matches = operator(outputLine, "regexplike", expectedLine)
        call cclutAssertI4Equal(CURREF, build2("validate output line ", lineNumberStr), matches, TRUE)
    endcase
  endwhile
end ;;;



;**********************************************************************************************************************************
;** Equal
;**********************************************************************************************************************************
subroutine testSuccessOfF8EqualAssert(null)
  declare expected = f8 with protect, noconstant(0.0)
  declare actual = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = 5.3
  set actual = 5.3
  set assertReturn = cclutAssertF8Equal(CURREF, "Test Assert 1", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 1", TRUE, assertReturn)
end ;testSuccessOfF8EqualAssert
 
subroutine testSuccessOfI4EqualAssert(null)
  declare expected = i4 with protect, noconstant(0)
  declare actual = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = 3645
  set actual = 3645
  set assertReturn = cclutAssertI4Equal(CURREF, "Test Assert 2 ", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 2", TRUE, assertReturn)
end ;testSuccessOfI4EqualAssert
 
subroutine testSuccessOfI2EqualAssert(null)
  declare expected = i2 with protect, noconstant(0)
  declare actual = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = TRUE
  set actual = TRUE
  set assertReturn = cclutAssertI2Equal(CURREF, "Test Assert 3", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 3", TRUE, assertReturn)
end ;testSuccessOfI2EqualAssert
 
subroutine testSuccessOfVCEqualAssert(null)
  declare expected = vc with protect, noconstant("")
  declare actual = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = "Super"
  set actual = "Super"
  set assertReturn = cclutAssertVCEqual(CURREF, "Test Assert 4", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 4", TRUE, assertReturn)
end ;testSuccessOfVCEqualAssert
 
subroutine testSuccessOfDateTimeEqualAssert(null)
  declare expected = f8 with protect, noconstant(0.0)
  declare actual = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set expected = cnvtdatetime(curdate, 0)
  set actual = cnvtdatetime(curdate, 0)
  set assertReturn = cclutAssertDateTimeEqual(CURREF, "Test Assert 5", expected, actual)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 5", TRUE, assertReturn)

end ;testSuccessOfDateTimeEqualAssert


;**********************************************************************************************************************************
;** AlmostEqual
;**********************************************************************************************************************************
subroutine testSuccessOfF8AlmostEqualAssert(null)
  declare value1 = f8 with protect, noconstant(0.0) 
  declare value2 = f8 with protect, noconstant(0.0) 
  declare tolerance = f8 with protect, noconstant(0.0) 
  declare assertReturn = i2 with protect, noconstant(99) 
  
  set value1 = 5.3
  set value2 = 5.32
  set tolerance = 0.03
  set assertReturn = cclutAssertF8AlmostEqual(CURREF, "Test Assert6", value1, value2, tolerance)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 6", TRUE, assertReturn)
  
  set value1 = 5.31
  set value2 = 5.3 
  set tolerance = 0.03
  set assertReturn = cclutAssertF8AlmostEqual(CURREF, "Test Assert 7", value1, value2,tolerance)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 7", TRUE, assertReturn)
end ;testSuccessOfF8AlmostEqualAssert

subroutine testSuccessOfF8NotAlmostEqualAssert(null)
  declare value1 = f8 with protect, noconstant(0.0) 
  declare value2 = f8 with protect, noconstant(0.0) 
  declare tolerance = f8 with protect, noconstant(0.0) 
  declare assertReturn = i2 with protect, noconstant(99) 
  
  set value1 = 7.8
  set value2 = 7.82
  set tolerance = 0.01
  set assertReturn = cclutAssertF8NotAlmostEqual(CURREF, "Test Assert 8", value1, value2, tolerance)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 8", TRUE, assertReturn)
end ;testSuccessOfF8NotAlmostEqualAssert


;**********************************************************************************************************************************
;** NotEqual
;**********************************************************************************************************************************
subroutine testSuccessOfF8NotEqualAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 5.3
  set value2 = 25.3
  set assertReturn = cclutAssertF8NotEqual(CURREF, "Test Assert 9", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 9", TRUE, assertReturn)
end ;testSuccessOfF8NotEqualAssert

subroutine testSuccessOfI4NotEqualAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
    
  set value1 = 3645
  set value2 = 45
  set assertReturn = cclutAssertI4NotEqual(CURREF, "Test Assert 10", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 10", TRUE, assertReturn)
end ;testSuccessOfI4NotEqualAssert

subroutine testSuccessOfI2NotEqualAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 10
  set value2 = 12
  set assertReturn = cclutAssertI2NotEqual(CURREF, "Test Assert 11", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 11", TRUE, assertReturn)
end ;testSuccessOfI2NotEqualAssert

subroutine testSuccessOfVCNotEqualAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = "Super"
  set value2 = "Wonderful"
  set assertReturn = cclutAssertVCNotEqual(CURREF, "Test Assert 12", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 12", TRUE, assertReturn)
end ;testSuccessOfVCNotEqualAssert

subroutine testSuccessOfDateTimeNotEqualAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime(curdate, 0)
  set value2 = cnvtdatetime("23-JUN-1980 12:34:56")
  set assertReturn = cclutAssertDateTimeNotEqual(CURREF, "Test Assert 12", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 12", TRUE, assertReturn)
end ;testSuccessOfDateTimeNotEqualAssert


;**********************************************************************************************************************************
;** LessThan
;**********************************************************************************************************************************
subroutine testSuccessOfF8LessThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 4.3
  set value2 = 5.3
  set assertReturn = cclutAssertF8LessThan(CURREF, "Test Assert 13", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 13", TRUE, assertReturn)
end ;testSuccessOfF8LessThanAssert

subroutine testSuccessOfI4LessThanAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
    
  set value1 = 1645
  set value2 = 3645
  set assertReturn = cclutAssertI4LessThan(CURREF, "Test Assert 14", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 14", TRUE, assertReturn)
end ;testSuccessOfI4LessThanAssert

subroutine testSuccessOfI2LessThanAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 25
  set value2 = 55
  set assertReturn = cclutAssertI2LessThan(CURREF, "Test Assert 15", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 15", TRUE, assertReturn)
end ;testSuccessOfI2LessThanAssert

subroutine testSuccessOfVCLessThanAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = "Good"
  set value2 = "Super"
  set assertReturn = cclutAssertVCLessThan(CURREF, "Test Assert 16", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 16", TRUE, assertReturn)

  set value1 = "hw"
  set value2 = notrim("hw ")
  set assertReturn = cclutAssertVCLessThan(CURREF, "Test Assert 16a", value1, notrim(value2))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 16a", TRUE, assertReturn)

  set assertReturn = cclutAssertVCLessThan(CURREF, "Test Assert 16b", trim(""), "")
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 16b", TRUE, assertReturn)

  set assertReturn = cclutAssertVCLessThan(CURREF, "Test Assert 16c", "", " ")
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 16c", TRUE, assertReturn)
end ;testSuccessOfVCLessThanAssert

subroutine testSuccessOfDateTimeLessThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime(curdate-1, 0)
  set value2 = cnvtdatetime(curdate, 0)
  set assertReturn = cclutAssertDateTimeLessThan(CURREF, "Test Assert 17", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 17", TRUE, assertReturn)
end ;testSuccessOfDateTimeLessThanAssert


;**********************************************************************************************************************************
;** NotLessThan
;**********************************************************************************************************************************
subroutine testSuccessOfF8NotLessThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 5.302
  set value2 = 5.3
  set assertReturn = cclutAssertF8NotLessThan(CURREF, "Test Assert 18", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 18", TRUE, assertReturn)
  
  set value1 = 2.4
  set value2 = 2.4
  set assertReturn = cclutAssertF8NotLessThan(CURREF, "Test Assert 19", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 19", TRUE, assertReturn)
end ;testSuccessOfF8NotLessThanAssert

subroutine testSuccessOfI4NotLessThanAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
    
  set value1 = 10645
  set value2 = 3645
  set assertReturn = cclutAssertI4NotLessThan(CURREF, "Test Assert 20", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 20", TRUE, assertReturn)
  
  set value1 = 827
  set value2 = 827
  set assertReturn = cclutAssertI4NotLessThan(CURREF, "Test Assert 21", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 21", TRUE, assertReturn)
end ;testSuccessOfI4NotLessThanAssert

subroutine testSuccessOfI2NotLessThanAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 825
  set value2 = 55
  set assertReturn = cclutAssertI2NotLessThan(CURREF, "Test Assert 22", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 22", TRUE, assertReturn)
  
  set value1 = 14
  set value2 = 14
  set assertReturn = cclutAssertI2NotLessThan(CURREF, "Test Assert 23", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 23", TRUE, assertReturn)
end ;testSuccessOfI2NotLessThanAssert

subroutine testSuccessOfVCNotLessThanAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = "Terrific"
  set value2 = "Super"
  set assertReturn = cclutAssertVCNotLessThan(CURREF, "Test Assert 24", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 24", TRUE, assertReturn)
  
  set value1 = notrim("Good ")
  set value2 = "Good"
  set assertReturn = cclutAssertVCNotLessThan(CURREF, "Test Assert 25", notrim(value1), value2)
  call cclutAssertI2Equal(CURREF, "A space padded strings is not less than the non-padded string", TRUE, assertReturn)
end ;testSuccessOfVCNotLessThanAssert

subroutine testSuccessOfDateTimeNotLessThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime(curdate+2, 0)
  set value2 = cnvtdatetime(curdate, 0)
  set assertReturn = cclutAssertDateTimeNotLessThan(CURREF, "Test Assert 26", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 26", TRUE, assertReturn)
  
  set value1 = cnvtdatetime("01-JAN-2001 00:01:02")
  set value2 = cnvtdatetime("01-JAN-2001 00:01:02")
  set assertReturn = cclutAssertDateTimeNotLessThan(CURREF, "Test Assert 27", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 27", TRUE, assertReturn)
end ;testSuccessOfDateTimeNotLessThanAssert


;**********************************************************************************************************************************
;** GreaterThan
;**********************************************************************************************************************************
subroutine testSuccessOfF8GreaterThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 14.3
  set value2 = 5.3
  set assertReturn = cclutAssertF8GreaterThan(CURREF, "Test Assert 28", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 28", TRUE, assertReturn)
end ;testSuccessOfF8GreaterThanAssert

subroutine testSuccessOfI4GreaterThanAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
    
  set value1 = 9000
  set value2 = 3645
  set assertReturn = cclutAssertI4GreaterThan(CURREF, "Test Assert 29", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 29", TRUE, assertReturn)
end ;testSuccessOfI4GreaterThanAssert

subroutine testSuccessOfI2GreaterThanAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 925
  set value2 = 55
  set assertReturn = cclutAssertI2GreaterThan(CURREF, "Test Assert 30", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 30", TRUE, assertReturn)
end ;testSuccessOfI2GreaterThanAssert

subroutine testSuccessOfVCGreaterThanAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = "Table"
  set value2 = "Super"
  set assertReturn = cclutAssertVCGreaterThan(CURREF, "Test Assert 31", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 31", TRUE, assertReturn)

  set value1 = notrim("hw ")
  set value2 = "hw"
  set assertReturn = cclutAssertVCGreaterThan(CURREF, "Test Assert 31a", notrim(value1), value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 31a", TRUE, assertReturn)

  set assertReturn = cclutAssertVCGreaterThan(CURREF, "Test Assert 31b", " ", "")
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 31b", TRUE, assertReturn)

  set assertReturn = cclutAssertVCGreaterThan(CURREF, "Test Assert 31c", "", trim(" "))
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 31c", TRUE, assertReturn)
end ;testSuccessOfVCGreaterThanAssert

subroutine testSuccessOfDateTimeGreaterThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime(curdate+1, 0)
  set value2 = cnvtdatetime(curdate, 0)
  set assertReturn = cclutAssertDateTimeGreaterThan(CURREF, "Test Assert 32", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 32", TRUE, assertReturn)
end ;testSuccessOfDateTimeGreaterThanAssert


;**********************************************************************************************************************************
;** NotGreaterThan
;**********************************************************************************************************************************
subroutine testSuccessOfF8NotGreaterThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 4.302
  set value2 = 5.3
  set assertReturn = cclutAssertF8NotGreaterThan(CURREF, "Test Assert 33", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 33", TRUE, assertReturn)
  
  set value1 = 2.4
  set value2 = 2.4
  set assertReturn = cclutAssertF8NotGreaterThan(CURREF, "Test Assert 34", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 34", TRUE, assertReturn)
end ;testSuccessOfF8NotGreaterThanAssert

subroutine testSuccessOfI4NotGreaterThanAssert(null)
  declare value1 = i4 with protect, noconstant(0)
  declare value2 = i4 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
    
  set value1 = 89
  set value2 = 3645
  set assertReturn = cclutAssertI4NotGreaterThan(CURREF, "Test Assert 35", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 35", TRUE, assertReturn)
  
  set value1 = 827
  set value2 = 827
  set assertReturn = cclutAssertI4NotGreaterThan(CURREF, "Test Assert 36", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 36", TRUE, assertReturn)
end ;testSuccessOfI4NotGreaterThanAssert

subroutine testSuccessOfI2NotGreaterThanAssert(null)
  declare value1 = i2 with protect, noconstant(0)
  declare value2 = i2 with protect, noconstant(0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = 13
  set value2 = 55
  set assertReturn = cclutAssertI2NotGreaterThan(CURREF, "Test Assert 37", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 37", TRUE, assertReturn)
  
  set value1 = 14
  set value2 = 14
  set assertReturn = cclutAssertI2NotGreaterThan(CURREF, "Test Assert 38", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 38", TRUE, assertReturn)
end ;testSuccessOfI2NotGreaterThanAssert

subroutine testSuccessOfVCNotGreaterThanAssert(null)
  declare value1 = vc with protect, noconstant("")
  declare value2 = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = "Goodbye"
  set value2 = "Super"
  set assertReturn = cclutAssertVCNotGreaterThan(CURREF, "Test Assert 39", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 39", TRUE, assertReturn)
  
  set value1 = "Good"
  set value2 = notrim("Good ")
  set assertReturn = cclutAssertVCNotGreaterThan(CURREF, "Test Assert 40", value1, notrim(value2))
  call cclutAssertI2Equal(CURREF, "A string is not greater than the string padded with spaces", TRUE, assertReturn)
end ;testSuccessOfVCNotGreaterThanAssert

subroutine testSuccessOfDateTimeNotGreaterThanAssert(null)
  declare value1 = f8 with protect, noconstant(0.0)
  declare value2 = f8 with protect, noconstant(0.0)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set value1 = cnvtdatetime(curdate-2, 0)
  set value2 = cnvtdatetime(curdate, 0)
  set assertReturn = cclutAssertDateTimeNotGreaterThan(CURREF, "Test Assert 41", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 41", TRUE, assertReturn)
  
  set value1 = cnvtdatetime("01-JAN-2001 00:01:02")
  set value2 = cnvtdatetime("01-JAN-2001 00:01:02")
  set assertReturn = cclutAssertDateTimeNotGreaterThan(CURREF, "Test Assert 42", value1, value2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 42", TRUE, assertReturn)
end ;testSuccessOfDateTimeNotGreaterThanAssert


;**********************************************************************************************************************************
;** Contains
;**********************************************************************************************************************************
subroutine testSuccessOfContainsAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = "good"
  set bigStr = "This is good for me!"
  set assertReturn = cclutAssertContains(CURREF, "Test Assert 43", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 43", TRUE, assertReturn)
end ;testSuccessOfContainsAssert

subroutine testSuccessOfNotContainsAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = "bad"
  set bigStr = "What is Badmiton?"
  set assertReturn = cclutAssertNotContains(CURREF, "Test Assert 44", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 44", TRUE, assertReturn)
end ;testSuccessOfNotContainsAssert


;**********************************************************************************************************************************
;** StartsWith
;**********************************************************************************************************************************
subroutine testSuccessOfStartsWithAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = "This"
  set bigStr = "This is good for me!"
  set assertReturn = cclutAssertStartsWith(CURREF, "Test Assert 45", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 45", TRUE, assertReturn)
end ;testSuccessOfStartsWithAssert

subroutine testSuccessOfNotStartsWithAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = " This"
  set bigStr = "This is good for me!"
  set assertReturn = cclutAssertNotStartsWith(CURREF, "Test Assert 46", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 46", TRUE, assertReturn)
end ;testSuccessOfNotStartsWithAssert


;**********************************************************************************************************************************
;** EndsWith
;**********************************************************************************************************************************
subroutine testSuccessOfEndsWithAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = " me!"
  set bigStr = "This is good for me!"
  set assertReturn = cclutAssertEndsWith(CURREF, "Test Assert 47", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 47", TRUE, assertReturn)
end ;testSuccessOfEndsWithAssert

subroutine testSuccessOfNotEndsWithAssert(null)
  declare subStr = vc with protect, noconstant("")
  declare bigStr = vc with protect, noconstant("")
  declare assertReturn = i2 with protect, noconstant(99)
  
  set subStr = " ld"
  set bigStr = "hw"
  set assertReturn = cclutAssertNotEndsWith(CURREF, "Test Assert 48", subStr, bigStr)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 48", TRUE, assertReturn)
end ;testSuccessOfNotEndsWithAssert


;**********************************************************************************************************************************
;** Operartor
;**********************************************************************************************************************************
subroutine testSuccessOfF8OperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set assertReturn = cclutAssertF8Operator(CURREF, "Test Assert 49", 3.2, "<", 3.21)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 49", TRUE, assertReturn)
end ;testSuccessOfF8OperatorAssert

subroutine testSuccessOfF8NotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set assertReturn = cclutAssertF8NotOperator(CURREF, "Test Assert 50", 3.21, "<", 3.2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 50", TRUE, assertReturn)
end ;testSuccessOfF8NotOperatorAssert

subroutine testSuccessOfI4OperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set assertReturn = cclutAssertI4Operator(CURREF, "Test Assert 51", 4, ">=", 3)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 51", TRUE, assertReturn)
end ;testSuccessOfI4OperatorAssert

subroutine testSuccessOfI4NotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set assertReturn = cclutAssertI4NotOperator(CURREF, "Test Assert 52", 6, "!=", 6)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 52", TRUE, assertReturn)
end ;testSuccessOfI4NotOperatorAssert

subroutine testSuccessOfI2OperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  
  set assertReturn = cclutAssertI2Operator(CURREF, "Test Assert 53", TRUE, ">", FALSE)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 53", TRUE, assertReturn)
end ;testSuccessOfI2OperatorAssert

subroutine testSuccessOfI2NotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
 
  set assertReturn = cclutAssertI2NotOperator(CURREF, "Test Assert 54", TRUE, "not !=", FALSE)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 54", TRUE, assertReturn)
end ;testSuccessOfI2NotOperatorAssert

subroutine testSuccessOfVcOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  declare str = vc with protect, noconstant("")
  declare op = vc with protect, noconstant("")
  declare opval = vc with protect, noconstant("")
  
  set str = "41276 check    this out. "
  set op = "regexplike"
  set opval = "41[0-9]{3} check {4}this out\. "
  
  set assertReturn = cclutAssertVCOperator(CURREF, "Test Assert 55", str, op, opval)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 55", TRUE, assertReturn)
end ;testSuccessOfVcOperatorAssert

subroutine testSuccessOfVcNotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  declare str = vc with protect, noconstant("")
  declare op = vc with protect, noconstant("")
  declare opval = vc with protect, noconstant("")
  
  set str = "41276 check    this out. "
  set op = "regexplike"
  set opval = "41[0-9]{4} check *this out\. "
  
  set assertReturn = cclutAssertVCNotOperator(CURREF, "Test Assert 56", str, op, opval)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 56", TRUE, assertReturn)
end ;testSuccessOfVcNotOperatorAssert

subroutine testSuccessOfDateTimeOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  declare datetime1 = dq8 with protect, noconstant(cnvtdatetime(curdate, 120101))
  declare datetime2 = dq8 with protect, noconstant(cnvtdatetime(curdate, 120102))
  
  set assertReturn = cclutAssertDateTimeOperator(CURREF, "Test Assert 57", datetime1, "<", datetime2)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 57", TRUE, assertReturn)
end ;testSuccessOfDateTimeOperatorAssert

subroutine testSuccessOfDateTimeNotOperatorAssert(null)
  declare assertReturn = i2 with protect, noconstant(99)
  declare datetime1 = dq8 with protect, noconstant(cnvtdatetime(curdate, 120101))
  declare datetime2 = dq8 with protect, noconstant(cnvtdatetime(curdate, 120102))
 
  set assertReturn = cclutAssertDateTimeNotOperator(CURREF, "Test Assert 58", datetime2, "<", datetime1)
  call cclutAssertI2Equal(CURREF, "Test Assert Return Value 58", TRUE, assertReturn)
end ;testSuccessOfDateTimeNotOperatorAssert

subroutine testAssertFailures(null)
  declare stat = i4 with protect, noconstant(0)
  declare equals = vc with protect, constant(
    "========================================================================================================================")
  declare dashes = vc with protect, constant(
    "------------------------------------------------------------------------------------------------------------------------")
  declare stars = vc with protect, constant(
    "************************************************************************************************************************")

  record testing::outputLines (
    1 line[*]
      2 type = i4
      2 text = vc
  ) with protect
  
  set stat = alterlist(testing::outputLines->line, 297)
  set testing::outputLines->line[1].text = equals
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text =
    ^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text =
    ^ ^
  set testing::outputLines->line[4].text =
    ^Test Case: cclsource:ut_cclut_assert_failure^
  set testing::outputLines->line[5].text =
    ^passed: 0           FAILED: 49          errored: 0^
  set testing::outputLines->line[6].text =
    ^ ^
  set testing::outputLines->line[7].text =
    ^ ^
  set testing::outputLines->line[8].text =
    ^Failed Tests^
  set testing::outputLines->line[9].text = equals
  set testing::outputLines->line[10].text =
    ^TESTFAILUREOFF8EQUALASSERT^
  set testing::outputLines->line[11].text = dashes
  set testing::outputLines->line[12].text =
    ^    Assert Failure: 6.700000 = 2.400000^
  set testing::outputLines->line[13].text =
    ^    Test Assert Failure 1^ 
  set testing::outputLines->line[14].text =
    ^ ^
  set testing::outputLines->line[15].text =
    ^TESTFAILUREOFI4EQUALASSERT^
  set testing::outputLines->line[16].text = dashes
  set testing::outputLines->line[17].text =
    ^    Assert Failure: 9827 = 243^
  set testing::outputLines->line[18].text =
    ^    Test Assert Failure 2^
  set testing::outputLines->line[19].text =
    ^ ^
  set testing::outputLines->line[20].text =
    ^TESTFAILUREOFI2EQUALASSERT^
  set testing::outputLines->line[21].text = dashes
  set testing::outputLines->line[22].text =
    ^    Assert Failure: 1 = 0^
  set testing::outputLines->line[23].text =
    ^    Test Assert Failure 3^
  set testing::outputLines->line[24].text =
    ^ ^
  set testing::outputLines->line[25].text =
    ^TESTFAILUREOFVCEQUALASSERT^
  set testing::outputLines->line[26].text = dashes
  set testing::outputLines->line[27].text =
    ^    Assert Failure: "Fantastic" = "Terrible"^
  set testing::outputLines->line[28].text =
    ^    Test Assert Failure 4^
  set testing::outputLines->line[29].text = dashes
  set testing::outputLines->line[30].text =
    concat(^    Assert Failure: "^, NULL_MARKER, ^" = " "^)
  set testing::outputLines->line[31].text =
    ^    Test Assert Failure 4a^
  set testing::outputLines->line[32].text = dashes
  set testing::outputLines->line[33].text =
    concat(^    Assert Failure: "^, NULL_MARKER, ^" = " "^)
  set testing::outputLines->line[34].text =
    ^    Test Assert Failure 4b^
  set testing::outputLines->line[35].text = dashes
  set testing::outputLines->line[36].text =
    concat(^    Assert Failure: "^, NULL_MARKER, ^" = ""^)
  set testing::outputLines->line[37].text =
    ^    Test Assert Failure 4c^
  set testing::outputLines->line[38].text = dashes
  set testing::outputLines->line[39].text =
    ^    Assert Failure: "hw" = "hw "^
  set testing::outputLines->line[40].text =
    ^    Test Assert Failure 4d^
  set testing::outputLines->line[41].text =
    ^ ^
  set testing::outputLines->line[42].text =
    ^TESTFAILUREOFDATETIMEEQUALASSERT^
  set testing::outputLines->line[43].text = dashes
  set testing::outputLines->line[44].text =
    ^    Assert Failure: '2017-11-30 15:55:00' = '2001-01-01 00:01:02'^
  set testing::outputLines->line[45].text =
    ^    Test Assert Failure 5^
  set testing::outputLines->line[46].text =
    ^ ^
  set testing::outputLines->line[47].text =
    ^TESTFAILUREOFF8ALMOSTEQUALASSERT^
  set testing::outputLines->line[48].text = dashes
  set testing::outputLines->line[49].text =
    ^    Assert Failure: abs(2.600000-2.400000) <= 0.030000^
  set testing::outputLines->line[50].text =
    ^    Test Assert Failure 6^
  set testing::outputLines->line[51].text =
    ^ ^
  set testing::outputLines->line[52].text =
    ^TESTFAILUREOFF8NOTALMOSTEQUALASSERT^
  set testing::outputLines->line[53].text = dashes
  set testing::outputLines->line[54].text =
    ^    Assert Failure: abs(2.880000-2.900000) > 0.050000^
  set testing::outputLines->line[55].text =
    ^    Test Assert Failure 7^
  set testing::outputLines->line[56].text =
    ^ ^
  set testing::outputLines->line[57].text =
    ^TESTFAILUREOFF8NOTEQUALASSERT^
  set testing::outputLines->line[58].text = dashes
  set testing::outputLines->line[59].text =
    ^    Assert Failure: 6.700000 != 6.700000^
  set testing::outputLines->line[60].text =
    ^    Test Assert Failure 8^
  set testing::outputLines->line[61].text =
    ^ ^
  set testing::outputLines->line[62].text =
    ^TESTFAILUREOFI4NOTEQUALASSERT^
  set testing::outputLines->line[63].text = dashes
  set testing::outputLines->line[64].text =
    ^    Assert Failure: 9827 != 9827^
  set testing::outputLines->line[65].text =
    ^    Test Assert Failure 9^
  set testing::outputLines->line[66].text =
    ^ ^
  set testing::outputLines->line[67].text =
    ^TESTFAILUREOFI2NOTEQUALASSERT^
  set testing::outputLines->line[68].text = dashes
  set testing::outputLines->line[69].text =
    ^    Assert Failure: 12 != 12^
  set testing::outputLines->line[70].text =
    ^    Test Assert Failure 10^
  set testing::outputLines->line[71].text =
    ^ ^
  set testing::outputLines->line[72].text =
    ^TESTFAILUREOFVCNOTEQUALASSERT^
  set testing::outputLines->line[73].text = dashes
  set testing::outputLines->line[74].text =
    ^    Assert Failure: "Fantastic   " != "Fantastic"^
  set testing::outputLines->line[75].text =
    ^    Test Assert Failure 11^
  set testing::outputLines->line[76].text =
    ^ ^
  set testing::outputLines->line[77].text =
    ^TESTFAILUREOFDATETIMENOTEQUALASSERT^
  set testing::outputLines->line[78].text = dashes
  set testing::outputLines->line[79].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[79].text = concat(
    ^    Assert Failure: '[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}' != ^,
    ^'[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}'^)
  set testing::outputLines->line[80].text =
    ^    Test Assert Failure 12^
  set testing::outputLines->line[81].text =
    ^ ^
  set testing::outputLines->line[82].text =
    ^TESTFAILUREOFF8LESSTHANASSERT^
  set testing::outputLines->line[83].text = dashes
  set testing::outputLines->line[84].text =
    ^    Assert Failure: 2.400000 < 2.400000^
  set testing::outputLines->line[85].text =
    ^    Test Assert Failure 13^
  set testing::outputLines->line[86].text =
    ^ ^
  set testing::outputLines->line[87].text =
    ^TESTFAILUREOFI4LESSTHANASSERT^
  set testing::outputLines->line[88].text = dashes
  set testing::outputLines->line[89].text =
    ^    Assert Failure: 827 < 243^
  set testing::outputLines->line[90].text =
    ^    Test Assert Failure 14^
  set testing::outputLines->line[91].text =
    ^ ^
  set testing::outputLines->line[92].text =
    ^TESTFAILUREOFI2LESSTHANASSERT^
  set testing::outputLines->line[93].text = dashes
  set testing::outputLines->line[94].text =
    ^    Assert Failure: 57 < 14^
  set testing::outputLines->line[95].text =
    ^    Test Assert Failure 15^ 
  set testing::outputLines->line[96].text =
    ^ ^
  set testing::outputLines->line[97].text =
    ^TESTFAILUREOFVCLESSTHANASSERT^
  set testing::outputLines->line[98].text = dashes
  set testing::outputLines->line[99].text =
    ^    Assert Failure: "Wow" < "Terrible"^
  set testing::outputLines->line[100].text =
    ^    Test Assert Failure 16^
  set testing::outputLines->line[101].text = dashes
  set testing::outputLines->line[102].text =
    ^    Assert Failure: "hw" < "hw"^
  set testing::outputLines->line[103].text =
    ^    Test Assert Failure 16a^
  set testing::outputLines->line[104].text =
    ^ ^
  set testing::outputLines->line[105].text =
    ^TESTFAILUREOFDATETIMELESSTHANASSERT^
  set testing::outputLines->line[106].text = dashes
  set testing::outputLines->line[107].text =
    ^    Assert Failure: '2001-01-01 00:01:02' < '2001-01-01 00:01:02'^
  set testing::outputLines->line[108].text =
    ^    Test Assert Failure 17^
  set testing::outputLines->line[109].text =
    ^ ^
  set testing::outputLines->line[110].text =
    ^TESTFAILUREOFF8NOTLESSTHANASSERT^
  set testing::outputLines->line[111].text = dashes
  set testing::outputLines->line[112].text =
    ^    Assert Failure: 1.700000 >= 2.400000^
  set testing::outputLines->line[113].text =
    ^    Test Assert Failure 18^
  set testing::outputLines->line[114].text =
    ^ ^
  set testing::outputLines->line[115].text =
    ^TESTFAILUREOFI4NOTLESSTHANASSERT^
  set testing::outputLines->line[116].text = dashes
  set testing::outputLines->line[117].text =
    ^    Assert Failure: 127 >= 243^
  set testing::outputLines->line[118].text =
    ^    Test Assert Failure 19^
  set testing::outputLines->line[119].text =
    ^ ^
  set testing::outputLines->line[120].text =
    ^TESTFAILUREOFI2NOTLESSTHANASSERT^
  set testing::outputLines->line[121].text = dashes
  set testing::outputLines->line[122].text =
    ^    Assert Failure: 7 >= 14^
  set testing::outputLines->line[123].text =
    ^    Test Assert Failure 20^
  set testing::outputLines->line[124].text =
    ^ ^
  set testing::outputLines->line[125].text =
    ^TESTFAILUREOFVCNOTLESSTHANASSERT^
  set testing::outputLines->line[126].text = dashes
  set testing::outputLines->line[127].text =
    ^    Assert Failure: "Fantastic" >= "Terrible"^
  set testing::outputLines->line[128].text =
    ^    Test Assert Failure 21^
  set testing::outputLines->line[129].text = dashes
  set testing::outputLines->line[130].text =
    ^    Assert Failure: "hw" >= "hw "^
  set testing::outputLines->line[131].text =
    ^    Test Assert Failure 21a^
  set testing::outputLines->line[132].text =
    ^ ^
  set testing::outputLines->line[133].text =
    ^TESTFAILUREOFDATETIMENOTLESSTHANASSERT^
  set testing::outputLines->line[134].text = dashes
  set testing::outputLines->line[135].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[135].text = concat(
    ^    Assert Failure: '[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}' >= ^,
    ^'[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}'^)
  set testing::outputLines->line[136].text =
    ^    Test Assert Failure 22^
  set testing::outputLines->line[137].text =
    ^ ^
  set testing::outputLines->line[138].text =
    ^TESTFAILUREOFF8GREATERTHANASSERT^
  set testing::outputLines->line[139].text = dashes
  set testing::outputLines->line[140].text =
    ^    Assert Failure: 1.700000 > 2.400000^
  set testing::outputLines->line[141].text =
    ^    Test Assert Failure 23^
  set testing::outputLines->line[142].text =
    ^ ^
  set testing::outputLines->line[143].text =
    ^TESTFAILUREOFI4GREATERTHANASSERT^
  set testing::outputLines->line[144].text = dashes
  set testing::outputLines->line[145].text =
    ^    Assert Failure: 827 > 827^
  set testing::outputLines->line[146].text =
    ^    Test Assert Failure 24^
  set testing::outputLines->line[147].text =
    ^ ^
  set testing::outputLines->line[148].text =
    ^TESTFAILUREOFI2GREATERTHANASSERT^
  set testing::outputLines->line[149].text = dashes
  set testing::outputLines->line[150].text =
    ^    Assert Failure: 57 > 114^
  set testing::outputLines->line[151].text =
    ^    Test Assert Failure 25^
  set testing::outputLines->line[152].text =
    ^ ^
  set testing::outputLines->line[153].text =
    ^TESTFAILUREOFVCGREATERTHANASSERT^
  set testing::outputLines->line[154].text = dashes
  set testing::outputLines->line[155].text =
    ^    Assert Failure: "Dove" > "Terrible"^
  set testing::outputLines->line[156].text =
    ^    Test Assert Failure 26^
  set testing::outputLines->line[157].text = dashes
  set testing::outputLines->line[158].text =
    ^    Assert Failure: "hw" > "hw"^
  set testing::outputLines->line[159].text =
    ^    Test Assert Failure 26a^
  set testing::outputLines->line[160].text =
    ^ ^
  set testing::outputLines->line[161].text =
    ^TESTFAILUREOFDATETIMEGREATERTHANASSERT^
  set testing::outputLines->line[162].text = dashes
  set testing::outputLines->line[163].text =
    ^    Assert Failure: '2001-01-01 00:01:02' > '2001-01-01 00:01:02'^
  set testing::outputLines->line[164].text =
    ^    Test Assert Failure 27^
  set testing::outputLines->line[165].text =
    ^ ^
  set testing::outputLines->line[166].text =
    ^TESTFAILUREOFF8NOTGREATERTHANASSERT^
  set testing::outputLines->line[167].text = dashes
  set testing::outputLines->line[168].text =
    ^    Assert Failure: 981.700000 <= 2.400000^
  set testing::outputLines->line[169].text =
    ^    Test Assert Failure 28^
  set testing::outputLines->line[170].text =
    ^ ^
  set testing::outputLines->line[171].text =
    ^TESTFAILUREOFI4NOTGREATERTHANASSERT^
  set testing::outputLines->line[172].text = dashes
  set testing::outputLines->line[173].text =
    ^    Assert Failure: 593 <= 243^
  set testing::outputLines->line[174].text =
    ^    Test Assert Failure 29^
  set testing::outputLines->line[175].text =
    ^ ^
  set testing::outputLines->line[176].text =
    ^TESTFAILUREOFI2NOTGREATERTHANASSERT^
  set testing::outputLines->line[177].text = dashes
  set testing::outputLines->line[178].text =
    ^    Assert Failure: 87 <= 14^
  set testing::outputLines->line[179].text =
    ^    Test Assert Failure 30^
  set testing::outputLines->line[180].text =
    ^ ^
  set testing::outputLines->line[181].text =
    ^TESTFAILUREOFVCNOTGREATERTHANASSERT^
  set testing::outputLines->line[182].text = dashes
  set testing::outputLines->line[183].text =
    ^    Assert Failure: "Wave" <= "Terrible"^
  set testing::outputLines->line[184].text =
    ^    Test Assert Failure 31^
  set testing::outputLines->line[185].text = dashes
  set testing::outputLines->line[186].text =
    ^    Assert Failure: "hw " <= "hw"^
  set testing::outputLines->line[187].text =
    ^    Test Assert Failure 31a^
  set testing::outputLines->line[188].text =
    ^ ^
  set testing::outputLines->line[189].text =
    ^TESTFAILUREOFDATETIMENOTGREATERTHANASSERT^
  set testing::outputLines->line[190].text = dashes
  set testing::outputLines->line[191].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[191].text = concat(
    ^    Assert Failure: '[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}' <= ^,
    ^'[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}'^)
  set testing::outputLines->line[192].text =
    ^    Test Assert Failure 32^
  set testing::outputLines->line[193].text =
    ^ ^
  set testing::outputLines->line[194].text =
    ^TESTFAILUREOFCONTAINSASSERT^
  set testing::outputLines->line[195].text = dashes
  set testing::outputLines->line[196].text =
    ^    Assert Failure: findstring("Good","This is good for me!") > 0^
  set testing::outputLines->line[197].text =
    ^    Test Assert Failure 33^
  set testing::outputLines->line[198].text =
    ^ ^
  set testing::outputLines->line[199].text =
    ^TESTFAILUREOFNOTCONTAINSASSERT^
  set testing::outputLines->line[200].text = dashes
  set testing::outputLines->line[201].text =
    ^    Assert Failure: findstring("better","Good, better, worse") = 0^
  set testing::outputLines->line[202].text =
    ^    Test Assert Failure 34^
  set testing::outputLines->line[203].text =
    ^ ^
  set testing::outputLines->line[204].text =
    ^TESTFAILUREOFSTARTSWITHASSERT^
  set testing::outputLines->line[205].text = dashes
  set testing::outputLines->line[206].text =
    ^    Assert Failure: "This is good for me!" starts with "his"^
  set testing::outputLines->line[207].text =
    ^    Test Assert Failure 35^
  set testing::outputLines->line[208].text =
    ^ ^
  set testing::outputLines->line[209].text =
    ^TESTFAILUREOFNOTSTARTSWITHASSERT^
  set testing::outputLines->line[210].text = dashes
  set testing::outputLines->line[211].text =
    ^    Assert Failure: " This is bad for me!" does not start with " This"^
  set testing::outputLines->line[212].text =
    ^    Test Assert Failure 36^
  set testing::outputLines->line[213].text =
    ^ ^
  set testing::outputLines->line[214].text =
    ^TESTFAILUREOFENDSWITHASSERT^
  set testing::outputLines->line[215].text = dashes
  set testing::outputLines->line[216].text =
    ^    Assert Failure: " This is good for me!" ends with " e!"^
  set testing::outputLines->line[217].text =
    ^    Test Assert Failure 37^
  set testing::outputLines->line[218].text =
    ^ ^
  set testing::outputLines->line[219].text =
    ^TESTFAILUREOFNOTENDSWITHASSERT^
  set testing::outputLines->line[220].text = dashes
  set testing::outputLines->line[221].text =
    ^    Assert Failure: "Salt and pepper" does not end with " pepper"^
  set testing::outputLines->line[222].text =
    ^    Test Assert Failure 38^
  set testing::outputLines->line[223].text =
    ^ ^
  set testing::outputLines->line[224].text =
    ^TESTFAILUREOFF8OPERATORASSERT^
  set testing::outputLines->line[225].text = dashes
  set testing::outputLines->line[226].text =
    ^    Assert Failure: operator(3.200000, '>', 3.210000)^
  set testing::outputLines->line[227].text =
    ^    Test Assert Failure 39^
  set testing::outputLines->line[228].text =
    ^ ^
  set testing::outputLines->line[229].text =
    ^TESTFAILUREOFF8NOTOPERATORASSERT^
  set testing::outputLines->line[230].text = dashes
  set testing::outputLines->line[231].text =
    ^    Assert Failure: notoperator(3.210000, '>', 3.200000)^
  set testing::outputLines->line[232].text =
    ^    Test Assert Failure 40^
  set testing::outputLines->line[233].text =
    ^ ^
  set testing::outputLines->line[234].text =
    ^TESTFAILUREOFI4OPERATORASSERT^
  set testing::outputLines->line[235].text = dashes
  set testing::outputLines->line[236].text =
    ^    Assert Failure: operator(4, '<=', 3)^
  set testing::outputLines->line[237].text =
    ^    Test Assert Failure 41^
  set testing::outputLines->line[238].text =
    ^ ^
  set testing::outputLines->line[239].text =
    ^TESTFAILUREOFI4NOTOPERATORASSERT^
  set testing::outputLines->line[240].text = dashes
  set testing::outputLines->line[241].text =
    ^    Assert Failure: notoperator(6, '!=', 5)^
  set testing::outputLines->line[242].text =
    ^    Test Assert Failure 42^
 set testing::outputLines->line[243].text =
    ^ ^
  set testing::outputLines->line[244].text =
    ^TESTFAILUREOFI2OPERATORASSERT^
  set testing::outputLines->line[245].text = dashes
  set testing::outputLines->line[246].text =
    ^    Assert Failure: operator(1, '<', 0)^
  set testing::outputLines->line[247].text =
    ^    Test Assert Failure 43^
  set testing::outputLines->line[248].text =
    ^ ^
  set testing::outputLines->line[249].text =
    ^TESTFAILUREOFI2NOTOPERATORASSERT^
  set testing::outputLines->line[250].text = dashes
  set testing::outputLines->line[251].text =
    ^    Assert Failure: notoperator(1, 'not !=', 1)^
  set testing::outputLines->line[252].text =
    ^    Test Assert Failure 44^
  set testing::outputLines->line[253].text =
    ^ ^
  set testing::outputLines->line[254].text =
    ^TESTFAILUREOFVCOPERATORASSERT^
  set testing::outputLines->line[255].text = dashes
  set testing::outputLines->line[256].text =
    ^    Assert Failure: operator('41276 check    this out.', 'regexplike', '41[0-9]{4} check *this out\.')^
  set testing::outputLines->line[257].text =
    ^    Test Assert Failure 45^
  set testing::outputLines->line[258].text =
    ^ ^
  set testing::outputLines->line[259].text =
    ^TESTFAILUREOFVCNOTOPERATORASSERT^
  set testing::outputLines->line[260].text = dashes
  set testing::outputLines->line[261].text =
    ^    Assert Failure: 0 = 1^
  set testing::outputLines->line[262].text =
    ^    Test Assert Return Value^ 
  set testing::outputLines->line[263].text =
    ^ ^
  set testing::outputLines->line[264].text =
    ^TESTFAILUREOFDATETIMEOPERATORASSERT^
  set testing::outputLines->line[265].text = dashes
  set testing::outputLines->line[266].text =
    ^    Assert Failure: 0 = 1^
  set testing::outputLines->line[267].text =
    ^    Test Assert Return Value^ 
  set testing::outputLines->line[268].text =
    ^ ^
  set testing::outputLines->line[269].text =
    ^TESTFAILUREOFDATETIMENOTOPERATORASSERT^
  set testing::outputLines->line[270].text = dashes
  set testing::outputLines->line[271].text =
    ^    Assert Failure: 0 = 1^
  set testing::outputLines->line[272].text =
    ^    Test Assert Return Value^ 
  set testing::outputLines->line[273].text =
    ^ ^
  set testing::outputLines->line[274].text =
    ^TESTFAILUREOFVCASSERTSWITHCCLSTRINGLOGIC^
  set testing::outputLines->line[275].text = dashes
  set testing::outputLines->line[276].text =
    concat(^    Assert Failure: "hw " = "hw^, NULL_MARKER, ^"^)
  set testing::outputLines->line[277].text =
    ^    Test Assert Failure 49a^
  set testing::outputLines->line[278].text = dashes
  set testing::outputLines->line[279].text =
    concat(^    Assert Failure: "hw" < "hw^, NULL_MARKER, ^"^)
  set testing::outputLines->line[280].text =
    ^    Test Assert Failure 49b^
  set testing::outputLines->line[281].text = dashes
  set testing::outputLines->line[282].text =
    ^    Assert Failure: "hw" < "hw "^
  set testing::outputLines->line[283].text =
    ^    Test Assert Failure 49c^
  set testing::outputLines->line[284].text = dashes
  set testing::outputLines->line[285].text =
    concat(^    Assert Failure: "hw^, NULL_MARKER, ^" > "hw"^)
  set testing::outputLines->line[286].text =
    ^    Test Assert Failure 49d^
  set testing::outputLines->line[287].text = dashes
  set testing::outputLines->line[288].text =
    ^    Assert Failure: "hw " > "hw"^
  set testing::outputLines->line[289].text =
    ^    Test Assert Failure 49e^
  set testing::outputLines->line[290].text = dashes
  set testing::outputLines->line[291].text =
    concat(^    Assert Failure: "hw" != "hw^, NULL_MARKER, ^"^)
  set testing::outputLines->line[292].text =
    ^    Test Assert Failure 50a^
  set testing::outputLines->line[293].text = dashes
  set testing::outputLines->line[294].text =
    ^    Assert Failure: "hw" != "hw "^
  set testing::outputLines->line[295].text =
    ^    Test Assert Failure 50b^
  set testing::outputLines->line[296].text =
    ^ ^
  set testing::outputLines->line[297].text = stars

record ut_cclut_cclut1TestCaseRequest (
  1 testCaseDirectory = vc
  1 testCaseFileName = vc
  1 testNamePattern = vc
  1 programs[*]
    2 programName = vc
    2 compile = i2
  1 optimizerMode = vc
  1 deprecatedFlag = vc
  1 failFast = i2
) with protect
    
record ut_cclut_testCaseResults (
  1 resultInd = i2
  1 tests[*]
    2 name = vc
    2 asserts[*]
      3 lineNumber = i4
      3 context = vc
      3 resultInd = i2 ;TRUE = PASS, FALSE = FAIL
      3 condition = vc
    2 errors[*]
      3 lineNumber = i4
      3 errorText = vc
%i cclsource:status_block.inc
) with protect

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_assert_failure", "", "", "", FALSE with
      replace("CCLUT1TESTCASEREQUEST", ut_cclut_testCaseRequest),
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults)
  call testing::checkLines(testing::outputLines)
end ;;;testAssertFailures


subroutine testParameterNameConflicts(null)
  declare successIndA = i2 with protect, noconstant(0)
  declare successIndB = i2 with protect, noconstant(0)
  
  record lineNumber (1 line[*] 2 no = i4 2 text = vc) with protect
  record context (1 type = i4 1 description = vc) with protect
  record condition (1 type = i4 1 description = vc) with protect
  record display (1 type = i4 1 vertical = i4 1 horizontal = i4) with protect
  record comparison (1 id = i4 1 name = vc) with protect
  record value (1 id = i4 1 name = vc) with protect
  record comparisonValue (1 id = i4 1 name = vc) with protect
  record tolerance (1 id = i4 1 name = vc) with protect
  record value1 (1 id = i4 1 name = vc) with protect
  record value2 (1 id = i4 1 name = vc) with protect
  record successFlag (1 id = i4 1 name = vc) with protect
  record errorCodeBefore (1 id = i4 1 name = vc) with protect
  record errorMessageBefore (1 id = i4 1 name = vc) with protect
  record errorCode (1 id = i4 1 name = vc) with protect
  record errorMessage (1 id = i4 1 name = vc) with protect
  record string (1 id = i4 1 name = vc) with protect
  record substring (1 id = i4 1 name = vc) with protect
  
  set successIndA = cclutAssertf8AlmostEqual(CURREF, "almost equal", 3.25, 3.20, 0.051)
  set successIndB = cclutAssertI4Equal(CURREF, "assert success", successIndA, TRUE)

  set successIndA = cclutAssertVCOperator(CURREF, "vc operator", "hw", "regexplike", "h.*")
  set successIndB = cclutAssertI4Equal(CURREF, "assert success", successIndA, TRUE)
  
  set successIndA = cclutAssertStartsWith(CURREF, "starts with", "hello", "hello cerner")
  set successIndB = cclutAssertI4Equal(CURREF, "assert success", successIndA, TRUE)
end ;;;testParameterNameConflicts


/**
  Confirms that a cclut::useCclStringLogic will cause trailing nulls and spaces to be ignored with respect to asserts.
  Note that 1, 1a, 6a and 7a fail with or without cclut::useCclStringLogic, further support for the extra string logic.
  If vcNull equals vcEmpty and vcEmpty equals vcSpace (2/2a and 3/3a), why doesn't vcNull equal vcSpace (1/1a)?
*/
subroutine testStringLogicOverride(null)
  declare cclut::useCclStringLogic = i2 with protect, noconstant(TRUE)

  declare vcNull = vc with protect, constant("")
  declare vcEmpty = vc with protect, constant(trim(""))
  declare vcSpace = vc with protect, constant(notrim(" "))
  declare vcDoubleSpace = vc with protect, constant(notrim("  "))

;  call cclutAssertVCEqual(CURREF, "string logic override 1", vcNull, notrim(vcSpace))
;  call cclutAssertVCEqual(CURREF, "string logic override 1a", notrim(vcSpace), vcNull)

  call cclutAssertVCEqual(CURREF, "string logic override 2", vcNull, vcEmpty)
  call cclutAssertVCEqual(CURREF, "string logic override 2a", vcEmpty, vcNull)

  call cclutAssertVCEqual(CURREF, "string logic override 3", notrim(vcSpace), vcEmpty)
  call cclutAssertVCEqual(CURREF, "string logic override 3a", vcEmpty, notrim(vcSpace))

  call cclutAssertVCEqual(CURREF, "string logic override 4", notrim(vcSpace), notrim(vcDoubleSpace))
  call cclutAssertVCEqual(CURREF, "string logic override 4a", notrim(vcDoubleSpace), notrim(vcSpace))

  call cclutAssertVCEqual(CURREF,"string logic override 5a", "hw", concat("hw ", char(0)))
  call cclutAssertVCEqual(CURREF,"string logic override 5b", concat("hw ", char(0)), "hw")
  call cclutAssertVCEqual(CURREF,"string logic override 5c", "hw", concat("hw", char(0), char(32)))
  call cclutAssertVCEqual(CURREF,"string logic override 5d", concat("hw", char(0), char(32)), "hw")

;  call cclutAssertVCNotLessThan(CURREF,"string logic override 6a", vcNull, notrim(vcSpace))
  call cclutAssertVCNotLessThan(CURREF,"string logic override 6b", notrim(vcSpace), notrim(vcDoubleSpace))
  call cclutAssertVCNotLessThan(CURREF,"string logic override 6c", vcEmpty, vcNull)
  call cclutAssertVCNotLessThan(CURREF,"string logic override 6d", vcEmpty, notrim(vcSpace))
  call cclutAssertVCNotLessThan(CURREF,"string logic override 6e", "hw", "hw ")
  call cclutAssertVCNotLessThan(CURREF,"string logic override 6f", "hw", concat("hw ", char(0)))

;  call cclutAssertVCNotGreaterThan(CURREF,"string logic override 7a", notrim(vcSpace), vcNull)
  call cclutAssertVCNotGreaterThan(CURREF,"string logic override 7b", notrim(vcDoubleSpace), notrim(vcSpace))
  call cclutAssertVCNotGreaterThan(CURREF,"string logic override 7c", vcNull, vcEmpty)
  call cclutAssertVCNotGreaterThan(CURREF,"string logic override 7d", notrim(vcSpace), vcEmpty)
  call cclutAssertVCNotGreaterThan(CURREF,"string logic override 7e", "hw ", "hw")
  call cclutAssertVCNotGreaterThan(CURREF,"string logic override 7f", concat("hw ", char(0)), "hw")
end ;;;testStringLogicOverride

