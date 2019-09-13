/**
  Unit Tests for cclut_compile_test_case and cclut_compile_test_case_file
*/

declare setup(null) = null with protect
declare testCompileTestCaseFile(null) = null with protect
declare testCompileTestCaseFileGen(null) = null with protect
declare testCompileTestCaseFileDefaultsCerTemp(null) = null with protect

declare testCompileTestCaseFile(null) = null with protect
declare testCompileTestCaseFileDictionaryFailure(null) = null with protect
declare testCompileTestCaseDefaultsCclsource(null) = null with protect
declare testCompileTestCaseGen(null) = null with protect
declare testTestCaseFileDoesNotExist(null) = null with protect
declare testTestCaseFileNotProvided(null) = null with protect
declare testMemooryReplyStringNotProvided(null) = null with protect


declare testCompileTestCaseFile::genTCP(
    dirLogical = vc, fileName = vc, listingLogical = vc, listingName = vc, progName = vc, errMessage = vc(ref)) = i2
declare testCompileTestCaseFileDefaultsCerTemp::genTCP(
    dirLogical = vc, fileName = vc, listingLogical = vc, listingName = vc, progName = vc, errMessage = vc(ref)) = i2
declare mockCheckPrgTrue(programName = vc) = i2
declare mockCheckPrgFalse(programName = vc) = i2
declare mockEcho(msg = vc) = null

declare testObjectName = vc with protect
declare stat = i4 with protect

record compileRequest (
  1 testCaseDirectory = vc
  1 testCaseFileName = vc
) with protect

record compileReply (
  1 testCaseObjectName = vc
%i cclsource:status_block.inc
) with protect


subroutine setup(null)
  set stat = initrec(compileRequest)
  set stat = initrec(compileReply)
end ;setup

subroutine mockCheckPrgTrue(progamName)
  return(1)
end ;mockCheckPrgTrue

subroutine mockCheckPrgFalse(progamName)
  return(0)
end ;mockCheckPrgFalse


/**
  Confirms the behavior of cclut_compile_test_case_file verifying that it calls cclutGenerateTestCaseFile 
  to do the work and checking the values it passes.
*/
subroutine testCompileTestCaseFile(null)
  declare genTCPCalledA = i2 with protect, noconstant(FALSE)
  declare testCaseId = vc with protect

  set compileRequest->testCaseDirectory = "test_dir"
  set compileRequest->testCaseFileName = "some_file"
  execute cclut_compile_test_case_file with 
    replace("CCLUTREQUEST", compileRequest), replace("CCLUTREPLY", compileReply),
    replace("CCLUTGENERATETESTCASEPROGRAM", genTCP), replace("CHECKPRG", mockCheckPrgTrue),
    curnamespace = "testCompileTestCaseFile"
  call cclutAssertI2Equal(CURREF, "genTCP called A", genTCPCalledA, TRUE)
  call cclutAssertVcEqual(CURREF, "expect success A", compileReply->status_data->status, "S")
  call cclutAssertVcEqual(CURREF, "generated object name A", compileReply->testCaseObjectName, concat("prg_", testCaseId))
end ;testCompileTestCaseFile
subroutine testCompileTestCaseFile::genTCP(dirLogical, fileName, listingLogical, listingName, progName , errMessage)
  set genTCPCalledA = TRUE
  call cclutAssertVcEqual(CURREF, "expect dirLogical A", dirLogical, "test_dir")
  call cclutAssertVcEqual(CURREF, "expect fileName A", fileName, "some_file")
  call cclutAssertVcEqual(CURREF, "expect listingLogical A", listingLogical, "CCLUSERDIR")
  call cclutAssertStartsWith(CURREF, "expect progName A", "prg_", progName)
  set testCaseId substring(5, textlen(progName)-4, progName)
  call cclutAssertVcEqual(CURREF, "expect listingName A", listingName, concat("prg_", testCaseId, ".lis"))
end ;genTCP


