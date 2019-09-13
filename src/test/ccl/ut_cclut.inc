/**
  Unit Tests for the cclut script
*/

%i cclsource:cclut_get_file_as_string.inc

declare setup(null) = null with protect
declare testing::checkLines(lines = vc(ref)) = null with protect
declare testing::mergeCoverages(null) = null with protect
declare testNoIssues(null) = null with protect
declare testNoIssuesWithoutStructures(null) = null with protect
declare testNoIssuesCBO(null) = null with protect
declare testNoIssuesTestTwoAndThreeRBO(null) = null with protect
declare testFailTest(null) = null with protect
declare testSetupError(null) = null with protect
declare testSetupOnceErrorFailTest(null) = null with protect
declare testNoIssuesNotMuchSpecified(null) = null with protect
declare testtestCaseFileNameRequired(null) = null with protect
declare testFrameworkFailure(null) = null with protect
declare testNoTests(null) = null with protect
declare testAllTestsFail(null) = null with protect
declare testErroredTests(null) = null with protect
declare testTransferTestCaseResultsErrors(null) = null with protect
declare cclut1::transferTestCaseResultsError(source = vc(ref), target = vc(ref)) = null with protect
declare testGenerateResultsReportErrors(null) = null with protect
declare cclut1::generateResultsReportError(destination = vc, request = vc(ref), results = vc(ref)) = i2 with protect
declare testTransferAndGenerateErrors(null) = null with protect
declare testFailFastFailTest(null) = null with protect
declare testFailFastSetupError(null) = null with protect
declare testFailFastSetupOnceErrorFailTest(null) = null with protect
declare testFailFastAllTestsFail(null) = null with protect
declare testFailFastErroredTests(null) = null with protect

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

record ut_cclut_testCaseReply (
  1 environmentXml = vc
  1 listingXml = vc
  1 coverageXml = vc
  1 resultsXml = vc
  1 programs[*]
    2 programName = vc
    2 listingXml = vc
    2 coverageXml = vc
%i cclsource:status_block.inc
) with protect

declare testing::errorSetupOnce = i2 with protect, noconstant(FALSE)
declare testing::errorSetup = i2 with protect, noconstant(FALSE)
declare testing::errorTeardown = i2 with protect, noconstant(FALSE)
declare testing::errorTest = i2 with protect, noconstant(FALSE)
declare testing::errorTeardownOnce = i2 with protect, noconstant(FALSE)

declare testing::failSetupOnce = i2 with protect, noconstant(FALSE)
declare testing::failSetup = i2 with protect, noconstant(FALSE)
declare testing::failTeardown = i2 with protect, noconstant(FALSE)
declare testing::failTest = i2 with protect, noconstant(FALSE)
declare testing::failTeardownOnce = i2 with protect, noconstant(FALSE)

declare testing::NL = vc with protect, constant(char(10))

declare testing::ASSERT_TYPE_EQUAL = i4 with protect, constant(0)
declare testing::ASSERT_TYPE_DATE = i4 with protect, constant(1)
declare testing::ASSERT_TYPE_MATCH = i4 with protect, constant(2)
declare testing::ASSERT_TYPE_MERGE_NEXT = i4 with protect, constant(3)
  
record testing::outputLines (
  1 line[*]
    2 type = i4
    2 text = vc
) with protect

