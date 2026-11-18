# FableDecomp -- Codex hand-off pass 24 (2026-11-18): 100 parked near misses for Ego_d.pdb

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row in
`ROWS.tsv` the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters, Steam,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000), written the way the original Lionhead / Big Blue Box engineers
wrote it: their classes, member names, headers and idioms. The owner's words: "readable clean c++ that is portable and byte matches
retail code and behaves the same as retail."

The project has 56,632 of the 57,842 known retail functions landed as byte-matching C++ (97.91%; 87.37% of retail function bytes).
About 1,210 functions are open; `docs/UNMATCHED_ROWS.tsv` lists every one with its status and best grade. The 100 in this package are
reserved for you (`pending/2026-11-18_codex_pass24/RESERVED.tsv`). Every one is a parked near miss that **Ego_d.pdb**, a debug PDB of
the same source tree found on 2026-11-17, can help with. Read "Ego_d.pdb" below before anything else.

## Ego_d.pdb (NEW: the reason for this pass)
**Download:** @@EGOD_PDB_LINK@@
(270 MB; SHA-256 `d7aa26ef34d4c1857e7cebf035f4a897b77751b9d5ca308535a618062efbeae1`, GUID `{B5DFE434-B3F5-441D-97D9-7DDCE5DF8227}`,
age 4; check the hash after download). The same folder holds Ego_r.pdb and FableWin.pdb, whose names and types this package already
carries (`names/`); you do not need those two.

What it is (`egod/EGOD_FINDINGS.md`, read it whole): the debug-configuration PDB of the TLC source tree (`Fable1_5MainPC`), built with
**VC10 /Od**, not retail's VC7.1 /O2. Its old module streams were carved back out of free pages, and they give two things no other
source has:
1. **Every function's own parameter and local names, their types, their scope blocks (`{ }` in which the source declared locals) and
   function-local statics.** Each row folder already has them extracted with types resolved: `rows/<addr>/egod_locals.txt`.
   To look up anything else: `python3 -I tools/egod_locals.py Ego_d.pdb '<Qualified::Name>'`, then
   `llvm-pdbutil dump -types Ego_d.pdb > types.txt` and `python3 tools/egod_type.py types.txt --locals <that output>` (or
   `tools/egod_type.py types.txt 0x<index>` for one type; class records `LF_FIELDLIST` give private statics and inline accessors).
2. **The object file (TU) each function came from and its order in it**: `names/egod_carved_functions.tsv`; per row, `rows/<addr>/egod_tu.txt`
   lists the row's whole TU in Ego_d order (`>>` = the row) with the retail address of every function already landed.

What it is NOT: frame offsets, inlining, code generation and STL internals are VC10 debug (Dinkumware `_Tree`, `_List_val`): spell STL from
`include/stlport/` (STLport 4.6), never from Ego_d. A whole `std::map<K, V>` type carries over (same template arguments); its internals do not.
**A name taken from Ego_d needs a second source** (Ego_r / FableWin name sets, strings, FSE, callers, vtable slots) like any other name.

### What worked with it (the Ego_d lane, 2026-11-17: 68 of the 107 rows it worked matched and landed; `egod/LESSONS_EGOD.md`, `egod/batches/`)
- **Same-TU callees defined in the row, in Ego_d's object order** (from `egod_tu.txt`) closed 5 of the first 7 matches. Prove each with a
  noinline stub first, then the real body, and grade that body at its own retail address when it has one.
- **Port onto the shared headers with Ego_d's locals FIRST, diagnose second.** Rows parked as "allocation gap" / "register tie" on local
  class models matched at the first compiling grade once ported (0x00920DC0, 0x00B81820, 0x00867D20).
- **Start from a LANDED row of the same TU** (`egod_tu.txt` marks them): copy its includes and opt-in set, then add Ego_d's locals verbatim.
- **Negative evidence:** no local in Ego_d means the source wrote ONE expression; a draft's named temporary that Ego_d lacks is wrong
  (0x00843310, 0x00AC7CB0 closed in one grade each).
- **Scope blocks** follow the source's `if` / loop nesting: declare each local in the block Ego_d puts it in (frame size / `[esp+N]` and
  register-tie residues).
