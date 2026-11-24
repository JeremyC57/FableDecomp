# 0x00E93EB0 NScript::CV_HauntedHouseScript::Main -- agent B24A3 notes (2026-11-24)
- Baseline (Codex 2026-11-13 draft = v0.cpp): DIFFER(5173v5321) 59 hunks, 148 bytes short. ../gx.sh <addr> <leaf> <src> "<note>" grades.
- Callee map (0xCB7940 = IsActiveThreadTerminating, 0x99A2E0 = ~CBaseClass, 0xBFE9BC = operator delete, 0xBFEA14 = free,
  0x4AA840 = ~CScriptThing, 0x8AC970 = vector<CScriptThing> dtor, 0xCD1FB1 = vector<CScriptThing>::operator[] (called once, last loop),
  0xE95380 = `ret` stub = TRetVoid::Return called once). Retail v ours: delete 25 v 23, ~CBaseClass 22 v 20, free 7 v 6,
  ~CScriptThing 6 v 5, operator[] called 1 v 0, rets 3 v 2. => retail expands two more ~CScriptThing and one more vector dealloc,
  and keeps a third exit (ret 0xE9530B).
- Retail exits: cellar loop in-loop T (0xE950C1 -> 0xE950E7) everything expanded, ret 0xE951CF; cellar after-loop T (0xE950CC):
  vector dtor CALLED + three ~CScriptThing CALLED (shared tail 0xE95353, ret 0xE95378), Return inlined; final T (0xE9523A):
  Return CALLED (0xE95380), vector expanded, Forcefield2/Forcefield1 expanded, FXBarrier called (ret 0xE9530B); normal end:
  vector expanded, three ~CScriptThing called (0xE95328 -> 0xE95353).
- Frame: retail FXBarrier 0x28 / Forcefield1 0x1C / Forcefield2 0x10; ours FXBarrier and Forcefield1 swapped (first hunk).
- Draft devices to remove before landing: `CBaseClass::CBaseClass() {}` strong out-of-line definition under auto_inline(off) (a second
  definition of landed 0x0099A2D0: not allowed) and the pointer-to-member final Return call.
- Note: ../gx.sh (generic grader) sits at the b24a level, outside any row dir (B24A3).

## B24A4 (2026-11-24)
- v1.cpp = v0 minus the strong `CBaseClass::CBaseClass() {}` (auto_inline(off)) definition: byte-identical grade DIFFER(5173v5321)
  59 hunks -> dropped for good (differ/ updated). v2 = v1 + plain `return _Fail_Val.Return();` for the final check instead of the
  pointer-to-member call: 5145v5321, 88 hunks (retail CALLS Return 0xE95380 at that exit; the device is still what gives the call).
- c2 trace (inl/v1.txt, 468 decisions): at the end of Main the caller-size penalty is live (pen 27..36): ~CScriptThing sc 72/72/73
  expanded, the last three sc 74 called -- the end-of-function split sits ON the T=74 threshold, so it is decided by Main's running
  IL size (g) and the sites' f (3.0762). Retail: +2 ~CScriptThing expanded, +1 vector free, operator[] CALLED in the last candle loop,
  Return CALLED at the final check -> retail's running size differs from ours before the end; find the upstream IL size difference
  (e.g. GetPos / IsAlive / IsDead bodies, the three `if (!false)` blocks, CreateEffect argument temporaries) before tuning the end.
- Slot probe: declaring Forcefield1 before FXBarrier (v3) fixes the middle uses (hunks 100..488) but not the ctor order nor 601+ (42
  hunks): the slot pair is weight-driven, not declaration-driven; keep the v1 order (retail constructs 0x28, 0x1C, 0x10 in that order).
- Next: compare the decision list with retail's expansion pattern site by site (retail callee counts in the B24A3 section), and look
  for a source shape that lowers Main's IL size before the cellar section by ~2..6 tuples per site.

