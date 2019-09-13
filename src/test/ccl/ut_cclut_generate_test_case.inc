subroutine (setupOnce(null) = null)
    set trace deprecated "W"
end
subroutine (setup(null) = null)
    ; Place any test-level setup here
    null
end
subroutine (tearDown(null) = null)
    ; Place any test-level teardown here
    call cclutRemoveAllMocks(null)
    rollback
end
subroutine (tearDownOnce(null) = null)
    ; Place any test case-level teardown here
    null
end

;**********************************************************************************************************************************
;** outputUsageInstructions
;**********************************************************************************************************************************
/* testOUTPUTUSAGEINSTRUCTIONS ******************************************************************************
*  Scenario: Validates that the correct usage instructions are output to the output destination.            *
************************************************************************************************************/
subroutine testOUTPUTUSAGEINSTRUCTIONS(null)
    declare testOUTPUTUSAGEINSTRUCTIONS__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestOUTPUTUSAGEINSTRUCTIONS")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_OUTPUTUSAGEINSTRUCTIONS")

    call cclutAsserti2Equal(CURREF, "testOUTPUTUSAGEINSTRUCTIONS 002", testOUTPUTUSAGEINSTRUCTIONS__MainWasCalled, TRUE)
end
subroutine MAINtestOUTPUTUSAGEINSTRUCTIONS(null)
    set outputDestination = "ut_cclut_generate_test_case_usage.inc"
    call OUTPUTUSAGEINSTRUCTIONS(null)
    set testOUTPUTUSAGEINSTRUCTIONS__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testOUTPUTUSAGEINSTRUCTIONS 001",
        cclut::getFileAsString("ut_cclut_generate_test_case_usage.inc"), concat(
        "cclut_generate_test_case is a prompt program for generating a CCL Unit test case for a specified", char(10),
        "program object.  The generated test case will contain tests and mock implementations only for", char(10),
        "subroutines in the public namespace (i.e. with a public:: namespace or no namespace).  The", char(10),
        "following are the parameters for the program:", char(10),
        char(32), char(10),
        "outputDestination", char(10),
        "    The output destination for the test case.", char(10),
        char(32), char(10),
        "scriptUnderTest", char(10),
        "    The name of the CCL object for which the test case will be generated.", char(10),
        char(32), char(10),
        "sourceFileLocation", char(10),
        "    The location of the source file used to create the program. If not directly specified,", char(10),
        "    cclsource: is assumed for the directory and .prg is assumed for the extension.  If the", char(10),
        "    program's source file cannot be located, tests will be generated for all of the program's", char(10),
        "    subroutines in the public namespace.  If the located source file is not the file that was used", char(10),
        "    to create the program, unexpected results may occur.", char(10),
        char(32), char(10),
        "includeFiles", char(10),
        "    A pipe-delimited list of files included by the program's source file which define additional", char(10),
        "    subroutines for which tests should be generated.  By default, test are only generated for", char(10),
        "    subroutines defined directly in the program's source file.  The listed files are assumed to", char(10),
        "    reside in cclsource: and have a .inc extension unless otherwise specified.", char(10),
        char(32), char(10),
        "For additional information on how the test case is generated and examples, please visit", char(10),
        "https://github.com/cerner/cclunit-framework/blob/master/doc/CCLUTTEMPLATES.md", char(10), char(32), char(10)))
end

;**********************************************************************************************************************************
;** validateParameters
;**********************************************************************************************************************************
/* testVALIDATEPARAMETERS ***********************************************************************************
*  Scenario: Validates that the parameters are correct                                                      *
************************************************************************************************************/
subroutine testVALIDATEPARAMETERS(null)
    declare testVALIDATEPARAMETERS__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "VALIDATEPARAMETERS_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestVALIDATEPARAMETERS")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_VALIDATEPARAMETERS")

    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS 005", testVALIDATEPARAMETERS__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testVALIDATEPARAMETERS 006", cclutIsEmptyCount, 3)
    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS 007", echoRecordCalled, TRUE)
end
subroutine MAINtestVALIDATEPARAMETERS(null)
    declare sourceLocation = vc with protect, noconstant("cclsource:differentLocation.prg")
    call VALIDATEPARAMETERS("cclut_generate_test_case", "", sourceLocation)
    set testVALIDATEPARAMETERS__MainWasCalled = TRUE
    declare expected = vc with protect, constant(concat(trim(logical("CCLSOURCE")), "/differentLocation.prg"))
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS 001", sourceLocation, expected)
end
subroutine (1_VALIDATEPARAMETERS::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS 001", parameter, "cclut_generate_test_case")
            return (FALSE)
        of 2:
            call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS 002", parameter,
                "cclsource:differentLocation.prg")
            return (FALSE)
        of 3:
            call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS 003", parameter, "")
            return (TRUE)
    endcase
end
subroutine (1_VALIDATEPARAMETERS::CCLUTDOARRAYSPLIT(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (1_VALIDATEPARAMETERS::OUTPUTUSAGEINSTRUCTIONS(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (VALIDATEPARAMETERS_echorecord(templateReply = vc(ref)) = null)
    ; Happy path testing means the status should be empty since the success subroutine was never called and there were
    ; no failures
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS 004", templateReply->status_data.status, "")
    set echoRecordCalled = TRUE
end

/* testVALIDATEPARAMETERS_include_files *********************************************************************
*  Scenario: Validates that the parameters are correct when include files are sent to the prompt program.   *
************************************************************************************************************/
subroutine testVALIDATEPARAMETERS_include_files(null)
    declare testVALIDATEPARAMETERS_include_files__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)
    declare doArraySplitCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "VALIDATEPARAMETERS_include_files_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestVALIDATEPARAMETERS_include_files")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_VALIDATEPARAMETERS")

    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_include_files 010",
        testVALIDATEPARAMETERS_include_files__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testVALIDATEPARAMETERS_include_files 011", cclutIsEmptyCount, 3)
    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_include_files 012", doArraySplitCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_include_files 013", echoRecordCalled, TRUE)
end
subroutine MAINtestVALIDATEPARAMETERS_include_files(null)
    declare sourceLocation = vc with protect, noconstant("")
    call VALIDATEPARAMETERS("cclut_generate_test_case", "test includes", sourceLocation)
    set testVALIDATEPARAMETERS_include_files__MainWasCalled = TRUE
    declare expected = vc with protect, constant(concat(trim(logical("CCLSOURCE")), "/cclut_generate_test_case.prg"))
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 001", sourceLocation, expected)
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 002", includeRec->qual[1].str, "test string")
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 003", includeRec->qual[1].regex_str,
        "test regex string")
end
subroutine (2_VALIDATEPARAMETERS::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 004", parameter,
                "cclut_generate_test_case")
            return (FALSE)
        of 2:
            call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 005", parameter, "")
            return (TRUE)
        of 3:
            call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 006", parameter, "test includes")
            return (FALSE)
    endcase
end
subroutine (2_VALIDATEPARAMETERS::CCLUTDOARRAYSPLIT(includeRecord = vc(ref), includeFiles = vc,
    includeDelimiter = vc) = null)
        set doArraySplitCalled = TRUE
        call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 007", includeFiles, "test includes")
        call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 008", includeDelimiter, "|")
        set stat = alterlist(includeRecord->qual, 1)
        set includeRecord->qual[1].str = "test string"
        set includeRecord->qual[1].regex_str = "test regex string"
end
subroutine (2_VALIDATEPARAMETERS::OUTPUTUSAGEINSTRUCTIONS(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (VALIDATEPARAMETERS_include_files_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 009", templateReply->status_data.status, "")
    set echoRecordCalled = TRUE
end

/* testVALIDATEPARAMETERS_invalid_script ********************************************************************
*  Scenario: Validates that an error is thrown whenever the script cannot be found in the CCL dictionary.   *
************************************************************************************************************/
subroutine testVALIDATEPARAMETERS_invalid_script(null)
    declare testVALIDATEPARAMETERS_invalid_script__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "VALIDATEPARAMETERS_invalid_script_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestVALIDATEPARAMETERS_invalid_script")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "3_VALIDATEPARAMETERS")

    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_invalid_script 007",
        testVALIDATEPARAMETERS_invalid_script__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_invalid_script 008", echoRecordCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_invalid_script 009", cclutIsEmptyCalled, TRUE)
end
subroutine MAINtestVALIDATEPARAMETERS_invalid_script(null)
    set testVALIDATEPARAMETERS_invalid_script__MainWasCalled = TRUE
    declare sourceLocation = vc with protect, noconstant("")
    call VALIDATEPARAMETERS("not a program", "test includes", sourceLocation)
end
subroutine (3_VALIDATEPARAMETERS::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_include_files 001", parameter,
        "not a program")
    return (FALSE)
end
subroutine (3_VALIDATEPARAMETERS::CCLUTDOARRAYSPLIT(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (3_VALIDATEPARAMETERS::OUTPUTUSAGEINSTRUCTIONS(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (VALIDATEPARAMETERS_invalid_script_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_invalid_script 002", templateReply->status_data.status, "F")
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_invalid_script 003",
        trim(templateReply->status_data.subeventstatus[1].OperationName), "Validating parameters")
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_invalid_script 004",
        templateReply->status_data.subeventstatus[1].OperationStatus, "F")
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_invalid_script 005",
        trim(templateReply->status_data.subeventstatus[1].TargetObjectName), "not a program")
    call cclutAssertVcOperator(CURREF, "testVALIDATEPARAMETERS_invalid_script 006",
        templateReply->status_data.subeventstatus[1].TargetObjectValue, "regexplike",
        concat("%CCL-E-392-CCLUT_GENERATE_TEST_CASE\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\): ",
            "Script could not be found in CCL dictionary."))
    set echoRecordCalled = TRUE
end

/* testVALIDATEPARAMETERS_empty_script **********************************************************************
*  Scenario: Validates that usage instructions are output whenever the supplied script under test is empty. *
************************************************************************************************************/
subroutine testVALIDATEPARAMETERS_empty_script(null)
    declare testVALIDATEPARAMETERS_empty_script__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)
    declare outputUsageInstructionsCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "VALIDATEPARAMETERS_empty_script_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestVALIDATEPARAMETERS_empty_script")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "4_VALIDATEPARAMETERS")

    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_empty_script 003",
        testVALIDATEPARAMETERS_empty_script__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_empty_script 004", echoRecordCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_empty_script 005", cclutIsEmptyCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testVALIDATEPARAMETERS_empty_script 006", outputUsageInstructionsCalled, TRUE)
end
subroutine MAINtestVALIDATEPARAMETERS_empty_script(null)
    set testVALIDATEPARAMETERS_empty_script__MainWasCalled = TRUE
    declare sourceLocation = vc with protect, noconstant("")
    call VALIDATEPARAMETERS("", "test includes", sourceLocation)
end
subroutine (4_VALIDATEPARAMETERS::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_empty_script 001", parameter, "")
    return (TRUE)
end
subroutine (4_VALIDATEPARAMETERS::CCLUTDOARRAYSPLIT(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (4_VALIDATEPARAMETERS::OUTPUTUSAGEINSTRUCTIONS(null) = null)
    set outputUsageInstructionsCalled = TRUE
end
subroutine (VALIDATEPARAMETERS_empty_script_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testVALIDATEPARAMETERS_empty_script 002", templateReply->status_data.status, "")
    set echoRecordCalled = TRUE
end

;**********************************************************************************************************************************
;** createProgramXML
;**********************************************************************************************************************************
/* testCREATEPROGRAMXML *************************************************************************************
*  Scenario: Validates that the program is translated into XML and stored locally.                          *
************************************************************************************************************/
subroutine testCREATEPROGRAMXML(null)
    declare testCREATEPROGRAMXML__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "CREATEPROGRAMXML_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestCREATEPROGRAMXML")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_CREATEPROGRAMXML")

    call cclutAsserti2Equal(CURREF, "testCREATEPROGRAMXML 003", testCREATEPROGRAMXML__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEPROGRAMXML 004", cclutIsEmptyCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEPROGRAMXML 005", echoRecordCalled, TRUE)
end
subroutine MAINtestCREATEPROGRAMXML(null)
    call CREATEPROGRAMXML("cclut_generate_test_case_test")
    set testCREATEPROGRAMXML__MainWasCalled = TRUE
end
subroutine (1_CREATEPROGRAMXML::CCLUTISEMPTY(programXML = vc) = i2)
    call cclutAssertvcEqual(CURREF, "testCREATEPROGRAMXML 001", programXML, concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM. class="223" lev="0" kid="2" loc="0.0">', char(10),
    '  <USER. class="179" lev="1" kid="2" loc="0.0">', char(10),
    '   <NAME class="5" text="CCLUT_GENERATE_TEST_CASE_TEST" lev="2" loc="0.0"/>', char(10),
    '   <NAME class="5" text="DBA" lev="2" loc="0.0"/>', char(10),
    '  </USER.>', char(10),
    '  <Z_CALL. class="197" lev="1" kid="1" loc="0.0">', char(10),
    '   <CALL. class="125" lev="2" kid="2" loc="0.0">', char(10),
    '    <NAME class="5" text="ECHO" lev="3" loc="0.0"/>', char(10),
    '    <STRING class="7" text="This is a test program for the ut_cclut_generate_test_case.inc test case."',
        ' lev="3" loc="0.0"/>', char(10),
    '   </CALL.>', char(10),
    '  </Z_CALL.>', char(10),
    ' </ZC_PROGRAM.>', char(10)))
    set cclutIsEmptyCalled = TRUE
    return (FALSE)
end
subroutine (CREATEPROGRAMXML_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testCREATEPROGRAMXML 002", templateReply->status_data.status, "")
    set echoRecordCalled = TRUE
end

/* testCREATEPROGRAMXML_empty_program ***********************************************************************
*  Scenario: Validates that an error is thrown when the file containing the translated program is empty.    *
************************************************************************************************************/
subroutine testCREATEPROGRAMXML_empty_program(null)
    declare testCREATEPROGRAMXML_empty_program__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "CREATEPROGRAMXML_empty_program_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestCREATEPROGRAMXML_empty_program")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_CREATEPROGRAMXML")

    call cclutAsserti2Equal(CURREF, "testCREATEPROGRAMXML_empty_program 003",
        testCREATEPROGRAMXML_empty_program__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEPROGRAMXML_empty_program 004", cclutIsEmptyCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEPROGRAMXML_empty_program 005", echoRecordCalled, TRUE)
end
subroutine MAINtestCREATEPROGRAMXML_empty_program(null)
    set testCREATEPROGRAMXML_empty_program__MainWasCalled = TRUE
    call CREATEPROGRAMXML("cclut_generate_test_case_test")
end
subroutine (2_CREATEPROGRAMXML::CCLUTISEMPTY(programXML = vc) = i2)
    call cclutAssertvcEqual(CURREF, "testCREATEPROGRAMXML_empty_program 001", programXML, concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM. class="223" lev="0" kid="2" loc="0.0">', char(10),
    '  <USER. class="179" lev="1" kid="2" loc="0.0">', char(10),
    '   <NAME class="5" text="CCLUT_GENERATE_TEST_CASE_TEST" lev="2" loc="0.0"/>', char(10),
    '   <NAME class="5" text="DBA" lev="2" loc="0.0"/>', char(10),
    '  </USER.>', char(10),
    '  <Z_CALL. class="197" lev="1" kid="1" loc="0.0">', char(10),
    '   <CALL. class="125" lev="2" kid="2" loc="0.0">', char(10),
    '    <NAME class="5" text="ECHO" lev="3" loc="0.0"/>', char(10),
    '    <STRING class="7" text="This is a test program for the ut_cclut_generate_test_case.inc test case."',
        ' lev="3" loc="0.0"/>', char(10),
    '   </CALL.>', char(10),
    '  </Z_CALL.>', char(10),
    ' </ZC_PROGRAM.>', char(10)))
    set cclutIsEmptyCalled = TRUE
    return (TRUE)
end
subroutine (CREATEPROGRAMXML_empty_program_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testCREATEPROGRAMXML_empty_program 002", templateReply->status_data.status, "F")
    call cclutAssertvcEqual(CURREF, "testCREATEPROGRAMXML_empty_program 003",
        trim(templateReply->status_data.subeventstatus[1].OperationName), "Translate program")
    call cclutAssertvcEqual(CURREF, "testCREATEPROGRAMXML_empty_program 004",
        templateReply->status_data.subeventstatus[1].OperationStatus, "F")
    call cclutAssertvcEqual(CURREF, "testCREATEPROGRAMXML_empty_program 005",
        trim(templateReply->status_data.subeventstatus[1].TargetObjectName), "cclut_generate_test_case_")
    call cclutAssertVcOperator(CURREF, "testCREATEPROGRAMXML_empty_program 006",
        templateReply->status_data.subeventstatus[1].TargetObjectValue, "regexplike",
        concat("%CCL-E-392-CCLUT_GENERATE_TEST_CASE\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\): ",
            "Failed to translate program.  XML file ccluserdir:cclut_tmpl_prg", CCLUT_SUFFIX, ".xml"))
    set echoRecordCalled = TRUE
end

;**********************************************************************************************************************************
;** getNamedElementName
;**********************************************************************************************************************************
/* testGETNAMEDELEMENTNAME **********************************************************************************
*  Scenario: Validates that the correct name is returned for a given XML element node.                      *
************************************************************************************************************/
subroutine testGETNAMEDELEMENTNAME(null)
    declare testGETNAMEDELEMENTNAME__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGETNAMEDELEMENTNAME")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GETNAMEDELEMENTNAME")

    call cclutAsserti2Equal(CURREF, "testGETNAMEDELEMENTNAME 004", testGETNAMEDELEMENTNAME__MainWasCalled, TRUE)
end
subroutine MAINtestGETNAMEDELEMENTNAME(null)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    declare subroutineName = vc with protect, noconstant("")
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <SUBROUTINE.>', char(10),
    '  <NAME text="TESTSUBROUTINE1" />', char(10),
    '  <NAME text="TESTSUBROUTINE2" />', char(10),
    '  <NAME text="TESTSUBROUTINE3" />', char(10),
    ' </SUBROUTINE.>'), hXmlFile)
    set subroutineName = GETNAMEDELEMENTNAME(cclut::getXmlListItemHandle(hXmlTest, "SUBROUTINE.", 1), 1)
    call cclutAssertvcEqual(CURREF, "testGETNAMEDELEMENTNAME 001", subroutineName, "TESTSUBROUTINE1")
    set subroutineName = GETNAMEDELEMENTNAME(cclut::getXmlListItemHandle(hXmlTest, "SUBROUTINE.", 1), 2)
    call cclutAssertvcEqual(CURREF, "testGETNAMEDELEMENTNAME 002", subroutineName, "TESTSUBROUTINE2")
    set subroutineName = GETNAMEDELEMENTNAME(cclut::getXmlListItemHandle(hXmlTest, "SUBROUTINE.", 1), 3)
    call cclutAssertvcEqual(CURREF, "testGETNAMEDELEMENTNAME 003", subroutineName, "TESTSUBROUTINE3")
    set testGETNAMEDELEMENTNAME__MainWasCalled = TRUE
end

/* testGETNAMEDELEMENTNAME_empty ****************************************************************************
*  Scenario: Validates that an empty string is returned if there is no NAME node.                           *
************************************************************************************************************/
subroutine testGETNAMEDELEMENTNAME_empty(null)
    declare testGETNAMEDELEMENTNAME_empty__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGETNAMEDELEMENTNAME_empty")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_GETNAMEDELEMENTNAME")

    call cclutAsserti2Equal(CURREF, "testGETNAMEDELEMENTNAME_empty 004", testGETNAMEDELEMENTNAME_empty__MainWasCalled,
        TRUE)
end
subroutine MAINtestGETNAMEDELEMENTNAME_empty(null)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    declare subroutineName = vc with protect, noconstant("")
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <SUBROUTINE.>', char(10),
    '  <NAME text="TESTSUBROUTINE1" />', char(10),
    '  <NAME text="TESTSUBROUTINE2" />', char(10),
    '  <NAME />', char(10),
    ' </SUBROUTINE.>', char(10),
    ' <SUBROUTINE.>', char(10),
    ' </SUBROUTINE.>', char(10)), hXmlFile)
    set subroutineName = GETNAMEDELEMENTNAME(cclut::getXmlListItemHandle(hXmlTest, "SUBROUTINE.", 1), 4)
    call cclutAssertvcEqual(CURREF, "testGETNAMEDELEMENTNAME_empty 001", subroutineName, trim(""))
    set subroutineName = GETNAMEDELEMENTNAME(cclut::getXmlListItemHandle(hXmlTest, "SUBROUTINE.", 1), 3)
    call cclutAssertvcEqual(CURREF, "testGETNAMEDELEMENTNAME_empty 002", subroutineName, trim(""))
    set subroutineName = GETNAMEDELEMENTNAME(cclut::getXmlListItemHandle(hXmlTest, "SUBROUTINE.", 2), 1)
    call cclutAssertvcEqual(CURREF, "testGETNAMEDELEMENTNAME_empty 003", subroutineName, trim(""))
    set testGETNAMEDELEMENTNAME_empty__MainWasCalled = TRUE
end

;**********************************************************************************************************************************
;** getPublicNamedElementName
;**********************************************************************************************************************************
/* testGETPUBLICNAMEDELEMENTNAME ****************************************************************************
*  Scenario: Validates that the correct name is returned for a given public XML element node.               *
************************************************************************************************************/
subroutine testGETPUBLICNAMEDELEMENTNAME(null)
    declare testGETPUBLICNAMEDELEMENTNAME__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestGETPUBLICNAMEDELEMENTNAME")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GETPUBLICNAMEDELEMENTNAME")

    call cclutAsserti2Equal(CURREF, "testGETPUBLICNAMEDELEMENTNAME 003", testGETPUBLICNAMEDELEMENTNAME__MainWasCalled,
        TRUE)
    call cclutAsserti4Equal(CURREF, "testGETPUBLICNAMEDELEMENTNAME 004", getNamedElementNameCount, 2)
