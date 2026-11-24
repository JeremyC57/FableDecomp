# 0x00E20600 CTraderToEscort::Main -- escalation (agent B24A, 2026-11-24)

Residue: the predicate prefix only (everything after it is ebp-push / frame-slot / padding fallout). Retail:
  temporaries CIsThingAlive(Me), CIsThingAlive(hero): ~CIsThingAlive EXPANDED in Main, its ~CScriptThing CALLED (0x004AA840), its
  ~CPredicateBase expanded (store 0x012C2FF4); the And dtor expanded, its two members' ~CIsThingAlive CALLED (0x00E241B0).
  The out-of-line owner 0x00E241B0 (landed) EXPANDS ~CScriptThing fully.

Measured with the c2 inline trace (agent3009 tool, $W/inl/, nat.json / szbase.json):
- VC7.1 c2 compiles ~CIsThingAlive's own pipe before Main (bottom-up; source order of the definitions does not change that, probes L1/L2)
  and Main inherits that body. Own pipe: ~CScriptThing sz 66, c1 63 < T 74 -> expanded -> ~CIsThingAlive sz 84.
  Main temporaries: sav 6 -> c1 78 >= 74 -> called (retail: expanded). And members (sav 3): c1 81 -> called (= retail).
- IL size is spelling-insensitive (four ReleaseData spellings, legacy CCountedPointer, in-class/out-of-class dtor: all sz 84).
- To get retail's temporaries, ~CIsThingAlive's own pipe must CALL ~CScriptThing (Codex device: inline_depth(0) on ~CIsThingAlive).
  That makes the TU's ??1CIsThingAlive COMDAT `vtbl; call ~CScriptThing; jmp ~CPredicateBase`, which differs from the landed owner
  0x00E241B0, and Main binds it (the And members) -> held under the 2026-11-20/21 pick-any rule. So the Codex baseline (best grade
  14526v14510) can never land even if its last residue were fixed.
- Implicit ~CIsThingAlive (FableWin has no ??1CIsThingAlive): And exact, temporaries expand ~CScriptThing; refuted anyway: its copy
  lacks the own-vtable store 0x00E241B0 has.
- Budget inliner (pragma optimize("g",on) + /Og- /d2vol>=20, the landed 0x00D17A80 device): prefix exact AND the ??1CIsThingAlive copy
  equals 0x00E241B0; but the global inline distribution is wrong: the empty TRetVoid::Return is called at 8 sites (retail 0) and
  ~CScriptThing expanded 13x (retail 0, 47 calls); 15225 v 14510. (A reduced CAttackingBandit-shaped probe in the row TU also failed under
  it, but that probe dropped sites, so it is weak evidence; the distribution above is the real argument.)
- /Ob1, /d2inlT55..70, /d2inlS5..12, LTCG (/GL: 14510 bytes but And dtor called, temporaries fully expanded): no.

Conclusion: in this TU retail inlined a ~CIsThingAlive body that calls ~CScriptThing while emitting a standalone copy that expands it.
No single-definition form reproduces both on the lane's cl 13.10.3077 (same open mechanism as FINDINGS_LOG 2026-11-20 p25B, 0x00EC4A00).
Owner decision needed: either accept a ??1CIsThingAlive pick-any copy that differs from 0x00E241B0 (declaration-only opt-in is impossible
because the temporaries need the body), or keep the row parked until the mechanism is found.
