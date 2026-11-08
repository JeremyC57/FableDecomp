# Chunk 877 header overlay (hdr/engine) -- every change is an additive `#ifdef FABLE_RETAIL_*` opt-in; no layout change

Used by the parked differ/00839940.cpp (CThingGameObject::OnHit). Each file is a full copy of rebuild/include/engine/<file>
plus the block named here.

- `CHitParameters.h` -- FABLE_RETAIL_CHITPARAMETERS_ONHIT: includes stlport/fable_map.h + engine/EDamageAttribute.h; inline
  SetHitNegated (+0x27), IsToCauseRecoil / SetToCauseRecoil (+0x28) (FableWin.pdb, bytes read/written in place); declarations
  SetHitLocation 0x00868590, SetPHitMaterialDef 0x00868640, GetDamageToThing 0x00868690, SetAsModifiedByArmour 0x00869020,
  IsLightningStrike 0x008689B0 / IsDivineWrathStrike 0x008689E0 / IsUnholyPowerStrike 0x00868A10; inline GetDamageMap (the +0x30
  map through a reinterpret, the member stays opaque).
- `EDamageAttribute.h` (new) -- `enum EDamageAttribute {};` (PDB enum; enumerator names not needed by OnHit).
- `CThing.h` -- FABLE_RETAIL_CTHING_VIRTUALS: CThing virtual slots 17..45 in PDB order (from the landed
  CThingDeadCreature_OnCreate_00835fd0.cpp list) + forward declarations; FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (probe device only,
  see NOTES: HasTC `__forceinline`, body unchanged); FABLE_RETAIL_CTHING_GETTCDRIVER_DECL (probe device only: GetTCDriver<T>
  declared, retail calls ??$GetTCDriver@VCTCDExplosion@@ 0x007512C0 out of line from OnHit).
- `CThingPhysical.h` -- FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS: slots 46..59 (PeekPMaterial slot 58 / +0xE8, ModifyHealth(float,bool)
  slot 59 / +0xEC, retail `call [edx+0E8h]` / `call [edx+0ECh]` in OnHit); inline IsToNegateAllHits (+0xBC bit 4).
- `CThingManager.h` -- FABLE_RETAIL_CTHINGMANAGER_COLLISIONDEBUG: public inline IsDrawCombatCollisionDebug (+0x78 byte, OnHit 0x0083994F).
- `CPlayer.h` -- FABLE_RETAIL_CPLAYER_GETNUMBER: inline GetNumber() (+0x28, OnHit rumble site).
- `CThingCreatureBase.h` -- FABLE_RETAIL_CTHINGCREATUREBASE_BLOCKEDREACTION: PerformBlockedReaction(CThingCreatureBase*, bool) 0x00661EF0.
- `CThingGameObject.h` -- FABLE_RETAIL_CTHINGGAMEOBJECT_HITQUERIES: IsAddToComboMultiplierOnHit 0x00838B20,
  IsGivingHeroStatChangesOnBeingHit 0x00838B60 (declarations).
- `CTCDExplosion.h` -- FABLE_RETAIL_CTCDEXPLOSION_ONHIT: static `__fastcall` Create(long, const C3DVector&) 0x0080C460,
  SetSpecificThingToHit(CThing&) 0x0080CCF0 (declarations).
