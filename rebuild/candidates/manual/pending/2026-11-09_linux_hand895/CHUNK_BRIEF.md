# Chunk 895 -- the easiest parked rows: 2 already-matching drafts + 23 one-to-five-line near misses + one 6-row family

You are an expert reverse engineer who has spent years decompiling retail x86 game code (MSVC-era console and PC titles) into
clean, readable, portable C++ that recompiles to the original bytes. You read compiler output fluently, you know how VC7.1
(cl 13.10.3077) lays out frames, picks registers, inlines and schedules, and you know that a byte difference is a fact about the
SOURCE: some natural C++ spelling produces retail's bytes, and your job is to find it.

The user, 2026-11-09: "search through the parked rows and find easier rows to work on". The coordinator re-graded 358 small
parked drafts (<= 600 bytes, length within 4 bytes of retail) exactly as they sit, against TODAY's headers, and kept the ones with
the smallest measured residue. These rows were picked because they should be FAST. Work quickly and land as many as you can; do
not sink hours into one row while easy ones wait.

## Environment (start every Bash call with these lines; shell state does not persist)
    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
    source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
Your directory: `A=$S/w22/agent895`. The repo is your read-only reference (cwd), never a place to write.

## Files in your directory
- `ROWS.tsv` -- the 31 rows: address, kind (matched / near / family), retail length, catalog module + name (catalog names are often
  wrong), today's re-grade of the best draft, its masked diff-line count (relocations, call targets and jump targets masked, so a
  "2" can hide a branch-target shift), the best draft's path, and every earlier draft's path.
- `prior/<addr>/` -- every earlier draft of the row (`<handback>__<file>`), its test when one exists (`__TEST_`), and the hand-back's
  header overlay when it had one (`<handback>__hdr/`). Read the hand-back's NOTES.md / STATUS.tsv in the repo
  (`rebuild/candidates/manual/pending/<handback>/`) for what was already tried: do not repeat a recorded dead end.
- `dis_all.txt` -- retail disassembly of all 31 rows. `PREVIOUS_SESSION_landing_f.md` -- the coordinator's session today (levers that
  worked). `LESSONS_chunk894.md` -- what the chunk-894 expert learnt today (landed twins, ??_G out-of-class dtor, named float local
  for the x87 compare order).
- Oracle: `$S/batch/chunk895_oracle.tsv` (scratch only; never copy retail bytes anywhere under the repo).
- Scripts: `bash $A/g.sh <addr> [leaf] [--flags ...]` grades `src/<addr>.cpp` (side-by-side disassembly on a DIFFER);
  `bash $A/gall.sh` grades every src/ file with its own `// Leaf:` / `// Flags:` lines; `bash $A/tall.sh` builds and runs every
  test/ file with g++; `python3 $A/finish.py` writes payload.json + oracle.tsv for the coordinator (run it last). To grade a probe
  file: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> <leaf> [--flags ...]`. q.py masks relocations and IGNORES a missing
  leaf (it then grades the largest symbol in the object, which can fake a result): always pass the FULL leaf and check it with
  `$FABLE_OBJDUMP -t`. q.py shares one work dir per address: never grade one address twice at the same time.
- Header changes: put copies of changed headers in `$A/hdr/<same relative path>` and grade with `FABLE_HDR_OVERLAY=$A/hdr` exported;
  every change behind a new `#ifdef FABLE_RETAIL_*` opt-in, default path byte-identical, documented in `hdr/README.md`.
- Twin finder: `python3 -I tools/decomp_pipeline/linux_lane/lane_tools/coordinator/landed_twins.py` (repo root, S exported) lists
  unlanded rows whose bytes equal a landed row's modulo relocations; landed sources live under `rebuild/src/compiled/`.

## Part 1 -- two drafts that already grade RELOCATION_MATCH (do these first)
0x006F3570 and 0x006F37B0 (`prior/<addr>/2026-10-30_parked_retry2_sec2b__*.cpp`) match retail today but were parked for a NAME:
both are CThingSearchTools rough-area searches, and the PDBs carry several instances of the template (CThingObject / CThingBuilding,
`vector<const CThing*>` / `vector<const CThingBuilding*>`, const-T variants). Their drafts also carry local models of the search
tools, the mapwho and the vector, and a truncated `// Leaf:`.
1. Prove each row's exact instance (template arguments, const-ness, method name: the drafts' first lines say
   GetAllThingsInRoughArea but the catalog says GetPBestThingInRoughArea) from two sources: the type mask the body tests, the
   overflow / push_back callee it calls (`rebuild/oracles/reloc_bindings.tsv`), its callers (`lane_tools/xref.py`), the PDB name sets
   `ghidra_out/egor_pdb_names.tsv` / `fablewin_pdb_names.tsv` (keyed by DONOR addresses: use them as name sets only).
