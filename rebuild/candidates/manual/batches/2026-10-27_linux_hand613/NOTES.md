# Hand chunk 613 (CWorldMap .. CAIStateGroup_SitRoundCampfire): 2 of 10 byte-exact

All ten rows were PARKED same-length misses from the 2026-10-30/31 retry sweep. Two now match (RELOCATION_MATCH, behaviour PASS
under g++ and the lane's VC7.1; vland: 2 WINS, no warnings).

## What matched (2)
- NShape::CTrack::GetTrackPosAtLengthFraction 0x00760C40 (342 B; was 342v342 with 53 diff lines, frame 0x38 v 0x30). The
  frame was not a slot-assignment gap: retail's first point and previous point are named C3DVector copies through a user copy
  constructor (VC7.1 homes all six words, keeps first.Y on the x87 stack across the call-free loop, copies the rest as
  integers); `covered` is declared between the two copies (its zero store lands between prev.Y and prev.Z); the inline
  operator/ rescales its own parameter (`f = 1.0f / f`), which homes the reciprocal in the dead `fraction` slot and
  multiplies from memory. Model re-spelled: class C3DVector, NShape::CTrack : NShape::CShape (Ego_r.pdb, V keys, virtual
  dtor), STLport std::vector<C3DVector> (the CRetailVector stand-in is gone). Leaf ?GetTrackPosAtLengthFraction@CTrack@NShape@@QBE?AVC3DVector@@M@Z.
- NScript::CV_MurderTwistScript::GemsFoundProcess 0x00EAD9F0 (356 B; was a one-register residue). Copying the landed sibling
  CV_MurderTwistScript::MagicRuneProcess 0x00EA8C20 model fixed it at the first grade: namespace NScript, the retail
  FABLE_RETAIL_NSCRIPT headers (CScriptThing / CGameScriptInterface / CScriptBase), CCountedPointer.h, an inline
  CScriptThing dtor and operator== as `PImp->IsEqualTo(*other.PImp)` (FableWin protected IsEqualTo(const CScriptThing&)),
  STLport vector<CScriptThing>::erase(iterator) bound spelling. Leaf ?GemsFoundProcess@CV_MurderTwistScript@NScript@@QAEXXZ
  (Ego_r / FableWin). All callee symbols checked with objdump -t.

## Parked for Fable 5.1 (8)
- 0063e590 CWorldMap::GetNavigationLayerAt DIFFER(340v340): the Bind3 branch stores F3 (&overlap) one instruction before `lea ecx,[esp+14h]` | tried: Bind3 body-assign, named tools / IsNot / overlap locals, QBE ForEachInRoughArea + const finder, PDB F1..F3 non-const members, branch swap | next: lane gap: one-store scheduling
- 007bc004 NThingShot::CTCShotDefinitionNymphFun::InitMovement DIFFER(342v342): frame 0x50 v 0x5c (CRightHandedSet temp packed over the RandomiseShotDirection temp) | tried: C3DVector copy ctor, early return, named set (345), velocity temp, user operator= (345), named / const& randomised result (frame right, 343v342 `lea esi,[ebp-44h]` v `mov esi,eax`) | next: a form that keeps the call-result temporary alive without naming it
- 007bee40 NThingShot::CTCShotDefinitionBase::UpdateFacingDirection DIFFER(344v344): retail sums Z*Z + X*X + Y*Y and loads forward.Z before side.X in the cross product | tried: 3 sum orders, Dot member/free, 3-arg ctor, pointer CrossProduct, swapped product, copy ctor | next: lane gap: x87 commutative operand canonicalisation
- 00654438 NPlayerGui::CDrawQuestInfo::Draw DIFFER(347v347) (retail_asm row, file in retail_asm/differ/): `fld st(1) ; fadd [pos.X]` v `fld [pos.X] ; fadd st(2)` | tried: C2DBoxF(C2DVector, C2DExtentsF) ctor both orders, named width/height (343), size.Y first, compound BRX, operator+ (364) | next: lane gap: x87 operand choice
- 00744b50 CTCPerceiveThing::UpdateSmell DIFFER(353v349) best (same-length form 349v349, 37 lines): user C3DVector copy ctor fixes the frame; residue r kept on the x87 stack (`fst r ; fmul st(2)` v `fstp r ; fxch ; fmul [r]`) and normalised Z kept live | tried: FastInvSqrt helper (landed IsInFOV shape), address-taken r, parameter-rescale helper, direction.Dot(wind), const wind, direction declared first (389) | next: the "one float on the x87 stack" choice
- 007fe990 CTCSingleUserActionPoints::OnCreate DIFFER(351v351): esi/edi swap throughout | tried: while-loop find form | next: lane gap: equal-use colouring
- 00744fb0 CTCPerceiveThing::UpdateHearing DIFFER(355v355): the empty comparator temporary gets its own frame byte (retail passes the Bind2's address) shifting key/noise/radius 4-8 | tried: implicit comparator ctor (zeroes it, 360), filter temporaries (frame 0x28) | next: lane gap: empty-temporary slot allocation
- 00939670 CAIStateGroup_SitRoundCampfire::IsRunnable DIFFER(358v358): this esi / map edi (retail edi / esi) | tried: landed Bandit sibling model with PDB names, nested-if lookups (MinionSitOutside shape), named creature | next: lane gap: equal-use colouring

## Skipped
None.

## Names (evidence)
- Proven (two sources): both matched leaves (Ego_r/FableWin name sets + body/members); NShape::CShape/CTrack and C3DVector V keys
  (pdb_class_keys.tsv); CScriptThing::IsEqualTo protected const& (FableWin.pdb, engine/CScriptThing.h); erase spelling (bound).
- Re-proven in the parked differs: 0x007BEE40 is CTCShotDefinitionBase::UpdateFacingDirection (14 vtables incl. the base's slot 36 +
  Ego_r name set + FaceMovementDirection member); 0x007BC004 is NThingShot::CTCShotDefinitionNymphFun::InitMovement UAE (public
  virtual, Ego_r); 0x00939670 callee 0x00658D5D is IsSociableTime const; UpdateSmell / UpdateHearing are private (AAE) and their
  closing calls are SetLastAwareInfo (AAE) / ReactToAlerts (AAE); 0x0063E590's engine call is CMainGameComponent::IsEditorActive()
  const (FableWin) on the protected static PMainGameComponent (bound by the landed 0x00929650).
- Descriptive: 0x0137571C g_bMapNavigationLayers (one reference, unnamed), 0x0129BA3C g_epsilon (five different bound names, none
  proven), filler virtual slots (v00.., Slot00..) in the parked differs.

## Label corrections
- 0x007BEE40 "NThingShot::CTCShotDefinitionThrow::vfunc_36" -> NThingShot::CTCShotDefinitionBase::UpdateFacingDirection.
- 0x007FE990 "CTCGraphicAppearance_InitDummyObjects" -> CTCSingleUserActionPoints::OnCreate (as the parked file already said).
- 0x00744D00 landed as "CTCPerceiveAlert::FindAlertEvent" -> CTCPerceiveThing::ReactToAlerts (NAME_FIXES.tsv).
- 0x00658D5D used as "CTCVillage::IsCurfewTime" by the landed 0x0093AE80 -> IsSociableTime (NAME_FIXES.tsv).

## Header debt
- engine/CWorldMap.h, engine/CTCBase.h, engine/CAIStateGroup_SitRoundCampfire.h, engine/CTCPerceiveThing.h flatten the vptr as data,
  so the parked differs keep local polymorphic models (as the landed siblings do). CWorldMap has two vtables (0x01244AEC primary,
  31 slots; 0x01244AB4 at +4).
- engine/CCountedPointer.h CCPPointerInfo::RefCount is unsigned; 0x00EAD9F0 matches with it (its `dec ; cmp [eax],0 ; jne` tests ==0).