end
subroutine MAINtestGETPUBLICNAMEDELEMENTNAME(null)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    declare subroutineName = vc with protect, noconstant("")
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <SUBROUTINE.>', char(10),
    '  <NAMESPACE.>', char(10),
    '   <NAME text="PUBLIC" />', char(10),
    '   <NAME text="TESTSUBROUTINE1" />', char(10),
    '  </NAMESPACE.>', char(10),
    ' </SUBROUTINE.>', char(10)), hXmlFile)
    set subroutineName = GETPUBLICNAMEDELEMENTNAME(cclut::getXmlListItemHandle(hXmlTest, "SUBROUTINE.", 1))
    set testGETPUBLICNAMEDELEMENTNAME__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testGETPUBLICNAMEDELEMENTNAME 002", subroutineName, "TESTSUBROUTINE1")
end
subroutine (1_GETPUBLICNAMEDELEMENTNAME::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    call cclutAsserti4Equal(CURREF, "testGETPUBLICNAMEDELEMENTNAME 001", index, getNamedElementNameCount)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end

/* testGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace ********************************************************
*  Scenario: Validates that an empty string is returned if the namespace of the item is not PUBLIC.         *
************************************************************************************************************/
subroutine testGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace(null)
    declare testGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_GETPUBLICNAMEDELEMENTNAME")

    call cclutAsserti2Equal(CURREF, "testGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace 003",
        testGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace 004", getNamedElementNameCount,
        1)
end
subroutine MAINtestGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace(null)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    declare subroutineName = vc with protect, noconstant("")
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <SUBROUTINE.>', char(10),
    '  <NAMESPACE.>', char(10),
    '   <NAME text="CCLUT" />', char(10),
    '   <NAME text="TESTSUBROUTINE1" />', char(10),
    '  </NAMESPACE.>', char(10),
    ' </SUBROUTINE.>', char(10)), hXmlFile)
    set subroutineName = GETPUBLICNAMEDELEMENTNAME(cclut::getXmlListItemHandle(hXmlTest, "SUBROUTINE.", 1))
    set testGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace 002", subroutineName, "")
end
subroutine (2_GETPUBLICNAMEDELEMENTNAME::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    call cclutAsserti4Equal(CURREF, "testGETPUBLICNAMEDELEMENTNAME_nonpublic_namespace 001", index,
        getNamedElementNameCount)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end

/* testGETPUBLICNAMEDELEMENTNAME_no_namespace ***************************************************************
*  Scenario: Validates that the correct name is returned if there is no namespace.                          *
************************************************************************************************************/
subroutine testGETPUBLICNAMEDELEMENTNAME_no_namespace(null)
    declare testGETPUBLICNAMEDELEMENTNAME_no_namespace__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestGETPUBLICNAMEDELEMENTNAME_no_namespace")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "3_GETPUBLICNAMEDELEMENTNAME")

    call cclutAsserti2Equal(CURREF, "testGETPUBLICNAMEDELEMENTNAME_no_namespace 003",
        testGETPUBLICNAMEDELEMENTNAME_no_namespace__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testGETPUBLICNAMEDELEMENTNAME_no_namespace 004", getNamedElementNameCount, 1)
end
subroutine MAINtestGETPUBLICNAMEDELEMENTNAME_no_namespace(null)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    declare subroutineName = vc with protect, noconstant("")
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <SUBROUTINE.>', char(10),
    '   <NAME text="TESTSUBROUTINE1" />', char(10),
    ' </SUBROUTINE.>', char(10)), hXmlFile)
    set subroutineName = GETPUBLICNAMEDELEMENTNAME(cclut::getXmlListItemHandle(hXmlTest, "SUBROUTINE.", 1))
    set testGETPUBLICNAMEDELEMENTNAME_no_namespace__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testGETPUBLICNAMEDELEMENTNAME_no_namespace 002", subroutineName, "TESTSUBROUTINE1")
end
subroutine (3_GETPUBLICNAMEDELEMENTNAME::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    call cclutAsserti4Equal(CURREF, "testGETPUBLICNAMEDELEMENTNAME_no_namespace 001", index, getNamedElementNameCount)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end

;**********************************************************************************************************************************
;** findProgramSubroutines
;**********************************************************************************************************************************
/* testFINDPROGRAMSUBROUTINES *******************************************************************************
*  Scenario: Validates that subroutines are correctly identified in an XML string.                          *
************************************************************************************************************/
subroutine testFINDPROGRAMSUBROUTINES(null)
    declare testFINDPROGRAMSUBROUTINES__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getPublicNamedElementNameCount = i4 with protect, noconstant(0)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "FINDPROGRAMSUBROUTINES_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestFINDPROGRAMSUBROUTINES")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_FINDPROGRAMSUBROUTINES")

    call cclutAsserti2Equal(CURREF, "testFINDPROGRAMSUBROUTINES 008", testFINDPROGRAMSUBROUTINES__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testFINDPROGRAMSUBROUTINES 009", getPublicNamedElementNameCount, 3)
    call cclutAsserti4Equal(CURREF, "testFINDPROGRAMSUBROUTINES 010", cclutIsEmptyCount, 3)
    call cclutAsserti2Equal(CURREF, "testFINDPROGRAMSUBROUTINES 011", echoRecordCalled, TRUE)
end
subroutine MAINtestFINDPROGRAMSUBROUTINES(null)
    record subroutinesRecord (
        1 targetSubroutines[*]
            2 name = vc
    ) with protect
    call FINDPROGRAMSUBROUTINES(concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM.>', char(10),
    '  <SUBROUTINE.>', char(10),
    '   <NAME text="TESTSUBROUTINE1" />', char(10),
    '  </SUBROUTINE.>', char(10),
    '  <SUBROUTINE.>', char(10),
    '   <NAMESPACE.>', char(10),
    '    <NAME text="CCLUT" />', char(10),
    '    <NAME text="TESTSUBROUTINE2" />', char(10),
    '   </NAMESPACE.>', char(10),
    '  </SUBROUTINE.>', char(10),
    '  <SUBROUTINE.>', char(10),
    '   <NAMESPACE.>', char(10),
    '    <NAME text="PUBLIC" />', char(10),
    '    <NAME text="TESTSUBROUTINE3" />', char(10),
    '   </NAMESPACE.>', char(10),
    '  </SUBROUTINE.>', char(10),
    ' </ZC_PROGRAM.>', char(10)), subroutinesRecord, FALSE, "test program")
    set testFINDPROGRAMSUBROUTINES__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testFINDPROGRAMSUBROUTINES 004", size(subroutinesRecord->targetSubroutines, 5), 2)
    call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES 005", subroutinesRecord->targetSubroutines[1].name,
        "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES 006", subroutinesRecord->targetSubroutines[2].name,
        "TESTSUBROUTINE3")
end
subroutine (1_FINDPROGRAMSUBROUTINES::ADDWARNING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (1_FINDPROGRAMSUBROUTINES::GETPUBLICNAMEDELEMENTNAME(element = h) = vc)
    set getPublicNamedElementNameCount = getPublicNamedElementNameCount + 1
    return (PUBLIC::GETPUBLICNAMEDELEMENTNAME(element))
end
subroutine (1_FINDPROGRAMSUBROUTINES::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES 001", parameter, "TESTSUBROUTINE1")
            return (FALSE)
        of 2:
            call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES 002", parameter, "")
            return (TRUE)
        of 3:
            call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES 003", parameter, "TESTSUBROUTINE3")
            return (FALSE)
    endcase
end
subroutine (FINDPROGRAMSUBROUTINES_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES 007", templateReply->status_data.status, "")
    set echoRecordCalled = TRUE
end

/* testFINDPROGRAMSUBROUTINES_include_failure ***************************************************************
*  Scenario: Validates that a warning is added when an include XML fails to be parsed.                      *
************************************************************************************************************/
subroutine testFINDPROGRAMSUBROUTINES_include_failure(null)
    declare testFINDPROGRAMSUBROUTINES_include_failure__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare addWarningCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "FINDPROGRAMSUBROUTINES_include_failure_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestFINDPROGRAMSUBROUTINES_include_failure")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_FINDPROGRAMSUBROUTINES")

    call cclutAsserti2Equal(CURREF, "testFINDPROGRAMSUBROUTINES_include_failure 003",
        testFINDPROGRAMSUBROUTINES_include_failure__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testFINDPROGRAMSUBROUTINES_include_failure 004", addWarningCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testFINDPROGRAMSUBROUTINES_include_failure 005", echoRecordCalled, TRUE)
end
subroutine MAINtestFINDPROGRAMSUBROUTINES_include_failure(null)
    record subroutinesRecord (
        1 targetSubroutines[*]
            2 name = vc
    ) with protect
    call FINDPROGRAMSUBROUTINES("", subroutinesRecord, FALSE, "test include")
    set testFINDPROGRAMSUBROUTINES_include_failure__MainWasCalled = TRUE
end
subroutine (2_FINDPROGRAMSUBROUTINES::ADDWARNING(warning = vc) = null)
    set addWarningCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES_include_failure 001", warning,
        "Failed to parse XML from include file test include.")
end
subroutine (2_FINDPROGRAMSUBROUTINES::GETPUBLICNAMEDELEMENTNAME(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (2_FINDPROGRAMSUBROUTINES::CCLUTISEMPTY(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (FINDPROGRAMSUBROUTINES_include_failure_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES_include_failure 002", templateReply->status_data.status,
        "")
    set echoRecordCalled = TRUE
end

/* testFINDPROGRAMSUBROUTINES_program_failure ***************************************************************
*  Scenario: Validates that an error is thrown when a program XML fails to be parsed.                       *
************************************************************************************************************/
subroutine testFINDPROGRAMSUBROUTINES_program_failure(null)
    declare testFINDPROGRAMSUBROUTINES_program_failure__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "FINDPROGRAMSUBROUTINES_program_failure_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestFINDPROGRAMSUBROUTINES_program_failure")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "3_FINDPROGRAMSUBROUTINES")

    call cclutAsserti2Equal(CURREF, "testFINDPROGRAMSUBROUTINES_program_failure 006",
        testFINDPROGRAMSUBROUTINES_program_failure__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testFINDPROGRAMSUBROUTINES_program_failure 007", echoRecordCalled, TRUE)
end
subroutine MAINtestFINDPROGRAMSUBROUTINES_program_failure(null)
    record subroutinesRecord (
        1 targetSubroutines[*]
            2 name = vc
    ) with protect
    set testFINDPROGRAMSUBROUTINES_program_failure__MainWasCalled = TRUE
    call FINDPROGRAMSUBROUTINES("", subroutinesRecord, TRUE, "test program")
end
subroutine (3_FINDPROGRAMSUBROUTINES::ADDWARNING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (3_FINDPROGRAMSUBROUTINES::GETPUBLICNAMEDELEMENTNAME(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (3_FINDPROGRAMSUBROUTINES::CCLUTISEMPTY(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (FINDPROGRAMSUBROUTINES_program_failure_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES_program_failure 001", templateReply->status_data.status,
        "F")
    call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES_program_failure 002",
        trim(templateReply->status_data.subeventstatus[1].OperationName), "Parse program")
    call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES_program_failure 003",
        templateReply->status_data.subeventstatus[1].OperationStatus, "F")
    call cclutAssertvcEqual(CURREF, "testFINDPROGRAMSUBROUTINES_program_failure 004",
        trim(templateReply->status_data.subeventstatus[1].TargetObjectName), "test program")
    call cclutAssertVcOperator(CURREF, "testFINDPROGRAMSUBROUTINES_program_failure 005",
        templateReply->status_data.subeventstatus[1].TargetObjectValue, "regexplike",
        concat("%CCL-E-392-CCLUT_GENERATE_TEST_CASE\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\): ",
            "findProgramSubroutines failed to parse program XML.  XML: "))
    set echoRecordCalled = TRUE
end

;**********************************************************************************************************************************
;** populateMockableSubroutines
;**********************************************************************************************************************************
/* testPOPULATEMOCKABLESUBROUTINES **************************************************************************
*  Scenario: Validates that subroutines are copied from targetSubroutines to mockableSubroutines correctly. *
************************************************************************************************************/
subroutine testPOPULATEMOCKABLESUBROUTINES(null)
    declare testPOPULATEMOCKABLESUBROUTINES__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestPOPULATEMOCKABLESUBROUTINES")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_POPULATEMOCKABLESUBROUTINES")

    call cclutAsserti2Equal(CURREF, "testPOPULATEMOCKABLESUBROUTINES 027",
        testPOPULATEMOCKABLESUBROUTINES__MainWasCalled, TRUE)
end
subroutine MAINtestPOPULATEMOCKABLESUBROUTINES(null)
    record subroutinesRecord (
        1 targetSubroutines[*]
            2 name = vc
            2 namespaceForMocks = vc
            2 namespaceForMocksIdentifier = i4
        1 mockableSubroutines[*]
            2 name = vc
    ) with protect
    set stat = alterlist(subroutinesRecord->targetSubroutines, 6)
    set subroutinesRecord->targetSubroutines[1].name = "TESTSUBROUTINE1"
    set subroutinesRecord->targetSubroutines[2].name = "LONGNAMETHATISGREATERTHAN35CHARACTERS1"
    set subroutinesRecord->targetSubroutines[3].name = "TESTSUBROUTINE2"
    set subroutinesRecord->targetSubroutines[4].name = "LONGNAMETHATISGREATERTHAN35CHARACTERS2"
    set subroutinesRecord->targetSubroutines[5].name = "TESTSUBROUTINE3"
    set subroutinesRecord->targetSubroutines[6].name = "LONGNAMETHATISGREATERTHAN35CHARACTERS3"
    call POPULATEMOCKABLESUBROUTINES(subroutinesRecord)
    set testPOPULATEMOCKABLESUBROUTINES__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testPOPULATEMOCKABLESUBROUTINES 001",
        size(subroutinesRecord->targetSubroutines, 5), 6)
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 002", subroutinesRecord->targetSubroutines[1].name,
        "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 003",
        subroutinesRecord->targetSubroutines[1].namespaceForMocks, "TESTSUBROUTINE1")
    call cclutAsserti4Equal(CURREF, "testPOPULATEMOCKABLESUBROUTINES 004",
        subroutinesRecord->targetSubroutines[1].namespaceForMocksIdentifier, 0)
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 005", subroutinesRecord->targetSubroutines[2].name,
        "LONGNAMETHATISGREATERTHAN35CHARACTERS1")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 006",
        subroutinesRecord->targetSubroutines[2].namespaceForMocks, "LONGNAMETHATISGREATERTHAN35CHARACTE")
    call cclutAsserti4Equal(CURREF, "testPOPULATEMOCKABLESUBROUTINES 007",
        subroutinesRecord->targetSubroutines[2].namespaceForMocksIdentifier, 1)
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 008", subroutinesRecord->targetSubroutines[3].name,
        "TESTSUBROUTINE2")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 009",
        subroutinesRecord->targetSubroutines[3].namespaceForMocks, "TESTSUBROUTINE2")
    call cclutAsserti4Equal(CURREF, "testPOPULATEMOCKABLESUBROUTINES 010",
        subroutinesRecord->targetSubroutines[3].namespaceForMocksIdentifier, 0)
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 011", subroutinesRecord->targetSubroutines[4].name,
        "LONGNAMETHATISGREATERTHAN35CHARACTERS2")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 012",
        subroutinesRecord->targetSubroutines[4].namespaceForMocks, "LONGNAMETHATISGREATERTHAN35CHARACTE")
    call cclutAsserti4Equal(CURREF, "testPOPULATEMOCKABLESUBROUTINES 013",
        subroutinesRecord->targetSubroutines[4].namespaceForMocksIdentifier, 2)
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 014", subroutinesRecord->targetSubroutines[5].name,
        "TESTSUBROUTINE3")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 015",
        subroutinesRecord->targetSubroutines[5].namespaceForMocks, "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testPOPULATEMOCKABLESUBROUTINES 016",
        subroutinesRecord->targetSubroutines[5].namespaceForMocksIdentifier, 0)
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 017", subroutinesRecord->targetSubroutines[6].name,
        "LONGNAMETHATISGREATERTHAN35CHARACTERS3")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 018",
        subroutinesRecord->targetSubroutines[6].namespaceForMocks, "LONGNAMETHATISGREATERTHAN35CHARACTE")
    call cclutAsserti4Equal(CURREF, "testPOPULATEMOCKABLESUBROUTINES 019",
        subroutinesRecord->targetSubroutines[6].namespaceForMocksIdentifier, 3)
    call cclutAsserti4Equal(CURREF, "testPOPULATEMOCKABLESUBROUTIENS 020",
        size(subroutinesRecord->mockableSubroutines, 5), 6)
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 021",
        subroutinesRecord->mockableSubroutines[1].name, "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 022",
        subroutinesRecord->mockableSubroutines[2].name, "LONGNAMETHATISGREATERTHAN35CHARACTERS1")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 023",
        subroutinesRecord->mockableSubroutines[3].name, "TESTSUBROUTINE2")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 024",
        subroutinesRecord->mockableSubroutines[4].name, "LONGNAMETHATISGREATERTHAN35CHARACTERS2")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 025",
        subroutinesRecord->mockableSubroutines[5].name, "TESTSUBROUTINE3")
    call cclutAssertvcEqual(CURREF, "testPOPULATEMOCKABLESUBROUTINES 026",
        subroutinesRecord->mockableSubroutines[6].name, "LONGNAMETHATISGREATERTHAN35CHARACTERS3")
end

;**********************************************************************************************************************************
;** generateIncludeFileRegularExpressions
;**********************************************************************************************************************************
/* testGENERATEINCLUDEFILEREGULAREXPRESSIONS ****************************************************************
*  Scenario: Validates that the regular expressions for include files are generated correctly.              *
************************************************************************************************************/
subroutine testGENERATEINCLUDEFILEREGULAREXPRESSIONS(null)
    declare testGENERATEINCLUDEFILEREGULAREXPRESSIONS__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATEINCLUDEFILEREGULAREXPRESSIONS")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GENERATEINCLUDEFILEREGULAREXPRESSIO")

    call cclutAsserti2Equal(CURREF, "testGENERATEINCLUDEFILEREGULAREXPRESSIONS 008",
        testGENERATEINCLUDEFILEREGULAREXPRESSIONS__MainWasCalled, TRUE)
end
subroutine MAINtestGENERATEINCLUDEFILEREGULAREXPRESSIONS(null)
    set stat = alterlist(includeRec->qual, 3)
    set includeRec->qual[1].str = "cclsource:test_include.inc"
    set includeRec->qual[2].str = "test_include2"
    set includeRec->qual[3].str = "test_include3.dat"
    call GENERATEINCLUDEFILEREGULAREXPRESSIONS(null)
    set testGENERATEINCLUDEFILEREGULAREXPRESSIONS__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testGENERATEINCLUDEFILEREGULAREXPRESSIONS 001", size(includeRec->qual, 5), 3)
    call cclutAssertvcEqual(CURREF, "testGENERATEINCLUDEFILEREGULAREXPRESSIONS 002", includeRec->qual[1].str,
        "cclsource:test_include.inc")
    call cclutAssertvcEqual(CURREF, "testGENERATEINCLUDEFILEREGULAREXPRESSIONS 003", includeRec->qual[1].regex_str,
        concat("^%[iI][^[:space:]]*[[:space:]]+[Cc][Cc][Ll][Ss][Oo][Uu][Rr][Cc][Ee][::][Tt][Ee][Ss][Tt][__][Ii][Nn]",
            "[Cc][Ll][Uu][Dd][Ee][..][Ii][Nn][Cc]($|([[:space:]]+.*$))"))
    call cclutAssertvcEqual(CURREF, "testGENERATEINCLUDEFILEREGULAREXPRESSIONS 004", includeRec->qual[2].str,
        "CCLSOURCE:test_include2.inc")
    call cclutAssertvcEqual(CURREF, "testGENERATEINCLUDEFILEREGULAREXPRESSIONS 005", includeRec->qual[2].regex_str,
        concat("^%[iI][^[:space:]]*[[:space:]]+[Cc][Cc][Ll][Ss][Oo][Uu][Rr][Cc][Ee][::][Tt][Ee][Ss][Tt][__][Ii][Nn]",
            "[Cc][Ll][Uu][Dd][Ee][22][..][Ii][Nn][Cc]($|([[:space:]]+.*$))"))
    call cclutAssertvcEqual(CURREF, "testGENERATEINCLUDEFILEREGULAREXPRESSIONS 006", includeRec->qual[3].str,
        "CCLSOURCE:test_include3.dat")
    call cclutAssertvcEqual(CURREF, "testGENERATEINCLUDEFILEREGULAREXPRESSIONS 007", includeRec->qual[3].regex_str,
        concat("^%[iI][^[:space:]]*[[:space:]]+[Cc][Cc][Ll][Ss][Oo][Uu][Rr][Cc][Ee][::][Tt][Ee][Ss][Tt][__][Ii][Nn]",
            "[Cc][Ll][Uu][Dd][Ee][33][..][Dd][Aa][Tt]($|([[:space:]]+.*$))"))
end

