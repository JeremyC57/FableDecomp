# Hand chunk 233 (CTCInventoryQuests .. CTCPhysicsRigidBody): 22 of 40 byte-exact

All 22 sources in `src/` grade RELOCATION_MATCH under the lane's RTM cl 13.10.3077 (`grades.txt`) and every test in `test/`
prints PASS under g++ (`tall.sh`) and the lane's VC7.1 (`tall.py`). No QFE toolset was needed.

## What matched (22)

- **CTCInventoryQuests** — 006223c0 `vfunc_70` (slot 70: take the selected quest card through the quest manager, drop it
  from the trader's hero centre, rebuild the list). Shape: the outer tests are nested ifs (pushes at entry, one shared false
  epilogue) but each activation arm returns false itself; a merged `bool activated` lets VC7.1 tail-merge the two calls.
  The component pointers are the landed GetTCIfPresent form (unset on a miss).
- **CTCInventoryTrade** (frame-pointer size TU: `optimize("y",off)` + `("s",on)` unless noted)
  - 0059ca95 `RefreshNameAndTitlePC` and 0059cfbd `RefreshNameAndTitle` (twins; see label corrections): name + " x N" into
    two children. The landed FinalRefreshSubtitles model; `name += GetXString() + IntToWideString(count)` gives retail's
    temporary order (count string first, homed in the dead argument slots).
  - 0059d0b8 `FinalConstructSubtitles` / 0059d65a `FinalConstructSubtitlesPC`: the FinalRefreshSubtitles text logic plus the
    CreateTitlePC creation (manager fetched first, counted-pointer local); the PC twin adds slot 60 and a `list` slot-124 call.
  - 0059ec48 `~CTCInventoryTrade`: needs `#pragma optimize("s", on)` (speed gives `test ; jbe` for the loop entry) and the
    Components range emptied through an inline `clear() { erase(begin(), end()); }` so the range is pushed from `[edi+4]/[edi]`.
  - 005ae86b `OpenInventoryPC`: the landed CTCInventoryExperience::OpenInventory model; the front-end manager's virtual
    slots must live in `CManager` (a non-polymorphic CManager base makes `CreateComponent` take `lea ecx,[edi+4]`).
- **CTCKraken** — 007fb500 `HasTargetBeenHitByKraken`: null intelligent-pointer temporary for `list::remove`, STLport
  `size()` counting walk, the double-GetPItem IS_THING_ALIVE, then a const map lookup; matched first try.
- **CTCLifeTimer** — 007e59b0 `FrameUpdate` (slot 9): the EntityFadeOut component-map model; a named `CThing* thing =
  ParentThing` taken after the block-scoped name puts the parent reload into the dead `this` register before the AddTC pushes.
- **CTCLook**
  - 00751970 `GetBodySettingsSlowInFastOut`: the body copy always snaps the angle to the target (`*angle = targetAngle`), the
    second AngleDifference result is unused; int Sign helper homed in the dead argument slot.
  - 00751c70 `GetAnglesFromBodyToPos` and 00751e80 `GetTurningAngles`: IS_THING_ALIVE must be the `template<class T> bool
    IS_THING_ALIVE(T* const& p)` helper (retail tests the pointer in eax and re-reads it into ecx; a macro or by-value helper
    keeps it in ecx). GetTurningAngles also needs nested ifs with one `return false`, `__forceinline` WrapAngle and a
    `__forceinline` C3DVector `operator!=` (its ecx/edx bases are reused by the fastcall).
  - 00752000 `UpdateHeadPos`: world/position named locals, operator- and a single-expression `X*X + Y*Y + Z*Z` (VC7.1 orders
    the squares by stack position, which reproduces both retail orders).
  - 00752200 `ChangeEyeAngles`: `(float)fabs(x) * 2.0f <= range`, Sign helper, change test written `Eye == old`.
  - 00753170 `CTCLook::CTCLook`: the landed CTCLight ctor model (CTCBase + inline CAGraphicAppearanceComponent base, STLport
    vector base, counted pointers with zeroing ctors, CachedHeadPos as a pack(1) record at +0xa1); matched first try.
- **CTCOwnedEntity** — 007e8460 `OnSerialise`: the landed CTCActivationTrigger UID-remap model; the load arm's version local
  must be uninitialised.
