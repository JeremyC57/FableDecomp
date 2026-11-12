## 0x0075A7A0


## 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions -- RELOCATION_MATCH (~60 grades)
- Made it match: the body is Chris Hecker's rigid-body ResolveCollisions (named Position copy, numerator spelled `-(1.0f + e) * v.Dot(n)`,
  impulse `n * (num / den)` with NO named j) + header opt-ins FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX and NEW overlay opt-in
  FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP (operator^ with X and Z components parenthesised). The last residue (one `lea ecx,[esp+2c]`
  scheduled late) was the numerator spelling; `-(dot * (e + 1))` and named j both leave it.
- Model: SUPERSEDED by Repair 3 below (the row is on the shared engine/CTCPhysicsRigidBody.h typed branch + _BODY with the repo's engine/CRigidBody.h; four overlay files; no local class, no made-up names). The text that follows on this bullet describes the state before Repair 3.
  (before Repair 3) Model (repair 2026-11-12): CMatrix3x4 from the shared engine/CMatrix3x4.h + overlay opt-in FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR
  (declares the landed 0x00A56530). CTCPhysicsRigidBody / vectors still local (see below); CRigidBody / CRigidBodyConfig have no header. CopyVectors/CopyTrivial/CVectorVector/CLongVector are Simulate's
  descriptive (made-up) names for the inline STLport clear() forms. No flags. Overlay: hdr/engine/C3DVector.h.
- Names: manifest label correct (Ego_r + FableWin `?ResolveCollisions@CTCPhysicsRigidBody@@AAEXAAVCRigidBodyConfig@CRigidBody@@@Z`). No NAME_FIXES.

## Verifier findings not applied (repair 2026-11-12) -- SUPERSEDED: Repair 3 applied all of them
- Finding 1 (local `class CTCPhysicsRigidBody` while engine/CTCPhysicsRigidBody.h exists): the verifier is right that the header carries
  the retail offsets (+0x40/+0x4c/+0xa0, sizeof 0x258); the row comment saying otherwise is corrected. But the header cannot be used
  without losing the match: its flat branch has opaque byte members only, and its typed branch (FABLE_RETAIL_CTCPHYSICSRIGIDBODY) holds
  real std::vector<C3DVector>/<long> from stlport/fable_containers.h. With those (repair_diag/v_hdr1.cpp, with the trial overlay repair_diag/CTCPhysicsRigidBody.h opt-in
  FABLE_RETAIL_CTCPHYSICSRIGIDBODY_BODY typing Body + declaring the member) the row grades DIFFER(729v726): VC7.1 drops retail's second
  induction slot ([esp+14h] i beside [esp+10h] byte offset) and no longer holds &config.AngularMomentum / &config.AngularVelocity in
  ebx/ebp (0x0075A7CB/0x0075A7CE). Isolation on the matching source: only CollisionNormal as std::vector breaks it (d3 index vector,
  d5 BoundingVertices: RELOCATION_MATCH; d4: 729v726); not the clear()/_Destroy path (d12 trivial-dtor traits: still 729) and not the index
  type; the single cause is STLport's operator[] going through begin(): local `C3DVector& operator[](long n) { return *(begin() + n); }`
  = 729v726 (d19), `{ return *(_M_start + n); }` = RELOCATION_MATCH (d20/d21). Tried with the std::vector form without success: begin()[i],
  a named normal pointer/reference/copy, const operator[], unsigned/int counts, every CROSS_* x DOT_* x SCALE_X_PAREN opt-in (50 grades),
  numerator/impulse/position spellings x cross/dot (40 grades), STLport fastcall/forceinline opt-ins, inline_depth 2..6, /Ob1 /Oa /Ow.
  Same model as the landed same-TU siblings CalculateVertices 0x00759EC0 and Simulate 0x0075AD50. Open lead: whatever makes retail's TU
  see CollisionNormal[i] without begin()'s extra inline level (a different STLport 4.6 configuration?) -- needs the real _vector.h.

## Repair 3 (2026-11-12) -- RELOCATION_MATCH on the shared headers (strict headers too); test PASS
- Finding 1 fixed: no local CTCPhysicsRigidBody / CRigidBody / vector stand-ins. The row includes engine/CTCPhysicsRigidBody.h typed branch
  (FABLE_RETAIL_CTCBASE, _CTCPHYSICSBASE, _CTCPHYSICSRIGIDBODY, _CTCPHYSICSRIGIDBODY_BODY -> the repo's new engine/CRigidBody.h, which needs
  FABLE_RETAIL_C3DVECTOR_CLEAR) plus NEW nested opt-in FABLE_RETAIL_CTCPHYSICSRIGIDBODY_RESOLVECOLLISIONS (declares the private member).
  The 729 v 726 residue of STLport's `operator[] { return *(begin()+n); }` is fixed by NEW opt-in FABLE_RETAIL_STLP_VECTOR_INDEX_START
  in stlport/fable_containers.h (`*(this->_M_start+n)`); deciding retail bytes: [esp+14h] index slot (0x0075A7BA, 0x0075A7E0) beside the
  [esp+10h] byte offset (0x0075A7D4), and &config.AngularMomentum / &config.AngularVelocity held in ebx / ebp (0x0075A7CB / 0x0075A7CE).
  Control grade with the same shared headers and the default operator[]: DIFFER(729v726). Marked Inferred in the row header (STLport 4.6
  text has begin(); retail's configuration evidently indexed without that inline level).
- Finding 2 fixed: no `v00` placeholder (the class comes from the header).
- Finding 3 fixed: every overlay file rebased on today's repo header (C3DVector.h from verify_evidence re-checked; CTCPhysicsRigidBody.h
  re-copied after the repo's 22:21 InitialiseBody update); `diff <repo> <overlay> | grep -c '^<'` = 0 for all four. hdr/README.md rewritten.
- Finding 4 fixed: the test defines FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR (and the row's other opt-ins) before including the shared
  header; its CMatrix3x4::operator* body is the landed row 0x00A56530's. The test runs the real member on a raw-storage object (abstract
  class) reached through the explicit-instantiation access idiom. vt.py needs FABLE_HDR_OVERLAY=$W/hdr (the opt-ins live in the overlay).
- Grades this repair: 4 (a1/a2 RELOCATION_MATCH, a3 control 729v726, final strict RELOCATION_MATCH). Backup of the pre-repair state: repair3_backup/.

## 0x00854DD0 (not landed)

## 0x00854DD0 CCreatureAction_SpiritAttack::PerformAction -- DIFFER(697v720), 17 hunks (wf17 author, ~50 grades)
Best: differ/00854dd0.cpp (= v57). Ported onto shared headers: CThing (+_GETTC_EXPANDED, _ORIGINAL_VTABLE for OnHit slot 65),
CThingPhysical (+_ISINRANGE), CIntelligentPointer (+_ASSIGN: temp ctor + SetPItem/GetPItem assignment + dtor), CTCTargetingBase,
CHitParameters (+_CTOR, overlay _HIT_CTOR), C3DVector (+_SQUAREDMAGNITUDE_ZYX), CCreatureActionBase (+_PCREATURE);
CTCCoopSpirit = FrameUpdate-sibling wrapper over _quarantine fields (StatGetInterfaceType 0xC7, GetMeleeTargetRange/AddExperience/OnStrike).
What fixed the big register residue (57 -> 20 hunks): ONE `CTCCoopSpirit* tc_spirit` declared at function top and reused by both
GetTC(0xC7) calls (edi=this spilled, ebx=8 constant then spirit -- the whole prologue/first half now matches).
Range: `float range = GetMeleeTargetRange(); CTCPhysicsBase* physics = target->GetPhysicsTC(); CThingCreatureBase* creature = PCreature.Get();`
gives retail's call order (range call, PhysicsTC load, GetPItem, radius) -- only the fadd operand order stays swapped.
Remaining residue: (1) `fld radius; fadd range` vs retail `fld range; fadd radius` (0x00854ED2; operand swap / range+= / GetRadius
deferred all canonicalise to the same); (2) both C3DVector subtractions: ours Z,Y,X (ctor args right-to-left), retail X,Y,Z keeping X/Y
on the x87 stack and spilling only Z (`fst [esp+28h]; fmul [esp+28h]`); (3) final GetTC(0xC7) through ebx vs retail eax + tail dup.
Ideas tried (no match): /Op /EHsc /G5 /G7 /Ob1, optimize("s"); default GetTC (HasTC+Find); separate spirit vars (57 hunks);
range local variants (v12/26/27/28/54/56/58 break regalloc unless creature local also added); operator- memberwise overlay (exact
X-first + Z-spill subtraction, but the named result is COPIED into direction: 22-25 hunks); copy+`-=`, friend operator-, float-locals
operator-, explicit per-component blocks (X first but stores X/Y immediately or keeps all three in regs); ctor assign/forceinline;
Newton paren variants; NORMALISE_LOCALS(_XYZ); SQUAREDMAGNITUDE XYZ/default; (a-b).Normalise() (const return, does not compile).
Best next idea: find the operator- / direction spelling that evaluates X first AND constructs straight into `direction`
(memberwise overlay gets the subtraction bytes exact; the only gap there is the result->direction copy) -- e.g. a non-inline-at-c1
memberwise helper, or a direction built by a CThing helper (check FableWin for a GetDirectionTo-style inline on CThing/CThingPhysical);
the same X-first issue is open in wf13 0x00951D60 (operator+), solved there with a per-component block.
Names: GetPTargetedThing (not GetTarget), GetMeleeTargetRange (landed 0x00670000 is mislabelled CTCPhysicsBase_GetAirResistance), OnStrike
(not OnHit), OnHit vslot 65 (not Hit), IsHitNegated (not Deflected) -- per DOSSIER, confirmed in Ego_r/FableWin signatures.

## 0x00854DD0 attempt 2 -- DIFFER(710v720), 9 hunks (was 17); ~35 grades + 4 probes
Best: differ/00854dd0.cpp (= A2.cpp). Same headers/opt-ins as attempt 1 (no new opt-in used); no flags.
What improved it (two source facts):
1. Tail (eax + duplicated epilogue, 0x0085507B/0x0085508D) MATCHES: the first spirit TC `CTCCoopSpirit* tc_spirit` declared inside the
   target block, and the final strike through a separate `CTCCoopSpirit* tc_strike` at function end (no longer a shared variable).
2. Hit position MATCHES (X, Y, Z computed and stored one at a time, interleaved with the pushes, into [esp+14h]): the position is
   written member by member from `const C3DVector& creature_pos = PCreature->GetPos()` into a named `C3DVector hit_pos`, and an extra
   block `{ ... }` encloses hit_pos + CHitParameters + OnHit + AddExperience (without that block hit_pos gets its own slot, frame 0x84).
   The block encloses only the hit, NOT direction/CreateAttackEffect (those placements give 23 hunks).
Remaining (9 hunks, two places): (1) `fld [physics+34h] ; fadd range` vs retail `fld range ; fadd [physics+34h]` (0x00854ED2);
(2) direction subtraction Z,Y,X (all on the x87 stack) vs retail X,Y,Z with only Z homed (`fst/fmul [esp+28h]`) and the scaling's
`fxch st(1)` / `fld [esp+28h]`.
Probe facts: (a) with the hit removed (probe 2) or with direction copied before the hit (probe 4) the subtraction becomes X-first: the
Z-first order holds only because direction is address-taken by the CHitParameters ctor; (b) the Normalise sum spelling (ZYX/XYZ/default/
UNPAREN/SQUARE_HELPER/LOCALS) and Newton paren variants never change the order; (c) float locals x,y,z in any order + ctor/Initialise/
member stores: X always ends up computed last (on top) or X/Y get stored eagerly (member stores, 10 hunks); (d) 1-3 dead loops (empty or
unrolled 2-trip, wf15 lesson) change nothing; (e) the QFE-4035 compiler gives the same bytes.
Also tried (no gain): XY_GROUP, implicit copy ctor, operator- / ctor defined out of class, plain-inline arithmetic (Normalise then goes out
of line), const / const-ref ctor params, memberwise operator- (+COPY_ASSIGN_BODY), copy+`-=`, /G6 /G7 /Ox, range spellings on the new
base (`range +=`, distance local, const range, creature_pos ref local, fully inline expression -> regalloc collapses to 54-57 hunks).
Best next idea: the direction is the only open x87 question. Retail behaves as if the three differences were scalar temporaries computed
X first BEFORE direction's address escapes, with direction.Z stored once (its home) -- look for a spelling where direction is not yet
address-taken when built, e.g. the hit built from a by-value/copied direction that VC7.1 coalesces, or the CHitParameters ctor reached
through an inline helper of the TU taking `C3DVector` by value; then re-check whether the fadd order flips with it.

## 0x00854DD0 attempt 3 -- DIFFER(710v720), 9 hunks unchanged (best legit source still differ/00854dd0.cpp); ~25 grades + ~12 toy probes
Two level-changing facts found with toy compiles (cl /O2 /FAs on a minimal replica; see ATTEMPTS.log probes A3-1/A3-2):
1. fadd order (0x00854ED2): VC7.1 emits `fld range ; fadd [physics+34h]` ONLY when the radius is an lvalue (direct `physics->Radius`
   or a `const float&` getter). Any by-value inline getter (GetRadius in-class, deferred, forceinline, const return, local copy, cast,
   parens, flags /Op /O1 /G7 /Oa /Ow) gives `fld [physics+34h]` first. PROBE m1 (overlay opt-in FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_PUBLIC_PROBE
   making Radius public + `range + physics->Radius`) FIXES this hunk -> 8 hunks (differ/00854dd0_radius_lvalue_probe.cpp). Not adopted:
   Ego_r/FableWin say ?GetRadius@CTCPhysicsBase@@QBEMXZ (by value) and Radius is protected in the header -- needs a second source for
   the access (friend? public member in the retail header?) before it can land.
2. direction (0x00854F14..): X-first + only-Z-homed + the `fstp f; fxch st(1); fmul f; ... fld [z]` scaling appears ONLY for a vector
   whose address is never taken. `C3DVector offset = a - b; C3DVector direction = offset; direction.Normalise();` (x2,
   differ/00854dd0_offset_copy_probe.cpp) gives retail's exact instruction sequence but offset gets its own slot ([1c..24]) and direction
   moves to [28] (frame 0x84, 20 hunks). Retail homes Z at direction.Z ([28]) itself, so retail's vector is one object at [20..28].
Also tried (no gain): hit gets C3DVector(direction) / named hit_dir copies (21-24 hunks); temp-normalise `C3DVector(a-b).Normalise()`
(+IMPLICIT_COPY / COPY_ASSIGN_BODY); default ctor + assign; offset in a block; creature-pos ref then copy+`-=` (SUBTRACT_ASSIGN, call
order right but copies); parens/casts as no-code tuples; radius local/distance local/`+=` forms; fully inline range expressions
(54-57 hunks); range declaration placement.
Best next idea: find the source shape in which retail's single direction object at [20] is NOT address-taken while the subtraction is
built (e.g. a TU-local inline helper that returns the hit direction by value into the CHitParameters argument slot, or a hit built from
a by-value C3DVector that VC7.1 constructs in place at [20]); and find a second source for an lvalue radius access (header friend /
public Radius) -- with both, the row should be 0-1 hunks.

