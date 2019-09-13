%#ifndef CCLUTASSERT_IMPL_FF_INC
%#ifndef CCLUTASSERT_IMPL_NFF_INC
%#def CCLUTASSERT_IMPL_FF_INC
;**********************************************************************************************************************************
;
; Provides implementations of the evaluateCondition and evaluateComparison subroutines to support the
; cclutAssert* implementations from cclutassert_base_impl.inc. This implementation is intended to run within a CCL session
; and echo information to that CCL session.
; It fails fast and the fail fast feature is implemented using "go to exit_script". (internal fail-fast)
;
;**********************************************************************************************************************************


%i cclsource:cclutassert_impl_base.inc

declare cclut::doOutput(cclutMsg = vc) = null with protect

subroutine cclut::doOutput(cclutMsg)
  call echo(cclutMsg) ;intentional
  if (validate(cclut_ff::output) = TRUE)
    if (cclut_ff::output = "")
      set cclut_ff::output = cclutMsg
    else
      set cclut_ff::output = concat(cclut_ff::output, char(10), char(13), cclutMsg)
    endif
  endif
end ;;;doOutput


/**
  Determines if a provided test operation is successful as part of an assert check. If not, routes execution to exit_script and
  sets cclut_test_runner::assertSuccess to FALSE if it exists.
  @param cclutLineNum
    The line number where the assert occurred.
  @param cclutCtx
    The context provided with the assert.
  @param cclutCondition
    The test operation to be performed (a string that will be evaluated with a parser call).
  @returns
    The boolean value from evaluating the condition, but only TRUE will be returned since control gets routed to exit_script if
    the condition is FALSE.
*/
subroutine cclut::evaluateCondition (cclutLineNum, cclutCtx, cclutCondition)
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCondition, cclutCondition))
end ;;;cclut::evaluateCondition


/**
  Determines if a provided test operation is successful as part of an assert check. If not, routes execution to exit_script and
  sets cclut_test_runner::assertSuccess to FALSE if it exists.
  @param cclutLineNum
    The line number where the assert occurred.
  @param cclutCtx
    The cclutCtx provided with the assert.
  @param cclutCondition
    The test operation to be performed (a string that will be evaluated with a parser call).
  @param cclutDisp
    The display for the condition, operator("hello world", "regexplike", "hell.*") for example.
  @returns
    The boolean value from evaluating the condition, but only TRUE will be returned since control gets routed to exit_script if
    the condition is FALSE.
*/
subroutine cclut::evaluateCondition2 (cclutLineNum, cclutCtx, cclutCondition, cclutDisp)
  declare cclutReturnValue = i2 with protect, noconstant(FALSE)

  if (validate(cclut_test_runner::assertSuccess, TRUE) = FALSE)
    ;;;an assert already failed without failing fast
    ;;;probably because the previous exit_script was not the outermost one.
    go to exit_script
  endif

  if (parser(cclutCondition) = TRUE)
    set cclutReturnValue = TRUE
  else
    set cclutReturnValue = FALSE
  endif

  if ( cclutReturnValue = FALSE )
    call cclut::doOutput(build2(
      "FAILURE: line number [", trim(build(cclutLineNum),3), "]; context [", cclutCtx, "]; condition [", cclutDisp, "]"))
    if (validate(cclut_test_runner::assertSuccess) = TRUE)
      set cclut_test_runner::assertSuccess  = FALSE
    endif
    go to exit_script
  endif

  return (cclutReturnValue)
end ;;;evaluateCondition