/**
  Confirms the behavior of cclut_compile_test_case_file when the object compiles but
  fails to land in the dictionary.
*/
subroutine testCompileTestCaseFileDictionaryFailure(null)
  declare genTCPCalledA = i2 with protect, noconstant(FALSE)
  declare testCaseId = vc with protect

  set compileRequest->testCaseDirectory = "test_dir"
  set compileRequest->testCaseFileName = "some_file"
  execute cclut_compile_test_case_file with
    replace("CCLUTREQUEST", compileRequest), replace("CCLUTREPLY", compileReply),
    replace("CCLUTGENERATETESTCASEPROGRAM", genTCP), replace("CHECKPRG", mockCheckPrgFalse),
    curnamespace = "DictionaryFailure"
  call cclutAssertI2Equal(CURREF, "genTCP called B", genTCPCalledA, TRUE)
  call cclutAssertVcEqual(CURREF, "expect failure B", compileReply->status_data->status, "F")
  call cclutAssertVcEqual(CURREF, "expect no generated object name", compileReply->testCaseObjectName, trim(""))
end ;testCompileTestCaseFileDictionaryFailure
subroutine DictionaryFailure::genTCP(dirLogical, fileName, listingLogical, listingName, progName , errMessage)
  set genTCPCalledA = TRUE
  call cclutAssertVcEqual(CURREF, "expect dirLogical A", dirLogical, "test_dir")
  call cclutAssertVcEqual(CURREF, "expect fileName A", fileName, "some_file")
  call cclutAssertVcEqual(CURREF, "expect listingLogical A", listingLogical, "CCLUSERDIR")
  call cclutAssertStartsWith(CURREF, "expect progName A", "prg_", progName)
  set testCaseId substring(5, textlen(progName)-4, progName)
  call cclutAssertVcEqual(CURREF, "expect listingName A", listingName, concat("prg_", testCaseId, ".lis"))
end ;genTCP


/**
  Confirms that cclut_compile_test_case_file defaults cer_temp for the test case directory, verifies it calls
  cclutGenerateTestCaseFile to do the work and confirms the values it passes.
*/
subroutine testCompileTestCaseFileDefaultsCerTemp(null)
  declare genTCPCalledB = i2 with protect, noconstant(FALSE)

  set compileRequest->testCaseFileName = "some_file"
  execute cclut_compile_test_case_file with 
    replace("CCLUTREQUEST", compileRequest), replace("CCLUTREPLY", compileReply),
    replace("CCLUTGENERATETESTCASEPROGRAM", genTCP), replace("CHECKPRG", mockCheckPrgTrue),
    curnamespace = "testCompileTestCaseFileDefaultsCerTemp"
  call cclutAssertI2Equal(CURREF, "genTCP called B", genTCPCalledB, TRUE)
  call cclutAssertVcEqual(CURREF, "expect success B", compileReply->status_data->status, "S")
end ;testCompileTestCaseFileDefaultsCerTemp
subroutine testCompileTestCaseFileDefaultsCerTemp::genTCP(dirLogical, fileName, listingLogical, listingName, progName , errMessage)
  set genTCPCalledB = TRUE
  call cclutAssertVcEqual(CURREF, "expect dirLogical", dirLogical, "cer_temp")
end ;genTCP