- **CTCPhysicsBase**
  - 006b0610 `ModifyResponseForNearVerticalSlopes`: the cos result in a float local (`limit`) gives retail's `fxch ; fxch`;
    Normalise sum grouped `x*x + (y*y + z*z)`; `angle / 360.0f * 6.2831854820251465` (double pool).
  - 008802b0 `HasSphereCylinderCollidedAt`: `d = pos - other->Position` through an inline operator- (the component ctor form
    swaps which dead argument slots the squares are homed in).
- **CTCPhysicsControlled** — 00725a30 `GetPossibleCollidedWithArea` (slot 98): two named float locals for the movement (a
  C2DVector local lets VC7.1 share its words with the box), `radius + (Position.Y + Max(moveY, 0))` for maxY.
- **CTCPhysicsNavigator** — 007267c0 `GetPossibleCollidedWithAreaWithImpulse`: same body without the margin select.
- **CTCPhysicsStandard / CTCPhysicsRigidBody** — 00724290 / 0075a350 `OnSerialise` (slot 1): Up and Forward copied into two
  C3DVector locals through a user copy ctor (the implicit copy uses a carrier register), defaults homed in the dead bool slot.

## Parked for Fable 5.1 (18)

- 006afaf0 CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject DIFFER(249v249): XY length sum `fld st0;fadd st2` v `fld st1;fadd st1`, dot kept on the stack (`fst;fmul st2`) v retail `fstp` + `fmul [dot]` | tried: extern epsilon, component ctor copy, LengthSquaredXY, statement/grouped sums, dot statement order, operator* forms | next: lane gap: x87 spill choice (same shape as 00880510)
- 008803e0 CTCPhysicsBase::HasSphereSphereCollidedAt DIFFER(305v301): last Normalise product `fld [z];fmul st1;...;fstp st0` v retail `fmul [z]` | tried: operator*=, `Z = r*Z`, locals, inline vs forceinline, operator- forms, response copy orders | next: lane gap: x87 reciprocal consumption
- 00880510 CTCPhysicsBase::HasSphereSphereCollidedAt(from, to) DIFFER(299v301): Normalise reciprocal `fst;fmul st2;fstp st1` v retail `fstp [r]; fxch; fmul [r]` | tried: `*this *= r` (by value / const&), `X = X*r`, reference-aliased reciprocal | next: lane gap: x87 spill choice
- 005cd33f CTCInventoryStats::OnCreate DIFFER(246v244): this in esi with edi's push sunk (retail edi, all pushes at entry); final release `mov eax;dec [eax+4];mov ecx,eax` v `mov ecx;dec [ecx+4]` | tried: explicit/inline lookups, self local, accessor, DecRefToZero/Release/ReleaseDef forms | next: lane gap: DecRefToZero eax release + register choice
- 005c4671 CTCInventoryWeapons::GetFreeAugmentationSlotForWeapon DIFFER(248v248): register permutation only (retail this=edi, pointer=esi, weapon=ebx) | tried: FindTC members, self local, named pointer local, inline helper, /Oy-, inline_depth(0) | next: lane gap: register allocation
- 0059c983 CTCInventoryTrade::IsSpecialCase DIFFER(240v274): one shared release chain v retail's two (true / false exits), ebx v esi zero register | tried: first draft only | next: write each exit's releases out (inline-once destructor band), clearing tattoo pointer type
- 0059fafe CTCInventoryTrade::RefreshEntriesAmountsAndAvailabilitiesPC DIFFER(284v281): 0x10 frame v 0x14, a key copy homed in the component slot | tried: per-loop counters, named key, const&/assigned sub pair | next: find what owns retail's extra frame word (maybe a by-value key or a 12-byte temporary)
- 007fbb00 CTCKraken::CreateHeadEffect DIFFER(294v299): the intelligent-pointer temporary not packed over the dead GetSubDef out-slot (0x10 v 0xc frame, esi push sunk) | tried: block-scoped out-slot, block-scoped named effect, both | next: lane gap: frame packing of an address-taken out slot
- 00752e00 CTCLook::SetBodyMoving DIFFER(272v296): Sign int and wrapped target get their own frame word (0xc v 8) and no jump-back copy arm | tried: bodyTarget local, ternary, if-assign, declaration order, Sign by reference, operand order | next: IsLookTypeBodyOnly()/GetHeadTurnRangeXY() as real inline members (PDB names) and a separate bodyTarget copy arm
- 00753380 CTCLook::FrameUpdateHeadInterested DIFFER(298v297): retail keeps a dead `xor esi,esi` and the camera block as the first fall-through | tried: four if/else layouts, declaration order, ternary target | next: camera position through an inline helper so it is not cross-jumped
- 00745730 CTCPerceiveThing::UpdatePerception DIFFER(263v272): SetAsDirectlyAware tail call moved to the end and the timer arm shares the incapacitated epilogue; `fst` v `fstp/fld` timer | tried: if/else, negated test, returns, inline DecayAwareness, extern decay constant | next: lane gap: tail-call block placement
- 0078809b CTCMeleeTrap::InitialiseAnimation DIFFER(-v253): not attempted | tried: - | next: CAnimInit (0x0070C050) temporary + def dance as the landed melee-trap rows
- 007cce20 CTCMultiStrike::FrameUpdate DIFFER(-v299): not attempted | tried: - | next: model the 0x5c event local (0x007A3BC0 / 0x00789358) and the `new (0x18)` action
- 00771660 CTCOpinionSource::OnCreate DIFFER(-v265): not attempted | tried: - | next: new'd vector-holder + list push_back of a CIntelligentPointer
- 007e5ff0 CTCOracleMinigame::GetTextWithNarrator DIFFER(-v255): not attempted | tried: - | next: function-static CWideString (guard 0x013BAE90) + `?:` return with construction flag
- 006b0460 CTCPhysicsBase::GetOrientationInterpolator DIFFER(-v310): not attempted | tried: - | next: two RH sets through slots 72/73 into C3DOrientationInterpolator
- 00726be0 CTCPhysicsNavigator::UpdateAdditionalImpulse DIFFER(-v254): not attempted | tried: - | next: GetAllThingsInRoughArea (0x00725F10) into a vector + by-value CApplyAdditionalImpulseIfInCombatAndTooClose for_each (0x00727500)
- 0075aec0 CTCPhysicsRigidBody::Move DIFFER(-v306): not attempted | tried: - | next: matrix-vector product statement order around the two virtual calls

