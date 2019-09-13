/**
  Unit Tests for cclut_execute_test_case_file.
*/

%i cclsource:cclut_xml_functions.inc
%i cclsource:cclut_code_coverage.inc


declare testNoIssues(null) = null with protect
declare testNoIssuesCBO(null) = null with protect
declare testNoIssuesRBO(null) = null with protect
declare testNoIssuesInvalidOM(null) = null with protect
declare testErrorsEverywhere(null) = null with protect
declare testFailsAndErrorsEverywhere(null) = null with protect
declare testFailsEverywhere(null) = null with protect
declare testFailsTestsOnly(null) = null with protect
declare testSetupErrors(null) = null with protect
declare testLoadingErrors(null) = null with protect
declare testTestCaseFileDoesNotExist(null) = null with protect
declare testCclMinimumNotMet(null) = null with protect
declare cclut::getOutdatedCclVersion(null) = vc with protect
declare testCompileProgramFailure(null) = null with protect
declare public::compileProgramFailure(fileDir = vc, fileName = vc, listingDir = vc, listingName = vc, errMsg = vc(ref)) = i2
declare testGenerateTestCaseProgramFailure(null) = null with protect
declare public::generateTestCaseProgramFailure(fileDir = vc, fileName = vc, listingDir = vc, listingName = vc, 
    objName = vc, errorMessage = vc(ref)) = i2
declare testCheckPrgFailure(null) = null with protect
declare checkPrgFailure(prgName = vc) = i4 with protect
declare testPostExecutionFailure(null) = null with protect
declare mockRemove(fileLocation = vc) = i4 with protect
declare testErrorThenExitScript(null) = null with protect
declare testCoverageExcludes(null) = null with protect
declare testMockTableCleanup(null) = null with protect
declare testDefaultsNotFailFast(null) = null with protect
declare checkFailFast(checkFunction = vc) = null with protect
declare testFailFastSetupOnceFailure(null) = null with protect
declare testFailFastSetupFailure(null) = null with protect
declare testFailFastTestFailure(null) = null with protect
declare testFailFastTeardownFailure(null) = null with protect
declare testFailFastSetupOnceError(null) = null with protect
declare testFailFastSetupError(null) = null with protect
declare testFailFastTestError(null) = null with protect
declare testFailFastTeardownError(null) = null with protect
declare testFailFastTeardownOnce(null) = null with protect

declare checkFailFastSetupOnceFailure(null) = null with protect
declare checkFailFastSetupFailure(null) = null with protect
declare checkFailFastTestFailure(null) = null with protect
declare checkFailFastTeardownFailure(null) = null with protect
declare checkFailFastSetupOnceError(null) = null with protect
declare checkFailFastSetupError(null) = null with protect
declare checkFailFastTestError(null) = null with protect
declare checkFailFastTeardownError(null) = null with protect
declare checkFailFastTeardownOnce(null) = null with protect

%i cclsource:ut_cclut_execute_tc_comm.inc

declare testing::sut = vc with protect, constant("cclut_execute_test_case_file")

record ucetcf_request (
  1 testCaseDirectory = vc
  1 testCaseFileName = vc
  1 testNamePattern = vc
  1 programs[*]
    2 programName = vc
    2 compile = i2
  1 optimizerMode = vc
  1 enforcePredeclare = i2
  1 deprecatedFlag = vc
  1 legacyResultsFormat = i2
  1 failFast = i2
) with protect

