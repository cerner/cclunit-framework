drop program cclut:dba go
create program cclut:dba
 
/**
  A prompt program for executing a specified test case with the CCL Unit framework and reporting the results.
  @arg (vc) The output destination for the results: stdout (MINE), a file, or a print queue.
    @default MINE
  @arg (vc) A (case insensitive) logical for the directory where the test case file is located.
    @default CCLUSERDIR
  @arg (vc) The name of the test case file with or without the .inc extension. 
  @arg (vc) A regular expression pattern for limiting, by name, which tests within the test case get executed.
    @default .*
  @arg The (case insensitive) optimizer mode (CBO, RBO) to set for the current session before runnning the tests. 
  The session optimizer mode is left unchanged if any other value is provided. 
    @default current
  @arg (vc) The deprecated severity to apply.
    @default E
  @arg (i2) A boolean flag indicating whether to fail fast.
    @default FALSE
*/
prompt 
  "Ouput Destination [MINE]: " = "MINE",
  "Test Case Directory [cclsource]: " = "cclsource",
  "Test Case File Name: " = "",
  "Test Name Pattern [.*]: " = ".*",
  "Optimizer Mode (CBO, RBO) [current]: " = "",
  "Deprecated Flag (E, W, L, I, D) [E]: " = "",
  "FailFast [FALSE]: " = FALSE
with outputDestination, testCaseDirectory,
    testCaseFileName, testNamePattern, optimizerMode, deprecatedFlag, failFast

declare cclut1::main(null) = null with protect
declare cclut1::errorCheck(null) = null with protect
declare cclut1::checkParameters(null) = null with protect
declare cclut1::executeTestCase(null) = null with protect
declare cclut1::processTestCaseResponse(cclutDestination = vc, cclutReq = vc(ref), 
    cclutResp = vc(ref), cclutRawResults = vc(ref), cclutResults = vc(ref)) = null with protect
declare cclut1::transferTestCaseResults(cclutSource = vc(ref), cclutTarget = vc(ref)) = null with protect
declare cclut1::generateResultsReport(cclutDestination = vc, cclutReq = vc(ref), cclutResults = vc(ref)) = i2 with protect
declare cclut1::generateErrorReport(cclutDestination = vc, testCaseFileName = vc, cclutErrorCode = vc) = i2 with protect


declare cclut1::RESULT_STATUS_PASSED = vc with protect, constant("PASSED")
declare cclut1::RESULT_STATUS_FAILED = vc with protect, constant("FAILED")
declare cclut1::RESULT_STATUS_ERRORED = vc with protect, constant("ERRORED")
  
declare cclut1::outputDestination = vc with protect, noconstant(trim($outputDestination, 3))
declare cclut1::testCaseDirectory = vc with protect, noconstant(trim($testCaseDirectory, 3))
declare cclut1::testCaseFileName = vc with protect, noconstant(trim($testCaseFileName, 3))
declare cclut1::testNamePattern = vc with protect, noconstant(trim($testNamePattern, 3))
declare cclut1::optimizerMode = vc with protect, noconstant(trim(cnvtupper($optimizerMode), 3))
declare cclut1::deprecatedFlag = vc with protect, noconstant(trim($deprecatedFlag, 3))
declare cclut1::failFast = i2 with protect, noconstant($failFast)


;allow the calling program to supply this
if (validate(cclut1TestCaseRequest) = FALSE)
  record cclut1TestCaseRequest (
    1 testCaseDirectory = vc
    1 testCaseFileName = vc
    1 testNamePattern = vc
    1 programs[*]
      2 programName = vc
      2 compile = i2
    1 optimizerMode = vc
    1 deprecatedFlag = vc
    1 failFast = i2
  ) with protect
endif

