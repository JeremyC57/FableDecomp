# NF385 naming hand-back (2026-11-24)

Targets: 581 shared stand-in rows (TARGETS.tsv). Result: 21 ACCEPT (batches/b1), 1 HOLD (batches/b2, ruling R2),
559 skipped with a reason (STATUS.tsv). Every new name is the exact PDB spelling. None is a descriptive made-up name.
About 45 grades used out of the ~150 budget.

## Batches
- `batches/b1`: a land_scripted run dir with 21 rows. It holds:
  - rows/ and tests/ (5 respelt tests, TESTS.tsv), orig/, head/payload.json
  - RESULTS.tsv, UNITS.tsv, SELF_SYMBOLS.tsv
  - TABLE_CHANGES.tsv: one delete, `?BaseActivate@CModeChargeForcePush@@QAEXXZ` at 00861d00. Its only witness was 008d6980.
  - evidence/ and logs/
  - Gates are in RESULTS.tsv reason / work/gates_b1.txt.
  - Each row is its own unit (no Option S owner rows).
- `batches/b2`: 00b1ae00 HOLD (see R2).

## Gates (all 21 rows)
- vland HEAD and new: MATCH / RELOCATION_MATCH, test PASS, at one LANDVERIFY_WORK path per run (logs/).
- objsec: ALL_EQ under the self rename (evidence/objsec_*).
- External symbol sets equal under the renames, with no new `??_7` / `??_R` (evidence/CHECKS_*).
- pk_all + pickany_vs_retail, HEAD v new: no copies NEQ, 0 reach flips (evidence/pk/, tools/reachflip_coordinator.py).
- /FORCE link:
  - The 15 objects of runs p1+b1: duplicates 7 -> 0.
  - b2+b3: 1 -> 0.
  - b4+b5: 2 -> 0.
  - Nothing that HEAD resolved is left unresolved. The only new unresolved names are the PDB names `?OnActivate@CModeStandBase@@UAEXXZ` / `?OnDeactivate@CModeStandBase@@UAEXXZ`. At HEAD the stand-ins were unresolved instead, and their owners 008621a0 / 00861d00 lack the PDB leaf in self_symbols (NAME_FIXES).
- Census: each old spelling is named only by the changed rows (evidence/census_b1.txt). The CComponent_SetParentColour stand-in stays defined by 00411980 and has no referencing row.
- apply_table --dry: 0 misses, multi-target 1 -> 1.

## Accepted rows: the two sources for each
Source A for every row is both PDB name sets (FableWin + Ego_r), except where marked "FW only". Source B is given below.

- **008d6960 / 008d6980** `NCreatureMode::CModeChargeForcePush::OnActivate / OnDeactivate`.
  - B: retail RTTI vftable 0x01268328 slots 5 / 6 = FableWin vftable slots 5 / 6.
  - The base calls now name `CModeStandBase::OnActivate / OnDeactivate`. These are existing table lines: 008621a0 (witness 008d7270) and 00861d00 (witness 008e7850).
- **0052cbf0** `NUISystem::CChangingStateComponent::AllChildrenChangedState` (protected virtual).
  - B: slot 139 of the RTTI CChangingStateComponent vftable 0x01245DE4 and of about 40 derived NUISystem vftables = FableWin slot 139.
- **00536189** `NUISystem::CList::AllScrollingChildrenDeleting`.
  - B: RTTI CList 0x012462E4 / CFrontEndList 0x01249224, slot 162 = FableWin.
- **00547be0** `NUISystem::CMovie::GetSize`.
  - B: RTTI CMovie 0x0124881C, slot 23 = FableWin.
- **0054e5a0** `NUISystem::CViewport::GetSize`.
  - B: RTTI CViewport 0x01249A5C / CScrollingViewport 0x012468D4, slot 23 = FableWin.
- **00403fc0 / 0044b7e0** `CPersistContext_TransferableComponent<bool> / <unsigned __int64>::TransferIn`.
  - B: RTTI vftables 0x0122D6C4 / 0x01231C80, slot 1 = FableWin.
  - 00403fc0 uses the shared header `engine/CPersistContext_TransferableComponent.h` with an explicit specialization.
- **005cbe9d / 005cbeba** `CPersistContext_TransferableComponent<CDefIndex>::TransferIn / TransferOut`.
  - B: RTTI 0x01253F18, slots 1 / 0 = FableWin.
  - The callees keep their landed stand-in symbols. Their real names are in NAME_FIXES.
- **00b7ec30 / 00b7ece0** `CWaterPatchTesselatorBase<CTVertexWaterBackground / CTVertexSeaBackground>::GetSizeOfVertex`.
  - B: RTTI 0x012A3334 / 0x012A334C, slot 3 = FableWin.
  - The Sea name is FW only (absent from the Ego_r set, which was linked with ICF).
- **009fea00** `CGraphicInfoFrame::CFrameData::ReleaseTexture`.
  - B: RTTI 0x0129C89C, slot 10 = FableWin.
- **006bbed0 / 009efeb0** `CConsoleCommand<CGameTimeManager / CConsole>::CallCommandFunction` (FW only).
  - B: RTTI 0x0125D668 / 0x0129C4E0, slot 5 = FableWin slot 5.
