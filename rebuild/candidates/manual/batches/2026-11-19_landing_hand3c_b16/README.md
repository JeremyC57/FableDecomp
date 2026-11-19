# Landing hand3c b16 (2026-11-19): CTCInventoryQuests::InitialiseQuestCardList (0x00621A20)

**Result:** strict vland --land gives RELOCATION_MATCH + `00621a20_TEST PASS`; no flags. The parked local model was rewritten onto shared headers, in FableWin
0x01F7D6C0's statement order with Ego_d local names (5 grades).

**TU opt-ins that closed it:**
- FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL, the same TU convention as the landed 0x0061AD60;
- FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND (PeekTC's const LowerBound inline; GetTC 0x0040F020 stays a call).

**New declaration-level opt-ins** (pure additions; pin_asserts finds no moved assert):
- CTCInventoryQuests.h RESETCATEGORYLIST (inline `Categories.clear()`, Ego_r / FableWin ?ResetCategoryList@CTCInventoryQuests@@QAEXXZ);
- NInventory_CTCInventoryBase.h ADDCATEGORY (0x005C024A);
- CCategory.h SETASSELECTED (0x005BC934).

**Helpers:** __copy<NInventory::CCategory*> grades RELOCATION_MATCH at its own address 0x00625820. The 0x005C2A8E range destroy is another TU's COMDAT, so not
graded from here.

**Header-user recheck:** 114 / 114 good.

**Labels to review** (unproven, so no NAME_FIXES row): vland RELOC_NAME on 0x005C2A8E (labelled ForEachElem0x2c__CCreatureAction_FireMiss /
_Destroy_range<pair<..CAnimationEntry>>) and 0x005C2739 (~CCreatureAction_FireMissileWeapon). Both look like __destroy_aux<NInventory::CCategory*> / ~CCategory,
i.e. ICF-shared byte-twin labels; the naming agent should prove them.
