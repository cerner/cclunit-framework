%#ifndef CCLUT_GET_FILE_AS_STRING_INC
%#def CCLUT_GET_FILE_AS_STRING_INC

;**********************************************************************************************************************************
;
; Defines the cclut::getFileAsString subroutine used to obtain the contents of a file as a string.
;
;**********************************************************************************************************************************
 
set modify maxvarlen 10000000


/**
  Retrieves the contents of a file as a vc.
  @param  cclutFullyQualifiedFileName  
    The fully qualified name for the file to retrieve (direcotry + fileName).
  @returns
    The file contents as a string if successful, "" otherwise.
*/
declare cclut::getFileAsString(cclutFullyQualifiedFileName  = vc) = vc with protect
subroutine cclut::getFileAsString(cclutFullyQualifiedFileName )
  declare CCLUT_SEEK_SET = i4 with protect, constant(0)
  declare CCLUT_SEEK_END = i4 with protect, constant(2)
  declare fileLength = i4 with protect
  declare cclutStat = i4 with protect, noconstant(0)

  record frec(
    1 file_desc = H
    1 file_offset = i4
    1 file_dir = i4
    1 file_name = vc
    1 file_buf = vc
  ) with protect


  SET frec->file_name = cclutFullyQualifiedFileName
  SET frec->file_buf = "r"
  SET cclutStat = cclio("OPEN", frec)

  set frec->file_dir = CCLUT_SEEK_END
  SET cclutStat = CCLIO("SEEK", frec)
  set fileLength = CCLIO("TELL", frec)
  set frec->file_dir = CCLUT_SEEK_SET
  set cclutStat = CCLIO("SEEK", frec)

  set frec->file_buf = notrim(fillstring(value(fileLength)," "))
  set cclutStat = cclio("READ", frec)
  set cclutStat = cclio("CLOSE", frec)
  return (frec->file_buf)
end ;;;cclut::getFileAsString

%#endif CCLUT_GET_FILE_AS_STRING_INC
 