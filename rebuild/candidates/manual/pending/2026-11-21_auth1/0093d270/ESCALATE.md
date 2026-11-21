# ESCALATE 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable (295 B) -- AUTH1, 2026-11-21

## Best rule-clean source
`$S/w22/auth1/0093d270/e1.cpp` = DIFFER(305v295), 6 differing lines (Ego_d text: locals ptc_targeting / pcreature_base / {ptc_nav},
`IS_THING_ALIVE(pcreature_base)`, one-statement line 55 with the by-value FableWin getter, ARROW_ASSERTED_BODY, MOVE header). Same as
B's p25R best. No source in the repo or here is better under the rules (hand219's 297v295 reads `manager->m_reach` directly on a local
model: the bracket class).

## Remaining diff (retail - / ours +)
```
 call GetCombatManager (0x00661ED0)
+mov eax, dword ptr [eax + 0x28]
 lea ecx, [ebx + 0x14]
-mov esi, eax
+mov dword ptr [esp + 0x10], eax        ; home of the inlined getter's float return value
 call GetPItem (0x00A01B50)
 mov ecx, dword ptr [ebx + 4]
-mov edx, dword ptr [esi + 0x28]
-mov edi, dword ptr [ecx + 0x18]
+mov edx, dword ptr [esp + 0x10]
+mov esi, dword ptr [ecx + 0x18]
 push edx
```
(+ one je that becomes near; 305 v 295.)

## New this pass (mini evidence, LOG.md for the list)
1. The home store is the inline getter's RETURN-VALUE variable (a getter with a local `float r = p; return r;` homes `_r$` in the same
   place). VC7.1 homes it whenever the float is passed as an argument of a call whose LATER-evaluated arguments contain calls; result
   used or discarded alike (B's "result used" trigger is not the factor in our minis: m1 f3 discards and keeps it).
2. No home store for: a `long` getter, a single-argument call (`W(x.GetP())`, which is why landed 0x00AC27A0 GetRadius matches), a
   direct member read, /Op. /Os and /O1 give fld/fstp. Flags /arch:SSE(2), /G7, /Ow, /Oa, /Og, /Ox, /Ob1, /Z7, /GF /Gy /G6, PCH: no change.
3. **Retail 0x006AA720 (another TU, thing_player_creature) at 0x006AAA22 is the plain minimal shape** `n->IsAbleToNavigateFromPosToPos(
   a->GetPos(), b->GetPos(), creature.GetCombatManager().GetProximityForNavigation())`: retail `call 0x00661ED0 ; mov edx,[eax+28h] ;
   push edx ; GetPos ; push ; GetPos ; push ; call`. Our VC7.1 homes the value there for every by-value getter text (mini m2/m3/m6/m7).
   Check: 0x006AA720 = CThingPlayerCreature::SetPlayerCreatureActionForMelee; Ego_d lists NO float local in its nav block (locals
   ptc_nav, pkickable_def, ignore_nav_test, {tc_stealth}), so B's "named float local" escape does not apply there; the call line's
   text itself is from disassembly only (Ego_d line not decoded).
   So BOTH retail sites of this getter compile as if the argument were an lvalue read (direct member or `const float&`), in two TUs,
   and 0x0093D270's deferred load is only the consequence (the manager pointer stays in esi until the push).

## Tried this pass (grades)
q8 305; e1 305; argument respellings v01..v06 ((float) cast, (&x)->, PEnemy.Get() [308, worse], const CCombatManager& cast, const
ptc_nav, `!= false`) all 305 / residue 6; e1 + /Z7, + /GF /Gy /G6: 305; + /Op 293 (loads early, diagnostic). 11 grades, no reduction.

## Hypotheses for what retail did
(a) The TLC header's GetProximityForNavigation is not the by-value `return ProximityForNavigation;` the FableWin/Ego_d revision has
    (e.g. `const float&` in the TLC branch); contradicted by FableWin QBEMXZ and Ego_d line 55 = 76 B (p25R calibration), unless the
    TLC retail header differs from the debug builds' revision at this one inline.
(b) The getter took c2's other inline path in retail (callee not yet compiled when the caller compiles: "budget path", p25R c1 harness),
    and that path substitutes the body as an expression with no return-value variable. In our one-row TU c2 compiles the getter first.
    Not testable with source alone; the p25R c2path.py hook could log the decision path for the m2 mini and for the row.
(c) Some global flag of the retail build that we lack (none in the sweep above reproduces it). Weak: landed 0x00AC27A0 shows retail
    emitting the same kind of dead home store for the by-value `BoundingSphere.GetCenter().X` argument (0x00AC27EC / 0x00AC280E), so
    retail's compiler does make these stores; what differs is this getter.

## Question for the advisor
Both retail call sites of this getter (0x0093D270 and 0x006AA720, two TUs) compile like an lvalue read, and no by-value text gives that
in our VC7.1 even in the minimal shape. May I (1) run p25R's c2path.py on the m2 mini to see whether forcing the getter onto the budget
path (callee compiled after caller) drops the return-value home, and if it does, look for a natural TU construct that orders it so; or
(2) is the evidence (two independent retail sites, minimal shape) enough to reopen a FableWin-contradicted `const float&` /
direct-read text as Inferred (a TLC revision difference at this inline), or does the row stay escalated?