;**********************************************************************************************************************************
;** addWarning
;**********************************************************************************************************************************
/* testADDWARNING *******************************************************************************************
*  Scenario: Validates that the warning is added to the template record structure correctly.                *
************************************************************************************************************/
subroutine testADDWARNING(null)
    declare testADDWARNING__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare echoCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("echo", "ADDWARNING_echo")

    call cclutAddMockImplementation("MAIN", "MAINtestADDWARNING")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_ADDWARNING")

    call cclutAsserti2Equal(CURREF, "testADDWARNING 008", testADDWARNING__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testADDWARNING 009", echoCount, 3)
end
subroutine MAINtestADDWARNING(null)
    call ADDWARNING("warning 1")
    call ADDWARNING("warning 2")
    call ADDWARNING("warning 3")
    set testADDWARNING__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testADDWARNING 001", size(templateRec->warnings, 5), 3)
    call cclutAssertvcEqual(CURREF, "testADDWARNING 002", templateRec->warnings[1].warning, "warning 1")
    call cclutAssertvcEqual(CURREF, "testADDWARNING 003", templateRec->warnings[2].warning, "warning 2")
    call cclutAssertvcEqual(CURREF, "testADDWARNING 004", templateRec->warnings[3].warning, "warning 3")
end
subroutine (ADDWARNING_echo(parameter = vc) = null with protect)
    set echoCount = echoCount + 1
    case (echoCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testADDWARNING 005", parameter, "warning 1")
        of 2:
            call cclutAssertvcEqual(CURREF, "testADDWARNING 006", parameter, "warning 2")
        of 3:
            call cclutAssertvcEqual(CURREF, "testADDWARNING 007", parameter, "warning 3")
    endcase
end

;**********************************************************************************************************************************
;** addMessage
;**********************************************************************************************************************************
/* testADDMESSAGE *******************************************************************************************
*  Scenario: Validates that the message is added to the template record structure correctly.                *
************************************************************************************************************/
subroutine testADDMESSAGE(null)
    declare testADDMESSAGE__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare echoCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("echo", "ADDMESSAGE_echo")

    call cclutAddMockImplementation("MAIN", "MAINtestADDMESSAGE")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_ADDMESSAGE")

    call cclutAsserti2Equal(CURREF, "testADDMESSAGE 008", testADDMESSAGE__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testADDMESSAGE 009", echoCount, 3)
end
subroutine MAINtestADDMESSAGE(null)
    call ADDMESSAGE("message 1")
    call ADDMESSAGE("message 2")
    call ADDMESSAGE("message 3")
    set testADDMESSAGE__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testADDMESSAGE 001", size(templateRec->messages, 5), 3)
    call cclutAssertvcEqual(CURREF, "testADDMESSAGE 002", templateRec->messages[1].message, "message 1")
    call cclutAssertvcEqual(CURREF, "testADDMESSAGE 003", templateRec->messages[2].message, "message 2")
    call cclutAssertvcEqual(CURREF, "testADDMESSAGE 004", templateRec->messages[3].message, "message 3")
end
subroutine (ADDMESSAGE_echo(parameter = vc) = null with protect)
    set echoCount = echoCount + 1
    case (echoCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testADDMESSAGE 005", parameter, "message 1")
        of 2:
            call cclutAssertvcEqual(CURREF, "testADDMESSAGE 006", parameter, "message 2")
        of 3:
            call cclutAssertvcEqual(CURREF, "testADDMESSAGE 007", parameter, "message 3")
    endcase
end

;**********************************************************************************************************************************
;** getExcludedIncludeList
;**********************************************************************************************************************************
/* testGETEXCLUDEDINCLUDELIST *******************************************************************************
*  Scenario: Validates that the correct include lines are identified and added to the exclusion list.       *
************************************************************************************************************/
subroutine testGETEXCLUDEDINCLUDELIST(null)
    declare testGETEXCLUDEDINCLUDELIST__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare generateIncludeFileRegularExpressionsCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGETEXCLUDEDINCLUDELIST")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GETEXCLUDEDINCLUDELIST")

    call cclutAsserti2Equal(CURREF, "testGETEXCLUDEDINCLUDELIST 004", testGETEXCLUDEDINCLUDELIST__MainWasCalled, TRUE)
end
subroutine MAINtestGETEXCLUDEDINCLUDELIST(null)
    record excludeRec (
        1 exclude[*]
            2 str = vc
    ) with protect
    set stat = alterlist(includeRec->qual, 3)
    set includeRec->qual[1].str = "cclsource:test_include.inc"
    set includeRec->qual[2].str = "cclsource:test_include10.inc"
    set includeRec->qual[3].str = "ccluserdir:test_include5.inc"
    call generateIncludeFileRegularExpressions(null)
    call GETEXCLUDEDINCLUDELIST(concat(CCLSOURCE_LOCATION, "ut_cclut_excluded_includes.inc"), excludeRec)
    set testGETEXCLUDEDINCLUDELIST__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testGETEXCLUDEDINCLUDELIST 001", size(excludeRec->exclude, 5), 4)
    call cclutAssertvcEqual(CURREF, "testGETEXCLUDEDINCLUDELIST 002", excludeRec->exclude[1].str,
        "%include cclsource:test_include2.inc")
    call cclutAssertvcEqual(CURREF, "testGETEXCLUDEDINCLUDELIST 003", excludeRec->exclude[2].str,
        "%i          cclsource:test_include3.inc")
    call cclutAssertvcEqual(CURREF, "testGETEXCLUDEDINCLUDELIST 004", excludeRec->exclude[3].str,
        "%i ccluserdir:test_include4.inc some junk characters")
    call cclutAssertvcEqual(CURREF, "testGETEXCLUDEDINCLUDELIST 005", excludeRec->exclude[4].str,
        "%INCLUDE CCLUSERDIR:TEST_INCLUDE6.INC")
end
subroutine (1_GETEXCLUDEDINCLUDELIST::ADDWARNING(null) = null)
   call cclexception(100, "E", "Should not have been called")
end

/* testGETEXCLUDEDINCLUDELIST_empty_script ******************************************************************
*  Scenario: Validates that a warning is added if the source script is empty or cannot be found.            *
************************************************************************************************************/
subroutine testGETEXCLUDEDINCLUDELIST_empty_script(null)
    declare testGETEXCLUDEDINCLUDELIST_empty_script__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare generateIncludeFileRegularExpressionsCalled = i2 with protect, noconstant(FALSE)
    declare addWarningCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGETEXCLUDEDINCLUDELIST_empty_script")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_GETEXCLUDEDINCLUDELIST")

    call cclutAsserti2Equal(CURREF, "testGETEXCLUDEDINCLUDELIST_empty_script 002",
        testGETEXCLUDEDINCLUDELIST_empty_script__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETEXCLUDEDINCLUDELIST_empty_script 003",
        generateIncludeFileRegularExpressionsCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGETEXCLUDEDINCLUDELIST_empty_script 004", addWarningCalled, TRUE)
end
subroutine MAINtestGETEXCLUDEDINCLUDELIST_empty_script(null)
    record excludeRec (
        1 exclude[*]
            2 str = vc
    ) with protect
    call GETEXCLUDEDINCLUDELIST("not an include", excludeRec)
    set testGETEXCLUDEDINCLUDELIST_empty_script__MainWasCalled = TRUE
end
subroutine (2_GETEXCLUDEDINCLUDELIST::GENERATEINCLUDEFILEREGULAREXPRESSIONS(null) = null)
    set generateIncludeFileRegularExpressionsCalled = TRUE
end
subroutine (2_GETEXCLUDEDINCLUDELIST::ADDWARNING(warning = vc) = null)
    set addWarningCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testGETEXCLUDEDINCLUDELIST_empty_script 001", warning, concat("Source file could ",
        "not be found or was empty at not an include.  All include file subroutines will be included."))
end

;**********************************************************************************************************************************
;** createExcludeXML
;**********************************************************************************************************************************
/* testCREATEEXCLUDEXML *************************************************************************************
*  Scenario: Validates that the XML for an include file is created and returned correctly.                  *
************************************************************************************************************/
subroutine testCREATEEXCLUDEXML(null)
    declare testCREATEEXCLUDEXML__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare cclutCompileProgramCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmpty = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestCREATEEXCLUDEXML")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_CREATEEXCLUDEXML")

    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML 008", testCREATEEXCLUDEXML__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML 009", cclutCompileProgramCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML 010", cclutIsEmpty, TRUE)
end
subroutine MAINtestCREATEEXCLUDEXML(null)
    declare xml = vc with protect, noconstant("")
    set xml = CREATEEXCLUDEXML("%i cclsource:ut_cclut_excluded_include.inc", 1)
    set testCREATEEXCLUDEXML__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML 007", xml, concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM. class="223" lev="0" kid="3" loc="1.15">', char(10),
    '  <USER. class="179" lev="1" kid="1" loc="1.15">', char(10),
    '   <NAME class="5" text="CCLUT_TMPL_INC1', cnvtupper(CCLUT_SUFFIX), '" lev="2" loc="1.15"/>', char(10),
    '  </USER.>', char(10),
    '  <Z_CALL. class="197" lev="1" kid="1" loc="4.5">', char(10),
    '   <CALL. class="125" lev="2" kid="2" loc="4.5">', char(10),
    '    <NAME class="5" text="ECHO" lev="3" loc="4.5"/>', char(10),
    '    <STRING class="7" text="This is a test include for the ut_cclut_generate_test_case.inc test case."',
        ' lev="3" loc="4.10"/>', char(10),
    '   </CALL.>', char(10),
    '  </Z_CALL.>', char(10),
    '  <LABEL. class="182" lev="1" kid="1" loc="0.0">', char(10),
    '   <NAME class="5" text="END" lev="2" loc="0.0"/>', char(10),
    '  </LABEL.>', char(10),
    ' </ZC_PROGRAM.>', char(10)))
end
subroutine (1_CREATEEXCLUDEXML::ADDWARNING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (1_CREATEEXCLUDEXML::CCLUTCOMPILEPROGRAM(sourceDirectory = vc, sourceLocation = vc, listingDirectory = vc,
    listingLocation = vc, failureMessage = vc) = i2)
        set cclutCompileProgramCalled = TRUE
        call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML 001", sourceDirectory, "CCLUSERDIR")
        call cclutAssertvcOperator(CURREF, "testCREATEEXCLUDEXML 002", sourceLocation, "regexplike",
            concat("cclut_tmpl_inc1", CCLUT_SUFFIX, ".prg"))
        call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML 003", listingDirectory, "CCLUSERDIR")
        call cclutAssertvcOperator(CURREF, "testCREATEEXCLUDEXML 004", listingLocation, "regexplike",
            concat("cclut_tmpl_inc1", CCLUT_SUFFIX, ".lis"))
        call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML 005", failureMessage, "")
        call PUBLIC::CCLUTCOMPILEPROGRAM(sourceDirectory, sourceLocation, listingDirectory, listingLocation,
            failureMessage)
        return (TRUE)
end
subroutine (1_CREATEEXCLUDEXML::ADDMESSAGE(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (1_CREATEEXCLUDEXML::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmpty = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML 006", parameter, concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM. class="223" lev="0" kid="3" loc="1.15">', char(10),
    '  <USER. class="179" lev="1" kid="1" loc="1.15">', char(10),
    '   <NAME class="5" text="CCLUT_TMPL_INC1', cnvtupper(CCLUT_SUFFIX), '" lev="2" loc="1.15"/>', char(10),
    '  </USER.>', char(10),
    '  <Z_CALL. class="197" lev="1" kid="1" loc="4.5">', char(10),
    '   <CALL. class="125" lev="2" kid="2" loc="4.5">', char(10),
    '    <NAME class="5" text="ECHO" lev="3" loc="4.5"/>', char(10),
    '    <STRING class="7" text="This is a test include for the ut_cclut_generate_test_case.inc test case."',
        ' lev="3" loc="4.10"/>', char(10),
    '   </CALL.>', char(10),
    '  </Z_CALL.>', char(10),
    '  <LABEL. class="182" lev="1" kid="1" loc="0.0">', char(10),
    '   <NAME class="5" text="END" lev="2" loc="0.0"/>', char(10),
    '  </LABEL.>', char(10),
    ' </ZC_PROGRAM.>', char(10)))
    return (FALSE)
end

/* testCREATEEXCLUDEXML_missing_file ************************************************************************
*  Scenario: Validates that a warning is added when an include file could not be found.                     *
************************************************************************************************************/
subroutine testCREATEEXCLUDEXML_missing_file(null)
    declare testCREATEEXCLUDEXML_missing_file__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare findfileCalled = i2 with protect, noconstant(FALSE)
    declare addWarningCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("findfile", "CREATEEXCLUDEXML_missing_file_findfile")

    call cclutAddMockImplementation("MAIN", "MAINtestCREATEEXCLUDEXML_missing_file")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_CREATEEXCLUDEXML")

    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_missing_file 004",
        testCREATEEXCLUDEXML_missing_file__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_missing_file 005", findFileCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_missing_file 006", addWarningCalled, TRUE)
end
subroutine MAINtestCREATEEXCLUDEXML_missing_file(null)
    declare xml = vc with protect, noconstant("")
    set xml = CREATEEXCLUDEXML("%i cclsource:ut_cclut_excluded_include.inc", 2)
    set testCREATEEXCLUDEXML_missing_file__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_missing_file 003", xml, "")
end
subroutine (2_CREATEEXCLUDEXML::ADDWARNING(warning = vc) = null)
    set addWarningCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_missing_file 002", warning, concat("Program file could not be created ",
        "for excluding the include file %i cclsource:ut_cclut_excluded_include.inc.  Its subroutines may appear in ",
        "the generated test file."))
end
subroutine (2_CREATEEXCLUDEXML::CCLUTCOMPILEPROGRAM(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (2_CREATEEXCLUDEXML::ADDMESSAGE(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (2_CREATEEXCLUDEXML::CCLUTISEMPTY(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (CREATEEXCLUDEXML_missing_file_findfile(file = vc) = i2 with protect)
    set findFileCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_missing_file 001", file,
        concat(CCLUSERDIR_LOCATION, "cclut_tmpl_inc2", CCLUT_SUFFIX, ".prg"))
    return (FALSE)
end

/* testCREATEEXCLUDEXML_compilation_failure *****************************************************************
*  Scenario: Validates that a message is added when an include file could not be compiled successfully.     *
************************************************************************************************************/
subroutine testCREATEEXCLUDEXML_compilation_failure(null)
    declare testCREATEEXCLUDEXML_compilation_failure__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare addMessageCalled = i2 with protect, noconstant(FALSE)
    declare cclutCompileProgramCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestCREATEEXCLUDEXML_compilation_failure")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "3_CREATEEXCLUDEXML")

    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_compilation_failure 008",
        testCREATEEXCLUDEXML_compilation_failure__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_compilation_failure 009", addMessageCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_compilation_failure 010", cclutCompileProgramCalled, TRUE)
end
subroutine MAINtestCREATEEXCLUDEXML_compilation_failure(null)
    declare xml = vc with protect, noconstant("")
    set xml = CREATEEXCLUDEXML("%i cclsource:ut_cclut_excluded_include.inc", 3)
    set testCREATEEXCLUDEXML_compilation_failure__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_compilation_failure 007", xml, "")
end
subroutine (3_CREATEEXCLUDEXML::ADDWARNING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (3_CREATEEXCLUDEXML::ADDMESSAGE(message = vc) = null)
    set addMessageCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_compilation_failure 006", message, concat("Program file ",
        "could not be compiled for excluding the include file %i cclsource:ut_cclut_excluded_include.inc.  Its ",
        "subroutines may appear in the generated test file.  Error from compilation: test failure"))
end
subroutine (3_CREATEEXCLUDEXML::CCLUTCOMPILEPROGRAM(sourceDirectory = vc, sourceLocation = vc, listingDirectory = vc,
    listingLocation = vc, failureMessage = vc(ref)) = i2)
        set cclutCompileProgramCalled = TRUE
        call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_compilation_failure 001", sourceDirectory, "CCLUSERDIR")
        call cclutAssertvcOperator(CURREF, "testCREATEEXCLUDEXML_compilation_failure 002", sourceLocation, "regexplike",
            concat("cclut_tmpl_inc3", CCLUT_SUFFIX, ".prg"))
        call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_compilation_failure 003", listingDirectory, "CCLUSERDIR")
        call cclutAssertvcOperator(CURREF, "testCREATEEXCLUDEXML_compilation_failure 004", listingLocation,
            "regexplike", concat("cclut_tmpl_inc3", CCLUT_SUFFIX, ".lis"))
        call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_compilation_failure 005", failureMessage, "")
        set failureMessage = "test failure"
        return (FALSE)
end
subroutine (3_CREATEEXCLUDEXML::CCLUTISEMPTY(null) = null)
    call cclexception(100, "E", "Should not have been called")
end

/* testCREATEEXCLUDEXML_empty_program ***********************************************************************
*  Scenario: Validates that a warning is added when an include file translation is empty.                   *
************************************************************************************************************/
subroutine testCREATEEXCLUDEXML_empty_program(null)
    declare testCREATEEXCLUDEXML_empty_program__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare addWarningCalled = i2 with protect, noconstant(FALSE)
    declare cclutCompileProgramCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmpty = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestCREATEEXCLUDEXML_empty_program")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "4_CREATEEXCLUDEXML")

    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_empty_program 009",
        testCREATEEXCLUDEXML_empty_program__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_empty_program 010", addWarningCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_empty_program 011", cclutCompileProgramCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEEXCLUDEXML_empty_program 012", cclutIsEmpty, TRUE)
end
subroutine MAINtestCREATEEXCLUDEXML_empty_program(null)
    declare xml = vc with protect, noconstant("")
    set xml = CREATEEXCLUDEXML("%i cclsource:ut_cclut_excluded_include.inc", 4)
    set testCREATEEXCLUDEXML_empty_program__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_empty_program 008", xml, concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM. class="223" lev="0" kid="3" loc="1.15">', char(10),
    '  <USER. class="179" lev="1" kid="1" loc="1.15">', char(10),
    '   <NAME class="5" text="CCLUT_TMPL_INC4', cnvtupper(CCLUT_SUFFIX), '" lev="2" loc="1.15"/>', char(10),
    '  </USER.>', char(10),
    '  <Z_CALL. class="197" lev="1" kid="1" loc="4.5">', char(10),
    '   <CALL. class="125" lev="2" kid="2" loc="4.5">', char(10),
    '    <NAME class="5" text="ECHO" lev="3" loc="4.5"/>', char(10),
    '    <STRING class="7" text="This is a test include for the ut_cclut_generate_test_case.inc test case."',
        ' lev="3" loc="4.10"/>', char(10),
    '   </CALL.>', char(10),
    '  </Z_CALL.>', char(10),
    '  <LABEL. class="182" lev="1" kid="1" loc="0.0">', char(10),
    '   <NAME class="5" text="END" lev="2" loc="0.0"/>', char(10),
    '  </LABEL.>', char(10),
    ' </ZC_PROGRAM.>', char(10)))
end
subroutine (4_CREATEEXCLUDEXML::ADDWARNING(warning = vc) = null)
    set addWarningCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_empty_program 007", warning, concat("Translation of program ",
        "file was empty for excluding the include file %i cclsource:ut_cclut_excluded_include.inc.  Its ",
        "subroutines may appear in the generated test file."))
end
subroutine (4_CREATEEXCLUDEXML::CCLUTCOMPILEPROGRAM(sourceDirectory = vc, sourceLocation = vc, listingDirectory = vc,
    listingLocation = vc, failureMessage = vc(ref)) = i2)
        set cclutCompileProgramCalled = TRUE
        call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_empty_program 001", sourceDirectory, "CCLUSERDIR")
        call cclutAssertvcOperator(CURREF, "testCREATEEXCLUDEXML_empty_program 002", sourceLocation, "regexplike",
            concat("cclut_tmpl_inc4", CCLUT_SUFFIX, ".prg"))
        call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_empty_program 003", listingDirectory, "CCLUSERDIR")
        call cclutAssertvcOperator(CURREF, "testCREATEEXCLUDEXML_empty_program 004", listingLocation,
            "regexplike", concat("cclut_tmpl_inc4", CCLUT_SUFFIX, ".lis"))
        call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML_empty_program 005", failureMessage, "")
        call PUBLIC::CCLUTCOMPILEPROGRAM(sourceDirectory, sourceLocation, listingDirectory, listingLocation,
            failureMessage)
        return (TRUE)
end
subroutine (1_CREATEEXCLUDEXML::ADDMESSAGE(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (4_CREATEEXCLUDEXML::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmpty = TRUE
    call cclutAssertvcEqual(CURREF, "testCREATEEXCLUDEXML 006", parameter, concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM. class="223" lev="0" kid="3" loc="1.15">', char(10),
    '  <USER. class="179" lev="1" kid="1" loc="1.15">', char(10),
    '   <NAME class="5" text="CCLUT_TMPL_INC4', cnvtupper(CCLUT_SUFFIX), '" lev="2" loc="1.15"/>', char(10),
    '  </USER.>', char(10),
    '  <Z_CALL. class="197" lev="1" kid="1" loc="4.5">', char(10),
    '   <CALL. class="125" lev="2" kid="2" loc="4.5">', char(10),
    '    <NAME class="5" text="ECHO" lev="3" loc="4.5"/>', char(10),
    '    <STRING class="7" text="This is a test include for the ut_cclut_generate_test_case.inc test case."',
        ' lev="3" loc="4.10"/>', char(10),
    '   </CALL.>', char(10),
    '  </Z_CALL.>', char(10),
    '  <LABEL. class="182" lev="1" kid="1" loc="0.0">', char(10),
    '   <NAME class="5" text="END" lev="2" loc="0.0"/>', char(10),
    '  </LABEL.>', char(10),
    ' </ZC_PROGRAM.>', char(10)))
    return (TRUE)
end

;**********************************************************************************************************************************
;** identifySubroutinesToTest
;**********************************************************************************************************************************
/* testIDENTIFYSUBROUTINESTOTEST ****************************************************************************
*  Scenario: Validates that subroutines are removed from the targetSubroutine list if they are excluded.    *
************************************************************************************************************/
subroutine testIDENTIFYSUBROUTINESTOTEST(null)
    declare testIDENTIFYSUBROUTINESTOTEST__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare findProgramSubroutinesCount = i4 with protect, noconstant(0)
    declare getExcludedIncludeListCalled = i2 with protect, noconstant(FALSE)
    declare createExcludeXMLCount = i4 with protect, noconstant(0)
    declare populateMockableSubroutinesCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestIDENTIFYSUBROUTINESTOTEST")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_IDENTIFYSUBROUTINESTOTEST")

    call cclutAsserti2Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 031", testIDENTIFYSUBROUTINESTOTEST__MainWasCalled,
        TRUE)
    call cclutAsserti4Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 032", findProgramSubroutinesCount, 3)
    call cclutAsserti2Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 033", getExcludedIncludeListCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 034", createExcludeXMLCount, 3)
    call cclutAsserti2Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 035", populateMockableSubroutinesCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 036", cclutIsEmptyCount, 3)
