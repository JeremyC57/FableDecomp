# Hand chunk 657 (CCreatureAction_* implicit dtors .. CTCTurncoatSpell): 10 of 10 byte-exact

NA003 of the never-attempted pool: ten rows whose masked body equals a landed twin. Identity first (callers, vtable slot 0 of
the deleting destructors, PDB member offsets and donor order), then the twin's shape with this row's own class. Every row
graded RELOCATION_MATCH on its first probe (the 12-probe rule never applied); vland (read-only) 10/10, behaviour PASS under
g++ and the lane's VC7.1. Every catalog label was wrong (five `_Dest_val<...CSymbolMap::CSymbolInfo...>`, two
`~C3DAnimationBlendState`, `vfunc_74`, two `FinalCleanup`).

## What matched (10)
- Creature-action compiler-generated destructors (no vtable store; members in reverse order through their out-of-line
  destructors, base by tail jump). Each class is proven by its deleting destructor's identical member sequence plus callers or
  TU placement:
  - 0x00628EE0 ~CCreatureAction_GiveItemToThing (PItemBeingGiven +0xB0, PReceivingThing +0xA8, ~CCreatureActionBase).
  - 0x007ED750 ~CCreatureAction_ShowItemToThing (PItemBeingShown +0xB0, PReceivingThing +0xA8).
  - 0x006DD690 ~CCreatureAction_ShowTrophy (PEmitter +0xB5, PTrophyBeingShown +0xA8; pointer classes under pack(1)).
  - 0x008C9580 ~CCreatureAction_BanditKingStuck (PObstruction +0x120, PStuckEffect +0x118, ~CCombatActionBase).
  - 0x0066A530 ~CCombatAction_GenericStrikeResponseKnockdown (PThingThatHit +0x118, AnimName ~CCharString +0x114, ~CCombatActionBase).
  Shape lesson: the class declares no destructor; any TU that destroys the object by name emits the implicit `??1` COMDAT
  (a TU harness with a local object is enough; it is not graded).
- Def-pointer owners, compiler-generated destructors over engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE): the inline
  `~CDefPointer -> Release()` gives retail's `test ; je ; dec [ecx+4] ; jne ; call [eax+4] ; mov [m],0` exactly.
  - 0x007710A0 ~CTCDegradableBase (PDefinition +0x10, ~CTCBase); emitted through the CTCDegradableBarrier constructor, as in
    retail's barrier TU.
  - 0x00529AD0 ~CThingHolySite (Def +0xD0, ~CThingGameObject).
- 0x007599C0 CTCPhysicsRigidBody::SetFacingAnglesFromRHSet (slot 74): the twin body as the virtual, with real STLport vector
  members in place of the twin's invented `CRetailVector`.
- 0x007C7560 CTCTurncoatSpell::FinalCleanup (private, unreferenced copy) and 0x007C75A0 CTCTurncoatSpell::OnKill (slot 14):
  OnKill is `FinalCleanup();` with FinalCleanup a plain member defined above it; VC7.1's automatic inlining expands it, exactly
  as retail's three identical bodies show. Callee respelled to CTCSpecialAbilities::FadeOrKillEffect(CThing*) (static __fastcall).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Proven
- 0x00628EE0 ??1CCreatureAction_GiveItemToThing@@UAE@XZ -- both callers (0x0062A8FF in CWorld::EASkipCutScene, 0x007ED37B in
  CTCCarriedActionUseGiveBeer's slot 28) build the local with ctor 0x0062D9E0 (landed ??0CCreatureAction_GiveItemToThing) and
  call this after SetCurrentAction; ??_G 0x0062DA90 (vtable 0x0125919C slot 0) inlines the same sequence; FableWin.pdb
  ??1CCreatureAction_GiveItemToThing@@UAE@XZ; Ego_r members PReceivingThing/PItemBeingGiven.
- 0x007ED750 ??1CCreatureAction_ShowItemToThing@@UAE@XZ -- TU placement just before ctor 0x007ED780 (vtable 0x012714C4) and its
  ??_G 0x007ED830 (identical sequence); CTCCarriedActionUseShow slot 28 (0x007ED870) builds the action and inlines this
  sequence; the only +0xA8/+0xB0 two-pointer action in that band; FableWin.pdb ??1CCreatureAction_ShowItemToThing@@UAE@XZ.
- 0x006DD690 ??1CCreatureAction_ShowTrophy@@UAE@XZ -- unaligned +0xB5 pointer = PEmitter (Ego_r +0xC1 after the bool, retail
  +0xB5); only ShowTrophy / ShowToSheatheTrophy deleting dtors (0x006DF7C0 / 0x006DF8C0) destroy +0xB5/+0xA8; of the
  back-to-back pair 0x006DD690/0x006DD6C0 the first is the base's (VC7.1 emits the base's implicit ??1 first in either use
  order, lane experiment tmp/order_a.cpp; retail ctor/??_G order ShowTrophy first); FableWin.pdb ??1CCreatureAction_ShowTrophy.
  (Order argument is the weakest link of the chunk; see Review at landing.)
