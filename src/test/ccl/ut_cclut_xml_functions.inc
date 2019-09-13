/**
  Unit tests for cclut_xml_functions.inc
*/
%i cclsource:cclut_xml_functions.inc

declare testCreateXmlElement(null) = null with protect
declare testRetrieveXmlContent(null) = null with protect


/**
  Validates the behavior of the cclut::createXmlElement function
*/
subroutine testCreateXmlElement(null)
  declare xml = vc with protect, noconstant("")
  declare stat = i4 with protect, noconstant(0)

  set xml = cclut::createXmlElement("some_element", "the content for some element")
  set stat = cclutAssertVCEqual(
      CURREF, "cclut::createXmlElement expected equal simple", xml, "<SOME_ELEMENT>the content for some element</SOME_ELEMENT>")
  set xml = cclut::createXmlElement("another_element", "<INNER_ELEMENT>the content for some element</INNER_ELEMENT>")
  set stat = cclutAssertVCEqual(
      CURREF, "cclut::createXmlElement expected equal complex", xml,
      "<ANOTHER_ELEMENT><INNER_ELEMENT>the content for some element</INNER_ELEMENT></ANOTHER_ELEMENT>")
end ;;;testCreateXmlElement


/**
  Validates cclut::retrieveXmlContent can find empty content.
*/
subroutine testRetrieveXmlContent0(null)
  declare found = i2 with protect, noconstant(FALSE)
  declare value = vc with protect, noconstant("")
  set value = cclut::retrieveXmlContent(~<LINELIMIT></LINELIMIT><CONTEXT><![CDATA[setupOnce executed]]></CONTEXT>~,
      "linelimit", 1, found)
  call cclutAssertI2Equal(CURREF, "linenumber found", found, TRUE)
  call cclutAssertVCEqual(CURREF, "line number", value, trim(""))
end ;;;testRetrieveXmlContent0

/**
  Validates the cclut::retrieveXmlContent can find non-empty content correctly
*/
subroutine testRetrieveXmlContentA(null)
  declare found = i2 with protect, noconstant(FALSE)
  declare value = vc with protect, noconstant("")
  set value = cclut::retrieveXmlContent(~<LINENUMBER>801</LINENUMBER><CONTEXT><![CDATA[setupOnce executed]]></CONTEXT>\
<RESULT>PASSED</RESULT><CONDITION><![CDATA["SO"="SO"]]></CONDITION>~, "linenumber", 1, found)
  call cclutAssertI2Equal(CURREF, "linenumber found", found, TRUE)
  call cclutAssertVCEqual(CURREF, "line number", value, "801")
end ;;;testRetrieveXmlContentA

/**
  Validates cclut::retrieveXmlContent strips CDATA tags from content and retains leading spaces in content.
*/
subroutine testRetrieveXmlContentB(null)
  declare xml = vc with protect, noconstant("")
  declare content = vc with protect, noconstant("")
  declare found = i2 with protect, noconstant(FALSE)
  declare stat = i4 with protect, noconstant(0)

  set xml = "<TAG_1><TAG_1_1>1.1 content</TAG_1_1><TAG_1_2><TAG_1_2_1>1.2.1 content</TAG_1_2_1>\
<TAG_1_2_2>1.2.2 content</TAG_1_2_2></TAG_1_2><TAG_1_3><![CDATA[1.3 content]]></TAG_1_3></TAG_1>"

  set content = cclut::retrieveXmlContent(xml, "TAG_1_2_1", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "tag 1.2.1 found", found, TRUE)
  set stat = cclutAssertVCEqual(CURREF, "tag 1.2.1 content", content, "1.2.1 content")
  set content = cclut::retrieveXmlContent(xml, "TAG_1_2", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "tag 1.2 found", found, TRUE)
  set stat = cclutAssertVCEqual(CURREF, "tag 1.2 content", content,
      "<TAG_1_2_1>1.2.1 content</TAG_1_2_1><TAG_1_2_2>1.2.2 content</TAG_1_2_2>")
  set content = cclut::retrieveXmlContent(xml, "TAG_1", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "tag 1 found", found, TRUE)
  set stat = cclutAssertVCEqual(CURREF, "tag 1.2.1 content", content,
      "<TAG_1_1>1.1 content</TAG_1_1><TAG_1_2><TAG_1_2_1>1.2.1 content</TAG_1_2_1><TAG_1_2_2>1.2.2 content</TAG_1_2_2>\
</TAG_1_2><TAG_1_3><![CDATA[1.3 content]]></TAG_1_3>")

  set xml = "<TAG_3>this is bad xml</TAG_4>"
  set content = cclut::retrieveXmlContent(xml, "TAG_3", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "tag 3 found", found, FALSE)
  set stat = cclutAssertVCEqual(CURREF, "tag 3 content", content, "")

  set xml = "<TAG_4><![CDATA[  check this out  ]]></TAG_4>"
  set content = cclut::retrieveXmlContent(xml, "tag_4", 3, found)
  set stat = cclutAssertI2Equal(CURREF, "tag 4 @ 3 found", found, FALSE)
  set stat = cclutAssertVCEqual(CURREF, "tag 4 @ 3 content", content, "")
  set content = cclut::retrieveXmlContent(xml, "tag_4", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "tag 4 found", found, TRUE)
  set stat = cclutAssertVCEqual(CURREF, "tag 4 content", content, "  check this out")
