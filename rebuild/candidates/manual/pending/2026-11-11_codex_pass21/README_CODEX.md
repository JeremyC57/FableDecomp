# FableDecomp -- Codex hand-off pass 21 (2026-11-11): the 40 longest parked rows open for an outside pass

## Who you are and what is asked
You are a professional decompiler of Microsoft Visual C++ 7.1 output (Visual Studio .NET 2003, `cl` 13.10.3077 RTM). For every row in
`ROWS.tsv` the goal is C++ that this compiler turns into the **exact retail bytes** of PC `Fable.exe` (Fable: The Lost Chapters, Steam,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`, ImageBase 0x400000), written the way the original Lionhead / Big Blue Box engineers
wrote it: their classes, member names, headers and idioms. Not a functional equivalent and not a stand-in: the retail source, as closely
as the evidence allows. The owner's words: "readable clean c++ that is portable and byte matches retail code and behaves the same as retail."

The project has 47,649 of the 49,568 catalogued retail functions landed as byte-matching C++ (96.13%; 69.30% of retail function
bytes). What is left is long: every row in this pass is 4,344-9,648 bytes.

Your pass 20 landed 34 of 40 rows -- all 34 are in the repo (landing batch `2026-11-11_landing_bu`). Merge notes:
- Your header overlay was merged three ways against the repo's own changes since the hand-out. Where both sides added a new model
  (CTCDParticleEmitter, CGameTimeManager), the repo model stays the default and yours is behind a new opt-in
  (`FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL`, `FABLE_RETAIL_CGAMETIMEMANAGER_FULL`) that your matched rows define. CTargetingDef and
  CInterpolationInfo kept the repo model (your rows matched on it). Overlapping opt-in conditions were joined with `||`
  (CTCGraphicAppearance, CTCStealth, CGameDefinitionManager).
- The 0x00C3DB80 test aborted: the maintainers' harness builds tests `/Od /W3` WITHOUT `/EHsc`, so a test that throws must guard that
  case with `#if !defined(_MSC_VER) || defined(_CPPUNWIND)`. Write tests that pass without exception handling.
- 10 of your LABELS.tsv names came from the leaf of a different overload / scope than the row's own; the manifest was fixed from the
  leaves. Return LABELS that equal the leaf you compiled.
- Your 176 name fixes are queued as `land1011bu`; your 5 extent corrections went into `names/BOUNDS_FIX.tsv`.
- Your six parks (0x004B2890, 0x0067E290, 0x007F6200, 0x00919240, 0x00A55F90, 0x00C3D470) are NOT in this pass.
Your pass-20 lessons, START_HERE, status and row details are in `lessons/codex_pass20_*`. Thank you.

Since pass 20 the in-house lanes worked the longest parked rows with a dedicated "long rows" agent (chunks H40-H40d: 14 rows of
4.3-9.6 KB matched, e.g. 0x00E58D40 at 9,616 bytes) and naming agents NF8-NF15 respelled about 1,000 landed rows to proven PDB names
(`lessons/naming_agentNF*`). `names/NAME_FIXES_QUEUE.tsv` lists every spelling proven wrong so far -- check it before you declare a callee.

The owner rules still stand and override older text anywhere in the package:
- **Made-up names**: "you can use a made up name only if you cant find original name anywhere, are consistent in the names usage, and
  the name is as accurate as you can make it and makes sense". Search every name source first (the PDB name sets, the exe's RTTI and
  strings, `names/fse_api_manifest.json`, `names/labels/`); then one descriptive name per address, used everywhere, saying what the
  body does -- no `VirtualNN_<addr>` / `member_<addr>` / `*_Unproven` placeholders -- marked "descriptive" in NAMING_EVIDENCE.md.
- **`volatile`**: "if retail used it and volatile doesnt cause portability issues, then it should be fine" (docs/ASM_RULES.md
  `volatile` section: PDB type records or vendor source, declared once in the shared header with a `// Retail-volatile:` line).
- **Inline devices** follow the owner ladder: plain C++, then a row flag `/d2inlS<N>` / `/d2inlT<N>`, then `#pragma inline_depth`,
  and `FABLE_FORCEINLINE` only when none of those works.
- **Wide text** is `unsigned short` (`G`), no `/Zc:wchar_t`.

## The rows
All 40 are **parked** (`ROWS.tsv` column `kind`): drafts exist, none matched. They are the 40 longest parked rows not currently
with an in-house agent (the 23 very longest are with the in-house long-rows agent right now and are not in this package):
- **30 quest-script coroutine `Main` bodies** (`NScript::CQ_*Script::C*::Main`, `NScript::CV_*Script::C*::Main`): long straight-line
  scripts of `CScriptThing` calls, `while (!...) { ...; NewScriptFrame(); }` waits, scoped CScriptThing / CCharString / vector
  temporaries, and many thread-exit returns. Retail quest-script behaviour is known: the Aeon Lua ports of these scripts are the
  maintainers' oracle (ask, or read the neighbours: dozens of sibling `Main` bodies of the same scripts are landed and are in
  `rows/<addr>/neighbours/`). The residue is almost never the statements -- it is c2's inline budget per call site (which
  CScriptThing / vector / CCharString ctor and dtor sites are expanded and which are called), register allocation in the loop
  tops, and how returns were cross-jumped into the common exit. See "Levers from the long-rows lane" below.
