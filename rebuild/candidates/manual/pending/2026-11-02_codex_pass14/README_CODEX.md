# FableDecomp -- Codex pass 14: 100 never-attempted rows (2026-11-02)

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row
in `ROWS.tsv` the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters,
Steam, SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000), written the way the original Lionhead / Big Blue Box
engineers wrote it: their class, their member names, their headers, their idioms. Not a functional equivalent, not a stand-in: the
retail source as closely as the evidence allows.

The 100 rows were never authored by anyone. They are large functions (1044-3962 bytes each, 200331 bytes in all; mostly unnamed or mislabelled)
from never-attempted pool chunks NA097-NA106. They are reserved for you: in-house agents will not touch them until 2026-11-30.

Work each row with patience. **Make at least 10 genuinely different source-shape attempts on a row before you park it** (not flag
re-runs), and keep going while the residue still shrinks. A row that matches early still gets its name, its `// Leaf:`, its test and its
evidence checked as hard as a parked row. Make no mistakes: verify every name with two sources, every extent against the int3 / alignment
padding, every Leaf against your object's symbol table, and grade every source you hand back.

## Read first, in this order
1. `docs/EXTERNAL_AI_HANDOFF.md` -- the compiler (where to get it; the free "Toolkit 2003" is a different build and does NOT match),
   flags, the hard rules, what MATCH / RELOCATION_MATCH mean. `docs/BUILDING.md` shows how the maintainers run cl.exe under Wine.
2. `docs/EXTRA_RULES.md` -- the current agent rules (naming, headers, STLport); the newest "Owner" sections override older text.
3. `docs/GOTCHAS.md` -- read it from the END backwards: the newest lines (2026-11-02, chunks 703-714) are the house styles that matched
   the last 60 rows in a row. Then `docs/lessons/*` (chunk650-714, the ChatGPT pass 10 and Codex parked-50 lessons).
4. `docs/INLINE_DEPTH_REVISIT.md` -- why `#pragma inline_depth` regions are no longer accepted.
5. Per row: `rows/<addr>/ROW.md` (callers, vtable slots, automatic notes, twin group) and `rows/<addr>/neighbours/` (landed sources from
   the same address range -- usually the same translation unit: reuse their class models, includes, pragmas and style).

## House styles learned so far (details and evidence in GOTCHAS; respect them before inventing anything)
- A virtual override that ends in `jmp <base>` is `Base::F();` as the last statement of `Derived::F()`; name it from its vtable slot.
- Destructors store their own vtable first (`~X() { }` with a body stores it; a compiler-generated dtor does not), destroy members in
  reverse order and tail-jump to the base dtor symbol.
- VC7.1 lays out overloaded virtuals in REVERSE declaration order.
- Name a pointer you call through as a block local (`CThingAICreature* creature = ...; creature->F(a, b);`): reading a member twice reloads
  it, and the inline cast form `static_cast<T*>(p.GetPItem())->F(a, b)` pushes the arguments before the call.
- A function with two exits that both tail-jump to the same call is a guard clause: `if (!A() || B()) { Terminate(); return; } ...; Terminate();`.
- `new T(...)` with an inline ctor: `push size; call ??2@YAPAXI@Z` then the inline field stores; the pushed size proves sizeof(T).
- `(float)((float)a*b)` forces the early product spill; three adjacent float slots are one C3DVector local; a C2DVector temporary assignment
  gives the 8-byte frame temporary.
- STLport 4.6 containers in namespace `std` with their real templates and retail sizes (EXTRA_RULES "RETAIL CONTAINER SIZES"): list 4,
  vector 0xC, map/set/multimap 0xC, basic_string<char> 0xC, deque 0x28; CVectorMap 0x10. `(end-begin)/sizeof` recomputed each loop
  iteration is a real STLport `size()`; an STLport-style vector member defeats the size CSE.
