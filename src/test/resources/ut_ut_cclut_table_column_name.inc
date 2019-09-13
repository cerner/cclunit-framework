/**
    Unit tests for ut_cclut_table_column_name used to validate the the table mocking behavior of cclutExecuteProgramWithMocks 
    when the script under test involves a table such as code_value which shares the same name as one of its columns.    
*/
declare setup(null) = null with protect
declare testSameColumnName(null) = null with protect

/**
    There is no setup for this test case, but it needs to have a setup subroutine. 
    Otherwise, the framework will find the setup subroutine for the unit test which is running this test case.
*/
subroutine setup(null)
    null
end 

subroutine testSameColumnName(null)
    if (cclutGetCurrentGroup(null) = 1 and reqinfo->updt_id = 0.0)
        call cclutAssertI2Equal(CURREF, "authentication required to run this test", TRUE, FALSE)
        return(null)
    endif
    if (validate(cclut_testRecord) = FALSE)
        record cclut_testRecord (1 tableName = vc 1 value = vc)
    endif
    set cclut_testRecord->tableName = cclutDefineMockTable("sample_table", "sample_table", "vc")
    call cclutCreateMockTable("sample_table")
    call cclutAddMockData("sample_table", "sample_value")
    call cclutExecuteProgramWithMocks("ut_cclut_table_column_name")
    call cclutRemoveMockTable("sample_table")
    call cclutAssertVcEqual(CURREF, "expected value", cclut_testRecord->value, "sample_value")
end ;testSameColumnName