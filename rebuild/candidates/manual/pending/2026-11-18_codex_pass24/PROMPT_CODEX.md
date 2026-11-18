# Paste this into ChatGPT Codex together with this package zip, the maintainers' tools zip you used for passes 15-23 (unchanged), your own Fable.exe, and Ego_d.pdb (link in README_CODEX.md)

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the 100
functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe (Fable: The
Lost Chapters), written the way the original engineers wrote it -- their class names, member names, local names, headers and idioms.

Every row is a parked near miss (a draft within 0-3 bytes of retail's size) that Ego_d.pdb can help with. Ego_d.pdb is the debug PDB of
the same source tree (VC10 /Od): it holds each function's real parameter and local names, their types and scope blocks, and the object
file each function came from in source order. Download it from the link in README_CODEX.md "Ego_d.pdb" and check its SHA-256.

Start with README_CODEX.md and follow it exactly: read the documents it lists, in order and in full, before writing code -- first
egod/EGOD_FINDINGS.md and egod/LESSONS_EGOD.md (what matched 68 rows with Ego_d yesterday), then the measured compiler rules in
docs/MATCHING_PLAYBOOK.md, the newest lines of docs/GOTCHAS.md and the hand-back lessons in lessons/. docs/EXTRA_RULES.md is the current
rule set and overrides older text.

ROWS.tsv has two kinds of row:
- 35 **egod_parked** rows: the Ego_d lane already ported these onto Ego_d's locals and parked them one named residue short (ROW.md quotes
  its verdict). Start from that site; do not redo the port.
- 65 **egod_open** rows: parked before Ego_d existed. Regrade rows/<addr>/best_2026-11-18.cpp on the package headers, then rebuild it from
  Ego_d: rows/<addr>/egod_locals.txt (the source's locals in their scopes; no local = one expression), rows/<addr>/egod_tu.txt (same-TU
  callees in source order; landed TU-mates show the includes and opt-ins), then the playbook levers.
Ego_d's names are source evidence, not a byte recipe, and a name taken from it needs a second source. Its frame offsets and STL internals
are VC10's, not retail's: spell STL from include/stlport/.

For every row: cut the retail bytes from my Fable.exe with tools/cut_row.py, compile with the real VC7.1 RTM cl.exe, compare with
relocated operands masked, then check every relocation names the right symbol. Make at least 10 genuinely different source-shape
attempts before parking a row, keep going while the residue shrinks, and log each attempt in rows/<addr>/PROBES.md.

Headers: read README_CODEX.md "Header changes" before touching include/. Additions only, behind new `#ifdef FABLE_RETAIL_*` opt-ins with
the default path unchanged; never reuse or edit an existing opt-in; never move a FABLE_STATIC_ASSERT without pinning it
(FABLE_STATIC_ASSERT_AT); hand each change back as old-text / new-text pairs (hdr/PATCHES.json as in pass 23); re-verify every matched
row on your final include tree.

Hard rules: no __asm / naked / _emit / register anywhere; volatile only where retail used it; no new #pragma pack; inline devices only
per the owner ladder (FABLE_FORCEINLINE with an `// Inline-budget:` line, never a bare __forceinline); exact PDB spellings proven by two
sources (a descriptive name only when none exists anywhere); one retail address = one mangled name; no local model of a class that has a
shared header; a self-contained behaviour test for every row that builds /Od /W3 WITHOUT /EHsc with include/ only (no test_include/),
with a case for every condition in the row; LABELS.tsv names equal the leaf you compiled; no retail bytes, game files, PDB contents
beyond names / types you cite, or links in your output. Verify every name, every extent and every Leaf, and grade every file you return.

Return the zip described in README_CODEX.md "Return" (rows/, hdr/ with EVERY header change your sources need, STATUS.tsv, NAME_FIXES.tsv,
LABELS.tsv, BOUNDS_FIX.tsv for any extent you correct, LESSONS.md, START_HERE_CLAUDE.md, SHA256SUMS).
