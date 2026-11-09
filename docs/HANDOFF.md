# Resume here (one page) — updated 2026-11-09 (lane path) / 2026-11-08 late (chunks 782-853 recorded; AUTHORING STOPPED by the user; chunk 854's four saved matches landed)

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
  - **Fable-model agents (the user, 2026-11-07):** "dont relaunch the fable agents for now", then "start a fable retry agent on retrying even
    harder on the ones from 839 and 840 that didnt land": chunk 844 landed 4 of those 11 rows (`2026-11-07_landing_u/`); the 7 left
    (0x00C52BB0, 0x0058301A, 0x0047ED76: 2-4-line ties; 0x00E76E90; three documented blockers) are in `pending/2026-11-07_linux_hand844/`.
    Then the user asked for another Fable retry agent, "15 rows with at least 50 attempts each" (50+ by that agent itself): chunk 847
    (landed 5 of 15, `2026-11-07_landing_y/`; the other 10 are 2-8-line ties after 53-1071 attempts each, `pending/2026-11-07_linux_hand847/`) retried fifteen 4-7-line near misses no expert had worked (0x00C75F80, 0x00A852B0, 0x005029A0, 0x0065B142,
    0x0063E590, 0x00C71F80, 0x00A22E10, 0x00B92300, 0x00F36A30, 0x006AFE60, 0x006A9690, 0x0075E270, 0x00760A70, 0x00986ED0, 0x0085C7D0).
    No Fable agent is running; start none unless the user asks.
  - **Resumed 2026-11-08 late** (the user: "continue where the last claude chat left off"; authoring stays stopped, no agent started):
    the a013f3ec-... scratch is gone; the older 5221bbec-... scratch holds the whole lane (game SHA-1 checked; the Lane block below
    names it, but `$S/w22/*.sh`, `$S/w22/jtext.py` and `$S/plan/` were in a013 and are gone). Chunk 854's four saved matches landed
    (`2026-11-08_landing_e/`: CMouseDX::Update, LHHttp2::ReceiveDocumentAsync with a missing `operator new[]` declaration added,
    GetLevelFilesFromWorldFile on its N+1 row, CAIStateGroup_KrakenAttack::Update; two tests written) plus the CMouseDX ctor 0x00AB5D00
    respelled to the PDB `CMouse::EmptyEventStore`. Coverage 47,013 genuine (94.85%; 62.40% of bytes). **Do not run
    `tools/sync_catalog_names.py` bare and commit it:** it now rewrites 4,284 unrelated names (a backlog, some to placeholders such as
    `T::Get`); keep only the landing's addresses (landing_e README) until that backlog is reviewed.
  - **Authoring RESUMED (the user, 2026-11-08 late: "continue chunk authoring ... Two agents, both experts at game decompilation ... large
    attempt budgets per row").** Running: chunk 855 (`$S/w22/agent855`: six never-attempted 0.9-2.1 KB rows + chunk 854's two WIP rows)
    and chunk 856 (`$S/w22/agent856`: expert retry of ten parked near misses, 2-22 lines, no expert had worked them), 50+ attempts per
    row. Briefs `$S/w22/agent85N/CHUNK_BRIEF.md` (copied into the pending dir at landing). Land each like landing_e (next letter `_f`).
    Chunk 856 finished **0 of 10** (51-103 grades per row, no source beat the parked best; saved `pending/2026-11-08_linux_hand856/`,
    nothing to land; it reports the 0x00AAD650 oracle extent mis-cut: 1574 + int3, not 1840). Its slot went to chunk 857
    (`$S/w22/agent857`: eight never-attempted 1.6-3.5 KB engine / UI rows, several forge-autoname placeholders to prove).
    Chunk 855 finished 3 of 8 (RELOCATION_MATCH: floor1_encode 0x00CA2BB0, CSubtitleRenderer::GetGraphicFromEmbeddedTag 0x00490BE0,
    NUISystem::CComponent::UpdateColour 0x0052F900; landed `2026-11-08_landing_f/`, coverage 47,016 genuine, 62.45% of bytes;
    queue land0811f PRIORITY: respell the landed CCharString ToUpper / ToLower 0x0099EE20 / 0x0099ED10 to the PDB GetAsUpperCase /
    GetAsLowerCase with their 22 callers). Its slot went to chunk 859, a **Fable** retry agent (the user: "give
    huge budget ... keep trying every row until half or more match"): twenty parked 2-15-line near misses, rounds over all rows, goal
    10+, `$S/w22/agent859`. Chunk 858 (six small gap-scan rows + four 3.3-3.5 KB UI rows, `$S/w22/agent858`, brief written) is cut
    but HELD (not launched, not in chunk_history) until a lane slot frees.
    Chunk 857 finished 2 of 8 (ComputeVertexCost 0x00AC8C80, CNavigatorAStar::CullWayPoints 0x00A1C010; landed `2026-11-08_landing_g/`,
    queue land0811g) but stopped its six other rows early (four had one draft); they went on to chunk 860 (`$S/w22/agent860`, 100-attempt
    floor, tests required). Running now: chunks 858 (launched on the user's "do another chunk agent"), 859 (Fable retry) and 860.
    Chunk 859 round 1: 3 of 20 (CVectorMap::Insert 0x00A852B0, CreateEditBoxActions 0x0058301A, ResolveRelativeMovementAcceleration
    0x006A9690; landed `2026-11-08_landing_h/`, queue land0811h) after only 89 grades; sent back for round 2 (goal 10 of 20, 100+ grades per
    unmatched row; its matches will land as new rows only). Round 2 (2,283 grades in all): +1, CCombFilter::Allocate 0x00C75F80 plain MATCH
    (`2026-11-08_landing_i/`); 4 of 20 total, the other 16 parked after 100-254 grades each with no movement. Chunk 859 is finished.
    Chunk 860 finished **0 of 6** after 1,160 grades (`pending/2026-11-08_linux_hand860/`; ClearRenderTarget md 32 -> 12, SellItem md 349 -> 76,
    both still need the shared-header rebase / last register tie; name fixes queued land0811j). Running: chunk 858 and a Fable **research**
    agent (`$S/w22/research861`, the user: research VC7.1's register / scheduling ties and remedies, full web access; deliverable
    RESEARCH.md -> docs/pipeline/ at landing; test set = the 26 parked rows of chunks 856 + 859, oracle `$S/batch/chunk861_oracle.tsv`).
    Chunk 858 finished **7 of 10** (CFrontendGameComponent::Output + six small gap-scan rows; landed `2026-11-08_landing_j/`, queue land0811k;
    CSystemManager font-manager offset is header debt, see its pending README). The research agent finished: `docs/pipeline/VC71_CODEGEN_RESEARCH.md`
    (ranked remedies in section 6; put them in every new chunk brief; highest-value follow-up: a winedbg probe of c2.dll's
    `color.c` allocator priorities, section 7). That probe is now built: `docs/pipeline/C2_ALLOCATOR_PROBE.md`,
    tool `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py` (diagnoses 0x0085C7D0 / 0x0050A650 / 0x005253F0 in its section 6).
    Running (the user: "launch new agents with the research rules as well as a fable retry agent with the new research rules"):
    chunk 862 (seven tiny `$E` / atexit initializers + Script_Villager_Reputation_Dialog 0x00DAADD0 + CGuiComponent::Update 0x00531EC0),
    chunk 863 (eight never-attempted 2.4-4.1 KB rows: four script functions, CPlayerGui ctor, CLightningBeam::FrameUpdate, two unnamed) and
    chunk 864 (Fable retry of twenty parked near misses, research rules + probe, goal 10 of 20). Briefs `$S/w22/agent86N/CHUNK_BRIEF.md`
    carry the shared block `$S/w22/RESEARCH_RULES.md`.
    Chunk 862 finished 7 of 9 (the seven static initializers; landed `2026-11-08_landing_k/`, queue land0811l); its two parks (0x00531EC0,
    0x00DAADD0) are pure inline-budget splits. Owner decision 2026-11-08 (EXTRA_RULES, "Owner decision, 2026-11-08"): an inline-budget
    residue may use an inline control (per-row `/d2inlT<N>` / `/d2inlS<N>` flag first, else a scoped `#pragma inline_depth` / `noinline`) when
    the row matches, its test passes and it stays portable clean C++; line 2 `// Inline-budget: ...`. Chunk 865 lands the inline-budget parks.
    Chunk 863: 1 of 8 (CPlayerGui ctor 0x0043B570, `2026-11-08_landing_m/`). Chunk 864: 2 of 20; 0x00D33730 CTheRealGuildmaster::Main landed
    (`2026-11-08_landing_l/`, the first row landed with an inline_depth region under the new rule); 0x0050A650 went back to its agent
    because its header opt-in holds stand-in template bodies that do not behave like retail's 0x00518340 / 0x005185F0 / 0x00518900.
    0x0050A650 landed after the redo (`2026-11-08_landing_n/`, header templates written from retail's logic). Queue tag land0811m holds the respells.
    Chunk 865 (inline-budget parks): 11 of 20 landed (`2026-11-08_landing_o/`; nine by a per-row /d2inlS15 / T36 / T72 flag with the source
    untouched, two with a scoped device); 0x0083F650 landed after a shared-STL rewrite (`2026-11-08_landing_p/`, 443-byte extent).
    Chunk 866: 10 of 10 Ogg Vorbis rows landed (`2026-11-08_landing_q/`, real Xiph names).
    Chunk 868: 2 of 10 landed (`2026-11-08_landing_r/`: CBH_GangDeputy2::Main, CTalkingTrader1::Main); its parks continue. Chunk 869: 19 Vorbis functions landed (`2026-11-08_landing_s/`; 2 parked).
    Chunk 878 (libpk): 2 of 3 landed (`2026-11-08_landing_z/`, SSE2 FIR resamplers on dvec.h; SetEAXProperty parked at 10 lines).
    Chunks 877 / 879: 0 landed (OnHit, PopulateRegionWithVillagers parked as full bodies; hand877 / hand879). Chunk 880: OpenPCOptionsMenu landed
    (`2026-11-08_landing_aa/`). Chunk 881: both stereo SSE2 FIR resamplers landed
    (`2026-11-08_landing_ab/`). Chunk 882: ConstructQuestListPC 0x0061E6D0 parked
    at DIFFER(12357v12356) (inline budget spent earlier in retail; `pending/2026-11-08_linux_hand882/`). Chunk 883: CTCInventoryMap::ConstructLogbookListPC
    0x006096E0 parked at DIFFER(12832v12832) (/d2inlT71 fixes the call sequence; 17 register-allocation segments left;
    `pending/2026-11-08_linux_hand883/`). No chunk agents running (owner). Single-row worker
    parked 0x00CBFB7D RunCutsceneMacro at DIFFER(74879v74778) (95% exact, 99% call order; the caller-size inline cap falls between
    /d2inlS8 and S9 on our STLport/CCharString header costs -- a lane gap, `pending/2026-11-08_linux_hand876/`) ; row 885 (per-site inline devices on
    /d2inlS8) cleared every inline-site difference: DIFFER(74965v74778), 96.4% strict / 98.7% masked / 99.2% calls; left: register
    allocation (prologue name/-1/def roles), frame 0x18E4 v 0x18DC, cross-jump tails (`pending/2026-11-08_linux_hand885/`). Single-row 884: CTCBase::RegisterTCs 0x004EE23F (44,806 bytes) landed (`2026-11-08_landing_ac/`).
    Chunk 890 (expert, instrumented): CTable::ConstructSpritesToDraw 0x00551EA0 landed (`2026-11-08_landing_ad/`); 0x0056AD40 (= CTCBoastUI::Activate)
    and 0x00498490 DrawRetailDisplay parked undrafted (`pending/2026-11-08_linux_hand890/`). Chunk 889 parked both rows (`pending/2026-11-08_linux_hand889/`).
    Codex merge (2026-11-08): Action 0x0041E6D3 now DIFFER(35702v35651), frame 0x800 = retail, all six Codex fixes kept (hand886_wip);
    RunCutsceneMacro 0x00CBFB7D DIFFER(74882v74778), frame = retail, 89.8% exact / 98.5% regs+frame masked (hand885). Both: entry
    register roles + slot tie order left. Chunk 891: ConstructActualWantedInventoryPC 0x005A5B31 landed (`2026-11-09_landing_a/`); Sell / Buy PC builders parked at one /
    two loop-shape differences (hand891). Chunk 890 session 5: DrawRetailDisplay 0x00498490 landed (`2026-11-09_landing_b/`);
    CTCBoastUI::Activate 0x0056AD40 landed (`2026-11-09_landing_d/`, no device); chunk 890 is 3 of 3. Read docs/pipeline/MATCHING_PLAYBOOK.md before parking anything (the method + c2 rules that closed Action).
    Playbook family pushes (owner, 2026-11-09: one agent, one family at a time): 425 parked rows within 8 bytes of retail
    are listed by residue family in `rebuild/candidates/manual/pending/PLAYBOOK_CANDIDATES.tsv`. Order: the two 12 KB PC menu rows
    (0x006096E0, 0x0061E6D0; running), then scheduler + test/or ties, shared tails, register permutations, frame, x87, inline, loops.
    Scheduler + test/or family (chunk 892): 24 of 27 landed (`2026-11-09_landing_e/`; PAREN region cap, same-TU no-capture,
    commutative operand ids); 0x00B6FA40 / 0x00F05A10 / 0x005FD5E0 parked (hand892). Shared-tails family (chunk 893, 17 rows)
    stopped by the owner after 15 min: 3 rows matched but not landed (need tests; pending/2026-11-09_linux_hand893/matched/), plus
    the duplicate-return survivor rule (GOTCHAS, playbook). Resume there, then register permutations, frame, x87, inline, loops.
    Quick-win pass (user, 2026-11-09: "10 easiest rows in under an hour"): 7 landed (`2026-11-09_landing_f/`: chunk 893's three
    matches with tests, MixGainRampUp/Down 0x00C6DA40/DAD0 on their twins' lever, decal sphere 0x00BBA3E0 by PAREN tuples, __median
    0x00807170 on the shared comparator bodies); coverage 47,139 genuine. Tried and left: GFStringToInt pair, 0x00A17620,
    0x00C720C0, 0x00D259A0 (one-line residues; tries listed in that README).
    All-means push (owner, 2026-11-09): NUISystem::CManager::Action 0x0041E6D3 (35,651 bytes) landed
    (`2026-11-09_landing_c/`); RunCutsceneMacro 0x00CBFB7D parked after session 9 (owner: no relaunch): hand-written h03 DIFFER(74846v74778), strict 96.7%,
    calls 4040/4068; h02 holds retail frame 0x18DC at 74858v74778; residue = constant-register pieces (hand885 WORKLOG Session 9).
    Chunk 874 (libpk): 3 of 7 landed (`2026-11-08_landing_y/`, ivec resamplers). Chunk 875: 0 of 3 (decodes kept).
    Chunk 872 (libpk): 12 of 14 landed (`2026-11-08_landing_x/`, land0811x). Chunk 873 (game) running (Opus high); pass 1: 2 of 10 landed (`2026-11-08_landing_w/`), sent back for 8.
    Chunk 867 (PC UI): 4 of 10 landed (`2026-11-08_landing_v/`; 31 headers gain opt-ins, 12 new); tag land0811v.
    Chunk 871 (libpk + 2 game rows, Opus high): 18 of 18 landed (`2026-11-08_landing_u/`, new libpk/CpuIdentify.h asm helper); tag land0811u.
    Chunk 870 (libpk, Opus high): 16 of 17 landed (`2026-11-08_landing_t/`; 0x00C1D1C0 parked, x87 CSE order); queue tag land0811t.
    Chunk 868 round 2: 0 new (0x00A2F740 rebuilt on new shared mesh headers, 720 lines from 1333; all eight parks re-graded, `pending/2026-11-08_linux_hand868/`).
    Library direction (owner, 2026-11-08; EXTRA_RULES "Owner direction, 2026-11-08 ..."): third-party vendor code (CRT, zlib 1.2.1,
    LZO 1.08, libogg/libvorbis 1.0.1 minus vorbisfile.c, strmbase, ImeUi) is linked from the vendor's source built unmodified with VC7.1,
    not hand-written; map + routes in docs/pipeline/LIBRARY_MAP.tsv / LIBRARY_LINKING.md (zlib inflate.c proven RELOCATION_MATCH).
    Research found libpk + the CODA audio engine and LHNetworkLib are Lionhead in-house code (no legitimate source exists), so they stay
    reconstruction (owner, 2026-11-08: "treat as game code"); chunks 870 (minus zlib) and 871 relaunched on them (Opus, high).
    Vendor route built (tools/decomp_pipeline/libsig/vendor_libs.tsv, vendor_lib.py, fetch_vendor_libs.py; lib_verify / libsig_scan
    --vendor): z_inflate 0x00C02470 landed as a `// Vendor-library:` toolchain_library stub; 248 landed zlib/LZO/Ogg/Vorbis rows carry a
    `// Vendor-proof:` line (bodies kept); dashboard splits game / Lionhead library / third-party / toolchain. Open: the four /Gr
    comparator TUs (floor1/lsp/psy/sharedbook) need a source patch, 37 unmanifested vendor functions. SDK audit (docs/pipeline/SDK_NEEDS.md):
    retail needs VS2003 Vc7/include + Vc7/lib, DX9.0c SDK Summer 2004 (BaseClasses, strmiids; 588/588 proven), DX SDK April 2005
    (d3d9/d3dx9_25/dinput8 libs, dxguid, c_dfDI*), Feb 2003 PSDK import libs; the lane has only Vc7/lib. Strmbase vendor route done
    (Summer 2004 installer, 8 stubs incl. TimeDiff, 348 Vendor-proof rows; fetch_sdks.py fetches all four era SDKs, env vars in
    env.sh.template). Next: catalog re-grade on Vc7/include; 56 strmbase respellings queued (vend1108b).
    0x00C0..0x00C4 gap-scan rows of `mov ecx,[ecx+4]; mov eax,[ecx]; jmp [eax+N]` forwarders (72-byte extents) are thunks, not chunk material.
    Running (2026-11-08, 150 grades a row): chunk 866 (done) (ten never-attempted Ogg Vorbis / libpk rows, Xiph names), chunk 867 (ten
    never-attempted PC UI / inventory rows in packed TUs), chunk 868 (Fable retry of ten parks: chunk 863's seven + 0x00D18DF0,
    0x00C52BB0, 0x00B92300). Only 466 of the 2,875 unlanded functions are never-attempted (`$S/w22/never_attempted.json`).
  - (superseded) Authoring was STOPPED (the user, 2026-11-08: "No more authoring"). No agent is running; do not cut or launch chunks until the
    user asks again. Last state: chunk 853 landed `2026-11-08_landing_d/` (~CPlayerGui 0x0043A570; row 0x00CE6CF0 recut into
    CGameflowScript::Init 16 B + DeclareGossipCategories 0x00CE6D00 2221 B, both landed; landed 0x00D3B410 respelled
    CQ_GuildTrainingScript::GossipSetup; GetMapNavigationAreaInit 0x0050A650 parked at 6 lines). Chunk 854 was stopped mid-run; its four RELOCATION_MATCH sources landed later (`2026-11-08_landing_e/`); its two WIP rows (0x007602F0, 0x00507070) stay in `pending/2026-11-08_linux_hand854_stopped/`. Earlier today:
    chunk 850 landed 2 of 6 (`2026-11-08_landing_a/`), 851 4 of 6 (`_b/`, 30 headers, 321 includers rechecked), 852 0 of 6 (`_c/`).
    The two open landing follow-ups: queue line land0811a 0x009EF900 (respell landed 0x009EBB30 to the `__false_type` _M_insert_overflow)
    and land0811d 0x0066B370 (respell landed FrameUpdate 0x0066BBF0's UpdateSounds callee to PlayAllSoundEvents). Chunks 834 / 836 landed 2 of 6 each
    (`2026-11-07_landing_k/`, `_l/`). The remaining never-chunked unlanded Mains (about 20, mostly 3-4 KB Codex rows plus the 20-32 KB
    CHobbeTorrent / CCombatApprentice / CGameflowScript) come from `rebuild/manifest/functions.tsv` filtered by `rebuild/src/compiled/` files
    (the manifest's compiled_status column is not reliable for this) and `rebuild/candidates/manual/chunk_history.tsv`. Earlier (history; nothing below runs now) (the user, 2026-11-07: "add a fable agent who is an expert reverse engineer ... work on retries of parked rows and
    give it more tries per row"): chunk 832 landed 2 of 10 (`_h/`), chunk 835 1 of 10 (`_m/`; the CIsThingAlive blocker is structural, owner question
    pending: allow a shared-header inline_depth(0) region round the predicate special members? it still leaves 9-22 lines); now chunk 840
    (`$S/w22/agent840`, Fable model, 25+ attempts per row) retries ten 1-3-line scheduling parks (0x0070C4E0, 0x00C52BB0, 0x0052EA30, 0x007BC8FC,
    0x0058301A, 0x00773950, 0x0073ACC0, 0x0047ED76, 0x009E5C90, 0x007534B0; ranked list `$S/w22/parked_rank.txt`). When it reports, land it and cut the next expert retry set from the parked pool ranked by residue. Rows were picked to avoid the CIsThingAlive copy
    blocker. A retry that matches needs its pending header overlay copied in; a Main outside
    the manifest goes into `rebuild/manifest/boundary-starts.tsv` + `crawl/manifest_add_boundaries.py --write` first. Measure a cut
    by its first `ret` + int3 unless a jump table follows (jtext's maxbranch can sit in the next function: chunk 818). Land
    each like 782-818 with the next free letter (`2026-11-08_landing_e`, then `_f`; queue tags `land0811e`, ...),
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
  - Landed since the resume: chunk 782 4 of 6 (`batches/2026-11-05_landing_i/`), 783 1 of 6 (`_j/`), 784 1 of 6 (`_k/`), 785 2 of 6 (`_l/`), 786 4 of 6 (`_m/`), 787 6 of 6 (`_n/`), 788 6 of 6 (`_o/`), 789 2 of 6 (`_p/`), 790 4 of 6 (`_q/`), 791 4 of 6 (`_r/`), 792 2 of 6 (`_s/`), 793 0 of 6 (`_t/`), 794 3 of 6 (`_u/`), 795 2 of 6 (`_v/`), 797 3 of 6 (`_w/`), 796 3 of 6 (`_x/`), 798 3 of 6 (`_y/`), 799 5 of 6 (`_z/`), 800 2 of 6 (`2026-11-06_landing_a/`), 801 1 of 6 (`_b/`), 802 2 of 6 (`_c/`), 803 4 of 6 (`_d/`), 804 1 of 6 (`_e/`), 805 2 of 6 (`_f/`), 806 1 of 6 (`_g/`), 807 2 of 6 (`_h/`), 808 1 of 6 (`_i/`), 809 3 of 6 (`_j/`), 810 0 of 6 (`_k/`), 811 1 of 6 (`_l/`), 812 2 of 6 (`_m/`), 813 2 of 6 (`_n/`), 814 2 of 6 (`_o/`), 815 1 of 6 (`_p/`), 816 0 of 6 (`_q/`), 817 3 of 6 (`_r/`), 818 1 of 6 (`_s/`), 819 0 of 6 (`_t/`), 820 2 of 6 (`_u/`; 0x00DA91A0 held, see below), 821 2 of 6 (`_v/`), 822 2 of 6 (`_w/`), 823 0 of 6 (`_x/`), 824 1 of 6 (`_y/`), 825 3 of 6 (`_z/`; 0x00E44980 added to the manifest), 827 1 of 6 (`2026-11-07_landing_a/`), 826 0 of 6 (`_b/`), 828 1 of 6 (`_c/`), 820's held 0x00DA91A0 (`_d/`, owner ruling), 829 1 of 6 (`_e/`), 830 1 of 6 (`_f/`), 833 1 of 6 (`_g/`), expert 832 2 of 10 (`_h/`), 831 2 of 6 (`_i/`), 0x00B5AF80 on the owner ruling (`_j/`); from 810 on, the header overlay of parked-only rows is kept in the pending dir because no landed row needs it. The
    parked rows are in `pending/2026-11-05_linux_hand782..799/` and `pending/2026-11-06_linux_hand800..825/` and `pending/2026-11-07_linux_hand826..830/`, `pending/2026-11-07_linux_hand831..849/`, `pending/2026-11-08_linux_hand850..853/` (+ `hand854_stopped/`).
  - A container restart stopped chunk 784 once; the scratch survived, and the agent was resumed from its transcript.
  - Naming: 0x0122DEDC (the shared 0.0f) is the compiler literal `__real@00000000` (chunk 796 evidence, queued `land0511x`).
    Open (chunk 800): 0x00544D20 binds the NUISystem CCountedPointer<CAction> / <CComponent> dtors to the 5-byte jmp stubs
    0x00429B72 / 0x00429C53, other rows to the bodies 0x00429160 / 0x004291DE (one symbol, two addresses; queued CHECK `land0611a`).
    Owner question (chunk 804): the three CQ_ArenaHoldingScriptScript fan Mains (0x00CF18D0 / 0x00CF2B70 / 0x00CF3D20) match
    only under the undocumented VC7.1 flag `/d2inlS20` (the caller-size inline cap), and chunk 809's CNOVI_Guard / CGhostGrannySon
    Mains (0x00DAC760 / 0x00E8EF80) only under `/d2inlT72` (the inline threshold); owner ruled 2026-11-08 that this is allowed (chunk 865).
    Owner ruling (2026-11-07): a same-type cast that keeps behaviour identical and byte-matches is acceptable; 0x00DA91A0
    CMapKeeper::Main landed on it (`2026-11-07_landing_d/`). The six landed CScriptThingView rows may move to the cast-only form later. Chunk 821: `/d2inlS15` (diagnostic) matches 0x00D2ED20 / 0x00D2D930. Naming open: 0x0129BA3C
    EPSILON v DELTA_NOTIONAL_ZERO; 0x004BFFC0 also bound as the placeholder ClearMeshEffect_004bffc0 (queued `land0611v`).
    PRIORITY naming (chunk 829, `land0711e`): 0x00687B30 / 0x00687BD0 are IsEventButtonHeld / IsEventButtonReleased, swapped in the
    landed leaves and 36 landed files; respell all together. 0x0129BA3C is DELTA_NOTIONAL_ZERO (owner: decide names from evidence).
    Owner ruling (2026-11-07): the same-behaviour + byte-match rule also covers identity spellings such as `* 1.0f` (0x00B5AF80 landed,
    `2026-11-07_landing_j/`). Decision line (chunk 831): binding the
    anonymous-namespace global 0x013BAFDC from a second TU.
    Owner ruling (2026-11-07) on chunk 827: no row-local `__forceinline` helper for a compiler-inlining residue (ASM_RULES order:
    intrinsic, shared-header macro `__asm` + FABLE_PORTABLE twin, `__forceinline` helper, all only where retail used asm). 0x0094AB00 stays parked.
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
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
# (2026-11-09: 5221bbec-... is gone; 40effb6c-... holds the lane again: env.sh, vc71rtm + vc71 cl.exe, game SHA-1 39fd416b... checked,
#  $S/w22/prep_landing.sh / finish_landing.sh present; $S/w22/oracle_from_exe.py and $S/tools/jtext.py are not. Check `ls $S/wine/env.sh` first.)
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
