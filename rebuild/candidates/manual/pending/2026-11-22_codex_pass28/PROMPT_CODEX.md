# Paste this into ChatGPT Codex together with this package zip, your own Fable.exe, the VC7.1 RTM compiler, and Ego_d.pdb, Ego_r.pdb and FableWin.pdb (links in README_CODEX.md)

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. Your job: reconstruct the
25 functions in this package as C++ that compiles, under that exact compiler, to the byte-identical retail code of PC Fable.exe
(Fable: The Lost Chapters). Write it the way the original engineers wrote it: their class names, member names, local names, headers
and idioms.

These are the 25 longest parked rows (1,776 to 3,415 bytes, 53,875 in all). Each has a draft on record, but they are not near misses:
most drafts are 20-200 bytes from retail length, and four (0x00647AB4, 0x00A87960, 0x008E8060, 0x00BE6880) compile to a stub, so
treat those as new work. Start with README_CODEX.md and follow it exactly. Before writing any code, read docs/NAMING_AND_HEADERS.md: names and shared headers are graded
as strictly as bytes, and its new section 8 lists what the pass 27 landing had to fix. Then for each row read rows/<addr>/ROW.md (naming evidence, headers and opt-in macros, callers, same-TU
neighbours), its notes, its Ego_d files and its Ego_r / FableWin records. docs/EXTRA_RULES.md is the current rule set and overrides older text;
docs/FINDINGS_LOG.md holds the measured compiler mechanisms.

Key facts:
- Ego_d.pdb is the debug (VC10 /Od) PDB of the same source. rows/<addr>/egod_locals.txt gives the real local names, types and scope
  blocks; egod_lines.txt gives per-line debug byte counts (a size oracle for statement shape). Text that Ego_d contradicts cannot land.
- Ego_r.pdb (VS 2010 game release) and FableWin.pdb (VS 2010 editor debug) are other builds of the same engine. rows/<addr>/ego_r_proc.txt and fablewin_proc.txt give the
  function's full signature, parameter / local names and types, and the call signatures of indirect calls. Use them for names and
  types only: their sizes, frame offsets and registers are not retail, and a local missing from Ego_r was optimised away.
- Names: two sources for every name (Ego_r / FableWin mangled names, Ego_d, vtable slots, caller types, strings). Manifest and catalog
  labels are hints, often placeholders. Check names/reloc_bindings.tsv for the spelling each callee address is bound to, and
  names/NAME_FIXES_QUEUE.tsv for spellings already proven wrong. 9 rows have no Ego_d name in ROWS.tsv: name them from Ego_r / FableWin, the callers and the vtables before writing code.
  Where the parked label is a placeholder or a mangled string, give the leaf-derived module::name in NOTES.md.
- Retail-asm helpers: a row that calls one needs a `// Retail-asm-helper: <name>` line and a `// Retail-asm: 0x<VA> `<instruction>`` line
  per site, or it grades naked_asm (docs/NAMING_AND_HEADERS.md section 7).
- Headers: model every class with include/engine/<Class>.h plus its FABLE_RETAIL_* opt-ins. Copy the same-TU neighbours' #define block
  first. Add a missing class header from names/struct_layouts_egor.tsv; never write a row-local struct.
- Pick-any copies: before calling a row matched, run tools/pickany_vs_retail.py on its object. Every inline copy that the binding
  table maps to another address must equal Fable.exe there. If the table line is stale, hand back the corrected address with the
  evidence (BINDING_EVIDENCE.json / PICKANY_CORRECTED.txt). Keep strong same-TU copies of other functions and report each one with
  its owner address and whether it equals retail.

For every row: cut the retail bytes from my Fable.exe with tools/cut_row.py and compile with the real VC7.1 RTM cl.exe. Compare with
relocated operands masked, check that every relocation names the right symbol, and run the pick-any check. Make at least 5 genuinely
different source-shape attempts before stopping on a row, keep going while the residue shrinks, and log each attempt in
rows/<addr>/PROBES.md.

Headers: additions only, behind new `#ifdef FABLE_RETAIL_*` opt-ins with the default path unchanged. Never edit an existing opt-in,
and never move a FABLE_STATIC_ASSERT without pinning it (give the re-pinned lines yourself, both copies of a duplicated assert). Gate EVERY edit to an existing line under
the row's own opt-in (pass 27 broke 14 landed rows with two ungated edits): run tools/lane_merge/equiv_check.py HEAD-include v your
include and recheck each landed row whose text changes. Hand each change back as an old-text / new-text pair whose old text is unique
and as short as possible, and re-verify every matched row on your final include tree.

Hard rules:
- No __asm / naked / _emit / register anywhere; volatile only where retail used it.
- Exact PDB spellings proven by two sources; one retail address = one mangled name.
- No local model of a class that has a shared header.
- A self-contained behaviour test for every row (C++03, /Od /W3, no /EHsc, include/ only), with a case for every condition in the row.
- No retail bytes, game files or PDB contents beyond the names / types you cite in your output.

Return STATUS.tsv with every row's grade, its pick-any result and a one-line residue, plus LABELS.tsv, NAME_FIXES.tsv and
hdr/PATCHES.json.
