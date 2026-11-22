# One-row hand-off (2026-11-22): 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (3491 B)

The owner wants a second opinion on this row before deciding it. Our agents could not make it byte-exact, and the remaining
decision is a rule question, so the owner wants to hear what you think as well as what you can measure.

## Where it stands (read `history/OWNER_PACKET.md` first, then `history/auth7_ESCALATE.md`)
- **Best text:** `row/best_b3.cpp`, built with `/Gr` and the headers `include/` plus the overlay `hdr_overlay/` (whole header files,
  also given as a diff against `include/` in `hdr_overlay/hdr.diff`). The overlay goes first on the include path
  (`/I hdr_overlay /I include`).
- **Grade:** DIFFER(3491v3491), equal length. After masking relocations, 16 bytes differ, all in one hunk: the "position" block evaluates
  `compressed_size < uncompressed_size` with its operands in the opposite order from retail.
- **Matches the debug build:** under VC10 the text equals Ego_d's line table on all 158 lines (3872 B), and every local sits at Ego_d's
  frame offset (`row/egod_*.txt`; the VC10 model is `row/v10/`). The helper bodies also match Ego_d in size, with the exceptions
  named in the packet (STLport v Dinkumware containers, another revision's CCharString getters, AllocateType).
- **Why it still differs:** we read the residue as VC7.1's c2 tie-breaking on its internal symbol order. Header-only changes (unreferenced
  declarations) do not move it; only text the row expands moves it. `tools/c2_symbol_order_kit/`, `tools/ord.sh` and `tools/pp.py` are
  the readout kit we used.
- **Last test:** `row/c1_allocatetype_local.cpp` gives CSimpleMemoryPool::AllocateType Ego_d's exact shape (parameter `elements`, a block
  local `unsigned __int64 overflow_check`, which FableWin.pdb records in all 12 instantiations). The row stayed at 16. Three landed rows
  that expand the same AllocateType broke (`users/`):
  - 0x00AA0FA0 and 0x00BD2D90 match only without a named local;
  - 0x00A50510 matches only with the one-line ternary body.
  Their landed sources are `users/landed_*.cpp`; `users/*_with_c1_header.cpp` are the same sources under c1's header shape.

## What the owner asks you
1. **Independent attempt.** Find a text that makes the row byte-exact AND keeps 0x00A50510 / 0x00AA0FA0 / 0x00BD2D90 matching with the
   same header, without contradicting Ego_d (a local Ego_d lacks, a contradicted statement order or line size). If you find one, return
   it as a normal row (rules below).
2. **Your opinion on the rule question** (`OPINION.md`, one page). The only freedom Ego_d cannot calibrate is in the STLport CArray /
   vector members and std::copy (retail uses STLport, Ego_d Dinkumware) and the CCharString getters. Any change there would be
   chosen because it makes the bytes match ("fitted"). The options the owner weighs:
   - (a) leave the row unlanded as a documented compiler order tie, b3 as its text;
   - (b) allow one fitted text in those uncalibrated bodies, under a stated rule;
   - (c) something else.
   Say which you would pick and why. If you would pick (b), state the rule you would use and what it must not break.
3. If you think our reading of the residue is wrong (for example, it is not a symbol-order tie), say so with the measurement.

## Rules (full text: `docs/EXTRA_RULES.md`, `docs/ASM_RULES.md`, `docs/NAMING_AND_HEADERS.md`; general procedure `docs/GENERAL_PROCEDURE_pass27.md`)
- Genuine compiler-emitted C++ only: no `__asm` / `naked` / `_emit` / `register`. Compiler: VC7.1 RTM cl 13.10.3077
  (`/nologo /c /O2 /Oy /W3 /Gr`, as the row's `// Flags:` line says).
- Header changes are add-only, behind new `#ifdef FABLE_RETAIL_*` opt-ins, default path unchanged; re-pin any moved
  `FABLE_STATIC_ASSERT_AT`. A header shape must hold for every landed row that uses it: here, the three users.
- Names in the exact PDB spelling, two sources (`row/ego_r_proc.txt`, `row/fablewin_proc.txt`, `row/egod_*`). No local model of a class
  that has a shared header.
- Cut retail bytes from your own Fable.exe with `tools/cut_row.py`; compare with relocations masked; check every relocation's symbol
  against `names/reloc_bindings.tsv`; run `tools/pickany_vs_retail.py` on the object.
- The row needs a behaviour test (`00a4efc0_test.cpp`: C++03, /Od /W3, no /EHsc, include/ only, prints `00a4efc0_TEST PASS`); none
  exists yet.
- No retail bytes, game files or PDB contents in what you return beyond the names and types you cite.

## Return
- `OPINION.md`: your answer to question 2, plus question 3 if it applies.
- `STATUS.tsv`: your best grade and residue, with the three users' grades under any header change you made.
- `PROBES.md`: each attempt, one line each.
- If it matches: `00a4efc0.cpp` (`// Leaf:` and `// Flags:` lines), `00a4efc0_test.cpp`, `hdr/PATCHES.json` (old_text / new_text pairs),
  and `NAME_FIXES.tsv` / `LABELS.tsv` if you prove names.
- Queued name points you may settle: AllocateType's parameter is `elements` in all three PDBs (ours: `count`), and
  C3DVectorWord::operator=='s parameter is `rhs` (`names/NAME_FIXES_relevant.txt`).

The three PDBs are in the shared "debug tlc" folder: https://drive.google.com/drive/folders/1YYGZGTzO4ARouXjs_d6OUridsda1HVKK?usp=drive_link
(Ego_d.pdb, Ego_r.pdb, FableWin.pdb; Ego_r / FableWin need `tools/repack4k.py` before llvm-pdbutil).

Package: `fabledecomp_codex_row_00a4efc0_2026-11-22.zip` (6.4 MB, sha256 fe4648c825148490dbd7a7f1af6dc52a986282d83dfdacfc61992324e6771398), built by hand from AUTH7 / AUTH11 files and HEAD 2c68aace4b include/ (scratch, not in git). No PDB, exe or object files; regraded before sending: DIFFER(3491v3491).
