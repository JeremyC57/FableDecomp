# FableDecomp -- Codex hand-off pass 26 (2026-11-21): 100 parked near misses

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row in
`ROWS.tsv` the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters, Steam,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000), written the way the original Lionhead / Big Blue Box engineers
wrote it: their classes, member names, headers and idioms. The owner's words: "readable clean c++ that is portable and byte matches
retail code and behaves the same as retail."

These 100 rows are reserved for you (`RESERVED.tsv` in the repo's `pending/2026-11-21_codex_pass26/`). No earlier Codex pass has had
any of them. Each one's best draft is 0-7 bytes from retail length (85 B to 4.6 KB; 76,560 bytes in all). What is left is usually one
or two instruction groups. Earlier agents' drafts and notes are in each row folder; do not repeat a recorded dead end.

**Read `docs/NAMING_AND_HEADERS.md` before writing any code.** Names and headers are graded as strictly as bytes: a row with a
placeholder or wrong name, or a row-local model of a class that has a header, does not land. Section 6 there is new: pass 25 lost three
byte matches to pick-any copies that differed from retail.

## The rows (`ROWS.tsv`; per-row detail in `rows/<addr>/ROW.md`)
Each row folder has:
- `ROW.md`: extent, best grade, where it stands, **naming evidence** (Ego_d name, mangled candidates from Ego_r and FableWin, queued
  name fixes), **headers** (the class headers and their opt-in macros), callers, and landed same-TU neighbours.
- `best_draft.cpp`: the draft on record. Regrade it first on the package headers.
- `egod_locals.txt`, `egod_lines.txt`, `egod_tu.txt` (75 rows): Ego_d evidence, described below.
- `neighbours/`: landed rows of the same TU (copy their includes, opt-ins, pragmas and flags first). `prior/`: every earlier file.
- `handback_*`: the notes of the folder the draft came from (search them for the address).

Work in order of `ROWS.tsv` (closest first) unless a TU group is faster together (rows sharing an Ego_d object, `egod_tu.txt`).

## Evidence sources (each name needs two)
- **Ego_d.pdb** (download: @@EGOD_PDB_LINK@@; SHA-256 `d7aa26ef34d4c1857e7cebf035f4a897b77751b9d5ca308535a618062efbeae1`) is the VC10 /Od
  debug PDB of the same source tree. Already extracted per row:
  - `egod_locals.txt`: parameter / local names, types and `{ }` scope blocks. No local means the source wrote one expression. Frame
    offsets are VC10's, not retail's.
  - `egod_lines.txt`: the debug byte count per source line (`+offset line bytes`). It is a size oracle for statement shape: compile your
    text at VC7.1 /Od and compare, using the calibration in `docs/FINDINGS_LOG.md` (2026-11-20/21 entries).
  - `egod_tu.txt`: the row's object file in Ego_d order, with landed retail addresses.
  - Tools: `tools/egod_locals.py`, `tools/egod_type.py`, `tools/egod_proc.py`, `tools/egod_lines.py`, for any other function.
- **Names** in `names/` (full table in `docs/NAMING_AND_HEADERS.md`):
  - Ego_r and FableWin: mangled names (`*_pdb_names.tsv`) and per-object function lists (`*_obj_functions.tsv`).
  - Ego_d: function tables (`egod_*`).
  - Classes: `struct_layouts_egor.tsv` and `fable_types.h` (3,807 classes, real member names), `pdb_class_keys.tsv`.
  - `rtti-vtables.tsv`.
  - Bindings and spellings: `reloc_bindings.tsv` (the symbol each retail address is bound to), `ESTABLISHED_SPELLINGS.tsv`,
    `NAME_FIXES_QUEUE.tsv` (landed spellings already proven wrong).
  - Landed rows: `catalog_names.tsv`, `landed_sources.tsv`, `manifest_names.tsv`, `labels/`.
  - `fse_api_manifest.json`.
- **Headers** in `include/` are the project's tree as of this package, including every landing through the pass 25 landing of
  2026-11-21.

## Rules that bind these rows (`docs/EXTRA_RULES.md` is the current rule set and overrides older text)
1. Genuine compiler-emitted C++ only. No `__asm`, `naked`, `_emit` or `register`; `volatile` only where retail used it. A call to a
   shared retail-asm helper header (e.g. `engine/GFFloatToLongFloor.h`) needs a `// Retail-asm:` line citing the retail address and
   instructions (`docs/ASM_RULES.md`).
2. Shared headers for `this` and every class. Header changes are ADD-ONLY behind new `#ifdef FABLE_RETAIL_*` opt-ins, with the
   default path unchanged. Move a `FABLE_STATIC_ASSERT` only by pinning it (`tools/lane_merge/pin_asserts.py`). Hand header changes
   back as old_text / new_text pairs.
3. Inline devices (owner, 2026-11-08) are only for residues that are purely inline decisions, smallest first:
   - plain C++;
   - a per-row `/d2inlT<N>` / `/d2inlS<N>` flag;
   - a scoped source control (`#pragma inline_depth` around ONE statement, `auto_inline`, `FABLE_FORCEINLINE` on a row-local helper).

   Each needs a `// Inline-budget:` line. Never a bare `__forceinline`.
4. Text Ego_d contradicts (an absent or extra local, a contradicted statement order or line size) cannot land, not even as
   `// Inferred:`. Fitted choices Ego_d does not contradict may land as `// Inferred:` with the measurement.
5. Names exactly as in the PDBs (two sources). A descriptive name only where none exists anywhere, marked `// Descriptive:`.
6. **Pick-any copies:** every function COMDAT your object exports that `names/reloc_bindings.tsv` maps to another address must be
   byte-identical to Fable.exe there. Check with `tools/pickany_vs_retail.py Fable.exe names/reloc_bindings.tsv <row>.obj <addr>`.
   A NEQ the leaf references holds the row; report any NEQ in STATUS.tsv.

## How to work and what to return
- Cut retail bytes from YOUR Fable.exe with `tools/cut_row.py`. Compile with the real VC7.1 RTM cl.exe (`/nologo /c /O2 /Oy /W3`, plus
  any flags a same-TU neighbour uses; `names/landed_sources.tsv` has every landed row's flags). Compare with relocated operands masked,
  then check that every relocation names the right symbol, and run the pick-any check.
- Try at least 5 genuinely different source shapes per row before stopping on it; keep going while the residue shrinks. Log each
  attempt in `rows/<addr>/PROBES.md`.
- Return per row:
  - `<addr>.cpp`: one TU with a full mangled `// Leaf:` line, a `// Flags:` line, Ego_d-consistent text, and labelled `// Inferred:`
    / `// Inline-budget:` / `// Descriptive:` lines.
  - `<addr>_test.cpp`: a self-contained behaviour test (C++03, builds at /Od /W3 without /EHsc, include/ only) with a case for every
    condition in the row. It prints `<addr>_TEST PASS`.
- Return once for the whole package:
  - `hdr/PATCHES.json`;
  - `NAME_FIXES.tsv` (`address<TAB>correct_symbol<TAB>evidence<TAB>wrong_spellings_seen`) for every landed spelling you prove wrong;
  - `STATUS.tsv` (row, grade, pick-any result, one-line residue);
  - `LABELS.tsv` (row, the mangled leaf).

  No retail bytes, game files or PDB contents beyond the names and types you cite.
