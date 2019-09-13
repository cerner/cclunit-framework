declare tearDown(null) = null
declare table_replace_version(null) = null

declare cclutEPWM_happy::cclutExecuteProgramWithMocks(
    pgm = vc (val), params = vc(val ,""), ns = vc(val ,"")) = null
declare cclutEPWM_no_parameters::cclutExecuteProgramWithMocks(
    pgm = vc (val), params = vc(val ,""), ns = vc(val ,"")) = null
declare cclutEPWM_namespace::cclutExecuteProgramWithMocks(
    pgm = vc (val), params = vc(val ,""), ns = vc(val ,"")) = null
declare cclutEPWM_mock_subroutine::cclutExecuteProgramWithMocks(
    pgm = vc (val), params = vc(val ,""), ns = vc(val ,"")) = null
declare cclutEPWM_errors::cclutExecuteProgramWithMocks(
    pgm = vc (val), params = vc(val ,""), ns = vc(val ,"")) = null


declare tdbexecuteA(
    app = i4, task = i4, request = i4, reqType = vc, req = vc(ref), repType = vc, rep = vc(ref)) = i4
declare tdbexecuteB(
    app = i4, task = i4, request = i4, reqType = vc, req = vc(ref), repType = vc, rep = vc(ref)) = i4
declare tdbexecuteC(
    app = i4, task = i4, request = i4, reqType = vc, req = vc(ref), repType = vc, rep = vc(ref)) = i4
declare tdbexecuteD(
    app = i4, task = i4, request = i4, reqType = vc, req = vc(ref), repType = vc, rep = vc(ref)) = i4


declare cclutEPWM_group1_tdb::cclutGetCurrentGroup(null) = i4

declare cclut::internalSubroutine(null) = null with protect
declare cclut::mockSubroutine(null) = null with protect

subroutine setupOnce(null)
    if (reqinfo->updt_id = 0.0)
        call cclutAssertI2Equal(CURREF, "authentication required to run this test case", TRUE, FALSE)
        go to exit_script
    endif
end ;setupOnce

subroutine tearDown(null)
    call cclutRemoveAllMocks(null)
    rollback
end ;tearDown


;**********************************************************************************************************************************
;** cclutAddMockImplementation
;**********************************************************************************************************************************
/* test_cclutAddMockImplementation_happy ********************************************************************
*  Scenario: Adds mock implementations that can be used with cclutExecuteProgramWithMocks                   *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_happy(null)
    call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_happy 001",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 002",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 003",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")

    call cclutAddMockImplementation("second_subroutine", "new_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_happy 004",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 005",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 006",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 007",
        cclut_mockImplementations->implementations[2].originalName, "SECOND_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_happy 008",
        cclut_mockImplementations->implementations[2].replaceName, "NEW_SUBROUTINE")
end ;test_cclutAddMockImplementation_happy

/* test_cclutAddMockImplementation_different_cases **********************************************************
*  Scenario: Adds mock implementations with different cases                                                 *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_different_cases(null)
    call cclutAddMockImplementation("sAmpLE_subROUtine", "MoCK_suBROutiNE")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_different_cases 001",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 002",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 003",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")

    call cclutAddMockImplementation("sEcOnD_sUbRoUtInE", "NeW_sUbRoUtInE")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_different_cases 004",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 005",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 006",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 007",
        cclut_mockImplementations->implementations[2].originalName, "SECOND_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_different_cases 008",
        cclut_mockImplementations->implementations[2].replaceName, "NEW_SUBROUTINE")
end ;test_cclutAddMockImplementation_different_cases

/* test_cclutAddMockImplementation_existing_implementation **************************************************
*  Scenario: Replaces the mock name of an implementation if it was already added                            *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_existing_implementation(null)
	call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_existing_implementation 001",
        size(cclut_mockImplementations->implementations, 5), 1)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 002",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 003",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")

    call cclutAddMockImplementation("second_subroutine", "new_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_existing_implementation 004",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 005",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 006",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 007",
        cclut_mockImplementations->implementations[2].originalName, "SECOND_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 008",
        cclut_mockImplementations->implementations[2].replaceName, "NEW_SUBROUTINE")

    call cclutAddMockImplementation("sample_subroutine", "replace_subroutine")
    call cclutAddMockImplementation("second_subroutine", "test_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutAddMockImplementation_existing_implementation 009",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 010",
        cclut_mockImplementations->implementations[1].originalName, "SAMPLE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 011",
        cclut_mockImplementations->implementations[1].replaceName, "REPLACE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 012",
        cclut_mockImplementations->implementations[2].originalName, "SECOND_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutAddMockImplementation_existing_implementation 013",
        cclut_mockImplementations->implementations[2].replaceName, "TEST_SUBROUTINE")
end ;test_cclutAddMockImplementation_happy

/* test_cclutAddMockImplementation_missing_originalName *****************************************************
*  Scenario: Throws an error when an empty originalName is supplied to the function (after trimming)        *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_missing_originalName(null)
    declare stat = i4 with protect, noconstant(0)
    call cclutAddMockImplementation("     ", "mock_subroutine")
    call cclutAssertI4Equal(CURREF,
        "test_cclutAddMockImplementation_missing_originalName expect one error", size(cclutErrorStash->error, 5), 1)
    call cclutAssertVcEqual(CURREF,
        "test_cclutAddMockImplementation_missing_originalName error message", cclutErrorStash->error[1].errorMessage,
        "cclutAddMockImplementation() - originalName is a required field.")
    set stat = alterlist(cclutErrorStash->error, 0)
end ;test_cclutAddMockImplementation_missing_originalName

/* test_cclutAddMockImplementation_missing_replaceName ******************************************************
*  Scenario: Throws an error when an empty replaceName is supplied to the function (after trimming)         *
************************************************************************************************************/
subroutine test_cclutAddMockImplementation_missing_replaceName(null)
    declare stat = i4 with protect, noconstant(0)
    call cclutAddMockImplementation("sample_subroutine", "     ")
    call cclutAssertI4Equal(CURREF,
        "test_cclutAddMockImplementation_missing_originalName expect one error", size(cclutErrorStash->error, 5), 1)
    call cclutAssertVcEqual(CURREF,
        "test_cclutAddMockImplementation_missing_originalName error message", cclutErrorStash->error[1].errorMessage,
        "cclutAddMockImplementation(SAMPLE_SUBROUTINE) - replaceName is a required field.")
    set stat = alterlist(cclutErrorStash->error, 0)
