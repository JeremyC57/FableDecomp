# NF329 (2026-11-24): NF324 Q1 -- the flat-header combat families on generated opt-in derived models (301 rows, 118 headers)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Rulings applied: NF322 Q1 / Q2 (generated opt-in
models, probe, asserts, equiv_check), NF318 Q1 / NF322 Q3 (libcp on header / switch routes), NF324's per-row and cross-row proof.
Nothing was written under the repo and no git was run.

## Result (STATUS.tsv: one line per NF324 Q1 entry, 396)
| status | rows |
|---|---:|
| done | 301 |
| skipped-port | 95 (brief item 4, the thin remainder: not reached; triage reason per line) |
| held / rejected | 0 |

| batch | rows | route | headers (land first) | proof |
|---|---:|---|---|---|
| b1 | 80 | header: CCombatActionBase family, 47 classes CCombatAction_ControlledStrafeJump .. CCreatureAction_FinishHealLifeSpell (alphabetical) | HEADERS_b1.tsv, 47 headers (batches/b1/hdr/) | 80 ACCEPT, MATCH PASS, objcheck equal + libcp |
| b2 | 75 | header: CCombatActionBase family, 34 classes CCreatureAction_FinishWillForceSpell .. CCreatureAction_WhisperJumpIntoSky | HEADERS_b2.tsv, 34 headers (batches/b2/hdr/) | 75 ACCEPT, objcheck equal + libcp |
| b3 | 60 | switch: 57 CCombatSequence rows on NF325's LANDED models, 2 CAIStateGroup_ServeFoodAt* (NF327's landed model), 1 CPhysicsFacesSegmentBranch (NF325's) | none | 60 ACCEPT (58 + libcp, 2 equal) |
| b4 | 86 | header: CCombatSequenceBase family, 37 new models | HEADERS_b4.tsv, 37 headers (batches/b4/hdr/) | 86 ACCEPT, objcheck equal + libcp |
Each batch dir is a land_scripted.py run dir (RESULTS.tsv, rows/, head/payload.json + HEAD_FROM_REPO.txt, TABLE_CHANGES.tsv (empty)), plus
HEADERS.tsv / hdr/engine / hdr/diff / orig/ / PROBES.tsv / EQUIV_CHECK.tsv / EQUIV_CONTROL.tsv for the header batches. A class never straddles two batches.
Landing order: b3 needs nothing; hdr of b1 / b2 / b4 before their rows. 0x008CA950 (b2): its landed HEAD source carries a retail
byte-dump comment, so its HEAD entry is left out (head/HEAD_FROM_REPO.txt; NF324 Q4 form); the new source drops that line.

## Re-triage (brief item 1)
- tools/retriage.py (work/RETRIAGE.tsv) + a dry v5 `gen --no-pre` over all 396 (evidence/GEN.tsv is the graded gen; work/dry_all the dry
  run): none of the 396 was done since NF324 (no landed self defines the PDB symbol; the generator found the flattened definition in each);
  no other owner's lock, no codex 26/27/28, acctB or nf326 reservation, no CScriptThing text in any of the 301 sources; each entry has
  exactly one T3_PORT_QUEUE line (source nf324, column 2, no unit column), released to this pass by the brief.
- NF325's 25 landed CCombatSequence models already declare every RTTI-labelled slot of their classes, so 57 Q1 rows there are now switch-route
  rows (b3); likewise 2 ServeFood rows (NF327) and 1 CPhysicsFacesSegmentBranch row (NF325).

## Headers (brief items 2-3; hdr/engine = full copies, orig/hdr_<X>.h = the repo copy used, batches/<b>/hdr/diff/<X>.diff)
- tools/genhdr.sh -> tools/genmodel.py (NF325's, base mode, unchanged logic) per class: `#ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` new model /
  `#else` the flat model text unchanged / `#endif`; members >= sizeof(base) kept from the flat model with FABLE_STATIC_ASSERT on sizeof and
  every offset; every slot whose RTTI-port label names the class declared (overrides at the base's slots, new virtuals past the base count
  in slot order); moved flat asserts pinned (lane_merge/pin_asserts.py). tools/hdrcheck.py: dropping each opt-in gives the repo header back
  byte for byte (evidence/hdrcheck.txt, 118 SAME). Declarations only, no bodies.
  - CCombatActionBase family (81): base = engine/CCombatActionBase.h branch 1 (FABLE_RETAIL_KRAKEN_ROAR; only that header reads it; NF323
    proved its 81 slots), sizeof 0x114. The opt-in forward-declares CSoundPair / CAnimationEntry (names branch 1 uses undeclared) and
    carries a `// Needs: FABLE_RETAIL_BASE_OBJECT, _DEF_CLASS_BASE, _CCREATUREACTIONBASE, _KRAKEN_ROAR.` line; every one has landed definers.
  - CCombatSequenceBase family (37): base = NF325's widened model (FABLE_RETAIL_CCOMBATSEQUENCEBASE, landed), sizeof 0x28. No widening
    needed: NF325's `|| defined(FABLE_RETAIL_CCOMBATSEQUENCEBASE)` is in the repo.
- Model proof: emitted-vftable probe (tools/vprobe2.py `new X(x)` + tools/vtcmp.py; CS with the scratch-only `class CCombatAction {};`),
  summarised strictly by tools/probesum.py (evidence/probe_summary.tsv, per batch batches/<b>/PROBES.tsv): 118 OK / 0 FAIL. Every class:
  retail slot count == emitted count (81 / 82 / 83 for CA, 19 / 20 for CS) and every slot SAME, except slot 0 on 55 classes where VC7.1
  emits `??_E<X>@@UAEPAXI@Z` and retail has `??_G<X>@@UAEPAXI@Z` (same class and public-virtual tail: the VC7.1 weak alias, as NF325).
  Negative controls: BlockBase without IsToFaceTarget -> slot 73 DIFF; 2StrikeCombo without GetZone -> slot 15 DIFF (evidence/probe_neg*.log).
- Two sources per declaration (tools/declsrc.py, evidence/declsrc.tsv): 510 declarations, each RTTI-placed (labels_rtti_port.tsv) and in a
  PDB name set (271 both, 239 FableWin only); 0 in neither (NF320 Q3).
- equiv_check (stock lane_merge/equiv_check.py via tools/eqrun.sh, S in my folder; coordinator's include-closure scope): tools/includers.py
  closure = the 118 headers themselves (no header includes them), 18 landed rows / tests include them (evidence/equiv_rows_includer_closure_v1.txt).
  repo rebuild/include v work/tree (repo + hdr/): 0 lines (evidence/equiv_check_v1.tsv). Planted control (tools/plant.py: a typedef in each
  header's default `#else` branch): 19 lines, all 18 rows flagged (evidence/equiv_planted_control_v1.tsv); per header
  (evidence/equiv_control_per_header.tsv) the 14 headers with landed includers are all flagged; the other 104 have no landed includer
  (grep of rebuild/src, rebuild/tests, rebuild/include for `engine/<X>.h"`: 0 files).

## Rows (tools/flatten_pilot.py v5 = NF324's v4 + the changes below; work dir work/gen1)
- v5 changes: (1) with --hdr-tree a `#ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` block counts as a switch (this run's own when no landed row
  defines it: route header; else landed: route switch); (2) the block's `// Needs` line adds those switches; (3) no precedent preamble for
  this run's own models (v0 + error-driven prerequisites); (4) the header-route override gate accepts a declaration the model tsv places on a
  NEW slot (15 rows; the probe is the proof: D-a); (5) the nf324 Q1 T3 lines are released (D-b); (6) `export --only` (one batch per export).
- Order: `gen --tests-only` (301 tests PASS clang, evidence/tests_prelock_PRECHECK.tsv) -> lockall claim (301 OK) -> `gen` with the full
  precheck into work/gen1 (new / HEAD / test: 301 x PASS PASS PASS; evidence/PRECHECK.tsv) -> `grade` per batch in later calls (vland
  --recheck-landed HEAD then new under one work path, FABLE_HDR_OVERLAY = work/tree) -> export.
- Per row (evidence/grade_b*/GRADE.tsv, vland_{head,new}.txt): HEAD graded symbol = the flattened self, new graded symbol = the PDB symbol;
  objcheck: every non-debug section equal under the one rename, DEF sets equal under it, UND sets equal, no new ??_7 / ??_R; `.drectve`
  equal (2 rows) or HEAD + exactly /DEFAULTLIB:"libcp" (299 rows; NF318 Q1 / NF322 Q3); MATCH 297 / RELOCATION_MATCH 4, every unchanged
  landed test PASS. recheck_final.log = the new-side vland status lines (301 PASS; 4 pre-existing SHARED_HEADER_AVAILABLE warnings on
  0x00906FA0 / 0x00906FC0 / 0x009076A0, the same on the HEAD side).
- Across rows: one /FORCE link of the 301 HEAD and the 301 new objects (tools/linkcheck.sh, evidence/linkcheck_all.txt): 0 LNK2005 either
  side, unresolved sets equal (4 symbols: ?ConstantFPS@CMainGameComponent@@1JA, ?GetFrame@CWorld@@SIJXZ, ?g_FloatOne@@3MA, __fltused;
  none is an old or new self). New-name census (tools/inclass_census.py, evidence/census_301.txt): 0 hits in rebuild/src; 8 hits
  in other rows' standalone tests (0x008C4680 / 0x008C5490 tests model ScorpionCharge members; 0x00A85FF0 / 0x00A86F60 tests model
  CPhysicsFacesSegmentBranch::HasChildren in-class): a test links alone or with its own row only, so no hold (NF324 census rule).
- Tables: TABLE_CHANGES.tsv empty. reloc_bindings.tsv has NO line whose target is any of the 301 addresses (evidence/table_targets.tsv,
  empty: they are reached only through their vftables), and the generator's unit check found no other landed file naming any flattened
  self. NAME_FIXES.tsv: none.
- Stay-off screens (final): COORDINATION_CLAIMS.tsv newest lines: 0 open claims on the 301 addresses or the 118 classes (two CCombatActionBase
  items, both A and done: NF259 / NF261); UNMATCHED_ROWS.tsv: 0 of the 301; pending/2026-11-24_acctB_b24 (637 addresses in its files): 0;
  codex pass 26 / 27 / 28 RESERVED.tsv (100 / 100 / 25 addresses parsed): 0; NF326 rows.txt: 0; no `<engine/` / `<stlport/` angle includes
  anywhere (closure cross-check). hdr/engine == work/tree/engine == batches/<b>/hdr/engine for all 118.
- Final checks: treesync 0 stale; every orig/hdr_<X>.h and orig/rows/<addr>.cpp equals the repo; rows/ == the graded gen source; the 301
  locks are NF329's (kept for the landing). Byte scan (tools/bytescan.py): 0 byte dumps in rows/, payloads, hdr/, evidence/ (the one HEAD
  source with a dump, 0x008CA950, is not copied: orig/rows/LEFT_OUT.txt).

## Not done (brief item 4)
The 95 thin-remainder rows: CCreatureActionBase 22, CSubDefClassBase 20, CA-subfamily bases (TrollWhackGroundBase 7, BuildUpDivineWrath 5,
BuildUpUnholyPower 5, SheatheItemToInventory 4, BlockBase 1, Strafe* 2, GenericStrikeResponseKnockdownRecover 1), CAIStateGroupBase 6,
Gaze 4, and singles. Several chain onto b1 / b2 models (BuildUp*, BlockBase), so they are a next pass after these land.

## Decisions for the coordinator
- D-a: v5 relaxes NF324's header-route override gate for declarations on a NEW slot (15 rows), with the vftable probe as the proof.
- D-b: v5 releases the 301 nf324 Q1 lines of T3_PORT_QUEUE (they should be closed at landing).
- D-c: the macro is FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL (NF323 Q1 / NF325 style), not the brief's bare FABLE_RETAIL_<CLASS>.
- D-d: the CA opt-ins hard-wire branch 1 of CCombatActionBase.h (FABLE_RETAIL_KRAKEN_ROAR, via the Needs line) and forward-declare
  CSoundPair / CAnimationEntry inside the opt-in; OK as the family's base selection?
- D-e: six CA headers also sit in old parked pending overlays (Nov 3-13: hand732 StabThing / InterruptableMidAttackAutoTurn, handH40i
  BuildUp/ReleaseDivineWrathSpell, 2026-11-13_acctB_parked BuildUp/ReleaseUnholyPowerSpell); my edits are on the CURRENT repo headers.
- D-g: the derived models keep the generated header's `#pragma pack(push, 1)` (NF325's landed form; the flat members were laid out under
  it; every kept offset and sizeof is asserted, so no layout changes). EXTRA_RULES says no authored packing without byte evidence; the
  pack here is the generator's, not authored.
- D-f: 0x008CA950's HEAD entry needs rebuilding from the repo at landing (byte-dump comment; NF324 Q4 form).

## Process notes / slips
- Reading-list slip: docs/COORDINATION.md, ASM_RULES.md, EXTRA_RULES.md, GOTCHAS.md and the nf322 landing README were not read before the
  first compile (only the standing brief, the prefix, the NF324 / NF325 landing READMEs and hand-backs, and CLAUDE.md). Read / grepped
  afterwards (COORDINATION.md in full; GOTCHAS / EXTRA_RULES for opt-in, pack and header rules): the stay-off screens above and D-g
  come from that; nothing found changes a row.
- work/ is scratch, not hand-back: the vland wd/ object dirs, oracle.tsv files, link outputs and probe objects were cleaned with clean.py;
  GEN / PRECHECK / GRADE, gen/ and pre/ stay.
- flatten_pilot.py's export got `--only` while gen1 was running (export-only code; the generated text is unchanged; gen1 ran the code
  before that edit).
- gen / probe / grade / link / census / equiv runs were background jobs under `timeout 900`, checked with `kill -0` in later calls;
  genhdr.sh, hdrcheck, declsrc, probesum, export, assemble and bytescan ran in the foreground (seconds of Python, no compile).
