# FableDecomp -- Codex hand-off pass 29 (2026-11-25): 200 parked rows

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row in
`ROWS.tsv`, the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters, Steam,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000).

Write it the way the original Lionhead / Big Blue Box engineers wrote it: their classes, member names, headers and idioms. The owner's
words: "readable clean c++ that is portable and byte matches retail code and behaves the same as retail."

**These 200 rows are reserved for you** (`RESERVED.tsv` in the repo's `pending/2026-11-25_codex_pass29/`).
- They come to 199,236 bytes, between 69 and 1,769 B each. Every one has a graded draft on record.
- **The first 8 rows are AUTH26's.** AUTH26 is the owner's hand-authoring agent, now released. The owner: "auth 26 can be released so
  codex can take its rows and continue the ones that arent matched".
  - Row 0x00723E50 had four rounds of hand authoring. Its draft, `auth26/g18.cpp`, is 2 instructions off. **Read
    `rows/00723e50/auth26/TERMINAL.md` before you try anything there**: every hypothesis already graded is listed with its result.
  - The other seven were never started by AUTH26. 0x00723E50 and 0x009F3DA0 were in Codex pass 26 and were not matched.
- **The other 192** are the parked rows with a DIFFER grade that no Codex pass has had, closest first.
  - Over all 200, the draft is 0-20 B from retail length on 59 rows, 21-100 B on 121 and over 100 B on 20.
  - 11 rows are flagged NAME CONFLICT in ROW.md: settle those names first.

Partial progress counts: hand back every row's best text and its residue, even when it does not match. Earlier agents' drafts and notes
are in each row folder; do not repeat a recorded dead end.

**Read `docs/NAMING_AND_HEADERS.md` before writing any code.** Names and headers are graded as strictly as bytes: a row with a
placeholder or wrong name, or a row-local model of a class that has a header, does not land. Section 9 is new: what the coordinator's
name gate checks, and the pass-28 to pass-29 lessons.

## What changed since pass 28
- **Pass 28:** 21 of 25 rows landed (`lessons/codex_pass28_landing_README.md`). Its 137 header patches are in `include/`. Why each
  row was stuck and how it came unstuck: `lessons/CODEX_P28_LESSONS.md`, `lessons/p28_PER_ROW.md`.
- **Typed-in retail addresses are a new non-genuine grade, `baked_address`** (`lessons/baked_address_grade_README.md`).
  - A vftable, global, IAT or code address written as an integer (`*(void**)0x012C3594`, `(T*)0x0143E8F8`) does not count as
    reconstruction, even when the bytes match.
  - Spell the class, the global or the bound symbol instead. `names/data_vas.tsv` lists the 5,861 data addresses the detector knows.
  - Typed-in string addresses were already `baked_string`.
- **A real polymorphic class emits its `??_7` vftable.** No slot in it may name a different retail function than retail's table holds.
  - A local model missing one of retail's overrides binds the base function, e.g. slot 12 `CCombatActionBase::Initialise` where
    retail has the class's own.
  - 111 Clone rows were held on this today. The checker is `tools/reference/vtw425.py`.
- **Inline controls** (owner rule 2026-11-08, `docs/EXTRA_RULES.md`): `/d2inlS<N>` works without `/Og`.
  - Use the smallest N that matches, and say why on a `// Inline-budget:` line.
  - Flags go on the row's `// Flags:` line; the landing copies them to the catalog.
- **Hand authoring that worked (AUTH26, `lessons/2026-11-24_landing_auth26_u1.md`, `_u3.md`)**:
  - FableWin.exe, the editor's debug build, has the same function at /Od. Its body gives the statement order, and its call targets
    give every callee's exact PDB name. Use `tools/fwdis.py FableWin.exe <va>`; FableWin.exe is in the shared folder to disassemble it and
    `tools/fwsym.py FableWin.exe names/FableWin.publics.txt <va>...` to name the targets.
  - Combined with Ego_d's locals, this matched 0x006C0B20 on its first grade.

## The rows (`ROWS.tsv`; per-row detail in `rows/<addr>/ROW.md`)
Each row folder has:
- **`ROW.md`:** extent, best grade, where it stands, **naming evidence** (Ego_d name, mangled candidates from Ego_r and FableWin,
  queued name fixes), **headers** (the class headers and their opt-in macros), callers, and landed same-TU neighbours.
- **`best_draft.cpp`:** the draft on record. Regrade it first on the package headers.
- **Ego_d evidence (148 rows):** `egod_locals.txt`, `egod_lines.txt`, `egod_tu.txt`, described below. Where ROW.md says **Check the
  Ego_d identity**, the record found by name may be another overload: confirm the signature first.
- **PDB procedure records (182 rows):** `ego_r_proc.txt`, `fablewin_proc.txt`, the function's records in Ego_r.pdb / FableWin.pdb.
- **`neighbours/`:** landed rows of the same TU. Copy their includes, opt-ins, pragmas and flags first.
- **`prior/`:** every earlier file.
- **`handback_*`:** the notes of the folder the draft came from. Search them for the address.
- **`auth26/` (0x00723E50 only):** TERMINAL.md, LOG.md, NOTES.md, ESCALATE.md, the owner's hypotheses (OWNER_NOTES_round4.md), and
  g17 / g18.
  - Use g18 as the base for a fix that adds no scheduler tuple at the front; use g17 for one that does.

Work in `ROWS.tsv` order (AUTH26's rows first, then closest) unless a TU group is faster together (rows sharing an Ego_d object,
`egod_tu.txt`).

## Evidence sources (each name needs two)
- **You already have the three PDBs**: Ego_d.pdb (@@EGOD_PDB_LINK@@), Ego_r.pdb and FableWin.pdb. The shared "debug tlc" folder also
  holds FableWin.exe, the editor build `tools/fwdis.py` reads, and the editor's Fable.vcxproj.
  - Ego_r and FableWin use 1 KB MSF blocks that llvm-pdbutil refuses ("Too many directory blocks"). Repack them first:
    `python3 tools/repack4k.py Ego_r.pdb Ego_r.4k.pdb`, then `llvm-pdbutil dump -symbols Ego_r.4k.pdb > Ego_r.symbols.txt`.
  - For any function, run `tools/pdb_proc.py`.
- **The package carries the PDBs' tables** (owner: "any tables of the pdbs would be good to include"), all in `names/`:
  - mangled names: `egor_pdb_names.tsv`, `fablewin_pdb_names.tsv`, `egod_pdb_names.tsv`, `egod_pdb_func_names.tsv`;
  - publics, the full dumps: `Ego_r.publics.txt`, `FableWin.publics.txt`. Also `*_publics_noproc.tsv`, the publics with no procedure
    record (data, thunks);
  - per-object function lists (TU order and length): `egor_obj_functions.tsv`, `fablewin_obj_functions.tsv`;
  - module lists: `Ego_r.modules.txt`, `FableWin.modules.txt`;
  - Ego_d carvings: `egod_carved_functions.tsv`, `egod_carved_names.tsv`;
  - class layouts: `struct_layouts_egor.tsv` and `fable_types.h` (3,807 classes, real member names), `pdb_class_keys.tsv`;
  - signatures and sources: `egor_signatures.tsv`, `egor_srcfiles.tsv`, `srcmap_ego_r.exe.tsv`;
  - FableWin editor symbols: `fablewin_editor_symbols.tsv`;
  - vtables: `rtti-vtables.tsv`, `cgsi_vtable_slots.tsv`.
- **Ego_r.pdb / FableWin.pdb** are VS 2010 (cl 16.00.40219) builds of the same engine, not the VC7.1 retail build. Ego_r is the game's
  release build; FableWin is the editor's debug build, with edit and continue.
  - Each row's `ego_r_proc.txt` / `fablewin_proc.txt` holds the S_GPROC32 record with the full signature, the parameter and local
    names and types, and the S_CALLSITEINFO signatures of indirect and virtual calls.
  - Use them for names, const-ness, parameter types and callee signatures. Their sizes, frame offsets and registers are not retail's.
  - Ego_d alone decides which locals the source had (rule 4).
- **Ego_d.pdb** is the VC10 /Od debug PDB of the same source tree. Already extracted per row:
  - `egod_locals.txt`: parameter / local names, types and `{ }` scope blocks. No local means the source wrote one expression.
  - `egod_lines.txt`: the debug byte count per source line. It is a size oracle for statement shape (calibration:
    `docs/FINDINGS_LOG.md`, 2026-11-20/21).
  - `egod_tu.txt`: the row's object file in Ego_d order, with landed retail addresses.
  - Tools for any other function: `tools/egod_locals.py`, `egod_type.py`, `egod_proc.py`, `egod_lines.py`.
- **What the project has already bound** (`names/`):
  - `reloc_bindings.tsv`: the symbol each retail address is bound to;
  - `self_symbols.tsv`: each landed row's own symbol;
  - `ESTABLISHED_SPELLINGS.tsv`;
  - `NAME_FIXES_QUEUE.tsv`: landed spellings already proven wrong;
  - the naming tracker's `OPEN.tsv` / `SUMMARY.md`: every open address and why;
  - `T3_PORT_QUEUE.tsv`: queued ports;
  - `catalog_names.tsv`, `landed_sources.tsv` (with each row's flags), `manifest_names.tsv`, `labels/`;
  - `fse_api_manifest.json`.
- **Headers** in `include/` are the project's tree as of this package, with every landing through 2026-11-25. That includes NF424-NF426
  and the AUTH26 rows; `engine/CTCBuyableHouse.h` gained one opt-in today.

## Rules that bind these rows (`docs/EXTRA_RULES.md` is the current rule set and overrides older text)
1. **Genuine compiler-emitted C++ only.**
   - No `__asm`, `naked`, `_emit` or `register`; `volatile` only where retail used it.
   - No typed-in retail addresses (`baked_address`) and no typed-in string addresses (`baked_string`).
   - A call to a shared retail-asm helper header needs the `// Retail-asm-helper:` / `// Retail-asm:` lines (`docs/ASM_RULES.md`).
2. **Shared headers for `this` and every class.**
   - Header changes are ADD-ONLY behind new `#ifdef FABLE_RETAIL_*` opt-ins, with the default path unchanged; every edit to an
     existing line needs its own `#ifdef`.
   - Prove it with `tools/lane_merge/equiv_check.py`.
   - Move a `FABLE_STATIC_ASSERT` only by pinning it (`tools/lane_merge/pin_asserts.py`).
   - Hand header changes back as old_text / new_text pairs.
3. **Inline devices** (owner, 2026-11-08) are only for residues that are purely inline decisions. Try them smallest first:
   - plain C++;
   - a per-row `/d2inlT<N>` / `/d2inlS<N>` flag;
   - a scoped source control.

   Each needs a `// Inline-budget:` line. Never a bare `__forceinline`.
4. **Ego_d decides the text.** Text Ego_d contradicts (an absent or extra local, a contradicted statement order or line size) cannot
   land. A fitted choice Ego_d does not contradict may land as `// Inferred:`, with the measurement.
5. **Names exactly as in the PDBs**, from two sources, one retail address = one name. A descriptive name only where none exists
   anywhere, marked `// Descriptive:`.
6. **Pick-any copies:** every function COMDAT your object exports that `names/reloc_bindings.tsv` maps to another address must be
   byte-identical to Fable.exe there.
   - Check: `tools/pickany_vs_retail.py Fable.exe names/reloc_bindings.tsv <row>.obj <addr>`.
   - A NEQ the leaf references holds the row.
   - A `??_7` your object emits must also pass the slot check (`tools/reference/vtw425.py` shows the method).
7. **Strong same-TU copies** of other functions are kept. Report each one in STATUS.tsv with the owner's retail address and whether
   it equals retail there.

## How to work and what to return
- **Cut and compile.**
  - Cut retail bytes from YOUR Fable.exe with `tools/cut_row.py`.
  - Compile with the real VC7.1 RTM cl.exe: `/nologo /c /O2 /Oy /W3`, plus any flags a same-TU neighbour uses
    (`names/landed_sources.tsv`).
  - Compare with relocated operands masked, check that every relocation names the right symbol, and run the pick-any check.
- **Pin what the grader masks.** A relocated operand (a float constant, a string, a global) is masked by the byte compare. Pin it with
  the behaviour test or a direct compare. AUTH26's 0x006C0B20 graded the same with 20.0f as with retail's 400.0f.
- **Persist.** Try at least 5 genuinely different source shapes per row before stopping on it, and keep going while the residue shrinks.
  Log each attempt in `rows/<addr>/PROBES.md`.
- **Return per row:**
  - `<addr>.cpp`: one TU with a full mangled `// Leaf:` line, a `// Flags:` line, Ego_d-consistent text, and labelled `// Inferred:` /
    `// Inline-budget:` / `// Descriptive:` lines;
  - `<addr>_test.cpp`: a self-contained behaviour test (C++03, builds at /Od /W3 without /EHsc, include/ only) with a case for every
    condition in the row. It prints `<addr>_TEST PASS`.
- **Return once for the whole package:**
  - `hdr/PATCHES.json`: every old_text unique in its file and as short as possible, with the re-pinned `FABLE_STATIC_ASSERT_AT` lines
    for every assert a patch moves;
  - `NAME_FIXES.tsv` (`address<TAB>correct_symbol<TAB>evidence<TAB>wrong_spellings_seen`) for every landed spelling you prove wrong;
  - `STATUS.tsv`: row, grade, pick-any result, vftable slot result, and a one-line residue;
  - `LABELS.tsv`: row, the mangled leaf.

  No retail bytes, game files or PDB contents beyond the names and types you cite.
