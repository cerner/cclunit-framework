declare tearDown(null) = null
declare test_cclutIsEmpty_true(null) = null
declare test_cclutIsEmpty_false(null) = null
declare test_cclutDoArraySplit_pipe_zero_empty(null) = null
declare test_cclutDoArraySplit_pipe_zero_non_empty(null) = null
declare test_cclutDoArraySplit_pipe_one(null) = null
declare test_cclutDoArraySplit_pipe_three(null) = null
declare test_cclutDoArraySplit_pipe_three_empty_ends(null) = null
declare test_cclutDoArraySplit_triple_pipe_three(null) = null

subroutine tearDown(null)
	rollback
end ;tearDown

/**
    Verifies that cclutIsEmpty returns TRUE when the input string is empty or only contains whitespace characters.
*/
subroutine test_cclutIsEmpty_true(null)
	call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_true 001", cclutIsEmpty(""), TRUE)
    call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_true 002", cclutIsEmpty("     "), TRUE)
    call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_true 003",
        cclutIsEmpty(concat(char(10), char(20), char(30))), TRUE)
end ;test_cclutIsEmpty_true

/**
    Verifies that cclutIsEmpty returns FALSE when the input string contains non-whitespace whitespace characters.
*/
subroutine test_cclutIsEmpty_false(null)
	call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_false 001", cclutIsEmpty("aaaaa"), FALSE)
    call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_false 002", cclutIsEmpty("  aaaaa  "), FALSE)
    call cclutAsserti4Equal(CURREF, "test_cclutIsEmpty_false 003",
        cclutIsEmpty(concat(char(10), "aaaaa", char(30))), FALSE)
end ;test_cclutIsEmpty_false

/**
    verifies the behavior for cclutDoArraySplit for a pipe delimited string with no pipes and only whitespace.
*/
subroutine test_cclutDoArraySplit_pipe_zero_empty(null)
    record inRec(1 qual[*] 2 str = vc) with protect
    declare itemCount = i4 with protect, noconstant(0)
    declare strSource = vc with protect, noconstant("")
    set strSource = notrim("   ")
    set itemCount = cclutDoArraySplit(inRec, strSource, "|")

    call cclutAssertI4Equal(CURREF, "item count a", itemCount, 1)
    call cclutAssertVcEqual(CURREF, "item value a 1", inRec->qual[1].str, trim(""))
end

/**
    verifies the behavior for cclutDoArraySplit for a pipe delimited string with no pipes and some non-whitespace.
*/
subroutine test_cclutDoArraySplit_pipe_zero_non_empty(null)
    record inRec(1 qual[*] 2 str = vc) with protect
    declare itemCount = i4 with protect, noconstant(0)
    declare strSource = vc with protect, noconstant("")
    set strSource = notrim("  i am non-empty   ")
    set itemCount = cclutDoArraySplit(inRec, strSource, "|")

    call cclutAssertI4Equal(CURREF, "item count b", itemCount, 1)
    call cclutAssertVcEqual(CURREF, "item value b 1", inRec->qual[1].str, "  i am non-empty")
end

/**
    verifies the behavior for cclutDoArraySplit for a pipe delimited with one pipe.
*/
subroutine test_cclutDoArraySplit_pipe_one(null)
    record inRec(1 qual[*] 2 str = vc) with protect
    declare itemCount = i4 with protect, noconstant(0)
    declare strSource = vc with protect, noconstant("")
    set strSource = "  this string contains  |    one pipe   "
    set itemCount = cclutDoArraySplit(inRec, strSource, "|")

    call cclutAssertI4Equal(CURREF, "item count c", itemCount, 2)
    call cclutAssertVcEqual(CURREF, "item value c 1", inRec->qual[1].str, "  this string contains")
    call cclutAssertVcEqual(CURREF, "item value c 2", inRec->qual[2].str, "    one pipe")

end

/**
    verifies the behavior for cclutDoArraySplit for a pipe delimited with three pipes.
*/
subroutine test_cclutDoArraySplit_pipe_three(null)
    record myRec(1 qual[*] 2 str = vc) with protect
    declare itemCount = i4 with protect, noconstant(0)
    declare strSource = vc with protect, noconstant("")
    set strSource = "  this string |contains|three|  pipes"
    set itemCount = cclutDoArraySplit(myRec, strSource, "|")

    call cclutAssertI4Equal(CURREF, "item count d", itemCount, 4)
    call cclutAssertVcEqual(CURREF, "item value d 1", myRec->qual[1].str, "  this string")
    call cclutAssertVcEqual(CURREF, "item value d 1", myRec->qual[2].str, "contains")
    call cclutAssertVcEqual(CURREF, "item value d 1", myRec->qual[3].str, "three")
    call cclutAssertVcEqual(CURREF, "item value d 1", myRec->qual[4].str, "  pipes")
end

/**
    verifies the behavior for cclutDoArraySplit for a pipe delimited string with three pipes but nothing
    before the first or after the last.
*/
subroutine test_cclutDoArraySplit_pipe_three_empty_ends(null)
    record yourRec(1 qual[*] 2 str = vc) with protect
    declare itemCount = i4 with protect, noconstant(0)
    declare strSource = vc with protect, noconstant("")
    set strSource = "|field 2|field 3|"
    set itemCount = cclutDoArraySplit(yourRec, strSource, "|")

    call cclutAssertI4Equal(CURREF, "item count e", itemCount, 4)
    call cclutAssertVcEqual(CURREF, "item value e 1", yourRec->qual[1].str, trim(""))
    call cclutAssertVcEqual(CURREF, "item value e 1", yourRec->qual[2].str, "field 2")
    call cclutAssertVcEqual(CURREF, "item value e 1", yourRec->qual[3].str, "field 3")
    call cclutAssertVcEqual(CURREF, "item value e 1", yourRec->qual[4].str, trim(""))
end

/**
    verifies the behavior for cclutDoArraySplit for a triple-pipe delimited string with three delimiters.
*/
subroutine test_cclutDoArraySplit_triple_pipe_three(null)
    record yourRec(1 qual[*] 2 str = vc) with protect
    declare itemCount = i4 with protect, noconstant(0)
    declare strSource = vc with protect, noconstant("")
    set strSource = "field|1  |||field|2|||field|3|||field|4"
    set itemCount = cclutDoArraySplit(yourRec, strSource, "|||")

    call cclutAssertI4Equal(CURREF, "item count e", itemCount, 4)
    call cclutAssertVcEqual(CURREF, "item value e 1", yourRec->qual[1].str, "field|1")
    call cclutAssertVcEqual(CURREF, "item value e 1", yourRec->qual[2].str, "field|2")
    call cclutAssertVcEqual(CURREF, "item value e 1", yourRec->qual[3].str, "field|3")
    call cclutAssertVcEqual(CURREF, "item value e 1", yourRec->qual[4].str, "field|4")
end
