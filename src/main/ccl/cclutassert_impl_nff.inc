%#ifndef CCLUTASSERT_IMPL_NFF_INC
%#ifndef CCLUTASSERT_IMPL_FF_INC
%#def CCLUTASSERT_IMPL_NFF_INC
;**********************************************************************************************************************************
;
; Provides implementations of the evaluateCondition and evaluateComparison subroutines to support the cclutAssert*
; implementations from cclutassert_base_impl.inc. This implementation writes information to cclut_runResult which can
; be harvested by the calling program. This implementation does not fail fast. It continues executing tests even after it
; encounters a failed assert. (external non-fail-fast)
;
;**********************************************************************************************************************************


%i cclsource:cclutassert_impl_base.inc


declare cclut::reportAssert(cclutLineNum = i4, cclutCtx = vc, cclutCondition = vc, cclutSuccessFlag = i2,
    cclutECodeBefore = i4, cclutEMessageBefore = vc, cclutECode = i4, cclutEMessage = vc) = null

/**
  Evaluates whether a provided condition is true or false using call parser then invokes reportAssert
  to report the outcome. Used for checking an assert.

  @param cclutLineNum
    The line number where the assert occurs
  @param cclutCtx
    The context of the assert
  @param cclutCondition
    The condition to evaluate.
  @returns
    The boolean value from evaluating the condition.
*/
subroutine cclut::evaluateCondition (cclutLineNum, cclutCtx, cclutCondition)
  return (cclut::evaluateCondition2(cclutLineNum, cclutCtx, cclutCondition, cclutCondition))
end ;;;cclut::evaluateCondition

/**
  Evaluates whether a provided condition is true or false using call parser then invokes reportAssert
  to report the outcome. Used for checking an assert.

  @param cclutLineNum
    The line number where the assert occurs
  @param cclutCtx
    The context of the assert
  @param cclutCondition
    The condition to evaluate.
  @param cclutDisp
    The display for the condition, operator("hello world", "regexplike", "hell.*") for example.
  @returns
    The boolean value from evaluating the condition.
*/
subroutine cclut::evaluateCondition2 (cclutLineNum, cclutCtx, cclutCondition, cclutDisp)
  declare cclutReturnValue = i2 with protect
  declare cclutECodeBefore = i4 with protect
  declare cclutEMessageBefore = vc with protect
  declare cclutECode = i4 with protect
  declare cclutEMessage = vc with protect

  /* Clear the queue before running */
  set cclutECodeBefore = error(cclutEMessageBefore, 1)
  if (parser(cclutCondition))
    set cclutReturnValue = TRUE
  else
    set cclutReturnValue = FALSE
  endif

  call cclut::reportAssert(cclutLineNum, cclutCtx, cclutDisp,
      cclutReturnValue, cclutECodeBefore, cclutEMessageBefore, cclutECode, cclutEMessage)
  return (cclutReturnValue)
end ;;;evaluateCondition

/**
  Evaluates whether a pair of values satisfy a given binary comparison then invokes reportAssert to report the outcome.
  Used for checking an assert.

  @param cclutLineNum
    The line number where the assert occurs.
  @param cclutCtx
    The context of the assert.
  @param cclutComparison
    The comparison to evaluate.
  @param cclutVal1
    The left hand value.
  @param cclutVal2
    The right hand value.
*/
subroutine cclut::evaluateComparison(cclutLineNum, cclutCtx, cclutComparison, cclutVal1, cclutVal2)
  declare cclutReturnValue = i2 with protect
  declare cclutECodeBefore = i4 with protect
  declare cclutEMessageBefore = vc with protect
  declare cclutECode = i4 with protect
  declare cclutEMessage = vc with protect
  declare cclutCondition = vc with protect

  /* Clear the error queue before running */
  set cclutECodeBefore = error(cclutEMessageBefore, 1)
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
  call cclut::reportAssert(cclutLineNum, cclutCtx, cclutCondition,
                    cclutReturnValue, cclutECodeBefore, cclutEMessageBefore, cclutECode, cclutEMessage)
  return (cclutReturnValue)
end ;;;evaluateComparison

/**
  Stores the outcome of an assert into the cclut_runResult structure.
  @param cclutLineNum
    The line number where the assert is located.
  @param cclutCtx
    The context of the assert.
  @param cclutCondition
    The condition that was evaluated.
  @param cclutSuccessFlag
    The boolean value of the evaluated condition.
  @param cclutECodeBefore
    The error code that was registerd before the condition was evaluated; 0 if there was none.
  @param cclutEMsgBefore
    The error message that was registerd before the condition was evaluated; "" if there was none.
  @param cclutECode
    The error code that was generated while evaluating the condition.
  @param cclutEMsg
    The error message that was generated while evaluating the condition.
*/
subroutine cclut::reportAssert(cclutLineNum, cclutCtx, cclutCondition, cclutSuccessFlag,
    cclutECodeBefore, cclutEMessageBefore, cclutECode, cclutEMessage)
  declare cclutIdx = i4 with protect
  declare cclutStat = i4 with protect

  set cclutIdx = size(cclut_runResult->line, 5) + 1
  set cclutStat = alterlist(cclut_runResult->line, cclutIdx)

  set cclut_runResult->line[cclutIdx].lineNumber = cclutLineNum
  set cclut_runResult->line[cclutIdx].context = trim(cclutCtx, 3)
  set cclut_runResult->line[cclutIdx].result = evaluate(cclutSuccessFlag, FALSE, "FAIL", TRUE, "PASS", "UNKNOWN")
  set cclut_runResult->line[cclutIdx].datetime = format(cnvtdatetime(curdate, curtime3), "YYYY-MM-DD HH:MM:SS;;d")
  set cclut_runResult->line[cclutIdx].condition = trim(cclutCondition, 3)
  set cclut_runResult->line[cclutIdx].errorCodeBefore = cclutECodeBefore
  set cclut_runResult->line[cclutIdx].errorMessageBefore = trim(cclutEMessageBefore, 3)
  set cclut_runResult->line[cclutIdx].errorCode = cclutECode
  set cclut_runResult->line[cclutIdx].errorMessage = trim(cclutEMessage, 3)
end ;;;reportAssert

%#endif CCLUTASSERT_IMPL_FF_INC
%#endif CCLUTASSERT_IMPL_NFF_INC