- **10 engine rows**: 0x00B0DC20 NParticleEngine::CPSCUpdateNormal::Update, 0x00AFCC30 CPSCRenderSprite::PreparePrimitives,
  0x00B13530 CPSCEmitterGeneric::DatabaseSerialise, 0x00804020 (player targeting update, label a hint), 0x00A1E5C0 (A* navigator move,
  label a hint), 0x00839940 CThingGameObject::OnHit, 0x005E4BE0 CTCQuestCompletionUI::GetQuestDescription, 0x00975BB0
  CAIStateGroup_JackOfBladesStage1::Update, 0x007C1000 CTCShotDefinitionBase::OnCollisionProper, 0x00B24850 CEngine::SetDefaultRenderStates.
  Labels marked "a hint" are not proven names: settle identity first.
- `residue_or_note` gives the last grade recorded in the best draft (its line 2) and the draft's path. **Regrade the best draft first**
  with today's headers: the headers moved a lot today and several drafts may be closer (or further) than recorded.
- **Extents**: four of the five rows the in-house agent left at its 10-grade budget (0x00D3E2E0, 0x00E2FF00, 0x00DBB310, 0x00EC8300)
  are here; their hand-back notes are in `handback_*`. 0x00D3E2E0's catalog length (9,648) and the length its oracle was cut at
  (9,440 + padding) disagree: check the true end of the function before grading and report it in STATUS / BOUNDS_FIX if it differs.
  Check every extent: an extent that runs into int3 padding or the next function is a wrong extent, not a residue.

A fresh idea is worth more here than more probes of the same idea: read what was tried and do something different.

## Levers from the long-rows lane (2026-11-11; details in `lessons/handH40*_LESSONS.md` and `handH40*_NOTES.md`, the newest GOTCHAS lines)
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
5. `lessons/` -- your own pass-15 to pass-20 lessons (`codex_pass1[5-9]_*.md`, `codex_pass20_*`), the newest in-house hand-backs
   (the long-rows lane `handH40*_LESSONS.md` / `handH40*_NOTES.md` FIRST -- it worked rows exactly like these -- then chunks 1020-1026), the landing
   READMEs of 2026-11-11 (which lever landed which row), and the naming passes (`naming_agentNF*`, `naming_codex20nf_2026-11-10.md`).
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
  `// Retail opt-in (Codex pass 21): <evidence>` comment. Your row `#define`s the macro before the include. Hundreds of landed rows
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
   `<addr>_TEST PASS`. It must build under VC7.1 `/Od /W3` WITHOUT `/EHsc` (guard any throwing case with
   `#if !defined(_MSC_VER) || defined(_CPPUNWIND)`) and 64-bit g++ `-std=c++03` (where `long` is 8 bytes).
4. If a row's extent is wrong or it is a mid-function fragment, report the true function and extent instead of authoring the fragment.
5. No retail bytes, game files, binaries or links in what you return.

## How you grade
Cut the retail bytes from your own exe (`python3 tools/cut_row.py Fable.exe 0x<addr> <length> --dis`), compile your TU with
`cl /nologo /c /O2 /Oy /W3` (+ the row's flags; include path = `include/`), and compare the function's bytes with relocated operands
masked (call targets, global / vtable / string addresses). Then check every relocation names the retail address's one symbol
(`names/reloc_bindings.tsv`). MATCH = identical bytes; RELOCATION_MATCH = identical except relocated operands that all name the right
symbols. Jump-table entries are relocations too. The separate tools zip from passes 15-20 (unchanged) has the maintainers' grading and probing helpers; `tools/cut_row.py` and `tools/xref.py` are in this package.

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

## Data in this package (all current as of this hand-out)
- `include/` -- every shared header at HEAD (your pass-20 overlay merged, plus today's in-house opt-ins).
- `names/` -- `reloc_bindings.tsv` (symbol, retail target, first caller), `ESTABLISHED_SPELLINGS.tsv`, `NAME_FIXES_QUEUE.tsv` (spellings
  proven wrong, incl. your pass-18 / 19 / 20 fixes queued as land1011aq / land1011bb / land1011bu), `catalog_names.tsv` / `manifest_names.tsv`, the PDB name sets
  (`egor_` / `fablewin_` / `egod_pdb_names.tsv`, `egod_pdb_func_names.tsv`), `struct_layouts_egor.tsv` + `fable_types.h` (PDB types),
  `pdb_class_keys.tsv`, `rtti-vtables.tsv`, `fse_api_manifest.json` (933 reversed script API functions), the body-signature sets
  `Fable.exe.signatures.tsv` / `ego_r.exe.signatures.tsv` (hashes only: match a retail body to its Ego_r donor by shape),
  `BOUNDS_FIX.tsv` (extent corrections) and every label file in `names/labels/`.
