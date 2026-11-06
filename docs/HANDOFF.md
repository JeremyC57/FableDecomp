# Resume here (one page) — updated 2026-11-05 late (plan phase 7 and chunks 782-793 recorded; chunks 794 and 795 running)

**Overarching plan:** `docs/PLAN.md` (every known function by pool, the parked residue classes with their cheapest fixes, the
naming debt, the order of work). This session's details: `docs/journal/2026-11/2026-11-03_SESSION.md` and the 2026-11-03 lines
of `docs/journal/FINDINGS_LOG.md`. Solved problems: the 2026-11-0x lines at the end of `docs/pipeline/GOTCHAS.md`. Agent rules:
`tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`. The 2026-11-01 page is at the end of
`docs/journal/HANDOFF_ARCHIVE.md`.

## State
- **Resumed 2026-11-05 late** (the user: "continue where the last claude chat left off"). Done since the pause:
  - lane: the older a013f3ec-... scratch survived with the whole lane (game SHA-1 checked, prime recheck 5/5); the 40effb6c-...
    scratch is gone, and with it the phase 1-5 plan tools (hdrmig5 ...). Only the copies in `naming/2026-11-04_vendor_v2/scripts/`
    and `2026-11-04_wave35c/scripts/apply_w35c.py` survive; the phase 6 agent rebuilds the rest and hands them back for committing.
  - symbol index rebuilt at b7d036105 (`$S/plan/idx`: OK 53,902 / NO_FUNC 441 / LEAFGUESS 186 / COMPILE_FAIL 4); reloc_mask_audit
    found 0x00A8E160 (no Leaf line, graded the base ctor COMDAT), fixed: 0 suspects.
  - chunk 779 landed 2 of 5 (`batches/2026-11-05_landing_f/`), chunk 780 landed 1 of 5 (`_landing_g/`); parked rows in
    `pending/2026-11-05_linux_hand779/` and `_hand780/`.
  - chunk 781 landed 5 of 6 (`_landing_h/`: the CV_DemonDoorsScript TU incl. CDF_Dragon::Main with its jump table after the row
    split, and CTrophiesIntroWatcher::Main); CHTE_FreedomFighter::Main parked one `lea` off (`pending/2026-11-05_linux_hand781/`).
  - **Plan phase 6 applied** (`naming/2026-11-05_plan_p6_w1/README.md`): 27 binders of 0x0041E5F2 respelled to
    NUISystem::CManager::GetInstance; 1,759 CThing rows on engine/CThing.h (GetTC/PeekTC lifted behind three sub-macros);
    coordinator regrade 2,006/2,006 PASS. The rebuilt plan tools are committed in `plan_p6_w1/scripts/`. Phase 7 brief:
    `plan_p6_w1/NEXT.md` (2,431 deferred CThing rows by class, waves 3-4 CIntelligentPointer / CTCBase / CThingCreatureBase,
    62 old-spelling binders in `plan_p6_w2/CALLERS.tsv`). Refresh the index first (`$S/plan/symindex2.py $S/plan/idx --all`).
  - **Plan phase 7 applied** (`naming/2026-11-05_plan_p7_w1/README.md`): w1 87 old-spelling binders (incl. CScriptThing::NullCoord
    0x0143E8E0, CThing::PeekPlayer), w2 123 CThing leftovers, w3 388 CIntelligentPointer leftovers; coordinator regrade
    6,778/6,778 PASS. Phase 8 brief: `plan_p7_w1/NEXT.md` (wave 4 CTCBase / CThingCreatureBase, the CThing deferral classes, the
    wave 1 naming decisions incl. 0x004BFFC0 ClearHighlighted vs ClearMeshEffect). Refresh the index first.
  - **Running (the user: "continue authoring chunks on another agent"; 2026-11-06: "you can have two chunk authors"):** chunk 794
    (`$S/w22/agent794`: CThingObject::OnHit, CTCSummonSpell::CreateMinion, CTCWallMount::AttachObject,
    CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed, CTCHealLife::StartHealingEffect, CThingAICreature::Save) and chunk 795
    (`$S/w22/agent795`: CCombatAction_KickThingOnGround::PerformAction, CTCQuestCompletionUI / CTCHeroReceiveItems::GlobalUpdate,
    CQuestManager::LoadGameState, CTCHeroOnlineScoreboard::ChooseAClan, CAIStateGroup_MinionReactToPlayerInteraction::
    TestOrDoInteraction). Land each like 782-792 with the next free letter (`landing_u`, `_v`), `pending/2026-11-05_linux_hand79N`,
    labels `2026-11-05<letter>` incl. the landed rows' own labels, queue tag `land0511<letter>`. Each agent runs at most 2
    grades. When one lands, cut the next chunk so two authors stay busy. Chunk 793 (`_t/`) parked all six x87-heavy engine rows:
    prefer game-logic rows (CTC*, CThing*, quest/UI, AI state) over renderers and maths. Pool: the groups ledger (`pending/2026-11-03_groups/ROWS.tsv`) still has about 174 unworked rows (the earlier "about
    40" counted only rows with no remaining callees). Take rows that are 16-byte aligned (an odd start such as 0x00651273 is a
    mid-function entry) and check each extent for a second function after its tables. The parked 782-793 rows are the other pool.
  - Landed since the resume: chunk 782 4 of 6 (`batches/2026-11-05_landing_i/`), 783 1 of 6 (`_j/`), 784 1 of 6 (`_k/`), 785 2 of 6 (`_l/`), 786 4 of 6 (`_m/`), 787 6 of 6 (`_n/`), 788 6 of 6 (`_o/`), 789 2 of 6 (`_p/`), 790 4 of 6 (`_q/`), 791 4 of 6 (`_r/`), 792 2 of 6 (`_s/`), 793 0 of 6 (`_t/`). The
    parked rows are in `pending/2026-11-05_linux_hand782..793/`.
  - A container restart stopped chunk 784 once; the scratch survived, and the agent was resumed from its transcript.
  - Open naming decision: 0x0122DEDC (the shared 0.0f, 30+ spellings, no PDB name), like EPSILON / TWO_PI on 2026-11-01.
  - The FSE Lua ports of the quest scripts are in the repo: `refs/script_recovery/seed_corpus/sources/<Quest>/FSE/`.