/**
  Confirms that cclut_compile_test_case_file actually generates a test case object
*/
subroutine testCompileTestCaseFileGen(null)
  set compileRequest->testCaseDirectory = "cclsource"
  set compileRequest->testCaseFileName = "ut_cclut_suo_su_td_tdo"
  
  execute cclut_compile_test_case_file with  replace("CCLUTREQUEST", compileRequest), replace("CCLUTREPLY", compileReply)  
  
  declare testing::failSetupOnce = i2 with protect, noconstant(FALSE)
  declare testing::failSetup = i2 with protect, noconstant(FALSE)
  declare testing::failTest = i2 with protect, noconstant(FALSE)
  declare testing::failTeardown = i2 with protect, noconstant(FALSE)
  declare testing::failTeardownOnce = i2 with protect, noconstant(FALSE)
  declare testing::errorSetupOnce = i2 with protect, noconstant(FALSE)
  declare testing::errorSetup = i2 with protect, noconstant(FALSE)
  declare testing::errorTest = i2 with protect, noconstant(FALSE)
  declare testing::errorTeardown = i2 with protect, noconstant(FALSE)
  declare testing::errorTeardownOnce = i2 with protect, noconstant(FALSE)

  record cclutTestCaseRequest (
    1 testNamePattern = vc
    1 failFast = i2
  ) with protect
  
  record cclutTestCaseResults (
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

  execute value(cnvtupper(compileReply->testCaseObjectName)) with
      replace ("CCLUTREQUEST", cclutTestCaseRequest),
      replace ("CCLUTREPLY", cclutTestCaseResults)

  call cclutAssertVcEqual(CURREF, "test case status C", cclutTestCaseResults->status_data->status, "S")
  call cclutAssertI2Equal(CURREF, "test case result C", cclutTestCaseResults->resultInd, TRUE)
  call cclutAssertI4Equal(CURREF, "test count C", size(cclutTestCaseResults->tests, 5), 5)

  call parser(concat("drop program ", compileReply->testCaseObjectName, " go"))
end ;testCompileTestCaseFileGen


/**
  Confirms that cclut_compile_test_case invokes cclut_compile_test_case_file.
*/
subroutine testCompileTestCase(null)
  declare _memory_reply_string = vc with protect
  declare _mock_request = vc with protect
  execute cclut_compile_test_case "", "some_dir", "some_file" with 
      replace("CCLUTREQUEST", tctcRequest), replace("CCLUTREPLY", tctcReply),
      replace("CCLUT_COMPILE_TEST_CASE_FILE", mock_cclut_compile_tcf)
      
  set stat = cnvtjsontorec(_memory_reply_string)
  set stat = cnvtjsontorec(_mock_request)

  call cclutAssertVcEqual(CURREF, "tctcReply status D", tctcReply->status_data->status, "S")
  call cclutAssertVcEqual(CURREF, "tctcReply object name D", tctcReply->testCaseObjectName, "mock")
  call cclutAssertVcEqual(CURREF, "test case directory D", mockRequest->testCaseDirectory, "some_dir")
  call cclutAssertVcEqual(CURREF, "test case directory D", mockRequest->testCaseFileName, "some_file")
end ;testCompileTestCase

/**
  Confirms that cclut_compile_test_case defaults the test case directory to CCLSOURCE.
*/
subroutine testCompileTestCaseDefaultsCclsource(null)
  declare _memory_reply_string = vc with protect
  declare _mock_request = vc with protect
  execute cclut_compile_test_case "", "", "some_file" with 
      replace("CCLUTREQUEST", tctcdcRequest), replace("CCLUTREPLY", tctcdcReply),
      replace("CCLUT_COMPILE_TEST_CASE_FILE", mock_cclut_compile_tcf)
      
  set stat = cnvtjsontorec(_memory_reply_string)
  set stat = cnvtjsontorec(_mock_request)

  call cclutAssertVcEqual(CURREF, "tctcdcReply status E", tctcdcReply->status_data->status, "S")
  call cclutAssertVcEqual(CURREF, "tctcdcReply object name E", tctcdcReply->testCaseObjectName, "mock")
  call cclutAssertVcEqual(CURREF, "test case directory E", mockRequest->testCaseDirectory, "CCLSOURCE")
  call cclutAssertVcEqual(CURREF, "test case directory D", mockRequest->testCaseFileName, "some_file")
end ;testCompileTestCase


/**
  Confirms that cclut_compile_test_case actually generates a test case object.
*/
subroutine testCompileTestCaseGen(null)
  declare _memory_reply_string = vc with protect
  execute cclut_compile_test_case "", "", "ut_cclut_suo_su_td_tdo" with 
      replace("CCLUTREQUEST", tctcgRequest), replace("CCLUTREPLY", tctcgReply)
      
  set stat = cnvtjsontorec(_memory_reply_string)  
  
  declare testing::failSetupOnce = i2 with protect, noconstant(FALSE)
  declare testing::failSetup = i2 with protect, noconstant(FALSE)
  declare testing::failTest = i2 with protect, noconstant(FALSE)
  declare testing::failTeardown = i2 with protect, noconstant(FALSE)
  declare testing::failTeardownOnce = i2 with protect, noconstant(FALSE)
  declare testing::errorSetupOnce = i2 with protect, noconstant(FALSE)
  declare testing::errorSetup = i2 with protect, noconstant(FALSE)
  declare testing::errorTest = i2 with protect, noconstant(FALSE)
  declare testing::errorTeardown = i2 with protect, noconstant(FALSE)
  declare testing::errorTeardownOnce = i2 with protect, noconstant(FALSE)

  record cclutTestCaseRequest (
    1 testNamePattern = vc
    1 failFast = i2
  ) with protect
  
  record cclutTestCaseResults (
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

  execute value(cnvtupper(tctcgReply->testCaseObjectName)) with
      replace ("CCLUTREQUEST", cclutTestCaseRequest),
      replace ("CCLUTREPLY", cclutTestCaseResults)

  call cclutAssertVcEqual(CURREF, "test case status", cclutTestCaseResults->status_data->status, "S")
  call cclutAssertI2Equal(CURREF, "test case result", cclutTestCaseResults->resultInd, TRUE)
  call cclutAssertI4Equal(CURREF, "test count", size(cclutTestCaseResults->tests, 5), 5)

  call parser(concat("drop program ", tctcgReply->testCaseObjectName, " go"))
end ;testCompileTestCaseFileGen



/**
  Confirms that cclut_compile_test_case reports a failure if the test case file does not exist.
*/
subroutine testTestCaseFileDoesNotExist(null)
  declare _memory_reply_string = vc with protect
  declare stat = i4 with protect
  execute cclut_compile_test_case "", "", "i.am.not.a.file" with
      replace("CCLUTREQUEST", tctcgRequest), replace("CCLUTREPLY", tctcgReply)

  set stat = cnvtjsontorec(_memory_reply_string)
  call cclutAssertVCEqual(CURREF, "expect no object name", tctcgReply->testCaseObjectName, trim(""))
  call cclutAssertVCEqual(CURREF, "expect a failure", tctcgReply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "expect a failure messsage a", tctcgReply->status_data.subeventstatus[1].operationName,
      "generateTestCaseProgram")
  call cclutAssertVCEqual(CURREF, "expect a failure messsage b", tctcgReply->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVCEqual(CURREF, "expect a failure messsage a", tctcgReply->status_data.subeventstatus[1].targetObjectName,
      "i.am.not.a.file")
  call cclutAssertVCEqual(CURREF, "expect a failure messsage a", tctcgReply->status_data.subeventstatus[1].targetObjectValue,
      "i.am.not.a.file.inc not found in CCLSOURCE")
end ;testTestCaseFileDoesNotExist

/**
  Confirms that cclut_compile_test_case reports a failure if a test case file is not provided.
*/
subroutine testTestCaseFileNotProvided(null)
  declare _memory_reply_string = vc with protect
  execute cclut_compile_test_case "", "", ""
  call cclutAssertVCEqual(CURREF, "expect error message", _memory_reply_string, "A test case file name must be provided")
end ;testTestCaseFileNotProvided

/**
  Confirms that cclut_compile_test_case does not fail if _memory_reply_string is not provided.
*/
subroutine testMemooryReplyStringNotProvided(null)
  execute cclut_compile_test_case "", "", "" with replace("ECHO", mockEcho)
end ;testMemooryReplyStringNotProvided
subroutine mockEcho(msg)
  call cclutAssertVCEqual(CURREF, "expect error message", msg, "A test case file name must be provided")
end ;mockEcho