record ucetcf_reply (
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



/**
  Run ut_cclut_suo_su_td_tdo and validate the results when no issues occur.
*/
subroutine testNoIssues(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = copyRec(ucetcf_request, ucetcf_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  if (validate(cclut_request) = TRUE)
    ;to capture code coverage if running from maven
    set stat = moverec(cclut_request->programs, ucetcf_request1->programs)
    if (size(cclut_request->programs, 5) > 0)
      set cclut_request->programs[1].compile = TRUE
    endif
  endif
  set ucetcf_request1->enforcePredeclare = TRUE

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request1),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")
  call validateNoIssuesResponse(ucetcf_testCaseResults)

  call validateEnvironmentXml(ucetcf_reply->environmentXml)
  call validateListingXml(ucetcf_reply->listingXml)
  call validateResultsXml(ucetcf_reply->resultsXml, FALSE)
  call validateCoverageXml(ucetcf_reply->coverageXml)
end ;;;testNoIssues


/**
  Run ut_cclut_suo_su_td_tdo and validate the results with optimizer mode CBO when no issues occur.
*/
subroutine testNoIssuesCBO(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = copyRec(ucetcf_request, ucetcf_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  set ucetcf_request1->optimizerMode = "CBO"

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request1),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")
  call validateNoIssuesResponse(ucetcf_testCaseResults)
end ;;;testNoIssuesCBO

/**
  Run ut_cclut_suo_su_td_tdo and validate the results with optimizer mode RBO when no issues occur.
*/
subroutine testNoIssuesRBO(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = copyRec(ucetcf_request, ucetcf_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  set ucetcf_request1->optimizerMode = "RBO"

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request1),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")
  call validateNoIssuesResponse(ucetcf_testCaseResults)
end ;;;testNoIssuesRBO


/**
  Run ut_cclut_suo_su_td_tdo and validate the results with an invalid optimizer mode when no issues occur.
*/
subroutine testNoIssuesInvalidOM(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = copyRec(ucetcf_request, ucetcf_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  set ucetcf_request1->optimizerMode = "invalid"

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request1),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success 5",
      ucetcf_reply->status_data.subeventstatus[1].targetObjectValue, "Invalid optimizer mode specified.")
end ;;;testNoIssuesInvalidOM


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when all subroutines produce an error.
*/
subroutine testErrorsEverywhere(null)
  declare testCount = i4 with protect, noconstant(0)
  declare errorIdx = i4 with protect, noconstant(0)

  set testing::errorSetupOnce = TRUE
  set testing::errorSetup = TRUE
  set testing::errorTest = TRUE
  set testing::errorTeardown = TRUE
  set testing::errorTeardownOnce = TRUE

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")

  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)

  set testCount = size(ucetcf_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[1].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[1].asserts[1].condition, '"SO" = "SO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[1].asserts[2].context, "setupOnce error")
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[1].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText",
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetcf_testCaseResults->tests[1].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[2].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 3)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[2].context, "setup error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[2].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[3].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[4].context, "testOne error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[4].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[5].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[6].context, "teardown error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[6].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[6].resultInd, TRUE)

  for (errorIdx = 1 to 3)
    call cclutAssertStartsWith(CURREF, "errorText",
        "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[2].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText",
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetcf_testCaseResults->tests[2].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[3].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[3].errors, 5), 2)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[2].context, "setup error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[2].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[4].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[4].condition, '"A" = "A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[5].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[6].context, "teardown error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[6].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[6].resultInd, TRUE)
  for (errorIdx = 1 to 2)
    call cclutAssertStartsWith(CURREF, "errorText",
        "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[3].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText",
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetcf_testCaseResults->tests[3].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[4].asserts, 5), 5)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[4].errors, 5), 4)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[2].context, "setup error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[2].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[3].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[4].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[5].context, "teardown error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[5].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[5].resultInd, TRUE)
  for (errorIdx = 1 to 4)
    call cclutAssertStartsWith(CURREF, "errorText",
        "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[4].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText",
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetcf_testCaseResults->tests[4].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[5].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[5].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[5].asserts[1].condition, '"TO" = "TO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[5].asserts[2].context, "teardownOnce error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[5].asserts[2].condition, '1 = 1')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[5].asserts[2].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[4].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText",
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetcf_testCaseResults->tests[4].errors[1].errorText)
end ;;;testErrorsEverywhere


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when all subroutines produce a failed assert and an error.
*/
subroutine testFailsAndErrorsEverywhere(null)
  declare testCount = i4 with protect, noconstant(0)
  declare errorIdx = i4 with protect, noconstant(0)

  set testing::errorSetupOnce = TRUE
  set testing::errorSetup = TRUE
  set testing::errorTest = TRUE
  set testing::errorTeardown = TRUE
  set testing::errorTeardownOnce = TRUE

  set testing::failSetupOnce = TRUE
  set testing::failSetup = TRUE
  set testing::failTest = TRUE
  set testing::failTeardown = TRUE
  set testing::failTeardownOnce = TRUE

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")

  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)

  set testCount = size(ucetcf_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[1].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[1].asserts[1].condition, '"SO" = "SO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[1].asserts[2].context, "setupOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[1].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[1].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[1].asserts[3].context, "setupOnce error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[1].asserts[3].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[1].asserts[3].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[1].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText",
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetcf_testCaseResults->tests[1].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[2].asserts, 5), 9)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 3)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[3].context, "setup error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[3].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[4].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[4].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[5].context, "testOne failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[5].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[5].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[6].context, "testOne error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[6].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[6].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[7].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[7].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[7].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[8].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[8].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[8].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[9].context, "teardown error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[9].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[9].resultInd, TRUE)
  for (errorIdx = 1 to 3)
    call cclutAssertStartsWith(CURREF, "errorText",
        "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[2].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText",
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetcf_testCaseResults->tests[2].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[3].asserts, 5), 8)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[3].errors, 5), 2)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[3].context, "setup error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[3].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[4].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[4].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[5].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[5].condition, '"A" = "A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[6].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[6].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[6].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[7].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[7].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[7].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[8].context, "teardown error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[8].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[8].resultInd, TRUE)
  for (errorIdx = 1 to 2)
    call cclutAssertStartsWith(CURREF, "errorText",
        "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[3].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText",
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetcf_testCaseResults->tests[3].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[4].asserts, 5), 9)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[4].errors, 5), 3)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[3].context, "setup error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[3].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[4].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[4].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[5].context, "testThree failure A")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[5].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[5].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[6].context, "testThree failure B")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[6].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[6].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[7].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[7].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[7].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[8].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[8].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[8].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[9].context, "teardown error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[9].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[9].resultInd, TRUE)
  for (errorIdx = 1 to 3)
    call cclutAssertStartsWith(CURREF, "errorText",
        "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[4].errors[errorIdx].errorText)
    call cclutAssertContains(CURREF, "errorText",
        "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
        ucetcf_testCaseResults->tests[4].errors[errorIdx].errorText)
  endfor

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[5].asserts, 5), 3)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[5].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[5].asserts[1].condition, '"TO" = "TO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[5].asserts[2].context, "teardownOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[5].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[5].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[5].asserts[3].context, "teardownOnce error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[5].asserts[3].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[5].asserts[3].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText", "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[5].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText",
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetcf_testCaseResults->tests[5].errors[1].errorText)
end ;;;testFailsAndErrorsEverywhere


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when all subroutines produce a failed assert but no errors.
*/
subroutine testFailsEverywhere(null)
  declare testCount = i4 with protect, noconstant(0)

  set testing::failSetupOnce = TRUE
  set testing::failSetup = TRUE
  set testing::failTest = TRUE
  set testing::failTeardown = TRUE
  set testing::failTeardownOnce = TRUE

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")

  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)

  set testCount = size(ucetcf_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[1].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[1].asserts[1].condition, '"SO" = "SO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[1].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[1].asserts[2].context, "setupOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[1].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[1].asserts[2].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[2].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[3].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[4].context, "testOne failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[4].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[5].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[6].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[6].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[3].asserts, 5), 6)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[3].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[4].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[4].condition, '"A" = "A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[5].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[5].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[6].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[6].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[6].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[4].asserts, 5), 7)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[4].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[2].context, "setup failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[2].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[3].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[4].context, "testThree failure A")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[4].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[5].context, "testThree failure B")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[5].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[5].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[6].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[6].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[6].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[7].context, "teardown failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[7].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[7].resultInd, FALSE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[5].asserts, 5), 2)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[5].asserts[1].condition, '"TO" = "TO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[5].asserts[2].context, "teardownOnce failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[5].asserts[2].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[5].asserts[2].resultInd, FALSE)