/**
  Deterimes if two values satisfy a specified operation as part an assert check. If not, routes execution to exit_script and
  sets cclut_test_runner::assertSuccess to FALSE if it exists.
  @param cclutLineNum
    The line number where the assert is being performed.
  @param cclutCtx
    The context provided with the assert.
  @param cclutComparison
    The operation such as '&lt;=' to perform on the values.
  @param cclutVal1
    The left hand value
  @param cclutVal2
    The right hand value
  @returns
    A boolean indicating whether comparison is TRUE, but only TRUE can be returned since control gets routed to
    exit_script if the comparison is FALSE.
*/
subroutine cclut::evaluateComparison(cclutLineNum, cclutCtx, cclutComparison, cclutVal1, cclutVal2)
  declare cclutReturnValue = i2 with protect
  declare cclutCondition = vc with protect

  if (validate(cclut_test_runner::assertSuccess, TRUE) = FALSE)
    ;;;an assert already failed without failing fast
    ;;;probably because the previous exit_script was not the outermost one.
    go to exit_script
  endif

  set cclutReturnValue = FALSE
  set cclutCondition = build2('"', cclutVal1, '" ', cclutComparison, ' "', cclutVal2, '"')
  if (cclutComparison = "=")
    if (
         cclutVal1 = cclutVal2
         and
         (textlen(cclutVal1) = textlen(cclutVal2) or cclut::useCclStringLogic)
       )
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = ">")
    if (
         cclutVal1 > cclutVal2
         or
         (cclutVal1 = cclutVal2 and textlen(cclutVal1) > textlen(cclutVal2) and not cclut::useCclStringLogic)
       )
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = ">=")
    if (
         cclutVal1 > cclutVal2
         or
         (cclutVal1 = cclutVal2 and (textlen(cclutVal1) >= textlen(cclutVal2) or cclut::useCclStringLogic))
       )
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = "<")
    if (
         cclutVal1 < cclutVal2
         or
         (cclutVal1 = cclutVal2 and textlen(cclutVal1) < textlen(cclutVal2) and not cclut::useCclStringLogic)
       )
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = "<=")
    if (
         cclutVal1 < cclutVal2
         or
         (cclutVal1 = cclutVal2 and (textlen(cclutVal1) <= textlen(cclutVal2) or cclut::useCclStringLogic))
       )
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = "!=")
    if (
         cclutVal1 != cclutVal2
         or
         (textlen(cclutVal1) != textlen(cclutVal2) and not cclut::useCclStringLogic)
       )
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = "contains")
    set cclutCondition = concat('findstring("', cclutVal1, '","', cclutVal2, '") > 0')
    if (findstring(cclutVal1, cclutVal2) > 0)
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = "not contains")
    set cclutCondition = concat('findstring("', cclutVal1, '","', cclutVal2, '") = 0')
    if (findstring(cclutVal1, cclutVal2) = 0)
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = "starts with")
    set cclutCondition = concat('"', cclutVal2, '" starts with "', cclutVal1, '"')
    if (
         cclutVal1 = substring(1, textlen(cclutVal1), cclutVal2)
         and
         (textlen(cclutVal1) <= textlen(cclutVal2) or cclut::useCclStringLogic)
       )
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = "not starts with")
    set cclutCondition = concat('"', cclutVal2, '" does not start with "', cclutVal1, '"')
    if (
         cclutVal1 != substring(1, textlen(cclutVal1), cclutVal2)
         or
         (cclutVal1 = cclutVal2 and textlen(cclutVal1) > textlen(cclutVal2) and not cclut::useCclStringLogic)
       )
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = "ends with")
    set cclutCondition = concat('"', cclutVal2, '" ends with "', cclutVal1, '"')
    if (
         cclutVal1 = substring(1+textlen(cclutVal2)-textlen(cclutVal1), textlen(cclutVal1), cclutVal2)
         and
         (textlen(cclutVal1) <= textlen(cclutVal2) or cclut::useCclStringLogic)
       )
      set cclutReturnValue = TRUE
    endif
  elseif (cclutComparison = "not ends with")
    set cclutCondition = concat('"', cclutVal2, '" does not end with "', cclutVal1, '"')
    if (
         cclutVal1 != substring(1+textlen(cclutVal2)-textlen(cclutVal1), textlen(cclutVal1), cclutVal2)
         or
         (cclutVal1 = cclutVal2 and textlen(cclutVal1) > textlen(cclutVal2) and not cclut::useCclStringLogic)
       )
      set cclutReturnValue = TRUE
    endif
  endif
  set cclutCondition = replace(cclutCondition, char(0), char(128))

  if ( cclutReturnValue = FALSE )
    call cclut::doOutput(build2(
      "FAILURE: line number [", trim(build(cclutLineNum),3), "]; context [", cclutCtx, "];, condition [", cclutCondition, "]"))
    if (validate(cclut_test_runner::assertSuccess) = TRUE)
      set cclut_test_runner::assertSuccess = FALSE
    endif
    go to exit_script
  endif
  return (cclutReturnValue)
end ;;;evaluateComparison

%#endif CCLUTASSERT_IMPL_NFF_INC
%#endif CCLUTASSERT_IMPL_FF_INC
