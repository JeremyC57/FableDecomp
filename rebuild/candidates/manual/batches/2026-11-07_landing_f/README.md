# Landing 2026-11-07f: chunk 830 (1 of 6 rows landed, 5 parked)

One expert agent authored six new ledger rows (all names proven from both PDBs or a vtable slot), sharing the lane with chunks 831 and 832.

| Row | Grade | Note |
|---|---|---|
| 0x00DD9A50 NScript::CQ_PrisonEscapeRescueMotherScript::ResetCells | **RELOCATION_MATCH + PASS, landed** | 4696 bytes incl. the 5-entry switch table; reloc_bindings had it as CV_ChapelOfEvilScript::ResetCells (queued) |
| 0x0083E1F0 CThingScriptUIDReplacer::CThingScriptUIDReplacer | DIFFER(1984v1973), 214 lines | index / iterator registers swapped in the second replace loop; a 15-byte loop pad |
| 0x0090D030 CAIStateGroup_MinionGossip::Update | DIFFER(3243v3269), 577 lines | frame 0x5D0 v 0x580; retail expands GetPOriginator<CThingAICreature> twice and calls 0x0090DF30 once |
| 0x00931390 CAIStateGroup_MinionReactToCriminal::Update | DIFFER(3836v3863), 809 lines | `result` in memory (retail ebx); inline CTCTalk binary search; loop alignment |
| 0x007C1000 NThingShot::CTCShotDefinitionBase::OnCollisionProper | DIFFER(4655v4510), 974 lines | call sequence identical; CDefPointer release form and deflection maths scheduling |
| 0x007542B0 CTCLook::OnAppearanceDraw | DIFFER(2268v2266), 557 lines | frame slot assignment only (matrix slot order, CPrimitive temp at 0x110 v 0x34) |

- **Headers:** four opt-ins (CMainGameComponent `_GETWORLD`, CThingManager `_ALLOCATEUID`, CMatrix3x4 `_SETFROMVECTORANGLE`, CRightHandedSet
  `_INIT`) are used only by parked rows; they stay in `pending/2026-11-07_linux_hand830/hdr/`.
- **Names:** 14 queue lines (`land0711f`): ResetCells, GFUHUGEToCharString 0x0099F930, GetEyeGraphic 0x00662160, GetPOriginator<CThingAICreature>
  0x0090DF30, CRightHandedSet::Init 0x004BBE10 (landed as Set), GetTCDriver<CTCDExplosion> 0x007512C0, the __fastcall IS_THING_ALIVE template
  0x0065F8CA, and eight landed-row label corrections (ProcessRaces 0x00DD9900, the CEnginePrimitiveMorphedStaticMesh ctor / dtor, the map dtor
  0x00CDBFB0 with three bound spellings). 0x0129BA3C DELTA_NOTIONAL_ZERO was already queued.
