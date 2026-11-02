# Resume here (one page) — updated 2026-11-01 (naming wave 13 + chunk 651 landed)

**Next session: read `docs/journal/2026-10/2026-10-31_HANDOFF.md`** (lane rebuild incl. the downloads, resume steps, open
decisions) and the 2026-10-31 lines at the end of `docs/journal/FINDINGS_LOG.md`. Fresh container:
`docs/pipeline/FRESH_SESSION_HANDOFF.md` (section 2 now lists all four downloads: DDK compiler, RTM compiler, VS 2003 libs,
LZO 1.08). Coordinator commands: `tools/decomp_pipeline/linux_lane/lane_tools/coordinator/README.md`. Older one-page
handoffs: `docs/journal/HANDOFF_ARCHIVE.md`.

## State
- Branch `FullDecomp` (`JeremyC57/FableDecomp`), pushed. 2026-11-01: lane rebuilt in the surviving scratch
  `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad` (game, both compilers, VS 2003 libs, LZO;
  5/5 proof + lib_verify MATCH). **Nothing is running** (chunk 651 landed 3/10 under the increased-attempts brief). The owner gives the `Fable.exe` link in a new
  container; it is never written into the repo.
- Coverage (`rebuild/COVERAGE.md`; `tools/landed_metrics.py` → `write_decomp_dashboard.py` → `update_readme_progress.py`,
  never hand-edit): 53,304 of 57,609 known functions (92.5%), 45,858 of the original 49,564 (92.5%); 174 `retail_asm`,
  441 `toolchain_library`, 13 asm_bake oracles.
- Retail compiler cl 13.10.3077 RTM (lane default `$S/wine/vc71rtm`); `--qfe` 13.10.4035 (`$S/wine/vc71`). Lane-only
  downloads (never committed): VS 2003 `Vc7/lib` (`libsig/fetch_vs2003_libs.sh` → `$S/wine/vs2003lib`,
  `FABLE_VS2003_LIB`), LZO 1.08 tree (`libsig/fetch_lzo108.sh` → `$S/thirdparty`, `FABLE_THIRDPARTY`; `ml.exe` from
  the RTM bin assembles `lzo1x_f1.asm` = 0x00C069D0).
- Catalog + manifest names: `tools/sync_catalog_names.py` (2026-10-31) makes `auto-re-candidates.tsv` and
  `rebuild/manifest/functions.tsv` follow the `// Leaf:` symbol of each landed source (3,727 catalog renames, 6,219
  manifest cells, 32 missing manifest rows). 23,447 landed sources have no Leaf line and 12,335 a bare leaf: their
  catalog names are the landing-time labels. Re-run the tool after every naming wave.

## Owner rules in force (2026-10-28..31)
- Names right the first time: exact PDB spelling, two sources, one retail address = one mangled name. At landing, when a
  new row carries the PDB spelling of a callee that landed callers bind under a stand-in, respell those callers and
  recheck before committing (`vland.py ... --recheck-landed`). NAME_FIXES → `naming/NAME_FIXES_QUEUE.tsv`.
- A class with a retail vtable is modelled from `engine/<Class>.h` (`#define Class ClassLayout` include pattern; a local
  model only where the header flattens the vptr, said in a comment). No `__asm`/`_emit`/`naked`/`volatile`/new
  `#pragma pack`; `retail_asm` only for instructions VC7.1 cannot emit; `toolchain_library` via `libsig/lib_verify.py`.
- Wide text `unsigned short`; STLport containers spelled as STLport instantiations; "lane gap"/"tie" labels are opinions to
  retest. Dead-load lever probes (`pending/2026-10-27_linux_hand649/lever_probe_not_reconstruction/`) never land.
- At most two Opus high-effort agents; agents never run git, never write under the repo, never kill processes.

