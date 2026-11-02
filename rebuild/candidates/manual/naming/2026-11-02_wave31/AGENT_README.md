# Naming pass wave 31 (2026-11-02): 94 landed rows edited + 3 shared headers; recheck of the family rows and every landed includer

## Inputs and index
- Brief naming/briefs/BRIEF_W31.md; QUEUE_W31_input.tsv (152 rows: 114 carried, 20 c702 + 18 c703 appended mid-wave); DEFERRED_W30_INPUT.tsv.
- idx/full.jsonl is HEAD: the wave 30 after.jsonl re-indexed for the 47 rows the wave 30 landing touched (only 0x007F0B10 differed: the
  coordinator's DoDieFX respell) plus chunk 702 (idx/delta_c702.jsonl, 10 rows) and chunk 703 (idx/delta_c703.jsonl, 10 rows), found by modification time after the
  coordinator's notes (none includes an edited header); nothing newer than idx/.c703_marker at hand-back.
  idx/after.jsonl is HEAD plus ov_final.

## Commands (from $S/naming/w31, S exported, $S/wine/env.sh sourced)
1. `python3 scripts/compose31.py ov_final` runs scripts/hand31/h31_<fam>.py on fresh repo copies (headers from hdr/); writes runs/<fam>.rows.txt
   and ov_<fam>/.
2. `bash scripts/rkfinal31.sh` (two parts, 1596 rows: the first 61 family rows + runs/hdrdep.rows.txt) and the supplementary recheck runs/rk_sup.*
   (34 rows: the c703 / srtf families + 0x00658020 after its last recompose; runs/sup.rows.txt);
   `bash scripts/chain31.sh` then indexes ov_final, builds after.jsonl, qscan31, mkq31 (+ mkq31_notes), mkdef31 (+ runs/deferred31_manual.tsv),
   mkbind31 (+ runs/reloc_bindings.after.tsv), bindings_multi_check, mkren31 + fixren31, mklabels31 + fixlab31.
- scripts/ovinc.py (wave 31): an edited overlay header that includes another edited header is shadowed with its includes rewritten (CTCDoor.h ->
  CTCBase.h). runs/hdrdep.rows.txt = includers.py of fable_string.h (1309: runs/inc_fs.txt), engine/CTCBase.h (249: runs/inc_tcb.txt),
  engine/CTCDoor.h (15: runs/inc_door.txt) -- real #include lines, direct or through another header.

## Families (compose order; runs/<fam>.rows.txt)
- **fs (header + 2 tests)** -- fable_string.h: `CCharString(const CCharString& other); // 0x0099EC30` (DECISIONS_W30 D2). Tests 0x00401C00 /
  0x00A45150 define the copy ctor as a bitwise stub (they copy a CCharString and stopped linking).
- **csm (4)** -- the D2 setters 0x008A41B0 / 0x008A4730 / 0x008A6570 / 0x008A6840 -> ?X@CGameScriptInterface@@UAEXVCScriptThing@@VCCharString@@@Z.
- **tcb (header + 3)** -- engine/CTCBase.h retail opt-in FABLE_RETAIL_CTCBASE (28 slots, CTCBase : CBaseClassNonCopyable); 0x006868E0,
  0x00713740 (was v00..v12 padding), 0x007F0890 move their local CTCBase to it (D5).
- **tcd (13)** -- ~CTCBase binders re-authored as the implicit ??1 of their CTC class (CTCActionUseBase, CTCSmashableBase, CTCExpression,
  CTCDecapitation, CTCResurrectionItem, CTCDRumble, CTCHasName, CTCWeapon, CTCClock, CTCBed, CTCHeroRangedAttacks, CTCFish, CTCWillResponse).
- **dgp (5)** -- NPlayerGui::CDrawBase::GetPlayer pair 0x00643D7C / 0x00643D84 (were T::Run) + callers 0x00657D90 / 0x00658020.
- **c702 (1)** -- 0x00643FC8 CDrawAbilityButtons::GetAssignableSpecialMoveIndexForButtonDef (protected virtual const) -> const 0x00445EE0.
- **door (header + 2)** -- engine/CTCDoor.h retail opt-in FABLE_RETAIL_CTCDOOR; CTCDoor::Copy 0x007305A0, ~CTCDoor 0x00731330.
- **pek (3)** -- NParticleEngine::CParticleEmitter in 0x00AEE8C0 (Kill), 0x00AEE9F0 (Update), 0x005440B0.
- **hos (3)** -- private ClearScoreboard in 0x00562E70 / 0x00563130 / 0x00563410; their bare Leafs -> emitted symbols.
- **d3 (3)** -- D3: 0x0057ABD6 / 0x0057AE53 inherit CTCBase::GetThingManager() const (0x0057ABD6 also -> PDB GetPMaritalHome signature);
  0x008976F0 -> ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ.
- **libpk (23)** -- D7: CVtable_012bf050 (virtual dtor) over the own row, its ctor and 20 reverb rows; CVtable_012bf0c8 class key.
- **c703 (9)** -- chunk 703 priorities 2-4: 0x004C8BA0 / 0x0082FD90 WorldUpdateBulletTime virtual (UAE; 0x004C8BA0 on engine/CTCBase.h's retail
  branch, slot 10 BulletTimeFrameUpdate); ClearAllPrimitives 0x004BF9A0 protected (IAE) in its 3 binders + inline in 0x004C0000; own rows 0x004BF9A0 /
  0x004BF960 re-authored as NGraphicAppearance::CTCGraphicAppearance members on the 0x004BF8F0 model; 0x004C5690's allocator class-keyed (V), and
  0x004C0000 uses std::list<CPrimitive> (the MemberAt38 / SubObj / CPrimitiveListObject bindings retire).
- **srtf (24)** -- the 0x004C7590 DECISION: ?SetAsRenderedThisFrame@CThing@@QAEXK@Z (Ego_r block order + FableWin; DECISIONS_W31 D9) on the own row and
  all 23 binders (seven spellings); 0x007CE370 re-authored as ?OnInitialActivate@CTCElectrocutedEffect@@UAEXXZ.

## Results
- Recheck (logs/recheck_final.log; 1626 distinct rows = runs/final_plus.rows.txt: 94 family rows + 1544 header includers, overlapping):
  1595 RELOCATION_MATCH + 27 MATCH, all PASS; the 4 failures are pre-existing at HEAD without the overlay (DECISIONS_W31 D8: 0x00692230 / 0x006922C0
  DIFFER(40v30), 0x004D9861 DIFFER(36v33), 0x009E1440 TCC_FAIL).
- QUEUE_W31.tsv (152): 11 respelled, 10 respelled-partial, 17 done, 114 deferred. BINDINGS.tsv: 42 RETIRE, 13 KEEP, 10 ADD, 0 CONFLICT;
  bindings_multi_check 0. RENAMES.tsv 25, LABELS.tsv 22, DEFERRED.tsv 708 (113 queue, 10 manual, 585 carried; 1 resolved).
- DECISIONS_W31.tsv D1-D13.