end ;;;testRetrieveXmlContentB


/**
  Validates cclut::retrieveXmlContent can deal with nested elements matching the tag name it is looking for.
*/
subroutine testRetrieveXmlContentComplex(null)
  declare xml = vc with protect, noconstant("")
  declare content = vc with protect, noconstant("")
  declare contentTc = vc with protect, noconstant("")
  declare contentTsts = vc with protect, noconstant("")
  declare contentTst = vc with protect, noconstant("")
  declare contentAssrts = vc with protect, noconstant("")
  declare contentAssrt = vc with protect, noconstant("")
  declare name = vc with protect, noconstant("")
  declare lineno = vc with protect, noconstant("")
  declare ctx = vc with protect, noconstant("")
  declare condition = vc with protect, noconstant("")
  declare found = i2 with protect, noconstant(FALSE)
  declare stat = i4 with protect, noconstant(0)

  set xml = 
~<TC><NM>ut_cclut_suo_su_td_tdo</NM><TSTS><TST><NM>SETUPONCE</NM><ASSRTS><ASSRT><LINENO>801</LINENO><CTX><![CDATA[setupOnce e\
xecuted]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA["SO"="SO"]]></TST></ASSRT></ASSRTS><ERRS></ERRS><RSLT>PASSED</RSLT></TST><TS\
T><NM>TESTONE</NM><ASSRTS><ASSRT><LINENO>821</LINENO><CTX><![CDATA[setup executed]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA[0=\
0]]></TST></ASSRT><ASSRT><LINENO>841</LINENO><CTX><![CDATA[testOne executed]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA[0=0]]></\
TST></ASSRT><ASSRT><LINENO>831</LINENO><CTX><![CDATA[teardown executed]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA[0=0]]></TST><\
/ASSRT></ASSRTS><ERRS></ERRS><RSLT>PASSED</RSLT></TST><TST><NM>TESTTWO</NM><ASSRTS><ASSRT><LINENO>821</LINENO><CTX><![CDATA[s\
etup executed]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA[0=0]]></TST></ASSRT><ASSRT><LINENO>851</LINENO><CTX><![CDATA[testTwo e\
xecuted]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA[0=0]]></TST></ASSRT><ASSRT><LINENO>852</LINENO><CTX><![CDATA[testTwo execute\
d]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA["A"="A"]]></TST></ASSRT><ASSRT><LINENO>831</LINENO><CTX><![CDATA[teardown executed\
]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA[0=0]]></TST></ASSRT></ASSRTS><ERRS></ERRS><RSLT>PASSED</RSLT></TST><TST><NM>TESTTHR\
EE</NM><ASSRTS><ASSRT><LINENO>821</LINENO><CTX><![CDATA[setup executed]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA[0=0]]></TST><\
/ASSRT><ASSRT><LINENO>856</LINENO><CTX><![CDATA[testThree executed]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA[0=0]]></TST></ASS\
RT><ASSRT><LINENO>831</LINENO><CTX><![CDATA[teardown executed]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA[0=0]]></TST></ASSRT></\
ASSRTS><ERRS></ERRS><RSLT>PASSED</RSLT></TST><TST><NM>TEARDOWNONCE</NM><ASSRTS><ASSRT><LINENO>811</LINENO><CTX><![CDATA[teard\
ownOnce executed]]></CTX><RSLT>PASSED</RSLT><TST><![CDATA["TO"="TO"]]></TST></ASSRT></ASSRTS><ERRS></ERRS><RSLT>PASSED</RSLT>\
</TST></TSTS></TC>~

  set contentTC = cclut::retrieveXmlContent(xml, "tc", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "tc found", found, TRUE)
  set name = cclut::retrieveXmlContent(contentTc, "nm", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "nm found", found, TRUE)
  set stat = cclutAssertVCEqual(CURREF, "nm value", name, "ut_cclut_suo_su_td_tdo")

  set contentTsts = cclut::retrieveXmlContent(contentTc, "tsts", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "tsts found", found, TRUE)
  set contentTst = cclut::retrieveXmlContent(contentTsts, "tst", 1, found)
  set name = cclut::retrieveXmlContent(contentTst, "nm", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "name found", found, TRUE)
  set stat = cclutAssertVCEqual(CURREF, "nm value", name, "SETUPONCE")

  set contentAssrts = cclut::retrieveXmlContent(contentTst, "assrts", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "assrts found", found, TRUE)
  set contentAssrt = cclut::retrieveXmlContent(contentAssrts, "assrt", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "assrt found", found, TRUE)

  set lineno = cclut::retrieveXmlContent(contentAssrt, "lineno", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "lineno found", found, TRUE)
  set stat = cclutAssertVCEqual(CURREF, "lineno value", lineno, "801")

  set ctx = cclut::retrieveXmlContent(contentAssrt, "ctx", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "ctx found", found, TRUE)
  set stat = cclutAssertVCEqual(CURREF, "ctx value", ctx, "setupOnce executed")

  set condition = cclut::retrieveXmlContent(contentAssrt, "tst", 1, found)
  set stat = cclutAssertI2Equal(CURREF, "condition found", found, TRUE)
  set stat = cclutAssertVCEqual(CURREF, "condition value", condition, ~"SO"="SO"~)
end ;;;testRetrieveXmlContentComplex