- **Function-local statics:** a writable `.data` constant with one reference is often a block-scope `static float f = ...;` (0x007B6810):
  check `static S_LDATA32` lines before inventing a global.
- **Real PDB overloads:** an x87-order residue in an expanded transform closed with the OTHER overload (two-argument TransformPoint);
  a callee with a descriptive landed name often has a real name (read the name sets around its neighbours).
- **`egod_tu.txt` omits common inline COMDATs** (C2DVector::operator+): absence is not evidence a callee was out of line.
- Ego_d's debug length equals FableWin's /Od size, so it is a free size check for an /Od build of your source.

## The rows (`ROWS.tsv`, column `kind`; every row is one bounded function -- the extent scan agrees with the recorded retail length)
### 32 `egod_parked` rows: the Ego_d lane's own near misses
Worked on 2026-11-17 WITH Ego_d's locals and TU and parked one residue short; ROW.md quotes the lane's verdict (mostly same-size
`DIFFER(NvN)` with one named site: a load swap, one scheduling slot, a register rename in one arm, a call to a forwarder instead of the
real getter). Two (0x0099F7A0, 0x009F18F0) carry made-up catalog names the lane identified from Ego_d (ROW.md, the note); name them only with a second source. `best_2026-11-18.cpp` is that lane's draft; its folder notes are `handback_*`. Start from the named site; do not redo the port.

### 68 `egod_open` rows: parked before Ego_d, within 0-2 bytes of retail's size
Every one has a draft within 2 bytes of retail's length (7 already the same size), a complete Ego_d locals record with at least two named
locals, and a parked residue class Ego_d speaks to (register-tie, stack-slot/frame, x87-order, block/arm-layout; others unclassified).
Nobody has looked at them with Ego_d. Regrade `best_2026-11-18.cpp` on the package headers, then rebuild it from Ego_d: the source's
locals in their scopes, the TU's callees, the landed TU-mates' includes. `docs/PARKED_INDEX.tsv` and `prior/` record what was tried.

## Since pass 23
Your pass-23 hand-back landed 103 rows (landing p23, `lessons/landings/2026-11-16_landing_p23.md`, `lessons/codex/pass23__*`): the
`hdr/PATCHES.json` header hand-back, START_HERE_CLAUDE.md, BOUNDS_FIX.tsv and per-row NAMING_EVIDENCE / PROBES were exactly right --
do the same again. The pass-22 rules (self-contained tests, every header change in `hdr/`, checked assert pins, `// Retail-asm:` lines
with a `0x` address, exact mangled `// Leaf:`) still hold.
- **asm rule:** a whole-function `naked` helper is NOT accepted (an amendment proposed on another branch on 2026-11-18 awaits the
  owner). `docs/ASM_RULES.md` in this package carries that proposed section; treat it as not in force.

## Header changes (NEW -- read before touching `include/`; details in docs/HANDBACK_RULES_FROM_LANDING_FY.md and docs/HEADER_MERGE_AND_PROOF.md)
Landing a thousand rows that were verified against old header copies cost days. What keeps your hand-back landable:
- **Additions only, behind a new `#ifdef FABLE_RETAIL_<CLASS>[_<FEATURE>]`**, made against the header in this package; the default
  path stays byte-for-byte unchanged. About 56,000 landed rows include these headers.
- **Hand back each change as (old text, new text) pairs or a small diff per header in `hdr/`**, never only as a whole-file copy. The
  maintainers' headers will have moved by the time you return; a whole-file copy cannot be merged safely.
- **Never reuse an existing opt-in name for different content, and never edit the body of an existing opt-in** (landed rows depend on
  it). To serve a second row from an existing line, extend its condition: `#if defined(A) || defined(B)`.
- **Do not move a `FABLE_STATIC_ASSERT`.** The macro names its typedef after `__LINE__`, and VC7.1's stack-slot order in some rows
  depends on those names (row 0x007E2A60 swaps two slots when they change). If your edit shifts lines above an assert, respell the
  assert `FABLE_STATIC_ASSERT_AT(<its old line number>, <expr>)` (include/rebuild_abi.h); `tools/lane_merge/pin_asserts.py <old include>
  <new include> <headers>` does it for you. The same effect can decide one of YOUR rows: if a draft differs only in two swapped
  stack slots, check whether an assert typedef name is in play.
