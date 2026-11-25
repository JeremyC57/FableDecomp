# NF427 round 2 (2026-11-25): behaviour tests ported onto the shared headers

r2/ supersedes batches/p2a..p2d for Part 2. Only the tests (and the evidence that depends on them) changed. Rows, headers (hdr/),
UNITS, TABLE_CHANGES, SELF_SYMBOLS, selfmap, expect and HEADERS are the same files as in round 1. `diff -rq batches/p2X r2/p2X`
lists only tests/*, CHECKS.tsv, evidence/name_gate.tsv and the new logs/. head/payload.json was rebuilt by recheck.py and is byte-identical.
r2/t1..t3 are the test-only batches for Task B.

## Test form (both tasks): no class body in any test
- The test copies the row's prelude verbatim (its `#define`s, including the class's `FABLE_RETAIL_<C>_VIRTUAL_MODEL` opt-in and its
  Needs, and its `#include`s). So the base class comes from `engine/<Base>.h`, and where the row includes `engine/<C>.h`, C comes from
  that header.
- Members the test must supply, or must call without a class body, go through `extern "C" __fastcall` stand-ins mapped by
  `/alternatename`. A __fastcall function (ecx, edx, stack) has the __thiscall frame: ecx is this, edx is unused, and the callee
  pops the stack arguments. The stand-ins are:
  - the callee constructors and assignments: `??0<Base>@@QAE@ABV0@@Z` → `@nf427_base_copy@12` / `@nf427_copy_ctor@12`, and
    `??4<C>@@QAEAAV0@ABV0@@Z` → `@nf427_assign@12`. Task B takes these mangled names from the OLD test object's symbol table.
  - the row's own function (the `// Leaf:` symbol): `@nf427_clone@4` / `@nf427_copy@12` → leaf. This reaches private (EBE) and
    protected Clone without a friend.
- Kept from the old tests: the slot `/alternatename` list, every check, the retail size and offset literals, and the
  printf / pass lines. Every HEAD pass_pattern is asserted present in the new text.
- vland links the test with the row object: its self-contained link fails on the unresolved leaf, and it falls back to `sobj + tobj`.

## Task A: Part 2 (108 tests; tools425/gen_test427.py)
- p2a (50) and p2b (13) use the header class. `sizeof(C)` is the allocation check, with a compile-time
  `sizeof(C) == <retail operator new size>` check. Own members are checked at the retail offsets, plus `offsetof(C, m)` against
  `engine/<C>.h`. This applies to the 8 rows that have own members (p2a 6, p2b 2).
- p2c (38, q1) and p2d (7, q2): C is the row's own class, so the test names it only through its Clone symbol and the retail size.
  Its base comes from its header. The 7 rows with own members (p2c 5, p2d 2) check them at the retail offsets only.
- vland, round 1 vs round 2, per address:
  - HEAD and new are identical: 108/108 RELOCATION_MATCH PASS with the new tests.
  - The mutation control (`return new C(*this);` → `return 0;`) gives 108/108 FAIL (DIFFER 3vN).
- `name_gate.py --test`:
  - p2a 50/50 and p2b 13/13 print NAME_GATE PASS.
  - p2c 38/38 print FAIL (1). The one failure is the row's LOCAL_CLASS (q1).
  - p2d 7/7 print FAIL (1). The one failure is SHARED_HEADER_AVAILABLE (row) (q2).
  - Test-side SHARED_HEADER_AVAILABLE: 0 (round 1: 2 per row in p2a, p2b and p2d, 1 per row in p2c).
- CHECKS.tsv: head_grade, new_grade and mutant come from the round 2 logs (r2/<b>/logs), plus test_r2 / name_gate_r2 columns. The
  other columns are the rows' and are unchanged.

## Task B: 226 landed NF425 tests (221 introduced + the 5 pre_existing), tools425/gen_tb427.py
- Locks: all 226 were missing, so they are now locked NF427. Claims: 0 Account B, 1 Account A done. T3: 0 lines name them.
  (tools/screen427.py → work/tb/screen.tsv)
- Batches, by test shape:
  - t1: 77 Copy tests (operator= stand-in). This holds all 5 pre_existing tests.
  - t2: 74 Clone tests whose copy constructor is out of line (C's copy-ctor stand-in).
  - t3: 75 Clone tests whose copy constructor is inlined (base copy-ctor stand-in).
- Each batch has rows/ = orig/rows = the repo row (unchanged, asserted byte-equal) and orig/tests = the repo test. tests/ holds the new
  test. UNITS.tsv gives the repo paths and pass pattern, and head/payload.json is from recheck.py.
- The new payload is the HEAD payload with ONLY test_cpp replaced. Every other field is asserted equal.
- Include rule, per test (evidence/port.tsv col engine_header):
  - The row's prelude, plus `engine/<C>.h` when the test compiles with it under that prelude at /Od /W3 (`yes`).
  - `prelude`: the row already includes it.
  - `none`: no engine/<C>.h exists. This is 10 t3 rows; their local body is gone anyway.
  - `no-compile`: 9 rows. The header exists but fails under the row's prelude (work/tb/nocompile.tsv).
    - 6 t1 Def headers: cl did not finish in 120 s.
    - CBuyableHouseDef: C2440 CCharString.
    - CTavernGameDef: cl did not finish in 120 s.
    - CCreatureAction_DeclarePlayerACriminal: CIntelligentPointer not declared.
    - These 9 tests define no class body either. The header is simply not included; this is T3 header debt.
- No sizeof/offsetof checks against engine/<C>.h in Task B. These rows are built on local classes whose `// Header:` lines say the
  header lacks those members or that size, so such a check would test the header, not the row.
- vland: HEAD and new grades are identical for 226/226. t1 is 64 MATCH + 13 RELOCATION_MATCH; t2 and t3 are RELOCATION_MATCH. All
  are PASS with the new test.
- Mutation control (mut425.sh): Copy `other` → `this` gives 77/77 FAIL (DIFFER 9v5). Clone → `return 0;` gives 149/149 FAIL.
  RESULTS.tsv: 226 ACCEPT (test-only).
- `name_gate.py --test`: test-side SHARED_HEADER_AVAILABLE is 0 on all 226. The remaining FAILs are all row-side, on landed rows
  this round leaves unchanged (rows unchanged was the brief):
  - SHARED_HEADER_AVAILABLE (row): the landed rows define C locally although engine/<C>.h exists.
  - LOCAL_CLASS: t3, 10 rows with no engine/<C>.h.
  - one RELOC_NAME at 0045c2e3: `??4CCombatTypeDef@@QAEAAV0@ABV0@@Z` → 0x0045C2E8, which the Ghidra label calls 'CCombatTypeDef::Copy'.
  - The DEBT lines (operator new `??2@YAPAXI@Z`, agree_unverified at its address) are not FAILs.
  - These are listed in CHECKS.tsv name_gate_row_side.

- Overlay independence: every engine header a Task B test includes exists in rebuild/include (0 overlay-only, of 253 distinct
  includes).
  - 16 tests include a header that hdr/ also carries (an add-only copy holding NF427's VIRTUAL_MODEL branch; no Task B test
    defines those opt-ins).
  - Re-graded with FABLE_HDR_OVERLAY unset (repo headers only): 16/16 RELOCATION_MATCH PASS (work/vland_tbnov.log,
    tools425/runtb_nov427.sh).
  - So t1..t3 can land before or without the Part 1/2 headers.

## Locks
NF427 holds the 108 Part 2 locks, kept from round 1, and the 226 Task B locks taken this round.

## T3 (r2/T3.txt)
- The round 1 Part 2 test-debt line (T3.txt, last line) is resolved.
- 9 header lines are added, one per Task B test that cannot include its engine/<C>.h.
- 0045c2e3 RELOC_NAME is listed for review, not as a NAME_FIXES entry: it is not proven which name is wrong.

## Tools (round 2)
- tools425/gen_test427.py: Task A tests.
- tools425/survey_tb.py: Task B shape survey.
- tools425/gen_tb427.py: Task B port.
- tools425/runtb427.sh: Task B HEAD/new/mutant vland.
- tools425/runtb_nov427.sh: no-overlay regrade.
- tools425/fin_r2.py: CHECKS/RESULTS.

## Clean-up
- The work-only links batches/r2* and the batches/*/tests.r2 drafts are removed.
- Old round 1 tests are kept in work/r2_tests_r1/ (identical to batches/p2X/tests).