- 0x008C9580 ??1CCreatureAction_BanditKingStuck@@UAE@XZ -- follows BanditKingStuck's members (ctor 0x008C94D0, 0x008C9560,
  0x008C9570); its ??_G 0x008C9690 (vtable 0x01281C54 slot 0) is the identical sequence and no other action slot 0 destroys
  +0x120/+0x118; Ego_r PStuckEffect/PObstruction; FableWin.pdb ??1CCreatureAction_BanditKingStuck@@UAE@XZ.
- 0x0066A530 ??1CCombatAction_GenericStrikeResponseKnockdown@@UAE@XZ -- both callers in CThingCreatureBase::ReactToGenericStrike
  follow the Knockdown ctor 0x008679C0 (vtable 0x01277344) + SetCurrentAction; ??_G 0x00868350 identical sequence; Ego_r and
  FableWin name the ??1; Ego_r members AnimName (CCharString) / PThingThatHit (CIntelligentPointer<const CThing>).
- 0x007710A0 ??1CTCDegradableBase@@UAE@XZ -- called by ??_GCTCDegradableBarrier 0x00771120 and ??_GCTCDegradableBase 0x008ECE80
  (the base ??_G can only call the base dtor); Ego_r PDefinition (CDefPointer<const CDegradableDef>) +0x10; RTTI
  CTCDegradableBarrier : CTCDegradableBase : CTCBase.
- 0x00529AD0 ??1CThingHolySite@@UAE@XZ -- sole caller ??_GCThingHolySite 0x00529AB0 (vtable 0x012452DC slot 0); Def +0xD0
  (Ego_r +0xE8 - 0x18); tail jump ??1CThingGameObject@@UAE@XZ 0x00838A00 (both PDBs).
- 0x007599C0 ?SetFacingAnglesFromRHSet@CTCPhysicsRigidBody@@UAEXABVCRightHandedSet@@@Z -- vtable 0x0126996C slot 74; slot 74/75 =
  SetFacingAnglesFromRHSet/SetRHSet from CTCPhysicsStandard's retail TU order (slot74, UpdateOldRHSet, ResetRHSet, slot75) =
  Ego_r order (0x0073A5A0, 0x0073A5F0, 0x0073A640, 0x0073A680); rigid body pair keeps that order in retail, Ego_r and FableWin.
- 0x007C7560 ?FinalCleanup@CTCTurncoatSpell@@AAEXXZ -- unreferenced first of three identical bodies; Ego_r order FinalCleanup,
  OnKill, OnRemove (0x007C6420/0x007C6470/0x007C64C0) = retail 0x007C7560 (no ref) / 0x007C75A0 (slot 14) / 0x007C75E0 (slot 2).
- 0x007C75A0 ?OnKill@CTCTurncoatSpell@@UAEXXZ -- vtable 0x0126E5C4 slot 14 = CTCBase::OnKill (landed AssassinRush/WillLightning
  slot 2 OnRemove / slot 14 OnKill pairs); Ego_r ?OnKill@CTCTurncoatSpell@@UAEXXZ.
