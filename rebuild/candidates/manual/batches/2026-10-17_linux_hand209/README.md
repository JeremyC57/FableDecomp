# Hand chunk 209 (CQ_GuildTrainingWoodsWillScript .. CRandomAppearanceMorph): 3 of 40 byte-exact

## What matched (3)

- 00d68f00 CQ_HangingTreeEvilScript constructor: CCharString, script handle, intelligent pointer and counter member offsets reproduce the retail initialization order; the nine trailing zero words were required.
- 00daac00 CQ_NewOakValeIntroScript constructor: an embedded explicit zero-word initializer preserves retail `xor eax,eax` and the two timer calls.
- 00e398f0 CQ_HangingTreeGoodScript constructor: one CBase script handle, four intelligent handles and a map with a pre-allocation null store and post-allocation count store reproduce retail.

## Parked for Fable 5.1 (37)

- 00a88b60 CQuaternion::operator* DIFFER(172v172): x87 arithmetic matches but temporary spill slots differ (retail stores X at +0xc and shifts the other components) | tried: named result, four-float constructor result, named Y/Z/W/X locals | next: model the original hidden-result constructor temporary.
- 009de890 CQuickDrawTriInfo::CQuickDrawTriInfo DIFFER(160v164): remaining difference is callee-saved/register scheduling for flag bits and CRGBColour byte copies | tried: scalar window members, explicit byte copies, base-subobject copy | next: lane gap: register scheduling.
- 00de0320 CQ_RansomVictimScript constructor DIFFER(184v184): behavior and member offsets match; candidate differs only in unresolved ctor/vtable relocation representation | tried: five intelligent members, explicit callee address comments, derived virtual override | next: lane gap: vtable relocation.
- 00edeba0 CQ_WizardBattleScript constructor DIFFER(163v163): member offsets and map initialization match; candidate differs only in unresolved ctor/vtable relocation representation | tried: three intelligent members, map padding/order, callee address comments | next: lane gap: vtable relocation.
- Remaining unstarted rows are listed in STATUS.tsv for continuation by the next lane.

## Skipped

None.

## Label corrections

None.
