# pragma3 (2026-11-03): third pass over the 70 `// Revisit: inline-depth` rows (coordinator-requested deliverable)

Read-only study; nothing landed/committed. Grades on a full mirror of rebuild/include at d0f753026 (`mirror/`). Row copies:
`rows/<addr>/` (`landed.cpp`, `plain.cpp` = pragma-free). Per-row table `summary.tsv`; finalised matches `out/`; parked best drafts `parked/`.

## Result
5 rows RELOCATION_MATCH pragma-free; `vland.py --recheck-landed` RELOCATION_MATCH + PASS for all five (`vland/r1/vland.log` WINS 4,
`vland/r2/vland.log` WINS 1):
0x00E504C0 WatchForBarberLadyDeath, 0x00E6AF20 / 0x00EDCB80 WatchForAcolyteHit (twins) -- loop-head macro (lever "r");
0x00D99A30 CheckIfGotTreasure -- loop-head macro + the same do-while(0) on the check after the inner region-wait loop.
These four: line 2 `// Inline-split: exits wrapped individually; retail's construct is unproven`, Shape updated, forced dtor now
`inline CScriptThing::~CScriptThing() {}`. Tests of 0x00E504C0 / 0x00E6AF20 still copy a `#pragma inline_depth` line: drop at landing.
0x00EF5200 CPersonalScript_EmptyScript::CheckForHeroActions -- natural plain `return;` (section 7); marker dropped, no wrapping, no
Inline-split note needed.
65 parked, each with >= 11 genuinely different graded attempts (~6,700 variants overall; `allres.json`, `g_*.txt`; per-variant mask dirs deleted to keep the scratch lean).

## 1. Loop-head macro = hot loop exit (new lever; `t4.py` flag `r`)
A function-final `while (!IsActiveThreadTerminating()) { ... }` rewritten `for (;;) { do { if (IsActiveThreadTerminating()) return; } while (0); ... }`
(same semantics: only scope dtors follow). c2 counts the do-while(0) as an inner loop, so the loop's own exit becomes a hot inner-loop exit and
expands the destructor; the other in-loop returns stay cold and call 0x004AA840 -- retail's split.
Evidence (marker build 0x00E504C0, `mk.py` + `mkseq.sh`, /d2inlT diagnostic only): T* wait-loop return 60-70, post-wait return 80-90,
killed return and loop exit 120-150. Retail needs the loop exit expanded and post-wait + killed called: impossible with wrap masks
(they only heat returns). Of six loop forms (for-break, do-break, do-while, flag loop, while(true)+break, goto out of do-while(0)) only
those whose exit leaves a do-while(0) give A:I B:C D:I C:C. Applies to 9 rows: 3 match outright, 0x00D99A30 with one more wrapped
check (masks 000001 / 010001 on top of r); 0x00CE0600, 0x00E0C4C0, 0x00E32080, 0x00EF12F0, 0x00EFAB40 do not.

## 2. "Exits of one loop share equally" is not general
Marker builds: in 0x00E53D90 plain, the exits of one for(;;) score T* loop-head 75-90, attacked-block return 75-90, first !alive return
90-110, second !alive return and break/end >150 -- position/structure dependent. Measure (mkseq.sh) instead of assuming equal shares.
In 0x00E53D90 the exit retail calls is among the two hottest; hence the dead-branch rows (0x00D97170, 0x00E53D90, 0x00E36390,
0x00F09950) stay 3 bytes short under every mask (3-level exhaustive 81 variants, while-conversion + masks, class-uniform).