end ;test_cclutAddMockImplementation_missing_replaceName

;**********************************************************************************************************************************
;** cclutRemoveMockImplementation
;**********************************************************************************************************************************
/* test_cclutRemoveMockImplementation_happy *****************************************************************
*  Scenario: Removes a mock implementation                                                                  *
************************************************************************************************************/
subroutine test_cclutRemoveMockImplementation_happy(null)
    call cclutAddMockImplementation("one_subroutine", "mock_one_subroutine")
    call cclutAddMockImplementation("two_subroutine", "mock_two_subroutine")
    call cclutAddMockImplementation("three_subroutine", "mock_three_subroutine")
    call cclutAddMockImplementation("four_subroutine", "mock_four_subroutine")
    call cclutAddMockImplementation("five_subroutine", "mock_five_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockImplementation_happy 001",
        size(cclut_mockImplementations->implementations, 5), 5)
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 002",
        cclut_mockImplementations->implementations[1].originalName, "ONE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 003",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_ONE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 004",
        cclut_mockImplementations->implementations[2].originalName, "TWO_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 005",
        cclut_mockImplementations->implementations[2].replaceName, "MOCK_TWO_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 006",
        cclut_mockImplementations->implementations[3].originalName, "THREE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 007",
        cclut_mockImplementations->implementations[3].replaceName, "MOCK_THREE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 008",
        cclut_mockImplementations->implementations[4].originalName, "FOUR_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 009",
        cclut_mockImplementations->implementations[4].replaceName, "MOCK_FOUR_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 010",
        cclut_mockImplementations->implementations[5].originalName, "FIVE_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 011",
        cclut_mockImplementations->implementations[5].replaceName, "MOCK_FIVE_SUBROUTINE")

    call cclutRemoveMockImplementation("one_subroutine")
    call cclutRemoveMockImplementation("three_subroutine")
    call cclutRemoveMockImplementation("five_subroutine")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveMockImplementation_happy 012",
        size(cclut_mockImplementations->implementations, 5), 2)
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 013",
        cclut_mockImplementations->implementations[1].originalName, "TWO_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 014",
        cclut_mockImplementations->implementations[1].replaceName, "MOCK_TWO_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 015",
        cclut_mockImplementations->implementations[2].originalName, "FOUR_SUBROUTINE")
    call cclutAssertvcEqual(CURREF, "test_cclutRemoveMockImplementation_happy 016",
        cclut_mockImplementations->implementations[2].replaceName, "MOCK_FOUR_SUBROUTINE")
end ;test_cclutRemoveMockImplementation_happy

/* test_cclutRemoveMockImplementation_missing_originalName **************************************************
*  Scenario: Throws an error when an empty originalName is supplied to the function (after trimming)        *
************************************************************************************************************/
subroutine test_cclutRemoveMockImplementation_missing_originalName(null)
    declare stat = i4 with protect, noconstant(0)
    call cclutRemoveMockImplementation("     ")
    call cclutAssertI4Equal(CURREF,
        "test_cclutAddMockImplementation_missing_originalName expect one error", size(cclutErrorStash->error, 5), 1)
    call cclutAssertVcEqual(CURREF,
        "test_cclutAddMockImplementation_missing_originalName error message", cclutErrorStash->error[1].errorMessage,
        "cclutRemoveMockImplementation() - originalName is a required field.")
    set stat = alterlist(cclutErrorStash->error, 0)
end ;test_cclutRemoveMockImplementation_missing_originalName

;**********************************************************************************************************************************
;** cclutRemoveAllMockImplementations
;**********************************************************************************************************************************
/* test_cclutRemoveAllMockImplementations_happy *************************************************************
*  Scenario: Removes all mock implementations                                                               *
************************************************************************************************************/
subroutine test_cclutRemoveAllMockImplementations_happy(null)
    call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")
    call cclutAddMockImplementation("sample_subroutine_2", "mock_subroutine_2")
    call cclutAddMockImplementation("sample_subroutine_3", "mock_subroutine_3")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMockImplementations_happy 001",
        size(cclut_mockImplementations->implementations, 5), 3)

    call cclutRemoveAllMockImplementations(null)

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMockImplementations_happy 002",
        size(cclut_mockImplementations->implementations, 5), 0)
end ;test_cclutRemoveAllMockImplementations_happy


