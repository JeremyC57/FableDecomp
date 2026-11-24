# 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable -- AUTH1 log (residue = normalised differing lines, nd.py)
1. q8.cpp (repo Q8 draft, local `manager`/`target`/`self` named locals): reproduce -> DIFFER(305v295), residue 6.
2. e1.cpp Ego_d text (locals ptc_targeting / pcreature_base / {ptc_nav}, IS_THING_ALIVE(pcreature_base), one-statement l.55,
   ARROW_ASSERTED_BODY, MOVE header): -> DIFFER(305v295), residue 6 (= B's best rule-clean). Residue: the GetProximityForNavigation
   float return homed in [esp+10h] right after GetCombatManager; retail keeps the manager in esi and loads +0x28 after GetPItem.
3-8. v01..v06 argument respellings: (float) cast, (&x)->, PEnemy.Get(), const CCombatManager& cast, const ptc_nav cast, `!= false`
   -> v01/v02/v04/v05/v06 305v295 residue 6 (no move); v03 (PEnemy.Get) 308v295 residue 19 (worse: drops the arrow locals).
9. f_z7: e1 + /Z7 (retail had a release PDB) -> 305v295, residue 6.
10. f_gf: e1 + /GF /Gy /G6 -> 305v295, residue 6.
11. f_op: e1 + /Op -> 293v295, residue 5 ($T gone; load early into esi, push esi; retail loads late). Diagnostic only (TU flag).
Minis (mini/m1..m7.cpp, VC7.1 /O2 listing, not grades):
- m1/m2/m3/m5/m6/m7: a by-value inline FLOAT getter passed as an argument of a call that has LATER-evaluated arguments containing
  calls (GetPos) always gets a dead home store (`mov $T,eax`), result used OR discarded (m1 f3 discards and still has it), getter object
  a call result, a pointer member, a named reference (f5) or this-member (m4 f1); forceinline (m6 f1), out-of-class inline (f2),
  `const float` return (m7, mangles ?BM so out anyway), `(float)p` body: all keep it. A local `r` in the getter shows it is the inline's
  return-value variable (`_r$` homed, m6 f3).
- No $T: long getter (m1 f2), single-argument call (m4 f2/f3/f4 `W(x.GetP())`), direct member read (m6 f4), /Op (m1).
- PCH build (/Yc + /Yu, pch.h holds the getter): unchanged (m5).
- Retail 0x006AA720 (different TU, thing_player_creature) at 0x006AAA22: `call GetCombatManager ; mov edx,[eax+28h] ; push edx ;
  GetPos ; push ; GetPos ; push ; call 0x006CB620` -- the m2 shape, and retail has NO home store. Our VC7.1 gives one for every
  by-value getter text there too. So the residue is not a 0x0093D270 text question: both retail sites behave like a direct read.
- Retail 0x00AC27A0 (landed, matches): GetRadius() as the ONLY argument of WriteFloat -> no $T in both (consistent with m4 f2).
- Flag sweep on the m2 mini: /arch:SSE, /arch:SSE2, /G7, /Ow, /Oa, /Og, /Ox, /Ob1 keep the home store; /Os, /O1 replace it with
  fld/fstp (not retail); /Ob0 calls the getter. None gives retail's `mov edx,[eax+28h] ; push edx`.
