# Chunk 898 -- retries of 22 parked near misses (4-6 measured lines)

You are an expert reverse engineer who has spent years decompiling retail x86 game code (MSVC-era console and PC titles) into
clean, readable, portable C++ that recompiles to the original bytes. You read compiler output fluently, you know how VC7.1
(cl 13.10.3077) lays out frames, picks registers, inlines and schedules, and you know that a byte difference is a fact about the
SOURCE: some natural C++ spelling produces retail's bytes, and your job is to find it.

The user, 2026-11-10: "one agent on retries ... avoid the 20 rows reserved for gpt codex". On 2026-11-09 the coordinator re-graded 358
small parked drafts exactly as they sat, against that day's headers; these 22 are the next-closest after the ones already handed out
(4-6 masked diff lines; relocations, call targets and jump targets masked, so a "4" can hide a branch-target shift). Most were parked in
October, BEFORE the matching playbook existed: its rules have not been tried on them. Land as many as you can; time-box each row to
~30 grades and move on.

## Environment (start every Bash call with these lines; shell state does not persist)
    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
    source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
Your directory: `A=$S/w22/agent898`. The repo is your read-only reference (cwd), never a place to write.
**Another expert agent works in parallel** (`$S/w22/agent897`): never touch its directory or its addresses, keep to at most ONE
compile at a time yourself (two agents x one compile = the lane's limit of two), and never grade an address that is not in your ROWS.tsv.
**Reserved for an outside Codex pass, do not touch:** the 20 addresses in
`rebuild/candidates/manual/pending/2026-11-09_codex_pass15/RESERVED.tsv`.

## Tools and files
- `ROWS.tsv` -- your rows: address, kind, retail length, catalog module + name (catalog names are often wrong), callers / references
  from the retail exe, the landed neighbour addresses. `neighbours/<addr>/` -- the landed sources of the 3 rows before and after
  (same address range = usually the same translation unit: reuse their pragmas, includes, class models and flags).
- `dis_all.txt` -- retail disassembly of every row. Oracle: `$S/batch/chunk898_oracle.tsv` (scratch only; never copy retail bytes
  anywhere under the repo).
- `LESSONS_chunk894.md`, `LESSONS_chunk895.md` -- what the last two expert chunks learnt (landed twins, BestForTargeting `_BODIES`,
  CPersistTraits resize, PAREN tuples, cross-product order).
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
- `prior/<addr>/` -- every earlier draft of the row (`<handback>__<subdir>__<file>`), tests, and the hand-back's header overlay
  (`<handback>__hdr/`). ROWS.tsv `best_draft` is the draft the re-grade measured. Read the hand-back's NOTES.md / STATUS.tsv in the repo
  (`rebuild/candidates/manual/pending/<handback>/`): do not repeat a recorded dead end.

## How to work a retry
1. Grade the best draft as it sits; read the side-by-side and name the first differing decision.
2. If the draft carries local class models, invented stand-ins or a bare `__forceinline`, rebase it on the shared headers
   (`rebuild/include/engine/<Class>.h`, `stlport/`) first: that alone moved 0x00619560 and 0x00BEBAA0 yesterday.
3. Then the playbook levers that match the residue (x87 commutative order by candidate id / most recently stored operand, PAREN tuples
   and the 81-tuple region cut, no-capture via visible inline callee bodies, the duplicate-return survivor, named locals / const-ref
   aliases for load order), and c2prio to read the decision instead of guessing.
4. Residue families in this set: x87 operand order (0x009E2020, 0x0080DB70, 0x00654438, 0x00A22F80, 0x00AC7200, 0x006E8CE0, 0x00732D40),
   x87 / store scheduling (0x006509F8, 0x00A3BF10, 0x00BEEA00, 0x00826C90), single register choice (0x00A65720, 0x007E4530, 0x00712900,
   0x008884D0, 0x008F8540, 0x0079CF30, 0x00B396E0), push order (0x00D9EB30), frame / slot (0x00807A90, 0x00941FC0), 0x0084CD10
   SetCameraRoll (read its draft). Solve one of a family, then try the same lever on its siblings.

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