/**
    Common code for all test_cclutExecuteProgramWithMocks_* unit tests.
*/
subroutine common_test_cclutExecuteProgramWithMocks(null)
    declare stat = i4 with protect, noconstant(0)
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

    record ucetcf_testCaseResults (
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

    set ucetcf_request->testCaseDirectory = "cclsource"
    set ucetcf_request->testCaseFileName = "ut_ut_cclut_emh"
    if (validate(etcfKickoff) = FALSE)
        execute cclut_execute_test_case_file with
        replace("CCLUTREQUEST", ucetcf_request, 0),
        replace("CCLUTREPLY", ucetcf_reply, 0),
        replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults, 0),
        curnamespace = testNamespace
    else
        call parser(concat("call ", etcfKickoff, "(null) go"))
    endif
    ;uncomment the following echorecord if test results are confusing or doubted.
    ;call echorecord(ucetcf_testCaseResults)
    if (validate(expectTestFailure, FALSE) = TRUE)
        call cclutAssertI2Equal(CURREF, "check resultInd", ucetcf_testCaseResults->resultInd, 0)
    else
        call cclutAssertI2Equal(CURREF, "check resultInd", ucetcf_testCaseResults->resultInd, 1)
    endif
    if (validate(callPostProcess, FALSE) = TRUE)
        call parser(concat("call ", testNamespace, "::postProcess", "(null) go"))
    endif
end ;common_test_cclutExecuteProgramWithMocks


/**
    Verifies that cclutExecuteProgramWithMocks applies mock tables and implementations when executing a program
    with parameters and no namespace.
*/
subroutine test_cclutExecuteProgramWithMocks_happy(null)
    declare executeParams = vc with protect, constant("10,^test param^")
    declare executeNamespace = vc with protect, constant("")
    declare testNameSpace = vc with protect, constant("cclutEPWM_happy")
    call common_test_cclutExecuteProgramWithMocks(null)
end ;test_cclutExecuteProgramWithMocks_happy
subroutine cclutEPWM_happy::cclutExecuteProgramWithMocks(pgm, params, ns)
    call cclutAssertVcEqual(CURREF, "check program name", pgm, "ut_cclut_execute_mocks_happy")
    call cclutAssertVcEqual(CURREF, "check params", params, "10,^test param^")
    call cclutAssertVcEqual(CURREF, "check namespace", ns, "")
    call public::cclutExecuteProgramWithMocks(pgm, params)
end ;test_cclutExecuteProgramWithMocks_happy::cclutExecuteProgramWithMocks
subroutine cclutEPWM_happy::unitTestSpecificValidations(null)
    call cclutAssertVcEqual(CURREF, "check internalSubroutineSource", executeReply->internalSubroutineSource, "public")
end ;test_cclutExecuteProgramWithMocks_happy::unitTestSpecificValidations


/**
    Verifies that cclutExecuteProgramWithMocks applies mock tables and implementations when executing a program
    with no parameters and no namespace.
*/
subroutine test_cclutExecuteProgramWithMocks_no_parameters(null)
    declare executeParams = vc with protect, constant("")
    declare executeNamespace = vc with protect, constant("")
    declare testNameSpace = vc with protect, constant("cclutEPWM_no_parameters")
    call common_test_cclutExecuteProgramWithMocks(null)
end ;test_cclutExecuteProgramWithMocks_no_parameters
subroutine cclutEPWM_no_parameters::cclutExecuteProgramWithMocks(pgm, params, ns)
    call cclutAssertVcEqual(CURREF, "check program name", pgm, "ut_cclut_execute_mocks_happy")
    call cclutAssertVcEqual(CURREF, "check params", params, "")
    call cclutAssertVcEqual(CURREF, "check namespace", ns, "")
    call public::cclutExecuteProgramWithMocks(pgm, params)
end ;test_cclutExecuteProgramWithMocks_no_parameters::cclutExecuteProgramWithMocks
subroutine cclutEPWM_no_parameters::unitTestSpecificValidations(null)
    call cclutAssertVcEqual(CURREF, "check internalSubroutineSource", executeReply->internalSubroutineSource, "public")
end ;testtest_cclutExecuteProgramWithMocks_no_parameters::unitTestSpecificValidations


/**
    Verifies that cclutExecuteProgramWithMocks applies mock tables and implementations when executing a program
    with a namespace but no parameters.
*/
subroutine test_cclutExecuteProgramWithMocks_namespace(null)
    declare cclut::internalVariable = i4 with protect, noconstant(0)
    record cclut::internalRecord (
        1 item = i4
    ) with protect

    declare executeParams = vc with protect, constant("")
    declare executeNamespace = vc with protect, constant("cclut")
    declare testNameSpace = vc with protect, constant("cclutEPWM_namespace")
    call common_test_cclutExecuteProgramWithMocks(null)
end ;test_cclutExecuteProgramWithMocks_namespace
subroutine cclut::internalSubroutine(null)
    ;executeReply rather than reply b/c "with replace" will not act on a reference to reply in this location as it is not
    ;part of the compiled ut_cclut_execute_mocks_happy program.
    set executeReply->internalSubroutineSource = "cclut::internalSubroutine"
end
subroutine cclutEPWM_namespace::cclutExecuteProgramWithMocks(pgm, params, ns)
    call cclutAssertVcEqual(CURREF, "check program name", pgm, "ut_cclut_execute_mocks_happy")
    call cclutAssertVcEqual(CURREF, "check params", params, "")
    call cclutAssertVcEqual(CURREF, "check namespace", ns, "cclut")
    call public::cclutExecuteProgramWithMocks(pgm, params, ns)
end ;test_cclutExecuteProgramWithMocks_namespace::cclutExecuteProgramWithMocks
subroutine cclutEPWM_namespace::execute_mocks_happy_validations(null)
    call cclutAssertVcEqual(CURREF, "check internalSubroutineSource",
        executeReply->internalSubroutineSource, "cclut::internalSubroutine")
    call cclutAssertI4Equal(CURREF, "check cclut::internalVariable", cclut::internalVariable, 1)
    call cclutAsserti4Equal(CURREF, "check cclut::internalRecord", cclut::internalRecord->item, 1)
    call cclutAssertVcEqual(CURREF, "check internalSubroutineSource",
        executeReply->internalSubroutineSource, "cclut::internalSubroutine")
end ;execute_mocks_happy_validations