end
subroutine MAINtestIDENTIFYSUBROUTINESTOTEST(null)
    call IDENTIFYSUBROUTINESTOTEST("test xml", "test location", "test script")
    set testIDENTIFYSUBROUTINESTOTEST__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 028", size(templateRec->targetSubroutines, 5), 2)
    call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 029", templateRec->targetSubroutines[1].name,
        "test subroutine 1")
    call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 030", templateRec->targetSubroutines[2].name,
        "test subroutine 3")
end
subroutine (1_IDENTIFYSUBROUTINESTOTEST::FINDPROGRAMSUBROUTINES(programXML = vc, targetRec = vc(ref),
    xmlFailureError = i2, objectName = vc) = null)
        set findProgramSubroutinesCount = findProgramSubroutinesCount + 1
        case (findProgramSubroutinesCount)
            of 1:
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 001", programXML, "test xml")
                set stat = alterlist(targetRec->targetSubroutines, 3)
                set targetRec->targetSubroutines[1].name = "test subroutine 1"
                set targetRec->targetSubroutines[2].name = "test subroutine 2"
                set targetRec->targetSubroutines[3].name = "test subroutine 3"
                call cclutAsserti2Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 002", xmlFailureError, TRUE)
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 003", objectName, "test script")
            of 2:
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 008", programXML, "include xml 1")
                call cclutAsserti4Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 009", size(targetRec->exclude, 5), 3)
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 010", targetRec->exclude[1].str,
                    "test include 1")
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 011", targetRec->exclude[2].str,
                    "test include 2")
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 012", targetRec->exclude[3].str,
                    "test include 3")
                call cclutAsserti2Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 013", xmlFailureError, FALSE)
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 014", objectName, "test include 1")
            of 3:
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 021", programXML, "include xml 3")
                call cclutAsserti4Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 022", size(targetRec->exclude, 5), 3)
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 023", targetRec->exclude[1].str,
                    "test include 1")
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 024", targetRec->exclude[2].str,
                    "test include 2")
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 025", targetRec->exclude[3].str,
                    "test include 3")
                call cclutAsserti2Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 026", xmlFailureError, FALSE)
                call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 027", objectName, "test include 3")
                set stat = alterlist(targetRec->targetSubroutines, 3)
                set targetRec->targetSubroutines[1].name = "test subroutine 4"
                set targetRec->targetSubroutines[2].name = "test subroutine 5"
                set targetRec->targetSubroutines[3].name = "test subroutine 2"
        endcase
end
subroutine (1_IDENTIFYSUBROUTINESTOTEST::POPULATEMOCKABLESUBROUTINES(templateRec = vc(ref)) = null)
    set populateMockableSubroutinesCalled = TRUE
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "test subroutine 1"
    set templateRec->targetSubroutines[2].name = "test subroutine 2"
    set templateRec->targetSubroutines[3].name = "test subroutine 3"
end
subroutine (1_IDENTIFYSUBROUTINESTOTEST::GETEXCLUDEDINCLUDELIST(sourceFileLocation = vc, excludeRec = vc(ref)) = null)
    set getExcludedIncludeListCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 004", sourceFileLocation, "test location")
    set stat = alterlist(excludeRec->exclude, 3)
    set excludeRec->exclude[1].str = "test include 1"
    set excludeRec->exclude[2].str = "test include 2"
    set excludeRec->exclude[3].str = "test include 3"
end
subroutine (1_IDENTIFYSUBROUTINESTOTEST::CREATEEXCLUDEXML(includeFile = vc, includeIndex = i4) = vc)
    set createExcludeXMLCount = createExcludeXMLCount + 1
    case (createExcludeXMLCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 005", includeFile, "test include 1")
            call cclutAsserti4Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 006", includeIndex, 1)
            return ("include xml 1")
        of 2:
            call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 015", includeFile, "test include 2")
            call cclutAsserti4Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 016", includeIndex, 2)
            return ("include xml 2")
        of 3:
            call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 018", includeFile, "test include 3")
            call cclutAsserti4Equal(CURREF, "testIDENTIFYSUBROUTINESTOTEST 019", includeIndex, 3)
            return ("include xml 3")
    endcase
end
subroutine (1_IDENTIFYSUBROUTINESTOTEST::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 007", parameter, "include xml 1")
            return (FALSE)
        of 2:
            call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 017", parameter, "include xml 2")
            return (TRUE)
        of 3:
            call cclutAssertvcEqual(CURREF, "testIDENTIFYSUBROUTINESTOTEST 020", parameter, "include xml 3")
            return (FALSE)
    endcase
end

;**********************************************************************************************************************************
;** handleSubroutineNodeMocking
;**********************************************************************************************************************************
/* testHANDLESUBROUTINENODEMOCKING **************************************************************************
*  Scenario: Validates that a subroutine is added as a mock if it is within the same program.               *
************************************************************************************************************/
subroutine testHANDLESUBROUTINENODEMOCKING(null)
    declare testHANDLESUBROUTINENODEMOCKING__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLESUBROUTINENODEMOCKING")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_HANDLESUBROUTINENODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING 015",
        testHANDLESUBROUTINENODEMOCKING__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING 016", getNamedElementNameCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING 017", cclutIsEmptyCalled, TRUE)
end
subroutine MAINtestHANDLESUBROUTINENODEMOCKING(null)
    set stat = alterlist(templateRec->mockableSubroutines, 3)
    set templateRec->mockableSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->mockableSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->mockableSubroutines[3].name = "TESTSUBROUTINE3"
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE4"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE5"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE6"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <CALL.>', char(10),
    '  <NAME text="TESTSUBROUTINE2" />', char(10),
    ' </CALL.>', char(10)), hXmlFile)
    call HANDLESUBROUTINENODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "CALL.", 1), 1)
    set testHANDLESUBROUTINENODEMOCKING__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING 003", size(templateRec->mockableSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING 004", templateRec->mockableSubroutines[1].name,
        "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING 005", templateRec->mockableSubroutines[2].name,
        "TESTSUBROUTINE2")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING 006", templateRec->mockableSubroutines[3].name,
        "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING 007", size(templateRec->targetSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING 008", templateRec->targetSubroutines[1].name,
        "TESTSUBROUTINE4")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING 009", templateRec->targetSubroutines[2].name,
        "TESTSUBROUTINE5")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING 010", templateRec->targetSubroutines[3].name,
        "TESTSUBROUTINE6")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING 011",
        size(templateRec->targetSubroutines[1].mockSubroutines, 5), 1)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING 012",
        templateRec->targetSubroutines[1].mockSubroutines[1].name, "TESTSUBROUTINE2")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING 013",
        size(templateRec->targetSubroutines[2].mockSubroutines, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING 014",
        size(templateRec->targetSubroutines[3].mockSubroutines, 5), 0)
end
subroutine (1_HANDLESUBROUTINENODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING 001", index, 1)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (1_HANDLESUBROUTINENODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING 002", parameter, "TESTSUBROUTINE2")
    return (FALSE)
end

/* testHANDLESUBROUTINENODEMOCKING_empty_node ***************************************************************
*  Scenario: Validates that a subroutine is not added as a mock if the element has an empty name.           *
************************************************************************************************************/
subroutine testHANDLESUBROUTINENODEMOCKING_empty_node(null)
    declare testHANDLESUBROUTINENODEMOCKING_empty_node__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLESUBROUTINENODEMOCKING_empty_node")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_HANDLESUBROUTINENODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 014",
        testHANDLESUBROUTINENODEMOCKING_empty_node__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 015", getNamedElementNameCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 016", cclutIsEmptyCalled, TRUE)
end
subroutine MAINtestHANDLESUBROUTINENODEMOCKING_empty_node(null)
    set stat = alterlist(templateRec->mockableSubroutines, 3)
    set templateRec->mockableSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->mockableSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->mockableSubroutines[3].name = "TESTSUBROUTINE3"
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE4"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE5"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE6"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <CALL.>', char(10),
    '  <NAME text="TESTSUBROUTINE2" />', char(10),
    ' </CALL.>', char(10)), hXmlFile)
    call HANDLESUBROUTINENODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "CALL.", 1), 1)
    set testHANDLESUBROUTINENODEMOCKING_empty_node__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 003",
        size(templateRec->mockableSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 004",
        templateRec->mockableSubroutines[1].name, "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 005",
        templateRec->mockableSubroutines[2].name, "TESTSUBROUTINE2")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 006",
        templateRec->mockableSubroutines[3].name, "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 007",
        size(templateRec->targetSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 008",
        templateRec->targetSubroutines[1].name, "TESTSUBROUTINE4")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 009",
        templateRec->targetSubroutines[2].name, "TESTSUBROUTINE5")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 010",
        templateRec->targetSubroutines[3].name, "TESTSUBROUTINE6")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 011",
        size(templateRec->targetSubroutines[1].mockSubroutines, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 012",
        size(templateRec->targetSubroutines[2].mockSubroutines, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 013",
        size(templateRec->targetSubroutines[3].mockSubroutines, 5), 0)
end
subroutine (2_HANDLESUBROUTINENODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 001", index, 1)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (2_HANDLESUBROUTINENODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_empty_node 002", parameter, "TESTSUBROUTINE2")
    return (TRUE)
end

/* testHANDLESUBROUTINENODEMOCKING_undefined_subroutine *****************************************************
*  Scenario: Validates that a subroutine is not added as a mock if the subroutine is not defined.           *
************************************************************************************************************/
subroutine testHANDLESUBROUTINENODEMOCKING_undefined_subroutine(null)
    declare testHANDLESUBROUTINENODEMOCKING_undefined_subroutine__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLESUBROUTINENODEMOCKING_undefined_subroutine")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "3_HANDLESUBROUTINENODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 013",
        testHANDLESUBROUTINENODEMOCKING_undefined_subroutine__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 014",
        getNamedElementNameCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 015", cclutIsEmptyCalled,
        TRUE)
end
subroutine MAINtestHANDLESUBROUTINENODEMOCKING_undefined_subroutine(null)
    set stat = alterlist(templateRec->mockableSubroutines, 2)
    set templateRec->mockableSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->mockableSubroutines[2].name = "TESTSUBROUTINE3"
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE4"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE5"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE6"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <CALL.>', char(10),
    '  <NAME text="TESTSUBROUTINE2" />', char(10),
    ' </CALL.>', char(10)), hXmlFile)
    call HANDLESUBROUTINENODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "CALL.", 1), 1)
    set testHANDLESUBROUTINENODEMOCKING_undefined_subroutine__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 003",
        size(templateRec->mockableSubroutines, 5), 2)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 004",
        templateRec->mockableSubroutines[1].name, "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 005",
        templateRec->mockableSubroutines[2].name, "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 006",
        size(templateRec->targetSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 007",
        templateRec->targetSubroutines[1].name, "TESTSUBROUTINE4")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 008",
        templateRec->targetSubroutines[2].name, "TESTSUBROUTINE5")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 009",
        templateRec->targetSubroutines[3].name, "TESTSUBROUTINE6")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 010",
        size(templateRec->targetSubroutines[1].mockSubroutines, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 011",
        size(templateRec->targetSubroutines[2].mockSubroutines, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 012",
        size(templateRec->targetSubroutines[3].mockSubroutines, 5), 0)
end
subroutine (3_HANDLESUBROUTINENODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 001", index, 1)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (3_HANDLESUBROUTINENODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_undefined_subroutine 002", parameter,
        "TESTSUBROUTINE2")
    return (FALSE)
end

/* testHANDLESUBROUTINENODEMOCKING_already_mocked ***********************************************************
*  Scenario: Validates that a subroutine is not added as a mock if the subroutine is already mocked.        *
************************************************************************************************************/
subroutine testHANDLESUBROUTINENODEMOCKING_already_mocked(null)
    declare testHANDLESUBROUTINENODEMOCKING_already_mocked__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLESUBROUTINENODEMOCKING_already_mocked")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "4_HANDLESUBROUTINENODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 015",
        testHANDLESUBROUTINENODEMOCKING_already_mocked__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 016",
        getNamedElementNameCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 017", cclutIsEmptyCalled,
        TRUE)
end
subroutine MAINtestHANDLESUBROUTINENODEMOCKING_already_mocked(null)
    set stat = alterlist(templateRec->mockableSubroutines, 3)
    set templateRec->mockableSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->mockableSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->mockableSubroutines[3].name = "TESTSUBROUTINE3"
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE4"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE5"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE6"
    set stat = alterlist(templateRec->targetSubroutines[2].mockSubroutines, 1)
    set templateRec->targetSubroutines[2].mockSubroutines[1].name = "TESTSUBROUTINE2"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <CALL.>', char(10),
    '  <NAME text="TESTSUBROUTINE2" />', char(10),
    ' </CALL.>', char(10)), hXmlFile)
    call HANDLESUBROUTINENODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "CALL.", 1), 2)
    set testHANDLESUBROUTINENODEMOCKING_already_mocked__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 003",
        size(templateRec->mockableSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 004",
        templateRec->mockableSubroutines[1].name, "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 005",
        templateRec->mockableSubroutines[2].name, "TESTSUBROUTINE2")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 006",
        templateRec->mockableSubroutines[3].name, "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 007",
        size(templateRec->targetSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 008",
        templateRec->targetSubroutines[1].name, "TESTSUBROUTINE4")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 009",
        templateRec->targetSubroutines[2].name, "TESTSUBROUTINE5")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 010",
        templateRec->targetSubroutines[3].name, "TESTSUBROUTINE6")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 011",
        size(templateRec->targetSubroutines[1].mockSubroutines, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 012",
        size(templateRec->targetSubroutines[2].mockSubroutines, 5), 1)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 013",
        templateRec->targetSubroutines[2].mockSubroutines[1].name, "TESTSUBROUTINE2")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 014",
        size(templateRec->targetSubroutines[3].mockSubroutines, 5), 0)
end
subroutine (4_HANDLESUBROUTINENODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 001", index, 1)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (4_HANDLESUBROUTINENODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_already_mocked 002", parameter,
        "TESTSUBROUTINE2")
    return (FALSE)
end

/* testHANDLESUBROUTINENODEMOCKING_recursive_subroutine *****************************************************
*  Scenario: Validates that a subroutine is not added as a mock if the subroutine is recursive.             *
************************************************************************************************************/
subroutine testHANDLESUBROUTINENODEMOCKING_recursive_subroutine(null)
    declare testHANDLESUBROUTINENODEMOCKING_recursive_subroutine__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLESUBROUTINENODEMOCKING_recursive_subroutine")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "5_HANDLESUBROUTINENODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 015",
        testHANDLESUBROUTINENODEMOCKING_recursive_subroutine__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 016",
        getNamedElementNameCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 017", cclutIsEmptyCalled,
        TRUE)
end
subroutine MAINtestHANDLESUBROUTINENODEMOCKING_recursive_subroutine(null)
    set stat = alterlist(templateRec->mockableSubroutines, 3)
    set templateRec->mockableSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->mockableSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->mockableSubroutines[3].name = "TESTSUBROUTINE3"
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE4"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE5"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE2"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <CALL.>', char(10),
    '  <NAME text="TESTSUBROUTINE2" />', char(10),
    ' </CALL.>', char(10)), hXmlFile)
    call HANDLESUBROUTINENODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "CALL.", 1), 3)
    set testHANDLESUBROUTINENODEMOCKING_recursive_subroutine__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 003",
        size(templateRec->mockableSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 004",
        templateRec->mockableSubroutines[1].name, "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 005",
        templateRec->mockableSubroutines[2].name, "TESTSUBROUTINE2")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 006",
        templateRec->mockableSubroutines[3].name, "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 007",
        size(templateRec->targetSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 008",
        templateRec->targetSubroutines[1].name, "TESTSUBROUTINE4")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 009",
        templateRec->targetSubroutines[2].name, "TESTSUBROUTINE5")
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 010",
        templateRec->targetSubroutines[3].name, "TESTSUBROUTINE2")
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 011",
        size(templateRec->targetSubroutines[1].mockSubroutines, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 012",
        size(templateRec->targetSubroutines[2].mockSubroutines, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 014",
        size(templateRec->targetSubroutines[3].mockSubroutines, 5), 0)
end
subroutine (5_HANDLESUBROUTINENODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 001", index, 1)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (5_HANDLESUBROUTINENODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testHANDLESUBROUTINENODEMOCKING_recursive_subroutine 002", parameter,
        "TESTSUBROUTINE2")
    return (FALSE)
end

;**********************************************************************************************************************************
;** handleScriptNodeMocking
;**********************************************************************************************************************************
/* testHANDLESCRIPTNODEMOCKING ******************************************************************************
*  Scenario: Validates that a script is added as a mock if it has not already been mocked.                  *
************************************************************************************************************/
subroutine testHANDLESCRIPTNODEMOCKING(null)
    declare testHANDLESCRIPTNODEMOCKING__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLESCRIPTNODEMOCKING")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_HANDLESCRIPTNODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLESCRIPTNODEMOCKING 011", testHANDLESCRIPTNODEMOCKING__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESCRIPTNODEMOCKING 012",
        getNamedElementNameCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESCRIPTNODEMOCKING 013", cclutIsEmptyCalled,
        TRUE)
end
subroutine MAINtestHANDLESCRIPTNODEMOCKING(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE3"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_EXECUTE.>', char(10),
    '  <USER.>', char(10),
    '   <NAME text="TESTSCRIPT" />', char(10),
    '  </USER.>', char(10),
    ' </Z_EXECUTE.>', char(10)), hXmlFile)
    call HANDLESCRIPTNODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "Z_EXECUTE.", 1), 1)
    set testHANDLESCRIPTNODEMOCKING__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING 003",
        size(templateRec->targetSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING 004",
        templateRec->targetSubroutines[1].name, "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING 005",
        templateRec->targetSubroutines[2].name, "TESTSUBROUTINE2")
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING 006",
        templateRec->targetSubroutines[3].name, "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING 007",
        size(templateRec->targetSubroutines[1].mockScripts, 5), 1)
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING 008",
        templateRec->targetSubroutines[1].mockScripts[1].name, "TESTSCRIPT")
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING 009",
        size(templateRec->targetSubroutines[2].mockScripts, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING 010",
        size(templateRec->targetSubroutines[3].mockScripts, 5), 0)
end
subroutine (1_HANDLESCRIPTNODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING 001", index, 1)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (1_HANDLESCRIPTNODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING 002", parameter,
        "TESTSCRIPT")
    return (FALSE)
end

/* testHANDLESCRIPTNODEMOCKING_empty_node *******************************************************************
*  Scenario: Validates that a script is not added as a mock if the element has an empty name.               *
************************************************************************************************************/
subroutine testHANDLESCRIPTNODEMOCKING_empty_node(null)
    declare testHANDLESCRIPTNODEMOCKING_empty_node__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLESCRIPTNODEMOCKING_empty_node")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_HANDLESCRIPTNODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 010",
        testHANDLESCRIPTNODEMOCKING_empty_node__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 011",
        getNamedElementNameCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 012", cclutIsEmptyCalled,
        TRUE)
end
subroutine MAINtestHANDLESCRIPTNODEMOCKING_empty_node(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE3"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_EXECUTE.>', char(10),
    '  <USER.>', char(10),
    '   <NAME text="TESTSCRIPT" />', char(10),
    '  </USER.>', char(10),
    ' </Z_EXECUTE.>', char(10)), hXmlFile)
    call HANDLESCRIPTNODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "Z_EXECUTE.", 1), 1)
    set testHANDLESCRIPTNODEMOCKING_empty_node__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 003",
        size(templateRec->targetSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 004",
        templateRec->targetSubroutines[1].name, "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 005",
        templateRec->targetSubroutines[2].name, "TESTSUBROUTINE2")
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 006",
        templateRec->targetSubroutines[3].name, "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 007",
        size(templateRec->targetSubroutines[1].mockScripts, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 008",
        size(templateRec->targetSubroutines[2].mockScripts, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 009",
        size(templateRec->targetSubroutines[3].mockScripts, 5), 0)
end
subroutine (2_HANDLESCRIPTNODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 001", index, 1)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (2_HANDLESCRIPTNODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING_empty_node 002", parameter,
        "TESTSCRIPT")
    return (TRUE)
end

/* testHANDLESCRIPTNODEMOCKING_already_mocked ***************************************************************
*  Scenario: Validates that a script is not added as a mock if the script was already mocked.               *
************************************************************************************************************/
subroutine testHANDLESCRIPTNODEMOCKING_already_mocked(null)
    declare testHANDLESCRIPTNODEMOCKING_already_mocked__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCalled = i2 with protect, noconstant(FALSE)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLESCRIPTNODEMOCKING_already_mocked")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "3_HANDLESCRIPTNODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 011",
        testHANDLESCRIPTNODEMOCKING_already_mocked__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 012",
        getNamedElementNameCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 013", cclutIsEmptyCalled,
        TRUE)
end
subroutine MAINtestHANDLESCRIPTNODEMOCKING_already_mocked(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE3"
    set stat = alterlist(templateRec->targetSubroutines[2].mockScripts, 1)
    set templateRec->targetSubroutines[2].mockScripts[1].name = "TESTSCRIPT"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_EXECUTE.>', char(10),
    '  <USER.>', char(10),
    '   <NAME text="TESTSCRIPT" />', char(10),
    '  </USER.>', char(10),
    ' </Z_EXECUTE.>', char(10)), hXmlFile)
    call HANDLESCRIPTNODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "Z_EXECUTE.", 1), 2)
    set testHANDLESCRIPTNODEMOCKING_already_mocked__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 003",
        size(templateRec->targetSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 004",
        templateRec->targetSubroutines[1].name, "TESTSUBROUTINE1")
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 005",
        templateRec->targetSubroutines[2].name, "TESTSUBROUTINE2")
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 006",
        templateRec->targetSubroutines[3].name, "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 007",
        size(templateRec->targetSubroutines[1].mockScripts, 5), 0)
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 008",
        size(templateRec->targetSubroutines[2].mockScripts, 5), 1)
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 009",
        templateRec->targetSubroutines[2].mockScripts[1].name, "TESTSCRIPT")
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 010",
        size(templateRec->targetSubroutines[3].mockScripts, 5), 0)
