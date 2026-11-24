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
