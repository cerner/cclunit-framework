/**
    Unit tests for ut_cclut_execute_mocks_happy used for unit testing the mocking framework.
*/
declare teardown(null) = null
declare test_ut_cclut_execute_mocks_happy_basic(null) = null with protect
declare public::execute_mocks_happy_validations(null) = null with protect

subroutine teardown(null)
    if (cclutGetCurrentGroup(null) != 0 and reqinfo->updt_id = 0.0)
        call cclutAssertI2Equal(CURREF, "authentication required to run this test", TRUE, FALSE)
        return(null)
    endif
    call cclutRemoveAllMocks(null)
end

subroutine test_ut_cclut_execute_mocks_happy_basic(null)
;    if (cclutGetCurrentGroup(null) = 1 and reqinfo->updt_id = 0.0)
;        call cclutAssertI2Equal(CURREF, "authentication required to run this test", TRUE, FALSE)
;        return(null)
;    endif
    declare mock_sample_person = vc with protect, noconstant("")
    declare mock_sample_encounter = vc with protect, noconstant("")
    declare mock_sample_encounter_alias = vc with protect, noconstant("")

    declare stat = i4 with protect, noconstant(0)

    record public::executeReply (
        1 number_parameter = i4
        1 string_parameter = vc
        1 internalSubroutineSource = vc
        1 regular_join[*]
            2 person_id = f8
            2 encounter_id = f8
            2 encounter_alias = vc
        1 left_join[*]
            2 person_id = f8
            2 encounter_id = f8
            2 encounter_alias = vc
        1 rdb_join[*]
            2 columns = vc
            2 data = vc
    ) with protect

    set mock_sample_person = cclutDefineMockTable("sample_person", "person_id", "f8")
    set mock_sample_encounter = cclutDefineMockTable("sample_encounter", "encounter_id|person_id", "f8|f8")
    set mock_sample_encounter_alias = cclutDefineMockTable("sample_encounter_alias", "encounter_id|encounter_alias", "f8|vc")
    call cclutCreateMockTable("sample_person")
    call cclutCreateMockTable("sample_encounter")
    call cclutCreateMockTable("sample_encounter_alias")

    call cclutDefineMockTable("sample_person_rdb", "person_id", "f8", TRUE)
    call cclutDefineMockTable("sample_encounter_rdb", "encounter_id|person_id", "f8|f8", TRUE)
    call cclutDefineMockTable("sample_encounter_alias_rdb", "encounter_id|encounter_alias", "f8|vc", TRUE)

    call cclutCreateMockTable("sample_person_rdb")
    call cclutCreateMockTable("sample_encounter_rdb")
    call cclutCreateMockTable("sample_encounter_alias_rdb")

    call cclutAddMockData("sample_person", "1.0")
    call cclutAddMockData("sample_person", "2.0")
    call cclutAddMockData("sample_person", "3.0")
    call cclutAddMockData("sample_encounter", "4.0|1.0")
    call cclutAddMockData("sample_encounter", "5.0|2.0")
    call cclutAddMockData("sample_encounter_alias", "4.0|test alias")
    call cclutAddMockData("sample_person_rdb", "6.0")
    call cclutAddMockData("sample_person_rdb", "7.0")
    call cclutAddMockData("sample_person_rdb", "8.0")
    call cclutAddMockData("sample_encounter_rdb", "9.0|6.0")
    call cclutAddMockData("sample_encounter_rdb", "10.0|7.0")
    call cclutAddMockData("sample_encounter_alias_rdb", "9.0|test alias rdb")

    call cclutAddMockImplementation("reply", "executereply")

    if (checkfun("ADDUNITTESTSPECIFICMOCKS") = 7)
        call addUnitTestSpecificMocks(null)
    endif

    if (executeParams = "" and executeNamespace = "")
        call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy")
    elseif (executeNamespace = "")
        call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy", executeParams)
    else 
        call cclutExecuteProgramWithMocks("ut_cclut_execute_mocks_happy", executeParams, executeNamespace)
    endif

    call execute_mocks_happy_validations(null)
