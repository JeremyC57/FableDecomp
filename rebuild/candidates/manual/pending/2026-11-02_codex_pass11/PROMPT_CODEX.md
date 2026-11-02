# Paste this into ChatGPT Codex together with the package zip (and your own Fable.exe)

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the
100 functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe
(Fable: The Lost Chapters), written the way the original engineers wrote it -- their class names, member names, headers and idioms.

Start with README_CODEX.md and follow it exactly: read the documents it lists, in order and in full, before writing code. The house styles
in docs/GOTCHAS.md (read from the end backwards) and docs/lessons/ are what matched the last several hundred rows -- use them before
inventing anything.

For every row in ROWS.tsv:
- Settle its true identity first (callers via tools/xref.py, vtable slots, the PDB name sets); the label is only a hint and is usually wrong.
- Cut the retail bytes from my Fable.exe with tools/cut_row.py and read the disassembly.
- Author rows/<addr>/<addr>.cpp and a behaviour test, compile with the real VC7.1 RTM cl.exe, and compare against retail with relocated
  operands masked; then check every relocation names the right symbol.
- Make at least 10 genuinely different source-shape attempts before parking a row, and keep going while the byte residue still shrinks.
  Log each attempt in rows/<addr>/PROBES.md.
- Fix wrong shared headers with opt-in `#ifdef FABLE_RETAIL_<CLASS>` branches (default branch untouched) instead of local stand-ins.

Hard rules: no __asm / naked / _emit / volatile / #pragma inline_depth / new #pragma pack anywhere; exact PDB spellings proven by two
sources; one retail address = one mangled name; no stand-in classes; no retail bytes, game files or links in your output. Make no
mistakes: verify every name, every extent and every Leaf, and grade every file you return.

Return the zip described in README_CODEX.md "Return" (rows/, hdr/, STATUS.tsv, NAME_FIXES.tsv, LESSONS.md, SHA256SUMS).