end
subroutine (3_HANDLESCRIPTNODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 001", index, 1)
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (3_HANDLESCRIPTNODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testHANDLESCRIPTNODEMOCKING_already_mocked 002", parameter,
        "TESTSCRIPT")
    return (TRUE)
end

;**********************************************************************************************************************************
;** mergeTableRecs
;**********************************************************************************************************************************
/* testMERGETABLERECS ***************************************************************************************
*  Scenario: Validates that tables and columns to be mocked are correctly merged for the given subroutine.  *
************************************************************************************************************/
subroutine testMERGETABLERECS(null)
    declare testMERGETABLERECS__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare addWarningCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestMERGETABLERECS")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_MERGETABLERECS")

    call cclutAsserti2Equal(CURREF, "testMERGETABLERECS 020", testMERGETABLERECS__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testMERGETABLERECS 021", addWarningCalled, TRUE)
end
subroutine MAINtestMERGETABLERECS(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE2"
    set stat = alterlist(templateRec->targetSubroutines[2].mockTables, 1)
    set templateRec->targetSubroutines[2].mockTables[1].name = "test table 3"
    set stat = alterlist(templateRec->targetSubroutines[2].mockTables[1].columns, 1)
    set templateRec->targetSubroutines[2].mockTables[1].columns[1].name = "test table 3 column 2"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE3"
    record mockTableRec (
        1 tables[*]
            2 name = vc
            2 columns[*]
                3 name = vc
    ) with protect
    set stat = alterlist(mockTableRec->tables, 3)
    set mockTableRec->tables[1].name = "test table 1"
    set mockTableRec->tables[2].name = "test table 2"
    set stat = alterlist(mockTableRec->tables[2].columns, 3)
    set mockTableRec->tables[2].columns[1].name = "test table 2 column 1"
    set mockTableRec->tables[2].columns[2].name = "test table 2 column 2"
    set mockTableRec->tables[2].columns[3].name = "test table 2 column 3"
    set mockTableRec->tables[3].name = "test table 3"
    set stat = alterlist(mockTableRec->tables[3].columns, 3)
    set mockTableRec->tables[3].columns[1].name = "test table 3 column 1"
    set mockTableRec->tables[3].columns[2].name = "test table 3 column 2"
    set mockTableRec->tables[3].columns[3].name = "test table 3 column 3"
    call MERGETABLERECS(2)
    set testMERGETABLERECS__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testMERGETABLERECS 002", size(templateRec->targetSubroutines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 003", templateRec->targetSubroutines[1].name, "TESTSUBROUTINE1")
    call cclutAsserti4Equal(CURREF, "testMERGETABLERECS 004", size(templateRec->targetSubroutines[1].mockTables, 5), 0)
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 005", templateRec->targetSubroutines[2].name, "TESTSUBROUTINE2")
    call cclutAsserti4Equal(CURREF, "testMERGETABLERECS 006", size(templateRec->targetSubroutines[2].mockTables, 5), 3)
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 007", templateRec->targetSubroutines[2].mockTables[1].name,
        "test table 3")
    call cclutAsserti4Equal(CURREF, "testMERGETABLERECS 008",
        size(templateRec->targetSubroutines[2].mockTables[1].columns, 5), 3)
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 009",
        templateRec->targetSubroutines[2].mockTables[1].columns[1].name, "test table 3 column 2")
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 010",
        templateRec->targetSubroutines[2].mockTables[1].columns[2].name, "test table 3 column 1")
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 011",
        templateRec->targetSubroutines[2].mockTables[1].columns[3].name, "test table 3 column 3")
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 012", templateRec->targetSubroutines[2].mockTables[2].name,
        "test table 1")
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 013", templateRec->targetSubroutines[2].mockTables[3].name,
        "test table 2")
    call cclutAsserti4Equal(CURREF, "testMERGETABLERECS 014",
        size(templateRec->targetSubroutines[2].mockTables[3].columns, 5), 3)
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 015",
        templateRec->targetSubroutines[2].mockTables[3].columns[1].name, "test table 2 column 1")
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 016",
        templateRec->targetSubroutines[2].mockTables[3].columns[2].name, "test table 2 column 2")
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 017",
        templateRec->targetSubroutines[2].mockTables[3].columns[3].name, "test table 2 column 3")
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 018", templateRec->targetSubroutines[3].name, "TESTSUBROUTINE3")
    call cclutAsserti4Equal(CURREF, "testMERGETABLERECS 019", size(templateRec->targetSubroutines[3].mockTables, 5), 0)
end
subroutine (1_MERGETABLERECS::ADDWARNING(parameter = vc) = null)
    set addWarningCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testMERGETABLERECS 001", parameter, concat("No columns could be identified for ",
        "one of the queries involving table test table 1.  It is recommended to always use table aliases when ",
        "referencing columns.  If the program is correct, you will need to add columns to the instances of ",
        "cclutDefineMockTable in this generated test case that use the table."))
end

;**********************************************************************************************************************************
;** addMockTablesAndAliases
;**********************************************************************************************************************************
/* testADDMOCKTABLESANDALIASES ******************************************************************************
*  Scenario: Validates that tables and aliases are correctly added to the set of mocks.                     *
************************************************************************************************************/
subroutine testADDMOCKTABLESANDALIASES(null)
    declare testADDMOCKTABLESANDALIASES__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestADDMOCKTABLESANDALIASES")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_ADDMOCKTABLESANDALIASES")

    call cclutAsserti2Equal(CURREF, "testADDMOCKTABLESANDALIASES 017", testADDMOCKTABLESANDALIASES__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES 018", getNamedElementNameCount, 6)
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES 019", cclutIsEmptyCount, 6)
end
subroutine MAINtestADDMOCKTABLESANDALIASES(null)
    record mockTableRec (
        1 tables[*]
            2 name = vc
            2 aliases[*]
                3 alias = vc
    ) with protect
    set stat = alterlist(mockTableRec->tables, 1)
    set mockTableRec->tables[1].name = "TESTTABLE2"
    set stat = alterlist(mockTableRec->tables[1].aliases, 1)
    set mockTableRec->tables[1].aliases[1].alias = "TT2"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_SELECT.>', char(10),
    '  <COMMA. />', char(10),
    '  <COMMA. />', char(10),
    '  <COMMA.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="TESTTABLE1" />', char(10),
    '    <NAME text="TT1" />', char(10),
    '   </TABLE.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="TESTTABLE2" />', char(10),
    '    <NAME text="TT2" />', char(10),
    '   </TABLE.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="TESTTABLE3" />', char(10),
    '   </TABLE.>', char(10),
    '  </COMMA.>', char(10),
    ' </Z_SELECT.>', char(10)), hXmlFile)
    call ADDMOCKTABLESANDALIASES(cclut::getXmlListItemHandle(hXmlTest, "Z_SELECT.", 1), TRUE)
    set testADDMOCKTABLESANDALIASES__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES 009", size(mockTableRec->tables, 5), 3)
    call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 010", mockTableRec->tables[1].name, "TESTTABLE2")
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES 011", size(mockTableRec->tables[1].aliases, 5), 1)
    call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 012", mockTableRec->tables[1].aliases[1].alias,
        "TT2")
    call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 013", mockTableRec->tables[2].name, "TESTTABLE1")
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES 014", size(mockTableRec->tables[2].aliases, 5), 1)
    call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 015", mockTableRec->tables[2].aliases[1].alias,
        "TT1")
    call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 016", mockTableRec->tables[3].name, "TESTTABLE3")
end
subroutine (1_ADDMOCKTABLESANDALIASES::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    case (getNamedElementNameCount)
        of 1:
        of 3:
        of 5:
            call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES 001", index, 1)
        of 2:
        of 4:
        of 6:
            call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES 002", index, 2)
    endcase
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (1_ADDMOCKTABLESANDALIASES::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 003", parameter, "TESTTABLE1")
            return (FALSE)
        of 2:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 004", parameter, "TT1")
            return (FALSE)
        of 3:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 005", parameter, "TESTTABLE2")
            return (FALSE)
        of 4:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 006", parameter, "TT2")
            return (FALSE)
        of 5:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 007", parameter, "TESTTABLE3")
            return (FALSE)
        of 6:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES 008", parameter, trim(""))
            return (TRUE)
    endcase
end

/* testADDMOCKTABLESANDALIASES_dummyt_dual ******************************************************************
*  Scenario: Validates that dummyt and dual tables are excluded from being mocked.                          *
************************************************************************************************************/
subroutine testADDMOCKTABLESANDALIASES_dummyt_dual(null)
    declare testADDMOCKTABLESANDALIASES_dummyt_dual__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestADDMOCKTABLESANDALIASES_dummyt_dual")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_ADDMOCKTABLESANDALIASES")

    call cclutAsserti2Equal(CURREF, "testADDMOCKTABLESANDALIASES_dummyt_dual 007",
        testADDMOCKTABLESANDALIASES_dummyt_dual__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_dummyt_dual 008", getNamedElementNameCount, 6)
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_dummyt_dual 009", cclutIsEmptyCount, 3)
end
subroutine MAINtestADDMOCKTABLESANDALIASES_dummyt_dual(null)
    record mockTableRec (
        1 tables[*]
            2 name = vc
            2 aliases[*]
                3 alias = vc
    ) with protect
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_INSERT.>', char(10),
    '  <COMMA.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="DUMMYT" />', char(10),
    '    <NAME text="D1" />', char(10),
    '   </TABLE.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="DUAL" />', char(10),
    '    <NAME text="D" />', char(10),
    '   </TABLE.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME />', char(10),
    '   </TABLE.>', char(10),
    '  </COMMA.>', char(10),
    ' </Z_INSERT.>', char(10)), hXmlFile)
    call ADDMOCKTABLESANDALIASES(cclut::getXmlListItemHandle(hXmlTest, "Z_INSERT.", 1), FALSE)
    set testADDMOCKTABLESANDALIASES_dummyt_dual__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_dummyt_dual 006", size(mockTableRec->tables, 5), 0)
end
subroutine (2_ADDMOCKTABLESANDALIASES::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    case (getNamedElementNameCount)
        of 1:
        of 3:
        of 5:
            call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_dummyt_dual 001", index, 1)
        of 2:
        of 4:
        of 6:
            call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_dummyt_dual 002", index, 2)
    endcase
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (2_ADDMOCKTABLESANDALIASES::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_dummyt_dual 003", parameter, "DUMMYT")
            return (FALSE)
        of 2:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_dummyt_dual 004", parameter, "DUAL")
            return (FALSE)
        of 3:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_dummyt_dual 005", parameter, trim(""))
            return (TRUE)
    endcase
end

/* testADDMOCKTABLESANDALIASES_inline_select ****************************************************************
*  Scenario: Validates that inline select tables are ignored from being mocked until a future run.          *
************************************************************************************************************/
subroutine testADDMOCKTABLESANDALIASES_inline_select(null)
    declare testADDMOCKTABLESANDALIASES_inline_select__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestADDMOCKTABLESANDALIASES_inline_select")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "3_ADDMOCKTABLESANDALIASES")

    call cclutAsserti2Equal(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 013",
        testADDMOCKTABLESANDALIASES_inline_select__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 014", getNamedElementNameCount, 6)
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 015", cclutIsEmptyCount, 5)
end
subroutine MAINtestADDMOCKTABLESANDALIASES_inline_select(null)
    record mockTableRec (
        1 tables[*]
            2 name = vc
            2 aliases[*]
                3 alias = vc
    ) with protect
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_DELETE.>', char(10),
    '  <COMMA.>', char(10),
    '   <TABLE.>', char(10),
    '    <Z_SELECT.>', char(10),
    '     <COMMA. />', char(10),
    '     <COMMA. />', char(10),
    '     <COMMA.>', char(10),
    '      <TABLE.>', char(10),
    '       <NAME text="TESTTABLE1" />', char(10),
    '       <NAME text="TT1" />', char(10),
    '      </TABLE.>', char(10),
    '     </COMMA.>', char(10),
    '    </Z_SELECT.>', char(10),
    '    <NAME text="D1" />', char(10),
    '   </TABLE.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="CLINICAL_EVENT" />', char(10),
    '    <NAME text="CE1" />', char(10),
    '   </TABLE.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="CLINICAL_EVENT" />', char(10),
    '    <NAME text="CE2" />', char(10),
    '   </TABLE.>', char(10),
    '  </COMMA.>', char(10),
    ' </Z_DELETE.>', char(10)), hXmlFile)
    call ADDMOCKTABLESANDALIASES(cclut::getXmlListItemHandle(hXmlTest, "Z_DELETE.", 1), FALSE)
    set testADDMOCKTABLESANDALIASES_inline_select__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 008", size(mockTableRec->tables, 5), 1)
    call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 009", mockTableRec->tables[1].name,
        "CLINICAL_EVENT")
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 010",
        size(mockTableRec->tables[1].aliases, 5), 2)
    call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 011",
        mockTableRec->tables[1].aliases[1].alias, "CE1")
    call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 012",
        mockTableRec->tables[1].aliases[2].alias, "CE2")
end
subroutine (3_ADDMOCKTABLESANDALIASES::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    case (getNamedElementNameCount)
        of 1:
        of 3:
        of 5:
            call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 001", index, 1)
        of 2:
        of 4:
        of 6:
            call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 002", index, 2)
    endcase
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (3_ADDMOCKTABLESANDALIASES::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 003", parameter, "D1")
            return (FALSE)
        of 2:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 004", parameter,
                "CLINICAL_EVENT")
            return (FALSE)
        of 3:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 005", parameter, "CE1")
            return (FALSE)
        of 4:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 006", parameter,
                "CLINICAL_EVENT")
            return (FALSE)
        of 5:
            call cclutAssertvcEqual(CURREF, "testADDMOCKTABLESANDALIASES_inline_select 007", parameter, "CE2")
            return (FALSE)
    endcase
end

/* testADDMOCKTABLESANDALIASES_merge_update *****************************************************************
*  Scenario: Validates that non-select tables within a merge command are ignored from being mocked.         *
************************************************************************************************************/
subroutine testADDMOCKTABLESANDALIASES_merge_update(null)
    declare testADDMOCKTABLESANDALIASES_merge_update__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestADDMOCKTABLESANDALIASES_merge_update")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "4_ADDMOCKTABLESANDALIASES")

    call cclutAsserti2Equal(CURREF, "testADDMOCKTABLESANDALIASES_merge_update 002",
        testADDMOCKTABLESANDALIASES_merge_update__MainWasCalled, TRUE)
end
subroutine MAINtestADDMOCKTABLESANDALIASES_merge_update(null)
    record mockTableRec (
        1 tables[*]
            2 name = vc
            2 aliases[*]
                3 alias = vc
    ) with protect
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_UPDATE.>', char(10),
    '  <COMMA. />', char(10),
    '  <COMMA. />', char(10),
    '  <COMMA.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="TESTTABLE1" />', char(10),
    '    <NAME text="TT1" />', char(10),
    '   </TABLE.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="TESTTABLE2" />', char(10),
    '    <NAME text="TT2" />', char(10),
    '   </TABLE.>', char(10),
    '   <TABLE.>', char(10),
    '    <NAME text="TESTTABLE3" />', char(10),
    '   </TABLE.>', char(10),
    '  </COMMA.>', char(10),
    ' </Z_UPDATE.>', char(10)), hXmlFile)
    call ADDMOCKTABLESANDALIASES(cclut::getXmlListItemHandle(hXmlTest, "Z_UPDATE.", 1), TRUE)
    set testADDMOCKTABLESANDALIASES_merge_update__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testADDMOCKTABLESANDALIASES_merge_update 001", size(mockTableRec->tables, 5), 0)
end
subroutine (4_ADDMOCKTABLESANDALIASES::GETNAMEDELEMENTNAME(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (4_ADDMOCKTABLESANDALIASES::CCLUTISEMPTY(null) = null)
    call cclexception(100, "E", "Should not have been called")
end

;**********************************************************************************************************************************
;** handleTableNodeMocking
;**********************************************************************************************************************************
/* testHANDLETABLENODEMOCKING *******************************************************************************
*  Scenario: Validates that table node mocking is handled correctly for top-level queries.                  *
************************************************************************************************************/
subroutine testHANDLETABLENODEMOCKING(null)
    declare testHANDLETABLENODEMOCKING__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare addMockTablesAndAliasesCalled = i2 with protect, noconstant(FALSE)
    declare searchForMockableItemsCalled = i2 with protect, noconstant(FALSE)
    declare mergeTableRecsCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLETABLENODEMOCKING")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_HANDLETABLENODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLETABLENODEMOCKING 005", testHANDLETABLENODEMOCKING__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLETABLENODEMOCKING 006", addMockTablesAndAliasesCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLETABLENODEMOCKING 007", searchForMockableItemsCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLETABLENODEMOCKING 008", mergeTableRecsCalled, TRUE)
end
subroutine MAINtestHANDLETABLENODEMOCKING(null)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_SELECT.>', char(10),
    '    <NAME text="TESTTABLE1" />', char(10),
    ' </Z_SELECT.>', char(10)), hXmlFile)
    declare mainHZCrud = h with protect, noconstant(cclut::getXmlListItemHandle(hXmlTest, "Z_SELECT.", 1))
    call HANDLETABLENODEMOCKING(mainHZCrud, 1, TRUE)
    set testHANDLETABLENODEMOCKING__MainWasCalled = TRUE
end
subroutine (1_HANDLETABLENODEMOCKING::ADDMOCKTABLESANDALIASES(hZCrud = h, isParentMerge = i2) = null)
    set addMockTablesAndAliasesCalled = TRUE
    if (mainHZCrud != hZCrud)
        call cclexception(100, "E", "XML node passed to addMockTablesAndAliases does not match XML node from MAIN.")
    endif
    call cclutAsserti2Equal(CURREF, "testHANDLETABLENODEMOCKING 001", isParentMerge, TRUE)
    call cclutAsserti4Equal(CURREF, "testHANDLETABLENODEMOCKING 002", validate(mockTableRec), 1)
end
subroutine (1_HANDLETABLENODEMOCKING::SEARCHFORMOCKABLEITEMS(hZCrud = h, subroutineIndex = i4) = null)
    set searchForMockableItemsCalled = TRUE
    if (mainHZCrud != hZCrud)
        call cclexception(100, "E", "XML node passed to searchForMockableItems does not match XML node from MAIN.")
    endif
    call cclutAsserti4Equal(CURREF, "testHANDLETABLENODEMOCKING 003", subroutineIndex, 1)
end
subroutine (1_HANDLETABLENODEMOCKING::MERGETABLERECS(subroutineIndex = i4) = null)
    set mergeTableRecsCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLETABLENODEMOCKING 004", subroutineIndex, 1)
end

/* testHANDLETABLENODEMOCKING_record_bubble *****************************************************************
*  Scenario: Validates that when the mockTableRec is already defined, mergeTableRecs is not called.         *
************************************************************************************************************/
subroutine testHANDLETABLENODEMOCKING_record_bubble(null)
    declare testHANDLETABLENODEMOCKING_record_bubble__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare addMockTablesAndAliasesCalled = i2 with protect, noconstant(FALSE)
    declare searchForMockableItemsCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLETABLENODEMOCKING_record_bubble")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_HANDLETABLENODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLETABLENODEMOCKING_record_bubble 003",
        testHANDLETABLENODEMOCKING_record_bubble__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLETABLENODEMOCKING_record_bubble 004", addMockTablesAndAliasesCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testHANDLETABLENODEMOCKING_record_bubble 005", searchForMockableItemsCalled, TRUE)
end
subroutine MAINtestHANDLETABLENODEMOCKING_record_bubble(null)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_SELECT.>', char(10),
    '    <NAME text="TESTTABLE1" />', char(10),
    ' </Z_SELECT.>', char(10)), hXmlFile)
    declare mainHZCrud = h with protect, noconstant(cclut::getXmlListItemHandle(hXmlTest, "Z_SELECT.", 1))
    record mockTableRec (
        1 test = vc
    ) with protect
    call HANDLETABLENODEMOCKING(mainHZCrud, 2, FALSE)
    set testHANDLETABLENODEMOCKING_record_bubble__MainWasCalled = TRUE
end
subroutine (2_HANDLETABLENODEMOCKING::ADDMOCKTABLESANDALIASES(hZCrud = h, isParentMerge = i2) = null)
    set addMockTablesAndAliasesCalled = TRUE
    if (mainHZCrud != hZCrud)
        call cclexception(100, "E", "XML node passed to addMockTablesAndAliases does not match XML node from MAIN.")
    endif
    call cclutAsserti2Equal(CURREF, "testHANDLETABLENODEMOCKING 001", isParentMerge, FALSE)
end
subroutine (2_HANDLETABLENODEMOCKING::SEARCHFORMOCKABLEITEMS(hZCrud = h, subroutineIndex = i4) = null)
    set searchForMockableItemsCalled = TRUE
    if (mainHZCrud != hZCrud)
        call cclexception(100, "E", "XML node passed to searchForMockableItems does not match XML node from MAIN.")
    endif
    call cclutAsserti4Equal(CURREF, "testHANDLETABLENODEMOCKING_record_bubble 002", subroutineIndex, 2)
end
subroutine (2_HANDLETABLENODEMOCKING::MERGETABLERECS(null) = null)
    call cclexception(100, "E", "Should not have been called")
end

;**********************************************************************************************************************************
;** addMockColumn
;**********************************************************************************************************************************
/* testADDMOCKCOLUMN ****************************************************************************************
*  Scenario: Validates that columns are added to the list of mocks.                                         *
************************************************************************************************************/
subroutine testADDMOCKCOLUMN(null)
    declare testADDMOCKCOLUMN__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestADDMOCKCOLUMN")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_ADDMOCKCOLUMN")

    call cclutAsserti2Equal(CURREF, "testADDMOCKCOLUMN 009", testADDMOCKCOLUMN__MainWasCalled, TRUE)
