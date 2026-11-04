# Resume here (one page) — updated 2026-11-04 (owner resumed chunk authoring; chunks 738-740 cut, 739 landed in round 2026-11-04a)

**Overarching plan:** `docs/PLAN.md` (every known function by pool, the parked residue classes with their cheapest fixes, the
naming debt, the order of work). This session's details: `docs/journal/2026-11/2026-11-03_SESSION.md` and the 2026-11-03 lines
of `docs/journal/FINDINGS_LOG.md`. Solved problems: the 2026-11-0x lines at the end of `docs/pipeline/GOTCHAS.md`. Agent rules:
`tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`. The 2026-11-01 page is at the end of
`docs/journal/HANDOFF_ARCHIVE.md`.

## State
- Branch `FullDecomp` (`JeremyC57/FableDecomp`). Owner 2026-11-04 resumed authoring: at most 3 concurrent agents, 10+ attempts per
  row, verify and land before launching more. **No asm first**; only proven-impossible instructions (bare `fistp dword`, `rdtsc`,
  `cpuid`, MASM SIMD) go through isolated shared helpers with a `FABLE_PORTABLE` C++ path, and those rows land as `retail_asm`
  (EXTRA_RULES.md, last three bullets).
- Chunks 738 (CPSCRenderSprite), 739 (CPSCUpdateNormal), 740 (CPSCRenderMesh) cut from the particle groups; brief
  `$S/w22/BRIEF_738.md`, work in `$S/w22/chunk73{8,9}`, `chunk740`. 739 landed 2 of 5 (`batches/2026-11-04_landing_a/`), rest staged in
  `pending/2026-11-04_linux_hand739/`. If 738 / 740 are not yet landed, their agents were running when this page was written:
  check each dir's STATUS.tsv / NOTES.md / payload.json, then land (their overlays are based on 40c4d6d63; merge onto 739's
  C3DVector / CDataInputStream / CParticleSystem / CRightHandedSet branches with `git merge-file`). Next cut: CPSCEmitterGeneric
  (group G00b10b10, 4 rows) and a retry of 739's Update 0x00B0DC20.
- Coverage (refresh with `tools/write_decomp_dashboard.py` then `tools/update_readme_progress.py`; never hand-edit):
  46,556 of the 49,564-function catalog (93.931%), 56.08% of retail function bytes, 54,104 landed rows (191 retail_asm),
  after landing 2026-11-04a. Records: `rebuild/candidates/manual/batches/2026-11-03_landing_b/` .. `_landing_h/`, `2026-11-04_landing_a/`.
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
# (2026-11-03: the 5221bbec-... scratch of the previous page is gone; this one holds the same lane. Check `ls $S/wine/env.sh` first.)
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

## Next (in order, once the owner restarts work)
1. Land chunk 733's two rows once the owner's `cvdump` settles the CUIScreenUnitConverter statics' access (private: `@@0`, and the landed
   `@@2` binders get respelled).
2. **Priority name fixes:** 518 open in `NAME_FIXES_QUEUE.tsv` (507 addresses; note starts with PRIORITY, no ` done]`). Cheapest first;
   the day's rounds queued 39 re-model lines (`land0311e/f/g/h`).
3. Attempt the two never-tried LoadFromFile rows (chunk 736 NOTES.md has their callee maps) and retry the parked chunk rows above.
4. The 65 inline-depth rows: the CScriptThing special-member split first (16+ rows share it).
5. Older open work: `docs/PLAN.md`, `docs/ROADMAP.md`, pass 11 leftovers (`pending/2026-11-02_codex_pass11/retry_differ/`).
