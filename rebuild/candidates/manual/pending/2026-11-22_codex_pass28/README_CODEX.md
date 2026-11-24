# FableDecomp -- Codex hand-off pass 28 (2026-11-22): the 25 longest parked rows

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row in
`ROWS.tsv` the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters, Steam,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000), written the way the original Lionhead / Big Blue Box engineers
wrote it: their classes, member names, headers and idioms. The owner's words: "readable clean c++ that is portable and byte matches
retail code and behaves the same as retail."

These 25 rows are reserved for you (`RESERVED.tsv` in the repo's `pending/2026-11-22_codex_pass28/`). No earlier Codex pass has had
any of them. They are the longest parked rows with a graded draft on record: 1,776 to 3,415 bytes, 53,875 bytes in all. They are not
near misses. Most drafts are 20 to 200 bytes from retail length. Four drafts (0x00647AB4, 0x00A87960, 0x008E8060, 0x00BE6880) compile
to a few bytes, so treat those rows as new work. Earlier agents' drafts and notes are in each row folder; do not repeat a recorded dead end.
Partial progress counts: hand back every row's best text and its residue, even when it does not match.

**Read `docs/NAMING_AND_HEADERS.md` before writing any code.** Names and headers are graded as strictly as bytes: a row with a
placeholder or wrong name, or a row-local model of a class that has a header, does not land. Section 8 is new: what the pass 27
landing had to fix.

## What pass 27's landing changed (`lessons/codex_pass27_landing_README.md`)
- All 100 of pass 27's rows landed (1 MATCH + 99 RELOCATION_MATCH, every test PASS). Its 354 header patches are in this package's
  `include/` (44 new headers).
- Two patches were ungated and broke 14 landed rows: an assert line added to the shared `CBankFile::GetEntryDataType` (12 rows stopped
  compiling) and a declaration-only `CTCPerceiveThing::IsAwareOfThing` (2 rows changed bytes). Both now sit under the one row's opt-in.
  **Every edit to an existing line needs its own `#ifdef`; prove it with `tools/lane_merge/equiv_check.py`** and a recheck of every landed
  row whose preprocessed text changes.
- One row lost its `// Flags:` (0x00C95580 needs `/TC /MD /Gr /Ob1`); the landing reads that line, so keep it exact.
- 16 rows carried inline devices without `// Inline-budget:` lines (`lessons/codex_pass27_inline_devices_without_marker.txt`): add the
  line and an ablation for each device you use.
- Codex's binding evidence was right: IsEventButtonHeld (0x00687B30) and IsEventButtonReleased (0x00687BD0) had been bound swapped. The
  landing renamed them across 70 files. 54 new binding lines that would give an address a second spelling are held.
- Pick-any and link: the MovieBase `??_G` copies, the JackDragon IsStillRunnable copies and the GetSubDef<CTavernGameShoveHaPennyDef>
  copies all equal retail or existing landed copies (debt). 0x00628000 / 0x00627BE0 are pair debt (one TU, two grading views).
- Open questions carried into this pass: `lessons/NEXT_CODEX_QUESTIONS.md`.

## The rows (`ROWS.tsv`; per-row detail in `rows/<addr>/ROW.md`)
Each row folder has:
- `ROW.md`: extent, best grade, where it stands, **naming evidence** (Ego_d name, mangled candidates from Ego_r and FableWin, queued
  name fixes), **headers** (the class headers and their opt-in macros), callers, and landed same-TU neighbours. No row is flagged
  NAME CONFLICT this pass; 9 rows have no Ego_d name (`-` in ROWS.tsv): name them from Ego_r / FableWin and the callers first.
- `best_draft.cpp`: the draft on record. Regrade it first on the package headers.
- `egod_locals.txt`, `egod_lines.txt`, `egod_tu.txt` (16 rows): Ego_d evidence, described below.
  On 1 row (0x00BBE090), ROW.md says **Check the Ego_d identity**: the record found by name is 683 B (VC10 /Od) for a 2,464 B retail
  row, so it may be another overload or the retail body may inline heavily. Confirm the signature before you trust those locals.
- `hdr_overlay/` (2 rows: 0x00879900, 0x00A2F740, acctC round six): the draft on record defines opt-ins that today's `include/` lacks.
  `hdr_overlay/hdrdiff/` holds the diffs that carry them. Port the opt-in blocks onto `include/` as patches; do not drop the files over it.
- `ego_r_proc.txt`, `fablewin_proc.txt` (24 rows; none for 0x00560D00): the function's records in Ego_r.pdb and FableWin.pdb, described below.
- `neighbours/`: landed rows of the same TU (copy their includes, opt-ins, pragmas and flags first). `prior/`: every earlier file.
- `handback_*`: the notes of the folder the draft came from (search them for the address).

Work in order of `ROWS.tsv` (longest first; take the closer drafts first if you prefer) unless a TU group is faster together (rows sharing an Ego_d object, `egod_tu.txt`).

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
- **Headers** in `include/` are the project's tree as of this package, including every landing through the pass 27 landing of
  2026-11-22 (its 354 header patches) and the nf235 / auth24 landings after it.

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