end
subroutine MAINtestADDMOCKCOLUMN(null)
    record mockTableRec (
        1 tables[*]
            2 columns[*]
                3 name = vc
    ) with protect
    set stat = alterlist(mockTableRec->tables, 3)
    set stat = alterlist(mockTableRec->tables[2].columns, 3)
    set mockTableRec->tables[2].columns[1].name = "test column 1"
    set mockTableRec->tables[2].columns[2].name = "test column 2"
    set mockTableRec->tables[2].columns[3].name = "test column 3"
    call ADDMOCKCOLUMN(2, "test column 4")
    set testADDMOCKCOLUMN__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testADDMOCKCOLUMN 001", size(mockTableRec->tables, 5), 3)
    call cclutAsserti4Equal(CURREF, "testADDMOCKCOLUMN 002", size(mockTableRec->tables[1].columns, 5), 0)
    call cclutAsserti4Equal(CURREF, "testADDMOCKCOLUMN 003", size(mockTableRec->tables[2].columns, 5), 4)
    call cclutAssertvcEqual(CURREF, "testADDMOCKCOLUMN 004", mockTableRec->tables[2].columns[1].name, "test column 1")
    call cclutAssertvcEqual(CURREF, "testADDMOCKCOLUMN 005", mockTableRec->tables[2].columns[2].name, "test column 2")
    call cclutAssertvcEqual(CURREF, "testADDMOCKCOLUMN 006", mockTableRec->tables[2].columns[3].name, "test column 3")
    call cclutAssertvcEqual(CURREF, "testADDMOCKCOLUMN 007", mockTableRec->tables[2].columns[4].name, "test column 4")
    call cclutAsserti4Equal(CURREF, "testADDMOCKCOLUMN 008", size(mockTableRec->tables[3].columns, 5), 0)
end

/* testADDMOCKCOLUMN_already_mocked *************************************************************************
*  Scenario: Validates that columns are not added to the list of mocks if they are already mocked.          *
************************************************************************************************************/
subroutine testADDMOCKCOLUMN_already_mocked(null)
    declare testADDMOCKCOLUMN_already_mocked__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestADDMOCKCOLUMN_already_mocked")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_ADDMOCKCOLUMN")

    call cclutAsserti2Equal(CURREF, "testADDMOCKCOLUMN_already_mocked 008",
        testADDMOCKCOLUMN_already_mocked__MainWasCalled, TRUE)
end
subroutine MAINtestADDMOCKCOLUMN_already_mocked(null)
    record mockTableRec (
        1 tables[*]
            2 columns[*]
                3 name = vc
    ) with protect
    set stat = alterlist(mockTableRec->tables, 3)
    set stat = alterlist(mockTableRec->tables[2].columns, 3)
    set mockTableRec->tables[2].columns[1].name = "test column 1"
    set mockTableRec->tables[2].columns[2].name = "test column 2"
    set mockTableRec->tables[2].columns[3].name = "test column 3"
    call ADDMOCKCOLUMN(2, "test column 3")
    set testADDMOCKCOLUMN_already_mocked__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testADDMOCKCOLUMN_already_mocked 001", size(mockTableRec->tables, 5), 3)
    call cclutAsserti4Equal(CURREF, "testADDMOCKCOLUMN_already_mocked 002", size(mockTableRec->tables[1].columns, 5), 0)
    call cclutAsserti4Equal(CURREF, "testADDMOCKCOLUMN_already_mocked 003", size(mockTableRec->tables[2].columns, 5), 3)
    call cclutAssertvcEqual(CURREF, "testADDMOCKCOLUMN_already_mocked 004", mockTableRec->tables[2].columns[1].name,
        "test column 1")
    call cclutAssertvcEqual(CURREF, "testADDMOCKCOLUMN_already_mocked 005", mockTableRec->tables[2].columns[2].name,
        "test column 2")
    call cclutAssertvcEqual(CURREF, "testADDMOCKCOLUMN_already_mocked 006", mockTableRec->tables[2].columns[3].name,
        "test column 3")
    call cclutAsserti4Equal(CURREF, "testADDMOCKCOLUMN_already_mocked 007", size(mockTableRec->tables[3].columns, 5), 0)
end

;**********************************************************************************************************************************
;** handleColumnNodeMocking
;**********************************************************************************************************************************
/* testHANDLECOLUMNNODEMOCKING ******************************************************************************
*  Scenario: Validates that columns are mocked when they are referenced using the table name.               *
************************************************************************************************************/
subroutine testHANDLECOLUMNNODEMOCKING(null)
    declare testHANDLECOLUMNNODEMOCKING__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)
    declare addMockColumnCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLECOLUMNNODEMOCKING")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_HANDLECOLUMNNODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLECOLUMNNODEMOCKING 007", testHANDLECOLUMNNODEMOCKING__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING 008", getNamedElementNameCount, 2)
    call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING 009", cclutIsEmptyCount, 2)
    call cclutAsserti2Equal(CURREF, "testHANDLECOLUMNNODEMOCKING 010", addMockColumnCalled, TRUE)
end
subroutine MAINtestHANDLECOLUMNNODEMOCKING(null)
    record mockTableRec (
        1 tables[*]
            2 name = vc
    ) with protect
    set stat = alterlist(mockTableRec->tables, 3)
    set mockTableRec->tables[1].name = "TESTTABLE1"
    set mockTableRec->tables[2].name = "TESTTABLE2"
    set mockTableRec->tables[3].name = "TESTTABLE3"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <ATTR.>', char(10),
    '  <NAME text="TESTTABLE2" />', char(10),
    '  <NAME text="TESTCOLUMN1" />', char(10),
    ' </ATTR.>', char(10)), hXmlFile)
    call HANDLECOLUMNNODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "ATTR.", 1))
    set testHANDLECOLUMNNODEMOCKING__MainWasCalled = TRUE
end
subroutine (1_HANDLECOLUMNNODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    case (getNamedElementNameCount)
        of 1:
            call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING 001", index, 1)
        of 2:
            call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING 002", index, 2)
    endcase
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (1_HANDLECOLUMNNODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testHANDLECOLUMNNODEMOCKING 003", parameter, "TESTTABLE2")
        of 2:
            call cclutAssertvcEqual(CURREF, "testHANDLECOLUMNNODEMOCKING 004", parameter, "TESTCOLUMN1")
    endcase
    return (FALSE)
end
subroutine (1_HANDLECOLUMNNODEMOCKING::ADDMOCKCOLUMN(tableIndex = i4, columnText = vc) = null)
    set addMockColumnCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING 005", tableIndex, 2)
    call cclutAssertvcEqual(CURREF, "testHANDLECOLUMNNODEMOCKING 006", columnText, "TESTCOLUMN1")
end

/* testHANDLECOLUMNNODEMOCKING_aliased_column ***************************************************************
*  Scenario: Validates that columns are mocked when they are referenced using a table alias.                *
************************************************************************************************************/
subroutine testHANDLECOLUMNNODEMOCKING_aliased_column(null)
    declare testHANDLECOLUMNNODEMOCKING_aliased_column__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)
    declare addMockColumnCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLECOLUMNNODEMOCKING_aliased_column")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_HANDLECOLUMNNODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 007",
        testHANDLECOLUMNNODEMOCKING_aliased_column__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 008", getNamedElementNameCount, 2)
    call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 009", cclutIsEmptyCount, 2)
    call cclutAsserti2Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 010", addMockColumnCalled, TRUE)
end
subroutine MAINtestHANDLECOLUMNNODEMOCKING_aliased_column(null)
    record mockTableRec (
        1 tables[*]
            2 name = vc
            2 aliases[*]
                3 alias = vc
    ) with protect
    set stat = alterlist(mockTableRec->tables, 3)
    set mockTableRec->tables[1].name = "TESTTABLE1"
    set mockTableRec->tables[2].name = "TESTTABLE2"
    set mockTableRec->tables[3].name = "TESTTABLE3"
    set stat = alterlist(mockTableRec->tables[3].aliases, 3)
    set mockTableRec->tables[3].aliases[1].alias = "T"
    set mockTableRec->tables[3].aliases[2].alias = "TT3"
    set mockTableRec->tables[3].aliases[3].alias = "THREE"
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <ATTR.>', char(10),
    '  <NAME text="TT3" />', char(10),
    '  <NAME text="TESTCOLUMN1" />', char(10),
    ' </ATTR.>', char(10)), hXmlFile)
    call HANDLECOLUMNNODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "ATTR.", 1))
    set testHANDLECOLUMNNODEMOCKING_aliased_column__MainWasCalled = TRUE
end
subroutine (2_HANDLECOLUMNNODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    case (getNamedElementNameCount)
        of 1:
            call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 001", index, 1)
        of 2:
            call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 002", index, 2)
    endcase
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (2_HANDLECOLUMNNODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 003", parameter, "TT3")
        of 2:
            call cclutAssertvcEqual(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 004", parameter, "TESTCOLUMN1")
    endcase
    return (FALSE)
end
subroutine (2_HANDLECOLUMNNODEMOCKING::ADDMOCKCOLUMN(tableIndex = i4, columnText = vc) = null)
    set addMockColumnCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 005", tableIndex, 3)
    call cclutAssertvcEqual(CURREF, "testHANDLECOLUMNNODEMOCKING_aliased_column 006", columnText, "TESTCOLUMN1")
end

/* testHANDLECOLUMNNODEMOCKING_no_table *********************************************************************
*  Scenario: Validates that columns are not mocked when there is no table name or alias name.               *
************************************************************************************************************/
subroutine testHANDLECOLUMNNODEMOCKING_no_table(null)
    declare testHANDLECOLUMNNODEMOCKING_no_table__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)
    declare cclutIsEmptyCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLECOLUMNNODEMOCKING_no_table")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "3_HANDLECOLUMNNODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_table 004",
        testHANDLECOLUMNNODEMOCKING_no_table__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_table 005", getNamedElementNameCount, 2)
    call cclutAsserti2Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_table 006", cclutIsEmptyCalled, TRUE)
end
subroutine MAINtestHANDLECOLUMNNODEMOCKING_no_table(null)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <ATTR.>', char(10),
    '  <NAME text="" />', char(10),
    '  <NAME text="TESTCOLUMN1" />', char(10),
    ' </ATTR.>', char(10)), hXmlFile)
    call HANDLECOLUMNNODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "ATTR.", 1))
    set testHANDLECOLUMNNODEMOCKING_no_table__MainWasCalled = TRUE
end
subroutine (3_HANDLECOLUMNNODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    case (getNamedElementNameCount)
        of 1:
            call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_table 001", index, 1)
        of 2:
            call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_table 002", index, 2)
    endcase
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (3_HANDLECOLUMNNODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testHANDLECOLUMNNODEMOCKING_no_table 003", parameter, trim(""))
    return (TRUE)
end
subroutine (3_HANDLECOLUMNNODEMOCKING::ADDMOCKCOLUMN(null) = null)
    call cclexception(100, "E", "Should not have been called")
end

/* testHANDLECOLUMNNODEMOCKING_no_column ********************************************************************
*  Scenario: Validates that columns are not mocked when there is a table name but no column name.           *
************************************************************************************************************/
subroutine testHANDLECOLUMNNODEMOCKING_no_column(null)
    declare testHANDLECOLUMNNODEMOCKING_no_column__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getNamedElementNameCount = i4 with protect, noconstant(0)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestHANDLECOLUMNNODEMOCKING_no_column")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "4_HANDLECOLUMNNODEMOCKING")

    call cclutAsserti2Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_column 005",
        testHANDLECOLUMNNODEMOCKING_no_column__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_column 006", getNamedElementNameCount, 2)
    call cclutAsserti2Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_column 007", cclutIsEmptyCount, 2)
end
subroutine MAINtestHANDLECOLUMNNODEMOCKING_no_column(null)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <ATTR.>', char(10),
    '  <NAME text="TESTTABLE1" />', char(10),
    '  <NAME text="" />', char(10),
    ' </ATTR.>', char(10)), hXmlFile)
    call HANDLECOLUMNNODEMOCKING(cclut::getXmlListItemHandle(hXmlTest, "ATTR.", 1))
    set testHANDLECOLUMNNODEMOCKING_no_column__MainWasCalled = TRUE
end
subroutine (4_HANDLECOLUMNNODEMOCKING::GETNAMEDELEMENTNAME(element = h, index = i4) = vc)
    set getNamedElementNameCount = getNamedElementNameCount + 1
    case (getNamedElementNameCount)
        of 1:
            call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_column 001", index, 1)
        of 2:
            call cclutAsserti4Equal(CURREF, "testHANDLECOLUMNNODEMOCKING_no_column 002", index, 2)
    endcase
    return (PUBLIC::GETNAMEDELEMENTNAME(element, index))
end
subroutine (4_HANDLECOLUMNNODEMOCKING::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testHANDLECOLUMNNODEMOCKING_no_column 003", parameter, "TESTTABLE1")
            return (FALSE)
        of 2:
            call cclutAssertvcEqual(CURREF, "testHANDLECOLUMNNODEMOCKING_no_column 004", parameter, trim(""))
            return (TRUE)
    endcase
end
subroutine (4_HANDLECOLUMNNODEMOCKING::ADDMOCKCOLUMN(null) = null)
    call cclexception(100, "E", "Should not have been called")
end

;**********************************************************************************************************************************
;** searchForMockableItems
;**********************************************************************************************************************************
/* testSEARCHFORMOCKABLEITEMS *******************************************************************************
*  Scenario: Validates that subroutines, scripts and columns are handled correctly.                         *
************************************************************************************************************/
subroutine testSEARCHFORMOCKABLEITEMS(null)
    declare testSEARCHFORMOCKABLEITEMS__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare handleSubroutineNodeMockingCalled = i2 with protect, noconstant(FALSE)
    declare handleScriptNodeMockingCalled = i2 with protect, noconstant(FALSE)
    declare handleColumnNodeMockingCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestSEARCHFORMOCKABLEITEMS")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_SEARCHFORMOCKABLEITEMS")

    call cclutAsserti2Equal(CURREF, "testSEARCHFORMOCKABLEITEMS 003", testSEARCHFORMOCKABLEITEMS__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testSEARCHFORMOCKABLEITEMS 004", handleSubroutineNodeMockingCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testSEARCHFORMOCKABLEITEMS 005", handleScriptNodeMockingCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testSEARCHFORMOCKABLEITEMS 006", handleColumnNodeMockingCalled, TRUE)
end
subroutine MAINtestSEARCHFORMOCKABLEITEMS(null)
    record mockTableRec (
        1 test = vc
    ) with protect
    declare hProgram = h with protect, noconstant(0)
    declare hCall = h with protect, noconstant(0)
    declare hExecute = h with protect, noconstant(0)
    declare hAttr = h with protect, noconstant(0)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM.>', char(10),
    '  <CALL. />', char(10),
    '  <Z_EXECUTE. />', char(10),
    '  <ATTR. />', char(10),
    ' </ZC_PROGRAM.>', char(10)), hXmlFile)
    set hProgram = cclut::getXmlListItemHandle(hXmlTest, "ZC_PROGRAM.", 1)
    set hCall = cclut::getXmlListItemHandle(hProgram, "CALL.", 1)
    set hExecute = cclut::getXmlListItemHandle(hProgram, "Z_EXECUTE.", 1)
    set hAttr = cclut::getXmlListItemHandle(hProgram, "ATTR.", 1)
    call SEARCHFORMOCKABLEITEMS(hProgram, 1)
    set testSEARCHFORMOCKABLEITEMS__MainWasCalled = TRUE
end
subroutine (1_SEARCHFORMOCKABLEITEMS::HANDLETABLENODEMOCKING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (1_SEARCHFORMOCKABLEITEMS::HANDLESUBROUTINENODEMOCKING(callNode = h, subroutineIndex = i4) = null)
    set handleSubroutineNodeMockingCalled = TRUE
    if (callNode != hCall)
        call cclexception(100, "E", "XML node passed to handleSubroutineNodeMocking does not match XML node from MAIN.")
    endif
    call cclutAsserti4Equal(CURREF, "testSEARCHFORMOCKABLEITEMS 001", subroutineIndex, 1)
end
subroutine (1_SEARCHFORMOCKABLEITEMS::HANDLESCRIPTNODEMOCKING(executeNode = h, subroutineIndex = i4) = null)
    set handleScriptNodeMockingCalled = TRUE
    if (executeNode != hExecute)
        call cclexception(100, "E", "XML node passed to handleScriptNodeMocking does not match XML node from MAIN.")
    endif
    call cclutAsserti4Equal(CURREF, "testSEARCHFORMOCKABLEITEMS 002", subroutineIndex, 1)
end
subroutine (1_SEARCHFORMOCKABLEITEMS::HANDLECOLUMNNODEMOCKING(attrNode = h) = null)
    set handleColumnNodeMockingCalled = TRUE
    if (attrNode != hAttr)
        call cclexception(100, "E", "XML node passed to handleColumnNodeMocking does not match XML node from MAIN.")
    endif
end

/* testSEARCHFORMOCKABLEITEMS_table_node ********************************************************************
*  Scenario: Validates that tables are handled correctly when searching for mockable items.                 *
************************************************************************************************************/
subroutine testSEARCHFORMOCKABLEITEMS_table_node(null)
    declare testSEARCHFORMOCKABLEITEMS_table_node__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare handleTableNodeMockingCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestSEARCHFORMOCKABLEITEMS_table_node")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_SEARCHFORMOCKABLEITEMS")

    call cclutAsserti2Equal(CURREF, "testSEARCHFORMOCKABLEITEMS_table_node 003",
        testSEARCHFORMOCKABLEITEMS_table_node__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testSEARCHFORMOCKABLEITEMS_table_node 004", handleTableNodeMockingCount, 2)
end
subroutine MAINtestSEARCHFORMOCKABLEITEMS_table_node(null)
    declare hProgram = h with protect, noconstant(0)
    declare hSelect = h with protect, noconstant(0)
    declare hInsert = h with protect, noconstant(0)
    declare hAttr = h with protect, noconstant(0)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM.>', char(10),
    '  <Z_SELECT. />', char(10),
    '  <Z_INSERT. />', char(10),
    '  <ATTR. />', char(10),
    ' </ZC_PROGRAM.>', char(10)), hXmlFile)
    set hProgram = cclut::getXmlListItemHandle(hXmlTest, "ZC_PROGRAM.", 1)
    set hSelect = cclut::getXmlListItemHandle(hProgram, "Z_SELECT.", 1)
    set hInsert = cclut::getXmlListItemHandle(hProgram, "Z_INSERT.", 1)
    set hAttr = cclut::getXmlListItemHandle(hProgram, "ATTR.", 1)
    call SEARCHFORMOCKABLEITEMS(hProgram, 2)
    set testSEARCHFORMOCKABLEITEMS_table_node__MainWasCalled = TRUE
end
subroutine (2_SEARCHFORMOCKABLEITEMS::HANDLETABLENODEMOCKING(crudNode = h, subroutineIndex = i4, isMerge = i2) = null)
    set handleTableNodeMockingCount = handleTableNodeMockingCount + 1
    case (handleTableNodeMockingCount)
        of 1:
            if (crudNode != hSelect)
                call cclexception(100, "E",
                    "XML node passed to handleTableNodeMocking does not match XML node from MAIN.")
            endif
        of 2:
            if (crudNode != hInsert)
                call cclexception(100, "E",
                    "XML node passed to handleTableNodeMocking does not match XML node from MAIN.")
            endif
    endcase
    call cclutAsserti4Equal(CURREF, "testSEARCHFORMOCKABLEITEMS_table_node 001", subroutineIndex, 2)
    call cclutAsserti2Equal(CURREF, "testSEARCHFORMOCKABLEITEMS_table_node 002", isMerge, FALSE)