- **00618170 / 0074d5d0** `std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory> / <CDefIndex>::clear` (FW only).
  - B: the FableWin TransferVector<T> body calls CheckCRC and then clear, in the same order as the retail caller.
  - The retail caller 0x0074DE50 is TransferVector<CDefIndex>: its landed callee is GFSerialiseVectorBinaryIn<CDefIndex>, and both PDBs list it.
  - The STLport spelling comes from the repo STLport headers (EXTRA_RULES STLport section).
- **00d25d20 / 00d25ef0** `NScript::CQ_DragonBossFightScript::CDBMinion / CDBSummoner::OnPredicateFail` (private virtual).
  - B: CQ_DragonBossFightScript::Main (0x00D255E0) registers Alloc 0x00D25C60 under the literal "DBMinion" and Alloc 0x00D25E30 under "DBSummoner".
  - Those Allocs install the vftables 0x012CB78C / 0x012CB7A8 (no RTTI on script classes). Slot 5 of each is 0x00D25D20 / 0x00D25EF0.
  - The FableWin vftables stored by ??0CDBMinion / ??0CDBSummoner hold OnPredicateFail at slot 5. The shared slots 4 / 6 (CEntityScriptBase OnPersist / OnInterrupted) line up with retail.
  - The two bodies are not byte twins: they use parent offsets +0x70/+0x90 and +0x74/+0x94.
- **007a6e20 / 007a9870** `CTCBattleCharge / CTCAssassinRush::SetupFacingDirection` (private).
  - B: in retail, RTTI CTCBattleCharge vftable slot 9 (0x007A8BE0) calls 0x007A8700, which calls 0x007A6E20. CTCAssassinRush slot 9 (0x007AB190) calls 0x007AAC50, which calls 0x007A9870.
  - FableWin has the same path: slot 9 FrameUpdate > RushForwards > SetupFacingDirection for each class (tools/reach.py, tools/reachfw.py).
  - Ego_r object order (tc_battle_charge before tc_assassin_rush) matches the retail address order.

## Held
- **00b1ae00** `NParticleEngine::CPSCLight::GetClassTypeName`.
  - Sources: RTTI 0x012A0624 slot 3 = FableWin slot 3, plus both PDBs.
  - The row also fixes its string literal: retail pushes "CPSCLight", HEAD has "Bounding Sphere". objsec therefore differs in the literal. This is ruling R2.

## Rulings needed
- **R1:** CRetailTree erase pairs (154 rows). QUEUE_PROPOSALS q2 has the exp1 emission-order experiment.
- **R2:** 00b1ae00, the literal fix (q10).
- **Funclet tracker artifact:** q1 and the data +offset rows (q6). These are tracker fixes, not naming work.
  - 155 funclet thin rows, plus 126 rows (mostly in the vendor range) whose only sharer is their own funclet, plus 2 data rows: 283 in all.

## Skips by reason (STATUS.tsv, 559)
| Reason | Rows |
|---|---|
| Tracker artifact | 283 |
| Ruling R1 (RetailTree) | 154 |
| No source | 81 (unreferenced twins, or vendor rows with no PDB name) |
| No second source | 30 (referenced only from unnamed / stand-in callers; vt.py finds no vftable slot; no RTTI call-path sweep was run on these) |
| Ctor port (??_G) | 4 |
| Queued port (~CEngineCamera, __find_if) | 2 |
| Unit work (_Push_heap) | 2 |
| Already right / partner pending, own name unproven, stay-off | 1 each |

## Disclosed warnings and things left open
- Five rows gain a new advisory vland warning, `SHARED_HEADER_AVAILABLE`, because each keeps its row-local model for a name-only change:
  - 008d6960 / 008d6980: CModeStandBase
  - 00618170: CHeroLogBook
  - 007a6e20: CTCBattleCharge
  - 00b1ae00: CPSCLight
- The warning is noted in each RESULTS reason. The header ports are queued as q8. Strict mode would reject these rows until they are ported.
- 00d25d20 / 00d25ef0 already carried the CQ_DragonBossFightScript warning at HEAD.
- 00619890 (TransferVector<ECategory>) still references its own stand-in `?clear@?$vector@W4ECategory@@@@QAEXXZ`. That reference is unresolved both at HEAD and after this hand-back.
  - Moving it is a unit port: its own symbol and its GFSerialise callee symbols respell with the element type.
  - Not done here; recorded in NAME_FIXES.
- No reloc_bindings.tsv line targets any of the 22 changed addresses (checked by target column). apply_table --dry was rerun on the final 21-row batch: 0 misses.

## Method notes
- Twins are told apart only by RTTI slot identity, by string-literal registration, or by an RTTI-rooted call path matched in FableWin. Byte identity never decided a name (no-ICF ruling).
- Vendor rows (PK, CODA, LHA, DirectShow, Ogg/Vorbis, Mix) have no PDB names and stay unrespelled (NF372).
- Locks held by NF385: all 22 changed rows (21 ACCEPT + 00b1ae00). They are kept for landing (evidence/locks.log).
