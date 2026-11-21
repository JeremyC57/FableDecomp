# Paste this into ChatGPT Codex together with this package zip, your own Fable.exe, the VC7.1 RTM compiler, and Ego_d.pdb (link in README_CODEX.md)

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the 8
functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe (Fable: The
Lost Chapters), written the way the original engineers wrote it -- their class names, member names, local names, headers and idioms.

These are the project's hardest open rows: large (2.6-14.5 KB) and already close. Each one is down to a compiler decision -- which
inline calls are expanded, how locals are packed into the stack frame, or one register choice -- and several agents have measured each
one. Start with README_CODEX.md and follow it exactly. Then for each row read rows/<addr>/ROW.md, its notes and its Ego_d files before
writing code. docs/ADVISOR_DECISIONS_B.md and docs/FINDINGS_LOG.md (2026-11-20/21 entries) hold the rulings and the measured compiler
mechanisms; docs/EXTRA_RULES.md is the current rule set and overrides older text.

Key facts:
- Ego_d.pdb is the debug (VC10 /Od) PDB of the same source: rows/<addr>/egod_locals.txt gives the real local names, types and scope
  blocks; egod_lines.txt gives per-line debug byte counts (a size oracle for statement shape). Text Ego_d contradicts cannot land.
- Some drafts reach their grade only with devices that are not allowed (named in ROW.md). Start those rows from the admissible
  B_base / B_natural_base file instead.
- Inline controls are allowed only for pure inline-decision residues, smallest first: per-row /d2inlT or /d2inlS flag, then a
  #pragma inline_depth region around one statement, each with an `// Inline-budget:` line.

For every row: cut the retail bytes from my Fable.exe with tools/cut_row.py, compile with the real VC7.1 RTM cl.exe, compare with
relocated operands masked, then check every relocation names the right symbol. Make at least 10 genuinely different source-shape
attempts before stopping on a row, keep going while the residue shrinks, and log each attempt in rows/<addr>/PROBES.md.

Headers: additions only, behind new `#ifdef FABLE_RETAIL_*` opt-ins with the default path unchanged; never edit an existing opt-in;
never move a FABLE_STATIC_ASSERT without pinning it; hand each change back as old-text / new-text pairs; re-verify every matched row
on your final include tree.

Hard rules: no __asm / naked / _emit / register anywhere; volatile only where retail used it; exact PDB spellings proven by two
sources; one retail address = one mangled name; no local model of a class that has a shared header; a self-contained behaviour test
for every row (C++03, /Od /W3, no /EHsc, include/ only) with a case for every condition in the row; no retail bytes, game files or
PDB contents beyond the names / types you cite in your output. Return STATUS.tsv with every row's grade and one-line residue.