end ;;;testFailsEverywhere


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when the test subroutines produce a failed assert
  but there are no other problems.
*/
subroutine testFailsTestsOnly(null)
  declare testCount = i4 with protect, noconstant(0)

  set testing::failTest = TRUE

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")

  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)

  set testCount = size(ucetcf_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[1].asserts[1].condition, '"SO" = "SO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[1].asserts[1].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[2].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[2].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[2].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[3].context, "testOne failure")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[3].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[3].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[4].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[4].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[3].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[3].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[2].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[2].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[3].condition, '"A" = "A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[4].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[4].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[4].asserts, 5), 5)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[4].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[2].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[2].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[3].context, "testThree failure A")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[3].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[3].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[4].context, "testThree failure B")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[4].condition, "0 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[4].resultInd, FALSE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[5].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[5].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[5].asserts[1].condition, '"TO" = "TO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
end ;;;testFailsTestsOnly


/**
  Run ut_cclut_suo_su_td_tdo and validate the results when the setup subroutine causes an error
  but there are no other problems.
*/
subroutine testSetupErrors(null)
  declare testCount = i4 with protect, noconstant(0)

  set testing::errorSetup = TRUE

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")

  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)

  set testCount = size(ucetcf_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[1].asserts[1].context, "setupOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[1].asserts[1].condition, '"SO" = "SO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[1].asserts[1].resultInd, TRUE)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[2].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[2].context, "setup error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[2].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[3].context, "testOne executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[2].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[2].asserts[4].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[2].asserts[4].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText",
      "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[2].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText",
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetcf_testCaseResults->tests[2].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[3].asserts, 5), 5)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[3].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[2].context, "setup error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[2].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[3].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[4].context, "testTwo executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[4].condition, '"A" = "A"')
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[4].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[3].asserts[5].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[3].asserts[5].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[3].asserts[5].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText",
      "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[2].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText",
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetcf_testCaseResults->tests[2].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[4].asserts, 5), 4)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[4].errors, 5), 1)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[1].context, "setup executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[1].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[1].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[2].context, "setup error")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[2].condition, "1 = 1")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[2].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[3].context, "testThree executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[3].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[3].resultInd, TRUE)
  call cclutAssertVCEqual(CURREF, "assert context", ucetcf_testCaseResults->tests[4].asserts[4].context, "teardown executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[4].asserts[4].condition, "0 = 0")
  call cclutAssertI4Equal(CURREF, "assert status", ucetcf_testCaseResults->tests[4].asserts[4].resultInd, TRUE)
  call cclutAssertStartsWith(CURREF, "errorText",
      "%CCL-E-94-PRG_", ucetcf_testCaseResults->tests[2].errors[1].errorText)
  call cclutAssertContains(CURREF, "errorText",
      "{GUARANTEEDERROR}Assignment of Report expression (GUARANTEEDERROR) to incompatible type.",
      ucetcf_testCaseResults->tests[2].errors[1].errorText)

  call cclutAssertI4Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[5].asserts, 5), 1)
  call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[5].errors, 5), 0)
  call cclutAssertVCEqual(CURREF, "assert context",
      ucetcf_testCaseResults->tests[5].asserts[1].context, "teardownOnce executed")
  call cclutAssertVCEqual(CURREF, "assert condition", ucetcf_testCaseResults->tests[5].asserts[1].condition, '"TO" = "TO"')
  call cclutAssertI4Equal(CURREF, "assert value",
      ucetcf_testCaseResults->tests[5].asserts[1].resultInd, TRUE)
end ;;;testSetupErrors

/**
  Test a scenario where errors occur before any test is executed.
*/
subroutine testLoadingErrors(null)
  declare testing::errorLoading = i4 with protect, constant(TRUE)
  declare matches = i2 with protect, noconstant(FALSE)

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_testCaseResults->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success 2",
      ucetcf_testCaseResults->status_data.subeventstatus[1].operationName, "pre-executeTestLogic-0   ")
  call cclutAssertVCEqual(CURREF, "execute success 3",
      ucetcf_testCaseResults->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVCEqual(CURREF, "execute success 4",
      ucetcf_testCaseResults->status_data.subeventstatus[1].targetObjectName, "ut_cclut_suo_su_td_tdo   ")
  set matches = operator(ucetcf_testCaseResults->status_data.subeventstatus[1].targetObjectValue, "regexplike",
"%CCL-E-94-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{GUARANTEEDERROR\}\
Assignment of Report expression \(GUARANTEEDERROR\) to incompatible type.")
  call cclutAssertI4Equal(CURREF, "execute success 5", matches, TRUE)
  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)
end ;;;testLoadingErrors


/**
  Test a scenario where the test case file does not exist
*/