## Done 2026-11-01 (details in FINDINGS_LOG)
- Naming wave 20 landed (105/105 rechecked byte-identical: the c686/c687/c688 fixes, STLport sort / spline / erase rows, FastCompare -> operator< in 36 callers; 46 renames, 56 bindings retired / 41 added, 31 labels; `naming/2026-11-01_wave20/`, DECISIONS_W20_RESOLVED: CSoundSystem, CPKVoiceParams, CDebugStream, the PDB set copy-ctor form); 456 deferred lines feed wave 21.
- Naming wave 19 landed (136/136 rechecked byte-identical: PeekThingSearchTools protected const, the c684/c685/c680 fixes, CPersistContext Transfer<T>, CBasicString::Compare; 9 renames, 50 bindings retired / 37 added, 7 labels; `naming/2026-11-01_wave19/`, DECISIONS_W19_RESOLVED); 440 deferred lines feed wave 20.
- Naming wave 18 landed (1,486/1,486 rechecked byte-identical: the class-static globals through their PDB accessors (PMainGameComponent, PClass, pDSTGame, PPlayerGui, thing manager, ConstantFPS), DELTA_NOTIONAL_ZERO in 146 TUs, bitset::test 229, 13 renames, 153 bindings retired / 12 added; `naming/2026-11-01_wave18/`, DECISIONS_W18_RESOLVED); 437 deferred lines feed wave 19.
- Naming wave 17 landed (414/414 rechecked, 4 MATCH + 410 RELOCATION_MATCH; 35 file renames; 110 bindings retired, 32 added; 46 labels; `naming/2026-11-01_wave17/`, DECISIONS_W17_RESOLVED); 397 deferred lines + the c680-c683 queue rows feed wave 18.
- Naming wave 16 landed (148/148 rechecked, 9 MATCH + 139 RELOCATION_MATCH; 38 file renames incl. 17 CVtable_* modules into NScript; 36 bindings retired, 61 added; `naming/2026-11-01_wave16/`, DECISIONS_W16_RESOLVED); 230 deferred lines + the c675-c678 queue rows feed wave 17.
- Naming wave 15 landed (566/566 rechecked, 13 MATCH + 553 RELOCATION_MATCH; 217 bindings retired, 45 added; `naming/2026-11-01_wave15/`, DECISIONS_W15_RESOLVED: friend access and `mutable NoEventScansRunning` accepted, libpk CHandleBasedMemoryPool::Free deferred until named); 208 deferred lines + the c667-c674 queue rows feed wave 16.
- Chunks 663-673 (NA008-NA018: the authorable band, 100-250 bytes, no twins) landed 10/10 each, chunks 674-677 (NA019-NA022, the 257-512 band) 9/10 each (parked: PlayCutsceneSound on a prologue tie, its manifest row split at GetScriptActiveStatus 0x00CBF9CF via boundary-starts.tsv; PreloadReflectionTexture with its two lift-era DIFFER siblings as a trio; SetupStaticBlocksNoMorph on a register-allocation gap; IsIllegal on an inline-budget residue); 345 NAME_FIXES
  (c663-c677), 364 labels (chunk 677: all ten descriptive labels wrong; DirectShow ctlutil.cpp ctors need the per-row /EHsc flag) (chunk 676: 8 of 10 descriptive labels wrong; 0x00D25410's landed ctor file is CQ_DragonBossFightScript's, not NewOakValeIntro's) (chunk 675: 7 of 10 descriptive `_global` labels were wrong classes) (chunk 673: ~CEngine; gap-scan rows proven as CPosPassThru::QueryInterface and a `_$E1` static initializer, not library bytes) (chunk 672: 7 of 10 script-range dtor labels wrong; entity scripts named via the parent quest's Main) (chunk 671: three "OnDie" rows were three classes' implicit dtors; GetChecksum, ~CEngineLocalDetailGenerator mislabelled) (chunk 670: ten dtors, 6 of 10 catalog labels wrong, resolved by stored vtable + RTTI COL chain) (chunk 669: nine dtors, 4 of 9 catalog labels wrong; 0x00510D40 is _List_base<CIntelligentPointer<CThingAICreature>>::clear) (chunk 668: ~CEngineMap, ~CASoundManager, ~CRenderManager2D, CBaseVideoRenderer::QueryInterface is a DECLARE_IUNKNOWN forwarder; retail's FadeSnowToTarget copy-paste kept) (two int3-after-noreturn stubs landed on the N+1 span). Owner rule 2026-11-01: wrong catalog names are fixed at landing (`coordinator/apply_labels.py` + sync);
  landed-source respells go to naming wave 15 (brief written) right after the chunk round. zlib is 1.2.1 (C, ordinary reconstruction). The pool's authorable band lands like the twins: identity first. 0x00CA4xxx-0x00CA7xxx is DirectShow strmbase.