- Same-TU non-inline bodies for callees that take a local's address let VC7.1 keep the local in registers (chunk 422 recipe).
- Dense switches become `jmp [reg*4+table]` with the table inside the function's extent.
- Size-optimised TUs (`#pragma optimize("s", on)`): push/pop instead of mov, fewer inlines, `lea ebp,[esp-N]` frames.
- C++ EH frames (`push -1; push handler; mov eax,fs:[0]`) need `/EHsc` and an object with a destructor alive across a call.
- `/GS`: the prologue xors the security cookie into a frame slot.
- Well-known callees: 0x00A01B50 = `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` (CBaseObject* return; ~60 stand-in
  spellings in reloc_bindings.tsv are WRONG), 0x00BFEA1A = `??2@YAPAXI@Z`, 0x00BFE9BC = `??3@YAXPAX@Z`, 0x00BFEA14 = `_free` (write
  `free()`, never `operator delete`, when retail calls it), 0x00BFEA70 = `__ftol2`.

## Attempt order (exhaust each lever before the next)
1. The newest GOTCHAS lines and the neighbours' shapes.
2. Block-scope local placement (frame size / `[esp+N]` residues).
3. Same-file non-inline stand-ins for every callee the retail body inlines or calls, letting VC7.1's automatic inlining decide (never
   `#pragma inline_depth`, never `__forceinline` unless the PDB says so).
4. The real STLport 4.6 container bodies instead of hand models for iterator residues.
5. `(float)((float)a*b)` style explicit float temporaries for x87 order.
6. Condition restructuring (`||` merges, hoisted common calls, early returns, guard clauses).
7. The retail class header vs a local model when member-access shapes differ.
8. Flags last: `/O2 /Oy` default, `/Og-`, `/Ob1`, `/EHsc`, `/GS`, `#pragma optimize("s"|"t", on)`.
Log every attempt as one line in `rows/<addr>/PROBES.md` (attempt number, the lever, the grade / residue).

## Naming and identity (owner rules; the maintainers' landing refuses anything else)
- The label in ROWS.tsv is a HINT and is wrong more often than not (in-house chunks 696-716 found most labels in this band wrong). Settle
  each row's own identity first: callers (`tools/xref.py`, then `names/catalog_names.tsv` for the landed caller), vtable slots
  (`names/rtti-vtables.tsv`), the PDB name sets by shape and by the neighbours' proven names, then author.
- Names right the first time: the exact PDB spelling, proven by two sources (the PDB name lists `names/egor_pdb_names.tsv`,
  `fablewin_pdb_names.tsv`, `egod_pdb_names.tsv` are name SETS keyed by the debug builds' own addresses -- never look a retail address up
  in them directly; match by name, donor order and shape), one retail address = one mangled name.
- Before declaring ANY callee or global, look its retail address up in `reloc_bindings.tsv` (symbol, target, caller). If the PDB spelling
  is there, use exactly it (class, class key from `names/pdb_class_keys.tsv`, const-ness, access, calling convention). If only stand-ins
  are there, use the PDB spelling and write a NAME_FIXES row. Canonical globals: `names/ESTABLISHED_SPELLINGS.tsv`. Spellings already
  proven wrong: `names/NAME_FIXES_QUEUE.tsv` column 5 -- never reintroduce one.
- Where nothing proves a name (libpk / vendor code with no PDB), use one descriptive name with the address suffix and say so.
- Byte twins (ROW.md "Twin group"): name each copy from its own callers / vtable; only provably identical symbols share a source.
- Wide text is `unsigned short`. Never invent stand-in classes (`CUnproven*`, `*Retail`, `*Native`, `Native::` namespaces are rejected).

## Headers (correct them while you work)
- Model `this` with `#include "engine/<Class>.h"` from `include/` (PDB-derived, real member names and offsets). A local model only when
  the header provably differs from retail; say why in a `// Layout:` comment.
