# Paste this into ChatGPT Codex together with this package zip, your own Fable.exe, the VC7.1 RTM compiler, and Ego_d.pdb (link in README_CODEX.md)

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the
100 functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe
(Fable: The Lost Chapters). Write it the way the original engineers wrote it: their class names, member names, local names, headers
and idioms.

Every row is already close: its best draft is 0-7 bytes from retail length, usually one or two instruction groups off. Start with
README_CODEX.md and follow it exactly. Before writing any code, read docs/NAMING_AND_HEADERS.md: names and shared headers are graded
as strictly as bytes. Then for each row read rows/<addr>/ROW.md (naming evidence, headers and opt-in macros, callers, same-TU
neighbours), its notes and its Ego_d files. docs/EXTRA_RULES.md is the current rule set and overrides older text;
docs/FINDINGS_LOG.md holds the measured compiler mechanisms.

Key facts:
- Ego_d.pdb is the debug (VC10 /Od) PDB of the same source. rows/<addr>/egod_locals.txt gives the real local names, types and scope
  blocks; egod_lines.txt gives per-line debug byte counts (a size oracle for statement shape). Text that Ego_d contradicts cannot land.
- Names: two sources for every name (Ego_r / FableWin mangled names, Ego_d, vtable slots, caller types, strings). Manifest and catalog
  labels are hints, often placeholders. Check names/reloc_bindings.tsv for the spelling each callee address is bound to, and
  names/NAME_FIXES_QUEUE.tsv for spellings already proven wrong. 13 rows have a NAME CONFLICT flagged in ROW.md; settle those first.
- Headers: model every class with include/engine/<Class>.h plus its FABLE_RETAIL_* opt-ins. Copy the same-TU neighbours' #define block
  first. Add a missing class header from names/struct_layouts_egor.tsv; never write a row-local struct.
- Pick-any copies: before calling a row matched, run tools/pickany_vs_retail.py on its object. Every inline copy that the binding
  table maps to another address must equal Fable.exe there.

For every row: cut the retail bytes from my Fable.exe with tools/cut_row.py and compile with the real VC7.1 RTM cl.exe. Compare with
relocated operands masked, check that every relocation names the right symbol, and run the pick-any check. Make at least 5 genuinely
different source-shape attempts before stopping on a row, keep going while the residue shrinks, and log each attempt in
rows/<addr>/PROBES.md.

Headers: additions only, behind new `#ifdef FABLE_RETAIL_*` opt-ins with the default path unchanged. Never edit an existing opt-in,
and never move a FABLE_STATIC_ASSERT without pinning it. Hand each change back as an old-text / new-text pair, and re-verify every
matched row on your final include tree.

Hard rules:
- No __asm / naked / _emit / register anywhere; volatile only where retail used it.
- Exact PDB spellings proven by two sources; one retail address = one mangled name.
- No local model of a class that has a shared header.
- A self-contained behaviour test for every row (C++03, /Od /W3, no /EHsc, include/ only), with a case for every condition in the row.
- No retail bytes, game files or PDB contents beyond the names / types you cite in your output.

Return STATUS.tsv with every row's grade, its pick-any result and a one-line residue, plus LABELS.tsv, NAME_FIXES.tsv and
hdr/PATCHES.json.
