# NF325 (2026-11-24): NF322's held / port rows under its rulings, two class-model defects, three NF321 requeues (89 entries)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Rulings applied: NF322 Q1 (widen the
CCombatSequenceBase `_PASS27` guard), Q2 (a new base header is part of the port), Q3 (libcp on header-route rows).
Work order (NF285 E2 / NF319 Q1, recorded per STATUS line as "order N"): 1 CCombatSequence_*, 2 (none) , 3 `_PASS24` hold, 4 the
CEngineInternalPrimitive defect + 0x00BBDDB0, 5 the EnemyCreatureBase defect, 6 MazeBase / ServeFood, 7-8 the NF321 requeues,
9 the carried-over port rows (re-read, STATUS re-stated, not compiled).

## Result (STATUS.tsv: 88 addresses; 0x00BBDDB0 is listed twice in ENTRIES and has one line)
- **32 done**: 31 changed rows (rows.txt) + 0x00900D3D (header-only defect fix, its landed row unchanged).
- **1 skipped-held**: 0x00A5F9C0 (class shared with NF323).
- **55 skipped-port**: the CCreatureAction family (12, left to NF323), body / callee / other-class ports NF322's rulings do not cover,
  0x00A86030 (Q3 below), the two ServeFood rows (flat-member bodies), 0x00896E60 / 0x00692480 (requeues: body port / NF323's base).
- REJECTED.tsv: none.

## Done rows (31) and their proof
| batch | rows | route |
|---|---|---|
| c1 | 23 CCombatSequence_* GetName + 0x00908DE0 OnStart | `#define FABLE_RETAIL_<X>_VIRTUAL_MODEL` + `FABLE_RETAIL_CCOMBATSEQUENCEBASE` |
| c2 | 0x008B3810, 0x008B42E0 | as c1, plus the stand-in callee `?Init@CCharString@@QAEXPBDH@Z` -> PDB ctor `??0CCharString@@QAE@PBDJ@Z` |
| c3 | 0x00A85FB0 | new CPhysicsFacesSegmentBranch opt-in |
| c4 | 0x00BBDDB0 | CEngineInternalPrimitiveParticleSpriteTrailGroup opt-in over the two fixed bases |
| c5 | 0x00A0D2B0 (requeue) | header route on engine/CAInputProcess.h + two add-only opt-ins; `Register` -> `AddInputProcess` |
| c6 | 0x0090080B, 0x0090086F, 0x009008A1 | MazeTutorialBattle* opt-ins over the NEW engine/CAIStateGroup_MazeBase.h |

Per row (evidence/):
1. precheck: landed test clang (T_) before the lock; after the lock new (N_, NF_TREE=work/tree) and HEAD (H_) source, plus the
   rewritten test (NT_008b3810): all PASS (evidence/pre.log); gcc cpp #error pass CPP_OK.
2. vland `--recheck-landed` HEAD then new under ONE LANDVERIFY_WORK path (tools/runpair.sh): every row MATCH or RELOCATION_MATCH PASS
   on both sides, the landed test unchanged except 0x008B3810 (evidence/recheck_c{1..6}{h,n}.log).
3. objsec (tools/libcp.py, tools/libcpN.py for the two-name maps): every non-debug section equal in bytes and relocations under the
   rename(s), UND / DEF sets equal under the map, no COMDAT added or dropped, no `??_7` / `??_R` (evidence/libcp_*.txt).
   `.drectve`: EQUAL on 0x00A0D2B0 and the three Maze rows; HEAD + exactly `/DEFAULTLIB:"libcp"` on the 27 others (their opt-in base
   headers pull stlport/fable_containers.h; NF318 Q1 / NF322 Q3).
4. graded symbol = PDB symbol (vland result lines; RENAMES section 1 new leaf = the ENTRIES pdb_spelling / the requeue's PDB name).
5. link: /FORCE link of the 31 HEAD objects v the 31 new objects (evidence/link_all.txt): 0 duplicates both; HEAD unresolved
   `?Init@CCharString@@QAEXPBDH@Z` (2) and `?Register@CInputProcessManager@@...` (1), new unresolved only the PDB names
   `??0CCharString@@QAE@PBDJ@Z` (defined by the landed self row 0x0099EBF0) and `?AddInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z`
   (its self row 0x00A0D4F0 still defines a stand-in: unresolved before and after, NF302 Q2; queued in NAME_FIXES.tsv).
6. name census (tools/namecensus.py, evidence/namecensus.tsv): no other landed source defines `X::M(` or Leafs any of the 31 new
   symbols (positive control: the regex finds 0x00BBBE60's own definition). In-class census (evidence/inclass.txt): the 20 local models
   with an in-class body of these members are all in standalone tests, none in rebuild/src.
- Pre-existing vland WARNs, unchanged by this batch: RELOC_CONFLICT `_kName` (file-static) on 0x008B3D80 / 0x008B4200 (HEAD and new);
  SHARED_HEADER_AVAILABLE CMainGameComponent on 0x00908DE0. Gone on the new side: SHARED_HEADER_AVAILABLE on 0x00A85FB0 / 0x00A0D2B0
  and the RELOC_NAME stand-in warnings on 0x008B3810 / 0x008B42E0 / 0x00A0D2B0.
- Script use: tools/genmodel.py generated all 31 class models (25 CCombatSequence_*, CPhysicsFacesSegmentBranch,
  CEngineInternalPrimitiveParticleSpriteTrailGroup, CAIStateGroup_MazeBase + 3 MazeTutorialBattle*); tools/mvrow.py drafted 28 rows
  (fixed here: it matched a definition head inside a comment on 0x008B49A0 / 0x008B4D90); 0x008B3810, 0x008B42E0, 0x00A86030 and
  0x00A0D2B0 were written by hand. Generated output was checked by tools/hdrcheck.py (removing each opt-in gives the repo header back
  byte for byte, 31 SAME) and by the emitted-vftable probes below.

## Headers (hdr/engine/, diffs hdr/diff/<X>.diff against the current repo header; orig/hdr_<X>.h = the repo copy used)
- **CCombatSequenceBase.h (NF322 Q1).** The only existing-line edit: `#ifdef FABLE_RETAIL_COMBAT_SEQUENCE_SELECTION_PASS27` becomes
  `#if defined(..._PASS27) || defined(FABLE_RETAIL_CCOMBATSEQUENCEBASE)` on the same line. The widened region is the class model only
  (19 virtual declarations, IsZoneValid, 7 members, the sizeof assert) plus the stlport include the vector member needs; no inline body,
  no pass-specific member. Add-only under the new macro: `protected:` before GetFramesToPredictTargetPosition / OnStart / OnStop and
  `public:` after (retail MBE / MAE: RTTI slots 16-18 + FableWin.pdb); the moved assert is pinned `FABLE_STATIC_ASSERT_AT(37, ...)`.
  Other `_PASS27` uses under rebuild/include: all separate guards on their own macros (stlport/fable_containers.h, fable_map.h,
  fable_algorithm.h, CTCBoss, CBankFile, CTCBed, CTCGuard, CArray, ...); CCombatManager.h's `_PASS27` block (the PASS27 definer's
  private members) is not widened and no row of mine defines `_PASS27`. Landed definer of `_PASS27`: 0x006EAE20 only.
- **25 CCombatSequence_* headers**: genmodel base mode (base CCombatSequenceBase, sizeof 0x28); each declares the slots retail labels
  with its own name (OnCreate / GetName / GetZone in every one, plus each class's own overrides); 3 keep a +0x28 member.
- **CPhysicsFacesSegmentBranch.h**: base CIPhysicsFacesSegmentBase (its model is NOT under `_PASS24`: only CalculateDividingPlane and
  three includes are, so NF322's `_PASS24` reason was wrong and nothing needed widening); plus `virtual ~CPhysicsFacesSegmentBranch();`
  under `protected:` by hand (both PDBs `??1...@@MAE@XZ`, retail slot 3 `??_E...@@MAEPAXI@Z`; genmodel leaves the dtor implicit).
- **CEngineInternalPrimitiveBase.h (defect 1a)**: `#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETCHILDPRIMITIVE_ORDER` the two
  SetChildPrimitive declarations swapped / `#else` the old two lines / `#endif` (VC7.1 lays an overloaded pair out in reverse
  declaration order; GOTCHAS 48 / NF29).
- **CEngineInternalPrimitiveParticleGroup.h (defect 1b)**: add-only `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_SLOTS`:
  the 9 overrides retail places under the class (slots 6-12, 15, 18) and its two new virtuals GetRenderLayerMask (28) and
  PrimTypeMatchesGroup (29); every spelling = RTTI-port label and in both PDB name sets. (The landed model declared only Update, so a
  derived table was wrong at 11 slots, not only slot 28.)
- **CEngineInternalPrimitiveParticleSpriteTrailGroup.h**: genmodel base mode over ParticleGroup (Update slot 5, PrimTypeMatchesGroup 29).
- **CAIStateGroup_EnemyCreatureBase.h (defect 2)**: add-only `FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_OVERRIDES`: Initialise /
  OnActivate / Cleanup / IsRunnable, public (RTTI labels + both PDB name sets).
- **CAInputProcess.h**: add-only `FABLE_RETAIL_CAINPUTPROCESS_INIT`: `virtual void Init();` slot 2 (RTTI + FableWin.pdb).
- **CInputProcessManager.h**: add-only `FABLE_RETAIL_CINPUTPROCESSMANAGER_FRIEND_CAINPUTPROCESS`: `friend class CAInputProcess;`
  (AddInputProcess is protected in both PDBs and in the header already; NF321's "declared public" was wrong; NF305 Q5 / NF309 Q2).
  The header names CEnginePrimitivePrepareInfo undeclared, so the row forward-declares it before the include (15 landed rows reach the
  header transitively; the planted control flags them).
- **CAIStateGroup_MazeBase.h (NEW, NF322 Q2)**: in rtti_bases.tsv (`: CAIStateGroupBase`) and the layout TSV; the flat part is
  gen_class_headers.py's own output (a copy of the tool, tools/gen_class_headers_nf325.py, redirected to write into work/gen/ only;
  orig/hdr_CAIStateGroup_MazeBase.h is that output), then the genmodel opt-in (OnActivate slot 2, OnReplace slot 10), asserts inside.
  Must land with or before 0x0090080B / 0x0090086F / 0x009008A1. 20 landed files (13 sources, 7 tests;
  evidence/local_models_mazebase.txt) model CAIStateGroup_MazeBase locally; with a shared header they get a SHARED_HEADER_AVAILABLE
  warning (strict mode would reject them); none is changed here. Generator lines: evidence/gen/ (INDEX: OK, 0 retail files; no
  RECONCILE verdicts; the generated flat header as written).
- **CAIStateGroup_MazeTutorialBattle / _RespondToLightning / _UseShield.h**: genmodel base mode over MazeBase (GETNAME_ALLOC lines carried).
- Assert pins: tools/pin_asserts (repo lane_merge/pin_asserts.py) on every header; hdrcheck (evidence/hdrcheck.txt).
- **Slot probes** (tools/vprobe2.py = vprobe.py + a scratch-only PROBE_PRE preamble; tools/vtcmp.py), evidence/probe_*.txt:
  - CCombatSequence_*: all 25 classes (probe_cs25.txt; scratch TU `class CCombatAction {};` so the implicit copy ctor compiles; the
    model has reference members and no ctor, so the probe uses `new X(x)`): 16 classes 19/19 SAME, 9 classes 18/19 (slot 0 `??_E` v
    retail `??_G`, the VC7.1 weak alias; UnblockableFacing, WhisperAdvance, WhisperAttackBigCombo, WhisperIdles, WhisperLunge,
    WhisperStrafeAround, GenericLeader, ScorpionLungeAttack, TrollBurrowPursue). STATUS gives each row its class's count.
    Not probed: CCombatSequenceBase's own table. Its slots 1 / 2 / 15 (OnCreate / GetName / GetZone) are `_purecall` in retail but
    declared non-pure in the `_PASS27` model (left as is, Q4); every derived class overrides all three, so no derived table shows it.
  - CPhysicsFacesSegmentBranch 5/5 SAME. ParticleGroup and SpriteTrailGroup 29/30 SAME (slot 0 `??_E` v retail `??_G`, the VC7.1
    weak alias). CEngineInternalPrimitiveBase with the fix 27/28 SAME (slot 0 alias), without it 25/28 (slots 7 / 8 swapped: the control).
  - EnemyCreatureBase-derived: HoverAttack / HoverHornetQueenTakeDamage / HoverAttackArea / HoverAttackUnlessTargetIsFleeing 23/23,
    WhisperPotionCombat 21/22 (slot 0 alias); NF322 had 19-21 there.
  - MazeTutorialBattle 13/14 (slot 0 alias), RespondToLightning 14/14, UseShield 14/14; MazeBase itself is abstract in retail (slot 12
    `_purecall`), so its own table is checked through the three derived tables (inherited slots 2 / 10 SAME).
- **equiv_check** (stock lane_merge/equiv_check.py, repo rebuild/include v work/tree = repo + hdr/, S in my folder, background):
  see "Final checks"; the planted control is tools/plant.py (work/tree_ctl).

## Defect census (brief item 2)
- **Virtual calls through slots 7 / 8 of the CEngineInternalPrimitiveBase model**: none. 76 landed files define
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED (evidence/attached_users.txt; no header defines it); every line of theirs that
  names SetChildPrimitive is listed in evidence/slot78_census.txt (16 lines, any call form): the three sources 0x00B1F200, 0x00AFCC30,
  0x00B03CA0 call it on CIEngine (`info.Engine->SetChildPrimitive`, engine/CIEngine.h, already in retail order), and two tests
  (0x00B92950, 0x00B4A700) only give the model's two overloads `abort()` stub bodies so their emitted tables link. The 2D viewport / group / changer SetChildPrimitive rows do not use the model. One row emits a vftable
  through the model: 0x00BA0FC0 AddStaticMesh does `new CEngineInternalPrimitiveStaticMesh` (inline default ctor), so its object holds a
  pick-any `??_7CEngineInternalPrimitiveStaticMesh@@6B@` whose inherited slots 7 / 8 follow the old (swapped) order; function bytes are not
  affected (one relocation to the table symbol) and the fix is opt-in, so nothing changes until that row opts in. Not compiled here.
- The base's own SetChildPrimitive self rows 0x00B8FDA0 (3-arg, slot 8) / 0x00B8FDB0 (4-arg, slot 7) are free `__stdcall` stand-ins
  (NAME_FIXES.tsv); their addresses agree with retail order.
- EnemyCreatureBase: landed binders of wrong spellings of the four targets: 0x0096A250 (`?base_init@@YIXPAUC@@@Z`), 0x0095A890 (QAE,
  `JJ` parameters) for 0x0091D820; 0x00959F30 (QAE) for 0x0091DD30 (NAME_FIXES.tsv). No landed row changes with the opt-in.

## Questions
- **Q1 (0x00A86030)**: the CPhysicsFacesSegmentBranch model is done (probe 5/5), but the body recurses through
  `CIPhysicsFacesSegmentBase*` into GetPossibleCollisionSet, which engine/CIPhysicsFacesSegmentBase.h declares `protected:` (clang:
  "is a protected member"). Retail compiled it, so the original had either a public base declaration or a friend. No PDB record I can
  read decides (the pure base slot has no symbol; llvm-pdbutil cannot open these PDBs: "Too many directory blocks"). May I add
  `friend class CPhysicsFacesSegmentBranch;` to the base under an opt-in (NF305 Q5 / NF309 Q2 shape), or must the access be settled
  from the PDB type records first? Draft: evidence/held_drafts/00a86030.cpp.
- **Q2 (ServeFood)**: CAIStateGroup_ServeFood has three pure new slots 14-16 with no PDB symbol on ServeFood (names only on the
  derived overrides). For its future base model: are the derived overrides' spellings (both PDBs, MAE) plus the retail `_purecall`
  slots enough to declare `virtual bool IsTime() = 0;` etc. on the base?
- **Q3**: CAIStateGroup_MazeBase.h makes 20 landed files' local MazeBase models SHARED_HEADER_AVAILABLE (warning). Land it with the
  three rows (as NF322 Q2 says), or hold until those local models are ported?
- **Q4 (CCombatSequenceBase pure slots)**: retail slots 1 / 2 / 15 of CCombatSequenceBase are `_purecall`; the widened `_PASS27`
  model declares OnCreate / GetName / GetZone non-pure. Rows and derived probes are unaffected (every derived class overrides all
  three). Should they become `= 0` under the new macro (add-only `#ifdef` new / `#else` old on those three lines), in a later unit?

## Final checks (2026-11-24, before hand-back)
- **Binder census** (tools/census_addr.py over the 31 addresses + 0x00A0D4F0, evidence/census_addr.tsv): 0x00A0D2B0 is called by
  0x00687A70, which already binds the PDB name (table line); 0x00A0D4F0 only by 0x00A0D2B0 (so TABLE_CHANGES is `replace`);
  0x008B54C0 shows two absolute-dword hits in script rows 0x00D89160 / 0x00D89380, but no instruction there has it as an operand
  (capstone, screen only): misaligned bytes, not references. No other landed row reaches any of the 31.
- **equiv_check (deviation, read this)**: the full 57,146-row run (eq3) could not finish (load average 36 on 4 cores: ~9% after 25
  minutes; killed by its PID). I ran the stock tool with its address-file argument over the include closure instead:
  tools/includers.py computes every landed row whose source or test (following folded `#include "../src/compiled/..."` TUs) reaches
  a changed header through quoted includes (160 headers in the reverse closure, 365 rows, hdr/equiv_rows_includer_closure.txt); a row
  outside it never reads a changed file. Cross-checks: every row the first (unpinned) run flagged and every row the control flags is
  inside the set; no landed row uses `<engine/...>` / `<stlport/...>` includes. Result: **365 rows, 0 differing**
  (hdr/equiv_check.tsv / .out). Planted control (tools/plant.py, one typedef per header on a path every existing includer reads):
  **213 lines flagged** (hdr/equiv_planted_control.tsv), incl. the `_PASS27` definer 0x006EAE20 and the HEAD sources of 30 of the 31 rows (0x00A0D2B0's HEAD does not define
  FABLE_RETAIL_INPUTPROCESS, so it reads CAInputProcess.h's generated branch, which no edit and no plant touches);
  per header (evidence/equiv_control_per_header.tsv) every header has >= 1 flagged includer except the new CAIStateGroup_MazeBase.h
  (no existing includer). Full-tree rerun command for the landing: `cd /home/user/FableDecomp && S=<dir> python3
  tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py rebuild/include <tree> <out.tsv>`.
- **Final recheck** (vland `--recheck-landed`, FABLE_HDR_OVERLAY = work/tree = the final hdr/ set): 31/31 PASS (5 MATCH,
  26 RELOCATION_MATCH), recheck_final.log. 0x00A0D2B0's source then got a comment-only fix; its HEAD / new pair was regraded on the
  same tree (evidence/recheck_c5{h,n}.log, objsec again LIBCP_OK); every other rows/<a>.cpp equals the graded payload text.
- Row switches (evidence/row_defines_landed.tsv): every `#define FABLE_RETAIL_*` in rows/ besides the new ones has landed non-`_PASS`
  definers (minimum 1: ..._PARTICLEGROUP_RTTI_DECLS, NF319's 0x00BBBE60).
- hdr/engine equals work/tree/engine; every orig/hdr_*.h and orig/<a>.cpp still equals the repo; no header shared with NF323 / NF324 hdr/.

## Locks
Kept on the 31 changed rows (rows.txt). 0x00A86030 was locked, prechecked (clang FAIL, protected access) and released. No other lock.

## Rule slips (none reached the repo)
- mvrow.py was run on two wrong ENTRIES indices (drafts of 0x00B2F7C0 / 0x00B9D0F0, and later 0x00902CA7); the drafts and their orig/
  copies were removed with clean.py at once; nothing was compiled from them.
- The first equiv_check ran before the generated headers' default-branch asserts were pinned (it flagged their includers); killed, the
  asserts pinned, rerun. The c1 grades ran with those unpinned headers: the new rows define the opt-ins, so the pinned (#else) text
  is not part of what they compile, and the final recheck uses the final tree.
- One command left a background busy-wait loop (`while kill -0 ...; do :; done &`) for about a minute; killed by its own PID.
- Three commands contained a `sleep 0` (a no-op, but the rule is no sleep at all).
- The full-tree equiv_check was replaced by the include-closure run (Final checks); a deviation from "ALL landed rows" in form only.

## Tools added or changed (tools/, paths set to w22/agentNF325)
- vprobe2.py: vprobe.py + PROBE_PRE (scratch-TU-only preamble). plant.py: the planted-control tree. includers.py: the include-closure
  row set for equiv_check's address-file argument. ctlcover.py: per-header control coverage. namecensus.py: other definers / Leaf claims
  of each new self symbol. gen_class_headers_nf325.py: the repo generator with its output redirected into work/gen/.
  assemble_nf325.py: RENAMES / APPLIED / TABLE_CHANGES / NAME_FIXES / REJECTED.
- mvrow.py: the definition-head search masks comments first. st.py: one line per address when ENTRIES lists it twice; keeps the
  PDB spelling from the non-empty line. hdrcheck.py: non-generated headers list their diff blocks (OTHER). genmodel.py: NF325 header text.
- NF322's shell helpers copied in (runpair.sh, pre_list.sh, pre_tests.sh, cpp_err.sh, zsq.sh, recheck.sh, link_g.sh, grade_lane.sh)
  and retargeted to this folder; mkoff.py / whose.py retargeted to NF323 / NF324.
