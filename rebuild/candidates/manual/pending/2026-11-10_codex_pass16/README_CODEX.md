# FableDecomp -- Codex hand-off 2026-11-10: 40 parked near misses

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row in
`ROWS.tsv` the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters, Steam,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000), written the way the original Lionhead / Big Blue Box engineers
wrote it: their classes, member names, headers and idioms. Not a functional equivalent and not a stand-in: the retail source, as closely
as the evidence allows. The owner's words: "readable clean c++ that is portable and byte matches retail code and behaves the same as retail."

The project is at 95.7% of all known functions (55,292 of 57,788; 47,326 of the 49,567 original catalog functions). Your last pass
(pass 15) landed 17 of 20 rows; thank you -- its lessons are in `lessons/codex_pass15_*.md` and several are now in the playbook.

This package is **40 parked near misses** (`kind = parked`), 64-578 bytes, chosen by re-grading the 358 small parked drafts against today's
headers on 2026-11-10 and keeping the closest. `ROWS.tsv` column `residue_or_note` gives today's grade and the number of differing
instruction lines after masking relocated operands (0-7). Each row has its best source as it sits in the repo (`best_2026-11-10.cpp`),
every earlier draft (`prior/`), the notes of the hand-back that produced the best draft (`handback_*.md`) and its landed neighbours.

Groups worth knowing:
- **0x009E1960 / 0x009E1A80 GFStringToInt** grade DIFFER with 0 differing lines after masking: the instruction stream already matches,
  so the residue is in a masked operand (a wrong relocation target, a constant / jump-table entry, or the symbol a relocation names).
  Find which operand differs before touching the code shape.
- **0x00660AE2 .. 0x00660D49**: six instances of one template, `CTCVillage::RemoveThingFromTask<T>` (2 lines each). One fix should
  match all six; check the shared template body and the six T's.
- **Shared-math rows** (CRightHandedSet::Rotate / MakePerpendicular, the two fractal GetFractalHeight rows, CCamera::CCamera,
  MakeOrthogonal, Geometry_ClosestPointOnLineSegment, CHybridMultifractal): x87 order and PAREN-tuple residues, the class of
  problem your pass-15 lessons solved for 0x00A22CA0, 0x00A358F0, 0x006D6CB0 and 0x0074E140.
- **0x006509F8 CQuestInfoElementCounter::DrawAt** is a sibling of the two quest-bar DrawAt rows you parked in pass 15
  (0x0064FDB4 / 0x00651273); an in-house agent is retrying those two now, so do not change their shared quest-bar models in a way
  that would conflict -- put any header change behind a new opt-in.
- Several labels are generated (`_global::Foo_Bar`, `CAFile_...`, `Inventory_...`): settle the identity and the exact PDB name first.

A fresh idea is worth more here than more probes of the same idea: read what was tried and do something different.

## Read first, in this order
1. `docs/EXTERNAL_AI_HANDOFF.md` -- the compiler (where to get it; the free "Toolkit 2003" is a pre-RTM build and does NOT match), flags,
   hard rules, MATCH / RELOCATION_MATCH. `docs/BUILDING.md` shows how the maintainers run cl.exe under Wine.
2. `docs/EXTRA_RULES.md` -- the CURRENT rules (naming, headers, STLport, inline devices). Where it disagrees with any other document,
   EXTRA_RULES wins; within it, the newest "Owner" sections win.
3. `docs/MATCHING_PLAYBOOK.md` -- the measured rules of VC7.1's code generator (c2) that closed the last hundred near misses: the
   duplicate-return survivor, PAREN (no-code) scheduler tuples and the 81-tuple region cut, no-capture by a visible inline callee body,
   commutative operand order by candidate id, x87 load order (= most recently stored operand), branch frequencies.
   `docs/C2_ALLOCATOR_PROBE.md` explains the probe that reads c2's own decisions (the probe tool is in the separate tools zip).
4. `docs/GOTCHAS.md` -- one line per solved problem. Read it from the END backwards: the newest lines (2026-11-08/09) are the levers that
   matched the latest rows.
5. `lessons/` -- your own pass-15 lessons and guidance (`codex_pass15_*.md`), the newest in-house hand-backs (chunks 894-1001:
   LESSONS.md + NOTES.md) and the landing READMEs of 2026-11-10 (which lever landed which row), plus the 2026-11-10 naming pass
   (`naming_codex20nf_2026-11-10.md`: which of your 22 name fixes were applied, and how).
6. `docs/INLINE_DEPTH_REVISIT.md`, `docs/ASM_RULES.md`, `docs/VC71_CODEGEN_RESEARCH.md`, `docs/REGISTER_SWAP_SUMMARY.md`,
   `docs/SYMBOLS.md`, `docs/NAMING_STRAGGLERS.md`, `docs/QFE4035_COMPILER_GATE.md` as needed.
