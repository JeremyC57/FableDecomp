# Paste this into ChatGPT Codex together with this package zip, the maintainers' tools zip you used for passes 15-20 (unchanged), and your own Fable.exe

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the 40
functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe (Fable: The
Lost Chapters), written the way the original engineers wrote it -- their class names, member names, headers and idioms.

Start with README_CODEX.md and follow it exactly: read the documents it lists, in order and in full, before writing code. The measured
compiler rules in docs/MATCHING_PLAYBOOK.md, the newest lines of docs/GOTCHAS.md (read from the end backwards) and the long-rows lane's
lessons (lessons/handH40*_LESSONS.md, lessons/handH40*_NOTES.md) are what matched rows like these -- use them before inventing anything.
docs/EXTRA_RULES.md is the current rule set and overrides older text.

ROWS.tsv has 40 parked rows, the longest still open (4,344-9,648 bytes): 30 quest-script coroutine Main bodies (NScript::CQ_* /
CV_*) and 10 engine rows (particle update / render / serialise, player targeting, A* navigator, OnHit, quest description, JackOfBlades
AI, shot collision, default render states). Every row has a best draft (rows/<addr>/best_2026-11-11.cpp), every earlier draft in prior/,
the hand-back notes next to it (handback_*) and its landed neighbours -- dozens of sibling Main bodies of the same scripts are landed and
show the exact helper spellings, scopes and pragmas. Regrade the best draft first with today's headers, then try what has NOT been tried.
In the script Mains the residue is usually c2's per-site inline split (which CScriptThing / vector / string ctor-dtor sites are expanded
v called), loop-top register ties and cross-jumped returns: tabulate retail's split per site, then find the loop / branch shape that
reproduces it (README "Levers from the long-rows lane"). Check every extent (0x00D3E2E0 is suspect). Wide text stays unsigned short:
never /Zc:wchar_t. Read the owner rules at the top of README_CODEX.md (made-up names; volatile only where retail used it; the inline
ladder) -- they override older text.

For every row: cut the retail bytes from my Fable.exe with tools/cut_row.py, compile with the real VC7.1 RTM cl.exe, compare with
relocated operands masked, then check every relocation names the right symbol. Make at least 10 genuinely different source-shape
attempts before parking a row, keep going while the residue shrinks, and log each attempt in rows/<addr>/PROBES.md. Fix wrong shared
headers with opt-in `#ifdef FABLE_RETAIL_<CLASS>` branches (default branch untouched) instead of local stand-ins.

Hard rules: no __asm / naked / _emit / register anywhere; volatile only where retail used it (README_CODEX.md); no new #pragma pack; inline
devices only per the owner ladder in README_CODEX.md; exact PDB spellings proven by two sources (a descriptive name only when none exists
anywhere, per the owner rule); one retail address = one mangled name; no invented stand-in classes; a behaviour test for every row that
builds /Od /W3 WITHOUT /EHsc; LABELS.tsv names equal the leaf you compiled; no retail bytes, game files or links in your output. Verify
every name, every extent and every Leaf, and grade every file you return.

Return the zip described in README_CODEX.md "Return" (rows/, hdr/, STATUS.tsv, NAME_FIXES.tsv, LABELS.tsv, BOUNDS_FIX.tsv for any
extent you correct, LESSONS.md, SHA256SUMS).