## B24A5 (2026-11-24) -- ROOT CAUSE FOUND: c2 compile order (budget path), not source text
- Lessons applied (read first): c2 has two inline rules (pending/2026-11-12_acctB_f4004: cost rule for callees already compiled, BUDGET
  rule c2.dll 0x107284A3 for callees not yet compiled: reject once the running total [0x10882B5C] > 35000, else accept w <= 40 / w <= lim);
  FINDINGS 2026-11-21: "budget path" = callee compiled AFTER its caller (c2 compile order); 0x00E20600 is the same family (escalated).
- Empty-callee fact: `#pragma inline_depth(0)` round `_Fail_Val.Return();` alone (v5: 5145, = plain v2) does NOT give the call: c2 drops a
  call to a callee it has compiled and knows is empty (frame shrinks 0xcc -> 0xc8, _Fail_Val gone). So NO call-site control (pragma /
  flag) can emit retail's `call 0x00E95380`: retail's c2 had not compiled TRetVoid::Return when it compiled Main.
- Diagnostic (inl/inlf.py + inl/inlf_host.py + inl/anf.py, FORCE regex sends matching callees down the budget path by setting esi=0 at
  c2 0x1078CF85; NOT a source): FORCE='Return@TRetVoid|??A vector<CScriptThing>' on v2.cpp (inl/f1.txt): every Return / operator[] site
  accepted while the total grows (3270 ... 34701 at the last candle loop's terminate Return, decision 423), then operator[] in the LAST
  candle loop (tot 35072 > 35000) REJECTED and the final terminate Return REJECTED -- exactly retail's two odd calls (0x00E9520A
  operator[] 0xCD1FB1, 0x00E9524E Return 0xE95380) and nothing else. So retail compiled Main BEFORE TRetVoid::Return and
  vector<CScriptThing>::operator[] (both first compiled after this Main in register_scripts.obj; 0x00E95380 has exactly one caller in the
  whole exe, i.e. this is the only place any c2 run ever called Return), while ~CScriptThing / ~vector / CCountedPointer / operator= etc.
  were compiled before Main (their splits are frequency/heuristic splits).
- Probe pr/p1.cpp (+ pr/hdr ctor decl; probe only): Main `inline` + vtable emitted by an inline ctor -> Main is vtable-reached and
  compiled first, but then EVERY callee takes the budget path (443 budget decisions) -- not retail. Retail needs a TU where other,
  earlier-compiled functions need the dtors/accessors but not Return / operator[] (the unity register_scripts.obj, Ego_r name stream
  shows all scripts in one obj). Not expressible in a one-row TU without a stand-in function -> ESCALATE.md.
- Second residue that remains even with the budget pair: final-T FXBarrier ~CScriptThing sc 73 (retail calls: needs 74) and the normal
  end's first ~CScriptThing sc 73 (retail calls): retail's caller growth g is ~220+ tuples larger at the end (one pen unit). Unexplained.
- Grades this session: v5 (depth0 round Return call only) 5145v5321 88 hunks; v6 (+depth0 round last-loop EntitySetLightAsOn) 5258v5321.
  Best stays v1 (= Codex pmf device) 5173v5321 59 hunks.
- B24A5 follow-up: v7 (= v1 minus post-cellar pragma) trace: post-cellar ~vector sc 73 (retail calls -> pragma = same missing pen unit).
  Diagnostic v7 + FORCE operator[] + BUMP 200/250 on caller growth: DIFFER(5333v5321), slot swap gone, all exits = retail, residue only
  in the last candle loop's registers (see ESCALATE.md). BUMP 300 breaks (4981). Diagnostic-only files: pr/ (probe p1 + pr/hdr ctor
  decl), inl/inlf.py, inl/inlf_host.py, inl/anf.py, inl/og.py (grade a prebuilt obj), inl/*.json/*.obj -- never sources for landing.
- Best landable-shaped draft unchanged: differ/00e93eb0.cpp (= v1, Codex pmf device + post-cellar pragma), DIFFER(5173v5321) 59 hunks.