end
subroutine (2_SEARCHFORMOCKABLEITEMS::HANDLESUBROUTINENODEMOCKING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (2_SEARCHFORMOCKABLEITEMS::HANDLESCRIPTNODEMOCKING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (2_SEARCHFORMOCKABLEITEMS::HANDLECOLUMNNODEMOCKING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end

/* testSEARCHFORMOCKABLEITEMS_merge_node ********************************************************************
*  Scenario: Validates that tables are handled correctly when searching for mockable items in a merge node. *
************************************************************************************************************/
subroutine testSEARCHFORMOCKABLEITEMS_merge_node(null)
    declare testSEARCHFORMOCKABLEITEMS_merge_node__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare handleTableNodeMockingCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestSEARCHFORMOCKABLEITEMS_merge_node")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "3_SEARCHFORMOCKABLEITEMS")

    call cclutAsserti2Equal(CURREF, "testSEARCHFORMOCKABLEITEMS_merge_node 003",
        testSEARCHFORMOCKABLEITEMS_merge_node__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testSEARCHFORMOCKABLEITEMS_merge_node 004", handleTableNodeMockingCount, 3)
end
subroutine MAINtestSEARCHFORMOCKABLEITEMS_merge_node(null)
    declare hParentMerge = h with protect, noconstant(0)
    declare hUpdate = h with protect, noconstant(0)
    declare hDelete = h with protect, noconstant(0)
    declare hMerge = h with protect, noconstant(0)
    declare hXmlTest = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    set hXmlTest = cclut::parseXmlBuffer(concat(
    '<?xml version="1.0"?>', char(10),
    ' <Z_MERGE.>', char(10),
    '  <Z_UPDATE. />', char(10),
    '  <Z_DELETE. />', char(10),
    '  <Z_MERGE. />', char(10),
    ' </Z_MERGE.>', char(10)), hXmlFile)
    set hParentMerge = cclut::getXmlListItemHandle(hXmlTest, "Z_MERGE.", 1)
    set hUpdate = cclut::getXmlListItemHandle(hParentMerge, "Z_UPDATE.", 1)
    set hDelete = cclut::getXmlListItemHandle(hParentMerge, "Z_DELETE.", 1)
    set hMerge = cclut::getXmlListItemHandle(hParentMerge, "Z_MERGE.", 1)
    call SEARCHFORMOCKABLEITEMS(hParentMerge, 3)
    set testSEARCHFORMOCKABLEITEMS_merge_node__MainWasCalled = TRUE
end
subroutine (3_SEARCHFORMOCKABLEITEMS::HANDLETABLENODEMOCKING(crudNode = h, subroutineIndex = i4, isMerge = i2) = null)
    set handleTableNodeMockingCount = handleTableNodeMockingCount + 1
    case (handleTableNodeMockingCount)
        of 1:
            if (crudNode != hUpdate)
                call cclexception(100, "E",
                    "XML node passed to handleTableNodeMocking does not match XML node from MAIN.")
            endif
        of 2:
            if (crudNode != hDelete)
                call cclexception(100, "E",
                    "XML node passed to handleTableNodeMocking does not match XML node from MAIN.")
            endif
        of 3:
            if (crudNode != hMerge)
                call cclexception(100, "E",
                    "XML node passed to handleTableNodeMocking does not match XML node from MAIN.")
            endif
    endcase
    call cclutAsserti4Equal(CURREF, "testSEARCHFORMOCKABLEITEMS_merge_node 001", subroutineIndex, 3)
    call cclutAsserti2Equal(CURREF, "testSEARCHFORMOCKABLEITEMS_merge_node 002", isMerge, TRUE)
end
subroutine (3_SEARCHFORMOCKABLEITEMS::HANDLESUBROUTINENODEMOCKING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (3_SEARCHFORMOCKABLEITEMS::HANDLESCRIPTNODEMOCKING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (3_SEARCHFORMOCKABLEITEMS::HANDLECOLUMNNODEMOCKING(null) = null)
    call cclexception(100, "E", "Should not have been called")
end

;**********************************************************************************************************************************
;** createMockList
;**********************************************************************************************************************************
/* testCREATEMOCKLIST ***************************************************************************************
*  Scenario: Validates that mockable items are searched for each public subroutine.                         *
************************************************************************************************************/
subroutine testCREATEMOCKLIST(null)
    declare testCREATEMOCKLIST__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare getPublicNamedElementNameCount = i4 with protect, noconstant(0)
    declare cclutIsEmptyCount = i4 with protect, noconstant(0)
    declare searchForMockableItemsCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "CREATEMOCKLIST_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestCREATEMOCKLIST")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_CREATEMOCKLIST")

    call cclutAsserti2Equal(CURREF, "testCREATEMOCKLIST 006", testCREATEMOCKLIST__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testCREATEMOCKLIST 007", getPublicNamedElementNameCount, 3)
    call cclutAsserti4Equal(CURREF, "testCREATEMOCKLIST 008", cclutIsEmptyCount, 3)
    call cclutAsserti2Equal(CURREF, "testCREATEMOCKLIST 009", searchForMockableItemsCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEMOCKLIST 010", echoRecordCalled, TRUE)
end
subroutine MAINtestCREATEMOCKLIST(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE4"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE5"
    call CREATEMOCKLIST(concat(
    '<?xml version="1.0"?>', char(10),
    ' <ZC_PROGRAM.>', char(10),
    '  <SUBROUTINE.>', char(10),
    '   <NAMESPACE.>', char(10),
    '    <NAME text="CCLUT" />', char(10),
    '    <NAME text="TESTSUBROUTINE1" />', char(10),
    '   </NAMESPACE.>', char(10),
    '  </SUBROUTINE.>', char(10),
    '  <SUBROUTINE.>', char(10),
    '   <NAMESPACE.>', char(10),
    '    <NAME text="PUBLIC" />', char(10),
    '    <NAME text="TESTSUBROUTINE2" />', char(10),
    '   </NAMESPACE.>', char(10),
    '  </SUBROUTINE.>', char(10),
    '  <SUBROUTINE.>', char(10),
    '   <NAME text="TESTSUBROUTINE3" />', char(10),
    '  </SUBROUTINE.>', char(10),
    ' </ZC_PROGRAM.>', char(10)))
    set testCREATEMOCKLIST__MainWasCalled = TRUE
end
subroutine (1_CREATEMOCKLIST::GETPUBLICNAMEDELEMENTNAME(element = h) = vc)
    set getPublicNamedElementNameCount = getPublicNamedElementNameCount + 1
    return (PUBLIC::GETPUBLICNAMEDELEMENTNAME(element))
end
subroutine (1_CREATEMOCKLIST::CCLUTISEMPTY(parameter = vc) = i2)
    set cclutIsEmptyCount = cclutIsEmptyCount + 1
    case (cclutIsEmptyCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testCREATEMOCKLIST 001", parameter, "")
            return (TRUE)
        of 2:
            call cclutAssertvcEqual(CURREF, "testCREATEMOCKLIST 002", parameter, "TESTSUBROUTINE2")
            return (FALSE)
        of 3:
            call cclutAssertvcEqual(CURREF, "testCREATEMOCKLIST 004", parameter, "TESTSUBROUTINE3")
            return (FALSE)
    endcase
end
subroutine (1_CREATEMOCKLIST::SEARCHFORMOCKABLEITEMS(hSubroutine = h, subroutineIndex = i4) = null)
    set searchForMockableItemsCalled = TRUE
    if (cclut::getXmlListItemHandle(hProgram, "SUBROUTINE.", 2) != hSubroutine)
        call cclexception(100, "E", "XML node passed to searchForMockableItems does not match XML node from MAIN.")
    endif
    call cclutAsserti4Equal(CURREF, "testCREATEMOCKLIST 003", subroutineIndex, 2)
end
subroutine (CREATEMOCKLIST_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testCREATEMOCKLIST 005", templateReply->status_data.status, "")
    set echoRecordCalled = TRUE
end

/* testCREATEMOCKLIST_invalid_xml ***************************************************************************
*  Scenario: Validates that an error is thrown when invalid XML is supplied to the subroutine.              *
************************************************************************************************************/
subroutine testCREATEMOCKLIST_invalid_xml(null)
    declare testCREATEMOCKLIST_invalid_xml__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "CREATEMOCKLIST_invalid_xml_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestCREATEMOCKLIST_invalid_xml")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_CREATEMOCKLIST")

    call cclutAsserti2Equal(CURREF, "testCREATEMOCKLIST 006", testCREATEMOCKLIST_invalid_xml__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testCREATEMOCKLIST 007", echoRecordCalled, TRUE)
end
subroutine MAINtestCREATEMOCKLIST_invalid_xml(null)
    set scriptUnderTest = "test script"
    set testCREATEMOCKLIST_invalid_xml__MainWasCalled = TRUE
    call CREATEMOCKLIST("")
end
subroutine (2_CREATEMOCKLIST::GETPUBLICNAMEDELEMENTNAME(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (2_CREATEMOCKLIST::CCLUTISEMPTY(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (2_CREATEMOCKLIST::SEARCHFORMOCKABLEITEMS(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (CREATEMOCKLIST_invalid_xml_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testCREATEMOCKLIST_invalid_xml 001", templateReply->status_data.status, "F")
    call cclutAssertvcEqual(CURREF, "testCREATEMOCKLIST_invalid_xml 002",
        trim(templateReply->status_data.subeventstatus[1].OperationName), "Create Mock List")
    call cclutAssertvcEqual(CURREF, "testCREATEMOCKLIST_invalid_xml 003",
        templateReply->status_data.subeventstatus[1].OperationStatus, "F")
    call cclutAssertvcEqual(CURREF, "testCREATEMOCKLIST_invalid_xml 004",
        trim(templateReply->status_data.subeventstatus[1].TargetObjectName), "test script")
    call cclutAssertVcOperator(CURREF, "testCREATEMOCKLIST_invalid_xml 005",
        templateReply->status_data.subeventstatus[1].TargetObjectValue, "regexplike",
        concat("%CCL-E-392-CCLUT_GENERATE_TEST_CASE\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\): ",
            "createMockList failed to parse program XML.  XML: "))
    set echoRecordCalled = TRUE
end

;**********************************************************************************************************************************
;** breakLine
;**********************************************************************************************************************************
/* testBREAKLINE ********************************************************************************************
*  Scenario: Validates that if a line is shorter than the max line length in CCL, the line is not broken up.*
************************************************************************************************************/
subroutine testBREAKLINE(null)
    declare testBREAKLINE__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestBREAKLINE")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_BREAKLINE")

    call cclutAsserti2Equal(CURREF, "testBREAKLINE 003", testBREAKLINE__MainWasCalled, TRUE)
end
subroutine MAINtestBREAKLINE(null)
    record buffer (
        1 lines[*]
            2 line = vc
    ) with protect
    call BREAKLINE("test line", buffer)
    set testBREAKLINE__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testBREAKLINE 001", size(buffer->lines, 5), 1)
    call cclutAssertvcEqual(CURREF, "testBREAKLINE 002", buffer->lines[1].line, "test line")
end

/* testBREAKLINE_too_long ***********************************************************************************
*  Scenario: Validates that if a line is longer than the max line length in CCL, the line is broken up.     *
************************************************************************************************************/
subroutine testBREAKLINE_too_long(null)
    declare testBREAKLINE_too_long__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestBREAKLINE_too_long")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_BREAKLINE")

    call cclutAsserti2Equal(CURREF, "testBREAKLINE_too_long 005", testBREAKLINE_too_long__MainWasCalled, TRUE)
end
subroutine MAINtestBREAKLINE_too_long(null)
    record buffer (
        1 lines[*]
            2 line = vc
    ) with protect
    call BREAKLINE(concat("abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 ",
        "abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 ",
        "abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 ",
        "abcdefghijklmnopqrstuvwxyz123 "), buffer)
    set testBREAKLINE_too_long__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testBREAKLINE_too_long 001", size(buffer->lines, 5), 3)
    call cclutAssertvcEqual(CURREF, "testBREAKLINE_too_long 002", buffer->lines[1].line,
        concat("abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 ",
        "abcdefghijklmnopqrstuvwxyz123 abcdefghi\"))
    call cclutAssertvcEqual(CURREF, "testBREAKLINE_too_long 003", buffer->lines[2].line,
        concat("jklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqrstuvwxyz123 ",
        "abcdefghijklmnopqrstuvwxyz123 abcdefghijklmnopqr\"))
    call cclutAssertvcEqual(CURREF, "testBREAKLINE_too_long 004", buffer->lines[3].line,
        "stuvwxyz123 abcdefghijklmnopqrstuvwxyz123")
end

;**********************************************************************************************************************************
;** generateSetupTearDownSubroutines
;**********************************************************************************************************************************
/* testGENERATESETUPTEARDOWNSUBROUTINES *********************************************************************
*  Scenario: Validates that the setup and tearDown subroutines are generated correctly for the final output.*
************************************************************************************************************/
subroutine testGENERATESETUPTEARDOWNSUBROUTINES(null)
    declare testGENERATESETUPTEARDOWNSUBROUTINES__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATESETUPTEARDOWNSUBROUTINES")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GENERATESETUPTEARDOWNSUBROUTINES")

    call cclutAsserti2Equal(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 020",
        testGENERATESETUPTEARDOWNSUBROUTINES__MainWasCalled, TRUE)
end
subroutine MAINtestGENERATESETUPTEARDOWNSUBROUTINES(null)
    call GENERATESETUPTEARDOWNSUBROUTINES(null)
    set testGENERATESETUPTEARDOWNSUBROUTINES__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 001", size(templateRec->testFile, 5), 18)
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 002", templateRec->testFile[1].line,
        "subroutine (setupOnce(null) = null)")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 003", templateRec->testFile[2].line,
        "    ; Place any test case-level setup here")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 004", templateRec->testFile[3].line,
        "    null")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 005", templateRec->testFile[4].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 006", templateRec->testFile[5].line,
        "subroutine (setup(null) = null)")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 007", templateRec->testFile[6].line,
        "    ; Place any test-level setup here")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 008", templateRec->testFile[7].line,
        "    null")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 009", templateRec->testFile[8].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 010", templateRec->testFile[9].line,
        "subroutine (tearDown(null) = null)")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 011", templateRec->testFile[10].line,
        "    ; Place any test-level teardown here")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 012", templateRec->testFile[11].line,
        "    call cclutRemoveAllMocks(null)")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 013", templateRec->testFile[12].line,
        "    rollback")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 014", templateRec->testFile[13].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 015", templateRec->testFile[14].line,
        "subroutine (tearDownOnce(null) = null)")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 016", templateRec->testFile[15].line,
        "    ; Place any test case-level teardown here")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 017", templateRec->testFile[16].line,
        "    null")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 018", templateRec->testFile[17].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATESETUPTEARDOWNSUBROUTINES 019", templateRec->testFile[18].line,
        trim(""))
end

;**********************************************************************************************************************************
;** generateMockSubroutines
;**********************************************************************************************************************************
/* testGENERATEMOCKSUBROUTINES ******************************************************************************
*  Scenario: Validates that the mock subroutines are generated correctly for the supplied subroutine.       *
************************************************************************************************************/
subroutine testGENERATEMOCKSUBROUTINES(null)
    declare testGENERATEMOCKSUBROUTINES__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATEMOCKSUBROUTINES")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GENERATEMOCKSUBROUTINES")

    call cclutAsserti2Equal(CURREF, "testGENERATEMOCKSUBROUTINES 017", testGENERATEMOCKSUBROUTINES__MainWasCalled, TRUE)
end
subroutine MAINtestGENERATEMOCKSUBROUTINES(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set stat = alterlist(templateRec->targetSubroutines[2].mockSubroutines, 3)
    set templateRec->targetSubroutines[2].mockSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->targetSubroutines[2].mockSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->targetSubroutines[2].mockSubroutines[3].name = "TESTSUBROUTINE3"
    call GENERATEMOCKSUBROUTINES("testnamespace", 2)
    set testGENERATEMOCKSUBROUTINES__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testGENERATEMOCKSUBROUTINES 001", size(templateRec->testFile, 5), 15)
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 002", templateRec->testFile[1].line,
        concat("; TODO: All calls to subroutines declared in the script-under-test have been prepped for mocking ",
            "using namespaces such as below."))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 003", templateRec->testFile[2].line,
        concat("; If the inbound parameters need to be captured or the return type specified, each mock should be ",
            "updated accordingly."))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 004", templateRec->testFile[3].line,
        "subroutine (testnamespace::TESTSUBROUTINE1(null) = null)")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 005", templateRec->testFile[4].line,
        "    ; TODO: delete line or add mock subroutine implementation")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 006", templateRec->testFile[5].line,
        "    null")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 007", templateRec->testFile[6].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 008", templateRec->testFile[7].line,
        "subroutine (testnamespace::TESTSUBROUTINE2(null) = null)")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 009", templateRec->testFile[8].line,
        "    ; TODO: delete line or add mock subroutine implementation")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 010", templateRec->testFile[9].line,
        "    null")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 011", templateRec->testFile[10].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 012", templateRec->testFile[11].line,
        "subroutine (testnamespace::TESTSUBROUTINE3(null) = null)")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 013", templateRec->testFile[12].line,
        "    ; TODO: delete line or add mock subroutine implementation")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 014", templateRec->testFile[13].line,
        "    null")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 015", templateRec->testFile[14].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSUBROUTINES 016", templateRec->testFile[15].line,
        trim(""))
end

;**********************************************************************************************************************************
;** generateMockScriptCalls
;**********************************************************************************************************************************
/* testGENERATEMOCKSCRIPTCALLS ******************************************************************************
*  Scenario: Validates that an implementation line is generated correctly for each script that is mockable. *
************************************************************************************************************/
subroutine testGENERATEMOCKSCRIPTCALLS(null)
    declare testGENERATEMOCKSCRIPTCALLS__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATEMOCKSCRIPTCALLS")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GENERATEMOCKSCRIPTCALLS")

    call cclutAsserti2Equal(CURREF, "testGENERATEMOCKSCRIPTCALLS 008", testGENERATEMOCKSCRIPTCALLS__MainWasCalled, TRUE)
end
subroutine MAINtestGENERATEMOCKSCRIPTCALLS(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set stat = alterlist(templateRec->targetSubroutines[2].mockScripts, 3)
    set templateRec->targetSubroutines[2].mockScripts[1].name = "TESTSCRIPT1"
    set templateRec->targetSubroutines[2].mockScripts[2].name = "TESTSCRIPT2"
    set templateRec->targetSubroutines[2].mockScripts[3].name = "TESTSCRIPT3"
    call GENERATEMOCKSCRIPTCALLS(2)
    set testGENERATEMOCKSCRIPTCALLS__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testGENERATEMOCKSCRIPTCALLS 001", size(templateRec->testFile, 5), 6)
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSCRIPTCALLS 002", templateRec->testFile[1].line,
        concat("    ; TODO: Script calls in the script-under-test have been prepped for mocking in this file using ",
            "cclutAddMockImplementation."))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSCRIPTCALLS 003", templateRec->testFile[2].line,
        concat("    ; Each instance of cclutAddMockImplementation in this file must be updated with the mock script ",
            "name in the second parameter."))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSCRIPTCALLS 004", templateRec->testFile[3].line,
        '    call cclutAddMockImplementation("TESTSCRIPT1", "TODO: delete line or add mock script name")')
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSCRIPTCALLS 005", templateRec->testFile[4].line,
        '    call cclutAddMockImplementation("TESTSCRIPT2", "TODO: delete line or add mock script name")')
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSCRIPTCALLS 006", templateRec->testFile[5].line,
        '    call cclutAddMockImplementation("TESTSCRIPT3", "TODO: delete line or add mock script name")')
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKSCRIPTCALLS 007", templateRec->testFile[6].line,
        trim(""))
end

/* testGENERATEMOCKSCRIPTCALLS_no_scripts *******************************************************************
*  Scenario: Validates that nothing is generated if there are no scripts to be mocked for the subroutine.   *
************************************************************************************************************/
subroutine testGENERATEMOCKSCRIPTCALLS_no_scripts(null)
    declare testGENERATEMOCKSCRIPTCALLS_no_scripts__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATEMOCKSCRIPTCALLS_no_scripts")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_GENERATEMOCKSCRIPTCALLS")

    call cclutAsserti2Equal(CURREF, "testGENERATEMOCKSCRIPTCALLS_no_scripts 002",
        testGENERATEMOCKSCRIPTCALLS_no_scripts__MainWasCalled, TRUE)
end
subroutine MAINtestGENERATEMOCKSCRIPTCALLS_no_scripts(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    call GENERATEMOCKSCRIPTCALLS(3)
    set testGENERATEMOCKSCRIPTCALLS_no_scripts__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testGENERATEMOCKSCRIPTCALLS_no_scripts 001", size(templateRec->testFile, 5), 0)
end

;**********************************************************************************************************************************
;** generateMockTableCalls
;**********************************************************************************************************************************
/* testGENERATEMOCKTABLECALLS *******************************************************************************
*  Scenario: Validates that implementation lines are generated correctly for each table that is mockable.   *
************************************************************************************************************/
subroutine testGENERATEMOCKTABLECALLS(null)
    declare testGENERATEMOCKTABLECALLS__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATEMOCKTABLECALLS")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GENERATEMOCKTABLECALLS")

    call cclutAsserti2Equal(CURREF, "testGENERATEMOCKTABLECALLS 016", testGENERATEMOCKTABLECALLS__MainWasCalled, TRUE)
end
subroutine MAINtestGENERATEMOCKTABLECALLS(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set stat = alterlist(templateRec->targetSubroutines[2].mockTables, 3)
    set templateRec->targetSubroutines[2].mockTables[1].name = "TESTTABLE1"
    set stat = alterlist(templateRec->targetSubroutines[2].mockTables[1].columns, 3)
    set templateRec->targetSubroutines[2].mockTables[1].columns[1].name = "TESTCOLUMN1"
    set templateRec->targetSubroutines[2].mockTables[1].columns[2].name = "TESTCOLUMN2"
    set templateRec->targetSubroutines[2].mockTables[1].columns[3].name = "TESTCOLUMN3"
    set templateRec->targetSubroutines[2].mockTables[2].name = "TESTTABLE2"
    set stat = alterlist(templateRec->targetSubroutines[2].mockTables[2].columns, 1)
    set templateRec->targetSubroutines[2].mockTables[2].columns[1].name = "ANOTHERCOLUMN1"
    set templateRec->targetSubroutines[2].mockTables[3].name = "TESTTABLE3"
    call GENERATEMOCKTABLECALLS(2)
    set testGENERATEMOCKTABLECALLS__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testGENERATEMOCKTABLECALLS 001", size(templateRec->testFile, 5), 14)
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 002", templateRec->testFile[1].line,
        concat("    ; TODO: Table calls in the script-under-test have been prepped for mocking in this file using ",
            "cclutDefineMockTable.  Each"))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 003", templateRec->testFile[2].line,
        concat("    ; instance of cclutDefineMockTable in this file must be updated with the column types in the ",
            "second parameter."))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 004", templateRec->testFile[3].line,
        concat('    call cclutDefineMockTable("TESTTABLE1", "TESTCOLUMN1|TESTCOLUMN2|TESTCOLUMN3", "TODO: delete line ',
            'or add parameter types for mock table columns")'))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 005", templateRec->testFile[4].line,
        concat('    call cclutCreateMockTable("TESTTABLE1")'))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 006", templateRec->testFile[5].line,
        concat("    ; TODO: All mock tables have the below line to aid in adding mock data.  Each instance of ",
            "cclutAddMockData in this file must"))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 007", templateRec->testFile[6].line,
        concat("    ; be updated with the mock data in the second parameter.  The line can be copied multiple times ",
            "for multiple rows of data."))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 008", templateRec->testFile[7].line,
        concat('    call cclutAddMockData("TESTTABLE1", "TODO: delete line or add mock data")'))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 009", templateRec->testFile[8].line,
        concat('    call cclutDefineMockTable("TESTTABLE2", "ANOTHERCOLUMN1", "TODO: delete line or add parameter ',
            'types for mock table columns")'))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 010", templateRec->testFile[9].line,
        concat('    call cclutCreateMockTable("TESTTABLE2")'))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 011", templateRec->testFile[10].line,
        concat('    call cclutAddMockData("TESTTABLE2", "TODO: delete line or add mock data")'))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 012", templateRec->testFile[11].line,
        concat('    call cclutDefineMockTable("TESTTABLE3", "", "TODO: delete line or add parameter ',
            'types for mock table columns")'))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 013", templateRec->testFile[12].line,
        concat('    call cclutCreateMockTable("TESTTABLE3")'))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 014", templateRec->testFile[13].line,
        concat('    call cclutAddMockData("TESTTABLE3", "TODO: delete line or add mock data")'))
    call cclutAssertvcEqual(CURREF, "testGENERATEMOCKTABLECALLS 015", templateRec->testFile[14].line,
        trim(""))