- Paused state before that (2026-11-05):
  Exact progress at the pause and the resume steps: `docs/journal/2026-11/2026-11-05_SESSION.md`.
  - Coverage: 46,897 genuine of 49,568 (94.62%); 57.63% of retail function bytes.
  - One asm bake left (0x009BEF80, parked); 0 in-row `__asm`; 0 reloc-masked false matches.
  - Naming/header plan: steps 1-2 done; steps 3-4 partly (phases 3-5); step 5 not started (CThing 4,190 local rows first);
    step 6 first pass only. Phase 6 brief drafted, not launched (`naming/2026-11-05_progress_snapshot/`).
- Branch `FullDecomp` (`JeremyC57/FableDecomp`). Owner 2026-11-04 resumed authoring: at most 3 concurrent agents, 10+ attempts per
  row, verify and land before launching more. **Asm rules: `docs/pipeline/ASM_RULES.md`** (owner-confirmed 2026-11-05): plain C++ first; beyond it only where
  the original code used asm (proven from retail bytes): intrinsic, then a shared helper header with a `FABLE_PORTABLE` twin, then
  `__forceinline` only if necessary; never for a residue; no asm in a row source or test; such rows land as `retail_asm`.
- Chunks 738 (CPSCRenderSprite), 739 (CPSCUpdateNormal), 740 (CPSCRenderMesh) cut from the particle groups; brief
  `$S/w22/BRIEF_738.md`, work in `$S/w22/chunk73{8,9}`, `chunk740`. 739 landed 2 of 5 (`batches/2026-11-04_landing_a/`), 740 3 of 5 (`_landing_b/`), 738 3 of 5 (`_landing_c/`), 741 2 of 5 (`_landing_d/`), 743 11 of 11 + 4 Vorbis re-models (`_landing_e/`), 742 2 of 6 (`_landing_f/`); rest staged in
  `pending/2026-11-04_linux_hand73{8,9}/`, `_hand74{0,1,2,3}/`. 744 (CQ_HangingTreeGoodScript) landed 1 of 5 + the GetGuardsHealth
  respelling (`_landing_g/`), 745 (CV_HeroDuelScript) 4 of 4 (`_landing_h/`), 746 (MurderTwist sheriff/guard) 4 of 5 (`_landing_i/`); staged in `pending/2026-11-04_linux_hand74{4,5,6}/`.
