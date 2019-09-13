/**
  A program that demonstrates several oddities in the way CCL decides that strings are equal.
*/
drop program cclut_ccl_string_oddities go
create program cclut_ccl_string_oddities
  subroutine (doOutput(equal = i2) = null with protect)
    if (equal)
      call echo('they are equal')
    else
      call echo('they are NOT equal')
    endif
    call echo("")  
  end

  declare vcNull = vc with protect, constant("")
  declare vcEmpty = vc with protect, constant(" ")
  declare vcSpace = vc with protect, constant(notrim(" "))
  declare vcDoubleSpace = vc with protect, constant(notrim("  "))

  call echo("")  

  call echo("empty/null")
  call doOutput(parser(^notrim(vcEmpty) = notrim(vcNull)^))

  call echo("empty/space")
  call doOutput(parser(^notrim(vcEmpty) = notrim(vcSpace)^))

  call echo("null/space")
  call doOutput(parser(^notrim(vcNull) = notrim(vcSpace)^))

  call echo("space/double-space")
  call doOutput(parser(^notrim(vcSpace) = notrim(vcDoubleSpace)^))

  call echo("final-null/final-spaces")
  call doOutput(parser(^concat("hw", char(0)) = concat("hw", "  ")^))

  call echo("trimmed/final-spaces")
  call doOutput(parser(^"hw" = concat("hw", "  ")^))

  call echo("trimmed/final-null")
  call doOutput(parser(^"hw" = concat("hw", char(0))^))

  call echo("trimmed/final-space-null")
  call doOutput(parser(^"hw" = concat("hw", char(32), char(0))^))
end go
