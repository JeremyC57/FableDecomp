# NF330 (2026-11-24): NF324 Q1 remainder -- the thin-family flat-header rows on generated opt-in derived models (81 of 95 rows, 55 headers)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Route: NF329's landed route (batches/2026-11-24_landing_nf329_b3
README, rulings D-a..D-g) with NF329's tools copied into tools/ (paths retargeted). Nothing was written under the repo and no git was run.
Reading list read in full before the first compile (BRIEF_NF_BATCH, COORDINATION, ASM_RULES, EXTRA_RULES, GOTCHAS, the four nf329 landing READMEs,
the nf329 hand-back NOTES / LESSONS / STATUS). The container restarted once before any compile, lock or background job (nothing to relaunch).
Running log of the session: work/notes_log.md.

## Result (STATUS.tsv: one line per ENTRIES address, 95)
| status | rows |
|---|---:|
| done | 81 |
| skipped-port | 14 (measured reason per line; see "Not done") |
| held / rejected | 0 |

| batch | rows | classes (headers) | proof |
|---|---:|---|---|
| b1 | 20 | CSubDefClassBase family: 17 Def models (CSmashableDef .. CAIScratchpadDef), base CSubDefClassBase (FABLE_RETAIL_DEF_CLASS_BASE, 0x25) | 20 ACCEPT: 18 MATCH, 2 RELOCATION_MATCH; objcheck ALL_EQ (no libcp) |
| b2 | 26 | CCreatureActionBase family: 13 models on CCreatureActionBase (FABLE_RETAIL_CCREATUREACTIONBASE, 0xA8) + the 2 SheatheItemToInventory_For* models on CCreatureAction_SheatheItemToInventory (0xB0) | 26 ACCEPT MATCH; objcheck equal + libcp |
| b3 | 18 | ReleaseAndLoopDivineWrath / UnholyPower on NF329's landed BuildUp* models, BlockRespond on NF329's BlockBase (0x124), TrollAngryGroundPound / TrollProjectileAttack / TrollTunnel on the landed TrollWhackGroundBase _CLASS model | 18 ACCEPT (17 MATCH, 1 RELOCATION_MATCH); objcheck equal + libcp |
| b4 | 11 | CAIStateGroupBase family (KidWanderNearKids, LightHomeCandle + LightWorkCandle chained on it, ChickenBeg, MinionReactToNoise, MinionLeaveRegion) + singles CPhysicsFacesSegmentLeaf, CParticleSimulationRenderData, CEngineInternalPrimitiveLight / ShadowedSpotLight | 11 ACCEPT (9 MATCH, 2 RELOCATION_MATCH); 10 ALL_EQ, 1 + libcp |
| b5 (decision D1) | 4 | GazeOutside / GazeAtHome on CAIStateGroup_Gaze, plus an add-only `FABLE_RETAIL_CAISTATEGROUP_GAZE_RTTI_DECLS` block inside the LANDED Gaze model | 4 ACCEPT MATCH, ALL_EQ |
| b6 (decision D1) | 2 | BanditKingAdvance / BackOff on the Strafe PROTOTYPE models, plus add-only `FABLE_RETAIL_CCREATUREACTION_STRAFE{FORWARD,BACKWARD}_RTTI_DECLS` blocks | 2 ACCEPT MATCH; equal + libcp |
Each batch dir is a land_scripted.py run dir (RESULTS.tsv, rows/, head/payload.json + HEAD_FROM_REPO.txt, TABLE_CHANGES.tsv (empty)) plus HEADERS.tsv,
hdr/engine (full copies), hdr/diff, orig/ (headers + HEAD row sources), PROBES.tsv, EQUIV_CHECK.tsv, EQUIV_CONTROL.tsv. HEADERS_<batch>.tsv at the top.
No class straddles batches; the LightHomeCandle -> LightWorkCandle chain is one batch (b4). Landing order: each batch's hdr/ before its rows; b3
needs NF329's landed BuildUp* / BlockBase models (in the repo). 0x00908120 (b3): its landed HEAD source carries a retail byte-dump comment, so its
HEAD entry is left out (head/HEAD_FROM_REPO.txt, NF329 D-f form); the new source drops that line.