- When a header is wrong for retail (vptr flattened into a data member, Ego_r-only members or offsets, a missing base or member function),
  fix the header instead of writing a local model: add an opt-in branch `#ifdef FABLE_RETAIL_<CLASS>` (the retail layout) `#else` (the
  existing text, byte-for-byte unchanged) `#endif`, with a `// Retail opt-in (Codex pass 14): <evidence>` comment (RTTI bases, vtable
  address and slot count, member offsets from the PDB checked against retail bytes). Your row `#define`s the macro before the include.
  Never change the default branch: hundreds of landed rows include these headers. Return each changed header under `hdr/engine/` with
  `hdr/README.md` (what changed, the evidence).
- Check `include/engine/<Class>.h` for an existing `FABLE_RETAIL_` branch first and reuse it (one class, one retail model).
- No local model may drop a member retail had: an unmodelled member is opaque storage at its retail offset and size
  (`unsigned char Body_[0x1B8]; // CRigidBody Body +0xA0`), up to the retail sizeof.

## Hard rules
1. No `__asm`, `naked`, `_emit` or `volatile` anywhere (comments and identifiers included), no `#pragma inline_depth`, no forwarding
   overloads, no new `#pragma pack`, no device retail never had. (A genuine retail inline-asm block -- an instruction VC7.1 cannot emit from
   C++ -- is reported, not written.)
2. One self-contained translation unit per row: `rows/<addr>/<addr>.cpp`. Line 1: `// <Class::Function(params)> -- <what it does>`.
   Line 2: `// Leaf: <exact mangled symbol>` (confirm it in your object's symbol table). Exactly one `// Flags:` line if flags beyond
   `/O2 /Oy` are needed.
3. A behaviour test `rows/<addr>/<addr>_test.cpp`: standalone C++03, exercises the function's own logic, prints `<addr>_TEST PASS`. It is
   built on a 64-bit g++ host (where `long` is 8 bytes) and under VC7.1. Keep any `FABLE_STATIC_ASSERT` on one line (the host transform
   drops lines containing `static_assert`).
4. If a row's extent is wrong or the row is a mid-function fragment, report the true function and extent instead of authoring the fragment.
5. No retail bytes, game files, binaries or links in what you return.

## How you grade
Cut the retail bytes from your own exe (`python3 tools/cut_row.py Fable.exe 0x<addr> <length> --dis`), compile your TU with
`cl /nologo /c /O2 /Oy /W3` (+ the row's flags), and compare the function's bytes with relocated operands masked (call targets,
global / vtable / string addresses). Then check every relocation names the retail address's one symbol. MATCH = identical bytes;
RELOCATION_MATCH = identical except relocated operands that all name the right symbols. Jump-table entries are relocations too: check
each one points at the right case.

## Return
A zip with:
- `rows/<addr>/{<addr>.cpp,<addr>_test.cpp,PROBES.md,NAMING_EVIDENCE.md}` for every row (a parked row keeps its best attempt as
  `<addr>.cpp` with line 3 `// Grade: DIFFER(<ours>v<retail>) -- <residue>`),
- `hdr/engine/*.h` + `hdr/README.md` for every corrected or new header,
- `STATUS.tsv` (address, status VERIFIED_MATCH|RELOCATION_MATCH|PARKED|EXTENT_WRONG, attempts, assigned_name (mangled), retail_size,
  object_size, flags, lever, residue, test_status),
- `NAME_FIXES.tsv` (address 0x+8 hex, correct_symbol, evidence, wrong_spellings_seen) for every landed or bound spelling you prove wrong,
- `LESSONS.md` (one line per idea that unlocked a row, GOTCHAS style), `SHA256SUMS`.
The maintainers regrade every row with the real compiler and their verifier (`vland.py`), check every bound symbol against the PDB name
sets, and land MATCH / RELOCATION_MATCH rows; parked rows feed the next pass.

## Headers from other passes
This is one of several Codex passes (11-14) handed out in turn. `include/` is the maintainers' tree as of 2026-11-02. If an earlier
pass's returned `hdr/` is in your workspace, build on it rather than adding a second retail model of the same class; otherwise add your
own opt-in branch and the maintainers merge them at landing.
