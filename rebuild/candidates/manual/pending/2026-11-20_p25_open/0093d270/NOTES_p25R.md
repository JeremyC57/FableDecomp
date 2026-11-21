# 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable -- reserved, rotated (p25R 2026-11-21)
Best rule-clean source: 0093d270.cpp (= src/93_A1.cpp), DIFFER(305v295), shared headers, Ego_d locals only.
- Ego_d lines 43..62 (40/377/19/19/49/26/409/76/4/2/14); l.47 = `IS_THING_ALIVE(pcreature_base)` (19 B /Od), l.55 = ONE statement
  `ptc_nav->IsAbleToNavigateFromPosToPos(GetCreature().GetPos(), PEnemy->GetPos(), GetCreature().GetCombatManager().GetProximityForNavigation())`
  (VC7.1 /Od 76 B = Ego_d 76 B; a direct member read would be ~72 with VC10's +3 float-argument fld/fstp).
- CINTELLIGENTPOINTER_ARROW_ASSERTED_BODY (or CHECKED_LOCALS) removes the &PEnemy CSE / extra frame word (308 -> 305).
- Residue: the proximity argument. Retail: GetCombatManager -> `mov esi,eax` -> GetPItem -> `mov edx,[esi+28h]` -> push edx. Ours: load right
  after GetCombatManager into a front-end $T slot (reloaded), +4/+6 B and long jumps.
- Bracket (advisor 2026-11-21, NOT admissible): a `const float& GetProximityForNavigation() const` getter (mini o4.cpp h3) reproduces retail's
  instruction pattern exactly; FableWin's ?GetProximityForNavigation@CCombatManager@@QBEMXZ returns by value, so it stays a bracket.
- Mini rule (o3/o8/o13/o14.cpp, tools/mini.py): a by-value inline getter as an argument of a call whose RESULT is used is pre-evaluated into a
  $T (all getter bodies, forceinline, late-defined, const this, /GX /Gr /G6 /Oa /Ob1 ...); discarded result or a named float local: no $T.
  /Op removes the $T but loads early (293v295). Same residue in parked 0x006AA720 (DECISION_006aa720.md).
- Result-consuming forms measured for l.55 (all 76 B /Od, all keep the $T at /O2 in the minis): `if (call) return true;` (row), `return call;`,
  `bool r = call;`, `if (!call) return false;`, `== true`, `!= false`, `k && call`, ternary (minis x2/x4/y1..y7).
- Open (advisor checks a/b): find 2-3 landed byte-matching rows with a by-value inline getter on a call-returned object passed to a used-result
  call with an opaque call between; Ego_r type records for CCombatManager (return type / access / friends).

## Session 2 (p25R 2026-11-21, main's "check each getter/temporary's role against Ego_d" lens)
- Advisor check (b): Ego_d TPI CCombatManager: GetProximityForNavigation public, `float ()` thiscall (0x27EB2); ProximityForNavigation
  private; no LF_FRIEND records in the field list. Nothing admits the direct read or a reference getter.
- Line 55's third argument, re-measured against VC10 /Od calibration:
  - by-value getter: 76 at VC7.1 /Od, and VC10 does NOT spill a float return passed straight as an argument (landed 0x00AC27A0
    C3DMeshPartitionElement::WriteToFile line 97 `out.WriteFloat(BoundingSphere.GetRadius())` is 23 in both compilers; neighbours align).
    76 = Ego_d 76.
  - direct member read: 69 + 3 (float lvalue argument) = 72 (no match).
  - const float& getter: 75 + 3 = 78 (no match).
  So Ego_d's size independently confirms the by-value getter text; the bracket form is Ego_d-contradicted as well.
- Every other Ego_d line fits our text with the usual VC10 deltas (43: virtual call +5, 45: the CIntelligentPointer temporary's EH
  states +14, 46: virtual call +6, 45/54: 377/409 B checked GetTC lines with no release bytes). Ego_d's locals (ptc_targeting,
  pcreature_base, { ptc_nav }) leave no room for another named temporary.
- The residue is unchanged: 305v295 = the $T store / reload (+6) plus one je that becomes near (+4).
- Advisor check (a): no landed byte-matching row found with an inline by-value getter whose load retail defers past an intervening
  call inside an argument list (the candidates found, 0x00847520 / 0x0077A430, use out-of-line getters).

## Session 3 (p25R 2026-11-21, main's step 1 "trace the $T") -- partial, rotated to 0x008E18D0 by main
- IL dump (tools/il/dump2.py at the lowering loop) + symlog on mini o13.cpp (x1 void call, x2 `return n->D(m.GetP())`,
  x3 discarded bool, x4 `if (..) return true`, x5 long result). /O2 listing: $T in x2/x4/x5 only.
- The $T is c2's, NOT the front end's: in x2/x4/x5 only, c2 allocates a kind-4 "field of #3" symbol (#3 = the inlined GetP's
  local copy of its `this`, kind 4, present in ALL five functions) during lowering; chain 10769fe4 <- 10759fc1 <- 10769f16 <-
  1082b1e9 <- 107f3950 <- 10715fe9 <- 107731c2 <- 1075ac21 (lowering loop 0x10773138 region). x2/x4 also get one more kind-15
  symbol (chain 10761819 10763822). In x3 the push reads *(m+0x28) directly; in x2 the inlined `this` store survives
  (`= M(k4 #3) <- IND[0] S(_m)`) and the push reads through it.
- The mini needs no other argument or second inline to make the $T, so "tracks the other arguments' inlines / definition order"
  is not needed to produce it; the trigger is the result-consuming call.
- Step 3 started: landed candidates checked so far are not the shape: 0x00697F50 (Camera.GetFOV() stays out of line),
  0x00BBDDD0 (ClipSphere itself is inlined). Still to find: a landed row whose outer call stays out of line, result used,
  with an inline float getter argument.