- Chunk 659 (the parked pairs) landed 0x006276F0 GetItemsListUI with the real same-file ConstructItemCategoryList body; that
  sibling (destructor-inlining gap, chunk 583) and the Minion pair 0x00935E40 / 0x009363D0 (one source) stay parked with 12-33 probes.
- Chunks 656-658, 660-662 (NA002-NA007: landed-twin rows) landed 10/10 each, almost every row on the first probe; 105 NAME_FIXES
  (c656-c662), 105 labels; the twin pipeline holds (655-662 = 70/70; 0x009F88C0 respelled at landing). Rows above 0x01200000 were dynamic initializers, not library code;
  masked-identical "_Dest_val" twins are ??1 / ??_G / _Destroy triples of different element types (chunk 661 GOTCHAS). Every twin label so far was wrong: identity from callers first.
- Naming wave 14 landed: 160 rows, 6 family commits, recheck 160/160, bindings 73 RETIRE / 4 ADD, 1 rename, 7 labels; its three
  decisions resolved by the coordinator under the owner delegation (`naming/2026-11-01_wave14/DECISIONS_W14_RESOLVED.tsv`).
- Owner delegated the five open decisions (2026-11-01, "do what retail did"): recorded in EXTRA_RULES "Owner decisions, 2026-11-01";
  0x009C4F20 ProcessAlways landed with retail's `volatile` PCompletedRead (`// Retail-volatile:` evidence line; land_chunk.sh
  accepts the qualifier only with that line); `__forceinline` header helpers are retail (debt closed); libpk is plain C++;
  data symbols stay gated; the parked pairs (0x00626E40+0x006276F0, 0x00935E40+0x009363D0) go out as one chunk.
- Naming wave 13 landed: 314 rows, 11 family commits, coordinator recheck 314/314, bindings 156 RETIRE / 52 ADD, 8 renames,
  22 labels; 4 owner decisions scoped (`naming/2026-11-01_wave13/`). ESTABLISHED_SPELLINGS now applied across the tree.
- Chunk 655 = NA001 of the never-attempted pool (ten landed-twin rows) landed 10/10 in 1-2 probes each: settle the row's own
  identity from the caller chain, then author from the twin's source. NA002-NA007 (59 more twins) go the same way.
- Chunk 654 landed 3/10 (IsTimeToGoToSchool, CThingCreatureDef::Validate, vector<CIVCountedPointer<..>>::_M_fill_insert); 7 parked
  with 12-30 probes; 13 NAME_FIXES (c654); `__forceinline` debt noted in its README.
- Chunks 652 (4/10) and 653 (3/10) landed under the increased-attempts + retail-fidelity brief; owner decisions pending in their
  READMEs: `volatile` on CFileInstaller::PCompletedRead (0x009C4F20; landed 0x009C42F0 already carries it), the 0x00626E40 /
  0x006276F0 pair (reconstruct the 1.5 KB sibling first). `lane_tools/const_check.py` checks __real@ constants the reloc mask hides.
- Chunk 651 landed 3/10 (TransformOpinionFromHeroStats MATCH, IsAnimationStateBaseValid, GetInputProcessEvent; 7 parked with 12-20
  probes each, residues now single-register/allocation ties); 14 NAME_FIXES (c651); review debts in `batches/2026-10-27_linux_hand651/LESSONS.md`.