/**
  per-test initialization
*/
subroutine setup(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = initRec(ut_cclut_testCaseResults)
  set stat = initRec(ut_cclut_testCaseReply)
  set stat = initRec(testing::outputLines)

  set testing::errorSetupOnce = FALSE
  set testing::errorSetup = FALSE
  set testing::errorTeardown = FALSE
  set testing::errorTest = FALSE
  set testing::errorTeardownOnce = FALSE

  set testing::failSetupOnce = FALSE
  set testing::failSetup = FALSE
  set testing::failTeardown = FALSE
  set testing::failTest = FALSE
  set testing::failTeardownOnce = FALSE
end ;;;setup

subroutine testing::checkLines(lines)
  declare output = vc with protect, noconstant("")
  declare startPos = i4 with protect, noconstant(0)
  declare nullPos = i4 with protect, noconstant(0)
  declare lineNumberN = i4 with protect, noconstant(0)
  declare lineNumberStr = vc with protect, noconstant("")
  declare outputLineHead = vc with protect, noconstant("")
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
    if (textlen(trim(outputLineHead)) > 0)
      if (textlen(outputLineHead) < 116)
        set outputLine = concat(outputLineHead, " ", substring(5, textlen(outputLine)-4, outputLine))
      else
        set outputLine = concat(outputLineHead, substring(5, textlen(outputLine)-4, outputLine))
      endif
      set outputLineHead = ""
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
        call cclutAssertVCOperator(CURREF, build2("validate output line ", lineNumberStr), outputLine, "regexplike", expectedLine)
      of testing::ASSERT_TYPE_MERGE_NEXT:
        set outputLineHead = outputLine
    endcase
  endwhile
end ;;;

subroutine testing::mergeCoverages(null)
  declare stat = i4 with protect, noconstant(0)

  ;only collect code coverate if running from maven
  if (validate(cclut_reply) = FALSE)
    return (null)
  endif
  
  declare idx = i4 with protect, noconstant(0)
  declare programCount = i4 with protect, constant(size(ut_cclut_testCaseReply->programs,5))

  record mergecc_request (
    1 sourceXml = vc
    1 targetXml = vc
  ) with protect
  
  record mergecc_reply (
    1 mergedXml = vc
  ) with protect

  if (size(cclut_reply->programs, 5) = 0)
    set stat = alterlist(cclut_reply->programs, programCount)
    for (idx = 1 to programCount)  
      set cclut_reply->programs[idx].programName = ut_cclut_testCaseReply->programs[idx].programName
    endfor
  endif
  for (idx = 1 to programCount)
    set mergecc_request->sourceXml = ut_cclut_testCaseReply->programs[idx].coverageXml
    set mergecc_request->targetXml = cclut_reply->programs[idx].coverageXml
    execute cclut_merge_cc with replace("CCLUTREQUEST", mergecc_request), replace("CCLUTREPLY", mergecc_reply)
    set cclut_reply->programs[idx].coverageXml = mergecc_reply->mergedXml
  endfor
end ;;;mergeCoverages


/**
  Test the happy path with no optimizer mode specified
*/
subroutine testNoIssues(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           17-NOV-2017 04:17:17.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 5           failed: 0           errored: 0


Passed Tests
========================================================================================================================
SETUPONCE
TESTONE
TESTTWO
TESTTHREE
TEARDOWNONCE
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 15)
  set testing::outputLines->line[1].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
    ^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
    ^ ^
  set testing::outputLines->line[4].text = 
    ^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
    ^PASSED: 5           failed: 0           errored: 0^
  set testing::outputLines->line[6].text = 
    ^ ^
  set testing::outputLines->line[7].text = 
    ^ ^
  set testing::outputLines->line[8].text = 
    ^Passed Tests^
  set testing::outputLines->line[9].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[10].text = 
    ^SETUPONCE^
  set testing::outputLines->line[11].text = 
    ^TESTONE^
  set testing::outputLines->line[12].text = 
    ^TESTTWO^
  set testing::outputLines->line[13].text = 
    ^TESTTHREE^
  set testing::outputLines->line[14].text = 
    ^TEARDOWNONCE^
  set testing::outputLines->line[15].text = 
^************************************************************************************************************************^

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo" with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)
  call testing::checkLines(testing::outputLines)
end ;;;testNoIssues


/**
  Test the happy path without providing the request and reply structures
*/
subroutine testNoIssuesWithoutStructures(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 15)
  set testing::outputLines->line[1].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
    ^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
    ^ ^
  set testing::outputLines->line[4].text = 
    ^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
    ^PASSED: 5           failed: 0           errored: 0^
  set testing::outputLines->line[6].text = 
    ^ ^
  set testing::outputLines->line[7].text = 
    ^ ^
  set testing::outputLines->line[8].text = 
    ^Passed Tests^
  set testing::outputLines->line[9].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[10].text = 
    ^SETUPONCE^
  set testing::outputLines->line[11].text = 
    ^TESTONE^
  set testing::outputLines->line[12].text = 
    ^TESTTWO^
  set testing::outputLines->line[13].text = 
    ^TESTTHREE^
  set testing::outputLines->line[14].text = 
    ^TEARDOWNONCE^
  set testing::outputLines->line[15].text = 
^************************************************************************************************************************^

  set stat = copyrec(ut_cclut_testCaseReply, ut_cclut_testCaseReplyX, 1)
  
  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo" with
      replace("CCLUT1TESTCASERESULTS", test_case_result_not_provided),
      replace("CCLUT1TESTCASEREQUEST", test_case_request_not_provided), 
      replace("CCLUT1TESTCASEREPLY", cclut_reply_not_provided)      
  call testing::checkLines(testing::outputLines)
end ;;;testNoIssuesWithoutStructures


/**
  Test the happy path with optimizer mode CBO specified
*/
subroutine testNoIssuesCBO(null)
/*
========================================================================================================================
                                               CCL Unit Test Report - CBO                        17-NOV-2017 04:17:17.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 5           failed: 0           errored: 0


Passed Tests
========================================================================================================================
SETUPONCE
TESTONE
TESTTWO
TESTTHREE
TEARDOWNONCE
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 15)
  set testing::outputLines->line[1].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
    ^                                               CCL Unit Test Report - CBO                        ^
  set testing::outputLines->line[3].text = 
    ^ ^
  set testing::outputLines->line[4].text = 
    ^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
    ^PASSED: 5           failed: 0           errored: 0^
  set testing::outputLines->line[6].text = 
    ^ ^
  set testing::outputLines->line[7].text = 
    ^ ^
  set testing::outputLines->line[8].text = 
    ^Passed Tests^
  set testing::outputLines->line[9].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[10].text = 
    ^SETUPONCE^
  set testing::outputLines->line[11].text = 
    ^TESTONE^
  set testing::outputLines->line[12].text = 
    ^TESTTWO^
  set testing::outputLines->line[13].text = 
    ^TESTTHREE^
  set testing::outputLines->line[14].text = 
    ^TEARDOWNONCE^
  set testing::outputLines->line[15].text = 
^************************************************************************************************************************^

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "", "CBO" with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testNoIssuesCBO


/**
  Test the happy path with specific tests and optimizer mode RBO specified
*/
subroutine testNoIssuesTestTwoAndThreeRBO(null)
/*
========================================================================================================================
                                               CCL Unit Test Report - RBO                        17-NOV-2017 04:17:17.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 4           failed: 0           errored: 0


Passed Tests
========================================================================================================================
SETUPONCE
TESTTWO
TESTTHREE
TEARDOWNONCE
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 14)
  set testing::outputLines->line[1].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
    ^                                               CCL Unit Test Report - RBO                        ^
  set testing::outputLines->line[3].text = 
    ^ ^
  set testing::outputLines->line[4].text = 
    ^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
    ^PASSED: 4           failed: 0           errored: 0^
  set testing::outputLines->line[6].text = 
    ^ ^
  set testing::outputLines->line[7].text = 
    ^ ^
  set testing::outputLines->line[8].text = 
    ^Passed Tests^
  set testing::outputLines->line[9].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[10].text = 
    ^SETUPONCE^
  set testing::outputLines->line[11].text = 
    ^TESTTWO^
  set testing::outputLines->line[12].text = 
    ^TESTTHREE^
  set testing::outputLines->line[13].text = 
    ^TEARDOWNONCE^
  set testing::outputLines->line[14].text = 
^************************************************************************************************************************^

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(two|three)", "RBO" with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testNoIssuesCBO


/**
  Test a case where the test cases have assert failures
*/
subroutine testFailTest(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           17-NOV-2017 18:24:36.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 3           FAILED: 2           errored: 0


Failed Tests
========================================================================================================================
TESTONE
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    testOne failure

TESTTHREE
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    testThree failure A
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    testThree failure B


Passed Tests
========================================================================================================================
SETUPONCE
TESTTWO
TEARDOWNONCE
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 29)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^PASSED: 3           FAILED: 2           errored: 0^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Failed Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^TESTONE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[13].text = 
^    testOne failure^
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^TESTTHREE^
  set testing::outputLines->line[16].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[17].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[18].text = 
^    testThree failure A^
  set testing::outputLines->line[19].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[20].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[21].text = 
^    testThree failure B^
  set testing::outputLines->line[22].text = 
^ ^
  set testing::outputLines->line[23].text = 
^ ^
  set testing::outputLines->line[24].text = 
^Passed Tests^
  set testing::outputLines->line[25].text = 
^========================================================================================================================^
  set testing::outputLines->line[26].text = 
^SETUPONCE^
  set testing::outputLines->line[27].text = 
^TESTTWO^
  set testing::outputLines->line[28].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[29].text = 
^************************************************************************************************************************^

  set testing::failTest = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "", "", "", FALSE with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testTestsFail


/**
  Test a case where the setup routine throws errors
*/
subroutine testSetupError(null)
/*
^========================================================================================================================^
^                                                  CCL Unit Test Report                           ^
^ ^
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
^PASSED: 2           failed: 0           ERRORED: 3^
^ ^
^ ^
^Errored Tests^
^========================================================================================================================^
^TESTONE^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2175368(822,1)171118:0602{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^ ^
^TESTTWO^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2175368(822,1)171118:0602{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^ ^
^TESTTHREE^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2175368(822,1)171118:0602{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^ ^
^ ^
^Passed Tests^
^========================================================================================================================^
^SETUPONCE^
^TEARDOWNONCE^
^************************************************************************************************************************^
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 34)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^PASSED: 2           failed: 0           ERRORED: 3^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Errored Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^TESTONE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[13].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[13].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^TESTTWO^
  set testing::outputLines->line[16].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[17].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[18].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[18].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
  set testing::outputLines->line[19].text = 
^ ^
  set testing::outputLines->line[20].text = 
^TESTTHREE^
  set testing::outputLines->line[21].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[22].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[23].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[23].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
  set testing::outputLines->line[24].text = 
^ ^
  set testing::outputLines->line[25].text = 
^ ^
  set testing::outputLines->line[26].text = 
^Passed Tests^
  set testing::outputLines->line[27].text = 
^========================================================================================================================^
  set testing::outputLines->line[28].text = 
^SETUPONCE^
  set testing::outputLines->line[29].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[30].text = 
^************************************************************************************************************************^

  set testing::errorSetup = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "", "", "", FALSE with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testSetupError


/**
  Test a case where the setupOnce routine throws errors and the tests have assert failures
*/
subroutine testSetupOnceErrorFailTest(null)
/*
^========================================================================================================================^
^                                                  CCL Unit Test Report                           ^
^ ^
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
^PASSED: 2           FAILED: 2           ERRORED: 1^
^ ^
^ ^
^Errored Tests^
^========================================================================================================================^
^SETUPONCE^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2215992(802,1)171118:0609{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^ ^
^ ^
^Failed Tests^
^========================================================================================================================^
^TESTONE^
^------------------------------------------------------------------------------------------------------------------------^
^    Assert Failure: 0 = 1^
^    testOne failure^
^ ^
^TESTTHREE^
^------------------------------------------------------------------------------------------------------------------------^
^    Assert Failure: 0 = 1^
^    testThree failure A^
^------------------------------------------------------------------------------------------------------------------------^
^    Assert Failure: 0 = 1^
^    testThree failure B^
^ ^
^ ^
^Passed Tests^
^========================================================================================================================^
^TESTTWO^
^TEARDOWNONCE^
^************************************************************************************************************************^
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 36)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^PASSED: 2           FAILED: 2           ERRORED: 1^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Errored Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^SETUPONCE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[13].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[13].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
^    EDERROR) to incompatible type.^
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^ ^
  set testing::outputLines->line[16].text = 
^Failed Tests^
  set testing::outputLines->line[17].text = 
^========================================================================================================================^
  set testing::outputLines->line[18].text = 
^TESTONE^
  set testing::outputLines->line[19].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[20].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[21].text = 
^    testOne failure^
  set testing::outputLines->line[22].text = 
^ ^
  set testing::outputLines->line[23].text = 
^TESTTHREE^
  set testing::outputLines->line[24].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[25].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[26].text = 
^    testThree failure A^
  set testing::outputLines->line[27].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[28].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[29].text = 
^    testThree failure B^
  set testing::outputLines->line[30].text = 
^ ^
  set testing::outputLines->line[31].text = 
^ ^
  set testing::outputLines->line[32].text = 
^Passed Tests^
  set testing::outputLines->line[33].text = 
^========================================================================================================================^
  set testing::outputLines->line[34].text = 
^TESTTWO^
  set testing::outputLines->line[35].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[36].text = 
^************************************************************************************************************************^

  set testing::errorSetupOnce = TRUE
  set testing::failTest = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "", "", "", FALSE with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testSetupOnceErrorFailTest

/**
  Test the happy path with nothing specified but the test case.
*/
subroutine testNoIssuesNotMuchSpecified(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           17-NOV-2017 04:17:17.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 5           failed: 0           errored: 0


Passed Tests
========================================================================================================================
SETUPONCE
TESTONE
TESTTWO
TESTTHREE
TEARDOWNONCE
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 15)
  set testing::outputLines->line[1].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
    ^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
    ^ ^
  set testing::outputLines->line[4].text = 
    ^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
    ^PASSED: 5           failed: 0           errored: 0^
  set testing::outputLines->line[6].text = 
    ^ ^
  set testing::outputLines->line[7].text = 
    ^ ^
  set testing::outputLines->line[8].text = 
    ^Passed Tests^
  set testing::outputLines->line[9].text = 
    ^========================================================================================================================^
  set testing::outputLines->line[10].text = 
    ^SETUPONCE^
  set testing::outputLines->line[11].text = 
    ^TESTONE^
  set testing::outputLines->line[12].text = 
    ^TESTTWO^
  set testing::outputLines->line[13].text = 
    ^TESTTHREE^
  set testing::outputLines->line[14].text = 
    ^TEARDOWNONCE^
  set testing::outputLines->line[15].text = 
^************************************************************************************************************************^

  execute cclut "ut_cclut_output", "", "ut_cclut_suo_su_td_tdo", "", "", "" with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testNoIssuesNotMuchSpecified


/**
  Test that a test case name must be provided.
*/
subroutine testtestCaseFileNameRequired(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           20-NOV-2017 17:59:38.00

Test Case:
========================================================================================================================
    A test case file name is required
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 7)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                               CCL Unit Test Error Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:^
  set testing::outputLines->line[5].text = 
^========================================================================================================================^
  set testing::outputLines->line[6].text = 
^    A test case file name is required^
  set testing::outputLines->line[7].text = 
^************************************************************************************************************************^

  execute cclut "ut_cclut_output", "", "" with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testtestCaseFileNameRequired



/**
  Test a framework failure.
*/
subroutine testFrameworkFailure(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           20-NOV-2017 18:06:34.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
========================================================================================================================
    Execution Failed: %CCL-E-260-PRG_7081391_6518947(1771,1)171120:1806{PREPLYSTRUCT->ERRORS}Found an invalid record
     structure member in expr.
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)
  declare msg = vc with protect, noconstant("")
  declare expectedErrorPattern = vc with protect, noconstant("")

  set stat = alterlist(testing::outputLines->line, 8)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                               CCL Unit Test Error Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^========================================================================================================================^
  set testing::outputLines->line[6].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[7].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[7].text = 
    concat("    Execution Failed: %CCL-E-260-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{NON_TESTCASERESULTS->RESULTIND\}",
        ^Found an invalid record structure member in [eE]xpr.^)
  set testing::outputLines->line [8].text =     
^************************************************************************************************************************^

  record non_testCaseResults (
%i cclsource:status_block.inc
  ) with protect
  
  set stat = copyrec(ut_cclut_testCaseReply, ut_cclut_testCaseReplyX, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo" with
      replace("CCLUT1TESTCASERESULTS", non_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_requestX), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReplyX)

  set expectedErrorPattern = concat("%CCL-E-260-PRG_[0-9]+_[0-9]+\([^)]+\)",
      "[0-9]+:[0-9]+\{NON_TESTCASERESULTS->RESULTIND\}Found an invalid record structure member in [eE]xpr.")

  ;The error gets thrown multiple times.
  ;Need to flush the error queue so it does not get attributed to this unit test.
  set stat = error(msg, 1)
  while (stat > 0)
    call cclutAssertVcOperator(CURREF, "flushing the error queue", msg, "regexplike", expectedErrorPattern)
    set stat = error(msg, 0)
  endwhile
  
  call cclutAssertVcEqual(CURREF, "framework failure status", ut_cclut_testCaseReplyX->status_data.status, "F")
  call cclutAssertVcEqual(CURREF, "framework failure operationName", 
      ut_cclut_testCaseReplyX->status_data.subeventstatus[1].operationName, "pre-executeTestLogic-1   ")
  call cclutAssertVcEqual(CURREF, "framework failure operationStatus", 
      ut_cclut_testCaseReplyX->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVcEqual(CURREF, "framework failure targetObjectName", 
      ut_cclut_testCaseReplyX->status_data.subeventstatus[1].targetObjectName, "ut_cclut_suo_su_td_tdo   ")
  call cclutAssertVcOperator(CURREF, "framework failure targetObjectValue", 
    ut_cclut_testCaseReplyX->status_data.subeventstatus[1].targetObjectValue, 
    "regexplike", concat("Execution Failed: ", expectedErrorPattern))

  call testing::mergeCoverages(null)
  call testing::checkLines(testing::outputLines)
end ;;;testFrameworkFailure


/**
  Test no tests
*/
subroutine testNoTests(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           20-NOV-2017 19:03:51.00   
Test Case: cclsource:ut_cclut_no_tests
========================================================================================================================
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 8)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                               CCL Unit Test Error Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_no_tests^
  set testing::outputLines->line[5].text = 
^========================================================================================================================^
  set testing::outputLines->line[6].text =
^    No tests qualified to be executed^     
  set testing::outputLines->line[7].text = 
^************************************************************************************************************************^

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_no_tests" with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)
  call testing::checkLines(testing::outputLines)
end ;;;testNoTests


/**
  Test no tests pass
*/
subroutine testAllTestsFail(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           17-NOV-2017 18:24:36.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 3           FAILED: 2           errored: 0


Failed Tests
========================================================================================================================
SETUPONCE
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    setupOnce failure

TESTONE
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    testOne failure

TESTTHREE
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    testThree failure A
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    testThree failure B

TEARDOWNONCE
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    teardownOnce failure


Passed Tests
========================================================================================================================
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 33)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^passed: 0           FAILED: 4           errored: 0^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Failed Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^SETUPONCE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[13].text = 
^    setupOnce failure^
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^TESTONE^
  set testing::outputLines->line[16].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[17].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[18].text = 
^    testOne failure^
  set testing::outputLines->line[19].text = 
^ ^
  set testing::outputLines->line[20].text = 
^TESTTHREE^
  set testing::outputLines->line[21].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[22].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[23].text = 
^    testThree failure A^
  set testing::outputLines->line[24].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[25].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[26].text = 
^    testThree failure B^
  set testing::outputLines->line[27].text = 
^ ^
  set testing::outputLines->line[28].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[29].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[30].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[31].text = 
^    teardownOnce failure^
  set testing::outputLines->line[32].text = 
^ ^
  set testing::outputLines->line[33].text = 
^************************************************************************************************************************^

  set testing::failSetupOnce = TRUE
  set testing::failTest = TRUE
  set testing::failTeardownOnce = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|three)", "", "", FALSE with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testAllTestsFail

/**
  Test test error out.
*/
subroutine testErroredTests(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           17-NOV-2017 18:24:36.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 3           failed: 0           ERRORED: 2


Errored Tests
========================================================================================================================
^TESTONE^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2175368(822,1)171118:0602{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^ ^
^TESTTHREE^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2175368(822,1)171118:0602{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2175368(822,1)171118:0602{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^ ^
Passed Tests
========================================================================================================================
SETUPONCE
TESTTWO
TEARDOWNONCE
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 28)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^PASSED: 2           failed: 0           ERRORED: 2^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Errored Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^TESTONE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[13].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[13].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^TESTTHREE^
  set testing::outputLines->line[16].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[17].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[18].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[18].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
  set testing::outputLines->line[19].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[20].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[21].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[21].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
  set testing::outputLines->line[22].text = 
^ ^
  set testing::outputLines->line[23].text = 
^ ^
  set testing::outputLines->line[24].text = 
^Passed Tests^
  set testing::outputLines->line[25].text = 
^========================================================================================================================^
  set testing::outputLines->line[26].text = 
^SETUPONCE^
  set testing::outputLines->line[27].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[28].text = 
^************************************************************************************************************************^

  set testing::errorTest = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|three)", "", "", FALSE  with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testErroredTests


/**
  Test cclut errors inside transferTestCaseResults
*/
subroutine testTransferTestCaseResultsErrors(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           20-NOV-2017 22:21:01.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
passed: 0           failed: 0           ERRORED: 1


Errored Tests
========================================================================================================================
TESTCCLUTERRORS
------------------------------------------------------------------------------------------------------------------------
    Error: %CCL-E-94-CCLUT(2100,1)171130:0642{FORCED1}Assignment of Report expression (FORCED1) to incompatible type.
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 7)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                               CCL Unit Test Error Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^========================================================================================================================^
  set testing::outputLines->line[6].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[6].text = 
"    %CCL-E-94-CCLUT\([^)]+\)[0-9]+:[0-9]+\{FORCED1\}Assignment of Report expression \(FORCED1\) to incompatible type."
  set testing::outputLines->line[7].text = 
^************************************************************************************************************************^

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|three)" with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply),
      replace("TRANSFERTESTCASERESULTS", transferTestCaseResultsError)
  call testing::mergeCoverages(null)
  call testing::checkLines(testing::outputLines)
end 
subroutine cclut1::transferTestCaseResultsError(source, target)
  declare forced1 = i4 with protect, noconstant(0)
  set forced1 = "this is a forced error"
end ;;;testTransferTestCaseResultsErrors


/**
  Test cclut errors inside generateResultsReport
*/
subroutine testGenerateResultsReportErrors(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           20-NOV-2017 22:21:01.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
passed: 0           failed: 0           ERRORED: 1


Errored Tests
========================================================================================================================
TESTCCLUTERRORS
------------------------------------------------------------------------------------------------------------------------
    Error: %CCL-E-94-CCLUT(2100,1)171130:0642{FORCED2}Assignment of Report expression (FORCED2) to incompatible type.
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 7)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                               CCL Unit Test Error Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^========================================================================================================================^
  set testing::outputLines->line[6].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[6].text = 
"    %CCL-E-94-CCLUT\([^)]+\)[0-9]+:[0-9]+\{FORCED2\}Assignment of Report expression \(FORCED2\) to incompatible type."
  set testing::outputLines->line[7].text = 
^************************************************************************************************************************^

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|three)" with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply),
      replace("GENERATERESULTSREPORT", generateResultsReportError)
  call testing::mergeCoverages(null)
  call testing::checkLines(testing::outputLines)
end 
subroutine cclut1::generateResultsReportError(destination, request, results)
  declare forced2 = i4 with protect, noconstant(0)
  set forced2 = "this is a forced error"
end ;;;testGenerateResultsReportErrors


/**
  Test cclut errors inside both transferTestCaseResults and generateResultsReport
*/
subroutine testTransferAndGenerateErrors(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           20-NOV-2017 22:21:01.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
passed: 0           failed: 0           ERRORED: 1


Errored Tests
========================================================================================================================
TESTCCLUTERRORS
------------------------------------------------------------------------------------------------------------------------
    Error: %CCL-E-94-CCLUT(2100,1)171130:0642{FORCED2}Assignment of Report expression (FORCED2) to incompatible type.
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 7)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                               CCL Unit Test Error Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^========================================================================================================================^
  set testing::outputLines->line[6].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[6].text = 
"    %CCL-E-94-CCLUT\([^)]+\)[0-9]+:[0-9]+\{FORCED1\}Assignment of Report expression \(FORCED1\) to incompatible type."
  set testing::outputLines->line[7].text = 
^************************************************************************************************************************^

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|three)" with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply),
      replace("TRANSFERTESTCASERESULTS", transferTestCaseResultsError),
      replace("GENERATERESULTSREPORT", generateResultsReportError)
  call testing::mergeCoverages(null)
  call testing::checkLines(testing::outputLines)