/**
    Confirms that cclutExecuteProgramWithMocks can perform a mock substitution on a subroutine.
*/
subroutine test_cclutExecuteProgramWithMocks_mock_subroutine(null)
    declare cclut::internalVariable = i4 with protect, noconstant(0)
    record cclut::internalRecord (
        1 item = i4
    ) with protect

    declare executeParams = vc with protect, constant("10,^test param^")
    declare executeNamespace = vc with protect, constant("cclut")
    declare testNameSpace = vc with protect, constant("cclutEPWM_mock_subroutine")
    call common_test_cclutExecuteProgramWithMocks(null)
end ;test_cclutExecuteProgramWithMocks_mock_subroutine
subroutine cclut::mockSubroutine(null)
    ;executeReply rather than reply b/c "with replace" will not act on a reference to reply in this location as it is not
    ;part of the compiled ut_cclut_execute_mocks_happy program.
    set executeReply->internalSubroutineSource = concat(executeReply->internalSubroutineSource, "cclut::mockSubroutine")
end
subroutine cclutEPWM_mock_subroutine::addUnitTestSpecificMocks(null)
    ;this must be done within the cclutEPWM_mock_subroutine
    ;otherwise the mock gets adeded to cclut_mockImplementations not cclutEPWM_mock_subroutine::cclut_mockImplementations
    call cclutAddMockImplementation("internalSubroutine", "mockSubroutine")
end ;cclutEPWM_mock_subroutine::addUnitTestSpecificMocks
subroutine cclutEPWM_mock_subroutine::cclutExecuteProgramWithMocks(pgm, params, ns)
    call cclutAssertVcEqual(CURREF, "check program name", pgm, "ut_cclut_execute_mocks_happy")
    call cclutAssertVcEqual(CURREF, "check params", params, "10,^test param^")
    call cclutAssertVcEqual(CURREF, "check namespace", ns, "cclut")
    call public::cclutExecuteProgramWithMocks(pgm, params, ns)
end ;test_cclutExecuteProgramWithMocks_namespace::cclutExecuteProgramWithMocks
subroutine cclutEPWM_mock_subroutine::execute_mocks_happy_validations(null)
    call cclutAssertI4Equal(CURREF, "check cclut::internalVariable", cclut::internalVariable, 1)
    call cclutAsserti4Equal(CURREF, "check cclut::internalRecord", cclut::internalRecord->item, 1)
    call cclutAssertVcEqual(CURREF, "check internalSubroutineSource",
        executeReply->internalSubroutineSource, "cclut::mockSubroutine")
end ;execute_mocks_happy_validations


/**
    Confirms that mock tables get dropped for a group1 user.

    Note that in conjunction with test_mock_tables_dropped_via_tdbexecute_for_group1_user this test demonstrates that the
    tdbexecute call gets performed successfully.
*/
subroutine test_mock_tables_dropped_for_group1_user(null)
    declare idx = i4 with protect, noconstant(0)
    record mockTables(1 item[*] 2 name = vc)

    declare executeParams = vc with protect, constant("10,^test param^")
    declare executeNamespace = vc with protect, constant("")
    declare testNameSpace = vc with protect, constant("cclutEPWM_group1_drop")

    call common_test_cclutExecuteProgramWithMocks(null)

    select into 'nl:' from dprotect dp where dp.platform = 'H0000' and dp.rcode = '5' and dp.group = 0 and dp.object = 'T' and
    expand(idx, 1, size(mockTables->item, 5), dp.object_name, mockTables->item[idx].name)
    with nocounter
    call cclutAssertI4Equal(CURREF, "check all tables dropped", CURQUAL, 0)
end; test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_drop::cclutGetCurrentGroup(null)
    return (1)
end ;test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_drop::execute_mocks_happy_validations(null)
    declare idx = i4 with protect, noconstant(0)
    declare stat = i4 with protect, noconstant(0)
    declare mockTableCount = i4 with protect, noconstant(0)
    set mockTableCount = size(cclutEPWM_group1_drop::cclut_mockTables->tables, 5)
    set stat = alterlist(mockTables->item, mockTableCount)
    for (idx = 1 to mockTableCount)
        set mockTables->item[idx].name = cclutEPWM_group1_drop::cclut_mockTables->tables[idx].replaceName
    endfor
end ;execute_mocks_happy_validations

/**
    common startup code for group1 table drop exception tests.
*/
subroutine group1_mock_table_test(null)
    record mockTables(1 item[*] 2 name = vc)
    declare executeParams = vc with protect, constant("10,^test param^")
    declare executeNamespace = vc with protect, constant("")
    declare etcfKickoff = vc with protect, constant(concat(testNamespace, "::etcfKickoff"))

    record errors(1 item[*] 2 code = i4 2 message = vc) with protect
    declare errorMessage = vc with protect, noconstant("")
    declare errorCode = i4 with protect, noconstant(0)
    declare errorCount = i4 with protect, noconstant(0)
    declare stat = i4 with protect, noconstant(0)

    call common_test_cclutExecuteProgramWithMocks(null)

    call group1_mock_table_cleanup(null)
end ;group1_mock_table_cleanup

/**
    Common table collection code for group1 table drop exception tests.
    This must occur in the context of the test's namespace since <namespace>::cclut_mockTables contains the data.
*/
subroutine group1_collect_tables(null)
    declare idx = i4 with protect, noconstant(0)
    declare stat = i4 with protect, noconstant(0)
    declare mockTableCount = i4 with protect, noconstant(0)
    set mockTableCount = size(cclut_mockTables->tables, 5)
    set stat = alterlist(mockTables->item, mockTableCount)
    for (idx = 1 to mockTableCount)
        set mockTables->item[idx].name = cclut_mockTables->tables[idx].replaceName
    endfor
