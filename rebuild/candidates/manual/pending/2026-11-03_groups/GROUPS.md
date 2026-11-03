# Remaining rows grouped by translation unit (3420 rows, 1436 groups; reserved rows excluded: 300)

The binding evidence is retail's own structure, not the catalog's guesses: VC7.1 lays each object file's functions out
contiguously, a vtable's slots are one class, an EH funclet is a slice of its parent's object, a class's members and the STL
instantiations they caused share a TU, and a same-TU callee is the lever that fixes most incomplete-model parks. Links, strongest
first: funclet->parent (retail EH tables), same pointer table in .rdata/.data (RTTI-named where RTTI exists), same non-template
class scope, a direct E8/E9 call to another remaining row in the same landed neighbourhood, address adjacency with no landed row
between. Template helpers (CThingSearchTools, CPersistContext, GF*, std::) never form a TU of their own; they ride with the TU that
instantiated them.

## How to read a group (the reverse-engineer's view)

* **anchor** is the class / TU; **tu_context** lists the landed classes around the group's addresses with their counts. A group
  whose anchor has many landed siblings is a TU we have mostly rebuilt: its remaining rows are the ones whose model needed
  something the siblings have since proven (the retail header, a PDB signature, an inlined helper). Cut the whole group as one
  chunk and give the agent the siblings' sources: the same-TU callee lever is free there.
* A group whose anchor has few landed siblings is a TU we have barely touched. Cut it class-first: ctor, dtor, the vtable slots in
  order (the vtables column names it), then the free helpers the calls column points at.
* Funclet groups land with their parent, never alone (`batches/2026-11-03_funclets_parents/STATUS.tsv` has the parents).
* An impossible-majority group is CRT / LTCG / vendor-asm territory: prove `toolchain_library` or `retail_asm`, or retire it.
* Single rows are mostly leftovers of landed TUs (F2): append them to a neighbouring chunk of the same class rather than cut them
  alone; the F1 giants get one session each with a drafted skeleton.

## Proposed cutting order

1. **A finish-the-TU groups**, largest first (CTCInventoryAbilities, CTCInventory, CTCShop, CTCVillage, CUserProfileManager,
   CNavigatorAStar, CEngineLightingManager, CGameCameraManager, CGameScriptInterface, CWorldMap ...): one chunk per group, the
   brief lists the group's landed siblings as the model.
2. **C vtable / class completion** and **B2 fresh TUs** (CEngine, CEngineWaterRenderer, CEnginePrimitiveManager*MeshesBase,
   QuadricOptimiserInternals, NParticleEngine CPSC*, CChunkedFileChunk): class-first order inside each chunk.
3. **B parked-heavy TUs** only after their class has its `FABLE_RETAIL_<CLASS>` branch (PLAN section 3): the residues there are
   register / x87 ties that re-probing does not fix.
4. **D funclet bundles** are free once their parents land: re-run the funclet procedure.
5. **F1 giants** one per session; **F2 singles** appended to the nearest chunk of the same class; **E** retired with evidence;
   **G gap-scan starts** through `bounds/extent.py` first.

## Categories

| category | groups | rows | bytes (est.) |
|---|---:|---:|---:|
| A finish-the-TU (few rows left in a mostly landed class) | 364 | 1601 | 1,763,308 |
| B parked-heavy TU (re-cut with the retail header + same-TU callees) | 33 | 205 | 193,644 |
| B2 unworked TU (fresh chunk) | 40 | 148 | 202,858 |
| C vtable / class completion (slots mostly unworked) | 32 | 159 | 245,903 |
| D funclet bundle (lands with its parent TU) | 55 | 261 | 47,222 |
| E impossible-majority (retire with evidence) | 52 | 111 | 82,792 |
| F1 single row >4 KB (skeleton, one session) | 25 | 25 | 243,680 |
| F2 single row, leftover of a landed TU | 281 | 281 | 242,053 |
| F3 single row, isolated | 388 | 388 | 263,853 |
| G gap-scan starts (extent first) | 166 | 241 | 835,184 |

Group sizes: {'1': 864, '10-29': 47, '2-4': 398, '30+': 1, '5-9': 126}. Columns in GROUPS.tsv; per-row detail in ROWS.tsv.