end ;;;testTransferAndGenerateErrors



/**
  Test a case where the test cases have assert failures
*/
subroutine testFailFastFailTest(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           17-NOV-2017 18:24:36.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 2           FAILED: 1           errored: 0


Failed Tests
========================================================================================================================
TESTONE
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    testOne failure


Passed Tests
========================================================================================================================
SETUPONCE
TEARDOWNONCE
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 29)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^PASSED: 2           FAILED: 1           errored: 0^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Failed Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^TESTONE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[13].text = 
^    testOne failure^
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^ ^
  set testing::outputLines->line[16].text = 
^Passed Tests^
  set testing::outputLines->line[17].text = 
^========================================================================================================================^
  set testing::outputLines->line[18].text = 
^SETUPONCE^
  set testing::outputLines->line[19].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[20].text = 
^************************************************************************************************************************^

  set testing::failTest = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "", "", "", TRUE with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testFailFastFailTest


/**
  Test a case where the setup routine throws errors
*/
subroutine testFailFastSetupError(null)
/*
^========================================================================================================================^
^                                                  CCL Unit Test Report                           ^
^ ^
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
^PASSED: 2           failed: 0           ERRORED: 1^
^ ^
^ ^
^Errored Tests^
^========================================================================================================================^
^TESTONE^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2175368(822,1)171118:0602{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^ ^
^ ^
^Passed Tests^
^========================================================================================================================^
^SETUPONCE^
^TEARDOWNONCE^
^************************************************************************************************************************^
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 34)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^PASSED: 2           failed: 0           ERRORED: 1^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Errored Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^TESTONE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[13].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[13].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^ ^
  set testing::outputLines->line[16].text = 
^Passed Tests^
  set testing::outputLines->line[17].text = 
^========================================================================================================================^
  set testing::outputLines->line[18].text = 
^SETUPONCE^
  set testing::outputLines->line[19].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[20].text = 
^************************************************************************************************************************^

  set testing::errorSetup = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "", "", "", TRUE with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testFailFastSetupError


/**
  Test a case where the setupOnce routine throws errors and the tests have assert failures
*/
subroutine testFailFastSetupOnceErrorFailTest(null)
/*
^========================================================================================================================^
^                                                  CCL Unit Test Report                           ^
^ ^
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
^PASSED: 1           FAILED: 0           ERRORED: 1^
^ ^
^ ^
^Errored Tests^
^========================================================================================================================^
^SETUPONCE^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2215992(802,1)171118:0609{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^ ^
^ ^
^Failed Tests^
^========================================================================================================================^
^ ^
^ ^
^Passed Tests^
^========================================================================================================================^
^TEARDOWNONCE^
^************************************************************************************************************************^
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 36)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^PASSED: 1           failed: 0           ERRORED: 1^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Errored Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^SETUPONCE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[13].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[13].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
^    EDERROR) to incompatible type.^
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^ ^
  set testing::outputLines->line[16].text = 
^Passed Tests^
  set testing::outputLines->line[17].text = 
^========================================================================================================================^
  set testing::outputLines->line[18].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[19].text = 
^************************************************************************************************************************^

  set testing::errorSetupOnce = TRUE
  set testing::failTest = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "", "", "", TRUE with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testFailFastSetupOnceErrorFailTest


/**
  Test FailFast when no tests pass
*/
subroutine testFailFastAllTestsFail(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           17-NOV-2017 18:24:36.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 0           FAILED: 2           errored: 0


Failed Tests
========================================================================================================================
SETUPONCE
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    setupOnce failure

TEARDOWNONCE
------------------------------------------------------------------------------------------------------------------------
    Assert Failure: 0 = 1
    teardownOnce failure


Passed Tests
========================================================================================================================
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 33)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^passed: 0           FAILED: 2           errored: 0^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Failed Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^SETUPONCE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[13].text = 
^    setupOnce failure^
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[16].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[17].text = 
^    Assert Failure: 0 = 1^
  set testing::outputLines->line[18].text = 