subroutine testTestCaseFileDoesNotExist(null)
  declare stat = i4 with protect, noconstant(0)

  set stat = copyRec(ucetcf_request, ucetcf_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  set ucetcf_request1->testCaseFileName = "non-existant-test-case-file"

  execute cclut_execute_test_case_file with
     replace("CCLUTREQUEST", ucetcf_request1),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success 2", 
      ucetcf_reply->status_data.subeventstatus[1].operationName, "generateTestCaseProgram  ")
  call cclutAssertVCEqual(CURREF, "execute success 3", 
      ucetcf_reply->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVCEqual(CURREF, "execute success 4",
      ucetcf_reply->status_data.subeventstatus[1].targetObjectName, "non-existant-test-case-fi")
  call cclutAssertVCEqual(CURREF, "execute success 5",
      ucetcf_reply->status_data.subeventstatus[1].targetObjectValue, "non-existant-test-case-file.inc not found in cclsource")
  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)
end ;;;testTestCaseFileDoesNotExist

/**
  Test a scenario where the current CCL version does not meet the minimum required version.
*/
subroutine testCclMinimumNotMet(null)
  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults),
      replace("GETCCLVERSION", getOutdatedCclVersion)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success 5", ucetcf_reply->status_data.subeventstatus[1].targetObjectValue,
  "The CCL version [1.0.0] does not meet the minimum version required [8.12.0] for this version of the CCL Unit Testing Framework")
  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)
end
subroutine cclut::getOutdatedCclVersion(null)
  return("1.0.0")
end ;;;testTestCaseFileDoesNotExist


/**
  Test a scenario where the test case program generation fails.
*/
subroutine testCompileProgramFailure(null)
  declare tov = vc with protect, noconstant("")
  declare idx = i4 with protect, noconstant(0)
  declare stat = i4 with protect, noconstant(0)

  set stat = copyRec(ucetcf_request, ucetcf_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)

  if (size(ucetcf_request1->programs, 5) = 0)
    set stat = alterlist(ucetcf_request1->programs, 1)
    set ucetcf_request1->programs[1].programName = "cclut"
    set ucetcf_request1->programs[1].compile = TRUE
  endif
  set ucetcf_request1->programs[1].compile = TRUE

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request1),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults),
      replace("CCLUTCOMPILEPROGRAM", compileProgramFailure)
      
  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success",
    ucetcf_reply->status_data.subeventstatus[1].operationName, "compileProgram           ")
  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.subeventstatus[1].operationStatus, "F")
  set tov = ucetcf_reply->status_data.subeventstatus[1].targetObjectValue
  call cclutAssertStartsWith(CURREF, "execute success", "compileProgram ", tov)
  call cclutAssertEndsWith(CURREF, "execute success", "compileProgram failure forced by unit test", tov)
  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)
end
subroutine public::compileProgramFailure(fileDir, fileName, listingDir, listingName, errMsg)
  set errMsg = "compileProgram failure forced by unit test"
  return (FALSE)
end ;;;testCompileProgramFailure


/**
  Test a scenario where the test case program generation fails.
*/
subroutine testGenerateTestCaseProgramFailure(null)
  declare prgStatus = i4 with protect, noconstant(0)

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults),
      replace("CCLUTGENERATETESTCASEPROGRAM", generateTestCaseProgramFailure)
      
  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success", 
      ucetcf_reply->status_data.subeventstatus[1].operationName, "generateTestCaseProgram  ")
  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.subeventstatus[1].targetObjectValue,
      "generateTestCaseProgram failure forced by unit test")
  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)
end
subroutine public::generateTestCaseProgramFailure(fileDir, fileName, listingDir, listingName, objName, errorMessage)
  set errorMessage = "generateTestCaseProgram failure forced by unit test"
  return (FALSE)
end ;;;testGenerateTestCaseProgramFailure


/**
  Test a scenario where the test case program generation succeeds but the program fails to get into the dictionary.
*/
subroutine testCheckPrgFailure(null)
  declare prgStatus = i4 with protect, noconstant(0)

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults),
      replace("CHECKPRG", checkPrgFailure)

  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success",
    ucetcf_reply->status_data.subeventstatus[1].operationName, "CHECKPRG                 ")
  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)

  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.subeventstatus[1].targetObjectValue,
      "Test case program for ut_cclut_suo_su_td_tdo not in CCL dictionary after compilation.")
  ;Unless something bad happened, the test case program actually was compiled.
  ;The following check confirms that the framework dropped it even though checkprg failed.
  set prgStatus = checkPrg(ucetcf_reply->status_data.subeventstatus[1].targetObjectName)
  call cclutAssertI4Equal(CURREF, "test case program dropped", prgStatus, 0)
end
subroutine checkPrgFailure(prgName)
  return (0)
end ;;;testCheckPrgFailure


/**
  Test a scenario where an error occurs in the framework after execution of the test case program has completed.
*/
subroutine testPostExecutionFailure(null)
  declare stat = i4 with protect, noconstant(0)
  declare fileCount = i4 with protect, noconstant(0)
  declare fileIndex = i4 with protect, noconstant(0)
  declare tov = vc with protect, noconstant("")
  declare cclut::debug = i2 with protect, noconstant(TRUE)

  record filesToRemove(1 item[*] 2 location = vc)

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults),
      replace("REMOVE", mockRemove)

  for (fileIndex = 1 to fileCount)
    set stat = remove(filesToRemove->item[fileIndex].location)
  endfor

  call validateNoIssuesResponse(ucetcf_testCaseResults)
  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "execute success", ucetcf_reply->status_data.subeventstatus[1].operationStatus, "F")

  set tov = trim(ucetcf_reply->status_data.subeventstatus[1].targetObjectValue)
  call cclutAssertStartsWith(CURREF, "execute success", "%CCL-E-94-CCLUT_EXECUTE_TEST_CASE_FILE(", tov)
  call cclutAssertEndsWith(CURREF, "execute success",
      "{REMOVEFAILURE}Assignment of Report expression (REMOVEFAILURE) to incompatible type.", tov)
