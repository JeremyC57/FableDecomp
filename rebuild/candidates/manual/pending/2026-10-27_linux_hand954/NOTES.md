# Hand chunk 954 (family pass 5: 129 shape families K001..K129, 600 rows): 560 of 600 byte-exact

54 MATCH + 506 RELOCATION_MATCH = 560 of 600; 40 parked in differ/ (`// Grade:` on line 2). Families: 121 matched completely, 0 partly, 8 parked. Every matched source's test passes under g++ and VC7.1 (promote.py, tall.sh, tall.py). Identities: 402 proven, 107 probable, 91 descriptive (gen/K*_ids.tsv; each generator gen/gen_Kxxx.py derives its instances' identities from their own bytes and callers).

## Parked rows (40, differ/)
- K005 (6 of 6 rows: 00660ae2 00660b5d 00660bd8 00660c53 00660cce 00660d49) -- 123v123 RemoveThingFromTask<T>: erase's discarded result temp homed in the tasks slot (+14h) where retail reuses the find_if temp's thing slot (+10h); 8 forms tried
- K006 (6 of 6 rows: 00c40e60 00c42c30 00c54a80 00c5c0b0 00c6bf60 00c77690) -- 15v75 x87 10^x inline-assembly kernel (frndint/f2xm1/fscale): no C++ form
- K009 (5 of 5 rows: 0051b880 0052afb0 00639590 006f50b0 00777b30) -- 83v83 Rb_tree erase(key) speed TU: one store/load pair scheduled differently (erase's two by-value iterators); 9 forms tried
- K016 (5 of 5 rows: 006f3570 006f37b0 007197c0 00959380 0095c7c0) -- 570v570/576v576 lane gap: one SIB operand order -- retail mov ecx,[ecx+eax+4] (level*20 as base, LevelInfo as index) where the lane emits [eax+ecx+4]; every other byte identical; 13 forms tried (index expression spellings, level/param types, const method, push temporaries, QFE)
- K039 (5 of 5 rows: 008b96a0 008b9740 008b98d0 008b9a60 008b9bf0) -- 151v151 DIFFER: callee-saved register choice (retail edi=&PItemRemoved, ebx=owner; build ebx/edi swapped); 9 source variants tried
- K062 (5 of 5 rows: 00c5bef0 00c6aae0 00c6f1a0 00c6f800 00c6fc80) -- 170v211 libpk MMX kernel: inline-assembly block in a C shell (ecx input, caller-cleaned ret)
- K072 (4 of 4 rows: 004084b0 00408500 00408550 004085a0) -- 84v67 lane gap: retail cmp [cap],-1/fild [cap] and the def release scheduled with the field read into the shared push-ecx slot; 14 forms tried (RemoveReference variants, def-local Release, /Os, optimize s/sy, QFE)
- K086 (4 of 4 rows: 00436d60 00863e70 008adcc0 00978250) -- 583v583 lane gap (as K016): one SIB operand order -- retail mov ecx,[ecx+eax+4] where the lane emits [eax+ecx+4]; every other byte identical; the K016 forms plus an STLport-vector operator[] spelling tried

## Label corrections (manifest / catalog / reloc_bindings)
- K001: the manifest's `CMemoryAllocatorVariableSize_FindInsertPointLeft`, `CMemoryAllocatorVariableSize_InsertOrFind_ScriptRight`, `RBTree_TransferAllocateInt` and kin are behavioural guesses; these are map hinted inserts.
- K002: catalog `_global::IsAugmentation` propagated onto all seven; 0x005D97F0 bound ?IsAugmentation@@YI_NJ@Z / ?IsWeaponItem@@ -- it is IsWeapon; the other stand-ins (?IsTutorialSubDefItem@@, ?IsAugmentation_005d98f0@@) likewise.
- K003: existing bindings for 0x00905FD0 / 0x008B0B10 / 0x008B0650 and the action ctors are stand-ins (CCountedPointer non-template, CCombatSequenceBase_008b0650, CCombatAction_<addr>, pointer-parameter ctors).
- K004: `_Cons_val<...CParticleMorphs::CEntry...>` is propagated onto five; 0x0098EC90's binding ?HasReadFinished@CThreadedFile@@ is wrong (RemoveObject); 0x01229170's `CCharString_Destructor_Stub3` is a CWideString static's atexit destructor.
- K005: catalog `RemoveThingFromTask<CVillageTaskPickCrops>` propagated onto all six; the bound names' list parameter is the landed dispatcher's CTaskList stand-in (std::list<CCountedPointer<T> > in reality).
- K006: `Pow2_Float` / `MathUtils_Pow2*` / `Math_Pow2_Fast` -- it is 10^x (the log2(10) constant 0x40549A78), not 2^x.
- K007: catalog `CBankFileAsyncData::FreeBuffer` propagated onto all five; _M_destroy_nodes stand-ins ?Release@T@@ / ?Release_<addr>@T@@ (0x0043F510, 0x0043F560, 0x007B3B50, 0x00913690); 0x0048D520 also ?DestructMember_0048d520@@, 0x007B4D60 ??1CBuffer_007b4d60@@.
- K008: catalog `_Fill_n<unsigned long*, ...>` propagated onto all five; 0x0074CD10 bound only as the non-template stand-in ?__uninitialized_fill_n@@YI...; 0x004AA190 also as ?FableUmove_construct_0065574c@@.
- K009: `BinaryTree_DeleteNode_Chain` / `Crime_RemoveEntry` / `BinaryTree_Erase_helper` behavioural; stand-ins ?erase@CUIDToThingMap@@, ?erase@CCrimeSet@@, ?erase@CCombatantMap@@.
- K010: the five manifest labels (`_Move<CDefPointer<CMeleeCombatAbilityDef const>*,...>` x4, `_gapscan::sub_00a964c0`) are propagated; all are STLport `__copy` instantiations of the element types above.
- K012: the manifest label `CItem::operator=` is propagated to all five; none involves CItem.
- K013: `_Copy_impl<CNavigationPosition*,CNavigationPosition*>` is propagated to all five (Dinkumware spelling; no copy, no CNavigationPosition).
- K014: `CCarriedReadableDef::CCarriedReadableDef` is propagated to all five.
- K015: 0x006E8B10 and 0x0091C030 were labelled CAIStateGroup_MinionPutOutFires::Cleanup (propagated): they are CProng::Reset and CTCNymph::ClearTarget.
- K016: the manifest's `GetPBestThingInRoughArea<CThingFilter_HasTrackName, CThingCompare_Nearest, CThingTrackNode>` / `<CThingFilter_IsPotentialCustomer, ..., CThingCreatureBase>` labels on all five were propagated across byte-identical bodies. These functions have no filter or compare call: they are GetAllThingsInRoughArea vector collectors (`ret 0Ch`, push_back + `_M_insert_overflow`).
- K017: `CTCCameraCollision::CTCCameraCollision` is propagated to all five; only 0x007E3020 is CTCCameraCollision's ctor.
- K018: `list<CCountedPointer<NCreatureMode::CModeHoverBase::CHoverAnim>::erase` (x3) and `_gapscan::sub_*` (x2) are propagated; only the element types above are real. Landed callers bind 0x00719210 as `CActionList::erase`, 0x00AACBB0 as `CSoundList::erase`, 0x00AEC7C0 as `CEmitterList::EraseOutOfLine` (stand-ins; review).
- K019: all five carry the propagated `_Push_heap<float*,int,float,std::less<float>>` label; none is a float heap.
- K020: `CTargetPoint_Copy_WithDestruction`, `NPlayerTargeting_CTargetPoint_Vector_Assign`, `std_vector_Erase_Range_FinishDynamicTextures`, `Vector_Move_Destructive`, `_InsertFillBackward_Pair` are behavioural/propagated; all five are vector::erase(first, last).
- K021: `vector_CopyAssignSegment`, `CTCSkeletalMorph_RangeAssign`, `Vector_Insert_CActionInputControl_RangeShift`, `CParticleMorphs::CopyAndConstruct`, `Container_MoveAndConstruct` are propagated; reloc_bindings' `?erase@CContainerItemVector@@` (0x0074FB20) and `CRetailVector<SPair<...>>` / `Holder::Assign` (0x00CBBDA0) are stand-ins.
- K022: all five carry the propagated `_Cons_val<allocator<pair<long const, CShopItem>>...>` label.
- K023: `QuestData::QuestData` x5 is propagated (FableWin.pdb's QuestData is NScript::CV_TalentlessBardScript's local type).
- K024: `Array_MoveBackwardCopy`, `vector<CBrainUpdateZone>::vector`, `Vector_Construct_CKeyCamera`, `std_vector_Init_CMapwhoLevelInfo`, `sub_00adc7c0` are propagated; reloc_bindings' `??0CGoodsPlaceVector@@` (0x00777430), `??0CScriptedMapCellVector@@` (0x00824DD0) and `??0CElement12_0076a030/0076a0a0@@` are stand-ins.
- K025: `Vector_Erase_TwoIntelligentPtr_v2/v3`, `CCreatureGroupCrowd_RemoveAt`, `CList_EraseElement`, `RBTree_RemoveNode` are propagated; reloc_bindings' `?erase@CSectionIndexMap@CodeSections@@` (0x009F0E80) is the landed caller's stand-in.
- K026: all five carry the propagated `CTCTavernGameCardBase::GetCurrentScore` (right only for 0x008F0A70).
- K027: `RBTree_Insert`, `RBTree_InsertNode`, `Map_InsertNewNode`, `std_set_insert_uint_pair` are behavioural; reloc_bindings' `?Insert@CRetailTree_007e9c00@@` / `CRetailTree_00a819b0` names (0x007E9B40 / 0x00A80C20) are stand-ins.
- K028: all five carry the propagated `CCreatureAction_AlertToPlayer::CCreatureAction_AlertToPlayer`; reloc_bindings' `??0CCreatureAction_LookThroughWindow@@QAE@AAVCThingAICreature@@@Z`, `??0CCreatureAction_AlertToPlayer@N_00842150/N_00933cc0@@...PAVCThingAICreature` and `??0DropGenericBox@Retail@@` are stand-ins (the real parameter is CThingCreatureBase&).
- K030: all five carry the propagated `vector<EWeaponClass>::_Umove<EWeaponClass*>`.
- K033: none (the doubled `CGameScriptInterface::CGameScriptInterface::` prefix aside). reloc_bindings' `g_gossipMap` / `g_tree_013bae2c` / `GossipMap` / `CBoolMap` / `?_M_find@?$map@...` spellings are stand-ins: the statics are CTCAIScratchpad::ActiveGossipCategories / IsGossipForPlayer / Gossip / GossipVillages / GossipFactions and 0x008AC200 is the _Rb_tree's _M_find.
- K034: all five carry the propagated `FindMostRecentMessage<CFilter_Bind3<IsOfType, HasExtraData, ...>>` label (right only for 0x008ABF20); bindings `CMessageEventManager_Retail`, `IsOriginatedByThing@N_006af2b0`, `IsForThing` are stand-ins.
- K035: all five carry the propagated `FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>>` label (Ego_r.pdb's CanBeSeenOrHeard instantiation is a different body); the reloc_bindings name of 0x008ABED0 with CanBeSeenOrHeard is wrong.
- K036: all five carry the propagated `CCombatAction_GenericResponseToDrain::CCombatAction_GenericResponseToDrain`; reloc_bindings' `??0CCreatureAction_AttackThing@@...PAV` (0x008AE160) and `??0CCreatureAction_SummonerChargeOutOf@@` (0x00970420) are wrong class names (the vtables' RTTI say InterruptableMidAttack / SummonerChargeSuccess).
- K038: `CCreatureAction_CastPhysicalShieldSpell::` (0x008B8A10), `CCombatAction_ControlledStrafeJump::` (0x008BEAB0), `CQuickAccessItemButtonGuiDef::` x2, `CMultiStaticMeshEntryDef::` are propagated; reloc_bindings' `??0CCreatureAction_CastPhysicalShieldSpell@@QAE@ABU0@@Z`, `??0CQuickAccessItemButtonGuiDef@@...`, `??0CMultiStaticMeshEntryDef@@...` for these addresses are wrong.
- K041: all five carry the propagated `CCombatAction_Guildmaster_HealFollowersInto::CCombatAction_Guildmaster_HealFollowersInto`; reloc_bindings' pointer-parameter spellings (`@QAE@PAVCThingCreatureBase@@@Z`) are stand-ins -- FableWin.pdb's are references. GetSoundPair 0x008561A0 is Ego_r.pdb's protected static `CCombatActionBase::GetSoundPair(const CThingCreatureBase&, const CCharString&)` (__fastcall); the landed H030 rows declare it as a free function.
- K044: all five carry the propagated `GetPBestThing<CIsFreeBonfire, CThingCompare_Nearest, CThingObject>`; reloc_bindings' `CThingFilter_DefName` / `CThingFilter_EscapePoint` / `PAPAVCThingPhysical` / `PAPAVCThing` spellings (0x00914330 / 0x009642D0) are stand-ins.
- K046: `sub_00918110`, `sub_00938e50` and three `_Find_if<list<CCountedPointer<CVillageTaskTorch>>, CIsThingInvolvedWithTask>` labels are propagated; reloc_bindings' `?FindIfCookingPotForMeal@@` (0x009465E0) is a stand-in.
- K047: four rows carry the propagated `CAIStateGroup_BriarRose::IsStillRunnable` (right only for 0x00956A70).
- K048: all five carry the propagated `CThingTrackNode::vector deleting destructor`; reloc_bindings' `?SubCleanup@@YIXPAVCInputProcessManager@@@Z` (0x00AF8860) / `@MemberDtor_686df0_00af8bc0@4` (0x00AF8BC0) are the CDynamicObjectHeap<156,2100> / <116,529>::Release bodies.
- K049: all five carry the propagated `_Move_backward<pair<unsigned __int64, CPersistInfo>*>` label.
- K050: all five carry the propagated `CLandscapeBackgroundPatch::vector deleting destructor`. 0x009FC550 is CResource::~CResource (CDefPointeeBase's base; bindings carry CMemoryBlock / CResource stand-ins).
- K051: all five carry the propagated `CAInputProcess::scalar deleting destructor`; 0x00AA75C0 / 0x00AA8490 are bound with stand-ins besides the real ~CChunkedFileChunk / ~CChunkedFileSuperChunk.
- K053: all five carry the propagated `C3DAnimFileXSequenceChunk::C3DAnimFileXSequenceChunk`; 0x00AA8360 is bound under six names in reloc_bindings (CBase_00aa8360, C3DMeshFileChunk, NativeSuperChunk, CChunkedFileSuperChunk QAE/IAE) -- Ego_r.pdb says ??0CChunkedFileSuperChunk@@IAE@PBD@Z.
- K054: all five carry the propagated `CQuadTreeElement::RemoveObjectCollections`. FableWin.pdb's large heaps are <172,1905> / <132,465> and Ego_r's <164,1998> / <124,495>; retail's strides say <156,2100> / <116,529> (the landed ctors agree) -- the template arguments differ per build.
- K055: all five carry the propagated `CAIStateGroupBase::IsStopping`; the PDB name is Alloc(long), not the landed 0x00B49660's `Allocate(unsigned long)` on a stand-in struct.
- K056: all five carry the propagated `CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite` (right only for 0x00BB1780).
- K057: all five carry the propagated `CLandscapeBackgroundPatch::vector_deleting_destructor`; reloc_bindings has 0x00BF73A0 as non-virtual `??1CLandscapeBackgroundTreeNode@@QAE@XZ`, FableWin.pdb says virtual UAE (slot 0 of its vtable is this ??_G).
- K058: the manifest labels (CWorldManager::OnShutdownAlt, MessageQueue_CheckAndProcess, PKCAutoDelete_AssertCheckedAndClear_*) are behavioural/propagated.
- K059: all five carry the propagated `CTCElectrocutedEffect::OnInitialActivate`.
- K060: propagated `CEditControlCentre::SetAtmosEditingMode` / `_gapscan::sub_00c15f30`.
- K061: manifest labels (CSBAgents_InvokeMethod, SteeringAgentList_Constructor, ...) are behavioural/propagated.
- K062: the manifest's five different labels (Audio_VectorMultiplyWithIncrementingCoeffs, Audio_ProcessSamples_IIRFilter, SIMD_AccumulateWeightedSamples_64bit, Filter_ApplyTransformSSE_64, SSE_MatrixMultiplyLonglong) name one kernel.
- K063: manifest `HeroAttribute_SetBitFlagBitN` are propagated game names; reloc_bindings still has agent443's `?Set...Flag@CEAXContext@@QAEJJJ@Z` for these five while sweep1's landed caller uses `?Set...@CEaxContext@@QAEJKJ@Z` -- rebind.
- K064: manifest `SetIntensity` / `CEntity_SetFloatProperty_0x4c..0x58` are propagated.
- K065: catalogued only as `_gapscan::sub_<addr>`.
- K066: catalogued only as `_gapscan::sub_<addr>`.
- K067: all five carry the propagated `CCOE_AltarEvil::Main`; CGameScriptInterface slot 7 is NewScriptFrame (cgsi_slots_aligned.tsv).
- K068: all five carry the propagated `CACompressor::CACompressor`.
- K070: all five carry the propagated `CLandscapeBackgroundSubNodeSplitBranch::scalar_deleting_destructor`; 0x00EF5B50's manifest label names the 4-deep And but its body (vtable 0x012E1B70, Right +0x14) is ~CPredicate_And<CIsThingAlive,CIsThingAlive>.
- K071: catalogued only as `_gapscan::sub_<addr>`.
- K072: all four carry the propagated `CUserProfileManager::GetMaxMeshDetail` (right only for 0x004085A0).
- K073: all four carry the propagated `_Copy_impl<std::pair<CCharString,CCharString>*, ...>`.
- K074: manifest `CPersistContext_TransferableComponent::vfunc_0/1`.
- K075: the landed TransferIn siblings carry T in their file names as long / unsigned long / int / EOpinionAttitudeType, but 0x004B8660's vtable belongs to Transfer<ENumAcceptedBoasts>, not int; all four TransferIn bodies are landed as the address-named stand-in CTransferableLong_009e2590::TransferIn (one name for four instantiations -- review).
- K076: all four carry the propagated `_Fill_n<unsigned long*, unsigned int, unsigned long>` (unsigned long is a POD: STLport would not null-test).
- K077: all four carry the propagated `CPersistTraits<float>::TransferBinaryIn`.
- K078: manifest `CPersistContext_TransferableVector::vfunc_3`.
- K079: behavioural manifest labels (Std_Vector_Insert_Range_4Bytes, Vector_InsertRange_uint, ...).
- K080: all four carry the propagated `CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>`; reloc_bindings holds stand-in forms for them (non-const QAE, reference out parameter, CDefinitionManager_Calls).
- K081: behavioural labels (Vector_FromRange, CHitParameters_CopyFromVector, Vector_CopyInit, vector<uint>); the landed 0x0082CAC0 "CEditPrimitiveStats::CEditPrimitiveStats" is the CMinimapData copy constructor.
- K082: the tree already holds these four as stand-ins (Global_GetMiscDirectoryA / GetGraphicsDirectory / GetCacheDirectory / GetShadersDirectory, `FableGet*_<addr>`); the manifest says CUserProfileManager::GetAutoSaveFileName.
- K083: all four carry the propagated `_global::GetSaveDir`.
- K084: manifest `BinarySearchTree_Find*` are behavioural.
- K085: behavioural manifest labels (StdMap_FindLE, BinarySearchTree_LowerBound, CDefStringTable_RBTree_Find, FindRegisteredCutsceneActor).
- K086: the manifest's `GetPBestThingInRoughArea<CThingFilter_HasTrackName, CThingCompare_Nearest, CThingTrackNode>` on all four was propagated across byte-identical bodies (no compare call; vector push_back). 0x00978250's stand-in `?GetAllThingsInRoughArea@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasScriptName@@AAUCThingPtrVector@@@Z` has the right function and filter, but the container is std::vector<CThing*>.
- K087: behavioural manifest labels (Vector_InsertWithMove, CVectorMap_InsertOrAssign, ...); the pair operator= callees 0x00448380.. are bound under stand-in names (FableHeap_00448950 pair<...>) and landed as `CItem::operator=`.
- K088: all four carry the propagated `_Sort_heap<pair<unsigned long, C3DHelperPoint>*, ...>` (a Dinkumware spelling of another element).
- K089: all four carry the propagated `CCarriedReadableDef::CCarriedReadableDef` (right only for 0x004D5ECA); 0x0044C0C0 is bound under nine names (CSubDefClassBase, CShoveHaPennyRegionDef, CDefBase, ...) -- the PDB's is ??0CSubDefClassBase@@QAE@XZ.
- K090: all four carry the propagated `CTattooDef::CTattooDef`; the map ctors 0x004534C4 / 0x00453447 / 0x00453536 / 0x00453554 / 0x004DF956 / 0x004DF86C are landed as `std::map<T_xxx>::map` / CDisplayManager::CopyBackBufferToTexture stand-ins and bound as CMember_00453447 / CDamageMap / CLongLongMap; 0x004538EB (landed as CApprenticeSpeedTest::OnPersist) is COpinionSet::COpinionSet().
- K091: propagated labels (CComponentContainer, CTCShotDefinitionNymphFun ctor) on three of the four.
- K092: propagated `CIntelligentPointer<const CThing>` / `C3DAnimFileHelpersChunk` constructor labels.
- K093: manifest `CCombatAbilityBlockDefBase::Constructor_CapacityN` (behavioural).
- K094: none (the manifest names are right); 0x00466A47 is bound as stand-in CStlVector/CLongVector Transfer overloads -- referenced here as ??$Transfer@V?$vector@J...@std@@@CPersistContext@@QAEXAAV...@Z.
- K095: behavioural manifest labels (CActiveFile_AllocateAndCopyVector, std_vector_allocate_*).
- K096: catalog label <long,long> is propagated; these maps are float-valued.
- K097: catalog `X::X::Copy` labels are right in class; 0x00431ED0 carries many propagated names (BaseCopy, Copy_00431ed0, ??4CBase_00431ed0) -- it is CParentDefClassBase::operator=.
- K098: `_global::Vector_Erase` / `std::vector_copy_12byte_norealloc` are behavioural; 0x00466DE5 is bound as the stand-in ?Erase@CRetailVector_elem12@@ by the landed resize 0x00466D9C -- this renames it.
- K099: `Vector_Erase` / `vector_copy_8byte*_norealloc` behavioural; 0x00469E3E is bound as stand-in ?erase_00469e3e@CArrayPair@@ (landed resize 0x00469E00) -- renamed here; the resizes' catalog label `CArray<pair<long,long>>::push_back` is propagated.
- K100: `Vector_Reallocate` / `vector_insert_reallocate_8byte*` are behavioural.
- K101: catalog `CArray<pair<long,..>>::push_back` is propagated. reloc_bindings `?resize@?$vector@VCMusicEntry@@@std@@QAEXI@Z` on 0x004E9F20 is WRONG -- it is vector<NParticleAttachment::CParticleAttachmentInfo>::resize(n) (CMusicEntry's resize(n) is 0x0046B65A). 0x00CD3CCB's stand-in names ?Resize@CThingRefVector@@ / ?resize@CScriptedThingVector@@ and the landed 0x00CD3DA4 "CThingRef" element are the scripted-thing-base vector.
- K102: `AbilityButton_AllocCopyInsert`, `*_vector_Reallocate`, `Vector_ReallocateAndCopy` are behavioural; 0x004EE137's binding spells it public (QAE) where STLport 4.6 and the K100 bindings have it protected (IAE); 0x004EB864 also carries the stand-in ??0CElement56_004eb864@@.
- K103: catalog `std::vector::insert` is generic; 0x00430300 carries 11 propagated names -- it is ~CParentDefClassBase (frees SubDefInfoMap +0x29, CDefPointeeBase vptr, jmp ~CResource); 0x00478722 is bound as the stand-in ?CAbilityTrackDef_Destruct@@YIXPAX@Z -- it is ~CAbilityTrackDef; 0x004787F6/0x004784B3 also carry ??0CElement72_* stand-ins.
- K104: `CCombatWheel::ResetRings` is propagated (J007); 0x0047BD70 / 0x006F3450 carry stand-ins ?_M_clear@CQuestObjectiveDescVector@@ / ?_M_clear@CSlotVector@@ -- they are vector<...>::_M_clear.
- K105: `Std_Vector_Insert_Range_Fill`, `std_vector_grow_ulong`, `Vector_InsertValue`, `Vector_Assign` are behavioural; 0x00485DBC / 0x00788FFC also carry stand-ins (?Grow@ChildVector@NUISystem@@, ?InsertOverflow@ScrollingArray@NUISystem@@, ?_M_insert_overflow@?$CRetailVector@..., ?_M_insert_overflow@ThingVector@@, ?_M_insert_overflow@?$CVector@...); 0x00405BA0 also carries ?ListBoxDataHelper@@YIKKKK@Z.
- K106: the catalog's `GetAllThings<..., CFilter_Bind2<CThingFilter_InMap, unary_negate<CThingFilter_InArea>>>` label is propagated onto all four -- each has a single filter; 0x0048D400 / 0x008ADBA0 carry stand-ins (?GetAllThings@CThingSearchTools@@QAEXABJ..., ?GetAllWithTC@ThingSearchMethods@@, ?GetAllThings@CThingSearchTools@@QAEXABVCThingFilter_HasDefGlobalIndex@@AAUCThingVector@@@Z).
- K107: `LinkedList_AppendNode` / `LinkedList_InsertNode` / `list::push_back_node` behavioural.
- K108: `Std_Vector_Insert_Range_SmartPtr`, `Vector_Resize_WithElements`, `vector_insert_range_CBaseIntelligentPointer`, `Vector_InsertRange_SmartPtr` behavioural; stand-ins on 0x004AC940 (?InsertOverflow@CHandleVector@@, ?_M_insert_overflow@?$CPointerVector@UCIntelligentPointerThing2@@, ?_M_insert_overflow@?$CRetailVector@...), 0x007AFD10 (?_M_insert_overflow@CCreaturePointerVector@@, CPointerVector<CIntelligentPointerCreature>), 0x007B99E0 (?InsertGrow@?$CPtrVector@...).
- K109: the catalog's `GetAllLeafThings<CThingAICreature, ..., CFilter_Bind2<CThingFilter_EnemyCreature, CThingFilter_InMap>>` is propagated onto all four; 0x004AD780 stand-in ?GetAllLeafThings@CThingSearchTools@@QAEXABVCThingFilter_HasTC@@AAU?$vector@PBVCThing@@@@@Z; 0x0049C780 also carries ?GetThingList@@YGPAUList@@H@Z.
- K110: catalog `QuestData::QuestData` is unsupported.
- K111: `map_find_lower_upper`, `Map_EraseRange_CCharString`, `map_Erase_Range`, `CThreadedFileManager_RemovePendingObject` are behavioural; stand-ins on 0x004BB270 (?erase@CCharStringSet@@, ?Assign@CountedPtr@@), 0x008AE420 (?Consume@GGlobal@@, ?RemoveGV@GObj@@, ?RemoveGossip@Gossip@@, ?remove@Mgr@@), 0x00990CD0 (?erase@CPendingMap@@).
- K112: catalog `GFToScreen` is wrong (propagated).
- K113: catalog `_Uninit_copy<_Vector_const_iterator<unsigned int>...>` is a propagated Dinkumware label; 0x009D23F0 / 0x009D3580 / 0x009D3A20 use the stand-in N009d3a20::Element -- it is CBankFile::CRuntimeData; 0x006B56F0 also carries ??$__uninitialized_copy@PBUSElement12@@.
- K114: catalog `list<CTCBase*>::remove` is propagated onto all four; stand-ins ?remove@CComponentListObject@@, ?remove@CThingPointerList@@, ?remove@?$list@PAVLSocket@@@@ (no std) are these std::list removes.
- K115: catalog `CIntelligentPointer<const CThing>::CIntelligentPointer` on three is wrong (they are pair copy ctors; the pointee is non-const CThing / CThingObject per RTTI).
- K116: catalog `list<CTCBase*>::remove` propagated onto three that are not; the landed 0x006404B0 comment claims list<CTCBase*> for 0x006405B0 -- that symbol is 0x004CC150's; stand-ins ?remove@TList@@, ?remove@CLongList@@.
- K117: catalog `swap<NParticleEngine::CRegisteredPSCInfo>` propagated onto all four (that class is 12 bytes).
- K118: catalog `_Destroy_range<allocator<CSpeechFrame>>` propagated onto all four; stand-ins ?DestroyRange@@YIXPAUCElement8b@@0@Z (0x004D0EE0), ?_Destroy@@YIXPAUSSlot@@0@Z (0x006F2B00).
- K119: catalog `CCreatureAction_PerformExpressionExtended::~...` propagated onto all four; the landed ??_G rows call stand-ins ?X_Destruct@@YIXPAX@Z (and 0x004DA967 is ?CTCActionUseBase_Destruct@@) -- they are ??1X@@UAE@XZ / ??1CTCActionUseBase@@UAE@XZ.
- K120: catalog `CCombatAction_GenericResponseToDecapitation::CCombatAction_GenericResponseToDecapitation` is propagated onto all four; the callees' ?Helper@CForwarder_*@@ stand-ins are __copy_backward.
- K121: `Vector_GrowReserve`, `std_vector_insert_pair_C3DVector_CCharString_range`, `CActiveFile_*` are behavioural; stand-ins ?InsertGrow@CRetailVector_CElement16_* on 0x004E3526 / 0x00656D5E / 0x00CD36DF, ??0CElement16_00654dd4@@ on 0x00654DD4 (= pair<C3DVector, CCharString> copy ctor).
- K122: catalog `_Sort_heap<pair<long, CDefPointer<const CContextSensitiveItemDef>>*, ...>` propagated onto all four.
- K123: `AllocateAndCopyPairStringDatetime`, `CShoveHaPennyRegionDef_AllocateCopyConstruct*`, `Pair_CCharString_CArray_AllocateCopyConstruct` are behavioural/propagated; 0x004EACBB / 0x004EAFE8 bind as X_004eaad0 / X_004eae1e namespace stand-ins.
- K124: `VectorReallocateCopyConstruct`, `Vector_InsertCopyRange_0x30`, `Vector_CopyElements_Destroy` are behavioural.
- K125: catalog `_Fill_n<unsigned long*, unsigned int, unsigned long>` is propagated onto all four.
- K126: `Vector_UninitializedCopy_Allocator`, `std_vector_Allocate_*` behavioural; stand-ins ??0CRetailVectorBase_CIntelligentPointer@@ / ??0CRetailVectorBase_CCountedPointer@@ on 0x0062ED90 / 0x0062EE20.
- K127: `PopulateRegionWithVillagers_RandChance`, `Array_RandomShuffle`, `Vector_RandomShuffle`, `Algorithm_FisherYatesShuffle` behavioural.
- K128: `InsertionSort_Compare`, `Sort_SiftDown_CThingComparator`, `std_sort_SiftUp_HeuristicBitch*` behavioural.
- K129: `AllocateAndInitMapInfoArray`, `Vector_AllocateNested`, `Vector_AllocAndInitVectorElements`, `C3DMeshInfo_AllocArray` behavioural; element copy ctors 0x00675230 / 0x00708830 carry only stand-ins (CElement12_*, CModifierEntryList).

## Callee names introduced
- K003: ?Reset@?$CCountedPointer@VCCreatureActionBase@@@@QAEXPAVCCreatureActionBase@@@Z (0x00905FD0, PDB), ?AddAction@CCombatSequenceBase@@QAEXABV?$CCountedPointer@VCCreatureActionBase@@@@@Z (0x008B0B10), ?OnCreate@CCombatSequenceBase@@UAEXXZ (0x008B0650, PDB), the actions' ??0X@@QAE@AAVCThingCreatureBase@@0@Z (PDB).

## Header debt
- K001: CEnginePrimitiveHandle (8 bytes) modelled opaque; CCharString as its PDB one-pointer layout (no engine/ headers exist for either).
- K014: engine/CScriptThing.h and engine/CScriptGameResourceObjectScriptedThingBase.h flatten the vptr into data members (unusable for a ctor that stores the vtable); local models from the PDB layout.
- K016: the same local retail-layout CThing / thing-map / search-tools models as the landed rows.
- K029: engine/CVillageTask{Torch,Horn,Patrol,PickCrops,CarryCrate}.h flatten the vptr and the intelligent pointers to bytes, so the classes are modelled locally (verifier SHARED_HEADER_AVAILABLE warning expected).
- K032: engine/CScriptThing.h / CGameScriptInterface.h flatten the vptr, so the classes stay local (as the landed J018 rows).
- K056: the parameter classes are modelled as empty `class Y : public CEngineInternalPrimitiveBase { };` (RTTI offset-0 derivation; members not touched) because the generated engine headers are flattened and cannot express the up-cast.
- K069: the ECutsceneBehaviour enumerator for 2 is unknown (cast).
- K097: engine/CParentDefClassBase.h lacks the polymorphic CDefClassBase base, so def classes still model the base locally.
- K117: CSoundBankRecord is a stand-in name with no PDB match (the pair<long, CSoundBankRecord> sort helpers 0x00992750.. use it); its real class is unidentified.
- K119: engine/CTCBase.h has no virtual destructor; modelled locally.

## Families
### K001 -- STLport _Rb_tree<K, pair<const K, V> >::insert_unique(iterator hint, const value_type&), frame/size TUs (11 of 11 matched, RELOCATION_MATCH)
- **Function:** the landed speed-TU hinted insert 0x00AD5030 (STLport 4.6 _tree.c text) compiled with `optimize("y", off)` + `("s", on)`; iterator ++/-- call the cdecl `_Rb_global<bool>::_M_increment` 0x004292C0 / `_M_decrement` 0x004294E2; `_M_insert` and `insert_unique(v)` out of line. The two helpers' STLport bodies must sit in the TU as noinline stand-ins: then VC7.1 knows they write no memory, keeps `this` in ebx and v's key in edi across the steps (without them: edi/ebx swapped, key reloaded, 25 bytes short).
- **Instances (all proven: each one's only caller is the bound map<K, V>::insert(iterator, const value_type&) forwarder):** `0042c1c1` <NUISystem::CManager::EUsedKeys, long>; `0042c300` <long, EEngine2DLayer>; `005940ec` <long, NUISystem::CComponent*>; `0059b35c` <FRONTEND_SCREEN, NUISystem::CComponent*>; `005b2a37` <long, long>; `005bb5ef` <EClothingSuitPart, long>; `005bb85a` <EClothingSuitPart, bool>; `005bbe32` <long, CEnginePrimitiveHandle>; `005f9714` <CTCInventoryAbilities::EUISpellCategory, NUISystem::CComponent*>; `005f9853` <EHeroAbility, NUISystem::CComponent*>; `00cd34c5` <long, CCharString>.
- **Label corrections:** the manifest's `CMemoryAllocatorVariableSize_FindInsertPointLeft`, `CMemoryAllocatorVariableSize_InsertOrFind_ScriptRight`, `RBTree_TransferAllocateInt` and kin are behavioural guesses; these are map hinted inserts.
- **Header debt:** CEnginePrimitiveHandle (8 bytes) modelled opaque; CCharString as its PDB one-pointer layout (no engine/ headers exist for either).
### K002 -- CTCInventoryItem::IsX(long defIndex) statics (7 of 7 matched, RELOCATION_MATCH)
- **Function:** `if (CacheDef(i)) { CDefPointer<const T> sub; return s_CachedThingDef->GetSubDef(&sub); } return false;` (the landed GetContextSensitiveType 0x005D9B90 model, with GetSubDef<T>'s same-TU noinline stand-in body -- needed for the in-memory `dec [ecx+4]` release).
- **Instances (FableWin.pdb names; sub-def class from the GetSubDef callee's name string):** `005d97f0` IsWeapon (CWeaponDef); `005d9830` IsExpression (CExpressionSubDef); `005d9870` IsAugmentation (CAugmentationDef, proven by binding); `005d98b0` IsStockItem (CStockItemDef); `005d98f0` IsHeroTitle (CHeroTitleDef); `005d9930` IsTrophy (CTrophyDef); `005d9970` IsBook (CCarriedReadableDef). Grades: IsAugmentation proven, the rest probable (name-to-type pairing).
- **Label corrections:** catalog `_global::IsAugmentation` propagated onto all seven; 0x005D97F0 bound ?IsAugmentation@@YI_NJ@Z / ?IsWeaponItem@@ -- it is IsWeapon; the other stand-ins (?IsTutorialSubDefItem@@, ?IsAugmentation_005d98f0@@) likewise.
### K003 -- two-action combat-sequence OnCreate overrides (7 of 7 matched, RELOCATION_MATCH)
- **Function:** one CCountedPointer<CCreatureActionBase> local: `action = new A(GetCreature(), GetTarget()); AddAction(action);` twice, then CCombatSequenceBase::OnCreate(); the local's release inline. The landed one-action 0x009062A0 shape retyped with PDB names; the inline GetCreature/GetTarget accessors are needed (reading the members directly flips the second AddAction's address register).
- **Instances (proven: vtable slot 1 + FableWin.pdb OnCreate names; actions by RTTI and PDB ctors):** `009061e0` CCombatSequence_BanditAdvanceFar (StrafeForward x2); `00907e00` CCombatSequence_TrollRockThrowRock (TrollGetProjectile, TrollThrowProjectile 0x130); `00908810` CCombatSequence_VillagerBackOff (VillagerBackOff x2); `00908960` CCombatSequence_VillagerAdvance (VillagerAdvance x2); `00908f20` CCombatSequenceWhisperStrafeAround (StrafeBackward, StrafeForward); `0090a340` CCombatSequenceNonInterruptable2StrikeCombo and `0090a400` CCombatSequence2StrikeCombo (MidAttackFacing x2).
- **Callee names introduced:** ?Reset@?$CCountedPointer@VCCreatureActionBase@@@@QAEXPAVCCreatureActionBase@@@Z (0x00905FD0, PDB), ?AddAction@CCombatSequenceBase@@QAEXABV?$CCountedPointer@VCCreatureActionBase@@@@@Z (0x008B0B10), ?OnCreate@CCombatSequenceBase@@UAEXXZ (0x008B0650, PDB), the actions' ??0X@@QAE@AAVCThingCreatureBase@@0@Z (PDB).
- **Label corrections:** existing bindings for 0x00905FD0 / 0x008B0B10 / 0x008B0650 and the action ctors are stand-ins (CCountedPointer non-template, CCombatSequenceBase_008b0650, CCombatAction_<addr>, pointer-parameter ctors).
### K004 -- 10-byte `mov ecx, offset global ; jmp member` forwarders (6 of 6 matched, RELOCATION_MATCH)
- **Function:** a forwarder whose only work is a member call on a global object (tail jump).
- **Instances:** `01229170` the dynamic-atexit destructor of CTCTavernGame::GetTextWithNarrator's static `no_text_entry` (probable: bound data symbol ?no_text_entry@?1??GetTextWithNarrator@...; emitted as the TU-local `_$E2` in .text$yd); `0098ec90` CThreadedFile::RemoveObject(unsigned long) (proven: PDB name, GetManager() inline, target CThreadedFileManager::RemoveObject 0x0098EB20); `0048573d` CUnprovenForwarder_0048573d (descriptive: clears the list at 0x013B8A98); `00c33600`, `00c33820`, `00c33990` CUnprovenForwarder_<addr>(void*) (descriptive: __stdcall one-argument forwarders to members of the global 0x0143BB58).
- **Label corrections:** `_Cons_val<...CParticleMorphs::CEntry...>` is propagated onto five; 0x0098EC90's binding ?HasReadFinished@CThreadedFile@@ is wrong (RemoveObject); 0x01229170's `CCharString_Destructor_Stub3` is a CWideString static's atexit destructor.
### K005 -- CTCVillage::RemoveThingFromTask<T>(CIntelligentPointer<CVillageTaskBase>, const CThing&, list<CCountedPointer<T> >&) const (0 of 6 matched, parked)
- **Function:** `if (task->IsThingInvolvedWithTask(thing)) { bool remove; task->RemoveThingFromTask(thing, remove); if (remove) tasks.erase(find_if(begin, end, CIsTask(task))); }` (frame/size TU), find_if / erase out of line.
- **Remaining difference:** one operand -- retail homes both the find_if result and erase's discarded result in the dead `thing` slot ([ebp+10h]); the rebuilt TU, which only stops eliding the find_if temporary when it is a named local, puts erase's result in the `tasks` slot ([ebp+14h]). Unnamed (elided into the argument), implicit iterator copy, explicit conversion, assignment forms, predicate local, `it = erase(it)` tried.
- **Instances (proven, bound names; T = CVillageTaskTorch / Horn / Patrol / PickCrops / SecuritySweep / CarryCrate):** `00660ae2`, `00660b5d`, `00660bd8`, `00660c53`, `00660cce`, `00660d49`. Predicate CIsTask (FableWin.pdb find_if<..., CIsTask>; Ego_r.pdb CIsTask::Task).
- **Label corrections:** catalog `RemoveThingFromTask<CVillageTaskPickCrops>` propagated onto all six; the bound names' list parameter is the landed dispatcher's CTaskList stand-in (std::list<CCountedPointer<T> > in reality).
### K006 -- unreferenced x87 10^x kernels (0 of 6 matched, parked)
- **Function:** `10^x` by the x87 split (n = frndint(x*log2 10), 2^n by fscale, 2^frac by f2xm1 + 1, explicit fwait), constants staged in its frame.
- **Why parked:** an inline-assembler kernel; VC7.1 C++ has no frndint / f2xm1 / fscale form (pow compiles to a CRT call). The differ copy is a pow() model (15 v 75 bytes).
- **Instances (descriptive, six byte-identical copies in the audio-library TUs, no reference):** `00c40e60`, `00c42c30`, `00c54a80`, `00c5c0b0`, `00c6bf60`, `00c77690`.
- **Label corrections:** `Pow2_Float` / `MathUtils_Pow2*` / `Math_Pow2_Fast` -- it is 10^x (the log2(10) constant 0x40549A78), not 2^x.
### K007 -- STLport deque<T>::~deque() (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `_Destroy(_M_start, _M_finish)` (__forceinline, iterators by value with STLport's user copy ctor, the tag with a user ctor) -> out-of-line __fastcall __destroy_aux; then the inline ~_Deque_base (landed 0x0048CAF0 body).
- **Instances:** `004427d0` CUnprovenElement_004427d0 (descriptive: 0x1C bytes, four CWideStrings); `004432b0` pair<CCharString, long> (proven, bound); `0048d520` C2DVector (proven: landed _M_destroy_nodes + bound __destroy_aux); `007b4d60`, `00913cc0` (descriptive).
- **Label corrections:** catalog `CBankFileAsyncData::FreeBuffer` propagated onto all five; _M_destroy_nodes stand-ins ?Release@T@@ / ?Release_<addr>@T@@ (0x0043F510, 0x0043F560, 0x007B3B50, 0x00913690); 0x0048D520 also ?DestructMember_0048d520@@, 0x007B4D60 ??1CBuffer_007b4d60@@.
- **Naming note:** the binding for 0x004432B0 spells deque with one template parameter; this source uses STLport's deque<T, allocator<T> > (see review).
### K008 -- STLport __uninitialized_fill_n (__false_type) over CIntelligentPointer<T> (5 of 5 matched, RELOCATION_MATCH)
- **Function:** null-tested placement copy loop with the intelligent pointer's copy ctor inline; the class's user-declared virtual dtor (landed 0x00739550 model) is what gives retail's register assignment.
- **Instances (proven: stored RTTI vtable):** `004aa190` CThing; `007377a0` const CThing (one-parameter template spelling, as bound); `0074cd10` CThingCreatureBase; `007b6d70` CThingMarker; `007b8230` CThingObject.
- **Label corrections:** catalog `_Fill_n<unsigned long*, ...>` propagated onto all five; 0x0074CD10 bound only as the non-template stand-in ?__uninitialized_fill_n@@YI...; 0x004AA190 also as ?FableUmove_construct_0065574c@@.
### K009 -- STLport _Rb_tree::erase(const key_type&), speed TU (0 of 5 matched, parked)
- **Function:** equal_range (out of line, hidden result), cdecl __distance, erase(first, last); the landed G053 0x0042CDE0 body without the size pragma.
- **Remaining difference:** one pair swapped -- retail loads `first` (`mov ecx,[esp+10h]`) before storing `last` into its argument slot (`mov [eax],edx`); the rebuilt TU stores first. Tried: user/implicit/base-init iterator copy ctors, pair copy ctor, iterator locals, out-of-class erase(key), size mode (adds a frame), QFE cl -- none moves it.
- **Instances:** `0051b880` (descriptive; reached via the jump thunk 0x0051C060 beside map<CThingBuilding*, list<CThingAICreature*>>), `0052afb0` <unsigned __int64, pair<const unsigned __int64, CThing*> > (probable), `00639590` set<long> (probable), `006f50b0` <unsigned __int64, pair<const unsigned __int64, CCombatantInfo> > (probable), `00777b30` (descriptive).
- **Label corrections:** `BinaryTree_DeleteNode_Chain` / `Crime_RemoveEntry` / `BinaryTree_Erase_helper` behavioural; stand-ins ?erase@CUIDToThingMap@@, ?erase@CCrimeSet@@, ?erase@CCombatantMap@@.
### K010 -- std::__copy(first, last, result, const random_access_iterator_tag&, int*) over 4-byte intrusive counted pointers (5 of 5 matched, MATCH)
- **Function:** STLport random-access `__copy` (/Gr `__fastcall`, `ret 0Ch`) whose element assignment is the intrusive pointer's inlined operator= (self test, release, clear, copy, add-ref).
- **Representative:** 0x0069CB80, 2 grades. **Shape:** the release must go through an inline pointee member `Release() const { if (--RefCount == 0) Destroy(); }` -- written in the pointer's operator= as `--Object->RefCount` it re-reads the member and loses `dec [ecx+4] ; jne`.
- **Recipe (`gen/gen_K010.py`):** element type and iterator const-ness from each instance's callers (erase(begin(), end()) = non-const, vector::operator= = const first) and the owning member's PDB type at its retail offset.
- **Instances:** `0069cb80` CDefPointer<const CCameraModeDef> (CGameCameraManager CameraList/SpecialityCameraList clear); `0071f430` const CDefPointer<const CThingBaseDef>* first (vector::operator= 0x00721150, CTCHeroMorph::Tattoos); `0071fb80` CDefPointer<const CTattooDef> (CTCHeroMorph::GetTattoos 0x0071DD50); `009ca6c0` NGameText::CDataBankGroupEntry (CDataBank::CloseTextAndGroups 0x009C8660, GroupEntryBuffer; PDB element = {CIVCountedPointer<CGroupEntry> PGroup}, 4 bytes); `00a964c0` CIVCountedPointer<N3DPrimitive::CTriangleList> (N3DPrimitive::CDeviceIndependent::ClearTriangleLists 0x00A980C0, TriangleLists +0x10). All proven.
- **Label corrections:** the five manifest labels (`_Move<CDefPointer<CMeleeCombatAbilityDef const>*,...>` x4, `_gapscan::sub_00a964c0`) are propagated; all are STLport `__copy` instantiations of the element types above.
### K011 -- `scalar deleting destructor' of a one-pointer intrusive counted wrapper (5 of 5 matched, RELOCATION_MATCH)
- **Function:** compiler-generated ??_G: the wrapper's inline destructor (one load of Object, `dec [ecx+4] ; jne ; call [vt+4]`, clear) then `if (flags & 1) operator delete(this)`.
- **Representative:** 0x0069CC00, grade 1 from agent952's landed H021 model (0x00708DA0); `delete p` in a helper makes VC7.1 emit the COMDAT.
- **Identity:** all five bodies are unreferenced (no call/jump/data xref), so the template argument is unprovable: `CIVCountedPointer<CUnprovenPointee_<addr>>` (descriptive), each header records its landed neighbours. Ego_r.pdb folds this body under ??_G?$CDefPointer@$$CBVCHeroAbilityDef@@@@ (the propagated manifest label); retail has no ICF.
- **Instances:** `0069cc00`, `00710280`, `007ca030`, `008699f0`, `00b83260` (all descriptive). The behaviour tests exercise the model through real `delete` expressions (not a re-implementation).
### K012 -- std::pair<K, V>::operator=(const pair&), compiler-generated, emitted out of line for the CVectorMap entry sorts (5 of 5 matched, RELOCATION_MATCH)
- **Function:** copy the key, then CCountedPointer's operator= (both source words read first, release with the post-decrement re-read, `__fastcall` deleter on the block's data, free the block, adopt + add-ref).
- **Representative:** 0x006B8E20, grade 1. **Shape:** the implicit pair operator= is emitted by taking its address in a non-const global member pointer (GOTCHAS 1255/1463); CCountedPointer is the Ego_r layout {Data, CCPPointerInfo* Info} with Info {RefCount, DeleteFunc, Data}.
- **Recipe (`gen/gen_K012.py`):** each instance's callers are the __adjust_heap / __linear_insert of one landed STLport sort family whose __pop_heap / __make_heap names the element type.
- **Instances:** `006b8e20` pair<unsigned long, CCountedPointer<CFaction> >; `00825090` pair<long, CCountedPointer<CContainmentVolume> >; `00827f80` pair<long, CCountedPointer<CRegionConnectionNode> >; `00ae98e0` pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> >; `00bed640` pair<long, CCompositeSurfaceInfo> (the second member is polymorphic: +4 vptr never assigned, Cache at +8). All proven.
- **Label corrections:** the manifest label `CItem::operator=` is propagated to all five; none involves CItem.
### K013 -- std::__unguarded_insertion_sort_aux<pair<K, CCountedPointer<V> >*, ..., CKeyPairCompareLess<..> > (5 of 5 matched, RELOCATION_MATCH)
- **Function:** STLport final-insertion-sort pass: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` (/Gr `__fastcall`, `ret 8`), the by-value pair copy built in the argument area through CCountedPointer's inline add-ref copy constructor.
- **Representative:** 0x006B9ED0, grade 1 (GOTCHAS 1474 form; comparator operator() declaration-only, __unguarded_linear_insert declared out of line with the landed J050 signature).
- **Instances (element from the landed agent953 J050 / chunk __unguarded_linear_insert each one calls):** `006b9ed0` pair<unsigned long, CCountedPointer<CFaction> > (0x006B8F40); `00825b40` <long, CContainmentVolume> (0x008251B0); `00828d80` <long, CRegionConnectionNode> (0x008280A0); `00a80ff0` <long, CNavConnectorNode> (0x00A7FF40); `00ae9db0` <long, NParticleEngine::CParticleEmitter> (0x00AE9A40). All proven.
- **Label corrections:** `_Copy_impl<CNavigationPosition*,CNavigationPosition*>` is propagated to all five (Dinkumware spelling; no copy, no CNavigationPosition).
### K014 -- default constructors of polymorphic classes whose only initialised member is an embedded CCountedPointer (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `X::X() { }`: out-of-line base ctor, own vtable store, the embedded CCountedPointer's inline default ctor (Data/Info through one zero register).
- **Representative:** 0x006E7B40 CScriptThing::CScriptThing(), grade 1.
- **Recipe (`gen/gen_K014.py`):** class from the RTTI of the stored vtable, base chain from ghidra_out/rtti_bases.tsv and the called base ctor, member name/type/offset from Ego_r.pdb; CScriptGameResourceObjectBase is a `novtable` intermediate whose inline ctor's vtable store is dead.
- **Instances:** `006e7b40` CScriptThing (PImp +4); `006e7b60` CScriptGameResourceObjectMovieBase (PImp +8); `007e72a0` CScriptGameResourceObjectScriptedThingBase (PImp +8); `00a0bd00` CASoundBank (PSymbolMap +4, no xrefs); `00a95fe0` C3DClothPrimitive (ParticleProgram +4, PrimitiveIndex/MaterialIndex left uninitialised; no xrefs). All proven (vtable RTTI + PDB ctor/member).
- **Label corrections:** `CCarriedReadableDef::CCarriedReadableDef` is propagated to all five.
- **Header debt:** engine/CScriptThing.h and engine/CScriptGameResourceObjectScriptedThingBase.h flatten the vptr into data members (unusable for a ctor that stores the vtable); local models from the PDB layout.
### K015 -- `Member = CIntelligentPointer<T>(0);` member clears (5 of 5 matched, RELOCATION_MATCH)
- **Function:** clear one CIntelligentPointer member through a converting frame temporary (base ctor 0x00A01B10, the temporary's CIntelligentPointer<T> vtable, SetPItem(0), inline operator= `SetPItem(other.GetPItem())`, base dtor 0x00A01C10).
- **Representative:** 0x006E8B10, grade 1 (the landed 0x0091D850 / 0x006E8B60 model with namepass F4's canonical CBaseIntelligentPointer).
- **Instances:** `00935a80` CAIStateGroup_MinionPutOutFires::Cleanup (PBuildingOnFire, proven); `009361c0` CAIStateGroup_MinionWatchFires::Cleanup (PBuildingOnFire, proven); `0093ca60` CAIStateGroup_NonVillageGuardPatrol::Cleanup (CurrentGuardpostThing, CIntelligentPointer<const CThingTrackNode>, proven) -- vtable RTTI slot 9 + PDB Cleanup + PDB member at +0x14; `006e8b10` CDefendingCombatantInfo::CProng::Reset() (probable: no xrefs; the PDB's only no-arg CProng mutator, and the landed CCombatRing::Reset 0x006E8B60 runs this exact body inlined on each prong); `0091c030` CTCNymph::ClearTarget() (probable: no xrefs; PDB ClearTarget + PTarget CIntelligentPointer<CThingCreatureBase> at PDB +0x6C = retail +0x68).
- **Label corrections:** 0x006E8B10 and 0x0091C030 were labelled CAIStateGroup_MinionPutOutFires::Cleanup (propagated): they are CProng::Reset and CTCNymph::ClearTarget.
### K016 -- CThingSearchTools::GetAllThingsInRoughArea<TThing, std::vector<P*>, CThingFilter_None> (0 of 5 matched, parked)
- **Function:** the landed list model 0x00937080 with an STLport vector push_back (noinline same-TU `_M_insert_overflow` stand-in as in the landed GetAllThings 0x005198B0). Every byte matches except one SIB operand order in the mapwho cell index: retail `mov ecx,[ecx+eax+4]` (the hoisted level*20 as base) where the lane emits `[eax+ecx+4]`. 13 forms tried (index expression spellings, a reference/local for the level info, parameter/level types, const method, the push temporary, QFE toolset).
- **Instances:** `006f3570` <CThingObject, vector<const CThing*> > (probable: mask 0x807, overflow 0x0049C370 = vector<const CThing*>'s, FableWin name); `006f37b0` <CThingBuilding, vector<const CThing*> > (probable: mask 0x47, same overflow, Ego_r/FableWin name); `007197c0` <CThingCreatureBase, vector<CThingCreatureBase*> > (proven: mask 0xF, the PDBs' only such instance); `00959380` <const CThingObject, vector<const CThingObject*> > (probable: mask 0x807, a third overflow copy 0x0091FFE0); `0095c7c0` <CThingObject, vector<CThing*> > (probable: mask 0x807, overflow 0x00436AC0 = vector<CThing*>'s, FableWin name).
- **Label corrections:** the manifest's `GetPBestThingInRoughArea<CThingFilter_HasTrackName, CThingCompare_Nearest, CThingTrackNode>` / `<CThingFilter_IsPotentialCustomer, ..., CThingCreatureBase>` labels on all five were propagated across byte-identical bodies. These functions have no filter or compare call: they are GetAllThingsInRoughArea vector collectors (`ret 0Ch`, push_back + `_M_insert_overflow`).
- **Header debt:** the same local retail-layout CThing / thing-map / search-tools models as the landed rows.
### K017 -- CTCx::CTCx(CThing& thing) : CTCBase(thing), Member(0) (5 of 5 matched, RELOCATION_MATCH)
- **Function:** thing-component constructor: CTCBase(CThing&) 0x00686800, own vtable, one zero store (scalar init-list zero or an embedded CDefPointer's inline default ctor).
- **Representative:** 0x00702AA0, grade 1 (agent952 H073 shape, here in speed TUs: no pragma).
- **Instances:** `00702aa0` CTCDayOrNightOnlySupport (DayNightOrBoth +0xC); `007cbda0` CTCGold (Gold +0xC); `007e3020` CTCCameraCollision (CameraCollisionDef CDefPointer +0xC); `007eebe0` CTCSmashableBase (protected ctor; Definition CDefPointer +0x14); `008ec850` CTCDegradableBase (protected ctor; PDefinition CDefPointer +0x10). All proven: vtable RTTI, FableWin.pdb ctor symbol, landed Alloc / derived-ctor callers, Ego_r member at the zeroed offset.
- **Label corrections:** `CTCCameraCollision::CTCCameraCollision` is propagated to all five; only 0x007E3020 is CTCCameraCollision's ctor.
### K018 -- std::list<CCountedPointer<T> >::erase(iterator), speed TUs (5 of 5 matched, RELOCATION_MATCH)
- **Function:** STLport single-node erase with CCountedPointer's destructor inlined (release with the post-decrement re-read, `__fastcall` deleter, free the block, zero both words), free(node), iterator(next) through the hidden pointer.
- **Representative:** 0x00719210, grade 1 (agent952 H020's erase text; the speed-TU difference is only the inline element destructor).
- **Instances:** `00719210` CTCScriptedControl::CActionBase (PActionList; proven); `00aec7c0` NParticleEngine::CParticleEmitter (CParticleEmitterList::Active; proven); `009aaae0` NBankFileManager::CRetailBank (probable: only reached from the unreferenced pop_back 0x009AAB40, next to the proven push_back 0x009AAAA0 of CBankFileManager::RetailBanks); `009d7c00` CBankFileAsyncData (probable: only reached from the unreferenced pop_front 0x009D7F00 between the proven push_back 0x009D7B00 / clear 0x009D7DB0 of CBankFileAsync::DisposalList); `00aacbb0` CUnprovenPointee_00aacbb0 (descriptive: CSoundEmitterLH's sound list; no PDB names the element).
- **Label corrections:** `list<CCountedPointer<NCreatureMode::CModeHoverBase::CHoverAnim>::erase` (x3) and `_gapscan::sub_*` (x2) are propagated; only the element types above are real. Landed callers bind 0x00719210 as `CActionList::erase`, 0x00AACBB0 as `CSoundList::erase`, 0x00AEC7C0 as `CEmitterList::EraseOutOfLine` (stand-ins; review).
### K019 -- std::__push_heap<T**, int, T*, Compare> over thing-pointer heaps (5 of 5 matched, RELOCATION_MATCH)
- **Function:** STLport 4.6 heap sift-up, /Gr `__fastcall` (first ecx, holeIndex edx), comparator by value on the stack (`ret 0Ch/14h/18h` = 8 + sizeof(Compare)).
- **Representative:** 0x00748880, grade 1 (comparator declaration-only; the 12/16-byte comparator instances use the same-TU noinline stand-in body the landed sibling sort rows compile).
- **Instances (all proven from their only caller, the landed/sibling __adjust_heap):** `00748880` CThing* / CCompare_FurthestFromInitPos2D; `007a30b0` CThing* / CThingCompare_NearestToPos; `0091f870` const CThingObject* / CThingCompare_Nearest (T*-returning comparator); `0095cdf0` CThing* / CThingCompare_Nearest; `00978a00` CThing* / (anonymous namespace)::CThingCompare_InDirectionOfAndNearestToHeuristicBitch.
- **Label corrections:** all five carry the propagated `_Push_heap<float*,int,float,std::less<float>>` label; none is a float heap.
### K020 -- std::vector<T>::erase(iterator first, iterator last), non-trivial elements (5 of 5 matched, RELOCATION_MATCH)
- **Function:** STLport 4.6 range erase: out-of-line /Gr `__copy` of the tail, `__destroy_aux` loop of direct destructor calls, finish = copy end, return first.
- **Representative:** 0x00749280, grade 1 (the landed H009 erase 0x00AD7C20 model with the element's real destructor; an implicit element destructor reaching CBaseIntelligentPointer's virtual dtor compiles to the direct call).
- **Instances (all proven):** `00749280` pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> (CTCStealth::DangerThings); `00808520` NPlayerTargeting::CTargetPoint; `0082cee0` CMinimapData (CMiniMapDisplay::MinimapList); `00a4a320` CLipSyncPhoneme; `00ad8170` QuadricOptimiserInternals::COptimisedVertex.
- **Label corrections:** `CTargetPoint_Copy_WithDestruction`, `NPlayerTargeting_CTargetPoint_Vector_Assign`, `std_vector_Erase_Range_FinishDynamicTextures`, `Vector_Move_Destructive`, `_InsertFillBackward_Pair` are behavioural/propagated; all five are vector::erase(first, last).
### K021 -- std::vector<T>::erase(iterator first, iterator last), member destructor at an offset (5 of 5 matched, RELOCATION_MATCH)
- **Function:** K020's STLport range erase where the element's implicit destructor is one member's destructor (`lea ecx,[esi+N]`).
- **Representative:** 0x0074FB20, grade 1 (K020 model reused).
- **Instances:** `0074fb20` pair<long, CCharString> (container items; proven); `00993030` pair<long, CSoundBankRecord> (probable: the CASoundManager bank vector; CSoundBankRecord is the landed descriptive record name); `00a33510` C3DGeneratorObject (proven); `00bf9420` CEngineTexturePalette::CPaletteEntry (proven); `00cbbda0` pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (proven).
- **Label corrections:** `vector_CopyAssignSegment`, `CTCSkeletalMorph_RangeAssign`, `Vector_Insert_CActionInputControl_RangeShift`, `CParticleMorphs::CopyAndConstruct`, `Container_MoveAndConstruct` are propagated; reloc_bindings' `?erase@CContainerItemVector@@` (0x0074FB20) and `CRetailVector<SPair<...>>` / `Holder::Assign` (0x00CBBDA0) are stand-ins.
### K022 -- std::__uninitialized_fill_n / uninitialized_fill_n over POD (key, record) pairs (5 of 5 matched, MATCH)
- **Function:** STLport 4.6 placement fill, /Gr `__fastcall`, unsigned n, placement-new null test, STLport's memberwise pair copy constructor (key, then the record as one `rep movsd`).
- **Representative:** 0x00758720, grade 1. The `ret 4` rows are the tag dispatcher `uninitialized_fill_n(first, n, x)` with the worker inlined -- the worker must also be `__fastcall` for VC7.1 to inline it (a cdecl worker stays an out-of-line call).
- **Instances (all proven from the landed callers):** `00758720` / `007587e0` pair<EBodyLocation, CWoundMorphs::CEntry> (worker / dispatcher); `0081a2b0` / `0081a530` pair<EHeroMorphType, CParticleMorphs::CEntry> (worker / dispatcher); `00a320a0` pair<unsigned long, C3DDummyObject> (worker).
- **Label corrections:** all five carry the propagated `_Cons_val<allocator<pair<long const, CShopItem>>...>` label.
- **Binding note:** reloc_bindings names 0x00758720 / 0x007587E0 with a one-parameter template (`??$__uninitialized_fill_n@U?$pair@...@std@@@std@@`, the landed caller's same-TU stand-in); STLport's real three-parameter form is used here (as 0x0081A2B0's binding already has).
### K023 -- default constructor of an 8-byte {CCharString, CCharString} record (5 of 5 matched, RELOCATION_MATCH)
- **Function:** two CCharString default constructors (0x0099E4B0) at +0/+4, returns this; unreferenced inline COMDATs, each followed by its two-string destructor.
- **Representative:** 0x00765D00, grade 1.
- **Instances (all descriptive, `CUnprovenRecord_<addr>`):** `00765d00`, `00768b80` (object-augmentation TUs), `009f0060`, `009f00d0` (console / code-section TU), `00cd45a0` (C3DAnimationUtil / persist-context TU). Ego_r.pdb has three 8-byte two-string records (pair<CCharString, CCharString>, CBankCreatorException, CV_TalentlessBardScript::QuestData) and nothing here picks one.
- **Label corrections:** `QuestData::QuestData` x5 is propagated (FableWin.pdb's QuestData is NScript::CV_TalentlessBardScript's local type).
### K024 -- std::vector<T>::vector(const vector&), non-power-of-two elements (5 of 5 matched, RELOCATION_MATCH)
- **Function:** STLport 4.6 copy constructor: reciprocal-divide size(), allocate, out-of-line /Gr `__uninitialized_copy<const T*, T*>`.
- **Representative:** 0x0076A030, grade 1 (the landed J011 0x00AD26D0 model; the element only needs its retail size since the copy is out of line).
- **Instances (all proven):** `0076a030` AttackParticleAndEndPoints; `0076a0a0` ParticleAndEndPoints; `00777430` CTCShop::GoodsPlace; `00824dd0` CScriptedMapBrush::CScriptedMapCell; `00adc7c0` CVertexGroupBlock.
- **Label corrections:** `Array_MoveBackwardCopy`, `vector<CBrainUpdateZone>::vector`, `Vector_Construct_CKeyCamera`, `std_vector_Init_CMapwhoLevelInfo`, `sub_00adc7c0` are propagated; reloc_bindings' `??0CGoodsPlaceVector@@` (0x00777430), `??0CScriptedMapCellVector@@` (0x00824DD0) and `??0CElement12_0076a030/0076a0a0@@` are stand-ins.
### K025 -- STLport map / _Rb_tree erase(iterator) with a two-member mapped value (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `_Rebalance_for_erase` 0x0042A34E, the value's implicit destructor (two member destructors in reverse), null-tested free, --count.
- **Representative:** 0x007921C0, grade 1 (a trimmed STLport 4.6 tree; map::erase(it) = `_M_t.erase(it)` inlined is byte-identical).
- **Instances (all probable -- container proven, map-vs-tree wrapper level not pinned):** `007921c0` (unreferenced twin, modelled as _Rb_tree::erase) / `00792640` (called, map::erase) over map<CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo>; `00797760` / `00797a60` the same pair over CTCUnholyPower::PerCreatureInfo; `009f0e80` map<unsigned long, CCodeSectionManager::CSectionDesc>::erase (RemoveSection's index map).
- **Label corrections:** `Vector_Erase_TwoIntelligentPtr_v2/v3`, `CCreatureGroupCrowd_RemoveAt`, `CList_EraseElement`, `RBTree_RemoveNode` are propagated; reloc_bindings' `?erase@CSectionIndexMap@CodeSections@@` (0x009F0E80) is the landed caller's stand-in.
### K026 -- tavern-game score texts with one decimal (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `GetText(def->X) + CCharString::FormatDouble(1, member).ToWideString()` -- the landed CTCTavernGameCoinGolf::GetCurrentScore 0x007D2B70 expression with `mov edx,1`.
- **Representative:** 0x007D53A0, grade 3 (CTCTavernGame must sit on a vptr-holding CTCBase: a vptr introduced in the 8-aligned class itself is padded to 8 by VC7.1 and shifts every member by 4).
- **Instances (all proven: vtable slot + Ego_r.pdb name):** `007d53a0` CTCTavernGameCardSorting::GetCurrentScore, `007d6670` CTCTavernGameCardPairs::GetCurrentScore, `008f0a70` CTCTavernGameCardBase::GetCurrentScore (Counter +0x154); `007d7d60` CTCTavernGameSpotTheAddition::GetCurrentScore (CurrentTime +0xF4); `007d7e10` CTCTavernGameSpotTheAddition::GetRequiredScore (TargetTime +0x98, def RequiredScore).
- **Label corrections:** all five carry the propagated `CTCTavernGameCardBase::GetCurrentScore` (right only for 0x008F0A70).
### K027 -- std::_Rb_tree::_M_insert(x, y, v, w) for 8-byte map values, unsigned keys (5 of 5 matched, RELOCATION_MATCH)
- **Function:** STLport 4.6 `_tree.c` node insert (0x18-byte node, null-tested placement pair copy, `_Rebalance` 0x0042971D).
- **Representative:** 0x007E9B40, grade 1 (the landed H011/H075 0x00835270 model with a (key, value) pair).
- **Instances (all proven):** `007e9b40` map<CThing*, float> (CTCAvoidRegionExit::DistMap); `008af120` map<unsigned long, CombatDialogueManager::TSpeechGroupInfo>; `00990a10` map<unsigned long, CThreadedFileManager::CFileObject*>; `00a80c20` map<const CANavQuadTreeNode*, long> (AddToIndexTable's index table); `00ad3440` map<unsigned long, float>.
- **Label corrections:** `RBTree_Insert`, `RBTree_InsertNode`, `Map_InsertNewNode`, `std_set_insert_uint_pair` are behavioural; reloc_bindings' `?Insert@CRetailTree_007e9c00@@` / `CRetailTree_00a819b0` names (0x007E9B40 / 0x00A80C20) are stand-ins.
### K028 -- creature-action constructors with a nonzero end time (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `CCreatureAction_X(CThingCreatureBase& owner) : CCreatureActionBase(owner, -1.0f, T, GetRandomAnimFromName(owner, "LIT"), CSoundPair(), 0, 0)`.
- **Representative:** 0x007EBA20, grade 1 (the landed G044 0x00842AF0 model; a nonzero T moves the CSoundPair zero stores before its address push).
- **Instances (all proven: vtable RTTI + Ego_r.pdb ctor name + literal):** `007eba20` CCreatureAction_LookThroughWindow ("ST_HOMICIDAL_INTRO", 0.5); `00842150` CCreatureAction_DropGenericBox ("ST_HOLD_BOX_PUT_DOWN", 0.5); `008c7f60` CCreatureAction_BanditKingChangeWeapons ("BK_NEW_WEAPONS", 0.75); `00911180` CCreatureAction_Curiosity ("WHATS_THAT", 1.0); `00933cc0` CCreatureAction_CallSpouseHome ("STANDARD_CAST", 1.0).
- **Label corrections:** all five carry the propagated `CCreatureAction_AlertToPlayer::CCreatureAction_AlertToPlayer`; reloc_bindings' `??0CCreatureAction_LookThroughWindow@@QAE@AAVCThingAICreature@@@Z`, `??0CCreatureAction_AlertToPlayer@N_00842150/N_00933cc0@@...PAVCThingAICreature` and `??0DropGenericBox@Retail@@` are stand-ins (the real parameter is CThingCreatureBase&).
### K029 -- CVillageTask*::IsThingInvolvedWithTask(const CThing&) const (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `return A.GetPItem() == &thing || B.GetPItem() == &thing;` over the task's two intelligent pointers (vtable slot 5).
- **Representative:** 0x00860310, grade 1.
- **Instances (all proven: vtable slot + FableWin.pdb label + Ego_r.pdb members):** `00860310` CVillageTaskTorch (Torch, CreatureAssignedToTask); `00860350` CVillageTaskHorn (PHorn, ...); `00860390` CVillageTaskPatrol (PatrolStart, ...); `008603d0` CVillageTaskPickCrops (Crop, ...); `00860520` CVillageTaskCarryCrate (CreatureAssignedToTask +8, PCrateStack +0x10).
- **Labels:** the manifest names are right (FableWin.pdb mangled names).
- **Header debt:** engine/CVillageTask{Torch,Horn,Patrol,PickCrops,CarryCrate}.h flatten the vptr and the intelligent pointers to bytes, so the classes are modelled locally (verifier SHARED_HEADER_AVAILABLE warning expected).
### K030 -- std::vector<T>::resize(size_type) over 12-byte zero default values (5 of 5 matched, RELOCATION_MATCH)
- **Function:** STLport 4.6 `resize(n) { resize(n, _Tp()); }` with the two-argument resize out of line; the zeroed temporary is never destroyed.
- **Representative:** 0x0086DE40, grade 3 (CTextureMorph needs its FableWin.pdb user default constructor so all three zero words come from one register before the pushes).
- **Model note:** for the CArray-holding elements the temporary's destructor (destroy loop + free) folds only when VC7.1 sees a same-TU body of resize(n, x) that reads but never writes through the reference (declaration-only keeps the destroy; a body that ignores x drops the zero stores). The stand-in body is not graded.
- **Instances (all proven: FableWin.pdb ?resize@...QAEXI@Z + the landed two-argument resize):** `0086de40` pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>; `00a4b1d0` CLipSyncFrameData; `00a752a0` CArray<C2DLineF>; `00afe980` CArray<long>; `00bcdee0` CArray<C3DVector>.
- **Label corrections:** all five carry the propagated `vector<EWeaponClass>::_Umove<EWeaponClass*>`.
### K031 -- CGameScriptInterface::MsgOn*/MsgIs*() const message checks (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `FindMostRecentMessage(CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween>(IsOfType(T), OccuredBetween(current, max)))` on World->PMessageEventManager, tested for null.
- **Representative:** 0x00893AB0, grade 12 (locals for the two frames, a manager local, and `if (...) return true; return false;` -- `!= 0` gives `neg/sbb/neg`, frame-filter temporaries without locals reorder the calls).
- **Instances (all proven: vtable 0x01260F0C slot + Ego_r.pdb name):** `00893ab0` MsgOnLeavingExperienceSpendingScreen (0x4A); `00893e20` MsgOnHeroUsedGuildSeal (0x3D); `00893e70` MsgOnGameSavedManually (0x3E); `00893f30` MsgOnHeroFiredRangedWeapon (0x43); `00894440` MsgIsActionModeButtonPressed (0x4B).
- **Labels:** right. EMessageEventType enumerator names not recovered (casts of the retail values); filter members from Ego_r.pdb (OccuredBetween MinFrame/MaxFrame, Bind2 F1/F2).
- **Model note:** Ego_r.pdb names the callee FindMostRecentMessage<CFilter_Bind2<IsOfType, OccuredBetween>>; reloc_bindings' `CMessageEventManager_Retail` / `CFilter_Bind2<..., CanBeSeenOrHeard>` spellings (from older landed rows) are stand-ins -- CanBeSeenOrHeard is a 4-byte {Creature} filter, the frame pair is CMessageEventFilter_OccuredBetween(long, long).
### K032 -- CGameScriptInterface::Get<Flag>(CScriptThing) CTCAIScratchpad flag getters, default true (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `if (!thing.GetThing()) return true; CTCAIScratchpad* tc; thing.GetThing()->GetTC(&tc); return tc->Flag;` with the by-value CScriptThing destroyed inline.
- **Representative:** 0x008A60D0, grade 1 (the landed J018 GetDisableHeadLooking 0x008A6BF0 model with `return true`).
- **Instances (all proven: vtable 0x01260F0C slot + Ego_r.pdb name + PDB member offset):** `008a60d0` GetKillSelfAndStealableItemAfterReachingDestination (+0x6B); `008a6390` GetAllowedToFollow (+0x6C); `008a6eb0` GetIsPushableByHero (+0x79); `008a7170` GetLookForFiniteTime (+0x7A); `008a7430` GetAvoidRegionExits (+0x7B).
- **Labels:** right.
- **Header debt:** engine/CScriptThing.h / CGameScriptInterface.h flatten the vptr, so the classes stay local (as the landed J018 rows).
### K033 -- CGameScriptInterface gossip map lookups (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `return CTCAIScratchpad::Get<Map>(name);` with Ego_r.pdb's static __fastcall CTCAIScratchpad::Get<Map>(CCharString) inlined: by-value copy, `it = map.find(name); if (it == map.end()) return 0; return &map[name];`.
- **Representative:** 0x008A9760, grade 2 (the found iterator must be a local: `find(name) == end()` reads end() first).
- **Instances (all proven: vtable 0x01260F0C slot + Ego_r.pdb names + the PDB static-member initializer names):** `008a9760` GetActiveGossipCategories (map<CCharString, bool> 0x013BAE2C); `008a9810` GetIsGossipForPlayer (0x013BAE38); `008a9f50` GetGossip (map<CCharString, vector<CCharString> > 0x013BAE44); `008aa1b0` GetGossipVillages (0x013BAE50); `008aa410` GetGossipFactions (0x013BAE5C).
- **Label corrections:** none (the doubled `CGameScriptInterface::CGameScriptInterface::` prefix aside). reloc_bindings' `g_gossipMap` / `g_tree_013bae2c` / `GossipMap` / `CBoolMap` / `?_M_find@?$map@...` spellings are stand-ins: the statics are CTCAIScratchpad::ActiveGossipCategories / IsGossipForPlayer / Gossip / GossipVillages / GossipFactions and 0x008AC200 is the _Rb_tree's _M_find.
### K034 -- CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<...> > const (5 of 5 matched, RELOCATION_MATCH)
- **Function:** STLport list walk, Bind3's inline `(*F1)(m) && (*F2)(m) && (*F3)(m)` on the out-of-line filter operators, the match as a named pointer local.
- **Representative:** 0x008ABE70, grade 2 (`return &*it;` swaps esi/edi and re-forms the address; the named message pointer is the landed Bind4 0x00959040 form).
- **Instances (all proven: filter operator bodies vs Ego_r.pdb CMessageEvent fields + the Ego_r.pdb instantiation names):** `008abe70` <IsOfType, OccuredBetween, HasExtraData>; `008abf20` <IsOfType, HasExtraData, OccuredBetween>; `0090edc0` <IsOfType, IsOriginatedByThing, CanBeHeard>; `00910cf0` <IsOfType, IsOriginatorAlive, CanBeSeenOrHeard>; `00911240` <IsOfType, IsOriginatorAlive, IsTargettedAtThing>.
- **Label corrections:** all five carry the propagated `FindMostRecentMessage<CFilter_Bind3<IsOfType, HasExtraData, ...>>` label (right only for 0x008ABF20); bindings `CMessageEventManager_Retail`, `IsOriginatedByThing@N_006af2b0`, `IsForThing` are stand-ins.
### K035 -- CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<...> > const (5 of 5 matched, RELOCATION_MATCH)
- **Function:** K034's list walk with two bound filters.
- **Representative:** 0x008ABED0, grade 1 (K034's text).
- **Instances (all proven: filter operator bodies + Ego_r.pdb instantiation names):** `008abed0` <IsOfType, OccuredBetween> (the K031 CGSI callee); `008d46d0` <IsOfType, IsTargettedAtThing>; `0090dee0` <IsOfType, IsOriginatedByThing>; `00939c60` <IsOfType, IsOriginatorAlive>; `00939cb0` <IsOfType, MessageIDGreaterThan>.
- **Label corrections:** all five carry the propagated `FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>>` label (Ego_r.pdb's CanBeSeenOrHeard instantiation is a different body); the reloc_bindings name of 0x008ABED0 with CanBeSeenOrHeard is wrong.
### K036 -- combat-action constructors (owner, target) with action code 150 (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `X(CThingCreatureBase& owner, CThing& target) : CCombatActionBase(owner, target, 150, CSoundPair()) {}`.
- **Representative:** 0x008AE160, grade 1.
- **Instances (all proven: vtable RTTI + FableWin.pdb ctor signature):** `008ae160` CCreatureAction_InterruptableMidAttack; `008b9190` CCreatureAction_ScreamerDrainOutOf; `00906ba0` CCreatureAction_InterruptableNearAttack; `00907fb0` CCreatureAction_TrollWhackGroundBase (target CThingCreatureBase&); `00970420` (anonymous namespace)::CCreatureAction_SummonerChargeSuccess.
- **Label corrections:** all five carry the propagated `CCombatAction_GenericResponseToDrain::CCombatAction_GenericResponseToDrain`; reloc_bindings' `??0CCreatureAction_AttackThing@@...PAV` (0x008AE160) and `??0CCreatureAction_SummonerChargeOutOf@@` (0x00970420) are wrong class names (the vtables' RTTI say InterruptableMidAttack / SummonerChargeSuccess).
### K037 -- combat-action Clone() const with the implicit copy constructor inline (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `return new X(*this);` -- operator new, CCombatActionBase copy ctor 0x0062F3B0, the vtable, the class's own members memberwise.
- **Representative:** 0x008B8840, grade 1.
- **Instances (all proven: vtable slot 11 + Ego_r.pdb Clone names + PDB members at retail offsets = PDB - 0x18):** `008b8840` CCreatureAction_PlayCombatAnimation; `008be290` CCreatureAction_CastWillForceSpell; `008be790` CCreatureAction_CastWillRaiseDeadSpell; `008be9c0` CCreatureAction_CastWillFireballSpell; `008cfb70` CCreatureAction_TentacleStrikeLoop.
- **Labels:** right (doubled class prefix aside).
### K038 -- combat-action copy constructors X(const X&) (5 of 5 matched, RELOCATION_MATCH)
- **Function:** the memberwise copy emitted out of line for the landed Clone rows: CCombatActionBase copy ctor 0x0062F3B0, vtable, members.
- **Representative:** 0x008B8A10, grade 3 (C3DVector needs its FableWin.pdb user copy constructor: a trivial C3DVector copy re-bases src/dst pointers instead of direct offsets; the init list written out equals the implicit copy).
- **Instances (all proven: vtable RTTI + FableWin.pdb copy-ctor names + the landed Clone callers):** `008b8a10` CCreatureAction_ProjectileCrouch (1-aligned FacePos at +0x115); `008beab0` CCreatureAction_TeleportOut; `008c5dc0` CCreatureAction_ScorpionCharge; `008c5e40` CCreatureAction_ScorpionChargeStrike; `00977690` CCreatureAction_JackFloatUp.
- **Label corrections:** `CCreatureAction_CastPhysicalShieldSpell::` (0x008B8A10), `CCombatAction_ControlledStrafeJump::` (0x008BEAB0), `CQuickAccessItemButtonGuiDef::` x2, `CMultiStaticMeshEntryDef::` are propagated; reloc_bindings' `??0CCreatureAction_CastPhysicalShieldSpell@@QAE@ABU0@@Z`, `??0CQuickAccessItemButtonGuiDef@@...`, `??0CMultiStaticMeshEntryDef@@...` for these addresses are wrong.
### K039 -- spell-finish FinishAction() re-equip tails (0 of 5 matched, parked)
- **Function:** `CCombatActionBase::FinishAction(); if (IS_THING_ALIVE(PItemRemoved)) { CCharString name("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE"); GetCreature()->SetCurrentAction(&CCreatureAction_UnsheatheItemFromInventory(GetCreature(), name, PItemRemoved.GetPItem(), 100, 250, 0)); }` -- the tail of the landed CCreatureAction_FinishFireballSpell::FinishAction 0x008BAC40.
- **Identities (proven: vtable slot 4 + labels / Ego_r.pdb):** `008b96a0` CCreatureAction_FinishWillTurncoatSpell, `008b9740` CCreatureAction_FinishDrainLifeSpell, `008b98d0` CCreatureAction_FinishHealLifeSpell, `008b9a60` CCreatureAction_FinishGhostSwordSpell, `008b9bf0` CCreatureAction_ReleaseWillSummonSpell (::FinishAction).
- **Remaining difference:** same length, same instructions; only the callee-saved register choice differs (retail keeps &PItemRemoved in edi and the unsheathe owner in ebx, pushing ebx inside the branch; VC7.1 RTM and QFE both pick ebx/edi the other way). Tried: helper/inline expression/`&&` forms of IS_THING_ALIVE, named action, implicit string temporary, owner local, a reference local, a __forceinline helper.
### K040 -- FinishAction(): one creature-mode change, then the base finish (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `if (GetCreature()->GetInterface(&modes, 0x31)) modes->RemoveMode/AddMode(M); CCombatActionBase::FinishAction();`.
- **Representative:** 0x008C19B0, grade 1 (the landed FinishFireballSpell 0x008BAC40 GetInterface model).
- **Instances (all proven: vtable slot 4 + Ego_r.pdb names, access from the PDB mangling):** `008c19b0` CCombatAction_Guildmaster_HealFollowersRelease (RemoveMode 0x3C); `008c1ac0` ..._ShieldFollowersRelease (0x3D); `008c1bd0` ..._SpeedUpFollowersRelease (0x3E); `00982b50` CCreatureAction_BeetleTakeOff (protected, AddMode 0x1A); `00982bb0` CCreatureAction_BeetleLand (protected, RemoveMode 0x1A).
- **Labels:** right.
### K041 -- one-owner combat-action constructors with a "SWORD_SWING" sound pair (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `X(CThingCreatureBase& owner) : CCombatActionBase(owner, code, GetSoundPair(owner, "SWORD_SWING")) {}` (code 150 or 250).
- **Representative:** 0x008C38B0, grade 1.
- **Instances (all proven: vtable RTTI + FableWin.pdb ctor names):** `008c38b0` CCombatAction_WillForcePushInto (150); `008e4ad0` CCreatureAction_DragonStrafeOutOf (250); `009534d0` CCreatureAction_DragonHoverInto (250); `00953530` CCreatureAction_DragonHoverOutOf (250); `0096c6e0` CCreatureAction_ThunderLightningStorm (150).
- **Label corrections:** all five carry the propagated `CCombatAction_Guildmaster_HealFollowersInto::CCombatAction_Guildmaster_HealFollowersInto`; reloc_bindings' pointer-parameter spellings (`@QAE@PAVCThingCreatureBase@@@Z`) are stand-ins -- FableWin.pdb's are references. GetSoundPair 0x008561A0 is Ego_r.pdb's protected static `CCombatActionBase::GetSoundPair(const CThingCreatureBase&, const CCharString&)` (__fastcall); the landed H030 rows declare it as a free function.
### K042 -- CGameScriptThing::Msg<Event>() const event-window checks (5 of 5 matched, RELOCATION_MATCH)
- **Function:** IS_THING_ALIVE(PThing); the running quest thread's last/current script frames; const GetTC<CTCEntityEvents>; walk the Events list for an event of one type with WorldFrame in (last, current].
- **Representative:** 0x008D2700, grade 2 (the bitset bit must be tested as `word & mask` in the `if`; STLport's const operator[] returns a bool that VC7.1 forms with shr/test).
- **Instances (all proven: vtable 0x012852CC slot + FableWin.pdb labels):** `008d2700` MsgPerformedAnySpecialAbility (0x1A); `008d3170` MsgOpenedChest (0x1C); `008d3850` MsgAttackedWithMeleeWeaponWithoutHittingAnything (0x1E); `008d3930` MsgAttackedWithBareHandsWithoutHittingAnything (0x1F); `008d3a10` MsgTalkedToAnyone (0x21).
- **Labels:** right. Model on Ego_r.pdb types (CQuestManager::PQuestCurrentlyRunning +0x88, CActiveQuest::PScript, CScriptBase::PActiveThread +0x2C, CSpawnedFuncBase Last/CurrentScriptFrame, CTCEntityEvents::Events list<CCountedPointer<NEntityEvents::CEventBase> >); the landed sibling 0x008D27E0 uses invented `*Native` stand-ins.
- **Callee note:** the lookup is CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound const (0x004365B0), the non-const one being 0x0040F020.
### K043 -- combat-sequence OnCreate() queueing one action (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `CCombatAction action; action.CreatureAction.Reset(new X(*Creature, *Target)); PushAction(action); CCombatSequenceBase::OnCreate();` with X's (owner, target) constructor inline (base code 50, zero CSoundPair).
- **Representative:** 0x009068A0, grade 1.
- **Instances (all proven: vtable slot 1 + FableWin.pdb labels + the action vtable RTTI):** `009068a0` CCombatSequence_BanditIdleNear (CCreatureAction_LeapBack); `00906940` CCombatSequence_BanditIdleMiddle (CombatIdle); `009069e0` CCombatSequence_BanditBoast (CombatBoast); `0090a0b0` CCombatSequenceWhisperBoast (CombatBoast); `0090a150` CCombatSequenceWhisperIdles (CombatIdle).
- **Labels:** right.
- **Callee names:** 0x00905FD0 is CCountedPointer<CCombatActionBase>::Reset(CCombatActionBase*) (landed as `CCountedPointer_0085a5a0::Reset`); 0x008B0B10 is the protected CCombatSequenceBase::PushAction(CCombatAction&) (Ego_r.pdb; landed file name PushAction, bindings AddAction).
### K044 -- CThingSearchTools::GetPBestThing<TFilter, CThingCompare_Nearest, TThing> const (5 of 5 matched, RELOCATION_MATCH)
- **Function:** nearest scan-visible TThing on PeekClassList() passing the filter (the landed 0x0093B640 walk).
- **Representative:** 0x00914330, grade 1.
- **Instances:** `00914330` <CThingFilter_HasScriptName, ..., CThingMarker> (proven); `00916f30` <CIsFreeLogPile, ..., CThingObject> and `00916fc0` <CIsFreeBonfire, ..., CThingObject> (probable: which filter is which follows Ego_r.pdb's GetPNearestFreeLogPile-before-GetPNearestBonfire order; the two filters differ only in the property bit, 22 vs 14); `009642d0` <CThingFilter_MarkerHasProperty, ..., const CThingMarker> (proven); `00965180` <CThingFilter_HasTC, ..., const CThingMarker> (proven).
- **Label corrections:** all five carry the propagated `GetPBestThing<CIsFreeBonfire, CThingCompare_Nearest, CThingObject>`; reloc_bindings' `CThingFilter_DefName` / `CThingFilter_EscapePoint` / `PAPAVCThingPhysical` / `PAPAVCThing` spellings (0x00914330 / 0x009642D0) are stand-ins.
- **Callee names:** 0x0049C770 is Ego_r.pdb's protected CThingSearchTools::PeekClassList() const (bound PeekAllThings@CThingSearchToolsMethods, the landed stand-in); 0x009141F0 = CThingCompare_Nearest::operator()<CThingMarker>, 0x00964160 = operator()<const CThingMarker>, 0x00916E30 / 0x00916E90 = CIsFreeLogPile / CIsFreeBonfire::operator() (probable).
### K045 -- AI-state-group OnReplace(): base OnReplace + CTCIdleScheduler::ChooseNewIdle (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `Base::OnReplace(); if (PBrain->PCreature->GetInterface(&scheduler, 0xD3)) scheduler->ChooseNewIdle();`.
- **Representative:** 0x00917D60, grade 1 (the landed MinionWatchFires::OnReplace 0x00936590 body after a qualified base call).
- **Instances (all proven: vtable slot 10 + Ego_r.pdb names with access + RTTI base chains):** `00917d60` CAIStateGroup_CleanHouse (base Clean 0x00917BB0); `00918510` CAIStateGroup_GazeOutside, `009185f0` CAIStateGroup_GazeAtHome (base Gaze 0x009183A0, protected); `009390e0` CAIStateGroup_MinionSitOutside, `00939220` CAIStateGroup_MinionSitAtHome (the inherited CAIStateGroup_UsesSeat::OnReplace 0x009387F0 through MinionSitOnSeat / MinionSitOnSeatBase, protected).
- **Labels:** right.
### K046 -- std::find_if over list<CIntelligentPointer<CThing> >::const_iterator with thing predicates (5 of 5 matched, RELOCATION_MATCH)
- **Function:** STLport find_if `while (first != last && !pred(*first)) ++first; return first;`, /Gr __fastcall with the hidden iterator result.
- **Representative:** 0x00918110, grade ~14 across the family (see model note).
- **Model note (review):** the 4-byte predicate row (CIsCandleNeedingChanging) matches the folded form (the landed find_if 0x0076D810 model: predicate used in its own slot); the four empty-predicate rows address the predicate in the dead `first` slot with no copy instruction, which VC7.1 reproduces only through the `__find_if` hand-off with a user-declared no-op predicate copy constructor -- a modelling device without PDB evidence; a trivial copy emits a 1-byte self-copy, the folded form uses the predicate's own slot.
- **Instances (all proven):** `00918110` CIsFreeBench (CAIStateGroup_Clean's bench search); `00938e50` CIsFreeSeat (MinionSitAtSchool); `009465e0` CIsFreeCookingPot; `00946d40` CIsCandleNeedingChanging; `0094ba90` CIsBenchWithOnePerson.
- **Label corrections:** `sub_00918110`, `sub_00938e50` and three `_Find_if<list<CCountedPointer<CVillageTaskTorch>>, CIsThingInvolvedWithTask>` labels are propagated; reloc_bindings' `?FindIfCookingPotForMeal@@` (0x009465E0) is a stand-in.
### K047 -- boss AI-state-group IsStillRunnable() (5 of 5 matched, RELOCATION_MATCH)
- **Function:** IS_THING_ALIVE(enemy); not stopping; the creature's targeting component (interface 8) still targets the enemy.
- **Representative:** 0x0094D930, grade 3 (operand order `enemy.GetPItem() == targeting->Get...()` evaluates the virtual call first -- 0x00962AF0 compares the other way, through a target local; the ternary entry pick puts the component straight into ecx; `if (...) return true; return false;` gives `sete`).
- **Instances (all proven: Ego_r.pdb IsStillRunnable names + neighbours/callers + PDB member offsets):** `0094d930` CAIStateGroup_TrollBase (public, PEnemy +0x14); `00956a70` CAIStateGroup_BriarRose (protected, PEnemy +0x14); `00962af0` CAIStateGroup_VersusEnemyBase (protected virtual, slot 14, PTargetThing +0x14); `009779a0` CAIStateGroup_Guildmaster (protected, PEnemy +0x18); `0097b870` CAIStateGroup_ScorpionKing (protected, PEnemy +0x30).
- **Label corrections:** four rows carry the propagated `CAIStateGroup_BriarRose::IsStillRunnable` (right only for 0x00956A70).
- **Probable callee name:** the targeting component's slot 29 is modelled as CTCTargetingBase::GetPTargetedThing() const (CTCTargetingAI's slot 29 returns an intelligent pointer's item; the PDB has that virtual but no slot map).
### K048 -- scalar deleting destructors around a one-call inline destructor (5 of 5 matched, RELOCATION_MATCH)
- **Function:** vtable store, the class's cleanup call, the base destructor, `if (flags & 1) delete this`.
- **Representative:** 0x00987F30, grade 1 (a `new X` in the file makes VC7.1 emit the vtable and the ??_G COMDAT).
- **Instances (all proven: vtable RTTI + Ego_r.pdb member names):** `00987f30` CSimpleMemoryPool (ClearMemoryBlocks); `009e2890` CVertexBufferWin32 (Uninitialise; CVertexBufferBase's destructor implicit -- a user-declared empty one stores its vptr); `00af8a70` CDynamicObjectHeap<156,2100>, `00af8ca0` CDynamicObjectHeap<116,529>, `00b49710` CDynamicObjectHeap<56,16> (private virtual Release, base ~CDynamicObjectHeapBase 0x00F36C90).
- **Label corrections:** all five carry the propagated `CThingTrackNode::vector deleting destructor`; reloc_bindings' `?SubCleanup@@YIXPAVCInputProcessManager@@@Z` (0x00AF8860) / `@MemberDtor_686df0_00af8bc0@4` (0x00AF8BC0) are the CDynamicObjectHeap<156,2100> / <116,529>::Release bodies.
### K049 -- std::__copy_backward over trivially assignable 16-byte elements (5 of 5 matched, MATCH)
- **Function:** STLport random-access `__copy_backward` (/Gr), the landed H024 text.
- **Representative:** 0x0098C240, grade 1.
- **Instances (all proven from their callers' landed relatives):** `0098c240` C4DVector; `00a74560` C2DLineF; `00a7d8a0` pair<unsigned __int64, CANavQuadTreeNode*>; `00bc9ef0` CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry; `00bdf600` C3DBoundingSphere.
- **Label corrections:** all five carry the propagated `_Move_backward<pair<unsigned __int64, CPersistInfo>*>` label.
### K050 -- scalar deleting destructors around an implicit destructor (5 of 5 matched, RELOCATION_MATCH)
- **Function:** member destructor, the base's user-declared inline destructor (its vptr store), the next base's out-of-line destructor, conditional delete.
- **Representative:** 0x00996F00, grade 1.
- **Instances:** `00996f00` CDataOutputStream (proven: vtable 0x0129A6E0 slot 0 RTTI, Ego_r.pdb ??_G name, NewLine CCharString +4, base CDataStream); `00f25b40`, `00f25cb0`, `00f26df0`, `00f26f40` CUnprovenDef_<addr> (descriptive: unreferenced def-class ??_G COMDATs, CDefPointeeBase-derived, destroying a 12-byte container at +0x2C via 0x00F25B70 / 0x00F25CE0 / 0x00F26E20 / 0x00F26F70).
- **Label corrections:** all five carry the propagated `CLandscapeBackgroundPatch::vector deleting destructor`. 0x009FC550 is CResource::~CResource (CDefPointeeBase's base; bindings carry CMemoryBlock / CResource stand-ins).
### K051 -- chunked-file chunk scalar deleting destructors (5 of 5 matched, RELOCATION_MATCH)
- **Function:** own vptr, free the CArray<POD> member's storage, the chunk base's destructor (CChunkedFileChunk 0x00AA75C0 / CChunkedFileSuperChunk 0x00AA8490), conditional delete.
- **Representative:** 0x00A9D360, grade 3 (the class's empty destructor must be defined out of line in the file -- Ego_r.pdb lists ??1X; an in-class inline one is called, not inlined, by ??_G).
- **Instances (all proven: vtable RTTI + Ego_r.pdb names + member offsets):** `00a9d360` CMirroredTextureHighlightsChunk (TriangleVertexIndexList); `00abecb0` C3DMeshFileSmoothingGroupsChunk (FaceGroupsList); `00abedd0` C3DMeshFileGroupChunk (VertexList +0x30); `00abee10` C3DMeshFileVerticesChunk (VertexList); `00abee50` C3DMeshFileTrianglesChunk (TriangleList +0x2C).
- **Label corrections:** all five carry the propagated `CAInputProcess::scalar deleting destructor`; 0x00AA75C0 / 0x00AA8490 are bound with stand-ins besides the real ~CChunkedFileChunk / ~CChunkedFileSuperChunk.
### K052 -- chunk CreateNewInstance factories (5 of 5 matched, RELOCATION_MATCH)
Model: the landed G048 0x00A98AB0 factory `return InitNewInstance(new X);` with X's inline ctor `X() : CChunkedFileChunk("TYPE") {}`
and one CArray<E> member (STLport vector default ctor = three zero stores). Identities proven (vtable slot 9 + RTTI + FableWin.pdb):
C3DAnimFileXSequenceChunk 0x00AA46B0, C3DAnimFileBoneMaskChunk 0x00AB0730, C3DMeshFileSmoothingGroupsChunk 0x00ABC500,
C3DMeshFileUniqueVerticesChunk 0x00ABC5F0, C3DMeshFileVerticesChunk 0x00ABC800.
### K053 -- one-array chunk default constructors (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `X::X() : CChunkedFileChunk("TYPE") { }` (CChunkedFileSuperChunk for TRIS) with the member CArray's inline default ctor.
- **Instances (all proven: vtable RTTI + FableWin/Ego_r ??0X@@QAE@XZ):** `00ab0700` C3DAnimFileBoneMaskChunk "AMSK"; `00abc470` C3DMeshFileSmoothingGroupsChunk "SMTH"; `00abc560` C3DMeshFileUniqueVerticesChunk "UNIV"; `00abc780` C3DMeshFileVerticesChunk "VERT"; `00abc860` C3DMeshFileTrianglesChunk "TRIS" (super-chunk base 0x00AA8360, TriangleList +0x2C).
- **Label corrections:** all five carry the propagated `C3DAnimFileXSequenceChunk::C3DAnimFileXSequenceChunk`; 0x00AA8360 is bound under six names in reloc_bindings (CBase_00aa8360, C3DMeshFileChunk, NativeSuperChunk, CChunkedFileSuperChunk QAE/IAE) -- Ego_r.pdb says ??0CChunkedFileSuperChunk@@IAE@PBD@Z.
### K054 -- CDynamicObjectHeap<Size,Count>::Release (5 of 5 matched, RELOCATION_MATCH)
- **Function:** private virtual slot 1: delete every allocator block chained after StartBlock, FreeList = 0. The landed <56,16> Release 0x00B48E10 verbatim.
- **Instances (all proven: vtable slot 1 + NextEntry offset = 0x18 + Size x Count, FableWin.pdb names where present):** `00af8780` <24,853>; `00af8860` <156,2100>; `00af8bc0` <116,529>; `00b48c70` <12,16>; `00b48d40` <72,16>.
- **Label corrections:** all five carry the propagated `CQuadTreeElement::RemoveObjectCollections`. FableWin.pdb's large heaps are <172,1905> / <132,465> and Ego_r's <164,1998> / <124,495>; retail's strides say <156,2100> / <116,529> (the landed ctors agree) -- the template arguments differ per build.
### K055 -- CDynamicObjectHeap<Size,Count>::Alloc(long) (5 of 5 matched, RELOCATION_MATCH)
- **Function:** empty free list -> grow (CreateMoreBlocks) or return 0; ++AllocCount; pop the head. The landed <56,16> 0x00B49660 body as the template member.
- **Instances (all proven: AllowHeapToGrow offset + per-heap landed CreateMoreBlocks callee + PDB ?Alloc@...@@QAEPAXJ@Z):** `00af89c0` <24,853>; `00af8a10` <156,2100>; `00af8c70` <116,529>; `00b495c0` <12,16>; `00b49610` <72,16>.
- **Label corrections:** all five carry the propagated `CAIStateGroupBase::IsStopping`; the PDB name is Alloc(long), not the landed 0x00B49660's `Allocate(unsigned long)` on a stand-in struct.
### K056 -- engine sub-primitive constructors (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `X::X(CEngineInternalPrimitiveY* parent) : CEngineSubPrimitiveBase(parent) { }`; the inline base ctor keeps Parent (+0x0C) and clears NextSubPrimitive (+0x10); the base's dead vptr store is dropped by VC7.1.
- **Instances (all proven: stored vtable RTTI + FableWin.pdb ctor names):** `00b98b10` CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveOcclusionBase*); `00bb1780` CEngineSubPrimitive2DClockSprite; `00bb6950` CEngineSubPrimitiveWeatherMask; `00bba260` CEngineSubPrimitiveDecal; `00be1b50` CEngineSubPrimitive2DViewport.
- **Label corrections:** all five carry the propagated `CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite` (right only for 0x00BB1780).
- **Header debt:** the parameter classes are modelled as empty `class Y : public CEngineInternalPrimitiveBase { };` (RTTI offset-0 derivation; members not touched) because the generated engine headers are flattened and cannot express the up-cast.
### K057 -- landscape / local-detail movable-resource scalar deleting destructors (5 of 5 matched, RELOCATION_MATCH)
- **Function:** ??_G (vtable slot 0): the out-of-line virtual destructor, then the class's static __fastcall operator delete inlined: `g_pEngine_01436ea8->m_pLandscapeMemory->FreeVirtual(this)`.
- **Model device:** an inline default ctor plus a placement-new helper emits the vtable and the ??_G COMDAT (the destructor must stay out of line so ??_G calls it).
- **Instances (all proven: vtable slot 0 + the landed destructor callee + FableWin ??1/??3 names):** `00bdf9b0` CLocalDetailCacheMap::CObjectTypeCollection; `00bdf9e0` CLocalDetailCacheMap::CObjectCacheGroupCollection; `00bf8ce0` CLandscapeBackgroundTreeNode; `00bfbc50` CLocalDetailPrimitiveRepeatedMesh; `00bfdc90` CEngineWaterBackgroundSubPatch.
- **Label corrections:** all five carry the propagated `CLandscapeBackgroundPatch::vector_deleting_destructor`; reloc_bindings has 0x00BF73A0 as non-virtual `??1CLandscapeBackgroundTreeNode@@QAE@XZ`, FableWin.pdb says virtual UAE (slot 0 of its vtable is this ??_G).
### K058 -- libpk PKCAutoDelete<T>::Release (5 of 5 matched, RELOCATION_MATCH)
- **Function:** libpk assert macro (`PKCAutoDelete.h(42) : Shouls have checked first...` through the CString stream operators to the assert hook, then the CDebugBreak temporary) when the owned pointer is null; then return it and clear it.
- **Instances (all descriptive: libpk has no PDB; template argument CUnprovenPKObject_<addr>, member name Release from the release semantics):** `00c099f0`, `00c17850`, `00c2f070` (T: Serialise 0x00C3B850), `00c2f190` (T: Serialise 0x00C3B970), `00c2f2b0` (T: the landed CBankCriteria_00c3b7a0).
- **Label corrections:** the manifest labels (CWorldManager::OnShutdownAlt, MessageQueue_CheckAndProcess, PKCAutoDelete_AssertCheckedAndClear_*) are behavioural/propagated.
- **Callee names:** the sweep1 names CString::operator<< 0x00C0DEC0 / 0x00C0DF00, ??1CString 0x00C0BA40, CDebugBreak 0x00C16750, g_pfnAssertHook 0x0143BB2C, g_pchNil 0x0139C900 (each address carries many older stand-in bindings).
### K059 -- libpk lock-guarded virtual setters (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `PKCProtectionInstance lock(&g_lock_0143bb14); Member = value;` (no EH frame: nothing inside can throw).
- **Instances (all descriptive: no RTTI, no PDB; class named by vtable, member by offset):** `00c10b00` vtable 0x012B2748 slot 1, long +0x888; `00c11120` slot 7, float +0x7C0 (slot 8 is the landed float getter 0x00C11150); `00c11200` slot 6, byte +0x884; `00c14860` vtable 0x012B29F8 slot 8, long +0x9C; `00c14890` slot 9, long +0xA0.
- **Label corrections:** all five carry the propagated `CTCElectrocutedEffect::OnInitialActivate`.
### K060 -- libpk lock-guarded record setters (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `PKCProtectionInstance lock(&g_lock_0143bb14); Record->WordNN = value;` with the record pointer at +8 (the landed CAtmosOwner_00c14820 / 0x00C15FC0 shape).
- **Instances (all descriptive, slots 7/8/9/11/12 of the no-RTTI vtable 0x012B2A74 stored by 0x00C16050 / 0x00C16130):** `00c15f30` +0x0C; `00c15f60` +0x10; `00c15f90` +0x14; `00c15ff0` +0x08; `00c16020` +0x04 (slot 10 = landed 0x00C15FC0, +0).
- **Label corrections:** propagated `CEditControlCentre::SetAtmosEditingMode` / `_gapscan::sub_00c15f30`.
### K061 -- libpk post-a-message members (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `Msg message(argument); m_receiver.Receive(&message);` (receiver embedded at +4 / +0x10 / +0x14, virtual slot 10). The landed CNavigatorSwitch::SendState 0x00C2A7A0 model: the message ctor body compiled out of line earlier in the TU so `this` parks in edx.
- **Instances (all descriptive; three reuse the stand-in names reloc_bindings already holds for them):** `00c2bcf0` CDispatcherB_00c2c7e0::Register; `00c2bd40` CAgentB_00c2c8f0::Handle; `00c363e0` CUnprovenPKAgent_00c363e0::Post; `00c36e00` CSequencePlayer_00c36ea0::Begin; `00c36fb0` CUnprovenPKAgent_00c36fb0::Post. Message types keep their landed ctor names CVtable_012b44b8 / 012b44c0 / 012b50e4 / 012b51d0 / 012b51e0.
- **Label corrections:** manifest labels (CSBAgents_InvokeMethod, SteeringAgentList_Constructor, ...) are behavioural/propagated.
### K062 -- libpk MMX ramped-volume accumulate kernel (0 of 5 matched, parked)
- **Function:** 8 samples per pass scaled by a pmulhw volume ramp (advanced by 4 x step per quad), widened to 32 bits and added into the accumulator; aligned frame, /GS, `emms`.
- **Why parked:** retail reads the ramp start from ecx before any assignment and returns with `ret` while its thunks pop four stack words -- no VC7.1 calling convention does that; the body is an inline-assembler block in a C shell. The differ copy is an MMX-intrinsics model (170 v 211 bytes).
- **Instances (descriptive, five byte-identical copies, one per audio TU):** `00c5bef0`, `00c6aae0`, `00c6f1a0`, `00c6f800`, `00c6fc80`.
- **Label corrections:** the manifest's five different labels (Audio_VectorMultiplyWithIncrementingCoeffs, Audio_ProcessSamples_IIRFilter, SIMD_AccumulateWeightedSamples_64bit, Filter_ApplyTransformSSE_64, SSE_MatrixMultiplyLonglong) name one kernel.
### K063 -- CEaxContext EAX 4.0 reverb-flag bit setters (5 of 5 matched, MATCH)
- **Function:** flip one EAXREVERBFLAGS bit of m_reverb[slot].ulFlags when it changes; mirror it into the committed record with dirty bit 0x800000 for the primary environment slot; return 0. The landed SetReflectionsScale 0x00C64020 (bit 1) with the bit changed.
- **Instances (probable: bit order of the landed SetReverbFlags 0x00C67A10 + EAX 4.0 SDK flag names; class sweep1-descriptive):** `00c640b0` SetReflectionsDelayScale (bit 2); `00c64140` SetReverbScale (3); `00c641d0` SetReverbDelayScale (4); `00c64260` SetDecayHFLimit (5); `00c642f0` SetEchoTimeScale (6).
- **Label corrections:** manifest `HeroAttribute_SetBitFlagBitN` are propagated game names; reloc_bindings still has agent443's `?Set...Flag@CEAXContext@@QAEJJJ@Z` for these five while sweep1's landed caller uses `?Set...@CEaxContext@@QAEJKJ@Z` -- rebind.
### K064 -- CEaxSource EAX 4.0 float source-property setters (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `if (v < 0 || v > max) { ctx->lastError = -2; return -3; } if (v != cur) { cur = deferred = v; dirty |= bit; } return 0;` -- the landed SetObstruction 0x00C64AC0 shape for floats.
- **Instances (probable: EAXSOURCEPROPERTIES word order fed by the landed SetAllSourceProperties 0x00C67C00 + EAX 4.0 ranges):** `00c64b00` SetObstructionLFRatio (0..1, 0x100); `00c64db0` SetDopplerFactor (0..10, 0x10000); `00c64e20` SetRolloffFactor (0x20000); `00c64e90` SetRoomRolloffFactor (0x40000); `00c64f00` SetAirAbsorptionFactor (0x80000).
- **Label corrections:** manifest `SetIntensity` / `CEntity_SetFloatProperty_0x4c..0x58` are propagated.
### K065 -- DirectShow CPosPassThru IMediaPosition getters (5 of 5 matched, RELOCATION_MATCH)
- **Function:** BaseClasses ctlutil.cpp `GetPeer(&pMP); hr = pMP->get_X(p); pMP->Release();` on the CMediaPosition subobject (+4, so `this` is adjusted by -4). The landed CPosPassThru::GetRate 0x00CAD7D0 model with GetPeer 0x00CAD400 as same-TU noinline body.
- **Instances (proven: SDK source + slot = forwarded slot; vtable entries 0x012BF8F8 + 4 x slot in both pass-through vtables):** `00cad870` get_Duration (7); `00cad8b0` get_CurrentPosition (9); `00cad930` get_StopTime (10); `00cad9c0` get_PrerollTime (12); `00cada40` get_Rate (15).
- **Label corrections:** catalogued only as `_gapscan::sub_<addr>`.
### K066 -- quest-script Init() state resets (5 of 5 matched, MATCH)
- **Function:** the script's Init override clears its own state members (one zero register, one store each, source order).
- **Instances (all proven: vtable slot + FableWin.pdb Init name + Ego_r.pdb members):** `00cf06d0` CQ_ArenaHoldingScriptScript (5 bools); `00d89da0` CQ_HerosOldHouseScript (MissionAborted not reset); `00dc4b50` CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper (private, slot 2); `00e7b420` CV_FishermanScript (HeroHasSaved.. before IntroPlayed; FishermanMustRunAway not reset); `00ebcf30` CV_OracleScript::CVO_OracleMarker (private).
- **Label corrections:** catalogued only as `_gapscan::sub_<addr>`.
### K067 -- entity-script Main(): guard the own thing and idle (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `{ CIsThingAlive alive(Me); SetPredicate(alive); } Game->NewScriptFrame(); while (!IsActiveThreadTerminating()) Game->NewScriptFrame();` -- the landed CHTE_Prisoner::Main 0x00D69F50 without the hand-over.
- **Instances (all proven: entity vtable slot 1 stored by the landed Alloc + FableWin.pdb Main names):** `00d293f0` CQ_EndGameScript::CGatewayUndead; `00d6a220` CQ_HangingTreeEvilScript::CGUARD1; `00d6a360` ::CGUARD2; `00d9bdc0` CQ_HobbeToothContestScript::CQ_HobbeToothHobbe; `00da4b10` CQ_MinionClifftopChaseScript::CMinionAttackingGuard.
- **Label corrections:** all five carry the propagated `CCOE_AltarEvil::Main`; CGameScriptInterface slot 7 is NewScriptFrame (cgsi_slots_aligned.tsv).
### K068 -- entity-script constructors with one vector member (5 of 5 matched, RELOCATION_MATCH)
- **Function:** PDB ctor `X(parent, data, me) : CEntityScriptBase(*parent.Game, me), ParentClass(&parent) { PMasterData = data; }` with an STLport vector member default-constructed in between.
- **Instances (probable: entity vtable + script region of the Alloc call + the Ego_r.pdb entity class with a vector after PMasterData; FableWin ctor names):** `00d73220` CQ_HangingTreeGoodScript::CHTG_Executioner (AudienceMembers); `00e5f240` CV_BodyGuardScript::CHenchmanComment; `00e5f640` ::CHenchmanRegionEntranceComment; `00e5f880` ::CHenchmanRegionExitComment (HenchmanVector; the three in FableWin/binary order); `00e64d90` CV_ChickenKickingScript::CChickenSign (TextKeys, vector<CWideString>).
- **Label corrections:** all five carry the propagated `CACompressor::CACompressor`.
### K069 -- entity-script Init(): one game-interface call on Me (5 of 5 matched, MATCH)
- **Function:** `Game->X(Me, value);` in the private Init (entity vtable slot 2).
- **Instances (all proven: vtable slot 2 stored by the landed Alloc + FableWin.pdb Init names; callee names from cgsi_slots_aligned.tsv):** `00dd4480` CQ_PrisonEscapeRescueMotherScript::CPrisonGuard -> EntitySetCutsceneBehaviour(Me, 2) (slot 512); `00e10e20` CQ_WaspBossScript::CWaspChaseWoman -> EntitySetAsScared(Me, true) (496); `00e11630` CQ_WaspBossScript::CWaspVictim -> same; `00e475b0` CV_BanditCampPathScript::CBCP_Bandit -> SetThingPersistent(Me, true) (477); `00ec09b0` CV_ParanoidWhispersScript::CPW_NecklaceDiggingSpot -> SetDiggingSpotAsHidden(Me, true) (578).
- **Header debt:** the ECutsceneBehaviour enumerator for 2 is unknown (cast).
### K070 -- NScript::CPredicate_And<L,R> scalar deleting destructors (5 of 5 matched, RELOCATION_MATCH)
- **Function:** ??_G (slot 2): And's vtable, ~Right, ~Left (out of line), CPredicateBase's inline dtor vtable store 0x012C2FF4, conditional delete. Template with a user-declared inline virtual dtor, emitted by explicit instantiation.
- **Instances (proven: vtable slot 2 + member offsets fix the instantiation, all present in FableWin/Ego_r):** `00e245a0` And<Alive,Alive>; `00ef5b80` And<And<Alive,Alive>,Alive> (Left's dtor 0x00EF5B50 = ~And<Alive,Alive>, declared as an out-of-line explicit specialisation -- VC7.1 otherwise inlines it); `00ef6ec0` the 5-deep And (Left = 4-deep, dtor 0x00EF6DE0); `00ef7b10` And<Conscious,Alive>; `00ef8700` And<Conscious,Conscious>.
- **Label corrections:** all five carry the propagated `CLandscapeBackgroundSubNodeSplitBranch::scalar_deleting_destructor`; 0x00EF5B50's manifest label names the 4-deep And but its body (vtable 0x012E1B70, Right +0x14) is ~CPredicate_And<CIsThingAlive,CIsThingAlive>.
### K071 -- level-script Main(): bind one entity script, post-add (5 of 5 matched, RELOCATION_MATCH)
- **Function:** `AddEntityScriptBinding(new CEntityScriptBinding<T>("Name", this, &E::Alloc, 0)); PostAddScriptedEntities(); Game->PostAddScriptedEntities();` -- the landed 0x00D61D20 binding idiom.
- **Instances (script vtable slot 2 + landed Alloc + name literal):** `00e96f10` CV_HeroDollsScript ("BS_Teacher"; proven, FableWin Main name); `00efa010` Cguy_scriptScript ("henchmen"); `00efb1e0` CProximityPodiumTestScript; `00efb740` CHitSwitchTestScript; `00efbca0` CGuildSealTriggerTestScript (probable: test scripts outside the PDB name sets).
- **Label corrections:** catalogued only as `_gapscan::sub_<addr>`.
### K072 -- CUserProfileManager::GetMax*Detail() const (0 of 4 matched, parked)
- **Function:** `if (cap != -1) return (float)cap; CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef(); if (def) return def->MaxXDetail; return 0.0f;`
- **Why parked (84 v 67 bytes):** retail compares and filds the cap straight from memory in a one-slot (`push ecx`) frame, reads the field into the def pointer's own slot and releases with `mov eax,[ecx+4] ; mov edx,[ecx+70h] ; dec eax ; mov [esp],edx ; mov [ecx+4],eax ; jne`; every form tried (base/derived refcount, RemoveReference/Release/dtor-local release, signed/unsigned count, early-return order, /Os, optimize("s")/("sy"), QFE 4035) gives `mov eax,[cap] ; sub esp,8 ; ...` with the value in a second slot and `add eax,4 ; dec edx ; test edx,edx` (or a frame pointer under "s").
- **Instances (proven: PDB names; the four getters sit in the PDB's address order and read the matching Max field):** `004084b0` GetMaxEffectsDetail (+0x70, g_MaxDetailA 0x0137507C); `00408500` GetMaxTextureDetail (+0x58, g_MaxDetailD); `00408550` GetMaxShadowDetail (+0x60, g_MaxDetailB); `004085a0` GetMaxMeshDetail (+0x68, g_MaxDetailC).
- **Label corrections:** all four carry the propagated `CUserProfileManager::GetMaxMeshDetail` (right only for 0x004085A0).
### K073 -- STLport __copy over elements with an out-of-line operator= (4 of 4 matched, RELOCATION_MATCH)
- **Function:** random-access `__copy` (`for (n = last - first; n > 0; --n) *result++ = *first++;`), the landed 0x00623150 text; one operator= call per element.
- **Instances (probable: element from the per-element operator= callee + stride):** `0040e290` CWideString; `0049ae30` CTexture (8 bytes); `008aac10` CCharString; `00a81fc0` std::list<C2DLineF> (landed operator= 0x00517720).
- **Label corrections:** all four carry the propagated `_Copy_impl<std::pair<CCharString,CCharString>*, ...>`.
### K074 -- CPersistContext_TransferableComponent<CCharString/CWideString>::TransferIn/Out (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `CPersistTraits<T>().TransferX(context, arg, *Object);` -- the traits temporary (DefaultVal T) is built and destroyed inline around the landed traits member.
- **Instances (proven: vtable slots 0 Out / 1 In + PDB names + landed traits callees):** `0040e4d0` <CCharString>::TransferIn; `0040e5f0` <CCharString>::TransferOut; `0040e6d0` <CWideString>::TransferIn; `0040e800` <CWideString>::TransferOut.
- **Label corrections:** manifest `CPersistContext_TransferableComponent::vfunc_0/1`.
### K075 -- CPersistContext_TransferableComponent<4-byte T>::TransferOut (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the object's value copied into a long frame word, `TransferOutLong(context, out, &value)` (__fastcall 0x009E2560).
- **Instances (T from the CPersistContext::Transfer<T> that stores the vtable):** `0040e910` long (proven, landed Transfer<long> 0x00410BE0); `0040f970` unsigned long (proven, bound Transfer<unsigned long> 0x004106F0); `004b8680` ENumAcceptedBoasts (probable, landed Transfer<ENumAcceptedBoasts> 0x004B8D70); `00685860` EOpinionAttitudeType (proven: "ForcedAttitude" key + Ego_r member type + FableWin name).
- **Label corrections:** the landed TransferIn siblings carry T in their file names as long / unsigned long / int / EOpinionAttitudeType, but 0x004B8660's vtable belongs to Transfer<ENumAcceptedBoasts>, not int; all four TransferIn bodies are landed as the address-named stand-in CTransferableLong_009e2590::TransferIn (one name for four instantiations -- review).
### K076 -- STLport uninitialized_fill_n over a 4-byte __false_type element (4 of 4 matched, MATCH)
- **Function:** the dispatcher with `__uninitialized_fill_n(.., __false_type())` inlined: placement-new null test, one dword copy per slot, unsigned n.
- **Instances:** `006132b0` CHeroLogBook::CHeroLogBookEntry::ECategory (probable: the sibling insert-overflow 0x00618200 is bound under vector<ECategory>); `0040ea70` ETutorialCategory (probable: its caller's overflow 0x00411A10 is bound under ETutorialCategory); `0085dc30`, `00ae4fe0` descriptive (CUnprovenEnum4_<addr>: 4-byte element of an unidentified vector).
- **Label corrections:** all four carry the propagated `_Fill_n<unsigned long*, unsigned int, unsigned long>` (unsigned long is a POD: STLport would not null-test).
### K077 -- CPersistTraits<4-byte T>::TransferBinaryIn (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed CPersistTraits<long>::TransferBinaryIn 0x0040FE60 body as the template member: inline range-checked 4-byte stream read into a local, copy out.
- **Instances:** `0040f8a0` unsigned long (proven: binding + Transfer<unsigned long> caller); `004aa270` ESaveType (probable: caller Transfer<ESaveType> 0x004AB640); `006148a0` CHeroLogBook::CHeroLogBookEntry::ECategory (probable: binary arm of TransferVector<ECategory>; its landed caller names it CPersistTraits<long>, whose symbol is 0x0040FE60); `0067aa40` EOpinionTargetingConditionType (proven: landed GFSerialiseMapBinaryIn caller, FableWin name).
- **Label corrections:** all four carry the propagated `CPersistTraits<float>::TransferBinaryIn`.
### K078 -- CPersistContext_TransferableVector<enum T>::TransferIn (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `T* elements = TransVector->begin(); elements[index] = CPersistTraits<T>().TransferIn(context, parser);` (the fastcall long reader 0x009E2590 into a local homed in the dead parser slot). The begin() local is what keeps the vector start in esi across the call.
- **Instances (vtable slot 3; T from the Transfer that builds the vtable):** `0040fb70` ETutorialCategory (proven: landed Transfer<ETutorialCategory> "GivenTutorialCategories" + FableWin instantiation); `00566330` CTCHeroOnlineScoreboard::EOnlineScoreboardActivity (probable: bound Transfer<vector<EOnlineScoreboardActivity>>); `005e8cc0` descriptive (local vectors "experience"/"renown"/"gold"); `006144c0` CHeroLogBook::CHeroLogBookEntry::ECategory (probable).
- **Label corrections:** manifest `CPersistContext_TransferableVector::vfunc_3`.
### K079 -- STLport vector<4-byte enum>::_M_fill_insert (4 of 4 matched, RELOCATION_MATCH)
- **Function:** vector.c `_M_fill_insert` verbatim over a __false_type 4-byte element: the landed vector<CComplexNumber> 0x00A4A3D0 with __copy_backward_ptrs inlined; out-of-line __uninitialized_copy / fill / uninitialized_fill_n (K076) / _M_insert_overflow.
- **Instances:** `00411f10` ETutorialCategory (probable: overflow 0x00411A10 bound under ETutorialCategory); `00618ad0` CHeroLogBook::CHeroLogBookEntry::ECategory (probable: overflow 0x00618200 bound under vector<ECategory>); `0085e6c0`, `00ae6f90` descriptive (CUnprovenEnum4_ named after their K076 fill callee).
- **Label corrections:** behavioural manifest labels (Std_Vector_Insert_Range_4Bytes, Vector_InsertRange_uint, ...).
### K080 -- CDefinitionManager::GetDef<T>(const CCharString&, CDefPointer<const T>*) const, size TUs (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed speed-TU GetDef 0x0083B000 under optimize("y",off)+("s",on) with the PDB signature (const member, out pointer); the converting operator= must be __forceinline.
- **Instances (probable: callers + FableWin.pdb instantiations):** `004196b2` CSoundDef; `00593666` CExpressionDef; `00cd2994` CScriptDef ("SCRIPT_DEF"); `00cd2a71` CCutsceneDef (RunCutsceneMacro).
- **Label corrections:** all four carry the propagated `CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>`; reloc_bindings holds stand-in forms for them (non-const QAE, reference out parameter, CDefinitionManager_Calls).
### K081 -- STLport vector<4-byte enum>(const vector&) with the copy inlined (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `_Vector_base(x.size())` (malloc) then the inline __false_type __uninitialized_copy (placement-new null test).
- **Instances:** `00419f50` CConsoleCommandParameters::CConsoleCommandParameters(const CConsoleCommandParameters&) (proven: FableWin name + binding; its vector<EGenericVarType> member copy needs a __forceinline vector ctor); `00811d40` vector<EObjectAugmentationType> (probable: CHitParameters::Augmentations); `0082c620` vector<CMinimapData::MinimapTheme> (probable: landed __uninitialized_copy<CMinimapData> caller + binding); `0085de40` descriptive.
- **Label corrections:** behavioural labels (Vector_FromRange, CHitParameters_CopyFromVector, Vector_CopyInit, vector<uint>); the landed 0x0082CAC0 "CEditPrimitiveStats::CEditPrimitiveStats" is the CMinimapData copy constructor.
### K082 -- NResourceDirectoryNames constant-folder getters (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `CWideString __fastcall GetXDir() { return CWideString(L"Data\\X\\"); }` -- the landed GetLevelDir 0x0041A060.
- **Instances (proven: PDB getter order matches the retail literal sequence):** `0041a0a0` GetMiscDir; `0041a100` GetGraphicBankDir; `0041a180` GetCacheDir; `0041a1e0` GetShaderBankDir.
- **Label corrections:** the tree already holds these four as stand-ins (Global_GetMiscDirectoryA / GetGraphicsDirectory / GetCacheDirectory / GetShadersDirectory, `FableGet*_<addr>`); the manifest says CUserProfileManager::GetAutoSaveFileName.
### K083 -- NResourceDirectoryNames user-data subfolder getters (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `return GetUserDataDir() + L"<sub>";` with the same-TU GetUserDataDir inlined (the landed GetSaveDir 0x0041A540 one level shorter).
- **Instances (proven: FableWin names in the PDB getter order + literals):** `0041a4f0` GetSaveDirWithoutTrailingBlackslash ("Saves"); `0041a5b0` GetScoreboardImageCacheDir ("Scoreboard\\"); `0041a600` GetPhotojournalDir ("Photojournal\\"); `0041a650` GetTattooUserDir ("Tattoos\\").
- **Label corrections:** all four carry the propagated `_global::GetSaveDir`.
### K084 -- STLport _Rb_tree<NUISystem::CComponent*, ...>::_M_lower_bound (4 of 4 matched, MATCH)
- **Function:** the landed _Rb_tree<unsigned long,...>::_M_lower_bound 0x005D84F2 (size TU) over a pointer key.
- **Instances (proven: names the landed callers bind):** `0042ad91` map<CComponent*, long>; `0042ae90` map<CComponent*, CComponent*> (CTCInventoryTrade); `005b1d38` map<CComponent*, pair<long,long> >; `005b1da0` map<CComponent*, CList*>.
- **Label corrections:** manifest `BinarySearchTree_Find*` are behavioural.
### K085 -- STLport _Rb_tree<CCharString, ...>::_M_find in frame/size TUs (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed _M_find 0x00756350 text under optimize("y",off)+("s",on) with less<CCharString>::operator() 0x0042AAB3 out of line; the header read into a local declared after the cursors (edi), the candidate in the frame word.
- **Instances:** `00cd2b86` map<CCharString, CScriptGameResourceObjectScriptedThingBase> (proven: binding; cutscene cast map); `0042b467`, `0042b52b`, `00cd2b1c` descriptive (CUnprovenValue_<addr> mapped types, empty placeholder structs).
- **Label corrections:** behavioural manifest labels (StdMap_FindLE, BinarySearchTree_LowerBound, CDefStringTable_RBTree_Find, FindRegisteredCutsceneActor).
### K086 -- CThingSearchTools::GetAllThingsInRoughArea<CThing, std::vector<CThing*>, F> with one out-of-line filter (0 of 4 matched, parked)
- **Function:** the K016 vector model with TThing = CThing (the mask-0 test keeps `xor dl,dl ; jne`) and the filter's operator() called on the thing; the same single SIB operand-order gap as K016 (retail `[ecx+eax+4]`, lane `[eax+ecx+4]`), every other byte identical.
- **Instances:** `00436d60` F = CThingFilter_OverlappingCircle (probable: the unbound filter 0x0063EF50 is a {C3DVector, float} X/Y circle-overlap test; PDB name exists; caller CDisplayEngine::DoRender); `00863e70` F = CThingFilter_HasPhysicsMesh (proven, bound filter); `008adcc0` F = CThingFilter_HasTC (proven, bound filter); `00978250` F = CThingFilter_HasScriptName (proven, bound filter).
- **Label corrections:** the manifest's `GetPBestThingInRoughArea<CThingFilter_HasTrackName, CThingCompare_Nearest, CThingTrackNode>` on all four was propagated across byte-identical bodies (no compare call; vector push_back). 0x00978250's stand-in `?GetAllThingsInRoughArea@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasScriptName@@AAUCThingPtrVector@@@Z` has the right function and filter, but the container is std::vector<CThing*>.
### K087 -- STLport __linear_insert over pair<K, CCountedPointer<V> > (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else __unguarded_linear_insert(last, val, comp);` -- the landed 0x00ADBC50 model; ~val inline (agent950's CCountedPointer ReleaseData).
- **Key lever:** the pair's operator= must stay IMPLICIT (VC7.1 emits it out of line and calls it, as retail 0x00448380..); a declared-only operator= makes VC reload the value's Info around the calls (128 v 113).
- **Instances (proven: reloc_bindings names from the landed callers/__insertion_sort rows):** `00448e90` <EGameAction, const CInputTypeBase>; `00826dd0` <long, CContainmentVolume>; `008293b0` <long, CRegionConnectionNode>; `00aea270` <long, NParticleEngine::CParticleEmitter>.
- **Label corrections:** behavioural manifest labels (Vector_InsertWithMove, CVectorMap_InsertOrAssign, ...); the pair operator= callees 0x00448380.. are bound under stand-in names (FableHeap_00448950 pair<...>) and landed as `CItem::operator=`.
### K088 -- STLport sort_heap over pair<K, CCountedPointer<V> > (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap/__pop_heap_aux inline and __pop_heap out of line (the landed 0x007590C0 text; the value copy add-refs into the callee's argument area).
- **Instances (proven: reloc_bindings names; the same four element types as K087):** `00448f10` <EGameAction, const CInputTypeBase>; `00826e50` <long, CContainmentVolume>; `00829430` <long, CRegionConnectionNode>; `00aea2f0` <long, NParticleEngine::CParticleEmitter>.
- **Label corrections:** all four carry the propagated `_Sort_heap<pair<unsigned long, C3DHelperPoint>*, ...>` (a Dinkumware spelling of another element).
### K089 -- def-class default constructors with CDefString members (4 of 4 matched, RELOCATION_MATCH)
- **Function:** base ctor, vtable, each CDefString member's inline `TablePos(-1)` as `or [m],-1` (size TU); POD members untouched.
- **Instances (proven: vtable RTTI + PDB ctor names + Ego_r layouts):** `0044c1dc` CCarrySlotDef (CSubDefClassBase; DummyPosName / SecondaryDummyPosName); `00456f09` CVillagerInteractionsDef (CParentDefClassBase; Animation1/2, retail = PDB - 4); `00457f73` CSpecialAbilitiesThunderLightningStormDef (InitSound / LoopingSound); `004d5eca` CCarriedReadableDef (TitleTextTag / BodyTextTag).
- **Label corrections:** all four carry the propagated `CCarriedReadableDef::CCarriedReadableDef` (right only for 0x004D5ECA); 0x0044C0C0 is bound under nine names (CSubDefClassBase, CShoveHaPennyRegionDef, CDefBase, ...) -- the PDB's is ??0CSubDefClassBase@@QAE@XZ.
### K090 -- def-class default constructors with three out-of-line member constructors (4 of 4 matched, RELOCATION_MATCH)
- **Function:** base ctor, vtable, then the class-type members' default constructors (CCharString / STLport map / COpinionSet), declaration order.
- **Instances (proven: vtable RTTI + PDB ctor names + Ego_r layouts at retail offsets):** `0044c3aa` CTattooDef (three CCharStrings); `00456ffe` COpinionSourceDef (map<EOpinion,float>, map<EOpinionReactionType,bool>, COpinionSet); `00458573` CArmourDef (map<EObjectAugmentationType,float>, two map<EDamageAttribute,float>); `004e0860` CExperienceDef (map<long,float>, map<long,long>, map<long,float>).
- **Label corrections:** all four carry the propagated `CTattooDef::CTattooDef`; the map ctors 0x004534C4 / 0x00453447 / 0x00453536 / 0x00453554 / 0x004DF956 / 0x004DF86C are landed as `std::map<T_xxx>::map` / CDisplayManager::CopyBackBufferToTexture stand-ins and bound as CMember_00453447 / CDamageMap / CLongLongMap; 0x004538EB (landed as CApprenticeSpeedTest::OnPersist) is COpinionSet::COpinionSet().
### K091 -- one-argument constructors with two member initialisers (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `X(arg) : Base(arg), m1(v1), m2(v2) { }` in size TUs (the pass-through `push dword ptr [esp+8]` needs optimize("s")).
- **Instances (proven: vtable RTTI + FableWin ctor names + Ego_r members):** `0044c4d5` CCombatAbilityProjectileAttackData(ECombatAbilityInterfaceType) (AbleToStrafeAndShoot false, ShotAccuracyPercentage 30); `004d3af4` CTCCameraPointTrack(CThing&) (CameraStringLength 7, CameraCageRadius 2); `004de623` CTCActivationReceptorCreatureGenerator(CThing&) (TriggerOnActivate true, ActivateOnActivate false); `00787dbc` CTCMeleeTrap(CThing&) (PlayingContinuous / StrikeAnimation false).
- **Label corrections:** propagated labels (CComponentContainer, CTCShotDefinitionNymphFun ctor) on three of the four.
### K092 -- default constructors passing a type constant to the base (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `X() : Base(type) { }` (size TU).
- **Instances (proven: vtable RTTI + FableWin ctor names):** `0044c535` CCombatAbilityBlockCounterAttackDef (interface type 5); `0044c54d` CCombatAbilityFlourishCounterAttackDef (6); `0044c565` CCombatAbilityGetHitCounterAttackDef (9 -- the landed 0x0044C503 header says 7: corrected); `008cf8f0` CCreatureAction_TentacleStrikeGetHit (CCombatActionBase action type 150).
- **Label corrections:** propagated `CIntelligentPointer<const CThing>` / `C3DAnimFileHelpersChunk` constructor labels.
### K093 -- block attack-def default constructors (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `X() : CCombatAbilityBlockDefBase(interfaceType, weaponClass) { }` (size TU).
- **Instances (proven: vtable RTTI + FableWin ctor names):** `00453e6a` CCombatAbilityBlockUnarmedAttackDef (1, 0); `00453e84` ...LightWeaponAttackDef (2, 1); `00453e9e` ...HeavyWeaponAttackDef (3, 2); `00453eb8` ...ProjectileWeaponAttackDef (4, 3).
- **Label corrections:** manifest `CCombatAbilityBlockDefBase::Constructor_CapacityN` (behavioural).
### K094 -- def-class Transfer(CPersistContext&) over five members (4 of 4 matched, RELOCATION_MATCH)
- **Function:** vtable slot 18: `context.Transfer(member)` per member through the CPersistContext::Transfer<T>(T&) helpers (float 0x00431061, long 0x00431102, bool 0x0043314A, vector<long> 0x00466A47, ELightingChannel 0x004735D6); size TU (every member address formed in eax).
- **Instances (proven: vtable slot 18 + FableWin names + Ego_r members):** `00457f13` CSpecialAbilitiesDrunkennessDef; `004dea13` CThingDrainLifeShotDef (MaxAngle before MaxDistance); `004e7ee0` CBossDef (three vector<long> + two scalars); `004eb8c3` CMultiStaticMeshEntryDef.
- **Label corrections:** none (the manifest names are right); 0x00466A47 is bound as stand-in CStlVector/CLongVector Transfer overloads -- referenced here as ??$Transfer@V?$vector@J...@std@@@CPersistContext@@QAEXAAV...@Z.
### K095 -- STLport vector<T>::_M_allocate_and_copy, frame/size TUs (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `result = allocate(n); __uninitialized_copy(first, last, result, tag()); return result;` under optimize("y",off)+("s",on); the allocator's allocate must be __forceinline (size mode calls it), the copy stays out of line (__fastcall, tag homed in the dead n slot).
- **Instances:** `0045f008` vector<float> (proven: landed __uninitialized_copy<const float*> 0x0045EFE5 names it); `005f83d7` vector<CDefPointer<const CHeroAbilityDef> >; `005f8bd8` vector<pair<long, EHeroAbility> >; `005f9a3c` vector<pair<long, CDefPointer<const CHeroAbilityDef> > > (proven: reloc_bindings names).
- **Label corrections:** behavioural manifest labels (CActiveFile_AllocateAndCopyVector, std_vector_allocate_*).
### K096 -- GFSerialiseMapBinaryIn<float, K, less<K> >, frame/size TUs (4 of 4 matched, RELOCATION_MATCH)
- **Function:** clear (out of line), inline count read, per entry key + float traits readers then map::insert; the landed 0x00469381 recipe (optimize("y",off)+("s",on), function-scope empty traits, same-TU noinline traits stand-ins, inline pair default ctor).
- **Instances (probable):** `004616d6` map<EObjectAugmentationType,float> (key reader 0x00452D8E bound); `0046190a` map<EDamageAttribute,float> (insert 0x004619BA bound); `00466889` map<EHeroTrainableStatType,float> (key reader 0x00463DCB bound); `004e50c0` map<long,float> (key reader CPersistTraits<long> 0x0040FE60).
- **Label corrections:** catalog label <long,long> is propagated; these maps are float-valued.
### K097 -- CParentDefClassBase children's virtual Copy(const CDefClassBase*) with one member (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `*this = *static_cast<const X*>(other);` with the implicit operator= inlined: base operator= 0x00431ED0 out of line, then the scalar member; base modelled locally (packed 0x39, as landed 0x00460256).
- **Instances (proven: slot 19 + FableWin.pdb ?Copy@X@@UAEXPBVCDefClassBase@@@Z):** `0046280e` CCreatureAbilityDef (Type +0x3C); `0046284b` CThingGroupDef (Dummy +0x39); `004629b9` CEngineThemeGroupDef (Dummy +0x39); `004e468b` CWaspQueenBattleDef (NumFramesPauseBetweenAttacks +0x3C).
- **Label corrections:** catalog `X::X::Copy` labels are right in class; 0x00431ED0 carries many propagated names (BaseCopy, Copy_00431ed0, ??4CBase_00431ed0) -- it is CParentDefClassBase::operator=.
- **Header debt:** engine/CParentDefClassBase.h lacks the polymorphic CDefClassBase base, so def classes still model the base locally.
### K098 -- STLport vector<T>::erase(first, last), 12-byte class elements, size TU (4 of 4 matched, MATCH)
- **Function:** the landed 0x004640D1 model: `__copy_ptrs(last, finish, first, __false_type)` element loop (implicit operator= = three movsd), destroy folds, `finish = i; return first`.
- **Instances (probable, via their only caller):** `00466de5` vector<CBrainUpdateZone> (bound resize 0x00466D9C); `004672d6` vector<CBrainBehaviour> (bound resize 0x0046728D); `0046bba2` vector<CMusicSetEntry> (bound resize 0x0046BB59); `0047073f` vector<pair<unsigned long, CSoundPair> > (resize 0x004706F6 called by CSoundMap::TransferBinaryIn on this+0 = SoundMap).
- **Label corrections:** `_global::Vector_Erase` / `std::vector_copy_12byte_norealloc` are behavioural; 0x00466DE5 is bound as the stand-in ?Erase@CRetailVector_elem12@@ by the landed resize 0x00466D9C -- this renames it.
### K099 -- STLport vector<T>::erase(first, last), 8-byte class elements, size TU (4 of 4 matched, MATCH)
- **Function:** the K098 / landed 0x004640D1 model with an 8-byte element (implicit operator= as two dword moves).
- **Instances (probable, via their only caller):** `00469e3e` vector<CTextureReplacementDef::CEntry> (bound resize 0x00469E00; CEntry members from the 8-byte OriginalTexture/NewTexture Ego_r CEntry, probable); `0046b182` vector<CAtmosBankEntry> (bound resize 0x0046B144); `0046c46e` vector<CObjectFamilyEntry> (bound resize 0x0046C430); `00470a66` vector<pair<unsigned long, CDefString> > (resize 0x00470A28 from CSoundMap::TransferBinaryIn on this+0x10 = CriteriaMap).
- **Label corrections:** `Vector_Erase` / `vector_copy_8byte*_norealloc` behavioural; 0x00469E3E is bound as stand-in ?erase_00469e3e@CArrayPair@@ (landed resize 0x00469E00) -- renamed here; the resizes' catalog label `CArray<pair<long,long>>::push_back` is propagated.
### K100 -- STLport vector<T>::_M_insert_overflow (__false_type), 8-byte class elements, frame/size TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed 0x0046AECC (CSoundBankEntry) model verbatim with an 8-byte element.
- **Instances (proven: reloc_bindings + only caller the vector's landed _M_fill_insert):** `00469fc8` vector<CTextureReplacementDef::CEntry>; `0046b30c` vector<CAtmosBankEntry>; `0046c5f8` vector<CObjectFamilyEntry>; `00470bf0` vector<pair<unsigned long, CDefString> > (this binding also confirms K099's 0x00470A66 element type).
- **Label corrections:** `Vector_Reallocate` / `vector_insert_reallocate_8byte*` are behavioural.
### K101 -- STLport vector<T>::resize(n, const T&), 16-byte elements, size TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `n < size() ? erase(begin()+n, end()) : insert(end(), n-size(), x)`, insert forwarding to the out-of-line _M_fill_insert; accessors __forceinline.
- **Instances:** `0046b681` vector<CMusicEntry> (proven, bound; CSoundDef::MusicEntries chain); `004e9f47` vector<NParticleAttachment::CParticleAttachmentInfo> (probable: CParticleAttacherDef::Transfer slot 18 -> this+0x28 ParticlesToAttach); `004eb41f` vector<CExplosionRing> (proven, bound); `00cd3ccb` vector<CScriptGameResourceObjectScriptedThingBase> (proven: the resize(n) caller's element ctor stores that class's RTTI vtable 0x0127094C).
- **Label corrections:** catalog `CArray<pair<long,..>>::push_back` is propagated. reloc_bindings `?resize@?$vector@VCMusicEntry@@@std@@QAEXI@Z` on 0x004E9F20 is WRONG -- it is vector<NParticleAttachment::CParticleAttachmentInfo>::resize(n) (CMusicEntry's resize(n) is 0x0046B65A). 0x00CD3CCB's stand-in names ?Resize@CThingRefVector@@ / ?resize@CScriptedThingVector@@ and the landed 0x00CD3DA4 "CThingRef" element are the scripted-thing-base vector.
### K102 -- STLport vector<T>::_M_insert_overflow (__false_type), def-class elements with out-of-line copy ctor, frame/size TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the K100 model with `T(const T&)` and `_M_clear()` out of line.
- **Instances (proven: the placement copies call the bound ??0X@@QAE@ABV0@@Z):** `00476816` vector<CAbilityButtonGuiDef> (0x5C); `00476cbb` vector<CLensFlareElementDef> (0x4C); `00479bb2` vector<CEngineLocalDetailLayerDef> (0x54); `004ee137` vector<CMultiStaticMeshEntryDef> (0x38).
- **Label corrections:** `AbilityButton_AllocCopyInsert`, `*_vector_Reallocate`, `Vector_ReallocateAndCopy` are behavioural; 0x004EE137's binding spells it public (QAE) where STLport 4.6 and the K100 bindings have it protected (IAE); 0x004EB864 also carries the stand-in ??0CElement56_004eb864@@.
### K103 -- STLport vector<T>::_M_fill_insert, CParentDefClassBase-derived def elements, frame/size TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed 0x004EDFDC model (inline copy helpers, __fastcall uninitialized_fill_n, _M_insert_overflow with a value-initialised __false_type); x_copy's destructor is the implicit element dtor (a bare ~CParentDefClassBase 0x00430300 call, no vptr store) or the element's out-of-line dtor.
- **Instances (proven: bound copy ctor + operator= of the element):** `00478c79` vector<CAbilityTrackLevelDef> (0x48); `00479630` vector<COpinionDeedReactionDef> (0x50, dtor 0x0047955B); `0047a08c` vector<CAppearanceModifierScalingDef> (0x48); `0047a4a2` vector<CAbilityTrackDef> (0x50, dtor 0x00478722).
- **Label corrections:** catalog `std::vector::insert` is generic; 0x00430300 carries 11 propagated names -- it is ~CParentDefClassBase (frees SubDefInfoMap +0x29, CDefPointeeBase vptr, jmp ~CResource); 0x00478722 is bound as the stand-in ?CAbilityTrackDef_Destruct@@YIXPAX@Z -- it is ~CAbilityTrackDef; 0x004787F6/0x004784B3 also carry ??0CElement72_* stand-ins.
- **Element callee names introduced:** ??1COpinionDeedReactionDef@@UAE@XZ -> 0x0047955B, ??1CAbilityTrackDef@@UAE@XZ -> 0x00478722.
### K104 -- STLport vector<T>::_M_clear(), element dtor = one member's out-of-line dtor (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed 0x006FBE60 (family J007) model: inline `_Destroy` loop then deallocate.
- **Instances:** `0047bd70` vector<CHeroLogBook::CQuestObjectiveDesc> (probable: caller the landed operator= 0x0047BE50; ~CCharString at +4); `004d1800` vector<CUnprovenCharStringEntry_004d1800> (descriptive: no caller or reference; 8-byte element, CCharString at +4); `006e2230` vector<pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > > (probable: caller the landed operator= 0x006E22F0; the desc's implicit dtor is the bare ~CBaseClass call); `006f3450` vector<CDefendingCombatantInfo::CProng> (probable: caller the landed operator= 0x006F3C20; POccupier +0x14, ~CBaseIntelligentPointer).
- **Label corrections:** `CCombatWheel::ResetRings` is propagated (J007); 0x0047BD70 / 0x006F3450 carry stand-ins ?_M_clear@CQuestObjectiveDescVector@@ / ?_M_clear@CSlotVector@@ -- they are vector<...>::_M_clear.
### K105 -- STLport vector<T*>::_M_insert_overflow (__true_type), frame/size TUs (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed F005 0x007A91A0 overload with allocate/_M_clear/_M_set __forceinline and std::__copy_trivial 0x00405BA0 + fill_n called (__fastcall).
- **Instances (proven: reloc_bindings):** `00485dbc` vector<NUISystem::CComponent*>; `0065625e` vector<NPlayerGui::CDrawBase*>; `0066ed1d` vector<const NHeroInformationScreens::CBase*>; `00788ffc` vector<CThingGameObject*>.
- **Label corrections:** `Std_Vector_Insert_Range_Fill`, `std_vector_grow_ulong`, `Vector_InsertValue`, `Vector_Assign` are behavioural; 0x00485DBC / 0x00788FFC also carry stand-ins (?Grow@ChildVector@NUISystem@@, ?InsertOverflow@ScrollingArray@NUISystem@@, ?_M_insert_overflow@?$CRetailVector@..., ?_M_insert_overflow@ThingVector@@, ?_M_insert_overflow@?$CVector@...); 0x00405BA0 also carries ?ListBoxDataHelper@@YIKKKK@Z.
### K106 -- CThingSearchTools::GetAllThings<CThing, vector<CThing*>, F> with one out-of-line filter (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed 0x005198B0 model verbatim with another filter (local CThingSearchToolsMethods; header debt as landed).
- **Instances:** `0048d400` F = CThingFilter_HasTC (proven); `0085b0d0` F = CUnprovenThingFilter_0085afd0 (descriptive: unnamed filter 0x0085AFD0, caller CCombatActionBase::OnAnimationLoaded); `008adba0` F = CThingFilter_HasDefGlobalIndex (proven); `008adc30` F = CThingFilter_InMap (proven).
- **Label corrections:** the catalog's `GetAllThings<..., CFilter_Bind2<CThingFilter_InMap, unary_negate<CThingFilter_InArea>>>` label is propagated onto all four -- each has a single filter; 0x0048D400 / 0x008ADBA0 carry stand-ins (?GetAllThings@CThingSearchTools@@QAEXABJ..., ?GetAllWithTC@ThingSearchMethods@@, ?GetAllThings@CThingSearchTools@@QAEXABVCThingFilter_HasDefGlobalIndex@@AAUCThingVector@@@Z).
### K107 -- STLport list<T>::push_back(const T&), 4-byte elements (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed 0x007A3870 (family H010) model with a 4-byte element.
- **Instances:** `004ac480` list<CUnprovenElement_004ac480*> (descriptive: no caller); `004ac4b0` list<CUnprovenElement_004ac4b0*> (descriptive: callers CWorldMap::PopulateRegionWithVillagers, CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed); `0052d740` list<NUISystem::CChangingStateComponent::EChangingStateComponentState> (probable: ChangeState pushes onto StatesToDo); `005550d0` list<CEnginePrimitiveHandle*> (probable: CMorphingSprite::Handles, pushed object has CEnginePrimitiveHandle RTTI).
- **Label corrections:** `LinkedList_AppendNode` / `LinkedList_InsertNode` / `list::push_back_node` behavioural.
### K108 -- STLport vector<CIntelligentPointer<T> >::_M_insert_overflow (__false_type) (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed 0x007B6EF0 (CThingMarker) model verbatim with another pointee.
- **Instances (proven: the inline copy stores the CIntelligentPointer<T> RTTI vtable):** `004ac940` T = CThing (0x01238C6C); `0074d600` T = CThingCreatureBase (0x0123E7D8); `007afd10` T = CThingAICreature (0x0123E7E0); `007b99e0` T = CThingObject (0x0125A858).
- **Label corrections:** `Std_Vector_Insert_Range_SmartPtr`, `Vector_Resize_WithElements`, `vector_insert_range_CBaseIntelligentPointer`, `Vector_InsertRange_SmartPtr` behavioural; stand-ins on 0x004AC940 (?InsertOverflow@CHandleVector@@, ?_M_insert_overflow@?$CPointerVector@UCIntelligentPointerThing2@@, ?_M_insert_overflow@?$CRetailVector@...), 0x007AFD10 (?_M_insert_overflow@CCreaturePointerVector@@, CPointerVector<CIntelligentPointerCreature>), 0x007B99E0 (?InsertGrow@?$CPtrVector@...).
### K109 -- CThingSearchTools::GetAllLeafThings<TThing, vector<X*>, F> with one out-of-line filter (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed 0x004AD700 model with a filter call (local CThingSearchToolsMethods; header debt as landed).
- **Instances:** `004ad780` <const CThingBuilding, vector<const CThingBuilding*>, CThingFilter_HasTC> (probable); `0077ec20` <CThingObject, vector<CThingObject*>, CThingFilter_HasTC> (probable; list 5 = CThingObject from its ctor 0x007031C0); `00910a90` <CThingObject, vector<CThingObject*>, CUnprovenThingFilter_009109b0> (descriptive filter); `00947780` <CThingBuilding, vector<CThing*>, CThingFilter_HasTC> (probable).
- **Label corrections:** the catalog's `GetAllLeafThings<CThingAICreature, ..., CFilter_Bind2<CThingFilter_EnemyCreature, CThingFilter_InMap>>` is propagated onto all four; 0x004AD780 stand-in ?GetAllLeafThings@CThingSearchTools@@QAEXABVCThingFilter_HasTC@@AAU?$vector@PBVCThing@@@@@Z; 0x0049C780 also carries ?GetThingList@@YGPAUList@@H@Z.
### K110 -- default constructors of unreferenced two-string records (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `X::X() {}` with members CCharString (+0) and CCharString/CWideString (+4), both default ctors called.
- **Instances (descriptive, CUnprovenStringRecord_<addr>):** `004aebc0` (CCharString, CCharString); `00526f50` (CCharString, CWideString); `00528830` (CCharString, CWideString); `009f0b10` (CCharString, CCharString). None has a caller, jump or address reference.
- **Label corrections:** catalog `QuestData::QuestData` is unsupported.
### K111 -- STLport _Rb_tree<K, V>::erase(const key_type&), speed TUs (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed G053 0x0042CDE0 body with equal_range inline (upper_bound then lower_bound calls), cdecl __distance out of line, erase(first, last); needs __distance's body as a same-TU noinline stand-in so both iterators stay in registers across the call.
- **Instances:** `004bb270` <CCharString, pair<const CCharString, long> > (proven via bound bounds); `007ade60` <CCharString, pair<const CCharString, bool> > (proven, bound); `008ae420` <CCharString, pair<const CCharString, vector<CCharString> > > (probable: lower bound bound as map<CCharString, vector<CCharString>>; gossip callers); `00990cd0` <CThreadedFileManager::CPendingObjectSortKey, pair<const ..., CThreadedFileManager::CFileObject*> > (proven, bound lower bound).
- **Label corrections:** `map_find_lower_upper`, `Map_EraseRange_CCharString`, `map_Erase_Range`, `CThreadedFileManager_RemovePendingObject` are behavioural; stand-ins on 0x004BB270 (?erase@CCharStringSet@@, ?Assign@CountedPtr@@), 0x008AE420 (?Consume@GGlobal@@, ?RemoveGV@GObj@@, ?RemoveGossip@Gossip@@, ?remove@Mgr@@), 0x00990CD0 (?erase@CPendingMap@@).
### K112 -- unreferenced std::swap<T> COMDATs, 12-byte elements (4 of 4 matched, MATCH)
- **Function:** STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;` (__fastcall), the landed J013 0x00758350 recipe.
- **Instances (descriptive, CUnprovenElement12_<addr>):** `004c1e90`, `004c2b60` (CMeshInfo sort TU; CMeshInfo is a candidate for one of them), `00adaa00`, `00adaf60` (CVertexGroupBlockOptimiser::CBlockInfo sort TU; candidate for one). No caller or reference.
- **Label corrections:** catalog `GFToScreen` is wrong (propagated).
### K113 -- STLport __uninitialized_copy (__false_type), 12-byte trivially copied classes (4 of 4 matched, MATCH)
- **Function:** null-tested placement copy loop (__fastcall).
- **Instances:** `004c2ad0` <const NGraphicAppearance::CMeshInfo*, CMeshInfo*> (proven, bound); `006b56f0` <const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, ...> (proven, bound); `008f6800` <CHeightMapCell*, CHeightMapCell*> (proven, bound); `009d23f0` <CBankFile::CRuntimeData*, ...> (probable: reached from the bound vector<CBankFile::CRuntimeData>::resize 0x009D4010).
- **Label corrections:** catalog `_Uninit_copy<_Vector_const_iterator<unsigned int>...>` is a propagated Dinkumware label; 0x009D23F0 / 0x009D3580 / 0x009D3A20 use the stand-in N009d3a20::Element -- it is CBankFile::CRuntimeData; 0x006B56F0 also carries ??$__uninitialized_copy@PBUSElement12@@.
### K114 -- STLport list<T*>::remove(T* const&) (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed 0x004BA390 model with a pointer element (inline erase, node freed).
- **Instances:** `004c3460` list<NGraphicAppearance::CAGraphicAppearanceComponent*> (probable: CTCGraphicAppearance::Components); `00863130` list<CThing*> (probable: the landed 0x008631B0 caller converts to CThing*); `0090b8e0` list<LSocket*> (probable: LSocketServer callers); `0091ad70` list<CUnprovenElement_0091ad70*> (descriptive).
- **Label corrections:** catalog `list<CTCBase*>::remove` is propagated onto all four; stand-ins ?remove@CComponentListObject@@, ?remove@CThingPointerList@@, ?remove@?$list@PAVLSocket@@@@ (no std) are these std::list removes.
### K115 -- unreferenced pair<CIntelligentPointer<T>, X> copy constructors (4 of 4 matched, RELOCATION_MATCH)
- **Function:** STLport pair copy ctor with the __forceinline intelligent-pointer copy (base ctor, vtable, SetPItem(GetPItem())), then the second; emitted by explicit instantiation of the pair.
- **Instances (descriptive seconds; `first` proven by the stored RTTI vtable):** `004cbbb0` <CIntelligentPointer<CThing>, CUnprovenDword_004cbbb0> (int likeliest); `006a2bd0` <CIntelligentPointer<CThing>, CUnprovenByte_006a2bd0>; `007ca520` <CIntelligentPointer<CThingObject>, CUnprovenByte_007ca520>; `008828d0` <CIntelligentPointer<CThing>, CUnprovenByte_008828d0>.
- **Label corrections:** catalog `CIntelligentPointer<const CThing>::CIntelligentPointer` on three is wrong (they are pair copy ctors; the pointee is non-const CThing / CThingObject per RTTI).
### K116 -- STLport list<T>::remove(const T&), 4-byte elements (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the K114 model.
- **Instances:** `004cc150` list<CTCBase*> (probable: CThing::WorldUpdate / AddTC / CTCBase::RemoveFromGlobalTCs); `006405b0` list<CUnprovenElement_006405b0*> (descriptive); `0084ace0` list<long> (probable: the landed WearGameObject caller's worn-id list); `009f98d0` list<CProfile*> (probable: CProfileManager::RemoveProfile on ProfileList +0x4C).
- **Label corrections:** catalog `list<CTCBase*>::remove` propagated onto three that are not; the landed 0x006404B0 comment claims list<CTCBase*> for 0x006405B0 -- that symbol is 0x004CC150's; stand-ins ?remove@TList@@, ?remove@CLongList@@.
### K117 -- unreferenced std::swap<pair<K, S> > with an out-of-line second (4 of 4 matched, RELOCATION_MATCH)
- **Function:** STLport swap over STLport pair (user copy ctor), second's copy ctor / operator= x2 / dtor called.
- **Instances (descriptive keys CUnprovenKey_<addr>):** `004cf400` second CCharString; `00565b60` second CWideString; `009928a0`, `00992a90` second CSoundBankRecord (the landed stand-in name already bound to 0x009922F0 / 0x00992630 / 0x00992510 -- reused, not invented; the two differ in their key type).
- **Label corrections:** catalog `swap<NParticleEngine::CRegisteredPSCInfo>` propagated onto all four (that class is 12 bytes).
- **Header debt:** CSoundBankRecord is a stand-in name with no PDB match (the pair<long, CSoundBankRecord> sort helpers 0x00992750.. use it); its real class is unidentified.
### K118 -- STLport _Destroy(first, last) out of line, element dtor = one member's dtor (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `_Destroy(first,last)` -> `__destroy_aux(..., __false_type)` loop (__fastcall), emitted by explicit instantiation.
- **Instances:** `004d0ee0` CUnprovenCharStringEntry_004d0ee0 (descriptive; caller the landed operator= 0x00719B30 "CElement8bVector"); `006e2200` pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > (probable); `006f2b00` CDefendingCombatantInfo::CProng (probable); `00722e80` CTCSmokeGenerator::CGenerationPos (proven: CTCSmokeGenerator slot 12 -> operator= on GenerationPositions).
- **Label corrections:** catalog `_Destroy_range<allocator<CSpeechFrame>>` propagated onto all four; stand-ins ?DestroyRange@@YIXPAUCElement8b@@0@Z (0x004D0EE0), ?_Destroy@@YIXPAUSSlot@@0@Z (0x006F2B00).
### K119 -- thing-component destructors, compiler-generated and out of line (4 of 4 matched, RELOCATION_MATCH)
- **Function:** implicit ~X(): the last member's out-of-line dtor, the CDefPointer's inline release (`if (Object) { Object->Release(); Object = 0; }`), the base dtor tail-jumped; no vptr store. Size TU. Emitted via `new X` (vtable -> ??_G -> out-of-line implicit ??1).
- **Instances (proven: only caller the landed ??_G; RTTI base chain; Ego_r members):** `004dc03d` ~CTCActionUseReadable (TextTag CWideString, PDef; base CTCActionUseBase 0x004DA967); `004dc659` ~CTCVillageMember (PVillage, PDef; ~CTCBase 0x00686830); `004e0bf6` ~CTCParticleAttacher (ParticleList vector<CIntelligentPointer<CThing>>, PDef); `004e1856` ~CTCAISummon (MySummonedCreatures list<CIntelligentPointer<CThing>>, PSummonDef).
- **Label corrections:** catalog `CCreatureAction_PerformExpressionExtended::~...` propagated onto all four; the landed ??_G rows call stand-ins ?X_Destruct@@YIXPAX@Z (and 0x004DA967 is ?CTCActionUseBase_Destruct@@) -- they are ??1X@@UAE@XZ / ??1CTCActionUseBase@@UAE@XZ.
- **Header debt:** engine/CTCBase.h has no virtual destructor; modelled locally.
### K120 -- unreferenced STLport copy_backward(first, last, result) wrappers (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);` (__fastcall, frame/size TU), the callee out of line.
- **Instances (named from the callee):** `004dfda6` pair<CCharString, CTCBase::CRegisteredTCInfo> (probable); `005946f1` pair<long, CDefPointer<const CContextSensitiveItemDef> > (probable); `005f93d1` pair<long, EHeroAbility> (proven: bound callee); `005f93f6` pair<long, long> (proven: bound callee). None has a caller.
- **Label corrections:** catalog `CCombatAction_GenericResponseToDecapitation::CCombatAction_GenericResponseToDecapitation` is propagated onto all four; the callees' ?Helper@CForwarder_*@@ stand-ins are __copy_backward.
### K121 -- STLport vector<T>::_M_insert_overflow (__false_type), 16-byte non-POD, helpers out of line, frame/size TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** K102 shape with __uninitialized_copy / __uninitialized_fill_n as same-TU noinline STLport bodies (tags then share the atEnd slot [ebp+1Bh]), copy ctor and _M_clear out of line.
- **Instances:** `004e3526` vector<pair<CCharString, CTCBase::CRegisteredTCInfo> > (proven: bound pair copy ctor); `00656d5e` vector<pair<C3DVector, CCharString> > (probable: CDrawMiniMap::RegionExits chain); `00cd36df` vector<CScriptGameResourceObjectScriptedThingBase> (proven, bound); `00cd3dcd` vector<CScreenFilterSThingByPass> (proven, bound).
- **Label corrections:** `Vector_GrowReserve`, `std_vector_insert_pair_C3DVector_CCharString_range`, `CActiveFile_*` are behavioural; stand-ins ?InsertGrow@CRetailVector_CElement16_* on 0x004E3526 / 0x00656D5E / 0x00CD36DF, ??0CElement16_00654dd4@@ on 0x00654DD4 (= pair<C3DVector, CCharString> copy ctor).
### K122 -- STLport sort_heap with __pop_heap_aux out of line, size TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `while (last - first > 1) pop_heap(first, last--, comp);`, pop_heap __forceinline, __pop_heap_aux called (__fastcall).
- **Instances:** `004e41dd` <pair<CCharString, CTCBase::CRegisteredTCInfo>*, CKeyPairCompareLess<...> > (probable, stand-in binding); `00594b8d` <pair<long, CDefPointer<const CContextSensitiveItemDef> >*, CContextSensitivePriorityCompare> (probable, stand-in binding); `005f9b1e` <pair<long, EHeroAbility>*, CKeyPairCompareLess<long, EHeroAbility> > (proven); `005f9b5a` <pair<long, long>*, CKeyPairCompareLess<long, long> > (proven).
- **Label corrections:** catalog `_Sort_heap<pair<long, CDefPointer<const CContextSensitiveItemDef>>*, ...>` propagated onto all four.
### K123 -- STLport vector<T>::_M_allocate_and_copy for def elements, size TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** allocate + __uninitialized_copy inline (__forceinline), _Construct out of line (__fastcall); one-parameter vector model matching the landed operator= rows' bound names.
- **Instances:** `004e9c63` vector<CShopItemDef> (proven, bound; engine/CShopItemDef.h); `004eacbb` vector<CPrizeScoreDef> (probable); `004eafe8` vector<CShoveHaPennyRegionDef> (probable); `004ebedb` vector<CCardPositionsDef> (proven, bound).
- **Label corrections:** `AllocateAndCopyPairStringDatetime`, `CShoveHaPennyRegionDef_AllocateCopyConstruct*`, `Pair_CCharString_CArray_AllocateCopyConstruct` are behavioural/propagated; 0x004EACBB / 0x004EAFE8 bind as X_004eaad0 / X_004eae1e namespace stand-ins.
### K124 -- STLport __copy_ptrs (__false_type) over def classes with two own dwords, frame/size TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** __copy element loop (__forceinline) with the element operator= written out __forceinline (base operator= call + two dwords; size mode would emit the implicit one out of line).
- **Instances:** `004eabe5` <const CPrizeScoreDef> (probable; operator= path, stand-in ?copy@X_004eaad0@@); `004ec03f` <const CCardDef> (proven, bound one-parameter spelling); `004edaf3` <CPrizeScoreDef*, CPrizeScoreDef*> (proven, bound); `004fa22c` <CCardDef*, CCardDef*> (proven, bound). The const-source (operator=) and non-const (erase) instantiations are distinct COMDATs.
- **Label corrections:** `VectorReallocateCopyConstruct`, `Vector_InsertCopyRange_0x30`, `Vector_CopyElements_Destroy` are behavioural.
### K125 -- STLport uninitialized_fill_n for def classes, size TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `__uninitialized_fill_n(..., __false_type)` loop inline, `_Construct` out of line (__fastcall).
- **Instances (proven, all bound; callers the landed _M_fill_insert rows):** `004ed437` CShopItemDef; `004eddf9` CPrizeScoreDef; `004fa087` CCardPositionsDef; `004fa516` CCardDef.
- **Label corrections:** catalog `_Fill_n<unsigned long*, unsigned int, unsigned long>` is propagated onto all four.
### K126 -- STLport _Vector_base(size_t n, const allocator&), 4/8-byte elements, speed TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** the landed G010 0x004F9450 body in a speed TU.
- **Instances:** `005128b0` <pair<long, long> > (proven: caller the bound vector<pair<long,long>> copy ctor 0x005BAEC4); `0062ed90` <CIntelligentPointer<CThing> > (proven: caller the landed copy ctor 0x0062ED40); `0062ee20` <CCountedPointer<NCreatureAction::CActionEventBase> > (probable: CCreatureActionBase::ActionEvents chain); `0062f5d0` <const CAnimationEntry*> (proven, bound).
- **Label corrections:** `Vector_UninitializedCopy_Allocator`, `std_vector_Allocate_*` behavioural; stand-ins ??0CRetailVectorBase_CIntelligentPointer@@ / ??0CRetailVectorBase_CCountedPointer@@ on 0x0062ED90 / 0x0062EE20.
### K127 -- STLport random_shuffle(first, last) with rand(), 4-byte elements (4 of 4 matched, RELOCATION_MATCH)
- **Function:** `for (i = first+1; i != last; ++i) iter_swap(i, first + rand() % (i - first + 1));` (__fastcall).
- **Instances:** `005131d0`, `007304f0`, `00777080` (descriptive CUnprovenElement_<addr>* -- local ranges in PopulateRegionWithVillagers, CTCDummyVillager::FrameUpdate, CTCShop::GetSaleItemsToPlace / SpotTheAddition); `007d7860` CCard* (probable: CTCTavernGameCardBase::Cards via ShuffleCards).
- **Label corrections:** `PopulateRegionWithVillagers_RandChance`, `Array_RandomShuffle`, `Vector_RandomShuffle`, `Algorithm_FisherYatesShuffle` behavioural.
### K128 -- STLport __unguarded_linear_insert over pointer arrays, comparator out of line (4 of 4 matched, RELOCATION_MATCH)
- **Function:** STLport loop, comparator by value (4/12/16 bytes) called thiscall on its argument slot.
- **Instances:** `00514050` <CThingBuilding**, CThingBuilding*, CIsHouseLargerThan> (proven, bound); `0095cda0` <CThing**, CThing*, CThingCompare_Nearest> (probable: bound template comparator returning CThing*); `009789b0`, `0097ada0` <CThing**, CThing*, (anonymous)::CThingCompare_InDirectionOfAndNearestToHeuristicBitch> (proven, bound; two TUs' anonymous-namespace comparators -- the ?A0x hash in my object differs from retail's, the Leaf prefix stops before it).
- **Label corrections:** `InsertionSort_Compare`, `Sort_SiftDown_CThingComparator`, `std_sort_SiftUp_HeuristicBitch*` behavioural.
### K129 -- STLport vector<T>::_M_allocate_and_copy with an out-of-line element copy ctor, speed TU (4 of 4 matched, RELOCATION_MATCH)
- **Function:** allocate + null-tested placement-copy loop inline; each instance keeps the spelling of its existing binding (member template vs const-pointer overload; with or without the allocator parameter) so one address keeps one name.
- **Instances (proven, all bound):** `00515020` vector<CMapInfo> (member template); `006758b0` vector<CArray<CRandomAppearanceMorph::CBodyPartMesh> > (const overload, with allocator); `007092b0` vector<vector<CTCHeroAttachableAppearanceModifiers::CModifierEntry> > (const overload); `00a2b030` vector<C3DMeshInfo> (member template, with allocator).
- **Label corrections:** `AllocateAndInitMapInfoArray`, `Vector_AllocateNested`, `Vector_AllocAndInitVectorElements`, `C3DMeshInfo_AllocArray` behavioural; element copy ctors 0x00675230 / 0x00708830 carry only stand-ins (CElement12_*, CModifierEntryList).

## Probable identities (107)
- 0040e290 K073 std::__copy<CWideString*, CWideString*, int>
- 0040ea70 K076 std::uninitialized_fill_n<ETutorialCategory*, unsigned int, ETutorialCategory>
- 00411f10 K079 std::vector<ETutorialCategory>::_M_fill_insert
- 004196b2 K080 CDefinitionManager::GetDef<CSoundDef>(const CCharString&, CDefPointer<const CSoundDef>*) const
- 00436d60 K086 CThingSearchTools::GetAllThingsInRoughArea<CThing, std::vector<CThing*>, CThingFilter_OverlappingCircle>
- 004616d6 K096 GFSerialiseMapBinaryIn<float, EObjectAugmentationType, less<EObjectAugmentationType> >
- 0046190a K096 GFSerialiseMapBinaryIn<float, EDamageAttribute, less<EDamageAttribute> >
- 00466889 K096 GFSerialiseMapBinaryIn<float, EHeroTrainableStatType, less<EHeroTrainableStatType> >
- 00466de5 K098 std::vector<CBrainUpdateZone>::erase(iterator, iterator)
- 004672d6 K098 std::vector<CBrainBehaviour>::erase(iterator, iterator)
- 00469e3e K099 std::vector<CTextureReplacementDef::CEntry>::erase(iterator, iterator)
- 0046b182 K099 std::vector<CAtmosBankEntry>::erase(iterator, iterator)
- 0046bba2 K098 std::vector<CMusicSetEntry>::erase(iterator, iterator)
- 0046c46e K099 std::vector<CObjectFamilyEntry>::erase(iterator, iterator)
- 0047073f K098 std::vector<pair<unsigned long, CSoundPair>>::erase(iterator, iterator)
- 00470a66 K099 std::vector<pair<unsigned long, CDefString>>::erase(iterator, iterator)
- 0047bd70 K104 std::vector<CHeroLogBook::CQuestObjectiveDesc>::_M_clear()
- 0049ae30 K073 std::__copy<CTexture*, CTexture*, int>
- 004aa270 K077 CPersistTraits<ESaveType>::TransferBinaryIn
- 004ad780 K109 CThingSearchTools::GetAllLeafThings<const CThingBuilding, std::vector<const CThingBuilding*>, CThingFilter_HasTC>(const CThingFilter_HasTC&, std::vector<const CThingBuilding*>&)
- 004b8680 K075 CPersistContext_TransferableComponent<ENumAcceptedBoasts>::TransferOut
- 004c3460 K114 std::list<NGraphicAppearance::CAGraphicAppearanceComponent*>::remove(NGraphicAppearance::CAGraphicAppearanceComponent* const&)
- 004cc150 K116 std::list<CTCBase*>::remove(CTCBase* const&)
- 004dfda6 K120 std::copy_backward<std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, std::pair<CCharString, CTCBase::CRegisteredTCInfo>*>
- 004e41dd K122 std::sort_heap<std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>>
- 004e50c0 K096 GFSerialiseMapBinaryIn<float, long, less<long> >
- 004e9f47 K101 std::vector<NParticleAttachment::CParticleAttachmentInfo>::resize(unsigned int, const NParticleAttachment::CParticleAttachmentInfo&)
- 004eabe5 K124 std::__copy_ptrs<CPrizeScoreDef>(const CPrizeScoreDef*, ..., const __false_type&)
- 004eacbb K123 std::vector<CPrizeScoreDef>::_M_allocate_and_copy(unsigned int, const CPrizeScoreDef*, const CPrizeScoreDef*)
- 004eafe8 K123 std::vector<CShoveHaPennyRegionDef>::_M_allocate_and_copy(unsigned int, const CShoveHaPennyRegionDef*, const CShoveHaPennyRegionDef*)
- 0052afb0 K009 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>>::erase(const key_type&)
- 0052d740 K107 std::list<NUISystem::CChangingStateComponent::EChangingStateComponentState>::push_back(const NUISystem::CChangingStateComponent::EChangingStateComponentState&)
- 005550d0 K107 std::list<CEnginePrimitiveHandle*>::push_back(CEnginePrimitiveHandle* const&)
- 00566330 K078 CPersistContext_TransferableVector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>::TransferIn
- 00593666 K080 CDefinitionManager::GetDef<CExpressionDef>(const CCharString&, CDefPointer<const CExpressionDef>*) const
- 005946f1 K120 std::copy_backward<std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*>
- 00594b8d K122 std::sort_heap<std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, CContextSensitivePriorityCompare>
- 005d97f0 K002 CTCInventoryItem::IsWeapon(long)
- 005d9830 K002 CTCInventoryItem::IsExpression(long)
- 005d98b0 K002 CTCInventoryItem::IsStockItem(long)
- 005d98f0 K002 CTCInventoryItem::IsHeroTitle(long)
- 005d9930 K002 CTCInventoryItem::IsTrophy(long)
- 005d9970 K002 CTCInventoryItem::IsBook(long)
- 006132b0 K076 std::uninitialized_fill_n<CHeroLogBook::CHeroLogBookEntry::ECategory*, unsigned int, CHeroLogBook::CHeroLogBookEntry::ECategory>
- 006144c0 K078 CPersistContext_TransferableVector<CHeroLogBook::CHeroLogBookEntry::ECategory>::TransferIn
- 006148a0 K077 CPersistTraits<CHeroLogBook::CHeroLogBookEntry::ECategory>::TransferBinaryIn
- 00618ad0 K079 std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory>::_M_fill_insert
- 0062ee20 K126 std::_Vector_base<CCountedPointer<NCreatureAction::CActionEventBase>>::_Vector_base(size_t, const allocator&)
- 00639590 K009 std::_Rb_tree<long, long>::erase(const key_type&)
- 00656d5e K121 std::vector<std::pair<C3DVector, CCharString>>::_M_insert_overflow(..., const __false_type&, unsigned int, bool)
- 006e2200 K118 std::_Destroy<std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >*>(std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >*, std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >*)
- 006e2230 K104 std::vector<pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >>::_M_clear()
- 006e8b10 K015 CDefendingCombatantInfo::CProng::Reset()
- 006f2b00 K118 std::_Destroy<CDefendingCombatantInfo::CProng*>(CDefendingCombatantInfo::CProng*, CDefendingCombatantInfo::CProng*)
- 006f3450 K104 std::vector<CDefendingCombatantInfo::CProng>::_M_clear()
- 006f3570 K016 CThingSearchTools::GetAllThingsInRoughArea<CThingObject, std::vector<const CThing*>, CThingFilter_None>
- 006f37b0 K016 CThingSearchTools::GetAllThingsInRoughArea<CThingBuilding, std::vector<const CThing*>, CThingFilter_None>
- 006f50b0 K009 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CCombatantInfo>>::erase(const key_type&)
- 0077ec20 K109 CThingSearchTools::GetAllLeafThings<CThingObject, std::vector<CThingObject*>, CThingFilter_HasTC>(const CThingFilter_HasTC&, std::vector<CThingObject*>&)
- 007921c0 K025 std::_Rb_tree<CThingCreatureBase*, pair<CThingCreatureBase* const, CTCDivineWrath::PerCreatureInfo>, _Select1st, less, allocator>::erase(iterator)
- 00792640 K025 std::map<CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo>::erase(iterator)
- 00797760 K025 std::_Rb_tree<CThingCreatureBase*, pair<CThingCreatureBase* const, CTCUnholyPower::PerCreatureInfo>, _Select1st, less, allocator>::erase(iterator)
- 00797a60 K025 std::map<CThingCreatureBase*, CTCUnholyPower::PerCreatureInfo>::erase(iterator)
- 007d7860 K127 std::random_shuffle<CCard**>(CCard**, CCard**)
- 00811d40 K081 std::vector<EObjectAugmentationType>::vector(const vector& x)
- 0082c620 K081 std::vector<CMinimapData::MinimapTheme>::vector(const vector& x)
- 0084ace0 K116 std::list<long>::remove(const long&)
- 00863130 K114 std::list<CThing*>::remove(CThing* const&)
- 008aac10 K073 std::__copy<CCharString*, CCharString*, int>
- 008ae420 K111 std::_Rb_tree<CCharString, pair<const CCharString, vector<CCharString>> >::erase(const CCharString&)
- 0090b8e0 K114 std::list<LSocket*>::remove(LSocket* const&)
- 00916f30 K044 CThingSearchTools::GetPBestThing<CIsFreeLogPile, CThingCompare_Nearest, CThingObject>
- 00916fc0 K044 CThingSearchTools::GetPBestThing<CIsFreeBonfire, CThingCompare_Nearest, CThingObject>
- 0091c030 K015 CTCNymph::ClearTarget()
- 00947780 K109 CThingSearchTools::GetAllLeafThings<CThingBuilding, std::vector<CThing*>, CThingFilter_HasTC>(const CThingFilter_HasTC&, std::vector<CThing*>&)
- 00959380 K016 CThingSearchTools::GetAllThingsInRoughArea<const CThingObject, std::vector<const CThingObject*>, CThingFilter_None>
- 0095c7c0 K016 CThingSearchTools::GetAllThingsInRoughArea<CThingObject, std::vector<CThing*>, CThingFilter_None>
- 0095cda0 K128 std::__unguarded_linear_insert<CThing**, CThing*, CThingCompare_Nearest>
- 00993030 K021 std::vector<std::pair<long, CSoundBankRecord>>::erase(iterator, iterator)
- 009aaae0 K018 std::list<CCountedPointer<NBankFileManager::CRetailBank> >::erase(iterator)
- 009d23f0 K113 std::__uninitialized_copy<CBankFile::CRuntimeData*, CBankFile::CRuntimeData*>(..., const __false_type&)
- 009d7c00 K018 std::list<CCountedPointer<CBankFileAsyncData> >::erase(iterator)
- 009f0e80 K025 std::map<unsigned long, CCodeSectionManager::CSectionDesc>::erase(iterator)
- 009f98d0 K116 std::list<CProfile*>::remove(CProfile* const&)
- 00a81fc0 K073 std::__copy<std::list<C2DLineF>*, std::list<C2DLineF>*, int>
- 00c640b0 K063 CEaxContext::SetReflectionsDelayScale(unsigned long, long)
- 00c64140 K063 CEaxContext::SetReverbScale(unsigned long, long)
- 00c641d0 K063 CEaxContext::SetReverbDelayScale(unsigned long, long)
- 00c64260 K063 CEaxContext::SetDecayHFLimit(unsigned long, long)
- 00c642f0 K063 CEaxContext::SetEchoTimeScale(unsigned long, long)
- 00c64b00 K064 CEaxSource::SetObstructionLFRatio(float)
- 00c64db0 K064 CEaxSource::SetDopplerFactor(float)
- 00c64e20 K064 CEaxSource::SetRolloffFactor(float)
- 00c64e90 K064 CEaxSource::SetRoomRolloffFactor(float)
- 00c64f00 K064 CEaxSource::SetAirAbsorptionFactor(float)
- 00cd2994 K080 CDefinitionManager::GetDef<CScriptDef>(const CCharString&, CDefPointer<const CScriptDef>*) const
- 00cd2a71 K080 CDefinitionManager::GetDef<CCutsceneDef>(const CCharString&, CDefPointer<const CCutsceneDef>*) const
- 00d73220 K068 NScript::CQ_HangingTreeGoodScript::CHTG_Executioner::CHTG_Executioner(CQ_HangingTreeGoodScript&, CScriptDataBase*, const CScriptThing&)
- 00e5f240 K068 NScript::CV_BodyGuardScript::CHenchmanComment::CHenchmanComment(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&)
- 00e5f640 K068 NScript::CV_BodyGuardScript::CHenchmanRegionEntranceComment::CHenchmanRegionEntranceComment(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&)
- 00e5f880 K068 NScript::CV_BodyGuardScript::CHenchmanRegionExitComment::CHenchmanRegionExitComment(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&)
- 00e64d90 K068 NScript::CV_ChickenKickingScript::CChickenSign::CChickenSign(CV_ChickenKickingScript&, CScriptDataBase*, const CScriptThing&)
- 00efa010 K071 NScript::Cguy_scriptScript::Main()
- 00efb1e0 K071 NScript::CProximityPodiumTestScript::Main()
- 00efb740 K071 NScript::CHitSwitchTestScript::Main()
- 00efbca0 K071 NScript::CGuildSealTriggerTestScript::Main()
- 01229170 K004 `dynamic atexit destructor for 'CTCTavernGame::GetTextWithNarrator::no_text_entry''

## Descriptive identities (91)
- 0042b467 K085 std::_Rb_tree<CCharString, pair<const CCharString, CUnprovenValue_0042b467>, ...>::_M_find
- 0042b52b K085 std::_Rb_tree<CCharString, pair<const CCharString, CUnprovenValue_0042b52b>, ...>::_M_find
- 004427d0 K007 std::deque<CUnprovenElement_004427d0>::~deque()
- 0048573d K004 CUnprovenForwarder_0048573d()
- 004ac480 K107 std::list<CUnprovenElement_004ac480*>::push_back(CUnprovenElement_004ac480* const&)
- 004ac4b0 K107 std::list<CUnprovenElement_004ac4b0*>::push_back(CUnprovenElement_004ac4b0* const&)
- 004aebc0 K110 CUnprovenStringRecord_004aebc0::CUnprovenStringRecord_004aebc0()
- 004c1e90 K112 std::swap<CUnprovenElement12_004c1e90>(CUnprovenElement12_004c1e90&, CUnprovenElement12_004c1e90&)
- 004c2b60 K112 std::swap<CUnprovenElement12_004c2b60>(CUnprovenElement12_004c2b60&, CUnprovenElement12_004c2b60&)
- 004cbbb0 K115 std::pair<CIntelligentPointer<CThing>, CUnprovenDword_004cbbb0>::pair(const pair&)
- 004cf400 K117 std::swap<std::pair<CUnprovenKey_004cf400, CCharString> >
- 004d0ee0 K118 std::_Destroy<CUnprovenCharStringEntry_004d0ee0*>(CUnprovenCharStringEntry_004d0ee0*, CUnprovenCharStringEntry_004d0ee0*)
- 004d1800 K104 std::vector<CUnprovenCharStringEntry_004d1800>::_M_clear()
- 005131d0 K127 std::random_shuffle<CUnprovenElement_005131d0**>(CUnprovenElement_005131d0**, CUnprovenElement_005131d0**)
- 0051b880 K009 std::_Rb_tree<CThingBuilding*, pair<CThingBuilding* const, list<CThingAICreature*> >>::erase(const key_type&)
- 00526f50 K110 CUnprovenStringRecord_00526f50::CUnprovenStringRecord_00526f50()
- 00528830 K110 CUnprovenStringRecord_00528830::CUnprovenStringRecord_00528830()
- 00565b60 K117 std::swap<std::pair<CUnprovenKey_00565b60, CWideString> >
- 005e8cc0 K078 CPersistContext_TransferableVector<CUnprovenEnum4_005e8cc0>::TransferIn
- 006405b0 K116 std::list<CUnprovenElement_006405b0*>::remove(CUnprovenElement_006405b0* const&)
- 0069cc00 K011 CIVCountedPointer<CUnprovenPointee_0069cc00>::`scalar deleting destructor'
- 006a2bd0 K115 std::pair<CIntelligentPointer<CThing>, CUnprovenByte_006a2bd0>::pair(const pair&)
- 00710280 K011 CIVCountedPointer<CUnprovenPointee_00710280>::`scalar deleting destructor'
- 007304f0 K127 std::random_shuffle<CUnprovenElement_007304f0**>(CUnprovenElement_007304f0**, CUnprovenElement_007304f0**)
- 00765d00 K023 CUnprovenRecord_00765d00::CUnprovenRecord_00765d00()
- 00768b80 K023 CUnprovenRecord_00768b80::CUnprovenRecord_00768b80()
- 00777080 K127 std::random_shuffle<CUnprovenElement_00777080**>(CUnprovenElement_00777080**, CUnprovenElement_00777080**)
- 00777b30 K009 std::_Rb_tree<CUnprovenKey_00777b30, pair<const CUnprovenKey_00777b30, CUnprovenValue_00777b30>>::erase(const key_type&)
- 007b4d60 K007 std::deque<CUnprovenElement_007b4d60>::~deque()
- 007ca030 K011 CIVCountedPointer<CUnprovenPointee_007ca030>::`scalar deleting destructor'
- 007ca520 K115 std::pair<CIntelligentPointer<CThingObject>, CUnprovenByte_007ca520>::pair(const pair&)
- 0085b0d0 K106 CThingSearchTools::GetAllThings<CThing, std::vector<CThing*>, CUnprovenThingFilter_0085afd0>(const CUnprovenThingFilter_0085afd0&, std::vector<CThing*>&)
- 0085dc30 K076 std::uninitialized_fill_n<CUnprovenEnum4_0085dc30*, unsigned int, CUnprovenEnum4_0085dc30>
- 0085de40 K081 std::vector<CUnprovenEnum4_0085de40>::vector(const vector& x)
- 0085e6c0 K079 std::vector<CUnprovenEnum4_0085dc30>::_M_fill_insert
- 008699f0 K011 CIVCountedPointer<CUnprovenPointee_008699f0>::`scalar deleting destructor'
- 008828d0 K115 std::pair<CIntelligentPointer<CThing>, CUnprovenByte_008828d0>::pair(const pair&)
- 00910a90 K109 CThingSearchTools::GetAllLeafThings<CThingObject, std::vector<CThingObject*>, CUnprovenThingFilter_009109b0>(const CUnprovenThingFilter_009109b0&, std::vector<CThingObject*>&)
- 00913cc0 K007 std::deque<CUnprovenElement_00913cc0>::~deque()
- 0091ad70 K114 std::list<CUnprovenElement_0091ad70*>::remove(CUnprovenElement_0091ad70* const&)
- 009928a0 K117 std::swap<std::pair<CUnprovenKey_009928a0, CSoundBankRecord> >
- 00992a90 K117 std::swap<std::pair<CUnprovenKey_00992a90, CSoundBankRecord> >
- 009f0060 K023 CUnprovenRecord_009f0060::CUnprovenRecord_009f0060()
- 009f00d0 K023 CUnprovenRecord_009f00d0::CUnprovenRecord_009f00d0()
- 009f0b10 K110 CUnprovenStringRecord_009f0b10::CUnprovenStringRecord_009f0b10()
- 00aacbb0 K018 std::list<CCountedPointer<CUnprovenPointee_00aacbb0> >::erase(iterator)
- 00adaa00 K112 std::swap<CUnprovenElement12_00adaa00>(CUnprovenElement12_00adaa00&, CUnprovenElement12_00adaa00&)
- 00adaf60 K112 std::swap<CUnprovenElement12_00adaf60>(CUnprovenElement12_00adaf60&, CUnprovenElement12_00adaf60&)
- 00ae4fe0 K076 std::uninitialized_fill_n<CUnprovenEnum4_00ae4fe0*, unsigned int, CUnprovenEnum4_00ae4fe0>
- 00ae6f90 K079 std::vector<CUnprovenEnum4_00ae4fe0>::_M_fill_insert
- 00b83260 K011 CIVCountedPointer<CUnprovenPointee_00b83260>::`scalar deleting destructor'
- 00c099f0 K058 PKCAutoDelete<CUnprovenPKObject_00c099f0>::Release()
- 00c10b00 K059 CUnprovenPKInterface_012B2748::SetMember888(long)
- 00c11120 K059 CUnprovenPKInterface_012B2748::SetMember7C0(float)
- 00c11200 K059 CUnprovenPKInterface_012B2748::SetMember884(unsigned char)
- 00c14860 K059 CUnprovenPKInterface_012B29F8::SetMember9C(long)
- 00c14890 K059 CUnprovenPKInterface_012B29F8::SetMemberA0(long)
- 00c15f30 K060 CUnprovenPKInterface_012B2A74::SetRecord0C(long)
- 00c15f60 K060 CUnprovenPKInterface_012B2A74::SetRecord10(long)
- 00c15f90 K060 CUnprovenPKInterface_012B2A74::SetRecord14(long)
- 00c15ff0 K060 CUnprovenPKInterface_012B2A74::SetRecord08(long)
- 00c16020 K060 CUnprovenPKInterface_012B2A74::SetRecord04(long)
- 00c17850 K058 PKCAutoDelete<CUnprovenPKObject_00c17850>::Release()
- 00c2bcf0 K061 CDispatcherB_00c2c7e0::Register(CEvent_00c2c7e0*)
- 00c2bd40 K061 CAgentB_00c2c8f0::Handle(CEvent_00c2c8f0*)
- 00c2f070 K058 PKCAutoDelete<CUnprovenPKObject_00c2f070>::Release()
- 00c2f190 K058 PKCAutoDelete<CUnprovenPKObject_00c2f190>::Release()
- 00c2f2b0 K058 PKCAutoDelete<CUnprovenPKObject_00c2f2b0>::Release()
- 00c33600 K004 CUnprovenForwarder_00c33600(arg)
- 00c33820 K004 CUnprovenForwarder_00c33820(arg)
- 00c33990 K004 CUnprovenForwarder_00c33990(arg)
- 00c363e0 K061 CUnprovenPKAgent_00c363e0::Post(void*)
- 00c36e00 K061 CSequencePlayer_00c36ea0::Begin(CSequenceClock*)
- 00c36fb0 K061 CUnprovenPKAgent_00c36fb0::Post(void*)
- 00c40e60 K006 CUnprovenPow10_00c40e60(float)
- 00c42c30 K006 CUnprovenPow10_00c42c30(float)
- 00c54a80 K006 CUnprovenPow10_00c54a80(float)
- 00c5bef0 K062 CUnprovenMixKernel_00c5bef0 (libpk MMX ramped accumulate)
- 00c5c0b0 K006 CUnprovenPow10_00c5c0b0(float)
- 00c6aae0 K062 CUnprovenMixKernel_00c6aae0 (libpk MMX ramped accumulate)
- 00c6bf60 K006 CUnprovenPow10_00c6bf60(float)
- 00c6f1a0 K062 CUnprovenMixKernel_00c6f1a0 (libpk MMX ramped accumulate)
- 00c6f800 K062 CUnprovenMixKernel_00c6f800 (libpk MMX ramped accumulate)
- 00c6fc80 K062 CUnprovenMixKernel_00c6fc80 (libpk MMX ramped accumulate)
- 00c77690 K006 CUnprovenPow10_00c77690(float)
- 00cd2b1c K085 std::_Rb_tree<CCharString, pair<const CCharString, CUnprovenValue_00cd2b1c>, ...>::_M_find
- 00cd45a0 K023 CUnprovenRecord_00cd45a0::CUnprovenRecord_00cd45a0()
- 00f25b40 K050 CUnprovenDef_00f25b40::`scalar deleting destructor'
- 00f25cb0 K050 CUnprovenDef_00f25cb0::`scalar deleting destructor'
- 00f26df0 K050 CUnprovenDef_00f26df0::`scalar deleting destructor'
- 00f26f40 K050 CUnprovenDef_00f26f40::`scalar deleting destructor'

