# Paste this into ChatGPT Codex with the package zip, your own Fable.exe, the VC7.1 RTM compiler, and the "debug tlc" folder (Ego_d.pdb, Ego_r.pdb, FableWin.pdb, FableWin.exe)

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the
57 functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe
(Fable: The Lost Chapters). Write it the way the original engineers wrote it: their class names, member names, local names, headers
and idioms.

**The rows.** 57 rows released to you from other holders, 61 to 14,510 bytes, 183,289 in all. None matches at the base.
- Read ISSUES.tsv first: each row's grade here, its best grade on record and its issue classes (HEADER_DRIFT, COMPILE_TIMEOUT,
  REGRESSED, ASM, EXHAUSTED_TIE, STUB, EXTENT, GAMECODA_NAMES, SET_ASIDE).
- Work the 7 HEADER_DRIFT rows and 0x00BE4FE0 (REGRESSED) first: they were close before a header changed.
- Do not rerun hypotheses already recorded in a row's prior/ and handback_* notes.

**Read first, in order:**
1. README_CODEX.md; follow it exactly.
2. **docs/NAMING_FIRST_TIME.md, before writing any code.** Names are graded as strictly as bytes, and the owner wants them right the
   first time so that nothing needs queuing:
   - spell every name from rows/<addr>/NAMES_TO_USE.tsv and names/answers/NAME_ANSWERS.tsv;
   - settle anything UNSETTLED from two PDB sources (names/pdb/) before you write it;
   - when your name contradicts the spelling the landed tree agrees on (you settle an UNSETTLED address, replace a registered guess or
     prove a landed self wrong), return those landed rows fixed in this pass, as RESPELL units with their tests and a BINDINGS.tsv line.
     Splits that already exist (PROVEN_LANDED_WRONG) are optional; matching comes first;
   - only rows listed in names/state/LOCKS.tsv or T3_PORT_QUEUE.tsv go to NAME_CONFLICTS.tsv;
   - names/hints/ is never evidence.
3. docs/NAMING_AND_HEADERS.md (headers, opt-ins, pick-any copies; section 9 is the name gate checklist).
4. For each row: rows/<addr>/ROW.md (names to use, naming evidence, headers and opt-in macros, callers, same-TU neighbours), its notes,
   its Ego_d files and its Ego_r / FableWin records.

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
- **names/answers/NAME_ANSWERS.tsv is the answer key:** for every address the landed tree names, the verdict and the spelling to use.
  names/pdb/ holds the tables from all three PDBs: mangled names, full publics dumps, per-object function lists, module lists, class
  layouts, signatures, source maps and RTTI slots. names/state/ is what the tree binds today, including stand-ins; names/hints/ is labels only.
- **Headers:** model every class with include/engine/<Class>.h plus its FABLE_RETAIL_* opt-ins. Copy the same-TU neighbours' #define
  block first. Add a missing class header from names/pdb/struct_layouts_egor.tsv. Never write a row-local struct.
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
- Exact PDB spellings proven by two sources; one retail address = one mangled name. A best guess only where no original name exists
  anywhere, in retail's style, registered in DESCRIPTIVE.tsv and marked `// Descriptive:`; never over a proven name.
- Wide characters are `unsigned short` (`G`), never `_W`; no /Zc:wchar_t.
- A row calling a retail-asm helper header carries its own `// Retail-asm:` line (address + mnemonic).
- A ??_7 you emit has at least retail's slot count, and its class is the retail RTTI class.
- No local model of a class that has a shared header.
- A self-contained behaviour test for every row (C++03, /Od /W3, no /EHsc, include/ only), with a case for every condition in the row.
- Inline devices only for pure inline residues, smallest first, each with an `// Inline-budget:` line.
- No retail bytes, game files or PDB contents beyond the names / types you cite in your output.

**Return:**
- STATUS.tsv, with every row's grade, its pick-any result, its vftable slot result and a one-line residue;
- LABELS.tsv (row, mangled leaf, catalog name) and hdr/PATCHES.json;
- the naming files: NEW_NAMES.tsv, DESCRIPTIVE.tsv, REGISTRY_REMOVE.tsv, respell/<addr>/ units with BINDINGS.tsv, and NAME_CONFLICTS.tsv;
- each row's <addr>.cpp and <addr>_test.cpp.

Partial progress counts: return every row's best text even when it does not match.
