# NF55 header overlays (round 7, game-side tail, second half)

Every file here is a copy of the current repo header (`rebuild/include/<same path>`, re-diffed against the repo at hand-back)
plus `#ifdef` opt-in blocks only. Nothing outside the opt-in blocks changed: without the macro each file is token-identical to the
repo version (`diff` shows additions only, 0 removed lines, every addition inside its own `#ifdef`/`#elif defined(...)` block).
None of NF54's overlaid headers (NGameText_CDataBank.h, CPersistTraits.h, CWorld.h, boost/scoped_ptr.h, CWorldMap.h, CAnimInit.h,
stlport/fable_containers.h) is overlaid here.

| header | macro | what it adds (declaration only unless said) | row(s) |
|---|---|---|---|
| engine/CTCInventoryAbilities.h | FABLE_RETAIL_CTCINVENTORYABILITIES_ONSPECIALABILITYBUTTONPRESSED | OnSpecialAbilityButtonPressed(long) 0x005EED4C, OnSpecialAbilityUsed(EHeroAbility,bool) 0x005EEA9B | 0x005EED4C |
| engine/CVectorMap.h | FABLE_RETAIL_CVECTORMAP_SORT_OUTOFLINE | `void Sort();` out of line + inline `Update() { if (Dirty) Sort(); }` | 0x005F6423 |
| engine/CTCQuickAccessMenu.h | FABLE_RETAIL_CTCQUICKACCESSMENU_ISQUITTING_ACTIVATE | IsQuitting() const 0x00625940, Activate() 0x00628000 | 0x00583BD5 |
| engine/CUserProfileManager.h | FABLE_RETAIL_CUSERPROFILEMANAGER_SUBTITLING | inline IsSubtitlingEnabled + UseNewProfileValues +0x0C, SubtitlingEnabled +0x0D, SubtitlingEnabledNewProfile +0xCD (layout inside the opt-in) | 0x0088F890 |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_ANIMSPEED (flattened branch) | GetAnimationSpeedMultiplier() const | 0x00692BC0 |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_UPDATELASTATTACKINGCREATURE (retail branch) | protected UpdateLastAttackingCreature() 0x006624A0 | 0x006624A0 |
| engine/CTCCreatureOpinionOfHero.h | FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_FLAT_CLEARPERSONALITY (flattened branch) | ClearPersonalityOverride() 0x00682310 | 0x0089C4C0 |
| engine/CInputProcessGameBase.h | FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISQUICKACCESSEVENTINQUEUE (retail branch) | protected IsQuickAccessEventInQueue(long, EInputButtonEventType) const | 0x0068E450 |
| engine/CInputProcessQuickAccessItems.h | FABLE_RETAIL_CINPUTPROCESSQUICKACCESSITEMS_UPDATE | virtual Update(CProcessedInput&), private CancelQuickAccessCharge(); a guarded `class CProcessedInput;` | 0x0068E450 |
| engine/NParticleEngine_CParticleUpdateParameters.h | FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_FORCEINLINE (`#elif` branch) | the same ctor body as the default branch, marked FABLE_FORCEINLINE | 0x007C25A0 |
| engine/CTCPhysicsStandard.h | FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE (retail branch) | protected virtual UpdateOnSurface (slot 97) / GetPossibleCollidedWithArea const (slot 98), protected PerformForceUpdates, UpdateIsMoving, PerformStepBasedMovement, GetPossibleInteractedWithList const; includes C2DBoxI.h + fable_containers.h inside the opt-in | 0x00725C40 |
| engine/CTCPhysicsControlled.h | FABLE_RETAIL_CTCPHYSICSCONTROLLED_MOVE | public virtual Move(), private AdjustThingListForDebugHelpers, UpdateAdditionalImpulse | 0x00725C40 |
| engine/CTCCombat.h | FABLE_RETAIL_CTCCOMBAT_FLAT_RESETCOMBO (flattened branch) | ResetComboChainPos() 0x00733210 | 0x00857B70 |
| engine/CTCPerceiveThing.h | FABLE_RETAIL_CTCPERCEIVETHING_FLAT_GETFOV (flattened branch) | GetFOV() const 0x007445D0 | 0x00665260 |
| engine/CTCContainer.h | FABLE_RETAIL_CTCCONTAINER_ADDITEMS | public AddItemsToInventory() 0x008ED170, protected AddItemToInventory(CThing&) 0x008ED090 | 0x008ED170 |
| engine/CTCContainerRewardHero.h | FABLE_RETAIL_CTCCONTAINERREWARDHERO_RELEASEITEMS | protected ReleaseItems(float) 0x0074E6E0 | 0x0074E6E0 |

Notes
- No opt-in changes a default (non-opt-in) layout or vtable. The two virtuals in CTCPHYSICSSTANDARD_MOVE declare slots the retail
  vtable 0x01265F84 already has (the row's `call [eax+184h]` / `[eax+188h]` match), so they add no slot to any existing row.
- An ACQUIRE opt-in for engine/CTCShopKeeper.h was written for the blocked row 0x00799900 and is NOT handed back (no landed row
  uses it); it is kept with the attempt under work/attempts/hdr_unused/.