^    teardownOnce failure^
  set testing::outputLines->line[19].text = 
^ ^
  set testing::outputLines->line[20].text = 
^************************************************************************************************************************^

  set testing::failSetupOnce = TRUE
  set testing::failTest = TRUE
  set testing::failTeardownOnce = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "", "", "", TRUE with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testFailFastAllTestsFail

/**
  Test fail fast when tests error out.
*/
subroutine testFailFastErroredTests(null)
/*
========================================================================================================================
                                                  CCL Unit Test Report                           17-NOV-2017 18:24:36.00

Test Case: cclsource:ut_cclut_suo_su_td_tdo
PASSED: 2           failed: 0           ERRORED: 1


Errored Tests
========================================================================================================================
^TESTONE^
^------------------------------------------------------------------------------------------------------------------------^
^    Error: %CCL-E-94-PRG_7073463_2175368(822,1)171118:0602{GUARANTEEDERROR}Assignment of Report expression (GUARANTE^
^    EDERROR) to incompatible type.^
^ ^
Passed Tests
========================================================================================================================
SETUPONCE
TEARDOWNONCE
************************************************************************************************************************
*/
  declare stat = i4 with protect, noconstant(0)

  set stat = alterlist(testing::outputLines->line, 28)
  set testing::outputLines->line[1].text = 