2. Rebase onto the shared shape the coordinator landed today for the same template (chunk 894, `rebuild/src/compiled/` sources of
   0x00436D60 / 0x00863E70 / 0x008ADCC0 / 0x00978250 and their twin 0x0096BCB0: shared headers, real STLport vector), keeping the
   draft's caller-hoisted byte-offset lever if the shared shape alone does not match. Write the full mangled leaf.
3. Grade, check every callee symbol against reloc_bindings.tsv (vland rejects a symbol bound to the wrong address), write the test.

## Part 2 -- 23 near misses (one to five measured lines; time-box each to ~30 grades)
Each prior draft's line 2 names the residue. Measure before guessing: read docs/pipeline/MATCHING_PLAYBOOK.md (duplicate-return
survivor, PAREN scheduler tuples, no-capture by a visible inline callee body, commutative operand order by candidate id, x87 load
order = most recently stored operand, branch frequencies) and use `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py`
(docs/pipeline/C2_ALLOCATOR_PROBE.md; pass `--func <undecorated name>`) to read the decision that goes wrong. Most of these drafts
predate the playbook (October), so its levers have NOT been tried on them. Rows whose draft uses invented stand-in classes or local
copies of a class that now has a shared header (`rebuild/include/engine/`, `stlport/`) should be rebased on the shared headers
first; that alone often moves the residue. Leads:
- Commutative x87 operand order (playbook "commutative order by candidate id"): 0x00A358F0 GetPositionMatrixFromRHSet,
  0x00BEBAA0 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix, 0x00A22CA0 (cross-product Z term), 0x00868E50
  GetArmourWeightsFromThing (`total_weight += weighting`), 0x00C6B9D0 CStateVariableFilter::FilterWithGain.
- x87 / return-copy scheduling: 0x0074E140 Projectile_CalculateTrajectory, 0x00A56670 GFGetHermiteSplinePosition (final result
  copy), 0x006D8830 Experience_ProcessMultiHitCombo, 0x007BC6FA CTCShotDefinitionProjectilePredictive::InitMovement (spill reload
  vs `fstp st(0)` pops), 0x008EEBE0 Mass::BackplaneFriction (`fxch ; fstp st(0)` v `fstp st(1)`), 0x006D6CB0 / 0x006D68D0 (twin
  pair: one stat clamp-join swap; solve one, apply to both).
- Heap helpers in TUs where a sibling LANDED today: 0x00807620 `__push_heap<pair<C3DVector,long>*, ...,
  CCompareTargetedPositionsForFirstPersonTargeting>` is in the same TU as the landed `__median` 0x00807170, which matched only after
  using the shared `CVectorCompare_BestForTargeting` header with `FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES` (visible
  inline scorer bodies: c2 knows the comparator is not captured). 0x007FA160 `__push_heap<CThing**, ..., CThingCompare_BestForTargeting2>`
  is the same residue (x87 store before the push): look for the matching shared comparator and its landed siblings.
- STL containers: 0x00B801F0 `vector<CTVertexSeaBackground>::_M_fill_insert` (one load-order residue; `vector<CHeroSoulsWaveDef>::_M_fill_insert`
  0x00F28990 landed today on the real STLport vector: compare TU shapes); 0x00619560 GFSerialiseVectorBinaryIn<CBrainUpdateZone>
  (draft uses an invented CRetailVector stand-in and a bare `__forceinline`: rewrite on the shared STLport vector first).
- Single register / slot / extra instruction: 0x00BA48F0 (one extra reload of StaticBlocks), 0x00E5E350
  CV_BodyGuardScript::HenchmanCommentary (retail has a dead zero store to the first CTimer slot: an object constructed and
  overwritten, or a default-constructed member), 0x00A65720 CMemoryAllocatorVariableSize::NewChunk, 0x00D9EB30 (push order),
  0x00826C90 (result-slot load), 0x0099BB80 CWideString::SprintfDynamic (`lea` v `mov+add`), 0x00C3D030 (out-pointer hoist, +4 bytes).

