# Resume here (one page) — updated 2026-11-05 late (plan phase 7 and chunks 782-825 and 827 recorded; chunks 826 and 828 running)

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
  - **Running (the user: "continue authoring chunks on another agent"; 2026-11-06: "you can have two chunk authors"):** chunk 826 (`$S/w22/agent826`: six retries,
    CContextSensitiveItemDef::IsValidOverride 0x00850A80, CQ_HeroSoulsArenaScript::WatchForRoundsInArena 0x00D7FE90, CGhostGrannySon
    0x00E8EF80, CSwordCrowd 0x00ED5F50, CAIStateGroup_DeedReactionRoot::Update 0x00960120, CAIStateGroup_JackDragon::UpdateHover
    0x00951FB0) and chunk 828 (`$S/w22/agent828`: six retries, CMeleeOpponent
    0x00D56790, the trophies CBriarRose 0x00E9E420, CBanditForger 0x00D0C410, InformRelevantThingsOfDeed 0x006C6040, CAS_Trickster
    0x00E2C100 at 2 lines, CAIStateGroup_JackOfBladesStage2::Update 0x00974AA0). Rows were picked to avoid the CIsThingAlive copy
    blocker. A retry that matches needs its pending header overlay copied in; a Main outside
    the manifest goes into `rebuild/manifest/boundary-starts.tsv` + `crawl/manifest_add_boundaries.py --write` first. Measure a cut
    by its first `ret` + int3 unless a jump table follows (jtext's maxbranch can sit in the next function: chunk 818). Land
    each like 782-818 with the next free letter (`2026-11-07_landing_b`, `_c`; queue tags `land0711b`, ...),
    `pending/2026-11-05_linux_hand79N`, labels `2026-11-05<letter>` incl. the landed rows' own labels, queue tag
    `land0511<letter>`. Diff every overlay header against rebuild/include first and three-way merge any that would drop another
    chunk's lines (796's overlay predates the 794 / 795 landings). When a hand-back extends an existing
    opt-in macro, recheck the landed sources that define it (GOTCHAS 2026-11-06 coordinator line). Each agent runs at most 2
    grades. When one lands, cut the next chunk so two authors stay busy. Chunk 793 (`_t/`) parked all six x87-heavy engine rows:
    prefer game-logic rows (CTC*, CThing*, quest/UI, AI state) over renderers and maths. Pool: the groups ledger (`pending/2026-11-03_groups/ROWS.tsv`) still has about 200 unworked named rows (plus 213 Unwind funclets), mostly renderers,
    audio / FFT and large inventory-UI builders; about 190 unnamed `gapscan` rows, most in the script range 0x00CF-0x00F2 (the script
    Mains left are over 3.6 KB, e.g. CHobbeTorrent::Main). Take rows that are 16-byte aligned (an odd start such as 0x00651273 is a
    mid-function entry). Game-logic chunks 800-802 landed 5 of 18, against 33 of 48 for script chunks 786-799; the unnamed gap-scan
    rows in the script range are placed by their landed neighbours and their single .rdata reference (chunk 803 proved all six
    names and landed 4; about 60 such rows remain (`$S/w22/gaps_left.txt`; after chunk 819 the gap-scan Mains left are 0x00EBCB60 MonitorParanoidQuest (176 B, uncatalogued until chunk 818), 0x00E44980 (uncatalogued, after CMagicman), 0x00D4A270 (19.8 KB+, jump table) and 0x00CE7670 (32 KB), so later chunks are mostly retries of parks; 0x00D2AA40 CQ_EndGameFocalSitesScript::Main has a jump table inside its extent: measure it by hand), most 4-15 KB entity Mains that follow their entity's scalar deleting dtor). The ledger's byte counts often run on into the next functions: end each row at its own int3 padding or
    after its switch tables (a byte index table can end off a 4-byte boundary). The parked 782-819 rows are the other pool (gap-scan chunks 803-819 landed 26 of 102; `$S/w22/parked_all.txt` lists 172 parks with their residue line counts; chunks 817-819 mix three retries with three new rows; the implicit MovieBase dtor: where retail mixes expanded and called sites, the implicit dtor + the declared-only ~CCountedPointer<MovieBase> reproduces it (chunk 814); where retail expands every site (call 0x6E7AB0 + GROBase vptr store + call 0x0099A430; 0x00CF68C0) it turns on frame-slot sharing of the movies and is still open; 0x00D647F0 CFinalMaze is 10 masked lines (one register choice) from a match; the 0x00C0-0x00CA gap rows are compiler adjustor / vcall thunks, not authoring material; 0x00D52E90 (11.9 KB, vtable 0x012CF840 slot 1, after CPreMeleeWhisper::Main) is a Main in neither the manifest nor the gap scan; chunk 810's six entity Mains all hit the CIsThingAlive / CIsThingConscious nested-copy blocker, so prefer quest-own Mains and jump-table Mains (`$S/tools/jtext.py <addr> <next>` measures a row whose switch tables follow its code); several parked ones differ only in register choice or block layout) (0x00DE2610 waits on the pragma-free inline-depth mechanism, not on more attempts).
  - Landed since the resume: chunk 782 4 of 6 (`batches/2026-11-05_landing_i/`), 783 1 of 6 (`_j/`), 784 1 of 6 (`_k/`), 785 2 of 6 (`_l/`), 786 4 of 6 (`_m/`), 787 6 of 6 (`_n/`), 788 6 of 6 (`_o/`), 789 2 of 6 (`_p/`), 790 4 of 6 (`_q/`), 791 4 of 6 (`_r/`), 792 2 of 6 (`_s/`), 793 0 of 6 (`_t/`), 794 3 of 6 (`_u/`), 795 2 of 6 (`_v/`), 797 3 of 6 (`_w/`), 796 3 of 6 (`_x/`), 798 3 of 6 (`_y/`), 799 5 of 6 (`_z/`), 800 2 of 6 (`2026-11-06_landing_a/`), 801 1 of 6 (`_b/`), 802 2 of 6 (`_c/`), 803 4 of 6 (`_d/`), 804 1 of 6 (`_e/`), 805 2 of 6 (`_f/`), 806 1 of 6 (`_g/`), 807 2 of 6 (`_h/`), 808 1 of 6 (`_i/`), 809 3 of 6 (`_j/`), 810 0 of 6 (`_k/`), 811 1 of 6 (`_l/`), 812 2 of 6 (`_m/`), 813 2 of 6 (`_n/`), 814 2 of 6 (`_o/`), 815 1 of 6 (`_p/`), 816 0 of 6 (`_q/`), 817 3 of 6 (`_r/`), 818 1 of 6 (`_s/`), 819 0 of 6 (`_t/`), 820 2 of 6 (`_u/`; 0x00DA91A0 held, see below), 821 2 of 6 (`_v/`), 822 2 of 6 (`_w/`), 823 0 of 6 (`_x/`), 824 1 of 6 (`_y/`), 825 3 of 6 (`_z/`; 0x00E44980 added to the manifest), 827 1 of 6 (`2026-11-07_landing_a/`); from 810 on, the header overlay of parked-only rows is kept in the pending dir because no landed row needs it. The
    parked rows are in `pending/2026-11-05_linux_hand782..799/` and `pending/2026-11-06_linux_hand800..825/` and `pending/2026-11-07_linux_hand827/`.
  - A container restart stopped chunk 784 once; the scratch survived, and the agent was resumed from its transcript.
  - Naming: 0x0122DEDC (the shared 0.0f) is the compiler literal `__real@00000000` (chunk 796 evidence, queued `land0511x`).
    Open (chunk 800): 0x00544D20 binds the NUISystem CCountedPointer<CAction> / <CComponent> dtors to the 5-byte jmp stubs
    0x00429B72 / 0x00429C53, other rows to the bodies 0x00429160 / 0x004291DE (one symbol, two addresses; queued CHECK `land0611a`).
    Owner question (chunk 804): the three CQ_ArenaHoldingScriptScript fan Mains (0x00CF18D0 / 0x00CF2B70 / 0x00CF3D20) match
    only under the undocumented VC7.1 flag `/d2inlS20` (the caller-size inline cap), and chunk 809's CNOVI_Guard / CGhostGrannySon
    Mains (0x00DAC760 / 0x00E8EF80) only under `/d2inlT72` (the inline threshold); they stay parked unless the owner rules on it.
    Owner question (chunk 820): 0x00DA91A0 CMapKeeper::Main matches only through `reinterpret_cast<const CScriptThing&>(escort).IsAlive()`
    on a by-value CScriptThing local (keeps the virtual call that VC7.1 otherwise devirtualises): the cast-only form of the I-44 view
    device. Held unlanded (source in `pending/2026-11-06_linux_hand820/differ/`); if approved it could also replace the view struct in
    the six landed CScriptThingView rows. Chunk 821: `/d2inlS15` (diagnostic) matches 0x00D2ED20 / 0x00D2D930. Naming open: 0x0129BA3C
    EPSILON v DELTA_NOTIONAL_ZERO; 0x004BFFC0 also bound as the placeholder ClearMeshEffect_004bffc0 (queued `land0611v`).
    Review (chunk 827): parked 0x0094AB00 GoToPub::Update reached 246 lines (from 743) with row-local `__forceinline` list push_back /
    LowerBound helpers at the two sites retail expands (the 0x008DDAD0 precedent): decide whether that counts as an invented local copy.
    Open decision: `pGlobalScriptDef` 0x0143E90C is spelled `CScriptDef*` in 148 landed rows and `CDefPointer<const CScriptDef>` in
    15 (queue lines `cg10`, `land0511x`).
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
