# Landing 2026-11-05e: chunk 778 (2 of 5 ledger rows)

Authored by one expert agent (owner, 2026-11-05: continue authoring with one expert agent). Both rows are plain C++ and grade
`genuine`.

| Address | Name | Grade | Grades | Key spelling |
|---|---|---|---|---|
| 0x00631720 | NUISystem::CUIDef::operator= | RELOCATION_MATCH PASS | 3 | `*this = *static_cast<const CUIDef*>(def);`: retail's operator= is the implicit one (FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN) |
| 0x00615E70 | CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferIn | RELOCATION_MATCH PASS | 5 | sibling 0x006159A0's shape; the optional Taverns field as `PeekNextItem && IsSymbol`, then a nested `GetSymbol() == ','` |

- **Headers.** Every changed header reads `default=same` in hdrcheck2.
  - The FABLE_RETAIL_CUIDEF branch of `NUISystem_CUIDef.h` loses its 10 `_pad_` members. An implicit copy copies pads too:
    with them in, the copy graded DIFFER(1619v1337).
  - The 13 landed sources whose preprocessed text changes all recheck RELOCATION_MATCH PASS (prep_landing siblings).
  - New opt-ins: PDCB_IMPLICIT_ASSIGN, STLP_TREE_ERASE, STLP_TREE_POSTINC, CCHARSTRING_CHAR_EQ, CPARSEDITEM_SYMBOL,
    CSTRINGPARSER_PEEKNEXTITEM, CTCINVENTORYMAP_REGIONINFO_CTOR, CHITPARAMETERS_EFFECTONHIT, CTHINGCREATUREBASE_LASTHANDEDNESS.
- **Landing:** `vland --land` (`logs/land.log`).

## Parked (`pending/2026-11-05_linux_hand778/`, each with its residue and next idea)
- **0x00ACCBF0 COptimisedPrimitive::PerformSubContraction:** DIFFER(1282v1264) after 17 grades. vertex1 and vertex2 sit in
  swapped registers, and our version reloads the map key where retail keeps it.
- **0x00E50910 CV_BeardyBaldyScript::WatchForQuestCardConditions:** DIFFER(1392v1328) after 20 grades. VC keeps a zero in ebx
  through the wait loop.
- **0x00839420 NThingHelpers::CreateEffectOnHit:** DIFFER(1304v1308) after 17 grades. The residue is the order of the locals'
  stack slots and the early-return epilogues.

## Names
Queued as NAME_FIXES (tag `land0511e`):
- 0x00E50E40 is `CV_BeardyBaldyScript::GoTalkToBarber` (landed as GoTalkToBeardyBaldy; the real one is 0x00E50650);
- 0x00AD71B0 is `COptimisedVertex::Kill` (landed as Remove, which is in neither PDB);
- 0x00839420 is `NThingHelpers::CreateEffectOnHit` (Ego_r.pdb).
