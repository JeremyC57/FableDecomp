# acctC wfB6 (parked rows, 2026-11-14): three byte-matches that are NOT handed back yet

`matched_needs_fixup/` holds three rows that grade RELOCATION_MATCH / test PASS (workflow verifier + coordinator) but break
lane rules, so they are not in a payload. Do not land them as they are.
- 0x005379F5 NUISystem::CList::ComputeChildrenVisibility (2,102): depends on inline header bodies of three CALLED callees
  (ConvertX 0x0052E470, ConvertY 0x0052E4D0, C2DBoxF::ContainsPoint 0x00535CD0) whose texts do not reproduce their own retail
  copies (ConvertX/Y operand order; ContainsPoint 63v66). Fix: the real texts, then regrade.
- 0x00772BB0 CTCShop::AddSaleItem (2,585): local models of CTCCreatedEntity / CTCStockItem although engine headers exist;
  local SetPCreator(const CThing*) contradicts the PDB (const CThing&).
- 0x007DE710 CTCWallMount::AttachObject (2,578): local models of CTCWallMount / CTCTargeted / a renamed CTCHeroStats pad
  struct; local IS_THING_ALIVE contradicts engine/ThingAlive.h and the PDB.
Both of the last two also use FABLE_RETAIL_CMATRIX3X4_SETINVERSEUSRT_SCALE_SQUARED (fitted named local in a shared header,
behaviour unchanged) and a force-inline Set (FABLE_RETAIL_CMATRIX3X4_SET_RHSET_INLINE) without an `// Inline-budget:` line.
Tool gap for the lander: the strict-header regex in verify_and_land.py (`class Name {`) misses local classes with a base clause.
Also here: drafts of 0x00875690 (726v762) and 0x007DB6A0 (2556v2556).
