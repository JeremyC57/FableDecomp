# Paste this into ChatGPT Codex with the package zip, your own Fable.exe, the VC7.1 RTM compiler, and the "debug tlc" folder (Ego_d.pdb, Ego_r.pdb, FableWin.pdb, FableWin.exe)

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the
200 functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe
(Fable: The Lost Chapters). Write it the way the original engineers wrote it: their class names, member names, local names, headers
and idioms.

**The rows.** All 200 are parked rows with a graded draft on record: 69 to 1,769 bytes, 199,236 in all.
- The first 8 are the rows of the owner's released hand-authoring agent, AUTH26.
  - 0x00723E50 is 2 instructions off after four rounds. Read rows/00723e50/auth26/TERMINAL.md before touching it.
  - The other seven were never started.
- The other 192 are the closest drafts no Codex pass has had: 59 are within 20 bytes of retail length.
- 11 rows are flagged NAME CONFLICT; settle those names first.

**Read first, in order:**
1. README_CODEX.md; follow it exactly.
2. docs/NAMING_AND_HEADERS.md, before writing any code. Names and shared headers are graded as strictly as bytes. Its new section 9 is
   the checklist the coordinator's name gate runs on every row: the full mangled leaf; every relocation's spelling a PDB or std name, or
   the name already bound there; no row-local class; no typed-in retail addresses.
3. For each row: rows/<addr>/ROW.md (naming evidence, headers and opt-in macros, callers, same-TU neighbours), its notes, its Ego_d
   files and its Ego_r / FableWin records.

docs/EXTRA_RULES.md is the current rule set and overrides older text; docs/FINDINGS_LOG.md holds the measured compiler mechanisms.

**Key facts:**
- **Ego_d.pdb** is the debug (VC10 /Od) PDB of the same source.
  - rows/<addr>/egod_locals.txt gives the real local names, types and scope blocks.
  - egod_lines.txt gives per-line debug byte counts, a size oracle for statement shape.
  - Text that Ego_d contradicts cannot land.
- **Ego_r.pdb** (VS 2010 game release) and **FableWin.pdb** (VS 2010 editor debug) are other builds of the same engine.
  rows/<addr>/ego_r_proc.txt and fablewin_proc.txt give the function's full signature, its parameter / local names and types, and the
  call signatures of indirect calls. Use them for names and types only.
- **FableWin.exe's /Od body of the same function** gives the statement order. Disassemble it with `tools/fwdis.py` and name its call
  targets with `tools/fwsym.py`, which gives exact PDB spellings. That is how AUTH26 matched 0x006C0B20 on its first grade.
- **Tables from all three PDBs are in names/.** They include mangled names, the full publics dumps, per-object function lists, module
  lists, class layouts, signatures and source maps. Also there: what the project already binds (reloc_bindings.tsv, self_symbols.tsv),
  every open naming address (OPEN.tsv), and spellings already proven wrong (NAME_FIXES_QUEUE.tsv).
- **Headers:** model every class with include/engine/<Class>.h plus its FABLE_RETAIL_* opt-ins. Copy the same-TU neighbours' #define
  block first. Add a missing class header from names/struct_layouts_egor.tsv. Never write a row-local struct.
- **Pick-any copies:** before calling a row matched, run tools/pickany_vs_retail.py on its object. Every inline copy that the binding
  table maps to another address must equal Fable.exe there. A ??_7 vftable your object emits must name retail's function in every slot.
- **Masked operands:** the byte compare masks relocated operands (float constants, strings, globals). Pin each one with the behaviour
  test.

**For every row:**
- Cut the retail bytes from your Fable.exe with tools/cut_row.py and compile with the real VC7.1 RTM cl.exe.
- Compare with relocated operands masked, check that every relocation names the right symbol, and run the pick-any check.
- Make at least 5 genuinely different source-shape attempts before stopping on a row, keep going while the residue shrinks, and log
  each attempt in rows/<addr>/PROBES.md.

**Headers:** additions only, behind new `#ifdef FABLE_RETAIL_*` opt-ins, with the default path unchanged.
- Never edit an existing opt-in.
- Never move a FABLE_STATIC_ASSERT without pinning it.
- Prove the tree with tools/lane_merge/equiv_check.py.
- Hand each change back as an old-text / new-text pair, and re-verify every matched row on your final include tree.

**Hard rules:**
- No __asm / naked / _emit / register anywhere; volatile only where retail used it.
- No retail address typed in as an integer (vftable, global, IAT, string or code).
- Exact PDB spellings proven by two sources; one retail address = one mangled name. Descriptive names only where none exists anywhere,
  marked `// Descriptive:`.
- No local model of a class that has a shared header.
- A self-contained behaviour test for every row (C++03, /Od /W3, no /EHsc, include/ only), with a case for every condition in the row.
- Inline devices only for pure inline residues, smallest first, each with an `// Inline-budget:` line.
- No retail bytes, game files or PDB contents beyond the names / types you cite in your output.

**Return:**
- STATUS.tsv, with every row's grade, its pick-any result, its vftable slot result and a one-line residue;
- LABELS.tsv, NAME_FIXES.tsv and hdr/PATCHES.json;
- each row's <addr>.cpp and <addr>_test.cpp.

Partial progress counts: return every row's best text even when it does not match.