- **Before handing back, re-verify every matched row against your final `include/` tree** (not the tree it first matched on), and list
  in `hdr/README.md` every opt-in you added, the row that needs it and the evidence.

## Levers from the long-rows lane (2026-11-11; details in `lessons/handbacks/2026-11-11_linux_handH40*__LESSONS.md` and `__NOTES.md`, the newest GOTCHAS lines)
- **Diagnose the inline split before shaping.** In these bodies most residue is "this CScriptThing dtor site was expanded in retail
  but called in ours" (or the reverse). VC7.1 decides per site from the callee's cost and the site's static frequency; frequencies
  come from loop nesting and branch shape. Tabulate retail's EXPANDED / CALLED pattern per site first, then find the loop / branch shape
  that reproduces it. The H40d notes show how to turn one trace into frequency bounds per callee (cost 44 below f 46.4, cost 60 below 38.1).
- **Loop shape decides frequencies.** `while (!T) { ...; NewScriptFrame(); }` and `for (; !T; NewScriptFrame()) { ... }` give different
  static frequencies when arms fall through to the loop end (0x00E50FD0), and identical ones in a straight-line body (0x00D0F640).
- **Loop-condition factor** (H40e, no code): c2 weights a `for` body 5x for a plain condition, 2.5x for `a && b`, 7.5x for `a || b`
  -- even when one operand folds away (`for (; !IsActiveThreadTerminating() && true; Game->NewScriptFrame())` halved every site and
  gave retail's ctor / dtor split at every site of 0x00E50FD0). A third code-free frequency lever beside loop form and branch shape.
  VC7.1 crashes (C1001) on a `for` condition with a temporary behind a folded `||`.
- **`#pragma inline_depth(0)` inside the body** around a single thread-exit return (scoped, with an `// Inline-budget:` note) when a
  site retail calls is expanded in ours and no shape changes its frequency (owner ladder step 3).
- **Cross-jumped returns.** `call T ; test al,al ; je <continue> ; jmp <exit>` marks a return that had its own cleanup copy at block
  layout time and was cross-jumped later; write the return in both arms (or keep its own scope) so it is not merged with its twins.
- **Named loop element re-ranks the allocator**: `const CScriptThing& follower = followers[i];` closed a loop-index / vector-base register
  swap at no instruction cost (0x00E6BC50). A named local can fix a register tie where nothing else moves it.
- **Predicate objects**: rows that build CPredicate* objects on the stack -- check whether retail stores the base vtable (0x012C2FF4,
  bytes `f4 2f 2c 01` in the oracle). Only when retail does NOT store it may a row define `FABLE_RETAIL_CPREDICATEBASE_NOVTABLE`.
- **Sibling bodies differ.** Two Mains with the same statements can split differently because the frequencies at the top differ:
  check a lever found on one against its sibling before believing it.

## Read first, in this order
-2. `egod/EGOD_FINDINGS.md`, `egod/LESSONS_EGOD.md`, `egod/BRIEF_TEMPLATE.md` (the brief the Ego_d lane's authors worked from) and
   `egod/LANDING_EGOD_README.md`; skim `egod/batches/` for your rows' addresses.
-1. `docs/HANDBACK_RULES_FROM_LANDING_FY.md` and `docs/HEADER_MERGE_AND_PROOF.md` (new: what makes a hand-back landable).
0. `docs/PROJECT_RULES_CLAUDE.md` (the project's own rule file) and `docs/HANDOFF.md` (today's state, one line per landing).
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
5. `lessons/` -- `lessons/codex/` (your own pass-15 to pass-22 lessons and status files, `pass22__*` and `pass22_after__*` for the newest, and the 2026-11-13 release list), `lessons/handbacks/`
   (every in-house and second-account hand-back since 2026-11-10: `<folder>__LESSONS.md` / `__NOTES.md`; the newest folders `handQ*`, `handWF*`,
   `acctB_*` first), `lessons/landings/` (every landing README since 2026-11-11: which lever landed which row; `landing_fy` and `landing_fz`
   are the header lessons), `lessons/naming/` (the naming passes) and `lessons/acctB_parked_notes/` (dossiers of large rows).
6. `docs/INLINE_DEPTH_REVISIT.md`, `docs/ASM_RULES.md`, `docs/VC71_CODEGEN_RESEARCH.md`, `docs/REGISTER_SWAP_SUMMARY.md`,
   `docs/SYMBOLS.md`, `docs/NAMING_STRAGGLERS.md`, `docs/QFE4035_COMPILER_GATE.md` as needed.
7. Per row: `rows/<addr>/ROW.md` (extent, today's residue, callers with the landed function each sits in, neighbours, earlier drafts).

## Older levers that matched rows (details and evidence in GOTCHAS / the playbook)
- **2026-11-10 in-house levers** (chunks 1013c-1016, SND2): a scoped coordinate local filled member by member puts a store between
  two `_ftol` calls (0x0063AB90); `/d2inlS<N>` is the size-mode inline budget (STLport resize(n) costs 39 / 29: 13 TransferVectorOfSubComponents
  rows on one shared body); an implicit `const char*` -> CCharString argument builds the temporary before other pushes; running `+=`
  statements give retail's operand order for `a*x + b*y + c*z`; `(~flags & mask) == 0` gives `not ; test`; `if (c) F(a, x); else F(0, x);`
  instead of `F(c ? a : 0, x)`; a scaled value in a named float local, not written back into the parameter; private FableWin accessors
  (`GetList(type).Field = ...`) keep retail's store order; a visible `__declspec(noinline)` callee body tells c2 nothing is captured.
- **Your pass-19 levers** (`lessons/codex_pass19_LESSONS.md` / `FINDINGS.md`): full genuine helper chains and original locals, PDB-qualified
  `volatile` members (CFileInstaller), retail float literals, grouped vector expressions, original type selection.
- **In-house naming levers** (`lessons/naming_agentNF5_LESSONS.md`): STLport members are named from STLport 4.6, not the VS2010-era PDBs;
  a tail jump forwarding a stack argument needs a named local (0x005BF2ED); a ctor that calls the grandparent ctor means the middle
  ctor was inline in that TU (0x00F336F0).
- **Your pass-18 levers** (`lessons/codex_pass18_LESSONS.md`): original helper chains (GetTC, CIntelligentPointer), grouped dot products, original local names / declaration order.
- **Your pass-17 levers** (`lessons/codex_pass17_LESSONS.md`): a real matrix-array destination for store dependencies (0x007767A0);
  full genuine helper visibility removing false alias dependencies (0x004C1060); original destructor composition with the real
  container bases (0x005954AD); the smallest positive `/d2inlT1` for a genuine inline-budget residue (0x00632830).
- **Today's in-house levers** (chunks 1012/1013): real PDB signatures on shared headers (`const std::set<CCharString>&`, `ELoadType`)
  matched three CWorldMap loaders on the first grade; `C3DVector d; d = *p;` (implicit operator=) copies through integer registers
  where `C3DVector d = *p;` does not; the order two locals are declared decides which dead argument slot each temporary is homed in.
- **Your pass-16 levers** (`lessons/codex_pass16_LESSONS.md`): branch-local result values that keep a common return conversion
  (0x006509F8); function-scope lifetime of an initialised local with deletion after both arms (0x00712900); the complete visible STLport
  `_Rb_tree::_M_insert` body behind FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (0x008C7280).
- **Today's in-house levers**: same-TU GetDef<T> / GetSubDef<T> bodies under `auto_inline(off)`; writing a call in both arms of a test
  to get retail's per-branch pushes; `partLists[i][j]` instead of a reference local (count-up loop); a visible STLport `_List_base`
  ctor body (allocator temporary placement); a row that calls a shared retail-asm helper keeps its own `// Retail-asm:` line.
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
- Pass a row's compiler flags as a `// Flags:` line only (one line); the maintainers turn it into a list for the lander.
- The label is a HINT and is wrong more often than not. Settle identity first: callers (`tools/xref.py` + ROW.md), vtable slots
  (`names/rtti-vtables.tsv`), the PDB name sets by shape and by the neighbours' proven names.
- Names right the first time: the exact PDB spelling proven by two sources. The PDB name lists (`names/egor_pdb_names.tsv`,
  `fablewin_pdb_names.tsv`, `egod_pdb_names.tsv`) are name SETS keyed by the debug builds' own addresses: never look a retail address up in
  them directly; match by name, donor order and shape. One retail address = one mangled name.
- Before declaring ANY callee or global, look its retail address up in `names/reloc_bindings.tsv` (symbol, target, caller) and use that
  exact spelling (class key from `names/pdb_class_keys.tsv`, const-ness, access, calling convention). If you prove a bound spelling
  wrong, use the PDB spelling and add a NAME_FIXES row. Canonical globals: `names/ESTABLISHED_SPELLINGS.tsv`. Spellings already proven
  wrong: `names/NAME_FIXES_QUEUE.tsv` column 5 -- never reintroduce one. Landed names: `names/catalog_names.tsv`.
- A name found nowhere after a real search gets ONE accurate descriptive name per the owner's made-up-name rule above, recorded as
  "descriptive" in NAMING_EVIDENCE.md with the sources searched. No invented "retail" / "native" stand-in classes.
- Wide text is `unsigned short`.

## Headers (correct them while you work)
- Model `this` with `#include "engine/<Class>.h"` from `include/` (PDB-derived; real member names and offsets). A local model only where
  the header provably differs from retail; say why in a `// Layout:` comment.
- Where a header is wrong for retail, fix the header, not the row: add an opt-in branch `#ifdef FABLE_RETAIL_<CLASS>` (or a narrower
  `FABLE_RETAIL_<CLASS>_<FEATURE>`) with the retail model, leaving the default branch byte-for-byte unchanged, and a
  `// Retail opt-in (Codex pass 24): <evidence>` comment. Your row `#define`s the macro before the include. Hundreds of landed rows
  include these headers, so the default path must not change. Check for an existing `FABLE_RETAIL_` branch first and reuse it (one class,
  one retail model). Return each changed header under `hdr/<same path>` with `hdr/README.md`.
- A local model may not drop a member retail had: unmodelled members are opaque storage at their retail offset and size.

## Inline devices (owner ladder, EXTRA_RULES)
Only for a residue that is purely the inline budget (which callee bodies VC7.1 expands): first plain C++, then a per-row `/d2inlS<N>` /
`/d2inlT<N>` flag, then a scoped `#pragma inline_depth(N)` ... `#pragma inline_depth()`, with a line-2 `// Inline-budget:` note. Never
for registers, layout or x87 order. `FABLE_FORCEINLINE` (include/fable_compiler.h), never a bare `__forceinline`.

## Hard rules
1. No `__asm`, `naked`, `_emit` or `register` anywhere (comments and identifiers included); `volatile` only per the rule above; no new `#pragma pack`; no
   device retail never had. MSVC-only spellings inside `#ifdef _MSC_VER`. C++03 that also compiles with g++ / clang.
2. One self-contained translation unit per row: `rows/<addr>/<addr>.cpp`. Line 1: `// <Class::Function(params)> -- <what the game does>`.
   A `// Leaf: <exact mangled symbol>` line (confirm it in your object's symbol table), a `// Retail:` line (address, length, convention),
   exactly one `// Flags:` line if flags beyond `/O2 /Oy` are needed, and comments that say what the code does and which retail address
   any unusual spelling reproduces.
3. A behaviour test `rows/<addr>/<addr>_test.cpp`: standalone C++03, stubs for callees, exercises the function's own logic, prints
   `<addr>_TEST PASS`. It must build ALONE (include/ only, no test_include/) under VC7.1 `/Od /W3` WITHOUT `/EHsc` (guard any throwing case with
   `#if !defined(_MSC_VER) || defined(_CPPUNWIND)`) and 64-bit g++ `-std=c++03` (where `long` is 8 bytes).
4. If a row's extent is wrong or it is a mid-function fragment, report the true function and extent instead of authoring the fragment.
5. No retail bytes, game files, binaries or links in what you return.

## How you grade
Cut the retail bytes from your own exe (`python3 tools/cut_row.py Fable.exe 0x<addr> <length> --dis`), compile your TU with
`cl /nologo /c /O2 /Oy /W3` (+ the row's flags; include path = `include/`), and compare the function's bytes with relocated operands
masked (call targets, global / vtable / string addresses). Then check every relocation names the retail address's one symbol
(`names/reloc_bindings.tsv`). MATCH = identical bytes; RELOCATION_MATCH = identical except relocated operands that all name the right
symbols. Jump-table entries are relocations too. The separate tools zip from passes 15-22 (unchanged) has the maintainers' grading and probing helpers; `tools/cut_row.py` and `tools/xref.py` are in this package.

## Return
A zip with:
- `rows/<addr>/{<addr>.cpp, <addr>_test.cpp, PROBES.md, NAMING_EVIDENCE.md}` for every row (an unmatched row keeps its best attempt as
  `<addr>.cpp` with line 2 `// Grade: DIFFER(<ours>v<retail>) -- <residue>`),
- `hdr/<path>` + `hdr/README.md` for every corrected or new header,
- `STATUS.tsv` (address, status VERIFIED_MATCH | RELOCATION_MATCH | PARKED | EXTENT_WRONG, attempts, assigned_name (mangled),
  retail_size, object_size, flags, lever, residue, test_status),
- `NAME_FIXES.tsv` (address 0x+8 hex, correct_symbol, evidence, wrong_spellings_seen) and `LABELS.tsv` (address, name, evidence),
- `LESSONS.md` (one line per idea that unlocked a row, GOTCHAS style, with the retail address), `SHA256SUMS`.
The maintainers regrade every row with the real compiler and their verifier, check every bound symbol against the PDB name sets, and land
MATCH / RELOCATION_MATCH rows with passing tests.

## Data in this package (all current as of this hand-out, repository commit of 2026-11-18)
- `include/` -- every shared header at HEAD (landing egod's 86 merged Ego_d-lane headers included).
- `egod/` -- the Ego_d findings, the lane's brief, lessons, ledger (`lane_STATUS.tsv`), lists and every batch's notes.
- `names/` -- `reloc_bindings.tsv` (symbol, retail target, first caller), `ESTABLISHED_SPELLINGS.tsv`, `NAME_FIXES_QUEUE.tsv` (spellings
  proven wrong), `catalog_names.tsv` (every landed address, name, length) / `manifest_names.tsv`, `landed_sources.tsv` (landed address ->
  source file and flags), the PDB name sets (`egor_` / `fablewin_` / `egod_pdb_names.tsv`, `egod_pdb_func_names.tsv`),
  `struct_layouts_egor.tsv` + `fable_types.h` (PDB types), `pdb_class_keys.tsv`, `rtti-vtables.tsv`, `fse_api_manifest.json` (933 reversed
  script API functions), the body-signature sets `Fable.exe.signatures.tsv` / `ego_r.exe.signatures.tsv` (hashes only),
  `BOUNDS_FIX.tsv` (extent corrections) and every label file in `names/labels/`.
- `docs/` -- the rules, the compiler research and every document on fixes that made rows match (GOTCHAS, MATCHING_PLAYBOOK,
  C2_ALLOCATOR_PROBE, VC71_CODEGEN_RESEARCH, REGISTER_SWAP_SUMMARY, INLINE_DEPTH_REVISIT, DECOMP_ACCELERATORS, METHODOLOGY), plus
  `UNMATCHED_ROWS.tsv` and `PARKED_INDEX.tsv` (the recorded residue and diagnosis of every parked row -- look your rows up in it).
- `lessons/` -- hand-back lessons and notes, landing READMEs, naming passes, your own returns (see "Read first" 5).
- `rows/<addr>/` -- ROW.md, `best_2026-11-18.cpp`, `egod_locals.txt`, `egod_tu.txt`, the draft folder's notes (`handback_*`), every
  earlier file (`prior/`), the landed neighbours' sources.
- `tools/` -- `cut_row.py`, `xref.py` (set `FABLE_EXE` to your own exe), `egod_locals.py`, `egod_type.py`, `lane_merge/pin_asserts.py` and `equiv_check.py`.
No retail bytes are in this package.