| group | anchor (class / TU) | rows | bytes | parked | unworked | funclets | residues | vtables | TU context (landed classes around it) |
|---|---|---:|---:|---:|---:|---:|---|---|---|
| A  G00b23b80 | CEngine::CEngine | 37 | 12,368 | 3 | 34 | 0 | x87-order:2 incomplete-model:1 | CEngine | CEngine:204 CTextureManager:3 CMapOwner_00b282b0:3 CIEngine:1 |
| D  G00c222d0 | CVtable_012b3850 | 29 | 946 | 0 | 1 | 28 |  |  | CVtable_012b3850:1 CPanelFailedAllocationHandler:1 CSoundEventSet:1 CVtable_012b3858:1 |
| C  G00b36ec0 | CEngineWaterRenderer | 28 | 69,424 | 4 | 24 | 0 | register-tie:2 block/arm-layout:2 | CEnginePrimitiveManagerParticleSpriteGroup CEngineWaterRende | CEngineWaterRenderer:76 CEngineLandscapeRenderer:48 CEnginePrimitiveManagerParticleSpriteGroup:12 CEngineInternalPrimitiveParticleSpriteGroup:7 |
| A  G0088e0f0 | CGameScriptInterface | 25 | 11,104 | 16 | 9 | 0 | register-tie:6 block/arm-layout:5 stack-slot/frame:2 x87-order:2 ltcg/crt/asm-impossible:1 | CGameScriptInterface | CGameScriptInterface:288 |
| B  G007bbc9f | CTCShotDefinitionBase | 24 | 17,309 | 15 | 9 | 0 | register-tie:6 stack-slot/frame:5 x87-order:2 incomplete-model:1 block/arm-layout:1 | CTCDrainLife NThingShot::CTCShotDefinitionFireballSpell NThi | CTCShotDefinitionBase:26 NThingShot::CTCShotDefinitionBase:24 NThingShot::CTCShotDefinitionTearInGround:13 NThingShot::CTCShotDefinitionMultiArrow:9 |
| D  G00c4a3b0 | PKCBufferedReader | 23 | 4,108 | 0 | 5 | 18 |  |  | PKCBufferedReader:22 CBufferOwner_00c4b010:4 PKCStaticQueue<SReadRequest>:3 PKCStaticQueue<CUnprovenStaticQueueWord_00C4BB20>:3 |
| A  G004fc6b0 | CWorldMap | 22 | 34,944 | 14 | 8 | 0 | register-tie:6 x87-order:4 block/arm-layout:2 incomplete-model:2 | CWorldMap | CWorldMap:179 CWADBank:5 CBankFile:2 CThingFilter_OverlappingCircle:2 |
| B  G0081f240 | CMap | 20 | 17,344 | 11 | 7 | 0 | ltcg/crt/asm-impossible:5 block/arm-layout:3 register-tie:2 x87-order:1 | table@012b5c60 | CNavQuadTree:44 CMap:26 CNavSwitchableLeafNode:19 CNavQuadTreeNode:17 |
| D  G00c3da60 | CTCHeroStats::CTCHeroStats | 20 | 3,856 | 3 | 4 | 13 | eh/unwind:2 block/arm-layout:1 | table@012b5ab0 | CTCHeroStats::CTCHeroStats:6 CEnginePrimitive:3 CAudioSystem:3 Row00c41c40::CameraConstants:3 |
| A  G005ee49a | CTCInventoryAbilities | 19 | 32,215 | 13 | 5 | 0 | register-tie:7 stack-slot/frame:5 ltcg/crt/asm-impossible:1 | CTCInventoryAbilities table@01256ba8 | CTCInventoryAbilities:122 |
| B  G00ac70f0 | COptimisedPrimitive | 19 | 23,616 | 11 | 8 | 0 | x87-order:4 stack-slot/frame:3 register-tie:2 ltcg/crt/asm-impossible:1 wrong-flags/pragma:1 |  | QuadricOptimiserInternals::COptimisedPrimitive:31 QuadricOptimiserInternals::COptimisedMesh:25 QuadricOptimiserInternals::COptimisedVertex:23 COptimisedPrimitive:17 |
| A  G00875b10 | CTools | 18 | 18,480 | 10 | 8 | 0 | x87-order:4 stack-slot/frame:4 register-tie:2 | NCameraModes::CProjectileWeapon | NCameraModes::CProjectileWeapon:57 NCameraModes::CTools:19 CTools:14 CFilter_Bind2<CThingFilter_IsOfType, CThingFilter_OverlappingSphere>:12 |
| G  G0121ac70 | `anonymous namespace' | 16 | 41,617 | 0 | 1 | 0 |  | table@013742b8 | `anonymous namespace':22 CCharString:2 NStreamingFontBankGlobals:2 _static_init::NBankFileManager:2 |
| A  G0058301a | CTCInventory | 16 | 39,016 | 12 | 3 | 1 | register-tie:7 stack-slot/frame:3 block/arm-layout:1 ltcg/crt/asm-impossible:1 | CTCInventory | CTCInventory:126 |
| A  G00659625 | CTCVillage | 16 | 7,422 | 8 | 8 | 0 | register-tie:5 incomplete-model:1 x87-order:1 stack-slot/frame:1 |  | CTCVillage:117 CGuiControlComboBox_0065be32:2 CInputProcessGameBase:2 |
| A  G0059d270 | CTCInventoryTrade | 15 | 36,055 | 9 | 6 | 0 | register-tie:4 stack-slot/frame:2 block/arm-layout:2 load/schedule-order:1 |  | CTCInventoryTrade:70 NInventory::CTCInventoryTrade:1 |
| B  G00b05c60 | CPSCLight | 15 | 21,072 | 9 | 6 | 0 | ltcg/crt/asm-impossible:6 x87-order:2 register-tie:1 | NParticleEngine::CPSCLight NParticleEngine::CPSCSingleSprite | NParticleEngine::CPSCLight:38 NParticleEngine::CPSCSingleSprite:37 NParticleEngine::CPSCAttractor:8 NParticleEngine::CPSCRenderMesh:5 |
| B  G007e8590 | CAIStateGroup_JackOfBladesBase | 15 | 19,552 | 11 | 4 | 0 | register-tie:5 x87-order:4 stack-slot/frame:2 | CAIStateGroup_JackOfBladesStage1 CAIStateGroup_JackOfBladesS | CAIStateGroup_JackOfBladesBase:24 CCreatureAction_JackFlash:15 CAIStateGroup_JackOfBladesStage3:9 CAIStateGroup_JackOfBladesStage2:7 |
| A  G005eaf46 | CTCInventoryMap | 14 | 59,085 | 6 | 8 | 0 | register-tie:3 wrong-flags/pragma:1 stack-slot/frame:1 block/arm-layout:1 | CTCInventoryMap | CTCInventoryMap:78 CTCTavernGame:15 |
| C  G00b9d730 | CEnginePrimitiveManagerStaticMeshesBase | 14 | 35,152 | 5 | 9 | 0 | register-tie:2 x87-order:1 ltcg/crt/asm-impossible:1 stack-slot/frame:1 | CEnginePrimitiveManagerStaticMeshesBase | CEnginePrimitiveManagerStaticMeshesBase:31 CEnginePrimitiveManager2DStaticMeshes:23 CEnginePrimitiveManagerStaticMeshes:9 CEngineInternalPrimitiveStaticMesh:6 |
| A  G0049ffd0 | CWorld | 14 | 17,294 | 10 | 3 | 1 | register-tie:4 stack-slot/frame:2 x87-order:2 block/arm-layout:1 wrong-flags/pragma:1 |  | CWorld:76 CThingBodyReorienter:18 CCreatureInteraction:6 CTCOpinionSource:6 |
| A  G00a1a260 | CNavigatorAStar | 14 | 16,544 | 9 | 5 | 0 | x87-order:7 stack-slot/frame:1 register-tie:1 | CNavigatorAStar | CNavigatorAStar:91 CSteeringAgent:14 |
| A  G00b44820 | CEngineLightingManager | 14 | 11,808 | 6 | 8 | 0 | x87-order:4 register-tie:2 | CEngineLightingManager | CEngineLightingManager:82 CEngineInternalPrimitiveShadowedSpotLight:8 CEngineInternalPrimitiveLight:7 CEngineLocalLightElement:3 |
| A  G005c36c2 | CTCInventoryWeapons | 14 | 11,654 | 9 | 5 | 0 | register-tie:6 ltcg/crt/asm-impossible:3 | CTCInventoryWeapons table@012540c8 | CTCInventoryWeapons:89 |
| A  G00697490 | CGameCameraManager | 13 | 6,672 | 10 | 3 | 0 | x87-order:6 register-tie:2 block/arm-layout:1 ltcg/crt/asm-impossible:1 | CGameCameraManager | CGameCameraManager:86 NCameraModes::COrbitGame:11 CAIGameCameraBase:5 CMapInfo_00697c60:3 |
| D  G00c45980 | CMatrix::CMatrix | 13 | 2,496 | 1 | 3 | 9 | x87-order:1 |  | CAmbientCube:11 CFalloffTable_00c458a0:4 CEntryArray_012b7d50:4 CFractal:2 |
| C  G00bcee20 | CEnginePrimitiveManagerAnimatedMeshesBase | 12 | 28,912 | 5 | 7 | 0 | incomplete-model:2 x87-order:1 register-tie:1 ltcg/crt/asm-impossible:1 | CEngineInternalPrimitiveAnimatedMesh CEnginePrimitiveManager | CEngineInternalPrimitiveAnimatedMesh:21 CEngineInternalPrimitive2DAnimatedMesh:20 CEnginePrimitiveManager2DAnimatedMeshes:20 CEnginePrimitiveManagerAnimatedMeshesBase:19 |
| A  G0045a159 | CDefinitionManager | 12 | 20,945 | 10 | 2 | 0 | register-tie:5 stack-slot/frame:2 incomplete-model:2 ltcg/crt/asm-impossible:1 | CParentDefClassBase | CDefinitionManager:87 CParentDefClassBase:9 NUISystem::CUILocaleGraphicsDef:4 CPlayerGuiDef:2 |
| A  G004b01b0 | CQuestManager | 12 | 14,032 | 10 | 2 | 0 | register-tie:7 block/arm-layout:2 stack-slot/frame:1 |  | CQuestManager:88 NQuestManager::CRuntimeQuestPersistence:12 CTCBoastUI:7 CManager:3 |
| A  G00a17620 | CPhysicsMesh | 12 | 9,440 | 7 | 5 | 0 | x87-order:4 stack-slot/frame:3 |  | CPhysicsMesh:57 CNavigatorManager:9 CNavigatorFlyer:6 CWorldMap:2 |
| B  G00723e50 | CTCPhysicsStandard | 12 | 7,296 | 9 | 3 | 0 | x87-order:5 register-tie:4 | CTCPhysicsNavigator CTCPhysicsStandard | CTCPhysicsControlled:31 CTCPhysicsNavigator:31 CTCPhysicsStandard:30 CTCSimpleAppearanceMorph:5 |
| A  G00406ae0 | CUserProfileManager | 12 | 5,568 | 6 | 6 | 0 | register-tie:3 stack-slot/frame:2 block/arm-layout:1 |  | CUserProfileManager:88 CPlayerGui:4 NCreatureMode::CModeBlock:1 NInventory::CTCInventoryBase:1 |
| A  G00772490 | CTCShop | 12 | 5,376 | 10 | 2 | 0 | register-tie:9 ltcg/crt/asm-impossible:1 | CTCShop | CTCShop:86 |
| B2 G009bc8f0 | CChunkedFileChunk | 12 | 2,320 | 0 | 12 | 0 |  |  | CChunkedFileChunk:7 CVtable_012bed7c:5 C3DMeshFileXAnimatedPrimitiveBlockChunk:4 CVtable_012bed90:4 |
| D  G00c52bb0 | CEngineInternalPrimitiveParticleGroup | 12 | 1,984 | 0 | 4 | 8 |  |  | CCpuInfo:6 CDirectSoundDevice:3 CPersistContext_TransferableVector<CSoundBankEntry>:2 CEAXManager:2 |
| D  G00c1a510 | PKCDefaultFileManager | 12 | 1,798 | 0 | 3 | 9 |  |  | PKCDefaultFileManager:18 PKCTRRedblackMap:3 |
| D  G00c491e0 | PKCStaticQueue<SDecoderMessage> | 12 | 1,235 | 0 | 2 | 10 |  |  | PKCStaticQueue<SDecoderMessage>:2 CVtable_012b8138:2 SQueuedSoundEvent:1 CCodaFrame:1 |
| A  G00434a10 | CScriptThing | 12 | 192 | 0 | 12 | 0 |  |  | CTCCreatureNavigation:45 CThingSoundEmitter:15 CDisplayEngine:7 CGameCameraManager:7 |
| A  G005cd4c4 | CTCInventoryStats | 11 | 44,059 | 5 | 6 | 0 | register-tie:2 wrong-flags/pragma:1 stack-slot/frame:1 x87-order:1 | CTCInventoryStats table@01254bb0 | CTCInventoryStats:43 CEnginePrimitiveAttachmentManager:18 CEngineInternalAttachedPrimitive:13 CPersistTraits<EInputKey>:3 |
| B2 G00b3f5e0 | CEngineMap | 11 | 10,096 | 4 | 7 | 0 | register-tie:3 ltcg/crt/asm-impossible:1 |  | CEngineWorldMap:22 CEngineMap:21 CEngineMapManager:16 CStaticMapBankStateBlock:9 |
| A  G00a114b0 | CPhysicsMeshObject | 11 | 6,064 | 9 | 2 | 0 | x87-order:9 |  | CPhysicsMeshObject:47 CMainGameComponent:6 CRenderTarget:1 CEngineInternalPrimitiveLine:1 |
| A  G00a02df0 | CHandleBasedMemoryPool | 11 | 2,386 | 1 | 6 | 4 | register-tie:1 | table@012b3fdc | CHandleBasedMemoryPool_012b4098:33 CHandleBasedMemoryPool:9 CStreamBuffer:4 CHandleBasedMemoryPool_012b4098::CHandleBasedMemoryPool_012b4098:4 |
| A  G0047d296 | CTCInGameMenu | 10 | 25,654 | 8 | 2 | 0 | x87-order:6 stack-slot/frame:1 register-tie:1 | CTCInGameMenu | CTCInGameMenu:35 CTCInventoryAbilities:5 CTavernTableDef:2 CAIStateGroup_BuyFromSeller:1 |
| A  G00441ee0 | CPSCDecalRenderer | 10 | 17,024 | 5 | 5 | 0 | ltcg/crt/asm-impossible:4 register-tie:1 | NParticleEngine::CPSCDecalRenderer | NParticleEngine::CPSCDecalRenderer:43 CEnginePrimitiveParticleDecal:4 CSprite_00b1b5e0:3 SDecalRequest:2 |
| A  G0048e0a2 | CSubtitleRenderer | 10 | 12,876 | 6 | 4 | 0 | x87-order:2 stack-slot/frame:1 register-tie:1 ltcg/crt/asm-impossible:1 block/arm-layout:1 |  | CSubtitleRenderer:77 CMainGameComponent:3 CEngineSubPrimitiveBase:2 CRetailAllocator_20:1 |
| C  G00802e00 | CTCTargetingPlayer | 10 | 11,424 | 5 | 5 | 0 | register-tie:3 stack-slot/frame:2 | CTCTargetingPlayer table@01242888 | CTCTargetingPlayer:17 CGetBestTargetInDirection:6 NPlayerTargeting:4 CThingFilter_IsValidTargetForPlayerCreature:4 |
| D  G00c16f00 | CAutoDeletePointer | 10 | 1,652 | 0 | 3 | 7 |  |  | CAutoDeletePointer:3 PKCAutoDelete<CUnprovenPKObject_00c17850>:2 CWaypointDataOwner_00c17910:2 |
| D  G00c08d40 | CSystem | 10 | 1,338 | 0 | 1 | 8 |  | table@012b1e20 | CAudioParamBlock:5 CSoundSystem:2 CList:1 CVtable_012b1b40:1 |
| B2 G00c969d0 | libvorbis | 9 | 21,056 | 0 | 9 | 0 |  |  | libvorbis:19 |
| A  G00b62440 | CEngineSkyRenderer | 9 | 15,120 | 2 | 7 | 0 | stack-slot/frame:1 register-tie:1 | CEngineSkyRenderer | CEngineSkyRenderer:45 CGraphicInfoFrame:2 |
| A  G00537776 | CList | 9 | 14,233 | 2 | 7 | 0 | stack-slot/frame:1 register-tie:1 | NUISystem::CList table@012463b4 table@0124649c table@012464f | NUISystem::CList:68 CList:9 |
| A  G00947eb0 | CAIStateGroup_GoToPub | 9 | 8,784 | 6 | 3 | 0 | stack-slot/frame:3 register-tie:2 incomplete-model:1 | CAIStateGroup_GoToPub | CAIStateGroup_GoToPub:74 CCreatureAction_PutDownTankard:2 CTCTavernTankard:2 CVillageTaskCarryCrate:2 |
| B  G00687540 | CMessageEventManager | 9 | 8,352 | 5 | 4 | 0 | register-tie:3 stack-slot/frame:2 | CAIStateGroup_BanditGossip CAIStateGroup_MinionGossip | CMessageEventManager:16 CAIStateGroup_MinionGossip:12 CAIStateGroup_BanditGossip:9 CInputProcessGameBase:7 |
| A  G008fcfb0 | CAIStateGroupBase | 9 | 8,112 | 6 | 3 | 0 | block/arm-layout:3 register-tie:2 stack-slot/frame:1 | CAIStateGroup_ScorpionKing | CAIStateGroupBase:62 CAIStateGroup_ScorpionKing:14 CCreatureAction_ScorpionUnblockableLeftTurnAttack:7 CCreatureAction_VillagerAttackKnockdown:1 |
| A  G0051eda0 | CEntityRuntimePersistence | 9 | 7,856 | 3 | 6 | 0 | register-tie:2 block/arm-layout:1 |  | CThingManager:47 NThingManager::CEntityRuntimePersistence:14 CEntityRuntimePersistence:4 CIntelligentPointer<CThingPhysical>:1 |
| A  G00487570 | CPlayer | 9 | 7,200 | 4 | 5 | 0 | stack-slot/frame:2 register-tie:2 |  | CPlayer:72 CPlayer_Methods:5 CIntelligentPointer<const CThingPlayerCreature>:2 CIntelligentPointer<const CThing>:2 |
| A  G004c7a50 | CThing | 9 | 6,599 | 2 | 7 | 0 | block/arm-layout:1 register-tie:1 | CThing | CThing:81 CTCPhysicsBase:1 |
| A  G00752b00 | CTCLook | 9 | 3,808 | 6 | 3 | 0 | register-tie:3 stack-slot/frame:2 block/arm-layout:1 |  | CTCLook:75 CCountedPointer<CScriptThing>:2 CCountedComponentHolder_007550c0:1 CDefClassBase:1 |
| A  G00c20720 | CActionSummonerLightningAttackTarget | 9 | 1,576 | 0 | 5 | 4 |  |  | CSoundEvent:32 CUnprovenStateObject_00C20720:5 CPKVoiceParams:2 COverriddenChickenSign:1 |
| D  G00c14f90 | Unidentified_00c14f90 | 9 | 356 | 1 | 0 | 8 | block/arm-layout:1 |  | PKCDeferredBank:1 CVtable_012b283c:1 CVtable_012b2834:1 |
| A  G00a9d750 | C3DAnimationManager | 8 | 20,272 | 1 | 7 | 0 | ltcg/crt/asm-impossible:1 |  | C3DAnimationManager:44 CQuaternion:4 C3DMeshFileConvexVolumeChunk:2 C3DKeyframe:2 |
| A  G00b4b8b0 | CEngineShadowRenderer | 8 | 16,176 | 3 | 5 | 0 | x87-order:1 incomplete-model:1 block/arm-layout:1 |  | CEngineShadowRenderer:53 CShadowClipVolume:5 CEngine:3 CMatrix4x4:3 |
| A  G006cf530 | CTCWife | 8 | 12,464 | 3 | 5 | 0 | register-tie:2 block/arm-layout:1 | CTCWife | CTCWife:49 |
| A  G006d5d30 | CThingAICreature | 8 | 12,176 | 4 | 4 | 0 | register-tie:3 stack-slot/frame:1 | CThingAICreature table@012729c0 table@012729e0 | CThingAICreature:64 CBrainOutput:3 CCountedPointer<CBrainOutput>:1 CCountedPointer_006d6210:1 |
| A  G0094fc00 | CAIStateGroup_JackDragon | 8 | 10,672 | 4 | 4 | 0 | x87-order:2 stack-slot/frame:1 register-tie:1 | CAIStateGroup_JackDragon | CAIStateGroup_JackDragon:47 CTCEntityEvents:8 CCreatureAction_DragonRearUpFull:5 CAIStateGroup_BalverineJumpAtFleeingEnemy:1 |
| B2 G00a4cec0 | C3DAnimation2 | 8 | 9,424 | 3 | 5 | 0 | ltcg/crt/asm-impossible:2 block/arm-layout:1 |  | C3DAnimation2:20 C3DAnimationSequenceData:6 CAnimationData_00a4cd70:4 C3DAnimationSequence:4 |
| C  G008daa80 | CModeThunderLightningStorm | 8 | 7,232 | 4 | 4 | 0 | block/arm-layout:2 stack-slot/frame:1 register-tie:1 | NCreatureMode::CModeGuildmasterPowerUpShield NCreatureMode:: | CGetBestIciclePointForTarget:10 NCreatureMode::CModeDragonStrafeBurninate:7 NCreatureMode::CModeCreatureIsProjectile:6 NCreatureMode::CModeThunderLightningStorm:4 |
| A  G006c5a80 | CTCHeroOpinionDeedLog | 8 | 7,120 | 5 | 2 | 1 | register-tie:4 block/arm-layout:1 | CTCHeroOpinionDeedLog | CTCHeroOpinionDeedLog:28 CDeedOwner_006c6010:5 CTCCreatureNavigation:5 CPersistContext:2 |
| A  G007da400 | CTCTavernGameCardBlackjack | 8 | 6,720 | 5 | 3 | 0 | stack-slot/frame:2 register-tie:2 block/arm-layout:1 | CTCTavernGameCardBlackjack | CTCTavernGameCardBlackjack:52 |
| A  G006e94d0 | CCombatManager | 8 | 6,672 | 7 | 1 | 0 | x87-order:2 stack-slot/frame:2 block/arm-layout:1 incomplete-model:1 load/schedule-order:1 |  | CCombatManager:61 CCombatRing:7 CDefendingCombatantInfo::CCombatRing:4 CDefendingCombatantInfo::CCombatWheel:3 |
| A  G004bc800 | CTCGraphicAppearance | 8 | 6,080 | 7 | 1 | 0 | incomplete-model:4 block/arm-layout:2 register-tie:1 |  | NGraphicAppearance::CTCGraphicAppearance:65 CTCGraphicAppearance:5 |
| A  G0074a4e0 | CTCCreatureGenerator | 8 | 5,232 | 2 | 6 | 0 | bounds:1 stack-slot/frame:1 | CTCCreatureGenerator | CTCCreatureGenerator:34 CTCShop:5 CCreatureGeneratorDef:4 NCreatureHiding:3 |
| A  G005ea650 | CTCTavernGame | 8 | 5,162 | 6 | 2 | 0 | register-tie:3 stack-slot/frame:2 block/arm-layout:1 | CTCTavernGame | CTCTavernGame:65 NCameraModes::CScriptModuleBase:2 CTCCameraPointDefinitionBase:1 |
| A  G00663990 | CThingCreatureBase | 8 | 4,992 | 4 | 4 | 0 | ltcg/crt/asm-impossible:1 register-tie:1 block/arm-layout:1 x87-order:1 | CThingCreatureBase | CThingCreatureBase:67 CThingPlayerCreature:5 CAnimationEntry:1 CSpecialEffectsDef:1 |
| A  G00cf9a50 | CQ_AmbushTradersScript | 7 | 18,464 | 2 | 4 | 0 | inline-vs-call:1 block/arm-layout:1 | table@012c6c90 table@012f7ddc | NScript::CQ_AmbushTradersScript:35 NScript::CQ_CinemaTestScript:4 NScript::CQ_ArenaScript:3 TeleportData:3 |
| B2 G00c4e3c0 | CEAXManager | 7 | 16,432 | 0 | 5 | 0 |  |  | CEAXManager:16 CVtable_012b87f4:9 CBankFile:7 CGuiWindow:3 |
| A  G00b51570 | CEngineWeatherRenderer | 7 | 14,480 | 2 | 5 | 0 | ltcg/crt/asm-impossible:2 | CEngineWeatherRenderer | CEngineWeatherRenderer:49 CWeatherState:4 CEngineDynamic3DResource<CVertexBufferWin32,3>:3 CIndexBuffer:3 |
| A  G0056ef2d | CTCInventoryExperience | 7 | 11,591 | 5 | 2 | 0 | register-tie:2 x87-order:2 stack-slot/frame:1 | CTCInventoryExperience | CTCInventoryExperience:59 SExpSpendUndo:2 |
| A  G0055e2c0 | CTCHeroOnlineScoreboard | 7 | 10,640 | 2 | 5 | 0 | stack-slot/frame:2 |  | CTCHeroOnlineScoreboard:47 CActivity:2 CTCPhotojournalCapture:1 SStringPairRecord_0055de90:1 |
| C  G009a1140 | CStreamingFontBank | 7 | 10,208 | 3 | 4 | 0 | register-tie:2 stack-slot/frame:1 | CStreamingFontBank | CStreamingFontBank:15 `anonymous namespace':6 CFontBank:5 CCountedPointer<CScriptThing>:1 |
