# FableDecomp -- Codex hand-off pass 31 (2026-11-28): 57 released rows that do not match yet

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row in
`ROWS.tsv`, the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters, Steam,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000).

Write it the way the original Lionhead / Big Blue Box engineers wrote it: their classes, member names, headers and idioms. The owner's
words: "readable clean c++ that is portable and byte matches retail code and behaves the same as retail."

**These 57 rows are reserved for you** (`RESERVED.tsv` in the repo's `pending/2026-11-28_codex_pass31/`).
- **What they are:** every unlanded row that was claimed, locked or reserved elsewhere (account B, an earlier Codex pass, a paused
  authoring agent, account A) and does not match at the base. The owner released them all to this pass. Rows whose bytes already match
  were left out.
- **Size:** 183,289 bytes in all, 61 to 14,510 B each.
- **`ISSUES.tsv` comes first:** one line per row with the grade of its draft at this base, the best grade on record, and the issue classes
  found by review (several can apply):
  - **MISSING_OPT_IN (6):** the draft turns on `FABLE_RETAIL_*` opt-ins that never landed in include/ (it was graded with a lane
    overlay). The macros are listed; the overlay headers we found ship as `hdr_overlay_optins*/`. Port their blocks onto include/ as
    add-only patches, re-add the rest from the draft's notes, then regrade.
  - **SRC_FAIL (1):** 0x005AC10E does not compile here and no missing opt-in explains it (first error quoted).
  - **COMPILE_NO_TERMINATE (7):** cl.exe ran 5 minutes (earlier 15) at about 99% CPU without finishing on our Linux / Wine lane.
    Size does not explain it, since 5-6 KB drafts compiled normally; look for runaway inline or template expansion first.
  - **REGRESSED (1):** 0x00BE4FE0 matched on 2026-11-24 and differs here; find the header change.
  - **ASM (8):** retail uses instructions no VC7.1 C++ spelling emits. Follow docs/ASM_RULES.md; such a row lands only with owner approval.
    0x009BEF80 is landed as `naked_asm` and needs genuine C++.
  - **EXHAUSTED_TIE (11):** earlier passes graded many hypotheses; read `prior/` and `handback_*` first and bring a new mechanism.
  - **STUB (4):** the body on record was never fully reconstructed.
  - **EXTENT (11):** the ledger length is shorter than the bytes up to the next known start: check where the function really ends.
  - **GAMECODA_NAMES (4):** GameCODA / libpk names; pass 30 backed out a cluster there for name splits.
  - **SET_ASIDE (1):** 0x00723E50 was set aside by the owner after four rounds; only with new evidence.
- **Drafts:** `rows/<addr>/best_draft.cpp` is the selected checkpoint of the last holder (not an alternate), graded at this base.
  `hdr_overlay/` in a row folder is the header overlay that draft was graded with.

Partial progress counts: hand back every row's best text and its residue, even when it does not match.

**Naming is the first gate (owner, 2026-11-27): name right the first time, so that nothing needs queuing.**
- Read `docs/NAMING_FIRST_TIME.md` before writing any code.
- Spell every name from `rows/<addr>/NAMES_TO_USE.tsv` / `names/answers/NAME_ANSWERS.tsv`. `names/hints/` is never evidence.
- When your name contradicts the spelling the landed tree agrees on, return those landed rows fixed, as RESPELL units, in this pass.
  PROVEN_LANDED_WRONG targets (splits that already exist) are optional fixes; matching comes first.

`docs/NAMING_AND_HEADERS.md` covers headers and the pick-any rules.

**Base:** this package is the project tree at repo commit @@BASE_COMMIT@@ (include/, names/, landed/).

## What changed since pass 30
- **Pass 30 landed 45 rows and 152 respell units** with 282 headers (`lessons/codex_pass30_landing_README.md`). 7 rows and 28 units were
  backed out because their names would split against landed rows that need a port first. Its 43 primary-matching rows with link holds
  stay reserved for that pass.
- **78 catalog names were relabelled** to their rows' proven leaves (catalog text only).
- **`lessons/auth30b_*.md`:** what the paused authoring agent learned on its rows (pick-any copies, strong duplicates, inline Main devices).
- **`names/` is split** into `answers/` (spell these), `pdb/` (evidence), `state/` (what the tree binds; `LOCKS.tsv` lists rows you may not
  edit) and `hints/` (not evidence). `names/README.md` describes each folder.
- **`NAME_ANSWERS.tsv`** gives the verdict and the spelling to use for all 64,151 addresses the landed code names:
  - 35,842 PROVEN and 2,458 PROVEN_RTTI;
  - 4,401 PROVEN_LANDED_WRONG (we know the PDB name, and the listed landed rows spell it wrong);
  - 2,017 REGISTERED_GUESS and 44 FOLDED;
  - 19,389 UNSETTLED. That includes 106 vftable names whose class retail RTTI contradicts, or whose base is a stand-in.
- **Wide characters stay `G` (`unsigned short`)**, never `_W`, with no `/Zc:wchar_t` (owner, 2026-10-29).
- **The anonymous-namespace hash `?A0x<8 hex>@` is per TU.** The tools now ignore it when comparing names.

## The rows (`ROWS.tsv`; per-row detail in `rows/<addr>/ROW.md`)
Each row folder has:
- **`NAMES_TO_USE.tsv`:** the spelling to use for the row and for every retail call / jump / data target it references (docs/NAMING_FIRST_TIME.md).
- **`ROW.md`:** extent, best grade, where it stands, **names to use** (the non-proven targets listed), **naming evidence** (Ego_d name, mangled candidates from Ego_r and FableWin,
  queued name fixes), **headers** (the class headers and their opt-in macros), callers, and landed same-TU neighbours.
- **`best_draft.cpp`:** the draft on record. Regrade it first on the package headers.
- **Ego_d evidence (7 rows):** `egod_locals.txt`, `egod_lines.txt`, `egod_tu.txt`, described below. Where ROW.md says **Check the
  Ego_d identity**, the record found by name may be another overload: confirm the signature first.
- **PDB procedure records (50 Ego_r, 50 FableWin):** `ego_r_proc.txt`, `fablewin_proc.txt`, the function's records in Ego_r.pdb / FableWin.pdb.
- **`neighbours/`:** landed rows of the same TU. Copy their includes, opt-ins, pragmas and flags first.
- **`prior/`:** every earlier file.
- **`handback_*`:** the notes of the folder the draft came from. Search them for the address.

Work the MISSING_OPT_IN and REGRESSED rows first (they were close), then `ROWS.tsv` order, unless a TU group is faster together (rows sharing an Ego_d object, `egod_tu.txt`).

## Evidence sources (each name needs two)
- **You already have the three PDBs**: Ego_d.pdb (@@EGOD_PDB_LINK@@), Ego_r.pdb and FableWin.pdb. The shared "debug tlc" folder also
  holds FableWin.exe, the editor build `tools/fwdis.py` reads, and the editor's Fable.vcxproj.
  - Ego_r and FableWin use 1 KB MSF blocks that llvm-pdbutil refuses ("Too many directory blocks"). Repack them first:
    `python3 tools/repack4k.py Ego_r.pdb Ego_r.4k.pdb`, then `llvm-pdbutil dump -symbols Ego_r.4k.pdb > Ego_r.symbols.txt`.
  - For any function, run `tools/pdb_proc.py`.
- **The package carries the PDBs' tables** (owner: "any tables of the pdbs would be good to include"), in `names/pdb/` (`names/README.md`):
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
- **The answer key** (`names/answers/`): `NAME_ANSWERS.tsv`, `descriptive_names.tsv`, `ESTABLISHED_SPELLINGS.tsv` (docs/NAMING_FIRST_TIME.md).
- **What the tree binds today** (`names/state/`): `reloc_bindings.tsv`, `self_symbols.tsv` (both include stand-ins), `data_vas.tsv`, the tracker's
  `OPEN.tsv` / `SUMMARY.md`, `T3_PORT_QUEUE.tsv`, `NAME_FIXES_QUEUE.tsv`, `LOCKS.tsv`.
- **Hints, not evidence** (`names/hints/`): `catalog_names.tsv`, `manifest_names.tsv`, `landed_sources.tsv` (with each row's flags), `labels/`.
- **Headers** in `include/` are the project's tree as of this package, with every landing through 2026-11-27: Codex pass 29's 176 rows and
  341 header paths, and its name fixes (NF479).

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
5. **Names exactly as `docs/NAMING_FIRST_TIME.md` says:** spell NAMES_TO_USE.tsv; settle anything unsettled from two sources; fix every landed
   row your names contradict in this pass (RESPELL units; existing PROVEN_LANDED_WRONG splits optional); a best guess only where no original name exists, registered in DESCRIPTIVE.tsv.
6. **Pick-any copies:** every function COMDAT your object exports that `names/state/reloc_bindings.tsv` maps to another address must be
   byte-identical to Fable.exe there.
   - Check: `tools/pickany_vs_retail.py Fable.exe names/state/reloc_bindings.tsv <row>.obj <addr>`.
   - A NEQ the leaf references holds the row.
   - A `??_7` your object emits must also pass the slot check (`tools/reference/vtw425.py` shows the method).
7. **Strong same-TU copies** of other functions are kept. Report each one in STATUS.tsv with the owner's retail address and whether
   it equals retail there.

## How to work and what to return
- **Cut and compile.**
  - Cut retail bytes from YOUR Fable.exe with `tools/cut_row.py`.
  - Compile with the real VC7.1 RTM cl.exe: `/nologo /c /O2 /Oy /W3`, plus any flags a same-TU neighbour uses
    (`names/hints/landed_sources.tsv`).
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
  - the naming files of docs/NAMING_FIRST_TIME.md: `NEW_NAMES.tsv` (names you settled, with two sources), `DESCRIPTIVE.tsv` (best
    guesses you register), `REGISTRY_REMOVE.tsv` (our guesses you prove wrong), `respell/<addr>/` units with `BINDINGS.tsv` (every landed
    row whose spelling you fix, gated like a row), and `NAME_CONFLICTS.tsv` (only what could not be fixed in this pass, and why).
    Pass 29's NAME_FIXES.tsv is retired: a wrong landed spelling comes back fixed, not as a line;
  - `STATUS.tsv`: row, grade, pick-any result, vftable slot result, and a one-line residue;
  - `LABELS.tsv`: row, the mangled leaf, the catalog name (demangled: no calling convention, no `class ` / `struct ` keywords).

  No retail bytes, game files or PDB contents beyond the names and types you cite.