end
subroutine mockRemove(fileLocation)
  declare removeFailure = i4 with protect, noconstant(0)
  declare fileCount = i4 with protect, noconstant(1 + size(filesToRemove->item, 5))

  set cclut::stat = alterlist(filesToRemove->item, fileCount)
  set filesToRemove->item[fileCount].location = fileLocation

  if (fileLocation = concat(cclut::outputDirectory, cclut::testCaseListingName))
    set removeFailure = "remove failure"
    return(FALSE)
  endif
  return(TRUE)
end ;;;testPostExecutionFailure


/**
  Test a scenario where a test produces an error and calls go to exit script without producing any asserts.
  The error should be attributed to the test not to the fraemwork.
*/
subroutine testErrorThenExitScript(null)
  set ucetcf_request->testCaseFileName = "ut_cclut_error_then_exit_script"

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertI2Equal(CURREF, "test case should fail", ucetcf_testCaseResults->resultInd, FALSE)
  call cclutAssertI4Equal(CURREF, "test count", size(ucetcf_testCaseResults->tests, 5), 2)
  call cclutAssertVcEqual(CURREF, "setupOnce name", ucetcf_testCaseResults->tests[1].name, "SETUPONCE")
  call cclutAssertI4Equal(CURREF, "setupOnce assert count", size(ucetcf_testCaseResults->tests[1].asserts, 5), 1)
  call cclutAssertVcEqual(CURREF,
      "setupOnce assert context", ucetcf_testCaseResults->tests[1].asserts[1].context, "setupOnce - guaranteed success")
  call cclutAssertI4Equal(CURREF, "setupOnce error count", size(ucetcf_testCaseResults->tests[1].errors, 5), 0);

  call cclutAssertVcEqual(CURREF, "testOne name", ucetcf_testCaseResults->tests[2].name, "TESTONE")
  call cclutAssertI4Equal(CURREF, "testOne assert count", size(ucetcf_testCaseResults->tests[2].asserts, 5), 0)
  call cclutAssertI4Equal(CURREF, "testOne error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 1)
  call cclutAssertVcOperator(CURREF, "testOne error text", ucetcf_testCaseResults->tests[2].errors.errorText, "regexplike",
    "%CCL-E-392-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\): forced error.")
end ;;;testErrorThenExitScript


/**
  Confirms that code coverage exclusion works as exepcted.
*/
subroutine testCoverageExcludes(null)
  declare stat = i4 with protect, noconstant(0)

  if (currev > 8 or (currev = 8 and currevminor > 12))
    declare debugAvailable = i4 with protect, noconstant(0)
    declare previousDebugTrace = i4 with protect, noconstant(0)
    declare outputLine = vc with protect, noconstant("")

    select into cclut_check_debug from dummyt detail
      outputLine = "drop program cclut_check_debug go"
      outputLine row+1
      outputLine = "create program cclut_check_debug"
      outputLine row+1
      outputLine = "set debugAvailable = evaluate(trace('DEBUG'),1,1,0)"
      outputLine row+1
      outputLine = "set previousDebugTrace = debugAvailable"
      outputLine row+1
      outputLine = "if (debugAvailable = FALSE)"
      outputLine row+1
      outputLine = "set trace debug"
      outputLine row+1
      outputLine = "set debugAvailable = evaluate(trace('DEBUG'),1,1,0)"
      outputLine row+1
      outputLine = "if (previousDebugTrace = 0) set trace nodebug endif"
      outputLine row+1
      outputLine = "endif"
      outputLine row+1
      outputLine = "end go"
      outputLine row+1
    with nocounter
    call compile("cclut_check_debug.dat")
    execute cclut_check_debug
    call parser ("drop program cclut_check_debug go")
    set stat = remove("cclut_check_debug.dat")
    set stat =
        cclutAssertI4Equal(CURREF, "cclora_dbg required for code coverage since CCL version 8.13.0", debugAvailable, TRUE)
    if (stat = FALSE)
      return (null)
    endif
  endif

  declare coverageXml = vc with protect, noconstant("")

  set stat = copyrec(ucetcf_request, tce_request)
  set stat = copyrec(ucetcf_reply, tce_reply)
  set stat = copyrec(ucetcf_testCaseResults, tce_testCaseResults)

  set tce_request->testCaseDirectory = "cclsource"
  set tce_request->testCaseFileName = "ut_ex_ut_cclut_cc_exclusions"
  set stat = alterlist(tce_request->programs, 1)
  set tce_request->programs[1].programName = "ut_cclut_cc_exclusions"
  set tce_request->programs[1].compile = TRUE

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", tce_request),
      replace("CCLUTREPLY", tce_reply),
      replace("CCLUTTESTCASERESULTS", tce_testCaseResults)
  call cclutAssertVcNotEqual(CURREF, "expected non-empty listingXml", tce_reply->programs[1].listingXml, "")
  set coverageXml = tce_reply->programs[1].coverageXml
  call cclutAssertContains(CURREF, "expect 14 non-executable", "<LINE><NBR>14</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 27 non-executable", "<LINE><NBR>27</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 28 non-executable", "<LINE><NBR>28</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 38 non-executable", "<LINE><NBR>38</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 45 non-executable", "<LINE><NBR>45</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 46 non-executable", "<LINE><NBR>46</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 47 non-executable", "<LINE><NBR>47</NBR><TYPE>N</TYPE></LINE>", coverageXml)
  call cclutAssertContains(CURREF, "expect 48 non-executable", "<LINE><NBR>48</NBR><TYPE>N</TYPE></LINE>", coverageXml)

  set stat = initrec(tce_reply)
  set stat = initrec(tce_testCaseResults)
  set tce_request->programs[1].compile = FALSE
  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", tce_request),
      replace("CCLUTREPLY", tce_reply),
      replace("CCLUTTESTCASERESULTS", tce_testCaseResults)
  call cclutAssertVcEqual(CURREF, "expected empty listingXml", tce_reply->programs[1].listingXml, "")
  call cclutAssertVcEqual(CURREF, "expected same coverageXml", tce_reply->programs[1].coverageXml, coverageXml)
end ;;;testCoverageExcludes

/**
  Test that the framework removes any mock tables that test cases fail to do when calling cclut_execute_test_case_file.
*/
subroutine testMockTableCleanup(null)
  declare cclutMockTableName = vc with protect, noconstant("")

  set ucetcf_request->testCaseFileName = "ut_cclut_mock_table_misbehaving_cleanup"

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertStartsWith(CURREF, "testMockTableCleanup 001", "CUST_CCLUT", cclutMockTableName)

  declare mockId = f8 with protect, noconstant(0.0)
  declare mockText = vc with protect, noconstant("")
  declare mockDate = dq8 with protect, noconstant(0)
  select into "nl:"
  from (value(cclutMockTableName) m)
  detail
      mockId = m.SAMPLE_TABLE_ID
      mockText = m.SAMPLE_TABLE_TEXT
      mockDate = m.SAMPLE_TABLE_DATE
  with nocounter

  declare errorMessage = vc with protect, noconstant("")
  declare errorCode = i4 with protect, noconstant(0)
  set errorCode = error(errorMessage, 0)

  call cclutAssertVcOperator(CURREF, "testMockTableCleanup 002", trim(errorMessage, 3),
      "regexplike", concat("%CCL-E-18-PRG_[0-9]+_[0-9]+\([^)]+\)[0-9]+:[0-9]+\{\}Unable to add range, definition for",
          " table \(CUST_CCLUT_[^)]*\) not found in dictionary."))
end ;;;testMockTableCleanup


/**
    Confirms that the new fail fast request attribute needn't be populated
*/
subroutine testDefaultsNotFailFast(null)
  declare testCount = i4 with protect, noconstant(0)
  declare errorIdx = i4 with protect, noconstant(0)

  set testing::errorSetupOnce = TRUE
  set testing::errorSetup = TRUE
  set testing::errorTest = TRUE
  set testing::errorTeardown = TRUE
  set testing::errorTeardownOnce = TRUE

  set testing::failSetupOnce = TRUE
  set testing::failSetup = TRUE
  set testing::failTest = TRUE
  set testing::failTeardown = TRUE
  set testing::failTeardownOnce = TRUE

  record ucetcf_request (
    1 testCaseDirectory = vc
    1 testCaseFileName = vc
    1 testNamePattern = vc
    1 programs[*]
      2 programName = vc
      2 compile = i2
    1 optimizerMode = vc
    1 enforcePredeclare = i2
    1 deprecatedFlag = vc
    1 legacyResultsFormat = i2
  ) with protect
  set ucetcf_request->testCaseDirectory = "cclsource"
  set ucetcf_request->testCaseFileName = "ut_cclut_suo_su_td_tdo"

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  ;that's it.
  ;if cclut_execute_test_case_file requires the value to be set an error would occur.
  ;the remaining checks confirm it did not fail fast

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")
  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)
  set testCount = size(ucetcf_testCaseResults->tests, 5)
  call cclutAssertI4Equal(CURREF, "test count", testCount, 5)