## Skipped

None (no row holds a second function).

## Label corrections

- 00724290: catalog "CTCPhysicsRigidBody::OnSerialise" is **CTCPhysicsStandard::OnSerialise** (vtable 0x01265F84 slot 1).
- 0075a350: catalog "CTCPhysicsRigidBody::vfunc_1" is CTCPhysicsRigidBody::OnSerialise (slot 1).
- 007267c0: catalog "CTCPhysicsRigidBody::GetPossibleCollidedWithArea" is **CTCPhysicsNavigator::GetPossibleCollidedWithAreaWithImpulse(const C3DVector&) const** (FableWin.pdb; callers in UpdateAdditionalImpulse).
- 00725a30: "CTCPhysicsControlled::vfunc_98" is CTCPhysicsControlled::GetPossibleCollidedWithArea() const.
- 007e59b0: "CTCLifeTimer::vfunc_9" is CTCLifeTimer::FrameUpdate.
- 0059ca95: catalog "RefreshNameAndTitle" is **RefreshNameAndTitlePC** (PC_SUBTITLE_INDEPENDANT; called from BuyItemPC 0x005B0AAC); 0059cfbd is the console RefreshNameAndTitle (called from BuyItem 0x005AF64E).
- 00880510: the second "HasSphereSphereCollidedAt" takes (other, from, to, response) and calls the 3-argument 0x008803E0.
- Callee labels: 0x0065DE8A is list<CIntelligentPointer<CThing>>::remove (not CEditControlCentre::SetScriptBrushThingsToMove); 0x005B1A43 map<CComponent*,CList*>::find (not std::set_FindLE); 0x00485AA7 vector<CComponent*>::erase(first,last) (not CGuiControlListBox::SelectEntryFromData); 0x005B211F/0x005B2160/0x005B21F4/0x005B2847 are STLport map destructors (not CActiveFile::OnReadFinished); 0x00686A70 CTCBase::GetWorld (not CViewBase::GetDisplayEngine); 0x00754E70 is the YZ twin of AngleBetweenPositions.