;allow the calling program to supply this
if (validate(cclut1TestCaseReply) = FALSE)
  record cclut1TestCaseReply (
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
endif

;allow the calling program to supply this
if (validate(cclut1TestCaseResults) = FALSE)
  record cclut1TestCaseResults (
    1 resultInd = i2
    1 tests[*]
      2 name = vc
      2 asserts[*]
        3 lineNumber = i4
        3 context = vc
        3 resultInd = i2
        3 condition = vc
      2 errors[*]
        3 lineNumber = i4
        3 errorText = vc
%i cclsource:status_block.inc
  ) with protect
endif


subroutine cclut1::main(null)
  
  record cclut1TestResults (
    1 testCaseFileName = vc
    1 result = vc
    1 passedCount = i4
    1 failedCount = i4
    1 erroredCount = i4
    1 test[*]
      2 statusFlag = i2 ;0=passed, 1=failed, 2=errored
      2 testName = vc
      2 result = vc
      2 assert[*]
        3 context = vc
        3 condition = vc
        3 result = vc
      2 error[*]
        3 text = vc
  ) with protect

  if (cclut1::optimizerMode not in ("CBO", "RBO"))
    set cclut1::optimizerMode = ""
  endif
  call cclut1::checkParameters(null)

  set cclut1TestCaseRequest->testCaseDirectory = cclut1::testCaseDirectory
  set cclut1TestCaseRequest->testCaseFileName = cclut1::testCaseFileName
  set cclut1TestCaseRequest->testNamePattern = cclut1::testNamePattern
  set cclut1TestCaseRequest->optimizerMode = cclut1::optimizerMode
  set cclut1TestCaseRequest->deprecatedFlag = cclut1::deprecatedFlag
  set cclut1TestCaseRequest->failFast = cclut1::failFast

  call cclut1::executeTestCase(null)

  call cclut1::processTestCaseResponse(
      cclut1::outputDestination, cclut1TestCaseRequest, cclut1TestCaseReply, cclut1TestCaseResults, cclut1TestResults)
  call cclut1::errorCheck(null)
end ;;;main

/**
  Checks for CCL errors. Exits the script with a failed status if any has occurred.
*/
subroutine cclut1::errorCheck(null)
  declare cclutErrorCode = i4 with protect, noconstant(0)
  declare cclutErrorMessage = vc with protect, noconstant("")
  set cclutErrorCode = error(cclutErrorMessage, 1)
  if (cclutErrorCode > 0 and cclut1TestCaseReply->status_data.status != "F")
    set cclut1TestCaseReply->status_data.status = "F"
    set cclut1TestCaseReply->status_data.subeventstatus[1].targetObjectValue = cclutErrorMessage
    go to exit_script
  endif      
end ;;;errorCheck

/**
  Validates that required parameters have been populated. Routes control to exit_script if not.
*/
subroutine cclut1::checkParameters(null)  
  if (textlen(trim(cclut1::outputDestination, 3)) = 0)
;;;CCLUNIT:OFF  
    set cclut1::outputDestination = "MINE"
;;;CCLUNIT:ON
  endif
  
  if (textlen(trim(cclut1::testCaseDirectory, 3)) = 0)
    set cclut1::testCaseDirectory = "cclsource"
  endif
   
  if (textlen(cclut1::testCaseFileName) = 0)
    set cclut1TestCaseReply->status_data.status = "F"
    set cclut1TestCaseReply->status_data.subeventstatus[1].targetObjectValue = "A test case file name is required"
    go to exit_script
  endif
end ;;;checkParameters

/**
  Invokes the CCL Unit framework script cclut_execute_test_case_file to execute the test case.
*/ 
subroutine cclut1::executeTestCase(null)
  ;set the codecover trace here because CCL will error when the framework sets it if the session or top-level script did not.
  if (curimage = "CCL")
    set trace codecover 0; summary
  endif
  call cclut1::errorCheck(null)
  execute cclut_execute_test_case_file with 
    replace("CCLUTREQUEST", cclut1TestCaseRequest), 
    replace("CCLUTREPLY", cclut1TestCaseReply),
    replace("CCLUTTESTCASERESULTS", cclut1TestCaseResults)
end ;;;executeTestCase


/**
  Transfers test case results from a cclut1TestCaseResults source to a cclut1TestResults target
  @param cclutSource
    The cclut1TestCaseResults strucutre containing the raw results.
  @param cclutTarget
    The cclut1TestResults into which the results will be stored.
*/
subroutine cclut1::transferTestCaseResults(cclutSource, cclutTarget)
  declare cclutStat = i4 with protect, noconstant(0)
  declare cclutTestIndex = i4 with protect, noconstant(0)
  declare cclutTestCout = i4 with protect, noconstant(size(cclutSource->tests, 5))
  declare cclutAssertIndex = i4 with protect, noconstant(0)
  declare cclutAssertCount = i4 with protect, noconstant(0)
  declare cclutAssertFailureCount = i4 with protect, noconstant(0)
  declare cclutErrorIndex = i4 with protect, noconstant(0)
  declare cclutErrorCount = i4 with protect, noconstant(0)
  declare cclutResult = vc with protect, noconstant("")

  set cclutStat = alterlist(cclutTarget->test, cclutTestCout)
  for (cclutTestIndex = 1 to cclutTestCout)
    set cclutResult = cclut1::RESULT_STATUS_PASSED
    set cclutTarget->test[cclutTestIndex].testName = cclutSource->tests[cclutTestIndex].name

    set cclutAssertFailureCount = 0
    set cclutAssertCount = size(cclutSource->tests[cclutTestIndex].asserts, 5)
    set cclutStat = alterlist(cclutTarget->test[cclutTestIndex].assert, cclutAssertCount)
    for (cclutAssertIndex = 1 to cclutAssertCount)
      if (cclutSource->tests[cclutTestIndex].asserts[cclutAssertIndex].resultInd = FALSE)
        set cclutAssertFailureCount = cclutAssertFailureCount + 1
        set cclutTarget->test[cclutTestIndex].assert[cclutAssertFailureCount].context = 
            cclutSource->tests[cclutTestIndex].asserts[cclutAssertIndex].context 
        set cclutTarget->test[cclutTestIndex].assert[cclutAssertFailureCount].condition = 
            cclutSource->tests[cclutTestIndex].asserts[cclutAssertIndex].condition
        set cclutTarget->test[cclutTestIndex].assert[cclutAssertFailureCount].result = cclut1::RESULT_STATUS_FAILED
        set cclutResult = cclut1::RESULT_STATUS_FAILED
      endif
    endfor
    set cclutStat = alterlist(cclutTarget->test[cclutTestIndex].assert, cclutAssertFailureCount)

    set cclutErrorCount = size(cclutSource->tests[cclutTestIndex].errors, 5)
    set cclutStat = alterlist(cclutTarget->test[cclutTestIndex].error, cclutErrorCount)
    for (cclutErrorIndex = 1 to cclutErrorCount)
      set cclutTarget->test[cclutTestIndex].error[cclutErrorIndex].text = 
          cclutSource->tests[cclutTestIndex].errors[cclutErrorIndex].errorText 
      set cclutResult = cclut1::RESULT_STATUS_ERRORED
    endfor

    set cclutTarget->test[cclutTestIndex].result = cclutResult
    case (cclutResult)
      of cclut1::RESULT_STATUS_PASSED:
        set cclutTarget->passedCount = cclutTarget->passedCount + 1
      of cclut1::RESULT_STATUS_FAILED:
        set cclutTarget->test[cclutTestIndex].statusFlag = 1
        set cclutTarget->failedCount = cclutTarget->failedCount + 1
      of cclut1::RESULT_STATUS_ERRORED:
        set cclutTarget->test[cclutTestIndex].statusFlag = 2
        set cclutTarget->erroredCount = cclutTarget->erroredCount + 1
    endcase
  endfor
end ;;;transferTestCaseResults


/**
  Processes the response from a call to cclut_execute_test_case_file and stores the results in a provided buffer.
  @param cclutDestination
    The destination for the generated output.
  @param cclutReq
    The cclut_execute_test_case_file request which generated the response.
  @param cclutResp
    The cclut_execute_test_case_file reponse to process.
  @param cclutRawResults
    A cclut1TestCaseResults strucutre containing raw results.
  @param cclutResults
    A test results structure into which the test results will be stored.
*/
subroutine cclut1::processTestCaseResponse(cclutDestination, cclutReq, cclutResp, cclutRawResults, cclutResults)
  if (cclutResp->status_data.status = "F")
    set cclut1TestCaseReply->status_data.status = "F"
    set cclut1TestCaseReply->status_data.subeventstatus[1].targetObjectValue = 
        concat("Execution Failed: ", cclutResp->status_data.subeventstatus[1].targetobjectvalue)
    go to exit_script
  endif
  call cclut1::transferTestCaseResults(cclutRawResults, cclutResults)
  call cclut1::errorCheck(null)
  if (cclut1::generateResultsReport(cclutDestination, cclutReq, cclutResults) = TRUE)
    set cclut1TestCaseReply->status_data.status = "S"
    set cclut1TestCaseReply->status_data.subeventstatus[1].targetObjectValue = "Report Generated Successfully"
  else
    call cclut1::errorCheck(null)
    set cclut1TestCaseReply->status_data.status = "F"
    set cclut1TestCaseReply->status_data.subeventstatus[1].targetObjectValue = "No tests qualified to be executed"
  endif
end ;;;collectResults


/**
  Generates a report for the results of executing a test case file.
  @param cclutDestination
    The destination for the report: stdout, a file or a print queue.
  @param cclutReq
    The cclut_execute_test_case_file request used to generate the results.
  @param cclutResults
    A record containing the processed results of the test case file execution.
*/
subroutine cclut1::generateResultsReport(cclutDestination, cclutReq, cclutResults)
  declare CCLUT_OPTIMIZER_MODE = vc with protect, constant(cclutReq->optimizerMode)
  declare CCLUT_LINE_OF_EQUALS = vc with protect, constant(fillstring(120, "="))
  declare CCLUT_LINE_OF_ASTERISKS = vc with protect, constant(fillstring(120, "*"))
  declare CCLUT_LINE_OF_HYPHENS = vc with protect, constant(fillstring(120, "-"))
  declare CCLUT_LINE_WIDTH = i4 with protect, constant(120)
  declare CCLUT_INDENTED_LINE_WIDTH = i4 with protect, constant(CCLUT_LINE_WIDTH - 8) ;4 character margins

  declare cclutOutputData = vc with protect, noconstant("")
  declare cclutOutputLine = vc with protect, noconstant("")
  declare cclutDataPosition = i4 with protect, noconstant(0)
  declare cclutDataLength = i4 with protect, noconstant(0)
  declare cclutAssertIndex = i4 with protect, noconstant(0)
  declare cclutErrorIndex = i4 with protect, noconstant(0)

  if (size(cclutResults->test, 5) > 0)
    select into value(cclutDestination) statusFlag = cclutResults->test[d.seq].statusFlag
    from (dummyt d with seq = size(cclutResults->test, 5))
    order by statusFlag desc
    head report
      col 0 CCLUT_LINE_OF_EQUALS row+1

      if (CCLUT_OPTIMIZER_MODE in ("CBO", "RBO"))
        cclutOutputLine = concat("CCL Unit Test Report - ", CCLUT_OPTIMIZER_MODE) ;26 characters
        col 47 cclutOutputLine ;(CCLUT_LINE_WIDTH - 26)/2
      else
        cclutOutputLine = "CCL Unit Test Report" ;20 characters
        col 50 cclutOutputLine ;(CCLUT_LINE_WIDTH - 20)/2
      endif

      cclutOutputLine = format(cnvtdatetime(curdate,curtime3), ";;Q") ;23 characters
      col 97 cclutOutputLine row+2 ;CCLUT_LINE_WIDTH - 23
 
      cclutOutputLine = concat("Test Case: ", cclutReq->testCaseDirectory, ":", cclutReq->testCaseFileName)
      col 0 cclutOutputLine row+1
 
      if (cclutResults->passedCount > 0)
        cclutOutputLine = concat("PASSED: ", trim(cnvtstring(cclutResults->passedCount, 100)))
      else
        cclutOutputLine = concat("passed: ", trim(cnvtstring(cclutResults->passedCount, 100)))
      endif
      col 0 cclutOutputLine
 
      if (cclutResults->failedCount > 0)
        cclutOutputLine = concat("FAILED: ", trim(cnvtstring(cclutResults->failedCount, 100)))
      else
        cclutOutputLine = concat("failed: ", trim(cnvtstring(cclutResults->failedCount, 100)))
      endif
      col 20 cclutOutputLine
 
      if (cclutResults->erroredCount > 0)
        cclutOutputLine = concat("ERRORED: ", trim(cnvtstring(cclutResults->erroredCount, 100)))
      else
        cclutOutputLine = concat("errored: ", trim(cnvtstring(cclutResults->erroredCount, 100)))
      endif
      col 40 cclutOutputLine row+1 
      row+1
    head statusFlag
      case (statusFlag)
        of 0:
          cclutOutputLine = "Passed Tests"
        of 1: 
          cclutOutputLine = "Failed Tests"
        of 2:
          cclutOutputLine = "Errored Tests"
      endcase
      row+1
      col 0 cclutOutputLine row+1
      col 0 CCLUT_LINE_OF_EQUALS row+1
    detail
      cclutOutputLine = cclutResults->test[d.seq].testName
      col 0 cclutOutputLine row+1
       
      ;output the errors
      for (cclutErrorIndex = 1 to size(cclutResults->test[d.seq].error, 5))
        if (cclutErrorIndex = 1)
          col 0 CCLUT_LINE_OF_HYPHENS row+1
        endif

        cclutOutputData = concat("Error: ", trim(cclutResults->test[d.seq].error[cclutErrorIndex].text, 3))
        cclutDataLength = textlen(cclutOutputData)
        cclutDataPosition = 1
        while (cclutDataPosition <= cclutDataLength)
          cclutOutputLine = trim(substring(cclutDataPosition, CCLUT_INDENTED_LINE_WIDTH, cclutOutputData), 2)
          cclutDataPosition = cclutDataPosition + CCLUT_INDENTED_LINE_WIDTH
          col 4 cclutOutputLine row+1
        endwhile

        if (cclutErrorIndex != size(cclutResults->test[d.seq].error, 5))
          col 0 CCLUT_LINE_OF_HYPHENS row+1
        endif
      endfor
 
      for (cclutAssertIndex = 1 to size(cclutResults->test[d.seq].assert, 5))
        if (cclutAssertIndex = 1)
          col 0 CCLUT_LINE_OF_HYPHENS row+1
        endif

        cclutOutputData = concat("Assert Failure: ", trim(cclutResults->test[d.seq].assert[cclutAssertIndex].condition, 3))
        cclutDataLength = textlen(cclutOutputData)
        cclutDataPosition = 1
        while (cclutDataPosition <= cclutDataLength)
          cclutOutputLine = trim(substring(cclutDataPosition, CCLUT_INDENTED_LINE_WIDTH, cclutOutputData), 2)
          cclutDataPosition = cclutDataPosition + CCLUT_INDENTED_LINE_WIDTH
          col 4 cclutOutputLine row+1
        endwhile
        cclutOutputData = trim(cclutResults->test[d.seq].assert[cclutAssertIndex].context, 3)
        cclutDataLength = textlen(cclutOutputData)
        cclutDataPosition = 1
        while (cclutDataPosition <= cclutDataLength)
          cclutOutputLine = trim(substring(cclutDataPosition, CCLUT_INDENTED_LINE_WIDTH, cclutOutputData), 2)
          cclutDataPosition = cclutDataPosition + CCLUT_INDENTED_LINE_WIDTH
          col 4 cclutOutputLine row+1
        endwhile

        if (cclutAssertIndex != size(cclutResults->test[d.seq].assert, 5))
          col 0 CCLUT_LINE_OF_HYPHENS row+1
        endif
      endfor
      if (statusFlag > 0)
        row+1
      endif
    foot report
      col 0 CCLUT_LINE_OF_ASTERISKS row+1
    with nocounter, maxrow = 3, maxcol = 200, formfeed=none
    return (TRUE)
  endif
  return (FALSE)
end ;;;generateResultsReport
 
 
/**
  Generates an error report.
  @param cclutDestination
    The destination for the generated output.
  @param testCaseFileName
    The name of the test case file that was executed.
  @param cclutErrorCode
    The error message to display on the report.
    
*/
subroutine cclut1::generateErrorReport(cclutDestination, testCaseFileName, cclutErrorCode) 
  declare CCLUT_LINE_OF_EQUALS = vc with protect, constant(fillstring(120, "="))
  declare CCLUT_LINE_OF_ASTERISKS = vc with protect, constant(fillstring(120, "*"))
  declare CCLUT_LINE_WIDTH = i4 with protect, constant(120)
  declare CCLUT_INDENTED_LINE_WIDTH = i4 with protect, constant(CCLUT_LINE_WIDTH - 8) ;4 character margins

  declare cclutOutputData = vc with protect, noconstant("")
  declare cclutOutputLine = vc with protect, noconstant("")
  declare cclutDataPosition = i4 with protect, noconstant(0)
  declare cclutDataLength = i4 with protect, noconstant(0)

  select into value(cclutDestination)
  from (dummyt d with seq = 1)
  head report
      col 0 CCLUT_LINE_OF_EQUALS row+1
 
      cclutOutputLine = "CCL Unit Test Error Report" ;20 characters
      col 47 cclutOutputLine ;(CCLUT_LINE_WIDTH - 26)/2

      cclutOutputLine = format(cnvtdatetime(curdate,curtime3), ";;Q") ;23 characters
      col 97 cclutOutputLine row+2 ;CCLUT_LINE_WIDTH - 23
 
      cclutOutputLine = concat("Test Case: ", testCaseFileName)
      col 0 cclutOutputLine row+1
 
      col 0 CCLUT_LINE_OF_EQUALS row+1
  detail
  
      cclutOutputData = trim(cclutErrorCode)
      cclutDataLength = textlen(cclutOutputData)
      cclutDataPosition = 1
      while (cclutDataPosition <= cclutDataLength)
        cclutOutputLine = notrim(substring(cclutDataPosition, CCLUT_INDENTED_LINE_WIDTH, cclutOutputData))
        cclutDataPosition = cclutDataPosition + CCLUT_INDENTED_LINE_WIDTH
        col 4 cclutOutputLine row+1
      endwhile
      
      col 0 CCLUT_LINE_OF_ASTERISKS row+1
  with nocounter, maxrow = 3, maxcol = 200, formfeed=none
 
  return (TRUE)
end ;;;generateErrorReport

call cclut1::main(null)


#exit_script
if (cclut1TestCaseReply->status_data.status = "F")
  call cclut1::generateErrorReport(cclut1::outputDestination, 
      concat(cclut1::testCaseDirectory, ":", cclut1::testCaseFileName), 
          cclut1TestCaseReply->status_data.subeventstatus[1].targetObjectValue)
endif
 
end go
