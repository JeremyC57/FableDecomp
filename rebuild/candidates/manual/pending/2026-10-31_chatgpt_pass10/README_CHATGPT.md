# FableDecomp — 100 parked rows for ChatGPT pass 10, 2026-10-31

Goal: byte-exact C++ for retail PC `Fable.exe` (Fable: The Lost Chapters, Steam, SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`),
compiler **MSVC 7.1 / VS .NET 2003, cl 13.10.3077 RTM**, base flags `cl /nologo /c /O2 /Oy /W3`. Read `docs/EXTERNAL_AI_HANDOFF.md`
first (compiler, rules, levers), then `docs/EXTRA_RULES.md` (naming), `docs/GOTCHAS.md` from the end backwards, then `docs/lessons/*`
and `docs/CODEX_PARKED50_LESSONS.md` (the last outside pass: 11 of 50 landed; what worked there).

## Why these 100
Every row was authored once and retried in one or two sweeps, and each miss is *small*: 6 to 778 retail bytes, our best attempt within
0-11 bytes of retail (57 rows within 2 bytes, 12 the same length). The in-house classifier put all of them in the "fixable" bucket: the
recorded residue is control flow, frame layout, a missing/extra inline, a wrong parameter kind or name — not a register-colouring or x87
operand-order tie. They are the easiest-looking rows left in the parked pool; the previous attempts are the baseline to beat, not the
answer.

## What is in here
- `ROWS.tsv` — address (VA, ImageBase 0x400000; file offset in your own `Fable.exe` = VA − 0x400000), best known name, module, retail
  length, residue in bytes (0 = same length), number of attempt files, number of graded status rows, path of the best attempt, the
  latest residue line from that attempt, byte-identical template copies (one source shape lands all of them).
- `rows/<addr>/<addr>_best.cpp` — the closest attempt so far (compile it as-is first; residue on its head comment lines).
- `rows/<addr>/attempts/` — every earlier attempt, named `<batch>__<kind>__<file>` (oldest batch first; `*_test.cpp` are behaviour tests).
- `rows/<addr>/HISTORY.md` — every graded status row for the address across all passes: the levers tried and the exact residue the
  grader reported. **Do not repeat the listed levers; the point of this pass is a different idea.**
- `include/engine/*.h` — PDB-derived class headers (real member names/offsets). Model `this` with these; never a local struct
  stand-in for a class that has a header (a local model only when the header flattens the vptr, and say so in a comment).
  `HEADERS_USED_BY_ATTEMPTS.txt` lists the ones the attempts already use.
- `names/egor_pdb_names.tsv`, `names/fablewin_pdb_names.tsv`, `names/egod_pdb_names.tsv` — address → mangled name from the three
  debug-build PDBs (the two-source rule: a spelling must appear in two of {these lists, struct_layouts, a landed binding, the bytes}).
  `names/struct_layouts_egor.tsv` — class/member layouts with real member names. `names/NAME_FIXES_QUEUE.tsv` — spellings already
  proven wrong and their corrections (do not reintroduce a spelling listed in column 5).
- `reloc_bindings.tsv` — mangled symbol → retail address for every landed callee/global. Every callee/global you declare must name the
  retail address's one symbol (one address = one mangled name). When the existing binding is not the PDB spelling, use the PDB spelling
  and write a NAME_FIXES row. No retail bytes are in this package: cut them from your own exe.

## Rules that get a row landed (the grader enforces them)
1. One self-contained translation unit per row, `rows/<addr>/<addr>.cpp`, first line `// <addr> <Class::Function>`, second line
   `// Leaf: <mangled name>`, plus the flags it needs (`/GS`, `/EHsc`, `#pragma optimize("s"|"t", on)`, `#pragma inline_depth(N)`).
2. No `__asm`, `naked`, `_emit`, `volatile`, no new `#pragma pack` (even in comments/identifiers). The rows were screened to exclude
   every row whose residue was an instruction VC7.1 cannot emit.
3. Names: exact PDB spelling, two sources, one retail address = one mangled name. If you believe a landed spelling is wrong, write
   `NAME_FIXES.tsv` (address, correct_symbol, evidence, wrong_spellings_seen) — never silently rename, never invent a class
   (`CUnproven*`, `*Retail`, `*Native` stand-ins are rejected at landing). Wide text is `unsigned short`. STLport 4.6 containers are
   spelled as STLport instantiations (`?erase@?$_Rb_tree@...`), not as descriptive wrappers.
4. Include `rows/<addr>/<addr>_test.cpp` (C++03; it is built on a g++ 64-bit host where `long` is 8 bytes, and under VC7.1) printing
   `<addr>_TEST PASS`. If the test embeds `rebuild_abi.h`, keep `FABLE_STATIC_ASSERT` on one line (the host transform drops lines
   containing `static_assert`).
5. For a miss, keep the best attempt as `rows/<addr>/<addr>.cpp` and one line of residue in `STATUS.tsv`.

## Return
A zip with `rows/<addr>/{<addr>.cpp,<addr>_test.cpp,HISTORY.md,NAMING_EVIDENCE.md}`, `STATUS.tsv` (address, status
VERIFIED_MATCH|PARKED, grades, selected_attempt, assigned_name, retail_size, object_size, grade_status, lever, residue, test_status),
`ROWS.tsv`, `NAME_FIXES.tsv`, `LESSONS.md` (which idea unlocked which row), `SHA256SUMS`. No game files, binaries or links in the
return. The maintainers regrade with the real compiler (`vland.py`), check every bound symbol against the PDB name sets, and land
MATCH / RELOCATION_MATCH rows. These 100 rows are reserved for this pass until 2026-11-04; in-house agents will not touch them before then.