end

/**
    Common cleanup code for group1 table drop exception tests.
*/
subroutine group1_mock_table_cleanup(null)
    declare idx = i4 with protect, noconstant(0)
    declare mockTableCount = i4 with protect, noconstant(0)
    set mockTableCount = size(mockTables->item, 5)
    call cclutAssertI4GreaterThan(CURREF, "mock table count", mockTableCount, 0)
    select into 'nl:' from dprotect dp where dp.platform = 'H0000' and dp.rcode = '5' and dp.group = 0 and dp.object = 'T' and
    expand(idx, 1, size(mockTables->item, 5), dp.object_name, mockTables->item[idx].name)
    with nocounter
    call cclutAssertI4Equal(CURREF, "no tables dropped", CURQUAL, mockTableCount)
    ;clean up the mess
    for (idx = 1 to mockTableCount)
        call parser(concat("drop table ", mockTables->item[idx].name, " go"))
    endfor
end ;group1_mock_table_cleanup


/**
    Confirms that tdbexecute is used to drop mock tables for a group1 user.
*/
subroutine test_mock_tables_dropped_via_tdbexecute_for_group1_user(null)
    record cclut_drop_table_reply(1 document = gvc 1 status_data 2 status = c1)
    set cclut_drop_table_reply->status_data.status = "Z"
    set cclut_drop_table_reply->document = ^{"CCLUTDROPMOCKTABLEREPLY": {"STATUS": "S", "MESSAGE": "" }}^
    declare testNamespace = vc with protect, constant("cclutEPWM_group1_tdb")
end; test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_tdb::cclutGetCurrentGroup(null)
    return (1)
end ;test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_tdb::etcfKickoff(null)
    execute cclut_execute_test_case_file with
        replace("CCLUTREQUEST", ucetcf_request, 0),
        replace("CCLUTREPLY", ucetcf_reply, 0),
        replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults, 0),
        replace("TDBEXECUTE", tdbexecuteA),
        curnamespace = testNamespace
end
subroutine tdbexecuteA(app, task, request, reqType, req, repType, rep)
    call cclutAssertI4Equal(CURREF, "app number", app, 3202004)
    call cclutAssertI4Equal(CURREF, "task number", task, 3202004)
    call cclutAssertI4Equal(CURREF, "request number", request, 3050002)
    call cclutAssertVcEqual(CURREF, "reqType", reqType, "REC")
    call cclutAssertVcEqual(CURREF, "repType", repType, "REC")
    call cclutAssertVcEqual(CURREF, "req->program_name", req->program_name, "cclut_drop_mock_table_def")
    call cclutAssertVcEqual(CURREF, "req->output_device", req->output_device, "MINE")
    call cclutAssertI4Equal(CURREF, "req->isBlog", req->isBlob, 1)
    call cclutAssertStartsWith(CURREF, "req->params", "^MINE^,^CUST_CCLUT_", req->params)
    return (0)
end ;tdbexecuteA
subroutine cclutEPWM_group1_tdb::execute_mocks_happy_validations(null)
    call group1_collect_tables(null)
end ;execute_mocks_happy_validations


/**
    Confirms the exceptions raised when a failure occurs in the tdbexecute call used to drop mock tables for a group1 user.
*/
subroutine test_tdbexecute_error_during_group1_user_table_drop(null)
    declare testNamespace = vc with protect, constant("cclutEPWM_group1_tdb_error")
    declare expectTestFailure = i2 with protect, constant(TRUE)
    declare callPostProcess = i2 with protect, constant(TRUE)
    record cclut_drop_table_reply(1 document = gvc 1 status_data 2 status = c1)
    call group1_mock_table_test(null)
end; test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_tdb_error::cclutGetCurrentGroup(null)
    return (1)
end ;test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_tdb_error::etcfKickoff(null)
    execute cclut_execute_test_case_file with
        replace("CCLUTREQUEST", ucetcf_request, 0),
        replace("CCLUTREPLY", ucetcf_reply, 0),
        replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults, 0),
        replace("TDBEXECUTE", tdbexecuteB),
        curnamespace = testNamespace
end
subroutine tdbexecuteB(app, task, request, reqType, req, repType, rep)
    declare retVal = i4 with protect, noconstant(0)
    set retVal = rand(curtime)
    set retVal = 1 + mod(rand(0), 25)
    return (retVal)
end ;tdbexecuteB
subroutine cclutEPWM_group1_tdb_error::execute_mocks_happy_validations(null)
    call group1_collect_tables(null)
end ;execute_mocks_happy_validations
subroutine cclutEPWM_group1_tdb_error::postProcess(null)
    declare idx = i4 with protect, noconstant(0)
    call cclutAssertI4Equal(CURREF, "cclutEPWM_group1_tdb_error error count", size(ucetcf_testCaseResults->tests[2]->errors, 5), 12)

    for (idx = 1 to 11 by 2)
        call cclutAssertVcOperator(CURREF, "tdbexecute failure", ucetcf_testCaseResults->tests[2]->errors[idx].errorText,
            "regexplike", "TDBEXECUTE error \(status = [0-9]+\)")
    endfor
    for (idx = 2 to 12 by 2)
        call cclutAssertVcOperator(CURREF, "tdbexecute failure message", ucetcf_testCaseResults->tests[2]->errors[idx].errorText,
            "regexplike", concat("cclutRemoveMockTable\([A-Z_]+\) - Unable to drop table.  ",
            "Manual cleanup may be required.  Mock table name is CUST_CCLUT.[A-Z_0-9]+"))
    endfor
end ;postProcess