## 3. Nested special-member family -- the main unsolved blocker (>= 16 rows)
Retail calls CScriptThing's copy 0x004ABE90 / dtor 0x004AA840 out of line INSIDE the inline-expanded special members of classes holding
a CScriptThing member (CIsThingAlive/CIsThingConscious filter ctor/dtor at SetPredicate temporaries; CSpawnedFunc1<T,CScriptThing> ctor
Par1 copy + by-value par1 dtor; CSpawnedFunc1 deleting dtor 0x00D172E0), while expanding them at ordinary sites of the same function/TU
(by-value argument copies, GetThingWithScriptName temporaries, SuspendableProcess 0x00D16780). Our VC7.1 expands them inside the
ctor/dtor too (except the compiler-generated ??_G of CSpawnedFunc1, which calls ??1CScriptThing in ours as well, = retail 0x00D172E0).
The out-of-line ~CIsThingAlive 0x00E241B0 (COMDAT from another TU) inlines its member dtor: TU/context dependent, not a class property.
Diagnostic (never a lever): plain builds at /d2inlT55..65 match 9 rows exactly (`g_T.txt`): 0x00D701C0, 0x00D99A30, 0x00DA4E30,
0x00DA7D30, 0x00E05360, 0x00E12580, 0x00E386E0, 0x00EF12F0, 0x00F045E0 -- the only flipping site is the nested member site, which scores
57..74 in our model; retail's effective cost there was ~1.15-1.3x ours, every other site already agrees.
Tried, no effect (`g_sf.txt`, `probes/sf/p1..p5.cpp`, `rows/00e386e0/{idtor,icdtor}.cpp`): CSpawnedFunc1 ctor plain / forceinline /
out-of-class / const by-value param (overlays xh/sfplain, xh/sfout, xh/sfconst); CCountedPointer copy plain; implicit CScriptThing copy
(FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY); implicit CScriptThing dtor (xh/idtor); copy/dtor in-class (xh/icc, xh/icdtor); copy after
the caller / non-inline; definition order before/after template and caller (templates and plain classes); functor into a local pointer;
implicit / virtual filter dtor; /Ob1, /Os, /Ox, /Z7, /Zi, /Zd; c2 knobs inluserinl, inlS, inld, inlld, inlcrmax, inlfcsa, inlocsa*,
inlnobr, inlfcsw (no grade changed). Open question: what makes c2 cost a CScriptThing special member ~20% higher on a member subobject
inside an inlined special member (per-site argument savings 0x1078d03c? c1's member-init/member-dtor call emission?). c2's `INL:` debug
prints are POGO-only (-pgi/-pgo globals 0x10883334/0x10883338), not usable to trace it.

## 4. ~CTimer cold call (0x00CEF3B0) is not frequency-driven
~CTimer is tiny: only two sites stay calls even at /d2inlT1, every site expands at T>=25 (`tseq.sh`). Retail's single out-of-line
~CTimer at the in-branch return cannot come from the score. 243 three-level masks + 23 uniform variants: no.

## 5. UI Pointer rows (0x005518E0, 0x00551BC0)
At /d2inlT50 ours calls the ENTRY-site Pointer(Component*) and expands the in-loop one -- the opposite of retail. The loop site is hotter,
so retail's call in the loop is not frequency-driven; consistent with family 3 if retail built the child pointer inside an inline helper
(unproven). 0x00429C15 is /O1-style (pop ecx, `and [mem],0`): the linker took that COMDAT from a size-optimised TU.

## 7. Natural plain return beats the landed goto device (0x00EF5200 match)
The landed 0x00EF5200 left the vector's block by `goto terminating` to `if (false) { terminating: return; }` (depth-0 region, b18 recipe).
Pragma-free, that goto form grades DIFFER(610v592); the plain `if (IsActiveThreadTerminating()) return;` inside the vector block, with
both dtors plain in-class inline, is RELOCATION_MATCH: c2 rates that exit the coldest, so the vector's free expands there but the hero's
~CScriptThing is called (0x004AA840) -- exactly retail's split. The goto device was an artefact of reaching the split with pragmas; the
natural construct needed nothing. Lesson: before searching masks, undo every goto / `if (false)` / shared-label device the pragma
recipe introduced. Only one other row had such a device: 0x00CE1280 (shared `ool:` label); goto-free it grades 952v808 and all 1024
two-level masks give the identical grade (wrapping only heats sites, and retail needs its 4 inner-arm exits COLDER: they share one
out-of-line 0x008AC970 block at 0x00CE13E9, tail-merged); conv/r forms and 4 inner-arm constructs (else-return, continue, else-if, ?:)
no better -- parked.

## 6. c2.dll knob defaults (0x107085e4, non-POGO)
inlS 9, inlT 74, fcsw 30, flcsw 0, ocsa1 45, ocsa4 45, userinl 7, 0x10881964 = -4, nobr 10, ld 3, d 7, niln 1, nild 2, noln 2,
nold 3, crmax 10, fcsa 5. Decision 0x1078d1aa: hard reject when callee > inlS and caller size + growth counter 0x10882b58 > 9000
(the counter is reset per function at 0x1077ca2e, so no TU-wide budget); early accept for callee flag 0x20 when cost/3 (1 call
site) or cost*(1-1/2n) (n sites) < inlS+5; hot-site discounts x niln/nild (f > 7.05) and x noln/nold (f > 5).

## Tooling notes
- The earlier t3.py/t2.py de-force regex turned out-of-class `__forceinline X::~X()` into a NON-inline definition (strong external
  symbol). t4.py writes `inline X::~X()`; regraded, plain grades unchanged except the `__forceinline virtual` rows 0x00E39140 / 0x00F08590.
- qq.py kept one work dir per address: parallel variants of one row overwrote each other's .obj (SRC_FAIL / OBJDUMP_ERR / wrong grade).
  Now per variant (QQTAG or the source stem).

## Files
qq.py/qq.sh (grader, --calls --dis --flags, MIR= overlay chain); t4.py (transform: classes H/W/S, `2` nested, `m<mask>` 0/1/2, `c`
for->while, `r` while->for+loop-head macro); ex3.py (exhaustive masks --levels/--conv/--r); sm.py (single/leave-one-out masks);
genu.sh/gradev.sh (uniform sweep); mk.py, mkseq.sh, tseq.sh, tsweep.sh, tall.sh (marker / threshold diagnostics); td.py (function-final
do-while(!term) rewrite); sfgen.py (CSpawnedFunc1 variants); fin.py (finaliser); mkpayload.py; rx.py/rdis.py/odis.sh; allres.json;
g_*.txt logs; probes/sf/; xh/ (experiment-only overlays, not proposed: no header change is needed by the 4 matches).
