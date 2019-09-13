drop program cclut_generate_test_case:dba go
create program cclut_generate_test_case:dba

/**
  A prompt program for generating a CCL Unit test case for a specified program object.
  The generated test case will contain tests and mock implementations only for subroutines in the public namespace (i.e.
  with a <b>public::</b> namespace or no namespace).
  @arg (vc) The output destination for the test case.
    @default MINE
  @arg (vc) The name of the CCL object for which the test case will be generated.
    Required for test case generatation. Omit to see usage instructions.
    @default ""
  @arg (vc) The location of the source file used to create the program. If not directly specified, <b>cclsource:</b> is
    assumed for the directory and <b>.prg</b> is assumed for the extension.  If the program's source file cannot be
    located, tests will be generated for all of the program's subroutines in the public namespace.  If the located
    source file is not the file that was used to create the program, unexpected results may occur.
    @default <b>cclsource:&lt;objectName&gt;.prg</b>
  @arg (vc) A pipe-delimited list of files included by the program's source file which define additional
    subroutines for which tests should be generated.
    By default, test are only generated for subroutines defined directly in the program's source file.
    The listed files are assumed to reside in <b>cclsource:</b> and have a <b>.inc</b> extension unless otherwise
    specified.
    @default ""
*/
prompt
  "Output Destination [MINE]: " = "MINE",
  'Script Under Test (Object Name) [""]: ' = "",
  "Source File Location [cclsource:<objectName>.prg]: " = "",
  'Include Files To Be Tested [""]: ' = ""
with outputDestination, scriptUnderTest, sourceFileLocation, includeFiles

%i cclsource:cclut_utils.inc
%i cclsource:cclut_error_handling.inc
%i cclsource:cclut_get_file_as_string.inc
%i cclsource:cclut_xml_access_subs.inc
%i cclsource:cclut_compile_subs.inc

record templateRec (
    1 programXML = vc
    1 mockableSubroutines[*]
        2 name = vc
    1 targetSubroutines[*]
        2 name = vc
        2 namespaceForMocks = vc
        2 namespaceForMocksIdentifier = i4
        2 mockSubroutines[*]
            3 name = vc
        2 mockScripts[*]
            3 name = vc
        2 mockTables[*]
            3 name = vc
            3 columns[*]
                4 name = vc
    1 warnings[*]
        2 warning = vc
    1 messages[*]
        2 message = vc
    1 testFile[*]
        2 line = vc
    1 mockSubroutineCommentAdded = i2
    1 mockScriptCommentAdded = i2
    1 mockTableCommentAdded = i2
    1 mockDataCommentAdded = i2
) with protect

record includeRec (
    1 qual[*]
        2 str = vc
        2 regex_str = vc
) with protect

record templateReply (
%i cclsource:status_block.inc
) with protect

declare INCLUDE_DELIMITER = vc with protect, constant("|")
declare CCLUT_PREFIX = vc with protect, constant("cclut_")
declare CCLUT_SUFFIX = vc with protect, constant(concat("_", trim(cnvtstring(curtime3, 11), 3), cnvtlower(curuser)))
declare CCLSOURCE = vc with protect, constant("CCLSOURCE")
declare CCLUSERDIR = vc with protect, constant("CCLUSERDIR")
declare CCLSOURCE_LOCATION = vc with protect, constant(concat(trim(logical(CCLSOURCE), 3), "/"))
declare CCLUSERDIR_LOCATION = vc with protect, constant(concat(trim(logical(CCLUSERDIR), 3), "/"))
declare INCLUDE_REGEX = vc with protect, constant("^%[iI][^[:space:]]*[[:space:]]+[^[:space:]]+.*$")
declare TAB = vc with protect, constant(notrim(fillstring(4, char(32))))
declare LINE_FEED = vc with protect, constant(char(10))
declare LINE_MAX_LENGTH = i4 with protect, constant(130)
declare NAMESPACE_MAX_LENGTH = i4 with protect, constant(37) ;The actual limit is 40, but this gives some flexibility
                                                             ;for incrementing test numbers
declare EMPTY_LINE = vc with protect, constant(trim(""))
declare TEST_IDENTIFIER_PREFIX = vc with protect, constant("1_")
declare TEST_PREFIX = vc with protect, constant("test")
declare MAIN_SUBROUTINE = vc with protect, constant("MAIN")
declare SUBROUTINE_COMMENT_PART_1 = vc with protect, constant(concat("; TODO: All calls to subroutines declared in ",
    "the script-under-test have been prepped for mocking using namespaces such as below."))
declare SUBROUTINE_COMMENT_PART_2 = vc with protect, constant(concat("; If the inbound parameters need to be captured ",
    "or the return type specified, each mock should be updated accordingly."))
declare SCRIPT_COMMENT_PART_1 = vc with protect, constant(concat(TAB, "; TODO: Script calls in the script-under-test ",
    "have been prepped for mocking in this file using cclutAddMockImplementation."))
declare SCRIPT_COMMENT_PART_2 = vc with protect, constant(concat(TAB, "; Each instance of cclutAddMockImplementation ",
    "in this file must be updated with the mock script name in the second parameter."))
declare TABLE_COMMENT_PART_1 = vc with protect, constant(concat(TAB, "; TODO: Table calls in the script-under-test ",
    "have been prepped for mocking in this file using cclutDefineMockTable.  Each"))
declare TABLE_COMMENT_PART_2 = vc with protect, constant(concat(TAB, "; instance of cclutDefineMockTable in this file ",
    "must be updated with the column types in the second parameter."))
declare DATA_COMMENT_PART_1 = vc with protect, constant(concat(TAB, "; TODO: All mock tables have the below line to ",
    "aid in adding mock data.  Each instance of cclutAddMockData in this file must"))
declare DATA_COMMENT_PART_2 = vc with protect, constant(concat(TAB, "; be updated with the mock data in the second ",
    "parameter.  The line can be copied multiple times for multiple rows of data."))

declare outputDestination = vc with protect, noconstant(trim($outputDestination, 3))
declare scriptUnderTest = vc with protect, noconstant(trim($scriptUnderTest, 3))
declare sourceFileLocation = vc with protect, noconstant(trim($sourceFileLocation, 3))
declare includeFiles = vc with protect, noconstant(trim($includeFiles, 3))

/**
Outputs the usage instructions for the program to the output destination.
*/
subroutine (PUBLIC::outputUsageInstructions(null) = null with protect)
    select into value(outputDestination)
        from (dummyt d1 with seq = 1)
        plan d1
        head report
            value = fillstring(132, " ")
            value = "cclut_generate_test_case is a prompt program for generating a CCL Unit test case for a specified"
            col 0 value row+1
            value = "program object.  The generated test case will contain tests and mock implementations only for"
            col 0 value row+1
            value = "subroutines in the public namespace (i.e. with a public:: namespace or no namespace).  The"
            col 0 value row+1
            value = "following are the parameters for the program:"
            col 0 value row+1
            value = " "
            col 0 value row+1
            value = "outputDestination"
            col 0 value row+1
            value = "    The output destination for the test case."
            col 0 value row+1
            value = " "
            col 0 value row+1
            value = "scriptUnderTest"
            col 0 value row+1
            value = "    The name of the CCL object for which the test case will be generated."
            col 0 value row+1
            value = " "
            col 0 value row+1
            value = "sourceFileLocation"
            col 0 value row+1
            value = "    The location of the source file used to create the program. If not directly specified,"
            col 0 value row+1
            value = "    cclsource: is assumed for the directory and .prg is assumed for the extension.  If the"
            col 0 value row+1
            value = "    program's source file cannot be located, tests will be generated for all of the program's"
            col 0 value row+1
            value = "    subroutines in the public namespace.  If the located source file is not the file that was used"
            col 0 value row+1
            value = "    to create the program, unexpected results may occur."
            col 0 value row+1
            value = " "
            col 0 value row+1
            value = "includeFiles"
            col 0 value row+1
            value = "    A pipe-delimited list of files included by the program's source file which define additional"
            col 0 value row+1
            value = "    subroutines for which tests should be generated.  By default, test are only generated for"
            col 0 value row+1
            value = "    subroutines defined directly in the program's source file.  The listed files are assumed to"
            col 0 value row+1
            value = "    reside in cclsource: and have a .inc extension unless otherwise specified."
            col 0 value row+1
            value = " "
            col 0 value row+1
            value = "For additional information on how the test case is generated and examples, please visit"
            col 0 value row+1
            value = "https://github.com/cerner/cclunit-framework/blob/master/doc/CCLUTTEMPLATES.md"
            col 0 value row+1
        with nocounter, maxrow = 1, maxcol = 133, compress, nullreport, noheading, format = variable, formfeed = none
end ;PUBLIC::outputUsageInstructions