/**
    Confirms the exceptions raised if vccl_run_program fails when trying to drop mock tables for a group1 user.
*/
subroutine test_run_program_fails_during_group1_user_table_drop(null)
    declare testNamespace = vc with protect, constant("cclutEPWM_group1_vrp_fail")
    declare expectTestFailure = i2 with protect, constant(TRUE)
    declare callPostProcess = i2 with protect, constant(TRUE)
    record cclut_drop_table_reply(1 document = gvc 1 status_data 2 status = c1)
    set cclut_drop_table_reply->status_data.status = "F"
    call group1_mock_table_test(null)
end; test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_vrp_fail::cclutGetCurrentGroup(null)
    return (1)
end ;test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_vrp_fail::etcfKickoff(null)
    execute cclut_execute_test_case_file with
        replace("CCLUTREQUEST", ucetcf_request, 0),
        replace("CCLUTREPLY", ucetcf_reply, 0),
        replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults, 0),
        replace("TDBEXECUTE", tdbexecuteC),
        curnamespace = testNamespace
end
subroutine tdbexecuteC(app, task, request, reqType, req, repType, rep)
    return (0)
end ;tdbexecuteC
subroutine cclutEPWM_group1_vrp_fail::execute_mocks_happy_validations(null)
    call group1_collect_tables(null)
end ;execute_mocks_happy_validations
subroutine cclutEPWM_group1_vrp_fail::postProcess(null)
    declare idx = i4 with protect, noconstant(0)
    call cclutAssertI4Equal(CURREF,
        "test_run_program_fails_during_group1_user_table_drop", size(ucetcf_testCaseResults->tests[2]->errors, 5), 12)

    for (idx = 1 to 11 by 2)
        call cclutAssertVcEqual(CURREF, "vrp failure", ucetcf_testCaseResults->tests[2]->errors[idx].errorText,
            "VCCL_RUN_PROGRAM failure.")
    endfor
    for (idx = 2 to 12 by 2)
        call cclutAssertVcOperator(CURREF, "tdbexecute failure message", ucetcf_testCaseResults->tests[2]->errors[idx].errorText,
            "regexplike", concat("cclutRemoveMockTable\([A-Z_]+\) - Unable to drop table.  ",
            "Manual cleanup may be required.  Mock table name is CUST_CCLUT.[A-Z_0-9]+"))
    endfor
end ;postProcess


/**
    Confirms the exceptions raised if cclut_drop_mock_table_def fails to drop mock tables for a group1 user.
*/
subroutine test_script_fails_during_group1_user_table_drop(null)
    declare testNamespace = vc with protect, constant("cclutEPWM_group1_script_fail")
    declare expectTestFailure = i2 with protect, constant(TRUE)
    declare callPostProcess = i2 with protect, constant(TRUE)
    record cclut_drop_table_reply(1 document = gvc 1 status_data 2 status = c1)
    set cclut_drop_table_reply->status_data.status = "Z"
    set cclut_drop_table_reply->document = ^{"CCLUTDROPMOCKTABLEREPLY": {"STATUS": "F", "MESSAGE": "table not found" }}^
    call group1_mock_table_test(null)
end; test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_script_fail::cclutGetCurrentGroup(null)
    return (1)
end ;test_group1_user_can_drop_mocked_table
subroutine cclutEPWM_group1_script_fail::etcfKickoff(null)
    execute cclut_execute_test_case_file with
        replace("CCLUTREQUEST", ucetcf_request, 0),
        replace("CCLUTREPLY", ucetcf_reply, 0),
        replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults, 0),
        replace("TDBEXECUTE", tdbexecuteC),
        curnamespace = testNamespace
end
subroutine tdbexecuteD(app, task, request, reqType, req, repType, rep)
    return (0)
end ;tdbexecuteD
subroutine cclutEPWM_group1_script_fail::execute_mocks_happy_validations(null)
    call group1_collect_tables(null)
end ;execute_mocks_happy_validations
subroutine cclutEPWM_group1_script_fail::postProcess(null)
    declare idx = i4 with protect, noconstant(0)
    call cclutAssertI4Equal(CURREF,
        "test_script_fails_during_group1_user_table_drop", size(ucetcf_testCaseResults->tests[2]->errors, 5), 12)

    for (idx = 1 to 11 by 2)
        call cclutAssertVcEqual(CURREF, "script failure", ucetcf_testCaseResults->tests[2]->errors[idx].errorText,
            "table not found")
    endfor
    for (idx = 2 to 12 by 2)
        call cclutAssertVcOperator(CURREF, "tdbexecute failure message", ucetcf_testCaseResults->tests[2]->errors[idx].errorText,
            "regexplike", concat("cclutRemoveMockTable\([A-Z_]+\) - Unable to drop table.  ",
            "Manual cleanup may be required.  Mock table name is CUST_CCLUT.[A-Z_0-9]+"))
    endfor
end ;postProcess


/**
    Verifies that cclutExecuteProgramWithMocks applies replace option 0 when applying mock table substitutions
    if the CCL revision is 9 and the CCL version is less than 9.3.0.
*/
subroutine test_table_replace_pre_9_3(null)
    declare testver = i4 with protect, constant(90299)
    declare testNameSpace = vc with protect, constant("test_table_replace_pre_9_3")
    call table_replace_version(null)
end ;test_table_replace_pre_9_3
subroutine test_table_replace_pre_9_3::cclutGetCurrentRevision(null)
    return (9)
end
subroutine test_table_replace_pre_9_3::cclutGetCurrentVersion(null)
    return (90299)
end
subroutine test_table_replace_pre_9_3::cclutInvokeParser(strParser)
    call cclutAssertVcOperator(CURREF, "test_table_replace_pre_9_3 parser",
        strParser, "regexplike", ^replace\("SAMPLE_TABLE", CUST_CCLUT_[a-zA-Z0-9]*, 0\)^)
    call parser(strParser)
