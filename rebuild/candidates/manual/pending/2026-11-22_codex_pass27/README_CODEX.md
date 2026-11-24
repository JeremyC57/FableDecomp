# FableDecomp -- Codex hand-off pass 27 (2026-11-22): 100 more parked near misses

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row in
`ROWS.tsv` the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters, Steam,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000), written the way the original Lionhead / Big Blue Box engineers
wrote it: their classes, member names, headers and idioms. The owner's words: "readable clean c++ that is portable and byte matches
retail code and behaves the same as retail."

These 100 rows are reserved for you (`RESERVED.tsv` in the repo's `pending/2026-11-22_codex_pass27/`). No earlier Codex pass has had
any of them. They are the next-closest parked rows after pass 26's: each one's best draft is 0-13 bytes from retail length (most 7-10;
52 B to 2.5 KB; 88,321 bytes in all). What is left is usually a few instruction groups. Earlier agents' drafts and notes are in each
row folder; do not repeat a recorded dead end.

**Read `docs/NAMING_AND_HEADERS.md` before writing any code.** Names and headers are graded as strictly as bytes: a row with a
placeholder or wrong name, or a row-local model of a class that has a header, does not land. Section 7 there is new: what the pass 26
landing had to fix (retail-asm evidence lines, leaf-derived names, patch anchors and asserts, TU-mate copies).

## What pass 26's landing changed (`lessons/codex_pass26_landing_README.md`)
- 95 of pass 26's 100 rows landed (all RELOCATION_MATCH + PASS). Its 327 header patches are in this package's `include/`: 153 headers
  changed, 60 new. 326 applied as given; one was re-anchored because another landing had put a block between its anchor lines.
- Two rows called shared retail-asm helpers without the `// Retail-asm-helper:` / `// Retail-asm:` evidence lines and had to be fixed
  at landing. Four static asserts were pinned to the wrong lines (two duplicated blocks in CNavigatorBase.h, one in
  CEngineTextureDecal.h).
- 44 rows were renamed from their verified leaf because the parked label was a placeholder, a VC10 template name or a mangled string.
  The four audio rows were named from the GameCODA 2.10 libraries.
- Pick-any: 475 copies equalled retail and 7 did not. All 7 were stale table lines that Codex had corrected with BINDING_EVIDENCE.json
  / PICKANY_CORRECTED.txt, so do the same. 15 strong same-TU copies all equalled retail at their owners' addresses. FractureShape
  0x00763060 and SetCameraState 0x00B30B50 turned up as whole extra matching functions; FractureShape is queued as its own row and
  SetCameraState is not landed yet.
- Not landed: 0x00BAA870, 0x005F59C7 and 0x00723E50 (IN_PROGRESS), and 0x00EB5750 and 0x009F3DA0 (HELD_PICKANY). Their notes are in
  `lessons/p26_open_notes/`.

## The rows (`ROWS.tsv`; per-row detail in `rows/<addr>/ROW.md`)
Each row folder has:
- `ROW.md`: extent, best grade, where it stands, **naming evidence** (Ego_d name, mangled candidates from Ego_r and FableWin, queued
  name fixes), **headers** (the class headers and their opt-in macros), callers, and landed same-TU neighbours. 7 rows are flagged
  NAME CONFLICT; settle those first.
- `best_draft.cpp`: the draft on record. Regrade it first on the package headers.
- `egod_locals.txt`, `egod_lines.txt`, `egod_tu.txt` (79 rows): Ego_d evidence, described below.
  On 7 rows, ROW.md says **Check the Ego_d identity**: the record found by name is far from the retail size, so it may be another
  overload (Ego_d's carved list keeps one record per name). Example: 0x006E0880, where Ego_d's `CTCDParticleEmitter::Create` is 1438 B,
  while Ego_r has a 235 B record of that name. Confirm the signature before you trust those locals.
- `hdr_overlay/` (3 rows: 0x005BFD5E, 0x0081EA50, 0x0083E1F0): the draft on record defines opt-ins that today's `include/` lacks. These
  are older header copies, or a diff, that carry them. Port the opt-in blocks onto `include/` as patches; do not drop the files over it.
- `ego_r_proc.txt`, `fablewin_proc.txt` (96 rows, new in pass 27): the function's records in Ego_r.pdb and FableWin.pdb, described below.
- `neighbours/`: landed rows of the same TU (copy their includes, opt-ins, pragmas and flags first). `prior/`: every earlier file.
- `handback_*`: the notes of the folder the draft came from (search them for the address).

Work in order of `ROWS.tsv` (closest first) unless a TU group is faster together (rows sharing an Ego_d object, `egod_tu.txt`).

## Evidence sources (each name needs two)
- **The three PDBs** are in the shared "debug tlc" folder (https://drive.google.com/drive/folders/1YYGZGTzO4ARouXjs_d6OUridsda1HVKK?usp=drive_link):
  - Ego_d.pdb: @@EGOD_PDB_LINK@@ (SHA-256 `d7aa26ef34d4c1857e7cebf035f4a897b77751b9d5ca308535a618062efbeae1`).
  - Ego_r.pdb: https://drive.google.com/file/d/1__Nofiu-96nx52nOIonxojM6CAOk51pC/view?usp=drive_link (SHA-256
    `8d2a921be7f4732eef4e6dcd9a239ef979bda0fee793c20d38ab5b51fdbcf573`).
  - FableWin.pdb: https://drive.google.com/file/d/1RlzNvZmo_3SXFtTLloJa0lv6Szg4-JFM/view?usp=drive_link (SHA-256
    `b2d87182f556f8c3d8fbb6b71f5491d1cb4111f8abb7cb7cb0cda269609b72cb`).

  Ego_r and FableWin use 1 KB MSF blocks that llvm-pdbutil refuses ("Too many directory blocks"). Repack them first with
  `python3 tools/repack4k.py Ego_r.pdb Ego_r.4k.pdb`, then run `llvm-pdbutil dump -symbols Ego_r.4k.pdb > Ego_r.symbols.txt`; for any
  function, run `tools/pdb_proc.py`.
- **Ego_r.pdb / FableWin.pdb** are VS 2010 (cl 16.00.40219) builds of the same engine, not the VC7.1 retail build: Ego_r is the game's
  release build, FableWin the editor's debug build (edit and continue). Already extracted per row as `ego_r_proc.txt` / `fablewin_proc.txt`: the S_GPROC32 record with the full signature (`type = ...`), the
  parameter and local names and types (S_REGISTER / S_BPREL32 / S_REGREL32), and the S_CALLSITEINFO signatures of indirect and virtual
  calls. Use them for names, const-ness, parameter types and callee signatures. Their code sizes, frame offsets and registers are not
  retail's. A local Ego_r lacks was optimised away; Ego_d alone decides which locals the source had (rule 4), and FableWin's debug
  records can only corroborate it. Where a name has several records (overloads, statics, inline copies), each is listed with its
  object file: pick the one whose signature and object fit the row.
- **Ego_d.pdb** is the VC10 /Od debug PDB of the same source tree. Already extracted per row:
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
- **Headers** in `include/` are the project's tree as of this package, including every landing through the pass 26 landing of
  2026-11-22 (its 327 header patches).

## Rules that bind these rows (`docs/EXTRA_RULES.md` is the current rule set and overrides older text)
1. Genuine compiler-emitted C++ only. No `__asm`, `naked`, `_emit` or `register`; `volatile` only where retail used it. A call to a
   shared retail-asm helper header (e.g. `engine/GFFloatToLongFloor.h`) needs a `// Retail-asm-helper: <name>` line, plus a
   `// Retail-asm: 0x<VA> `<instruction>`` line for each site citing the retail instruction (`docs/ASM_RULES.md`; NAMING_AND_HEADERS
   section 7). Without them, the row grades naked_asm.
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
   A NEQ the leaf references holds the row; report any NEQ in STATUS.tsv. If the table line is stale, hand back the corrected
   address with the evidence (BINDING_EVIDENCE.json / PICKANY_CORRECTED.txt, as in pass 26).
7. **Strong same-TU copies** of other functions are kept. Report each one in STATUS.tsv with the owner's retail address and whether it
   equals retail there. Also report a whole extra function that matches retail.

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
  - `hdr/PATCHES.json`: every old_text unique in its file and as short as possible, with the re-pinned `FABLE_STATIC_ASSERT_AT` lines
    for every assert a patch moves (both copies of a duplicated assert);
  - `NAME_FIXES.tsv` (`address<TAB>correct_symbol<TAB>evidence<TAB>wrong_spellings_seen`) for every landed spelling you prove wrong;
  - `STATUS.tsv` (row, grade, pick-any result, one-line residue);
  - `LABELS.tsv` (row, the mangled leaf).

  No retail bytes, game files or PDB contents beyond the names and types you cite.
