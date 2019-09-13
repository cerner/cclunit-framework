/**
  Unit tests for cclut_find_unit_tests.
*/
declare mockParser(instruction = vc) = i4 with protect
declare testItA(null) = null with protect
declare testItB(null) = null with protect
declare testItC(null) = null with protect


/**
  Mock no-op for the parser function.
*/
subroutine mockParser(instruction)
    null
end 

/**
  Confirm that cclut_find_unit_test can find all four kinds of tests
*/
subroutine testItA(null)
  record request (
    1 programName = vc
  ) with protect
  record reply (
    1 tests[*]
      2 testName = vc
%i cclsource:status_block.inc
  ) with protect
  
  set request->programName = "ut_cclut_find_unit_tests"
  execute cclut_find_unit_tests with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply)

  call cclutAssertI4Equal(CURREF, "expect eight tests", size(reply->tests, 5), 8)
  call cclutAssertVCEqual(CURREF, "test 1", reply->tests[1].testName, "TESTONE")
  call cclutAssertVCEqual(CURREF, "test 2", reply->tests[2].testName, "TESTING::TESTTWO")
  call cclutAssertVCEqual(CURREF, "test 3", reply->tests[3].testName, "TESTTHREE")
  call cclutAssertVCEqual(CURREF, "test 4", reply->tests[4].testName, "TESTING::TESTFOUR")
  call cclutAssertVCEqual(CURREF, "test 5", reply->tests[5].testName, "TESTFIVE")
  call cclutAssertVCEqual(CURREF, "test 6", reply->tests[6].testName, "TESTING::TESTSIX")
  call cclutAssertVCEqual(CURREF, "test 7", reply->tests[7].testName, "NS::TESTSEVEN")
  call cclutAssertVCEqual(CURREF, "test 8", reply->tests[8].testName, "NS::TESTEIGHT")
end ;;;testItA


/**
  Validate that cclut_find_unit_test fails if the program does not exist.
*/
subroutine testItB(null)
  declare stat = i4 with protect, noconstant(0)
  declare cclut::debug = i2 with protect, noconstant(TRUE)
  
  record request (
    1 programName = vc
  ) with protect
  record reply (
    1 tests[*]
      2 testName = vc
%i cclsource:status_block.inc
  ) with protect

  set stat = remove("ut_cclut_find_unit_tests_not.xml")
  set request->programName = "ut_cclut_find_unit_tests_not"
  execute cclut_find_unit_tests with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply), replace("PARSER", mockParser)

  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.subeventstatus[1].operationName, "TRANSLATE                ")
  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.subeventstatus[1].targetObjectName, 
      "ut_cclut_find_unit_tests_")
  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.subeventstatus[1].targetObjectValue, 
      "Failed to translate program. Listing file ccluserdir:ut_cclut_find_unit_tests_not.xml is empty.")
end ;;;testItB

/**
  Validate that cclut_find_unit_test fails if the program translation is not valid xml
*/
subroutine testItC(null)
  record request (
    1 programName = vc
  ) with protect
  record reply (
    1 tests[*]
      2 testName = vc
%i cclsource:status_block.inc
  ) with protect
  
  set request->programName = "ut_cclut_find_unit_tests_not"
  select into "ut_cclut_find_unit_tests_not.xml" from dual detail "<this><is><not><valid><xml><\this>" row+1 with nocounter
  select into 'nl:' x = 0 from dummyt with nocounter ;the file doesn't get written without this.

  execute cclut_find_unit_tests with replace("CCLUTREQUEST", request), replace("CCLUTREPLY", reply), replace("PARSER", mockParser)

  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.status, "F")
  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.subeventstatus[1].operationName, "PARSE                    ")
  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.subeventstatus[1].operationStatus, "F")
  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.subeventstatus[1].targetObjectName, 
      "ut_cclut_find_unit_tests_")
  call cclutAssertVCEqual(CURREF, "expect failure", reply->status_data.subeventstatus[1].targetObjectValue, 
      "Failed to parse xml from listing file ccluserdir:ut_cclut_find_unit_tests_not.xml.")
end ;;;testItC