end

/**
    Verifies that cclutExecuteProgramWithMocks applies replace option 6 when applying mock table substitutions
    if the CCL revision is 9 and the CCL version is 9.3.0.
*/
subroutine test_table_replace_9_3(null)
    declare testver = i4 with protect, constant(90300)
    declare testNameSpace = vc with protect, constant("test_table_replace_9_3")
    call table_replace_version(null)
end ;test_table_replace_9_3
subroutine test_table_replace_9_3::cclutGetCurrentVersion(null)
    return (90300)
end
subroutine test_table_replace_9_3::cclutInvokeParser(strParser)
    call cclutAssertVcOperator(CURREF, "test_table_replace_9_3 parser",
        strParser, "regexplike", ^replace\("SAMPLE_TABLE", CUST_CCLUT_[a-zA-Z0-9]*, 6\)^)
    call parser(strParser)
end

/**
    Verifies that cclutExecuteProgramWithMocks applies replace option 0 when applying mock table substitutions
    if the CCL revision is 8 and the CCL version is less than 8.15.0.
*/
subroutine test_table_replace_pre_8_1_5(null)
    declare testver = i4 with protect, constant(81499)
    declare testNameSpace = vc with protect, constant("test_table_replace_pre_8_1_5")
    call table_replace_version(null)
end ;test_table_replace_pre_8_1_5
subroutine test_table_replace_pre_8_1_5::cclutGetCurrentRevision(null)
    return (8)
end
subroutine test_table_replace_pre_8_1_5::cclutGetCurrentVersion(null)
    return (81499)
end
subroutine test_table_replace_pre_8_1_5::cclutInvokeParser(strParser)
    call cclutAssertVcOperator(CURREF, "test_table_replace_pre_8_1_5 parser",
        strParser, "regexplike", ^replace\("SAMPLE_TABLE", CUST_CCLUT_[a-zA-Z0-9]*, 0\)^)
    call parser(strParser)
end

/**
    Verifies that cclutExecuteProgramWithMocks applies replace option 6 when applying mock table substitutions
    if the CCL revision is 8 and the CCL version is 8.15.0.
*/
subroutine test_table_replace_8_1_5(null)
    declare testver = i4 with protect, constant(81500)
    declare testNameSpace = vc with protect, constant("test_table_replace_8_1_5")
    call table_replace_version(null)
end ;test_table_replace_8_1_5
subroutine test_table_replace_pre_8_1_5::cclutGetCurrentRevision(null)
    return (8)
end
subroutine test_table_replace_pre_8_1_5::cclutGetCurrentVersion(null)
    return (81500)
end
subroutine test_table_replace_8_1_5::cclutInvokeParser(strParser)
    call cclutAssertVcOperator(CURREF, "test_table_replace_8_1_5 parser",
        strParser, "regexplike", ^replace\("SAMPLE_TABLE", CUST_CCLUT_[a-zA-Z0-9]*, 6\)^)
    call parser(strParser)
end


/**
  Common code for the test_table_replace_* tests
*/
subroutine table_replace_version(null)
    if (curcclver < 81500 or (currev = 9 and curcclver < 90300))
        ;skip this test because the current CCL version is too old to test new functionality.
        call echo(build2("skipping test ", cclut::testName, " - the CCL version is ", curcclver)) ;intentional
        return (null)
    endif

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

    record ucetcf_testCaseResults (
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


    record cclut_testRecord (1 tableName = vc 1 value = vc)
    set ucetcf_request->testCaseDirectory = "cclsource"
    set ucetcf_request->testCaseFileName = "ut_ut_cclut_table_column_name"
        execute cclut_execute_test_case_file with
        replace("CCLUTREQUEST", ucetcf_request, 0),
        replace("CCLUTREPLY", ucetcf_reply, 0),
        replace("CCLUTTESTCASERESULTS", ucetcf_testCaseResults, 0),
        curnamespace = testNameSpace

    call cclutAssertI2Equal(CURREF, "test count", size(ucetcf_testCaseResults->tests, 5), 2)
    call cclutAssertI2Equal(CURREF, "assert count", size(ucetcf_testCaseResults->tests[2].asserts, 5), 2)
    call cclutAssertVcEqual(CURREF, "assert context 1",
        ucetcf_testCaseResults->tests[2].asserts[1].context, concat(testNameSpace, " parser"))
    call cclutAssertVcEqual(CURREF, "assert context 2", ucetcf_testCaseResults->tests[2].asserts[2].context, "expected value")
    call cclutAssertStartsWith(CURREF, "assert condition 1",
        ~operator('execute UT_CCLUT_TABLE_COLUMN_NAME with replace("SAMPLE_TABLE", CUST_CCLUT_~,
        ucetcf_testCaseResults->tests[2].asserts[1].condition)
    call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[2].asserts[1].resultInd, TRUE)

    case(testver)
      of 81499:
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 1)
        call cclutAssertVcEqual(CURREF, "assert success 1",
            ucetcf_testCaseResults->tests[2].asserts[2].condition, ~"" = "sample_value"~)
        call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[2].asserts[2].resultInd, FALSE)
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 1)
        call cclutAssertVcOperator(CURREF, "error message", ucetcf_testCaseResults->tests[2].errors[1].errorText ,"regexplike",
            concat("%CCL-E-26-UT_CCLUT_TABLE_COLUMN_NAME\([^)]+\)[0-9]+:[0-9]+\{ST.",
                cclut_testRecord->tableName, "}The attribute \(", cclut_testRecord->tableName,
                "\) does not exist in the table \(ST\)."))
      of 81500:
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 0)
        call cclutAssertVcEqual(CURREF, "assert success 1",
            ucetcf_testCaseResults->tests[2].asserts[2].condition, ~"sample_value" = "sample_value"~)
        call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 0)
      of 90299:
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 1)
        call cclutAssertVcEqual(CURREF, "assert success 1",
            ucetcf_testCaseResults->tests[2].asserts[2].condition, ~"" = "sample_value"~)
        call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[2].asserts[2].resultInd, FALSE)
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 1)
        call cclutAssertVcOperator(CURREF, "error message", ucetcf_testCaseResults->tests[2].errors[1].errorText ,"regexplike",
            concat("%CCL-E-26-UT_CCLUT_TABLE_COLUMN_NAME\([^)]+\)[0-9]+:[0-9]+\{ST.",
                cclut_testRecord->tableName, "}The attribute \(", cclut_testRecord->tableName,
                "\) does not exist in the table \(ST\)."))
      of 90300:
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 0)
        call cclutAssertVcEqual(CURREF, "assert success 1",
            ucetcf_testCaseResults->tests[2].asserts[2].condition, ~"sample_value" = "sample_value"~)
        call cclutAssertI2Equal(CURREF, "assert success 1", ucetcf_testCaseResults->tests[2].asserts[2].resultInd, TRUE)
        call cclutAssertI2Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2].errors, 5), 0)
      else
        call cclutAssertI2Equal(CURREF, "unexpected testver", TRUE, FALSE)
    endcase