end ;;;testDefaultsNotFailFast

/**
    Helper function for the testFailFast functions
*/
subroutine checkFailFast(checkFunction)
  declare stat = i4 with protect, noconstant(0)
  declare testCount = i4 with protect, noconstant(0)
  declare errorCount = i4 with protect, noconstant(0)
  declare context = vc with protect, noconstant("")
  declare idx = i4 with protect, noconstant(0)

  set stat = copyRec(ucetcf_request, ucetcf_request1, 1)
  call cclutAssertI4Equal(CURREF, "validate copyRec", stat, TRUE)
  if (validate(cclut_request) = TRUE)
    ;to capture code coverage if running from maven
    set stat = moverec(cclut_request->programs, ucetcf_request1->programs)
  endif
  set ucetcf_request1->failFast = TRUE

  execute cclut_execute_test_case_file with
      replace("CCLUTREQUEST", ucetcf_request1),
      replace("CCLUTREPLY", ucetcf_reply),
      replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults)

  call cclutAssertVCEqual(CURREF, "execute success 1", ucetcf_reply->status_data.status, "S")
  call cclutAssertI2Equal(CURREF, "test case failure", ucetcf_testCaseResults->resultInd, FALSE)
  set testCount = size(ucetcf_testCaseResults->tests, 5)

  call parser(concat("call ", checkFunction, "(null) go"))

end ;;;checkFailFast


/**
  Test that fail fast works as expected when setupOnce produces an assertFailure.
*/
subroutine testFailFastSetupOnceFailure(null)
  set testing::failSetupOnce = TRUE
  call checkFailFast("checkFailFastSetupOnceFailure")
end ;;;testFailFastSetupOnceFailure