end

/* testGENERATEMOCKTABLECALLS_no_tables *********************************************************************
*  Scenario: Validates that nothing is generated if there are no tables to be mocked for the subroutine.    *
************************************************************************************************************/
subroutine testGENERATEMOCKTABLECALLS_no_tables(null)
    declare testGENERATEMOCKTABLECALLS_no_tables__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATEMOCKTABLECALLS_no_tables")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_GENERATEMOCKTABLECALLS")

    call cclutAsserti2Equal(CURREF, "testGENERATEMOCKTABLECALLS_no_tables 002",
        testGENERATEMOCKTABLECALLS_no_tables__MainWasCalled, TRUE)
end
subroutine MAINtestGENERATEMOCKTABLECALLS_no_tables(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    call GENERATEMOCKTABLECALLS(3)
    set testGENERATEMOCKTABLECALLS_no_tables__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testGENERATEMOCKTABLECALLS_no_tables 001", size(templateRec->testFile, 5), 0)
end

;**********************************************************************************************************************************
;** generateTestCaseData
;**********************************************************************************************************************************
/* testGENERATETESTCASEDATA *********************************************************************************
*  Scenario: Validates that the test case is generated correctly for all the subroutines that can be tested.*
************************************************************************************************************/
subroutine testGENERATETESTCASEDATA(null)
    declare testGENERATETESTCASEDATA__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare generateSetupTearDownSubroutinesCalled = i2 with protect, noconstant(FALSE)
    declare generateMockScriptCallsCount = i4 with protect, noconstant(0)
    declare generateMockTableCallsCount = i4 with protect, noconstant(0)
    declare generateMockSubroutinesCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATETESTCASEDATA")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GENERATETESTCASEDATA")

    call cclutAsserti2Equal(CURREF, "testGENERATETESTCASEDATA 039", testGENERATETESTCASEDATA__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGENERATETESTCASEDATA 040", generateSetupTearDownSubroutinesCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testGENERATETESTCASEDATA 041", generateMockScriptCallsCount, 3)
    call cclutAsserti4Equal(CURREF, "testGENERATETESTCASEDATA 042", generateMockTableCallsCount, 3)
    call cclutAsserti4Equal(CURREF, "testGENERATETESTCASEDATA 043", generateMockSubroutinesCount, 3)
end
subroutine MAINtestGENERATETESTCASEDATA(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->targetSubroutines[1].namespaceForMocks = "TESTSUB"
    set templateRec->targetSubroutines[1].namespaceForMocksIdentifier = 0
    set templateRec->targetSubroutines[2].name = "MAIN"
    set templateRec->targetSubroutines[2].namespaceForMocks = "MAIN"
    set templateRec->targetSubroutines[2].namespaceForMocksIdentifier = 0
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE2"
    set templateRec->targetSubroutines[3].namespaceForMocks = "LONGSUBROUTINENAMETHATISGREATERTHAN40CHARACTERS"
    set templateRec->targetSubroutines[3].namespaceForMocksIdentifier = 5
    call GENERATETESTCASEDATA("test_script")
    set testGENERATETESTCASEDATA__MainWasCalled = TRUE
    call cclutAsserti4Equal(CURREF, "testGENERATETESTCASEDATA 007", size(templateRec->testFile, 5), 31)
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 008", templateRec->testFile[1].line,
        "subroutine testTESTSUBROUTINE1(null)")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 009", templateRec->testFile[2].line,
        "    declare testTESTSUBROUTINE1__MainWasCalled = i2 with protect, noconstant(FALSE)")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 010", templateRec->testFile[3].line,
        trim(""))
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 011", templateRec->testFile[4].line,
        '    call cclutAddMockImplementation("MAIN", "MAINtestTESTSUBROUTINE1")')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 012", templateRec->testFile[5].line,
        '    call cclutExecuteProgramWithMocks("test_script", "", "1_TESTSUB")')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 013", templateRec->testFile[6].line,
        trim(""))
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 014", templateRec->testFile[7].line,
        '    call cclutAsserti2Equal(CURREF, "testTESTSUBROUTINE1 001", testTESTSUBROUTINE1__MainWasCalled, TRUE)')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 015", templateRec->testFile[8].line,
        '    call cclutAsserti2Equal(CURREF, "testTESTSUBROUTINE1 auto-generate test.  Fail by default.", TRUE, FALSE)')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 016", templateRec->testFile[9].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 017", templateRec->testFile[10].line,
        "subroutine MAINtestTESTSUBROUTINE1(null)")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 018", templateRec->testFile[11].line,
        "    call TESTSUBROUTINE1(null)")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 019", templateRec->testFile[12].line,
        "    set testTESTSUBROUTINE1__MainWasCalled = TRUE")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 020", templateRec->testFile[13].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 021", templateRec->testFile[14].line,
        "subroutine testMAIN(null)")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 022", templateRec->testFile[15].line,
        '    call cclutExecuteProgramWithMocks("test_script", "", "1_MAIN")')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 023", templateRec->testFile[16].line,
        trim(""))
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 024", templateRec->testFile[17].line,
        '    call cclutAsserti2Equal(CURREF, "testMAIN auto-generate test.  Fail by default.", TRUE, FALSE)')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 025", templateRec->testFile[18].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 026", templateRec->testFile[19].line,
        "subroutine testTESTSUBROUTINE2(null)")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 027", templateRec->testFile[20].line,
        "    declare testTESTSUBROUTINE2__MainWasCalled = i2 with protect, noconstant(FALSE)")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 028", templateRec->testFile[21].line,
        trim(""))
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 029", templateRec->testFile[22].line,
        '    call cclutAddMockImplementation("MAIN", "MAINtestTESTSUBROUTINE2")')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 030", templateRec->testFile[23].line,
        '    call cclutExecuteProgramWithMocks("test_script", "", "1_5LONGSUBROUTINENAMETHATISGREATERTHAN40")')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 031", templateRec->testFile[24].line,
        trim(""))
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 032", templateRec->testFile[25].line,
        '    call cclutAsserti2Equal(CURREF, "testTESTSUBROUTINE2 001", testTESTSUBROUTINE2__MainWasCalled, TRUE)')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 033", templateRec->testFile[26].line,
        '    call cclutAsserti2Equal(CURREF, "testTESTSUBROUTINE2 auto-generate test.  Fail by default.", TRUE, FALSE)')
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 034", templateRec->testFile[27].line,
        "end")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 035", templateRec->testFile[28].line,
        "subroutine MAINtestTESTSUBROUTINE2(null)")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 036", templateRec->testFile[29].line,
        "    call TESTSUBROUTINE2(null)")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 037", templateRec->testFile[30].line,
        "    set testTESTSUBROUTINE2__MainWasCalled = TRUE")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 038", templateRec->testFile[31].line,
        "end")
end
subroutine (1_GENERATETESTCASEDATA::GENERATESETUPTEARDOWNSUBROUTINES(null) = null)
    set generateSetupTearDownSubroutinesCalled = TRUE
end
subroutine (1_GENERATETESTCASEDATA::GENERATEMOCKSCRIPTCALLS(subroutineIndex = i4) = null)
    set generateMockScriptCallsCount = generateMockScriptCallsCount + 1
    call cclutAsserti4Equal(CURREF, "testGENERATETESTCASEDATA 001", subroutineIndex, generateMockScriptCallsCount)
end
subroutine (1_GENERATETESTCASEDATA::GENERATEMOCKTABLECALLS(subroutineIndex = i4) = null)
    set generateMockTableCallsCount = generateMockTableCallsCount + 1
    call cclutAsserti4Equal(CURREF, "testGENERATETESTCASEDATA 002", subroutineIndex, generateMockTableCallsCount)
end
subroutine (1_GENERATETESTCASEDATA::GENERATEMOCKSUBROUTINES(testIdentifierNamespace = vc, subroutineIndex = i4) = null)
    set generateMockSubroutinesCount = generateMockSubroutinesCount + 1
    case (generateMockSubroutinesCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 003", testIdentifierNamespace, "1_TESTSUB")
        of 2:
            call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 004", testIdentifierNamespace, "1_MAIN")
        of 3:
            call cclutAssertvcEqual(CURREF, "testGENERATETESTCASEDATA 005", testIdentifierNamespace,
                "1_5LONGSUBROUTINENAMETHATISGREATERTHAN40")
    endcase
    call cclutAsserti4Equal(CURREF, "testGENERATETESTCASEDATA 006", subroutineIndex, generateMockSubroutinesCount)
end

;**********************************************************************************************************************************
;** generateFinalOutput
;**********************************************************************************************************************************
/* testGENERATEFINALOUTPUT **********************************************************************************
*  Scenario: Validates that the final output is written to a file correctly when there are no warnings.     *
************************************************************************************************************/
subroutine testGENERATEFINALOUTPUT(null)
    declare testGENERATEFINALOUTPUT__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare breakLineCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATEFINALOUTPUT")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GENERATEFINALOUTPUT")

    call cclutAsserti2Equal(CURREF, "testGENERATEFINALOUTPUT 005", testGENERATEFINALOUTPUT__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testGENERATEFINALOUTPUT 006", breakLineCount, 3)
end
subroutine MAINtestGENERATEFINALOUTPUT(null)
    set stat = alterlist(templateRec->testFile, 3)
    set templateRec->testFile[1].line = "test line 1"
    set templateRec->testFile[2].line = "test line 2"
    set templateRec->testFile[3].line = "test line 3"
    call GENERATEFINALOUTPUT("ut_cclut_generate_test_case_output.inc")
    set testGENERATEFINALOUTPUT__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT 004",
        cclut::getFileAsString("ut_cclut_generate_test_case_output.inc"),
        concat("first line", char(10), "second line", char(10), "third line", char(10), "fourth line", char(10), " ", char(10)))
end
subroutine (1_GENERATEFINALOUTPUT::BREAKLINE(line = vc, buffer = vc(ref)) = null)
    set breakLineCount = breakLineCount + 1
    case (breakLineCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT 001", line, "test line 1")
        of 2:
            call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT 002", line, "test line 2")
            set stat = alterlist(buffer->lines, 1)
            set buffer->lines[1].line = "first line"
        of 3:
            call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT 003", line, "test line 3")
            set stat = alterlist(buffer->lines, 3)
            set buffer->lines[1].line = "second line"
            set buffer->lines[2].line = "third line"
            set buffer->lines[3].line = "fourth line"
    endcase
end

/* testGENERATEFINALOUTPUT_with_warnings ********************************************************************
*  Scenario: Validates that the final output is written to a file correctly when there are warnings.        *
************************************************************************************************************/
subroutine testGENERATEFINALOUTPUT_with_warnings(null)
    declare testGENERATEFINALOUTPUT_with_warnings__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare breakLineCount = i4 with protect, noconstant(0)

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATEFINALOUTPUT_with_warnings")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_GENERATEFINALOUTPUT")

    call cclutAsserti2Equal(CURREF, "testGENERATEFINALOUTPUT_with_warnings 005",
        testGENERATEFINALOUTPUT_with_warnings__MainWasCalled, TRUE)
    call cclutAsserti4Equal(CURREF, "testGENERATEFINALOUTPUT_with_warnings 006", breakLineCount, 6)
end
subroutine MAINtestGENERATEFINALOUTPUT_with_warnings(null)
    set stat = alterlist(templateRec->testFile, 3)
    set stat = alterlist(templateRec->warnings, 3)
    set templateRec->testFile[1].line = "test line 1"
    set templateRec->testFile[2].line = "test line 2"
    set templateRec->testFile[3].line = "test line 3"
    set templateRec->warnings[1].warning = "test warning 1"
    set templateRec->warnings[2].warning = "test warning 2"
    set templateRec->warnings[3].warning = "test warning 3"
    call GENERATEFINALOUTPUT("ut_cclut_generate_test_case_output.inc")
    set testGENERATEFINALOUTPUT_with_warnings__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT_with_warnings 004",
        cclut::getFileAsString("ut_cclut_generate_test_case_output.inc"),
        concat("/* WARNINGS", char(10), "first warning", char(10), "second warning", char(10), "third warning",
            char(10), "fourth warning", char(10), "*/", char(10), "first line", char(10), "second line", char(10),
            "third line", char(10), "fourth line", char(10), " ", char(10)))
end
subroutine (2_GENERATEFINALOUTPUT::BREAKLINE(line = vc, buffer = vc(ref)) = null)
    set breakLineCount = breakLineCount + 1
    case (breakLineCount)
        of 1:
            call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT_with_warnings 001", line, "test warning 1")
        of 2:
            call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT_with_warnings 002", line, "test warning 2")
            set stat = alterlist(buffer->lines, 1)
            set buffer->lines[1].line = "first warning"
        of 3:
            call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT_with_warnings 003", line, "test warning 3")
            set stat = alterlist(buffer->lines, 3)
            set buffer->lines[1].line = "second warning"
            set buffer->lines[2].line = "third warning"
            set buffer->lines[3].line = "fourth warning"
        of 4:
            call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT_with_warnings 001", line, "test line 1")
        of 5:
            call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT_with_warnings 002", line, "test line 2")
            set stat = alterlist(buffer->lines, 1)
            set buffer->lines[1].line = "first line"
        of 6:
            call cclutAssertvcEqual(CURREF, "testGENERATEFINALOUTPUT_with_warnings 003", line, "test line 3")
            set stat = alterlist(buffer->lines, 3)
            set buffer->lines[1].line = "second line"
            set buffer->lines[2].line = "third line"
            set buffer->lines[3].line = "fourth line"
    endcase
end

;**********************************************************************************************************************************
;** generateTestCase
;**********************************************************************************************************************************
/* testGENERATETESTCASE *************************************************************************************
*  Scenario: Validates that when a MAIN subroutine is identified, it correctly generates the test case.     *
************************************************************************************************************/
subroutine testGENERATETESTCASE(null)
    declare testGENERATETESTCASE__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare generateTestCaseDataCalled = i2 with protect, noconstant(FALSE)
    declare generateFinalOutputCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "GENERATETESTCASE_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATETESTCASE")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_GENERATETESTCASE")

    call cclutAsserti2Equal(CURREF, "testGENERATETESTCASE 004", testGENERATETESTCASE__MainWasCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGENERATETESTCASE 005", generateTestCaseDataCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGENERATETESTCASE 006", generateFinalOutputCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testGENERATETESTCASE 007", echoRecordCalled, TRUE)
end
subroutine MAINtestGENERATETESTCASE(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->targetSubroutines[2].name = "MAIN"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE2"
    call GENERATETESTCASE("test output", "test script")
    set testGENERATETESTCASE__MainWasCalled = TRUE
end
subroutine (1_GENERATETESTCASE::GENERATETESTCASEDATA(scriptName = vc) = null)
    set generateTestCaseDataCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASE 001", scriptName, "test script")
end
subroutine (1_GENERATETESTCASE::GENERATEFINALOUTPUT(outputDestination = vc) = null)
    set generateFinalOutputCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASE 002", outputDestination, "test output")
end
subroutine (GENERATETESTCASE_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASE 003", templateReply->status_data.status, "")
    set echoRecordCalled = TRUE
end

/* testGENERATETESTCASE_no_main *****************************************************************************
*  Scenario: Validates that when no MAIN subroutine is identified, it throws an error and exits the script. *
************************************************************************************************************/
subroutine testGENERATETESTCASE_no_main(null)
    declare testGENERATETESTCASE_no_main__MainWasCalled = i2 with protect, noconstant(FALSE)
    declare echoRecordCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("echorecord", "GENERATETESTCASE_no_main_echorecord")

    call cclutAddMockImplementation("MAIN", "MAINtestGENERATETESTCASE_no_main")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "2_GENERATETESTCASE")

    call cclutAsserti2Equal(CURREF, "testGENERATETESTCASE_no_main 006", testGENERATETESTCASE_no_main__MainWasCalled,
        TRUE)
    call cclutAsserti2Equal(CURREF, "testGENERATETESTCASE_no_main 007", echoRecordCalled, TRUE)
end
subroutine MAINtestGENERATETESTCASE_no_main(null)
    set stat = alterlist(templateRec->targetSubroutines, 3)
    set templateRec->targetSubroutines[1].name = "TESTSUBROUTINE1"
    set templateRec->targetSubroutines[2].name = "TESTSUBROUTINE2"
    set templateRec->targetSubroutines[3].name = "TESTSUBROUTINE3"
    set testGENERATETESTCASE_no_main__MainWasCalled = TRUE
    call GENERATETESTCASE("test output", "test script")
end
subroutine (2_GENERATETESTCASE::GENERATETESTCASEDATA(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (2_GENERATETESTCASE::GENERATEFINALOUTPUT(null) = null)
    call cclexception(100, "E", "Should not have been called")
end
subroutine (GENERATETESTCASE_no_main_echorecord(templateReply = vc(ref)) = null)
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASE_no_main 001", templateReply->status_data.status, "F")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASE_no_main 002",
        trim(templateReply->status_data.subeventstatus[1].OperationName), "Generate test case")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASE_no_main 003",
        templateReply->status_data.subeventstatus[1].OperationStatus, "F")
    call cclutAssertvcEqual(CURREF, "testGENERATETESTCASE_no_main 004",
        trim(templateReply->status_data.subeventstatus[1].TargetObjectName), "test script")
    call cclutAssertVcOperator(CURREF, "testGENERATETESTCASE_no_main 005",
        templateReply->status_data.subeventstatus[1].TargetObjectValue, "regexplike",
        concat("%CCL-E-392-CCLUT_GENERATE_TEST_CASE\([^)]+\)[0-9]+:[0-9]+\{CCLEXCEPTION\(\)\}Exception\(100\): ",
            "No main subroutine found in program.  A main subroutine must be present."))
    set echoRecordCalled = TRUE
end

;**********************************************************************************************************************************
;** addSuccessToReply
;**********************************************************************************************************************************
/* testADDSUCCESSTOREPLY ************************************************************************************
*  Scenario: Validates that the status of templateReply is updated to be "S".                               *
************************************************************************************************************/
subroutine testADDSUCCESSTOREPLY(null)
    declare testADDSUCCESSTOREPLY__MainWasCalled = i2 with protect, noconstant(FALSE)

    call cclutAddMockImplementation("MAIN", "MAINtestADDSUCCESSTOREPLY")
    call cclutExecuteProgramWithMocks("cclut_generate_test_case", "^nl:^", "1_ADDSUCCESSTOREPLY")

    call cclutAsserti2Equal(CURREF, "testADDSUCCESSTOREPLY 002", testADDSUCCESSTOREPLY__MainWasCalled, TRUE)
end
subroutine MAINtestADDSUCCESSTOREPLY(null)
    call ADDSUCCESSTOREPLY(null)
    set testADDSUCCESSTOREPLY__MainWasCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testADDSUCCESSTOREPLY 001", templateReply->status_data.status, "S")
end

;**********************************************************************************************************************************
;** main
;**********************************************************************************************************************************
/* testMAIN *************************************************************************************************
*  Scenario: Validates that the main subroutine correctly calls all the subroutines that it is supposed to. *
************************************************************************************************************/
subroutine testMAIN(null)
    declare validateParametersCalled = i2 with protect, noconstant(FALSE)
    declare createProgramXmlCalled = i2 with protect, noconstant(FALSE)
    declare identifySubroutinesToTestCalled = i2 with protect, noconstant(FALSE)
    declare createMockListCalled = i2 with protect, noconstant(FALSE)
    declare generateTestCaseCalled = i2 with protect, noconstant(FALSE)
    declare addSuccessToReplyCalled = i2 with protect, noconstant(FALSE)

    call cclutExecuteProgramWithMocks("cclut_generate_test_case",
        "^test output^, ^test script^, ^test location^, ^test include^", "1_MAIN")

    call cclutAsserti2Equal(CURREF, "testMAIN 011", validateParametersCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testMAIN 012", createProgramXmlCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testMAIN 013", identifySubroutinesToTestCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testMAIN 014", createMockListCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testMAIN 015", generateTestCaseCalled, TRUE)
    call cclutAsserti2Equal(CURREF, "testMAIN 016", addSuccessToReplyCalled, TRUE)
end
subroutine (1_MAIN::VALIDATEPARAMETERS(scriptUnderTest = vc, includeFiles = vc, sourceFileLocation = vc) = null)
    set validateParametersCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testMAIN 001", scriptUnderTest, "test script")
    call cclutAssertvcEqual(CURREF, "testMAIN 002", includeFiles, "test include")
    call cclutAssertvcEqual(CURREF, "testMAIN 003", sourceFileLocation, "test location")
end
subroutine (1_MAIN::CREATEPROGRAMXML(scriptUnderTest = vc) = null)
    set createProgramXmlCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testMAIN 004", scriptUnderTest, "test script")
    set templateRec->programXML = "test xml"
end
subroutine (1_MAIN::IDENTIFYSUBROUTINESTOTEST(programXML = vc, sourceFileLocation = vc, scriptUnderTest = vc) = null)
    set identifySubroutinesToTestCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testMAIN 005", programXML, "test xml")
    call cclutAssertvcEqual(CURREF, "testMAIN 006", sourceFileLocation, "test location")
    call cclutAssertvcEqual(CURREF, "testMAIN 007", scriptUnderTest, "test script")
end
subroutine (1_MAIN::CREATEMOCKLIST(programXML = vc) = null)
    set createMockListCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testMAIN 008", programXML, "test xml")
end
subroutine (1_MAIN::GENERATETESTCASE(outputDestination = vc, scriptUnderTest = vc) = null)
    set generateTestCaseCalled = TRUE
    call cclutAssertvcEqual(CURREF, "testMAIN 009", outputDestination, "test output")
    call cclutAssertvcEqual(CURREF, "testMAIN 010", scriptUnderTest, "test script")
end
subroutine (1_MAIN::ADDSUCCESSTOREPLY(null) = null)
    set addSuccessToReplyCalled = TRUE
end
