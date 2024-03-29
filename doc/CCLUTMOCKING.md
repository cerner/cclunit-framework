
# CCL Unit Mocking
The CCL Unit Test framework's mocking API is available for consumers to mock certain objects to better isolate unit tests from outside variability and provide more control over the scenarios under which unit tests are run.  The API provides ways to mock tables, variables, subroutines, scripts, etc.

**\*CAUTION\*** **The CCL Unit Mocking framework should only be used in non-production environments.  Table mocking creates new tables against an Oracle instance for the lifetime of the test.  Because the DDL is generated in a dynamic way, it is possible through inappropriate use of the framework to affect an actual table.  Please only use the documented API.**

In the rare event that CCL crashes midway through a test or another abnormal abort occurs (e.g. as the result of an infinite loop in a test), it may be necessary to clean up any tables that the framework could not.  All tables created by the CCL Unit Test framework will be prepended with "CUST_CCLUT_".

## Contents
[API](#api)  
[Implementation Notes](#implementation-notes)  
[Example](#example)

## API

**cclutExecuteProgramWithMocks(programName = vc, params = vc, namespace = vc)**

Executes a CCL program applying an indicated namespace and all mocks that have been specified using the functions 
described below.  programName is required.  params is a stringified representation of the parameters to be passed to the
program, so all commas and string delimiters must be specified.  If namespace is omitted, it will default to the PUBLIC 
namespace.  Additionally, to execute the program with specific "with replace()" functionality (e.g. record structures), 
call cclutAddMockImplementation with the Name_From as originalName and Name_To as replaceName prior to calling 
cclutExecuteProgramWithMocks. 

@param programName  
&nbsp;&nbsp;&nbsp;&nbsp;The program to be executed with mocks.  
@param params  
&nbsp;&nbsp;&nbsp;&nbsp;The parameters to be sent to the program.  
@param namespace  
&nbsp;&nbsp;&nbsp;&nbsp;The namespace under which to execute the program.
  
Example:  
```javascript
call cclutExecuteProgramWithMocks("ccl_my_program")
call cclutExecuteProgramWithMocks("ccl_my_program", "^MINE^, 1.0, ^string parameter^")
call cclutExecuteProgramWithMocks("ccl_my_program", "", "MYNAMESPACE")
call cclutExecuteProgramWithMocks("ccl_my_program", "^MINE^, 1.0, ^string parameter^", "MYNAMESPACE")
```

**cclutRemoveAllMocks**

Removes all mock implementations and mock tables that have been added through the mocking APIs.  This should be called at the completion of a test case to clean up all mocks.  
  
Example:  
```javascript
call cclutRemoveAllMocks(null)
```

**cclutDefineMockTable(tableName = vc, columnNames = vc, columnTypes = vc, isAccessedViaRdbCommands = i2)**

Defines a mock table structure that can be created for use within a program.  This is the first function to be called in the process of mocking a table.  It must be called before cclutAddMockIndex() or cclutCreateMockTable() can be called.  The table will not be mocked in cclutExecuteProgramWithMocks() unless finalized by calling cclutCreateMockTable().  This function can be called for the same table after cclutCreateMockTable() in order to redefine it; however, the existing mocked table will be dropped and cclutCreateMockTable() will need to be called again to recreate it with the new defintion.  

tableName, columnNames, and columnTypes are required.  columnNames and columnTypes are expected to be pipe-delimited strings.  The columnTypes should have the same count as columnNames and be in the same order.  The columnTypes should match CCL types (e.g. vc, i4, f8); however, for string data, if no count is specified, the default maximum number of characters for the column will be 100.  To specify a count, place it after vc (e.g. vc55 will have a maximum size of 55 characters).  isAccessedViaRdbCommands is optional and should be TRUE only if the script-under-test accesses the table using rdb commands. Table mocking will fail if there are both rdb and non-rdb queries against the table.
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The table to be mocked.  
@param columnNames  
&nbsp;&nbsp;&nbsp;&nbsp;A pipe-delimited list of columns to be mocked on the table  
@param columnTypes  
&nbsp;&nbsp;&nbsp;&nbsp;A pipe-delimited list of types for each column  
@param isAccessedViaRdbCommands  
&nbsp;&nbsp;&nbsp;&nbsp;An optional boolean parameter to set to TRUE if the script-under-test accesses the table using rdb commands  
@returns  
&nbsp;&nbsp;&nbsp;&nbsp;The name of the mock table (This can be used to select data for testing)  
  
Example:  
```javascript
call cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc500|vc500|dq8") 
```

**cclutAddMockIndex(tableName = vc, columnNames = vc, isUnique = i4)**

Adds an index to a mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error will be thrown.  This function may not be called after cclutCreateMockTable().  tableName, columnNames, and isUnique are required.  columnNames may be a single column name or a pipe-delimited list of columns for a composite index (the order of the columns will be the order of the index).  If isUnique is TRUE, then a unique index will be created.  If isUnique is FALSE, then a non-unique index will be created.  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The name of the source table for the mock table to which the index will be added.  
@param columnNames  
&nbsp;&nbsp;&nbsp;&nbsp;A pipe-delimited string of column names for the index.  
@param isUnique  
&nbsp;&nbsp;&nbsp;&nbsp;TRUE to create a unique index; FALSE to create a non-unique index  
  
Example:  
```javascript
call cclutAddMockIndex("person", "person_id", TRUE)  
call cclutAddMockIndex("person", "name_last|name_first", FALSE)
```

**cclutCreateMockTable(tableName = vc)**

Creates a mock table.  The table must already be defined through cclutDefineMockTable(), otherwise an error will be thrown.  If the table has already been created, the function will return silently.  tableName is required.  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The name of the source table to be mocked.  
  
Example:  
```javascript
call cclutCreateMockTable("person")
```

**cclutRemoveMockTable(tableName = vc)**

Removes a mock table.  If the table was already created, it will also be dropped.  If the table is not currently  
mocked, it will return silently.  tableName is required.  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The name of the source table that is mocked.  
  
Example:  
```javascript
call cclutRemoveMockTable("person")
```

**cclutRemoveAllMockTables(null)**

Removes all mock tables.  Any tables that have already been created will also be dropped.  
  
Example:  
```javascript
call cclutRemoveAllMockTables(null)
```

**cclutAddMockData(tableName = vc, rowData = vc)**

WARNING (v3.4):  
This can fail to set the value for a table attribute if there is a variable in scope having the same name as the attribute.

Add a row of mock data to a table.  tableName and rowData are required.  tableName must have already been created through `cclutCreateMockTable` or an error will be thrown.  rowData is a pipe-delimited string for each column in the same order that was used in cclutDefineMockTable().  For character fields, the backslash (\\) will serve as an escape character.  For date fields, the value in rowData will be supplied to the cnvtdatetime() function.  All other values will be passed as-is.  
  
Supported escape values  
\\| = | (to represent a pipe in character fields)  
\\\\ = \ (to represent a backslash in character fields)  
\null = null (no value will be inserted into the column)  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The name of the source table for the mock table to which the data will be added.  
@param rowData  
&nbsp;&nbsp;&nbsp;&nbsp;A pipe-delimited string of data to be inserted into the mock table.  
  
Example:  
```javascript
call cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc500|vc500|dq8")  
call cclutCreateMockTable("person")  
call cclutAddMockData("person", "1.0|Washington|George|01-JAN-1970 00:00") ;Will add George Washington  
call cclutAddMockData("person", "2.0|A\|d\\ams|John|02-FEB-1971 11:11") ;Will add John A|d\ams  
call cclutAddMockData("person", "3.0|Jefferson|\null|03-MAR-1972 22:22") ;Will add Jefferson (no first name)  
call cclutAddMockData("person", "4.0|Madison||04-APR-1973 10:33") ;Will add Madison (empty string for first name)
```

**cclutClearMockData(tableName = vc)**

Clears all data from a specified mock table.  This is functionally similar to a truncate.  tableName is required.  The table must have been created through `cclutCreateMockTable` or an error will be thrown.  
  
@param tableName  
&nbsp;&nbsp;&nbsp;&nbsp;The name of the source table for the mock table to be cleared.  
  
Example:  
```javascript
call cclutClearMockData("person")
```

**cclutAddMockImplementation(originalName = vc, replaceName = vc)**

Adds a mock implementation to be utilized by cclutExecuteProgramWithMocks.  This can include record structures, subroutines, or other programs.  originalName and replaceName are required.  If originalName is already being mocked, the mock will be overwritten with the new replaceName.  
  
@param originalName  
&nbsp;&nbsp;&nbsp;&nbsp;The object to be mocked.  
@param replaceName  
&nbsp;&nbsp;&nbsp;&nbsp;The mocked object.  
  
Example:  
```javascript
call cclutAddMockImplementation("uar_get_code_by", "mock_uar_get_code_by")
```

**cclutRemoveMockImplementation(originalName = vc)**

Removes a mock implementation.  
  
@param originalName  
&nbsp;&nbsp;&nbsp;&nbsp;The object that is mocked.  
  
Example:  
```javascript
call cclutRemoveMockImplementation("uar_get_code_by")
```

**cclutRemoveAllMockImplementations(null)**

Removes all mock implementations.  
  
Example:  
```javascript
call cclutRemoveAllMockImplementations(null)
```

## Implementation Notes
1. The mocking functions automatically normalize entity names to uppercase to match with CCL and Oracle.

1. `cclutRemoveAllMocks` should be called from teardown.
    * The framework removes outstanding mocks after a test case run, but it is good practice to do so explicitly per test.

1. Mocks can be leveraged for tables and fields that do not exist in the testing domain.
    * This can be useful to test tables that are still under construction.

1. Mocks created through `cclutCreateMockTable` and `cclutAddMockImplementation` are not applied to child scripts called from the script-under-test. Alternatives:
    * Mock each child script to return data appropriate for the test.
    * Mock each child script to execute the real child script using `with replace` to reference the mock entities.

1. Mock substitutions are not applied to statements executed via `call parser`. Alternatives:
    * Do not invoke `call parser` directly but define and invoke a wrapper subroutine instead. Mock the wrapper to make any necessary name substitutions before invoking `call parser`.
        * The mock wrapper could additionally assert that the program generates the expected command string.
    * Use subroutines to generate the parser commands and mock those subroutines to generate commands that incorporate the mock entity names.

1. The table mocking APIs are not supported within a reportwriter section. It might be tempting to use a dummyt query to set up mock data from a record structure, but several mocking calls cannot be executed in the context of a query because their implementations execute queries. Use a for loop instead.

1. Namespaces are not supported when using cclutAddMockImplementation.  The following does not work.
   ```javascript
   call cclutAddMockImplementation("public::testSubroutine", "myNamespace::mockSubroutine")
   ```

   Alternatives depend on the specifics of the script-under-test. An option that works for the example above is defining the mock implementation with the same namespace as the real subroutine (i.e., public::mockSubroutine) and excluding the namespaces when adding the mock.
   
   ```javascript
   call cclutAddMockImplementation("testSubroutine", "mockSubroutine")
   ```

1. Mocking the tdbexecute `reply_to` entity is unsupported under certain conditions, specifically if the `reply_to` entity is freed prior to calling tdbexecute. If this scenario is truly necessary, the best alternative is to define and use a subroutine to free the `reply_to` entity and mock that subroutine to not actually perform the free record statement.

1. Table mocking produces unexpected results with older versions of CCL if the table and one of its fields have the same name, like `code_value.code_value` for example (32-bit < 8.15.0, 64-bit < 9.3.0). Anomalies will occur in all versions of CCL if a mocked entity shares the same name as a table or field queried by the script-under-test.

1. Table mocking is ineffective for an rdb command that accesses a field with the same name as the mocked table (32-bit >= 8.15.0, 64-bit >= 9.3.0).

1. Mock substitutions can fail to occur in older versions of CCL if the script-under-test executes some other CCL program before declaring/implementing its subroutines (32-bit < 8.14.1, 64-bit < 9.2.4).
  
## Example
The following annotated example illustrates some of the APIs available in the CCL Unit Mocking framework.

Script-under-test:

```javascript
drop program abc_mo_get_persons:dba go
create program abc_mo_get_persons:dba
  subroutine (public::main(null) = null)
    declare newSize = i4 with protect, noconstant(0)

    select into "nl:" from person p
    plan p
    order by p.person_id
    detail
      newSize = newSize + 1
      stat = alterlist(reply->persons, newSize)
      reply->persons[newSize].person_id = p.person_id
      reply->persons[newSize].name_last = p.name_last
      reply->persons[newSize].name_first = p.name_first
      reply->persons[newSize].birth_dt_tm = p.birth_dt_tm
    with nocounter

    rdb set output "ccluserdir:rdb_output.dat" end
    rdb select p.person_id, p.position_cd, p.username from prsnl p where p.username = 'CCLUNIT' end
  end ;main

  call main(null)
end go
```

Test Code:

```javascript
declare teardown(null) = null
declare testIt(null) = null

subroutine teardown(null)
  call cclutRemoveAllMocks(null)
end

subroutine testIt(null)
  declare mock_table_person = vc with protect, noconstant("")

  ; Define a mock person table. The return value is the name of the mock table which can be useful to perform a select on the
  ; mock table after the script-under-test is complete to verify (among other things) that an insert or a delete worked correctly.
  set mock_table_person = cclutDefineMockTable("person", "person_id|name_last|name_first|birth_dt_tm", "f8|vc500|vc500|dq8")

  ; Add a non-unique index to name_last
  call cclutAddMockIndex("person", "name_last", FALSE)

  ; Create the mock table.  After this, it is available for DML statements.
  call cclutCreateMockTable("person")

  ; Create data for the table.
  call cclutAddMockData("person", "1.0|Washington|George|01-JAN-1970 00:00") ;Will add George Washington
  call cclutAddMockData("person", "2.0|Adams|John|02-FEB-1971 11:11") ;Will add John Adams
  call cclutAddMockData("person", "3.0|Jefferson|\null|03-MAR-1972 22:22") ;Will add Jefferson (no first name)
  call cclutAddMockData("person", "4.0|Madison||04-APR-1973 10:33") ;Will add Madison (empty string for first name)


  ; Perform similar steps to mock the prsnl table but pass TRUE into cclutDefineMockTable because the script-under-test
  ; uses an rdb command to read the prsnl table.
  call cclutDefineMockTable("prsnl", "person_id|position_cd|username", "f8|f8|vc30", TRUE)
  call cclutCreateMockTable("prsnl")
  call cclutAddMockData("prsnl", "1.0|441.0|CERNER")
  call cclutAddMockData("prsnl", "2.0|441.0|SYSTEM")
  call cclutAddMockData("prsnl", "3.0|315.0|CCLUNIT")
  call cclutAddMockData("prsnl", "4.0|689.0|DRONE")
  call cclutAddMockData("prsnl", "5.0|712.0|RNONE")


  record agp_reply (
    1 persons[*]
        2 person_id = f8
        2 name_last = vc
        2 name_first = vc
        2 birth_dt_tm = dq8
  ) with protect

  ; Have with replace("REPLY", AGP_REPLY) be applied when executing abc_mo_get_persons.
  call cclutAddMockImplementation("REPLY", "AGP_REPLY")

  ; Execute the script-under-test
  call cclutExecuteProgramWithMocks("abc_mo_get_persons", "")

  ; Do validation
  call cclutAssertf8Equal(CURREF, "check person_id a", agp_reply->persons[1].person_id, 1.0)
  call cclutAssertvcEqual(CURREF, "check name_last a", agp_reply->persons[1].name_last, "Washington")
  call cclutAssertvcEqual(CURREF, "check name_first a", agp_reply->persons[1].name_first, "George")
  call cclutAssertf8Equal(CURREF, "check birth_dt_tm a", agp_reply->persons[1].birth_dt_tm, cnvtdatetime("01-JAN-1970 00:00"))

  call cclutAssertf8Equal(CURREF, "check person_id b", agp_reply->persons[2].person_id, 2.0)
  call cclutAssertvcEqual(CURREF, "check name_last b", agp_reply->persons[2].name_last, "Adams")
  call cclutAssertvcEqual(CURREF, "check name_first b", agp_reply->persons[2].name_first, "John")
  call cclutAssertf8Equal(CURREF, "check birth_dt_tm b", agp_reply->persons[2].birth_dt_tm, cnvtdatetime("02-FEB-1971 11:11"))

  call cclutAssertf8Equal(CURREF, "check person_id c", agp_reply->persons[3].person_id, 3.0)
  call cclutAssertvcEqual(CURREF, "check name_last c", agp_reply->persons[3].name_last, "Jefferson")
  call cclutAssertvcEqual(CURREF, "check name_first c", agp_reply->persons[3].name_first, "")
  call cclutAssertf8Equal(CURREF, "check birth_dt_tm c", agp_reply->persons[3].birth_dt_tm, cnvtdatetime("03-MAR-1972 22:22"))

  call cclutAssertf8Equal(CURREF, "check person_id d", agp_reply->persons[4].person_id, 4.0)
  call cclutAssertvcEqual(CURREF, "check name_last d", agp_reply->persons[4].name_last, "Madison")
  call cclutAssertvcEqual(CURREF, "check name_first d", agp_reply->persons[4].name_first, "")
  call cclutAssertf8Equal(CURREF, "check birth_dt_tm d", agp_reply->persons[4].birth_dt_tm, cnvtdatetime("04-APR-1973 10:33"))

  ; Here is a contrived example to demonstrate a potential use for the return value from cclutDefineMockTable
  select into "nl:"
    personCount = count(*)
  from (value(mock_table_person) mtp)
  head report
    call cclutAsserti4Equal(CURREF, "check person count a", cnvtint(personCount), 4)
  with nocounter

  ; Validate the results of the rdb query
  free define rtl2
  define rtl2 is "ccluserdir:rdb_output.dat"
  select into "nl:" from rtl2t r
    head report
      call cclutAssertVcOperator(CURREF, "rdb result header", r.line,
               "regexplike", "PERSON_ID[ ]*\|POSITION_CD[ ]*\|USERNAME[ ]*\|")
    foot report
      call cclutAssertVcOperator(CURREF, "rdb result data", r.line,
               "regexplike", "3\|[ ]*315\|[ ]*|CCLUNIT[ ]*\|")
  with nocounter

end ;testIt
```