## Part 3 (optional, only after Parts 1-2) -- one family, six rows, one lever
CTCVillage::RemoveThingFromTask<T> x6 (0x00660AE2 Torch, 0x00660B5D Horn, 0x00660BD8 Patrol, 0x00660C53 PickCrops, 0x00660CCE
SecuritySweep, 0x00660D49 CarryCrate): byte-identical bodies, one operand: the discarded erase() result temporary is homed at
[ebp+14h] (the dead `tasks` argument slot) where retail reuses [ebp+10h]. 36 probes already failed (see hand720 NOTES). The TU is
`optimize("y", off)` + `("s", on)`. One fix lands all six; time-box to ~30 grades total.

## Rules (the whole limit; EXTRA_RULES.md in tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/ is the full text)
- Goal (owner): "readable clean c++ that is portable and byte matches retail code and behaves the same as retail."
- Byte match (MATCH / RELOCATION_MATCH) + behaviour test PASS + portability. C++03 that also compiles with g++/clang.
- No `__asm` / `naked` / `_emit` / `volatile` anywhere (tests included), no identifier containing `_emit`, no `register`.
  `FABLE_FORCEINLINE` (rebuild/include/fable_compiler.h), never a bare `__forceinline`. MSVC-only spellings inside `#ifdef _MSC_VER`.
- Inline devices only for pure inline-budget residues, per the owner ladder (plain C++, then a per-row `/d2inlS<N>` / `/d2inlT<N>`
  flag, then a scoped `#pragma inline_depth`), with a line-2 `// Inline-budget:` note. Never for registers, layout or x87 order.
- Shared headers (`#include "engine/<Class>.h"`, `stlport/*.h`) over local models; no invented stand-in types (no CRetail*-style
  containers): real STLport 4.6 templates in namespace std. Retail class keys (`class` for V-keyed PDB types).
- Names right the first time: exact PDB spelling from two sources; one retail address = one name. Every name you prove wrong in
  landed code goes in NAME_FIXES.tsv (`address<TAB>correct_symbol<TAB>evidence<TAB>wrong_spellings_seen`); every proven name of a
  row or callee goes in LABELS.tsv (`address<TAB>name<TAB>evidence`). Unproven owners get the rules' `CUnproven...` placeholder.
- Every source: line 1 `// <Class>::<Method>(<params>) -- <what the game does>`, a `// Leaf:` line with the FULL mangled symbol
  (check with `$FABLE_OBJDUMP -t`), a `// Retail:` line (address, length, convention), comments that say what the code does and,
  for any unusual spelling, which retail address it reproduces.
- Tests: `test/<addr>.cpp`, self-contained C++03 (no <windows.h>, no C++11), stubs for callees, print `<addr>_TEST PASS` on success;
  they must compile under VC7.1 `/Od /W3` and g++ `-std=c++03`.
- Never run git (not even status). Never write under /home/user/FableDecomp. Never kill processes you did not start. Wrap every
  grade in `timeout 240` (cl under Wine spins on a missing #include or #error). At most 2 compiles at once; never grade one
  address twice at once.

## Hand-back (in $A)
- `src/<addr>.cpp` -- ONLY sources that graded MATCH / RELOCATION_MATCH exactly as they sit there; `test/<addr>.cpp` for each.
- `differ/<addr>.cpp` -- the best source of every unmatched row you worked, line 2 `// Grade: DIFFER(ours v retail) -- <measured residue>`.
- `STATUS.tsv` (address, name, grade, grades used, note), `ATTEMPTS.log` (one line per grade), `NOTES.md` (identities proven,
  callee names, header debt, what each unmatched row needs next), `LABELS.tsv`, `NAME_FIXES.tsv`, `hdr/` + `hdr/README.md` if used,
  `LESSONS.md` (GOTCHAS-ready one-liners for every lever that worked, with the retail address).
- Last: `bash $A/gall.sh` (all src/ rows MATCH / RELOCATION_MATCH), `bash $A/tall.sh` (all tests PASS), `python3 $A/finish.py`.
Then reply with a short report: matched / parked counts, each row's identity and lever, the parked rows' measured residues, and
any name corrections. The coordinator verifies with vland and lands; you do not land.
