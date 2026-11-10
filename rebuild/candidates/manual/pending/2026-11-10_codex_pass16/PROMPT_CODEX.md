# Paste this into ChatGPT Codex together with this package zip, the maintainers' tools zip you used for pass 15 (unchanged), and your own Fable.exe

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the 40
functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe (Fable: The
Lost Chapters), written the way the original engineers wrote it -- their class names, member names, headers and idioms.

Start with README_CODEX.md and follow it exactly: read the documents it lists, in order and in full, before writing code. The measured
compiler rules in docs/MATCHING_PLAYBOOK.md and the newest lines of docs/GOTCHAS.md (read from the end backwards) are what matched the
latest rows -- use them before inventing anything. docs/EXTRA_RULES.md is the current rule set and overrides older text.

Every row in ROWS.tsv is a parked near miss, 0-7 differing instruction lines from retail after masking. Start from
rows/<addr>/best_2026-11-10.cpp, read every earlier draft in prior/, the hand-back notes next to it (handback_*.md) and your own
pass-15 lessons in lessons/, and try what has NOT been tried. Rebase any draft that still uses local class models onto the shared headers
first. The two GFStringToInt rows already match instruction for instruction: find the masked operand that differs. The six
CTCVillage::RemoveThingFromTask<T> rows are one template: fix them together.

For every row: cut the retail bytes from my Fable.exe with tools/cut_row.py, compile with the real VC7.1 RTM cl.exe, compare with
relocated operands masked, then check every relocation names the right symbol. Make at least 10 genuinely different source-shape attempts
before parking a row, keep going while the residue shrinks, and log each attempt in rows/<addr>/PROBES.md. Fix wrong shared headers with
opt-in `#ifdef FABLE_RETAIL_<CLASS>` branches (default branch untouched) instead of local stand-ins.

Hard rules: no __asm / naked / _emit / volatile / register anywhere; no new #pragma pack; inline devices only per the owner ladder in
README_CODEX.md; exact PDB spellings proven by two sources; one retail address = one mangled name; no invented stand-in classes; a
behaviour test for every row; no retail bytes, game files or links in your output. Verify every name, every extent and every Leaf, and
grade every file you return.

Return the zip described in README_CODEX.md "Return" (rows/, hdr/, STATUS.tsv, NAME_FIXES.tsv, LABELS.tsv, LESSONS.md, SHA256SUMS).