/**
Validates the three parameters supplied to cclut_generate_test_case and populate include files, if any, into
includeRec.  scriptUnderTest cannot be empty.

@param scriptUnderTest
    The script for which the test file will be generated.
@param includeFiles
    A pipe-delimited list of include files that should also be tested.
@param sourceFileLocation
    The location of the source file for the scriptUnderTest.
*/
subroutine (PUBLIC::validateParameters(scriptUnderTest = vc, includeFiles = vc,
    sourceFileLocation = vc(ref)) = null with protect)
        ; If the script is empty, output the usage instructions and terminate the program.
        if (cclutIsEmpty(scriptUnderTest))
            call outputUsageInstructions(null)
            go to exit_script
        endif

        ; Validate that scriptUnderTest exists
        if (checkprg(cnvtupper(scriptUnderTest)) = 0)
            call cclexception(100, "E", "Script could not be found in CCL dictionary.")
            call cclut::exitOnError("Validating parameters", scriptUnderTest, templateReply)
        endif

        if (cclutIsEmpty(sourceFileLocation) = TRUE)
            set sourceFileLocation = scriptUnderTest
        endif
        ; If there is no extension, default to .prg
        declare extensionDelimiter = i4 with protect, noconstant(findstring(".", sourceFileLocation))
        if (extensionDelimiter = 0)
            set sourceFileLocation = concat(sourceFileLocation, ".prg")
        endif
        ; If there is no directory, default to cclsource.
        declare directoryDelimiter = i4 with protect, noconstant(findstring(":", sourceFileLocation))
        if (directoryDelimiter = 0)
            set sourceFileLocation = concat(CCLSOURCE_LOCATION, sourceFileLocation)
        else
            set sourceFileLocation = concat(trim(logical(substring(1, directoryDelimiter - 1, sourceFileLocation)), 3),
                "/", substring((directoryDelimiter + 1), (textlen(sourceFileLocation) - directoryDelimiter),
                    sourceFileLocation))
        endif

        ; includeFiles is allowed to be empty (and defaults to empty if not provided)
        if (cclutIsEmpty(includeFiles) = FALSE)
            call cclutDoArraySplit(includeRec, includeFiles, INCLUDE_DELIMITER)
        endif
end ;PUBLIC::validateParamters

