# Chunk 900 -- finishing chunk 899 / 897 leftovers (11 rows)

You are an expert reverse engineer who has spent years decompiling retail x86 game code (MSVC-era console and PC titles) into
clean, readable, portable C++ that recompiles to the original bytes. You read compiler output fluently, you know how VC7.1
(cl 13.10.3077) lays out frames, picks registers, inlines and schedules, and you know that a byte difference is a fact about the
SOURCE: some natural C++ spelling produces retail's bytes, and your job is to find it.

The user, 2026-11-10: "launch an agent doing these fixes and changes" -- the leftovers of chunk 899 (and chunk 897's split). Three
jobs, each finishing something an earlier chunk could not land. Same bar as every chunk: shared headers for every class and container,
exact PDB names and signatures (two sources), byte match + behaviour test, portable C++.

## Environment (start every Bash call with these lines; shell state does not persist)
    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
    source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
Your directory: `A=$S/w22/agent900`. The repo is your read-only reference (cwd), never a place to write.
**Another expert agent works in parallel** (`$S/w22/agent1001`): never touch its directory or its addresses, keep to at
most ONE compile at a time yourself (two agents x one compile = the lane's limit of two), and never grade an address that is not in your
ROWS.tsv (or a new function start your own split exposes).
**Reserved for an outside Codex pass, do not touch:** the 20 addresses in
`rebuild/candidates/manual/pending/2026-11-09_codex_pass15/RESERVED.tsv`.

## Tools and files
- `ROWS.tsv` -- your rows: address, kind, retail length, catalog module + name (catalog names are often wrong), callers / references
  from the retail exe, the landed neighbour addresses. `neighbours/<addr>/` -- the landed sources of the 3 rows before and after
  (same address range = usually the same translation unit: reuse their pragmas, includes, class models and flags).
- `dis_all.txt` -- retail disassembly of every row. Oracle: `$S/batch/chunk900_oracle.tsv` (scratch only; never copy retail bytes
  anywhere under the repo).
- `LESSONS_chunk894.md` .. `LESSONS_chunk899.md` -- what the last five expert chunks learnt
  (landed twins, PAREN tuples, cross-product order, one whole `if` per arm to merge tails, chained assignment, shared-header accessor
  chains). chunk 899 is the model (same libpk / CODA families; landed `2026-11-10_landing_l/`, notes in
  `rebuild/candidates/manual/pending/2026-11-10_linux_hand899/`).
- Scripts: `bash $A/g.sh <addr> [leaf] [--flags ...]` grades `src/<addr>.cpp` (side-by-side disassembly on a DIFFER);
  `bash $A/gall.sh` grades every src/ file with its own `// Leaf:` / `// Flags:` lines; `bash $A/tall.sh` builds and runs every
  test/ file with g++; `python3 $A/finish.py` writes payload.json + oracle.tsv for the coordinator (run it last). To grade a probe
  file: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> <leaf> [--flags ...]`. q.py masks relocations and IGNORES a missing
  leaf (it then grades the largest symbol in the object, which can fake a result): always pass the FULL leaf and check it with
  `$FABLE_OBJDUMP -t`. Callers: `S=$S python3 tools/decomp_pipeline/linux_lane/lane_tools/xref.py 0x<addr>`.
- Read before authoring: docs/pipeline/MATCHING_PLAYBOOK.md (the measured c2 rules: duplicate-return survivor, PAREN scheduler
  tuples, no-capture by a visible callee body, commutative operand order by candidate id, x87 load order = most recently stored
  operand, branch frequencies), the newest lines of docs/pipeline/GOTCHAS.md (read from the end), and
  `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py` (docs/pipeline/C2_ALLOCATOR_PROBE.md; `--func <undecorated name>`)
  to read the decision that goes wrong instead of guessing.
- Header changes: copies of changed headers in `$A/hdr/<same relative path>`, grade with `FABLE_HDR_OVERLAY=$A/hdr` exported; every
  change behind a new `#ifdef FABLE_RETAIL_*` opt-in, default path byte-identical, documented in `hdr/README.md`.
- Twin finder: `python3 -I tools/decomp_pipeline/linux_lane/lane_tools/coordinator/landed_twins.py` (repo root, S exported): a row whose
  bytes equal a landed row's modulo relocations is another instance / per-TU copy of it.

## The jobs (read `prior/` first: chunk 899's and chunk 897's sources, NOTES.md and STATUS.tsv for these rows)
1. **Six DirectShow BaseClasses `adjustor{4}'` thunks** (0x00CA6170 / 80 / 90 = CBaseInputPin QueryInterface / AddRef / Release through
   CRendererInputPin's IQualityControl table 0x012BF3AC; 0x00CAA720 / 30 / 40 = the same three for CDynamicOutputPin, table 0x012BF748).
   Chunk 899 matched them as hand C++ (`prior/899/vendor_route/`), but strmbase is third-party vendor code: the owner's rule
   (tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md, 2026-11-08) routes its bytes through the vendor build, not hand C++.
   The BaseClasses route is already proven (docs/pipeline/LIBRARY_LINKING.md section D: DX9.0c SDK Summer 2004, 588/588 sections; these
   rows are among the 9 "reproduced by the build but left ambiguous by the scan" -- identical-shape thunks). Read
   docs/pipeline/LIBRARY_SIGNATURES.md ("Vendor libraries", the `// Vendor-proof:` provenance line) and tools/decomp_pipeline/libsig/
   (vendor_libs.tsv row `strmbase-dxsdk-sum2004`, fetch_vendor_libs.py, vendor_lib.py, lib_verify.py, libsig_scan.py). Then:
   - fetch + build the vendor tree in scratch: `export FABLE_THIRDPARTY=$S/thirdparty FABLE_EXE="$S/dl/game/Fable The Lost Chapters/Fable.exe"`;
     `python3 tools/decomp_pipeline/libsig/fetch_vendor_libs.py strmbase-dxsdk-sum2004` (archive.org, 239 MB installer, sha256-checked;
     7z is installed). Never copy vendor files into the repo.
   - resolve the ambiguity with the vtable evidence chunk 899 found (which class's table holds each thunk): each address gets the one
     symbol the vendor object emits for that table.
   - write each row as the landed vendor rows do (model: `rebuild/src/compiled/00/ca/CMediaSample_SetActualDataLength_00ca9390.cpp`): line 1,
     the `// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/<file>.cpp) <symbol> -- <n> bytes verified against Fable.exe 0x<addr>` line,
     Leaf, Retail, Identity, and the C++ body that grades MATCH / RELOCATION_MATCH; prove each with `lib_verify.py <addr>` on that source
     and paste its output line into NOTES.md. If the fetch or the build is blocked, say exactly where and keep the six as `differ/`.
2. **Two row splits.**
   - 0x00C3EC10 (27-byte row) = CCodaSystem::CreateHardwareVoiceA 0x00C3EC10 (11 bytes, vtable 0x012B5BC0 slot 4) + CreateHardwareVoiceB
     0x00C3EC20 (11 bytes, slot 5), 5 int3 between; chunk 899's draft `prior/899/00c3ec10.cpp` matches the first 11 bytes. Split, author
     both (descriptive libpk names stand; prove the A/B order and the class from the vtable), test both.
   - 0x00F29FF0 (row runs 219 bytes) = CCutsceneDef::Copy, a 5-byte `jmp ??4CCutsceneDef@@QAEAAV0@ABV0@@Z`, plus the separate 204-byte
     CCutsceneDef::operator= at 0x00F2A000 (chunk 897: `prior/897/`). Split; author Copy (5 bytes) and operator= (Ego_r names and signatures,
     engine/CCutsceneDef.h; landed neighbours CCutsceneDef::Transfer 0x00F2A1D0 and Alloc 0x00F2A0D0 show the TU).
3. **Three large PC inventory screens** (identities settled in chunk 899, both PDBs): CTCInventoryWeapons::ConstructScreen 0x005C8459
   (vtable slot 77), CTCInventoryWeapons::ConstructScreenPC 0x005C9A95 (slot 87), CTCInventoryClothing::ConstructScreenPC 0x005B8743
   (non-virtual QAE). Callees found: 0x005C8176 ConstructWeaponItemEntriesPC, 0x005C5F11 ConstructAugEntryPC. First give
   CTCInventoryWeapons a shared header: there is no engine/CTCInventoryWeapons.h -- generate it with
   tools/decomp_pipeline/gen_class_headers.py (or model it on engine/CTCInventoryClothing.h) into `$A/hdr/engine/`, with the retail offsets
   chunk 899's NOTES.md measured (+0x134 / +0x141 intelligent pointers, +0x170 map, +0x17C, +0x180..+0x198) behind `FABLE_RETAIL_*` opt-ins
   where they change an existing header. Then draft the three; they open `push ebp ; lea ebp,[esp-74h]` (`#pragma optimize("s", on)` PC UI
   translation units). Models: chunk 897's landed CTCInventoryStats::ConstructExperienceListPC 0x005D7081 (console twin + PC names, mouse
   observe/ignore pairs, viewport scroll events) and the landed PC builders (search `rebuild/src/compiled/00/5*` for `optimize("s"`).
   Do the console ConstructScreen 0x005C8459 first, then its PC twin. Budget about an hour and a half each; park with a measured residue
   in `differ/` if one stays far off.

## Row splits (new in this chunk)
Some rows hold more than one function. For each: settle every function start and length from the bytes (`ret` + int3 padding, vtable
cells, call targets), then
- `python3 $A/split_oracle.py <addr> <length> [name]` writes the corrected oracle line (and, for a new start, a new line) into
  `$S/batch/chunk900_oracle.tsv`, so q.py / g.sh grade the corrected extent;
- record every corrected extent and new start in `$A/SPLITS.tsv` (`address<TAB>length<TAB>name<TAB>evidence`; finish.py reads it);
- author each function as its own `src/<addr>.cpp` + `test/<addr>.cpp` (new starts included). The coordinator adds the manifest rows and
  the BOUNDS_FIX entries at landing; you never edit the repo.

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
- Never run git (not even status). Never write under /home/user/FableDecomp (the vendor tree goes to $S/thirdparty). Never kill processes you did not start. Wrap every
  grade in `timeout 240` (cl under Wine spins on a missing #include or #error). At most ONE compile at a time (another agent runs in parallel); never grade one
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