## Done 2026-10-31 (details in FINDINGS_LOG)
- Chunks 643-650 landed (2, 1, 0, 3, 0, 1, 2+bonus, 2 of 10; 650 stopped early at the owner's request); GFAXToVX
  re-authored without `volatile`. Codex parked-50 return: 11 of 50 landed, 11 callers respelled. ChatGPT pass 10 handed
  out (100 easiest parked rows, `pending/2026-10-31_chatgpt_pass10/`, reserved until 2026-11-04).
- Chunk pool re-ranked into fixable vs compiler-tie buckets (`coordinator/pick_parked.py` v2); chunk 651 is cut
  (`chunk_history.tsv`, `$S` dir gone) but was never launched.
- Naming wave 12: 357 rows respelled, 357/357 recheck (`naming/2026-10-31_wave12/`); LZO 0x00C069D0 landed as
  `toolchain_library` from the vendor .asm.

- ChatGPT pass 10 landed: 57/57 claimed matches regraded and landed, names audited (55 catalog labels fixed, 3 established spellings corrected), 43 unmatched rows retried as chunk 680 (`rebuild/candidates/manual/batches/2026-10-31_chatgpt_pass10/README.md`).

## Next
1. **Never-attempted pool is cut** (2026-11-01, `pending/never_attempted/CHUNKS.tsv`, 170 chunks NA001-NA170, easiest first;
   coordinator README "Cut a chunk from the never-attempted pool"): NA001 landed as chunk 655; NA002-NA006 = chunks 656-658/660/661 landed 10/10; the pairs chunk 659 landed 1/4; NA007-NA018 = 662-673 landed 10/10, NA019-NA022 = 674-677 9/10; NA023/NA024 = chunks 678/681 landed 9/10 each; NA025/NA026 = chunks 682/683 landed 7/10 and 8/10; chunk 680 (ChatGPT retry) landed 20/43 (23 parked on 1-14-byte residues; 0x00C6B260 is a retail_asm candidate); NA027/NA028/NA029 = chunks 684 / 685 landed 8/10 each (GFInterpolateVectors as retail_asm; strmbase / libvorbis / ImeUi vendor rows); chunk 686 landed 6/10; naming wave 19 landed (136/136); chunks 687 / 688 landed 7/10 and 5/10 (slots not refilled); naming wave 20 landed (105/105); chunk 689 (NA032) landed 8/10; naming wave 21 landed (165/165); chunk 690 (NA033) landed 6/10 (Int64x32Div32 as retail_asm; CBankFile::Update extent corrected to 1850); naming wave 22 landed (183/183; HasTC held out); chunk 691 (NA034) landed 7/10; naming wave 23 landed (38/38, rechecked 45/45 with chunk 691); naming wave 24 landed (60/60); chunk 692 landed 7/11; naming wave 25 landed (75/75); chunk 693 landed 7/10 + both repairs; chunk 694 landed 10/10 (pragma-free pilot failed; the 134 inline_depth rows stay, owner question); naming wave 26 landed 62/62; chunk 695 landed 10/10 (0x009E5C60 respelled UpdateDirtyQuick at landing); naming wave 27 landed 43/43; chunk 696 landed 10/10 (~CScriptThing 0x004AA840); chunk 697 landed 10/10 (six destructor labels fixed); naming wave 28 landed 212/212; chunk 698 landed 10/10 (nine destructor labels fixed); chunk 699 landed 10/10; naming wave 29 landed (396 rows + 2 headers, 697 rechecked); chunk 700 landed 10/10 (all ten labels fixed); chunk 701 landed 10/10; naming wave 30 landed (46 + 1 rows); chunk 702 landed 10/10; chunk 703 landed 10/10; naming wave 31 landed (94 rows + 3 headers, 1,628 rechecked; 4 pre-existing failures to chunk 705 as repairs); chunk 704 landed 10/10; chunk 705 landed 10/10 + all four repairs; naming wave 32 landed (103 rows + engine/CAIStateGroupBase.h, 178 rechecked); migration-wave sweep done (5,152 rows, no real failure); chunk 707 landed 10/10 (four opt-in headers, 179 includers rechecked); chunk 708 landed 10/10 (six opt-in headers, 112 includers rechecked); chunk 706 landed 10/10 (round 706-708: 30/30); chunks 710 / 711 landed 10/10; chunk 709 landed 9/9 + one fragment retired; chunk 712 landed 10/10 (round 706-712: 69/69); chunks 713 / 714 / 715 (NA056-NA058) running; NA067-NA076 reserved for Codex pass 11 until 2026-11-09 (`pending/2026-11-02_codex_pass11/`) (owner 2026-11-02: 10+ attempts per row, expert brief, header corrections via hdr/ overlay); 0x00BC9C30 / 0x009CDF50 repaired in chunk 693 (owner 2026-11-01: TWO concurrent agents; owner 2026-11-02: once naming wave 32 lands, THREE concurrent chunk agents on matching and no standing naming wave -- naming / migration passes wait until matching is done, header fixes only when a chunk needs one, and a landing must not give an address a second spelling unless the coordinator respells the old callers in the same commit); naming wave 18 landed (1,486/1,486); naming wave 17 landed (414/414); ChatGPT pass 10 return: 57/57 regraded byte-exact on our lane (audited and landed), its 43 unmatched rows retried as chunk 680 (20 more landed)
   (owner allowed three agents). Then the pairs chunk (EXTRA_RULES 2026-11-01), then NA005... The parked fixable bucket (97 rows after chunks 652-654) is the other source; keep the chunk 651
   increased-attempts + retail-fidelity brief:
   `pick_parked.py` needs `$S/retry1/WORKLIST.tsv` + `$S/retry/sec6b/WORKLIST.tsv` from the old scratch (gone) -- rebuild the ranked
   pool from `pending/*/differ/` + `chunk_history.tsv` first, or cut from `2026-10-31_wave12/DEFERRED.tsv`-adjacent parked rows.
2. **Naming pass after matching** (owner 2026-11-02: no standing naming waves while matching runs): resume from `naming/2026-11-02_wave32/` (QUEUE_W32.tsv, DEFERRED.tsv, DECISIONS_W32_RESOLVED.tsv: D3 strmbase overload folding, D4 the CBaseIntelligentPointer family, D5 libpk static type, D6 strmbase ??_G rows, D12 two retail copies) plus NAME_FIXES_QUEUE.tsv rows c704 / c705 onward. Kept and marked for revisit (owner 2026-11-02): the 129 `// Revisit: inline-depth` and 17 `// Revisit: two-view-CScriptThing` rows (docs/pipeline/INLINE_DEPTH_REVISIT.md); held:, the 121 CActiveEntityScript<T> ??_G rows, 0x00562E70 Close (one source), ICF / two-retail-copy symbols, engine/CBankFile.h + every CBankFile-family binder. Recheck payloads: `lane_tools/recheck.py` (leaf fallback + funclet offsets). Owner housekeeping: /pw and /ov_final at the filesystem root are stray wave 25 scratch (left for the owner to remove).
3. **Migration wave 10** per `headers/2026-10-30_retail_models_wave9/NEXT.md` (+ engine/CParentDefClassBase.h and engine/CLevelScriptBase.h implicit destructors, chunks 661/664 decisions): the 76 local `enum ENullDef` models, the
   1,317 proven CCountedPointer rows, and the shared `engine/CDefPointer.h` Release() shape (mov/dec/mov vs retail
   `dec [ecx+4]; jne`, header debt found in chunk 649).
4. **ChatGPT pass 10 return** (by 2026-11-04): regrade like the Codex pass (`batches/2026-10-31_codex_parked50/README.md`):
   oracle from `oracle_from_exe.py`, `vland.py` on our lane, check every name against both PDBs before landing, respell
   callers, then release the rest back to the pool (rename RESERVED.tsv → RETURNED.tsv).
5. Leaf lines: make `verify_and_land.py` write `// Leaf: <mangled>` into every landed source so the catalog sync covers
   all rows; then a one-off pass to add it to the 23,446 rows without one (the object symbol is known at recheck time).
6. Older open work: `docs/ROADMAP.md`.
