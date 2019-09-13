/**
  The minimum required version of CCL for the CCL Unit Testing Framework to work.

  If newer CCL functionality is leveraged during an update, this value must be updated to the be the oldest version of CCL
  which supports that functionality.

  If the minimum ever needs to be bumped, the architecture for this concept will need to be revised because
  there are now different minimums depending on the CCL revision. Currently this is okay because 9.0.0 equates to 8.12.2.
*/
declare public::cclutFillStr(cclutLen = i4, cclutFill = vc) = vc with protect

declare cclut::FRAMEWORK_VERSION = vc with protect, constant("3.5-SNAPSHOT")
declare cclut::MINIMUM_REQUIRED_CCL_VERSION = vc with protect, constant("8.12.0")
declare cclut::FRAMEWORK_STATE_MARGIN = i4 with protect, constant(4)

declare cclut::VERSION_DISPLAY = vc with protect, constant(concat("CCL Unit Framework version ", cclut::FRAMEWORK_VERSION))
declare cclut::CCL_VERSION_DISPLAY = vc with protect,
    constant(concat("Minimum required CCL version ", cclut::MINIMUM_REQUIRED_CCL_VERSION))

declare cclut::VERSION_LEN = i4 with protect, constant(textlen(cclut::VERSION_DISPLAY))
declare cclut::CCL_VERSION_LEN = i4 with protect, constant(textlen(cclut::CCL_VERSION_DISPLAY))
declare cclut::FRAMEWORK_STATE_TEXT_AREA_WIDTH = i4 with protect, constant(maxval(cclut::VERSION_LEN, cclut::CCL_VERSION_LEN))
declare cclut::FRAMEWORK_STATE_AREA_WIDTH = i4 with protect,
    constant(2*cclut::FRAMEWORK_STATE_MARGIN + cclut::FRAMEWORK_STATE_TEXT_AREA_WIDTH)
declare cclut::FRAMEWORK_STATE_LINE_WIDTH = i4 with protect, constant(2+cclut::FRAMEWORK_STATE_AREA_WIDTH)

declare cclut::FRAMEWORK_STATE_LINE_OF_STARS = vc with protect, constant(cclutFillStr(cclut::FRAMEWORK_STATE_LINE_WIDTH, "*"))
declare cclut::FRAMEWORK_STATE_LINE_OF_SPACES = vc with protect,
    constant(concat("*", cclutFillStr(cclut::FRAMEWORK_STATE_AREA_WIDTH, " "), "*"))


subroutine public::cclutFillStr(cclutLen, cclutFill)
  declare cclutRetVal = vc with protect, noconstant(trim(""))
  declare cclutIdx = i4 with protect, noconstant(0)
  for (cclutIdx = 1 to cclutLen)
    set cclutRetVal = notrim(concat(cclutRetVal, cclutFill))
  endfor
  return (cclutRetVal)
end ;;;cclutFillStr