^========================================================================================================================^
  set testing::outputLines->line[2].type = testing::ASSERT_TYPE_DATE
  set testing::outputLines->line[2].text = 
^                                                  CCL Unit Test Report                           ^
  set testing::outputLines->line[3].text = 
^ ^
  set testing::outputLines->line[4].text = 
^Test Case: cclsource:ut_cclut_suo_su_td_tdo^
  set testing::outputLines->line[5].text = 
^PASSED: 2           failed: 0           ERRORED: 1^
  set testing::outputLines->line[6].text = 
^ ^
  set testing::outputLines->line[7].text = 
^ ^
  set testing::outputLines->line[8].text = 
^Errored Tests^
  set testing::outputLines->line[9].text = 
^========================================================================================================================^
  set testing::outputLines->line[10].text = 
^TESTONE^
  set testing::outputLines->line[11].text = 
^------------------------------------------------------------------------------------------------------------------------^
  set testing::outputLines->line[12].type = testing::ASSERT_TYPE_MERGE_NEXT
  set testing::outputLines->line[13].type = testing::ASSERT_TYPE_MATCH
  set testing::outputLines->line[13].text = concat(
    "    Error: %CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}",
    ^Assignment of Report expression \(GUARANTEEDERROR) to incompatible type.^)
  set testing::outputLines->line[14].text = 
^ ^
  set testing::outputLines->line[15].text = 
^ ^
  set testing::outputLines->line[16].text = 
^Passed Tests^
  set testing::outputLines->line[17].text = 
^========================================================================================================================^
  set testing::outputLines->line[18].text = 
^SETUPONCE^
  set testing::outputLines->line[19].text = 
^TEARDOWNONCE^
  set testing::outputLines->line[20].text = 
^************************************************************************************************************************^

  set testing::errorTest = TRUE

  execute cclut "ut_cclut_output", "cclsource", "ut_cclut_suo_su_td_tdo", "(one|two|three)", "", "", TRUE with
      replace("CCLUT1TESTCASERESULTS", ut_cclut_testCaseResults),
      replace("CCLUT1TESTCASEREQUEST", cclut_request), 
      replace("CCLUT1TESTCASEREPLY", ut_cclut_testCaseReply)
  call testing::mergeCoverages(null)      
  call testing::checkLines(testing::outputLines)
end ;;;testFailFastErroredTests


