# Paste this into ChatGPT Codex together with this package zip, the maintainers' tools zip you used for passes 15-21 (unchanged), and your own Fable.exe

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the 100
functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe (Fable: The
Lost Chapters), written the way the original engineers wrote it -- their class names, member names, headers and idioms.

Start with README_CODEX.md and follow it exactly: read the documents it lists, in order and in full, before writing code. The measured
compiler rules in docs/MATCHING_PLAYBOOK.md, the newest lines of docs/GOTCHAS.md and the hand-back lessons in lessons/ are what matched
rows like these -- use them before inventing anything. docs/EXTRA_RULES.md is the current rule set and overrides older text.

ROWS.tsv has two kinds of row:
- 50 **parked** rows (1,088-2,531 bytes): each has a draft that already compiles to retail's exact size and still differs. Regrade
  rows/<addr>/best_2026-11-14.cpp on the package headers first, rebase any local class model onto the shared header, diff, classify the
  residue, then try what has NOT been tried (prior/, handback_*, docs/PARKED_INDEX.tsv record what has).
- 50 **never_worked** rows (21-3,344 bytes): no draft, extents unchecked. Decide what each is first -- a whole function, the tail of
  the function before it (report the true extent; do not author a fragment), or an EH funclet of a parent function (matched through
  the parent, never written by hand). README_CODEX.md "The rows" says how to tell and what to return for each case.

For every row: cut the retail bytes from my Fable.exe with tools/cut_row.py, compile with the real VC7.1 RTM cl.exe, compare with
relocated operands masked, then check every relocation names the right symbol. Make at least 10 genuinely different source-shape
attempts before parking a row, keep going while the residue shrinks, and log each attempt in rows/<addr>/PROBES.md.

Headers: read README_CODEX.md "Header changes" before touching include/. Additions only, behind new `#ifdef FABLE_RETAIL_*` opt-ins with
the default path unchanged; never reuse or edit an existing opt-in; never move a FABLE_STATIC_ASSERT without pinning it
(FABLE_STATIC_ASSERT_AT); hand each change back as old-text / new-text pairs; re-verify every matched row on your final include tree.

Hard rules: no __asm / naked / _emit / register anywhere; volatile only where retail used it; no new #pragma pack; inline devices only
per the owner ladder (FABLE_FORCEINLINE with an `// Inline-budget:` line, never a bare __forceinline); exact PDB spellings proven by two
sources (a descriptive name only when none exists anywhere); one retail address = one mangled name; no local model of a class that has a
shared header; a behaviour test for every row that builds /Od /W3 WITHOUT /EHsc; LABELS.tsv names equal the leaf you compiled; no retail
bytes, game files or links in your output. Verify every name, every extent and every Leaf, and grade every file you return.

Return the zip described in README_CODEX.md "Return" (rows/, hdr/, STATUS.tsv, NAME_FIXES.tsv, LABELS.tsv, BOUNDS_FIX.tsv for any
extent you correct, LESSONS.md, SHA256SUMS).