- 747 (retry of the 65 parked bare-fistp rows) landed 11 as retail_asm (`_landing_j/`), 17 parked + 37 deferred in
  `pending/2026-11-04_linux_hand747/` (per-site C++/intrinsic/macro attempts table there).
- 748 (8 hand-SSE parks) landed 1 via the new macro isolation headers (`_landing_k/`); the other 7 have exact SSE blocks and x87/frame
  residue (`pending/2026-11-04_linux_hand748/`). Next asm-rule work: de-bake the 13 landed asm_bake rows (e.g. 0x0042DF9E, 0x005BD993,
  0x00899EF0) with the same order (C++, intrinsics, macro vs function isolation); the 10 LTCG reg-arg parks (pure C++).
- Fable Anniversary exe (owner-supplied): naming waves high + 2 + 3 applied (`naming/2026-11-04_anniversary/README.md`, 2,523 labels,
  every name proven from TLC). **Naming pass wave 35** (agents write overlays, the coordinator applies and re-grades): 35a (923
  slot functions, `naming/2026-11-04_wave35a/`) and 35c (271 spawned-func slots, particle getters, queued fixes and their callers,
  `naming/2026-11-04_wave35c/`) and 35b (812 Main/Init/Alloc/ctors, `naming/2026-11-04_wave35b/`) and 35d (24 of the 30 35a D5 rows + follow-ups,
  `naming/2026-11-04_wave35d/`) are applied. Left: the NAME_FIXES lists of 35b/c/d, 6 `??_G` needing member models, ~CScriptBase.
  **Now (2026-11-05):** authoring resumed with ONE expert agent (owner). The owner asked to fix the 9 old bakes. Six are now clean C++:
  0x0042DF9E and 0x00551340 (`batches/2026-11-05_landing_a/`), and 0x004167A7, 0x005DACF0, 0x009A8170 and 0x009A8840
  (`_landing_b/`, chunk 775; the bank bakes moved to `rebuild/runtime/openretailbank/bake/`). Chunk 776 (`_landing_c/`) made 0x0053C332 and 0x00597BF2 plain C++ too, so 8 of the 9 are
  done. The last bake, 0x009BEF80 SetViewport, is parked 2 bytes short (DIFFER 477v479; `pending/2026-11-05_linux_hand776/`). It is a pure
  compiler residue: plain C++ only, or it stays.
  Chunk 777 (5 ledger rows, `_landing_d/`) landed 3; 0x0055F710 and 0x00803540 are parked in `pending/2026-11-05_linux_hand777/`. The
  groups ledger has only large (1 KB+) Fable rows left. Chunk 778 (`_landing_e/`) landed 2 (CUIDef::operator=, a CPersistTraits
  TransferIn); 3 parked in `pending/2026-11-05_linux_hand778/`. Plan phase 5 is applied (4,128 rows, regrade all PASS;
  `naming/2026-11-05_plan_p5_w1/README.md`); phase 6 is drafted, not launched.
  Asm rules: `docs/pipeline/ASM_RULES.md` (canonical). No row source has in-row `__asm`; 211 retail_asm rows use 26 shared helpers. Landed today: plan phases 2-4 (`naming/2026-11-0{4,5}_plan_p*`), stand-in
  naming s6 (1,796 rows; 7,946 inaccurate stand-ins left, `naming/2026-11-05_standin_s6/NEXT.md`), asm cleanup rounds 1/2a/2b
  (`naming/2026-11-05_asm_*`), and the volatile audit. Next after authoring: plan phase 5 (CScriptThing header, CThing offset waves,
  CCountedPointer/CIntelligentPointer, vendor headers; inputs `$S/plan/p4/NEXT.md`), then the rest of step 6. Phase 1: `naming/2026-11-04_vendor_v2/`.
  Owner 2026-11-04: batch/pending `oracle.tsv` files are no longer committed (.gitignore; landing-time input only, landed bytes live in the catalog oracle); the 1,646 already tracked are left as they are. Owner 2026-11-04 (latest): launch NO new chunks; once the next running chunk lands, implement the combined naming/header plan: (1) repo-wide symbol index, (2) vendor V2 rewritten header-first, (3) scripted value-class header waves, (4) CScriptThing family, (5) game-class offset-mapped header waves carrying their name fixes, (6) final small naming pass; strict mode for migrated classes. Earlier: after parked chunk 768 finishes, launch no more parked-row agents for now; free slots go to authoring. Vendor naming V1 applied 2026-11-04 (198 respells, 75 strmbase labels; `naming/2026-11-04_vendor_v1/`); V2 queue in its README. Owner 2026-11-04: vendor code gets the most accurate name from the vendor source or from what the body does (EXTRA_RULES); V1's 47 RELOC_NAME rows accepted; V2 takes the next free slot (`$S/naming/vendor/BRIEF_VENDOR_V2.md`)., C++ only. Landing helpers: `$S/w22/prep_landing.sh N L`, `finish_landing.sh N L`. Anniversary 360 Test PDB pilot done: no byte help (0/20 matched; `batches/2026-11-04_anniv360_pilot/`).
  The Xbox 360 Anniversary PDB is a private third-source lookup (`$S/anniv_pdb/`, rule in FABLE_ANNIVERSARY.md).