subroutine checkFailFastSetupOnceFailure(null)
  call cclutAssertI4Equal(CURREF, "FailFastSetupOnceFailure test count", testCount, 2)
  set context = ucetcf_testCaseResults->tests[1].asserts[2].context
  call cclutAssertVcEqual(CURREF, "FailFastSetupOnceFailure context", context, "setupOnce failure")
  ;;check that teardownOnce executes despite the setupOnce failure
  set context = ucetcf_testCaseResults->tests[2].asserts[1].context
  call cclutAssertVcEqual(CURREF, "FailFastSetupOnceFailure context", context, "teardownOnce executed")
  for (idx = 1 to testCount)
      set errorCount = size(ucetcf_testCaseResults->tests[idx].errors, 5)
      call cclutAssertI4Equal(CURREF, "FailFastSetupOnceFailure error", errorCount, 0)
  endfor
end ;;;checkFailFastSetupOnceFailure


/**
  Test that fail fast works as expected when setupOnce produces an error.
*/
subroutine testFailFastSetupOnceError(null)
  set testing::errorSetupOnce = TRUE
  call checkFailFast("checkFailFastSetupOnceError")
end ;;;testFailFastSetupOnceError

subroutine checkFailFastSetupOnceError(null)
  call cclutAssertI4Equal(CURREF, "FailFastSetupOnceError test count", testCount, 2)
  set context = ucetcf_testCaseResults->tests[1].asserts[2].context
  call cclutAssertVcEqual(CURREF, "FailFastSetupOnceError context", context, "setupOnce error")
  ;;check that teardownOnce executes despite the setupOnce error
  set context = ucetcf_testCaseResults->tests[2].asserts[1].context
  call cclutAssertVcEqual(CURREF, "FailFastSetupOnceFailure context", context, "teardownOnce executed")
  for (idx = 1 to testCount)
      set errorCount = size(ucetcf_testCaseResults->tests[idx].errors, 5)
      if (idx = 1)
        call cclutAssertI4Equal(CURREF, "FailFastSetupOnceError error", errorCount, 1)
      else
        call cclutAssertI4Equal(CURREF, "FailFastSetupOnceError error", errorCount, 0)
      endif
  endfor
end ;;;checkFailFastSetupOnceError


/**
  Test that fail fast works as expected when setup produces an assertFailure.
*/
subroutine testFailFastSetupFailure(null)
  set testing::failSetup = TRUE
  call checkFailFast("checkFailFastSetupFailure")
end ;;;testFailFastSetupFailure

subroutine checkFailFastSetupFailure(null)
  call cclutAssertI4Equal(CURREF, "FailFastSetupFailure test count", testCount, 3)
  set context = ucetcf_testCaseResults->tests[2].asserts[2].context
  call cclutAssertVcEqual(CURREF, "FailFastSetupFailure context", context, "setup failure")
  ;;check that teardown executes despite setup failure (the test actually does too)
  set context = ucetcf_testCaseResults->tests[2].asserts[4].context
  call cclutAssertVcEqual(CURREF, "FailFastTestFailure context", context, "teardown executed")
  ;;check that teardownOnce executes despite setup error
  set context = ucetcf_testCaseResults->tests[3].asserts[1].context
  call cclutAssertVcEqual(CURREF, "FailFastSetupFailure context", context, "teardownOnce executed")
  for (idx = 1 to testCount)
      set errorCount = size(ucetcf_testCaseResults->tests[idx].errors, 5)
      call cclutAssertI4Equal(CURREF, "FailFastSetupFailure error", errorCount, 0)
  endfor
end ;;;


/**
  Test that fail fast works as expected when setup produces an error.
*/
subroutine testFailFastSetupError(null)
  set testing::errorSetup = TRUE
  call checkFailFast("checkFailFastSetupError")
end ;;;testFailFastSetupError

subroutine checkFailFastSetupError(null)
  call cclutAssertI4Equal(CURREF, "FailFastSetupError test count", testCount, 3)
  set context = ucetcf_testCaseResults->tests[2].asserts[2].context
  call cclutAssertVcEqual(CURREF, "FailFastSetupError context", context, "setup error")
  ;;check that teardown executes despite the setup error (the test actually does too)
  set context = ucetcf_testCaseResults->tests[2].asserts[4].context
  call cclutAssertVcEqual(CURREF, "FailFastTestFailure context", context, "teardown executed")
  ;;check that teardownOnce executes despite the setup error
  set context = ucetcf_testCaseResults->tests[3].asserts[1].context
  call cclutAssertVcEqual(CURREF, "FailFastSetupError context", context, "teardownOnce executed")
  for (idx = 1 to testCount)
      set errorCount = size(ucetcf_testCaseResults->tests[idx].errors, 5)
      if (idx = 2)
        call cclutAssertI4Equal(CURREF, "FailFastSetupError error", errorCount, 1)
      else
        call cclutAssertI4Equal(CURREF, "FailFastSetupError error", errorCount, 0)
      endif
  endfor
end ;;;checkFailFastSetupError


/**
  Test that fail fast works as expected when some test produces an assertFailure.
*/
subroutine testFailFastTestFailure(null)
  set testing::failTest = TRUE
  call checkFailFast("checkFailFastTestFailure")
end ;;;testFailFastTestFailure

subroutine checkFailFastTestFailure(null)
  call cclutAssertI4Equal(CURREF, "FailFastTestFailure test count", testCount, 3)
  set context = ucetcf_testCaseResults->tests[2].asserts[3].context
  call cclutAssertVcEqual(CURREF, "FailFastTestFailure context", context, "testOne failure")
  ;;check that teardown execute despite test failure
  set context = ucetcf_testCaseResults->tests[2].asserts[4].context
  call cclutAssertVcEqual(CURREF, "FailFastTestFailure context", context, "teardown executed")
  ;;check that teardownOnce executes despite the failure
  set context = ucetcf_testCaseResults->tests[3].asserts[1].context
  call cclutAssertVcEqual(CURREF, "FailFastTestFailure context", context, "teardownOnce executed")
  for (idx = 1 to testCount)
      set errorCount = size(ucetcf_testCaseResults->tests[idx].errors, 5)
      call cclutAssertI4Equal(CURREF, "FailFastTestFailure error", errorCount, 0)
  endfor