/**
Creates an XML-translated version of the script program, verifies that it is not empty, saves the result in
templateRec->programXML, and removes it from the file system.

@param scriptUnderTest
    The name of the object to be translated.
*/
subroutine (PUBLIC::createProgramXML(scriptUnderTest = vc) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    ; Create a file location for translating the scriptUnderTest
    declare xmlFileName = vc with protect, constant(concat(CCLUT_PREFIX, "tmpl_prg", CCLUT_SUFFIX, ".xml"))
    declare xmlFilePath = vc with protect, constant(concat(CCLUSERDIR_LOCATION, xmlFileName))

    call parser(concat('translate into "', CCLUSERDIR, ":", xmlFileName, '" ', scriptUnderTest, " with xml go"))

    ; Read the translated file
    set templateRec->programXML = cclut::getFileAsString(xmlFilePath)

    if (validate(cclut::noRemove, FALSE) = FALSE)
        set cclStat = remove(xmlFileName)
    endif

    ; Throw an error if translated program is empty
    if (cclutIsEmpty(templateRec->programXML))
        call cclexception(100, "E",
            concat("Failed to translate program.  XML file ccluserdir:", xmlFileName, " is empty."))
        call cclut::exitOnError("Translate program", scriptUnderTest, templateReply)
    endif
end ;PUBLIC::createProgramXML

/**
Returns the text attribute of the NAME node at the supplied nameIndex within the supplied element.  If there are no
NAME nodes for the element, it returns an empty string.

@param element
    The named element
@param nameIndex
    The index of which NAME node to pull the text
@returns
    The name of the element.
*/
subroutine (PUBLIC::getNamedElementName(element = h, nameIndex = i4) = vc with protect)
    return(cclut::getXMLAttributeValue(cclut::getXmlListItemHandle(element, "NAME", nameIndex), "text"))
end ;PUBLIC::getNamedElementName

/**
Checks an element to see if it is in the public namespace and has a name.  If it does, the name of the public element is
returned.  Otherwise, an empty string is returned.

@param element
    The element to check if it is in the public namespace
@returns
    The name of the element.
*/
subroutine (PUBLIC::getPublicNamedElementName(element = h) = vc with protect)
    declare hNamespace = h with protect, noconstant(0)

    set hNamespace = cclut::getXmlListItemHandle(element, "NAMESPACE.", 1)
    ; If the namespace is public, return the subroutine name.
    if (hNamespace != 0)
        if (getNamedElementName(hNamespace, 1) = "PUBLIC")
            return (getNamedElementName(hNamespace, 2))
        endif
    else
        ; If there is no namespace, the element is in the public namespace by default.
        return (getNamedElementName(element, 1))
    endif

    return("")
end ;PUBLIC::getPublicNamedElementName

/**
Populates a record structure supplied by subroutinesRecord with the names of all subroutines implemented in a provided
XML program translation that are in the public namespace.

@param programXML
    The XML program translation in which to find subroutines
@param subroutinesRecord
    The record structure in which subroutines will be populated
@param xmlFailureError
    Boolean indicating whether an XML failure should result in an error or not.
@param objectName
    The program name for .prgs or the include file for .incs.
*/
subroutine (PUBLIC::findProgramSubroutines(programXML = vc, subroutinesRecord = vc(ref),
    xmlFailureError = i2, objectName = vc) = null with protect)
        declare cclStat = i4 with protect, noconstant(0)
        declare hXmlRoot = h with protect, noconstant(0)
        declare hXmlFile = h with protect, noconstant(0)
        declare hProgram = h with protect, noconstant(0)
        declare hSubroutine = h with protect, noconstant(0)
        declare totalSubroutineCount = i4 with protect, noconstant(1)
        declare publicSubroutineCount = i4 with protect, noconstant(size(subroutinesRecord->targetSubroutines, 5))
        declare subroutineName = vc with protect, noconstant("")

        ; Parse the XML file
        set hXmlRoot = cclut::parseXmlBuffer(programXML, hXmlFile)
        if (hXmlRoot = 0 or hXmlFile = 0)
            if (xmlFailureError)
                call cclexception(100, "E",
                    concat("findProgramSubroutines failed to parse program XML.  XML: ", programXML))
                call cclut::exitOnError("Parse program", objectName, templateReply)
            else
                ; One of the include XMLs failed, so log and end current subroutine.
                call addWarning(concat("Failed to parse XML from include file ", objectName, "."))
                return (null)
            endif
        endif

        set hProgram = cclut::getXmlListItemHandle(hXmlRoot, "ZC_PROGRAM.", 1)
        set hSubroutine = cclut::getXmlListItemHandle(hProgram, "SUBROUTINE.", totalSubroutineCount)

        ; Loop over all public subroutines defined in the program
        while (hSubroutine != 0)
            set subroutineName = getPublicNamedElementName(hSubroutine)
            if (cclutIsEmpty(subroutineName) = FALSE)
                set publicSubroutineCount = publicSubroutineCount + 1
                set cclStat = alterlist(subroutinesRecord->targetSubroutines, publicSubroutineCount)
                set subroutinesRecord->targetSubroutines[publicSubroutineCount].name = subroutineName
            endif

            set totalSubroutineCount = totalSubroutineCount + 1
            set hSubroutine = cclut::getXmlListItemHandle(hProgram, "SUBROUTINE.", totalSubroutineCount)
        endwhile

        call cclut::releaseXmlResources(hXmlFile)
end ;PUBLIC::findProgramSubroutines

/**
Copies the targetSubroutines list from the provided subroutinesRecord to the mockableSubroutines list adding in
appropriate values for namespaces (if necessary).

@param subroutinesRecord
    The record structure containing targetSubroutines and mockableSubroutines
*/
subroutine (PUBLIC::populateMockableSubroutines(subroutinesRecord = vc(ref)) = null with protect)
        declare cclStat = i4 with protect, noconstant(0)
        declare targetSubroutineSize = i4 with protect, noconstant(size(subroutinesRecord->targetSubroutines, 5))
        declare targetSubroutineIndex = i4 with protect, noconstant(0)
        declare namespaceIndex = i4 with protect, noconstant(0)
        declare matchIndex = i4 with protect, noconstant(0)

        set cclStat = alterlist(subroutinesRecord->mockableSubroutines, size(subroutinesRecord->targetSubroutines, 5))
        for (targetSubroutineIndex = 1 to targetSubroutineSize)
            set subroutinesRecord->mockableSubroutines[targetSubroutineIndex].name =
                subroutinesRecord->targetSubroutines[targetSubroutineIndex].name

            ; Check for collisions when the subroutine name is used as a namespace (using a for loop as the last one
            ; needs to be found and locateval goes forward)
            set subroutinesRecord->targetSubroutines[targetSubroutineIndex].namespaceForMocks =
                substring(1, NAMESPACE_MAX_LENGTH - textlen(TEST_IDENTIFIER_PREFIX),
                    subroutinesRecord->targetSubroutines[targetSubroutineIndex].name)
            set subroutinesRecord->targetSubroutines[targetSubroutineIndex].namespaceForMocksIdentifier = 0
            for (namespaceIndex = 1 to (targetSubroutineIndex - 1))
                set matchIndex = targetSubroutineIndex - namespaceIndex
                if (subroutinesRecord->targetSubroutines[targetSubroutineIndex].namespaceForMocks =
                    subroutinesRecord->targetSubroutines[matchIndex].namespaceForMocks)
                        ; If a match is found and it is the first duplicate, update the first instance to 1 and the
                        ; second instance to 2; if it is the second duplicate or higher, increment from the last
                        ; duplicate.
                        if (subroutinesRecord->targetSubroutines[matchIndex].namespaceForMocksIdentifier = 0)
                            set subroutinesRecord->targetSubroutines[matchIndex].namespaceForMocksIdentifier = 1
                        endif
                        set subroutinesRecord->targetSubroutines[targetSubroutineIndex].namespaceForMocksIdentifier =
                            subroutinesRecord->targetSubroutines[matchIndex].namespaceForMocksIdentifier + 1
                        set namespaceIndex = targetSubroutineIndex ;break
                endif
            endfor
        endfor
end ;PUBLIC::populateMockableSubroutines

/**
Takes each supplied include file and generates a regular expression for it to be used in searching the program source.
Any include file without an extension is assumed to be ".inc", and any include file without a directory is assumed to be
cclsource.
*/
subroutine (PUBLIC::generateIncludeFileRegularExpressions(null) = null with protect)
    declare includeIndex = i4 with protect, noconstant(0)
    declare includeCount = i4 with protect, noconstant(size(includeRec->qual, 5))
    declare includeString = vc with protect, noconstant("")
    declare regexIndex = i4 with protect, noconstant(0)
    declare regexCount = i4 with protect, noconstant(0)
    declare regexString = vc with protect, noconstant("")

    for (includeIndex = 1 to includeCount)
        set includeString = includeRec->qual[includeIndex].str
        ; If an include has no extension, default to .inc
        if (findstring(".", includeString) = 0)
            set includeString = concat(includeString, ".inc")
        endif
        ; If an include has no directory, default to cclsource:
        if (findstring(":", includeString) = 0)
            set includeString = concat(CCLSOURCE, ":", includeString)
        endif
        set includeRec->qual[includeIndex].str = includeString

        set regexCount = textlen(includeString)
        ; The regular expression checks if the line follows the pattern of
        ; %i<non-whitespace characters 0 or more times><whitespace characters 1 or more times>
        ; <the include file characters><end-of-line or ignored characters>
        ; This allows the following lines to all pass (which are valid for an include called sample_include.inc
        ; %i cclsource:sample_include.inc
        ; %I cclsource:sample_include.inc
        ; %include cclsource:sample_include.inc
        ; %ijunk cclsource:sample_include.inc junk
        set regexString = "^%[iI][^[:space:]]*[[:space:]]+"
        ; Construct a POSIX regular expression for uppercase and lowercase of each character
        for (regexIndex = 1 to regexCount)
            set regexString = concat(regexString, "[", cnvtupper(substring(regexIndex, 1, includeString)),
                cnvtlower(substring(regexIndex, 1, includeString)), "]")
        endfor
        set includeRec->qual[includeIndex].regex_str = concat(regexString, "($|([[:space:]]+.*$))")
    endfor
end ;PUBLIC::generateIncludeFileRegularExpressions

/**
Adds a warning to the list of all warnings for the test case generation.  A warning will be displayed in the test case
itself and as output to the listing.

@param warning
    The warning to be added
*/
subroutine (PUBLIC::addWarning(warning = vc) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare warningSize = i4 with protect, noconstant(size(templateRec->warnings, 5) + 1)

    set cclStat = alterlist(templateRec->warnings, warningSize)
    set templateRec->warnings[warningSize].warning = warning
end ;PUBLIC::addWarning

/**
Adds a message to the list of all messages for the test case generation.  A message will be displayed as output to the
listing.

@param message
    The message to be added
*/
subroutine (PUBLIC::addMessage(message = vc) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare messageSize = i4 with protect, noconstant(size(templateRec->messages, 5) + 1)

    set cclStat = alterlist(templateRec->messages, messageSize)
    set templateRec->messages[messageSize].message = message
end ;PUBLIC::addMessage

/**
Populates a record structure supplied by excludedIncludes with the names of all include files to be excluded from having
their subroutines tested.

@param sourceFileLocation
    The path of the source file for the script that is being tested.
@param excludedIncludes
    The record structure in which excluded includes will be populated
*/
subroutine (PUBLIC::getExcludedIncludeList(sourceFileLocation = vc, excludedIncludes = vc(ref)) = null with protect)
    call generateIncludeFileRegularExpressions(null)

    declare cclStat = i4 with protect, noconstant(0)
    declare programSource = vc with protect, noconstant(concat(cclut::getFileAsString(cnvtlower(sourceFileLocation)),
        LINE_FEED))
    declare programLength = i4 with protect, noconstant(textlen(programSource))
    declare lineStart = i4 with protect, noconstant(1)
    declare lineStop = i4 with protect, noconstant(findstring(LINE_FEED, programSource, 1))
    declare currentLine = vc with protect, noconstant("")
    declare includeIndex = i4 with protect, noconstant(1)
    declare includeCount = i4 with protect, noconstant(size(includeRec->qual, 5))
    declare includeMatch = i2 with protect, noconstant(FALSE)
    declare excludeCount = i4 with protect, noconstant(0)

    ; Check if the only character is the added line feed.
    if (programLength = 1)
        call addWarning(concat("Source file could not be found or was empty at ", sourceFileLocation,
            ".  All include file subroutines will be included."))
    else
        while (lineStart < lineStop)
            set currentLine = substring(lineStart, lineStop - lineStart, programSource)
            ; Check if line is an include line
            if (operator(currentLine, "regexplike", INCLUDE_REGEX))
                ; Check against each include to see if it should be excluded
                set includeMatch = FALSE
                for (includeIndex = 1 to includeCount)
                    if (operator(currentLine, "regexplike", includeRec->qual[includeIndex].regex_str))
                        set includeMatch = TRUE
                        set includeIndex = includeCount ;break
                    endif
                endfor

                if (includeMatch = FALSE)
                    ; The include should be excluded, so add it to the list
                    set excludeCount = excludeCount + 1
                    set cclStat = alterlist(excludedIncludes->exclude, excludeCount)
                    set excludedIncludes->exclude[excludeCount].str = trim(currentLine)
                endif
            endif

            ; Find the next linefeed or the end of the program
            set lineStart = lineStop + 1
            set lineStop = findstring(LINE_FEED, programSource, lineStart)
        endwhile
    endif
end ;PUBLIC::getExcludedIncludeList

/**
Creates a program for a supplied include file, compiles it, and returns the XML.  If a step fails, a warning will be
added to templateRec.

@param includeFile
    The name of the includeFile for which XML will be created.
@param includeIndex
    An index for uniquely identifying an include file.
@returns
    The XML of the created program.
*/
subroutine (PUBLIC::createExcludeXML(includeFile = vc, includeIndex = i4) = vc with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare excludeProgramName = vc with protect, noconstant("")
    declare sourceFileName = vc with protect, noconstant("")
    declare sourceFileLocation = vc with protect, noconstant("")
    declare xmlFileName = vc with protect, noconstant("")
    declare xmlFileLocation = vc with protect, noconstant("")
    declare listingFileName = vc with protect, noconstant("")
    declare listingFileLocation = vc with protect, noconstant("")
    declare cclutFailureMessage = vc with protect, noconstant("")
    declare xmlContent = vc with protect, noconstant("")

    ; Create a unique name for each exclude file as well as .prg, .xml, and .lis names.
    set excludeProgramName = substring(1, 30, concat(CCLUT_PREFIX, "tmpl_inc",
        trim(cnvtstring(includeIndex, 11), 3), CCLUT_SUFFIX))
    set sourceFileName = concat(excludeProgramName, ".prg")
    set sourceFileLocation = concat(CCLUSERDIR_LOCATION, sourceFileName)
    set xmlFileName = concat(excludeProgramName, ".xml")
    set xmlFileLocation = concat(CCLUSERDIR_LOCATION, xmlFileName)
    set listingFileName = concat(excludeProgramName, ".lis")
    set listingFileLocation = concat(CCLUSERDIR_LOCATION, listingFileName)

    ; Write out the exclude file into a program with nothing but the excluded include file
    select into value(concat(CCLUSERDIR, ":", sourceFileName))
    from (dummyt d1 with seq = 1)
    plan d1
    head report
        value = fillstring(132, " ")
        value = concat("drop program ", excludeProgramName, " go")
        col 0 value row+1
        value = concat("create program ", excludeProgramName)
        col 0 value row+1
        value = includeFile
        col 0 value row+1
        value = "end go"
        col 0 value row+1
    with nocounter, maxrow = 1, maxcol = 133, compress, nullreport, noheading, format = variable, formfeed = none

    if (not findfile(sourceFileLocation))
        call addWarning(concat("Program file could not be created for excluding the include file ", includeFile,
            ".  Its subroutines may appear in the generated test file."))
    else
        ; Attempt to compile the exclude file program
        if (not cclutCompileProgram(CCLUSERDIR, sourceFileName, CCLUSERDIR, listingFileName, cclutFailureMessage))
            call addMessage(concat("Program file could not be compiled for excluding the include file ", includeFile,
                ".  Its subroutines may appear in the generated test file.  Error from compilation: ",
                cclutFailureMessage))
        else
            call parser(concat('translate into "', CCLUSERDIR, ":", xmlFileName, '" ', excludeProgramName,
                ' with xml go'))

            ; Read the translated file
            set xmlContent = cclut::getFileAsString(xmlFileLocation)
            if (cclutIsEmpty(xmlContent))
                call addWarning(concat("Translation of program file was empty for excluding the include file ",
                    includeFile, ".  Its subroutines may appear in the generated test file."))
            endif

            if (validate(cclut::noRemove, FALSE) = FALSE)
                set cclStat = remove(xmlFileLocation)
            endif
        endif

        if (validate(cclut::noRemove, FALSE) = FALSE)
            set cclStat = remove(sourceFileLocation)
            set cclStat = remove(listingFileLocation)
        endif
    endif

    return(xmlContent)
end ;PUBLIC::createExcludeXML

/**
Identifies the subroutines to be tested from the source program and populates them into templateRec.  Subroutines from
include files that should be excluded will not be populated.

@param programXML
    The XML for the script to be tested
@param sourceFileLocation
    The location of the source file for the script to be tested
@param scriptUnderTest
    The object name for the script to be tested
*/
subroutine (PUBLIC::identifySubroutinesToTest(programXML = vc, sourceFileLocation = vc,
    scriptUnderTest = vc) = null with protect)
        declare cclStat = i4 with protect, noconstant(0)
        declare excludeIndex = i4 with protect, noconstant(0)
        declare excludeSubroutineIndex = i4 with protect, noconstant(0)
        declare excludeXML = vc with protect, noconstant("")
        declare targetSubroutineIndex = i4 with protect, noconstant(0)
        declare targetSubroutineSize = i4 with protect, noconstant(0)
        declare targetSubroutineLocate = i4 with protect, noconstant(0)

        record excludeRec (
            1 exclude[*]
                2 str = vc
            1 targetSubroutines[*]
                2 name = vc
        ) with protect

        call findProgramSubroutines(programXML, templateRec, TRUE, scriptUnderTest)
        call populateMockableSubroutines(templateRec)
        call getExcludedIncludeList(sourceFileLocation, excludeRec)
        for (excludeIndex = 1 to size(excludeRec->exclude, 5))
            set excludeXML = createExcludeXML(excludeRec->exclude[excludeIndex].str, excludeIndex)
            if (cclutIsEmpty(excludeXML) = FALSE)
                call findProgramSubroutines(excludeXML, excludeRec, FALSE, excludeRec->exclude[excludeIndex].str)
            endif
        endfor
        for (excludeSubroutineIndex = 1 to size(excludeRec->targetSubroutines, 5))
            ; Search for it in the list of subroutines to be tested
            set targetSubroutineSize = size(templateRec->targetSubroutines, 5)
            set targetSubroutineLocate = locateVal(targetSubroutineIndex, 1, targetSubroutineSize,
                excludeRec->targetSubroutines[excludeSubroutineIndex].name,
                templateRec->targetSubroutines[targetSubroutineIndex].name)
            ; Remove it from the list if found
            if (targetSubroutineLocate > 0)
                set cclStat = alterlist(templateRec->targetSubroutines, targetSubroutineSize - 1,
                    targetSubroutineLocate - 1)
            endif
        endfor
end ;PUBLIC::identifySubroutinesToTest

/**
Handles mocking of subroutine nodes by checking if the supplied node is valid to be mocked and adding it to the list of
mock subroutines.  Only subroutines defined within the same program will have mocks generated.

@param hCall
    The parent XML node to identify if a subroutine is being called.
@param subroutineIndex
    The index of the source subroutine currently being evaluated.
*/
subroutine (PUBLIC::handleSubroutineNodeMocking(hCall = h, subroutineIndex = i4) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare isDefined = i4 with protect, noconstant(0)
    declare mockableSubroutineIndex = i4 with protect, noconstant(0)
    declare isMocked = i4 with protect, noconstant(0)
    declare mockSubroutineIndex = i4 with protect, noconstant(0)
    declare mockSubroutineSize = i4 with protect, noconstant(
        size(templateRec->targetSubroutines[subroutineIndex].mockSubroutines, 5))
    declare mockSubroutine = vc with protect, noconstant(getNamedElementName(hCall, 1))

    ; Check that a subroutine is being called
    if (cclutIsEmpty(mockSubroutine) = FALSE)
        ; Check to see if the subroutine is 1) defined by the script, 2) not already mocked, and 3) not the same
        ; subroutine being tested (a recursive subroutine).  If all are true, add it to be mocked.
        set isDefined = locateval(mockableSubroutineIndex, 1, size(templateRec->mockableSubroutines, 5), mockSubroutine,
            templateRec->mockableSubroutines[mockableSubroutineIndex].name)
        set isMocked = locateval(mockSubroutineIndex, 1, mockSubroutineSize, mockSubroutine,
            templateRec->targetSubroutines[subroutineIndex].mockSubroutines[mockSubroutineIndex].name)
        if (isDefined > 0 and isMocked = 0 and mockSubroutine != templateRec->targetSubroutines[subroutineIndex].name)
            set mockSubroutineSize = mockSubroutineSize + 1
            set cclStat = alterlist(templateRec->targetSubroutines[subroutineIndex].mockSubroutines, mockSubroutineSize)
            set templateRec->targetSubroutines[subroutineIndex].mockSubroutines[mockSubroutineSize].name =
                mockSubroutine
        endif
    endif
end ;PUBLIC::handleSubroutineNodeMocking

/**
Handles mocking of script nodes by checking that they have not already been mocked and adding it to the list of mock
scripts.  Only scripts called using "execute" will have mocks generated.

@param hZExecute
    The parent XML node to identify if a script is being executed.
@param subroutineIndex
    The index of the source subroutine currently being evaluated.
*/
subroutine (PUBLIC::handleScriptNodeMocking(hZExecute = h, subroutineIndex = i4) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare mockScriptLocate = i4 with protect, noconstant(0)
    declare mockScriptIndex = i4 with protect, noconstant(0)
    declare mockScriptSize = i4 with protect, noconstant(size(
        templateRec->targetSubroutines[subroutineIndex].mockScripts, 5))
    declare mockScript = vc with protect, noconstant(getNamedElementName(cclut::getXmlListItemHandle(hZExecute,
        "USER.", 1), 1))

    ; Validate that an external script is being executed
    if (cclutIsEmpty(mockScript) = FALSE)
        ; Check that the script has not already had a mock added
        set mockScriptLocate = locateval(mockScriptIndex, 1, mockScriptSize, mockScript,
            templateRec->targetSubroutines[subroutineIndex].mockScripts[mockScriptIndex].name)
        if (mockScriptLocate = 0)
            set mockScriptSize = mockScriptSize + 1
            set cclStat = alterlist(templateRec->targetSubroutines[subroutineIndex].mockScripts, mockScriptSize)
            set templateRec->targetSubroutines[subroutineIndex].mockScripts[mockScriptSize].name = mockScript
        endif
    endif
end ;PUBLIC::handleScriptNodeMocking

/**
Merges the temporary mock table record structure into the overall record structure.  It is possible that tables/columns
already exist in the overall record structure, so duplicates will not be added.

@param subroutineIndex
    The index of the source subroutine currently being evaluated.
*/
subroutine (PUBLIC::mergeTableRecs(subroutineIndex = i4) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare mockTableIndex = i4 with protect, noconstant(0)
    declare mockTableSize = i4 with protect, noconstant(size(mockTableRec->tables, 5))
    declare mockTableName = vc with protect, noconstant("")
    declare tableLocate = i4 with protect, noconstant(0)
    declare tableIndex = i4 with protect, noconstant(0)
    declare tableSize = i4 with protect, noconstant(size(templateRec->targetSubroutines[subroutineIndex].mockTables, 5))
    declare mockColumnIndex = i4 with protect, noconstant(0)
    declare mockColumnSize = i4 with protect, noconstant(0)
    declare mockColumnName = vc with protect, noconstant("")
    declare columnLocate = i4 with protect, noconstant(0)
    declare columnIndex = i4 with protect, noconstant(0)
    declare columnSize = i4 with protect, noconstant(0)

    ; Loop over each table in the temporary record structure
    for (mockTableIndex = 1 to mockTableSize)
        set mockTableName = mockTableRec->tables[mockTableIndex].name
        set mockColumnSize = size(mockTableRec->tables[mockTableIndex].columns, 5)
        set tableLocate = locateval(tableIndex, 1, tableSize, mockTableName,
            templateRec->targetSubroutines[subroutineIndex].mockTables[tableIndex].name)

        ; If the mockColumnSize is 0, either aliases were not used (and columns could not be identified) or an asterisk
        ; was used.  In either case, the tester will need to fill in the table columns.
        if (mockColumnSize = 0)
            call addWarning(concat("No columns could be identified for one of the queries involving table ",
                mockTableName, ".  It is recommended to always use table aliases when referencing columns.  If the ",
                "program is correct, you will need to add columns to the instances of cclutDefineMockTable in this ",
                "generated test case that use the table."))
        endif

        ; If the table does not exist in the overall record structure, create a new entry for the table and move all
        ; columns over.
        if (tableLocate = 0)
            set tableSize = tableSize + 1
            set cclStat = alterlist(templateRec->targetSubroutines[subroutineIndex].mockTables, tableSize)
            set templateRec->targetSubroutines[subroutineIndex].mockTables[tableSize].name = mockTableName
            set cclStat = movereclist(mockTableRec->tables[mockTableIndex].columns,
                templateRec->targetSubroutines[subroutineIndex].mockTables[tableSize].columns, 1, 0, mockColumnSize,
                TRUE)
        else
            ; If the table already exists in the overall record structure, loop over each column to see if it also
            ; exists.
            set columnSize = size(templateRec->targetSubroutines[subroutineIndex].mockTables[tableLocate].columns, 5)
            for (mockColumnIndex = 1 to mockColumnSize)
                set mockColumnName = mockTableRec->tables[mockTableIndex].columns[mockColumnIndex].name
                set columnLocate =  locateval(columnIndex, 1, columnSize, mockColumnName,
                    templateRec->targetSubroutines[subroutineIndex].mockTables[tableLocate].columns[columnIndex].name)
                ; If the column does not exist, add it to the overall record structure.
                if (columnLocate = 0)
                    set columnSize = columnSize + 1
                    set cclStat =
                        alterlist(templateRec->targetSubroutines[subroutineIndex].mockTables[tableLocate].columns,
                        columnSize)
                    set templateRec->targetSubroutines[subroutineIndex].mockTables[tableLocate].columns[columnSize].
                        name = mockColumnName
                endif
            endfor
        endif
    endfor
end ;PUBLIC::mergeTableRecs

/**
Checks for all tables and aliases used within Selects, Inserts, Updates, Deletes, or Merges.  Dummyt and Dual are
excluded from mocking.

@param hZCrud
    The XML node to identify if a table is being leveraged.
@param isParentMerge
    A boolean to indicate whether the parent XML node is a Merge operation.
*/
subroutine (PUBLIC::addMockTablesAndAliases(hZCrud = h, isParentMerge = i2) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare hComma = h with protect, noconstant(0)
    declare hTable = h with protect, noconstant(0)
    declare hTableCount = i4 with protect, noconstant(1)
    declare tableLocate = i4 with protect, noconstant(0)
    declare tableIndex = i4 with protect, noconstant(0)
    declare tableName = vc with protect, noconstant("")
    declare tableSize = i4 with protect, noconstant(size(mockTableRec->tables, 5))
    declare tableAliasLocate = i4 with protect, noconstant(0)
    declare tableAliasIndex = i4 with protect, noconstant(0)
    declare tableAlias = vc with protect, noconstant("")
    declare tableAliasSize = i4 with protect, noconstant(0)
    declare isSelect = i2 with protect, noconstant(evaluate(uar_xml_getnodename(hZCrud), "Z_SELECT.", 1, 0))

    ; For Select, first Comma are keywords (i.e. distinct).  Second Comma are select expressions.  Third Comma are
    ; tables.  For all other CRUD operations, the first Comma are the tables.
    set hComma = cclut::getXmlListItemHandle(hZCrud, "COMMA.", evaluate(isSelect, 1, 3, 1))
    set hTable = cclut::getXmlListItemHandle(hComma, "TABLE.", hTableCount)

    ; Get all table names and aliases for the CRUD operation.  For CRUDs underneath a Merge, do not pull tables unless
    ; it is a Select.  Updates/Inserts can reference aliases for tables outside its context, which do not matter anyway
    ; as CCL ignores them and always uses the merge table if the aliases/tables are incorrect.
    if (isParentMerge = FALSE or isSelect = TRUE)
        while (hTable != 0)
            ; If two Names are present, they are the table name and table alias.  If there is only one Name, it is
            ; either a table without an alias or an inline select table.  Inline select tables can be ignored on this
            ; iteration because searchForMockableItems will pick it up when it digs down to that level.
            set tableName = getNamedElementName(hTable, 1)
            set tableAlias = getNamedElementName(hTable, 2)

            ; Exclude DUMMYT and DUAL (if a consumer really wants to mock these, they can add the lines in manually)
            if (cclutIsEmpty(tableName) = FALSE and tableName != "DUMMYT" and tableName != "DUAL" and
                cclut::getXmlListItemHandle(hTable, "Z_SELECT.", 1) = 0)
                    set tableLocate = locateval(tableIndex, 1, tableSize, tableName,
                        mockTableRec->tables[tableIndex].name)
                    ; If the table is new, add it to the temporary mock table record structure
                    if (tableLocate = 0)
                        set tableSize = tableSize + 1
                        set cclStat = alterlist(mockTableRec->tables, tableSize)
                        set mockTableRec->tables[tableSize].name = tableName
                        set tableLocate = tableSize
                    endif
                    ; If there is an alias associated with the table, add it to the temporary mock table record
                    ; structure if it has not already been added.
                    if (cclutIsEmpty(tableAlias) = FALSE)
                        set tableAliasSize = size(mockTableRec->tables[tableLocate].aliases, 5)
                        set tableAliasLocate = locateval(tableAliasIndex, 1, tableAliasSize, tableAlias,
                            mockTableRec->tables[tableLocate].aliases[tableAliasIndex].alias)
                        if (tableAliasLocate = 0)
                            set tableAliasSize = tableAliasSize + 1
                            set cclStat = alterlist(mockTableRec->tables[tableLocate].aliases, tableAliasSize)
                            set mockTableRec->tables[tableLocate].aliases[tableAliasSize].alias = tableAlias
                        endif
                    endif
            endif

            set hTableCount = hTableCount + 1
            set hTable = cclut::getXmlListItemHandle(hComma, "TABLE.", hTableCount)
        endwhile
    endif
end ;PUBLIC::addMockTablesAndAliases

/**
Handles mocking of tables by searching for all tables used in the specific operation and merging those into the
overall record structure.

@param hZCrud
    The XML node to identify if a table is being leveraged.
@param subroutineIndex
    The index of the source subroutine currently being evaluated.
@param isParentMerge
    A boolean to indicate whether the parent XML node is a Merge operation.
*/
subroutine (PUBLIC::handleTableNodeMocking(hZCrud = h, subroutineIndex = i4, isParentMerge = i2) = null with protect)
    declare recordBubble = i4 with protect, noconstant(TRUE)
    if (validate(mockTableRec) = 0)
        ; For every CRUD operation, a temporary record structure is created to house all the mock tables from that CRUD
        ; operation.  The reason for using this instead of the overall record structure is that a particular alias might
        ; be used for two different tables in two different queries.  Take for example, the following two Selects:
        ;
        ; select p.name_full_formatted
        ; from person p
        ; where p.person_id = 1
        ;
        ; select p.name_full_formatted
        ; from prsnl p
        ; where p.person_id = 2
        ;
        ; The program should create two mock tables (PERSON and PRSNL) each with both the person_id and
        ; name_full_formatted fields.  If we use the overall record structure, it would be difficult to distinguish if
        ; the second p.person_id should belong to person or prsnl (this can get even trickier with nested select
        ; queries).  As a result, a temporary record structure is created and merged back into the overall record
        ; structure after the query has been scanned.
        record mockTableRec (
            1 tables[*]
                2 name = vc
                2 aliases[*]
                    3 alias = vc
                2 columns[*]
                    3 name = vc
        ) with protect

        set recordBubble = FALSE
    endif

    call addMockTablesAndAliases(hZCrud, isParentMerge)

    ; Subroutines, nested tables, and columns can still be present in a CRUD XML node, so continue searching.
    call searchForMockableItems(hZCrud, subroutineIndex)

    ; If recordBubble is true, let the merge bubble up to the parent since this is a table inside a table.  If
    ; recordBubble is false, this is the top-level CRUD operation, so merge the tables into the program record
    ; structure.
    if (recordBubble = FALSE)
        call mergeTableRecs(subroutineIndex)
    endif
end ;PUBLIC::handleTableNodeMocking

/**
Checks that the column does not already have a mock for the table at the location specified by tableIndex.  If it does
not, one is added.

@param tableIndex
    The index of the table for which the column is associated.
@param hNameColumnText
    The name of the column.
*/
subroutine (PUBLIC::addMockColumn(tableIndex = i4, hNameColumnText = vc) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare columnSize = i4 with protect, noconstant(0)
    declare columnLocate = i4 with protect, noconstant(0)
    declare columnIndex = i4 with protect, noconstant(0)

    set columnSize = size(mockTableRec->tables[tableIndex].columns, 5)
    ; Check if the column has been added to the temporary mock table record structure, and if not, adds it.
    set columnLocate = locateval(columnIndex, 1, columnSize, hNameColumnText,
        mockTableRec->tables[tableIndex].columns[columnIndex].name)
    if (columnLocate = 0)
        set columnSize = columnSize + 1
        set cclStat = alterlist(mockTableRec->tables[tableIndex].columns, columnSize)
        set mockTableRec->tables[tableIndex].columns[columnSize].name = hNameColumnText
    endif
end ;PUBLIC::addMockColumn

/**
Handles mocking of column nodes by checking which table the supplied node references and adding it to the list of mock
columns.  Columns will be added to the corresponding table based on the table name or table alias.

@param hAttr
    The parent XML node to identify if a column is being leveraged.
*/
subroutine (PUBLIC::handleColumnNodeMocking(hAttr = h) = null with protect)
    declare hNameTableText = vc with protect, noconstant("")
    declare hNameColumnText = vc with protect, noconstant("")
    declare tableLocate = i4 with protect, noconstant(0)
    declare tableIndex = i4 with protect, noconstant(0)
    declare tableSize = i4 with protect, noconstant(0)
    declare tableAliasLocate = i4 with protect, noconstant(0)
    declare tableAliasIndex = i4 with protect, noconstant(0)
    declare tableAliasSize = i4 with protect, noconstant(0)

    ; The first name of hAttr is the table or alias and the second name is the column
    set hNameTableText = getNamedElementName(hAttr, 1)
    set hNameColumnText = getNamedElementName(hAttr, 2)

    ; If both the table/alias name and column name are present, add the column based on either the table name or alias
    ; name.
    if (cclutIsEmpty(hNameTableText) = FALSE and cclutIsEmpty(hNameColumnText) = FALSE)
        set tableSize = size(mockTableRec->tables, 5)
        set tableLocate = locateval(tableIndex, 1, tableSize, hNameTableText,
            mockTableRec->tables[tableIndex].name)
        if (tableLocate > 0)
            call addMockColumn(tableLocate, hNameColumnText)
        else
            for (tableIndex = 1 to tableSize)
                set tableAliasSize = size(mockTableRec->tables[tableIndex].aliases, 5)
                set tableAliasLocate = locateval(tableAliasIndex, 1, tableAliasSize, hNameTableText,
                    mockTableRec->tables[tableIndex].aliases[tableAliasIndex].alias)
                if (tableAliasLocate > 0)
                    call addMockColumn(tableIndex, hNameColumnText)
                    set tableIndex = tableSize
                endif
            endfor
        endif
    endif
end ;PUBLIC::handleColumnNodeMocking

/**
Checks each child XML node to see if it might possibly contain items to be mocked.  It then recurses over each child
node in order to walk the XML tree.

@param parentNode
    The parent XML node to search for mock items.
@param subroutineIndex
    The index of the source subroutine currently being evaluated.
*/
subroutine (PUBLIC::searchForMockableItems(parentNode = h, subroutineIndex = i4) = null with protect)
    declare childNodeCount = i4 with protect, noconstant(uar_xml_getchildcount(parentNode))
    declare childNodeIndex = i4 with protect, noconstant(0)
    declare childNode = h with protect, noconstant(0)
    declare childNodeName = vc with protect, noconstant("")

    ; Iterate over all child nodes.  Z_SELECT., Z_INSERT., Z_UPDATE., Z_DELETE., and Z_MERGE. are CRUD operations that
    ; utilize tables.  CALL. nodes indicate that a subroutine might be called.  Z_EXECUTE indicates an external script
    ; is being called.  ATTR. indicates that a table column might be called.
    for (childNodeIndex = 0 to childNodeCount - 1)
        set childNode = 0
        if (uar_xml_getchildnode(parentNode, childNodeIndex, childNode) = CCLUT_XML_SC_OK)
            set childNodeName = uar_xml_getnodename(childNode)
            if (childNodeName = "Z_SELECT." or childNodeName = "Z_INSERT." or childNodeName = "Z_UPDATE." or
                childNodeName = "Z_DELETE." or childNodeName = "Z_MERGE.")
                    call handleTableNodeMocking(childNode, subroutineIndex,
                        evaluate(uar_xml_getnodename(parentNode), "Z_MERGE.", 1, 0))
            else
                if (childNodeName = "CALL." or childNodeName = "Z_CALL.")
                    call handleSubroutineNodeMocking(childNode, subroutineIndex)
                elseif (childNodeName = "Z_EXECUTE.")
                    call handleScriptNodeMocking(childNode, subroutineIndex)
                elseif (childNodeName = "ATTR." and validate(mockTableRec))
                    call handleColumnNodeMocking(childNode)
                endif

                ; The reason that tables are separated from this recursion is because the table flow calls
                ; searchForMockableItems within handleTableNodeMocking because it needs to do some additional cleanup
                ; work after the call is made.
                call searchForMockableItems(childNode, subroutineIndex)
            endif
        endif
    endfor
end ;PUBLIC::searchForMockableItems

/**
Parses the XML tree for the program and searches for all subroutines that are public and should be included in the test
file.  Searches for any items that can be mocked for the subroutine.

@param programXML
    A string representation of the XML tree for the program.
*/
subroutine (PUBLIC::createMockList(programXML = vc) = null with protect)
    declare hXmlRoot = h with protect, noconstant(0)
    declare hXmlFile = h with protect, noconstant(0)
    declare hProgram = h with protect, noconstant(0)
    declare hSubroutine = h with protect, noconstant(0)
    declare subroutineCount = i4 with protect, noconstant(1)
    declare subroutineLocate = i4 with protect, noconstant(0)
    declare subroutineIndex = i4 with protect, noconstant(0)
    declare subroutineSize = i4 with protect, noconstant(size(templateRec->targetSubroutines, 5))
    declare subroutineName = vc with protect, noconstant("")

    ; Parse the program XML
    set hXmlRoot = cclut::parseXmlBuffer(programXML, hXmlFile)
    if (hXmlRoot = 0 or hXmlFile = 0)
        call cclexception(100, "E", concat("createMockList failed to parse program XML.  XML: ", programXML))
        call cclut::exitOnError("Create Mock List", scriptUnderTest, templateReply)
    endif

    set hProgram = cclut::getXmlListItemHandle(hXmlRoot, "ZC_PROGRAM.", 1)
    set hSubroutine = cclut::getXmlListItemHandle(hProgram, "SUBROUTINE.", subroutineCount)

    ; For every subroutine, check if it is public and check if it is in the overall record structure (excluded
    ; subroutines will not be present).
    while (hSubroutine != 0)
        set subroutineName = getPublicNamedElementName(hSubroutine)
        if (cclutIsEmpty(subroutineName) = FALSE)
            set subroutineLocate = locateVal(subroutineIndex, 1, subroutineSize, subroutineName,
                templateRec->targetSubroutines[subroutineIndex].name)
            if (subroutineLocate > 0)
                ; For each subroutine that is included in the test file, search for any items that can be mocked.
                call searchForMockableItems(hSubroutine, subroutineLocate)
            endif
        endif

        set subroutineCount = subroutineCount + 1
        set hSubroutine = cclut::getXmlListItemHandle(hProgram, "SUBROUTINE.", subroutineCount)
    endwhile

    call cclut::releaseXmlResources(hXmlFile)
end ;PUBLIC::createMockList

/**
Evaluates whether a given line is too long, and if so, breaks up the line.  The lines are added to the supplied record
structure.  If the line does not need to be broken, the single line is added to the supplied record structure.

@param line
    The line to be evaluated for breaking.
@param buffer
    The buffer record structure where the single line or broken lines are added.
*/
subroutine (PUBLIC::breakLine(line = vc, buffer = vc(ref)) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare tempLine = vc with protect, noconstant(line)
    declare tempLineLocation = i4 with protect, noconstant(1)
    set cclStat = alterlist(buffer->lines, tempLineLocation)

    ; Check if the line is too long for CCL
    if (textlen(tempLine) > LINE_MAX_LENGTH)
        ; So long as it is too long, keep breaking it with line continuation and add a new line to the testFile record
        ; structure
        while (textlen(tempLine) > LINE_MAX_LENGTH)
            set tempLineLocation = tempLineLocation + 1
            set cclStat = alterlist(buffer->lines, tempLineLocation)
            set buffer->lines[tempLineLocation - 1].line = concat(substring(1, (LINE_MAX_LENGTH - 1), tempLine), "\")
            set tempLine = substring(LINE_MAX_LENGTH, textlen(tempLine) - (LINE_MAX_LENGTH - 1), tempLine)
            set buffer->lines[tempLineLocation].line = tempLine
        endwhile
    else
        set buffer->lines[tempLineLocation].line = tempLine
    endif
end ;PUBLIC::breakLine

/**
Generates the lines in the test file for setup/tearDown subroutines.  The expected layout is as follows:

subroutine (setupOnce(null) = null)
    null
end
subroutine (setup(null) = null)
    null
end
subroutine (tearDown(null) = null)
    call cclutRemoveAllMocks(null)
    rollback
end
subroutine (tearDownOnce(null) = null)
    null
end
*/
subroutine (PUBLIC::generateSetupTearDownSubroutines(null) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare testFileSize = i4 with protect, noconstant(size(templateRec->testFile, 5))
    set testFileSize = testFileSize + 18
    set cclStat = alterlist(templateRec->testFile, testFileSize)

    ; Add setupOnce subroutine
    set templateRec->testFile[testFileSize - 17].line = "subroutine (setupOnce(null) = null)"
    set templateRec->testFile[testFileSize - 16].line = concat(TAB, "; Place any test case-level setup here")
    set templateRec->testFile[testFileSize - 15].line = concat(TAB, "null")
    set templateRec->testFile[testFileSize - 14].line = "end"

    ; Add setup and tearDown subroutines
    set templateRec->testFile[testFileSize - 13].line = "subroutine (setup(null) = null)"
    set templateRec->testFile[testFileSize - 12].line = concat(TAB, "; Place any test-level setup here")
    set templateRec->testFile[testFileSize - 11].line = concat(TAB, "null")
    set templateRec->testFile[testFileSize - 10].line = "end"
    set templateRec->testFile[testFileSize - 9].line = "subroutine (tearDown(null) = null)"
    set templateRec->testFile[testFileSize - 8].line = concat(TAB, "; Place any test-level teardown here")
    set templateRec->testFile[testFileSize - 7].line = concat(TAB, "call cclutRemoveAllMocks(null)")
    set templateRec->testFile[testFileSize - 6].line = concat(TAB, "rollback")
    set templateRec->testFile[testFileSize - 5].line = "end"

    ; Add tearDownOnce subroutine
    set templateRec->testFile[testFileSize - 4].line = "subroutine (tearDownOnce(null) = null)"
    set templateRec->testFile[testFileSize - 3].line = concat(TAB, "; Place any test case-level teardown here")
    set templateRec->testFile[testFileSize - 2].line = concat(TAB, "null")
    set templateRec->testFile[testFileSize - 1].line = "end"

    ; Add an empty line
    set templateRec->testFile[testFileSize].line = EMPTY_LINE
end ;PUBLIC::generateSetupTearDownSubroutines

/**
Generates the appropriate mock subroutines for each source subroutine using the testIdentifierNamespace.  Assuming a
subroutine called "ADDPERSON" calls a subroutine called "POPULATEREPLY", the following is the expected generated mock
subroutine:

subroutine test1ADDPERSON::POPULATEREPLY(null)
    null
end

@param testIdentifierNamespace
    The namespace that should be used for all the mock subroutines.
@param subroutineIndex
    The index of the source subroutine for which the mock subroutines will be generated.
*/
subroutine (PUBLIC::generateMockSubroutines(testIdentifierNamespace = vc,
    subroutineIndex = i4) = null with protect)
        declare cclStat = i4 with protect, noconstant(0)
        declare lineLocation = i4 with protect, noconstant(size(templateRec->testFile, 5) + 1)
        declare mockSubroutineSize = i4 with protect, noconstant(size(
            templateRec->targetSubroutines[subroutineIndex].mockSubroutines, 5))
        declare mockSubroutineIndex = i4 with protect, noconstant(0)

        ; Each mock subroutine should require four lines (assuming no comments)
        set cclStat = alterlist(templateRec->testFile, lineLocation + (4 * mockSubroutineSize))
        for (mockSubroutineIndex = 1 to mockSubroutineSize)
            if (templateRec->mockSubroutineCommentAdded = FALSE)
                set cclStat = alterlist(templateRec->testFile, size(templateRec->testFile, 5) + 2)
                set templateRec->testFile[lineLocation].line = SUBROUTINE_COMMENT_PART_1
                set templateRec->testFile[lineLocation + 1].line = SUBROUTINE_COMMENT_PART_2
                set lineLocation = lineLocation + 2
                set templateRec->mockSubroutineCommentAdded = TRUE
            endif
            ; All mock subroutines will be defaulted with null parameters and a null body.
            set templateRec->testFile[lineLocation].line = concat("subroutine (", testIdentifierNamespace, "::",
                templateRec->targetSubroutines[subroutineIndex].mockSubroutines[mockSubroutineIndex].name,
                "(null) = null)")
            set templateRec->testFile[lineLocation + 1].line = concat(TAB,
                "; TODO: delete line or add mock subroutine implementation")
            set templateRec->testFile[lineLocation + 2].line = concat(TAB, "null")
            set templateRec->testFile[lineLocation + 3].line = "end"

            set lineLocation = lineLocation + 4
        endfor

        set templateRec->testFile[lineLocation].line = EMPTY_LINE
end ;PUBLIC::generateMockSubroutines

/**
Generates the appropriate mock script calls using cclutAddMockImplementation adding TODO code comments to the first
instance.  Assuming a subroutine called "ADDPERSON" that executes a script called ccl_add_person, the following is the
expected generated mock:

call cclutAddMockImplementation("CCL_ADD_PERSON", "TODO: delete line or add mock script name")

@param subroutineIndex
    The index of the source subroutine for which the mock scripts will be generated.
*/
subroutine (PUBLIC::generateMockScriptCalls(subroutineIndex = i4) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare lineLocation = i4 with protect, noconstant(size(templateRec->testFile, 5) + 1)
    declare mockScriptSize = i4 with protect, noconstant(size(
        templateRec->targetSubroutines[subroutineIndex].mockScripts, 5))
    declare mockScriptIndex = i4 with protect, noconstant(0)

    ; Each mock script should require one line (assuming no comments)
    set cclStat = alterlist(templateRec->testFile, lineLocation - 1 + mockScriptSize)
    for (mockScriptIndex = 1 to mockScriptSize)
        if (templateRec->mockScriptCommentAdded = FALSE)
            set cclStat = alterlist(templateRec->testFile, size(templateRec->testFile, 5) + 2)
            set templateRec->testFile[lineLocation].line = SCRIPT_COMMENT_PART_1
            set templateRec->testFile[lineLocation + 1].line = SCRIPT_COMMENT_PART_2
            set lineLocation = lineLocation + 2
            set templateRec->mockScriptCommentAdded = TRUE
        endif
        ; All mock scripts will include a TODO: for the user to add an appropriate mock implementation for their use
        ; case.
        set templateRec->testFile[lineLocation].line = concat(TAB, 'call cclutAddMockImplementation("',
            templateRec->targetSubroutines[subroutineIndex].mockScripts[mockScriptIndex].name,
            '", "TODO: delete line or add mock script name")')
        set lineLocation = lineLocation + 1
    endfor
    ; If there are any mock scripts, throw in an empty line after all of them for readability.
    if (mockScriptSize > 0)
        set cclStat = alterlist(templateRec->testFile, lineLocation)
        set templateRec->testFile[lineLocation].line = EMPTY_LINE
    endif
end ;PUBLIC::generateMockScriptCalls

/**
Generates the appropriate mock tables using cclutDefineMockTable adding TODO code comments to the first instance.
Assuming a subroutine called "ADDPERSON" that performs an insert to the person table with values for person_id,
name_last, name_first, and birth_dt_tm, the following is the expected generated mock:

call cclutDefineMockTable("PERSON", "PERSON_ID|NAME_LAST|NAME_FIRST|BIRTH_DT_TM", "TODO: delete line or add parameter t\
ypes for mock table columns")
call cclutCreateMockTable("PERSON")
call cclutAddMockData("PERSON", "TODO: delete line or add mock data")

@param subroutineIndex
    The index of the source subroutine for which the mock tables will be generated.
*/
subroutine (PUBLIC::generateMockTableCalls(subroutineIndex = i4) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare lineLocation = i4 with protect, noconstant(size(templateRec->testFile, 5) + 1)
    declare mockTableSize = i4 with protect, noconstant(size(
        templateRec->targetSubroutines[subroutineIndex].mockTables, 5))
    declare mockTableIndex = i4 with protect, noconstant(0)
    declare mockColumnSize = i4 with protect, noconstant(0)
    declare mockColumnIndex = i4 with protect, noconstant(0)
    declare columnDeclaration = vc with protect, noconstant("")

    ; Each mock table should require three lines (assuming no comments)
    set cclStat = alterlist(templateRec->testFile, (lineLocation - 1) + (3 * mockTableSize))
    for (mockTableIndex = 1 to mockTableSize)
        set mockColumnSize = size(templateRec->targetSubroutines[subroutineIndex].mockTables[mockTableIndex].columns, 5)
        if (mockColumnSize > 0)
            ; Add all the necessary columns for cclutDefineMockTable
            set columnDeclaration =
                templateRec->targetSubroutines[subroutineIndex].mockTables[mockTableIndex].columns[1].name
            for (mockColumnIndex = 2 to mockColumnSize)
                set columnDeclaration = concat(columnDeclaration, "|",
                    templateRec->targetSubroutines[subroutineIndex].mockTables[mockTableIndex].columns[mockColumnIndex].
                        name)
            endfor
        else
            set columnDeclaration = trim("", 3)
        endif

        if (templateRec->mockTableCommentAdded = FALSE)
            set cclStat = alterlist(templateRec->testFile, size(templateRec->testFile, 5) + 2)
            set templateRec->testFile[lineLocation].line = TABLE_COMMENT_PART_1
            set templateRec->testFile[lineLocation + 1].line = TABLE_COMMENT_PART_2
            set lineLocation = lineLocation + 2
            set templateRec->mockTableCommentAdded = TRUE
        endif
        set templateRec->testFile[lineLocation].line = concat(TAB, 'call cclutDefineMockTable("',
            templateRec->targetSubroutines[subroutineIndex].mockTables[mockTableIndex].name, '", "', columnDeclaration,
            '", "TODO: delete line or add parameter types for mock table columns")')
        set templateRec->testFile[lineLocation + 1].line = concat(TAB, 'call cclutCreateMockTable("',
            templateRec->targetSubroutines[subroutineIndex].mockTables[mockTableIndex].name, '")')
        set lineLocation = lineLocation + 2
        if (templateRec->mockDataCommentAdded = FALSE)
            set cclStat = alterlist(templateRec->testFile, size(templateRec->testFile, 5) + 2)
            set templateRec->testFile[lineLocation].line = DATA_COMMENT_PART_1
            set templateRec->testFile[lineLocation + 1].line = DATA_COMMENT_PART_2
            set lineLocation = lineLocation + 2
            set templateRec->mockDataCommentAdded = TRUE
        endif
        set templateRec->testFile[lineLocation].line = concat(TAB, 'call cclutAddMockData("',
            templateRec->targetSubroutines[subroutineIndex].mockTables[mockTableIndex].name,
            '", "TODO: delete line or add mock data")')
        set lineLocation = lineLocation + 1
    endfor
    if (mockTableSize > 0)
        ; If there are any mock tables, throw in an empty line after all of them for readability.
        set cclStat = alterlist(templateRec->testFile, lineLocation)
        set templateRec->testFile[lineLocation].line = EMPTY_LINE
    endif
end ;PUBLIC::generateMockTableCalls

/**
Generates the appropriate mock subroutines, scripts, and tables along with a mock main (for non-main subroutines) and a
failing assertion to be updated by the user.

@param scriptName
    The name of the script under test.
*/
subroutine (PUBLIC::generateTestCaseData(scriptName = vc) = null with protect)
    ; Generate any setup/tearDown subroutines
    call generateSetupTearDownSubroutines(null)

    declare cclStat = i4 with protect, noconstant(0)
    declare subroutineSize = i4 with protect, noconstant(size(templateRec->targetSubroutines, 5))
    declare subroutineIndex = i4 with protect, noconstant(0)
    declare subroutineName = vc with protect, noconstant("")
    declare testIdentifier = vc with protect, noconstant("")
    declare testIdentifierNamespace = vc with protect, noconstant("")
    declare lineLocation = i4 with protect, noconstant(0)

    for (subroutineIndex = 1 to subroutineSize)
        ; For each subroutine, get the name and create a test identifier and unique namespace for mock subroutines.
        set subroutineName = templateRec->targetSubroutines[subroutineIndex].name
        set testIdentifier = concat(TEST_PREFIX, subroutineName)
        set testIdentifierNamespace = substring(1, 40, concat(TEST_IDENTIFIER_PREFIX,
            evaluate(templateRec->targetSubroutines[subroutineIndex].namespaceForMocksIdentifier, 0, trim(""),
                trim(cnvtstring(templateRec->targetSubroutines[subroutineIndex].namespaceForMocksIdentifier, 11), 3)),
            templateRec->targetSubroutines[subroutineIndex].namespaceForMocks))

        set lineLocation = size(templateRec->testFile, 5) + 1
        set cclStat = alterlist(templateRec->testFile, lineLocation)
        set templateRec->testFile[lineLocation].line = concat("subroutine ", testIdentifier, "(null)")
        if (subroutineName != MAIN_SUBROUTINE)
            set cclStat = alterlist(templateRec->testFile, lineLocation + 2)
            set templateRec->testFile[lineLocation + 1].line =
                concat(TAB, "declare ", testIdentifier, "__MainWasCalled = i2 with protect, noconstant(FALSE)")
            set templateRec->testFile[lineLocation + 2].line = EMPTY_LINE
        endif

        ; Generate any mock scripts
        call generateMockScriptCalls(subroutineIndex)
        ; Generate any mock tables
        call generateMockTableCalls(subroutineIndex)

        ; Generate the cclutExecuteProgramWithMocks line
        set lineLocation = size(templateRec->testFile, 5) + 1
        set cclStat = alterlist(templateRec->testFile, lineLocation + 3)
        if (subroutineName != MAIN_SUBROUTINE)
            set cclStat = alterlist(templateRec->testFile, size(templateRec->testFile, 5) + 1)
            set templateRec->testFile[lineLocation].line = concat(TAB, 'call cclutAddMockImplementation("',
                MAIN_SUBROUTINE, '", "', MAIN_SUBROUTINE, testIdentifier, '")')
            set lineLocation = lineLocation + 1
        endif
        set templateRec->testFile[lineLocation].line =
            concat(TAB, 'call cclutExecuteProgramWithMocks("', scriptName, '", "", "', testIdentifierNamespace, '")')
        set templateRec->testFile[lineLocation + 1].line = EMPTY_LINE
        set lineLocation = lineLocation + 2
        if (subroutineName != MAIN_SUBROUTINE)
            ; Assert that the mock main was called if the subroutine under test is not main
            set cclStat = alterlist(templateRec->testFile, size(templateRec->testFile, 5) + 1)
            set templateRec->testFile[lineLocation].line = concat(TAB, 'call cclutAsserti2Equal(CURREF, "',
                testIdentifier, ' 001", ', testIdentifier, "__MainWasCalled, TRUE)")
            set lineLocation = lineLocation + 1
        endif
        ; Add a failing assertion for all tests so that the user can update as appropriate.
        set templateRec->testFile[lineLocation].line = concat(TAB, 'call cclutAsserti2Equal(CURREF, "', testIdentifier,
            ' auto-generate test.  Fail by default.", TRUE, FALSE)')
        set templateRec->testFile[lineLocation + 1].line = "end"
        set lineLocation = lineLocation + 2

        if (subroutineName != MAIN_SUBROUTINE)
            ; If the subroutine is not the main subroutine, generate a mock main that will call the subroutine under
            ; test.  A variable will also be created to indicate that the correct main was called and asserted later in
            ; the test.
            set cclStat = alterlist(templateRec->testFile, lineLocation + 3)
            set templateRec->testFile[lineLocation].line =
                concat("subroutine ", MAIN_SUBROUTINE, testIdentifier, "(null)")
            set templateRec->testFile[lineLocation + 1].line =
                concat(TAB, "call ", templateRec->targetSubroutines[subroutineIndex].name, "(null)")
            set templateRec->testFile[lineLocation + 2].line =
                concat(TAB, "set ", testIdentifier, "__MainWasCalled = TRUE")
            set templateRec->testFile[lineLocation + 3].line = "end"
        endif

        ; Generate any mock subroutines
        call generateMockSubroutines(testIdentifierNamespace, subroutineIndex)
    endfor
end ;PUBLIC::generateTestCaseData

/**
Iterates over all the lines for the test file and writes them out to the destination specified in the outputDestination
parameter.

@param outputDestination
    The destination to which the test file should be written.
*/
subroutine (PUBLIC::generateFinalOutput(outputDestination = vc) = null with protect)
    declare cclStat = i4 with protect, noconstant(0)
    declare bufferIndex = i4 with protect, noconstant(0)
    declare warningIndex = i4 with protect, noconstant(0)
    record buffer (
        1 lines[*]
            2 line = vc
    ) with protect

    select into value(outputDestination)
    from (dummyt d1 with seq = size(templateRec->testFile, 5))
    plan d1
    head report
        ; Place all the warnings at the top of the file in a block-level comment.
        value = fillstring(132, " ")
        warningSize = size(templateRec->warnings, 5)
        if (warningSize > 0)
            col 0 "/* WARNINGS" row+1
            for (warningIndex = 1 to warningSize)
                cclstat = initrec(buffer)
                call breakLine(templateRec->warnings[warningIndex].warning, buffer)
                for (bufferIndex = 1 to size(buffer->lines, 5))
                    value = buffer->lines[bufferIndex].line
                    col 0 value row+1
                endfor
            endfor
            col 0 "*/" row+1
        endif
    detail
        cclStat = initrec(buffer)
        value = fillstring(132, " ")
        call breakLine(templateRec->testFile[d1.seq].line, buffer)
        for (bufferIndex = 1 to size(buffer->lines, 5))
            value = buffer->lines[bufferIndex].line
            col 0 value row+1
        endfor
    with nocounter, maxrow = 1, maxcol = 133, compress, nullreport, noheading, format = variable, formfeed = none
end ;PUBLIC::generateFinalOutput

/**
Validates that the script has a main subroutine, and if so, generates the test case data and generates the final output
to the output destination.  If no main subroutine is identified, log an error and exit.

@param outputDestination
    The destination to which the test file should be written.
@param scriptName
    The name of the script under test.
*/
subroutine (PUBLIC::generateTestCase(outputDestination = vc, scriptName = vc) = null with protect)
    declare subroutineLocate = i4 with protect, noconstant(0)
    declare subroutineIndex = i4 with protect, noconstant(0)

    set subroutineLocate = locateval(subroutineIndex, 1, size(templateRec->targetSubroutines, 5), MAIN_SUBROUTINE,
        templateRec->targetSubroutines[subroutineIndex].name)
    if (subroutineLocate > 0)
        ; Main subroutine found.  Generate test file based on it.
        call generateTestCaseData(scriptName)
        call generateFinalOutput(outputDestination)
    else
        ; Main subroutine not found.
        call cclexception(100, "E", "No main subroutine found in program.  A main subroutine must be present.")
        call cclut::exitOnError("Generate test case", scriptName, templateReply)
    endif
end ;PUBLIC::generateTestCase

/**
If the script has made it to this point without erroring (most of the expected errors will short-circuit this), then
change the status to S.
*/
subroutine (PUBLIC::addSuccessToReply(null) = null with protect)
    set templateReply->status_data.status = "S"
end ;PUBLIC::addSuccessToReply

/**
The main subroutine for this program.  Validates the parameters, creates the XML for the script under test, finds the
program's subroutines, excludes any subroutines that should not be tested, creates a list of mock items, generates the
test file, and successfully exits.
*/
subroutine (PUBLIC::main(null) = null with protect)
    call validateParameters(scriptUnderTest, includeFiles, sourceFileLocation)
    call createProgramXML(scriptUnderTest)
    call identifySubroutinesToTest(templateRec->programXML, sourceFileLocation, scriptUnderTest)
    call createMockList(templateRec->programXML)
    call generateTestCase(outputDestination, scriptUnderTest)
    call addSuccessToReply(null)
end ;PUBLIC::main

call main(null)

#exit_script
call echorecord(templateReply) ;intentional
declare messageIndex = i4 with protect, noconstant(0)
for (messageIndex = 1 to size(templateRec->messages, 5))
    call echo(templateRec->messages[messageIndex].message) ;intentional
endfor
declare warningIndex = i4 with protect, noconstant(0)
for (warningIndex = 1 to size(templateRec->warnings, 5))
    call echo(templateRec->warnings[warningIndex].warning) ;intentional
endfor

end go