- Coverage (refresh with `tools/write_decomp_dashboard.py` then `tools/update_readme_progress.py`; never hand-edit):
  46,897 genuine of the 49,568-function catalog (94.62%; 57.63% of retail function bytes), after landing 2026-11-05e.
  (chunk 766 authoring, 7 of 26). Records: `rebuild/candidates/manual/batches/2026-11-03_landing_b/` .. `_landing_h/`, `2026-11-04_landing_a/` .. `_landing_zc/`.
- `// Revisit: inline-depth` rows: 65 (129 at the start of the day). Mechanism, levers and why the 65 are left:
  `docs/pipeline/INLINE_DEPTH_REVISIT.md`; research `batches/2026-11-03_inline_split_research/`, `_inline_revisit2/`, `_inline_revisit3/`.
- Codex: pass 13 landed 47 of 51 (`batches/2026-11-03_codex_pass13_landing/`; data names for `cvdump` in its NAME_DECISIONS.tsv);
  0x00501450 held on CVectorMap::LowerBound. Pass 14 was never sent; its rows are released (`pending/2026-11-02_codex_pass14/RELEASED.tsv`).
- Staged, not landed (best drafts in each `pending/2026-11-03_linux_handNNN/differ/`):
  - 733: 0x004075C0, 0x0040BCA0 match but wait on the owner's `cvdump` for CUIScreenUnitConverter BackBufferWidth/Height access.
  - 734: PreparePrimitivesForEngine, AutoLoadRespawn, LoadQuestsFromFile (DIFFERs); Init blocked on CWorld branch flattening.
  - 735: AddAbilityToList(PC) (two-call compare order), OpenInventoryInternalPC (member-sum reassociation).
  - 736: SetUpRegions (copy-ctor inline split); CMap::LoadFromFile 0x00821A80 and CNavQuadTree::LoadFromFile 0x00A7AF60 never attempted.
  - 737: both CPSC PreparePrimitives (frame-slot sharing); 0x00B087C0's test fails to compile under vland.
  - Older: 723 r2/r3, 724 r3, 727 (DECISION_00b625e0.md), 728-731, 721 r4 (its LIST_MEMBERS block must shrink when it lands:
    `batches/2026-11-03_landing_g/scripts/fc_721_rebase.diff`).
- Chunks are cut from the category A groups with `$S/cut_groups.py N <group ids>` (reads `pending/2026-11-03_groups/ROWS.tsv`,
  skips landed and chunk_history rows; 714 unworked rows remain after chunk 737); oracle via `oracle_from_exe.py`.
- Scratch is limited: the session disk allowance filled once at 21 GB under `$S`. Delete landed-and-committed scratch after each round.

## Lane (already set up in the scratch directory)
```
export S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad; source $S/wine/env.sh; export S
# (2026-11-05 late: the 40effb6c-... scratch is gone; this older one holds the same lane. Check `ls $S/wine/env.sh` first.)
cd /home/user/FableDecomp   # every tool runs from the repo root
```
`$S` holds Wine + cl 13.10.3077 RTM (`$S/wine/vc71rtm`), QFE 4035 (`$S/wine/vc71`), VS 2003 libs, LZO, and the game
(`$S/dl/game/Fable The Lost Chapters/Fable.exe`, never committed). A fresh container rebuilds it with
`docs/pipeline/FRESH_SESSION_HANDOFF.md`. Write scratch only under `$S`, never under `/tmp` or `/`.
- Leftover agent scratch at the filesystem root: `/codex12land`, `/codex13land` (pass 12/13 landing scratch, both passes landed).
  Leave them for the owner; the safety check blocks agents from deleting outside the scratch directory.