- Callee 0x0072AC40 ?FadeOrKillEffect@CTCSpecialAbilities@@SIXPAVCThing@@@Z -- Ego_r donor order CreateParticleEffect,
  FadeOrKillEffect(CIntelligentPointer<CThing>&), FadeOrKillEffect(CThing*) = retail 0x0072AB70, 0x0072ABD0 (landed as
  CTCSpecialAbilities::FadeOrKillEffect), 0x0072AC40; static __fastcall with the thing in ecx and a null test.

Descriptive names: only the TU harness functions (`TUHarness_<Class>`, not retail functions; they exist so the TU emits the
implicit destructor) and a few model field names (`m_flags` for CThing +0x91, `_body` pads).

## Callee names
All bound spellings checked against reloc_bindings.tsv and the PDB sets: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10,
??1CCreatureActionBase@@UAE@XZ 0x00693EF0, ??1CCombatActionBase@@UAE@XZ 0x0062E900, ??1CCharString@@QAE@XZ 0x0099EAE0,
??1CTCBase@@UAE@XZ 0x00686830, ??1CThingGameObject@@UAE@XZ 0x00838A00, ?SetAsMoving@CTCPhysicsBase@@QAEX_N@Z 0x006B0130,
?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50, ?StopTurncoatingEffects@CTCTurncoatSpell@@AAEXXZ 0x007C6DA0
(private per Ego_r; the landed 0x007C75E0 binds it QAE), ?FadeOrKillEffect@CTCSpecialAbilities@@SIXPAVCThing@@@Z 0x0072AC40
(new; vland WARN RELOC_NAME against the catalog label "CTCPhysicalShieldSpell::FadeOutEffect": NAME_FIXES row).

## Label corrections
- 0x00628EE0 / 0x0066A530 / 0x006DD690 / 0x007ED750 / 0x008C9580 `_Dest_val<...CSymbolMap::CSymbolInfo...>` -> the five
  action destructors above; the neighbour 0x006DD6C0 (-> ~CCreatureAction_ShowToSheatheTrophy) carries the same propagated
  label, as does 0x006DF860 (another 33-byte implicit dtor: +0xBC ~CBaseIntelligentPointer, +0xB8 ~CCharString; not
  identified here); the ShowToSheatheTrophy constructor at 0x006DF890 (calls 0x006DF730, stores 0x0125F9F4, `ret 0Ch`) has no catalog row.
- 0x007710A0 / 0x00529AD0 `~C3DAnimationBlendState` -> ~CTCDegradableBase / ~CThingHolySite.
- 0x007599C0 `vfunc_74` -> SetFacingAnglesFromRHSet; landed 0x00759A00 `vfunc_75` -> SetRHSet; landed 0x007239C0 -> SetRHSet.
- 0x007C75A0 / 0x007C7560 `FinalCleanup` -> OnKill / FinalCleanup; landed 0x007C75E0 -> OnRemove.
- 0x0072AC40 `CTCPhysicalShieldSpell::FadeOutEffect` -> CTCSpecialAbilities::FadeOrKillEffect(CThing*).
- Catalog "vector deleting destructor" rows 0x00771120 / 0x008ECE80 / 0x00529AB0 / 0x0062DA90 are the scalar deleting
  destructors of CTCDegradableBarrier / CTCDegradableBase / CThingHolySite / CCreatureAction_GiveItemToThing.
All in NAME_FIXES.tsv (19 rows).

## Header debt
- engine/CCreatureActionBase.h (PDB 0xB4, flattened) vs retail 0xA8; CCombatActionBase retail 0x114 (no usable header):
  local models in the five action rows.
- engine/CThingGameObject.h (flattened PDB layout, no virtuals) vs retail 0xD0 polymorphic base: local model in 0x00529AD0.
- engine/CTCBase.h flattens the vptr: local CTCBase over engine/CBaseClassNonCopyable.h in 0x007710A0.
- engine/CTCPhysicsBase.h / CTCPhysicsRigidBody.h (flattened, PDB vector sizes): local models in 0x007599C0.
- engine/CTCTurncoatSpell.h (PDB offsets, 4 bytes high after Timer): local model in 0x007C7560 / 0x007C75A0.
- No engine header for CBaseIntelligentPointer / CIntelligentPointer: local copies (pack(1) in 0x006DD690, matching the PDB).
