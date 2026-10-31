# Chunk 612 -- notes (10 parked same-length rows)

Result: 3 MATCHED (0x007D9D70, 0x0093C2F0, 0x009852C0; RELOCATION_MATCH, tests PASS under g++ and VC7.1, vland 3/3
PASS), 7 parked in differ/ with renamed, improved models (all same length as retail).

## Names and evidence (per row)
- 0x0093C2F0 ?IsRunnable@CAIStateGroup_StealthSeekEnemy@@UAE_NXZ (Ego_r). Every emitted symbol checked with objdump against
  the Ego_r/FableWin name sets: ??$PeekTC@$$CBVCTCPerceiveThing@@@CThing@@QBE_NPAPBVCTCPerceiveThing@@@Z,
  ??$PeekTC@$$CBVCTCTargetingAI@@@CThing@@QBE_NPAPBVCTCTargetingAI@@@Z, ?HasTC@CThing@@QBE_NW4ETCInterfaceType@@@Z,
  ?test@?$bitset@$0BBC@@std@@QBE_NI@Z, ??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z,
  ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ, ?GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ,
  ?StatGetInterfaceType@CTCPerceiveThing@@SI?AW4ETCInterfaceType@@XZ; callees see NAME_FIXES.tsv. Interface numbers:
  0x34 (bit 20 of +0x24) for CTCPerceiveThing and 8 (bit 8 of +0x20) for CTCTargetingAI, from the retail bit tests plus the
  class of the method called on each component. CTCTargetingAI::StatGetInterfaceType is not in either name set (inline
  everywhere); it is implied by PeekTC<const CTCTargetingAI>.
- 0x00970DF0 ?GetCoverThingList@CAIStateGroup_CreatureUseCoverBase@@MAEXAAV?$vector@PAVCThing@@...@Z (Ego_r): RTTI
  hierarchy CAIStateGroupBase -> CAIStateGroup_EnemyCreatureBase (PTargetThing +0x14, CIntelligentPointer<CThingCreatureBase>)
  -> CAIStateGroup_CreatureUseCoverBase (rtti_bases.tsv); filter types from the callee instantiation 0x00971530 and the
  Ego_r layouts (MakesLine {Pos, Line}; Bind2 {F1*, F2*}); C2DBoxF(const C2DVector&, float) and C2DVector(const C3DVector&)
  are Ego_r ctors; all external symbols checked against Ego_r.
- 0x009852C0 ?FollowUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NM_N@Z (Ego_r, private): callee spellings in the file
  header (GetPItem returns CBaseObject*, CThing::GetPos, TurnToFace private, GetNearbyWanderPosition const,
  AIAction_MoveToPosProximity protected with the exact PDB parameter types, CTCCreatureNavigation interface 7).
- 0x007D9D70 ?_M_fill_insert@?$vector@UKnickKnack@@...@Z: identity from the landed resize 0x007D9FC0; helper symbols
  bound in reloc_bindings.tsv (_M_insert_overflow protected IAE).
- Parked rows: names in each differ/ header (00aa39a0 C3DKeyframe::ToMatrix FableWin; 00760a70 private GetUpDirection;
  00c71f80 / 00c3db80 libpk, descriptive (no PDB/RTTI, landed CAudioSource / CAudioSystem / CNodePool names);
  0058301a Ego_r signature; 0085c7d0 Ego_r + vtable RTTI).

## Descriptive (unproven) names
- 0x00C71F80 CAudioSource::ComputePitchScale and its depth member; 0x00C3DB80 CAudioSystem::CreateExclusiveAudioObject and
  CUnprovenExclusiveAudioObject_00C3D910 (libpk: no PDB, no RTTI). Both parked, so nothing descriptive lands.
- Shape helpers that never reach an object: SetRow (00aa39a0), CInfo (0085c7d0, a CCountedPointer helper ctor that does
  emit ??0CInfo@?$CCountedPointer@... inline only).

## Label corrections
See NAME_FIXES.tsv (20 rows). Most important for landed code: 0x007F6170 is CTCTargetingAI::IsThingValidTarget(const CThing&)
const, not IsThingAttackable (landed file CTCTargetingAI_IsThingAttackable_007f6170.cpp); 0x004C73D0 is CThing::GetPos
(labels say GetPosition); 0x00A55A30 is C2DBoxF::ToC2DBoxI (labels say ToBoxI); 0x0084E810 is
NCameraModes::CBase::AddSubComponent (labels say SetPhysics); 0x007446A0 is CTCPerceiveThing::GetPerceivedThing (one label
says CTCPerception).

## Header debt
- engine/CAIStateGroupBase.h models the vptr as a data member, so it cannot be the base of a class with virtual members
  (a derived virtual adds a second vptr and shifts every member +4). 0x0093C2F0 and 0x00970DF0 use
  `class CAIStateGroupBase : public CBaseClass { CAIBrain* PBrain; unsigned char rest[sizeof(CAIStateGroupBaseLayout) - 8]; }`
  (engine/CBaseClass.h is polymorphic and used as is), the landed 0x00939180 pattern.
- engine/CAIStateGroup_StealthSeekEnemy.h, CAIStateGroup_CreatureUseCoverBase.h, CAIStateGroup_EnemyCreatureBase.h and
  CAIStateGroup_OpinionReactionRoot.h flatten the base; their own members (PEnemy/DetectedPos, PTargetThing,
  ThingReactingTo ...) are copied locally with the real types (CIntelligentPointer<T>, C3DVector).
- engine/CTCPerceiveThing.h is used through the layout rename; LastPerceivedPos is split into _X/_Y/_Z floats, read
  through a C3DVector cast.
- engine/CCountedPointer.h changes the x87 operand order downstream in 0x0085C7D0 (28 diff lines v 5); local model kept.
- CThing has no shared header; the bitset / CVectorMap / Dead-bit model is local in each file (same as landed rows).
