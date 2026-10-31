# FableDecomp — 50 parked rows for a fresh outside pass (Codex), 2026-10-31

Goal: byte-exact C++ for retail PC `Fable.exe` (Fable: The Lost Chapters, Steam, SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`),
compiler **MSVC 7.1 / VS .NET 2003, cl 13.10.3077 RTM**, base flags `cl /nologo /c /O2 /Oy /W3`. Read `docs/EXTERNAL_AI_HANDOFF.md`
first (compiler, rules, levers), then `docs/GOTCHAS.md` from the end backwards, then `docs/lessons/*`.

## What is in here
- `ROWS.tsv` — the 50 rows: address (VA, ImageBase 0x400000; file offset in your own `Fable.exe` = VA − 0x400000), best known
  name, module, retail length, our best attempt's length, residue (0 = same length: the miss is instruction order / register
  choice / scheduling, not size), number of attempt files, number of graded status rows, path of the best attempt, byte-identical
  template copies (one source shape lands all of them).
- `rows/<addr>/<addr>_best.cpp` — the closest attempt so far (compile it as-is first; it is the baseline to beat).
- `rows/<addr>/attempts/` — every earlier attempt, named `<batch>__<kind>__<file>` (oldest batch first; `.test.cpp` are behaviour tests).
- `rows/<addr>/HISTORY.md` — every graded status row for the address across all passes: which levers were tried and the exact
  residue the grader reported (e.g. "retail loads p.first before storing p.second into the arg slot; ours stores first").
  **Do not repeat the listed levers; the point of this pass is a different idea.**
- `include/engine/*.h` — PDB-derived class headers (real member names/offsets). Model `this` with these; never a local struct
  stand-in for a class that has a header. `HEADERS_USED_BY_ATTEMPTS.txt` lists the ones the attempts already use.
- `reloc_bindings.tsv` — mangled symbol → retail address for every landed callee/global. Every callee/global you declare must
  name the retail address's one symbol (one address = one mangled name). No retail bytes are in this package: cut them from
  your own exe.

## Rules that get a row landed (the grader enforces them)
1. One self-contained translation unit per row, `rows/<addr>/<addr>.cpp`, first line `// <addr> <Class::Function>`, plus flags
   it needs (`/GS`, `/EHsc`, `#pragma optimize("s"|"t", on)`, `#pragma inline_depth(N)`).
2. No `__asm`, `naked`, `_emit`, `volatile` (even in comments/identifiers). The 50 rows were screened to exclude every row
   whose residue was an instruction VC7.1 cannot emit.
3. Names: exact PDB spelling, two sources; if you believe a landed spelling is wrong, write `NAME_FIXES.tsv`
   (address, correct_symbol, evidence, wrong_spellings_seen) — never silently rename. Wide text is `unsigned short`.
4. Include `<addr>_test.cpp` (C++03, g++ 64-bit host: `long` is 8 bytes, exact float literals) printing `<addr>_TEST PASS`.
5. For a miss, keep the best attempt and one line of residue in a `STATUS.tsv` (address, status, grades, lever, residue).

## Return
A zip with `rows/<addr>/{<addr>.cpp,<addr>_test.cpp}`, `STATUS.tsv`, optional `NAME_FIXES.tsv`, `LESSONS.md` (which idea
unlocked which row). The maintainers regrade with the real compiler (`verify_and_land.py`) and land MATCH / RELOCATION_MATCH rows.
These 50 rows are reserved for this pass until 2026-11-03; in-house agents will not touch them before then.