end ;test_ut_cclut_execute_mocks_happy_basic
subroutine public::execute_mocks_happy_validations(null)
    if (cclutGetCurrentGroup(null) = 1 and reqinfo->updt_id = 0.0)
        call cclutAssertI2Equal(CURREF, "authentication required to run this test", TRUE, FALSE)
        return(null)
    endif
    call cclutAsserti4Equal(CURREF, "check public::persistRecord->item", public::persistRecord->item, 1)

    if (executeParams != "")
        call cclutAsserti4Equal(CURREF, "check executeReply->number_parameter", executeReply->number_parameter, 10)
        call cclutAssertvcEqual(CURREF, "check executeReply->string_parameter", executeReply->string_parameter, "test param")
    else
        call cclutAsserti4Equal(CURREF, "check executeReply->number_parameter", executeReply->number_parameter, 0)
        call cclutAssertvcEqual(CURREF, "check executeReply->string_parameter", executeReply->string_parameter, trim(""))
    endif
    call cclutAsserti4Equal(CURREF, "size executeReply->regular_join", size(executeReply->regular_join, 5), 1)
    call cclutAssertf8Equal(CURREF, "value regular_join person_id 1", executeReply->regular_join[1].person_id, 1.0)
    call cclutAssertf8Equal(CURREF, "value regular_join encounter_id 1", executeReply->regular_join[1].encounter_id, 4.0)
    call cclutAssertvcEqual(CURREF, 
        "value regular_join encounter_alias 1", executeReply->regular_join[1].encounter_alias, "test alias")

    call cclutAsserti4Equal(CURREF, "size executeReply->left_join", size(executeReply->left_join, 5), 3)

    call cclutAssertf8Equal(CURREF, "value left_join person_id 1", executeReply->left_join[1].person_id, 1.0)
    call cclutAssertf8Equal(CURREF, "value left_join encounter_id 1", executeReply->left_join[1].encounter_id, 4.0)
    call cclutAssertvcEqual(CURREF, "value left_join encounter_alias 1", executeReply->left_join[1].encounter_alias, "test alias")

    call cclutAssertf8Equal(CURREF, "value left_join person_id 2", executeReply->left_join[2].person_id, 2.0)
    call cclutAssertf8Equal(CURREF, "value left_join encounter_id 2", executeReply->left_join[2].encounter_id, 5.0)
    call cclutAssertvcEqual(CURREF, "value left_join encounter_alias 2", executeReply->left_join[2].encounter_alias, trim(""))

    call cclutAssertf8Equal(CURREF, "value left_join person_id 3", executeReply->left_join[3].person_id, 3.0)
    call cclutAssertf8Equal(CURREF, "value left_join encounter_id 3", executeReply->left_join[3].encounter_id, 0.0)
    call cclutAssertvcEqual(CURREF, "value left_join encounter_alias 3", executeReply->left_join[3].encounter_alias, trim(""))

    ;The program ut_cclut_execute_mocks_happy performs an rdb inner join and writes the data to the file cclut_happy.dat.

    call cclutAssertVcOperator(CURREF, "rdb results header", executeReply->rdb_join[1].columns,
        "regexplike", "PERSON_ID[ ]*\|ENCOUNTER_ID[ ]*\|PERSON_ID[ ]*\|ENCOUNTER_ID[ ]*\|ENCOUNTER_ALIAS[ ]*\|")
    call cclutAssertVcOperator(CURREF, "rdb results data", executeReply->rdb_join[1].data,
        "regexplike", "[ ]*6\|[ ]*9|[ ]*6|[ ]*9|test alias rdb[ ]*\|")
    if (checkfun("UNITTESTSPECIFICVALIDATIONS") = 7)
        call unitTestSpecificValidations(null)
    endif
end ;public::execute_mocks_happy_validations