end ;table_replace_version

/**
    Confirms that a CCL exception is raised if the program name passed to cclutExecuteProgramWithMocks trims to empty.
*/
subroutine test_cclutExecuteProgramWithMocks_missing_programName(null)
    declare stat = i4 with protect, noconstant(0)
    call cclutExecuteProgramWithMocks("     ", "")
    call cclutAssertI4Equal(CURREF,
        "test_cclutAddMockImplementation_missing_originalName expect one error", size(cclutErrorStash->error, 5), 1)
    call cclutAssertVcEqual(CURREF,
        "test_cclutAddMockImplementation_missing_originalName error message", cclutErrorStash->error[1].errorMessage,
        "cclutExecuteProgramWithMocks() - programName is a required field.")
    set stat = alterlist(cclutErrorStash->error, 0)
end ;test_cclutExecuteProgramWithMocks_missing_programName


/**
    Confirms that cclutRemoveAllMocks removes all mock tables and implementations.
*/
subroutine test_cclutRemoveAllMocks_happy(null)
    call cclutDefineMockTable("sample_table", "sample_table_id|sample_table_text|sample_table_date", "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_2", "sample_table_2_id|sample_table_2_text|sample_table_2_date",
        "f8|vc|dq8")
    call cclutDefineMockTable("sample_table_3", "sample_table_3_id|sample_table_3_text|sample_table_3_date",
        "f8|vc|dq8")

    call cclutCreateMockTable("sample_table")

    call cclutAddMockImplementation("sample_subroutine", "mock_subroutine")
    call cclutAddMockImplementation("sample_subroutine_2", "mock_subroutine_2")
    call cclutAddMockImplementation("sample_subroutine_3", "mock_subroutine_3")

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy 001",
        size(cclut_mockTables->tables, 5), 3)
    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy 002",
        size(cclut_mockImplementations->implementations, 5), 3)

    call cclutRemoveAllMocks(null)

    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy 003", size(cclut_mockTables->tables, 5), 0)
    call cclutAsserti4Equal(CURREF, "test_cclutRemoveAllMocks_happy 004",
        size(cclut_mockImplementations->implementations, 5), 0)
end ;test_cclutRemoveAllMocks_happy

/**
    Verifies that errors generated by cclutExecuteProgramWithMocks get incorporated in the test results.
*/
subroutine test_cclutExecuteProgramWithMocks_errors(null)
    declare executeParams = vc with protect, constant("10,^test param^")
    declare executeNamespace = vc with protect, constant("")
    declare testNameSpace = vc with protect, constant("cclutEPWM_errors")
    declare expectTestFailure = i2 with protect, constant(TRUE)
    declare callPostProcess = i2 with protect, constant(TRUE)
    call common_test_cclutExecuteProgramWithMocks(null)
end ;testExecuteProgramWithMocksErrors
subroutine cclutEPWM_errors::cclutExecuteProgramWithMocks(pgm, params, ns)
    call cclexception(555, "E", "forced CCL error")
    call cclexception(556, "E", "forced CCL error")
    call cclexception(557, "E", "forced CCL error")
    call public::cclutExecuteProgramWithMocks(pgm, params)
end ;testExecuteProgramWithMocksErrors::cclutExecuteProgramWithMocks
subroutine cclutEPWM_errors::execute_mocks_happy_validations(null)
  null
end ;execute_mocks_happy_validations
subroutine cclutEPWM_errors::postProcess(null)
    call cclutAssertI4Equal(CURREF, "test count", size(ucetcf_testCaseResults->tests, 5), 2)
    call cclutAssertI4Equal(CURREF, "error count", size(ucetcf_testCaseResults->tests[2]->errors, 5), 3)
    call cclutAssertContains(
        CURREF, "error message one", "Exception(557): forced CCL error.", ucetcf_testCaseResults->tests[2]->errors[1].errorText)
    call cclutAssertContains(
        CURREF, "error message two", "Exception(556): forced CCL error.", ucetcf_testCaseResults->tests[2]->errors[2].errorText)
    call cclutAssertContains(
        CURREF, "error message three", "Exception(555): forced CCL error.", ucetcf_testCaseResults->tests[2]->errors[3].errorText)
end ;test_cclutExecuteProgramWithMocks_happy::unitTestSpecificValidations
