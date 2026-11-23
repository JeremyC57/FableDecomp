# NF289 notes (2026-11-23): T1 standin_alias, continuation of NF287, hand-back 1

## Summary (counts from STATUS.tsv / APPLIED.tsv)
- **65 of 175 ENTRIES addresses reached**: entries 1-60 in ENTRIES order, plus 66, 80, 95, 133 and 138 pulled forward by the stale-witness
  scan. 110 entries are open (61-175 except those five).
  - **done 4**, 4 rows changed (rows.txt):
    - 0x00449B60 GetMultiplayerColour (binder 0x00644424; it also fixes GetPlayer 0x004498C0 on the same line, ruling 1);
    - 0x00B4CED0 ClipBoundingBox (binder 0x00B92070);
    - 0x00415BD0 GetWidth (binder 0x009F35D0);
    - 0x0044A530 CPlayerManager::CreatePlayers (binder 0x004166A8, through one new add-only header opt-in).
  - **done-already 4** (table only): 0x00AC96E0, 0x0066B120, 0x004C7A80, 0x00CD3640.
  - **done-already, pending decision 1, 1** (table only, source evidence): 0x00EE2DB0. Its TABLE_CHANGES line is marked CONDITIONAL.
  - **Outside ENTRIES, 1** (ruling 1): 0x004498C0 GetPlayer, on the same line as entry 21. It has its own STATUS line.
  - **done-partial (table only) 1**: 0x008F9CA0.
  - **skipped-port 55**: entries 1-60 not otherwise done. Each binder was opened (tri3 / tri4 reads), and the specific reasons are in the STATUS notes.
  - No rejects, no decisions held.
- **Recheck** (recheck_final.log, one vland `--recheck-landed` run with hdr/ as the overlay): 4 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
  - The warnings are pre-existing: SHARED_HEADER_AVAILABLE on the rows' old local models, and one RELOC_NAME on 0x00644424's untouched GetSpiritScoreText stand-in.
- **One header opt-in:** hdr/engine/CPlayerManager.h (hdr/README.md = HDR_README.md).
- No landed source or test `#include`s a changed row.

## Order (E2)
1. **Stale-witness scan.** tri4.py patches tri3: it flags a binder when the stand-in's own identifier, not just any token, is missing from comment-stripped code (evidence/tri4_all175.tsv, all 175 entries).
   - I added the binders whose stand-in is a template / operator spelling, since a source scan cannot judge those.
   - That gave 22 binders. 20 were compiled at HEAD and checked with objbind.py (evidence/objbind_set1.tsv). 0x00EE3EE0 and 0x00676740 fail clang in their own lines (not D1), so they were not compiled. Their objbind lines are marked NO_OBJ, and objbind.py now says NO_OBJ for a missing object; entry 163's witness 0x00676740 is not judged.
2. **In-place pass.** It covered the `X_Methods : RealClass` binders and the stand-ins on a row's local real-class model. I read each binder's Methods struct, header and access before classifying.
3. **The rest of entries 1-60** were classified as T3 readings: tools/t3read.py plus per-entry notes from tools/t3notes_1_60.py.

## Done rows (proof per row: evidence/proof_r123.txt, proof_r4.txt, link_r1..r4.txt)
- **0x00644424 NPlayerGui::CDrawSpiritScores::Draw**
  - Before: its `PlayerManagerMethods : CPlayerManager` (flat header) declared GetPlayer and GetMultiplayerColour.
  - Change: it defines the existing opt-in `FABLE_RETAIL_CPLAYERMANAGER_MULTIPLAYERCOLOUR` and adds `class CRGBColour;` ahead of it, because the opt-in spells `class ::CRGBColour`. The two stand-in members are dropped; GetSpiritScoreText stays, since it has no header declaration and is not my entry.
  - Line count and every later line are kept; two comment pairs were merged.
  - objsec ALL_EQ under the two renames. Exe calls: 0x0064445E (GetPlayer) and 0x00644480 (GetMultiplayerColour).
  - Link: the new set resolves GetPlayer against 0x004498C0. `?GetMultiplayerColour@CPlayerManager@@...` stays unresolved, exactly as for the existing PDB binder 0x0064C869, because the owner row 0x00449B60 defines only its stand-in self `?GetMultiplayerColour@CPlayerManager_Methods@@...` (NAME_FIXES; owner self port, T3).