end ;;;checkFailFastTestFailure



/**
  Test that fail fast works as expected when some test produces an error.
*/
subroutine testFailFastTestError(null)
  set testing::errorTest = TRUE
  call checkFailFast("checkFailFastTestError")
end ;;;testFailFastTestError

subroutine checkFailFastTestError(null)
  call cclutAssertI4Equal(CURREF, "FailFastTestError test count", testCount, 3)
  set context = ucetcf_testCaseResults->tests[2].asserts[3].context
  call cclutAssertVcEqual(CURREF, "FailFastTestError context", context, "testOne error")
  ;;check that teardown execute despite test error
  set context = ucetcf_testCaseResults->tests[2].asserts[4].context
  call cclutAssertVcEqual(CURREF, "FailFastTestFailure context", context, "teardown executed")
  ;;check that teardownOnce executes despite the error
  set context = ucetcf_testCaseResults->tests[3].asserts[1].context
  call cclutAssertVcEqual(CURREF, "FailFastTestError context", context, "teardownOnce executed")
  for (idx = 1 to testCount)
      set errorCount = size(ucetcf_testCaseResults->tests[idx].errors, 5)
      if (idx = 2)
        call cclutAssertI4Equal(CURREF, "FailFastTestError error", errorCount, 1)
      else
        call cclutAssertI4Equal(CURREF, "FailFastTestError error", errorCount, 0)
      endif
  endfor
end ;;;checkFailFastTestError


/**
  Test that fail fast works as expected when teardown produces an assertFailure.
*/
subroutine testFailFastTeardownFailure(null)
  set testing::failTeardown = TRUE
  call checkFailFast("checkFailFastTeardownFailure")
end ;;;testFailFastTeardownFailure

subroutine checkFailFastTeardownFailure(null)
  call cclutAssertI4Equal(CURREF, "FailFastTeardownFailure test count", testCount, 3)
  set context = ucetcf_testCaseResults->tests[2].asserts[4].context
  call cclutAssertVcEqual(CURREF, "FailFastTeardownFailure context", context, "teardown failure")
  ;;check that teardownOnce executes despite the failure
  set context = ucetcf_testCaseResults->tests[3].asserts[1].context
  call cclutAssertVcEqual(CURREF, "FailFastTeardownFailure context", context, "teardownOnce executed")
  for (idx = 1 to testCount)
      set errorCount = size(ucetcf_testCaseResults->tests[idx].errors, 5)
      call cclutAssertI4Equal(CURREF, "FailFastTeardownFailure error", errorCount, 0)
  endfor
end ;;;checkFailFastTeardownFailure


/**
  Test that fail fast works as expected when teardown produces an error.
*/
subroutine testFailFastTeardownError(null)
  set testing::errorTeardown = TRUE
  call checkFailFast("checkFailFastTeardownError")
end ;;;testFailFastTeardownError

subroutine checkFailFastTeardownError(null)
  call cclutAssertI4Equal(CURREF, "FailFastTeardownError test count", testCount, 3)
  set context = ucetcf_testCaseResults->tests[2].asserts[4].context
  call cclutAssertVcEqual(CURREF, "FailFastTeardownError context", context, "teardown error")
  ;;check that teardownOnce executes despite the error
  set context = ucetcf_testCaseResults->tests[3].asserts[1].context
  call cclutAssertVcEqual(CURREF, "FailFastTeardownError context", context, "teardownOnce executed")
  for (idx = 1 to testCount)
      set errorCount = size(ucetcf_testCaseResults->tests[idx].errors, 5)
      if (idx = 2)
        call cclutAssertI4Equal(CURREF, "FailFastTeardownError error", errorCount, 1)
      else
        call cclutAssertI4Equal(CURREF, "FailFastTeardownError error", errorCount, 0)
      endif
  endfor
end ;;;checkFailFastTeardownError


/**
  Test that fail fast works as expected when teardownOnce produces an assert failure and an error.
*/
subroutine testFailFastTeardownOnce(null)
  set testing::failTeardownOnce = TRUE
  set testing::errorTeardownOnce = TRUE
  call checkFailFast("checkFailFastTeardownOnce")
end ;;;testFailFastTeardownOnce

subroutine checkFailFastTeardownOnce(null)
  call cclutAssertI4Equal(CURREF, "FailFastTeardownOnce test count", testCount, 5)
  set context = ucetcf_testCaseResults->tests[5].asserts[2].context
  call cclutAssertVcEqual(CURREF, "FailFastTeardownOnce context", context, "teardownOnce failure")
  set context = ucetcf_testCaseResults->tests[5].asserts[3].context
  call cclutAssertVcEqual(CURREF, "FailFastTeardownOnce context", context, "teardownOnce error")
  set context = ucetcf_testCaseResults->tests[1].asserts[1].context
  call cclutAssertVcEqual(CURREF, "FailFastTeardownOnce context", context, "setupOnce executed")
  for (idx = 1 to testCount)
      set errorCount = size(ucetcf_testCaseResults->tests[idx].errors, 5)
      if (idx = 5)
        call cclutAssertI4Equal(CURREF, "FailFastTeardownOnce error", errorCount, 1)
      else
        call cclutAssertI4Equal(CURREF, "FailFastTeardownOnce error", errorCount, 0)
      endif
  endfor
end ;;;checkFailFastTeardownOnce

