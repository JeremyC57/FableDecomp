# agentFAM05 header overlay (2026-11-11)

Full copies of the CURRENT repo headers (`rebuild/include/engine/...`) with `#ifdef` opt-in additions only. Every added block
starts `// agentFAM05 (2026-11-11):`; stripping the blocks (and the one `#ifndef X // agentFAM05` wrap in CTCTargetingPlayer.h)
gives back the repo file byte for byte (`work/hdrcheck.py`: 33 of 33 "same"). Default paths are unchanged. Copy each file over
its repo twin at landing.

| Header | Opt-in macro | Adds (declarations only unless stated) |
|---|---|---|
| CAnimateThing.h | FABLE_RETAIL_CANIMATETHING_MESHANIMS | GetInterpolatedMeshAnim / GetMeshAnims const (0x0070E650 / 0x0070E6A0) |
| CGameCameraManager.h | FABLE_RETAIL_CGAMECAMERAMANAGER_ADDCAMERAPOINT | AddCameraPoint(CThing&) 0x006991F0, RemoveCameraPoint(CThing&) 0x006984A0 |
| CTCActionUseBase.h | FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS3 | enum EWhenToActivate, GetInterfaceType (21), GetWhenToActivate (29) |
| CTCActivationReceptorBase.h | FABLE_RETAIL_CTCACTIVATIONRECEPTORBASE_REMOVETRIGGER | RemoveTrigger(CTCActivationTrigger&) 0x007F7750 |
| CTCAnimationComplex.h | FABLE_RETAIL_CTCANIMATIONCOMPLEX_VSLOTS3 | OnFrameUpdate (31) |
| CTCCameraPointGeneralCase.h | FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3 | enum ECameraPointType; DefinitionBase GetInterfaceType / FrameUpdate / GetCameraPointType()=0; GeneralCase GetCameraPointType |
| CTCCameraPointGeneralCase.h | FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VTABLE | DefinitionBase virtuals in TLC slot order 8..34 (use instead of _VSLOTS/_VSLOTS3 when a body calls them through the vtable) + private IsAngleValid 0x0075C530 |
| CTCCarryable.h | FABLE_RETAIL_CTCCARRYABLE_VSLOTS3 | OnKill (14), OnInitialActivate (6) |
| CTCCreatureNavigation.h | FABLE_RETAIL_CTCCREATURENAVIGATION_VSLOTS | HasFrameUpdate (8) |
| CTCDNavigationSeed.h | FABLE_RETAIL_CTCDNAVIGATIONSEED_VSLOTS | IsComponentSerialisable (23) |
| CTCDoor.h | FABLE_RETAIL_CTCDOOR_VSLOTS3 | BulletTimeFrameUpdate (10) |
| CTCGatherPointBase.h | FABLE_RETAIL_CTCGATHERPOINTBASE_VSLOTS3 | `public:` around OnSerialise (both PDBs: UAEX, the header has it protected) |
| CTCHeroStats.h | FABLE_RETAIL_CTCHEROSTATS_VSLOTS3 | GetInterfaceType (21) |
| CTCInventory.h | FABLE_RETAIL_CTCINVENTORY_VSLOTS3 | GetInterfaceType |
| CTCInventoryAbilities.h | FABLE_RETAIL_CTCINVENTORYABILITIES_VSLOTS3 | GetInterfaceType |
| CTCInventoryExperience.h | FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VSLOTS3 | GetInterfaceType, OnChangeCategory (37), ResetStatIndicatorValues 0x0056E5AB |
| CTCInventoryTrade.h | FABLE_RETAIL_CTCINVENTORYTRADE_VSLOTS3 | GetInterfaceType, GetInventoryShoppingValidity (48) |
| CTCInventoryWeapons.h | FABLE_RETAIL_CTCINVENTORYWEAPONS_VSLOTS3 | GetInterfaceType |
| CTCMeleeTrap.h | FABLE_RETAIL_CTCMELEETRAP_VSLOTS3 | protected virtual UpdateTrap (29), private UpdateAnimation 0x00788555 / UpdateStrikeAction 0x00788AEF |
| CTCMultiArrow.h | FABLE_RETAIL_CTCMULTIARROW_VSLOTS3 | FrameUpdate (9), protected CreateAmmoEffect 0x007CC2B0, members NumberInVolley +0x14 .. CutSceneCancelled +0x44 (layout addition after NumberOfShots; includes fable_containers.h / CIntelligentPointer.h) |
| CTCOracleMinigame.h | FABLE_RETAIL_CTCORACLEMINIGAME_VSLOTS | HasFrameUpdate (8), in the class branch |
| CTCSoundPlayer.h | FABLE_RETAIL_CTCSOUNDPLAYER_VSLOTS3 | BulletTimeFrameUpdate (10) |
| CTCTargeted.h | FABLE_RETAIL_CTCTARGETED_VSLOTS3 | protected OnSerialise (1) |
| CTCTargetingPlayer.h | FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3 | PProjectileTargetedThing / PTargetLockedThing typed CIntelligentPointer<CThing> (same offsets; `#ifndef` wrap of the two opaque lines), GetPProjectileTargetedThing (35) / GetPTargetLockedThing (36) |
| CTCTeleporter.h | FABLE_RETAIL_CTCTELEPORTER_VSLOTS3 | OnSerialise (1) |
| CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_VSLOTS3 | GetInterfaceType, OnInitialActivate (6), SetVillageLimbo 0x0065C36E, RemoveThingAndUpdateCameraPoints 0x0065B6DD |
| CTCWeapon.h | FABLE_RETAIL_CTCWEAPON_VSLOTS3 | OnSerialise (1), FrameUpdate (9) |
| CTCWillLightning.h | FABLE_RETAIL_CTCWILLLIGHTNING_VSLOTS3 | OnKill (14), private FinalCleanup |
| NEntityEvents_CEventHitBase.h | FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3 | enum EEventType; CEventBase GetType()=0 (1), GetLifeTime (3); CEventHitBase IsFlourish (5) |
| NEntityEvents_CEventTriggered.h | FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3 | GetType |
| NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_VSLOTS3 | BulletTimeFrameUpdate |
| NScreenEffect_CScreenEffectPriorityStack.h | FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_PROCESSES | CStackEntry, CScreenFilterEntry::Reset 0x00883AE0, ICustomProcessInterface, the three Process classes with OnErase |
| NSpeechGainManager.h | FABLE_RETAIL_NSPEECHGAINMANAGER_CDIALOGUELAYERDEF_COPY | CDialogueLayerDef virtual Copy(const CDefClassBase*), operator= |
| NThingShot_CTCShotDefinitionBase.h | FABLE_RETAIL_NTHINGSHOT_CTCSHOTDEFINITIONBASE_VSLOTS3 | OnKill |

Slot numbers: the TLC vtable slots agentFAM03 proved (re-verified by agentFAM05). Spellings: Ego_r / FableWin name sets.
Known header debt not fixed here: the _VSLOTS / _VSLOTS3 blocks of CTCCameraPointDefinitionBase declare its virtuals out of slot
order (fine for leaf bodies; a body that calls them through the vtable needs _VTABLE).