- **0x00B92070 CEngineSceneGrid::PreloadPrimitives**
  - Change: ClipBoundingBox(const C3DBoundingBox&, bool&) const is declared in the row's local CShadowClipVolume, with `class C3DBoundingBox;` on the same line. The call is `volume.ClipBoundingBox(...)`, and the CShadowClipVolumeMethods stand-in is now three comment lines.
  - objsec ALL_EQ.
  - Link: the call resolves against 0x00B4CED0.
- **0x009F35D0 CSurface::CanDoProperSignedConversion**
  - Change: GetWidth() const is declared beside GetHeight() in the row's local CSurface, and the CSurface_Methods line is a comment.
  - objsec ALL_EQ.
  - Link: the call resolves against 0x00415BD0.
- **0x004166A8 CMainGameComponent::CreatePlayers**
  - New add-only opt-in `FABLE_RETAIL_CPLAYERMANAGER_FLAT_CREATEPLAYERS`. It is a declaration only, in the flat branch, which has only pinned asserts. The binder defines it and drops its CPlayerManager_Methods member.
  - equiv_check over 2,112 includer rows: 0 differ. The control flags 493 (evidence/equiv_r4.txt).
  - objsec ALL_EQ.
  - Link: the call resolves against 0x0044A530.
- **Names.** Every new spelling is in both PDB name sets (evidence/pdbq.txt, tools/pdbq.sh); exe call sites are in evidence/xsites_r123.tsv; and every one is already bound at its address by another landed row.
- **Census** (evidence/name_census.txt): each name is defined out of line only by its own self row.
- **Tables.** No table emitted (`tables H=0 N=0`). Tests unchanged.

## Table-only entries (no row changed)
- **0x00AC96E0, 0x0066B120, 0x004C7A80, 0x00CD3640.** Each witness's HEAD object references the PDB spelling and not the stand-in (objbind), and a comment-stripped codegrep finds no landed code naming the stand-in identifier.
  - The CUnprovenMapKey_00CD3640 name survives only in 0x01224C6D's standalone test, as a stub model (D2).
  - Their stale lines are deleted. One PDB line is added (0x00AC96E0 had none).
- **0x00EE2DB0 (source evidence only).** 0x00EE3EE0's HEAD source declares `NScript::CV_TravellingHeroesScript::PlayCutscene(CCharString)` and its line 21 records the old respell. It could not be compiled, because its clang precheck fails in its own line 143 (not D1). The PDB line already exists (witness 0x00EE3FC0). Please confirm with an object at landing.
- **0x008F9CA0 (partial).** Witness 0x008FA000 is stale and its line is deleted. 0x008F9E60's live local CTimedTrigger stand-in stays T3; NF287 doubts its identity.
- **Scan results for the next agent** (evidence/objbind_set1.tsv), in entries 61-175:
  - Entry 17 (0x004DDF0C, inside 1-60, skipped-port) is live in 0x00580371, and its identity is doubtful (see its STATUS line).
  - Live stand-ins, which need ports: 0x00AD64C0 (binder 0x00ACA450; 0x00ACA830 is the PDB self), 0x00448380 / 0x006B8E20 / 0x00825090 / 0x00827F80 / 0x00A7FAF0 / 0x00AE98E0 (the FableHeap_<addr> pair operator= family).
  - Already PDB:
    - binders 0x00431493 (entries 104 / 105), 0x004A3740 (112), 0x008A8570 (113) and 0x0086E7C0 (170);
    - the stand-in witnesses of those entries are the other binders, which are live.
  - A third spelling not in the table:
    - 0x006C5E90 (entry 112) binds `...UCThingList@N_004ab4f0@@...`;
    - 0x0090EE20 (entry 102) binds `...N_006af2b0@@UCMessageEventFilter_IsForThing@@...`. Its identity is doubtful, since the filter types differ from the PDB.
  - 0x00433670 (entry 89) binds `??0CUIDef@@QAE@XZ`, a global struct standing for NUISystem::CUIDef. Its stale `??0CClass_0062fe60` line can be deleted when 89 is reached.
  - Entry 129's only witness 0x007F53A0 is locked by NF288 (skip it).