## Land / repair one row (T = tools/decomp_pipeline/linux_lane/lane_tools)
1. **Payload:** `{"result":{"authored":[{address,module,name,source_cpp,test_cpp,pass_pattern,leaf,flags?}]}}`.
2. **Oracle:** the catalog row (`address name length bytes`), or `python3 $T/chunk_pool/oracle_from_exe.py rows.tsv NAME` for a new row.
3. **Read-only grade:** `FABLE_HDR_OVERLAY=<hdr> LANDVERIFY_WORK=<dir> python3 -u $T/vland.py payload oracle`. Add `--recheck-landed` for a row that is already landed.
4. **Headers:** `python3 $T/coordinator/hdrcheck2.py <hdr>` must print `default=same` for every changed header (it stubs system headers; `hdrcheck.py` can compare nothing on fable_string.h). Recheck every landed includer and every definer of a touched FABLE_RETAIL_ macro: `python3 $T/recheck.py <out> <src paths>`, then vland `--recheck-landed`, 4 parallel splits.
   - A header with only `#pragma once` (fable_string.h) is included twice under a partial overlay (C2011). Recheck on a full mirror (`cp -r rebuild/include X; cp -r hdr/. X/`).
5. **Re-landing a landed row:** `python3 $T/coordinator/repair_remove.py <addr>` first.
6. **Land:** `python3 $T/chunk_pool/fixmove.py P; python3 $T/naming/keypayload.py P; LANDVERIFY_WORK=<dir> python3 -u $T/vland.py P O --land`.
7. **After landing:**
   - `python3 $T/coordinator/bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` must print 0.
   - Labels file `ghidra_out/labels_*.tsv`, then `python3 $T/coordinator/apply_labels.py <labels>` and `python3 tools/sync_catalog_names.py`.
   - Write records in `rebuild/candidates/manual/batches/<date>_<name>/` (README, payload, oracle, verify.log, includer_recheck_grades.tsv).
   - Second spellings go to `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv` as PRIORITY. Append ` [<date>: ...; done]` to the note when closed.
   - Run vland with a 120 s Bash timeout or in the background: a fatal cl error under Wine can spin at 100% CPU for minutes.

## Owner rules in force (latest first)
- 2026-11-03 "whichever way retail does it": SDK structs take `<pshpack1.h>/<poppack.h>`. An engine class retail packed (odd PDB offsets) takes `#pragma pack(push, 1)` in its own FABLE_RETAIL_<CLASS> branch: C2DVector, CIntelligentPointer.
- 2026-11-02: retail faithfulness in every decision; names right the first time (two sources, exact PDB spelling, one address = one name; respell old callers in the same commit or queue PRIORITY).
  - One class, one retail model, behind FABLE_RETAIL_<CLASS> with the default path unchanged.
  - Never add a device retail did not have: no inline_depth, no forwarding overloads, no member-dropping or invented local class copies, no pointer casts between "descriptive" element types.
  - Agents get 10+ attempts per row; no stopping agents mid-landing.

## Next (in order)
1. Land chunk 733's two rows once the owner's `cvdump` settles the CUIScreenUnitConverter statics' access (private: `@@0`, and the landed
   `@@2` binders get respelled).
2. **Priority name fixes:** 518 open in `NAME_FIXES_QUEUE.tsv` (507 addresses; note starts with PRIORITY, no ` done]`). Cheapest first;
   the day's rounds queued 39 re-model lines (`land0311e/f/g/h`).
3. Attempt the two never-tried LoadFromFile rows (chunk 736 NOTES.md has their callee maps) and retry the parked chunk rows above.
4. The 65 inline-depth rows: the CScriptThing special-member split first (16+ rows share it).
5. Older open work: `docs/PLAN.md`, `docs/ROADMAP.md`, pass 11 leftovers (`pending/2026-11-02_codex_pass11/retry_differ/`).