## Headers (hdr/engine = full copies of the 55 edited headers; orig/hdr_<X>.h = the repo copies used; all still equal the repo at hand-back)
- 51 generated `#ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` derived models (tools/genhdr2.sh -> tools/genmodel.py, NF329's generator plus: per-family
  base options in tools/families.tsv; `--preinc` for a base header that needs another included first (TrollWhackGroundBase needs CCombatActionBase.h);
  a non-public virtual destructor (`??_G..@@MAE`) is declared with its access, as retail user-declared it (CPhysicsFacesSegmentLeaf, PDB
  ??1CPhysicsFacesSegmentLeaf@@MAE@XZ in both sets)). Flat model kept as the `#else` branch, moved flat asserts pinned (lane_merge/pin_asserts.py).
  CEngineInternalPrimitiveLight's member list was hand-merged: its flat model carries NF151's `_TYPED` member opt-in, which the generator would have
  flattened into both member sets; the VM block keeps that `#ifdef _TYPED / #else / #endif` as written, with the asserts split the same way.
- 3 add-only `_RTTI_DECLS` blocks inside landed base models (D1): CAIStateGroup_Gaze (slots 3 / 10 / 13 / 15: InitCreaturePosition, OnReplace,
  InitStateGroup, InsideAtNight), CCreatureAction_StrafeForward / StrafeBackward (slots 12 / 73: Initialise, IsToFaceTarget).