## T3 readings worth a later unit (STATUS notes)
- **Flat-header add-only opt-ins would close these cheaply:**
  - entries 8 / 9: CMemoryDataOutputStream GetLength / PeekData, binders 0x00A99840 / 0x00A9BA70. The generated flat branch's FABLE_STATIC_ASSERT is unpinned, so run pin_asserts first.
  - entries 37 / 38: CTCBuyableHouse SetRented / SetOwnedByPlayer, binder 0x00895ED0. The declarations exist only in the retail branch, and the flat branch is marked "generated, do not edit".
  - entry 44: CTCDoor UpdateOpenStatusTriggerTypeScript, binder 0x007311F0. The declaration is only in the FABLE_RETAIL_CTCDOOR branch.
- **Self ports**, where the binder's own function is on the stand-in class:
  - private callees: 18 / 19 (CPlayer_Methods; CreateCharacter is private), 26 (SetupRenderStates) and 40 (UpdateUsableMeleeCombatAbilities);
  - 36: the free stand-in self.
- **Same-TU stand-in body:** entry 27, binder 0x00A07900 (D3).
- **Identity doubts:**
  - 20: the binder's ShowOnScreenMessage(CWideString, long, long, long) against the PDB's AddScriptScreenText(CWideString, C2DVector, CRGBColour, CCharString);
  - 29: three stand-ins and an empty self leaf;
  - 58: the PDB has a static fastcall CTCInventoryItem::ClearCachedDefs, but the binder calls a thiscall CTCInventory member.

## Decisions needed
1. **0x00EE2DB0** is table-only on source evidence alone: the precheck rule kept me from compiling 0x00EE3EE0. Accept it, or have the coordinator compile the HEAD object first?
2. **0x00644424 adds `class CRGBColour;`** before its includes. It is a forward declaration of the class the row already defines locally, needed because the shared opt-in spells `class ::CRGBColour`. objsec is ALL_EQ, including .drectve. Is that acceptable as part of "use an existing opt-in"?
3. **0x00449B60's owner self row** defines only `?GetMultiplayerColour@CPlayerManager_Methods@@...`, so both PDB binders (0x0064C869 at HEAD, 0x00644424 now) stay unresolved in a link with it. Is this already in the T3 self-port queue, or should it be added (NAME_FIXES line)?

## Rule notes
- **Locks, prechecks and timeouts.**
  - Every compiled source was prechecked in an earlier call, after its lock (work/pre_*.log).
  - Every landed test was prechecked before its lock was claimed, including owner rows compiled only for link checks. Nine tests passed `cl /Zs` under D1 / D1b (work/zs_t_set1.log, toolchain-header failures only), and a planted-error control fails (work/zs_ctl.log).
  - Every grade has `timeout 900` on its own launch line (gr.sh, lk.sh, rf.sh, zsrun.sh). equiv_check ran under `timeout 3000`; it is not a grade.
- **No forbidden operations.** No sleep, no foreground waits, no rm (clean.py), no git, nothing written under the repo, no `__asm` / `naked` / `_emit`.
- **orig/ copies.** orig/<addr>.cpp holds the HEAD copy of each changed row, and orig/hdr/ holds the header base.
- **Locks still held:** the 4 changed rows (0x00644424, 0x00B92070, 0x009F35D0, 0x004166A8). All others are released.
- **Reading list.** The prefix, the standing brief, GOTCHAS.md (all 511 lines), EXTRA_RULES.md (all), ASM_RULES, COORDINATION, the landing READMEs nf281..nf287 and NF287's hand-back were read in full before the first compile. The nf28x_handback folder named in the standing brief no longer exists.