7. Per row: `rows/<addr>/ROW.md` (extent, today's residue, callers with the landed function each sits in, neighbours, earlier drafts).

## Levers that matched rows in the last few days (details and evidence in GOTCHAS / the playbook)
- **Your pass-15 levers** (`lessons/codex_pass15_LESSONS.md`): `2.0f * s2` instead of `(s2 + s2)` (0x00A56670); explicit X/Y/Z
  constructor arguments instead of a copy (0x00A22CA0); a `const&`-bound temporary for the last cross-product operand (0x00A358F0);
  binding a return value to `const unsigned int&` to get retail's `lea` (0x0099BB80); distributing a product so one no-code PAREN tuple
  disappears (0x006D6CB0 / 0x006D68D0); a grouped call result `(GetX()) * a * b` (0x006D8830); `PeekTC(id, &p)` instead of
  `HasTC` + `GetTC` (0x00868E50); complete visible same-TU callee bodies (0x005F519F).
- **Size TUs call visible inline COMDATs** (`#pragma optimize("s")`): a size-optimised TU calls the out-of-line copy of an inline
  member where a speed TU expands it (2026-11-10, 0x0041DED2); headers now carry declaration-only opt-ins for the cases that need the
  opposite (`FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY`, `FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE`).
- **Shared headers first.** A draft that models a class locally, or uses an invented stand-in container, often moves the moment it is
  rebased on `include/engine/<Class>.h` and the real STLport 4.6 templates (`include/stlport/`). Several of the parked drafts (0x0099BB80,
  0x00868E50, 0x007BC6FA, 0x00A22CA0) still carry local models: rebase them before anything else.
- **Byte twins.** If an unworked row's bytes equal a landed row's modulo relocations, it is another template instance or a per-TU copy of
  that row: copy the landed source, swap the types the differing relocations name.
- **Duplicate-return survivor**: in an else-if chain whose arms all `return`, the FIRST duplicate return block keeps the tail.
- **PAREN tuples**: parentheses that generate no code still add scheduler tuples; three parenthesised negations moved a `pop esi` to the
  epilogue (0x00BEBAA0), `sqrtf((x * x) + (y * y))` moved `faddp` / `fsqrt` one slot (0x00BBA3E0). If extra parentheses make a residue
  WORSE, retail had fewer tuples before that point (0x00A56670, one of your rows).
- **No-capture**: a comparator or scorer whose body is visible inline (COMDAT) is not "captured", which changes push / fstp order
  (`__push_heap` 0x00807620 / 0x007FA160 with `FABLE_RETAIL_CVECTORCOMPARE_BESTFORTARGETING_BODIES`).
- **x87 compares and commutative pairs**: a named float local for the compared value (0x00C720C0); the shared `C3DVector::operator^`
  for cross-product term order (0x00BEBAA0); operand order follows the order load nodes are created.
- **Template identity from the body**: a type mask the body tests, or a caller passing the same object to two calls, proves the
  instance (0x006F3570 / 0x006F37B0).

## Attempt order (exhaust each lever before the next)
1. The newest GOTCHAS lines, the playbook rules and the neighbours' shapes.
2. Shared headers / real STLport instead of local models; header opt-in branches where a header is wrong for retail.
3. Block-scope local placement (frame size / `[esp+N]` residues).
4. Same-TU non-inline bodies for callees that take a local's address (the local stops escaping).
5. Statement shape: named locals, `const T&` aliases, parentheses, operand order, explicit `(float)` temporaries for x87.
6. Condition restructuring (`||` merges, guard clauses, early returns; the duplicate-return rule).
7. Flags last: `/O2 /Oy` default, `/EHsc`, `/GS`, `#pragma optimize("s"|"t", on)`; inline devices only per the owner ladder below.
**Make at least 10 genuinely different source-shape attempts on a row before you park it** (not flag re-runs), keep going while the
residue still shrinks, and log every attempt as one line in `rows/<addr>/PROBES.md` (attempt, lever, grade / residue).

## Naming and identity (owner rules; the maintainers' landing refuses anything else)
- The label is a HINT and is wrong more often than not. Settle identity first: callers (`tools/xref.py` + ROW.md), vtable slots
  (`names/rtti-vtables.tsv`), the PDB name sets by shape and by the neighbours' proven names.
- Names right the first time: the exact PDB spelling proven by two sources. The PDB name lists (`names/egor_pdb_names.tsv`,
  `fablewin_pdb_names.tsv`, `egod_pdb_names.tsv`) are name SETS keyed by the debug builds' own addresses: never look a retail address up in
  them directly; match by name, donor order and shape. One retail address = one mangled name.
- Before declaring ANY callee or global, look its retail address up in `names/reloc_bindings.tsv` (symbol, target, caller) and use that
  exact spelling (class key from `names/pdb_class_keys.tsv`, const-ness, access, calling convention). If you prove a bound spelling
  wrong, use the PDB spelling and add a NAME_FIXES row. Canonical globals: `names/ESTABLISHED_SPELLINGS.tsv`. Spellings already proven
  wrong: `names/NAME_FIXES_QUEUE.tsv` column 5 -- never reintroduce one. Landed names: `names/catalog_names.tsv`.
- A type or owner that stays unproven after a real search is named `CUnproven<Kind>_<addr>` (EXTRA_RULES), with the search recorded
  in NAMING_EVIDENCE.md. No invented "retail" / "native" stand-in classes.
- Wide text is `unsigned short`.

## Headers (correct them while you work)
- Model `this` with `#include "engine/<Class>.h"` from `include/` (PDB-derived; real member names and offsets). A local model only where
  the header provably differs from retail; say why in a `// Layout:` comment.
- Where a header is wrong for retail, fix the header, not the row: add an opt-in branch `#ifdef FABLE_RETAIL_<CLASS>` (or a narrower
  `FABLE_RETAIL_<CLASS>_<FEATURE>`) with the retail model, leaving the default branch byte-for-byte unchanged, and a
  `// Retail opt-in (Codex 2026-11-10): <evidence>` comment. Your row `#define`s the macro before the include. Hundreds of landed rows
  include these headers, so the default path must not change. Check for an existing `FABLE_RETAIL_` branch first and reuse it (one class,
  one retail model). Return each changed header under `hdr/<same path>` with `hdr/README.md`.
- A local model may not drop a member retail had: unmodelled members are opaque storage at their retail offset and size.

## Inline devices (owner ladder, EXTRA_RULES)
Only for a residue that is purely the inline budget (which callee bodies VC7.1 expands): first plain C++, then a per-row `/d2inlS<N>` /
`/d2inlT<N>` flag, then a scoped `#pragma inline_depth(N)` ... `#pragma inline_depth()`, with a line-2 `// Inline-budget:` note. Never
for registers, layout or x87 order. `FABLE_FORCEINLINE` (include/fable_compiler.h), never a bare `__forceinline`.

## Hard rules
1. No `__asm`, `naked`, `_emit`, `volatile` or `register` anywhere (comments and identifiers included); no new `#pragma pack`; no
   device retail never had. MSVC-only spellings inside `#ifdef _MSC_VER`. C++03 that also compiles with g++ / clang.
2. One self-contained translation unit per row: `rows/<addr>/<addr>.cpp`. Line 1: `// <Class::Function(params)> -- <what the game does>`.
   A `// Leaf: <exact mangled symbol>` line (confirm it in your object's symbol table), a `// Retail:` line (address, length, convention),
   exactly one `// Flags:` line if flags beyond `/O2 /Oy` are needed, and comments that say what the code does and which retail address
   any unusual spelling reproduces.
3. A behaviour test `rows/<addr>/<addr>_test.cpp`: standalone C++03, stubs for callees, exercises the function's own logic, prints
   `<addr>_TEST PASS`. It must build under VC7.1 `/Od /W3` and 64-bit g++ `-std=c++03` (where `long` is 8 bytes).
4. If a row's extent is wrong or it is a mid-function fragment, report the true function and extent instead of authoring the fragment.
5. No retail bytes, game files, binaries or links in what you return.

## How you grade
Cut the retail bytes from your own exe (`python3 tools/cut_row.py Fable.exe 0x<addr> <length> --dis`), compile your TU with
`cl /nologo /c /O2 /Oy /W3` (+ the row's flags; include path = `include/`), and compare the function's bytes with relocated operands
masked (call targets, global / vtable / string addresses). Then check every relocation names the retail address's one symbol
(`names/reloc_bindings.tsv`). MATCH = identical bytes; RELOCATION_MATCH = identical except relocated operands that all name the right
symbols. Jump-table entries are relocations too. The separate tools zip from pass 15 (unchanged) has the maintainers' grading and probing helpers; `tools/cut_row.py` and `tools/xref.py` are in this package.

## Return
A zip with:
- `rows/<addr>/{<addr>.cpp, <addr>_test.cpp, PROBES.md, NAMING_EVIDENCE.md}` for every row (a parked row keeps its best attempt as
  `<addr>.cpp` with line 2 `// Grade: DIFFER(<ours>v<retail>) -- <residue>`),
- `hdr/<path>` + `hdr/README.md` for every corrected or new header,
- `STATUS.tsv` (address, status VERIFIED_MATCH | RELOCATION_MATCH | PARKED | EXTENT_WRONG, attempts, assigned_name (mangled),
  retail_size, object_size, flags, lever, residue, test_status),
- `NAME_FIXES.tsv` (address 0x+8 hex, correct_symbol, evidence, wrong_spellings_seen) and `LABELS.tsv` (address, name, evidence),
- `LESSONS.md` (one line per idea that unlocked a row, GOTCHAS style, with the retail address), `SHA256SUMS`.
The maintainers regrade every row with the real compiler and their verifier, check every bound symbol against the PDB name sets, and land
MATCH / RELOCATION_MATCH rows with passing tests.