- Base sizes come from each base model's own sizeof assert (BlockBase is 0x124, not 0x114: the first BlockRespond draft failed its precheck on it).
- Proofs (evidence/): hdrcheck.txt 55 SAME (drop each opt-in and undo the pins = the repo header byte for byte; tools/hdrcheck.py now matches the
  opt-in's #else by #if depth and handles an inserted _RTTI_DECLS block); hdrcheck2.txt default=same on all 55, only the new macros DIFF;
  pin_asserts applied (pins kept in hdr/); declsrc.tsv: 204 declarations, each RTTI-placed + in a PDB name set (130 both, 74 FableWin only), 0 in neither.
- Probe (tools/vprobe2.py + vtcmp.py, strict summary tools/probesum.py, which now accepts the ??_E v ??_G alias at the destructor's slot whatever the
  slot): one consolidated run of the 51 generated models + the Gaze base on the frozen tree, plus the 2 BanditKing models after the Strafe opt-ins:
  53 OK / 0 FAIL (evidence/probe_summary.tsv, probe_final.log, probe_p6_banditking.log): retail slot count == emitted, every slot SAME except the
  slot-0 (Leaf: slot-3) ??_E / ??_G alias. The Strafe bases are proved through their derived models (their declared ctor keeps `new X` from emitting
  the vftable). Negative controls (evidence/probe_neg1.log, probe_neg2.log): Greet without IsPreventingLookTurning -> slot 50 DIFF; GazeAtHome without
  GAZE_RTTI_DECLS -> slot 3 DIFF; BanditKingAdvance without STRAFEFORWARD_RTTI_DECLS -> slot 12 DIFF; CSmashableDef without GetSizeofClass -> abstract
  (compile fail).
- equiv_check (stock lane_merge/equiv_check.py via tools/eqrun.sh, S in my folder; include-closure scope): tools/includers.py closure = 55 headers +
  5 headers that include them (CAIStateGroup_GazeAtWork.h, CTCOracleMinigame.h, CEngineParticleSimulationRenderData.h, CCreatureAction_ScreamerAdvance.h /
  BackOff.h), 83 landed rows / tests (evidence/equiv_rows_includer_closure.txt). repo v work/tree: 0 lines (evidence/equiv_check.tsv). Planted control
  (tools/plant.py: a typedef on the default `#else` branch of each generated header, just above each _RTTI_DECLS block in the base headers): 80 lines,
  71 of the 83 rows flagged (evidence/equiv_planted_control.tsv); per header (evidence/equiv_control_per_header.tsv) every header with a landed includer
  is flagged; the 0-coverage headers have no includer anywhere in rebuild/src, tests or include. The 12 unflagged closure rows (work/ctl_unflagged.txt) reach
  the closure only through guarded includes: 10 via CTCOracleMinigame.h (its COracleMinigameDef.h include sits under #ifdef
  FABLE_RETAIL_CTCORACLEMINIGAME_CLASS), 2 (0x008FDC1D, 0x0090C670) via CAIStateGroup_GazeAtWork.h (under #ifdef FABLE_RETAIL_CAISTATEGROUP_GAZEATWORK_RUNNABLE;
  0x0090C670 also includes CAIStateGroup_Gaze.h without defining FABLE_RETAIL_CAISTATEGROUP_GAZE, and that header's whole body, the planted line
  included, sits under that macro). Not every intermediate include is
  guarded (CEngineParticleSimulationRenderData.h includes CParticleSimulationRenderData.h unguarded), but no unflagged row goes through it. equiv_check
  hashes the cpp output (errors included), so no row is skipped silently; the proof for these 12 is the exact-text equality (0 lines above).

## Rows (tools/flatten_pilot.py v5 + `--own-optins`; work dirs work/gen4 (b1-b5), work/gen5 (b6))
- Order: gen --tests-only (74 tests PASS clang, evidence/tests_prelock_PRECHECK.tsv) -> 74 locks -> generate the models / probes -> gen4 with the full
  precheck (79 x PASS PASS PASS, evidence/PRECHECK.tsv) -> 5 more locks (their tests passed in gen4's precheck first) -> grade per batch in later calls
  -> Strafe opt-ins, gen5 (2 x PASS PASS PASS), 2 more locks, grade b6. Every compile / grade / probe / link / equiv run was a background job with
  `timeout 900` on its launch line, checked with kill -0 in a later call.
- `--own-optins FILE` (new): an opt-in that exists only in this run's tree and that a derived model names in its `// Needs` line (the base _RTTI_DECLS
  blocks, and LightHomeCandle's model for LightWorkCandle) counts as this run's own (route header), like v5's own _VIRTUAL_MODEL macros. Without it
  gen3 dropped the 4 Gaze rows as "no landed switch" (evidence/own_optins.txt lists the four macros).
- Per row (evidence/grade_b*/GRADE.tsv, vland_{head,new}_c*.txt): HEAD graded symbol = the flattened self, new graded symbol = the PDB symbol; objcheck:
  every non-debug section equal under the one rename, DEF sets equal under it, UND sets equal, no new ??_7 / ??_R; `.drectve` equal (34 rows) or HEAD +
  exactly /DEFAULTLIB:"libcp" (47 rows; NF318 Q1 / NF322 Q3); 76 MATCH / 5 RELOCATION_MATCH, every unchanged landed test PASS (recheck_final.log).
  WARN lines in recheck_final.log (SHARED_HEADER_AVAILABLE on 0x004522C6 / 0x004DED4E / 0x00911F00 / 0x0091CCB0; RELOC_NAME on 0x004522C6 PersistB and
  0x00911F00 f1 / f4) are identical on the HEAD side (pre-existing; 0x00911F00's f1 / f4 stand-ins are the nf315 T3 unit's, named only in that line's
  reason column, NF316 Q3).
- Across rows: one /FORCE link of the 81 HEAD and the 81 new objects (evidence/linkcheck_all81.txt): 0 LNK2005 either side, unresolved sets equal
  (10 symbols, none an old or new self). New-name census (evidence/census_81.txt): 0 hits in rebuild/src; 3 hits in other rows' standalone tests
  (0x00B81450 / 0x00B81D60 tests model CParticleSimulationRenderData::IsBusy in-class, 0x00A87140's test models CPhysicsFacesSegmentLeaf::HasChildren):
  a test links alone or with its own row, so no hold (NF324 census rule). reloc_bindings.tsv: no line targets any of the 81 (evidence/table_targets.tsv,
  empty; they are reached through their vftables); TABLE_CHANGES.tsv empty; NAME_FIXES.tsv: none.
- Stay-off screens: COORDINATION_CLAIMS newest lines: 0 on the 95 addresses and 0 on the 55 header classes; UNMATCHED_ROWS 0; pending/2026-11-24_acctB_b24 0
  (addresses and header names); codex 26 / 27 / 28 RESERVED 0; NF326 rows.txt 0; NF328's 9 rows 0; no CScriptThing text in any generated source or header;
  T3_PORT_QUEUE: each entry has its nf324 column-2 line only (97 matching lines for 95 addresses: the nf315 line for 0x008FCF90 names 0x00911F00, and the nf324 line for 0x004530BB names 0x004DED4E, both in the
  reason column only). No lock of another owner.
- Final checks: treesync 0 stale; every orig/ copy (80 HEAD rows, 55 headers) equals the repo; rows/ == the graded gen source; byte scan 0 hits over 707
  files (evidence/bytescan.txt); the 81 locks are NF330's and kept for the landing. work/ keeps GEN / PRECHECK / gen / pre and the include tree; the vland
  wd/ object dirs, oracle.tsv files, link dirs, probe objects and control trees were removed with clean.py.

## Not done: 14 skipped-port (STATUS.tsv reasons)
- CSB family (0x00A19CF0 CSB::Reset, 0x00A20860 CSBGoal::GetType, 0x00A88910 CSBAgents::GetType, 0x00A21F90 CSBCCorridor::GetType): CSB's landed model
  declares only Process; retail 0x0129CC1C has the deleting destructor at slot 0 and GetType / CreateLineList / Reset after Process, and GetType returns
  CSB::ESB_TYPE, which no shared header declares (enum + model port).
- Fonts (0x00A63340 CFontDataBank, 0x00A5EE90 CStreamingFontDataBank), measured: on the CBankFile STATICBLOCKS model both probe 21 emitted slots v
  retail 20 (slot 6 0x009CE5E0 is not an AddEntryMultipleFiles overload; slot 16 binds the base's PrepareDataForInclusion). Finding: the CBankFile model's
  vftable is one slot long / mis-ordered; it needs fixing before any CBankFile-derived model.
- 0x00BCEA50 AnimatedMeshFromFile, measured twice: on the default CEngineInternalPrimitiveAnimatedMesh model the derived sizeof assert fails and the class
  stays abstract under the base's prerequisite set.
- Bases with no header / out of generator scope: 0x007F0630 (base ...KnockdownRecover absent from the layout TSV), 0x00B900A0 (CEngineSceneEnumerator
  absent), 0x008FDB67 (CAIStateGroup_Clean quarantined), 0x009000EF / 0x009425E0 (template base CAIStateGroup_WorkBase<CAIStateGroupBase>),
  0x009021DA (three flat bases MinionSitOnSeat -> MinionSitOnSeatBase -> UsesSeat), 0x00406800 (CUserProfileManager: two RTTI bases).

## Decisions for the coordinator
- D1 (b5, b6): an add-only `FABLE_RETAIL_<BASE>_RTTI_DECLS` block inside an existing LANDED base model (CAIStateGroup_Gaze, CCreatureAction_StrafeForward /
  Backward) declaring the retail overrides a derived model needs (NF319 RTTI_DECLS precedent, NF322 Q2 for the base). No landed row defines the new macros
  (NF318 Q2 tension), so the rows land with them; the Gaze edit sits inside the branch 4 landed rows compile (hdrcheck2 default=same, equiv 0 lines, the
  planted control flags those rows). b5 / b6 are their own batches so a "no" drops them cleanly. Accept?
- D2: `--own-optins` (flatten_pilot v5 change) to treat such tree-only base opt-ins and a same-batch chained model (LightHomeCandle) as this run's own. OK?
- D3: genmodel declares a non-public virtual destructor (`protected: virtual ~CPhysicsFacesSegmentLeaf();`, declaration only) where the retail RTTI slot
  is `??_G..@@MAE` and both PDB sets carry ??1..@@MAE@XZ. Accept?
- D4: CEngineInternalPrimitiveLight's VM block hand-merged around NF151's `_TYPED` member opt-in (see Headers). Accept?
- D-e (as NF329): two of these headers also sit in parked pending overlays: pending/2026-11-11_linux_handH40i/hdr/engine/CCreatureAction_ReleaseAndLoopDivineWrathSpell.h
  and pending/2026-11-13_acctB_parked/hdr/00795300/engine/CCreatureAction_ReleaseAndLoopUnholyPowerSpell.h; my edits are on the CURRENT repo headers.
  Screened and NOT overlays: the hand728 / hand730 / hand732 whole-include-tree mirrors and the nf322 unused-model copies (snapshots, not pending
  header edits). New-slot rows (rows whose PDB symbol is a vftable slot past the base's last slot): evidence/new_slot_rows.tsv; the 55 hand-back
  header copies v repo (0 diffs): evidence/hdr_copies_check.txt.
- Finding: the CBankFile model's vftable (21 v 20) blocks the two font rows; queue it.

## Process notes / slips
- Slip: four `rm -f $A/hdr/engine/<X>.h` (my own generated headers, before regenerating) used a variable path against the clean.py rule; everything
  after that went through clean.py.
- After the container restart the first Wine cl started the prefix services holding probe p1's capture pipe (cl.exe zombie); I killed those six service
  PIDs (spawned by my own cl, same start time) and warmed Wine with a detached run (LESSONS.md).
- Slip: one command contained `sleep 0` (no wait in effect, but against the no-sleep rule); no other sleep or foreground wait.
