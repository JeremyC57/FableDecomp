# Landing 2026-11-05d: chunk 777 (3 of 5 ledger rows)

Authored by one expert agent (owner, 2026-11-05: continue authoring with one expert agent). The rows are the five smallest Fable
rows left in the groups ledger (1,045-1,272 bytes). All three landed rows are plain C++ and grade `genuine`.

## Landed

| Address | Name (from evidence) | Grade | Grades | Key spelling |
|---|---|---|---|---|
| 0x006303C0 | NUISystem::CUIDef::CUIDef (copy constructor) | RELOCATION_MATCH PASS | 3 | hand-written copy ctor, members assigned in retail order; new `NUISystem_CUIStateDef.h` |
| 0x005E8220 | CTCQuestCompletionUI::OnSerialise | RELOCATION_MATCH PASS | 6 | first map walk as a `while` loop with the iterator set at its declaration (one inline `_M_insert_overflow`) |
| 0x00AC92C0 | QuadricOptimiserInternals::COptimisedPrimitive::ComputeContraction | RELOCATION_MATCH PASS | 36 | same-TU noinline callee bodies; locals declared `vertex1, unique1, vertex2, unique2`; `!(cost > sub_cost)` |

- **Catalog names corrected.** The ledger names for 0x005E8220 (`CTCInventoryMapSerialization_TransferCharactersAndStats`) and
  0x00AC92C0 (`COptimisedPrimitive_ContractVertexPair`) were guesses. 0x005E8220 is vtable 0x0125589C slot 1.
- **Headers.** All changes are behind new sub-macros, and prep_landing's hdrcheck2 reads `default=same` on every changed header:
  - new sub-macros: FABLE_RETAIL_CTCTARGETINGPLAYER, _CTCHERORANGEDATTACKS, _CHEROCOMBATDEF_RANGED,
    _CTHINGCREATUREBASE_HEROCOMBATDEF, _STLP_SET_COPY, _STLP_VECTOR_RESIZE_OUTOFLINE, _SCOPED_PTR_RESET,
    _CUSERPROFILEMANAGER_SCOREBOARD and _LHHTTP2_SCOREBOARD;
  - new headers: CFloatRange.h, CHeroAbilityStatWeight.h and NUISystem_CUIStateDef.h.
  - All 6,285 landed sources that include them preprocess the same as before (ppcmp).
- **Landing:** `vland --land` (`logs/land.log`), 3 wins.

## Parked (`pending/2026-11-05_linux_hand777/`)
- **0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest:** DIFFER(1121v1122) after 19 grades, a register-choice
  residue. Next idea: same-TU noinline CryptoMatic and string range-constructor bodies, as the landed 0x0055F090 has.
- **0x00803540 CTCTargetingPlayer::UpdateTargetedPositionRandomWalk** (the ledger's CViewBase name was wrong): DIFFER(1108v1108)
  after 30 grades, an x87 scheduling residue around `fcos`.

## Names
- `ghidra_out/labels_landing_2026-11-05d.tsv`: 16 labels.
- 10 NAME_FIXES are queued (tag `land0511d`), among them:
  - 0x00789A70 CTCHeroRangedAttacks::GetPullBackAmount;
  - 0x00663340 CThingCreatureBase::GetPHeroCombatDef (4 wrong spellings);
  - 0x00AD4A70 ~CUniqueVertex;
  - 0x00AD3A10, the `set<unsigned long>` copy ctor, whose spelling conflicts with landed 0x0051C470's binding of 0x00513700.
