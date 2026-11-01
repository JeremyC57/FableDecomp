# Resume here (one page) — updated 2026-11-01 (naming wave 13 landed; chunk 651 agent in flight)

**Next session: read `docs/journal/2026-10/2026-10-31_HANDOFF.md`** (lane rebuild incl. the downloads, resume steps, open
decisions) and the 2026-10-31 lines at the end of `docs/journal/FINDINGS_LOG.md`. Fresh container:
`docs/pipeline/FRESH_SESSION_HANDOFF.md` (section 2 now lists all four downloads: DDK compiler, RTM compiler, VS 2003 libs,
LZO 1.08). Coordinator commands: `tools/decomp_pipeline/linux_lane/lane_tools/coordinator/README.md`. Older one-page
handoffs: `docs/journal/HANDOFF_ARCHIVE.md`.

## State
- Branch `FullDecomp` (`JeremyC57/FableDecomp`), pushed. 2026-11-01: lane rebuilt in the surviving scratch
  `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad` (game, both compilers, VS 2003 libs, LZO;
  5/5 proof + lib_verify MATCH). Chunk 651 agent (Opus 5.5, increased-attempts brief `$S/w22/agent651/CHUNK_BRIEF.md`) was
  running at this write; land it with `land_chunk.sh 651` + `records.py 651`. The owner gives the `Fable.exe` link in a new
  container; it is never written into the repo.
- Coverage (`rebuild/COVERAGE.md`; `tools/landed_metrics.py` → `write_decomp_dashboard.py` → `update_readme_progress.py`,
  never hand-edit): 52,774 of 57,611 known functions (91.6%), 45,384 of the original 49,568 (91.6%); 174 `retail_asm`,
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
- Naming wave 13 landed: 314 rows, 11 family commits, coordinator recheck 314/314, bindings 156 RETIRE / 52 ADD, 8 renames,
  22 labels; 4 owner decisions scoped (`naming/2026-11-01_wave13/`). ESTABLISHED_SPELLINGS now applied across the tree.

## Done 2026-10-31 (details in FINDINGS_LOG)
- Chunks 643-650 landed (2, 1, 0, 3, 0, 1, 2+bonus, 2 of 10; 650 stopped early at the owner's request); GFAXToVX
  re-authored without `volatile`. Codex parked-50 return: 11 of 50 landed, 11 callers respelled. ChatGPT pass 10 handed
  out (100 easiest parked rows, `pending/2026-10-31_chatgpt_pass10/`, reserved until 2026-11-04).
- Chunk pool re-ranked into fixable vs compiler-tie buckets (`coordinator/pick_parked.py` v2); chunk 651 is cut
  (`chunk_history.tsv`, `$S` dir gone) but was never launched.
- Naming wave 12: 357 rows respelled, 357/357 recheck (`naming/2026-10-31_wave12/`); LZO 0x00C069D0 landed as
  `toolchain_library` from the vendor .asm.

## Next
1. **Land chunk 651** when the agent reports (`$S/w22/agent651/`): `land_chunk.sh 651`, `records.py 651`, NAME_FIXES into the
   queue (prefix c651), batch README + FINDINGS_LOG + GOTCHAS, dashboard trio, commit, push. Then the next chunk cycle (two agents):
   `pick_parked.py` needs `$S/retry1/WORKLIST.tsv` + `$S/retry/sec6b/WORKLIST.tsv` from the old scratch (gone) -- rebuild the ranked
   pool from `pending/*/differ/` + `chunk_history.tsv` first, or cut from `2026-10-31_wave12/DEFERRED.tsv`-adjacent parked rows.
2. **Owner decisions from wave 13** (`naming/2026-11-01_wave13/DECISIONS_W13.tsv`): GetPWorld 0x008FCFC0 (= protected reference getter
   `?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ`; 72 rows, `->` becomes `.`), VIRTUAL_SCREEN_* 0x013961E8/F0 (3NA; GFAXToVX models it
   volatile), vector<CCharString> dtor 0x00414EA0 (header family vs table-only), GetCurrentWorldFrame 0x004167C8 (one source). Decide,
   then naming wave 14 = those four families + the wave 13 DEFERRED items that need per-row modelling (IsPerformingAnyAction, GetWorld
   IAE, Initialise UAE, LowerBound CVectorMap models, IS_THING_ALIVE<const CThingPhysical>, the 0x0097ABF0 comparator family) + the
   remaining queue rows. Reuse `naming/2026-11-01_wave13/scripts/` (compose13.py, fam_glob.py, fam_gptr.py, hand13/).
3. **Migration wave 10** per `headers/2026-10-30_retail_models_wave9/NEXT.md`: the 76 local `enum ENullDef` models, the
   1,317 proven CCountedPointer rows, and the shared `engine/CDefPointer.h` Release() shape (mov/dec/mov vs retail
   `dec [ecx+4]; jne`, header debt found in chunk 649).
4. **ChatGPT pass 10 return** (by 2026-11-04): regrade like the Codex pass (`batches/2026-10-31_codex_parked50/README.md`):
   oracle from `oracle_from_exe.py`, `vland.py` on our lane, check every name against both PDBs before landing, respell
   callers, then release the rest back to the pool (rename RESERVED.tsv → RETURNED.tsv).
5. Leaf lines: make `verify_and_land.py` write `// Leaf: <mangled>` into every landed source so the catalog sync covers
   all rows; then a one-off pass to add it to the 23,446 rows without one (the object symbol is known at recheck time).
6. Older open work: `docs/ROADMAP.md`.
