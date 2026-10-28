# Name recovery and limits

Names were checked against original PDB name sets, own retail RTTI/vtables, callers, literal strings and native member offsets. Donor PDB addresses were not treated as retail addresses. Manifest labels are preserved below for comparison.

Method and member spellings marked descriptive are not claimed recovered. C++ local variable names were chosen for readability; original local names were not recovered. Compiler-generated initialization labels and anonymous/TU-local symbols are distinguished from recovered engine APIs. Shared-header aliases restore missing callable APIs without editing the headers.

Specific new recovered globals: MatrixBufferPool, VectorBufferPool, GroupVectorBufferPool, corroborated by original initializer/atexit name sets and own typed CParticleProgram buffer accessor roles. Corrected CThing::PeekDefinitionManager() const is documented even where the containing row remains parked. Full evidence and uncertainty remain in each ROW.json and source Identity/Layout comments.

## 0069f700: CCreatureAction_SheatheItemToInventory_ForEnflame::Initialise
Original manifest label: `CCreatureAction_SheatheItemToInventory_ForEnflame / CCreatureAction_SheatheItemToInventory_ForEnflame::Initialise`.
Identity: proven: vtable RTTI class/PDB Initialise; own literal0x0123c444="CTCWillEnflame"; base call0x69F480; PCreature+8; AddTC0x4C9D60.
Layout: engine/CCreatureActionBase.h has a data-member vptr and no virtual Initialise/typed PCreature; the local polymorphic base preserves the retail +8 member.
Layout: engine/CCreatureAction_SheatheItemToInventory_ForEnflame.h flattens the polymorphic base and omits typed PCreature; retail lea this+8 targets CBaseIntelligentPointer::GetPItem. The model preserves that real base/member chain.

## 0069f7b0: CCreatureAction_SheatheItemToInventory_ForDivineWrath::Initialise
Original manifest label: `CCreatureAction_SheatheItemToInventory_ForDivineWrath / CCreatureAction_SheatheItemToInventory_ForDivineWrath::Initialise`.
Identity: proven: vtable RTTI class/PDB Initialise; own literal0x0123c280="CTCDivineWrath"; base call0x69F480; PCreature+8; AddTC0x4C9D60.
Layout: engine/CCreatureActionBase.h has a data-member vptr and no virtual Initialise/typed PCreature; the local polymorphic base preserves the retail +8 member.
Layout: engine/CCreatureAction_SheatheItemToInventory_ForDivineWrath.h flattens the polymorphic base and omits typed PCreature; retail lea this+8 targets CBaseIntelligentPointer::GetPItem. The model preserves that real base/member chain.

## 0069f860: CCreatureAction_SheatheItemToInventory_ForUnholyPower::Initialise
Original manifest label: `CCreatureAction_SheatheItemToInventory_ForUnholyPower / CCreatureAction_SheatheItemToInventory_ForUnholyPower::Initialise`.
Identity: proven: vtable RTTI class/PDB Initialise; own literal0x0123c290="CTCUnholyPower"; base call0x69F480; PCreature+8; AddTC0x4C9D60.
Layout: engine/CCreatureActionBase.h has a data-member vptr and no virtual Initialise/typed PCreature; the local polymorphic base preserves the retail +8 member.
Layout: engine/CCreatureAction_SheatheItemToInventory_ForUnholyPower.h flattens the polymorphic base and omits typed PCreature; retail lea this+8 targets CBaseIntelligentPointer::GetPItem. The model preserves that real base/member chain.

## 006a2a10: CVectorMap::LowerBound
Original manifest label: `CVectorMap<long / LowerBound`.
Identity: proven; CTCCarrying::CarrySlots; const HasCarrySlot/GetThingInCarrySlot/IsCarrySlotFree callers; J006 twin0x6A29B0; stride12.

## 006be5e0: CVectorMap::LowerBound
Original manifest label: `CVectorMap<long / LowerBound`.
Identity: proven; CMusicManager::RegisteredMusic at+28 (Ego_r); UpdateCurrentMusicOffset0x6BD8A0 and start-music0x6BD900 callers modify entries; stride24 matching nested20-byte record.

## 00827b60: CVectorMap::LowerBound
Original manifest label: `CVectorMap<long / LowerBound`.
Identity: proven; CRegionConnectionGraph::GraphNodes; const GetNextRegionOnWayToRegion/GetNextNodeOnWayToRegion callers; J006 twin0x827B00; stride12.

## 006a7c20: CThingFilter_IsInQuest::operator
Original manifest label: `CThingFilter_HasScriptData / operator()`.
Identity: proven; PDB ??RCThingFilter_IsInQuest@@QBE_NABVCThing@@@Z; own retail field+0x58 matches Ego_r CThing::SerialisationSectionName rebased-0x10; shared CCharString equality uses CBasicString<char>::Compare0x411570.

## 006a7e30: CThingFilter_HasScriptName::operator
Original manifest label: `CThingFilter_HasScriptData / operator()`.
Identity: proven; PDB ??RCThingFilter_HasScriptName@@QBE_NABVCThing@@@Z; own retail field+0x74 matches Ego_r CThing::ScriptName rebased-0x10; shared CCharString equality uses CBasicString<char>::Compare0x411570.

## 006a7e70: CThingFilter_HasScriptData::operator
Original manifest label: `CThingFilter_HasScriptData / operator()`.
Identity: proven; PDB ??RCThingFilter_HasScriptData@@QBE_NABVCThing@@@Z; own retail field+0x78 matches Ego_r CThing::ScriptData rebased-0x10; shared CCharString equality uses CBasicString<char>::Compare0x411570.

## 006b7af0: std::for_each
Original manifest label: `_For_each<std / _For_each<std::pair<long,CCharString>_const*,CAddDefNamesToComboBox>`.
Identity: proven; CGameMap::Resize0x6B75A0 passes Reset0x6B7490; stride7; PDB _For_each<CGameMapCell*,mem_fun_ref_t<void,CGameMapCell>>. STLport name replaces Dinkumware _For_each.

## 00781370: std::for_each
Original manifest label: `_For_each<std / _For_each<std::pair<long,CCharString>_const*,CAddDefNamesToComboBox>`.
Identity: proven; CTCFlammable::ExtinguishFire0x780C80 calls at0x780C95 with CFirePoint::ExtinguishFire0x780700; FirePoints+16; stride44; matching PDB mem_fun_ref_t instantiation. STLport name replaces Dinkumware _For_each.

## 008f6740: std::for_each
Original manifest label: `_For_each<std / _For_each<std::pair<long,CCharString>_const*,CAddDefNamesToComboBox>`.
Identity: proven; CHeightMap::Resize0x8F61A0 calls at0x8F6226 with Reset0x8F5EB0; stride12; matching PDB mem_fun_ref_t instantiation. STLport name replaces Dinkumware _For_each.
Layout: shared CHeightMapCell data layout reused under an alias solely to add its missing member declaration; all data members remain from the shared header.

## 006b9e00: std::_Rb_tree::operator=
Original manifest label: `_global / LTextTreeWalkThrough_CopyAssign`.
Identity: proven; set<CCharString> callers copy two string sets; _M_erase0x4ABA70 proven string set, _M_copy0x513300 copies CCharString; current binding confirms real tree assignment.

## 0099a1f0: std::_Rb_tree::operator=
Original manifest label: `_gapscan / sub_0099a1f0`.
Identity: descriptive; unreferenced assignment of same instantiation as current descriptive constructor0x99A0B0; _M_erase0x999E50,_M_copy0x99A140; key/value unknown, preserve shared address-suffixed instantiation names.

## 00ad6140: std::map::operator=
Original manifest label: `_global / LTextTreeWalkThrough_AssignTree_DeepCopy`.
Identity: proven; COptimisedVertex PairContractions; current map assignment binding from real PDB operator=0xAD7050; _M_erase0xAD5240 and _M_copy0xAD43A0 match that map.

## 006bbac0: CGameTimeManager::Load
Original manifest label: `CGameTimeManager / Load`.
Identity: proven; PDB exact Load(CPersistContext&); CWorld::LoadGameStateInternal0x4A21F0 calls0x4A2A60 and string-load0x6BBBA0 calls0x6BBBF1; own literals Time/TimeStep/DayCount and member offsets+8/+16/+20.

## 006bbb30: CGameTimeManager::Save
Original manifest label: `CGameTimeManager / Load`.
Identity: proven; PDB exact Save(CPersistContext&); CWorld::SaveGameStateInternal0x49F4C0 calls0x49FA74; own literals Time/TimeStep/DayCount and member offsets+8/+16/+20.

## 00e3bb00: NScript::CV_BordelloScript::CBordelloClient::OnPersist
Original manifest label: `_gapscan / sub_00e3bb00`.
Identity: proven; vtable slot4 at0x12E3B58; PDB nested CBordelloClient unique member triple BrainState+28 ClientID+32 SexChance+36; own three literals agree.

## 006bde50: CMusicManager::EnableDangerMusic
Original manifest label: `CGameEvent / AddUBYTE`.
Identity: proven; own state store matches Ego_r CState::Danger; FableWin.pdb exact method; InternalUpdate(const CState&,bool)0x006BD9B0; current state24bytes.

## 006bdf40: CMusicManager::SetCinematicOverridingMusicSet
Original manifest label: `CGameEvent / AddUBYTE`.
Identity: proven; own state store matches Ego_r CState::CinematicOverrideSet; FableWin.pdb exact method; InternalUpdate(const CState&,bool)0x006BD9B0; current state24bytes.

## 006bdfe0: CMusicManager::SetUIOverridingMusicSet
Original manifest label: `CGameEvent / AddUBYTE`.
Identity: proven; own state store matches Ego_r CState::CinematicOverrideSet; FableWin.pdb exact method; InternalUpdate(const CState&,bool)0x006BD9B0; current state24bytes.

## 006bdef0: CMusicManager::ClearOverridingMusicSet
Original manifest label: `CGameEvent / AddUBYTE`.
Identity: proven; own state store matches Ego_r CState::OverrideSet; FableWin.pdb exact method; InternalUpdate(const CState&,bool)0x006BD9B0; current state24bytes.

## 006bdf90: CMusicManager::ClearCinematicOverridingMusicSet
Original manifest label: `CGameEvent / AddUBYTE`.
Identity: proven; own state store matches Ego_r CState::CinematicOverrideSet; FableWin.pdb exact method; InternalUpdate(const CState&,bool)0x006BD9B0; current state24bytes.

## 006be030: CMusicManager::ClearUIOverridingMusicSet
Original manifest label: `CGameEvent / AddUBYTE`.
Identity: proven; own state store matches Ego_r CState::CinematicOverrideSet; FableWin.pdb exact method; InternalUpdate(const CState&,bool)0x006BD9B0; current state24bytes.

## 006c2b10: std::__copy
Original manifest label: `_Copy_impl<std::pair<CCharString,CCharString>*,std / _Copy_impl<std::pair<CCharString,CCharString>*,std::pair<class_CCharString,class_CCharString>*>`.
Identity: probable; CLevelLoader::CObject::CLevelFiles::operator=0x6C2940; level-file-vector mutable erase caller0x6C2C00 and assignment0x6C2D40; exact28-byte three-counted-file layout.

## 009de940: std::__copy
Original manifest label: `_Copy_impl<std::pair<CCharString,CCharString>*,std / _Copy_impl<std::pair<CCharString,CCharString>*,std::pair<class_CCharString,class_CCharString>*>`.
Identity: proven; CQuickDrawTriInfo::operator=0x9DE6F0 own landed memberwise implementation; vector erase0x9E15E0 calls;60-byte PDB record.

## 00ad7930: std::__copy
Original manifest label: `_Copy_impl<std::pair<CCharString,CCharString>*,std / _Copy_impl<std::pair<CCharString,CCharString>*,std::pair<class_CCharString,class_CCharString>*>`.
Identity: proven; PDB COptimisedVertex operator=0xAD7050; vector erase0xAD8170 explicitly binds mutable __copy; retail stride184.

## 006c7b50: std::vector::_M_allocate_and_copy
Original manifest label: `_global / Std_CopyPairElements`.
Identity: proven; CTCHeroOpinionDeedLog persistence local vector<pair<long,COpinionDeed*>> reserve0x6C8560 calls0x6C7B50; own map at+0xC and OpinionDeedMap literal; stride8; malloc0xBFEA0E.

## 009b27e0: std::vector::_M_allocate_and_copy
Original manifest label: `_global / Deque_AllocateBuffer_8byte`.
Identity: proven; CBankFile::SymbolCRCIndices CArray<pair<unsigned long,unsigned long>>::DeallocateUnusedMemory0x9B7B10 calls reserve0x9B4EB0 -> this helper; stride8; malloc0xBFEA0E.

## 00a39330: std::vector::_M_allocate_and_copy
Original manifest label: `_global / Vector_Reserve_8bytes`.
Identity: proven; CCRCSymbolMap::SLONGMap CArray<pair<unsigned long,long>>::DeallocateUnusedMemory0xA39A00 calls reserve0xA39590 -> this helper; stride8; malloc0xBFEA0E.

## 006cb0d0: std::__uninitialized_copy
Original manifest label: `std::vector / std::vector::_Uninit_copy_arrays`.

## 0070aa50: std::__uninitialized_copy
Original manifest label: `_global / Vector_Copy_BodyPartMesh`.

## 0077b700: std::__uninitialized_copy
Original manifest label: `_global / Vector_CopyConstruct_CBodyPartMesh`.

## 006cb470: std::vector::push_back
Original manifest label: `std::vector<pair<int,CDefPtr>> / std::vector<pair<int,CDefPtr>>::emplace_back`.

## 0070b350: std::vector::push_back
Original manifest label: `_global / std_vector_Cons_val_RandomAppearance`.

## 00721740: std::vector::push_back
Original manifest label: `_global / Vector_Push_CCompositeBlendDesc`.

## 006d3610: std::__copy_backward
Original manifest label: `_global / _Copy_backward<CLipSyncPhoneme*,CLipSyncPhoneme*>`.

## 009b1e20: std::__copy_backward
Original manifest label: `_global / _Copy_backward<CLipSyncPhoneme*,CLipSyncPhoneme*>`.

## 00a3f800: std::__copy_backward
Original manifest label: `_global / _Copy_backward<CLipSyncPhoneme*,CLipSyncPhoneme*>`.

## 006d3af0: std::fill
Original manifest label: `fill<std::pair<CCharString,long>*,std / fill<std::pair<CCharString,long>*,std::pair<class_CCharString,long>_>`.

## 006dbd20: std::fill
Original manifest label: `fill<std::pair<CCharString,long>*,std / fill<std::pair<CCharString,long>*,std::pair<class_CCharString,long>_>`.

## 00ae5820: std::fill
Original manifest label: `fill<std::pair<CCharString,long>*,std / fill<std::pair<CCharString,long>*,std::pair<class_CCharString,long>_>`.

## 006d4a20: std::vector::resize
Original manifest label: `vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> / resize`.

## 00739b40: std::vector::resize
Original manifest label: `vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> / resize`.

## 007b00c0: std::vector::resize
Original manifest label: `vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> / resize`.

## 006da200: std::_Rb_tree_iterator::operator++
Original manifest label: `_global / BSTInterval_FindSuccessor`.

## 006ef560: std::_Rb_tree_iterator::operator++
Original manifest label: `_global / BinaryTree_FindNextInOrder`.

## 00ad0a70: std::_Rb_tree_iterator::operator++
Original manifest label: `_global / BinaryTree_FindMinimumOrSuccessor`.

## 006f2110: std::_Rb_tree::_M_insert
Original manifest label: `CCharStringMap / CCharStringMap::Insert`.

## 007acf70: std::_Rb_tree::_M_insert
Original manifest label: `_global / RBTree_Insert`.

## 00a546d0: std::_Rb_tree::_M_insert
Original manifest label: `_global / MemoryAllocator_InsertNode`.

## 006f8210: CCreatureAction_ScreamerDrainAttack::Clone
Original manifest label: `CCreatureAction_ScreamerDrainAttack / CCreatureAction_ScreamerDrainAttack::Clone`.
Layout: retail CCombatActionBase copy0062f3b0 covers0x114 bytes; fields begin+0x114. Generated headers flatten the vptr and omit the real polymorphic base chain. RTTI proves both base classes; local polymorphic model is required.

## 007f0660: CCombatAction_GenericStrikeResponseKnockdownRecover::Clone
Original manifest label: `CCombatAction_GenericStrikeResponseKnockdownRecover / CCombatAction_GenericStrikeResponseKnockdownRecover::Clone`.
Layout: retail CCombatActionBase copy0062f3b0 covers0x114 bytes; fields begin+0x114. Generated headers flatten the vptr and omit the real polymorphic base chain. RTTI proves both base classes; local polymorphic model is required.

## 008ae330: CCombatAction_RecoverFromStunned::Clone
Original manifest label: `CCombatAction_RecoverFromStunned / CCombatAction_RecoverFromStunned::Clone`.
Layout: retail CCombatActionBase copy0062f3b0 covers0x114 bytes; fields begin+0x114. Generated headers flatten the vptr and omit the real polymorphic base chain. RTTI proves both base classes; local polymorphic model is required.

## 006fc2d0: std::vector::reserve
Original manifest label: `_global / Vector_Reallocate_Generic`.

## 008292d0: std::vector::reserve
Original manifest label: `_global / StdVector_ReserveExact`.

## 00a85a60: std::vector::reserve
Original manifest label: `_global / Vector_Assign_NavQuadTree`.

## 006fe5b0: CFilter_Bind2::operator
Original manifest label: `_global / CThingFilter_MinAllowedRadius_FindNearest`.
Layout: retail CThing PhysicsTC is+0x60; the available donor CThing header is quarantined.

## 00877eb0: CFilter_Bind2::operator
Original manifest label: `_global / CThingFilter_IsOfType_WithinRadius`.
Layout: retail CThing PhysicsTC is+0x60; the available donor CThing header is quarantined.

## 00877f40: CFilter_Bind2::operator
Original manifest label: `_global / CThingFilter_IsOfType_WithinRadius_v2`.
Layout: retail CThing PhysicsTC is+0x60; the available donor CThing header is quarantined.

## 00702d10: CThingObject::DrawGetEditDrawInfo
Original manifest label: `CThingObject / CThingObject::vfunc_25`.
Identity: proven; RTTI own vtable0x01264DCC slot25 and both PDB names virtual const DrawGetEditDrawInfo(float,CEditEngineDrawInfo*); own data ScriptName+0x74,DefGlobalIndex+0x8C and literal separator.
Layout: included generated Physical/GameObject headers flatten the polymorphic base chain and omit named base fields; CThing header is quarantined. Local minimal named member view uses proved retail offsets, and actual RTTI inheritance with matching class keys. Draw-info/string classes have no header.

## 0083b3e0: CThingVillage::DrawGetEditDrawInfo
Original manifest label: `CThingVillage / CThingVillage::vfunc_25`.
Identity: proven; RTTI own vtable0x012451BC slot25 and both PDB names virtual const DrawGetEditDrawInfo(float,CEditEngineDrawInfo*); own data ScriptName+0x74,DefGlobalIndex+0x8C and literal separator.
Layout: included generated Physical/GameObject headers flatten the polymorphic base chain and omit named base fields; CThing header is quarantined. Local minimal named member view uses proved retail offsets, and actual RTTI inheritance with matching class keys. Draw-info has no shared header; string header alias remains data evidence.

## 0083d710: CThingPhysicalSwitch::DrawGetEditDrawInfo
Original manifest label: `CThingPhysicalSwitch / CThingPhysicalSwitch::vfunc_25`.
Identity: proven; RTTI own vtable0x01245644 slot25 and both PDB names virtual const DrawGetEditDrawInfo(float,CEditEngineDrawInfo*); own data ScriptName+0x74,DefGlobalIndex+0x8C and literal separator.
Layout: included generated Physical/GameObject headers flatten the polymorphic base chain and omit named base fields; CThing header is quarantined. Local minimal named member view uses proved retail offsets, and actual RTTI inheritance with matching class keys. Draw-info has no shared header; string header alias remains data evidence.

## 00708540: std::vector::_M_allocate_and_copy
Original manifest label: `_global / Vector_AllocAndCopyPairs`.
Identity: proven; own allocation0x708540 <- inner vector op=0x708CA0 <- vector-of-vector __copy0x709090 <- outer op=0x709660 <- CTCHeroAttachableAppearanceModifiers::Copy0x706EC0; PDB AppearanceModifiers vector<vector<CModifierEntry>>; own8-byte stride; const source from vector assignment, malloc0xBFEA0E.
Layout: engine/CTCHeroAttachableAppearanceModifiers.h lacks nested CModifierEntry. The outer declaration is naming scope only; the PDB8-byte record has DefIndex+0 and Rendered+4. No outer instance is accessed.

## 00a5dee0: std::vector::_M_allocate_and_copy
Original manifest label: `_global / AllocAndCopyRange_u64`.
Identity: proven; own vector op=0xA5E4D0 shares uninitialized-copy0xA5DDE0 with vector copy0xA5E0A0; that copy is local CVertexStreamFormat::DataElements at0xAB3B40 (J076 proof); PDB8-byte CDataElement Register/DataType; const source from vector assignment, malloc0xBFEA0E.

## 00ae5a30: std::vector::_M_allocate_and_copy
Original manifest label: `_global / Vector_Alloc_PairRange`.
Identity: proven; vector<C3DVertexBlend2>::operator=0xAE6690 in C3DGroup2 VertexBlends/CParticleProgram TU; shared copy-constructor0xAE5D80 identity from J076; PDB8-byte VertexIndex/BlendValue; const source from vector assignment, malloc0xBFEA0E.

## 007091a0: std::vector::_M_insert_overflow
Original manifest label: `std / std::vector_insert_range_with_alloc`.
Identity: proven; own GetMeshesUsed outputs and PDB12-byte CMeshInfo; head/tailcopy0x4C2AD0/fill0x7080D0.
Layout: generated outer headers have no nested record declarations; outer declarations provide naming scope only and no outer object is accessed. Retail vector has3pointers.

## 009d3580: std::vector::_M_insert_overflow
Original manifest label: `std / std::vector_reserve_12byte_fill`.
Identity: proven; own fillinsert0x9D3A20/resize0x9D4010, hand954 line80 retyping proof; PDB12-byte RuntimeData; copy0x9D23F0/fill0x9D1CA0.
Layout: generated outer headers have no nested record declarations; outer declarations provide naming scope only and no outer object is accessed. Retail vector has3pointers.

## 00adbd50: std::vector::_M_insert_overflow
Original manifest label: `std / std::vector_InsertFillN_Element_Triple`.
Identity: proven; own push0xADBFB0 called by CVertexGroupBlockOptimiser::AddBlocks0xADA0E9 on local vector; input BlockIndex,Cost,Benefit stack12-byte record agrees with PDB CBlockInfo; copy0xADA7E0/fill0xADA820.
Layout: generated outer headers have no nested record declarations; outer declarations provide naming scope only and no outer object is accessed. Retail vector has3pointers.

## 00709350: std::__destroy_aux
Original manifest label: `vector<NGameText::CDataBankGroupEntry / _Destroy`.
Identity: proven; CTCHeroAttachableAppearanceModifiers destructor0x706E00 uses own CModifierGraphic range; PDB record12={PTextureReplacement,BankIndex,RenderSizeX}; pointee release through virtual Delete slot1.

## 009cabc0: std::__destroy_aux
Original manifest label: `vector<NGameText::CDataBankGroupEntry / _Destroy`.
Identity: proven; CDataBank::CloseTextAndGroups0x9C8660 and TextEntryBuffer; PDB record8={PText,NarratorIndex}; stride8; pointee release through virtual Delete slot1.

## 00a96990: std::__destroy_aux
Original manifest label: `vector<NGameText::CDataBankGroupEntry / _Destroy`.
Identity: proven; N3DPrimitive::CDeviceIndependent::TriangleLists atretail+16; own CDeviceIndependent constructor0xA98100 and triangle-list array helpers; stride4; pointee release through virtual Delete slot1.

## 00710560: C3DAnimationTransitionState::SetStartState
Original manifest label: `C3DAnimationTransitionState / SetEndState`.
Identity: proven; PDB SetStartState(CIVCountedPointer<C3DAnimationStateBase>const&); own +8 is StartState not propagated EndState+12; CalcInterpolatedMeshAnim0x70D940 callers.
Layout: shared transition header reused, including its named flattened StartState_Object; counts accessed through real CIVCountedPointeeBase.

## 007f3a90: CTCLightningNode::SetPDef
Original manifest label: `C3DAnimationTransitionState / SetEndState`.
Identity: proven; CTCLightningNode::FrameUpdate0x7F2DC0 (RTTI slot9) calls own setter; PDB SetPDef and PDef+36.
Layout: no usable engine header (mesh-base header quarantined); partial class preserves own typed member at retail offset 0x24.

## 00ba8030: CEngineInternalPrimitiveMeshBase::SetMeshEffect
Original manifest label: `CEngineInternalPrimitiveMeshBase / CEngineInternalPrimitiveMeshBase::SetMeshEffect`.
Identity: proven; RTTI vtable0x12A41D4 slot25; PDB SetMeshEffect(CIVCountedPointer<CEngineMeshEffect>const&) virtual; PDB own MeshEffect+104.
Layout: no usable engine header (mesh-base header quarantined); partial class preserves own typed member at retail offset 0x68.

## 00718cc0: std::__copy_ptrs
Original manifest label: `_Copy_impl<std::pair<float,EConversationAttitude>*,std / _Copy_impl<std::pair<float,EConversationAttitude>*,std::pair<float,enum_EConversationAttitude>*>`.
Identity: proven; CSubtitledMessage::AttitudeTags, own vector assignment0x719470 calls twice; PDB pair<float,EConversationAttitude>8bytes; const input from vector assignment.

## 00a39460: std::copy
Original manifest label: `_Copy_impl<std::pair<float,EConversationAttitude>*,std / _Copy_impl<std::pair<float,EConversationAttitude>*,std::pair<float,enum_EConversationAttitude>*>`.
Identity: probable function spelling; proven pair<unsigned long,long> from CCRCSymbolMap::SLONGMap. Own range-insert0xA39610 passes exactly three arguments at0xA3969D/F9; ret4 excludes the four-argument __copy_ptrs helper. Const input from source range.

## 00a5df30: std::__copy_ptrs
Original manifest label: `_Copy_impl<std::pair<float,EConversationAttitude>*,std / _Copy_impl<std::pair<float,EConversationAttitude>*,std::pair<float,enum_EConversationAttitude>*>`.
Identity: proven; own vector op=0xA5E4D0 shares helpers with CVertexStreamFormat::DataElements copy0xA5E0A0, J076 local0xAB3B40; PDB Register/DataType record8bytes; const input from vector assignment.

## 0071ea40: std::vector::_M_allocate_and_copy
Original manifest label: `_global / Vector_AllocAndCopy_CountedPtr_4byte`.
Identity: proven; CTCHeroMorph::GetTattooThings0x71DE20 reserve0x721240 calls own allocationhelper; PDB Tattoos vector<CDefPointer<const CThingBaseDef>>; mutable input from reserve, malloc0xBFEA0E.

## 0071f380: std::vector::_M_allocate_and_copy
Original manifest label: `_global / Vector_AllocAndCopy_CountedPtr_4byte`.
Identity: proven; CTCHeroMorph::GetTattoos0x71DD50 reserve0x721800 calls own allocationhelper; GetSubDef<const CTattooDef>0x5B1EFC; mutable input from reserve, malloc0xBFEA0E.

## 009ca610: std::vector::_M_allocate_and_copy
Original manifest label: `std / std::vector_refcount_copy`.
Identity: proven; CDataBank::GroupEntryBuffer and reserve0x9CB2A0; PDB CDataBankGroupEntry is4bytes PGroup (not stale8-byte model); mutable input from reserve, malloc0xBFEA0E.

## 0071eae0: CUnprovenRecord_0071eae0::CUnprovenRecord_0071eae0
Original manifest label: `CProfile / CProfile`.
Identity: descriptive; unreferenced body: zero word+0 then CDateAndTime default constructor0x9E9AB0 at+4; no caller proves owning record or constructor-versus-template-helper role; retain address-specific descriptive record.

## 0071f9f0: CUnprovenRecord_0071f9f0::CUnprovenRecord_0071f9f0
Original manifest label: `CProfile / CProfile`.
Identity: descriptive; unreferenced body: zero word+0 then CDateAndTime default constructor0x9E9AB0 at+4; no caller proves owning record or constructor-versus-template-helper role; retain address-specific descriptive record.

## 009f8c60: CProfile::CProfile
Original manifest label: `CProfile / CProfile`.
Identity: probable; PDB CProfile72-byte layout uniquely has Manager+0 and CWideString Name+48, matching own store0/call CWideString ctor0x99AED0; exact default-ctor PDB name exists; body has no references.

## 0071fa60: std::__copy
Original manifest label: `_global / _Move<CDefPointer<CMeleeCombatAbilityDef_const_>*,CDefPointer<class_CMeleeCombatAbilityDef_const_>*>`.
Identity: proven; CTCHeroMorph::Tattoos own caller GetTattooThings0x71DE20 uses exact mutable CDefPointer<const CThingBaseDef>; existing real binding; four-byte element and pointee count+4.

## 00737cf0: std::__copy
Original manifest label: `_global / _Move<CDefPointer<CMeleeCombatAbilityDef_const_>*,CDefPointer<class_CMeleeCombatAbilityDef_const_>*>`.
Identity: proven; CTCCombat::UpdateUsableMeleeCombatAbilities0x736590 and AddMeleeFlourishAbility0x7366C0 use own CDefPointer<const CMeleeCombatAbilityDef> vector; four-byte element and pointee count+4.

## 00a6b080: std::__copy
Original manifest label: `_global / _Move<CDefPointer<CMeleeCombatAbilityDef_const_>*,CDefPointer<class_CMeleeCombatAbilityDef_const_>*>`.
Identity: proven; CPreallocTexturePool::ResizePool0xA6A670 and vector resize0xA6C020; existing mutable CIVCountedPointer<CManagedTextureData> binding; four-byte element and pointee count+4.

## 00721240: std::vector::reserve
Original manifest label: `std / std::vector_reserve_uint`.
Identity: proven; CTCHeroMorph::GetTattooThings0x71DE20, own Tattoos vector, allocation0x71EA40/destroy0x5B26E9; four-byte elements and STLport12-byte vector.

## 00721800: std::vector::reserve
Original manifest label: `_global / Vector_Reserve_Reallocate_4byte`.
Identity: proven; CTCHeroMorph::GetTattoos0x71DD50, own CTattooDef sub-def output vector, allocation0x71F380/destroy0x580A0B; four-byte elements and STLport12-byte vector.

## 009cb2a0: std::vector::reserve
Original manifest label: `std / std::vector_reserve_4byte_refcount`.
Identity: proven; CDataBank::GroupEntryBuffer4-byte elements, allocation0x9CA610/destroy0x9CAC00; PDB reserve<CDataBankGroupEntry>; four-byte elements and STLport12-byte vector.

## 00721b10: std::vector::_M_insert_overflow
Original manifest label: `_global / Vector_InsertAndReplaceMulti_CountedPtr`.
Identity: proven; CTCHeroMorph::GetTattoos0x71DD50 grow path and own push_back0x721D30; PDB CTattooDef output vector; tailcopy0x71F1F0/destroy0x580A0B; four-byte intrusive-pointer element.

## 007ca250: std::vector::_M_insert_overflow
Original manifest label: `_global / vector_AllocateAndCopy_CCountedPtr`.
Identity: proven; CArray<CDefPointer<const CCreatureGenerationFamilyDef>>::push_back0x7CA350 called by CTCCreatureGeneratorCreator::FrameUpdate0x7C99F0; actual GetDef0x74D220; tailcopy0x7C9F70/destroy0x7CA090; four-byte intrusive-pointer element.

## 00a97be0: std::vector::_M_insert_overflow
Original manifest label: `_global / Vector_Assign_InsertRangeCountedPointer`.
Identity: proven; CDeviceIndependent::TriangleLists (ctor0xA98100) and AddTriangleList0xA90900; tailcopy0xA957C0/destroy0xA96990; four-byte intrusive-pointer element.

## 007236c0: CTCPhysicsStandard::FillPhysicsDebugInfoForSelf
Original manifest label: `CTCPhysicsStandard / CTCPhysicsStandard::vfunc_89`.
Identity: proven; RTTI CTCPhysicsStandard vtable slot89; PDB virtual const FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&); Moving bit0+0x3C and own stats MovingStandardObjects offset match PDB.
Layout: generated physics headers flatten vptr and omit the named Moving bit (some use the wrong struct key); local real polymorphic base/member chain preserves retail flag+0x3C.

## 00725040: CTCPhysicsControlled::FillPhysicsDebugInfoForSelf
Original manifest label: `CTCPhysicsControlled / CTCPhysicsControlled::vfunc_89`.
Identity: proven; RTTI CTCPhysicsControlled vtable slot89; PDB virtual const FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&); Moving bit0+0x3C and own stats MovingControlledObjects offset match PDB.
Layout: generated physics headers flatten vptr and omit the named Moving bit (some use the wrong struct key); local real polymorphic base/member chain preserves retail flag+0x3C.

## 00726960: CTCPhysicsNavigator::FillPhysicsDebugInfoForSelf
Original manifest label: `CTCPhysicsNavigator / CTCPhysicsNavigator::vfunc_89`.
Identity: proven; RTTI CTCPhysicsNavigator vtable slot89; PDB virtual const FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&); Moving bit0+0x3C and own stats MovingNavigatorObjects offset match PDB.
Layout: generated physics headers flatten vptr and omit the named Moving bit (some use the wrong struct key); local real polymorphic base/member chain preserves retail flag+0x3C.

## 00723b30: CTCPhysicsStandard::GetFacingAngleXY
Original manifest label: `CTCPhysicsStandard / CTCPhysicsStandard::vfunc_63`.
Identity: proven; PDB const GetFacingAngleXY returns float; physics virtual entries and CCamera callers; RHSet.Forward component offsets retail 0x5C/60/64.
Layout: generated physics headers use a flattened vptr with the wrong struct key and opaque RHSet bytes; local typed real base/member chain uses CRightHandedSet::Up then Forward. RigidBody three retail vectors are 12 bytes each; donor16-byte vectors account for the RHSet displacement. Camera/vector/right-handed-set have no generated header.

## 00759a50: CTCPhysicsRigidBody::GetFacingAngleXY
Original manifest label: `CTCPhysicsRigidBody / CTCPhysicsRigidBody::vfunc_63`.
Identity: proven; PDB const GetFacingAngleXY returns float; physics virtual entries and CCamera callers; RHSet.Forward component offsets retail 0x74/78/7C.
Layout: generated physics headers use a flattened vptr with the wrong struct key and opaque RHSet bytes; local typed real base/member chain uses CRightHandedSet::Up then Forward. RigidBody three retail vectors are 12 bytes each; donor16-byte vectors account for the RHSet displacement. Camera/vector/right-handed-set have no generated header.

## 00a0c380: CCamera::GetAngleXY
Original manifest label: `_global / Math_ComputeAngleXY`.
Identity: proven; PDB const GetAngleXY returns float; physics virtual entries and CCamera callers; RHSet.Forward component offsets retail 0x18/1C/20.
Layout: generated physics headers use a flattened vptr with the wrong struct key and opaque RHSet bytes; local typed real base/member chain uses CRightHandedSet::Up then Forward. RigidBody three retail vectors are 12 bytes each; donor16-byte vectors account for the RHSet displacement. Camera/vector/right-handed-set have no generated header.

## 00723b90: CTCPhysicsStandard::GetFacingAngleYZ
Original manifest label: `CTCPhysicsStandard / CTCPhysicsStandard::vfunc_64`.
Identity: proven; PDB const GetFacingAngleYZ returns float; physics virtual entries and CCamera callers; RHSet.Forward component offsets retail 0x5C/60/64.
Layout: generated physics headers use a flattened vptr with the wrong struct key and opaque RHSet bytes; local typed real base/member chain uses CRightHandedSet::Up then Forward. RigidBody three retail vectors are 12 bytes each; donor16-byte vectors account for the RHSet displacement. Camera/vector/right-handed-set have no generated header.

## 00759ab0: CTCPhysicsRigidBody::GetFacingAngleYZ
Original manifest label: `CTCPhysicsRigidBody / CTCPhysicsRigidBody::vfunc_64`.
Identity: proven; PDB const GetFacingAngleYZ returns float; physics virtual entries and CCamera callers; RHSet.Forward component offsets retail 0x74/78/7C.
Layout: generated physics headers use a flattened vptr with the wrong struct key and opaque RHSet bytes; local typed real base/member chain uses CRightHandedSet::Up then Forward. RigidBody three retail vectors are 12 bytes each; donor16-byte vectors account for the RHSet displacement. Camera/vector/right-handed-set have no generated header.

## 00a0c3e0: CCamera::GetAngleYZ
Original manifest label: `_global / Vector_ComputeAngle`.
Identity: proven; PDB const GetAngleYZ returns float; physics virtual entries and CCamera callers; RHSet.Forward component offsets retail 0x18/1C/20.
Layout: generated physics headers use a flattened vptr with the wrong struct key and opaque RHSet bytes; local typed real base/member chain uses CRightHandedSet::Up then Forward. RigidBody three retail vectors are 12 bytes each; donor16-byte vectors account for the RHSet displacement. Camera/vector/right-handed-set have no generated header.

## 00725f10: CThingSearchTools::GetAllThingsInRoughArea
Original manifest label: `CThingSearchTools / GetPBestThingInRoughArea<CFilter_Bind2<CThingFilter_IsNot,CIsVeryCloseMotionlessCreature>,CThingCompare_Nearest,CThingCreatureBase>`.
Identity: proven; mask0xF, own Navigator UpdateAdditionalImpulse0x726BE0/creature awareness0x7C04A0; PDB mutable vector<CreatureBase*> IsNot instance and overflow0x6F2680; own filter operator()0x006a76a0.
Layout: supplied SearchTools header exposes AreaMaps as opaque bytes; local mutable exact STLport vector gives its const search API. CThing header is quarantined; ThingTypeFlags+0x6C is a minimal data view. CTCMapwho header flattens CTCBase and node fields; local ParentThing+4/SearchID+0x4C and NThingMap node views retain actual reads. The map uses retail12-byte vectors (donor16), level-info20-byte grid metadata. Byte offsets use sizeof so the same algorithm executes on the64-bit host. Same-TU overflow is genuine POD STLport copy/fill/deallocate.

## 008ad950: CThingSearchTools::GetAllThingsInRoughArea
Original manifest label: `CThingSearchTools / GetPBestThingInRoughArea<CFilter_Bind2<CThingFilter_IsNot,CIsVeryCloseMotionlessCreature>,CThingCompare_Nearest,CThingCreatureBase>`.
Identity: proven; Own filter/caller/template mapping preserved. Original full CThing:CClass:CBaseObject:CBaseClass hierarchy through typed UID/bitset274/component CVectorMap/list/string/driver/physics/flags, actual CreatureBase:GameObject:Physical chain. CTCMapwho true CTCBase/NonCopy/BaseClass plus four complete16-byte original NThingMap nodes, SearchID76 and one-bit InMapwho. Original NThingMap::CThingMap true NonCopy/BaseClass with real MapwhoLevelInfo and nested Cells vectors, Manager/Area/NoThings,52bytes. Original level info Size/Extents/ScaleShift/NoThings20 and cell PFirstNode4; no flat vector/node/opaque manager fields or fabricated GetCellPtr helper. Real protected _Vector_base and three-argument _STLP_alloc_proxy, original active vector algorithms retained. Shared headers aliased unchanged. Primitive extents XExtent/YExtent projection follows validated rootN166 model; unused interfaces/suffixes omitted explicitly.
Layout: original known owner/container/base identities and typed storage, actual counter/global contracts preserved.

## 008e3eb0: CThingSearchTools::GetAllThingsInRoughArea
Original manifest label: `CThingSearchTools / GetPBestThingInRoughArea<CFilter_Bind2<CThingFilter_IsNot,CIsVeryCloseMotionlessCreature>,CThingCompare_Nearest,CThingCreatureBase>`.
Identity: proven; Own filter/caller/template mapping preserved. Original full CThing:CClass:CBaseObject:CBaseClass hierarchy through typed UID/bitset274/component CVectorMap/list/string/driver/physics/flags, actual CreatureBase:GameObject:Physical chain. CTCMapwho true CTCBase/NonCopy/BaseClass plus four complete16-byte original NThingMap nodes, SearchID76 and one-bit InMapwho. Original NThingMap::CThingMap true NonCopy/BaseClass with real MapwhoLevelInfo and nested Cells vectors, Manager/Area/NoThings,52bytes. Original level info Size/Extents/ScaleShift/NoThings20 and cell PFirstNode4; no flat vector/node/opaque manager fields or fabricated GetCellPtr helper. Real protected _Vector_base and three-argument _STLP_alloc_proxy, original active vector algorithms retained. Shared headers aliased unchanged. Primitive extents XExtent/YExtent projection follows validated rootN166 model; unused interfaces/suffixes omitted explicitly.
Layout: original known owner/container/base identities and typed storage, actual counter/global contracts preserved.

## 00727580: CTCSimpleAppearanceMorph::CTCSimpleAppearanceMorph
Original manifest label: `CComponentContainer / CComponentContainer`.
Identity: proven; own vtable RTTI CTCSimpleAppearanceMorph; PDB PMeshMorphSet+12 is8-byte counted pointer; own CThing& ctor factory0x4D2A14; base CTCBase(CThing&)0x686800 and own vtable0x012664a4.
Layout: no target-class engine header; generated CTCBase header uses a data-member vptr, so the real polymorphic base chain is required to retain the own vtable and member+12.

## 00732780: CTCClock::CTCClock
Original manifest label: `CComponentContainer / CComponentContainer`.
Identity: proven; own vtable RTTI CTCClock; PDB PDef+12 and LastTimeUpdated+16; own factory0x4D3422; base CTCBase(CThing&)0x686800 and own vtable0x01267044.
Layout: no target-class engine header; generated CTCBase header uses a data-member vptr, so the real polymorphic base chain is required to retain the own vtable and member+12.

## 007f5370: CTCSoundAtmosphere::CTCSoundAtmosphere
Original manifest label: `CComponentContainer / CComponentContainer`.
Identity: proven; own vtable RTTI CTCSoundAtmosphere; PDB TimeUntilNextUpdate+12 and PDef+16; own factory0x4DCE57; base CTCBase(CThing&)0x686800 and own vtable0x01271dfc.
Layout: no target-class engine header; generated CTCBase header uses a data-member vptr, so the real polymorphic base chain is required to retain the own vtable and member+12.

## 00727bf0: CTCRandomAppearanceMorph::OnAppearanceClear
Original manifest label: `CTCRandomAppearanceMorph / CTCRandomAppearanceMorph::OnAppearanceClear`.
Identity: proven; RTTI secondary CAGraphicAppearanceComponent+12 vtable0x1266520 slot1, PDB method and PrimHandles+28 (receiver-relative+16).
Layout: no target generated header. Real RTTI base chain retains the secondary vptr at+12 for appearance callbacks; donor16-byte vector becomes retail12-byte vector. The compiler supplies the secondary receiver convention.

## 007e1590: NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceClear
Original manifest label: `NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes / NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::vfunc_1`.
Identity: proven; RTTI secondary CAGraphicAppearanceComponent+12 vtable0x126FFB4 slot1, PDB method and PrimHandles+16 (receiver-relative+4).
Layout: no target generated header. Real RTTI base chain retains the secondary vptr at+12 for appearance callbacks; donor16-byte vector becomes retail12-byte vector. The compiler supplies the secondary receiver convention.

## 00b2f8b0: CEnginePrimitiveOcclusionInfo::ClearHandles
Original manifest label: `CTCGatherPointBuilding / Init`.
Identity: proven; own CheckVisibilityResult0xB2F8E0 calls at0xB2F9D4 on its info argument; PDB private ClearHandles and OcclusionTestHandles+12.
Layout: no target generated header. Real RTTI base chain retains the secondary vptr at+12 for appearance callbacks; donor16-byte vector becomes retail12-byte vector. The compiler supplies the secondary receiver convention.

## 007327f0: CTCClock::OnCreate
Original manifest label: `CTCClock / CTCClock::vfunc_4`.
Identity: proven; RTTI own vtable slot4; PDB virtual OnCreate(); PDef+12 and GetSubDef<CClockDef>0x00732bd0; CTCBase protected const GetDefinitionManager0x6869D0 then CDefinitionManager::GetDef<CThingBaseDef>0x42AF3C.
Layout: no target class header. Generated CTCBase/CThing headers flatten polymorphism (CThing is quarantined); local real base chain and minimal named DefGlobalIndex view preserve ParentThing+4 and retail index+0x8C. OverheadDisplay lists are retail4-byte nodes.

## 00746860: CTCBed::OnCreate
Original manifest label: `CTCBed / CTCBed::vfunc_4`.
Identity: proven; RTTI own vtable slot4; PDB virtual OnCreate(); PDef+20 and GetSubDef<CBedDef>0x00746c20; CTCBase protected const GetDefinitionManager0x6869D0 then CDefinitionManager::GetDef<CThingBaseDef>0x42AF3C.
Layout: no target class header. Generated CTCBase/CThing headers flatten polymorphism (CThing is quarantined); local real base chain and minimal named DefGlobalIndex view preserve ParentThing+4 and retail index+0x8C. OverheadDisplay lists are retail4-byte nodes.

## 007b7b60: CTCOverheadDisplay::OnCreate
Original manifest label: `CTCOverheadDisplay / CTCOverheadDisplay::vfunc_4`.
Identity: proven; RTTI own vtable slot4; PDB virtual OnCreate(); PDef+24 and GetSubDef<COverheadDisplayDef>0x007b7f50; CTCBase protected const GetDefinitionManager0x6869D0 then CDefinitionManager::GetDef<CThingBaseDef>0x42AF3C.
Layout: no target class header. Generated CTCBase/CThing headers flatten polymorphism (CThing is quarantined); local real base chain and minimal named DefGlobalIndex view preserve ParentThing+4 and retail index+0x8C. OverheadDisplay lists are retail4-byte nodes.
Layout: engine/CIVCountedPointeeBase.h flattens the vptr. Retail release reads the count at Object+4 and calls Delete at vtable+4; the local real polymorphic base preserves those offsets and const API.

## 00734190: CTCCombat::RemoveThingToAlwaysBlock
Original manifest label: `_global / CombatBlockingData_RemoveAndUpdate`.
Identity: proven; PDB name and AlwaysBlockThings donor+92 ->retail+84; own CGSI::EntitySetAlwaysBlockAttacksFromThing0x89DFF0 caller; find/remove_copy/erase helpers corroborate exact element8.
Layout: reused generated header data under alias; its opaque named vector is viewed as the exact retail STLport vector. CTCSolidToThing has no header and uses the actual CTCBase member chain.

## 0074a1e0: CTCCreatureGenerator::RemoveCreatureTriggerer
Original manifest label: `_global / CCreatureGenerator_RemoveTriggerer`.
Identity: proven; PDB name/reference prototype and CreatureTriggerers donor+40 ->retail+28; own CGSI::CreatureGeneratorRemoveTriggerer0x89ECC0 caller; find/remove_copy/erase helpers corroborate exact element8.
Layout: reused generated header data under alias; its opaque named vector is viewed as the exact retail STLport vector. CTCSolidToThing has no header and uses the actual CTCBase member chain.

## 00770f20: CTCSolidToThing::RemoveThingNotSolidTo
Original manifest label: `_global / CThing_RemoveFromList`.
Identity: proven; PDB name/pointer prototype and NotSolidToThings+12; own kick-finish/dragon-takeoff/JackRushToPos callers; find/remove_copy/erase helpers corroborate exact element8.
Layout: reused generated header data under alias; its opaque named vector is viewed as the exact retail STLport vector. CTCSolidToThing has no header and uses the actual CTCBase member chain.

## 00737d50: std::__copy
Original manifest label: `_global / CopyIntelligentPointers`.
Identity: proven; vector<CIntelligentPointer<const CThing>>::erase0x738D70 and combat block-list callers; hand951G026own-copy address0x737D50; stride8; real GetPItem0xA01B50 and SetPItem0xA01B90.

## 0074cfe0: std::__copy
Original manifest label: `_global / CBaseIntelligentPointer_Copy`.
Identity: proven; vector<CIntelligentPointer<CThingCreatureBase>>::erase0x74D3E0 called by creature-generator paths; hand951G021identifies this exacthelper; stride8; real GetPItem0xA01B50 and SetPItem0xA01B90.

## 007b8460: std::__copy
Original manifest label: `_global / CopyArray_Forward_CBaseIntelligentPointer_Clear`.
Identity: proven; vector<CIntelligentPointer<CThingObject>>::erase0x7B9870; hand951G026exact __copy address mapping; stride8; real GetPItem0xA01B50 and SetPItem0xA01B90.

## 00739030: CTCCombat::ResetBlockPercentageChance
Original manifest label: `CTCCombat / ResetBlockPercentageChance<CCombatAbilityBlockHeavyWeaponAttackDef>`.
Identity: own GetSubDef007380d0 contains exact class-name literal; PDB confirms member template. Retail lower_bound00738d10 and virtual data type slot3.
Layout: shared flattened definition header retained as evidence; real vptr/refcount hierarchy is required for virtual Delete at slot1 and count+4. CombatAbility starts+40 in block defs.
Layout: shared data headers are flattened; genuine four-slot polymorphic class preserves PercentageChance+4 and WeaponClass+44 in blocking data. Other fields follow PDB names.

## 00739140: CTCCombat::ResetBlockPercentageChance
Original manifest label: `CTCCombat / ResetBlockPercentageChance<CCombatAbilityBlockHeavyWeaponAttackDef>`.
Identity: own GetSubDef007381f0 contains exact class-name literal; PDB confirms member template. Retail lower_bound00738d10 and virtual data type slot3.
Layout: full original RTTI chain CTCCombat/CTCBase/NonCopy/BaseClass; blockdef/BlockDefBase/SubDef/Def/Pointee/Resource/CIV. Typed owner prefix through CombatAbilities112; trailing unused owner/CombatTypeDef state omitted without whole-size claim. Genuine STLport tree/base/proxy, bool color, rebound allocator, two-argument iterator traits and original set. CDefPointer stores CDefPointeeBase*, original Release/AddRef contract. Original shared flattened headers remain evidence only.

## 00739250: CTCCombat::ResetBlockPercentageChance
Original manifest label: `CTCCombat / ResetBlockPercentageChance<CCombatAbilityBlockHeavyWeaponAttackDef>`.
Identity: own GetSubDef007382b0 contains exact class-name literal; PDB confirms member template. Retail lower_bound00738d10 and virtual data type slot3.
Layout: full original RTTI chain CTCCombat/CTCBase/NonCopy/BaseClass; blockdef/BlockDefBase/SubDef/Def/Pointee/Resource/CIV. Typed owner prefix through CombatAbilities112; trailing unused owner/CombatTypeDef state omitted without whole-size claim. Genuine STLport tree/base/proxy, bool color, rebound allocator, two-argument iterator traits and original set. CDefPointer stores CDefPointeeBase*, original Release/AddRef contract. Original shared flattened headers remain evidence only.

## 0073a6f0: CTCCreatureModeManager::CModeBase::GetNoFramesBeforePlayingCombinationIdleAnims
Original manifest label: `CModeBase / GetNoFramesBeforePlayingCombinationIdleAnims`.
Identity: original PDB protected method and own stand/sit callers; default013bafe4, Berserk0078c000, IsAffected004c9920, multiplier004c5fa0.
Layout: no active CThingManager header; own global getter chains prove World+0x24. No other manager field is accessed.
Layout: CTCBerserk supplies only a static method name; no object instance is accessed.
Layout: original nested CModeBase ParentThing+4, as PDB and retail agree; no active nested base header exists. Other members are not accessed.

## 0073a760: CTCCreatureModeManager::CModeBase::GetNoFramesBeforePlayingIdleAnims
Original manifest label: `CModeBase / GetNoFramesBeforePlayingCombinationIdleAnims`.
Identity: original PDB protected method and own stand/sit callers; default013bafe8, Berserk0078c000, IsAffected004c9920, multiplier004c5fa0.
Layout: no active CThingManager header; own global getter chains prove World+0x24. No other manager field is accessed.
Layout: CTCBerserk supplies only a static method name; no object instance is accessed.
Layout: original nested CModeBase ParentThing+4, as PDB and retail agree; no active nested base header exists. Other members are not accessed.

## 0073a7d0: CTCCreatureModeManager::CModeBase::GetNoFramesBeforePlayingWaveAnims
Original manifest label: `CModeBase / GetNoFramesBeforePlayingCombinationIdleAnims`.
Identity: original PDB protected method and own stand/sit callers; default013bafec, Berserk0078c000, IsAffected004c9920, multiplier004c5fa0.
Layout: no active CThingManager header; own global getter chains prove World+0x24. No other manager field is accessed.
Layout: CTCBerserk supplies only a static method name; no object instance is accessed.
Layout: original nested CModeBase ParentThing+4, as PDB and retail agree; no active nested base header exists. Other members are not accessed.

## 0073cfc0: std::__unguarded_linear_insert
Original manifest label: `std / std::priority_queue_insert_ordered`.
Identity: own linear-insert caller00742360 and its sorting chain establish the element and comparator.
Layout: outer class supplies only the nested type scope. PDB CRegisteredModeInfo has one AllocFunc pointer; the retail pair stride is8. No outer object is accessed.

## 009b1aa0: std::__unguarded_linear_insert
Original manifest label: `_global / SortedArray_InsertElement`.
Identity: own linear-insert caller009b5920 and its sorting chain establish the element and comparator.

## 00b84f40: std::__unguarded_linear_insert
Original manifest label: `_global / BinaryHeap_Insert`.
Identity: own linear-insert caller00b85870 and its sorting chain establish the element and comparator.
Layout: outer class provides a name scope only. CSortElement is unsigned SortValue+0 and SubPrimitive pointer+4, confirmed by PDB and own sort caller. No outer object is accessed.

## 0073d210: CVectorMap::LowerBound
Original manifest label: `CVectorMap<unsigned_long / LowerBound`.
Identity: Own Count0073da50 and AllocateMode0073a8e0; PDB CRegisteredModeInfo::AllocFunc, const registry lookup.
Layout: retail CVectorMap has12-byte Data at+0, empty Compare+12 and Dirty+13; key/value element is8bytes. Own callers and loop stride confirm these offsets.
Layout: CTCCreatureModeManager supplies only a nested name scope. CRegisteredModeInfo is the original4-byte AllocFunc pointer record; no outer object is accessed.

## 009b21f0: CVectorMap::LowerBound
Original manifest label: `CVectorMap<unsigned_long / LowerBound`.
Identity: CCRCSymbolMap::SLONGMap and const FindSymbol00a38420; PDB unsigned-long key/long mapped type. Older CNameCRCToIndexMap wrapper is descriptive.
Layout: retail CVectorMap has12-byte Data at+0, empty Compare+12 and Dirty+13; key/value element is8bytes. Own callers and loop stride confirm these offsets.

## 009d20d0: CVectorMap::LowerBound
Original manifest label: `CVectorMap<unsigned_long / LowerBound`.
Identity: Own caller009cc410 is CBankFile::FindIndexBySymbolCRC(unsigned long) const, not CDataBank::LookupValue. Actual CBankFile constructor009cd480 and PostInitEntries009cfb70 place SymbolCRCIndices at+0xd0; PDB maps unsigned-long to unsigned-long. Both PDB sets confirm const FindIndexBySymbolCRC.
Layout: retail CVectorMap has12-byte Data at+0, empty Compare+12 and Dirty+13; key/value element is8bytes. Own callers and loop stride confirm these offsets.

## 0073fea0: NCreatureMode::CModeSitChair::CModeSitChair
Original manifest label: `CModeSitChair / CModeSitChair`.
Identity: retail RTTI01267b74 proves owner; strings and CModeSitBase008d7ca0/SetGroups0073e040 prove construction.
Layout: no active mode/filter/string shared headers exist. CModeSitBase is44bytes; Filter+44 is CAnimFilterOf2Groups20bytes. PDB fields and retail ctor stores agree.

## 009380e0: NCreatureMode::CModeSitChairBored::CModeSitChairBored
Original manifest label: `CModeSitChair / CModeSitChair`.
Identity: retail RTTI01292354 proves owner; strings and CModeSitBase008d7ca0/SetGroups0073e040 prove construction.
Layout: no active mode/filter/string shared headers exist. CModeSitBase is44bytes; Filter+44 is CAnimFilterOf2Groups20bytes. PDB fields and retail ctor stores agree.

## 009381b0: NCreatureMode::CModeSitChairSleeping::CModeSitChairSleeping
Original manifest label: `CModeSitChair / CModeSitChair`.
Identity: retail RTTI01292398 proves owner; strings and CModeSitBase008d7ca0/SetGroups0073e040 prove construction.
Layout: no active mode/filter/string shared headers exist. CModeSitBase is44bytes; Filter+44 is CAnimFilterOf2Groups20bytes. PDB fields and retail ctor stores agree.

## 00742ce0: NCreatureMode::CModeGuildmasterPowerUpHeal::CModeGuildmasterPowerUpHeal
Original manifest label: `CModeGuildmasterPowerUpHeal / CModeGuildmasterPowerUpHeal`.
Identity: proven original owner RTTI and constructor calls; base constructor00861e00, CRC004014a0.
Layout: corrected original ModeBase28 ->StandBase104 hierarchy and all named members; original AnimFilterBase operator/Clone/dtor slots0/1/2 and real STLport vector/base/proxy. Supersedes earlier flattened BaseState model.

## 00742da0: NCreatureMode::CModeGuildmasterPowerUpShield::CModeGuildmasterPowerUpShield
Original manifest label: `CModeGuildmasterPowerUpHeal / CModeGuildmasterPowerUpHeal`.
Identity: proven original owner RTTI and constructor calls; base constructor00861e00, CRC004014a0.
Layout: corrected original ModeBase28 ->StandBase104 hierarchy and all named members; original AnimFilterBase operator/Clone/dtor slots0/1/2 and real STLport vector/base/proxy. Supersedes earlier flattened BaseState model.

## 00742e60: NCreatureMode::CModeGuildmasterPowerUpSpeed::CModeGuildmasterPowerUpSpeed
Original manifest label: `CModeGuildmasterPowerUpHeal / CModeGuildmasterPowerUpHeal`.
Identity: proven original owner RTTI and constructor calls; base constructor00861e00, CRC004014a0.
Layout: corrected original ModeBase28 ->StandBase104 hierarchy and all named members; original AnimFilterBase operator/Clone/dtor slots0/1/2 and real STLport vector/base/proxy. Supersedes earlier flattened BaseState model.

## 00744470: CTCPerceiveThing::SetSoundRadius
Original manifest label: `CTCPerceiveThing / SetSightRadius`.
Layout: shared CTCPerceiveThing header preserves PDef_Object+0x34 and SoundRadius+0x3c; CPerceivedThingDef::SoundRadius+0x38 is independently confirmed by retail loads.

## 007444a0: CTCPerceiveThing::SetSmellRadius
Original manifest label: `CTCPerceiveThing / SetSightRadius`.
Layout: shared CTCPerceiveThing header preserves PDef_Object+0x34 and SmellRadius+0x48; CPerceivedThingDef::SmellRadius+0x3c is independently confirmed by retail loads.

## 007444d0: CTCPerceiveThing::SetGiveUpChaseRadius
Original manifest label: `CTCPerceiveThing / SetSightRadius`.
Layout: shared CTCPerceiveThing header preserves PDef_Object+0x34 and GiveUpChaseRadius+0x4c; CPerceivedThingDef::GiveUpChaseRadius+0x2c is independently confirmed by retail loads.

## 00744cb0: CTCPerceiveThing::OnHit
Original manifest label: `CTCPerceiveThing / OnHit`.
Layout: this uses the supplied shared data layout. The opaque PerceivedThing+12 is its real8-byte intelligent pointer. CTCBase is used only as the external GetWorld receiver at offset0, as the original base chain specifies.

## 00744e30: CTCPerceiveThing::OnHitByMelee
Original manifest label: `CTCPerceiveThing / OnHit`.
Layout: this uses the supplied shared data layout. The opaque PerceivedThing+12 is its real8-byte intelligent pointer. CTCBase is used only as the external GetWorld receiver at offset0, as the original base chain specifies.

## 00744e80: CTCPerceiveThing::OnHitByShot
Original manifest label: `CTCPerceiveThing / OnHit`.
Layout: this uses the supplied shared data layout. The opaque PerceivedThing+12 is its real8-byte intelligent pointer. CTCBase is used only as the external GetWorld receiver at offset0, as the original base chain specifies.

## 007462f0: CTCPerceiveThing::~CTCPerceiveThing
Original manifest label: `CCreatureAction_FireMissileWeapon / ~CCreatureAction_FireMissileWeapon`.
Layout: genuine member chain from PDB; retail PDef+0x34 and PerceivedThing+0xc. Shared headers included as evidence where present but flatten real base/vptr and lifetime-bearing members; Oracle list is4bytes in retail, moving later members by8.
Layout: pointee count+4 and virtual Delete slot1 are confirmed by each assigned destructor. Generated pointee headers flatten the vptr, so actual polymorphism is represented here.

## 00785d80: CTCVolumeContainmentTracker::~CTCVolumeContainmentTracker
Original manifest label: `CCreatureAction_FireMissileWeapon / ~CCreatureAction_FireMissileWeapon`.
Layout: genuine member chain from PDB; retail PDef+0x1c and PThingInside+0xc. Shared headers included as evidence where present but flatten real base/vptr and lifetime-bearing members; Oracle list is4bytes in retail, moving later members by8.
Layout: pointee count+4 and virtual Delete slot1 are confirmed by each assigned destructor. Generated pointee headers flatten the vptr, so actual polymorphism is represented here.

## 007e7030: CTCOracleMinigame::~CTCOracleMinigame
Original manifest label: `CCreatureAction_FireMissileWeapon / ~CCreatureAction_FireMissileWeapon`.
Layout: genuine member chain from PDB; retail PDef+0xe8 and InputsPending+0xe0. Shared headers included as evidence where present but flatten real base/vptr and lifetime-bearing members; Oracle list is4bytes in retail, moving later members by8.
Layout: pointee count+4 and virtual Delete slot1 are confirmed by each assigned destructor. Generated pointee headers flatten the vptr, so actual polymorphism is represented here.

## 007463e0: std::vector::reserve
Original manifest label: `_global / Vector_Reserve`.
Identity: CTCStealth::GetNumberOfAttackersOverMaximum00747890 and CTCPerceiveThing::UpdateLineOfSight00745120 use vector<const CThing*>. Retail malloc/memmove/free thunks preserved.
Layout: CDefinitionManager is a nested-name scope only; no owner or pointee instance is accessed by reserve.

## 009b4030: std::vector::reserve
Original manifest label: `_global / Vector_Reserve`.
Identity: C3DMeshFileSmoothingGroupsChunk::ReadFromFile00abc980 and CVertexGroupBlockOptimiser00ad9d70 use unsigned-long arrays. Retail malloc/memmove/free thunks preserved.
Layout: CDefinitionManager is a nested-name scope only; no owner or pointee instance is accessed by reserve.

## 009b4f30: std::vector::reserve
Original manifest label: `_global / Vector_Reserve_Reallocate_4byte`.
Identity: Own CArray::DeallocateUnusedMemory009b6eb0 and PDB CDefinitionManager::InstantiatedDefs establish original nested pointee type; older global CInstantiatedDefInfo name omitted its scope. Retail malloc/memmove/free thunks preserved.
Layout: CDefinitionManager is a nested-name scope only; no owner or pointee instance is accessed by reserve.

## 00748810: std::__unguarded_linear_insert
Original manifest label: `_global / Vector_InsertSorted_Item`.
Identity: own comparator007483e0 and its full PDB symbol establish const CThing* with CCompare_FurthestFromInitPos2D.
Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.

## 007a3040: std::__unguarded_linear_insert
Original manifest label: `_global / HeapSiftUp_CThingAICreature`.
Identity: own comparator007a2f90 and its full PDB symbol establish CThing* with CThingCompare_NearestToPos.
Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.

## 0091f810: std::__unguarded_linear_insert
Original manifest label: `_global / Sort_SiftDown_Comparator`.
Identity: own comparator0091f740 and its full PDB symbol establish const CThingObject* with CThingCompare_Nearest.
Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.

## 0074da40: std::vector::push_back
Original manifest label: `_global / CVector_PushBack_OrGrow`.
Identity: full RTTI at0123e7d8 proves CIntelligentPointer<CThingCreatureBase>; original base ctor/GetPItem/SetPItem and own overflow0074d600.
Layout: CBaseIntelligentPointer is the original polymorphic8-byte wrapper, PData+4; vector is12bytes. No active wrapper header exists.

## 007b0000: std::vector::push_back
Original manifest label: `_global / Vector_PushBack_CBaseIntelligentPointer`.
Identity: full RTTI at0123e7e0 proves CIntelligentPointer<CThingAICreature>; original base ctor/GetPItem/SetPItem and own overflow007afd10.
Layout: CBaseIntelligentPointer is the original polymorphic8-byte wrapper, PData+4; vector is12bytes. No active wrapper header exists.

## 007b7050: std::vector::push_back
Original manifest label: `_global / CBaseIntelligentPointer_InsertAt`.
Identity: full RTTI at0125a860 proves CIntelligentPointer<CThingMarker>; original base ctor/GetPItem/SetPItem and own overflow007b6ef0.
Layout: CBaseIntelligentPointer is the original polymorphic8-byte wrapper, PData+4; vector is12bytes. No active wrapper header exists.

## 00758390: std::__unguarded_linear_insert
Original manifest label: `_global / _Copy_impl<CDegradableInfo*,CDegradableInfo*>`.
Identity: CWoundMorphs::Morphs; own linear-insert00758e00 and matching44-byte pair helpers.
Layout: CWoundMorphs supplies only the nested name scope; original CEntry is40bytes, with the named fields below from PDB and retail pair stride44.

## 00817070: std::__unguarded_linear_insert
Original manifest label: `_global / _Copy_impl<CDegradableInfo*,CDegradableInfo*>`.
Identity: Own linear-insert00818240/PDB anonymous record fields key,hit_location,intersection_pos,outward_normal;32-byte element.
Layout: original anonymous hit record is32bytes: key+0,hit_location+4,intersection_pos+8,outward_normal+20; own caller00818240 and PDB types confirm.

## 008199f0: std::__unguarded_linear_insert
Original manifest label: `_global / _Copy_impl<CDegradableInfo*,CDegradableInfo*>`.
Identity: Own linear-insert0081be30 and CParticleMorphs::Morphs;40-byte element.
Layout: CParticleMorphs supplies only the nested name scope; CEntry36bytes and pair40bytes. PDB field names and own vector helpers confirm offsets.

## 007586e0: std::__uninitialized_copy
Original manifest label: `_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics / _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>`.
Identity: CWoundMorphs::Morphs and own vector overflow00759190/fill-insert00759290;44-byte pair (40-byte value). Mutable inputs are from original storage in the caller.
Layout: CWoundMorphs supplies only the nested name scope; original CEntry is40bytes, with the named fields below from PDB and retail pair stride44.

## 0081a270: std::__uninitialized_copy
Original manifest label: `_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics / _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>`.
Identity: CParticleMorphs::Morphs and own overflow0081c990/fill-insert0081cd00;40-byte pair (36-byte value). Mutable inputs are from original storage in the caller.
Layout: CParticleMorphs supplies only the nested name scope; CEntry36bytes and pair40bytes. PDB field names and own vector helpers confirm offsets.

## 00a32060: std::__uninitialized_copy
Original manifest label: `_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics / _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>`.
Identity: Own overflow00a33e80 used by dummy-object vector insert00a349b0 and C3DMesh path00a2fc17;56-byte pair (52-byte Transform/BoneIndex value). Mutable inputs are from original storage in the caller.
Layout: original C3DDummyObject is52bytes: Transform12floats+0, BoneIndex+48. Own vector overflow00a33e80 and PDB confirm56-byte unsigned-key pair.

## 0075b470: std::uninitialized_fill_n
Original manifest label: `_global / _Fill_n<unsigned_long*,unsigned_int,unsigned_long>`.
Identity: proven; Own fill-insert0075bb00 calls the same C3DVector uninitialized-copy0066ddf0 as established C3DVector vector operations;12-byte record.

## 0086adc0: std::uninitialized_fill_n
Original manifest label: `_global / _Fill_n<unsigned_long*,unsigned_int,unsigned_long>`.
Identity: proven; Own fill-insert0086ceb0 shares copy-backward0086a5c0 with known texture-morph linear-insert0086c5c0; PDB NewTexture/GroupID8-byte value and unsigned key.
Layout: outer class is nested-name scope only; PDB CTextureMorph fields are NewTexture+0 and GroupID+4. No outer instance is accessed.

## 0086f340: std::uninitialized_fill_n
Original manifest label: `_global / _Fill_n<unsigned_long*,unsigned_int,unsigned_long>`.
Identity: descriptive; Own fill-insert0086fd20 shares copy-backward0086eff0 with signed-key linear-insert0086fa00. Long key and8-byte POD value established; mapped class identity not proven. Do not infer CSkeletalMorph from adjacency (donor value is12 bytes).
Layout: retail mapped value is8bytes copied in two words; original type and field names unresolved.

## 0075c040: std::vector::resize
Original manifest label: `vector<CBrainBehaviour / resize`.
Identity: Own C3DVector fill/resize callers and12-byte element. Own copy0074ca20 and fill-insert0075bb00.

## 0081d9f0: std::vector::resize
Original manifest label: `vector<CBrainBehaviour / resize`.
Identity: CSkeletalMorphs::Morphs and own Expand0081df20 caller; PDB8-byte value MorphName/Priority/SupressesLowerMorphs. Own copy00819440 and fill-insert0081ce50.
Layout: outer CSkeletalMorphs is only a nested-name scope. Its CEntry is8bytes: MorphName+0,Priority+4,SupressesLowerMorphs+6; unsigned-key pair12bytes.

## 008f6e30: std::vector::resize
Original manifest label: `vector<CBrainBehaviour / resize`.
Identity: Own height-map resize caller008f63c0 and PDB12-byte cell layout. Own copy008f65c0 and fill-insert008f6c90.

## 007664e0: CTCObjectAugmentations::StopTrailEffect
Original manifest label: `CTCKraken / EndHeadEffects`.
Identity: Own shot callers lookup component0xbf; PDB StopTrailEffect and ProjectileTrailEffect donor+125 adjusted by six shrunk containers to retail+101.
Layout: CThing active header is absent (donor-only header quarantined). Retail Dead is bit0 at+0x91; no other fields accessed.
Layout: genuine polymorphic intelligent pointer has PData+4, total8bytes on x86; no active shared wrapper header.
Layout: no active owner header; ProjectileTrailEffect is an8-byte intelligent pointer at 101, proved by retail address and PDB member. Packed layout preserves the original unaligned member where applicable.

## 007f07a0: CTCKrakenTentacle::KillRippleFX
Original manifest label: `CTCKraken / EndHeadEffects`.
Identity: Own OnDie007f0890, OnInitialActivate007f08b0 and PDB private KillRippleFX; PRippleEffect+32.
Layout: CThing active header is absent (donor-only header quarantined). Retail Dead is bit0 at+0x91; no other fields accessed.
Layout: genuine polymorphic intelligent pointer has PData+4, total8bytes on x86; no active shared wrapper header.
Layout: no active owner header; PRippleEffect is an8-byte intelligent pointer at 32, proved by retail address and PDB member. Packed layout preserves the original unaligned member where applicable.

## 007fb150: CTCKraken::EndHeadEffects
Original manifest label: `CTCKraken / EndHeadEffects`.
Identity: Own CreateHeadEffect007fbb00 and InitTentaclesAndHead007fc710; PDB EndHeadEffects and PHeadEffect+40.
Layout: CThing active header is absent (donor-only header quarantined). Retail Dead is bit0 at+0x91; no other fields accessed.
Layout: genuine polymorphic intelligent pointer has PData+4, total8bytes on x86; no active shared wrapper header.
Layout: no active owner header; PHeadEffect is an8-byte intelligent pointer at 40, proved by retail address and PDB member. Packed layout preserves the original unaligned member where applicable.

## 0076b120: std::vector::push_back
Original manifest label: `_global / Vector_PushBack`.
Identity: Own overflow0076ada0 and CParticleUpdateParameters<unsigned long,unsigned long> RTTI0125ffdc, VectorParams member establish unsigned-long/C3DVector pairs.
Layout: original C3DVector has X,Y,Z at0,4,8; no active shared header exists.
Layout: original KnickKnack is Pos+0, Thing+12,16bytes on x86.
Layout: original COptimisedEdge fields are at0,4,8,12,16bytes on x86.

## 007d9f10: std::vector::push_back
Original manifest label: `_global / vector_PushBack_16byte`.
Identity: Own overflow007d9c90, CTCTavernGameSpotTheAddition::KnickKnacks and PDB struct/copy constructor establish KnickKnack.
Layout: original C3DVector has X,Y,Z at0,4,8; no active shared header exists.
Layout: original KnickKnack is Pos+0, Thing+12,16bytes on x86.
Layout: original COptimisedEdge fields are at0,4,8,12,16bytes on x86.

## 00ad5ce0: std::vector::push_back
Original manifest label: `std::vector / std::vector::_InsertFillOne`.
Identity: Own overflow00ad4d10 and caller00ac8c48 in COptimisedPrimitive::JoinAdjacentFaces insert FaceA,FaceB,UniqueVertex1,UniqueVertex2 into Edges at retail+0x28; PDB proves class and namespace.
Layout: original C3DVector has X,Y,Z at0,4,8; no active shared header exists.
Layout: original KnickKnack is Pos+0, Thing+12,16bytes on x86.
Layout: original COptimisedEdge fields are at0,4,8,12,16bytes on x86.

## 0076b320: std::vector::operator=
Original manifest label: `_global / Vector_Assign_WithGrow`.
Identity: CTCObjectAugmentations AttackParticleEffects vector assignment; own copy007698c0, assign00769c00,destroy0076a2a0.
Layout: original STLport vector12bytes and24/12/20-byte element layouts. Same-TU real helper loops expose input reads and destruction without modifying empty tags.
Layout: genuine polymorphic CVertexStreamFormat is16bytes: vptr+0 and12-byte DataElements+4. No active shared header.
Layout: owner supplies nested-name scope only. CStreamInfo is StreamIndex+0, StreamFormat+4,20bytes.

## 00a5ae00: std::vector::operator=
Original manifest label: `_global / Vector_Assign_Pair64_CountedPointer`.
Identity: CCachedVolumes::NavigationContainmentVolumes CVectorMap copy00a5aca0 and own destroy00825ae0 prove signed-long/containment-volume pointer pair, not manifest64-bit script pair.
Layout: original STLport vector12bytes and24/12/20-byte element layouts. Same-TU real helper loops expose input reads and destruction without modifying empty tags.
Layout: genuine polymorphic CVertexStreamFormat is16bytes: vptr+0 and12-byte DataElements+4. No active shared header.
Layout: owner supplies nested-name scope only. CStreamInfo is StreamIndex+0, StreamFormat+4,20bytes.

## 00a5e9c0: std::vector::operator=
Original manifest label: `_global / StdVector_Assign_CSkeletalMorph`.
Identity: Own uninitialized_copy00a5e7f0, copy00a5e830 and CVertexShaderInput DataStreams assignment; CVertexStreamFormat RTTI0129d578; PDB nested class keyV.
Layout: original STLport vector12bytes and24/12/20-byte element layouts. Same-TU real helper loops expose input reads and destruction without modifying empty tags.
Layout: genuine polymorphic CVertexStreamFormat is16bytes: vptr+0 and12-byte DataElements+4. No active shared header.
Layout: owner supplies nested-name scope only. CStreamInfo is StreamIndex+0, StreamFormat+4,20bytes.

## 0076bf80: std::vector::resize
Original manifest label: `vector<unsigned_int / resize`.
Identity: Own copy0071fa60 and destroy005b26e9 prove CDefPointer<const CThingBaseDef>; creature-generator callers00767200/007673c0.
Layout: original pointer wrapper is4bytes; STLport vector is12bytes.

## 00b83fc0: std::vector::resize
Original manifest label: `vector<unsigned_int / resize`.
Identity: Own erase00b83d30/reserve00b83970 use the exact CParticleSimulation counted-pointer helpers.
Layout: original pointer wrapper is8bytes; STLport vector is12bytes.

## 00b98170: std::vector::resize
Original manifest label: `vector<unsigned_int / resize`.
Identity: Own CEngineSceneGrid resize caller00b949a0 and reserve00b97af0 corroborate SceneGrid counted-pointer type; older generic helper labels are descriptive.
Layout: original pointer wrapper is8bytes; STLport vector is12bytes.

## 00770e50: CTCSolidToThing::CTCSolidToThing
Original manifest label: `CTCBed / CTCBed`.
Identity: proven; own RTTI vtable0x0126A62C, factory allocation caller and PDB CThing& constructor; NotSolidToThings vector+12, retail12bytes.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header; real polymorphic CTCBase12 retains ParentThing+4, flags+8 and vector+12.

## 00778ae0: CTCFamilyMember::CTCFamilyMember
Original manifest label: `CTCBed / CTCBed`.
Identity: proven; own RTTI vtable0x0126AADC, factory allocation caller and PDB CThing& constructor; FamilyMembers vector+12, retail12bytes.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header; real polymorphic CTCBase12 retains ParentThing+4, flags+8 and vector+12.

## 008d7ca0: NCreatureMode::CModeSitBase::CModeSitBase
Original manifest label: `CModeSitBase / CModeSitBase`.
Identity: proven; RTTI NCreatureMode::CModeSitBase vtable0x1285698; PDB CThing& ctor and own chair/floor/bench constructors; CTCCreatureModeManager::CModeBase0x73EAC0, PlayingIdle+32/PRemoveAction+36.
Layout: no target header; typed base28 and counted-pointer8 preserve observed fields. WeaponBasedFilter is an unused12-byte composite view; IdleCounter is uninitialized.

## 00772490: CTCShop::IsBarberShop
Original manifest label: `CTCShop / IsBarberShop`.
Identity: proven; PDB own const IsBarberShop name; CShopDef corresponding bool retail+0x99 (donor169/170/171 minus16); own shop inventory/map callers; CDefClassBase GetSubDef<CShopDef>0x5130A0.
Layout: engine/CIVCountedPointeeBase.h flattens the vptr. Actual inline Release needs count+4 and virtual Delete slot1; the local polymorphic interface preserves these observed accesses.
Layout: engine/CDefClassBase.h flattens the counted base into _base_0x04 and lacks GetSubDef/counted virtual APIs. Local query models project just the counted prefix and const template interface; the absolute ShopDef flag prefix includes unmodeled inherited bytes and does not assert a full CDefClassBase sizeof. In the heap source this outer declaration supplies nested CSubDefInfo naming scope only, with no outer instance.
Layout: engine/CDefinitionManager.h is a data-only layout without callable GetDef/GetPDefFromGlobalIndex APIs. The local declaration supplies those missing const template/member interfaces; no manager fields or sizeof are used. The same-TU GetDef body is genuine pointer lookup/assignment, not a callee substitute.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no CTCShop header; generated CTCBase flattens polymorphism and cannot declare inherited protected accessor. Local real base chain keeps ParentThing+4. CThing is quarantined. CShopDef has no header; count+4 and named category flags use the reconciled two-vector/two-map retail displacement. Genuine same-TU GetDef exposes output-slot lifetime.
Layout: engine/CIVCountedPointeeBase.h flattens the vptr. Retail release reads the count at Object+4 and calls Delete at vtable+4; the local real polymorphic base preserves those offsets and const API.

## 00772510: CTCShop::IsTattooShop
Original manifest label: `CTCShop / IsBarberShop`.
Identity: proven; Original const CTCShop query PDB and own field/caller. Full actual CDefClassBase/CSubDef/CParentDef/CThingBaseDef/ShopDef/Resource/CIV hierarchy, private Def default constructor with original friend classes, real CDefPointer storage/SetDynamicCast/lifetimes. Own005130a0 real GetSubDef body: StatGetTypeName CShopDef literal, const GetPSubDefInfo virtualslot14, original nested CSubDefInfo.DefIndex/OriginalParentDefIndex, mutable protected CDefinitionManager::GetPDefFromGlobalIndex009ada10 distinct from const009ad9e0. Source-visible genuine body exposes out-pointer lifetime; no fake callee. Full typed ShopDef prefix through own bools and correct genuine protected STLport vector/list/map storage; inactive ShopDef/manager suffix omitted without totalsizeclaim. CThing real CClass/CBaseObject/CBaseClass ancestry and correct DefGlobalIndex140, unused Thing prefix explicit opaque. Shared original headers aliased unchanged.
Layout: full original base chains and known typed data, context-only unused prefixes/suffixes explicit.

## 00772590: CTCShop::IsTitleShop
Original manifest label: `CTCShop / IsBarberShop`.
Identity: proven; Original const CTCShop query PDB and own field/caller. Full actual CDefClassBase/CSubDef/CParentDef/CThingBaseDef/ShopDef/Resource/CIV hierarchy, private Def default constructor with original friend classes, real CDefPointer storage/SetDynamicCast/lifetimes. Own005130a0 real GetSubDef body: StatGetTypeName CShopDef literal, const GetPSubDefInfo virtualslot14, original nested CSubDefInfo.DefIndex/OriginalParentDefIndex, mutable protected CDefinitionManager::GetPDefFromGlobalIndex009ada10 distinct from const009ad9e0. Source-visible genuine body exposes out-pointer lifetime; no fake callee. Full typed ShopDef prefix through own bools and correct genuine protected STLport vector/list/map storage; inactive ShopDef/manager suffix omitted without totalsizeclaim. CThing real CClass/CBaseObject/CBaseClass ancestry and correct DefGlobalIndex140, unused Thing prefix explicit opaque. Shared original headers aliased unchanged.
Layout: full original base chains and known typed data, context-only unused prefixes/suffixes explicit.

## 0077b250: NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::CTCGraphicAppearanceAnimatedMesh
Original manifest label: `CTCGraphicAppearanceAnimatedMesh / CTCGraphicAppearanceAnimatedMesh`.
Identity: proven; RTTI primary vtable0x0126ACDC/secondary0x0126ACC0, PDB own CThing& constructor and PrimHandle+16; CAGraphicAppearanceComponent secondary+12/RegisterComponent0x4BD390.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header. Real RTTI multiple inheritance supplies secondary vptr+12 and PrimHandle+16; CThing is quarantined, local named PTCGraphicAppearance view preserves+0x64.

## 0077b9b0: NGraphicAppearance::CTCGraphicAppearanceStaticMesh::CTCGraphicAppearanceStaticMesh
Original manifest label: `CTCGraphicAppearanceAnimatedMesh / CTCGraphicAppearanceAnimatedMesh`.
Identity: proven; RTTI primary vtable0x0126AD6C/secondary0x0126AD50, PDB own CThing& constructor and PrimHandle+16; CAGraphicAppearanceComponent secondary+12/RegisterComponent0x4BD390.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header. Real RTTI multiple inheritance supplies secondary vptr+12 and PrimHandle+16; CThing is quarantined, local named PTCGraphicAppearance view preserves+0x64.

## 0077be40: NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity::CTCGraphicAppearanceStaticMeshFadeOutOnProximity
Original manifest label: `CTCGraphicAppearanceAnimatedMesh / CTCGraphicAppearanceAnimatedMesh`.
Identity: proven; RTTI primary vtable0x0126AE04/secondary0x0126ADEC, PDB own CThing& constructor and PrimHandle+16; CAGraphicAppearanceComponent secondary+12/RegisterComponent0x4BD390.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header. Real RTTI multiple inheritance supplies secondary vptr+12 and PrimHandle+16; CThing is quarantined, local named PTCGraphicAppearance view preserves+0x64.

## 0077b2e0: NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::GetMeshesUsed
Original manifest label: `_global / Vector_PushBackOrInsertWithProperties`.
Identity: proven; own RTTI secondary vtable0x0126ACC0 slot2 and PDB GetMeshesUsed(vector<CMeshInfo>&)const; secondary this+12 -> ParentThing-relative-8; MainGraphic retail+0x44, BankIndex+0 and RenderSizeX+8.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header, and GraphicAppearance/CThing headers are quarantined. Real MI supplies the secondary receiver convention; minimal named MainGraphic view reconciles two maps/two lists from donor+92 to retail+68. Actual CMeshInfo bitfield PrimaryMesh=true preserves the retail OR into the otherwise uninitialized flag byte. Genuine same-TU vector overflow, as verified N0230x7091A0.

## 0077bc80: NGraphicAppearance::CTCGraphicAppearanceStaticMesh::GetMeshesUsed
Original manifest label: `_global / Vector_PushBackOrInsertWithProperties`.
Identity: proven; own RTTI secondary vtable0x0126AD50 slot2 and PDB GetMeshesUsed(vector<CMeshInfo>&)const; secondary this+12 -> ParentThing-relative-8; MainGraphic retail+0x44, BankIndex+0 and RenderSizeX+8.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header, and GraphicAppearance/CThing headers are quarantined. Real MI supplies the secondary receiver convention; minimal named MainGraphic view reconciles two maps/two lists from donor+92 to retail+68. Actual CMeshInfo bitfield PrimaryMesh=true preserves the retail OR into the otherwise uninitialized flag byte. Genuine same-TU vector overflow, as verified N0230x7091A0.

## 007a6ab0: NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::GetMeshesUsed
Original manifest label: `_global / Vector_Insert_ETCEvent`.
Identity: proven; own RTTI secondary vtable0x0126CB60 slot2 and PDB GetMeshesUsed(vector<CMeshInfo>&)const; secondary this+12 -> ParentThing-relative-8; MainGraphic retail+0x44, BankIndex+0 and RenderSizeX+8.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header, and GraphicAppearance/CThing headers are quarantined. Real MI supplies the secondary receiver convention; minimal named MainGraphic view reconciles two maps/two lists from donor+92 to retail+68. Actual CMeshInfo bitfield PrimaryMesh=true preserves the retail OR into the otherwise uninitialized flag byte. Genuine same-TU vector overflow, as verified N0230x7091A0.

## 00786140: CThingSearchTools::GetPBestThingInRoughArea
Original manifest label: `GetAllThingsInRoughArea<CThingGameObject_const_,std::vector<CIntelligentPointer<CThing_const_>,std / GetAllThingsInRoughArea<CThingGameObject_const_,std::vector<CIntelligentPointer<CThing_const_>,std::allocator<CIntelligentPointer<CThing_const_>_>_>,CThingFilter_None>`.
Identity: proven; own CTCVolumeContainmentTracker::Update0x785A6F, filter HasTC0x6A80A0/comparator<CThing>0x4A8D80; mask0 and PDB mutable CThing instance.
Layout: the generic search model carries only this class type-mask tag; these three instantiations search CThing/const CThing. No CreatureBase object, field, sizeof, constructor or method is used, so its generated flattened full header layout is not asserted or accessed.
Layout: the generic search model carries only this class type-mask tag; these three instantiations search CThing/const CThing. No AICreature object, field, sizeof, constructor or method is used. FreshDeed invokes an external filter and keeps its tracked-creature member opaque; no generated full AICreature layout is asserted or accessed.
Layout: engine/CTCMapwho.h flattens CTCBase and splits all node members into pointer fragments. The local minimal view reads inherited ParentThing+4 (named Thing in this view) and SearchID+0x4C, agreeing with the header; linked-node traversal uses only the first two real node pointer fields.
Layout: shared SearchTools header retains AreaMaps evidence; local mutable actual12-byte vector and real NThingMap::CThingMap namespace/key reconcile donor16. CThing/CTCMapwho partial named views are documented in N035; no instruction transplant. FreshDeed PCreature is an unused opaque8-byte member view because only its external filter call is performed.

## 00961080: CThingSearchTools::GetPBestThingInRoughArea
Original manifest label: `GetAllThingsInRoughArea<CThingGameObject_const_,std::vector<CIntelligentPointer<CThing_const_>,std / GetAllThingsInRoughArea<CThingGameObject_const_,std::vector<CIntelligentPointer<CThing_const_>,std::allocator<CIntelligentPointer<CThing_const_>_>_>,CThingFilter_None>`.
Identity: proven; hand951G070 exact mapping; own DeedReactionRoot::IsRunnable0x960B41/filter0x960FC0/comparator<constCThing>0x95FDA0 and exact constCThing PDB instance.
Layout: the generic search model carries only this class type-mask tag; these three instantiations search CThing/const CThing. No CreatureBase object, field, sizeof, constructor or method is used, so its generated flattened full header layout is not asserted or accessed.
Layout: the generic search model carries only this class type-mask tag; these three instantiations search CThing/const CThing. No AICreature object, field, sizeof, constructor or method is used. FreshDeed invokes an external filter and keeps its tracked-creature member opaque; no generated full AICreature layout is asserted or accessed.
Layout: engine/CTCMapwho.h flattens CTCBase and splits all node members into pointer fragments. The local minimal view reads inherited ParentThing+4 (named Thing in this view) and SearchID+0x4C, agreeing with the header; linked-node traversal uses only the first two real node pointer fields.
Layout: shared SearchTools header retains AreaMaps evidence; local mutable actual12-byte vector and real NThingMap::CThingMap namespace/key reconcile donor16. CThing/CTCMapwho partial named views are documented in N035; no instruction transplant. FreshDeed PCreature is an unused opaque8-byte member view because only its external filter call is performed.

## 009697e0: CThingSearchTools::GetPBestThingInRoughArea
Original manifest label: `GetAllThingsInRoughArea<CThingGameObject_const_,std::vector<CIntelligentPointer<CThing_const_>,std / GetAllThingsInRoughArea<CThingGameObject_const_,std::vector<CIntelligentPointer<CThing_const_>,std::allocator<CIntelligentPointer<CThing_const_>_>_>,CThingFilter_None>`.
Identity: proven; own TavernKeeper::FillJug0x9690CD/filter HasDefName0x6A7BE0/comparator<CThing>0x4A8D80 and matching mutableCThing PDB instance.
Layout: the generic search model carries only this class type-mask tag; these three instantiations search CThing/const CThing. No CreatureBase object, field, sizeof, constructor or method is used, so its generated flattened full header layout is not asserted or accessed.
Layout: the generic search model carries only this class type-mask tag; these three instantiations search CThing/const CThing. No AICreature object, field, sizeof, constructor or method is used. FreshDeed invokes an external filter and keeps its tracked-creature member opaque; no generated full AICreature layout is asserted or accessed.
Layout: engine/CTCMapwho.h flattens CTCBase and splits all node members into pointer fragments. The local minimal view reads inherited ParentThing+4 (named Thing in this view) and SearchID+0x4C, agreeing with the header; linked-node traversal uses only the first two real node pointer fields.
Layout: shared SearchTools header retains AreaMaps evidence; local mutable actual12-byte vector and real NThingMap::CThingMap namespace/key reconcile donor16. CThing/CTCMapwho partial named views are documented in N035; no instruction transplant. FreshDeed PCreature is an unused opaque8-byte member view because only its external filter call is performed.

## 0078e300: CTCDivineWrath::PerCreatureInfo::PerCreatureInfo
Original manifest label: `CAIStateGroup_OpinionReactionRoot / CAIStateGroup_OpinionReactionRoot`.
Identity: probable owner/function role; PDB unique24-byte PerCreatureInfo record has ActualCreature+0,FramesToGetHit+8,Effect+12,DefaultChannel+20; both PDB sets contain CTCDivineWrath nested ctor; owner allocation by position in own spell code region, no xrefs.
Layout: generated outer headers have no nested record; outer declaration supplies naming scope only. Real eight-byte tracked pointers and four-byte channel retain Pad/FramesToGetHit uninitialized.

## 00793940: CTCUnholyPower::PerCreatureInfo::PerCreatureInfo
Original manifest label: `CAIStateGroup_OpinionReactionRoot / CAIStateGroup_OpinionReactionRoot`.
Identity: probable owner/function role; PDB unique24-byte PerCreatureInfo record has ActualCreature+0,FramesToGetHit+8,Effect+12,DefaultChannel+20; both PDB sets contain CTCUnholyPower nested ctor; owner allocation by position in own spell code region, no xrefs.
Layout: generated outer headers have no nested record; outer declaration supplies naming scope only. Real eight-byte tracked pointers and four-byte channel retain Pad/FramesToGetHit uninitialized.

## 00902fb9: CAIStateGroup_CreatureUseCoverBase::CAIStateGroup_CreatureUseCoverBase
Original manifest label: `CAIStateGroup_CreatureUseCoverBase / CAIStateGroup_CreatureUseCoverBase`.
Identity: proven; own RTTI vtable0x128D31C, PDB default constructor; EnemyCreatureBase0x8FF720; packed PCoverThing+31 and CoverList+40 (own destructor0x902F71 corroborates).
Layout: engine/CAIStateGroupBase.h flattens the vptr and Name/priority types. Local real polymorphic CAIStateGroupBase20 is needed by the actual inherited constructor/virtual table; PBrain+4, priority+8, Name+12 and flags+16 agree with header/PDB.
Layout: engine/CAIStateGroup_EnemyCreatureBase.h uses the wrong struct key and flattens both inheritance and PTargetThing. Actual class-key V, base20 and typed eight-byte tracked member give real base28; this permits the packed CoverBase PCoverThing+31 and CoverList+40 proved by its own destructor.
Layout: included header uses wrong struct key/flattened vptr and opaque members; actual class base chain plus packed three flags preserves odd PCoverThing+0x1F and explicit pad+0x27 before vector.

## 00792830: CWillButtonHelperTimer::~CWillButtonHelperTimer
Original manifest label: `CTCInventoryItem / OnDie`.
Identity: probable function role; PDB unique CWillButtonHelperTimer PCreature+8 and vector<float>+24; body unreferenced in spell code region, destructor-versus-Destroy role uncertain.
Layout: no record header. Typed real members provide vector start and tracked pointer/string destruction; CProfile uses actual double-aligned72-byte layout and nested record key is struct.

## 009f99c0: CProfileManager::CProfileTreeEntry::~CProfileTreeEntry
Original manifest label: `CTCInventoryItem / OnDie`.
Identity: probable function role; PDB unique CProfileTreeEntry Profile+0/Name+48,Children+76,Parent+88; own ctor0x9F9990/copy0x9F99E0/Construct0x9F9A90 corroborate record, both bodies unreferenced so role spelling probable.
Layout: no record header. Typed real members provide vector start and tracked pointer/string destruction; CProfile uses actual double-aligned72-byte layout and nested record key is struct.

## 009f9ac0: std::_Destroy
Original manifest label: `CTCInventoryItem / OnDie`.
Identity: probable function role; PDB unique CProfileTreeEntry Profile+0/Name+48,Children+76,Parent+88; own ctor0x9F9990/copy0x9F99E0/Construct0x9F9A90 corroborate record, both bodies unreferenced so role spelling probable.
Layout: no record header. Typed real members provide vector start and tracked pointer/string destruction; CProfile uses actual double-aligned72-byte layout and nested record key is struct.

## 0079f610: CTCWillLightning::GetStrafeAnimName
Original manifest label: `CTCFireballSpell / GetReleaseAnimName`.
Identity: proven; PDB own const GetStrafeAnimName returns CCharString; caller CTCSpecialAbilities/FinishFireball SetAnims; own literal spell/pose names; WeaponState+12,LeftHandAnim+16.
Layout: WillLightning reuses generated header unchanged with typed enum view over named opaque WeaponState. Fireball has no header; minimal data view retains the proved named fields+0x64/+0x68. String ctor uses actual external API.

## 007c85f0: CTCFireballSpell::GetStrafeAnimName
Original manifest label: `CTCFireballSpell / GetReleaseAnimName`.
Identity: proven; PDB own const GetStrafeAnimName returns CCharString; caller CTCSpecialAbilities/FinishFireball SetAnims; own literal spell/pose names; WeaponState+100,LeftHandAnim+104.
Layout: WillLightning reuses generated header unchanged with typed enum view over named opaque WeaponState. Fireball has no header; minimal data view retains the proved named fields+0x64/+0x68. String ctor uses actual external API.

## 007c86f0: CTCFireballSpell::GetReleaseAnimName
Original manifest label: `CTCFireballSpell / GetReleaseAnimName`.
Identity: proven; PDB own const GetReleaseAnimName returns CCharString; caller CTCSpecialAbilities/FinishFireball SetAnims; own literal spell/pose names; WeaponState+100,LeftHandAnim+104.
Layout: WillLightning reuses generated header unchanged with typed enum view over named opaque WeaponState. Fireball has no header; minimal data view retains the proved named fields+0x64/+0x68. String ctor uses actual external API.

## 007a3410: std::__unguarded_insertion_sort_aux
Original manifest label: `_global / BuildHeap_CThingAICreature`.
Identity: proven; lightning target sort; own comparator0x7A2F90 bool CThingCompare_NearestToPos::operator()<CThing>, own linked partialsort0x7A3FA0 and search0x7A5190.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 0091faa0: std::__unguarded_insertion_sort_aux
Original manifest label: `_global / Sort_InsertionSort`.
Identity: probable; hand951G015/hand953J082: TestTestTest Update0x91F140 sort caller and search0x920200 mask0x807 identify vector<constCThingObject*>; comparator0x91F740 is distinct from CThing0x4A8D80; const element by PDB elimination, not direct RTTI.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 0095cf50: std::__unguarded_insertion_sort_aux
Original manifest label: `_global / std_sort_InsertionSort`.
Identity: proven; preferred-fire-point sort own GetFirePointList0x95D4C0/IsRunnable0x95D590; PDB std::sort<CThing**,CThingCompare_Nearest> and actual comparator0x4A8D80.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 007a37b0: std::__make_heap
Original manifest label: `_global / MakeHeap_NearestToPos`.
Identity: proven; lightning target sort; own comparator0x7A2F90 bool CThingCompare_NearestToPos::operator()<CThing>, own linked partialsort0x7A3FA0 and search0x7A5190.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 0091fdb0: std::__make_heap
Original manifest label: `_global / Sort_BuildHeap`.
Identity: probable; hand951G015/hand953J082: TestTestTest Update0x91F140 sort caller and search0x920200 mask0x807 identify vector<constCThingObject*>; comparator0x91F740 is distinct from CThing0x4A8D80; const element by PDB elimination, not direct RTTI.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 0095d880: std::__make_heap
Original manifest label: `_global / std_make_heap_helper`.
Identity: proven; preferred-fire-point sort own GetFirePointList0x95D4C0/IsRunnable0x95D590; PDB std::sort<CThing**,CThingCompare_Nearest> and actual comparator0x4A8D80.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 007a3ac0: std::__insertion_sort
Original manifest label: `_global / HeapSift_Down`.
Identity: proven; lightning target sort; own comparator0x7A2F90 bool CThingCompare_NearestToPos::operator()<CThing>, own linked partialsort0x7A3FA0 and search0x7A5190.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 009200c0: std::__insertion_sort
Original manifest label: `_global / std_sort_DownHeap`.
Identity: probable; hand951G015/hand953J082: TestTestTest Update0x91F140 sort caller and search0x920200 mask0x807 identify vector<constCThingObject*>; comparator0x91F740 is distinct from CThing0x4A8D80; const element by PDB elimination, not direct RTTI.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 0095da90: std::__insertion_sort
Original manifest label: `_global / std_sort_DownHeap`.
Identity: proven; preferred-fire-point sort own GetFirePointList0x95D4C0/IsRunnable0x95D590; PDB std::sort<CThing**,CThingCompare_Nearest> and actual comparator0x4A8D80.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 007a3f30: std::__final_insertion_sort
Original manifest label: `_global / make_heap<CThing**,CThingCompare_BestForTargeting2>`.
Identity: proven; lightning target sort; own comparator0x7A2F90 bool CThingCompare_NearestToPos::operator()<CThing>, own linked partialsort0x7A3FA0 and search0x7A5190.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 00920480: std::__final_insertion_sort
Original manifest label: `_global / make_heap<CThing**,CThingCompare_BestForTargeting2>`.
Identity: probable; hand951G015/hand953J082: TestTestTest Update0x91F140 sort caller and search0x920200 mask0x807 identify vector<constCThingObject*>; comparator0x91F740 is distinct from CThing0x4A8D80; const element by PDB elimination, not direct RTTI.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 0095ddf0: std::__final_insertion_sort
Original manifest label: `_global / make_heap<CThing**,CThingCompare_BestForTargeting2>`.
Identity: proven; preferred-fire-point sort own GetFirePointList0x95D4C0/IsRunnable0x95D590; PDB std::sort<CThing**,CThingCompare_Nearest> and actual comparator0x4A8D80.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 007a4140: std::__introsort_loop
Original manifest label: `_global / QuickSort_Partition`.
Identity: proven; lightning target sort; own comparator0x7A2F90 bool CThingCompare_NearestToPos::operator()<CThing>, own linked partialsort0x7A3FA0 and search0x7A5190.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 009205d0: std::__introsort_loop
Original manifest label: `_global / std_sort_QuickSort`.
Identity: probable; hand951G015/hand953J082: TestTestTest Update0x91F140 sort caller and search0x920200 mask0x807 identify vector<constCThingObject*>; comparator0x91F740 is distinct from CThing0x4A8D80; const element by PDB elimination, not direct RTTI.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 0095df40: std::__introsort_loop
Original manifest label: `_global / std_sort_QuickSort`.
Identity: proven; preferred-fire-point sort own GetFirePointList0x95D4C0/IsRunnable0x95D590; PDB std::sort<CThing**,CThingCompare_Nearest> and actual comparator0x4A8D80.
Layout: no comparator/vector header. Real C3DVector user copy constructor preserves by-value comparator12bytes. Same-TU comparator and STL helpers contain genuine null checks, distance calculation and full algorithms. CThingCompare_Nearest intentionally returns the nearer pointer, so STL tests pointer truth as retail does.

## 007af6c0: CTCBoss::AddEffect
Original manifest label: `CCreatureActionBase / AddDependentThing`.
Identity: proven; own JackFloatUp/ScorpionKingVulnerable callers use TCinterface0xAD; RTTI CTCBoss GetInterfaceType0x7AFCD0, PDB AddEffect(CThing&) and Effects donor64->retail60 after earlier vector shrink; own vector<CIntelligentPointer<CThing>>::push_back0x4AD520.
Layout: CreatureActionBase shared data header reused unchanged under alias, typed view over named opaque vector. Other owners have no usable header and use minimal member views at proved offsets. Real tracked-pointer constructor/base destructor calls remain external.

## 00810970: CUnprovenDriverOwner_00810970::AppendTrackedThing
Original manifest label: `CCreatureActionBase / AddDependentThing`.
Identity: descriptive; own IceTrollIcicleAttack caller uses returned thing PTCDriver+0x5C; tracked CThing temporary vtable0x1238C6C and vector+32 proved, dynamic owner and exact method/parameter role unproved; own vector<CIntelligentPointer<CThing>>::push_back0x4AD520.
Layout: CreatureActionBase shared data header reused unchanged under alias, typed view over named opaque vector. Other owners have no usable header and use minimal member views at proved offsets. Real tracked-pointer constructor/base destructor calls remain external.

## 00871cc0: CCreatureActionBase::AddDependentThing
Original manifest label: `CCreatureActionBase / AddDependentThing`.
Identity: proven; own CActionEventTriggerParticle::Dispatch caller; PDB AddDependentThing(CThing&), actual DependentThings donor164->retail156 due two earlier containers; own vector<CIntelligentPointer<CThing>>::push_back0x4AD520.
Layout: CreatureActionBase shared data header reused unchanged under alias, typed view over named opaque vector. Other owners have no usable header and use minimal member views at proved offsets. Real tracked-pointer constructor/base destructor calls remain external.

## 007b7c30: CTCOverheadDisplay::AddDamageChange
Original manifest label: `_global / List_AppendNode_IfFlag25`.
Identity: proven; PDB own AddDamageChange(float), own PDef+24->DisplayDamage bool and DamageValues list+12; Renown caller CTCHeroStats::AddRenown0x57D977.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: shared COverheadDisplayDef data header reused unchanged. No component header; real CTCBase12, retail4-byte list sentinel pointers and PDef+24 agree with own OnCreate. Actual STLport list node insertion and pair construction are inline C++.

## 007b7c80: CTCOverheadDisplay::AddRepairChange
Original manifest label: `_global / List_AppendNode_IfFlag26`.
Identity: proven; Own original AddRepairChange/AddRenownChange(float) namesets, flags38 and list16, PDef24 original. Restore original CTCBase:NonCopyable:BaseClass typed ParentThing/bools and protected noncopyable special members. Actual COverheadDisplayDef:CSubDefClassBase:CDefClassBase:CDefPointeeBase:CResource:CIVCountedPointeeBase RTTI, packed37 defbase and original bool37..39; real CDefPointer<COverheadDisplayDef const> stores CDefPointeeBase* with real AddRef/Release. Proper STLport listbase/rebound-node allocatorproxy and iterator/insert API; actual member PrimitiveHandles forward-only vector, full40-byte retailowner. No false flat T* wrapper, flat CTCBase or directlistsentinel storage.
Layout: complete original base chains and containers; shared definition/base headers aliased unchanged.

## 007b7cd0: CTCOverheadDisplay::AddRenownChange
Original manifest label: `_global / CTCMarketPlace_AddRenownEntry`.
Identity: proven; Own original AddRepairChange/AddRenownChange(float) namesets, flags39 and list20, PDef24 original. Restore original CTCBase:NonCopyable:BaseClass typed ParentThing/bools and protected noncopyable special members. Actual COverheadDisplayDef:CSubDefClassBase:CDefClassBase:CDefPointeeBase:CResource:CIVCountedPointeeBase RTTI, packed37 defbase and original bool37..39; real CDefPointer<COverheadDisplayDef const> stores CDefPointeeBase* with real AddRef/Release. Proper STLport listbase/rebound-node allocatorproxy and iterator/insert API; actual member PrimitiveHandles forward-only vector, full40-byte retailowner. No false flat T* wrapper, flat CTCBase or directlistsentinel storage.
Layout: complete original base chains and containers; shared definition/base headers aliased unchanged.

## 007ce390: CTCElectrocutedEffect::FinalCleanup
Original manifest label: `CTCElectrocutedEffect / FinalCleanup`.
Identity: proven owner/method; PDB own FinalCleanup prototype; OnKill RTTI slot14, OnRemove slot2 and private FinalCleanup adjacent identical body; GraphicAppearance+0x64; RemoveTintColour0x4BFAE0 proved hand951G005. ClearHighlighted0x4BFFC0 spelling probable from PDB API/order and tail-jump to ClearMeshEffect0x4BFE20.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header; real CTCBase inheritance retains ParentThing+4. Quarantined CThing requires local named PTCGraphicAppearance+0x64 view.

## 007ce3c0: CTCElectrocutedEffect::OnKill
Original manifest label: `CTCElectrocutedEffect / FinalCleanup`.
Identity: proven owner/method; PDB own OnKill prototype; OnKill RTTI slot14, OnRemove slot2 and private FinalCleanup adjacent identical body; GraphicAppearance+0x64; RemoveTintColour0x4BFAE0 proved hand951G005. ClearHighlighted0x4BFFC0 spelling probable from PDB API/order and tail-jump to ClearMeshEffect0x4BFE20.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header; real CTCBase inheritance retains ParentThing+4. Quarantined CThing requires local named PTCGraphicAppearance+0x64 view.

## 007ce3f0: CTCElectrocutedEffect::OnRemove
Original manifest label: `CTCElectrocutedEffect / FinalCleanup`.
Identity: proven owner/method; PDB own OnRemove prototype; OnKill RTTI slot14, OnRemove slot2 and private FinalCleanup adjacent identical body; GraphicAppearance+0x64; RemoveTintColour0x4BFAE0 proved hand951G005. ClearHighlighted0x4BFFC0 spelling probable from PDB API/order and tail-jump to ClearMeshEffect0x4BFE20.
Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
Layout: no target header; real CTCBase inheritance retains ParentThing+4. Quarantined CThing requires local named PTCGraphicAppearance+0x64 view.

## 007d97e0: std::iter_swap
Original manifest label: `CNearestBuildingWithProperty / CNearestBuildingWithProperty`.
Identity: descriptive original function/type role remains unproven; exact contract is16-byte bitwise exchange with ECX/EDX receivers. Actual standard STLport iter_swap->__iter_swap wrapper from documented GOTCHAS1378/1409, genuine value_type tag/placement and forceinline inner body; no invented nontrivial copy constructor or padding to force spills. Record words now portable32-bit unsignedint; original record field composition/name/class key not recovered, explicit address-specific CUnproven projection.

## 007d9a00: std::iter_swap
Original manifest label: `CNearestBuildingWithProperty / CNearestBuildingWithProperty`.
Identity: descriptive; unreferenced 16-byte value swap, ECX/EDX receivers; no actual owning record/type or swap-versus-iter_swap role is proved. Catalog NearestBuilding constructor is false; address-specific descriptive record retained.

## 00806220: std::iter_swap
Original manifest label: `CNearestBuildingWithProperty / CNearestBuildingWithProperty`.
Identity: descriptive; unreferenced 16-byte value swap, ECX/EDX receivers; no actual owning record/type or swap-versus-iter_swap role is proved. Catalog NearestBuilding constructor is false; address-specific descriptive record retained.

## 008195e0: std::__uninitialized_fill_n
Original manifest label: `_global / fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>`.
Identity: proven; corrected element CTextureMorphEntry20: 0081B300 texture morph binary transfer -> pair resize0081D920, and CTextureMorphs::GetAllMorphsOfType0081B0A0 -> vector resize0081DAA0 -> fillinsert0081D0B0 -> this fill helper. hand463 and both PDBs confirm actual class and five fields. Prior CMapwhoLevelInfo model was incorrect.
Layout: no shared CTextureMorphEntry header; PDB class key V, five POD fields.

## 0082f240: std::__uninitialized_fill_n
Original manifest label: `_global / fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>`.
Identity: proven; own CScriptedMapBrush ctor0x82E600 -> vectorctor0x82F630, Cells20-byte Height/Theme/ThemeBlend; existing landed ctor proves fill helper; mutable input from vector insertion for copies.
Layout: engine/CScriptedMapBrush.h contains opaque Cells bytes but has no nested CScriptedMapCell declaration. The local outer declaration supplies naming scope only; the target constructs the independently proved20-byte nested cell and never accesses a brush instance.
Layout: no record header. PixelFormat header is reused unchanged where present; CScriptedMapBrush outer header lacks the nested cell, so its declaration is naming scope only. Actual PDB field types/keys preserve sizes and copy semantics.

## 00b3e390: std::__uninitialized_fill_n
Original manifest label: `_global / fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>`.
Identity: proven; own CTextureManagerPool Initialize/reallocation callers0xB3E96C/B3EB0F, PDB Width/Height/PixelFormat/UseMipmaps/Count20; mutable input from vector insertion for copies.
Layout: no record header. PixelFormat header is reused unchanged where present; CScriptedMapBrush outer header lacks the nested cell, so its declaration is naming scope only. Actual PDB field types/keys preserve sizes and copy semantics.

## 0081a6e0: std::__uninitialized_copy
Original manifest label: `_global / _Copy_impl<CActionInputControl*,CActionInputControl*>`.
Identity: proven; own _M_fill_insert0x81D200/resize0x81DB00 called by CParticleMorphs::GetAllMorphsOfType0x81B160, PDB CEntry36; mutable input from vector insertion for copies.
Layout: no record header. PixelFormat header is reused unchanged where present; CScriptedMapBrush outer header lacks the nested cell, so its declaration is naming scope only. Actual PDB field types/keys preserve sizes and copy semantics.

## 00ab13a0: std::__uninitialized_copy
Original manifest label: `_global / _Copy_impl<CActionInputControl*,CActionInputControl*>`.
Identity: proven; own _M_fill_insert0xAB2E40/resize0xAB3290 -> C3DAnimationBuilder::BuildMeshAnimation0xAB2520; COutputAnimation::Sequences, PDB CSequenceInfo52; mutable input from vector insertion for copies.
Layout: no record header. PixelFormat header is reused unchanged where present; CScriptedMapBrush outer header lacks the nested cell, so its declaration is naming scope only. Actual PDB field types/keys preserve sizes and copy semantics.

## 00b7af30: std::__uninitialized_copy
Original manifest label: `_global / _Copy_impl<CActionInputControl*,CActionInputControl*>`.
Identity: proven; own _M_fill_insert0xB7CAB0 -> resize0xB7EDF0, CShaderRenderManager::BoneMatrices; PDB struct CPreTransposedBoneMatrix48; mutable input from vector insertion for copies.
Layout: no record header. PixelFormat header is reused unchanged where present; CScriptedMapBrush outer header lacks the nested cell, so its declaration is naming scope only. Actual PDB field types/keys preserve sizes and copy semantics.

## 0081ac20: std::__adjust_heap
Original manifest label: `_Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph / _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>`.
Identity: proven; own unsigned-key pair12 sort/map family and LowerBound0x819F70 -> CSkeletalMorphs::GetAllMorphsOfType0x81AE50 proves CEntry8; old CTextureMorph label belongs to separate0x86AFB0.
Layout: outer declarations supply actual nested naming scopes only; no outer object is accessed. PDB class keys V and ordinary STLport pair user copy preserve unsigned key+two-word payload12. No generated nested record header; genuine same-TU push_heap body.

## 0086afb0: std::__adjust_heap
Original manifest label: `_Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph / _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>`.
Identity: proven; own random appearance map sort helpers push0x86A770 and adjacent signed skeletal-morph sort; PDB BodyPartTextureMorphs/GeneralTextureMorphs CVectorMap<unsignedlong,CTextureMorph> with8-byte NewTexture/GroupID.
Layout: outer declarations supply actual nested naming scopes only; no outer object is accessed. PDB class keys V and ordinary STLport pair user copy preserve unsigned key+two-word payload12. No generated nested record header; genuine same-TU push_heap body.

## 009b2f30: std::__adjust_heap
Original manifest label: `_Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph / _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>`.
Identity: proven; own SubDefInfoMap pair12 sort/reserve family; allocation0x9B2780/reserve0x9B4D80 <- CParentDefClassBase::InternalLoadBinary0x9AED70 proves CSubDefInfo8 and unsigned CRC key.
Layout: engine/CDefClassBase.h flattens the counted base into _base_0x04 and lacks GetSubDef/counted virtual APIs. Local query models project just the counted prefix and const template interface; the absolute ShopDef flag prefix includes unmodeled inherited bytes and does not assert a full CDefClassBase sizeof. In the heap source this outer declaration supplies nested CSubDefInfo naming scope only, with no outer instance.
Layout: outer declarations supply actual nested naming scopes only; no outer object is accessed. PDB class keys V and ordinary STLport pair user copy preserve unsigned key+two-word payload12. No generated nested record header; genuine same-TU push_heap body.

## 0081b880: std::__unguarded_partition -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_global / std_partition_3UintSwap`.
Identity: proven; own map/sort helper chain shares N080 element: own unsigned-key pair12 sort/map family and LowerBound0x819F70 -> CSkeletalMorphs::GetAllMorphsOfType0x81AE50 proves CEntry8; old CTextureMorph label belongs to separate0x86AFB0.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 0086b320: std::__unguarded_partition -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `std / std::introsort_3way_partition_uint`.
Identity: proven; own map/sort helper chain shares N080 element: own random appearance map sort helpers push0x86A770 and adjacent signed skeletal-morph sort; PDB BodyPartTextureMorphs/GeneralTextureMorphs CVectorMap<unsignedlong,CTextureMorph> with8-byte NewTexture/GroupID.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 009b3800: std::__unguarded_partition -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_global / Quicksort_Partition_Uint`.
Identity: proven; own map/sort helper chain shares N080 element: own SubDefInfoMap pair12 sort/reserve family; allocation0x9B2780/reserve0x9B4D80 <- CParentDefClassBase::InternalLoadBinary0x9AED70 proves CSubDefInfo8 and unsigned CRC key.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 0081ba10: std::copy_backward
Original manifest label: `_Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,std / _Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,std::pair<unsigned_long,class_C3DDummyObject>*>`.
Identity: probable helper spelling; proven mutable element: particle morph pair40 __linear_insert0081BE30 and map pair chain/GetAllMorphsOfType0081B160; PDB CParticleMorphs::CEntry36. First row has three arguments; latter two also receive the trivial-assignment tag reference, as shown in their actual fill-insert callers.
Layout: real PDB record fields/class keys and sizes40/44/44; shared CPixelFormat reused. No matching record header; outer classes supply actual nested naming scope only. Ordinary trivial assignment emits rep movsd per record.

## 00a00460: std::__copy_backward_ptrs
Original manifest label: `_Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,std / _Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,std::pair<unsigned_long,class_C3DDummyObject>*>`.
Identity: probable helper spelling; proven mutable element: own CGraphicInfo44 fillinsert00A01290 and resize00A01790/00A01820; hand952H022 proves actual CGraphicInfo. First row has three arguments; latter two also receive the trivial-assignment tag reference, as shown in their actual fill-insert callers.
Layout: real PDB record fields/class keys and sizes40/44/44; shared CPixelFormat reused. No matching record header; outer classes supply actual nested naming scope only. Ordinary trivial assignment emits rep movsd per record.

## 00f37d30: std::__copy_backward_ptrs
Original manifest label: `_Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,std / _Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,std::pair<unsigned_long,class_C3DDummyObject>*>`.
Identity: probable helper spelling; proven mutable element: own resource44 fillinsert00F381F0 and resize00F385A0 <- AllocResourceEntry00F372B0, hand953J101/J102 confirm. First row has three arguments; latter two also receive the trivial-assignment tag reference, as shown in their actual fill-insert callers.
Layout: real PDB record fields/class keys and sizes40/44/44; shared CPixelFormat reused. No matching record header; outer classes supply actual nested naming scope only. Ordinary trivial assignment emits rep movsd per record.

## 0081bd10: std::__pop_heap_aux -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_gapscan / sub_0081bd10`.
Identity: proven; own map/sort helper chain shares N080 element: own unsigned-key pair12 sort/map family and LowerBound0x819F70 -> CSkeletalMorphs::GetAllMorphsOfType0x81AE50 proves CEntry8; old CTextureMorph label belongs to separate0x86AFB0.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 0086b630: std::__pop_heap_aux -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_gapscan / sub_0086b630`.
Identity: proven; own map/sort helper chain shares N080 element: own random appearance map sort helpers push0x86A770 and adjacent signed skeletal-morph sort; PDB BodyPartTextureMorphs/GeneralTextureMorphs CVectorMap<unsignedlong,CTextureMorph> with8-byte NewTexture/GroupID.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 009b3dc0: std::__pop_heap_aux -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_gapscan / sub_009b3dc0`.
Identity: proven; own map/sort helper chain shares N080 element: own SubDefInfoMap pair12 sort/reserve family; allocation0x9B2780/reserve0x9B4D80 <- CParentDefClassBase::InternalLoadBinary0x9AED70 proves CSubDefInfo8 and unsigned CRC key.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 0081c5f0: std::__insertion_sort -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std / _Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std::pair<unsigned_long,class_CDefString>_>`.
Identity: proven; own map/sort helper chain shares N080 element: own unsigned-key pair12 sort/map family and LowerBound0x819F70 -> CSkeletalMorphs::GetAllMorphsOfType0x81AE50 proves CEntry8; old CTextureMorph label belongs to separate0x86AFB0.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 0086c7b0: std::__insertion_sort -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std / _Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std::pair<unsigned_long,class_CDefString>_>`.
Identity: proven; own map/sort helper chain shares N080 element: own random appearance map sort helpers push0x86A770 and adjacent signed skeletal-morph sort; PDB BodyPartTextureMorphs/GeneralTextureMorphs CVectorMap<unsignedlong,CTextureMorph> with8-byte NewTexture/GroupID.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 009b5670: std::__insertion_sort -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std / _Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std::pair<unsigned_long,class_CDefString>_>`.
Identity: proven; own map/sort helper chain shares N080 element: own SubDefInfoMap pair12 sort/reserve family; allocation0x9B2780/reserve0x9B4D80 <- CParentDefClassBase::InternalLoadBinary0x9AED70 proves CSubDefInfo8 and unsigned CRC key.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 0081d580: std::__partial_sort -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_global / std_make_partial_heap_3Element`.
Identity: proven; own map/sort helper chain shares N080 element: own unsigned-key pair12 sort/map family and LowerBound0x819F70 -> CSkeletalMorphs::GetAllMorphsOfType0x81AE50 proves CEntry8; old CTextureMorph label belongs to separate0x86AFB0.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 0086d0c0: std::__partial_sort -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `std / std::partial_sort_top_k_CTextureMorph`.
Identity: proven; own map/sort helper chain shares N080 element: own random appearance map sort helpers push0x86A770 and adjacent signed skeletal-morph sort; PDB BodyPartTextureMorphs/GeneralTextureMorphs CVectorMap<unsignedlong,CTextureMorph> with8-byte NewTexture/GroupID.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 009b6ad0: std::__partial_sort -- ordinary STLport algorithm over its actual unsigned-key map element.
Original manifest label: `_global / PartialSort_Heap_Pair12byte`.
Identity: proven; own map/sort helper chain shares N080 element: own SubDefInfoMap pair12 sort/reserve family; allocation0x9B2780/reserve0x9B4D80 <- CParentDefClassBase::InternalLoadBinary0x9AED70 proves CSubDefInfo8 and unsigned CRC key.
Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.

## 0081d920: std::vector::resize
Original manifest label: `vector<CBrainBehaviour / resize`.
Identity: proven; texture morph map binary transfer0081B300 calls this pair24 resize; hand4630081DE90 and PDB CTextureMorphs::Morphs confirm.
Layout: no shared element header; actual PDB class keys and named POD fields; vector retail three-pointer layout. Trivial tail destruction folds away.

## 0081daa0: std::vector::resize
Original manifest label: `vector<CBrainBehaviour / resize`.
Identity: proven; CTextureMorphs::GetAllMorphsOfType0081B0A0 calls this output20 resize; hand4630081DFB0 and PDB CTextureMorphEntry confirm.
Layout: no shared element header; actual PDB class keys and named POD fields; vector retail three-pointer layout. Trivial tail destruction folds away.

## 0085e870: std::vector::resize
Original manifest label: `vector<CBrainBehaviour / resize`.
Identity: proven; CThingMap::InitMapwhoLevels0085D180 calls this resize20; PDB CMapwhoLevelInfo field sequence Size/Extents/ScaleShift/NoThings confirms.
Layout: no shared element header; actual PDB class keys and named POD fields; vector retail three-pointer layout. Trivial tail destruction folds away.

## 0081f3d0: CMap::IsPassableAt
Original manifest label: `CMap / IsPassableAt`.
Identity: proven; CWorldMap::IsPosPassable004FCBD0 and camera wrapper004FCC40 call this getter; actual bit1 and both PDB API.
Layout: unchanged quarantined CMap header reused through method-bearing alias; actual HeightMap_ptr+0x1A7C confirmed by callers. CHeightMap has vector12 rather than donor16: SizeX/SizeY at0x204/0x208. CHeightMapCell header collapses all five bitfields into NavPreferred[3], so actual PDB bitfields used locally; no CHeightMap or C2DCoordI headers.

## 0081f4b0: CMap::IsPrefNavAt
Original manifest label: `CMap / IsPassableAt`.
Identity: proven; navigation initialization00509A20 calls immediately after passability; matching PDB API order between IsCameraPassableAt and SetCameraPassableAt.
Layout: unchanged quarantined CMap header reused through method-bearing alias; actual HeightMap_ptr+0x1A7C confirmed by callers. CHeightMap has vector12 rather than donor16: SizeX/SizeY at0x204/0x208. CHeightMapCell header collapses all five bitfields into NavPreferred[3], so actual PDB bitfields used locally; no CHeightMap or C2DCoordI headers.

## 0081f630: CMap::GetNavPreferredAt
Original manifest label: `CMap / IsPassableAt`.
Identity: proven; CWorldMap::GetNavPreferredAtBlock0063A760 calls this getter; matching PDB API order after GetSoundAt/SetSoundAt and before SetNavPreferredAt.
Layout: unchanged quarantined CMap header reused through method-bearing alias; actual HeightMap_ptr+0x1A7C confirmed by callers. CHeightMap has vector12 rather than donor16: SizeX/SizeY at0x204/0x208. CHeightMapCell header collapses all five bitfields into NavPreferred[3], so actual PDB bitfields used locally; no CHeightMap or C2DCoordI headers.

## 0082e860: CScriptedMapBrush::GetThingsToMove
Original manifest label: `_global / LinkedList_BuildFromTreeIterator`.
Identity: proven; own CMap::ScriptedMapBrushMoveThings00820840 caller and both PDB const list<unsigned __int64>& API; ThingsToMove actual retail sorted tree at0x20.
Layout: unchanged shared CScriptedMapBrush header reused; typed read-only tree view over named opaque containers. ThingsToMove map iterator key at node+16 equals the set UID value view; mapped position/orientation unused. Actual UID unsigned __int64 under VC; portable tests use opaque two uint storage. STLport tree increment and list insertion are genuine C++.

## 0082e910: CScriptedMapBrush::GetThingsToDelete
Original manifest label: `_global / LinkedList_BuildFromTreeIterator`.
Identity: proven; Own CMap caller008209c0 and both original PDB const list<unsigned__int64>& API. OwnRTTI CScriptedMapBrush:CBaseObject:CBaseClass and original full namedfields84; shared aliases preserve unchanged flattened layout evidence. Full original STLport list/base/rebound-nodeproxy and set/tree/base/rebound-nodeproxy, boolcolor, _Rb_global<bool>class and _Rb_tree_iterator->_Rb_tree_base_iterator with traits/usercopy constructors. No artificial read-only tree view, opaque allocator word, directlist storage or flattened engine base. Original setkeys UID64; unused original nested cell/mapvalue types forward-only, no invented fields or element-sizeclaim.
Layout: correct original complete engine/container hierarchies and original class keys, fixed-size data fields.

## 0082e9c0: CScriptedMapBrush::GetThingsToAdd
Original manifest label: `_global / LinkedList_BuildFromTreeIterator`.
Identity: proven; Own CMap caller00820790 and both original PDB const list<unsigned__int64>& API. OwnRTTI CScriptedMapBrush:CBaseObject:CBaseClass and original full namedfields84; shared aliases preserve unchanged flattened layout evidence. Full original STLport list/base/rebound-nodeproxy and set/tree/base/rebound-nodeproxy, boolcolor, _Rb_global<bool>class and _Rb_tree_iterator->_Rb_tree_base_iterator with traits/usercopy constructors. No artificial read-only tree view, opaque allocator word, directlist storage or flattened engine base. Original setkeys UID64; unused original nested cell/mapvalue types forward-only, no invented fields or element-sizeclaim.
Layout: correct original complete engine/container hierarchies and original class keys, fixed-size data fields.

## 0082fed0: CThingAICreature::GetMaxRunningSpeed
Original manifest label: `CThingAICreature / GetMaxJoggingSpeed`.
Identity: proven; PDB API order/override field MaxRunningSpeed+0x1c0 and movement enumMOVEMENT_RUN; supplied jogging labels propagated; own virtual implementations unreferenced; CTCPhysicsBase slot70 unit-Y facing-vector body006B1410, mode GetMovementSpeed slot7.
Layout: shared creature/physics headers flatten vtables and lack required APIs; local typed projections preserve PTCPhysics60/PTCModeManager134 and speed fields1B8/1BC/1C0. Actual nested CTCCreatureModeManager::CModeBase and const reference GetCurrentMode; only known virtual slots typed.

## 0082ff70: CThingAICreature::GetMaxJoggingSpeed
Original manifest label: `CThingAICreature / GetMaxJoggingSpeed`.
Identity: proven; PDB API order/override field MaxJoggingSpeed+0x1bc and movement enumMOVEMENT_JOG; supplied jogging labels propagated; own virtual implementations unreferenced; CTCPhysicsBase slot70 unit-Y facing-vector body006B1410, mode GetMovementSpeed slot7.
Layout: shared creature/physics headers flatten vtables and lack required APIs; local typed projections preserve PTCPhysics60/PTCModeManager134 and speed fields1B8/1BC/1C0. Actual nested CTCCreatureModeManager::CModeBase and const reference GetCurrentMode; only known virtual slots typed.

## 00830000: CThingAICreature::GetMaxWalkingSpeed
Original manifest label: `CThingAICreature / GetMaxJoggingSpeed`.
Identity: proven; PDB API order/override field MaxWalkingSpeed+0x1b8 and movement enumMOVEMENT_WALK; supplied jogging labels propagated; own virtual implementations unreferenced; CTCPhysicsBase slot70 unit-Y facing-vector body006B1410, mode GetMovementSpeed slot7.
Layout: shared creature/physics headers flatten vtables and lack required APIs; local typed projections preserve PTCPhysics60/PTCModeManager134 and speed fields1B8/1BC/1C0. Actual nested CTCCreatureModeManager::CModeBase and const reference GetCurrentMode; only known virtual slots typed.

## 008435b0: CCreatureAction_UseLamp::CCreatureAction_UseLamp
Original manifest label: `CCreatureAction_DeclarePlayerACriminal / CCreatureAction_DeclarePlayerACriminal`.
Identity: proven; own RTTI vtable0x0127486c and PDB two-reference constructor; base CCreatureActionBase(Creature&,CSoundPair,0,0)00693C70; tracked member+0xA8 calls base ctor00A01B10 and SetPItem00A01B90.
Layout: available action headers flatten polymorphic bases and tracked pointers into data, so cannot express real ctor/vtable chain; local original CBaseClass/CBaseObject8 + action168, RTTI confirms action -> CBaseObject -> CBaseClass + actual tracked member8. Shared CSoundPair header absent; PDB default ctor sets both indices0.

## 00843610: CCreatureAction_UseWallMount::CCreatureAction_UseWallMount
Original manifest label: `CCreatureAction_DeclarePlayerACriminal / CCreatureAction_DeclarePlayerACriminal`.
Identity: proven; own RTTI vtable0x0127498c and PDB two-reference constructor; base CCreatureActionBase(Creature&,CSoundPair,0,0)00693C70; tracked member+0xA8 calls base ctor00A01B10 and SetPItem00A01B90.
Layout: available action headers flatten polymorphic bases and tracked pointers into data, so cannot express real ctor/vtable chain; local original CBaseClass/CBaseObject8 + action168, RTTI confirms action -> CBaseObject -> CBaseClass + actual tracked member8. Shared CSoundPair header absent; PDB default ctor sets both indices0.

## 00910c30: CCreatureAction_DeclarePlayerACriminal::CCreatureAction_DeclarePlayerACriminal
Original manifest label: `CCreatureAction_DeclarePlayerACriminal / CCreatureAction_DeclarePlayerACriminal`.
Identity: proven; own RTTI vtable0x0128f624 and PDB two-reference constructor; base CCreatureActionBase(Creature&,CSoundPair,0,0)00693C70; tracked member+0xA8 calls base ctor00A01B10 and SetPItem00A01B90.
Layout: available action headers flatten polymorphic bases and tracked pointers into data, so cannot express real ctor/vtable chain; local original CBaseClass/CBaseObject8 + action168, RTTI confirms action -> CBaseObject -> CBaseClass + actual tracked member8. Shared CSoundPair header absent; PDB default ctor sets both indices0.

## 0084a290: CCreatureAction_StartFillingTankard::PerformAction
Original manifest label: `CCreatureAction_StartFillingTankard / CCreatureAction_StartFillingTankard::PerformAction`.
Identity: proven; own RTTI class/method slot and both PDB method declaration; real tracked PCreature+8 GetPItem00A01B50; TC bitset+0x20 and map+0x44 LowerBound0040F020; AddMode argument42.
Layout: original RTTI CBaseClass/CBaseObject/CCreatureActionBase chain explicit, plus actual CClass/Thing/Physical/GameObject/Creature chain; shared CBaseObject data header included unchanged under alias. GoUnderground immediate base is CCombatActionBase.  shared CTCBase header included unchanged; own component storage is CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > over CArray/std::vector. Original Components donor+0x48 becomes retail+0x44; Compare/Dirty follow three-pointer data. Both PDB LowerBound template name sets confirm enum and real component base.  generated action/base headers flatten polymorphic inheritance and tracked-pointer APIs; minimal real base/PCreature chain retained. CThing header lacks usable template GetTC and bitset/map APIs, so named projection models actual bitset9 and sorted map. No false-branch pointer initialization invented; N091 requires its TC to exist.

## 0084a2f0: CCreatureAction_StopFillingTankard::PerformAction
Original manifest label: `CCreatureAction_StopFillingTankard / CCreatureAction_StopFillingTankard::PerformAction`.
Identity: proven; own RTTI class/method slot and both PDB method declaration; real tracked PCreature+8 GetPItem00A01B50; TC bitset+0x20 and map+0x44 LowerBound0040F020; RemoveMode argument42.
Layout: original RTTI CBaseClass/CBaseObject/CCreatureActionBase chain explicit, plus actual CClass/Thing/Physical/GameObject/Creature chain; shared CBaseObject data header included unchanged under alias. GoUnderground immediate base is CCombatActionBase.  shared CTCBase header included unchanged; own component storage is CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > over CArray/std::vector. Original Components donor+0x48 becomes retail+0x44; Compare/Dirty follow three-pointer data. Both PDB LowerBound template name sets confirm enum and real component base.  generated action/base headers flatten polymorphic inheritance and tracked-pointer APIs; minimal real base/PCreature chain retained. CThing header lacks usable template GetTC and bitset/map APIs, so named projection models actual bitset9 and sorted map. No false-branch pointer initialization invented; N091 requires its TC to exist.

## 008b75e0: CCreatureAction_GoUnderground::PerformAction
Original manifest label: `CCreatureAction_GoUnderground / CCreatureAction_GoUnderground::PerformAction`.
Identity: proven; own RTTI class/method slot and both PDB method declaration; real tracked PCreature+8 GetPItem00A01B50; TC bitset+0x20 and map+0x44 LowerBound0040F020; AddMode argument30.
Layout: original RTTI CBaseClass/CBaseObject/CCreatureActionBase chain explicit, plus actual CClass/Thing/Physical/GameObject/Creature chain; shared CBaseObject data header included unchanged under alias. GoUnderground immediate base is CCombatActionBase.  shared CTCBase header included unchanged; own component storage is CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > over CArray/std::vector. Original Components donor+0x48 becomes retail+0x44; Compare/Dirty follow three-pointer data. Both PDB LowerBound template name sets confirm enum and real component base.  generated action/base headers flatten polymorphic inheritance and tracked-pointer APIs; minimal real base/PCreature chain retained. CThing header lacks usable template GetTC and bitset/map APIs, so named projection models actual bitset9 and sorted map. No false-branch pointer initialization invented; N091 requires its TC to exist.

## 0084e1a0: NCameraModes::CBase::AddAttractorGoTo
Original manifest label: `_global / LinkedListInsertNode`.
Identity: proven; both PDB method declaration and named CPhysicsState::Attractors; retail GoTo.Attractors sentinel+120; first two have camera mode OnInit/Update callers, third unreferenced but exact matching API/order and LookAt offset.
Layout: no scoped base/CPhysicsState header; generated CBase namesake is unrelated. Actual CPhysicsState48 vs donor64 comes from two list12->4 contractions, giving LookAt16/GoTo84. C3DVector actual user copy and STLport list/pair bodies retained.

## 0084e200: NCameraModes::CBase::AddIncidentalAttractorGoTo
Original manifest label: `_global / CList_InsertNode`.
Identity: proven; both PDB method declaration and named CPhysicsState::IncidentalAttractors; retail GoTo.IncidentalAttractors sentinel+124; first two have camera mode OnInit/Update callers, third unreferenced but exact matching API/order and LookAt offset.
Layout: no scoped base/CPhysicsState header; generated CBase namesake is unrelated. Actual CPhysicsState48 vs donor64 comes from two list12->4 contractions, giving LookAt16/GoTo84. C3DVector actual user copy and STLport list/pair bodies retained.

## 0084e260: NCameraModes::CBase::AddAttractorLookAt
Original manifest label: `_global / LinkedList_InsertNode`.
Identity: proven; both PDB method declaration and named CPhysicsState::Attractors; retail LookAt.Attractors sentinel+52; first two have camera mode OnInit/Update callers, third unreferenced but exact matching API/order and LookAt offset.
Layout: no scoped base/CPhysicsState header; generated CBase namesake is unrelated. Actual CPhysicsState48 vs donor64 comes from two list12->4 contractions, giving LookAt16/GoTo84. C3DVector actual user copy and STLport list/pair bodies retained.

## 00855880: CCreatureAction_StartBlocking::Initialise
Original manifest label: `CCreatureAction_StartBlocking / CCreatureAction_StartBlocking::Initialise`.
Identity: proven; CTCCombat RTTI vtable0126724C slot21 returns0x30 at00739CE0; PDB SetStartingToBlock(bool) sets StartingToBlock retail0x62 (donor0x6E minus12); original navigator callee label was wrong.  own RTTI class/method slot and both PDB method declaration; real tracked PCreature+8 GetPItem00A01B50; TC bitset+0x20 and map+0x44 LowerBound0040F020; SetStartingToBlock argument1.
Layout: shared CTCBase header included unchanged; own component storage is CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > over CArray/std::vector. Original Components donor+0x48 becomes retail+0x44; Compare/Dirty follow three-pointer data. Both PDB LowerBound template name sets confirm enum and real component base.  generated action/base headers flatten polymorphic inheritance and tracked-pointer APIs; minimal real base/PCreature chain retained. CThing header lacks usable template GetTC and bitset/map APIs, so named projection models actual bitset9 and sorted map. No false-branch pointer initialization invented; N091 requires its TC to exist.

## 008558d0: CCreatureAction_StartBlocking::FinishAction
Original manifest label: `CCreatureAction_StartBlocking / CCreatureAction_StartBlocking::FinishAction`.
Identity: proven; CTCCombat RTTI vtable0126724C slot21 returns0x30 at00739CE0; PDB SetStartingToBlock(bool) sets StartingToBlock retail0x62 (donor0x6E minus12); original navigator callee label was wrong.  own RTTI class/method slot and both PDB method declaration; real tracked PCreature+8 GetPItem00A01B50; TC bitset+0x20 and map+0x44 LowerBound0040F020; SetStartingToBlock argument0.
Layout: shared CTCBase header included unchanged; own component storage is CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > over CArray/std::vector. Original Components donor+0x48 becomes retail+0x44; Compare/Dirty follow three-pointer data. Both PDB LowerBound template name sets confirm enum and real component base.  generated action/base headers flatten polymorphic inheritance and tracked-pointer APIs; minimal real base/PCreature chain retained. CThing header lacks usable template GetTC and bitset/map APIs, so named projection models actual bitset9 and sorted map. No false-branch pointer initialization invented; N091 requires its TC to exist.

## 008cb8f0: CCreatureAction_SummonerDie::FinishAction
Original manifest label: `CCreatureAction_SummonerDie / CCreatureAction_SummonerDie::FinishAction`.
Identity: proven; own RTTI class/method slot and both PDB method declaration; real tracked PCreature+8 GetPItem00A01B50; TC bitset+0x20 and map+0x44 LowerBound0040F020; AddMode argument92.
Layout: original RTTI CBaseClass/CBaseObject/CCreatureActionBase chain explicit, plus actual CClass/Thing/Physical/GameObject/Creature chain; shared CBaseObject data header included unchanged under alias. GoUnderground immediate base is CCombatActionBase.  shared CTCBase header included unchanged; own component storage is CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > over CArray/std::vector. Original Components donor+0x48 becomes retail+0x44; Compare/Dirty follow three-pointer data. Both PDB LowerBound template name sets confirm enum and real component base.  generated action/base headers flatten polymorphic inheritance and tracked-pointer APIs; minimal real base/PCreature chain retained. CThing header lacks usable template GetTC and bitset/map APIs, so named projection models actual bitset9 and sorted map. No false-branch pointer initialization invented; N091 requires its TC to exist.

## 0085ae20: std::list::push_back
Original manifest label: `_List_val<CCountedPointer<CSpawnedFuncBase> / _Buynode<CCountedPointer<CSpawnedFuncBase>_>`.
Identity: proven; SetupCombatStrikeEvents00859A70 calls on StrikeEvents+0xBC, PDB list<CCountedPointer<CStrikeEvent>>.
Layout: actual outer classes provide nested naming scopes only; generated CCombatActionBase/CFileInstaller headers lack these nested definitions. Two-word CCountedPointer holds Data+CCPPointerInfo, Count at info+0; retail list sentinel4/node16. Genuine inline allocator/placement copy/link operations.

## 008ebf90: std::list<CCountedPointer<NCreatureMode::CModeHoverBase::CHoverAnim>>::push_back
Original manifest label: `_List_val<CCountedPointer<CSpawnedFuncBase> / _Buynode<CCountedPointer<CSpawnedFuncBase>_>`.
Identity: proven; own CModeHoverBase::UpdateHoverAnim008ea9f0 calls AnimQueue; exact PDB nested CHoverAnim list type. Actual original STLport _List_base/rebound-node proxy, _List_iterator/usercopy and protected _M_insert(iterator,constT&) with original placement copy/link order. True countedpointer two words and original CCPPointerInfo12 RefCount/fastcallDeleteFunc/Data; no nonexistent CCPPointerInfo::Release or truncated count-only object. Outer classes naming scopes only, no layout/sizeof claims. Own storeddeleter00cdee00 consumesECX, matching actual fastcall pointer type.
Layout: genuine original list/node storage and non-POD counted-pointer special members; node16/sentinel4 native.

## 009c6bf0: std::list<CCountedPointer<const CFileInstaller::CFileInfo>>::push_back
Original manifest label: `_List_val<CCountedPointer<CSpawnedFuncBase> / _Buynode<CCountedPointer<CSpawnedFuncBase>_>`.
Identity: proven; own CFileInstaller::AddFilteredFiles009c47a0 and countedtemporary009c6a70; exact PDB const CFileInfo pointer/list. Actual original STLport _List_base/rebound-node proxy, _List_iterator/usercopy and protected _M_insert(iterator,constT&) with original placement copy/link order. True countedpointer two words and original CCPPointerInfo12 RefCount/fastcallDeleteFunc/Data; no nonexistent CCPPointerInfo::Release or truncated count-only object. Outer classes naming scopes only, no layout/sizeof claims. Own storeddeleter00cdee00 consumesECX, matching actual fastcall pointer type.
Layout: genuine original list/node storage and non-POD counted-pointer special members; node16/sentinel4 native.

## 0085c450: NCameraModes::CMoveOnCylinder::OnDie
Original manifest label: `NCameraModes::CMoveOnCylinder / NCameraModes::CMoveOnCylinder::vfunc_22`.
Identity: proven; own camera mode RTTI virtual slot22 and both PDB OnDie declaration; reads CBase::CameraManager+0x40, copies Camera4 to OldCamera0x3C (56 bytes), sets SingleCut0xB0.
Layout: no mode/CCamera header. Shared CGameCameraManager header carries donor container offsets: SingleCut+0xBC instead of retail+0xB0 (vector/list differences); local named projection uses actual copy destinations. CBase header is an unrelated namesake; real scoped base fields proven by same-mode methods.

## 008735b0: NCameraModes::CFirstPerson::OnDie
Original manifest label: `NCameraModes::CFirstPerson / NCameraModes::CFirstPerson::vfunc_22`.
Identity: proven; own camera mode RTTI virtual slot22 and both PDB OnDie declaration; reads CBase::CameraManager+0x40, copies Camera4 to OldCamera0x3C (56 bytes), sets SingleCut0xB0.
Layout: no mode/CCamera header. Shared CGameCameraManager header carries donor container offsets: SingleCut+0xBC instead of retail+0xB0 (vector/list differences); local named projection uses actual copy destinations. CBase header is an unrelated namesake; real scoped base fields proven by same-mode methods.

## 00878b40: NCameraModes::CProjectileWeapon::OnDie
Original manifest label: `NCameraModes::CProjectileWeapon / NCameraModes::CProjectileWeapon::vfunc_22`.
Identity: proven; own camera mode RTTI virtual slot22 and both PDB OnDie declaration; reads CBase::CameraManager+0x40, copies Camera4 to OldCamera0x3C (56 bytes), sets SingleCut0xB0.
Layout: no mode/CCamera header. Shared CGameCameraManager header carries donor container offsets: SingleCut+0xBC instead of retail+0xB0 (vector/list differences); local named projection uses actual copy destinations. CBase header is an unrelated namesake; real scoped base fields proven by same-mode methods.

## 0085ed90: std::vector::resize
Original manifest label: `vector<EWeaponClass / _Umove<EWeaponClass*>`.
Identity: probable; own callee0085ED20 is proven nested-vector resize via InitMapwhoLevels and erase0085EA40. Wrapper has no known caller.
Layout: original STLport protected _Vector_base with three-argument allocator proxy, real allocator/default constructor and base deallocation; CThingMapCell PFirstNode is a CMapwhoNode pointer (PDB structure). Complete active container lifetime retained.
Callee body: actual0085ED20 shrink/grow branches and actual erase/_M_fill_insert APIs are visible, with original const input semantics; no fabricated write or empty lifetime body.

## 00af2200: std::vector::resize
Original manifest label: `vector<EWeaponClass / _Umove<EWeaponClass*>`.
Identity: probable; actual default constructor proven by both PDBs (egor00AB0C30, fablewin02F02ED0), initializes Data/Bank/Index.  callee00AF2170 resize(CParticleEmitterInfo) proven hand953J086 via CParticleDataBank::NotifyBankSize00AF16E0 and ParticleEmitters+0x1E0; actual Data/Bank/Index12; wrapper unreferenced.
Layout: actual PDB class keys; retail three-pointer vector and CArray. No complete element headers. CIVCountedPointeeBase local virtual count view (Count+4, Delete slot1); generated flattened header lacks usable virtual API.

## 00f3fb60: std::vector::resize
Original manifest label: `vector<EWeaponClass / _Umove<EWeaponClass*>`.
Identity: probable; callee00F3FAF0 resize(CArray<unsigned short>) proven hand952H027/erase00F3F7C0 and both PDB names; wrapper unreferenced.
Layout: actual PDB class keys; retail three-pointer vector and CArray. No complete element headers. CIVCountedPointeeBase local virtual count view (Count+4, Delete slot1); generated flattened header lacks usable virtual API.

## 008610d0: CVillageTaskTorch::IsBetterThan
Original manifest label: `CVillageTaskHorn / IsBetterThan`.
Identity: proven; both PDB IsBetterThan(const CVillageTaskTorch*,const CThing&)const API, own CGetBestTaskForThing caller, hand952H091 corrected concrete classes; Torch+8, IsTaskAvailable virtualslot4 and IsCreatureMoreAppropriateForTask00860650.
Layout: generated task headers flatten virtual base/tracked fields and lack methods; local real CVillageTaskBase8 and tracked8 members preserve offsets. CThing position is an external const-reference accessor; no fabricated source body.

## 008611f0: CVillageTaskHorn::IsBetterThan
Original manifest label: `CVillageTaskHorn / IsBetterThan`.
Identity: proven; both PDB IsBetterThan(const CVillageTaskHorn*,const CThing&)const API, own CGetBestTaskForThing caller, hand952H091 corrected concrete classes; PHorn+8, IsTaskAvailable virtualslot4 and IsCreatureMoreAppropriateForTask00860840.
Layout: generated task headers flatten virtual base/tracked fields and lack methods; local real CVillageTaskBase8 and tracked8 members preserve offsets. CThing position is an external const-reference accessor; no fabricated source body.

## 00861310: CVillageTaskPatrol::IsBetterThan
Original manifest label: `CVillageTaskHorn / IsBetterThan`.
Identity: proven; both PDB IsBetterThan(const CVillageTaskPatrol*,const CThing&)const API, own CGetBestTaskForThing caller, hand952H091 corrected concrete classes; PatrolStart+8, IsTaskAvailable virtualslot4 and IsCreatureMoreAppropriateForTask00860AD0.
Layout: generated task headers flatten virtual base/tracked fields and lack methods; local real CVillageTaskBase8 and tracked8 members preserve offsets. CThing position is an external const-reference accessor; no fabricated source body.

## 00865710: std::__copy
Original manifest label: `_global / _Move<CLipSyncPhoneme*,CLipSyncPhoneme*>`.
Identity: proven; own neighboring hit-notification Flash vector helpers and CFlash copyctor00864F80; first-member list<CWeaponTrailEntry>::operator=0076FF90, CreationFrame+4; PDB CFlash namespace and fields.
Layout: no matching nested flash header; shared CCharString data header lacks assignment/destructor APIs, so actual four-byte PStringData view declares real methods. Flash uses actual namespace, node-list4+CreationFrame4; unknown sound value remains explicitly opaque.

## 00a3fcd0: std::__copy
Original manifest label: `_global / _Move<CLipSyncPhoneme*,CLipSyncPhoneme*>`.
Identity: descriptive; own erase00A40CB0/clear00A41360 are unreferenced; first-word CCharString assignment0099EFB0 proven, value word type/owner unproved (hand952H009). Descriptive record exposes opaque second word without claiming CSoundBankLH pointer.
Layout: no matching nested flash header; shared CCharString data header lacks assignment/destructor APIs, so actual four-byte PStringData view declares real methods. Flash uses actual namespace, node-list4+CreationFrame4; unknown sound value remains explicitly opaque.

## 00ae5910: std::__copy
Original manifest label: `_global / _Move<CLipSyncPhoneme*,CLipSyncPhoneme*>`.
Identity: proven; own erase00AE6A20, resize00AE7D10, CParticleProgram clear00AE3381 on ExportNames+0x88; hand952H009 and actual CCharString assignment0099EFB0.
Layout: no matching nested flash header; shared CCharString data header lacks assignment/destructor APIs, so actual four-byte PStringData view declares real methods. Flash uses actual namespace, node-list4+CreationFrame4; unknown sound value remains explicitly opaque.

## 00865d40: CCombatAction_GenericStrikeResponseKnockdownRecoverBase::OnAnimationLoaded
Original manifest label: `CCombatAction_GenericStrikeResponseKnockdownRecoverBase / ?OnAnimationLoaded@CCombatAction_GenericStrikeResponseKnockdownRecoverBase@@UAEXXZ`.
Identity: proven; own RTTI class/vtable hook and both PDB OnAnimationLoaded; Flip+0x115; actual CTCPhysicsBase GetLastRHSet slot72/SetRHSet slot32 and CThingCreatureBase::SetBodyReorienterActive00663490; base hook00692830.
Layout: shared action/physics headers flatten real virtual APIs and tracked pointers; local named projection preserves PCreature8/Physics60/FlipA8 or115. CRightHandedSet actual24-byte Up then Forward, PDB class keys and vector copy/unary operations retained.

## 008ed6e0: CCreatureAction_BeStunned::OnAnimationLoaded
Original manifest label: `CCreatureAction_BeStunned / ?OnAnimationLoaded@CCreatureAction_BeStunned@@UAEXXZ`.
Identity: proven; own RTTI class/vtable hook and both PDB OnAnimationLoaded; Flip+0xa8; actual CTCPhysicsBase GetLastRHSet slot72/SetRHSet slot32 and CThingCreatureBase::SetBodyReorienterActive00663490; base hook00692830.
Layout: shared action/physics headers flatten real virtual APIs and tracked pointers; local named projection preserves PCreature8/Physics60/FlipA8 or115. CRightHandedSet actual24-byte Up then Forward, PDB class keys and vector copy/unary operations retained.

## 008ed7a0: CCreatureAction_Die::OnAnimationLoaded
Original manifest label: `CCreatureAction_Die / ?OnAnimationLoaded@CCreatureAction_Die@@UAEXXZ`.
Identity: proven; own RTTI class/vtable hook and both PDB OnAnimationLoaded; Flip+0xa8; actual CTCPhysicsBase GetLastRHSet slot72/SetRHSet slot32 and CThingCreatureBase::SetBodyReorienterActive00663490; base hook00692830.
Layout: shared action/physics headers flatten real virtual APIs and tracked pointers; local named projection preserves PCreature8/Physics60/FlipA8 or115. CRightHandedSet actual24-byte Up then Forward, PDB class keys and vector copy/unary operations retained.

## 0086b550: std::__make_heap
Original manifest label: `std / std::make_heap_CRandomAppearanceMorph_CTextureMorph`.
Identity: proven; own random appearance map sort helpers push0x86A770 and adjacent signed skeletal-morph sort; PDB BodyPartTextureMorphs/GeneralTextureMorphs CVectorMap<unsignedlong,CTextureMorph> with8-byte NewTexture/GroupID.
Layout: no actual nested record headers; outer classes supply naming scopes only. Ordinary STLport pair user copy; actual unsigned CRC or signed expression enum key. Genuine same-TU adjust_heap/push_heap bodies retained.

## 0086f930: std::__make_heap
Original manifest label: `std / std::make_heap`.
Identity: proven; CCreatureDef transfer expression map chain and actual signed enum comparator0086F450; hand459 corrected old persistence-label propagation.
Layout: no actual nested record headers; outer classes supply naming scopes only. Ordinary STLport pair user copy; actual unsigned CRC or signed expression enum key. Genuine same-TU adjust_heap/push_heap bodies retained.

## 009b3c00: std::__make_heap
Original manifest label: `_global / MakeHeap_Pair12Byte_Build`.
Identity: proven; own SubDefInfoMap pair12 sort/reserve family; allocation0x9B2780/reserve0x9B4D80 <- CParentDefClassBase::InternalLoadBinary0x9AED70 proves CSubDefInfo8 and unsigned CRC key.
Layout: no actual nested record headers; outer classes supply naming scopes only. Ordinary STLport pair user copy; actual unsigned CRC or signed expression enum key. Genuine same-TU adjust_heap/push_heap bodies retained.

## 00883530: NScreenEffect::CScreenEffectPriorityStack::Process
Original manifest label: `Process<std::list<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry,std::allocator<NScreenEffect::CScreenEffectPriorityStack / Process<std::list<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry,std::allocator<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry>_>_>`.
Identity: own caller00880e50 passes CRadialBlurEntry stack and corresponding callback vtable; PDB exact private template and original CStackEntry fields.
Layout: no active shared owner/entry header. Original CStackEntry vptr0,curdurationcount4,durationcount8,fadingout12. Derived parameter bytes are untouched; sizes96/32/60 from PDB and retail STLport correction. Owner fields are unused by this member template.

## 00883640: NScreenEffect::CScreenEffectPriorityStack::Process
Original manifest label: `Process<std::list<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry,std::allocator<NScreenEffect::CScreenEffectPriorityStack / Process<std::list<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry,std::allocator<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry>_>_>`.
Identity: own caller00880e50 passes CDisplacementMonochromeEffectColourEntry stack and corresponding callback vtable; PDB exact private template and original CStackEntry fields.
Layout: no active shared owner/entry header. Original CStackEntry vptr0,curdurationcount4,durationcount8,fadingout12. Derived parameter bytes are untouched; sizes96/32/60 from PDB and retail STLport correction. Owner fields are unused by this member template.

## 00883750: NScreenEffect::CScreenEffectPriorityStack::Process
Original manifest label: `Process<std::list<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry,std::allocator<NScreenEffect::CScreenEffectPriorityStack / Process<std::list<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry,std::allocator<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry>_>_>`.
Identity: own caller00880e50 passes CScreenFilterEntry stack and corresponding callback vtable; PDB exact private template and original CStackEntry fields.
Layout: no active shared owner/entry header. Original CStackEntry vptr0,curdurationcount4,durationcount8,fadingout12. Derived parameter bytes are untouched; sizes96/32/60 from PDB and retail STLport correction. Owner fields are unused by this member template.

## 00884e90: CEnvironmentSpellEffectInterface::SetRadialBlurParams
Original manifest label: `CEnvironmentSpellEffectInterface / SetRadialBlurParams`.
Identity: original PDB nested struct and setter name; exact field offsets4/132 identify this overload.
Layout: no active shared owner/parameter headers. PDB named fields agree with retail copies: current+4/+52/+100, previous+132/+180/+228; sizes48/32/32.

## 00884f60: CEnvironmentSpellEffectInterface::SetRadialBlurPositionParams
Original manifest label: `CEnvironmentSpellEffectInterface / SetRadialBlurPositionParams`.
Identity: original PDB nested struct and setter name; exact field offsets52/180 identify this overload.
Layout: no active shared owner/parameter headers. PDB named fields agree with retail copies: current+4/+52/+100, previous+132/+180/+228; sizes48/32/32.

## 00884fe0: CEnvironmentSpellEffectInterface::SetScreenFilterParams
Original manifest label: `CEnvironmentSpellEffectInterface / SetRadialBlurPositionParams`.
Identity: original PDB nested struct and setter name; exact field offsets100/228 identify this overload.
Layout: no active shared owner/parameter headers. PDB named fields agree with retail copies: current+4/+52/+100, previous+132/+180/+228; sizes48/32/32.

## 00889010: std::vector::_M_insert_overflow
Original manifest label: `std::vector / std::vector::Reallocate_ulong_triplet`.
Identity: proven; Own SetThemeWeight00886d70 constructs ThemeID/Weight/LockWeight; PDB nested struct and CArray<type>; helper885ca0/885ce0.
Layout:12-byte POD element, real STLport vector12. Nested outer names provide only type scope, no outer fields touched.

## 008f6bb0: std::vector::_M_insert_overflow
Original manifest label: `_global / std_vector_insert_single<unsigned_long>`.
Identity: proven; Own resize008f6e30/fill_insert008f6c90, height-map owner and shared12-byte cell header; helpers008f6800/008f6860.
Layout:12-byte POD element, real STLport vector12. Nested outer names provide only type scope, no outer fields touched.

## 00ae6c70: std::vector::_M_insert_overflow
Original manifest label: `_gapscan / sub_00ae6c70`.
Identity: descriptive; Only caller00ae7290 is an unreferenced12-byte POD push_back. No use establishes record semantics. Three copied words and12-byte stride proven, record identity unresolved; helpers00ae4ea0/00ae4ee0 separate from other families.
Layout:12-byte POD element, real STLport vector12. Nested outer names provide only type scope, no outer fields touched.

## 0088ebc0: CGameScriptInterface::EntitySetAsToAddToComboMultiplierWhenHit
Original manifest label: `CGameScriptInterface / ?EntitySetAsToAddToComboMultiplierWhenHit@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z`.
Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee008389a0 is CThingGameObject::SetAddToComboMultiplierOnHit.
Layout: CThing active header is quarantined; ThingTypeFlags low byte+0x6c is read. All other fields are outside this call boundary.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CThingGameObject is used only as an external method receiver at base offset0; no derived fields accessed.
Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.

## 0088ebf0: CGameScriptInterface::EntitySetAsToAddToStatChangesWhenHit
Original manifest label: `CGameScriptInterface / ?EntitySetAsToAddToStatChangesWhenHit@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z`.
Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee008389c0 is CThingGameObject::SetAsGivingHeroStatChangesOnBeingHit.
Layout: CThing active header is quarantined; ThingTypeFlags low byte+0x6c is read. All other fields are outside this call boundary.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CThingGameObject is used only as an external method receiver at base offset0; no derived fields accessed.
Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.

## 0088f500: CGameScriptInterface::EntityBeginLoadingAnimation
Original manifest label: `CGameScriptInterface / ?EntityBeginLoadingAnimation@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z`.
Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee006631c0 is CThingCreatureBase::PreloadAnimation.
Layout: CThing active header is quarantined; ThingTypeFlags low byte+0x6c is read. All other fields are outside this call boundary.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CThingCreatureBase is used only as an external method receiver at base offset0; no derived fields accessed.
Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.

## 0088f9f0: CGameScriptInterface::SetIsThingForcePushable
Original manifest label: `CGameScriptInterface / ?SetIsThingForcePushable@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z`.
Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee007e3d50 is CTCWillResponse::SetThingIsForcePushable.
Layout: CThing active header is quarantined; ThingTypeFlags low byte+0x6c is read. All other fields are outside this call boundary.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CTCWillResponse is a static-method name scope only; no object or its shared header fields are used.
Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.

## 0088fa20: CGameScriptInterface::SetIsThingLightningable
Original manifest label: `CGameScriptInterface / ?SetIsThingLightningable@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z`.
Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee007e3f40 is CTCWillResponse::SetThingIsLightningable.
Layout: CThing active header is quarantined; ThingTypeFlags low byte+0x6c is read. All other fields are outside this call boundary.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CTCWillResponse is a static-method name scope only; no object or its shared header fields are used.
Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.

## 0088fa50: CGameScriptInterface::SetIsThingEpicSpellable
Original manifest label: `CGameScriptInterface / ?SetIsThingEpicSpellable@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z`.
Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee007e4330 is CTCWillResponse::SetThingIsEpicSpellable.
Layout: CThing active header is quarantined; ThingTypeFlags low byte+0x6c is read. All other fields are outside this call boundary.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CTCWillResponse is a static-method name scope only; no object or its shared header fields are used.
Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.

## 00895650: CGameScriptInterface::EntitySetAsMirroringHeroEnemyRelationsWhileFollowing
Original manifest label: `CGameScriptInterface / ?EntitySetAsMirroringHeroEnemyRelationsWhileFollowing@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z`.
Identity: PDB exact virtual const API; component kind73 and original CTCEnemy::EnableFollowersEnemyProxy field agree with retail.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: script owner header flattened; genuine virtual API required, owner fields untouched.

## 008a05a0: CGameScriptInterface::EntitySetTargetable
Original manifest label: `CGameScriptInterface / ?EntitySetTargetable@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z`.
Identity: PDB exact virtual const API; component kind9 and original CTCTargeted::Targetable field agree with retail.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: script owner header flattened; genuine virtual API required, owner fields untouched.

## 008a0660: CGameScriptInterface::EntitySetTargetingValidTargetWithoutLOS
Original manifest label: `CGameScriptInterface / ?EntitySetTargetingValidTargetWithoutLOS@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z`.
Identity: PDB exact virtual const API; component kind9 and original CTCTargeted::ValidTargetWithoutLOS field agree with retail.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: script owner header flattened; genuine virtual API required, owner fields untouched.

## 00895b60: CGameScriptInterface::HeroPlayOracleMinigame
Original manifest label: `CGameScriptInterface / CGameScriptInterface::HeroPlayOracleMinigame`.
Identity: own virtual const PDB API, component263 and CTCOracleMinigame::PlayGame callee007e5d20. Player accessor00487dc0 retains established GetPPlayerCharacter binding, distinct from duplicate GetPControlledCreature00487dd0.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared player/player-manager/oracle headers supply external receiver layout only. No instance fields are read on them; genuine methods restore original symbols.
Layout: no active CTCSpecialAbilities header; external method receiver only, no instance fields accessed.
Layout: genuine script vptr+0, PlayerManager+0x14 from shared header; no other owner fields read. CThing Dead bit0 at+0x91 is retail, donor+0xa1.

## 0089d690: CGameScriptInterface::CancelAbilitiesForCutscene
Original manifest label: `CGameScriptInterface / CGameScriptInterface::CancelAbilitiesForCutscene`.
Identity: own virtual const PDB API, component167 and CTCSpecialAbilities::CancelAbilitiesForCutscene callee0072bc60. Player accessor00487dc0 retains established GetPPlayerCharacter binding, distinct from duplicate GetPControlledCreature00487dd0.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared player/player-manager/oracle headers supply external receiver layout only. No instance fields are read on them; genuine methods restore original symbols.
Layout: no active CTCSpecialAbilities header; external method receiver only, no instance fields accessed.
Layout: genuine script vptr+0, PlayerManager+0x14 from shared header; no other owner fields read. CThing Dead bit0 at+0x91 is retail, donor+0xa1.

## 0089d6f0: CGameScriptInterface::ResumeAbilitiesForCutscene
Original manifest label: `CGameScriptInterface / CGameScriptInterface::ResumeAbilitiesForCutscene`.
Identity: own virtual const PDB API, component167 and CTCSpecialAbilities::ResumeAbilitiesForCutscene callee0072bec0. Player accessor00487dc0 retains established GetPPlayerCharacter binding, distinct from duplicate GetPControlledCreature00487dd0.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared player/player-manager/oracle headers supply external receiver layout only. No instance fields are read on them; genuine methods restore original symbols.
Layout: no active CTCSpecialAbilities header; external method receiver only, no instance fields accessed.
Layout: genuine script vptr+0, PlayerManager+0x14 from shared header; no other owner fields read. CThing Dead bit0 at+0x91 is retail, donor+0xa1.

## 00896660: CGameScriptInterface::CloseChest
Original manifest label: `CGameScriptInterface / ?CloseChest@CGameScriptInterface@@UBEXABVCScriptThing@@@Z`.
Identity: PDB exact virtual const signature; own interface kind60 and external callee007506c0 CTCChest::Close.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: only external method receivers; no active chest/mode-manager headers, no derived fields read. Base offsets agree with CTCBase.
Layout: shared script header flattened; genuine virtual API below, no owner fields touched.

## 00896730: CGameScriptInterface::DisplayLockedChestMessage
Original manifest label: `CGameScriptInterface / ?DisplayLockedChestMessage@CGameScriptInterface@@UBEXABVCScriptThing@@@Z`.
Identity: PDB exact virtual const signature; own interface kind60 and external callee00750110 CTCChest::DisplayLockedMessage.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: only external method receivers; no active chest/mode-manager headers, no derived fields read. Base offsets agree with CTCBase.
Layout: shared script header flattened; genuine virtual API below, no owner fields touched.

## 0089ddf0: CGameScriptInterface::EntityResetCreatureMode
Original manifest label: `CGameScriptInterface / ?EntityResetCreatureMode@CGameScriptInterface@@UBEXABVCScriptThing@@@Z`.
Identity: PDB exact virtual const signature; own interface kind49 and external callee0073b0d0 CTCCreatureModeManager::ResetModes.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: only external method receivers; no active chest/mode-manager headers, no derived fields read. Base offsets agree with CTCBase.
Layout: shared script header flattened; genuine virtual API below, no owner fields touched.

## 0089c000: CGameScriptInterface::EntitySetThingAsEnemyOfThing
Original manifest label: `CGameScriptInterface / ?EntitySetThingAsEnemyOfThing@CGameScriptInterface@@UBEXABVCScriptThing@@0@Z`.
Identity: PDB exact virtual const signature; own interface kind73 and external callee0076d1b0 CTCEnemy::AddEnemyThing.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: only external method receivers; no active chest/mode-manager headers, no derived fields read. Base offsets agree with CTCBase.
Layout: shared script header flattened; genuine virtual API below, no owner fields touched.

## 0089c080: CGameScriptInterface::EntityUnsetThingAsEnemyOfThing
Original manifest label: `CGameScriptInterface / ?EntityUnsetThingAsEnemyOfThing@CGameScriptInterface@@UBEXABVCScriptThing@@0@Z`.
Identity: PDB exact virtual const signature; own interface kind73 and external callee0076cc00 CTCEnemy::RemoveEnemyThing.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: only external method receivers; no active chest/mode-manager headers, no derived fields read. Base offsets agree with CTCBase.
Layout: shared script header flattened; genuine virtual API below, no owner fields touched.

## 008a7ee0: CGameScriptInterface::EntityUnsetThingAsAllyOfThing
Original manifest label: `CGameScriptInterface / ?EntityUnsetThingAsAllyOfThing@CGameScriptInterface@@UBEXABVCScriptThing@@0@Z`.
Identity: PDB exact virtual const signature; own interface kind73 and external callee0076c850 CTCEnemy::RemoveAllyThing.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: only external method receivers; no active chest/mode-manager headers, no derived fields read. Base offsets agree with CTCBase.
Layout: shared script header flattened; genuine virtual API below, no owner fields touched.

## 0089c330: CGameScriptInterface::EntitySetOpinionDeedTypeEnabled
Original manifest label: `CGameScriptInterface / CGameScriptInterface::EntitySetOpinionDeedTypeEnabled`.
Identity: PDB exact enum/virtual const API; original DeedMask shared-header member. Retail is a12-byte vector of byte bools, not a packed-bit specialization (indexed byte store).
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: use shared opinion header opaque container storage via its actual STLport vector<bool> type; offsets+0x24,+0x30,+0x3c are confirmed directly. Script owner fields unused, real virtual declaration restores API.

## 0089c380: CGameScriptInterface::EntitySetOpinionAttitudeEnabled
Original manifest label: `CGameScriptInterface / CGameScriptInterface::EntitySetOpinionAttitudeEnabled`.
Identity: PDB exact enum/virtual const API; original AttitudeMask shared-header member. Retail is a12-byte vector of byte bools, not a packed-bit specialization (indexed byte store).
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: use shared opinion header opaque container storage via its actual STLport vector<bool> type; offsets+0x24,+0x30,+0x3c are confirmed directly. Script owner fields unused, real virtual declaration restores API.

## 0089c3d0: CGameScriptInterface::EntitySetOpinionReactionEnabled
Original manifest label: `CGameScriptInterface / CGameScriptInterface::EntitySetOpinionReactionEnabled`.
Identity: PDB exact enum/virtual const API; original ReactionMask shared-header member. Retail is a12-byte vector of byte bools, not a packed-bit specialization (indexed byte store).
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: use shared opinion header opaque container storage via its actual STLport vector<bool> type; offsets+0x24,+0x30,+0x3c are confirmed directly. Script owner fields unused, real virtual declaration restores API.

## 0089d420: CGameScriptInterface::EntitySetAsHavingBoundHands
Original manifest label: `CGameScriptInterface / ?EntitySetAsHavingBoundHands@CGameScriptInterface@@UBEXAAVCScriptThing@@_N@Z`.
Identity: original PDB virtual const signature; direct mode/generator calls independently named.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CThing ThingTypeFlags+0x6c, Dead bit0+0x91 from retail; intermediate fields are not used.
Layout: no active mode-manager header; nested mode is polymorphic with GetType slot1, no other fields accessed.
Layout: shared script header retained as evidence; genuine virtual declaration, owner fields untouched.

## 0089d4c0: CGameScriptInterface::EntitySetAsScared
Original manifest label: `CGameScriptInterface / ?EntitySetAsScared@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z`.
Identity: original PDB virtual const signature; direct mode/generator calls independently named.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CThing ThingTypeFlags+0x6c, Dead bit0+0x91 from retail; intermediate fields are not used.
Layout: no active mode-manager header; nested mode is polymorphic with GetType slot1, no other fields accessed.
Layout: shared script header retained as evidence; genuine virtual declaration, owner fields untouched.

## 0089d560: CGameScriptInterface::EntitySetAsDrunk
Original manifest label: `CGameScriptInterface / ?EntitySetAsDrunk@CGameScriptInterface@@UBEXAAVCScriptThing@@_N@Z`.
Identity: original PDB virtual const signature; direct mode/generator calls independently named.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CThing ThingTypeFlags+0x6c, Dead bit0+0x91 from retail; intermediate fields are not used.
Layout: no active mode-manager header; nested mode is polymorphic with GetType slot1, no other fields accessed.
Layout: shared script header retained as evidence; genuine virtual declaration, owner fields untouched.

## 0089e9c0: CGameScriptInterface::CreatureGeneratorIsDepleted
Original manifest label: `CGameScriptInterface / ?CreatureGeneratorIsDepleted@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z`.
Identity: original PDB virtual const signature; direct mode/generator calls independently named.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CThing ThingTypeFlags+0x6c, Dead bit0+0x91 from retail; intermediate fields are not used.
Layout: no active mode-manager header; nested mode is polymorphic with GetType slot1, no other fields accessed.
Layout: shared script header retained as evidence; genuine virtual declaration, owner fields untouched.

## 0089eb80: CGameScriptInterface::CreatureGeneratorGetNumGeneratedCreatures
Original manifest label: `CGameScriptInterface / ?CreatureGeneratorGetNumGeneratedCreatures@CGameScriptInterface@@UBEJABVCScriptThing@@@Z`.
Identity: original PDB virtual const signature; direct mode/generator calls independently named.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CThing ThingTypeFlags+0x6c, Dead bit0+0x91 from retail; intermediate fields are not used.
Layout: no active mode-manager header; nested mode is polymorphic with GetType slot1, no other fields accessed.
Layout: shared script header retained as evidence; genuine virtual declaration, owner fields untouched.

## 0089ebf0: CGameScriptInterface::CreatureGeneratorAreAllCreaturesAlive
Original manifest label: `CGameScriptInterface / ?CreatureGeneratorAreAllCreaturesAlive@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z`.
Identity: original PDB virtual const signature; direct mode/generator calls independently named.
Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
Layout: CThing ThingTypeFlags+0x6c, Dead bit0+0x91 from retail; intermediate fields are not used.
Layout: no active mode-manager header; nested mode is polymorphic with GetType slot1, no other fields accessed.
Layout: shared script header retained as evidence; genuine virtual declaration, owner fields untouched.

## 008a9e90: CGameScriptInterface::SetGossip
Original manifest label: `CGameScriptInterface / ?SetGossip@CGameScriptInterface@@UAEXVCCharString@@0J@Z`.
Identity: original virtual API and FableWin static CTCAIScratchpad overload; category013bae44.
Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
Layout: static CTCAIScratchpad category only; no object used.

## 008aa0f0: CGameScriptInterface::SetGossipVillages
Original manifest label: `CGameScriptInterface / ?SetGossipVillages@CGameScriptInterface@@UAEXVCCharString@@0J@Z`.
Identity: original virtual API and FableWin static CTCAIScratchpad overload; category013bae50.
Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
Layout: static CTCAIScratchpad category only; no object used.

## 008aa350: CGameScriptInterface::SetGossipFactions
Original manifest label: `CGameScriptInterface / ?SetGossipFactions@CGameScriptInterface@@UAEXVCCharString@@0J@Z`.
Identity: original virtual API and FableWin static CTCAIScratchpad overload; category013bae5c.
Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
Layout: static CTCAIScratchpad category only; no object used.

## 008a9f00: CGameScriptInterface::GetGossip
Original manifest label: `CGameScriptInterface / CGameScriptInterface::GetGossip`.
Identity: original PDB exact virtual signature and CTCAIScratchpad static API; own 013bae44 identifies category.
Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
Layout: CTCAIScratchpad is used solely for proven static members, with no instance layout.

## 008aa160: CGameScriptInterface::GetGossipVillages
Original manifest label: `CGameScriptInterface / CGameScriptInterface::GetGossipVillages`.
Identity: original PDB exact virtual signature and CTCAIScratchpad static API; own 013bae50 identifies category.
Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
Layout: CTCAIScratchpad is used solely for proven static members, with no instance layout.

## 008aa3c0: CGameScriptInterface::GetGossipFactions
Original manifest label: `CGameScriptInterface / CGameScriptInterface::GetGossipFactions`.
Identity: original PDB exact virtual signature and CTCAIScratchpad static API; own 013bae5c identifies category.
Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
Layout: CTCAIScratchpad is used solely for proven static members, with no instance layout.

## 008aa090: CGameScriptInterface::AddGossip
Original manifest label: `CGameScriptInterface / CGameScriptInterface::AddGossip`.
Identity: original PDB exact virtual signature and CTCAIScratchpad static API; own 008ae5e0 identifies category.
Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
Layout: CTCAIScratchpad is used solely for proven static members, with no instance layout.

## 008aa2f0: CGameScriptInterface::AddGossipVillages
Original manifest label: `CGameScriptInterface / CGameScriptInterface::AddGossipVillages`.
Identity: original PDB exact virtual signature and CTCAIScratchpad static API; own 008ae810 identifies category.
Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
Layout: CTCAIScratchpad is used solely for proven static members, with no instance layout.

## 008aa550: CGameScriptInterface::AddGossipFactions
Original manifest label: `CGameScriptInterface / CGameScriptInterface::AddGossipFactions`.
Identity: original PDB exact virtual signature and CTCAIScratchpad static API; own 008aea40 identifies category.
Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
Layout: CTCAIScratchpad is used solely for proven static members, with no instance layout.

## 008ac660: CThingSearchTools::GetAllThings
Original manifest label: `GetAllThings<CThingMarker,std::vector<CThing*,std / GetAllThings<CThingMarker,std::vector<CThing*,std::allocator<CThing*>_>,CThingFilter_HasScriptName>`.
Identity: original PDB CThingObject specialization and actual list<CThing*> output; own mask0x807 and filter006a7be0 prove instance. Propagated CThingMarker/vector label is wrong.
Layout: real CClass prefix from shared header; CThing header quarantined. ThingTypeFlags+0x6c, state byte+0x91 from retail; original scanner flag meanings retained.

## 008ac700: CThingSearchTools::GetAllThings
Original manifest label: `GetAllThings<CThingMarker,std::vector<CThing*,std / GetAllThings<CThingMarker,std::vector<CThing*,std::allocator<CThing*>_>,CThingFilter_HasScriptName>`.
Identity: original PDB CThingObject specialization and actual list<CThing*> output; own mask0x807 and filter006a80a0 prove instance. Propagated CThingMarker/vector label is wrong.
Layout: real CClass prefix from shared header; CThing header quarantined. ThingTypeFlags+0x6c, state byte+0x91 from retail; original scanner flag meanings retained.

## 0094b490: CThingSearchTools::GetAllThings
Original manifest label: `GetAllThings<CThingMarker,std::vector<CThing*,std / GetAllThings<CThingMarker,std::vector<CThing*,std::allocator<CThing*>_>,CThingFilter_HasScriptName>`.
Identity: original PDB CThingObject specialization and actual list<CThingObject*> output; own mask0x807 and filter006a7e30 prove instance. Propagated CThingMarker/vector label is wrong.
Layout: real CClass prefix from shared header; CThing header quarantined. ThingTypeFlags+0x6c, state byte+0x91 from retail; original scanner flag meanings retained.

## 008adfe0: CCreatureAction_ForcePushInto::CCreatureAction_ForcePushInto
Original manifest label: `CCreatureAction_SheatheWeapons / CCreatureAction_SheatheWeapons`.
Identity: own full RTTI 012783d4 and original PDB constructor; base00858030 takes CThingCreatureBase&,long,const CSoundPair&.
Layout: CCombatActionBase header is quarantined. Retail base is0x114 bytes; derived bool+0x114, consistent with donor0x12c after container correction. Opaque base data is not accessed here. The genuine polymorphic model is required for constructor vtable stores; the available derived header is flattened.

## 008cc370: CCreatureAction_SheatheWeapons::CCreatureAction_SheatheWeapons
Original manifest label: `CCreatureAction_SheatheWeapons / CCreatureAction_SheatheWeapons`.
Identity: own full RTTI 01282cec and original PDB constructor; base00858030 takes CThingCreatureBase&,long,const CSoundPair&.
Layout: CCombatActionBase header is quarantined. Retail base is0x114 bytes; derived bool+0x114, consistent with donor0x12c after container correction. Opaque base data is not accessed here. The genuine polymorphic model is required for constructor vtable stores; the available derived header is flattened.

## 008e4800: CCreatureAction_ForcePushOutOf::CCreatureAction_ForcePushOutOf
Original manifest label: `CCreatureAction_SheatheWeapons / CCreatureAction_SheatheWeapons`.
Identity: own full RTTI 01286874 and original PDB constructor; base00858030 takes CThingCreatureBase&,long,const CSoundPair&.
Layout: CCombatActionBase header is quarantined. Retail base is0x114 bytes; derived bool+0x114, consistent with donor0x12c after container correction. Opaque base data is not accessed here. The genuine polymorphic model is required for constructor vtable stores; the available derived header is flattened.

## 008ae5e0: CTCAIScratchpad::AddGossip
Original manifest label: `_global / CGameScriptInterface_AddRumourToCategoryEx`.
Identity: FableWin static fastcall two-string overload; own static map013bae44 and string-vector overflow00433530.
Layout: CCharString has one pointer; STLport vector12/map12; no active headers for these types. CTCAIScratchpad is solely a static-member scope.

## 008ae810: CTCAIScratchpad::AddGossipVillages
Original manifest label: `_global / CGameScriptInterface_AddGossipVillageEx`.
Identity: FableWin static fastcall two-string overload; own static map013bae50 and string-vector overflow00433530.
Layout: CCharString has one pointer; STLport vector12/map12; no active headers for these types. CTCAIScratchpad is solely a static-member scope.

## 008aea40: CTCAIScratchpad::AddGossipFactions
Original manifest label: `_global / AddGossipFactionToCategory_Impl`.
Identity: FableWin static fastcall two-string overload; own static map013bae5c and string-vector overflow00433530.
Layout: CCharString has one pointer; STLport vector12/map12; no active headers for these types. CTCAIScratchpad is solely a static-member scope.

## 008ae660: CTCAIScratchpad::AddGossip
Original manifest label: `_global / CGameScriptInterface_FindOrCreateRumourCategory`.
Identity: PDB single-string static overload; own map013bae44 and find008ac200/operator[]007aebe0/vector assignment00432ee9.
Layout: real STLport map12/vector12, CCharString one pointer. Only static CTCAIScratchpad members are used.

## 008ae890: CTCAIScratchpad::AddGossipVillages
Original manifest label: `CGameScriptInterface / CGameScriptInterface::_AddGossipVillages_impl`.
Identity: PDB single-string static overload; own map013bae50 and find008ac200/operator[]007aebe0/vector assignment00432ee9.
Layout: real STLport map12/vector12, CCharString one pointer. Only static CTCAIScratchpad members are used.

## 008aeac0: CTCAIScratchpad::AddGossipFactions
Original manifest label: `CGameScriptInterface / CGameScriptInterface::_AddGossipFactions_impl`.
Identity: PDB single-string static overload; own map013bae5c and find008ac200/operator[]007aebe0/vector assignment00432ee9.
Layout: real STLport map12/vector12, CCharString one pointer. Only static CTCAIScratchpad members are used.

## 008af020: CTCEntityEvents::GetEvent
Original manifest label: `GetEvent<NEntityEvents::CEventHitBy_const_,NEntityEvents / GetEvent<NEntityEvents::CEventHitBy_const_,NEntityEvents::CEventFilter_None>`.
Identity: original mutable event template instance in PDB; full retail RTTI01272390 GetType slot1=00811d00 returns29, proving CEventHitThing rather than propagated CEventHitBy.
Layout: original counted block RefCount+0, DeleteFunc+4, Data+8; wrapper Data+0,Info+4. No active shared headers exist.
Layout: original CEventBase virtual destructor slot0/GetType slot1,WorldFrame+4. Unused event-specific data does not enter this lookup.
Layout: shared header provides Events+0x0c; interpret its opaque storage as the original4-byte STLport list, not an invented container.

## 008af0a0: CTCEntityEvents::GetEvent
Original manifest label: `GetEvent<NEntityEvents::CEventHitBy_const_,NEntityEvents / GetEvent<NEntityEvents::CEventHitBy_const_,NEntityEvents::CEventFilter_None>`.
Identity: original mutable event template instance in PDB; full retail RTTI01274240 GetType slot1=0084b070 returns3, proving CEventExpressionPerformedTo rather than propagated CEventHitBy.
Layout: original counted block RefCount+0, DeleteFunc+4, Data+8; wrapper Data+0,Info+4. No active shared headers exist.
Layout: original CEventBase virtual destructor slot0/GetType slot1,WorldFrame+4. Unused event-specific data does not enter this lookup.
Layout: shared header provides Events+0x0c; interpret its opaque storage as the original4-byte STLport list, not an invented container.

## 0091d0a0: CTCEntityEvents::GetEvent
Original manifest label: `GetEvent<NEntityEvents::CEventHitBy_const_,NEntityEvents / GetEvent<NEntityEvents::CEventHitBy_const_,NEntityEvents::CEventFilter_None>`.
Identity: original mutable event template instance in PDB; full retail RTTI0126d7d4 GetType slot1=007b2910 returns4, proving CEventApologyReceived rather than propagated CEventHitBy.
Layout: original counted block RefCount+0, DeleteFunc+4, Data+8; wrapper Data+0,Info+4. No active shared headers exist.
Layout: original CEventBase virtual destructor slot0/GetType slot1,WorldFrame+4. Unused event-specific data does not enter this lookup.
Layout: shared header provides Events+0x0c; interpret its opaque storage as the original4-byte STLport list, not an invented container.

## 008b0020: _global::CUnprovenCombatNameFactory_008b0020
Original manifest label: `_gapscan / sub_008b0020`.
Identity: descriptive; own literalCOMBAT_SEQUENCE_FODDER_MAIN_ATTACK and constructor0099EBF0; unreferenced callable body, no RTTI method association; caller role and original name unproven. By-value fastcall string factory is a descriptive ABI contract.
Layout: no shared CCharString header; original four-byte PStringData wrapper and constructor signature reused.

## 008b0440: _global::CUnprovenCombatNameFactory_008b0440
Original manifest label: `_gapscan / sub_008b0440`.
Identity: descriptive; own literalCOMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT and constructor0099EBF0; unreferenced callable body, no RTTI method association; caller role and original name unproven. By-value fastcall string factory is a descriptive ABI contract.
Layout: no shared CCharString header; original four-byte PStringData wrapper and constructor signature reused.

## 008b04c0: _global::CUnprovenCombatNameFactory_008b04c0
Original manifest label: `_gapscan / sub_008b04c0`.
Identity: descriptive; own literalCOMBAT_SEQUENCE_TWO_STRIKE and constructor0099EBF0; unreferenced callable body, no RTTI method association; caller role and original name unproven. By-value fastcall string factory is a descriptive ABI contract.
Layout: no shared CCharString header; original four-byte PStringData wrapper and constructor signature reused.

## 008b69f0: CCreatureAction_GoUnderground::Initialise
Original manifest label: `CCreatureAction_GoUnderground / ?Initialise@CCreatureAction_GoUnderground@@UAEXXZ`.
Identity: proven; own RTTI 0127A894 slot12 and both PDB Initialise declaration; real base hook00857520; actual CThingAICreature::SuspendAI(bool,void*)00833A10, PDB exact name and own Suspender-set+1CC/flag+1C8 body, not propagated SetSuspendAnimation labels.
Layout: shared CBaseObject header included unchanged under data-layout alias; local CBaseClass/CBaseObject preserve original RTTI virtual inheritance and IntelligentPointer+4. No active CThing header; original type discriminator at+0x10 is unnamed in donor layout, FastCastType is descriptive. Generated creature/action headers flatten virtual bases and omit required APIs; local real action/base and tracked-pointer chain preserve PCreature+8. No derived object size or unmodeled members are used.

## 008b6a60: CCreatureAction_RiseUp::Initialise
Original manifest label: `CCreatureAction_RiseUp / ?Initialise@CCreatureAction_RiseUp@@UAEXXZ`.
Identity: proven; own RTTI 01268C9C slot12 and both PDB Initialise declaration; real base hook00857520; actual CThingAICreature::SuspendAI(bool,void*)00833A10, PDB exact name and own Suspender-set+1CC/flag+1C8 body, not propagated SetSuspendAnimation labels.
Layout: shared CBaseObject header included unchanged under data-layout alias; local CBaseClass/CBaseObject preserve original RTTI virtual inheritance and IntelligentPointer+4. No active CThing header; original type discriminator at+0x10 is unnamed in donor layout, FastCastType is descriptive. Generated creature/action headers flatten virtual bases and omit required APIs; local real action/base and tracked-pointer chain preserve PCreature+8. No derived object size or unmodeled members are used.

## 008ed770: CCreatureAction_Die::Initialise
Original manifest label: `CCreatureAction_Die / ?Initialise@CCreatureAction_Die@@UAEXXZ`.
Identity: proven; own RTTI 01287FC4 slot12 and both PDB Initialise declaration; real base hook00694D10; actual CThingAICreature::SuspendAI(bool,void*)00833A10, PDB exact name and own Suspender-set+1CC/flag+1C8 body, not propagated SetSuspendAnimation labels.
Layout: shared CBaseObject header included unchanged under data-layout alias; local CBaseClass/CBaseObject preserve original RTTI virtual inheritance and IntelligentPointer+4. No active CThing header; original type discriminator at+0x10 is unnamed in donor layout, FastCastType is descriptive. Generated creature/action headers flatten virtual bases and omit required APIs; local real action/base and tracked-pointer chain preserve PCreature+8. No derived object size or unmodeled members are used.

## 008b8190: CCreatureAction_LeapLong::CCreatureAction_LeapLong
Original manifest label: `CCreatureAction_LeapLong / CCreatureAction_LeapLong`.
Identity: proven; own RTTI 0127A9EC and both PDB two-creature-reference ctor; actual CCombatActionBase00857F30 and Landed donor300->retail276 after container contraction; exact original RTTI CCreatureActionBase->CBaseObject->CBaseClass chain.
Layout: generated action/base headers flatten polymorphic bases, omit constructor API and have donor-derived member offsets; local actual real base chain preserves action168/combat276/Landed276. CThing tags only provide reference conversions, never object fields or size. Shared CSoundPair header absent.

## 0096bf10: CCreatureAction_WhisperSommersaultForward::CCreatureAction_WhisperSommersaultForward
Original manifest label: `CCreatureAction_LeapLong / CCreatureAction_LeapLong`.
Identity: proven; own RTTI 01296754 and both PDB two-creature-reference ctor; actual CCombatActionBase00857F30 and Landed donor300->retail276 after container contraction; exact original RTTI CCreatureActionBase->CBaseObject->CBaseClass chain.
Layout: generated action/base headers flatten polymorphic bases, omit constructor API and have donor-derived member offsets; local actual real base chain preserves action168/combat276/Landed276. CThing tags only provide reference conversions, never object fields or size. Shared CSoundPair header absent.

## 0096bf90: CCreatureAction_WhisperSommersaultBackward::CCreatureAction_WhisperSommersaultBackward
Original manifest label: `CCreatureAction_LeapLong / CCreatureAction_LeapLong`.
Identity: proven; own RTTI 0129689C and both PDB two-creature-reference ctor; actual CCombatActionBase00857F30 and Landed donor300->retail276 after container contraction; exact original RTTI CCreatureActionBase->CBaseObject->CBaseClass chain.
Layout: generated action/base headers flatten polymorphic bases, omit constructor API and have donor-derived member offsets; local actual real base chain preserves action168/combat276/Landed276. CThing tags only provide reference conversions, never object fields or size. Shared CSoundPair header absent.

## 008c1900: CCombatAction_Guildmaster_HealFollowersInto::Initialise
Original manifest label: `CCombatAction_Guildmaster_HealFollowersInto / CCombatAction_Guildmaster_HealFollowersInto::Initialise`.
Identity: proven; own RTTI/PDB Initialise hook; POWER_UP_INTO SetCombatAnimation008574D0 then CCombatActionBase::Initialise00857520; original enum/CTCBase component map mutable0040F020; real nested counted mode pointer ctor00718A00/AddMode0073ACC0 exact PDB; new mode008DB030(CThing&,5.f), actual allocation144.
Layout: shared CTCBase included unchanged. Generated action and mode headers flatten virtual base chains and omit actual constructors/helpers; local original CBaseClass/CBaseObject/action chain has PCreature+8, Thing primary hierarchy Components68. New mode uses genuine global stand-base104/filter12/timer4/two STLport vectors12, exact tracked element types; constructor external.

## 008c1a10: CCombatAction_Guildmaster_ShieldFollowersInto::Initialise
Original manifest label: `CCombatAction_Guildmaster_ShieldFollowersInto / CCombatAction_Guildmaster_ShieldFollowersInto::Initialise`.
Identity: proven; own RTTI/PDB Initialise hook; POWER_UP_INTO SetCombatAnimation008574D0 then CCombatActionBase::Initialise00857520; original enum/CTCBase component map mutable0040F020; real nested counted mode pointer ctor00718A00/AddMode0073ACC0 exact PDB; new mode008DB0E0(CThing&,5.f), actual allocation144.
Layout: shared CTCBase included unchanged. Generated action and mode headers flatten virtual base chains and omit actual constructors/helpers; local original CBaseClass/CBaseObject/action chain has PCreature+8, Thing primary hierarchy Components68. New mode uses genuine global stand-base104/filter12/timer4/two STLport vectors12, exact tracked element types; constructor external.

## 008c1b20: CCombatAction_Guildmaster_SpeedUpFollowersInto::Initialise
Original manifest label: `CCombatAction_Guildmaster_SpeedUpFollowersInto / CCombatAction_Guildmaster_SpeedUpFollowersInto::Initialise`.
Identity: proven; own RTTI/PDB Initialise hook; POWER_UP_INTO SetCombatAnimation008574D0 then CCombatActionBase::Initialise00857520; original enum/CTCBase component map mutable0040F020; real nested counted mode pointer ctor00718A00/AddMode0073ACC0 exact PDB; new mode008DB190(CThing&,5.f), actual allocation144.
Layout: shared CTCBase included unchanged. Generated action and mode headers flatten virtual base chains and omit actual constructors/helpers; local original CBaseClass/CBaseObject/action chain has PCreature+8, Thing primary hierarchy Components68. New mode uses genuine global stand-base104/filter12/timer4/two STLport vectors12, exact tracked element types; constructor external.

## 008c3810: CCreatureAction_JackGetUp::CCreatureAction_JackGetUp
Original manifest label: `CCreatureAction_GoUnderground / CCreatureAction_GoUnderground`.
Identity: proven; own RTTI 0128039C and exact PDB ctor; base00858030; protected GetSoundPair008561A0 and SetCombatAnimation008574D0 exact PDB names; own animation literal ST_GET_UP_FROM_FALL and SWORD_SWING sound name.
Layout: generated action headers flatten original CBaseClass/CBaseObject/action inheritance and omit constructor/helper APIs. Local genuine chain preserves action168/combat276; unused CThing tags supply reference conversions only. Four-byte CCharString/eight-byte CSoundPair have no shared headers.

## 009771f0: CCreatureAction_JackDeathInto::CCreatureAction_JackDeathInto
Original manifest label: `CCreatureAction_GoUnderground / CCreatureAction_GoUnderground`.
Identity: proven; own RTTI 01297DFC and exact PDB ctor; base00858030; protected GetSoundPair008561A0 and SetCombatAnimation008574D0 exact PDB names; own animation literal DEATH_INTO and SWORD_SWING sound name.
Layout: generated action headers flatten original CBaseClass/CBaseObject/action inheritance and omit constructor/helper APIs. Local genuine chain preserves action168/combat276; unused CThing tags supply reference conversions only. Four-byte CCharString/eight-byte CSoundPair have no shared headers.

## 00977320: CCreatureAction_JackFall::CCreatureAction_JackFall
Original manifest label: `CCreatureAction_GoUnderground / CCreatureAction_GoUnderground`.
Identity: proven; own RTTI 012980A4 and exact PDB ctor; base00858030; protected GetSoundPair008561A0 and SetCombatAnimation008574D0 exact PDB names; own animation literal ST_FALL_TO_GROUND and SWORD_SWING sound name.
Layout: generated action headers flatten original CBaseClass/CBaseObject/action inheritance and omit constructor/helper APIs. Local genuine chain preserves action168/combat276; unused CThing tags supply reference conversions only. Four-byte CCharString/eight-byte CSoundPair have no shared headers.

## 008c5410: CCreatureAction_WaspQueenDiveAttack::CCreatureAction_WaspQueenDiveAttack
Original manifest label: `CCreatureAction_BriarRoseShotFarInto / CCreatureAction_BriarRoseShotFarInto`.
Identity: proven; own RTTI 01281304 and exact PDB ctor; base00857F30; protected GetSoundPair008561A0 and SetCombatAnimation008574D0 exact PDB names; own animation literal DIVE_ATTACK and null sound name.
Layout: generated action headers flatten original CBaseClass/CBaseObject/action inheritance and omit constructor/helper APIs. Local genuine chain preserves action168/combat276; unused CThing tags supply reference conversions only. Four-byte CCharString/eight-byte CSoundPair have no shared headers.

## 008ca680: CCreatureAction_NymphShot::CCreatureAction_NymphShot
Original manifest label: `CCreatureAction_BriarRoseShotFarInto / CCreatureAction_BriarRoseShotFarInto`.
Identity: proven; own RTTI 012824B4 and exact PDB ctor; base00857F30; protected GetSoundPair008561A0 and SetCombatAnimation008574D0 exact PDB names; own animation literal STRIKE_MEDIUM_FRONT and null sound name.
Layout: generated action headers flatten original CBaseClass/CBaseObject/action inheritance and omit constructor/helper APIs. Local genuine chain preserves action168/combat276; unused CThing tags supply reference conversions only. Four-byte CCharString/eight-byte CSoundPair have no shared headers.

## 008ca700: CCreatureAction_NymphIdleAfterShot::CCreatureAction_NymphIdleAfterShot
Original manifest label: `CCreatureAction_BriarRoseShotFarInto / CCreatureAction_BriarRoseShotFarInto`.
Identity: proven; own RTTI 012825FC and exact PDB ctor; base00857F30; protected GetSoundPair008561A0 and SetCombatAnimation008574D0 exact PDB names; own animation literal IDLE_AFTER_SHOT and null sound name.
Layout: generated action headers flatten original CBaseClass/CBaseObject/action inheritance and omit constructor/helper APIs. Local genuine chain preserves action168/combat276; unused CThing tags supply reference conversions only. Four-byte CCharString/eight-byte CSoundPair have no shared headers.

## 008cc650: CCreatureAction_SummonerUnsheatheStrike::Clone
Original manifest label: `CCreatureAction_SummonerUnsheatheStrike / CCreatureAction_SummonerUnsheatheStrike::Clone`.
Identity: proven; own RTTI vtable slot11 Clone plus exact PDB const API returning CCreatureActionBase*; first copied vtable is CCreatureAction_UnsheatheWeapons; field WeaponType at0x114 from own transfer and PDB original; genuine implicit copy through CCombatActionBase helper0062F3B0.
Layout: generated headers flatten virtual base inheritance and omit copy/Clone APIs. Real original CBaseClass/CBaseObject/CCreatureActionBase/CCombatActionBase chain retained; base168/combat276, exact allocation0x118. The intermediate class member is original enum/bool, copied implicitly.

## 0094b6d0: CCreatureAction_PutDownTankard::Clone
Original manifest label: `CCreatureAction_PutDownTankard / CCreatureAction_PutDownTankard::Clone`.
Identity: proven; own RTTI vtable slot11 Clone plus exact PDB const API returning CCreatureActionBase*; first copied vtable is CCreatureAction_PutDownThingAtTable; field Mode at0xa8 from own transfer and PDB original; genuine implicit copy through CCreatureActionBase helper0062EBC0.
Layout: generated headers flatten virtual base inheritance and omit copy/Clone APIs. Real original CBaseClass/CBaseObject/CCreatureActionBase/CCombatActionBase chain retained; base168/combat276, exact allocation0xac. The intermediate class member is original enum/bool, copied implicitly.

## 00968100: CCreatureAction_PutDownJug::Clone
Original manifest label: `CCreatureAction_PutDownJug / CCreatureAction_PutDownJug::Clone`.
Identity: proven; own RTTI vtable slot11 Clone plus exact PDB const API returning CCreatureActionBase*; first copied vtable is CCreatureAction_PutDownThingAtTable; field Mode at0xa8 from own transfer and PDB original; genuine implicit copy through CCreatureActionBase helper0062EBC0.
Layout: generated headers flatten virtual base inheritance and omit copy/Clone APIs. Real original CBaseClass/CBaseObject/CCreatureActionBase/CCombatActionBase chain retained; base168/combat276, exact allocation0xac. The intermediate class member is original enum/bool, copied implicitly.

## 008cc6a0: CCreatureAction_SummonerSheatheWeapon::Clone
Original manifest label: `CCreatureAction_SummonerSheatheWeapon / CCreatureAction_SummonerSheatheWeapon::Clone`.
Identity: proven; own RTTI vtable slot11 Clone plus exact PDB const API returning CCreatureActionBase*; first copied vtable is CCreatureAction_SheatheWeapons; field PlayedSound at0x114 from own transfer and PDB original; genuine implicit copy through CCombatActionBase helper0062F3B0.
Layout: generated headers flatten virtual base inheritance and omit copy/Clone APIs. Real original CBaseClass/CBaseObject/CCreatureActionBase/CCombatActionBase chain retained; base168/combat276, exact allocation0x118. The intermediate class member is original enum/bool, copied implicitly.

## 00923be0: CCreatureAction_AddFoodToTable::Clone
Original manifest label: `CCreatureAction_AddFoodToTable / CCreatureAction_AddFoodToTable::Clone`.
Identity: proven; own RTTI vtable slot11 Clone plus exact PDB const API returning CCreatureActionBase*; first copied vtable is CCreatureAction_PutDownThingAtTable; field Mode at0xa8 from own transfer and PDB original; genuine implicit copy through CCreatureActionBase helper0062EBC0.
Layout: generated headers flatten virtual base inheritance and omit copy/Clone APIs. Real original CBaseClass/CBaseObject/CCreatureActionBase/CCombatActionBase chain retained; base168/combat276, exact allocation0xac. The intermediate class member is original enum/bool, copied implicitly.

## 0096d500: CCreatureAction_StopEatingAtTable::Clone
Original manifest label: `CCreatureAction_StopEatingAtTable / CCreatureAction_StopEatingAtTable::Clone`.
Identity: proven; own RTTI vtable slot11 Clone plus exact PDB const API returning CCreatureActionBase*; first copied vtable is CCreatureAction_PutDownThingAtTable; field Mode at0xa8 from own transfer and PDB original; genuine implicit copy through CCreatureActionBase helper0062EBC0.
Layout: generated headers flatten virtual base inheritance and omit copy/Clone APIs. Real original CBaseClass/CBaseObject/CCreatureActionBase/CCombatActionBase chain retained; base168/combat276, exact allocation0xac. The intermediate class member is original enum/bool, copied implicitly.

## 008db030: NCreatureMode::CModeGuildmasterPowerUpHeal::CModeGuildmasterPowerUpHeal
Original manifest label: `CModeGuildmasterPowerUpHeal / CModeGuildmasterPowerUpHeal`.
Identity: proven; own RTTI 012685BC and exact PDB NCreatureMode ctor(CThing&,float); original GLOBAL CModeStandBase ctor00861E00 and nested CTCCreatureModeManager::CModeBase; CAnimFilterOfGroup vtable01267568/GroupCRC+112; POWER_UP literal, real CCRC::Calc004014A0; own FPS global01375550 descriptive existing name.
Layout: generated mode header flattens original virtual base chain and uses donor vectors; local actual nested mode-base28/global stand-base104, Filter12/Timer4, vectors12 each. No CAnimFilterOfGroup/CAnimFilterBase headers; real virtual filter plus named Group/GroupCRC. Only CCharStringData first two fields accessed, remaining counted storage external.

## 008db0e0: NCreatureMode::CModeGuildmasterPowerUpShield::CModeGuildmasterPowerUpShield
Original manifest label: `CModeGuildmasterPowerUpHeal / CModeGuildmasterPowerUpHeal`.
Identity: proven; own RTTI 012685F0 and exact PDB NCreatureMode ctor(CThing&,float); original GLOBAL CModeStandBase ctor00861E00 and nested CTCCreatureModeManager::CModeBase; CAnimFilterOfGroup vtable01267568/GroupCRC+112; POWER_UP literal, real CCRC::Calc004014A0; own FPS global01375550 descriptive existing name.
Layout: generated mode header flattens original virtual base chain and uses donor vectors; local actual nested mode-base28/global stand-base104, Filter12/Timer4, vectors12 each. No CAnimFilterOfGroup/CAnimFilterBase headers; real virtual filter plus named Group/GroupCRC. Only CCharStringData first two fields accessed, remaining counted storage external.

## 008db190: NCreatureMode::CModeGuildmasterPowerUpSpeed::CModeGuildmasterPowerUpSpeed
Original manifest label: `CModeGuildmasterPowerUpHeal / CModeGuildmasterPowerUpHeal`.
Identity: proven; own RTTI 01268624 and exact PDB NCreatureMode ctor(CThing&,float); original GLOBAL CModeStandBase ctor00861E00 and nested CTCCreatureModeManager::CModeBase; CAnimFilterOfGroup vtable01267568/GroupCRC+112; POWER_UP literal, real CCRC::Calc004014A0; own FPS global01375550 descriptive existing name.
Layout: generated mode header flattens original virtual base chain and uses donor vectors; local actual nested mode-base28/global stand-base104, Filter12/Timer4, vectors12 each. No CAnimFilterOfGroup/CAnimFilterBase headers; real virtual filter plus named Group/GroupCRC. Only CCharStringData first two fields accessed, remaining counted storage external.

## 008e8c50: CCreatureAction_BriarRoseInvulnerableTeleport::CCreatureAction_BriarRoseInvulnerableTeleport
Original manifest label: `CCreatureAction_BriarRoseForcePush / CCreatureAction_BriarRoseForcePush`.
Identity: proven; own RTTI 01287A4C and both PDB two-creature-reference constructor; base00858030(Creature&,long,constCSoundPair&), PEnemy donor300->retail276, actual tracked-pointer ctor00A01B10/SetPItem00A01B90 and vtable0123E7D8.
Layout: generated headers flatten real CBaseClass/CBaseObject/action inheritance and tracked-pointer API; exact original base chain and combat276/PEnemy8 view retained. CThing tags supply reference and pointer conversions only, no fields or object size used. No shared CSoundPair header.

## 00957210: CCreatureAction_BriarRoseIntoInvulnerable::CCreatureAction_BriarRoseIntoInvulnerable
Original manifest label: `CCreatureAction_BriarRoseForcePush / CCreatureAction_BriarRoseForcePush`.
Identity: proven; own RTTI 01294B8C and both PDB two-creature-reference constructor; base00858030(Creature&,long,constCSoundPair&), PEnemy donor300->retail276, actual tracked-pointer ctor00A01B10/SetPItem00A01B90 and vtable0123E7D8.
Layout: generated headers flatten real CBaseClass/CBaseObject/action inheritance and tracked-pointer API; exact original base chain and combat276/PEnemy8 view retained. CThing tags supply reference and pointer conversions only, no fields or object size used. No shared CSoundPair header.

## 009572a0: CCreatureAction_BriarRoseForcePush::CCreatureAction_BriarRoseForcePush
Original manifest label: `CCreatureAction_BriarRoseForcePush / CCreatureAction_BriarRoseForcePush`.
Identity: proven; own RTTI 01294CD4 and both PDB two-creature-reference constructor; base00858030(Creature&,long,constCSoundPair&), PEnemy donor300->retail276, actual tracked-pointer ctor00A01B10/SetPItem00A01B90 and vtable0123E7D8.
Layout: generated headers flatten real CBaseClass/CBaseObject/action inheritance and tracked-pointer API; exact original base chain and combat276/PEnemy8 view retained. CThing tags supply reference and pointer conversions only, no fields or object size used. No shared CSoundPair header.

## 009000bc: CAIStateGroup_WanderAroundShops::CAIStateGroup_WanderAroundShops
Original manifest label: `CAIStateGroup_BalverineCornered / CAIStateGroup_BalverineCornered`.
Identity: proven; own RTTI 0128BE4C and exact PDB default ctor; original CAIStateGroup_WorkBase<CAIStateGroupBase> chain; tracked members Shop/PrettyThing at20/28 and own template vtables.
Layout: generated state-group headers flatten CBaseClass polymorphism and tracked members, so local original inheritance and typed members retain real virtual construction. Base20 unchanged; Jack base76 after vector contraction, unaccessed base tail modeled as an external-constructor prefix. Stage1 primitives intentionally uninitialized. No CThing/creature object fields or sizes used.

## 009004e0: CAIStateGroup_BalverineCornered::CAIStateGroup_BalverineCornered
Original manifest label: `CAIStateGroup_BalverineCornered / CAIStateGroup_BalverineCornered`.
Identity: proven; own RTTI 0128C0BC and exact PDB default ctor; original CAIStateGroupBase chain; tracked members PTarget/PLastJumpTarget at20/28 and own template vtables.
Layout: generated state-group headers flatten CBaseClass polymorphism and tracked members, so local original inheritance and typed members retain real virtual construction. Base20 unchanged; Jack base76 after vector contraction, unaccessed base tail modeled as an external-constructor prefix. Stage1 primitives intentionally uninitialized. No CThing/creature object fields or sizes used.

## 00903216: CAIStateGroup_JackOfBladesStage1::CAIStateGroup_JackOfBladesStage1
Original manifest label: `CAIStateGroup_BalverineCornered / CAIStateGroup_BalverineCornered`.
Identity: proven; own RTTI 0128D6B4 and exact PDB default ctor; original CAIStateGroup_JackOfBladesBase chain; tracked members PObstruction/PShieldThing and own template vtables; Stage1 member donor92/100->retail84/92 after two base vectors shrink8.
Layout: generated state-group headers flatten CBaseClass polymorphism and tracked members, so local original inheritance and typed members retain real virtual construction. Base20 unchanged; Jack base76 after vector contraction, unaccessed base tail modeled as an external-constructor prefix. Stage1 primitives intentionally uninitialized. No CThing/creature object fields or sizes used.

## 009074c0: CCreatureAction_FodderIdle::CCreatureAction_FodderIdle
Original manifest label: `CCreatureAction_DragonRoar / CCreatureAction_DragonRoar`.
Identity: proven; own RTTI 0128DFA4 and PDB exact ctor; actual CCombatActionBase00857F30 and protected static fastcall GetSoundPair008561A0 exact PDB spelling; null CCharString constructor0099EBF0/destructor0099EAE0.
Layout: generated headers flatten CBaseClass/CBaseObject/action hierarchy and omit constructor/protected helper APIs. Local original chain preserves action168/combat276. CThing tags only supply reference conversion; no object fields/sizeof used. Actual CCharString4/CSoundPair8; shared headers absent.

## 0097cff0: CCreatureAction_VillagerAdvance::CCreatureAction_VillagerAdvance
Original manifest label: `CCreatureAction_DragonRoar / CCreatureAction_DragonRoar`.
Identity: proven; own RTTI 012990BC and PDB exact ctor; actual CCombatActionBase00857F30 and protected static fastcall GetSoundPair008561A0 exact PDB spelling; null CCharString constructor0099EBF0/destructor0099EAE0.
Layout: generated headers flatten CBaseClass/CBaseObject/action hierarchy and omit constructor/protected helper APIs. Local original chain preserves action168/combat276. CThing tags only supply reference conversion; no object fields/sizeof used. Actual CCharString4/CSoundPair8; shared headers absent.

## 0097d050: CCreatureAction_VillagerBackOff::CCreatureAction_VillagerBackOff
Original manifest label: `CCreatureAction_DragonRoar / CCreatureAction_DragonRoar`.
Identity: proven; own RTTI 01299204 and PDB exact ctor; actual CCombatActionBase00857F30 and protected static fastcall GetSoundPair008561A0 exact PDB spelling; null CCharString constructor0099EBF0/destructor0099EAE0.
Layout: generated headers flatten CBaseClass/CBaseObject/action hierarchy and omit constructor/protected helper APIs. Local original chain preserves action168/combat276. CThing tags only supply reference conversion; no object fields/sizeof used. Actual CCharString4/CSoundPair8; shared headers absent.

## 0090c100: std::for_each
Original manifest label: `_Find_if<std::_List_unchecked_iterator<std::_List_val<CMiniMapDisplay::CMarkerInfo,std::allocator<CMiniMapDisplay::CMarkerInfo>_>_>,CMiniMapDisplay / _Find_if<std::_List_unchecked_iterator<std::_List_val<CMiniMapDisplay::CMarkerInfo,std::allocator<CMiniMapDisplay::CMarkerInfo>_>_>,CMiniMapDisplay::CIsMarkerBoundToThing>`.
Identity: proven; own callee0090C060 exact PDB CAddFreeBenchToList::operator()(constCIntelligentPointer<CThing>&)const; own caller building-search helper, node payload+8, original PDB for_each const-list-iterator instantiation and BenchList vector<CThing*>*.
Layout: original STLport _List_iterator<CIntelligentPointer<CThing>,_Const_traits<...>> with user copy constructor; functor is original class with vector pointer and user constructor, returned through memory. CThing/ThingObject are only unused pointer/template tags. No flat iterator stand-in.

## 00938e90: std::for_each
Original manifest label: `_Find_if<std::_List_unchecked_iterator<std::_List_val<CMiniMapDisplay::CMarkerInfo,std::allocator<CMiniMapDisplay::CMarkerInfo>_>_>,CMiniMapDisplay / _Find_if<std::_List_unchecked_iterator<std::_List_val<CMiniMapDisplay::CMarkerInfo,std::allocator<CMiniMapDisplay::CMarkerInfo>_>_>,CMiniMapDisplay::CIsMarkerBoundToThing>`.
Identity: proven; own callee00938DB0 exact PDB CAddFreeSeatToList::operator()(constCIntelligentPointer<CThing>&)const; own caller building-search helper, node payload+8, original PDB for_each const-list-iterator instantiation and SeatList vector<CThingObject*>*.
Layout: original STLport _List_iterator<CIntelligentPointer<CThing>,_Const_traits<...>> with user copy constructor; functor is original class with vector pointer and user constructor, returned through memory. CThing/ThingObject are only unused pointer/template tags. No flat iterator stand-in.

## 00943990: std::for_each
Original manifest label: `_Find_if<std::_List_unchecked_iterator<std::_List_val<CMiniMapDisplay::CMarkerInfo,std::allocator<CMiniMapDisplay::CMarkerInfo>_>_>,CMiniMapDisplay / _Find_if<std::_List_unchecked_iterator<std::_List_val<CMiniMapDisplay::CMarkerInfo,std::allocator<CMiniMapDisplay::CMarkerInfo>_>_>,CMiniMapDisplay::CIsMarkerBoundToThing>`.
Identity: proven; own callee009438F0 exact PDB CAddOccupiedSeatToList::operator()(constCIntelligentPointer<CThing>&)const; own caller building-search helper, node payload+8, original PDB for_each const-list-iterator instantiation and SeatList vector<CThingObject*>*.
Layout: original STLport _List_iterator<CIntelligentPointer<CThing>,_Const_traits<...>> with user copy constructor; functor is original class with vector pointer and user constructor, returned through memory. CThing/ThingObject are only unused pointer/template tags. No flat iterator stand-in.

## 0090e3d0: CMessageEventManager::FindMostRecentMessage
Original manifest label: `CMessageEventManager / FindMostRecentMessage<CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>,CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>_>_>`.
Identity: proven; own ordered callee chain IsOfType006AF1C0/CThingFilter_IsValidGossipee/CThingFilter_In2DRange and exact original PDB const FindMostRecentMessage API returning constCMessageEvent*; own event.POriginator+72 and AI mask0x1F; real CFilter_Bind3 and pointer-owning ThingSearchFilter layouts.
Container: original protected _List_base/rebound-node allocator proxy; original two-traits iterator stores its node directly, mutable iterator copy constructor is converting only for const_iterator (GOTCHAS1231/1383).
Layout: no shared manager/event headers. Original CMessageEvent->CBaseObject->CBaseClass with Type8/POriginator72/PTarget80; other event fields untouched. Genuine STLport list<CMessageEvent>4 at manager+4. Real Thing primary hierarchy and typeflags108, original filter/composite templates; adapters evaluate actual originator getters and predicates.

## 0090ebe0: CMessageEventManager::FindMostRecentMessage
Original manifest label: `CMessageEventManager / FindMostRecentMessage<CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>,CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>_>_>`.
Identity: proven; own ordered callee chain IsOfType006AF1C0/CThingFilter_In2DRange/CThingFilter_IsValidGossipee and exact original PDB const FindMostRecentMessage API returning constCMessageEvent*; own event.POriginator+72 and AI mask0x1F; real CFilter_Bind3 and pointer-owning ThingSearchFilter layouts.
Container: original protected _List_base/rebound-node allocator proxy; original two-traits iterator stores its node directly, mutable iterator copy constructor is converting only for const_iterator (GOTCHAS1231/1383).
Layout: no shared manager/event headers. Original CMessageEvent->CBaseObject->CBaseClass with Type8/POriginator72/PTarget80; other event fields untouched. Genuine STLport list<CMessageEvent>4 at manager+4. Real Thing primary hierarchy and typeflags108, original filter/composite templates; adapters evaluate actual originator getters and predicates.

## 00934e50: CMessageEventManager::FindMostRecentMessage
Original manifest label: `CMessageEventManager / FindMostRecentMessage<CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>,CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>_>_>`.
Identity: proven; own ordered callee chain IsOfType006AF1C0/CThingFilter_IsNot/CThingFilter_In2DRange and exact original PDB const FindMostRecentMessage API returning constCMessageEvent*; own event.POriginator+72 and AI mask0x1F; real CFilter_Bind3 and pointer-owning ThingSearchFilter layouts.
Container: original protected _List_base/rebound-node allocator proxy; original two-traits iterator stores its node directly, mutable iterator copy constructor is converting only for const_iterator (GOTCHAS1231/1383).
Layout: no shared manager/event headers. Original CMessageEvent->CBaseObject->CBaseClass with Type8/POriginator72/PTarget80; other event fields untouched. Genuine STLport list<CMessageEvent>4 at manager+4. Real Thing primary hierarchy and typeflags108, original filter/composite templates; adapters evaluate actual originator getters and predicates.

## 0090f770: CAIStateGroup_MinionSitAtWork::GetPOwnerObject
Original manifest label: `CAIStateGroup_MinionSitAtWork / CAIStateGroup_MinionSitAtWork::GetPOwnerObject`.
Identity: proven; own RTTI 0128B5D4 slot16 and exact protected virtual PDB bool(CThing**) API; PBrain+4->Creature+24->WorkBuilding+0x170; actual inherited CBaseIntelligentPointer::GetPItem00A01B50.
Layout: shared CAIBrain/CThingAICreature headers included unchanged; typed tracked-pointer projection over named opaque building field. Shared state-group headers flatten CBaseClass polymorphism and omit protected virtual API, so local actual CAIStateGroupBase/UsesSeat/MinionSitOnSeatBase chain models only inherited PBrain+4. Building primary CThing base is+0 from RTTI, no building fields or size used.

## 0090f7a0: CAIStateGroup_MinionSitAtSchool::GetPOwnerObject
Original manifest label: `CAIStateGroup_MinionSitAtSchool / CAIStateGroup_MinionSitAtSchool::GetPOwnerObject`.
Identity: proven; own RTTI 0128B674 slot16 and exact protected virtual PDB bool(CThing**) API; PBrain+4->Creature+24->WorkBuilding+0x170; actual inherited CBaseIntelligentPointer::GetPItem00A01B50.
Visible callee: original landed00A01B50 body with private const ClearData and mutable private PData, unchanged actual object/refcount semantics.
Layout: shared CAIBrain/CThingAICreature headers included unchanged; typed tracked-pointer projection over named opaque building field. Shared state-group headers flatten CBaseClass polymorphism and omit protected virtual API, so local actual CAIStateGroupBase/UsesSeat/MinionSitOnSeatBase chain models only inherited PBrain+4. Building primary CThing base is+0 from RTTI, no building fields or size used.
Original inline accessor: FableWin name set GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ; inherited header Creature+0x18. Layout alias restores the omitted method without editing header data.

## 00938ef0: CAIStateGroup_MinionSitAtHome::GetPOwnerObject
Original manifest label: `CAIStateGroup_MinionSitAtHome / CAIStateGroup_MinionSitAtHome::GetPOwnerObject`.
Identity: proven; own RTTI 0128B6C4 slot16 and exact protected virtual PDB bool(CThing**) API; PBrain+4->Creature+24->HomeBuilding+0x168; actual inherited CBaseIntelligentPointer::GetPItem00A01B50.
Visible callee: original landed00A01B50 body with private const ClearData and mutable private PData, unchanged actual object/refcount semantics.
Layout: shared CAIBrain/CThingAICreature headers included unchanged; typed tracked-pointer projection over named opaque building field. Shared state-group headers flatten CBaseClass polymorphism and omit protected virtual API, so local actual CAIStateGroupBase/UsesSeat/MinionSitOnSeatBase chain models only inherited PBrain+4. Building primary CThing base is+0 from RTTI, no building fields or size used.
Original inline accessor: FableWin name set GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ; inherited header Creature+0x18. Layout alias restores the omitted method without editing header data.

## 009107b0: CThingSearchTools::GetPBestThing
Original manifest label: `CThingSearchTools / GetPBestThing<CThingFilter_HasDefGlobalIndex,CThingCompare_Nearest,CThing>`.
Identity: proven; own filter/comparator006A70B0/004A8D80 and exact PDB GetPBestThing<CThingFilter_HasDefGlobalIndex,CThingCompare_Nearest,CThing> const API; PeekClassList0049C770 exact protected const list<CClass*> reference API; own scanflags+0x91 and zero CThing type mask.
Layout: shared CThingSearchTools data header included unchanged under alias; actual protected method declared on owner. No active CThing header; real primary CBaseClass/CBaseObject/CClass/Thing chain with ThingTypeFlags108 and StateBits145. Genuine STLport list<CClass*> const iterator, original filter/comparator class keys and fields.

## 00922320: CThingSearchTools::GetPBestThing
Original manifest label: `CThingSearchTools / GetPBestThing<CThingFilter_HasDefGlobalIndex,CThingCompare_Nearest,CThing>`.
Identity: proven; own filter/comparator006A7E30/004A8D80 and exact PDB GetPBestThing<CThingFilter_HasScriptName,CThingCompare_Nearest,CThing> const API; PeekClassList0049C770 exact protected const list<CClass*> reference API; own scanflags+0x91 and zero CThing type mask.
Layout: shared CThingSearchTools data header included unchanged under alias; actual protected method declared on owner. No active CThing header; real primary CBaseClass/CBaseObject/CClass/Thing chain with ThingTypeFlags108 and StateBits145. Genuine STLport list<CClass*> const iterator, original filter/comparator class keys and fields.

## 00935040: CThingSearchTools::GetPBestThing
Original manifest label: `CThingSearchTools / GetPBestThing<CThingFilter_HasDefGlobalIndex,CThingCompare_Nearest,CThing>`.
Identity: proven; own filter/comparator006A80A0/00934F20 and exact PDB GetPBestThing<CThingFilter_HasTC,CThingCompare_NearestGroupByMember,CThing> const API; PeekClassList0049C770 exact protected const list<CClass*> reference API; own scanflags+0x91 and zero CThing type mask.
Layout: shared CThingSearchTools data header included unchanged under alias; actual protected method declared on owner. No active CThing header; real primary CBaseClass/CBaseObject/CClass/Thing chain with ThingTypeFlags108 and StateBits145. Genuine STLport list<CClass*> const iterator, original filter/comparator class keys and fields.

## 00916e30: CIsFreeLogPile::operator
Original manifest label: `CIsFreeBonfire / operator()`.
Identity: proven; own callerGetPBestThing wrapper and exact PDB predicate API; object GetProperties virtualslot48 and own propertybit22; const enum/CTCBase map LowerBound004365B0; CTCUserActionPointBase::IsActionPointFree007FEBF0 exact PDB const API.
Layout: shared CTCBase included unchanged. No active CThing/Object header; actual CBaseClass/CBaseObject/CClass/Thing/Physical/GameObject/Object primary hierarchy and UsingComponents32/Components68 preserved. GameObject header lacks real virtual methods/inheritance; only required object slot48 is typed. Genuine CVectorMap enum/CTCBase storage through CArray/STLport vector, not an empty base surrogate.

## 00916e90: CIsFreeBonfire::operator
Original manifest label: `CIsFreeBonfire / operator()`.
Identity: proven; own callerGetPBestThing wrapper and exact PDB predicate API; object GetProperties virtualslot48 and own propertybit14; const enum/CTCBase map LowerBound004365B0; CTCUserActionPointBase::IsActionPointFree007FEBF0 exact PDB const API.
Layout: shared CTCBase included unchanged. No active CThing/Object header; actual CBaseClass/CBaseObject/CClass/Thing/Physical/GameObject/Object primary hierarchy and UsingComponents32/Components68 preserved. GameObject header lacks real virtual methods/inheritance; only required object slot48 is typed. Genuine CVectorMap enum/CTCBase storage through CArray/STLport vector, not an empty base surrogate.

## 0093b5e0: CIsCampfireWithSpace::operator
Original manifest label: `CIsFreeBonfire / operator()`.
Identity: proven; own callerGetPNearCampfire0093AC61 and original stateless CIsCampfireWithSpace versus16-byte near-campfire predicate; object GetProperties virtualslot48 and own propertybit14; const enum/CTCBase map LowerBound004365B0; CTCUserActionPointBase::IsActionPointFree007FEBF0 exact PDB const API.
Layout: shared CTCBase included unchanged. No active CThing/Object header; actual CBaseClass/CBaseObject/CClass/Thing/Physical/GameObject/Object primary hierarchy and UsingComponents32/Components68 preserved. GameObject header lacks real virtual methods/inheritance; only required object slot48 is typed. Genuine CVectorMap enum/CTCBase storage through CArray/STLport vector, not an empty base surrogate.

## 00918230: CAIStateGroup_GazeOutside::CAIStateGroup_GazeOutside
Original manifest label: `_global / CAIStateGroup_Gaze_ctor`.
Identity: proven; own RTTI 0128FE34 and exact PDB default ctor; actual base CAIStateGroup_Gaze ctor; Chance/MinJourney+28/+32 and Primary/SecondaryMarkerDef+40/+44 from PDB; own primary literal GAZE_OUT_OF_BUILDING_MARKER, secondary GAZE_OUT_OF_BUILDING_MARKER; real CGameDefinitionManager::Get0044C6B0 and CDefinitionManager::GetDefGlobalIndexFromName009AD410 exact PDB const API.
Layout: CDefinitionManager shared data header included unchanged under alias; original CGameDefinitionManager primary base+0. No manager fields or size accessed. Generated state-group headers flatten CBaseClass virtual chain and tracked member, so local original Gaze48 inheritance/fields retained. Constructor leaves MaxDistance to base default.

## 009182d0: CAIStateGroup_GazeAtWork::CAIStateGroup_GazeAtWork
Original manifest label: `CAIStateGroup_GazeAtWork / CAIStateGroup_GazeAtWork`.
Identity: proven; own RTTI 0128FEC4 and exact PDB default ctor; actual base CAIStateGroup_Gaze ctor; Chance/MinJourney+28/+32 and Primary/SecondaryMarkerDef+40/+44 from PDB; own primary literal GAZE_AT_WORK_MARKER, secondary GAZE_OUT_OF_BUILDING_MARKER; real CGameDefinitionManager::Get0044C6B0 and CDefinitionManager::GetDefGlobalIndexFromName009AD410 exact PDB const API.
Layout: CDefinitionManager shared data header included unchanged under alias; original CGameDefinitionManager primary base+0. No manager fields or size accessed. Generated state-group headers flatten CBaseClass virtual chain and tracked member, so local original Gaze48 inheritance/fields retained. Constructor leaves MaxDistance to base default.

## 00927c00: CAIStateGroup_ShopkeeperWatchPlayer::CAIStateGroup_ShopkeeperWatchPlayer
Original manifest label: `_global / CAIStateGroup_GazeAtWork_ctor`.
Identity: proven; own RTTI 01291784 and exact PDB default ctor; actual base CAIStateGroup_GazeAtWork ctor; Chance/MinJourney+28/+32 and Primary/SecondaryMarkerDef+40/+44 from PDB; own primary literal GAZE_AT_WORK_MARKER, secondary GAZE_OUT_OF_BUILDING_MARKER; real CGameDefinitionManager::Get0044C6B0 and CDefinitionManager::GetDefGlobalIndexFromName009AD410 exact PDB const API.
Layout: CDefinitionManager shared data header included unchanged under alias; original CGameDefinitionManager primary base+0. No manager fields or size accessed. Generated state-group headers flatten CBaseClass virtual chain and tracked member, so local original Gaze48 inheritance/fields retained. Constructor leaves MaxDistance to base default.

## 00918350: CAIStateGroup_Gaze::InitStateGroup
Original manifest label: `CAIStateGroup_Gaze / ?InitStateGroup@CAIStateGroup_Gaze@@MAEXABVCWorld@@AAVCThingAICreature@@@Z`.
Identity: proven; own RTTI InitStateGroup hook and both PDB exact protected virtual API; GazeMarker original member+20; exact CIntelligentPointer<CThingGameObject> temporary vtable and real base constructor/GetPItem/SetPItem/destructor calls.
Layout: generated state-group headers flatten CBaseClass polymorphism and tracked fields, omit InitStateGroup API; local real state-group base20 and tracked8 retained. Other original base members are an untouched prefix; world/creature/thing tags are unused reference/template names, no object fields or size accessed.

## 0093b6f0: CAIStateGroup_SeekEnemy::InitStateGroup
Original manifest label: `CAIStateGroup_SeekEnemy / ?InitStateGroup@CAIStateGroup_SeekEnemy@@UAEXABVCWorld@@AAVCThingAICreature@@@Z`.
Identity: proven; own RTTI InitStateGroup hook and both PDB exact public virtual API; PEnemy original member+20; exact CIntelligentPointer<CThing> temporary vtable and real base constructor/GetPItem/SetPItem/destructor calls.
Layout: generated state-group headers flatten CBaseClass polymorphism and tracked fields, omit InitStateGroup API; local real state-group base20 and tracked8 retained. Other original base members are an untouched prefix; world/creature/thing tags are unused reference/template names, no object fields or size accessed.

## 0093bea0: CAIStateGroup_SeekScriptTarget::InitStateGroup
Original manifest label: `CAIStateGroup_SeekScriptTarget / ?InitStateGroup@CAIStateGroup_SeekScriptTarget@@UAEXABVCWorld@@AAVCThingAICreature@@@Z`.
Identity: proven; own RTTI InitStateGroup hook and both PDB exact public virtual API; PEnemy original member+20; exact CIntelligentPointer<CThing> temporary vtable and real base constructor/GetPItem/SetPItem/destructor calls.
Layout: generated state-group headers flatten CBaseClass polymorphism and tracked fields, omit InitStateGroup API; local real state-group base20 and tracked8 retained. Other original base members are an untouched prefix; world/creature/thing tags are unused reference/template names, no object fields or size accessed.

## 00922f50: CCreatureAction_AddFoodToTable::CCreatureAction_AddFoodToTable
Original manifest label: `CCreatureAction_PutDownTankard / CCreatureAction_PutDownTankard`.
Identity: proven; own RTTI 012901B4 and exact PDB one-creature-reference ctor; original base ctor00842A20(Creature&,constCCharString&,NCreatureMode::EMode); own animation literal PUT_FOOD_ON_TABLE; real enum argument 0.
Layout: generated action headers flatten CBaseClass/CBaseObject hierarchy and omit inherited constructor API; local original base chain preserves action168/tableMode+168. CThingCreatureBase only a reference tag, no object fields or size. CCharString header absent. Enum identifier labels are descriptive; original enum type and values are proven.

## 00947be0: CCreatureAction_PutDownTankard::CCreatureAction_PutDownTankard
Original manifest label: `CCreatureAction_PutDownTankard / CCreatureAction_PutDownTankard`.
Identity: proven; own RTTI 01292E9C and exact PDB one-creature-reference ctor; original base ctor00842A20(Creature&,constCCharString&,NCreatureMode::EMode); own animation literal DRINK_AT_TABLE_OUTOF; real enum argument 39.
Layout: generated action headers flatten CBaseClass/CBaseObject hierarchy and omit inherited constructor API; local original base chain preserves action168/tableMode+168. CThingCreatureBase only a reference tag, no object fields or size. CCharString header absent. Enum identifier labels are descriptive; original enum type and values are proven.

## 0096c820: CCreatureAction_StopEatingAtTable::CCreatureAction_StopEatingAtTable
Original manifest label: `CCreatureAction_PutDownTankard / CCreatureAction_PutDownTankard`.
Identity: proven; own RTTI 01296DCC and exact PDB one-creature-reference ctor; original base ctor00842A20(Creature&,constCCharString&,NCreatureMode::EMode); own animation literal ST_SAT_EATING_OUTOF; real enum argument 40.
Layout: generated action headers flatten CBaseClass/CBaseObject hierarchy and omit inherited constructor API; local original base chain preserves action168/tableMode+168. CThingCreatureBase only a reference tag, no object fields or size. CCharString header absent. Enum identifier labels are descriptive; original enum type and values are proven.

## 00923aa0: CAIStateGroup_ServeFoodAtHome::IsTime
Original manifest label: `CAIStateGroup_ServeFoodAtHome / CAIStateGroup_ServeFoodAtHome::IsTime`.
Identity: Own RTTI0128cefc IsTime slot14 and PDB signature; TC35 CTCVillageMember GetPVillage007ee540.00658bb9 is IsEatTime: CVillageDef EatStartTimes retail132/EatDuration144 (donor152/168 reduced by five/four-byte vector changes), not catalog IsCookTime.
Layout: no active CThing header. Original UsingComponents+32 and Components+68; actual const LowerBound overload004365b0.
Layout: genuine state-group polymorphism replaces flat header vptr; PBrain+4. Other inherited owner fields are unused.

## 00923b00: CAIStateGroup_ServeFoodAtTavern::IsTime
Original manifest label: `CAIStateGroup_ServeFoodAtTavern / CAIStateGroup_ServeFoodAtTavern::IsTime`.
Identity: Own RTTI0128aa7c IsTime slot14 and PDB signature; TC35 CTCVillageMember GetPVillage007ee540.00658bb9 is IsEatTime: CVillageDef EatStartTimes retail132/EatDuration144 (donor152/168 reduced by five/four-byte vector changes), not catalog IsCookTime.
Layout: no active CThing header. Original UsingComponents+32 and Components+68; actual const LowerBound overload004365b0.
Layout: genuine state-group polymorphism replaces flat header vptr; PBrain+4. Other inherited owner fields are unused.

## 0096d200: CAIStateGroup_EatAtHome::IsTime
Original manifest label: `CAIStateGroup_EatAtHome / CAIStateGroup_EatAtHome::IsTime`.
Identity: Own RTTI0128d25c IsTime slot15 and PDB signature; TC35 CTCVillageMember GetPVillage007ee540.00658bb9 is IsEatTime: CVillageDef EatStartTimes retail132/EatDuration144 (donor152/168 reduced by five/four-byte vector changes), not catalog IsCookTime.
Layout: no active CThing header. Original UsingComponents+32 and Components+68; actual const LowerBound overload004365b0.
Layout: genuine state-group polymorphism replaces flat header vptr; PBrain+4. Other inherited owner fields are unused.

## 009240c0: CCreatureAction_KrakenLowAttackInto::Initialise
Original manifest label: `?A0x29922355::CCreatureAction_KrakenLowAttackInto / ?A0x29922355::CCreatureAction_KrakenLowAttackInto::vfunc_12`.
Identity: Own full anonymous RTTI01290b0c and vtable slot12; both PDB Initialise name; TC226 Kraken, HeadTrackTarget81, CCombatActionBase::Initialise00857520. Anonymous leaf uses documented unique prefix.
Layout: Kraken active header quarantined; HeadTrackTarget+81 is donor97 minus two list8-byte reductions. Original action hierarchy has CBaseObject8, PCreature+8 and action base168. Target requires its Kraken component; absent-component out value remains unset as in retail.

## 00924200: CCreatureAction_KrakenLowPoseInto::Initialise
Original manifest label: `?A0x29922355::CCreatureAction_KrakenLowPoseInto / ?A0x29922355::CCreatureAction_KrakenLowPoseInto::vfunc_12`.
Identity: Own full anonymous RTTI01290854 and vtable slot12; both PDB Initialise name; TC226 Kraken, HeadTrackTarget81, CCombatActionBase::Initialise00857520. Anonymous leaf uses documented unique prefix.
Layout: Kraken active header quarantined; HeadTrackTarget+81 is donor97 minus two list8-byte reductions. Original action hierarchy has CBaseObject8, PCreature+8 and action base168. Target requires its Kraken component; absent-component out value remains unset as in retail.

## 00924350: CCreatureAction_KrakenHighAttackInto::Initialise
Original manifest label: `?A0x29922355::CCreatureAction_KrakenHighAttackInto / ?A0x29922355::CCreatureAction_KrakenHighAttackInto::vfunc_12`.
Identity: Own full anonymous RTTI01290f1c and vtable slot12; both PDB Initialise name; TC226 Kraken, HeadTrackTarget81, CCombatActionBase::Initialise00857520. Anonymous leaf uses documented unique prefix.
Layout: Kraken active header quarantined; HeadTrackTarget+81 is donor97 minus two list8-byte reductions. Original action hierarchy has CBaseObject8, PCreature+8 and action base168. Target requires its Kraken component; absent-component out value remains unset as in retail.

## 0093cfe0: CAIStateGroup_ButterflyWander::Initialise
Original manifest label: `CAIStateGroup_ButterflyWander / CAIStateGroup_ButterflyWander::Initialise`.
Identity: Own RTTI0128b918 slot1; actual inheritance from RTTI and PDB StartPos+36; original creature GetPosition006a4d00 and CAIStateGroupBase::Initialise.
Layout: shared owner headers flatten inheritance and StartPos. Real base20, FlyingThingBase36 and EnemyCreatureBase28; inherited GetPosition uses distinct creature accessor006a4d00, not CThing004c73d0.

## 00959ee0: CAIStateGroup_HoverKeepInArea::Initialise
Original manifest label: `CAIStateGroup_HoverKeepInArea / CAIStateGroup_HoverKeepInArea::Initialise`.
Identity: Own RTTI0128c6f4 slot1; actual inheritance from RTTI and PDB StartPos+28; original creature GetPosition006a4d00 and CAIStateGroup_EnemyCreatureBase::Initialise.
Layout: shared owner headers flatten inheritance and StartPos. Real base20, FlyingThingBase36 and EnemyCreatureBase28; inherited GetPosition uses distinct creature accessor006a4d00, not CThing004c73d0.

## 0095a270: CAIStateGroup_HoverAttackArea::Initialise
Original manifest label: `CAIStateGroup_HoverAttackArea / CAIStateGroup_HoverAttackArea::Initialise`.
Identity: Own RTTI0128c754 slot1; actual inheritance from RTTI and PDB StartPos+28; original creature GetPosition006a4d00 and CAIStateGroup_EnemyCreatureBase::Initialise.
Layout: shared owner headers flatten inheritance and StartPos. Real base20, FlyingThingBase36 and EnemyCreatureBase28; inherited GetPosition uses distinct creature accessor006a4d00, not CThing004c73d0.

## 00950b20: CAIStateGroup_JackDragon::FindHitEvent
Original manifest label: `CAIStateGroup_JackDragon / FindHitEvent`.
Identity: Own owner IsRangeAttack offset, original PDB private FindHitEvent; helper953000 calls IsHitByShot00810f70,953090 IsHitByMelee00810f80; actual const event/filter template types, not duplicate manifest labels.
Layout: no active CThing header. Original UsingComponents+32 and Components+68; actual const LowerBound overload004365b0.
Layout: active owner and base/brain headers retained; inherited PBrain+4 projected through shared CAIStateGroupBase layout. Helpers953000/953090 are Shot/Melee const CEventHitBy instantiations.

## 0096c250: CAIStateGroup_Thunder::FindHitEvent
Original manifest label: `CAIStateGroup_JackDragon / FindHitEvent`.
Identity: Own owner bIsRangeAttack offset, original PDB protected FindHitEvent; helper953000 calls IsHitByShot00810f70,953090 IsHitByMelee00810f80; actual const event/filter template types, not duplicate manifest labels.
Layout: no active CThing header. Original UsingComponents+32 and Components+68; actual const LowerBound overload004365b0.
Layout: active owner and base/brain headers retained; inherited PBrain+4 projected through shared CAIStateGroupBase layout. Helpers953000/953090 are Shot/Melee const CEventHitBy instantiations.

## 0097baa0: CAIStateGroup_ScorpionKing::FindHitEvent
Original manifest label: `CAIStateGroup_JackDragon / FindHitEvent`.
Identity: Own owner bIsRangeAttack offset, original PDB protected FindHitEvent; helper953000 calls IsHitByShot00810f70,953090 IsHitByMelee00810f80; actual const event/filter template types, not duplicate manifest labels.
Layout: no active CThing header. Original UsingComponents+32 and Components+68; actual const LowerBound overload004365b0.
Layout: active owner and base/brain headers retained; inherited PBrain+4 projected through shared CAIStateGroupBase layout. Helpers953000/953090 are Shot/Melee const CEventHitBy instantiations.

## 00953fa0: CAIStateGroup_MazeDefensiveMeleeCombat::Initialise
Original manifest label: `CAIStateGroup_MazeDefensiveMeleeCombat / CAIStateGroup_MazeDefensiveMeleeCombat::Initialise`.
Identity: Own RTTI0128c224 slot1 and exact PDB Initialise; LastHealFrame+32. Helper008fcfc0 is original protected GetWorld() const returning reference, not propagated GetPWorld;0049d870 CWorld::GetFrame static fastcall.
Layout: shared state-group headers flatten the vptr and base fields. Genuine inheritance is needed for virtual Initialise and inherited GetWorld. PDB and retail agree base20 and MazeBase32; current-frame member+32.

## 00954040: CAIStateGroup_MazeTutorialBattleRespondToLightning::Initialise
Original manifest label: `CAIStateGroup_MazeTutorialBattleRespondToLightning / CAIStateGroup_MazeTutorialBattleRespondToLightning::Initialise`.
Identity: Own RTTI0128c314 slot1 and exact PDB Initialise; FrameOfLastHit+32. Helper008fcfc0 is original protected GetWorld() const returning reference, not propagated GetPWorld;0049d870 CWorld::GetFrame static fastcall.
Layout: shared state-group headers flatten the vptr and base fields. Genuine inheritance is needed for virtual Initialise and inherited GetWorld. PDB and retail agree base20 and MazeBase32; current-frame member+32.

## 00954080: CAIStateGroup_MazeTutorialBattleUseShield::Initialise
Original manifest label: `CAIStateGroup_MazeTutorialBattleUseShield / CAIStateGroup_MazeTutorialBattleUseShield::Initialise`.
Identity: Own RTTI0128c350 slot1 and exact PDB Initialise; FrameOfLastShield+32. Helper008fcfc0 is original protected GetWorld() const returning reference, not propagated GetPWorld;0049d870 CWorld::GetFrame static fastcall.
Layout: shared state-group headers flatten the vptr and base fields. Genuine inheritance is needed for virtual Initialise and inherited GetWorld. PDB and retail agree base20 and MazeBase32; current-frame member+32.

## 009540f0: CAIStateGroup_MazeBase::OnActivate
Original manifest label: `CAIStateGroup_MazeBase / ?OnActivate@CAIStateGroup_MazeBase@@UAEXXZ`.
Identity: Own RTTI0128c1ac slot2 and PDB OnActivate; target+20 tracked pointer, GetPItem00a01b50 twice; original RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool)006edc50, inherited GetWorld reference008fcfc0.
Layout: active CThing header is quarantined; retail type flags+108. No creature fields beyond CThing are read. Shared CAIBrain::Creature+24 and CWorld::PThingCombatManager_ptr+76 retained.
Layout: shared state-group headers flatten the vptr and base fields. Genuine inheritance is needed for virtual Initialise and inherited GetWorld. PDB and retail agree base20 and MazeBase32; current-frame member+32.

## 00954150: CAIStateGroup_MazeDefensiveMeleeCombat::OnActivate
Original manifest label: `CAIStateGroup_MazeDefensiveMeleeCombat / ?OnActivate@CAIStateGroup_MazeDefensiveMeleeCombat@@UAEXXZ`.
Identity: Own RTTI0128c224 slot2 and PDB OnActivate; target+20 tracked pointer, GetPItem00a01b50 twice; original RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool)006edc50, inherited GetWorld reference008fcfc0.
Layout: active CThing header is quarantined; retail type flags+108. No creature fields beyond CThing are read. Shared CAIBrain::Creature+24 and CWorld::PThingCombatManager_ptr+76 retained.
Layout: shared state-group headers flatten the vptr and base fields. Genuine inheritance is needed for virtual Initialise and inherited GetWorld. PDB and retail agree base20 and MazeBase32; current-frame member+32.

## 009541e0: CAIStateGroup_MazeTutorialBattleTeleport::OnActivate
Original manifest label: `CAIStateGroup_MazeTutorialBattleTeleport / ?OnActivate@CAIStateGroup_MazeTutorialBattleTeleport@@UAEXXZ`.
Identity: Own RTTI0128c2d8 slot2 and PDB OnActivate; target+20 tracked pointer, GetPItem00a01b50 twice; original RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool)006edc50, inherited GetWorld reference008fcfc0.
Layout: active CThing header is quarantined; retail type flags+108. No creature fields beyond CThing are read. Shared CAIBrain::Creature+24 and CWorld::PThingCombatManager_ptr+76 retained.
Layout: shared state-group headers flatten the vptr and base fields. Genuine inheritance is needed for virtual Initialise and inherited GetWorld. PDB and retail agree base20 and MazeBase32; current-frame member+32.

## 009670a0: CAIStateGroup_TavernServer::FreeServingPoint
Original manifest label: `CAIStateGroup_TavernKeeper / FreeBarPoint`.
Identity: Original PDB owner method and tracked/index fields; TC184 CTCTavernTable, helper007b9090 original FreeServingPoint(long). Own header offsets distinguish server from keeper; old FreeBarPoint labels propagated.
Layout: active owner header retained. Original tracked PTable viewed as CIntelligentPointer<CThingObject>; component must exist for a reserved index. Missing-component output remains unset as retail.

## 00967110: CAIStateGroup_TavernServer::FreeBarPoint
Original manifest label: `CAIStateGroup_TavernKeeper / FreeBarPoint`.
Identity: Original PDB owner method and tracked/index fields; TC185 CTCTavernBar, helper007ba4f0 original FreeSlot(long). Own header offsets distinguish server from keeper; old FreeBarPoint labels propagated.
Layout: active owner header retained. Original tracked PBar viewed as CIntelligentPointer<CThingObject>; component must exist for a reserved index. Missing-component output remains unset as retail.

## 00968950: CAIStateGroup_TavernKeeper::FreeBarPoint
Original manifest label: `CAIStateGroup_TavernKeeper / FreeBarPoint`.
Identity: Original PDB owner method and tracked/index fields; TC185 CTCTavernBar, helper007ba4f0 original FreeSlot(long). Own header offsets distinguish server from keeper; old FreeBarPoint labels propagated.
Layout: active owner header retained. Original tracked PBar viewed as CIntelligentPointer<CThingObject>; component must exist for a reserved index. Missing-component output remains unset as retail.

## 0098bfb0: std::__uninitialized_fill_n
Original manifest label: `_global / fill<C3DClothQuad*,C3DClothQuad>`.
Identity: Own caller0098d240 is vector<C4DVector> overflow reached by SetPixelShaderConstant0098b390 through push_back0098d820; established exact helper binding and PDB class key V.
Layout: original four-float class16, implicit copy constructor.

## 00a51c20: std::__uninitialized_fill_n
Original manifest label: `_global / fill<C3DClothQuad*,C3DClothQuad>`.
Identity: Own caller00a54fa0 vector<CQuaternion> overflow and established exact helper binding; original PDB X/Y/Z/W fields and class key V.
Layout: original four-float class16, implicit copy constructor.

## 00bdb710: std::__uninitialized_fill_n
Original manifest label: `_gapscan / sub_00bdb710`.
Identity: Own overflow00bdb8e0 shares uninitialized-copy00bdb6d0 with CEngineVSConstantLayoutBasic constructor00bdbb70; ReservedPresets vector at+320 copied through00bdbb60/00bdb9f0; PDB CVertexShaderConstant X/Y/Z/W sixteen-byte class.
Layout: original four-float class16, implicit copy constructor.

## 0098d240: std::vector::_M_insert_overflow
Original manifest label: `_global / std_vector_Reallocate`.
Identity: Own SetPixelShaderConstant0098b390 through push_back0098d820; actual uninitialized-copy0098bf70 and fill0098bfb0.
Layout: real STLport vector12, implicit sixteen-byte element copy, original class keys and fields.

## 00ad4d10: std::vector::_M_insert_overflow
Original manifest label: `std / std::vector_Reallocate_Reserve`.
Identity: Own JoinAdjacentFaces00ac8ae0 appends FaceA/FaceB/UniqueVertex1/UniqueVertex2 through00ad5ce0 into Edges+40, original donor52 minus3 preceding vector contractions; helpers00acffa0/00acffe0.
Layout: real STLport vector12, implicit sixteen-byte element copy, original class keys and fields.

## 00bdb8e0: std::vector::_M_insert_overflow
Original manifest label: `_gapscan / sub_00bdb8e0`.
Identity: Own uninitialized-copy00bdb6d0 also used by CEngineVSConstantLayoutBasic ctor00bdbb70, original ReservedPresets at+320; fill00bdb710 is N148.
Layout: real STLport vector12, implicit sixteen-byte element copy, original class keys and fields.

## 0099be70: global::operator+
Original manifest label: `_global / operator+(CWideString const&,CWideString const&)`.
Identity: original PDB free fastcall overload; own append0099b8d0 and UnassignString0099b4d0. Counter binding 013bca20 is descriptive, not a recovered original variable name.
Layout: CWideStringData real STLport basic_string12 plus NoRefs+12; donor string28 differs.

## 0099bf30: global::operator+
Original manifest label: `_global / operator+(CWideString const &,wchar_t const *)`.
Identity: original PDB free fastcall overload; own append0099b940 and UnassignString0099b4d0. Counter binding 013bca20 is descriptive, not a recovered original variable name.
Layout: CWideStringData real STLport basic_string12 plus NoRefs+12; donor string28 differs.

## 0099c120: global::operator+
Original manifest label: `CCharString / CCharString::InsertCharWith`.
Identity: original PDB free fastcall overload; own append0099b9a0 and UnassignString0099b4d0. Counter binding 013bca20 is descriptive, not a recovered original variable name.
Layout: CWideStringData has original STLport basic_string12 and NoRefs+12. basic_string inherits protected _String_base with allocator proxy; its original pointer/count/default-allocator constructor is evidenced by retail0099ca40. Shared fable_string.h flattens these original storage types and omits the character append API.

## 0099f570: global::operator+
Original manifest label: `CCharString / CCharString::AppendData`.
Identity: original PDB free fastcall overload; own append0099f0a0 and UnassignString0099e9b0. Counter binding 013bd800 is descriptive, not a recovered original variable name.
Layout: Original packed CBasicString<char>13 plus NoRefs+13, PStringData sole pointer.

## 0099f600: global::operator+
Original manifest label: `CCharString / CCharString::AppendCString`.
Identity: original PDB free fastcall overload; own append0099f100 and UnassignString0099e9b0. Counter binding 013bd800 is descriptive, not a recovered original variable name.
Layout: Original packed CBasicString<char>13 plus NoRefs+13, PStringData sole pointer.

## 0099f7a0: global::operator+
Original manifest label: `_global / CWideString_AppendChar`.
Identity: original PDB free fastcall overload; own append0099f180 and UnassignString0099e9b0. Counter binding 013bd800 is descriptive, not a recovered original variable name.
Layout: Original packed CBasicString<char>13 plus NoRefs+13, PStringData sole pointer.

## 009b6e00: CArray::DeallocateUnusedMemory
Original manifest label: `CArray<std::pair<CCharString / DeallocateUnusedMemory`.
Identity: Own CDefinitionManager::RemoveUnusedMemory009ae5f0 trims GlobalIndices; reserve009b4030, rangeinsert009b4240, DeallocateMemory009b4a90. PDB original CArray method and real STLport base templates.
Layout: CArray inherits twelve-byte vector; each type and stride derived independently from own callers.

## 009b7b10: CArray::DeallocateUnusedMemory
Original manifest label: `CArray<std::pair<CCharString / DeallocateUnusedMemory`.
Identity: Own CBankFile::PostInitEntries009cfb70 trims SymbolCRCIndices; reserve009b4eb0, rangeinsert009b54e0, DeallocateMemory009b5fb0. PDB original CArray method and real STLport base templates.
Layout: CArray inherits twelve-byte vector; each type and stride derived independently from own callers.

## 00a39a00: CArray::DeallocateUnusedMemory
Original manifest label: `CArray<std::pair<CCharString / DeallocateUnusedMemory`.
Identity: Own CCRCSymbolMap::DeallocateUnusedMemory00a38590 and LoadFromFile00a39010 trim SLONGMap+4; reserve00a39590, rangeinsert00a39610, DeallocateMemory00a39950. PDB original CArray method and real STLport base templates.
Layout: CArray inherits twelve-byte vector; each type and stride derived independently from own callers.

## 009b71b0: std::vector::_M_insert_overflow
Original manifest label: `_global / vector_insert`.
Identity: Own CDefinitionManager::CheckDependencies009b05f0 and established push_back009b7ca0 identify unsigned-long name-map values; helpers009b2680/009b26c0.
Layout: vector12, original element8; actual CCharString pointer with external copy0099ec30 and destructor0099eae0. CMeshDataBank is only a nested type scope, never an owner instance.

## 00a2d3b0: std::vector::_M_insert_overflow
Original manifest label: `_global / Vector_Reallocate_8bytes_Complex`.
Identity: Own CMeshDataBank::InitialiseEntry00a276d0 adds PhonemeName/AnimationIndex through map00a2e290 into LipSyncAnimations+968, then push_back00a2d8b0; original nested class V and PDB fields; helpers00a2a350/00a2a390.
Layout: vector12, original element8; actual CCharString pointer with external copy0099ec30 and destructor0099eae0. CMeshDataBank is only a nested type scope, never an owner instance.

## 00a41480: std::vector::_M_insert_overflow
Original manifest label: `_global / Vector_InsertOrFill`.
Identity: Own CSoundManagerLH NameMap and FindByName00a3ed30 establish pair<CCharString,CSoundBankLH*>; push_back00a418a0 exact helperbinding; helpers00a3fc40/00a3fc80.
Layout: vector12, original element8; actual CCharString pointer with external copy0099ec30 and destructor0099eae0. CMeshDataBank is only a nested type scope, never an owner instance.

## 009ba330: CStringParser::ReadAsStringUntilString
Original manifest label: `CStringParser / ReadAsStringUntilString`.
Identity: own delimiter helper009b9a20, original PDB return and reference signature.
Layout: shared parser owns opaque CCursorPos12 at+28; original InQuotes+0, StringPos+4, Line+8.

## 009ba3e0: CStringParser::ReadAsStringUntilPastString
Original manifest label: `CStringParser / ReadAsStringUntilString`.
Identity: own delimiter helper009b9ba0, original PDB return and reference signature.
Layout: shared parser owns opaque CCursorPos12 at+28; original InQuotes+0, StringPos+4, Line+8.

## 009ba440: CStringParser::ReadAsStringUntilWholeString
Original manifest label: `CStringParser / ReadAsStringUntilString`.
Identity: own delimiter helper009b9aa0, original PDB return and reference signature.
Layout: shared parser owns opaque CCursorPos12 at+28; original InQuotes+0, StringPos+4, Line+8.

## 009be830: CDisplayManager::GetBestCompressedOpaqueTextureFormat
Original manifest label: `CDisplayManager / GetBestCompressedAlphaTextureFormat`.
Identity: Both PDB sets original GetBestCompressedOpaqueTextureFormat(unsigned long,CPixelFormat&); caller texture-selector branch and own D3DFMT_DXT1 constant, IDirect3D9 CheckDeviceFormat slot10, Initialise(_D3DFORMAT)009e3830.
Layout: no active display-manager header. D3D+84, device+88, SDK caps+92(size304), CurrentMode+440; only caps DeviceType/AdapterOrdinal and mode Format+452 read.

## 009be870: CDisplayManager::GetBestCompressedAlphaTextureFormat
Original manifest label: `CDisplayManager / GetBestCompressedAlphaTextureFormat`.
Identity: Both PDB sets original GetBestCompressedAlphaTextureFormat(unsigned long,CPixelFormat&); caller texture-selector branch and own D3DFMT_DXT3 constant, IDirect3D9 CheckDeviceFormat slot10, Initialise(_D3DFORMAT)009e3830.
Layout: no active display-manager header. D3D+84, device+88, SDK caps+92(size304), CurrentMode+440; only caps DeviceType/AdapterOrdinal and mode Format+452 read.

## 009be8b0: CDisplayManager::GetBestCompressedBooleanAlphaTextureFormat
Original manifest label: `CDisplayManager / GetBestCompressedAlphaTextureFormat`.
Identity: Both PDB sets original GetBestCompressedBooleanAlphaTextureFormat(unsigned long,CPixelFormat&); caller texture-selector branch and own D3DFMT_DXT1 constant, IDirect3D9 CheckDeviceFormat slot10, Initialise(_D3DFORMAT)009e3830.
Layout: no active display-manager header. D3D+84, device+88, SDK caps+92(size304), CurrentMode+440; only caps DeviceType/AdapterOrdinal and mode Format+452 read.

## 009cb930: CBankCreatorException::GetText
Original manifest label: `CBankFile / GetBankHandle`.
Identity: PDB GetText const; caller009cf328 formats caught bank exception; EntrySymbol+0/ErrorText+4.
Layout: no active exception header; original EntrySymbol+0, ErrorText+4.

## 009cba40: CBankFile::GetBankHandle
Original manifest label: `CBankFile / GetBankHandle`.
Identity: Own field+0x88 and corrected active header; PDB exact const getter; callers009f82ac/00a09ddc.
Layout: active shared owner layout retained; opaque member viewed as its original composite type.

## 00b38890: CEngineShadowRenderer::PeekSceneData
Original manifest label: `CBankFile / GetBankHandle`.
Identity: Own scene field+12, copy helper00b38600; FableWin exact const-by-value PeekSceneData, caller00b330ee.
Layout: active shared owner layout retained; opaque member viewed as its original composite type.

## 009d88d0: MicroThread::MicroThreadDetails::~MicroThreadDetails
Original manifest label: `CSystemRegistry / ~CSystemRegistry`.
Identity: Own deleting-dtor009d88b0 and full RTTI129b938 prove MicroThreadDetails; base RTTI129b928 MicroThreadInfo; original Stack+8 vector<unsigned char>.
Layout: no active micro-thread header; bool Dead+4 and saved-stack vector+8, confirmed by original stack-size method.

## 00b2fb30: CEnginePrimitiveOcclusionInfo::~CEnginePrimitiveOcclusionInfo
Original manifest label: `CSystemRegistry / ~CSystemRegistry`.
Identity: Own deleting-dtor00b2fb10 and full RTTI12a1284; CIVCountedPointeeBase at0, InvisibleFrames+8, handles+12, base vtable12354d0.
Layout: shared base headers describe flattened vptrs; genuine polymorphic hierarchy is required for compiler-generated destruction. Original base fields and offsets retained; no target base reconstruction claim.

## 00bcc340: CEngineInternalPrimitiveSpriteTrail::~CEngineInternalPrimitiveSpriteTrail
Original manifest label: `CSystemRegistry / ~CSystemRegistry`.
Identity: Own deleting-dtor00bcc320 and full RTTI12a6a4c; actual inherited hierarchy and original Trail+104; base vtable12354d0.
Layout: shared base headers describe flattened vptrs; genuine polymorphic hierarchy is required for compiler-generated destruction. Original base fields and offsets retained; no target base reconstruction claim.

## 009e41e0: CJoystick::AddEventToStore
Original manifest label: `CJoystick / AddEventToStore`.
Identity: Own device callers and paired scan/add helpers, original PDB signatures; Joystick RTTI129c0ec slot1 and count+0xd10; capacity64, event52.
Layout: CInputEvent52, opaque original Data union32; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. ScanInfo Started+0, Index+4. Only whole-event copies touch Data.

## 00a66770: CKeyboard::AddEventToStore
Original manifest label: `CJoystick / AddEventToStore`.
Identity: Own device callers and paired scan/add helpers, original PDB signatures; CKeyboardDX caller and count+0x3404; capacity256, event52.
Layout: CInputEvent52, opaque original Data union32; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. ScanInfo Started+0, Index+4. Only whole-event copies touch Data.

## 00a66b20: CMouse::AddEventToStore
Original manifest label: `CJoystick / AddEventToStore`.
Identity: Own device callers and paired scan/add helpers, original PDB signatures; CMouseDX caller and count+0x3404; capacity256, event52.
Layout: CInputEvent52, opaque original Data union32; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. ScanInfo Started+0, Index+4. Only whole-event copies touch Data.

## 009e4280: CJoystick::EventScanNext
Original manifest label: `CJoystick / CJoystick::EventScanNext`.
Identity: Own device callers and paired scan/add helpers, original PDB signatures; Joystick RTTI129c0ec slot1 and count+0xd10; capacity64, event52.
Layout: CInputEvent52, opaque original Data union32; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. ScanInfo Started+0, Index+4. Only whole-event copies touch Data.
Layout: shared CJoystick flat vptr replaced by its genuine virtual interface; other fields retain retail offsets. STLport list has one sentinel pointer.

## 00a66810: CKeyboard::EventScanNext
Original manifest label: `CJoystick / EventScanNext`.
Identity: Own device callers and paired scan/add helpers, original PDB signatures; CKeyboardDX caller and count+0x3404; capacity256, event52.
Layout: CInputEvent52, opaque original Data union32; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. ScanInfo Started+0, Index+4. Only whole-event copies touch Data.

## 00a66bc0: CMouse::EventScanNext
Original manifest label: `CJoystick / EventScanNext`.
Identity: Own device callers and paired scan/add helpers, original PDB signatures; CMouseDX caller and count+0x3404; capacity256, event52.
Layout: CInputEvent52, opaque original Data union32; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. ScanInfo Started+0, Index+4. Only whole-event copies touch Data.

## 009e5c60: CSymbolMap::UpdateDirty
Original manifest label: `CTCVillage / OnInitialActivate`.
Identity: Own TransferIn009e2590 and canonical binding prove CSymbolMap::UpdateDirty()const; original nested CSymbolInfo GroupName/EntryName/Val gives16-byte pair. Own RTTI129c158 -> CBaseClassNonCopyable -> CBaseClass. Sort009e7e40.
Layout: original polymorphic bases and STLport containers; no active owner header. CVectorMap Data12, empty Compare byte+12, Dirty+13.

## 00a38fe0: CCRCSymbolMap::UpdateDirtyQuick
Original manifest label: `CTCVillage / OnInitialActivate`.
Identity: PDB original UpdateDirtyQuick()const; adjacent UpdateDirty00a38fb0 also trims through00a39a00, this quick form only sorts. SLONGMap+4 pair<ulong,long>, own RTTI129cf84 original base chain. Sort009b8520.
Layout: original polymorphic bases and STLport containers; no active owner header. CVectorMap Data12, empty Compare byte+12, Dirty+13.

## 00cb8930: CScriptBase::PostAddScriptedEntities
Original manifest label: `CTCVillage / OnInitialActivate`.
Identity: Own derived script constructors call PostAddScriptedEntities00cb8930 after bindings; canonical original name and PDB. EntityScriptBindings+24 after original Threads4 and EntityScripts16. Sort00cbdf70 pair<CCharString,CCountedPointer<CEntityScriptBindingBase>>.
Layout: original polymorphic bases and STLport containers; no active owner header. CVectorMap Data12, empty Compare byte+12, Dirty+13.

## 009e5fa0: std::__uninitialized_copy
Original manifest label: `_Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<unsigned_int,std::allocator<unsigned_int>_>_>,unsigned_int*,std / _Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<unsigned_int,std::allocator<unsigned_int>_>_>,unsigned_int*,std::allocator<unsigned_int>_>`.
Identity: Own GetSymbolsFromGroupName009e5290 outputs CArray<CSymbolInfo>; original GroupName/EntryName/Val and class V. Helpers009e5fa0/009e5fe0/_M_clear009e7250.
Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.

## 00a3fbc0: std::__uninitialized_copy
Original manifest label: `_Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<unsigned_int,std::allocator<unsigned_int>_>_>,unsigned_int*,std / _Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<unsigned_int,std::allocator<unsigned_int>_>_>,unsigned_int*,std::allocator<unsigned_int>_>`.
Identity: Own CSoundManagerLH::FindByNamePair00a3ee50 and canonical LowerBound00a40af0 prove pair<pair<CCharString,CCharString>,CSoundBankLH*>. Old fill00a3fc00 labels value long; correct pointer type used. Helpers00a3fbc0/00a3fc00/_M_clear00a40d00.
Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.

## 00cd4e60: std::__uninitialized_copy
Original manifest label: `_Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<unsigned_int,std::allocator<unsigned_int>_>_>,unsigned_int*,std / _Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<unsigned_int,std::allocator<unsigned_int>_>_>,unsigned_int*,std::allocator<unsigned_int>_>`.
Identity: Own parser00cd5170 and GetStringFromDataMarker00cd4ff0 prove CDataMarkerOutput::CElement, PDB original VariableName/DataAsString/DataAsULONG and class V. Replaces old descriptive CMarkerDataEntry with original name. Helpers00cd4e60/00cd4ea0/_M_clear00cdbcc0.
Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.

## 009e76f0: std::vector::_M_insert_overflow
Original manifest label: `CCombatWheel / CCombatWheel::InsertElements`.
Identity: Own GetSymbolsFromGroupName009e5290 outputs CArray<CSymbolInfo>; original GroupName/EntryName/Val and class V. Helpers009e5fa0/009e5fe0/_M_clear009e7250.
Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.

## 00a41390: std::vector::_M_insert_overflow
Original manifest label: `_global / CCombatWheel_Reallocate`.
Identity: Own CSoundManagerLH::FindByNamePair00a3ee50 and canonical LowerBound00a40af0 prove pair<pair<CCharString,CCharString>,CSoundBankLH*>. Old fill00a3fc00 labels value long; correct pointer type used. Helpers00a3fbc0/00a3fc00/_M_clear00a40d00.
Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.

## 00cdbdc0: std::vector::_M_insert_overflow
Original manifest label: `_global / CCombatWheel_ReinitializeUintVector`.
Identity: Own parser00cd5170 and GetStringFromDataMarker00cd4ff0 prove CDataMarkerOutput::CElement, PDB original VariableName/DataAsString/DataAsULONG and class V. Replaces old descriptive CMarkerDataEntry with original name. Helpers00cd4e60/00cd4ea0/_M_clear00cdbcc0.
Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.

## 009e7a50: std::vector::push_back
Original manifest label: `_global / CCombatWheel_InsertElement`.
Identity: Own GetSymbolsFromGroupName009e5290 outputs CArray<CSymbolInfo>; original GroupName/EntryName/Val and class V. Helpers009e5fa0/009e5fe0/_M_clear009e7250.
Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.

## 00a41840: std::vector::push_back
Original manifest label: `_global / Vector_PushBack_CCharStringTriple`.
Identity: Own CSoundManagerLH::FindByNamePair00a3ee50 and canonical LowerBound00a40af0 prove pair<pair<CCharString,CCharString>,CSoundBankLH*>. Old fill00a3fc00 labels value long; correct pointer type used. Helpers00a3fbc0/00a3fc00/_M_clear00a40d00.
Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.

## 00cdbf10: std::vector::push_back
Original manifest label: `_global / vector_CCharString_push_back`.
Identity: Own parser00cd5170 and GetStringFromDataMarker00cd4ff0 prove CDataMarkerOutput::CElement, PDB original VariableName/DataAsString/DataAsULONG and class V. Replaces old descriptive CMarkerDataEntry with original name. Helpers00cd4e60/00cd4ea0/_M_clear00cdbcc0.
Layout: original element12 with two CCharString pointers then one word; outer class used only as nested type scope; real STLport vector12.

## 009f2400: CMemoryAllocator::GetTotalMemoryControlledByAllocator
Original manifest label: `CMemoryAllocator / GetHeaderOverhead`.
Identity: Own allocator virtual slot6; VariableSize/FixedSize vtables129db1c/129db64 and original PDB GetTotalMemoryControlledByAllocator API. Table and owner layout independently supported by destructor009f2310; NoAllocators+1040. Do not reuse mislabeled neighboring sum names.
Layout: active allocator-base header is flattened. Restore real vptr; only getter slots6/8/12 are used. UnusedSlot names denote unmodeled entries, not recovered function names. Original PerformanceStats+4, Alignment+3092, AlignmentShift+3096 retained.
Layout: existing table g_AllocatorInfo01396868 has original PAllocator at+24, stride28; original global name unproven. Critical-section name is an established descriptive binding.

## 009f2500: CMemoryAllocator::GetNoFreeAreas
Original manifest label: `CMemoryAllocator / GetHeaderOverhead`.
Identity: Own allocator virtual slot8; VariableSize/FixedSize vtables129db1c/129db64 and original PDB GetNoFreeAreas API. Table and owner layout independently supported by destructor009f2310; NoAllocators+1040. Do not reuse mislabeled neighboring sum names.
Layout: active allocator-base header is flattened. Restore real vptr; only getter slots6/8/12 are used. UnusedSlot names denote unmodeled entries, not recovered function names. Original PerformanceStats+4, Alignment+3092, AlignmentShift+3096 retained.
Layout: existing table g_AllocatorInfo01396868 has original PAllocator at+24, stride28; original global name unproven. Critical-section name is an established descriptive binding.

## 009f2610: CMemoryAllocator::GetHeaderOverhead
Original manifest label: `CMemoryAllocator / GetHeaderOverhead`.
Identity: Own allocator virtual slot12; VariableSize/FixedSize vtables129db1c/129db64 and original PDB GetHeaderOverhead API. Table and owner layout independently supported by destructor009f2310; NoAllocators+1040. Do not reuse mislabeled neighboring sum names.
Layout: active allocator-base header is flattened. Restore real vptr; only getter slots6/8/12 are used. UnusedSlot names denote unmodeled entries, not recovered function names. Original PerformanceStats+4, Alignment+3092, AlignmentShift+3096 retained.
Layout: existing table g_AllocatorInfo01396868 has original PAllocator at+24, stride28; original global name unproven. Critical-section name is an established descriptive binding.

## 009fa100: CTexture::Unswizzle
Original manifest label: `CPaintMapDialog / InitNumberBox`.
Identity: PDB exact overload and own calls009f9e00 GetSurfaceLevel(long)const and009f2e20 Uninitialise. Surface helper009f4360 is original Unswizzle(ulong); distinct Swizzle sibling009f4330.
Layout: active CSurface header is flattened; real polymorphic CSurface has vptr+0, PD3DSurface+4, AllocationSource+8, PAllocatedMemory+12. Own vtable122f84c proves virtual destructor and no base. CTexture retains its active header layout.

## 009fa140: CTexture::Swizzle
Original manifest label: `CPaintMapDialog / InitNumberBox`.
Identity: PDB exact overload and own calls009f9e00 GetSurfaceLevel(long)const and009f2e20 Uninitialise. Surface helper009f2e40 is original Swizzle(CSurfaceLockedRect&), replacing E2 placeholder.
Layout: active CSurface header is flattened; real polymorphic CSurface has vptr+0, PD3DSurface+4, AllocationSource+8, PAllocatedMemory+12. Own vtable122f84c proves virtual destructor and no base. CTexture retains its active header layout.

## 009fa180: CTexture::Unswizzle
Original manifest label: `CPaintMapDialog / InitNumberBox`.
Identity: PDB exact overload and own calls009f9e00 GetSurfaceLevel(long)const and009f2e20 Uninitialise. Surface helper009f2e50 is original Unswizzle(CSurfaceLockedRect&), replacing E2 placeholder.
Layout: active CSurface header is flattened; real polymorphic CSurface has vptr+0, PD3DSurface+4, AllocationSource+8, PAllocatedMemory+12. Own vtable122f84c proves virtual destructor and no base. CTexture retains its active header layout.

## 00a00050: std::__copy_backward
Original manifest label: `_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std / _Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>`.
Identity: descriptive original element type unproven; actual SGI/STLport __copy_backward random-access five-argument ABI. CGraphicInfo is a plausible neighboring/PDB instantiation, but no callable xref identifies this body. Manifest string/dummy pair is inconsistent with POD block copying.
Layout: own reciprocal stride44 and rep movsd count11 prove the copied span only; record fields deliberately unnamed.

## 00ad0700: std::__copy_backward
Original manifest label: `_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std / _Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>`.
Identity: descriptive original element type unproven; actual SGI/STLport __copy_backward random-access five-argument ABI. COptimisedFace is a plausible size72 neighboring optimising-record candidate, but no callable xref identifies this body. Manifest string/dummy pair is inconsistent with POD block copying.
Layout: own reciprocal stride72 and rep movsd count18 prove the copied span only; record fields deliberately unnamed.

## 00b7abd0: std::__copy_backward
Original manifest label: `_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std / _Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>`.
Identity: descriptive original element type unproven; actual SGI/STLport __copy_backward random-access five-argument ABI. CPreTransposedBoneMatrix and other size48 records remain possible; no callable xref identifies this body. Manifest string/dummy pair is inconsistent with POD block copying.
Layout: own reciprocal stride48 and rep movsd count12 prove the copied span only; record fields deliberately unnamed.

## 00a013e0: CGraphicDataBank::GetNoFramesInGraphic
Original manifest label: `CGraphicDataBank / CGraphicDataBank::GetNoFramesInGraphic`.
Identity: Own CGraphicDataBank vtable129c8cc slot20 and PDB exact const virtual overload. GraphicList+480 size44; header FrameCount read. Out-of-range access has retail non-null precondition and is not converted to a zero result.
Layout: shared CBankFile header is flattened. Restore its proven CBaseClassNonCopyable base and retain every shared member. Retail CBankFile size272, CBankFileAsync356; RTTI CGraphicDataBank has CResourceBank at356, GraphicList at480. The async and resource payloads are opaque here and never accessed.
Layout: CGraphicHeader28 with natural padding at19; CGraphicInfo44. Original member names from PDB, fields confirmed by own reads.

## 00a01430: CGraphicDataBank::GetGraphicFrameWidth
Original manifest label: `CGraphicDataBank / CGraphicDataBank::GetGraphicFrameWidth`.
Identity: Own CGraphicDataBank vtable129c8cc slot21 and PDB exact const virtual overload. GraphicList+480 size44; header FrameWidth read. Out-of-range access has retail non-null precondition and is not converted to a zero result.
Layout: shared CBankFile header is flattened. Restore its proven CBaseClassNonCopyable base and retain every shared member. Retail CBankFile size272, CBankFileAsync356; RTTI CGraphicDataBank has CResourceBank at356, GraphicList at480. The async and resource payloads are opaque here and never accessed.
Layout: CGraphicHeader28 with natural padding at19; CGraphicInfo44. Original member names from PDB, fields confirmed by own reads.

## 00a01480: CGraphicDataBank::GetGraphicFrameHeight
Original manifest label: `CGraphicDataBank / CGraphicDataBank::GetGraphicFrameHeight`.
Identity: Own CGraphicDataBank vtable129c8cc slot22 and PDB exact const virtual overload. GraphicList+480 size44; header FrameHeight read. Out-of-range access has retail non-null precondition and is not converted to a zero result.
Layout: shared CBankFile header is flattened. Restore its proven CBaseClassNonCopyable base and retain every shared member. Retail CBankFile size272, CBankFileAsync356; RTTI CGraphicDataBank has CResourceBank at356, GraphicList at480. The async and resource payloads are opaque here and never accessed.
Layout: CGraphicHeader28 with natural padding at19; CGraphicInfo44. Original member names from PDB, fields confirmed by own reads.

## 00a03c80: CInputEvent::SetAsLeftMouseButtonPressed
Original manifest label: `CInputEvent / SetAsLeftMouseButtonPressed`.
Identity: Original PDB SetAsLeftMouseButtonPressed references and double parameters; mouse device3, own kind4, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.

## 00a03d90: CInputEvent::SetAsMiddleMouseButtonPressed
Original manifest label: `CInputEvent / SetAsLeftMouseButtonPressed`.
Identity: Original PDB SetAsMiddleMouseButtonPressed references and double parameters; mouse device3, own kind7, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.

## 00a03e40: CInputEvent::SetAsRightMouseButtonPressed
Original manifest label: `CInputEvent / SetAsLeftMouseButtonPressed`.
Identity: Original PDB SetAsRightMouseButtonPressed references and double parameters; mouse device3, own kind10, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.

## 00a03d10: CInputEvent::SetAsLeftMouseButtonHeld
Original manifest label: `CInputEvent / SetAsLeftMouseButtonHeld`.
Identity: Original PDB SetAsLeftMouseButtonHeld references and double parameters; mouse device3, own kind5, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.

## 00a03dc0: CInputEvent::SetAsMiddleMouseButtonHeld
Original manifest label: `CInputEvent / SetAsLeftMouseButtonHeld`.
Identity: Original PDB SetAsMiddleMouseButtonHeld references and double parameters; mouse device3, own kind8, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.

## 00a03e70: CInputEvent::SetAsRightMouseButtonHeld
Original manifest label: `CInputEvent / SetAsLeftMouseButtonHeld`.
Identity: Original PDB SetAsRightMouseButtonHeld references and double parameters; mouse device3, own kind11, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.

## 00a03d60: CInputEvent::SetAsLeftMouseButtonReleased
Original manifest label: `CInputEvent / SetAsLeftMouseButtonReleased`.
Identity: Original PDB SetAsLeftMouseButtonReleased references and double parameters; mouse device3, own kind6, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.

## 00a03e10: CInputEvent::SetAsMiddleMouseButtonReleased
Original manifest label: `CInputEvent / SetAsLeftMouseButtonReleased`.
Identity: Original PDB SetAsMiddleMouseButtonReleased references and double parameters; mouse device3, own kind9, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.

## 00a03ec0: CInputEvent::SetAsRightMouseButtonReleased
Original manifest label: `CInputEvent / SetAsLeftMouseButtonReleased`.
Identity: Original PDB SetAsRightMouseButtonReleased references and double parameters; mouse device3, own kind12, retail offsets24/32/40/44/48; held first-union offset0 field name not recovered.
Layout: original event Data32: first union12 not fully named by the donor, MouseMovement+12, MousePos+24; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. No active CInputEvent header. Held uses first union+0, not MouseMovement+12.

## 00a2a8f0: std::__copy_backward
Original manifest label: `_global / _Copy_backward<CCountedPointer<C3DGroup2>*,CCountedPointer<class_C3DGroup2>*>`.
Identity: Own fill-insert00a2d960 calls this; resize00a2dbc0 is called by CMeshDataBank::NotifyEntryAdded00a26d40 for DynamicInfoBuffer. Original CArray<CCountedPointer<CMeshDataBankEntry>>.
Layout: original CCPPointerInfo RefCount0/DeleteFunc4/Data8; wrapper Data0/Info4. Actual callback consumes its pointer in ECX, so fastcall in this TU. Scalar operator delete at BFE9BC releases the control block.

## 00a95e10: std::__copy_backward
Original manifest label: `_global / _Copy_backward<CCountedPointer<C3DGroup2>*,CCountedPointer<class_C3DGroup2>*>`.
Identity: Own fill-insert00a97f70/resize00a985a0 and N3DPrimitive::CDeviceIndependent Groups prove original CCountedPointer<C3DGroup2>.
Layout: original CCPPointerInfo RefCount0/DeleteFunc4/Data8; wrapper Data0/Info4. Actual callback consumes its pointer in ECX, so fastcall in this TU. Scalar operator delete at BFE9BC releases the control block.

## 00b96280: std::__copy_backward
Original manifest label: `_global / _Copy_backward<CCountedPointer<C3DGroup2>*,CCountedPointer<class_C3DGroup2>*>`.
Identity: Own fill-insert00b97e90/resize00b98170 and CEngineSceneGrid own caller00b949a0 prove original CCountedPointer<CEngineSceneGrid>; same verified type as N060.
Layout: original CCPPointerInfo RefCount0/DeleteFunc4/Data8; wrapper Data0/Info4. Actual callback consumes its pointer in ECX, so fastcall in this TU. Scalar operator delete at BFE9BC releases the control block.

## 00a32af0: std::pair::scalar_deleting_destructor
Original manifest label: `C3DMeshFilePartitionChunk / `vector_deleting_destructor'`.
Identity: Own C3DMeshStats destructor00a2f190 uses MeshVolumes+88, original CVectorMap<unsigned long,C3DMeshVolume>. Deleting helper unreferenced; exact same member contract gives probable pair identity.
Layout: real pair16 has scalar key0 and original twelve-byte second at4; C3DMeshVolume Name0/ConvexVolume4, CCompositeSurfaceInfo CBaseClass vptr0/Cache4. PDB fields and own helper calls agree; no active record headers.

## 00a6f960: std::pair::scalar_deleting_destructor
Original manifest label: `C3DMeshFilePartitionChunk / `vector_deleting_destructor'`.
Identity: Own CPhysicsMesh destructor00a6d550 uses NavigationContainmentVolumes, original CVectorMap<long,C3DMeshVolume>; separate string-keyed map has two string destructors. Deleting helper unreferenced; pair identity probable.
Layout: real pair16 has scalar key0 and original twelve-byte second at4; C3DMeshVolume Name0/ConvexVolume4, CCompositeSurfaceInfo CBaseClass vptr0/Cache4. PDB fields and own helper calls agree; no active record headers.

## 00ba9300: std::pair::scalar_deleting_destructor
Original manifest label: `C3DMeshFilePartitionChunk / `vector_deleting_destructor'`.
Identity: Own reserve00beeb80 and original CVectorMap<long,CCompositeSurfaceInfo>; actual CBaseClass destructor0099a2e0, not a string. Deleting helper unreferenced; pair identity probable.
Layout: real pair16 has scalar key0 and original twelve-byte second at4; C3DMeshVolume Name0/ConvexVolume4, CCompositeSurfaceInfo CBaseClass vptr0/Cache4. PDB fields and own helper calls agree; no active record headers.

## 00a33720: std::__destroy_aux
Original manifest label: `_Destroy_range<std::allocator<std / _Destroy_range<std::allocator<std::pair<long,CCompositeSurfaceInfo>_>_>`.
Identity: Own C3DMeshStats destructor00a2f190 uses MeshVolumes+88, original CVectorMap<unsigned long,C3DMeshVolume>. Deleting helper unreferenced; exact same member contract gives probable pair identity.
Layout: real pair16 has scalar key0 and original twelve-byte second at4; C3DMeshVolume Name0/ConvexVolume4, CCompositeSurfaceInfo CBaseClass vptr0/Cache4. PDB fields and own helper calls agree; no active record headers.

## 00a70e80: std::__destroy_aux
Original manifest label: `_Destroy_range<std::allocator<std / _Destroy_range<std::allocator<std::pair<long,CCompositeSurfaceInfo>_>_>`.
Identity: Own CPhysicsMesh destructor00a6d550 uses NavigationContainmentVolumes, original CVectorMap<long,C3DMeshVolume>; separate string-keyed map has two string destructors. Deleting helper unreferenced; pair identity probable.
Layout: real pair16 has scalar key0 and original twelve-byte second at4; C3DMeshVolume Name0/ConvexVolume4, CCompositeSurfaceInfo CBaseClass vptr0/Cache4. PDB fields and own helper calls agree; no active record headers.

## 00ba9fd0: std::__destroy_aux
Original manifest label: `_Destroy_range<std::allocator<std / _Destroy_range<std::allocator<std::pair<long,CCompositeSurfaceInfo>_>_>`.
Identity: Own reserve00beeb80 and original CVectorMap<long,CCompositeSurfaceInfo>; actual CBaseClass destructor0099a2e0, not a string. Deleting helper unreferenced; pair identity probable.
Layout: real pair16 has scalar key0 and original twelve-byte second at4; C3DMeshVolume Name0/ConvexVolume4, CCompositeSurfaceInfo CBaseClass vptr0/Cache4. PDB fields and own helper calls agree; no active record headers.

## 00a5f0b0: CStreamingFontDataBank::NotifyEntryRemoved
Original manifest label: `CStreamingFontDataBank / ?NotifyEntryRemoved@CStreamingFontDataBank@@MAEXK@Z`.
Identity: own vtable129d5cc slot14 and original PDB protected virtual method. Delete through original entry base; reread array after virtual deletion.
Layout: active owner/entry headers are flattened; restore original RTTI CBankFile/CBankFileAsync bases and CBaseClassNonCopyable entry. Shared bank272/async356 retained; Fonts at376. StateBlock opaque and untouched, real CArray pointers. Entry fields are never accessed, only its original virtual destructor.

## 00a61fc0: CShaderDataBank::NotifyEntryRemoved
Original manifest label: `CShaderDataBank / ?NotifyEntryRemoved@CShaderDataBank@@MAEXK@Z`.
Identity: own vtable129d644 slot14 and original PDB protected virtual method. Delete through original entry base; reread array after virtual deletion.
Layout: active owner/entry headers are flattened; restore original RTTI CBankFile/CBankFileAsync bases and CBaseClassNonCopyable entry. Shared bank272/async356 retained; Shaders at272. StateBlock opaque and untouched, real CArray pointers. Entry fields are never accessed, only its original virtual destructor.

## 00a635f0: CFontDataBank::NotifyEntryRemoved
Original manifest label: `CFontDataBank / ?NotifyEntryRemoved@CFontDataBank@@MAEXK@Z`.
Identity: own vtable129da84 slot14 and original PDB protected virtual method. Delete through original entry base; reread array after virtual deletion.
Layout: active owner/entry headers are flattened; restore original RTTI CBankFile/CBankFileAsync bases and CBaseClassNonCopyable entry. Shared bank272/async356 retained; Fonts at288. StateBlock opaque and untouched, real CArray pointers. Entry fields are never accessed, only its original virtual destructor.

## 00a7dc60: CVectorMap::LowerBound
Original manifest label: `CVectorMap<unsigned___int64 / LowerBound`.
Identity: Own CNavQuadTree::SetSwitchableNodesAsBlocked00a79240 and SwitchableNodes+64 prove mutable map<UID,CANavQuadTreeNode*>; prior pair class-key V corrected to original struct U.
Layout: original CArray/STLport vector12 with pair16, 8-byte-aligned UID. Comparer+12/Dirty+13.

## 00a84e80: CVectorMap::LowerBound
Original manifest label: `CVectorMap<unsigned___int64 / LowerBound`.
Identity: Own CNavSwitchableLeafNode::IsSwitchableOn00a84e40 is original PDB const virtual API; UniqueIDsOpen+32 original map<UID,bool>. Use const LowerBound/const iterator, not mutable placeholder.
Layout: original CArray/STLport vector12 with pair16, 8-byte-aligned UID. Comparer+12/Dirty+13.

## 00cb9840: CVectorMap::LowerBound
Original manifest label: `CVectorMap<unsigned___int64 / LowerBound`.
Identity: Original own script callers00cb7bc0/00cb88b0 and established map<UID,CCountedPointer<CActiveEntityScriptBase>> binding prove mutable LowerBound.
Layout: original CArray/STLport vector12 with pair16, 8-byte-aligned UID. Comparer+12/Dirty+13.

## 00a8d580: std::__uninitialized_copy
Original manifest label: `_gapscan / sub_00a8d580`.
Identity: descriptive pointee identity retained from own overflow00a8ddc0 and established helper binding. Own two copy callsites and count-block increment prove CCountedPointer, but no recovered pointee name is claimed.
Layout: Data0/Info4, control RefCount0; actual eight-byte source and destination stride.

## 00ad1650: std::__uninitialized_copy
Original manifest label: `_gapscan / sub_00ad1650`.
Identity: descriptive pointee identity retained from own overflow00ad68d0 and established helper binding. Own two copy callsites and count-block increment prove CCountedPointer, but no recovered pointee name is claimed.
Layout: Data0/Info4, control RefCount0; actual eight-byte source and destination stride.

## 00ad1740: std::__uninitialized_copy
Original manifest label: `_gapscan / sub_00ad1740`.
Identity: descriptive pointee identity retained from own overflow00ad69b0 and established helper binding. Own two copy callsites and count-block increment prove CCountedPointer, but no recovered pointee name is claimed.
Layout: Data0/Info4, control RefCount0; actual eight-byte source and destination stride.

## 00a97690: std::vector::operator=
Original manifest label: `_global / Vector_Assign_CountedPointerEntity`.
Identity: Own N3DPrimitive::CDeviceIndependent Groups at28 and caller00a90ed1; exact group pointer type independently confirmed by resize00a985a0 and N171.
Layout: original vector/base/allocator-proxy12, CCountedPointer8. Real visible copy/assign/destroy helpers read their inputs without modifying tags.

## 00aa9330: std::vector::operator=
Original manifest label: `_global / Vector_Assign_CountedPointerEntity_v2`.
Identity: Own CChunkedFileSuperChunk::InitNewInstance00aa8460 copies RegisteredSubChunkTypes at28, exact original CArray<CCountedPointer<CChunkedFileChunk>>.
Layout: original vector/base/allocator-proxy12, CCountedPointer8. Real visible copy/assign/destroy helpers read their inputs without modifying tags.

## 00ab30a0: std::vector::operator=
Original manifest label: `std / std::vector_AssignOther`.
Identity: Own C3DAnimationBuilder::GetIntersection00ab2160 calls twice at00ab2423/2434; both PDB name sets give its two CArray<CCountedPointer<C3DAnimFileSequenceChunk>>& arguments.
Layout: original vector/base/allocator-proxy12, CCountedPointer8. Real visible copy/assign/destroy helpers read their inputs without modifying tags.

## 00aa5a60: C3DMeshFileXTriangleStripChunk::CreateNewInstance
Original manifest label: `C3DMeshFileXTriangleStripChunk / C3DMeshFileXTriangleStripChunk::CreateNewInstance`.
Identity: own vtable129e380 slot9 and exact PDB const virtual method. Literal XTST, protected base constructor00aa7590 and virtual InitNewInstance slot10.
Layout: shared flattened headers aliased; real CBaseClassNonCopyable/Chunk/SuperChunk bases and original named arrays restored. Base16 or44, final28 or56. UnusedSlot names represent untouched virtual slots, not recovered names.

## 00abc8e0: C3DMeshFileTrianglesChunk::CreateNewInstance
Original manifest label: `C3DMeshFileTrianglesChunk / C3DMeshFileTrianglesChunk::CreateNewInstance`.
Identity: own vtable129ed34 slot9 and exact PDB const virtual method. Literal TRIS, protected base constructor00aa8360 and virtual InitNewInstance slot10.
Layout: shared flattened headers aliased; real CBaseClassNonCopyable/Chunk/SuperChunk bases and original named arrays restored. Base16 or44, final28 or56. UnusedSlot names represent untouched virtual slots, not recovered names.

## 00ac4fe0: CMirroredTextureHighlightsChunk::CreateNewInstance
Original manifest label: `CMirroredTextureHighlightsChunk / CMirroredTextureHighlightsChunk::CreateNewInstance`.
Identity: own vtable129e0e0 slot9 and exact PDB const virtual method. Literal MTEH, protected base constructor00aa7590 and virtual InitNewInstance slot10.
Layout: shared flattened headers aliased; real CBaseClassNonCopyable/Chunk/SuperChunk bases and original named arrays restored. Base16 or44, final28 or56. UnusedSlot names represent untouched virtual slots, not recovered names.

## 00aadfc0: CCountedPointer::DeleteData
Original manifest label: `CEngineWaterRenderer / DestroyWaterGenerator`.
Identity: Own LoadWaveFile00aad650 allocates24 bytes, Channels/SampleLength/SamplingFrequency and AudioData at12; Reset00aadff0 stores this callback. Exact original CCountedPointer<CWaveFile>::DeleteData PDB name.
Layout: genuine original CArray/std::vector/base/allocator-proxy. All owned array elements are trivial. Original fields retained, no active pointee header.

## 00ad5ed0: CCountedPointer::DeleteData
Original manifest label: `CEngineWaterRenderer / DestroyWaterGenerator`.
Identity: Own allocation00acbfc7 size40 and field stores Exists0/Unsplit1/SubContractions4/ListIterator36 bind callback00acc015; original nested type/PDB DeleteData. ListIterator unaccessed single-node-pointer projection.
Layout: genuine original CArray/std::vector/base/allocator-proxy. All owned array elements are trivial. Original fields retained, no active pointee header.

## 00b230e0: CCountedPointer::DeleteData
Original manifest label: `CEngineWaterRenderer / DestroyWaterGenerator`.
Identity: Own CPSCSplineBase::DatabaseCopy00b22530 constructor00b21a00 and Reset00b23170 bind this callback. Original nested CSplinePosParamData and SplinePosParam12, retail24. Outer CPSCSplineBase is naming scope only.
Layout: genuine original CArray/std::vector/base/allocator-proxy. All owned array elements are trivial. Original fields retained, no active pointee header.

## 00ab01e0: C3DAnimFileObjectChunk::GetChunkInfo
Original manifest label: `C3DAnimFileObjectChunk / C3DAnimFileObjectChunk::GetChunkInfo`.
Identity: own vtable129e904 slot2, exact original PDB const virtual class-valued return. Format literal129e940 and CRT _snprintfBFEB96; original named index fields.
Layout: active chunk headers flattened, alias them and restore original CBaseClassNonCopyable/Chunk/SuperChunk inheritance with genuine arrays. Retail SuperChunk44, Name44 and indices48..60; Bone indices44..56, LocalizationMatrix60, Name108. Shared submesh header confirms.

## 00aba390: C3DMeshFileSubMeshChunk::GetChunkInfo
Original manifest label: `C3DMeshFileSubMeshChunk / C3DMeshFileSubMeshChunk::GetChunkInfo`.
Identity: own vtable129eb18 slot2, exact original PDB const virtual class-valued return. Format literal129e940 and CRT _snprintfBFEB96; original named index fields.
Layout: active chunk headers flattened, alias them and restore original CBaseClassNonCopyable/Chunk/SuperChunk inheritance with genuine arrays. Retail SuperChunk44, Name44 and indices48..60; Bone indices44..56, LocalizationMatrix60, Name108. Shared submesh header confirms.

## 00abadd0: C3DMeshFileBoneChunk::GetChunkInfo
Original manifest label: `C3DMeshFileBoneChunk / C3DMeshFileBoneChunk::GetChunkInfo`.
Identity: own vtable129eb9c slot2, exact original PDB const virtual class-valued return. Format literal129e940 and CRT _snprintfBFEB96; original named index fields.
Layout: active chunk headers flattened, alias them and restore original CBaseClassNonCopyable/Chunk/SuperChunk inheritance with genuine arrays. Retail SuperChunk44, Name44 and indices48..60; Bone indices44..56, LocalizationMatrix60, Name108. Shared submesh header confirms.

## 00ac8680: QuadricOptimiserInternals::COptimisedMesh::CollapseSegments
Original manifest label: `_gapscan / sub_00ac8680`.
Identity: proven; own COptimisedMesh RTTI chain C3DOptimisedMeshBase/CBaseClassNonCopyable/CBaseClass, SubMeshes+4 stride24 and Primitives counted-pointer stride8; exact original PDB owner/callee CollapseSegments(float) APIs and own distinct callee00AC85D0; Initialise callee copies actual40-byte parameter object, CollapseSegments/Optimise float thresholds; no donor-address mapping used.
Layout: original mesh base hierarchy, CArray/std::vector storage, real COptimisedSubMesh two arrays and CCountedPointer<Data,Info>. Shared primitive data header included under alias as evidence; callable primitive declaration supplies only actual nonvirtual API and no fields/sizeof are used. Original primitive has no base. No shared mesh/submesh/parameter header; original donor member names/types reconcile24-byte retail submesh against32-byte donor.

## 00acd500: QuadricOptimiserInternals::COptimisedMesh::Initialise
Original manifest label: `COptimisedMesh / CollapseSegments`.
Identity: proven; own COptimisedMesh RTTI chain C3DOptimisedMeshBase/CBaseClassNonCopyable/CBaseClass, SubMeshes+4 stride24 and Primitives counted-pointer stride8; exact original PDB owner/callee Initialise(const COptimisationParameters&) APIs and own distinct callee00ACA830; Initialise callee copies actual40-byte parameter object, CollapseSegments/Optimise float thresholds; no donor-address mapping used.
Layout: original mesh base hierarchy, CArray/std::vector storage, real COptimisedSubMesh two arrays and CCountedPointer<Data,Info>. Shared primitive data header included under alias as evidence; callable primitive declaration supplies only actual nonvirtual API and no fields/sizeof are used. Original primitive has no base. No shared mesh/submesh/parameter header; original donor member names/types reconcile24-byte retail submesh against32-byte donor.

## 00acdae0: QuadricOptimiserInternals::COptimisedMesh::Optimise
Original manifest label: `_gapscan / sub_00acdae0`.
Identity: proven; own COptimisedMesh RTTI chain C3DOptimisedMeshBase/CBaseClassNonCopyable/CBaseClass, SubMeshes+4 stride24 and Primitives counted-pointer stride8; exact original PDB owner/callee Optimise(float) APIs and own distinct callee00ACD960; Initialise callee copies actual40-byte parameter object, CollapseSegments/Optimise float thresholds; no donor-address mapping used.
Layout: original mesh base hierarchy, CArray/std::vector storage, real COptimisedSubMesh two arrays and CCountedPointer<Data,Info>. Shared primitive data header included under alias as evidence; callable primitive declaration supplies only actual nonvirtual API and no fields/sizeof are used. Original primitive has no base. No shared mesh/submesh/parameter header; original donor member names/types reconcile24-byte retail submesh against32-byte donor.

## 00ae04e0: CParticleProgram::GetMatrixBuffer
Original manifest label: `_global / GetGroupVectorBuffer`.
Identity: proven; both PDB exact private static fastcall CParticleProgram::GetMatrixBuffer(ulong) and own PrepareForRendering/ClingToMesh/TransformVertices caller roles; original element CMatrix4x4 stride64 and genuine CSimpleMemoryPool::AllocateType guard; FreeAll00A5C720/Allocate00A5C910 exact PDB calls; pool MatrixBufferPool original dynamic-initializer name, buffer/capacity globals descriptive at013D2D98/013D2D9C, pool013D2DB0.
Layout: shared pool/program data headers retained unchanged under aliases as evidence. Own RTTI restores CSimpleMemoryPool : CBaseClassNonCopyable : CBaseClass and CParticleProgram : CMovableResource. Pool nested CMemoryBlock/CArea names and original Size/NextFreeArea/Status fields restored; enum scope/enumerator are descriptive where no PDB key exists. Program only supplies static naming scope, no owner fields/sizeof used. Declared callable methods are original APIs. Original CMatrix4x4/C4DVector POD data have no shared headers. Checked allocator uses the actual64-bit product and32-bit byte limit, preserving failed allocation capacity behavior.

## 00ae0540: CParticleProgram::GetVectorBuffer
Original manifest label: `_global / GetGroupVectorBuffer`.
Identity: proven; both PDB exact private static fastcall CParticleProgram::GetVectorBuffer(ulong) and own PrepareForRendering/ClingToMesh/TransformVertices caller roles; original element C4DVector stride16 and genuine CSimpleMemoryPool::AllocateType guard; FreeAll00A5C720/Allocate00A5C910 exact PDB calls; pool VectorBufferPool original dynamic-initializer name, buffer/capacity globals descriptive at013D2DA0/013D2DA4, pool013D2DCC.
Layout: shared pool/program data headers retained unchanged under aliases as evidence. Own RTTI restores CSimpleMemoryPool : CBaseClassNonCopyable : CBaseClass and CParticleProgram : CMovableResource. Pool nested CMemoryBlock/CArea names and original Size/NextFreeArea/Status fields restored; enum scope/enumerator are descriptive where no PDB key exists. Program only supplies static naming scope, no owner fields/sizeof used. Declared callable methods are original APIs. Original CMatrix4x4/C4DVector POD data have no shared headers. Checked allocator uses the actual64-bit product and32-bit byte limit, preserving failed allocation capacity behavior.

## 00ae05a0: CParticleProgram::GetGroupVectorBuffer
Original manifest label: `_global / GetGroupVectorBuffer`.
Identity: proven; both PDB exact private static fastcall CParticleProgram::GetGroupVectorBuffer(ulong) and own PrepareForRendering/ClingToMesh/TransformVertices caller roles; original element C4DVector stride16 and genuine CSimpleMemoryPool::AllocateType guard; FreeAll00A5C720/Allocate00A5C910 exact PDB calls; pool GroupVectorBufferPool original dynamic-initializer name, buffer/capacity globals descriptive at013D2DA8/013D2DAC, pool013D2DE8.
Layout: shared pool/program data headers retained unchanged under aliases as evidence. Own RTTI restores CSimpleMemoryPool : CBaseClassNonCopyable : CBaseClass and CParticleProgram : CMovableResource. Pool nested CMemoryBlock/CArea names and original Size/NextFreeArea/Status fields restored; enum scope/enumerator are descriptive where no PDB key exists. Program only supplies static naming scope, no owner fields/sizeof used. Declared callable methods are original APIs. Original CMatrix4x4/C4DVector POD data have no shared headers. Checked allocator uses the actual64-bit product and32-bit byte limit, preserving failed allocation capacity behavior.

## 00af2250: NParticleEngine::CParticleSystem::GetSystemTransform
Original manifest label: `CParticleSystem / GetInverseSystemTransform`.
Identity: probable helper name only; target owner/API proven; target exact original NParticleEngine::CParticleSystem PDB void(CMatrix3x4&)const API; own Scale+96/100/104 and matrix48 copy; helper00AF3D80 own body returns PParentEmitter+4 then Transform+0xB0; helper spelling reused existing binding, not present in PDB name sets.
Layout: no active particle-system header; quarantined shared header retained unchanged as layout evidence only; callable CParticleSystem and CParticleUpdateParameters restore actual RTTI CBaseClass inheritance. Original typed CVectorMap/CArray/STLport vector/list members, all eight original bool flags, C3DVector Scale, and CCharString Name replace flattened storage. UpdateParameters includes its own CBaseClass vptr; PC sizeof0x70 verified. EPSCType zero enumerator is descriptive and unused. Incomplete component/update-description value types occur only behind genuine vector storage pointers; no invented element layout. Named Scale agrees with original donor member and own retail offsets. CMatrix3x4/C3DVector have no shared headers; original four three-float rows. Final translation row remains copied and unscaled.

## 00af22c0: NParticleEngine::CParticleSystem::GetInverseSystemTransform
Original manifest label: `CParticleSystem / GetInverseSystemTransform`.
Identity: probable helper name only; target owner/API proven; target exact original NParticleEngine::CParticleSystem PDB void(CMatrix3x4&)const API; own Scale+96/100/104 and matrix48 copy; helper00AF3DA0 own body returns PParentEmitter+4 then InverseTransform+0xE0; helper spelling reused existing binding, not present in PDB name sets.
Layout: no active particle-system header; quarantined shared header retained unchanged as layout evidence only; callable CParticleSystem and CParticleUpdateParameters restore actual RTTI CBaseClass inheritance. Original typed CVectorMap/CArray/STLport vector/list members, all eight original bool flags, C3DVector Scale, and CCharString Name replace flattened storage. UpdateParameters includes its own CBaseClass vptr; PC sizeof0x70 verified. EPSCType zero enumerator is descriptive and unused. Incomplete component/update-description value types occur only behind genuine vector storage pointers; no invented element layout. Named Scale agrees with original donor member and own retail offsets. CMatrix3x4/C3DVector have no shared headers; original four three-float rows. Final translation row remains copied and unscaled.

## 00b05b10: NParticleEngine::CParticleSystem::GetSystemEndTransform
Original manifest label: `CParticleSystem / GetInverseSystemTransform`.
Identity: probable helper name only; target owner/API proven; target exact original NParticleEngine::CParticleSystem PDB void(CMatrix3x4&)const API; own Scale+96/100/104 and matrix48 copy; helper00AF3D90 own body returns PParentEmitter+4 then EndTransform+0x110; helper spelling explicit descriptive name, no credible original binding.
Layout: no active particle-system header; quarantined shared header retained unchanged as layout evidence only; callable CParticleSystem and CParticleUpdateParameters restore actual RTTI CBaseClass inheritance. Original typed CVectorMap/CArray/STLport vector/list members, all eight original bool flags, C3DVector Scale, and CCharString Name replace flattened storage. UpdateParameters includes its own CBaseClass vptr; PC sizeof0x70 verified. EPSCType zero enumerator is descriptive and unused. Incomplete component/update-description value types occur only behind genuine vector storage pointers; no invented element layout. Named Scale agrees with original donor member and own retail offsets. CMatrix3x4/C3DVector have no shared headers; original four three-float rows. Final translation row remains copied and unscaled.

## 00af8680: NParticleEngine::CRandomAcceleration::operator new
Original manifest label: `_global / operator_new`.
Identity: proven; own allocator callers and actual sibling deleting destructor/RTTI, exact original PDB static fastcall operatornew API; real CDynamicObjectHeap<24,853> instance RandomAccelerationHeap (existing descriptive global), FreeList+20/AllocCount+4; own AllowHeapToGrow at013D7E4C is member after embedded StartBlock, not a separate global; CreateMoreBlocks00AF88A0 original private template method.
Layout: real dynamic-heap base20 and nested embedded blocks with actual ObjectSize/BlockCount, AllowHeapToGrow after block link. No shared heap header. Owner declarations supply static operator naming only; no owner object fields/sizeof accessed. Particle/Random actual CBaseClassNonCopyable hierarchy retained; Decal has no base.

## 00af86c0: NParticleEngine::CParticle::operator new
Original manifest label: `_global / operator_new`.
Identity: proven; own allocator callers and actual sibling deleting destructor/RTTI, exact original PDB static fastcall operatornew API; real CDynamicObjectHeap<156,2100> instance ParticleHeap (existing descriptive global), FreeList+20/AllocCount+4; own AllowHeapToGrow at01427E1C is member after embedded StartBlock, not a separate global; CreateMoreBlocks00AF88F0 original private template method.
Layout: real dynamic-heap base20 and nested embedded blocks with actual ObjectSize/BlockCount, AllowHeapToGrow after block link. No shared heap header. Owner declarations supply static operator naming only; no owner object fields/sizeof accessed. Particle/Random actual CBaseClassNonCopyable hierarchy retained; Decal has no base.

## 00af8b00: NParticleEngine::CDecal::operator new
Original manifest label: `_global / operator_new`.
Identity: proven; own allocator callers and actual sibling deleting destructor/RTTI, exact original PDB static fastcall operatornew API; real CDynamicObjectHeap<116,529> instance DecalHeap (existing descriptive global), FreeList+20/AllocCount+4; own AllowHeapToGrow at01436DF0 is member after embedded StartBlock, not a separate global; CreateMoreBlocks00AF8C00 original private template method.
Layout: real dynamic-heap base20 and nested embedded blocks with actual ObjectSize/BlockCount, AllowHeapToGrow after block link. No shared heap header. Owner declarations supply static operator naming only; no owner object fields/sizeof accessed. Particle/Random actual CBaseClassNonCopyable hierarchy retained; Decal has no base.

## 00afadc0: NParticleEngine::CPSCRenderSprite::ClearAllPrimitives
Original manifest label: `CPSCDecalRenderer / ClearAllPrimitives`.
Identity: proven; own complete RTTI/vtable slot29 and both PDB exact CPSCRenderSprite::ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&) API; actual CPSCBase/CParticleUpdateParamCallback<ulong,ulong>/CBaseClassNonCopyable/CBaseClass hierarchy; own PrimHandle+0x3c/TrailPrimHandle+0x48 match original donor named handles; CIEngine actual CInitBaseClass/CBaseClass chain slot31->CEngine00B23C70 ClearAllChildPrimitives; original primitive CIVCountedPointeeBase/CEnginePersistentPrimitiveBase count+4 and own last-handle virtualslot3/Delete slot1 clear behavior.
Layout: shared count/persistent/engine headers retained unchanged as evidence; real original base chains and genuine CIVCountedPointer<T>/CEnginePrimitiveHandle restore flattened fields. Renderer parameter bytes explicitly unmodeled (not used by target), preserving exact original named handle offsets. No renderer header exists. Actual ClearAllPrimitives virtualslot29 restored on original CPSCBase; unknown unused component slots1-28, engine slots1-30 and primitive slot2 are descriptive; actual accessed virtual APIs/slots declared. Original second argument is unused, retained with real reference type.

## 00b01250: NParticleEngine::CPSCRenderMesh::ClearAllPrimitives
Original manifest label: `CPSCDecalRenderer / ClearAllPrimitives`.
Identity: proven; own complete RTTI/vtable slot29 and both PDB exact CPSCRenderMesh::ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&) API; actual CPSCBase/CParticleUpdateParamCallback<ulong,ulong>/CBaseClassNonCopyable/CBaseClass hierarchy; own PrimHandle+0x4c/TrailPrimHandle+0x58 match original donor named handles; CIEngine actual CInitBaseClass/CBaseClass chain slot31->CEngine00B23C70 ClearAllChildPrimitives; original primitive CIVCountedPointeeBase/CEnginePersistentPrimitiveBase count+4 and own last-handle virtualslot3/Delete slot1 clear behavior.
Layout: shared count/persistent/engine headers retained unchanged as evidence; real original base chains and genuine CIVCountedPointer<T>/CEnginePrimitiveHandle restore flattened fields. Renderer parameter bytes explicitly unmodeled (not used by target), preserving exact original named handle offsets. No renderer header exists. Actual ClearAllPrimitives virtualslot29 restored on original CPSCBase; unknown unused component slots1-28, engine slots1-30 and primitive slot2 are descriptive; actual accessed virtual APIs/slots declared. Original second argument is unused, retained with real reference type.

## 00b1dbe0: NParticleEngine::CPSCDecalRenderer::ClearAllPrimitives
Original manifest label: `CPSCDecalRenderer / ClearAllPrimitives`.
Identity: proven; own complete RTTI/vtable slot29 and both PDB exact CPSCDecalRenderer::ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&) API; actual CPSCBase/CParticleUpdateParamCallback<ulong,ulong>/CBaseClassNonCopyable/CBaseClass hierarchy; own LandscapeDecalsPrimHandle+0x4c/MeshDecalsPrimHandle+0x58 match original donor named handles; CIEngine actual CInitBaseClass/CBaseClass chain slot31->CEngine00B23C70 ClearAllChildPrimitives; original primitive CIVCountedPointeeBase/CEnginePersistentPrimitiveBase count+4 and own last-handle virtualslot3/Delete slot1 clear behavior.
Layout: shared count/persistent/engine headers retained unchanged as evidence; real original base chains and genuine CIVCountedPointer<T>/CEnginePrimitiveHandle restore flattened fields. Renderer parameter bytes explicitly unmodeled (not used by target), preserving exact original named handle offsets. No renderer header exists. Actual ClearAllPrimitives virtualslot29 restored on original CPSCBase; unknown unused component slots1-28, engine slots1-30 and primitive slot2 are descriptive; actual accessed virtual APIs/slots declared. Original second argument is unused, retained with real reference type.

## 00b48be0: CDynamicObjectHeap<12,16>::CDynamicObjectHeap
Original manifest label: `CDynamicObjectHeapBase / CDynamicObjectHeapBase::CDynamicObjectHeapBase_SmallPool`.
Identity: proven; own vtable012A20E4 complete RTTI CDynamicObjectHeap<12,16> and both PDB constructor(bool) API; own FreeList+14/StartBlock+18 and member growflag+DC; original base constructor00F36C60 and original CDynamicObjectHeapBase RTTI; exact embedded16 entries of stride12.
Layout: genuine original CDynamicObjectHeapBase primary virtual base20 and original template/nested block types, same source structure as landed24/853 constructor. No shared heap header. Original Release slot1 declared. Full embedded block and trailing grow flag retained; no foreign holder or separate flag global.

## 00b48cb0: CDynamicObjectHeap<72,16>::CDynamicObjectHeap
Original manifest label: `CDynamicObjectHeapBase / CDynamicObjectHeapBase::CDynamicObjectHeapBase_LargePool`.
Identity: proven; own vtable012A20F0 complete RTTI CDynamicObjectHeap<72,16> and both PDB constructor(bool) API; own FreeList+14/StartBlock+18 and member growflag+49C; original base constructor00F36C60 and original CDynamicObjectHeapBase RTTI; exact embedded16 entries of stride72.
Original base contract: own full RTTI and N357 prove CDynamicObjectHeapBase private pure Release and derived private override. Keep per-iteration FreeList stores: this retail16-entry loop reloads/stores the member each compiler-unrolled group, unlike large static heaps with a final publication.
Layout: genuine original CDynamicObjectHeapBase primary virtual base20 and original template/nested block types, same source structure as landed24/853 constructor. No shared heap header. Original Release slot1 declared. Full embedded block and trailing grow flag retained; no foreign holder or separate flag global.

## 00b48d80: CDynamicObjectHeap<56,16>::CDynamicObjectHeap
Original manifest label: `CDynamicObjectHeapBase / CDynamicObjectHeapBase::CDynamicObjectHeapBase_MediumPool`.
Identity: proven; own vtable012A20FC complete RTTI CDynamicObjectHeap<56,16> and both PDB constructor(bool) API; own FreeList+14/StartBlock+18 and member growflag+39C; original base constructor00F36C60 and original CDynamicObjectHeapBase RTTI; exact embedded16 entries of stride56.
Original base contract: own full RTTI and N357 prove CDynamicObjectHeapBase private pure Release and derived private override. Keep per-iteration FreeList stores: this retail16-entry loop reloads/stores the member each compiler-unrolled group, unlike large static heaps with a final publication.
Layout: genuine original CDynamicObjectHeapBase primary virtual base20 and original template/nested block types, same source structure as landed24/853 constructor. No shared heap header. Original Release slot1 declared. Full embedded block and trailing grow flag retained; no foreign holder or separate flag global.

## 00b6df50: CEngineWaterRenderer::GetIceForegroundVertexShader
Original manifest label: `CEngineLandscapeRenderer / GetForegroundSpotLightVertexShader`.
Identity: proven; own RenderIceForeground caller00B75B6C and original PDB private GetIceForegroundVertexShader(long)->CVertexShader& API; own array offset0x12c exactly shared Water header; propagated LandscapeRenderer/SpotLight names false.
Layout: shared CVertexShader included unchanged; shared CEngineWaterRenderer data header reused under alias, typed CArray/STLport-vector projection over named opaque shader array. Method is original private nonvirtual API. Requires a nonempty shader array, as retail.

## 00b6df90: CEngineWaterRenderer::GetIceForegroundShadowedVertexShader
Original manifest label: `CEngineLandscapeRenderer / GetForegroundSpotLightVertexShader`.
Identity: proven; own RenderIceForeground caller and original private PDB API, original Water array offsets138/144; own RTTI CEngineWaterRenderer : CEngineComponent.
Layout: genuine original CEngineComponent primary virtual base12 with original GlobalPointerAddress/RenderEnabled/KeepAlive fields. Shared component/water headers retained unchanged under layout aliases; source restores genuine typed CArray/STLport vector shader arrays. Unused intervening buffers/shaders explicitly unmodeled; no array rebinding over fake storage. Shared CVertexShader data header unchanged. Capped00B6DF50 representative retains separate original-base debt; this correction applies only to these two siblings.

## 00b6dfd0: CEngineWaterRenderer::GetIceForegroundColbuffShadowedVertexShader
Original manifest label: `CEngineLandscapeRenderer / GetForegroundSpotLightVertexShader`.
Identity: proven; own RenderIceForeground caller and original private PDB API, original Water array offsets138/144; own RTTI CEngineWaterRenderer : CEngineComponent.
Layout: genuine original CEngineComponent primary virtual base12 with original GlobalPointerAddress/RenderEnabled/KeepAlive fields. Shared component/water headers retained unchanged under layout aliases; source restores genuine typed CArray/STLport vector shader arrays. Unused intervening buffers/shaders explicitly unmodeled; no array rebinding over fake storage. Shared CVertexShader data header unchanged. Capped00B6DF50 representative retains separate original-base debt; this correction applies only to these two siblings.

## 00bb8690: CEnginePrimitiveManagerParticleDecalGroup::CreateInternalPrimitive
Original manifest label: `CEnginePrimitiveManagerParticleDecalGroup / CEnginePrimitiveManagerParticleDecalGroup::CreateInternalPrimitive`.
Identity: proven; own manager RTTI virtualslot1 and original PDB exact CreateInternalPrimitive(EEnginePrimitiveType) API; own created RTTI CEngineInternalPrimitiveParticleDecalGroup / CEngineInternalPrimitiveParticleDecal with original CIVCountedPointeeBase/CEnginePersistentPrimitiveBase/InternalPrimitiveBase/ParticleGroup chains; group/small IDs30/25 and flags0x4000/0x4000, actual allocation sizes196/124; external original CEngineInternalPrimitiveBase(long,ulong)00B8FF00 exact PDB JK (not enum prototype). Group header/list/vector/named sphere/default ctor store evidence and original typed members.
Layout: shared base/group/manager/quarantined derived headers retained unchanged as evidence; local real original bases and typed member definitions correct flattened data. Genuine STLport list<CEnginePrimitiveHandle>/list<long> sentinels16/12 and vector-based CArray<CPrimitiveIterator>. Actual C3DBoundingSphere default zeros only its four floats; C3DVector default leaves scalar fields untouched. Unused enum member constants descriptive. Full actual PC sizes compiler-checked; no raw new-size substitute.

## 00bc1420: CEnginePrimitiveManagerParticleSpriteGroup::CreateInternalPrimitive
Original manifest label: `CEnginePrimitiveManagerParticleSpriteGroup / CEnginePrimitiveManagerParticleSpriteGroup::CreateInternalPrimitive`.
Identity: proven; own manager RTTI virtualslot1 and original PDB exact CreateInternalPrimitive(EEnginePrimitiveType) API; own created RTTI CEngineInternalPrimitiveParticleSpriteGroup / CEngineInternalPrimitiveParticleSprite with original CIVCountedPointeeBase/CEnginePersistentPrimitiveBase/InternalPrimitiveBase/ParticleGroup chains; group/small IDs27/22 and flags0x88000/0x88000, actual allocation sizes188/104; external original CEngineInternalPrimitiveBase(long,ulong)00B8FF00 exact PDB JK (not enum prototype). Group header/list/vector/named sphere/default ctor store evidence and original typed members.
Layout: shared base/group/manager/quarantined derived headers retained unchanged as evidence; local real original bases and typed member definitions correct flattened data. Genuine STLport list<CEnginePrimitiveHandle>/list<long> sentinels16/12 and vector-based CArray<CPrimitiveIterator>. Actual C3DBoundingSphere default zeros only its four floats; C3DVector default leaves scalar fields untouched. Unused enum member constants descriptive. Full actual PC sizes compiler-checked; no raw new-size substitute.

## 00bc4cd0: CEnginePrimitiveManagerParticleLineGroup::CreateInternalPrimitive
Original manifest label: `CEnginePrimitiveManagerParticleLineGroup / CEnginePrimitiveManagerParticleLineGroup::CreateInternalPrimitive`.
Identity: proven; own manager RTTI virtualslot1 and original PDB exact CreateInternalPrimitive(EEnginePrimitiveType) API; own created RTTI CEngineInternalPrimitiveParticleLineGroup / CEngineInternalPrimitiveParticleLine with original CIVCountedPointeeBase/CEnginePersistentPrimitiveBase/InternalPrimitiveBase/ParticleGroup chains; group/small IDs26/21 and flags0x88000/0x188000, actual allocation sizes188/104; external original CEngineInternalPrimitiveBase(long,ulong)00B8FF00 exact PDB JK (not enum prototype). Group header/list/vector/named sphere/default ctor store evidence and original typed members.
Layout: shared base/group/manager/quarantined derived headers retained unchanged as evidence; local real original bases and typed member definitions correct flattened data. Genuine STLport list<CEnginePrimitiveHandle>/list<long> sentinels16/12 and vector-based CArray<CPrimitiveIterator>. Actual C3DBoundingSphere default zeros only its four floats; C3DVector default leaves scalar fields untouched. Unused enum member constants descriptive. Full actual PC sizes compiler-checked; no raw new-size substitute.

## 00bda090: CEngineLandscapeMemoryManager::DeleteIndexBufferBlockWhileBusy
Original manifest label: `_global / EdgeList_AppendNode`.
Identity: proven; both PDB exact original private DeleteIndexBufferBlockWhileBusy(CIndexBuffer*) API; own CurrentDisposalListIndex+34 selects three STLport list sentinel pointers at+28; source fields agree shared header and donor names; original multiple bases CFailedAllocationHandler@0/CMovableResource@4 proven RTTI; malloc00BFEA0E exactimport .
Layout: shared manager header retained unchanged under evidence alias; real original two polymorphic bases and typed std::list<T*>[3] members restore flattened data. Genuine STLport _List_base, class iterator with original _Nonconst_traits and copy constructor, protected _M_insert(iterator,constT&), node allocation, placement construction and tail insertion. No resource fields/sizeof used. CMovableResource Verify and CanRelocateData APIs declared; header offers only flattened vptr.

## 00bda0d0: CEngineLandscapeMemoryManager::DeleteVertexBufferBlockWhileBusy
Original manifest label: `LandscapeLayerList / LandscapeLayerList::InsertNode`.
Identity: proven; both PDB exact original private DeleteVertexBufferBlockWhileBusy(CVertexBufferWin32*) API; own CurrentDisposalListIndex+34 selects three STLport list sentinel pointers at+1C; source fields agree shared header and donor names; original multiple bases CFailedAllocationHandler@0/CMovableResource@4 proven RTTI; malloc00BFEA0E exactimport .
Layout: shared manager header retained unchanged under evidence alias; real original two polymorphic bases and typed std::list<T*>[3] members restore flattened data. Genuine STLport _List_base, class iterator with original _Nonconst_traits and copy constructor, protected _M_insert(iterator,constT&), node allocation, placement construction and tail insertion. No resource fields/sizeof used. CMovableResource Verify and CanRelocateData APIs declared; header offers only flattened vptr.

## 00bda110: CEngineLandscapeMemoryManager::DeleteTextureBlockWhileBusy
Original manifest label: `_global / CLandscapeBackgroundPatch_InsertTexture`.
Identity: proven; both PDB exact original private DeleteTextureBlockWhileBusy(CTexture*) API; own CurrentDisposalListIndex+34 selects three STLport list sentinel pointers at+10; source fields agree shared header and donor names; original multiple bases CFailedAllocationHandler@0/CMovableResource@4 proven RTTI; malloc00BFEA0E exactimport .
Layout: shared manager header retained unchanged under evidence alias; real original two polymorphic bases and typed std::list<T*>[3] members restore flattened data. Genuine STLport _List_base, class iterator with original _Nonconst_traits and copy constructor, protected _M_insert(iterator,constT&), node allocation, placement construction and tail insertion. No resource fields/sizeof used. CMovableResource Verify and CanRelocateData APIs declared; header offers only flattened vptr.

## 00bdd910: CLocalDetailCacheMap::CObjectTypeCollection::CanRelocateData
Original manifest label: `CLocalDetailCacheMap::CObjectTypeCollection / CLocalDetailCacheMap::CObjectTypeCollection::vfunc_2`.
Identity: proven; own RTTI 012A7D9C slot2 and both PDB CanRelocateData(long)const bool API; original CMovableResource base4; own Map+0x10 and original Map.LockedFromMemoryRelocation byte0.
Layout: shared CLocalDetailCacheMap data header reused unchanged under alias, including actual lock byte0; outer class adds nested naming scopes only. CMovableResource header flattens virtual interface, so genuine virtual base4 and required slot2 are modeled locally. Unused record prefix preserves actual Map member offsets.

## 00bdda10: CLocalDetailCacheMap::CObjectCacheGroupCollection::CanRelocateData
Original manifest label: `CLocalDetailCacheMap::CObjectCacheGroupCollection / CLocalDetailCacheMap::CObjectCacheGroupCollection::vfunc_2`.
Identity: proven; own RTTI 012A7DDC slot2 and both PDB CanRelocateData(long)const bool API; original CMovableResource base4; own Map+0x24 and original Map.LockedFromMemoryRelocation byte0.
Layout: shared CLocalDetailCacheMap data header reused unchanged under alias, including actual lock byte0; outer class adds nested naming scopes only. CMovableResource header flattens virtual interface, so genuine virtual base4 and required slot2 are modeled locally. Unused record prefix preserves actual Map member offsets.

## 00bf9a80: CLocalDetailPrimitiveBase::CanRelocateData
Original manifest label: `CLocalDetailPrimitiveBase / CLocalDetailPrimitiveBase::vfunc_2`.
Identity: proven; own RTTI 012A7DB0 slot2 and both PDB CanRelocateData(long)const bool API; original CMovableResource base4; own Map+0x34 and original Map.LockedFromMemoryRelocation byte0.
Layout: shared CLocalDetailCacheMap data header reused unchanged under alias, including actual lock byte0; outer class adds nested naming scopes only. CMovableResource header flattens virtual interface, so genuine virtual base4 and required slot2 are modeled locally. Unused record prefix preserves actual Map member offsets.

## 00be8e60: CLandscapeBackgroundPatch::scalar deleting destructor
Original manifest label: `CLandscapeBackgroundPatch / `vector_deleting_destructor'`.
Identity: proven; own RTTI scalar deletingdtor slot0 and exact original destructor API; original CLandscapeBackgroundPatch : CLandscapeBackgroundSubNodeBase : CMovableResource hierarchy; target invokes actual dtor00BE6090 then conditionalbit1 classdelete; global01436EA8 actual CEngineLandscapeRenderer* with MemoryManager+6B0 and original CEngineLandscapeMemoryManager::FreeVirtual(void*)00BD9B90 exactPDB. Legacy vector-deleting label rejected; ret4 flagsbit1 and single dtor prove scalar form.
Layout: compiler generates the deleting destructor from real virtual destructor declaration and actual class delete. Original base chains and typed owner members retained; PC full object size checked. Shared headers included unchanged as evidence or passive renderer layout; flattened active types reconstructed. CPatch edge strips restore original CBaseClassNonCopyable hierarchy and typed arrays. ForceNew only makes the actual vtable/deleting helper materialize. No hand-written destructor-flags wrapper.

## 00bfbd80: CLocalDetailPrimitiveMeshZSpriteBatch::scalar deleting destructor
Original manifest label: `CLandscapeBackgroundPatch / `vector_deleting_destructor'`.
Identity: proven; own RTTI scalar deletingdtor slot0 and exact original destructor API; original CLocalDetailPrimitiveMeshZSpriteBatch : CLocalDetailPrimitiveBase : CMovableResource hierarchy; target invokes actual dtor00BFB590 then conditionalbit1 classdelete; global01436EA8 actual CEngineLandscapeRenderer* with MemoryManager+6B0 and original CEngineLandscapeMemoryManager::FreeVirtual(void*)00BD9B90 exactPDB. Legacy vector-deleting label rejected; ret4 flagsbit1 and single dtor prove scalar form.
Layout: compiler generates the deleting destructor from real virtual destructor declaration and actual class delete. Original base chains and typed owner members retained; PC full object size checked. Shared headers included unchanged as evidence or passive renderer layout; flattened active types reconstructed. CPatch edge strips restore original CBaseClassNonCopyable hierarchy and typed arrays. ForceNew only makes the actual vtable/deleting helper materialize. No hand-written destructor-flags wrapper.

## 00bfe900: CLandscapeLayerMesh::scalar deleting destructor
Original manifest label: `CLandscapeBackgroundPatch / `vector_deleting_destructor'`.
Identity: proven; own RTTI scalar deletingdtor slot0 and exact original destructor API; original CLandscapeLayerMesh : CMovableResource : CMovableResource hierarchy; target invokes actual dtor00BFDE20 then conditionalbit1 classdelete; global01436EA8 actual CEngineLandscapeRenderer* with MemoryManager+6B0 and original CEngineLandscapeMemoryManager::FreeVirtual(void*)00BD9B90 exactPDB. Legacy vector-deleting label rejected; ret4 flagsbit1 and single dtor prove scalar form.
Layout: compiler generates the deleting destructor from real virtual destructor declaration and actual class delete. Original base chains and typed owner members retained; PC full object size checked. Shared headers included unchanged as evidence or passive renderer layout; flattened active types reconstructed. CPatch edge strips restore original CBaseClassNonCopyable hierarchy and typed arrays. ForceNew only makes the actual vtable/deleting helper materialize. No hand-written destructor-flags wrapper.

## 00bfcf00: CPatchTesselationEdgeStrip::SaveVerts<CVertex>
Original manifest label: `SaveVerts<CPatchTesselationEdgeStrip / SaveVerts<CPatchTesselationEdgeStrip::CFanBaseVertex>`.
Identity: proven; both PDB exact private CPatchTesselationEdgeStrip::SaveVerts<CVertex>(CEdgeStipVertexArray<CVertex>&,CDataOutputStream&) API; own Save00BFC770 passes field+8 and stride20 matches original nested struct; WriteSLONG00993EB0 and CRangeCompressor::Compress(constvoid*,long,long,CArray<byte>&)00F3AFC0 exact original APIs; stream Write virtualslot4, free00BFEA14.
Layout: original CBaseClass/CBaseClassNonCopyable/CPatch hierarchy and full original typed edge arrays; nested vertex fields from donor reconciled to actual strides. Genuine CArray/std::vector/_Vector_base/_STLP_alloc_proxy byte buffer and free in base destructor. Shared CDataOutputStream header retained as evidence, callable real CDataStream hierarchy and typed CCharString NewLine restored. Unused virtual slots1-3 explicitly descriptive; only actual Write slot4 invoked. Empty compressor occupies a dead argument slot naturally.

## 00bfcf90: CPatchTesselationEdgeStrip::SaveVerts<CFanBaseVertex>
Original manifest label: `SaveVerts<CPatchTesselationEdgeStrip / SaveVerts<CPatchTesselationEdgeStrip::CFanBaseVertex>`.
Identity: proven; both PDB exact private CPatchTesselationEdgeStrip::SaveVerts<CFanBaseVertex>(CEdgeStipVertexArray<CFanBaseVertex>&,CDataOutputStream&) API; own Save00BFC770 passes field+16 and stride16 matches original nested struct; WriteSLONG00993EB0 and CRangeCompressor::Compress(constvoid*,long,long,CArray<byte>&)00F3AFC0 exact original APIs; stream Write virtualslot4, free00BFEA14.
Layout: original CBaseClass/CBaseClassNonCopyable/CPatch hierarchy and full original typed edge arrays; nested vertex fields from donor reconciled to actual strides. Genuine CArray/std::vector/_Vector_base/_STLP_alloc_proxy byte buffer and free in base destructor. Shared CDataOutputStream header retained as evidence, callable real CDataStream hierarchy and typed CCharString NewLine restored. Unused virtual slots1-3 explicitly descriptive; only actual Write slot4 invoked. Empty compressor occupies a dead argument slot naturally.

## 00bfd020: CPatchTesselationEdgeStrip::SaveVerts<CWaterVertex>
Original manifest label: `SaveVerts<CPatchTesselationEdgeStrip / SaveVerts<CPatchTesselationEdgeStrip::CFanBaseVertex>`.
Identity: proven; both PDB exact private CPatchTesselationEdgeStrip::SaveVerts<CWaterVertex>(CEdgeStipVertexArray<CWaterVertex>&,CDataOutputStream&) API; own Save00BFC770 passes field+24 and stride60 matches original nested struct; WriteSLONG00993EB0 and CRangeCompressor::Compress(constvoid*,long,long,CArray<byte>&)00F3AFC0 exact original APIs; stream Write virtualslot4, free00BFEA14.
Layout: original CBaseClass/CBaseClassNonCopyable/CPatch hierarchy and full original typed edge arrays; nested vertex fields from donor reconciled to actual strides. Genuine CArray/std::vector/_Vector_base/_STLP_alloc_proxy byte buffer and free in base destructor. Shared CDataOutputStream header retained as evidence, callable real CDataStream hierarchy and typed CCharString NewLine restored. Unused virtual slots1-3 explicitly descriptive; only actual Write slot4 invoked. Empty compressor occupies a dead argument slot naturally.

## 00c0ddb0: CString::AppendSigned_00C0DDB0
Original manifest label: `CEngine / ClipBox`.
Identity: descriptive; own sprintf import01440128 format01265DE8 signed decimal; actual64-byte /GS stack buffer, cookie0139C8A8 and checker00BFE9F9; own Append(constchar*)00C0DD40 proven by SafeStrlen00C0BCC0/ConcatInPlace00C0C270 and already-landed CString::Append(double)00C0DD60. Original libpk/MFC-style CString identity descriptive from established source family, absent PDB/RTTI. Target method name address-suffixed; signed32-bit argument projection for the two decimal bodies does not claim distinct int/long source overload identity.
Layout: original libpk CString four-byte string-data pointer (no shared header). No member fields/sizeof used by target. Existing CEngine::ClipBox labels are propagation errors. Native C++ char[64] and real CRT sprintf produce the compiler-generated security cookie.

## 00c0de00: CString::AppendSigned_00C0DE00
Original manifest label: `CEngine / ClipBox`.
Identity: descriptive; own sprintf import01440128 format01265DE8 signed decimal; actual64-byte /GS stack buffer, cookie0139C8A8 and checker00BFE9F9; own Append(constchar*)00C0DD40 proven by SafeStrlen00C0BCC0/ConcatInPlace00C0C270 and already-landed CString::Append(double)00C0DD60. Original libpk/MFC-style CString identity descriptive from established source family, absent PDB/RTTI. Target method name address-suffixed; signed32-bit argument projection for the two decimal bodies does not claim distinct int/long source overload identity.
Layout: original libpk CString four-byte string-data pointer (no shared header). No member fields/sizeof used by target. Existing CEngine::ClipBox labels are propagation errors. Native C++ char[64] and real CRT sprintf produce the compiler-generated security cookie.

## 00c0de50: CString::AppendPointer_00C0DE50
Original manifest label: `CEngine / ClipBox`.
Identity: descriptive; own sprintf import01440128 format012B21DC pointer; actual64-byte /GS stack buffer, cookie0139C8A8 and checker00BFE9F9; own Append(constchar*)00C0DD40 proven by SafeStrlen00C0BCC0/ConcatInPlace00C0C270 and already-landed CString::Append(double)00C0DD60. Original libpk/MFC-style CString identity descriptive from established source family, absent PDB/RTTI. Target method name address-suffixed; signed32-bit argument projection for the two decimal bodies does not claim distinct int/long source overload identity.
Layout: original libpk CString four-byte string-data pointer (no shared header). No member fields/sizeof used by target. Existing CEngine::ClipBox labels are propagation errors. Native C++ char[64] and real CRT sprintf produce the compiler-generated security cookie.

## 00c13fc0: std::vector<CUnprovenFillWord_00C13FC0>::_Ufill
Original manifest label: `vector<float / _Ufill`.
Identity: descriptive; own thiscall three-argument42-byte helper ignores this, fills count stride4 elements from constreference via dword assignments then returns first+count; own vector reallocation callers00C14168/00C14202 establish Dinkumware vector _Ufill role. No source/PDB element identity proven. Distinct four-byte descriptive trivial value class per address preserves unique instantiation without assigning one original template symbol to multiple non-ICF bodies; original element identity remains model debt.
Layout: libpk uses original Dinkumware std::vector/_Vector_val/allocator rather than game STLport. No vector fields or sizeof used. Genuine typed fill_n assignment for trivial scalar storage; four-byte element is an explicit unknown-value projection, not a claimed recovered game class or original overload. No shared libpk header exists.

## 00c13ff0: std::vector<CUnprovenFillWord_00C13FF0>::_Ufill
Original manifest label: `vector<float / _Ufill`.
Identity: descriptive; own thiscall three-argument42-byte helper ignores this, fills count stride4 elements from constreference via dword assignments then returns first+count; own vector reallocation callers00C14358/00C143F2 establish Dinkumware vector _Ufill role. No source/PDB element identity proven. Distinct four-byte descriptive trivial value class per address preserves unique instantiation without assigning one original template symbol to multiple non-ICF bodies; original element identity remains model debt.
Layout: libpk uses original Dinkumware std::vector/_Vector_val/allocator rather than game STLport. No vector fields or sizeof used. Genuine typed fill_n assignment for trivial scalar storage; four-byte element is an explicit unknown-value projection, not a claimed recovered game class or original overload. No shared libpk header exists.

## 00c29580: std::vector<CUnprovenFillWord_00C29580>::_Ufill
Original manifest label: `vector<float / _Ufill`.
Identity: descriptive; own thiscall three-argument42-byte helper ignores this, fills count stride4 elements from constreference via dword assignments then returns first+count; own vector reallocation callers00C29698/00C29732 establish Dinkumware vector _Ufill role. No source/PDB element identity proven. Distinct four-byte descriptive trivial value class per address preserves unique instantiation without assigning one original template symbol to multiple non-ICF bodies; original element identity remains model debt.
Layout: libpk uses original Dinkumware std::vector/_Vector_val/allocator rather than game STLport. No vector fields or sizeof used. Genuine typed fill_n assignment for trivial scalar storage; four-byte element is an explicit unknown-value projection, not a claimed recovered game class or original overload. No shared libpk header exists.

## 00c17d90: CUnprovenMorphableHandle_00C185B0::Slot4
Original manifest label: `_gapscan / sub_00c17d90`.
Identity: descriptive; own virtualtarget in fourteen-slot derived vtable012B2DEC slot4 delegates same slot on Implementation+4; own diagnostic original filename .\PKCTRMorphable.cpp line74 and exact misspelling Error: NULL implimentation; related constructor00C185B0 proves original polymorphic base at0 with actual dtor00C17AD0 writing abstract fourteen-pure-slot basevtable012B2DB0. Original owner/interface/API names and parameter/return source types unproven; explicit void-pointer ABI projection only. Actual CString operator<<text00C0DEC0/long00C0DF00, destructor00C0BA40, CDebugBreak00C16750, _afxPchNil0139C900 and g_pfnAssertHook0143BB2C established existing libpk family.
Layout: genuine fourteen-slot abstract base and derived implementation-pointer+4, avoiding opaque fake vptr. Unknown unused slot signatures remain explicitly descriptive projections; only selected slot is called. No shared libpk header. Scoped real CString temporary is destroyed before empty debug-break temporary; after reporting implementation is re-read, preserving callback repair behavior.

## 00c181a0: CUnprovenMorphableHandle_00C185B0::Slot9
Original manifest label: `_gapscan / sub_00c181a0`.
Identity: descriptive; own virtualtarget in fourteen-slot derived vtable012B2DEC slot9 delegates same slot on Implementation+4; own diagnostic original filename .\PKCTRMorphable.cpp line104 and exact misspelling Error: NULL implimentation; related constructor00C185B0 proves original polymorphic base at0 with actual dtor00C17AD0 writing abstract fourteen-pure-slot basevtable012B2DB0. Original owner/interface/API names and parameter/return source types unproven; explicit void-pointer ABI projection only. Actual CString operator<<text00C0DEC0/long00C0DF00, destructor00C0BA40, CDebugBreak00C16750, _afxPchNil0139C900 and g_pfnAssertHook0143BB2C established existing libpk family.
Layout: genuine fourteen-slot abstract base and derived implementation-pointer+4, avoiding opaque fake vptr. Unknown unused slot signatures remain explicitly descriptive projections; only selected slot is called. No shared libpk header. Scoped real CString temporary is destroyed before empty debug-break temporary; after reporting implementation is re-read, preserving callback repair behavior.

## 00c184e0: CUnprovenMorphableHandle_00C185B0::Slot13
Original manifest label: `_gapscan / sub_00c184e0`.
Identity: descriptive; own virtualtarget in fourteen-slot derived vtable012B2DEC slot13 delegates same slot on Implementation+4; own diagnostic original filename .\PKCTRMorphable.cpp line128 and exact misspelling Error: NULL implimentation; related constructor00C185B0 proves original polymorphic base at0 with actual dtor00C17AD0 writing abstract fourteen-pure-slot basevtable012B2DB0. Original owner/interface/API names and parameter/return source types unproven; explicit void-pointer ABI projection only. Actual CString operator<<text00C0DEC0/long00C0DF00, destructor00C0BA40, CDebugBreak00C16750, _afxPchNil0139C900 and g_pfnAssertHook0143BB2C established existing libpk family.
Layout: genuine fourteen-slot abstract base and derived implementation-pointer+4, avoiding opaque fake vptr. Unknown unused slot signatures remain explicitly descriptive projections; only selected slot is called. No shared libpk header. Scoped real CString temporary is destroyed before empty debug-break temporary; after reporting implementation is re-read, preserving callback repair behavior.

## 00c1e640: CUnprovenPointerArray<CUnprovenPointerElement_00C1E640>::Expand
Original manifest label: `_global / Vector_ExpandCapacityBy`.
Identity: descriptive; own Grow helper allocates4*(AllocatedCount+extra), copies exactly AllocatedCount DWORDs, deletes oldarray only when countnonzero, replaces Data and increments AllocatedCount; Count+8 belongs same three-word pointerarray based on own append callers 00C1E780 object pointer appended fromargument after method onobject+8,00C1E8D0 generic append. Original pointer element class and array template/method names unproven; distinct descriptive pointed types avoid repeated one original symbol across independent no-ICF instances. Genuine native pointer new[]/delete[] plus memcpy intrinsics, not STLport or Dinkumware vector fiction.
Layout: source-proven pointer elements and three-word Data/AllocatedCount/Count shape, no active header or exact original class/PDB identity. No pointed object fields or size accessed. Numeric capacity is not a fabricated vector finish pointer. Allocation byte count and copied extent preserve retail behavior, including zero additional growth.

## 00c1eb30: CUnprovenPointerArray<CUnprovenPointerElement_00C1EB30>::Expand
Original manifest label: `_global / NUISystem_CComponent_GrowBuffer`.
Identity: descriptive; own Grow helper allocates4*(AllocatedCount+extra), copies exactly AllocatedCount DWORDs, deletes oldarray only when countnonzero, replaces Data and increments AllocatedCount; Count+8 belongs same three-word pointerarray based on own append callers 00C1EAA0 constructor appends own object pointer toparent+2C,00C1EBB0 generic append. Original pointer element class and array template/method names unproven; distinct descriptive pointed types avoid repeated one original symbol across independent no-ICF instances. Genuine native pointer new[]/delete[] plus memcpy intrinsics, not STLport or Dinkumware vector fiction.
Layout: source-proven pointer elements and three-word Data/AllocatedCount/Count shape, no active header or exact original class/PDB identity. No pointed object fields or size accessed. Numeric capacity is not a fabricated vector finish pointer. Allocation byte count and copied extent preserve retail behavior, including zero additional growth.

## 00c34d40: CUnprovenPointerArray<CUnprovenPointerElement_00C34D40>::Expand
Original manifest label: `_global / Thiscall_DynamicArrayGrow`.
Identity: descriptive; own Grow helper allocates4*(AllocatedCount+extra), copies exactly AllocatedCount DWORDs, deletes oldarray only when countnonzero, replaces Data and increments AllocatedCount; Count+8 belongs same three-word pointerarray based on own append callers 00C34F30 constructs12-byte object then appends it toowner+4,00C35040 generic append. Original pointer element class and array template/method names unproven; distinct descriptive pointed types avoid repeated one original symbol across independent no-ICF instances. Genuine native pointer new[]/delete[] plus memcpy intrinsics, not STLport or Dinkumware vector fiction.
Layout: source-proven pointer elements and three-word Data/AllocatedCount/Count shape, no active header or exact original class/PDB identity. No pointed object fields or size accessed. Numeric capacity is not a fabricated vector finish pointer. Allocation byte count and copied extent preserve retail behavior, including zero additional growth.

## 00c207a0: CUnprovenStateObject_00C20720::IsState5_00C207A0
Original manifest label: `_global / Thiscall_CheckEquipped`.
Identity: descriptive; own target calls00C20720(-1), then compares originalreceiver State WORD+AC with5 and returns normalizedEAX0/1. Actual helper only reads State, bounds-checks enum0..6 and dispatches all seven entries to same return; no writes or external effects, so real helper preserves ECX. External-only lane declaration may require callee-save receiver absent retail. No original owner/method/source field names or complete base identity proven; all descriptive address-suffixed.
Layout: explicit unknown prefix through+AC, native ushort tag. No header/PDB binding exists for libpk owner. No invented field/base identity; only exact observed field and genuine helper prototype projected. Retail same-TU clobber knowledge retained as lane-gap evidence if external call spills this.

## 00c20f20: CUnprovenStateObject_00C20720::IsState6_00C20F20
Original manifest label: `_global / IsWeaponSlotMelee`.
Identity: descriptive; own target calls00C20720(-1), then compares originalreceiver State WORD+AC with6 and returns normalizedEAX0/1. Actual helper only reads State, bounds-checks enum0..6 and dispatches all seven entries to same return; no writes or external effects, so real helper preserves ECX. External-only lane declaration may require callee-save receiver absent retail. No original owner/method/source field names or complete base identity proven; all descriptive address-suffixed.
Layout: explicit unknown prefix through+AC, native ushort tag. No header/PDB binding exists for libpk owner. No invented field/base identity; only exact observed field and genuine helper prototype projected. Retail same-TU clobber knowledge retained as lane-gap evidence if external call spills this.

## 00c20f40: CUnprovenStateObject_00C20720::IsState5_00C20F40
Original manifest label: `_global / IsWeaponSlotRanged`.
Identity: descriptive; own target calls00C20720(-1), then compares originalreceiver State WORD+AC with5 and returns normalizedEAX0/1. Actual helper only reads State, bounds-checks enum0..6 and dispatches all seven entries to same return; no writes or external effects, so real helper preserves ECX. External-only lane declaration may require callee-save receiver absent retail. No original owner/method/source field names or complete base identity proven; all descriptive address-suffixed.
Layout: explicit unknown prefix through+AC, native ushort tag. No header/PDB binding exists for libpk owner. No invented field/base identity; only exact observed field and genuine helper prototype projected. Retail same-TU clobber knowledge retained as lane-gap evidence if external call spills this.

## 00c22260: CUnprovenCollectionOwner_00C22260::ApplyFilter
Original manifest label: `_global / CTCHero_SaveAgentsToStream`.
Identity: descriptive; own stack8 polymorphic visitor built by original constructor00C22240 (actual complete same-TU body stores vtable012B3858 and Argument+4, no EDX clobber); own visitor virtual Visit00C21F00 slot0/deletingdtor slot1; true abstract base proven by actual dtor storingbasevtable012B380C. Owner embeddedcollection+0x2e8 Apply slot10 receives visitorreference. No original visitor/owner/collection/API/record spelling proven; source uses explicit unique descriptive names and void-pointer argument/node ABI projections.
Layout: actual original abstract visitor hierarchy under descriptive names, typed argument member and original two-slot visitor interface. Exact original constructor body compiled noinline above target conveys real clobber facts; no empty stand-in. Collection unknown slots0-9 explicitly descriptive; only slot10 used. Owner unknown prefix is not claimed as a recovered class/base. No shared libpk header.

## 00c2a7f0: CUnprovenCollectionOwner_00C2A7F0::ApplyFilter
Original manifest label: `_global / CSBAgents_InvokeMethod`.
Identity: descriptive; own stack8 polymorphic visitor built by original constructor00C2A690 (actual complete same-TU body stores vtable012B4180 and Argument+4, no EDX clobber); own visitor virtual Visit00C2A580 slot0/deletingdtor slot1; true abstract base proven by actual dtor storingbasevtable012B4160. Owner embeddedcollection+0x98 Apply slot10 receives visitorreference. No original visitor/owner/collection/API/record spelling proven; source uses explicit unique descriptive names and void-pointer argument/node ABI projections.
Layout: actual original abstract visitor hierarchy under descriptive names, typed argument member and original two-slot visitor interface. Exact original constructor body compiled noinline above target conveys real clobber facts; no empty stand-in. Collection unknown slots0-9 explicitly descriptive; only slot10 used. Owner unknown prefix is not claimed as a recovered class/base. No shared libpk header.

## 00c2a840: CUnprovenCollectionOwner_00C2A840::ApplyFilter
Original manifest label: `_global / Combat_HandleBlockParryEvent_AgentWrapper`.
Identity: descriptive; own stack8 polymorphic visitor built by original constructor00C2A6B0 (actual complete same-TU body stores vtable012B4188 and Argument+4, no EDX clobber); own visitor virtual Visit00C2A5A0 slot0/deletingdtor slot1; true abstract base proven by actual dtor storingbasevtable012B4160. Owner embeddedcollection+0x98 Apply slot10 receives visitorreference. No original visitor/owner/collection/API/record spelling proven; source uses explicit unique descriptive names and void-pointer argument/node ABI projections.
Layout: actual original abstract visitor hierarchy under descriptive names, typed argument member and original two-slot visitor interface. Exact original constructor body compiled noinline above target conveys real clobber facts; no empty stand-in. Collection unknown slots0-9 explicitly descriptive; only slot10 used. Owner unknown prefix is not claimed as a recovered class/base. No shared libpk header.

## 00c22840: PKCFrallocStatic<CUnprovenFrallocElement_00C22840>::Return
Original manifest label: `_global / NScript_EventQueue_Push`.
Identity: probable; own literal d:\dev\audio\ver_fable_1.5_b\libpk\PKCFrallocStatic.h line85 with Overflow; own two-slot table Return atslot0 and Take00C23BE0 atslot1 (second entry is a real dequeue method, not destructor); actual Take decrementsAvailableCount and returns pointer fromFreeList+14. Return writes incomingpointer to ring, wrapsWriteIndex moduloCapacity, incrementsAvailableCount even after warning. Original element and interface/base identity and method names unproven; explicit distinct descriptive pointerelement types, original filename establishes probable template owner.
Layout: direct observed two-slot polymorphic pool projection; no original base is proven or invented, no fake destructor slot. Source-proven pointer storage, count/capacity/head and underlying object storage; no pointed element size accessed. All fields/base/API spellings except filename owner are descriptive. Scoped actual CString/assert-hook/debug-break family preserves EH lifetime. No shared libpk header.

## 00c22920: PKCFrallocStatic<CUnprovenFrallocElement_00C22920>::Return
Original manifest label: `_global / NScript_EventSubscription_Push`.
Identity: probable; own literal d:\dev\audio\ver_fable_1.5_b\libpk\PKCFrallocStatic.h line85 with Overflow; own two-slot table Return atslot0 and Take00C23DD0 atslot1 (second entry is a real dequeue method, not destructor); actual Take decrementsAvailableCount and returns pointer fromFreeList+14. Return writes incomingpointer to ring, wrapsWriteIndex moduloCapacity, incrementsAvailableCount even after warning. Original element and interface/base identity and method names unproven; explicit distinct descriptive pointerelement types, original filename establishes probable template owner.
Layout: direct observed two-slot polymorphic pool projection; no original base is proven or invented, no fake destructor slot. Source-proven pointer storage, count/capacity/head and underlying object storage; no pointed element size accessed. All fields/base/API spellings except filename owner are descriptive. Scoped actual CString/assert-hook/debug-break family preserves EH lifetime. No shared libpk header.

## 00c22a40: PKCFrallocStatic<CUnprovenFrallocElement_00C22A40>::Return
Original manifest label: `_global / NScript_EventSubscriptionSet_Push`.
Identity: probable; own literal d:\dev\audio\ver_fable_1.5_b\libpk\PKCFrallocStatic.h line85 with Overflow; own two-slot table Return atslot0 and Take00C24020 atslot1 (second entry is a real dequeue method, not destructor); actual Take decrementsAvailableCount and returns pointer fromFreeList+14. Return writes incomingpointer to ring, wrapsWriteIndex moduloCapacity, incrementsAvailableCount even after warning. Original element and interface/base identity and method names unproven; explicit distinct descriptive pointerelement types, original filename establishes probable template owner.
Layout: direct observed two-slot polymorphic pool projection; no original base is proven or invented, no fake destructor slot. Source-proven pointer storage, count/capacity/head and underlying object storage; no pointed element size accessed. All fields/base/API spellings except filename owner are descriptive. Scoped actual CString/assert-hook/debug-break family preserves EH lifetime. No shared libpk header.

## 00c2ed90: CUnprovenTransferVisitor_00C2ED90::Visit
Original manifest label: `_global / ShaderGroup_TryLoad`.
Identity: descriptive; own two-slot callback vtable012B497C has this target atslot0 and deletingdtor atslot1; own+4 pointer passed as argument to 00C3B850 on callback argument receiver; callee serializes32/16-bit fields through actual archive00C31B30 wrappers00C31B50/00C31B70, and variable strings through00C31B90. Own+8 errorcounter increments only on failed transfer and suppresses later attempts. Original archive identity PKSerialiseCParser established by canonical TransferSize(ulong*)00C31B50 plus assertion filename; original visitor/record names unproven. Own deletingdtors00C2F3C0/3F0/420 call actualdtors00C2F3B0/3E0/410 that store distinct abstract base vtables012B4964/6C/74 (pure Visit slot0,dtor slot1); genuine abstract bases restored. No original visitor/record class name proven; descriptive owner/record/archive/method/member names explicitly marked.
Layout: no original owner header or RTTI/name-set binding. This three-word callback projection preserves own two-slot virtual interface, archive pointer+4, unsigned errorcounter+8, and correct independent helper/receiver types. No object size, construction or unknown inherited API assumed; original abstract base hierarchy restored under explicit descriptive names, as own base vtable stores prove. Archive canonical original type PKSerialiseCParser used consistently with N201 constructors. Legacy shader labels are rejected: own archive scalar/string transfers only establish serialization.

## 00c2edc0: CUnprovenTransferVisitor_00C2EDC0::Visit
Original manifest label: `_global / ComplexShaderSet_TryLoad`.
Identity: descriptive; own two-slot callback vtable012B4984 has this target atslot0 and deletingdtor atslot1; own+4 pointer passed as argument to 00C3B970 on callback argument receiver; callee serializes32/16-bit fields through actual archive00C31B30 wrappers00C31B50/00C31B70, and variable strings through00C31B90. Own+8 errorcounter increments only on failed transfer and suppresses later attempts. Original archive identity PKSerialiseCParser established by canonical TransferSize(ulong*)00C31B50 plus assertion filename; original visitor/record names unproven. Own deletingdtors00C2F3C0/3F0/420 call actualdtors00C2F3B0/3E0/410 that store distinct abstract base vtables012B4964/6C/74 (pure Visit slot0,dtor slot1); genuine abstract bases restored. No original visitor/record class name proven; descriptive owner/record/archive/method/member names explicitly marked.
Layout: no original owner header or RTTI/name-set binding. This three-word callback projection preserves own two-slot virtual interface, archive pointer+4, unsigned errorcounter+8, and correct independent helper/receiver types. No object size, construction or unknown inherited API assumed; original abstract base hierarchy restored under explicit descriptive names, as own base vtable stores prove. Archive canonical original type PKSerialiseCParser used consistently with N201 constructors. Legacy shader labels are rejected: own archive scalar/string transfers only establish serialization.

## 00c2edf0: CUnprovenTransferVisitor_00C2EDF0::Visit
Original manifest label: `_global / GraphicsSystem_TryLoadShaderData`.
Identity: descriptive; own two-slot callback vtable012B498C has this target atslot0 and deletingdtor atslot1; own+4 pointer passed as argument to 00C3B7A0 on callback argument receiver; callee serializes32/16-bit fields through actual archive00C31B30 wrappers00C31B50/00C31B70, and variable strings through00C31B90. Own+8 errorcounter increments only on failed transfer and suppresses later attempts. Original archive identity PKSerialiseCParser established by canonical TransferSize(ulong*)00C31B50 plus assertion filename; original visitor/record names unproven. Own deletingdtors00C2F3C0/3F0/420 call actualdtors00C2F3B0/3E0/410 that store distinct abstract base vtables012B4964/6C/74 (pure Visit slot0,dtor slot1); genuine abstract bases restored. No original visitor/record class name proven; descriptive owner/record/archive/method/member names explicitly marked.
Layout: no original owner header or RTTI/name-set binding. This three-word callback projection preserves own two-slot virtual interface, archive pointer+4, unsigned errorcounter+8, and correct independent helper/receiver types. No object size, construction or unknown inherited API assumed; original abstract base hierarchy restored under explicit descriptive names, as own base vtable stores prove. Archive canonical original type PKSerialiseCParser used consistently with N201 constructors. Legacy shader labels are rejected: own archive scalar/string transfers only establish serialization.

## 00c2ee20: CUnprovenTransferVisitor_00C2ED90::CUnprovenTransferVisitor_00C2ED90
Original manifest label: `CSBAgents / CSBAgents`.
Identity: Own caller00c2f935, derived table012b497c Visit00C2ED90, record transfer00C3B850. Own destructor restores separate abstract visitor base table012b4964. Names descriptive and shared consistently with N200; no original class identity claimed.
Layout: no shared header. Abstract visitor base vptr0, archive pointer4, unsigned Failures8. Base slot0 pure Visit, slot1 destructor, derived overrides Visit. Own destructor and two tables prove inherited interface.

## 00c2ee40: CUnprovenTransferVisitor_00C2EDC0::CUnprovenTransferVisitor_00C2EDC0
Original manifest label: `CSBAgents / CSBAgents`.
Identity: Own caller00c2fb85, derived table012b4984 Visit00C2EDC0, record transfer00C3B970. Own destructor restores separate abstract visitor base table012b496c. Names descriptive and shared consistently with N200; no original class identity claimed.
Layout: no shared header. Abstract visitor base vptr0, archive pointer4, unsigned Failures8. Base slot0 pure Visit, slot1 destructor, derived overrides Visit. Own destructor and two tables prove inherited interface.

## 00c2ee60: CUnprovenTransferVisitor_00C2EDF0::CUnprovenTransferVisitor_00C2EDF0
Original manifest label: `CSBAgents / CSBAgents`.
Identity: Own caller00c2fdd5, derived table012b498c Visit00C2EDF0, record transfer00C3B7A0. Own destructor restores separate abstract visitor base table012b4974. Names descriptive and shared consistently with N200; no original class identity claimed.
Layout: no shared header. Abstract visitor base vptr0, archive pointer4, unsigned Failures8. Base slot0 pure Visit, slot1 destructor, derived overrides Visit. Own destructor and two tables prove inherited interface.

## 00c2f030: PKCAutoDelete<CUnprovenTransferRecord_00C3B850>::~PKCAutoDelete
Original manifest label: `CActiveFile / OnReadFinished`.
Identity: Own unwind funclet from00c2f7d0 calls this destructor; sibling Release00c2f070 asserts actual PKCAutoDelete.h filename. Same record transfer00C3B850 and destructor00c3b840, allocation32 bytes. Original record name unknown; shared descriptive record spelling with N200/N201/N203.
Layout: genuine libpk PKCAutoDelete<T> has one owning pointer0. Unknown record has own allocation size32; destructor is external, no fictitious fields or behavior modeled. No shared headers.

## 00c2f150: PKCAutoDelete<CUnprovenTransferRecord_00C3B970>::~PKCAutoDelete
Original manifest label: `CActiveFile / OnReadFinished`.
Identity: Own unwind funclet from00c2fa20 calls this destructor; sibling Release00c2f190 asserts actual PKCAutoDelete.h filename. Same record transfer00C3B970 and destructor00c3b960, allocation48 bytes. Original record name unknown; shared descriptive record spelling with N200/N201/N203.
Layout: genuine libpk PKCAutoDelete<T> has one owning pointer0. Unknown record has own allocation size48; destructor is external, no fictitious fields or behavior modeled. No shared headers.

## 00c2f270: PKCAutoDelete<CUnprovenTransferRecord_00C3B7A0>::~PKCAutoDelete
Original manifest label: `CActiveFile / OnReadFinished`.
Identity: Own unwind funclet from00c2fc70 calls this destructor; sibling Release00c2f2b0 asserts actual PKCAutoDelete.h filename. Same record transfer00C3B7A0 and destructor00c3b6b0, allocation12 bytes. Original record name unknown; shared descriptive record spelling with N200/N201/N203.
Layout: genuine libpk PKCAutoDelete<T> has one owning pointer0. Unknown record has own allocation size12; destructor is external, no fictitious fields or behavior modeled. No shared headers.

## 00c2f7d0: CUnprovenRecordList_00c2f7d0::Transfer
Original manifest label: `_global / CMessageEvent_ProcessLoadingFrame_Vsync`.
Identity: own CBankData caller00c2ee80 uses three corresponding collections; record ctor00c3b930/dtor00c3b840/Transfer00C3B850, actual PKCAutoDelete Release00c2f070 and visitor ctor00c2ee20. Descriptive record/collection names.
Layout: no active shared headers. Own record allocation32; pointer guard4, abstract visitor base4 plus Archive4/Failures8. Collection uses observed virtual slots5 Clear/9 Add/10 ForEach/14 Reserve/15 Count only; unknown class/base size not claimed. PKSerialiseCParser name from existing helper bindings/assertions.
Visible Release body retains the original PKCAutoDelete.h(42) diagnostic and ownership clear.

## 00c2fa20: CUnprovenRecordList_00c2fa20::Transfer
Original manifest label: `_global / CMessageEvent_ProcessLoadingFrame_Water`.
Identity: own CBankData caller00c2ee80 uses three corresponding collections; record ctor00c3baf0/dtor00c3b960/Transfer00C3B970, actual PKCAutoDelete Release00c2f190 and visitor ctor00c2ee40. Descriptive record/collection names.
Layout: no active shared headers. Own record allocation48; pointer guard4, abstract visitor base4 plus Archive4/Failures8. Collection uses observed virtual slots5 Clear/9 Add/10 ForEach/14 Reserve/15 Count only; unknown class/base size not claimed. PKSerialiseCParser name from existing helper bindings/assertions.
Visible Release body retains the original PKCAutoDelete.h(42) diagnostic and ownership clear.

## 00c2fc70: CUnprovenRecordList_00c2fc70::Transfer
Original manifest label: `_global / CMessageEvent_ProcessLoadingFrame_Graphics`.
Identity: own CBankData caller00c2ee80 uses three corresponding collections; record ctor00c3b690/dtor00c3b6b0/Transfer00C3B7A0, actual PKCAutoDelete Release00c2f2b0 and visitor ctor00c2ee60. Descriptive record/collection names.
Layout: no active shared headers. Own record allocation12; pointer guard4, abstract visitor base4 plus Archive4/Failures8. Collection uses observed virtual slots5 Clear/9 Add/10 ForEach/14 Reserve/15 Count only; unknown class/base size not claimed. PKSerialiseCParser name from existing helper bindings/assertions.
Visible Release body retains the original PKCAutoDelete.h(42) diagnostic and ownership clear.

## 00c33fd0: global::SetVectorA
Original manifest label: `CChunkedFileSuperChunk / InitNewInstance`.
Identity: own wrapper00c33fd0 calls setter00c41320; field0x158 and mask0x8000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.

## 00c34010: global::SetVectorB
Original manifest label: `CChunkedFileSuperChunk / InitNewInstance`.
Identity: own wrapper00c34010 calls setter00c41380; field0x164 and mask0x10000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.

## 00c34140: global::SetVectorC
Original manifest label: `CChunkedFileSuperChunk / InitNewInstance`.
Identity: own wrapper00c34140 calls setter00c413e0; field0x170 and mask0x80000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.

## 00c344d0: CVoiceParams::SetPitchScale
Original manifest label: `CGuiInputProcess / AddToInterface`.
Identity: descriptive libpk names, no corresponding PDB name recovery claimed. Own caller00c20c50 and neighboring GetPitch00c34410 multiply field56 with Pitch52. Manifest CGuiInputProcess::AddToInterface is wrong.
Layout: no active shared header; untouched prefix56, own float field at56. Return is full-width0/1 as in retail.

## 00c34500: CVoiceParams::SetGainScale
Original manifest label: `CGuiInputProcess / AddToInterface`.
Identity: descriptive libpk names, no corresponding PDB name recovery claimed. Own caller00c20c80 and neighboring GetGain00c34440 multiply field48 with Gain44. Manifest CGuiInputProcess::AddToInterface is wrong.
Layout: no active shared header; untouched prefix48, own float field at48. Return is full-width0/1 as in retail.

## 00c34580: CVoiceParams::SetUnprovenParameter28
Original manifest label: `CGuiInputProcess / AddToInterface`.
Identity: descriptive libpk names, no corresponding PDB name recovery claimed. Own caller00c20eb0 passes the float to field40; original parameter role not established. Manifest CGuiInputProcess::AddToInterface is wrong.
Layout: no active shared header; untouched prefix40, own float field at40. Return is full-width0/1 as in retail.

## 00c3d2f0: CVtable_012B5870::SetWord88
Original manifest label: `_gapscan / sub_00c3d2f0`.
Identity: Own global singleton wrapper00c335f0 loads receiver0143bb58; existing SetSign8C00c3d310 and GetPair8000c3d2d0 prove same table012b5870. All owner/function names descriptive, no original PDB recovery claimed.
Layout: no active shared header. Actual vptr0, parameter136, dirty164. Untouched prefix and intervening members not modeled.

## 00c41550: CLookTarget::SetHeadTurnTarget
Original manifest label: `CTCLook / SetHeadTurnState`.
Identity: Own wrapper00c34180 virtual slot1, existing exact callee binding ?SetHeadTurnTarget@CLookTarget@@QAEXJ@Z. Head-turn role is unsupported libpk labeling; spelling retained only for binding consistency. All owner/function names descriptive, no original PDB recovery claimed.
Layout: no active shared header. Actual vptr0, parameter404, dirty472. Untouched prefix and intervening members not modeled.

## 00c415c0: CLibpkObject1D8::SetWord1A0
Original manifest label: `CTCLook / SetHeadTurnState`.
Identity: Own wrapper00c34229 and call00c4ee75 on libpk source object; neighboring float setters share dirty472. Parameter role unknown. All owner/function names descriptive, no original PDB recovery claimed.
Layout: no active shared header. Actual vptr0, parameter416, dirty472. Untouched prefix and intervening members not modeled.

## 00c3d440: CVtable_012B5870::SetDefaultStateCallback
Original manifest label: `_gapscan / sub_00c3d440`.
Identity: Own global wrapper00c33500 receiver0143bb58; getter00c3d2d0 copies both words into new audio object callbacks at00c42e15. Descriptive libpk owner/API spellings, not original PDB identifiers.
Layout: own CAudioObject constructor00c42d60 calls CPooledNode ctor00c4cf20, base12. Callback220/context224 proven by own SetState00c43570; last word-pair228/232 role unproven. Global default callback128/context132 copied by00c42e15 into these object fields. No shared headers.

## 00c43330: CAudioObject::SetStateCallback
Original manifest label: `_gapscan / sub_00c43330`.
Identity: Own wrapper00c35cf0; stored callback invoked by SetState00c43570 with object,low16flags,context. Descriptive libpk owner/API spellings, not original PDB identifiers.
Layout: own CAudioObject constructor00c42d60 calls CPooledNode ctor00c4cf20, base12. Callback220/context224 proven by own SetState00c43570; last word-pair228/232 role unproven. Global default callback128/context132 copied by00c42e15 into these object fields. No shared headers.

## 00c43350: CAudioObject::SetUnprovenPairE4
Original manifest label: `_gapscan / sub_00c43350`.
Identity: Own wrapper00c35bf0 forwards two words to228/232. Role and original parameter types unresolved; represented as opaque pointers. Descriptive libpk owner/API spellings, not original PDB identifiers.
Layout: own CAudioObject constructor00c42d60 calls CPooledNode ctor00c4cf20, base12. Callback220/context224 proven by own SetState00c43570; last word-pair228/232 role unproven. Global default callback128/context132 copied by00c42e15 into these object fields. No shared headers.

## 00c408c0: CListenerState::SetFloat30
Original manifest label: `_gapscan / sub_00c408c0`.
Identity: Own caller00c33a49 is a libpk listener wrapper. Related factory00c3dde0 constructs 128-byte listener state, vtable012b5a84, with CPooledNode base. Existing descriptive listener identity retained; parameter role not claimed.
Layout: original pooled-node base at0 (own constructor00c4cf20), vptr and two link pointers; listener property block+12, float parameters+48/+52/+56, DirtyFlags+120, effect handle+124. No active shared header. Original parameter names remain unknown.

## 00c408e0: CListenerState::SetFloat34
Original manifest label: `_gapscan / sub_00c408e0`.
Identity: Own caller00c33a69 is a libpk listener wrapper. Related factory00c3dde0 constructs 128-byte listener state, vtable012b5a84, with CPooledNode base. Existing descriptive listener identity retained; parameter role not claimed.
Layout: original pooled-node base at0 (own constructor00c4cf20), vptr and two link pointers; listener property block+12, float parameters+48/+52/+56, DirtyFlags+120, effect handle+124. No active shared header. Original parameter names remain unknown.

## 00c40900: CListenerState::SetFloat38
Original manifest label: `_gapscan / sub_00c40900`.
Identity: Own caller00c33a89 is a libpk listener wrapper. Related factory00c3dde0 constructs 128-byte listener state, vtable012b5a84, with CPooledNode base. Existing descriptive listener identity retained; parameter role not claimed.
Layout: original pooled-node base at0 (own constructor00c4cf20), vptr and two link pointers; listener property block+12, float parameters+48/+52/+56, DirtyFlags+120, effect handle+124. No active shared header. Original parameter names remain unknown.

## 00c41320: Row00c41c40::CameraConstants::SetA
Original manifest label: `CShaderRenderManager / SetCameraPos`.
Identity: own wrapper00c33fd0 calls setter00c41320; field0x158 and mask0x8000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.

## 00c41380: Row00c41c40::CameraConstants::SetB
Original manifest label: `CShaderRenderManager / SetCameraPos`.
Identity: own wrapper00c34010 calls setter00c41380; field0x164 and mask0x10000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.

## 00c413e0: Row00c41c40::CameraConstants::SetC
Original manifest label: `CShaderRenderManager / SetCameraPos`.
Identity: own wrapper00c34140 calls setter00c413e0; field0x170 and mask0x80000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.

## 00c44d30: CPKEmitter::ClearEvents
Original manifest label: `_global / CInventory_CleanupItems`.
Identity: own destructor caller00c44faf, real PKCTRList virtual Head11/Remove8, item release00c61fb0 and destructor00c61c90. Descriptive ambient emitter/source identities; helper spellings reused where available.
Layout: genuine libpk PKCTRList48 (CPKList vptr/implementation8 plus40-byte member implementation). Emitter list32; source triggers8 and channels56. No shared headers. Existing helper owner/record spellings retained for stable symbols, roles identified by own constructors/callers. Empty record destructors16/24/8 remain real out-of-line calls.

## 00c47170: SContext_00c62500::ClearTriggers
Original manifest label: `_global / CInventory_CleanupItems_Variant1`.
Identity: own destructor caller00c4741f, real PKCTRList virtual Head11/Remove8, item release00c62500 and destructor00c62340. Descriptive ambient emitter/source identities; helper spellings reused where available.
Layout: genuine libpk PKCTRList48 (CPKList vptr/implementation8 plus40-byte member implementation). Emitter list32; source triggers8 and channels56. No shared headers. Existing helper owner/record spellings retained for stable symbols, roles identified by own constructors/callers. Empty record destructors16/24/8 remain real out-of-line calls.

## 00c471c0: SContext_00c62500::ClearChannels
Original manifest label: `_global / CInventory_CleanupItems_Variant2`.
Identity: own destructor caller00c47418, real PKCTRList virtual Head11/Remove8, item release00c62180 and destructor00c62140. Descriptive ambient emitter/source identities; helper spellings reused where available.
Layout: genuine libpk PKCTRList48 (CPKList vptr/implementation8 plus40-byte member implementation). Emitter list32; source triggers8 and channels56. No shared headers. Existing helper owner/record spellings retained for stable symbols, roles identified by own constructors/callers. Empty record destructors16/24/8 remain real out-of-line calls.

## 00c535c0: CUnprovenReverbParameters::SetClamped28
Original manifest label: `_global / Attribute_SetClamped_Index10`.
Identity: Own callers 00c53d43 / 00c542f8 / 00c5463a; values clamped to[0.0f,0.3f] and corresponding dirty mask 0x2000. Descriptive parameter identity only.
Layout: no active shared header. Caller passes its subobject+0x290 to all three methods; common flags at0, float parameters40/48/100. Remaining members are not modeled. Libpk has no original PDB class name evidence here.

## 00c536b0: CUnprovenReverbParameters::SetClamped30
Original manifest label: `_global / Attribute_SetClamped_Index12`.
Identity: Own callers 00c53e33 / 00c5434d / 00c5464e; values clamped to[0.0f,0.1f] and corresponding dirty mask 0x10000. Descriptive parameter identity only.
Layout: no active shared header. Caller passes its subobject+0x290 to all three methods; common flags at0, float parameters40/48/100. Remaining members are not modeled. Libpk has no original PDB class name evidence here.

## 00c53a40: CUnprovenReverbParameters::SetClamped64
Original manifest label: `_global / Attribute_SetClamped_Index25`.
Identity: Own callers 00c53ef3 / 00c5437d / 00c5465a; values clamped to[0.1f,100.0f] and corresponding dirty mask 0x1. Descriptive parameter identity only.
Layout: no active shared header. Caller passes its subobject+0x290 to all three methods; common flags at0, float parameters40/48/100. Remaining members are not modeled. Libpk has no original PDB class name evidence here.

## 00c56150: CAudioContext::DistanceModel
Original manifest label: `CAudioContext / CAudioContext::DistanceModel`.
Identity: own literal alDistanceModel and function slot0xd4; actual helper00c4d740 and kernel GetProcAddress0143fe44. OpenAL API identity proven, wrapper class/method spelling descriptive existing convention.
Layout: no shared header. Actual module+4, source callbacks6c/74/7c/84, stringd0/distanced4, resolver flag104. Original libpk class name unknown; established CAudioContext binding retained. Genuine virtual interface slot0 and untouched fields retained as unmodeled ranges.

## 00c561a0: CAudioContext::GetString
Original manifest label: `CAudioContext / CAudioContext::GetString`.
Identity: own literal alGetString and function slot0xd0; actual helper00c4d740 and kernel GetProcAddress0143fe44. OpenAL API identity proven, wrapper class/method spelling descriptive existing convention.
Layout: no shared header. Actual module+4, source callbacks6c/74/7c/84, stringd0/distanced4, resolver flag104. Original libpk class name unknown; established CAudioContext binding retained. Genuine virtual interface slot0 and untouched fields retained as unmodeled ranges.

## 00c725b0: CAudioContext::SourceRewind
Original manifest label: `_global / CAudioSource_LazyLoadAlSourceRewind`.
Identity: own literal alSourceRewind and function slot0x84; actual helper00c4d740 and kernel GetProcAddress0143fe44. OpenAL API identity proven, wrapper class/method spelling descriptive existing convention.
Layout: no shared header. Actual module+4, source callbacks6c/74/7c/84, stringd0/distanced4, resolver flag104. Original libpk class name unknown; established CAudioContext binding retained. Genuine virtual interface slot0 and untouched fields retained as unmodeled ranges.

## 00c575f0: CAudioContext::SourcePlay
Original manifest label: `CAudioContext / CAudioContext::PlaySource`.
Identity: own literal alSourcePlay and function slot0x6c; actual helper00c4d740 and kernel GetProcAddress0143fe44. OpenAL API identity proven, wrapper class/method spelling descriptive existing convention.
Layout: no shared header. Actual module+4, source callbacks6c/74/7c/84, stringd0/distanced4, resolver flag104. Original libpk class name unknown; established CAudioContext binding retained. Genuine virtual interface slot0 and untouched fields retained as unmodeled ranges.

## 00c57640: CAudioContext::SourceStop
Original manifest label: `_global / CAudioContext_CallAlSourceStop`.
Identity: own literal alSourceStop and function slot0x7c; actual helper00c4d740 and kernel GetProcAddress0143fe44. OpenAL API identity proven, wrapper class/method spelling descriptive existing convention.
Layout: no shared header. Actual module+4, source callbacks6c/74/7c/84, stringd0/distanced4, resolver flag104. Original libpk class name unknown; established CAudioContext binding retained. Genuine virtual interface slot0 and untouched fields retained as unmodeled ranges.

## 00c72560: CAudioContext::SourcePause
Original manifest label: `CAudioSource / CAudioSource::PausePlayback`.
Identity: own literal alSourcePause and function slot0x74; actual helper00c4d740 and kernel GetProcAddress0143fe44. OpenAL API identity proven, wrapper class/method spelling descriptive existing convention.
Layout: no shared header. Actual module+4, source callbacks6c/74/7c/84, stringd0/distanced4, resolver flag104. Original libpk class name unknown; established CAudioContext binding retained. Genuine virtual interface slot0 and untouched fields retained as unmodeled ranges.

## 00c5d150: Row00c5d150::ScaleAccumulate16
Original manifest label: `_global / SIMD_FixedPointMulAccumulate`.
Retail-asm: 0x00c5d150 aligned hand SIMD loop fixes xmm7, uses pmulhw/punpcklwd/psrad/paddd and align16 loop. Intrinsics allocate different registers (existing verified sibling00c6d370); preserve original kernel and ordinary C++ local initialization.
Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.

## 00c6e1f0: Row00c6e1f0::ScaleAccumulate16
Original manifest label: `_global / SSE_MatrixMultiplyShort_NoVaryingScale`.
Retail-asm: 0x00c6e1f0 aligned hand SIMD loop fixes xmm7, uses pmulhw/punpcklwd/psrad/paddd and align16 loop. Intrinsics allocate different registers (existing verified sibling00c6d370); preserve original kernel and ordinary C++ local initialization.
Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.

## 00c6ecd0: Row00c6ecd0::ScaleAccumulate16
Original manifest label: `_global / SSE_MatrixMultiplyShort_NoVaryingScale`.
Retail-asm: 0x00c6ecd0 aligned hand SIMD loop fixes xmm7, uses pmulhw/punpcklwd/psrad/paddd and align16 loop. Intrinsics allocate different registers (existing verified sibling00c6d370); preserve original kernel and ordinary C++ local initialization.
Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.

## 00c5d1f0: Row00c5d1f0::ScaleAccumulate16Ramp
Original manifest label: `_global / SIMD_FixedPointRampMulAccumulate`.
Retail-asm: 0x00c5d1f0 aligned hand SIMD loop fixes xmm7, uses pmulhw/punpcklwd/psrad/paddd and align16 loop. Intrinsics allocate different registers (existing verified sibling00c6d370); preserve original kernel and ordinary C++ local initialization.
Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.

## 00c6e290: Row00c6e290::ScaleAccumulate16Ramp
Original manifest label: `_global / Filter_ApplyTransformSSE_16`.
Retail-asm: 0x00c6e290 aligned hand SIMD loop fixes xmm7, uses pmulhw/punpcklwd/psrad/paddd and align16 loop. Intrinsics allocate different registers (existing verified sibling00c6d370); preserve original kernel and ordinary C++ local initialization.
Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.

## 00c6ed70: Row00c6ed70::ScaleAccumulate16Ramp
Original manifest label: `_global / SSE_MatrixMultiplyShort`.
Retail-asm: 0x00c6ed70 aligned hand SIMD loop fixes xmm7, uses pmulhw/punpcklwd/psrad/paddd and align16 loop. Intrinsics allocate different registers (existing verified sibling00c6d370); preserve original kernel and ordinary C++ local initialization.
Identity: own mixer callers and exact signed high-word product, then double sign extension. Descriptive helper name; separate namespace distinguishes original copies. Count must be positive, buffers aligned16.

## 00c5e440: CEvent_00c5eca0::scalar_deleting_destructor
Original manifest label: `CWaterPatchMesh / `scalar_deleting_destructor'`.
Identity: own vtable012b90e8 only slot; 00c5ed90 embedded member+44. Existing descriptive libpk class names, not original PDB recovery.
Layout: no active shared header; vptr0 and handle4, size8. Closure import and scalar delete match own body.

## 00c5e4a0: CMutex_00c5eca0::scalar_deleting_destructor
Original manifest label: `CWaterPatchMesh / `scalar_deleting_destructor'`.
Identity: own vtable012b90ec only slot; 00c5ed90 embedded member+52. Existing descriptive libpk class names, not original PDB recovery.
Layout: no active shared header; vptr0 and handle4, size8. Closure import and scalar delete match own body.

## 00c739c0: CRegistryKey::scalar_deleting_destructor
Original manifest label: `CWaterPatchMesh / `scalar_deleting_destructor'`.
Identity: own vtable012befb0 only slot; Own constructor00c73790, destructor00c739a0 and registry Create00c737d0. Existing descriptive libpk class names, not original PDB recovery.
Layout: no active shared header; vptr0 and handle4, size8. Closure import and scalar delete match own body.

## 00c64a80: CEaxSource::SetRoomHF
Original manifest label: `_global / SetGain`.
Identity: own callers00c67e44 / 00c6864f select SDK EAXSOURCEPROPERTIES.lRoomHF, current field36. Original SDK property name recovered; libpk owner name descriptive established binding.
Layout: no active shared header. Actual dirty4, context20, SDK properties24; current field36. Error field980 in owning context. Deferred properties520 remain untouched here.

## 00c64b70: CEaxSource::SetOcclusion
Original manifest label: `_global / SetVelocity`.
Identity: own callers00c67e79 / 00c686c1 select SDK EAXSOURCEPROPERTIES.lOcclusion, current field48. Original SDK property name recovered; libpk owner name descriptive established binding.
Layout: no active shared header. Actual dirty4, context20, SDK properties24; current field48. Error field980 in owning context. Deferred properties520 remain untouched here.

## 00c64cd0: CEaxSource::SetExclusion
Original manifest label: `_global / SetPan`.
Identity: own callers00c67ec3 / 00c68759 select SDK EAXSOURCEPROPERTIES.lExclusion, current field64. Original SDK property name recovered; libpk owner name descriptive established binding.
Layout: no active shared header. Actual dirty4, context20, SDK properties24; current field64. Error field980 in owning context. Deferred properties520 remain untouched here.

## 00c6d120: CVtable_012bed7c::SetVolume
Original manifest label: `_gapscan / sub_00c6d120`.
Identity: own table012bed7c slot3 and inherited float32; actual helper00c6cf40 takes bool, stores flag36, rebuilds gain records under lock. Class and method names descriptive; unused parameter type unresolved.
Layout: original descriptive libpk CPKObject root (vtable012b5868) and CVtable_012beda4 base from constructor00c6e6a0/destructor00c6e630. Critical-section holder is a member with own vptr+4 and24 bytes at+8, volume32 and flag36; inherited base40. No shared headers. Slot3 has one float, an unused four-byte parameter of unknown original type, and bool consumed by actual rebuild helper. Unused parameter is represented as a word, no original signature claim.

## 00c6e020: CVtable_012bed90::SetVolume
Original manifest label: `_gapscan / sub_00c6e020`.
Identity: own table012bed90 slot3 and inherited float32; actual helper00c6dea0 takes bool, stores flag36, rebuilds gain records under lock. Class and method names descriptive; unused parameter type unresolved.
Layout: original descriptive libpk CPKObject root (vtable012b5868) and CVtable_012beda4 base from constructor00c6e6a0/destructor00c6e630. Critical-section holder is a member with own vptr+4 and24 bytes at+8, volume32 and flag36; inherited base40. No shared headers. Slot3 has one float, an unused four-byte parameter of unknown original type, and bool consumed by actual rebuild helper. Unused parameter is represented as a word, no original signature claim.

## 00c6ec50: CVtable_012bedb8::SetVolume
Original manifest label: `_gapscan / sub_00c6ec50`.
Identity: own table012bedb8 slot3 and inherited float32; actual helper00c6ebe0 takes bool, stores flag36, rebuilds gain records under lock. Class and method names descriptive; unused parameter type unresolved.
Layout: original descriptive libpk CPKObject root (vtable012b5868) and CVtable_012beda4 base from constructor00c6e6a0/destructor00c6e630. Critical-section holder is a member with own vptr+4 and24 bytes at+8, volume32 and flag36; inherited base40. No shared headers. Slot3 has one float, an unused four-byte parameter of unknown original type, and bool consumed by actual rebuild helper. Unused parameter is represented as a word, no original signature claim.

## 00c785d0: I32vec2::I32vec2
Original manifest label: `_global / __EH_epilog3`.
Identity: Unreferenced Intel ivec.h forwarding-constructor copies. Own TU grouping beside M64 conversion00c785c0, Is32vec2 operators00c78630/70/b0 and I16vec4 constructor00c78760 supports this class assignment only as probable, not exact original recovery.
Layout: SDK ivec.h inheritance and protected vec member; eight-byte MMX storage. No engine header. Native intrinsic argument convention and compiler-generated cleanup.

## 00c78600: Is32vec2::Is32vec2
Original manifest label: `_global / __EH_epilog3`.
Identity: Unreferenced Intel ivec.h forwarding-constructor copies. Own TU grouping beside M64 conversion00c785c0, Is32vec2 operators00c78630/70/b0 and I16vec4 constructor00c78760 supports this class assignment only as probable, not exact original recovery.
Layout: SDK ivec.h inheritance and protected vec member; eight-byte MMX storage. No engine header. Native intrinsic argument convention and compiler-generated cleanup.

## 00c787a0: Is16vec4::Is16vec4
Original manifest label: `_global / __EH_epilog3`.
Identity: Unreferenced Intel ivec.h forwarding-constructor copies. Own TU grouping beside M64 conversion00c785c0, Is32vec2 operators00c78630/70/b0 and I16vec4 constructor00c78760 supports this class assignment only as probable, not exact original recovery.
Layout: SDK ivec.h inheritance and protected vec member; eight-byte MMX storage. No engine header. Native intrinsic argument convention and compiler-generated cleanup.

## 00c7b7d0: CUnprovenSSEVector_00c7b7d0::CUnprovenSSEVector_00c7b7d0
Original manifest label: `CDateAndTime / CDateAndTime`.
Identity: Native __m128i argument in xmm0, aligned16 store, /GS cookie. Intel vector-library constructor shape; exact class among byte-identical copies not established.
Layout: sixteen-byte intrinsic integer vector; no active shared header. Descriptive class, no original name claimed.

## 00c7b930: CUnprovenSSEVector_00c7b930::CUnprovenSSEVector_00c7b930
Original manifest label: `CDateAndTime / CDateAndTime`.
Identity: Native __m128i argument in xmm0, aligned16 store, /GS cookie. Intel vector-library constructor shape; exact class among byte-identical copies not established.
Layout: sixteen-byte intrinsic integer vector; no active shared header. Descriptive class, no original name claimed.

## 00c7b970: CUnprovenSSEVector_00c7b970::CUnprovenSSEVector_00c7b970
Original manifest label: `CDateAndTime / CDateAndTime`.
Identity: Native __m128i argument in xmm0, aligned16 store, /GS cookie. Intel vector-library constructor shape; exact class among byte-identical copies not established.
Layout: sixteen-byte intrinsic integer vector; no active shared header. Descriptive class, no original name claimed.

## 00c90d40: CUnprovenLogScale_00c90d40::ApproximateLevel
Original manifest label: `_global / Debug_MatlabFormatted`.
Identity: descriptive arithmetic-only candidate. Own callee loads [EAX], masks sign, signed-converts magnitude bits and scales/subtracts literal floats012bf264/012bf260. 00c90e83 moves pointer into EAX before call00c90e85. Original source name and return precision unproven.
Layout: scalar input only. BLOCKED: retail passes pointer in EAX with plain ret; ordinary fastcall uses ECX. This candidate/test ABI is not equivalent to retail.

## 00c9dd40: CUnprovenLogScale_00c9dd40::ApproximateLevel
Original manifest label: `_global / Float_AbsScale`.
Identity: descriptive arithmetic-only candidate. Own callee loads [EAX], masks sign, signed-converts magnitude bits and scales/subtracts literal floats012bf264/012bf260. 00c9df62 loads address into EAX before call00c9df65; second call00c9df9e. Original source name and return precision unproven.
Layout: scalar input only. BLOCKED: retail passes pointer in EAX with plain ret; ordinary fastcall uses ECX. This candidate/test ABI is not equivalent to retail.

## 00c9ef70: CUnprovenLogScale_00c9ef70::ApproximateLevel
Original manifest label: `_global / Float10_NormalizeAbsValue`.
Identity: descriptive arithmetic-only candidate. Own callee loads [EAX], masks sign, signed-converts magnitude bits and scales/subtracts literal floats012bf264/012bf260. four callsites00c9f0ad/00c9f105/00c9f13f/00c9f277. Original source name and return precision unproven.
Layout: scalar input only. BLOCKED: retail passes pointer in EAX with plain ret; ordinary fastcall uses ECX. This candidate/test ABI is not equivalent to retail.

## 00ca92b0: CMediaSample::SetSyncPoint
Original manifest label: `_gapscan / sub_00ca92b0`.
Identity: original DirectShow IMediaSample2 implementation; own QueryInterface00ca9000 compares IID_IMediaSample56a8689a and IID_IMediaSample2 36b73884, actual table012bf6e0 slots8/16/10 and SDK flags1/4/2.
Layout: no engine owner header. Real IUnknown/IMediaSample/IMediaSample2 inheritance and original SDK CMediaSample fields; m_dwFlags+4 and completePCsize72 corroborated by own AddRef+44 and SetMediaType+3c.

## 00ca9300: CMediaSample::SetDiscontinuity
Original manifest label: `_gapscan / sub_00ca9300`.
Identity: original DirectShow IMediaSample2 implementation; own QueryInterface00ca9000 compares IID_IMediaSample56a8689a and IID_IMediaSample2 36b73884, actual table012bf6e0 slots8/16/10 and SDK flags1/4/2.
Layout: no engine owner header. Real IUnknown/IMediaSample/IMediaSample2 inheritance and original SDK CMediaSample fields; m_dwFlags+4 and completePCsize72 corroborated by own AddRef+44 and SetMediaType+3c.

## 00ca9350: CMediaSample::SetPreroll
Original manifest label: `_gapscan / sub_00ca9350`.
Identity: original DirectShow IMediaSample2 implementation; own QueryInterface00ca9000 compares IID_IMediaSample56a8689a and IID_IMediaSample2 36b73884, actual table012bf6e0 slots8/16/10 and SDK flags1/4/2.
Layout: no engine owner header. Real IUnknown/IMediaSample/IMediaSample2 inheritance and original SDK CMediaSample fields; m_dwFlags+4 and completePCsize72 corroborated by own AddRef+44 and SetMediaType+3c.

## 00cbedba: ScriptPredicates::IsTrue
Original manifest label: `_global / IsFalse`.
Identity: original global fastcall IsTrue in both PDB name sets; own literal true and actual MSVCR71 _stricmp thunk00bfeba8, not strcmp. ScriptPredicates is a descriptive catalog grouping only, function has global scope.
Layout: original CCharString pointer to character-data pointer; no active engine header. Exact original constructor(const char*,long)99ebf0 and destructor99eae0.

## 00cbee0c: ScriptPredicates::IsFalse
Original manifest label: `_global / IsFalse`.
Identity: original global fastcall IsFalse in both PDB name sets; own literal false and actual MSVCR71 _stricmp thunk00bfeba8, not strcmp. ScriptPredicates is a descriptive catalog grouping only, function has global scope.
Layout: original CCharString pointer to character-data pointer; no active engine header. Exact original constructor(const char*,long)99ebf0 and destructor99eae0.

## 00cbee5e: ScriptPredicates::IsNull
Original manifest label: `_global / IsFalse`.
Identity: original global fastcall IsNull in both PDB name sets; own literal null and actual MSVCR71 _stricmp thunk00bfeba8, not strcmp. ScriptPredicates is a descriptive catalog grouping only, function has global scope.
Layout: original CCharString pointer to character-data pointer; no active engine header. Exact original constructor(const char*,long)99ebf0 and destructor99eae0.

## 00cd4ac0: NScript::CEmptyData::Alloc
Original manifest label: `_gapscan / sub_00cd4ac0`.
Identity: original NScript class and static fastcall Alloc name in both PDB name sets; own script-info registration 00cda8a3/00cda8dd pairs literal Empty with this data allocator. Own vtable012c2724 has dtor/OnPersist/Init; own base constructor0099a2f0 is original CBaseClassNonCopyable. The propagated animation-util label is wrong.
Layout: no owner engine header. Genuine CBaseClass/CBaseClassNonCopyable/CScriptDataBase hierarchy, original pure OnPersist/Init interface; size4 and no own fields proven by allocation and constructor.

## 00cd4b20: NScript::Cjoss_test_scriptsData::Alloc
Original manifest label: `_gapscan / sub_00cd4b20`.
Identity: original NScript class and static fastcall Alloc name in both PDB name sets; own script-info registration 00cda705/00cda73f pairs literal joss_test_scripts with this data allocator. Own vtable012c2730 has dtor/OnPersist/Init; own base constructor0099a2f0 is original CBaseClassNonCopyable. The propagated animation-util label is wrong.
Layout: no owner engine header. Genuine CBaseClass/CBaseClassNonCopyable/CScriptDataBase hierarchy, original pure OnPersist/Init interface; size4 and no own fields proven by allocation and constructor.

## 00cd4b80: NScript::CDanyalTestScriptsData::Alloc
Original manifest label: `_gapscan / sub_00cd4b80`.
Identity: original NScript class and static fastcall Alloc name in both PDB name sets; own script-info registration 00cda390/00cda3ca pairs literal DanyalTestScripts with this data allocator. Own vtable012c273c has dtor/OnPersist/Init; own base constructor0099a2f0 is original CBaseClassNonCopyable. The propagated animation-util label is wrong.
Layout: no owner engine header. Genuine CBaseClass/CBaseClassNonCopyable/CScriptDataBase hierarchy, original pure OnPersist/Init interface; size4 and no own fields proven by allocation and constructor.

## 00cfbd90: NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost::Init
Original manifest label: `_gapscan / sub_00cfbd90`.
Identity: own registration00cfb196 assigns named entity factory00cfbce0, storing vtable012c7ba4 with Init slot2. Original PDB scopes/field names; actual game vtable01260f0c slot601 is EntitySetInFaction.
Layout: true CScriptThing CBaseClass and counted handle, entity Game+4/Me+8, ParentClass+20/PMasterData+24/MyGraveNumber+28,32 bytes. Outer parent declaration supplies genuine level-script base and nesting only; unused fields omitted. Header alias retains shared evidence.

## 00da49d0: NScript::CQ_MinionClifftopChaseScript::CMinionWarningGuard::Init
Original manifest label: `_gapscan / sub_00da49d0`.
Identity: own registration00da362b assigns named entity factory00da4920, storing vtable012d7458 with Init slot2. Original PDB scopes/field names; actual game vtable01260f0c slot350 is MiniMapAddMarker.
Layout: true CScriptThing CBaseClass and counted handle, entity Game+4/Me+8, ParentClass+20/PMasterData+24/HealthBarID+28,32 bytes. Outer parent declaration supplies genuine level-script base and nesting only; unused fields omitted. Header alias retains shared evidence.

## 00eb7aa0: NScript::CV_MayorsInvitationScript::CWiseVillager::Init
Original manifest label: `_gapscan / sub_00eb7aa0`.
Identity: own registration00eb6ec9 assigns named entity factory00eb79f0, storing vtable012f0610 with Init slot2. Original PDB scopes/field names; actual game vtable01260f0c slot619 is EntitySetPersonalityOverride.
Layout: true CScriptThing CBaseClass and counted handle, entity Game+4/Me+8, ParentClass+20/PMasterData+24/NumTimesHit+28,32 bytes. Outer parent declaration supplies genuine level-script base and nesting only; unused fields omitted. Header alias retains shared evidence.

## 00cfcb50: NScript::CQ_AwakeningTheOracleScript::CNecropolisEnemy::Main
Original manifest label: `CDarkwoodBalverineTrader / Main`.
Identity: own named registration/factory00cfb326->00cfca60 stores vtable012c7c94 slot1; FableWin original nested Main signatures. Actual game API slots7/70/451 are NewScriptFrame const, GetHero mutable reference, GiveThingBestEnemyTarget const.
Layout: entity20 plus typed parent/master pointers; original CIsThingAlive16, Thing+4 and CPredicateBase slots0 predicate/1 Clone/2 dtor. CScriptThing shared header aliased to real CBaseClass/counting members. Outer declaration supplies nesting and original level base only.

## 00cfddc0: NScript::CQ_AwakeningTheOracleScript::CFinalSummoner::Main
Original manifest label: `CDarkwoodBalverineTrader / Main`.
Identity: own named registration/factory00cfb460->00cfdca0 stores vtable012c7d84 slot1; FableWin original nested Main signatures. Actual game API slots7/70/451 are NewScriptFrame const, GetHero mutable reference, GiveThingBestEnemyTarget const.
Layout: entity20 plus typed parent/master pointers; original CIsThingAlive16, Thing+4 and CPredicateBase slots0 predicate/1 Clone/2 dtor. CScriptThing shared header aliased to real CBaseClass/counting members. Outer declaration supplies nesting and original level base only.

## 00e03480: NScript::CQ_TraderEscortScript::CDarkwoodBalverineTrader::Main
Original manifest label: `CDarkwoodBalverineTrader / Main`.
Identity: own named registration/factory00e00ff8->00e033a0 stores vtable012df5c4 slot1; FableWin original nested Main signatures. Actual game API slots7/70/451 are NewScriptFrame const, GetHero mutable reference, GiveThingBestEnemyTarget const.
Layout: entity20 plus typed parent/master pointers; original CIsThingAlive16, Thing+4 and CPredicateBase slots0 predicate/1 Clone/2 dtor. CScriptThing shared header aliased to real CBaseClass/counting members. Outer declaration supplies nesting and original level base only.

## 00d179b0: NScript::CQ_BountyHuntScript::CBH_GangLeader::OnPersist
Original manifest label: `_gapscan / sub_00d179b0`.
Identity: original PDB member signatures and unique persist field names; own vtable012ca5c4 slot4, constructed at00d163b6. Field strings and typed helper callsites independently identify this owner; donor addresses not used.
Layout: original field types after typed72-byte level-script base or20-byte entity base; PC owner size44. Shared handle headers aliased for true original bases. CPersistContext is an API-only projection, no storage used.

## 00e00ce0: NScript::CQ_TraderEscortScript::OnPersist
Original manifest label: `_gapscan / sub_00e00ce0`.
Identity: original PDB member signatures and unique persist field names; own vtable012df16c slot4, constructed at00e006a8/00e0af86. Field strings and typed helper callsites independently identify this owner; donor addresses not used.
Layout: original field types after typed72-byte level-script base or20-byte entity base; PC owner size124. Shared handle headers aliased for true original bases. CPersistContext is an API-only projection, no storage used.

## 00ed4c50: NScript::CV_SwordInTheStoneScript::OnPersist
Original manifest label: `_gapscan / sub_00ed4c50`.
Identity: original PDB member signatures and unique persist field names; own vtable012f301c slot4, constructed at00ed4abf. Field strings and typed helper callsites independently identify this owner; donor addresses not used.
Layout: original field types after typed72-byte level-script base or20-byte entity base; PC owner size128. Shared handle headers aliased for true original bases. CPersistContext is an API-only projection, no storage used.

## 00d2f710: NScript::CQ_EndGameBossBattleScript::Init
Original manifest label: `_gapscan / sub_00d2f710`.
Identity: own script-info registration 00cd5f68->00d34410 stores factory for literal Q_EndGameBossBattle, whose own allocation/constructor stores vtable012cc60c; target is slot3 Init. Original field names from named PDB class layout, independently checked against own offsets and setter uses.
Layout: shared handle headers aliased to restore real CBaseObject/CBaseClass hierarchy and counted pointers. Original CScriptBase64 typed list/maps, CLevelScriptBase72 Game/MasterScriptData; own registry uses common master-data allocator00cdbd20. Flags at72; PC owner size108. No opaque base prefix.

## 00d78500: NScript::CQ_HeroSoulsScript::Init
Original manifest label: `_gapscan / sub_00d78500`.
Identity: own script-info registration 00cd672d->00d7ac50 stores factory for literal Q_HeroSouls, whose own allocation/constructor stores vtable012d2f48; target is slot3 Init. Original field names from named PDB class layout, independently checked against own offsets and setter uses.
Layout: shared handle headers aliased to restore real CBaseObject/CBaseClass hierarchy and counted pointers. Original CScriptBase64 typed list/maps, CLevelScriptBase72 Game/MasterScriptData; own registry uses common master-data allocator00cdbd20. Flags at72; PC owner size80. No opaque base prefix.

## 00edec50: NScript::CV_TravellingHeroesScript::Init
Original manifest label: `_gapscan / sub_00edec50`.
Identity: own script-info registration 00cd98f2->00ee40a0 stores factory for literal V_TravellingHeroes, whose own allocation/constructor stores vtable012f407c; target is slot3 Init. Original field names from named PDB class layout, independently checked against own offsets and setter uses.
Layout: shared handle headers aliased to restore real CBaseObject/CBaseClass hierarchy and counted pointers. Original CScriptBase64 typed list/maps, CLevelScriptBase72 Game/MasterScriptData; own registry uses common master-data allocator00cdbd20. Flags at120; PC owner size140. No opaque base prefix.

## 00d34410: NScript::CQ_EndGameBossBattleScript::Alloc
Original manifest label: `_gapscan / sub_00d34410`.
Identity: own script-info registration00cd5f68 literalQ_EndGameBossBattle -> factory -> constructor00d2f6b0/vtable012cc60c; exact original PDB signature.
Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation108. Handle headers included under aliases to restore original inheritance. Constructor is external at00d2f6b0.

## 00d83300: NScript::CQ_HeroSoulsBriarScript::Alloc
Original manifest label: `_gapscan / sub_00d83300`.
Identity: own script-info registration00cd68f8 literalQ_HeroSoulsBriar -> factory -> constructor00d80c30/vtable012d41bc; exact original PDB signature.
Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation116. Handle headers included under aliases to restore original inheritance. Constructor is external at00d80c30.

## 00e477a0: NScript::CV_BanditCampPathScript::Alloc
Original manifest label: `_gapscan / sub_00e477a0`.
Identity: own script-info registration00cd823c literalV_BanditCampPath -> factory -> constructor00e46bd0/vtable012e4a9c; exact original PDB signature.
Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation112. Handle headers included under aliases to restore original inheritance. Constructor is external at00e46bd0.

## 00d5ab40: NScript::CQ_GuildTrainingSkillScript::RegisterMain
Original manifest label: `_gapscan / sub_00d5ab40`.
Identity: own binding literalTheRealGuildmaster and factory00d5abe0; original PDB parent/nested factory names and ctor signature; binding vtable012d09d0 slots0 GetEntityScriptName/1 CreateActiveEntityScript. Actual game PostAddScriptedEntities slot64.
Layout: original typed script base and class-template binding28 with CCharString+4,parent+8,member-function pointer+12,factory+16,bool+20,Flags+24. No extra binding destructor/base slot. Parent declaration omits unused own suffix; nested class is a static Alloc API projection only. Shared handle headers aliased.

## 00d5dd40: NScript::CQ_GuildTrainingWillScript::RegisterMain
Original manifest label: `_gapscan / sub_00d5dd40`.
Identity: own binding literalTheRealGuildmaster and factory00d5dde0; original PDB parent/nested factory names and ctor signature; binding vtable012d1068 slots0 GetEntityScriptName/1 CreateActiveEntityScript. Actual game PostAddScriptedEntities slot64.
Layout: original typed script base and class-template binding28 with CCharString+4,parent+8,member-function pointer+12,factory+16,bool+20,Flags+24. No extra binding destructor/base slot. Parent declaration omits unused own suffix; nested class is a static Alloc API projection only. Shared handle headers aliased.

## 00e90830: NScript::CV_GuildMasterScript::RegisterMain
Original manifest label: `_gapscan / sub_00e90830`.
Identity: own binding literalGuildMasterGameFlow and factory00e908d0; original PDB parent/nested factory names and ctor signature; binding vtable012ec168 slots0 GetEntityScriptName/1 CreateActiveEntityScript. Actual game PostAddScriptedEntities slot64.
Layout: original typed script base and class-template binding28 with CCharString+4,parent+8,member-function pointer+12,factory+16,bool+20,Flags+24. No extra binding destructor/base slot. Parent declaration omits unused own suffix; nested class is a static Alloc API projection only. Shared handle headers aliased.

## 00d5dc50: NScript::CQ_GuildTrainingWillScript::CQ_GuildTrainingWillScript
Original manifest label: `CEngineSubPrimitiveBase / CEngineSubPrimitiveBase`.
Identity: own script-info registration00cd62fe literalQ_GuildTrainingWill -> factory -> constructor00d5dc50/vtable012d1038; exact original PDB signature.
Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation108. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.

## 00dfa0b0: NScript::CQ_TraderConflictGoodScript::CQ_TraderConflictGoodScript
Original manifest label: `CEngineSubPrimitiveBase / CEngineSubPrimitiveBase`.
Identity: own script-info registration00cd7750 literalQ_TraderConflictGood -> factory -> constructor00dfa0b0/vtable012deac8; exact original PDB signature.
Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation104. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.

## 00ea86f0: NScript::CV_MurderTwistScript::CV_MurderTwistScript
Original manifest label: `CEngineSubPrimitiveBase / CEngineSubPrimitiveBase`.
Identity: own script-info registration00cd8ffb literalV_MurderTwist -> factory -> constructor00ea86f0/vtable012ee634; exact original PDB signature.
Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation96. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.

## 00d61bb0: NScript::CQ_GuildTrainingWillScript::Alloc
Original manifest label: `_global / Alloc`.
Identity: own script-info registration00cd62fe literalQ_GuildTrainingWill -> factory -> constructor00d5dc50/vtable012d1038; exact original PDB signature.
Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation108. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.

## 00e00610: NScript::CQ_TraderConflictGoodScript::Alloc
Original manifest label: `_global / Alloc`.
Identity: own script-info registration00cd7750 literalQ_TraderConflictGood -> factory -> constructor00dfa0b0/vtable012deac8; exact original PDB signature.
Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation104. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.

## 00eae890: NScript::CV_MurderTwistScript::Alloc
Original manifest label: `_global / Alloc`.
Identity: own script-info registration00cd8ffb literalV_MurderTwist -> factory -> constructor00ea86f0/vtable012ee634; exact original PDB signature.
Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation96. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.

## 00d8fd20: NScript::CQ_HobbeCaveScript::CGranny::OnPredicateFail
Original manifest label: `_gapscan / sub_00d8fd20`.
Identity: own entity allocation/binding00d8fc60 stores012d59e4; selected slot5 is original PDB OnPredicateFail. Actual CScriptThing vtable01238c8c slot18 =004aaaf0 MsgIsKilledBy, const ref-string signature. Original typed fields checked against own writes.
Layout: real polymorphic CScriptThing and script/entity bases, original parent fields and named long ReasonForFailure. Unknown uncalled virtual names are descriptive. Shared handle headers aliased.

## 00da4300: NScript::CQ_MinionClifftopChaseScript::CMapKeeper::OnPredicateFail
Original manifest label: `_gapscan / sub_00da4300`.
Identity: own entity allocation/binding00da34ff->00da4250 stores012d73e0; selected slot5 is original PDB OnPredicateFail. Actual CScriptThing vtable01238c8c slot18 =004aaaf0 MsgIsKilledBy, const ref-string signature. Original typed fields checked against own writes.
Layout: real polymorphic CScriptThing and script/entity bases, original parent fields and named long ReasonForFailure. Unknown uncalled virtual names are descriptive. Shared handle headers aliased.

## 00ea3640: NScript::CV_LostTraderScript::CLostTraderTrader::OnPredicateFail
Original manifest label: `_gapscan / sub_00ea3640`.
Identity: own entity allocation/binding00ea324a->00ea34f0 stores012edda8; selected slot5 is original PDB OnPredicateFail. Actual CScriptThing vtable01238c8c slot18 =004aaaf0 MsgIsKilledBy, const ref-string signature. Original typed fields checked against own writes.
Layout: real polymorphic CScriptThing and script/entity bases, original parent fields and named long ReasonForFailure. Unknown uncalled virtual names are descriptive. Shared handle headers aliased.

## 00d91da0: NScript::CQ_HobbeCaveScript::CGranny::OnPersist
Original manifest label: `_gapscan / sub_00d91da0`.
Identity: original PDB member signatures and unique persist field names; own vtable012d59e4 slot4, constructed at00d8fc60. Field strings and typed helper callsites independently identify this owner; donor addresses not used.
Layout: original field types after typed72-byte level-script base or20-byte entity base; PC owner size40. Shared handle headers aliased for true original bases. CPersistContext is an API-only projection, no storage used.

## 00dd2c80: NScript::CQ_PrisonEscapeRescueMotherScript::OnPersist
Original manifest label: `_gapscan / sub_00dd2c80`.
Identity: original PDB member signatures and unique persist field names; own vtable012db86c slot4, constructed at00dd26e0. Field strings and typed helper callsites independently identify this owner; donor addresses not used.
Layout: original field types after typed72-byte level-script base or20-byte entity base; PC owner size128. Shared handle headers aliased for true original bases. CPersistContext is an API-only projection, no storage used.

## 00e629a0: NScript::CV_ChickenKickingScript::OnPersist
Original manifest label: `_gapscan / sub_00e629a0`.
Identity: original PDB member signatures and unique persist field names; own vtable012e7318 slot4, constructed at00e62830. Field strings and typed helper callsites independently identify this owner; donor addresses not used.
Layout: original field types after typed72-byte level-script base or20-byte entity base; PC owner size144. Shared handle headers aliased for true original bases. CPersistContext is an API-only projection, no storage used.

## 00da30b0: NScript::CQ_MinionClifftopChaseScript::CQ_MinionClifftopChaseScript
Original manifest label: `CV_ArcheryCompetitionScript / CV_ArcheryCompetitionScript`.
Identity: own registration00cd6dc0 literalQ_MinionClifftopChase -> Alloc00daab70/ctor00da30b0 stores012d7158. Original PDB names/fields/signatures. Actual CTimer::TimerIndex long and constructorRegisterTimer slot87, real game-interface RTTI chain.
Layout: typed original script base64 and level base72; own PCsize108; timers MakeUniqueComment/MissionTimer. Shared headers aliased for true inheritance; global game-interface receiver is an API-only projection, no complete instance size claimed. BriarRoseOpinion is nested as original mangled PDB type proves.

## 00e57bb0: NScript::CV_BeggarAndChildScript::CV_BeggarAndChildScript
Original manifest label: `CV_ArcheryCompetitionScript / CV_ArcheryCompetitionScript`.
Identity: own registration00cd836e literalV_BeggarAndChild -> Alloc00e5d0b0/ctor00e57bb0 stores012e65e0. Original PDB names/fields/signatures. Actual CTimer::TimerIndex long and constructorRegisterTimer slot87, real game-interface RTTI chain.
Layout: typed original script base64 and level base72; own PCsize100; timers TauntTimer/BeggarReplyTimer. Shared headers aliased for true inheritance; global game-interface receiver is an API-only projection, no complete instance size claimed. BriarRoseOpinion is nested as original mangled PDB type proves.

## 00e9c360: NScript::CV_IntroductionToTrophiesScript::CV_IntroductionToTrophiesScript
Original manifest label: `CV_ArcheryCompetitionScript / CV_ArcheryCompetitionScript`.
Identity: own registration00cd8d97 literalV_IntroductionToTrophies -> Alloc00e9fb40/ctor00e9c360 stores012ed558. Original PDB names/fields/signatures. Actual CTimer::TimerIndex long and constructorRegisterTimer slot87, real game-interface RTTI chain.
Layout: typed original script base64 and level base72; own PCsize108; timers OpinionTimer/OutsideTavernTimer. Shared headers aliased for true inheritance; global game-interface receiver is an API-only projection, no complete instance size claimed. BriarRoseOpinion is nested as original mangled PDB type proves.

## 00daab70: NScript::CQ_MinionClifftopChaseScript::Alloc
Original manifest label: `_global / Alloc`.
Identity: own registration00cd6dc0 literalQ_MinionClifftopChase -> Alloc00daab70/ctor00da30b0 stores012d7158. Original PDB names/fields/signatures. Actual CTimer::TimerIndex long and constructorRegisterTimer slot87, real game-interface RTTI chain.
Layout: typed original script base64 and level base72; own PCsize108; timers MakeUniqueComment/MissionTimer. Shared headers aliased for true inheritance; global game-interface receiver is an API-only projection, no complete instance size claimed. BriarRoseOpinion is nested as original mangled PDB type proves.

## 00e5d0b0: NScript::CV_BeggarAndChildScript::Alloc
Original manifest label: `_global / Alloc`.
Identity: own registration00cd836e literalV_BeggarAndChild -> Alloc00e5d0b0/ctor00e57bb0 stores012e65e0. Original PDB names/fields/signatures. Actual CTimer::TimerIndex long and constructorRegisterTimer slot87, real game-interface RTTI chain.
Layout: typed original script base64 and level base72; own PCsize100; timers TauntTimer/BeggarReplyTimer. Shared headers aliased for true inheritance; global game-interface receiver is an API-only projection, no complete instance size claimed. BriarRoseOpinion is nested as original mangled PDB type proves.

## 00e9fb40: NScript::CV_IntroductionToTrophiesScript::Alloc
Original manifest label: `_global / Alloc`.
Identity: own registration00cd8d97 literalV_IntroductionToTrophies -> Alloc00e9fb40/ctor00e9c360 stores012ed558. Original PDB names/fields/signatures. Actual CTimer::TimerIndex long and constructorRegisterTimer slot87, real game-interface RTTI chain.
Layout: typed original script base64 and level base72; own PCsize108; timers OpinionTimer/OutsideTavernTimer. Shared headers aliased for true inheritance; global game-interface receiver is an API-only projection, no complete instance size claimed. BriarRoseOpinion is nested as original mangled PDB type proves.

## 00dc4d00: NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::OnPredicateFail
Original manifest label: `_gapscan / sub_00dc4d00`.
Identity: own named binding/factory00dc2ada->00dc4c40 stores vtable012da6a8 selected slot5. Original scoped names in FableWin name set; actual MsgIsKilledBy slot18. Own counter+108 and master flag+284 agree with original field names.
Layout: true script/entity/data bases and original typed fields. Master enum value spellings unspecified; EHeroNames retains donor field type and global-scope projection, no enumerator or enum scope recovery claim. Other enum scopes checked against original decorated Transfer names. Retail master size324 after STLport vector contraction. Shared handle headers aliased.

## 00dc5f10: NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageWoman::OnPredicateFail
Original manifest label: `_gapscan / sub_00dc5f10`.
Identity: own named binding/factory00dc2b43->00dc5e50 stores vtable012da7a4 selected slot5. Original scoped names in FableWin name set; actual MsgIsKilledBy slot18. Own counter+108 and master flag+284 agree with original field names.
Layout: true script/entity/data bases and original typed fields. Master enum value spellings unspecified; EHeroNames retains donor field type and global-scope projection, no enumerator or enum scope recovery claim. Other enum scopes checked against original decorated Transfer names. Retail master size324 after STLport vector contraction. Shared handle headers aliased.

## 00dc7420: NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageKeeperFriend::OnPredicateFail
Original manifest label: `_gapscan / sub_00dc7420`.
Identity: own named binding/factory00dc2bad->00dc7360 stores vtable012da8c8 selected slot5. Original scoped names in FableWin name set; actual MsgIsKilledBy slot18. Own counter+108 and master flag+284 agree with original field names.
Layout: true script/entity/data bases and original typed fields. Master enum value spellings unspecified; EHeroNames retains donor field type and global-scope projection, no enumerator or enum scope recovery claim. Other enum scopes checked against original decorated Transfer names. Retail master size324 after STLport vector contraction. Shared handle headers aliased.

## 00e24760: NScript::CPredicate_And<CIsThingAlive,CIsThingAlive>::CPredicate_And
Original manifest label: `CConsoleCommandParameters / CConsoleCommandParameters`.
Identity: original NScript CPredicate_And and original CIsThingAlive/CIsThingAlive names in both PDB name sets; own aggregate vtable012e1b70 contains evaluation/Clone/destructor slots0/1/2, embedded tables012c32f0/012c32f0 identify separate operand types. Original CScriptThing(const CScriptThing&)004abe90.
Layout: active CScriptThing header aliased for actual CBaseClass inheritance and real counted pointer. Direct original CPredicateBase interface order is operator()/Clone/dtor (no invented CBaseClass root); nested Predicate1+4,Predicate2+20, each Thing+4. PC conjunction size36.

## 00ef7b50: NScript::CPredicate_And<CIsThingConscious,CIsThingAlive>::CPredicate_And
Original manifest label: `CConsoleCommandParameters / CConsoleCommandParameters`.
Identity: original NScript CPredicate_And and original CIsThingConscious/CIsThingAlive names in both PDB name sets; own aggregate vtable012f6818 contains evaluation/Clone/destructor slots0/1/2, embedded tables012c2fe8/012c32f0 identify separate operand types. Original CScriptThing(const CScriptThing&)004abe90.
Layout: active CScriptThing header aliased for actual CBaseClass inheritance and real counted pointer. Direct original CPredicateBase interface order is operator()/Clone/dtor (no invented CBaseClass root); nested Predicate1+4,Predicate2+20, each Thing+4. PC conjunction size36.

## 00ef8740: NScript::CPredicate_And<CIsThingConscious,CIsThingConscious>::CPredicate_And
Original manifest label: `CConsoleCommandParameters / CConsoleCommandParameters`.
Identity: original NScript CPredicate_And and original CIsThingConscious/CIsThingConscious names in both PDB name sets; own aggregate vtable012f687c contains evaluation/Clone/destructor slots0/1/2, embedded tables012c2fe8/012c2fe8 identify separate operand types. Original CScriptThing(const CScriptThing&)004abe90.
Layout: active CScriptThing header aliased for actual CBaseClass inheritance and real counted pointer. Direct original CPredicateBase interface order is operator()/Clone/dtor (no invented CBaseClass root); nested Predicate1+4,Predicate2+20, each Thing+4. PC conjunction size36.

## 00e247a0: NScript::CPredicate_And<CIsThingAlive,CIsThingAlive>::Clone
Original manifest label: `_gapscan / sub_00e247a0`.
Identity: original NScript CPredicate_And and original CIsThingAlive/CIsThingAlive names in both PDB name sets; own aggregate vtable012e1b70 contains evaluation/Clone/destructor slots0/1/2, embedded tables012c32f0/012c32f0 identify separate operand types. Original CScriptThing(const CScriptThing&)004abe90.
Layout: active CScriptThing header aliased for actual CBaseClass inheritance and real counted pointer. Direct original CPredicateBase interface order is operator()/Clone/dtor (no invented CBaseClass root); nested Predicate1+4,Predicate2+20, each Thing+4. PC conjunction size36.

## 00ef7b90: NScript::CPredicate_And<CIsThingConscious,CIsThingAlive>::Clone
Original manifest label: `_gapscan / sub_00ef7b90`.
Identity: original NScript CPredicate_And and original CIsThingConscious/CIsThingAlive names in both PDB name sets; own aggregate vtable012f6818 contains evaluation/Clone/destructor slots0/1/2, embedded tables012c2fe8/012c32f0 identify separate operand types. Original CScriptThing(const CScriptThing&)004abe90.
Layout: active CScriptThing header aliased for actual CBaseClass inheritance and real counted pointer. Direct original CPredicateBase interface order is operator()/Clone/dtor (no invented CBaseClass root); nested Predicate1+4,Predicate2+20, each Thing+4. PC conjunction size36.

## 00ef8780: NScript::CPredicate_And<CIsThingConscious,CIsThingConscious>::Clone
Original manifest label: `_gapscan / sub_00ef8780`.
Identity: original NScript CPredicate_And and original CIsThingConscious/CIsThingConscious names in both PDB name sets; own aggregate vtable012f687c contains evaluation/Clone/destructor slots0/1/2, embedded tables012c2fe8/012c2fe8 identify separate operand types. Original CScriptThing(const CScriptThing&)004abe90.
Layout: active CScriptThing header aliased for actual CBaseClass inheritance and real counted pointer. Direct original CPredicateBase interface order is operator()/Clone/dtor (no invented CBaseClass root); nested Predicate1+4,Predicate2+20, each Thing+4. PC conjunction size36.

## 00e2e400: NScript::CV_AmbushScamScript::ResetRandomSpeechTime
Original manifest label: `CV_AmbushScamScript / ResetRandomSpeechTime`.
Identity: original PDB member names, own callers00e2cd88 and distinct script-def field pairs; CTimer GetIndex is original long const accessor. Actual global game interface SetTimer slot89 and MSVCR71 rand thunk00bfeb16.
Layout: original typed script prefix through timer+124, unused suffix omitted. True CScriptDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase; own constructor00f28f90 base initialization confirms packed parent57, first own long60, original typed vectors12. Def Max/Var+696/700. Only read prefix modeled, no complete owner/definition size claimed. Shared headers aliased.

## 00e337c0: NScript::CV_ArcheryCompetitionScript::ResetRandomSpeechTime
Original manifest label: `CV_AmbushScamScript / ResetRandomSpeechTime`.
Identity: original PDB member names, own callers00e3372f/00e34332/00e34a77/00e34b70/00e34d3e and distinct script-def field pairs; CTimer GetIndex is original long const accessor. Actual global game interface SetTimer slot89 and MSVCR71 rand thunk00bfeb16.
Layout: original typed script prefix through timer+112, unused suffix omitted. True CScriptDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase; own constructor00f28f90 base initialization confirms packed parent57, first own long60, original typed vectors12. Def Max/Var+912/916. Only read prefix modeled, no complete owner/definition size claimed. Shared headers aliased.

## 00e53c70: NScript::CV_BeardyBaldyScript::ResetRandomSpeechTime
Original manifest label: `CV_AmbushScamScript / ResetRandomSpeechTime`.
Identity: original PDB member names, own callers00e512d3/00e51409/00e518cb and distinct script-def field pairs; CTimer GetIndex is original long const accessor. Actual global game interface SetTimer slot89 and MSVCR71 rand thunk00bfeb16.
Layout: original typed script prefix through timer+116, unused suffix omitted. True CScriptDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase; own constructor00f28f90 base initialization confirms packed parent57, first own long60, original typed vectors12. Def Max/Var+1172/1176. Only read prefix modeled, no complete owner/definition size claimed. Shared headers aliased.

## 00e64d10: NScript::CV_ChickenKickingScript::CChickenSign::Alloc
Original manifest label: `_gapscan / sub_00e64d10`.
Identity: proven own named binding registration00e62b4d ->factory00e64d10 ->original constructor00e64d90 and exact PDB owner/Alloc/constructor prototypes. Public CCountedPointer<CEntityScriptBase>(T*) and static fastcall DeleteData(void*) are PDB-proven; own controlblock stores callback00cdee00, which consumes ECX and deletes through entity virtual dtor.
Layout: actual entity20 Game/Me prefix +original parent/master fields and typed own members, PCsize40. True counted pointer8 and controlblock12 RefCount/fastcallDeleteFunc/Data; no invented constructor/helper signatures for controlblock. Source directly initializes original fields before assigning Info. Non-POD default/copy/dtor declarations preserve native class-return hidden result ABI; genuine original virtual Entity destructor and correct out-of-line constructor target. Parent typed level72/script64 prefix retained, unused parent suffix explicitly omitted. Shared handle headers aliased.

## 00eb1f80: NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::Alloc
Original manifest label: `_gapscan / sub_00eb1f80`.
Identity: proven own named binding registration00eaead0 ->factory00eb1f80 ->original constructor00eb2000 and exact PDB owner/Alloc/constructor prototypes. Public CCountedPointer<CEntityScriptBase>(T*) and static fastcall DeleteData(void*) are PDB-proven; own controlblock stores callback00cdee00, which consumes ECX and deletes through entity virtual dtor.
Layout: actual entity20 Game/Me prefix +original parent/master fields and typed own members, PCsize56. True counted pointer8 and controlblock12 RefCount/fastcallDeleteFunc/Data; no invented constructor/helper signatures for controlblock. Source uses the original PDB-proven three-argument controlblock constructor with the observed field initialization order; no invented helper or constructor signature. Non-POD default/copy/dtor declarations preserve native class-return hidden result ABI; genuine original virtual Entity destructor and correct out-of-line constructor target. Parent typed level72/script64 prefix retained, unused parent suffix explicitly omitted. Shared handle headers aliased.

## 00ee0240: NScript::CV_TravellingHeroesScript::CTH_SnowTroll::Alloc
Original manifest label: `_gapscan / sub_00ee0240`.
Identity: proven own named binding registration00edef50 ->factory00ee0240 ->original constructor00ee02c0 and exact PDB owner/Alloc/constructor prototypes. Public CCountedPointer<CEntityScriptBase>(T*) and static fastcall DeleteData(void*) are PDB-proven; own controlblock stores callback00cdee00, which consumes ECX and deletes through entity virtual dtor.
Layout: actual entity20 Game/Me prefix +original parent/master fields and typed own members, PCsize52. True counted pointer8 and controlblock12 RefCount/fastcallDeleteFunc/Data; no invented constructor/helper signatures for controlblock. Source uses the original PDB-proven three-argument controlblock constructor with the observed field initialization order; no invented helper or constructor signature. Non-POD default/copy/dtor declarations preserve native class-return hidden result ABI; genuine original virtual Entity destructor and correct out-of-line constructor target. Parent typed level72/script64 prefix retained, unused parent suffix explicitly omitted. Shared handle headers aliased.

## 00e730a0: NScript::CV_DemonDoorsScript::CDF_ThreeSuits::DressedAsGoodKnight
Original manifest label: `CDF_ThreeSuits / DressedAsEvilMage`.
Identity: proven; original PDB method owner/privateAAE or publicQAE signature and own four unique item-definition literals. Original Game interface GetHero()ref slot70/IsWearingClothingItem(constThing&,constString&)const slot187 follow actual own vtable01260f0c. First two are GoodKnight/EvilMage nested DemonDoors entity scripts, third is TalentlessBard apprentice method; catalog copied labels are false.
Layout: original typed script64/level72 and entity20 API projection (Game+4/Me+8), nested parent/master fields28; TalentlessBard own suffix unused and explicitly omitted. Full original declared bases where known; CEntityScriptBase polymorphic prefix has no proven further base. Available handle headers aliased. Game interface actual NonCopyable/CBaseClass chain; unused slots explicitly descriptive. True CCharString implicit ctor(constchar*,long) and external dtor retain conditional temporary lifetimes, short circuit and original virtual arguments.

## 00e731f0: NScript::CV_DemonDoorsScript::CDF_ThreeSuits::DressedAsEvilMage
Original manifest label: `CDF_ThreeSuits / DressedAsEvilMage`.
Identity: proven; original PDB method owner/privateAAE or publicQAE signature and own four unique item-definition literals. Original Game interface GetHero()ref slot70/IsWearingClothingItem(constThing&,constString&)const slot187 follow actual own vtable01260f0c. First two are GoodKnight/EvilMage nested DemonDoors entity scripts, third is TalentlessBard apprentice method; catalog copied labels are false.
Layout: original typed script64/level72 and entity20 API projection (Game+4/Me+8), nested parent/master fields28; TalentlessBard own suffix unused and explicitly omitted. Full original declared bases where known; CEntityScriptBase polymorphic prefix has no proven further base. Available handle headers aliased. Game interface actual NonCopyable/CBaseClass chain; unused slots explicitly descriptive. True CCharString implicit ctor(constchar*,long) and external dtor retain conditional temporary lifetimes, short circuit and original virtual arguments.

## 00eda490: NScript::CV_TalentlessBardScript::IsHeroDressedAsApprentice
Original manifest label: `CDF_ThreeSuits / DressedAsEvilMage`.
Identity: proven; original PDB method owner/privateAAE or publicQAE signature and own four unique item-definition literals. Original Game interface GetHero()ref slot70/IsWearingClothingItem(constThing&,constString&)const slot187 follow actual own vtable01260f0c. First two are GoodKnight/EvilMage nested DemonDoors entity scripts, third is TalentlessBard apprentice method; catalog copied labels are false.
Layout: original typed script64/level72 and entity20 API projection (Game+4/Me+8), nested parent/master fields28; TalentlessBard own suffix unused and explicitly omitted. Full original declared bases where known; CEntityScriptBase polymorphic prefix has no proven further base. Available handle headers aliased. Game interface actual NonCopyable/CBaseClass chain; unused slots explicitly descriptive. True CCharString implicit ctor(constchar*,long) and external dtor retain conditional temporary lifetimes, short circuit and original virtual arguments.

## 00e75790: NScript::CV_ExposeMayorScript::OnPersist
Original manifest label: `_gapscan / sub_00e75790`.
Identity: proven original field-name literals in own body reconcile original named PDB owner layout and public virtual OnPersist signature; actual script virtualslot4. PATH_ID/STAGE_IN_PATH are original nested enums of CV_ExposeMayorScript (PDB template Transfer qualifiers), zero enumerator names explicitly descriptive. Exact bool/long/enum reference Transfer<T>(constchar*,T&,constT&) APIs.
Layout: accepted original typed CScriptBase64/CLevelScriptBase72, original own fields and full typed handle inheritance; retail size160. CCountedPointer specialmembers retained; actual CCharString ctor(constchar*,long) and non-POD members, true context44 typed prefix. Unused original interfaces omitted explicitly; shared handle headers aliased. Own flag/scalar offsets reconcile donor20-byte larger CScriptBase.

## 00eb98a0: NScript::CV_MayorsInvitationScript::OnPersist
Original manifest label: `_gapscan / sub_00eb98a0`.
Identity: proven original field-name literals in own body reconcile original named PDB owner layout and public virtual OnPersist signature; actual script virtualslot4. PATH_ID/STAGE_IN_PATH are original nested enums of CV_ExposeMayorScript (PDB template Transfer qualifiers), zero enumerator names explicitly descriptive. Exact bool/long/enum reference Transfer<T>(constchar*,T&,constT&) APIs.
Layout: accepted original typed CScriptBase64/CLevelScriptBase72, original own fields and full typed handle inheritance; retail size108. CCountedPointer specialmembers retained; actual CCharString ctor(constchar*,long) and non-POD members, true context44 typed prefix. Unused original interfaces omitted explicitly; shared handle headers aliased. Own flag/scalar offsets reconcile donor20-byte larger CScriptBase.

## 00ecd7b0: NScript::CV_SickChildScript::OnPersist
Original manifest label: `_gapscan / sub_00ecd7b0`.
Identity: proven original field-name literals in own body reconcile original named PDB owner layout and public virtual OnPersist signature; actual script virtualslot4. PATH_ID/STAGE_IN_PATH are original nested enums of CV_ExposeMayorScript (PDB template Transfer qualifiers), zero enumerator names explicitly descriptive. Exact bool/long/enum reference Transfer<T>(constchar*,T&,constT&) APIs.
Layout: accepted original typed CScriptBase64/CLevelScriptBase72, original own fields and full typed handle inheritance; retail size116. CCountedPointer specialmembers retained; actual CCharString ctor(constchar*,long) and non-POD members, true context44 typed prefix. Unused original interfaces omitted explicitly; shared handle headers aliased. Own flag/scalar offsets reconcile donor20-byte larger CScriptBase.

## 00eadb60: NScript::CV_MurderTwistScript::CMurderTwistBrotherYounger::OnPredicateFail
Original manifest label: `CMurderTwistBrother / OnPredicateFail`.
Identity: proven own named binding00ea88df ->original class factory ->vtable012ef2c0 slot5; original PDB privatevirtual EAE method. Actual entity20 Game/Me prefix, parent/master+20/+24, original BrainState28 where present; parent exact script96 with ItemsFound72/State76/Event80 and typed vector84.
Layout: real typed script/level/handle storage; original CScriptThing nonvirtual operator== forwards protected virtual IsEqualTo(constThing&)const slot78 through original PImp.Data; base default IsEqualTo returns false as own004aa8a0. Original game const RemoveThing108/EntitySetAsKillable517; genuine STLport vector begin/end/erase/push_back/_M_insert_overflow. Counted-info unsigned refcount and copy constructor retain actual addref. Unused virtual slots are descriptive projections; no fabricated active APIs. Shared handle headers aliased; fixtures alone expose protected storage.

## 00eae550: NScript::CV_MurderTwistScript::CMurderTwistBrother::OnPredicateFail
Original manifest label: `CMurderTwistBrother / OnPredicateFail`.
Identity: proven own named binding00ea8943 ->original class factory ->vtable012ef2dc slot5; original PDB privatevirtual EAE method. Actual entity20 Game/Me prefix, parent/master+20/+24, original BrainState28 where present; parent exact script96 with ItemsFound72/State76/Event80 and typed vector84.
Layout: real typed script/level/handle storage; original CScriptThing nonvirtual operator== forwards protected virtual IsEqualTo(constThing&)const slot78 through original PImp.Data; base default IsEqualTo returns false as own004aa8a0. Original game const RemoveThing108/EntitySetAsKillable517; genuine STLport vector begin/end/erase/push_back/_M_insert_overflow. Counted-info unsigned refcount and copy constructor retain actual addref. Unused virtual slots are descriptive projections; no fabricated active APIs. Shared handle headers aliased; fixtures alone expose protected storage.

## 00eae620: NScript::CV_MurderTwistScript::CMurderTwistBuriedGem::OnPredicateFail
Original manifest label: `CMurderTwistBrother / OnPredicateFail`.
Identity: proven own named binding00ea8ad3 ->original class factory ->vtable012ef7c8 slot5; original PDB privatevirtual EAE method. Actual entity20 Game/Me prefix, parent/master+20/+24, original BrainState28 where present; parent exact script96 with ItemsFound72/State76/Event80 and typed vector84.
Layout: real typed script/level/handle storage; original CScriptThing nonvirtual operator== forwards protected virtual IsEqualTo(constThing&)const slot78 through original PImp.Data; base default IsEqualTo returns false as own004aa8a0. Original game const RemoveThing108/EntitySetAsKillable517; genuine STLport vector begin/end/erase/push_back/_M_insert_overflow. Counted-info unsigned refcount and copy constructor retain actual addref. Unused virtual slots are descriptive projections; no fabricated active APIs. Shared handle headers aliased; fixtures alone expose protected storage.

## 00eae670: NScript::CV_MurderTwistScript::CMurderTwistBrotherYounger::Init
Original manifest label: `_gapscan / sub_00eae670`.
Identity: proven own named binding00ea88df ->original class factory ->vtable012ef2c0 slot2; original PDB privatevirtual EAE method. Actual entity20 Game/Me prefix, parent/master+20/+24, original BrainState28 where present; parent exact script96 with ItemsFound72/State76/Event80 and typed vector84.
Layout: real typed script/level/handle storage; original CScriptThing nonvirtual operator== forwards protected virtual IsEqualTo(constThing&)const slot78 through original PImp.Data; base default IsEqualTo returns false as own004aa8a0. Original game const RemoveThing108/EntitySetAsKillable517; genuine STLport vector begin/end/erase/push_back/_M_insert_overflow. Counted-info unsigned refcount and copy constructor retain actual addref. Unused virtual slots are descriptive projections; no fabricated active APIs. Shared handle headers aliased; fixtures alone expose protected storage.

## 00eae700: NScript::CV_MurderTwistScript::CMurderTwistBrother::Init
Original manifest label: `_gapscan / sub_00eae700`.
Identity: proven own named binding00ea8943 ->original class factory ->vtable012ef2dc slot2; original PDB privatevirtual EAE method. Actual entity20 Game/Me prefix, parent/master+20/+24, original BrainState28 where present; parent exact script96 with ItemsFound72/State76/Event80 and typed vector84.
Layout: real typed script/level/handle storage; original CScriptThing nonvirtual operator== forwards protected virtual IsEqualTo(constThing&)const slot78 through original PImp.Data; base default IsEqualTo returns false as own004aa8a0. Original game const RemoveThing108/EntitySetAsKillable517; genuine STLport vector begin/end/erase/push_back/_M_insert_overflow. Counted-info unsigned refcount and copy constructor retain actual addref. Unused virtual slots are descriptive projections; no fabricated active APIs. Shared handle headers aliased; fixtures alone expose protected storage.

## 00eae790: NScript::CV_MurderTwistScript::CMurderTwistBrotherFriend::Init
Original manifest label: `_gapscan / sub_00eae790`.
Identity: proven own named binding00ea8a0b ->original class factory ->vtable012ef604 slot2; original PDB privatevirtual EAE method. Actual entity20 Game/Me prefix, parent/master+20/+24, original BrainState28 where present; parent exact script96 with ItemsFound72/State76/Event80 and typed vector84.
Layout: real typed script/level/handle storage; original CScriptThing nonvirtual operator== forwards protected virtual IsEqualTo(constThing&)const slot78 through original PImp.Data; base default IsEqualTo returns false as own004aa8a0. Original game const RemoveThing108/EntitySetAsKillable517; genuine STLport vector begin/end/erase/push_back/_M_insert_overflow. Counted-info unsigned refcount and copy constructor retain actual addref. Unused virtual slots are descriptive projections; no fabricated active APIs. Shared handle headers aliased; fixtures alone expose protected storage.

## 00ee9f00: NScript::CExpression_FishScript::Alloc
Original manifest label: `_gapscan / sub_00ee9f00`.
Identity: proven; own RegisterAllScripts registration00cd9b7c literalExpression_Fish ->factory00ee9f00 ->ownvtable012f54e4. Exact PDB Alloc/data pointer/Game reference and public constructor(Game&) namesets. Data argument ignored.
Layout: original typed CScriptBase64 (Threads list, EntityScripts/Bindings maps, runtime persist map) ->CLevelScriptNoMasterBase68 (Game+64), original own fields and PCsize72. No MasterScriptData here. Accepted main N228 shared typed prefix reused read-only; available handle headers aliased. Counted-pointer special members explicitly non-POD, original base ctor00cb8110/dtor00cbd510 external declarations. No fake flat vptr, extra callbacks or invented owner arguments.

## 00eeb730: NScript::CExpression_PickpocketScript::Alloc
Original manifest label: `_gapscan / sub_00eeb730`.
Identity: proven; own RegisterAllScripts registration00cd9de0 literalExpression_Pickpocket ->factory00eeb730 ->ownvtable012f56a0. Exact PDB Alloc/data pointer/Game reference and public constructor(Game&) namesets. Data argument ignored.
Layout: original typed CScriptBase64 (Threads list, EntityScripts/Bindings maps, runtime persist map) ->CLevelScriptNoMasterBase68 (Game+64), original own fields and PCsize68. No MasterScriptData here. Accepted main N228 shared typed prefix reused read-only; available handle headers aliased. Counted-pointer special members explicitly non-POD, original base ctor00cb8110/dtor00cbd510 external declarations. No fake flat vptr, extra callbacks or invented owner arguments.

## 00eed060: NScript::CGlobal_ToggleTimeDisplayScript::Alloc
Original manifest label: `_gapscan / sub_00eed060`.
Identity: proven; own RegisterAllScripts registration00cda044 literalGlobal_ToggleTimeDisplay ->factory00eed060 ->ownvtable012f57a0. Exact PDB Alloc/data pointer/Game reference and public constructor(Game&) namesets. Data argument ignored.
Layout: original typed CScriptBase64 (Threads list, EntityScripts/Bindings maps, runtime persist map) ->CLevelScriptNoMasterBase68 (Game+64), original own fields and PCsize72. No MasterScriptData here. Accepted main N228 shared typed prefix reused read-only; available handle headers aliased. Counted-pointer special members explicitly non-POD, original base ctor00cb8110/dtor00cbd510 external declarations. No fake flat vptr, extra callbacks or invented owner arguments.

## 00eed360: NScript::CPersonalScript_alextestScript::RegisterMain
Original manifest label: `_gapscan / sub_00eed360`.
Identity: own literals/factory pointers BurningPerson->00eed4d0,Signpost->00eed5d0,StopDrawing->00eed7d0 match original PDB nested owner names/static Alloc prototypes. Actual game PostAddScriptedEntities slot64. Original base data CalextestData comes from nested ParentClass/PMasterData donor fields, not donor addresses.
Layout: accepted main N230 genuine binding28: abstract interface slot0 GetEntityScriptName/slot1 CreateActiveEntityScript (no destructor), CCharString+4,parent+8,memberfunctionpointer+12,fastcall class-return factory+16,bool20,Flags24. CCountedPointer explicitly non-POD copy/dtor identity retained. Full typed CScriptBase64/level72; unused parent suffix explicitly omitted, nested entity20 +original parent/master fields. Correct CCharString ctor(constchar*,long). Real compiler new-expression temporary lifetime; no fake allocation/helper callback. Shared handles aliased.

## 00efca00: NScript::CMarkTestScriptsScript::RegisterMain
Original manifest label: `_gapscan / sub_00efca00`.
Identity: own literals/factory pointers TestMan->00efcb70,Gift->00efd680,ShopKeeper->00efd9c0 match original PDB nested owner names/static Alloc prototypes. Actual game PostAddScriptedEntities slot64. Original base data CQ_SunnyvaleMasterData comes from nested ParentClass/PMasterData donor fields, not donor addresses.
Layout: accepted main N230 genuine binding28: abstract interface slot0 GetEntityScriptName/slot1 CreateActiveEntityScript (no destructor), CCharString+4,parent+8,memberfunctionpointer+12,fastcall class-return factory+16,bool20,Flags24. CCountedPointer explicitly non-POD copy/dtor identity retained. Full typed CScriptBase64/level72; unused parent suffix explicitly omitted, nested entity20 +original parent/master fields. Correct CCharString ctor(constchar*,long). Real compiler new-expression temporary lifetime; no fake allocation/helper callback. Shared handles aliased.

## 00efe4b0: NScript::CBenTestScriptsScript::RegisterMain
Original manifest label: `_gapscan / sub_00efe4b0`.
Identity: own literals/factory pointers Dummy->00efe620,Fader->00efe7f0,BalvMarker->00efea80 match original PDB nested owner names/static Alloc prototypes. Actual game PostAddScriptedEntities slot64. Original base data CQ_SunnyvaleMasterData comes from nested ParentClass/PMasterData donor fields, not donor addresses.
Layout: accepted main N230 genuine binding28: abstract interface slot0 GetEntityScriptName/slot1 CreateActiveEntityScript (no destructor), CCharString+4,parent+8,memberfunctionpointer+12,fastcall class-return factory+16,bool20,Flags24. CCountedPointer explicitly non-POD copy/dtor identity retained. Full typed CScriptBase64/level72; unused parent suffix explicitly omitted, nested entity20 +original parent/master fields. Correct CCharString ctor(constchar*,long). Real compiler new-expression temporary lifetime; no fake allocation/helper callback. Shared handles aliased.

## 00f25b00: std::_Construct<CArenaRoundDef,CArenaRoundDef>
Original manifest label: `_Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard / _Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&>`.
Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f26db0: std::_Construct<CHeroSoulsRoundDef,CHeroSoulsRoundDef>
Original manifest label: `_Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard / _Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&>`.
Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f26f00: std::_Construct<CHeroSoulsWaveDef,CHeroSoulsWaveDef>
Original manifest label: `_Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard / _Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&>`.
Identity: proven; own virtual Transfer slot18 literals NumWaveCreatures/Creatures, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f26420: CPersistContext_TransferableVectorOfSubComponents<CArenaCreatureDef>::AreDefaultValsApplied
Original manifest label: `_gapscan / sub_00f26420`.
Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.

## 00f276a0: CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsCreatureDef>::AreDefaultValsApplied
Original manifest label: `_gapscan / sub_00f276a0`.
Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.

## 00f34510: CPersistContext_TransferableVectorOfSubComponents<CFeatDef>::AreDefaultValsApplied
Original manifest label: `_gapscan / sub_00f34510`.
Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.

## 00f26440: CPersistContext_TransferableVectorOfSubComponents<CArenaCreatureDef>::SetDefaultValsApplied
Original manifest label: `_gapscan / sub_00f26440`.
Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.

## 00f276c0: CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsCreatureDef>::SetDefaultValsApplied
Original manifest label: `_gapscan / sub_00f276c0`.
Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.

## 00f34530: CPersistContext_TransferableVectorOfSubComponents<CFeatDef>::SetDefaultValsApplied
Original manifest label: `_gapscan / sub_00f34530`.
Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.

## 00f26470: CPersistContext_TransferableVectorOfSubComponents<CArenaCreatureDef>::Transfer
Original manifest label: `_gapscan / sub_00f26470`.
Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.

## 00f276f0: CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsCreatureDef>::Transfer
Original manifest label: `_gapscan / sub_00f276f0`.
Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.

## 00f34560: CPersistContext_TransferableVectorOfSubComponents<CFeatDef>::Transfer
Original manifest label: `_gapscan / sub_00f34560`.
Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.

## 00f268c0: CPersistContext_TransferableVectorOfSubComponents<CArenaCreatureDef>::Resize
Original manifest label: `CPersistContext / Transfer<CAnimationSet>`.
Identity: proven original privatevirtual Resize(long), five-entry generic wrapper vtable. Own temporary constructor00f263d0, destructor00f25e40, typed vector resize00f26490; constructor fields/literals reconcile original donor records and retail size56.
Layout: original DefClass37/CDefPointee/Resource/CIV base chain and named typed fields; genuine STLport vector base/proxy, original constructors/destructors and real vector resize API. Unknown unused definition slots2–17 are explicit projections, Transfer18. Available shared flattened base headers are referenced in accompanying model evidence.

## 00f27b60: CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsCreatureDef>::Resize
Original manifest label: `CPersistContext / Transfer<CAnimationSet>`.
Identity: proven original privatevirtual Resize(long), five-entry generic wrapper vtable. Own temporary constructor00f27650, destructor00f270d0, typed vector resize00f27710; constructor fields/literals reconcile original donor records and retail size52.
Layout: original DefClass37/CDefPointee/Resource/CIV base chain and named typed fields; genuine STLport vector base/proxy, original constructors/destructors and real vector resize API. Unknown unused definition slots2–17 are explicit projections, Transfer18. Available shared flattened base headers are referenced in accompanying model evidence.

## 00f33b60: CPersistContext_TransferableVectorOfSubComponents<CConversation>::Resize
Original manifest label: `CPersistContext / Transfer<CAnimationSet>`.
Identity: proven original privatevirtual Resize(long), five-entry generic wrapper vtable. Own temporary constructor00f336f0, destructor00e54d80, typed vector resize00f33730; constructor fields/literals reconcile original donor records and retail size92.
Layout: original DefClass37/CDefPointee/Resource/CIV base chain and named typed fields; genuine STLport vector base/proxy, original constructors/destructors and real vector resize API. Unknown unused definition slots2–17 are explicit projections, Transfer18. Available shared flattened base headers are referenced in accompanying model evidence.

## 00f268f0: CArenaRoundDef::Copy
Original manifest label: `_gapscan / sub_00f268f0`.
Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f271d0: CHeroSoulsWaveDef::Copy
Original manifest label: `_gapscan / sub_00f271d0`.
Identity: proven; own virtual Transfer slot18 literals NumWaveCreatures/Creatures, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f27b90: CHeroSoulsRoundDef::Copy
Original manifest label: `_gapscan / sub_00f27b90`.
Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f27490: CHeroSoulsWaveDef::Transfer
Original manifest label: `_gapscan / sub_00f27490`.
Identity: proven; own virtual Transfer slot18 literals NumWaveCreatures/Creatures, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f27e60: CArenaRoundDef::Transfer
Original manifest label: `_gapscan / sub_00f27e60`.
Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f285d0: CHeroSoulsRoundDef::Transfer
Original manifest label: `_gapscan / sub_00f285d0`.
Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f28990: std::vector<CHeroSoulsWaveDef>::_M_fill_insert
Original manifest label: `_global / vector_insert_impl_CThingBuildingDef`.
Identity: proven from own script wrapper/vtable/Transfer literal/typed vector constructor and previous script definition helper mappings. Correct original CHeroSoulsWaveDef element56, CHeroSoulsCreatureDef vector member. Genuine STLport4.6 canonical _M_fill_insert body, false_type nontrivial construction/assignment/destruction.
Layout: full original CIV/Resource/DefPointee/packedDef37 chain and typed scalar40/vector44; actual private default Def ctor/public copy API, proper destructor phases and original base/vector callees. Shared flattened base headers aliased. Unknown unused slots2–17 explicit; real Transfer18/Copy19. Actual standard _Construct helper is declared out of line (matched sibling N248 supplies its body); count1 growth uses equivalent direct placement construction. Fill-insert declares its original constructor and grow helper out of line; their independently supplied sources retain all behavior. No fake fields, identity barriers or replacement flat container.

## 00f34d70: std::vector<CArenaRoundDef>::_M_fill_insert
Original manifest label: `_global / vector_insert_impl_CThingBuildingDef_v2`.
Identity: proven from own script wrapper/vtable/Transfer literal/typed vector constructor and previous script definition helper mappings. Correct original CArenaRoundDef element56, CArenaWaveDef vector member. Genuine STLport4.6 canonical _M_fill_insert body, false_type nontrivial construction/assignment/destruction.
Layout: full original CIV/Resource/DefPointee/packedDef37 chain and typed scalar40/vector44; actual private default Def ctor/public copy API, proper destructor phases and original base/vector callees. Shared flattened base headers aliased. Unknown unused slots2–17 explicit; real Transfer18/Copy19. Actual standard _Construct helper is declared out of line (matched sibling N248 supplies its body); count1 growth uses equivalent direct placement construction. Fill-insert declares its original constructor and grow helper out of line; their independently supplied sources retain all behavior. No fake fields, identity barriers or replacement flat container.

## 00f35570: std::vector<CHeroSoulsRoundDef>::_M_fill_insert
Original manifest label: `_global / Vector_Resize_CThingBuildingDef`.
Identity: proven from own script wrapper/vtable/Transfer literal/typed vector constructor and previous script definition helper mappings. Correct original CHeroSoulsRoundDef element56, CHeroSoulsWaveDef vector member. Genuine STLport4.6 canonical _M_fill_insert body, false_type nontrivial construction/assignment/destruction.
Layout: full original CIV/Resource/DefPointee/packedDef37 chain and typed scalar40/vector44; actual private default Def ctor/public copy API, proper destructor phases and original base/vector callees. Shared flattened base headers aliased. Unknown unused slots2–17 explicit; real Transfer18/Copy19. Actual standard _Construct helper is declared out of line (matched sibling N248 supplies its body); count1 growth uses equivalent direct placement construction. Fill-insert declares its original constructor and grow helper out of line; their independently supplied sources retain all behavior. No fake fields, identity barriers or replacement flat container.

## 00f28ba0: CHeroSoulsWaveDef::CHeroSoulsWaveDef
Original manifest label: `CAbilityTrackDef / CAbilityTrackDef`.
Identity: proven; own virtual Transfer slot18 literals NumWaveCreatures/Creatures, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f34f80: CArenaRoundDef::CArenaRoundDef
Original manifest label: `CAbilityTrackDef / CAbilityTrackDef`.
Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f35780: CHeroSoulsRoundDef::CHeroSoulsRoundDef
Original manifest label: `CAbilityTrackDef / CAbilityTrackDef`.
Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.

## 00f28bd0: std::vector<CHeroSoulsWaveDef>::_M_insert_overflow
Original manifest label: `_global / vector_reallocate_insert_CThingBuildingDef`.
Identity: proven from own script wrapper/vtable/Transfer literal/typed vector constructor and previous script definition helper mappings. Correct original CHeroSoulsWaveDef element56, CHeroSoulsCreatureDef vector member. Genuine STLport4.6 canonical _M_insert_overflow body, false_type nontrivial construction/assignment/destruction.
Layout: full original CIV/Resource/DefPointee/packedDef37 chain and typed scalar40/vector44; actual private default Def ctor/public copy API, proper destructor phases and original base/vector callees. Shared flattened base headers aliased. Unknown unused slots2–17 explicit; real Transfer18/Copy19. Actual standard _Construct helper is declared out of line (matched sibling N248 supplies its body); count1 growth uses equivalent direct placement construction. Fill-insert declares its original constructor and grow helper out of line; their independently supplied sources retain all behavior. No fake fields, identity barriers or replacement flat container.

## 00f34fb0: std::vector<CArenaRoundDef>::_M_insert_overflow
Original manifest label: `_global / Vector_Reallocate_CClanMember`.
Identity: proven from own script wrapper/vtable/Transfer literal/typed vector constructor and previous script definition helper mappings. Correct original CArenaRoundDef element56, CArenaWaveDef vector member. Genuine STLport4.6 canonical _M_insert_overflow body, false_type nontrivial construction/assignment/destruction.
Layout: full original CIV/Resource/DefPointee/packedDef37 chain and typed scalar40/vector44; actual private default Def ctor/public copy API, proper destructor phases and original base/vector callees. Shared flattened base headers aliased. Unknown unused slots2–17 explicit; real Transfer18/Copy19. Actual standard _Construct helper is declared out of line (matched sibling N248 supplies its body); count1 growth uses equivalent direct placement construction. Fill-insert declares its original constructor and grow helper out of line; their independently supplied sources retain all behavior. No fake fields, identity barriers or replacement flat container.

## 00f357b0: std::vector<CHeroSoulsRoundDef>::_M_insert_overflow
Original manifest label: `_global / Vector_Reallocate_CThingBuildingDef`.
Identity: proven from own script wrapper/vtable/Transfer literal/typed vector constructor and previous script definition helper mappings. Correct original CHeroSoulsRoundDef element56, CHeroSoulsWaveDef vector member. Genuine STLport4.6 canonical _M_insert_overflow body, false_type nontrivial construction/assignment/destruction.
Layout: full original CIV/Resource/DefPointee/packedDef37 chain and typed scalar40/vector44; actual private default Def ctor/public copy API, proper destructor phases and original base/vector callees. Shared flattened base headers aliased. Unknown unused slots2–17 explicit; real Transfer18/Copy19. Actual standard _Construct helper is declared out of line (matched sibling N248 supplies its body); count1 growth uses equivalent direct placement construction. Fill-insert declares its original constructor and grow helper out of line; their independently supplied sources retain all behavior. No fake fields, identity barriers or replacement flat container.

## 00f28d30: std::__copy_backward_ptrs<CHeroSoulsWaveDef*,CHeroSoulsWaveDef*>
Original manifest label: `_global / vector_shift_up_CThingBuildingDef`.
Identity: proven; own _M_fill_insert callers pass result+__false_type (ret8), genuine STLport pointer dispatch around inlined random-access __copy_backward; same-template landed CFireHeartPatternDef twin00f34110. Record identity comes from its actual private wrapper/Transfer fields, not propagated catalog CThingBuildingDef labels.
Layout: original DefClass37 full CIV/Resource/DefPointee chain, original scalar40/vector44, true typed STLport vector/base/proxy; actual compiler-generated memberwise operator= invokes original base00431f10 and typed vector assignment. Unknown unused definition slots2–17 explicit, Transfer18/Copy19. Shared flattened headers retained as evidence.

## 00f35110: std::__copy_backward_ptrs<CArenaRoundDef*,CArenaRoundDef*>
Original manifest label: `_global / Vector_CopyBackward_CThingBuildingDef`.
Identity: proven; own _M_fill_insert callers pass result+__false_type (ret8), genuine STLport pointer dispatch around inlined random-access __copy_backward; same-template landed CFireHeartPatternDef twin00f34110. Record identity comes from its actual private wrapper/Transfer fields, not propagated catalog CThingBuildingDef labels.
Layout: original DefClass37 full CIV/Resource/DefPointee chain, original scalar40/vector44, true typed STLport vector/base/proxy; actual compiler-generated memberwise operator= invokes original base00431f10 and typed vector assignment. Unknown unused definition slots2–17 explicit, Transfer18/Copy19. Shared flattened headers retained as evidence.

## 00f35910: std::__copy_backward_ptrs<CHeroSoulsRoundDef*,CHeroSoulsRoundDef*>
Original manifest label: `_global / Vector_CopyAssignBackward_CThingBuildingDef`.
Identity: proven; own _M_fill_insert callers pass result+__false_type (ret8), genuine STLport pointer dispatch around inlined random-access __copy_backward; same-template landed CFireHeartPatternDef twin00f34110. Record identity comes from its actual private wrapper/Transfer fields, not propagated catalog CThingBuildingDef labels.
Layout: original DefClass37 full CIV/Resource/DefPointee chain, original scalar40/vector44, true typed STLport vector/base/proxy; actual compiler-generated memberwise operator= invokes original base00431f10 and typed vector assignment. Unknown unused definition slots2–17 explicit, Transfer18/Copy19. Shared flattened headers retained as evidence.

## 01224140: StaticInitialization::Initialize_CUnprovenGlobal_013CA8A8
Original manifest label: `_gapscan / sub_01224140`.
Identity: proven original CVertexShaderConstantLayoutEmpty from own vtable/base chain and constructor, global013CA8A8 ->original destructor callback. Compiler-generated initializer performs base construction, genuine derived vtable store and atexit registration. Original global spelling unproven, descriptive name retained.
Layout: true original polymorphic hierarchy and typed named fields, PCsize332. No opaque base/vptr. Original base constructors/destructors external; derived implicit destructor is genuine. CInkeyBuffer CCharEvent4 donor wchar_t Char/boolPressed has no provable class key/qualified scope; explicit CUnprovenCharEvent projection preserves only verified fields/size without claiming original V/U identity. For shader layout, typed range12 and genuine STLport CArray/vector12 reconcile donor336->retail332; CCharString non-POD by-value PeekName ABI retained. Available shader header aliased. No handwritten initializer/callback wrapper.

## 01224250: StaticInitialization::Initialize_CUnprovenGlobal_013CB2F8
Original manifest label: `_gapscan / sub_01224250`.
Identity: proven original CInkeyBuffer from own vtable/base chain and constructor, global013CB2F8 ->original destructor callback. Compiler-generated initializer performs base construction, genuine derived vtable store and atexit registration. Original global spelling unproven, descriptive name retained.
Layout: true original polymorphic hierarchy and typed named fields, PCsize416. No opaque base/vptr. Original base constructors/destructors external; derived implicit destructor is genuine. CInkeyBuffer CCharEvent4 donor wchar_t Char/boolPressed has no provable class key/qualified scope; explicit CUnprovenCharEvent projection preserves only verified fields/size without claiming original V/U identity. For shader layout, typed range12 and genuine STLport CArray/vector12 reconcile donor336->retail332; CCharString non-POD by-value PeekName ABI retained. Available shader header aliased. No handwritten initializer/callback wrapper.

## 01224270: StaticInitialization::Initialize_CUnprovenGlobal_013CB498
Original manifest label: `_gapscan / sub_01224270`.
Identity: proven original CScratchBuffer from own vtable/base chain and constructor, global013CB498 ->original destructor callback. Compiler-generated initializer performs base construction, genuine derived vtable store and atexit registration. Original global spelling unproven, descriptive name retained.
Layout: true original polymorphic hierarchy and typed named fields, PCsize20. No opaque base/vptr. Original base constructors/destructors external; derived implicit destructor is genuine. CInkeyBuffer CCharEvent4 donor wchar_t Char/boolPressed has no provable class key/qualified scope; explicit CUnprovenCharEvent projection preserves only verified fields/size without claiming original V/U identity. For shader layout, typed range12 and genuine STLport CArray/vector12 reconcile donor336->retail332; CCharString non-POD by-value PeekName ABI retained. Available shader header aliased. No handwritten initializer/callback wrapper.

## 01224450: StaticInitialization::Initialize_MatrixBufferPool
Original manifest label: `_gapscan / sub_01224450`.
Identity: original CSimpleMemoryPool proven by own ctor00A5C570 and vtable0129A064/RTTI NonCopyable/CBaseClass bases; global013D2DB0 and callback0122AB10 are own producer/consumer addresses. Original global MatrixBufferPool is proven by both PDB initializer/atexit name sets and own N182 typed buffer accessor/caller chain; constructor(unsigned long,unsigned long) KK exact. Compiler emits actual initializer and atexit callback.
Layout: actual CSimpleMemoryPool28, nested original class CArea12 fields Size/NextFreeArea/Status, CMemoryBlock pointer, Capacity/GrowthGranularity; protected NonCopyable ctor/dtor IAE/MAE from PDB namesets. Source headers retained. Donor Status enum display is EAreaStatus but its original qualified scope/enumerator names are unprovable; explicit CUnprovenAreaStatus projection preserves enum kind/4byte storage without asserting a false name. Only original ctor and real private ClearMemoryBlocks/dtor operations used; no fake vptr or callback function.

## 01224470: StaticInitialization::Initialize_VectorBufferPool
Original manifest label: `_gapscan / sub_01224470`.
Identity: original CSimpleMemoryPool proven by own ctor00A5C570 and vtable0129A064/RTTI NonCopyable/CBaseClass bases; global013D2DCC and callback0122AB30 are own producer/consumer addresses. Original global VectorBufferPool is proven by both PDB initializer/atexit name sets and own N182 typed buffer accessor/caller chain; constructor(unsigned long,unsigned long) KK exact. Compiler emits actual initializer and atexit callback.
Layout: actual CSimpleMemoryPool28, nested original class CArea12 fields Size/NextFreeArea/Status, CMemoryBlock pointer, Capacity/GrowthGranularity; protected NonCopyable ctor/dtor IAE/MAE from PDB namesets. Source headers retained. Donor Status enum display is EAreaStatus but its original qualified scope/enumerator names are unprovable; explicit CUnprovenAreaStatus projection preserves enum kind/4byte storage without asserting a false name. Only original ctor and real private ClearMemoryBlocks/dtor operations used; no fake vptr or callback function.

## 01224490: StaticInitialization::Initialize_GroupVectorBufferPool
Original manifest label: `_gapscan / sub_01224490`.
Identity: original CSimpleMemoryPool proven by own ctor00A5C570 and vtable0129A064/RTTI NonCopyable/CBaseClass bases; global013D2DE8 and callback0122AB50 are own producer/consumer addresses. Original global GroupVectorBufferPool is proven by both PDB initializer/atexit name sets and own N182 typed buffer accessor/caller chain; constructor(unsigned long,unsigned long) KK exact. Compiler emits actual initializer and atexit callback.
Layout: actual CSimpleMemoryPool28, nested original class CArea12 fields Size/NextFreeArea/Status, CMemoryBlock pointer, Capacity/GrowthGranularity; protected NonCopyable ctor/dtor IAE/MAE from PDB namesets. Source headers retained. Donor Status enum display is EAreaStatus but its original qualified scope/enumerator names are unprovable; explicit CUnprovenAreaStatus projection preserves enum kind/4byte storage without asserting a false name. Only original ctor and real private ClearMemoryBlocks/dtor operations used; no fake vptr or callback function.

## 01224600: StaticInitialization::Initialize_01436e04
Original manifest label: `_gapscan / sub_01224600`.
Identity: descriptive global and initializer names, original identity unproven. Own body computes intrinsic cosine of double012a1140 then multiplies float012a1138 and stores float01436e04. Only middle result has an observed reader00b61de1; other two are unreferenced outside initialization.
Layout: scalar globals only; exact double angle0.4712389409542084 and float scale13000.0 observed in own read-only data. No owner class or shared header.

## 01224830: StaticInitialization::Initialize_0143782c
Original manifest label: `_gapscan / sub_01224830`.
Identity: descriptive global and initializer names, original identity unproven. Own body computes intrinsic cosine of double012a1140 then multiplies float012a1138 and stores float0143782c. Only middle result has an observed reader00b61de1; other two are unreferenced outside initialization.
Layout: scalar globals only; exact double angle0.4712389409542084 and float scale13000.0 observed in own read-only data. No owner class or shared header.

## 012248b0: StaticInitialization::Initialize_014378b4
Original manifest label: `_gapscan / sub_012248b0`.
Identity: descriptive global and initializer names, original identity unproven. Own body computes intrinsic cosine of double012a1140 then multiplies float012a1138 and stores float014378b4. Only middle result has an observed reader00b61de1; other two are unreferenced outside initialization.
Layout: scalar globals only; exact double angle0.4712389409542084 and float scale13000.0 observed in own read-only data. No owner class or shared header.

## 012246e0: StaticInitialization::Initialize_LOCAL_HEAP_CEngineLightingManager__LightElementLink
Original manifest label: `_gapscan / sub_012246e0`.
Identity: probable. Own RTTI CDynamicObjectHeap<12,16> and global01436ed0; own lighting allocation consumers corroborate element type/size. FableWin nameset contains dynamic initializer for LOCAL_HEAP_CEngineLightingManager__LightElementLink. CDynamicObjectHeapBase ctor00f36c60, base20, FreeList20, StartBlock24, sixteen entries stride12, NextEntry216, grow byte220. Base RefCountedRelease00f36ca0 dispatches slot1; base vtable slot1 is purecall. No engine header for these types.
Layout: original heap template, typed base and embedded block. Nested record keys follow the landed same-template constructor; template APIs not used here omitted.

## 01224730: StaticInitialization::Initialize_LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight
Original manifest label: `_gapscan / sub_01224730`.
Identity: probable. Own RTTI CDynamicObjectHeap<72,16> and global01436fb0; own lighting allocation consumers corroborate element type/size. FableWin nameset contains dynamic initializer for LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight. CDynamicObjectHeapBase ctor00f36c60, base20, FreeList20, StartBlock24, sixteen entries stride72, NextEntry1176, grow byte1180. Base RefCountedRelease00f36ca0 dispatches slot1; base vtable slot1 is purecall. No engine header for these types.
Layout: original heap template, typed base and embedded block. Nested record keys follow the landed same-template constructor; template APIs not used here omitted.

## 01224780: StaticInitialization::Initialize_LOCAL_HEAP_CEngineLightingManager__ManagerShadowedSpotLight
Original manifest label: `_gapscan / sub_01224780`.
Identity: probable. Own RTTI CDynamicObjectHeap<56,16> and global01437450; own lighting allocation consumers corroborate element type/size. FableWin nameset contains dynamic initializer for LOCAL_HEAP_CEngineLightingManager__ManagerShadowedSpotLight. CDynamicObjectHeapBase ctor00f36c60, base20, FreeList20, StartBlock24, sixteen entries stride56, NextEntry920, grow byte924. Base RefCountedRelease00f36ca0 dispatches slot1; base vtable slot1 is purecall. No engine header for these types.
Layout: original heap template, typed base and embedded block. Nested record keys follow the landed same-template constructor; template APIs not used here omitted.

## 01229030: StaticDestruction::Destroy_s_CachedThingDef
Original manifest label: ``anonymous_namespace' / `anonymous_namespace'::`dynamic_atexit_destructor_for_'DEFAULT_CAMERA_MODE_DEF''`.
Identity: probable. Actual atexit registration and body identify static CDefPointer cleanup for global013b8c14. Original CDefPointer stores CDefPointeeBase*, count is unsigned long, Release() const returns long and dispatches Delete() const at slot1. CacheDef005d94b0 gets CThingBaseDef via0042af3c, then CInventoryItemDef via005bade1; stores first/second result to013b8c14/013b8c18. Existing s_Cached names retained as descriptive member spellings, not PDB-proven. CThingDef is a prior false label; original CThingBaseDef is corroborated by GetDef template/name sets. Static-only owner projection retains real CTCBase ancestry; instance suffix and unused virtual declarations omitted, no owner instantiated.
Layout: actual Resource28/CIV8 hierarchy, Object pointer and original unsigned reference count. Pointee type is forward declared because no derived field is used. ENullDef enumerator spelling descriptive; no engine this receiver.

## 01229060: StaticDestruction::Destroy_s_CachedItemDef
Original manifest label: ``anonymous_namespace' / `anonymous_namespace'::`dynamic_atexit_destructor_for_'DEFAULT_CAMERA_MODE_DEF''`.
Identity: probable. Actual atexit registration and body identify static CDefPointer cleanup for global013b8c18. Original CDefPointer stores CDefPointeeBase*, count is unsigned long, Release() const returns long and dispatches Delete() const at slot1. CacheDef005d94b0 gets CThingBaseDef via0042af3c, then CInventoryItemDef via005bade1; stores first/second result to013b8c14/013b8c18. Existing s_Cached names retained as descriptive member spellings, not PDB-proven. CThingDef is a prior false label; original CThingBaseDef is corroborated by GetDef template/name sets. Static-only owner projection retains real CTCBase ancestry; instance suffix and unused virtual declarations omitted, no owner instantiated.
Layout: actual Resource28/CIV8 hierarchy, Object pointer and original unsigned reference count. Pointee type is forward declared because no derived field is used. ENullDef enumerator spelling descriptive; no engine this receiver.

## 01229340: StaticDestruction::Destroy_DEFAULT_CAMERA_MODE_DEF
Original manifest label: ``anonymous_namespace' / `anonymous_namespace'::`dynamic_atexit_destructor_for_'DEFAULT_CAMERA_MODE_DEF''`.
Identity: proven. Actual atexit registration and body identify static CDefPointer cleanup for global013ba8d8. Original CDefPointer stores CDefPointeeBase*, count is unsigned long, Release() const returns long and dispatches Delete() const at slot1. Both PDB sets name anonymous-namespace DEFAULT_CAMERA_MODE_DEF initializer/destructor; camera manager consumers00699970/0069e460 corroborate CCameraModeDef. Global declared in anonymous namespace, no invented external binding.
Layout: actual Resource28/CIV8 hierarchy, Object pointer and original unsigned reference count. Pointee type is forward declared because no derived field is used. ENullDef enumerator spelling descriptive; no engine this receiver.

## 0041a780: CUnprovenThreeByteState_0041a780::CUnprovenThreeByteState_0041a780
Original manifest label: `_gapscan / sub_0041a780`.
Identity: descriptive. No true callers found: raw xref005817ac is the rel32 displacement of an unrelated call, not an address-taking reference. Three byte writes0/1/2 and constructor return-this proven; original field/class identity unknown.
Layout: own exact writes; no vptr or base stores observed. SDK scalar widths portable in host tests; unused union alternatives omitted while preserving original reserved arrays and extents. No engine owner header exists for this unproven identity.

## 00c71420: CUnprovenMixerVolumeControl_00c71420::CUnprovenMixerVolumeControl_00c71420
Original manifest label: `CNavigationLayer / CNavigationLayer`.
Identity: descriptive. Actual caller00c55d00 initializes contained object+110. Helper00c71430 uses mixerOpen out at0, MIXERCONTROLA output at+4 with SDK cbmxctrl148, saved unsigned volume+152 from mixerGetControlDetailsA; close00c715d0 tests Control.szName[0] at+40 before restoring volume. This is not navigation. Original owner/field names descriptive, SDK types/layout proven.
Layout: own exact writes; no vptr or base stores observed. SDK scalar widths portable in host tests; unused union alternatives omitted while preserving original reserved arrays and extents. No engine owner header exists for this unproven identity.

## 0042d4f7: NUISystem::CManager::SetKeyPressed
Original manifest label: `NUISystem::CManager / NUISystem::CManager::vfunc_12`.
Identity: Own CManager vtable01230134 slot12 and original PDB SetKeyPressed(wchar_t), member KeyPressed+192. RTTI proves CObservable direct base, original lists4 and maps12 explain donor drift.
Layout: original class keys, actual bases and typed STLport containers/counting handles. CManager header renamed to restore actual inheritance/types; enum value spellings unspecified. Bank owner declaration is a prefix through UsingRetailBanks only. Unused virtual slots explicitly descriptive.

## 009a76d0: CBankFileManager::SetUsingRetailBanks
Original manifest label: `CTCPhysicsNavigator / EnableNavigator`.
Identity: Own sole direct caller00402875 uses global013ca79c, actual CBankFileManager identity/RTTI and PDB SetUsingRetailBanks(bool), UsingRetailBanks+20 after map12/list4. Prior physics-navigator label rejected.
Layout: original class keys, actual bases and typed STLport containers/counting handles. CManager header renamed to restore actual inheritance/types; enum value spellings unspecified. Bank owner declaration is a prefix through UsingRetailBanks only. Unused virtual slots explicitly descriptive.

## 00434540: CEngineVideoOptionsDef::Alloc
Original manifest label: `_gapscan / sub_00434540`.
Identity: proven. Own factory allocates120 bytes, calls CResource ctor009fbec0, sets packed parent fields and vtable012311f4. Own RTTI and PDB original static __fastcall Alloc()->CDefClassBase*, original full derived fields, parent packed57 with first own field60. No target generated header exists. Unused slots2..17 are explicitly descriptive; original full virtual ownership not claimed.
Layout: genuine CIV/Resource/Def/Parent hierarchy and typed STLport base/proxy, vector12, map16. CResource28, CDefClassBase37, Parent57; original fields follow. Default base ctor does not initialize GlobalIndex or definition-specific fields.

## 00f29fa0: CRegionScriptDef::Alloc
Original manifest label: `_gapscan / sub_00f29fa0`.
Identity: proven. Own factory allocates76 bytes, calls CResource ctor009fbec0, sets packed parent fields and vtable012fb630. Vtable shared with named ctor00f29c10 and registration00f2a1a8; PDB original static __fastcall Alloc()->CDefClassBase*, original full derived fields, parent packed57 with first own field60. No target generated header exists. Unused slots2..17 are explicitly descriptive; original full virtual ownership not claimed.
Layout: genuine CIV/Resource/Def/Parent hierarchy and typed STLport base/proxy, vector12, map16. CResource28, CDefClassBase37, Parent57; original fields follow. Default base ctor does not initialize GlobalIndex or definition-specific fields.

## 0043d100: std::allocator::deallocate
Original manifest label: `_gapscan / sub_0043d100`.
Identity: descriptive. No true callers: raw absolute cross-reference falls inside an unrelated rel32 call displacement. Body ignores receiver/size, conditionally calls CRT free00bfea14, pops two stack parameters. STLport allocator::deallocate is a shape-based hypothesis; actual element and owning instantiation cannot be recovered. Forward-declared descriptive element avoids claiming a record size or identity. ABI evidence cannot distinguish equivalent stdcall helper.
Layout: empty original allocator template; pointee is incomplete and never accessed. No engine receiver header.

## 00754d00: std::allocator::deallocate
Original manifest label: `_gapscan / sub_00754d00`.
Identity: descriptive. No true callers: raw absolute cross-reference falls inside an unrelated rel32 call displacement. Body ignores receiver/size, conditionally calls CRT free00bfea14, pops two stack parameters. STLport allocator::deallocate is a shape-based hypothesis; actual element and owning instantiation cannot be recovered. Forward-declared descriptive element avoids claiming a record size or identity. ABI evidence cannot distinguish equivalent stdcall helper.
Layout: empty original allocator template; pointee is incomplete and never accessed. No engine receiver header.

## 00441800: NPlayerGui::CDrawHealth::~CDrawHealth
Original manifest label: `_global / Catch@0047e8cc`.
Identity: proven. Own scalar-dtor00442960 and counted deleter00443500 -> Reset00444050 -> CPlayerGui constructor0043bdc2, matching CDrawHealth ctor0064e1e4 and own typed vectors28/40. PDB original nonvirtual compiler-generated destructor; own class intentionally declares no destructor, so no invented vptr store. Genuine STLport vector/base/proxy, original polymorphic CEnginePrimitiveHandle:CBaseClass and CIntelligentPointer:CBaseIntelligentPointer element types. Original named fields all retained; unused constructors/APIs omitted.
Layout: base Draw40, Health108, Stat72 in retail; two element types8 each. No matching target engine header exists.

## 00442280: NPlayerGui::CStatBase::~CStatBase
Original manifest label: `_global / Catch@0047e8cc`.
Identity: proven. Own derived stat-bar destructor/deleter chains00443190/00444d90, CPlayerGui creation0065431d and original CStatBase ctor0064cc11 establish base identity, vectors48/60. Own base vtable0125a4e4 contains three pure slots; their unused identities remain descriptive. PDB original nonvirtual compiler-generated destructor; own class intentionally declares no destructor, so no invented vptr store. Genuine STLport vector/base/proxy, original polymorphic CEnginePrimitiveHandle:CBaseClass and CIntelligentPointer:CBaseIntelligentPointer element types. Original named fields all retained; unused constructors/APIs omitted.
Layout: base Draw40, Health108, Stat72 in retail; two element types8 each. No matching target engine header exists.

## 00442d50: NPlayerGui::CDrawDamage::ScalarDeletingDestructor
Original manifest label: `_global / Catch@0047e8cc`.
Identity: probable. Own scalar-deleting cleanup first destroys member40 then inherited handle vector28, conditionally scalar-delete00bfe9bc and returns receiver. Out-of-line list base destructor43fcf0 frees trivial SDamageValue nodes; own plain dtor441c70 and counted deleter443840 connect matching CDrawDamage creator via Reset4446e0. Original nonvirtual implicit destructor; no fake vptr store, standard compiler-generated deleting form requested through delete expression.
Layout: inherited Draw40 and real list4 or CWideString4 member. No target header. Source includes only typed original declarations and compiler destruction.

## 004430f0: NPlayerGui::CDrawNPCName::ScalarDeletingDestructor
Original manifest label: `_global / Catch@0047e8cc`.
Identity: probable. Own scalar-deleting cleanup first destroys member40 then inherited handle vector28, conditionally scalar-delete00bfe9bc and returns receiver. Only draw owner with original CWideString at donor44/retail40 and trivial trailingcounter is CDrawNPCName; cached GUI ownership confirms the sibling destruction family. Original wide dtor99b510. Original nonvirtual implicit destructor; no fake vptr store, standard compiler-generated deleting form requested through delete expression.
Layout: inherited Draw40 and real list4 or CWideString4 member. No target header. Source includes only typed original declarations and compiler destruction.

## 00444d90: CCountedPointer<NPlayerGui::CStatBar>::DeleteData
Original manifest label: `_gapscan / sub_00444d90`.
Identity: proven. Own caller Reset00445190/00445200 and CPlayerGui constructors0065431d/00654392 prove original counted owner; original PDB DeleteData static fastcall void(void*) symbol. Original StatBase72, StatBar104 with list ChangeTexts100, Crime112. List clear004407b0, base dtor00442280 and scalar operator delete00bfe9bc. No target owner header. Genuine STLport list/base/proxy and original CChangeText nested class. Original owner destructors remain implicit.
Layout: original named fields with 12-byte vectors and four-byte list; the base virtual identities are established by CStatBar overrides.

## 00444dd0: CCountedPointer<NPlayerGui::CCrimeStatBar>::DeleteData
Original manifest label: `_gapscan / sub_00444dd0`.
Identity: proven. Own caller Reset00445190/00445200 and CPlayerGui constructors0065431d/00654392 prove original counted owner; original PDB DeleteData static fastcall void(void*) symbol. Original StatBase72, StatBar104 with list ChangeTexts100, Crime112. List clear004407b0, base dtor00442280 and scalar operator delete00bfe9bc. No target owner header. Genuine STLport list/base/proxy and original CChangeText nested class. Original owner destructors remain implicit.
Layout: original named fields with 12-byte vectors and four-byte list; the base virtual identities are established by CStatBar overrides.

## 0044ebe4: std::allocator::deallocate
Original manifest label: `_gapscan / sub_0044ebe4`.
Identity: descriptive. No true callers: raw absolute cross-reference falls inside an unrelated rel32 call displacement. Body ignores receiver/size, conditionally calls CRT free00bfea14, pops two stack parameters. STLport allocator::deallocate is a shape-based hypothesis; actual element and owning instantiation cannot be recovered. Forward-declared descriptive element avoids claiming a record size or identity. ABI evidence cannot distinguish equivalent stdcall helper.
Layout: empty original allocator template; pointee is incomplete and never accessed. No engine receiver header.

## 004d6ed0: std::allocator::deallocate
Original manifest label: `_gapscan / sub_004d6ed0`.
Identity: descriptive. No true callers: raw absolute cross-reference falls inside an unrelated rel32 call displacement. Body ignores receiver/size, conditionally calls CRT free00bfea14, pops two stack parameters. STLport allocator::deallocate is a shape-based hypothesis; actual element and owning instantiation cannot be recovered. Forward-declared descriptive element avoids claiming a record size or identity. ABI evidence cannot distinguish equivalent stdcall helper.
Layout: empty original allocator template; pointee is incomplete and never accessed. No engine receiver header.

## 0047d0d5: _global::PCCancelQuitToFrontEnd
Original manifest label: `_gapscan / sub_0047d0d5`.
Identity: proven. Own callback pointers0047d340/0047de50 installed by PCSetQuitToFrontEnd0047d296/PCSetToLoad; PDB original free fastcall PCCancelQuitToFrontEnd. Globals013b87a8/013b87a4. Original manager getter0041e5f2 ->CManager:CObservable; actual Observable vtable01230044 slot10 RemoveConcurrentExclusiveObserver andslot2 AddObserver. Component RTTI proves CUIInterface0/CObserver4/CActionParam24; FindChild slot3, observer ObserveEvent slot3 with original EEvent enum. Globals descriptive names, original spellings unproven.
Layout: genuine multiple inheritance supplies null-preserving observer adjustment. Original Observer20/Observable16 with STLport set12/list4; unused Component and Manager suffix fields/APIs omitted. No engine this receiver is defined here.

## 0056e866: _global::CancelUndoAbility
Original manifest label: `_gapscan / sub_0056e866`.
Identity: proven. Own callback pointer00576d0c installed by CTCInventoryExperience::UndoBuyAbility00576c4f, SetNoCallBack slot143. PDB original free fastcall CancelUndoAbility. Globals013b8b04/013b8b00. Original manager getter0041e5f2 ->CManager:CObservable; actual Observable vtable01230044 slot10 RemoveConcurrentExclusiveObserver andslot2 AddObserver. Component RTTI proves CUIInterface0/CObserver4/CActionParam24; FindChild slot3, observer ObserveEvent slot3 with original EEvent enum. Globals descriptive names, original spellings unproven.
Layout: genuine multiple inheritance supplies null-preserving observer adjustment. Original Observer20/Observable16 with STLport set12/list4; unused Component and Manager suffix fields/APIs omitted. No engine this receiver is defined here.

## 005129c0: CReplaceSerialise::GetMaxSize
Original manifest label: `_gapscan / sub_005129c0`.
Identity: proven. Own tail caller005144c0 and exact version3/2 plus record-size constants[13, 17, 21] identify CReplaceSerialise<COldFileFormatLevel3,COldFileFormatLevel2>::GetMaxSize(long,long), original static fastcall signature from both PDB sets. Actual packed original record fields and five-byte serialisation header. VERSION enum names are explicitly compile-time model markers, not recovered original enumerator names. Macro repeats the older GetMaxSize expression, reproducing the redundant tail comparisons.
Layout: packed records, original class keys/templates and named fields; FileLong/FileULong preserve 32-bit disk widths in the host test. No engine header for these record types.

## 00824ce0: CReplaceSerialise::GetMaxSize
Original manifest label: `_gapscan / sub_00824ce0`.
Identity: proven. Own tail caller008256a0 and exact version3/2 plus record-size constants[9, 10, 15] identify CReplaceSerialise<COldFileFormatHeightMapCell3,COldFileFormatHeightMapCell2>::GetMaxSize(long,long), original static fastcall signature from both PDB sets. Actual packed original record fields and five-byte serialisation header. VERSION enum names are explicitly compile-time model markers, not recovered original enumerator names. Macro repeats the older GetMaxSize expression, reproducing the redundant tail comparisons.
Layout: packed records, original class keys/templates and named fields; FileLong/FileULong preserve 32-bit disk widths in the host test. No engine header for these record types.

## 00565030: std::allocator::allocate
Original manifest label: `_gapscan / sub_00565030`.
Identity: descriptive. Own count zero guard, shift3, CRT malloc00bfea0e, return two stack args; receiver unused. No true callers: first raw xref is unrelated rel32; second matches ASCII Pos in data strings. STLport allocator::allocate count/hint is a shape-based hypothesis; exact element identity unknown. Only element extent8 is recovered, no invented semantic fields.
Layout: original empty allocator template with descriptive element whose only modeled property is the proven extent. No engine header.

## 00736f50: std::allocator::allocate
Original manifest label: `_gapscan / sub_00736f50`.
Identity: descriptive. Own count zero guard, shift2, CRT malloc00bfea0e, return two stack args; receiver unused. No true callers: first raw xref is unrelated rel32; second matches ASCII Pos in data strings. STLport allocator::allocate count/hint is a shape-based hypothesis; exact element identity unknown. Only element extent4 is recovered, no invented semantic fields.
Layout: original empty allocator template with descriptive element whose only modeled property is the proven extent. No engine header.

## 00624e40: std::vector<CQuestCardItem>::_M_fill_insert
Original manifest label: `std / std::vector_insert_range_pairs`.
Identity: proven. Own resize006253d0 ->CTCInventoryQuests::OnCreate0061a6a0 atcall0061a731 establishes original QuestCardItemList and CQuestCardItem classV, ItemIndex/ItemText. Eight-byte element with original CWideString copy ctor/dtor; original SGI/STLport fill-insert branch semantics and fastcall free helpers. All declared helper targets retain original template names and typed signatures; unused outer owner instance layout omitted.
Layout: original vector12 with _Vector_base and _STLP_alloc_proxy; original element8. Helpers have visible real loop bodies, unnamed tag parameters, and no writes through input references.

## 00bf95d0: std::vector<CEngineTexturePalette::CPaletteEntry>::_M_fill_insert
Original manifest label: `_global / Std_Vector_ResizeWithFillAndCopy`.
Identity: proven. Own grow00bf94d0 ->CEngineTexturePalette constructor00bf90a0/GetTexturePaletteIndex00bf9150 establishes original nested struct CPaletteEntry U, Index/Name. Eight-byte element with original CCharString copy ctor/dtor; original SGI/STLport fill-insert branch semantics and fastcall free helpers. All declared helper targets retain original template names and typed signatures; unused outer owner instance layout omitted.
Layout: original vector12 with _Vector_base and _STLP_alloc_proxy; original element8. Helpers have visible real loop bodies, unnamed tag parameters, and no writes through input references.

## 00633690: CPlayer::InitModeRebootGame
Original manifest label: `_gapscan / sub_00633690`.
Identity: proven. Actual CPlayer mode-table callback initializer0121b2f7/0121b697, original PDB private member InitModeRebootGame(EPlayerMode,CProcessedInput*), memberInputProcessRebootGame at376, PlayerInterface12. Own dispatch calls original CAInputProcess/CInputProcessGameBase::AddToInterface(CInputProcessManager&) bool virtualslot1 and ignores result. Genuine boost::scoped_ptr, nested CPlayer::CThingRespawnInfo, CInitBaseClass ancestry; original fields through476 retained, unused suffix omitted.
Layout: shared flattened header retained as evidence; local typed base/field declarations preserve actual owner layout. Unused API bodies and enum values omitted; descriptive enum fixture value is not an original enumerator claim.

## 00633b70: CPlayer::InitModeCreditsUI
Original manifest label: `_gapscan / sub_00633b70`.
Identity: proven. Actual CPlayer mode-table callback initializer0121b2f7/0121b697, original PDB private member InitModeCreditsUI(EPlayerMode,CProcessedInput*), memberInputProcessCreditsUI at476, PlayerInterface12. Own dispatch calls original CAInputProcess/CInputProcessGameBase::AddToInterface(CInputProcessManager&) bool virtualslot1 and ignores result. Genuine boost::scoped_ptr, nested CPlayer::CThingRespawnInfo, CInitBaseClass ancestry; original fields through476 retained, unused suffix omitted.
Layout: shared flattened header retained as evidence; local typed base/field declarations preserve actual owner layout. Unused API bodies and enum values omitted; descriptive enum fixture value is not an original enumerator claim.

## 006349b0: CPlayer::InitModeFreezeControls
Original manifest label: `_gapscan / sub_006349b0`.
Identity: proven. Original private CPlayer callback signature and typed member offsets328/480, PlayerInterface12, trackedPlayerCharacter52. Own code calls original CBaseIntelligentPointer::GetPItem()const00a01b50 three times, tests original Dead bit at145, component167 atUsingComponents52, nonconst LowerBound0040f020, CTCSpecialAbilities::CancelCurrentMutexAbility0072b420. True CClass17/CBaseObject8/CBaseClass4, CThing fields throughDead145, original creature ancestry with unused suffix explicitly omitted.
Layout: original typed player and thing prefixes, true STLport bitset/map/vector/list bases. Available owner/base headers retained as layout evidence; unknown enum fixture names are descriptive.

## 00636400: CPlayer::InitModeBetting
Original manifest label: `_gapscan / sub_00636400`.
Identity: proven. Original private CPlayer callback signature and typed member offsets328/480, PlayerInterface12, trackedPlayerCharacter52. Own code calls original CBaseIntelligentPointer::GetPItem()const00a01b50 three times, tests original Dead bit at145, component167 atUsingComponents52, nonconst LowerBound0040f020, CTCSpecialAbilities::CancelCurrentMutexAbility0072b420. True CClass17/CBaseObject8/CBaseClass4, CThing fields throughDead145, original creature ancestry with unused suffix explicitly omitted.
Layout: original typed player and thing prefixes, true STLport bitset/map/vector/list bases. Available owner/base headers retained as layout evidence; unknown enum fixture names are descriptive.

## 00723750: CTCPhysicsStandard::GetFacingAngleXZ
Original manifest label: `CTCPhysicsStandard / CTCPhysicsStandard::vfunc_65`.
Identity: proven. Own RTTI vtable slot65 and PDB exact GetFacingAngleXZ()const ->float. Own add ecx,80 then00a22ca0 matches CRightHandedSet::GetAngleXZ()const from original nameset and callee XZ angle implementation. Original CTCBase/PhysicsBase ancestry; receiver offsets80 reconcile donor fields with STLport vectors12. Shared header is flattened and aliased; actual model preserves typed bases and original named members through RHSet; unused suffix omitted without a full-size claim. Unused virtual slots are descriptive projections.
Layout: PhysicsBase64, CTCBase12, C3DVector12 packed1, CRightHandedSet24; real CDefPointer base-pointer storage and STLport hierarchy. Original constructor declarations retained; test-only fixture default constructors are added only in the standalone test.

## 007599b0: CTCPhysicsRigidBody::GetFacingAngleXZ
Original manifest label: `CTCPhysicsRigidBody / CTCPhysicsRigidBody::vfunc_65`.
Identity: proven. Own RTTI vtable slot65 and PDB exact GetFacingAngleXZ()const ->float. Own add ecx,104 then00a22ca0 matches CRightHandedSet::GetAngleXZ()const from original nameset and callee XZ angle implementation. Original CTCBase/PhysicsBase ancestry; receiver offsets104 reconcile donor fields with STLport vectors12. Shared header is flattened and aliased; actual model preserves typed bases and original named members through RHSet; unused suffix omitted without a full-size claim. Unused virtual slots are descriptive projections.
Layout: PhysicsBase64, CTCBase12, C3DVector12 packed1, CRightHandedSet24; real CDefPointer base-pointer storage and STLport hierarchy. Original constructor declarations retained; test-only fixture default constructors are added only in the standalone test.

## 00743530: NCreatureMode::CModeGuildmasterPowerUpHeal::~CModeGuildmasterPowerUpHeal
Original manifest label: `_global / Catch@0047e8cc`.
Identity: proven. Own scalar destructor at00743510 and RTTI012685bc prove exact NCreatureMode owner ->CModeStandBase->CTCCreatureModeManager::CModeBase. Original named fields: Filter104,Timer116,LightningBeamParticleEmitters120,Guards132. Original vectors use polymorphic CIntelligentPointer8, not primitive handles. Actual filter base vtable0122ffcc has two pure slots followed by destructor slot2. Original base destructor0073d860.
Layout: typed original ModeBase28 and StandBase104; owner144. Shared flattened base retained as evidence, reconstructed hierarchy and original members used below. Original owner/filter destructors are implicit. Unused mode virtual slots after dtor omitted, no false method names.

## 007436d0: NCreatureMode::CModeGuildmasterPowerUpSpeed::~CModeGuildmasterPowerUpSpeed
Original manifest label: `_global / Catch@0047e8cc`.
Identity: proven. Own scalar destructor at007436b0 and RTTI01268624 prove exact NCreatureMode owner ->CModeStandBase->CTCCreatureModeManager::CModeBase. Original named fields: Filter104,Timer116,LightningBeamParticleEmitters120,Guards132. Original vectors use polymorphic CIntelligentPointer8, not primitive handles. Actual filter base vtable0122ffcc has two pure slots followed by destructor slot2. Original base destructor0073d860.
Layout: typed original ModeBase28 and StandBase104; owner144. Shared flattened base retained as evidence, reconstructed hierarchy and original members used below. Original owner/filter destructors are implicit. Unused mode virtual slots after dtor omitted, no false method names.

## 0075c0e0: CameraState::ResetCameraPointEnable
Original manifest label: `_global / $E2`.
Identity: descriptive. Only true caller004ee281 resets during level transition; read0075d8f0 gates camera-point activation. Existing descriptive global spelling retained. Original function name/scope unproven; descriptive namespace/free-function spelling. Propagated $E2 label is not an atexit destructor: own callers use it at runtime.
Layout: one global boolean, no receiver or engine header.

## 00878a50: CameraState::ResetCameraInputFlag
Original manifest label: `_global / $E2`.
Identity: descriptive. Only true caller0069b515 resets during camera manager setup; reader008799c0 scales one input component when the flag is set. Original flag identity unproven. Original function name/scope unproven; descriptive namespace/free-function spelling. Propagated $E2 label is not an atexit destructor: own callers use it at runtime.
Layout: one global boolean, no receiver or engine header.

## 007ad160: std::map<CCharString,bool>::insert
Original manifest label: `_gapscan / sub_007ad160`.
Identity: probable. Own callee007ad060 is original _Rb_tree<CCharString,pair<const CCharString,bool>,...>::insert_unique, confirmed named script map consumers and corresponding008aca90 typed instantiation. Original input8-byte pair and output8-byte pair<iterator,bool>. Wrapper itself is unreferenced (raw absolute text hits are instruction operands), so map forwarding-layer identification is probable, not a recovered standalone symbol claim. Genuine STLport map/tree/base/proxy/iterator/traits and original key/value types; no manufactured wrapper helper.
Layout: standard12-byte tree and one-pointer iterator; user iterator constructors preserve native hidden return ABI. No target engine header.

## 00a80500: std::map<long,CANavQuadTreeNode*>::find
Original manifest label: `_gapscan / sub_00a80500`.
Identity: probable. Own callee00a7f880 is tree find with signedlong key; actual CNavQuadTreeNode::ConvertIndexesToPointers00a77ca0 uses map<long,CANavQuadTreeNode*> and original PDB signature. Output4-byte iterator class by hidden pointer. Wrapper itself is unreferenced (raw absolute text hits are instruction operands), so map forwarding-layer identification is probable, not a recovered standalone symbol claim. Genuine STLport map/tree/base/proxy/iterator/traits and original key/value types; no manufactured wrapper helper.
Layout: standard12-byte tree and one-pointer iterator; user iterator constructors preserve native hidden return ABI. No target engine header.

## 007cd930: CTCBurntEffect::FinalCleanup
Original manifest label: `_gapscan / sub_007cd930`.
Identity: proven. Own OnKill/OnRemove wrappers007cd980/990 or007cf090/0a0 call target; matching original private FinalCleanup names and RTTI CTCBase ancestry. Retail ParentThing4 graphic appearance100, Dead145 bit0, TintList44 with eight-byte CIntelligentPointer entries. Real GetPItem const00a01b50 and NGraphicAppearance::CTCGraphicAppearance::RemoveTintColour004bfae0 identified by original API and caller semantics; old global class binding is a naming discrepancy.
Layout: original CTCBase12; typed CThing prefix, original CClass17 base, intelligent pointer8 and vector12. Effects/TintList offsets follow original fields; no target engine header.

## 007cf040: CTCCreatureTintEffect::FinalCleanup
Original manifest label: `_gapscan / sub_007cf040`.
Identity: proven. Own OnKill/OnRemove wrappers007cd980/990 or007cf090/0a0 call target; matching original private FinalCleanup names and RTTI CTCBase ancestry. Retail ParentThing4 graphic appearance100, Dead145 bit0, TintList36 with eight-byte CIntelligentPointer entries. Real GetPItem const00a01b50 and NGraphicAppearance::CTCGraphicAppearance::RemoveTintColour004bfae0 identified by original API and caller semantics; old global class binding is a naming discrepancy.
Layout: original CTCBase12; typed CThing prefix, original CClass17 base, intelligent pointer8 and vector12. Effects/TintList offsets follow original fields; no target engine header.

## 0084d790: NCameraModes::CBase::GetDefaultFOV
Original manifest label: `NCameraModes::CBase / NCameraModes::CBase::vfunc_18`.
Identity: proven. Own RTTI vtable slot18 and donor name set independently identify original const virtual float GetDefaultFOV. Retail reads PCameraModeDef at68, FOV at80 then multiplies by 1/360 literal at01238e00. Original typed CBase and CPhysicsState fields reconcile through four-byte STLport lists; CCameraModeDef fields follow packed Parent57 with first field60. Target headers absent (CCameraModeDef exists only in quarantine). Unused virtual slots are descriptive projections; CZTargetNew unused derived suffix is omitted without a size claim.
Layout: CBase164, CPhysicsState48, pointer wrapper4, vector12, list4, CCameraModeDef216 with FOV80. Original complete definition and owner base hierarchy; no anonymous offset substitute.

## 00872480: NCameraModes::CZTargetNew::GetDefaultFOV
Original manifest label: `NCameraModes::CZTargetNew / NCameraModes::CZTargetNew::vfunc_18`.
Identity: proven. Own RTTI vtable slot18 and donor name set independently identify original const virtual float GetDefaultFOV. Retail reads PCameraModeDef at68, FOV at80 then multiplies by 1/360 literal at01238e00. Original typed CBase and CPhysicsState fields reconcile through four-byte STLport lists; CCameraModeDef fields follow packed Parent57 with first field60. Target headers absent (CCameraModeDef exists only in quarantine). Unused virtual slots are descriptive projections; CZTargetNew unused derived suffix is omitted without a size claim.
Layout: CBase164, CPhysicsState48, pointer wrapper4, vector12, list4, CCameraModeDef216 with FOV80. Original complete definition and owner base hierarchy; no anonymous offset substitute.

## 0084f740: std::vector<CCountedPointer<NCameraModes::CSubComponentBase>>::operator=
Original manifest label: `_gapscan / sub_0084f740`.
Identity: proven. Own destruction0056dbf0 original counted camera subcomponent and CBase::SubComponents72; constructor0084fab0 reuses same helper. Original STLport vector copy assignment with allocate/copy/destroy/free paths. Real typed base/proxy hierarchy and genuine helper loops; tag parameters unnamed and never read or written.
Layout: vector12, original element8, exact scalar malloc/free thunks and explicit nontrivial element semantics. No direct concrete element this layout beyond recovered original members.

## 008b3180: std::vector<CCombatAction>::operator=
Original manifest label: `std::vector / std::vector::_Copy_assign`.
Identity: proven. Own destruction008b3010 reused by original CCombatAction vector growth008b3430; original CCombatAction8 consists of CCountedPointer<CCombatActionBase>. Original STLport vector copy assignment with allocate/copy/destroy/free paths. Real typed base/proxy hierarchy and genuine helper loops; tag parameters unnamed and never read or written.
Layout: vector12, original element8, exact scalar malloc/free thunks and explicit nontrivial element semantics. No direct concrete element this layout beyond recovered original members.

## 00a6c5a0: CPhysicsMesh::GetNoHelperPoints
Original manifest label: `_gapscan / sub_00a6c5a0`.
Identity: proven. Own CPhysicsMeshObject forwarding wrapper00a11750/00a11800 reached by graphic appearance helper/dummy queries; original PDB exact CPhysicsMesh const GetNoHelperPoints/GetNoDummyObjects signatures. HelperPoints16 and DummyObjects32 reconcile original CVectorMap fields with STLport12-byte vector; record strides20/56 match original pair<CCharString,C3DHelperPoint/C3DDummyObject>. Original CIVCountedPointeeBase ancestry from own RTTI. String helper original protected static CBasicString<char>::Compare00411570.
Layout: original typed mesh prefix through both maps; unused suffix omitted. StringData packed17/BasicString13; original vector/base/proxy hierarchy. No active owner header.

## 00a6c5f0: CPhysicsMesh::GetNoDummyObjects
Original manifest label: `_gapscan / sub_00a6c5f0`.
Identity: proven. Own CPhysicsMeshObject forwarding wrapper00a11750/00a11800 reached by graphic appearance helper/dummy queries; original PDB exact CPhysicsMesh const GetNoHelperPoints/GetNoDummyObjects signatures. HelperPoints16 and DummyObjects32 reconcile original CVectorMap fields with STLport12-byte vector; record strides20/56 match original pair<CCharString,C3DHelperPoint/C3DDummyObject>. Original CIVCountedPointeeBase ancestry from own RTTI. String helper original protected static CBasicString<char>::Compare00411570.
Layout: original typed mesh prefix through both maps; unused suffix omitted. StringData packed17/BasicString13; original vector/base/proxy hierarchy. No active owner header.

## 00a6eae0: CCountedPointer<const C3DMeshFilePrimitiveChunk>::operator=
Original manifest label: `_gapscan / sub_00a6eae0`.
Identity: proven. Own call00a8c33d consumes return of C3DMeshFileSubMeshChunk::FindNextPrimitive const00abaa10; exact original return type CCountedPointer<const C3DMeshFilePrimitiveChunk> matches typed consumer. Own target is the shared-control-block copy assignment; original wrapper/control-block class keys and fields from names/layouts. Fastcall deleter consumes ECX; scalar delete releases control block.
Layout: wrapper Data0/Info4; CCPPointerInfo RefCount0/DeleteFunc4/Data8. Original template and genuine pointer element type; no concrete owner layout is instantiated.

## 00a8d2e0: CCountedPointer<const C3DMeshFileTransformationChunk>::operator=
Original manifest label: `_gapscan / sub_00a8d2e0`.
Identity: proven. Own call00a8c577 consumes const submesh FindTransformation00aba4a0, independently confirmed TRFM identifier0129eaec and following matrix48-byte read at returned object44. Original PDB return type CCountedPointer<const C3DMeshFileTransformationChunk>. Own target is the shared-control-block copy assignment; original wrapper/control-block class keys and fields from names/layouts. Fastcall deleter consumes ECX; scalar delete releases control block.
Layout: wrapper Data0/Info4; CCPPointerInfo RefCount0/DeleteFunc4/Data8. Original template and genuine pointer element type; no concrete owner layout is instantiated.

## 00a83ab0: std::__introsort_loop
Original manifest label: `_global / Heap_AdjustDown_32ByteElements`.
Identity: proven. Own unsigned64 key comparisons, median-of-three, threshold16 and depth-limited recursion identify SGI __introsort_loop. Related own partialsort00a828c0/00a829e0 and makeheap00a80000/00a80080 recover original pair<unsigned __int64,bool>/pair<unsigned __int64,CANavQuadTreeNode*> and CKeyPairCompareLess. Pair16 copies only key8 and mappedvalue1/4; genuine memberwise pair copy constructor. No engine header applies to algorithm this-free instantiation.
Layout: original unsigned64 key, pair16 and empty by-value comparator; no copied data padding.

## 00a83bc0: std::__introsort_loop
Original manifest label: `_global / Heap_AdjustDown_16ByteElements`.
Identity: proven. Own unsigned64 key comparisons, median-of-three, threshold16 and depth-limited recursion identify SGI __introsort_loop. Related own partialsort00a828c0/00a829e0 and makeheap00a80000/00a80080 recover original pair<unsigned __int64,bool>/pair<unsigned __int64,CANavQuadTreeNode*> and CKeyPairCompareLess. Pair16 copies only key8 and mappedvalue1/4; genuine memberwise pair copy constructor. No engine header applies to algorithm this-free instantiation.
Layout: original unsigned64 key, pair16 and empty by-value comparator; no copied data padding.

## 00aab570: std::vector<CPlane>::_M_fill_insert
Original manifest label: `_global / Vector_Assign_ClothQuadWithFill`.
Identity: proven. Own C3DMeshFileConvexVolumeChunk::ReadFromFile00aaab10 invokes target for Planes48, same growth00a13720 and nonconst copy00a11210; CPlane Normal+Distance type independently confirmed. Memberwise user copy constructors and inline uninitialized_fill_n preserve the sixteen-byte local and placement-new null guard; exact original STLport typed helpers.
Layout: element16 and genuine vector12/base/proxy. No target engine header.

## 00bcc080: std::vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>::_M_fill_insert
Original manifest label: `_global / std_vector_insert_fill_stride`.
Identity: proven. Own RTTI CEngineInternalPrimitiveSpriteTrail::Update00bcbc00 invokes target for Trail104, same growth00bcbf80/copy00bca370; original nested CSpriteTrailEntry classV Position+Width. Memberwise user copy constructors and inline uninitialized_fill_n preserve the sixteen-byte local and placement-new null guard; exact original STLport typed helpers.
Layout: element16 and genuine vector12/base/proxy. No target engine header.

## 00ad5630: std::_Rb_tree::insert_unique
Original manifest label: `std / std::map_FindOrInsert`.
Identity: proven. Own callers00ad6094/00ad6799 and insert00ad4620/unhinted00ad4720 establish pair<long,long> key and CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> value; original optimizer PDB template corroborates.
Layout: genuine tree/base/node/iterator hierarchy, allocator proxy header0, nodecount4, comparator8; node value16. Retail engine vector12 within texture morph.

## 00b2f240: std::_Rb_tree::insert_unique
Original manifest label: `_global / RBTree_InsertOrFind`.
Identity: proven. Own map subscript00b2f657 constructs key pair and mapped two-long-plus-vector20 record via copy005bb346; original CEngineSurfaceCompositionManager GeneratedMorphs template and CEngineTextureMorph fields corroborate. Own insert00b2f060/unhinted00b2f130, iterator ++00b2e130/--00b2e5e0; comparator00ad11a0 shared with first tree.
Layout: genuine tree/base/node/iterator hierarchy, allocator proxy header0, nodecount4, comparator8; node value16. Retail engine vector12 within texture morph.

## 00afe7c0: std::vector<CArray<long>>::_M_fill_insert
Original manifest label: `_global / Vector_ResizeArray`.
Identity: proven. Own CTriangleStripifier2::BuildVertexAndPolyMaps00f3e6b0 calls both target fills. Original PDB signature has CArray<CArray<long>> and CArray<CArray<unsigned short>> output maps; full inner vector assignment callees00454886/00f3b640 and copy constructors00514160/00a87090 establish scalar widths and genuine template identity. First copy is inherited vector<long>, second explicit original CArray<unsigned short> copy; inner destructor frees its data through CRT free.
Layout: outer vector12 and inner CArray/vector12 with genuine base/proxy inheritance. No fabricated twelve-byte record.

## 00f3f960: std::vector<CArray<unsigned short>>::_M_fill_insert
Original manifest label: `_global / Vector_ResizeWithGrowth`.
Identity: proven. Own CTriangleStripifier2::BuildVertexAndPolyMaps00f3e6b0 calls both target fills. Original PDB signature has CArray<CArray<long>> and CArray<CArray<unsigned short>> output maps; full inner vector assignment callees00454886/00f3b640 and copy constructors00514160/00a87090 establish scalar widths and genuine template identity. First copy is inherited vector<long>, second explicit original CArray<unsigned short> copy; inner destructor frees its data through CRT free.
Layout: outer vector12 and inner CArray/vector12 with genuine base/proxy inheritance. No fabricated twelve-byte record.

## 00c0af90: CUnprovenAudioChannelBank::ReadSecond
Original manifest label: `_gapscan / sub_00c0af90`.
Identity: descriptive. Own getters share scoped module critical section0143bb14 via original PKCProtectionInstance00c0ffc0/00c0ffe0. Own indexed record52 atreceiver4, floats0/4 and bool48; capacity16 independently corroborated by neighboring count00c0aa10. If wrapping enabled, value>=pi subtracts2pi, value<-pi adds2pi exactly once. Actual literal floats0128f604/608 and012b1eb8 confirmed. Library owner/field names remain descriptive, no PDB claims.
Layout: polymorphic owner prefix4, channel52 with explicit unknown8..47 region; original guard8 and Win32 criticalsection24. No active engine owner header.

## 00c0b060: CUnprovenAudioChannelBank::ReadFirst
Original manifest label: `_gapscan / sub_00c0b060`.
Identity: descriptive. Own getters share scoped module critical section0143bb14 via original PKCProtectionInstance00c0ffc0/00c0ffe0. Own indexed record52 atreceiver4, floats0/4 and bool48; capacity16 independently corroborated by neighboring count00c0aa10. If wrapping enabled, value>=pi subtracts2pi, value<-pi adds2pi exactly once. Actual literal floats0128f604/608 and012b1eb8 confirmed. Library owner/field names remain descriptive, no PDB claims.
Layout: polymorphic owner prefix4, channel52 with explicit unknown8..47 region; original guard8 and Win32 criticalsection24. No active engine owner header.

## 00c14080: std::vector::_Insert_n
Original manifest label: `_global / vector_insert_realloc_grow`.
Identity: descriptive. Own worker-queue destructor00c14560 inserts null quit request into member24, whose actual allocator/first/last/end offsets0/4/8/12 prove Dinkumware vector16. Own _Xlen00c13840,_Ucopy00c13ec0,_Ufill00c13fc0,fill00c13a80,copy_backward00c13d40. Original library pointer pointee type not recovered, distinct descriptive incomplete classes prevent conflating helper addresses. Actual scalar new00bfea1a/scalar delete00bfe9bc, imported memmove01440144; 1.5 capacity growth and max elements0x3fffffff.
Layout: genuine vector with allocator member then three pointers; helpers use actual copy/fill semantics.

## 00c14270: std::vector::_Insert_n
Original manifest label: `_global / vector_insert_realloc_grow_variant`.
Identity: descriptive. Own worker-queue destructor00c14630 inserts null quit request into member24, whose actual allocator/first/last/end offsets0/4/8/12 prove Dinkumware vector16. Own _Xlen00c138f0,_Ucopy00c13ef0,_Ufill00c13ff0,fill00c13aa0,copy_backward00c13d70. Original library pointer pointee type not recovered, distinct descriptive incomplete classes prevent conflating helper addresses. Actual scalar new00bfea1a/scalar delete00bfe9bc, imported memmove01440144; 1.5 capacity growth and max elements0x3fffffff.
Layout: genuine vector with allocator member then three pointers; helpers use actual copy/fill semantics.

## 00c1ba60: CUnprovenPackedParameterRange::GetMinimum
Original manifest label: `_global / CTCInventoryItem_GetWeaponClass_Wrapper`.
Identity: descriptive. Own sibling wrappers and EncodeFixed16/DecodeFixed16 chain00c1ba20/00c1ba40; original decoder accepts unsigned16 stack word, returnsfloat, multiplies10000 and1/65535. Native callers00c12940/00c20580/00c62530 use both endpoints. Existing weapon/inventory labels are propagated and unsupported in this libpk range. Owner and original accessor names unproven; descriptive shared packed-parameter view. No evidence supports a specific base or first16-byte field layout, retained opaque explicitly.
Layout: field offsets16/18 from own loads; unseen16-byte prefix unproven. No target engine header. Existing helper spelling DecodeFixed16 reused.

## 00c1ba70: CUnprovenPackedParameterRange::GetMaximum
Original manifest label: `_global / Weapon_ExtractClassAndScale`.
Identity: descriptive. Own sibling wrappers and EncodeFixed16/DecodeFixed16 chain00c1ba20/00c1ba40; original decoder accepts unsigned16 stack word, returnsfloat, multiplies10000 and1/65535. Native callers00c12940/00c20580/00c62530 use both endpoints. Existing weapon/inventory labels are propagated and unsupported in this libpk range. Owner and original accessor names unproven; descriptive shared packed-parameter view. No evidence supports a specific base or first16-byte field layout, retained opaque explicitly.
Layout: field offsets16/18 from own loads; unseen16-byte prefix unproven. No target engine header. Existing helper spelling DecodeFixed16 reused.

## 00c265e0: NUnprovenLibraryState::CMember::OnEventOne
Original manifest label: `_gapscan / sub_00c265e0`.
Identity: descriptive. Own shared virtual table012b3fdc slots1/4, helper00c265a0 compares ushort State116==1; helper00c26580 sets2 and calls context+44 interface slot8 with original receiver. No PDB covers this library. Names and event signatures remain descriptive ABI projections; original event meanings and unused fields unproven. Visible real helper bodies preserve ECX across the state check, not an invented calling convention.
Layout: original member vptr0, opaque4..115, State116; context interface44. No applicable engine header. Unused interface slots are explicit projections.

## 00c26630: NUnprovenLibraryState::CMember::OnEventFour
Original manifest label: `_gapscan / sub_00c26630`.
Identity: descriptive. Own shared virtual table012b3fdc slots1/4, helper00c265a0 compares ushort State116==1; helper00c26580 sets2 and calls context+44 interface slot8 with original receiver. No PDB covers this library. Names and event signatures remain descriptive ABI projections; original event meanings and unused fields unproven. Visible real helper bodies preserve ECX across the state check, not an invented calling convention.
Layout: original member vptr0, opaque4..115, State116; context interface44. No applicable engine header. Unused interface slots are explicit projections.

## 00c266b0: NUnprovenLibraryResponse::CDefaultStateResponse::Evaluate
Original manifest label: `_gapscan / sub_00c266b0`.
Identity: descriptive. Own virtual table012b3fdc slot9 identified by constructor vptr stores (00c266e0 or00c37cc0); target ignores receiver and first native-width argument, writes signed output 0 and returns float 0.0f. No original library PDB coverage; class/method/argument semantics remain descriptive ABI projections. Constant is actual compiler literal, not an invented external variable.
Layout: only virtual interface slots are projected; no instance data is accessed or size claimed. No applicable engine header.

## 00c37a90: NUnprovenLibraryResponse::CDefaultPositionResponse::Evaluate
Original manifest label: `_gapscan / sub_00c37a90`.
Identity: descriptive. Own virtual table012b5370 slot9 identified by constructor vptr stores (00c266e0 or00c37cc0); target ignores receiver and first native-width argument, writes signed output -1 and returns float 0.5f. No original library PDB coverage; class/method/argument semantics remain descriptive ABI projections. Constant is actual compiler literal, not an invented external variable.
Layout: only virtual interface slots are projected; no instance data is accessed or size claimed. No applicable engine header.

## 00c29d00: NUnprovenLibraryCallbacks::CGroup::VisitOne
Original manifest label: `_gapscan / sub_00c29d00`.
Identity: descriptive. Own table012b40a4 slots1..4, actual constructor00c29f50 sets index4=-1 and First8=null; Add00c29ec0 stores member.Group48 and Next52. These original libpk names are not covered by PDB; existing catalog CSteeringGroup/CSteeringAgent are descriptive and collide with unrelated proven engine CSteeringAgent. Isolated descriptive namespace avoids claiming that identity. Arguments are opaque native-width ABI words projected as pointers. Retail caches Next before each callback and invokes the current member.
Layout: group vptr0, Index4, First8; member vptr0, unproven44-byte field range4..47, Group48, Next52. No engine header fits this library identity; unused virtual slot0 is only an ABI projection.

## 00c29d90: NUnprovenLibraryCallbacks::CGroup::VisitFour
Original manifest label: `_gapscan / sub_00c29d90`.
Identity: descriptive. Own table012b40a4 slots1..4, actual constructor00c29f50 sets index4=-1 and First8=null; Add00c29ec0 stores member.Group48 and Next52. These original libpk names are not covered by PDB; existing catalog CSteeringGroup/CSteeringAgent are descriptive and collide with unrelated proven engine CSteeringAgent. Isolated descriptive namespace avoids claiming that identity. Arguments are opaque native-width ABI words projected as pointers. Retail caches Next before each callback and invokes the current member.
Layout: group vptr0, Index4, First8; member vptr0, unproven44-byte field range4..47, Group48, Next52. No engine header fits this library identity; unused virtual slot0 is only an ABI projection.

## 00c29d30: NUnprovenLibraryCallbacks::CGroup::VisitTwo
Original manifest label: `_gapscan / sub_00c29d30`.
Identity: descriptive. Own table012b40a4 slots1..4, actual constructor00c29f50 sets index4=-1 and First8=null; Add00c29ec0 stores member.Group48 and Next52. These original libpk names are not covered by PDB; existing catalog CSteeringGroup/CSteeringAgent are descriptive and collide with unrelated proven engine CSteeringAgent. Isolated descriptive namespace avoids claiming that identity. Arguments are opaque native-width ABI words projected as pointers. Retail deliberately reloads First for each callback while traversal advances through saved Next; this behavior is retained, including head changes during callbacks.
Layout: group vptr0, Index4, First8; member vptr0, unproven44-byte field range4..47, Group48, Next52. No engine header fits this library identity; unused virtual slot0 is only an ABI projection.

## 00c29d60: NUnprovenLibraryCallbacks::CGroup::VisitThree
Original manifest label: `_gapscan / sub_00c29d60`.
Identity: descriptive. Own table012b40a4 slots1..4, actual constructor00c29f50 sets index4=-1 and First8=null; Add00c29ec0 stores member.Group48 and Next52. These original libpk names are not covered by PDB; existing catalog CSteeringGroup/CSteeringAgent are descriptive and collide with unrelated proven engine CSteeringAgent. Isolated descriptive namespace avoids claiming that identity. Arguments are opaque native-width ABI words projected as pointers. Retail deliberately reloads First for each callback while traversal advances through saved Next; this behavior is retained, including head changes during callbacks.
Layout: group vptr0, Index4, First8; member vptr0, unproven44-byte field range4..47, Group48, Next52. No engine header fits this library identity; unused virtual slot0 is only an ABI projection.

## 00c30210: CUnprovenCStringComparisons::EqualValue
Original manifest label: `_gapscan / sub_00c30210`.
Identity: descriptive. Own function entries in tables012b4b00 and012b4bd8 four comparison slots0/1/2/3, both equality/less pairs share CString::Compare(const char*)const00c0bce0. Second argument dereferenced at0 is original CString m_pchData; first is Compare receiver. this is unused. Original container/comparison-owner identity and method names unproven; descriptive shared four-slot interface preserves observed ABI. int32 return of normalized comparison bool is ABI-derived, not claimed PDB recovery. No collision between distinct target methods. Original abstract table012b1ab0 has four pure comparison slots followed by virtual destructor00c09890; concrete CString table012b4b00 slot4c30290 calls destructorc2fe10 resetting that exact base table. Full comparator inheritance and destructor slot now represented; names remain descriptive.
Layout: original CString data pointer0; unused comparer owner state omitted. No available target header.

## 00c30250: CUnprovenCStringComparisons::EqualKey
Original manifest label: `_gapscan / sub_00c30250`.
Identity: descriptive. Own function entries in tables012b4b00 and012b4bd8 four comparison slots0/1/2/3, both equality/less pairs share CString::Compare(const char*)const00c0bce0. Second argument dereferenced at0 is original CString m_pchData; first is Compare receiver. this is unused. Original container/comparison-owner identity and method names unproven; descriptive shared four-slot interface preserves observed ABI. int32 return of normalized comparison bool is ABI-derived, not claimed PDB recovery. No collision between distinct target methods. Original abstract table012b1ab0 has four pure comparison slots followed by virtual destructor00c09890; concrete CString table012b4b00 slot4c30290 calls destructorc2fe10 resetting that exact base table. Full comparator inheritance and destructor slot now represented; names remain descriptive.
Layout: original CString data pointer0; unused comparer owner state omitted. No available target header.

## 00c30230: CUnprovenCStringComparisons::LessValue
Original manifest label: `_gapscan / sub_00c30230`.
Identity: descriptive. Own function entries in tables012b4b00 and012b4bd8 four comparison slots0/1/2/3, both equality/less pairs share CString::Compare(const char*)const00c0bce0. Second argument dereferenced at0 is original CString m_pchData; first is Compare receiver. this is unused. Original container/comparison-owner identity and method names unproven; descriptive shared four-slot interface preserves observed ABI. int32 return of normalized comparison bool is ABI-derived, not claimed PDB recovery. No collision between distinct target methods. Original abstract table012b1ab0 has four pure comparison slots followed by virtual destructor00c09890; concrete CString table012b4b00 slot4c30290 calls destructorc2fe10 resetting that exact base table. Full comparator inheritance and destructor slot now represented; names remain descriptive.
Layout: original CString data pointer0; unused comparer owner state omitted. No available target header.

## 00c30270: CUnprovenCStringComparisons::LessKey
Original manifest label: `_gapscan / sub_00c30270`.
Identity: descriptive. Own function entries in tables012b4b00 and012b4bd8 four comparison slots0/1/2/3, both equality/less pairs share CString::Compare(const char*)const00c0bce0. Second argument dereferenced at0 is original CString m_pchData; first is Compare receiver. this is unused. Original container/comparison-owner identity and method names unproven; descriptive shared four-slot interface preserves observed ABI. int32 return of normalized comparison bool is ABI-derived, not claimed PDB recovery. No collision between distinct target methods. Original abstract table012b1ab0 has four pure comparison slots followed by virtual destructor00c09890; concrete CString table012b4b00 slot4c30290 calls destructorc2fe10 resetting that exact base table. Full comparator inheritance and destructor slot now represented; names remain descriptive.
Layout: original CString data pointer0; unused comparer owner state omitted. No available target header.

## 00c32470: PKCTRArray::FindValue
Original manifest label: `_gapscan / sub_00c32470`.
Identity: descriptive. Own callees00c32160/00c32220 implement binary search via comparator slots0/1 or2/3, writing full32-bit found output; own returned index then selects Entries4. Same class neighbor assertion literal .\PKCTRArray.cpp confirms PKCTRArray owner; Entries4/Capacity8/Count12. Method/comparator names and unused virtual interface are descriptive; no library PDB. Real helper body reads all inputs and writes only the found output, enabling native dead argument-slot reuse without artificial effects. Original abstract table012b1ab0 has four pure comparison slots followed by virtual destructor00c09890; concrete CString table012b4b00 slot4c30290 calls destructorc2fe10 resetting that exact base table. Full comparator inheritance and destructor slot now represented; names remain descriptive.
Layout: polymorphic owner prefix16, typed pointer entries and original count/capacity evidence; unknown virtual slot0 is an explicit projection. No applicable engine header.

## 00c324b0: PKCTRArray::FindKey
Original manifest label: `_gapscan / sub_00c324b0`.
Identity: descriptive. Own callees00c32160/00c32220 implement binary search via comparator slots0/1 or2/3, writing full32-bit found output; own returned index then selects Entries4. Same class neighbor assertion literal .\PKCTRArray.cpp confirms PKCTRArray owner; Entries4/Capacity8/Count12. Method/comparator names and unused virtual interface are descriptive; no library PDB. Real helper body reads all inputs and writes only the found output, enabling native dead argument-slot reuse without artificial effects. Original abstract table012b1ab0 has four pure comparison slots followed by virtual destructor00c09890; concrete CString table012b4b00 slot4c30290 calls destructorc2fe10 resetting that exact base table. Full comparator inheritance and destructor slot now represented; names remain descriptive.
Layout: polymorphic owner prefix16, typed pointer entries and original count/capacity evidence; unknown virtual slot0 is an explicit projection. No applicable engine header.

## 00c33330: CUnprovenBackendAudioSource_00C331A0::vector deleting destructor
Original manifest label: `_gapscan / sub_00c33330`.
Identity: descriptive owner, proven compiler-helper role; Own vector deleting target flags2 array arm calls compiler EH-vector destructor00bfeed8 with exact stride64, own plain destructor00c331a0, array cookie count and adjusted allocation pointer; flags1 invokes original custom deallocator00c3c370. Exact target is compiler generated from ordinary virtual destructor plus array new; no hand-written flags/iteration wrapper. Complete original object5868 -> CPooledNode8778 chain proven node full dtor4cf40 final5868store. Additional counted-node5860/RefCount0c -> audio source4f74/seven-word descriptor10..28/Flags2c -> backend source4f84/token30..3f. Counted dtor3c5c0 tailcallsnode; resource33120 cleanup3c450 then counted dtor; backend331a0 cleanup3c720 then resource33120. Descriptor is not WAVEFORMATEX; Format full32-bit incl80000000 and Channels+4, Bytes+c, Data+18 are own observed. Token handle/flags proven OpenAL helper4d9b0.
Layout: complete observed bases and typed data preserve retail size. Every unrecovered library type/member name explicit; ordinary C++ new[] makes the actual helper materialize. No applicable shared engine header.

## 00c3d0f0: CListenerState::vector deleting destructor
Original manifest label: `_gapscan / sub_00c3d0f0`.
Identity: descriptive owner, proven compiler-helper role; Own vector deleting target flags2 array arm calls compiler EH-vector destructor00bfeed8 with exact stride128, own plain destructor00c3cec0, array cookie count and adjusted allocation pointer; flags1 invokes original custom deallocator00c3c370. Exact target is compiler generated from ordinary virtual destructor plus array new; no hand-written flags/iteration wrapper. Complete original object5868 -> CPooledNode8778 chain proven node full dtor4cf40 final5868store. Listener constructor3cea0 callsnode; dtor3cec0 cleanup40800 thennode4cf40. Full original128-byte typed listener model shared with N310/N311; custom class deallocation declarations bind actual common free address. Inactive original allocator member ownership/name not claimed.
Layout: complete observed bases and typed data preserve retail size. Every unrecovered library type/member name explicit; ordinary C++ new[] makes the actual helper materialize. No applicable shared engine header.

## 00c34df0: CUnprovenTwoLevelCollection_00C34DD0::InnerCount
Original manifest label: `_gapscan / sub_00c34df0`.
Identity: descriptive; Own constructor00c34dd0 sets012b501c; destructor00c34e60 transitions to original abstract base012b3444,11 operation slots plus virtual dtor11. Target atslot4. Original outer pointer array embedded+4 is Entries0/Capacity4/Count8, ctor initializes all to0, add00c34f30 appends12-byte inner arrays from00c42a60; inner Count helper00c42ac0 or random-selection helper00c42a70. All owner/helper names and template spelling descriptive; no PDB library names proven and no claim this is polymorphic PKCTRArray.
Layout: actual original abstract base with no data beyond its vptr; genuine observed nested pointer-array storage, outer16bytes and inner12bytes in32-bit build. Inactive unknown virtual prototypes are projections; no applicable shared engine header.

## 00c34e40: CUnprovenTwoLevelCollection_00C34DD0::RandomValue
Original manifest label: `_gapscan / sub_00c34e40`.
Identity: descriptive; Own constructor00c34dd0 sets012b501c; destructor00c34e60 transitions to original abstract base012b3444,11 operation slots plus virtual dtor11. Target atslot7. Original outer pointer array embedded+4 is Entries0/Capacity4/Count8, ctor initializes all to0, add00c34f30 appends12-byte inner arrays from00c42a60; inner Count helper00c42ac0 or random-selection helper00c42a70. All owner/helper names and template spelling descriptive; no PDB library names proven and no claim this is polymorphic PKCTRArray.
Layout: actual original abstract base with no data beyond its vptr; genuine observed nested pointer-array storage, outer16bytes and inner12bytes in32-bit build. Inactive unknown virtual prototypes are projections; no applicable shared engine header.

## 00c39d50: PKCOneShotQueue<CUnprovenOneShotValue_00C39D50>::Take
Original manifest label: `_gapscan / sub_00c39d50`.
Identity: probable original template owner from exact assertion filename; Own diagnostic filename PKCOneShotQueue.h line28 and exact Nothing to return! message. Own four-slot table012b5484 is Take/Put/HasValue/Count with no virtual destructor; constructors initialize full32-bit Filled after4/12-byte trivial Value, not bool. Take returns T by value using native hidden class-return pointer (EAX preserved); it has no explicit output reference. Own EAX tail proves returned buffer identity and explains the copy scheduling. Target reports invalid state using actual CString chain before CDebugBreak then still performs operation; no original element identity recoverable. Distinct descriptive trivial unsigned-word projections preserve own observed copy width without claiming recovered scalar/vector source types.
Layout: source-proven polymorphic queue Value+4, full32-bit Filled+8; no unsupported base or destructor invented. Actual CString/assert-hook/debug-break family reused, preserving EH lifetime and callback effects. No shared middleware header available. Element projections are explicit original-identity uncertainty allowed by owner rules.

## 00c3a540: PKCOneShotQueue<CUnprovenOneShotValue_00C3A540>::Take
Original manifest label: `_gapscan / sub_00c3a540`.
Identity: probable original template owner from exact assertion filename; Own diagnostic filename PKCOneShotQueue.h line28 and exact Nothing to return! message. Own four-slot table012b54c4 is Take/Put/HasValue/Count with no virtual destructor; constructors initialize full32-bit Filled after4/12-byte trivial Value, not bool. Take returns T by value using native hidden class-return pointer (EAX preserved); it has no explicit output reference. Own EAX tail proves returned buffer identity and explains the copy scheduling. Target reports invalid state using actual CString chain before CDebugBreak then still performs operation; no original element identity recoverable. Distinct descriptive trivial unsigned-word projections preserve own observed copy width without claiming recovered scalar/vector source types.
Layout: source-proven polymorphic queue Value+4, full32-bit Filled+8; no unsupported base or destructor invented. Actual CString/assert-hook/debug-break family reused, preserving EH lifetime and callback effects. No shared middleware header available. Element projections are explicit original-identity uncertainty allowed by owner rules.

## 00c39e20: PKCOneShotQueue<CUnprovenOneShotValue_00C39D50>::Put
Original manifest label: `_gapscan / sub_00c39e20`.
Identity: probable original template owner from exact assertion filename; Own diagnostic filename PKCOneShotQueue.h line35 and exact Already filled this queue! message. Own four-slot table012b5484 is Take/Put/HasValue/Count with no virtual destructor; constructors initialize full32-bit Filled after4/12-byte trivial Value, not bool. Take returns T by value using native hidden class-return pointer (EAX preserved); it has no explicit output reference. Own EAX tail proves returned buffer identity and explains the copy scheduling. Target reports invalid state using actual CString chain before CDebugBreak then still performs operation; no original element identity recoverable. Distinct descriptive trivial unsigned-word projections preserve own observed copy width without claiming recovered scalar/vector source types.
Layout: source-proven polymorphic queue Value+4, full32-bit Filled+8; no unsupported base or destructor invented. Actual CString/assert-hook/debug-break family reused, preserving EH lifetime and callback effects. No shared middleware header available. Element projections are explicit original-identity uncertainty allowed by owner rules.

## 00c3a610: PKCOneShotQueue<CUnprovenOneShotValue_00C3A540>::Put
Original manifest label: `_gapscan / sub_00c3a610`.
Identity: probable original template owner from exact assertion filename; Own diagnostic filename PKCOneShotQueue.h line35 and exact Already filled this queue! message. Own four-slot table012b54c4 is Take/Put/HasValue/Count with no virtual destructor; constructors initialize full32-bit Filled after4/12-byte trivial Value, not bool. Take returns T by value using native hidden class-return pointer (EAX preserved); it has no explicit output reference. Own EAX tail proves returned buffer identity and explains the copy scheduling. Target reports invalid state using actual CString chain before CDebugBreak then still performs operation; no original element identity recoverable. Distinct descriptive trivial unsigned-word projections preserve own observed copy width without claiming recovered scalar/vector source types.
Layout: source-proven polymorphic queue Value+4, full32-bit Filled+8; no unsupported base or destructor invented. Actual CString/assert-hook/debug-break family reused, preserving EH lifetime and callback effects. No shared middleware header available. Element projections are explicit original-identity uncertainty allowed by owner rules.

## 00c39f30: PKCOneShotQueue<CUnprovenOneShotValue_00C39F30>::Take
Original manifest label: `_gapscan / sub_00c39f30`.
Identity: probable original template owner from exact assertion filename; Own diagnostic filename PKCOneShotQueue.h line28 and exact Nothing to return! message. Own four-slot table012b5494 is Take/Put/HasValue/Count with no virtual destructor; constructors initialize full32-bit Filled after4/12-byte trivial Value, not bool. Take returns T by value using native hidden class-return pointer (EAX preserved); it has no explicit output reference. Own EAX tail proves returned buffer identity and explains the copy scheduling. Target reports invalid state using actual CString chain before CDebugBreak then still performs operation; no original element identity recoverable. Distinct descriptive trivial unsigned-word projections preserve own observed copy width without claiming recovered scalar/vector source types.
Layout: source-proven polymorphic queue Value+4, full32-bit Filled+16; no unsupported base or destructor invented. Actual CString/assert-hook/debug-break family reused, preserving EH lifetime and callback effects. No shared middleware header available. Element projections are explicit original-identity uncertainty allowed by owner rules.

## 00c3a130: PKCOneShotQueue<CUnprovenOneShotValue_00C3A130>::Take
Original manifest label: `_gapscan / sub_00c3a130`.
Identity: probable original template owner from exact assertion filename; Own diagnostic filename PKCOneShotQueue.h line28 and exact Nothing to return! message. Own four-slot table012b54a4 is Take/Put/HasValue/Count with no virtual destructor; constructors initialize full32-bit Filled after4/12-byte trivial Value, not bool. Take returns T by value using native hidden class-return pointer (EAX preserved); it has no explicit output reference. Own EAX tail proves returned buffer identity and explains the copy scheduling. Target reports invalid state using actual CString chain before CDebugBreak then still performs operation; no original element identity recoverable. Distinct descriptive trivial unsigned-word projections preserve own observed copy width without claiming recovered scalar/vector source types.
Layout: source-proven polymorphic queue Value+4, full32-bit Filled+16; no unsupported base or destructor invented. Actual CString/assert-hook/debug-break family reused, preserving EH lifetime and callback effects. No shared middleware header available. Element projections are explicit original-identity uncertainty allowed by owner rules.

## 00c3a010: PKCOneShotQueue<CUnprovenOneShotValue_00C39F30>::Put
Original manifest label: `_gapscan / sub_00c3a010`.
Identity: probable original template owner from exact assertion filename; Own diagnostic filename PKCOneShotQueue.h line35 and exact Already filled this queue! message. Own four-slot table012b5494 is Take/Put/HasValue/Count with no virtual destructor; constructors initialize full32-bit Filled after4/12-byte trivial Value, not bool. Take returns T by value using native hidden class-return pointer (EAX preserved); it has no explicit output reference. Own EAX tail proves returned buffer identity and explains the copy scheduling. Target reports invalid state using actual CString chain before CDebugBreak then still performs operation; no original element identity recoverable. Distinct descriptive trivial unsigned-word projections preserve own observed copy width without claiming recovered scalar/vector source types.
Layout: source-proven polymorphic queue Value+4, full32-bit Filled+16; no unsupported base or destructor invented. Actual CString/assert-hook/debug-break family reused, preserving EH lifetime and callback effects. No shared middleware header available. Element projections are explicit original-identity uncertainty allowed by owner rules.

## 00c3a210: PKCOneShotQueue<CUnprovenOneShotValue_00C3A130>::Put
Original manifest label: `_gapscan / sub_00c3a210`.
Identity: probable original template owner from exact assertion filename; Own diagnostic filename PKCOneShotQueue.h line35 and exact Already filled this queue! message. Own four-slot table012b54a4 is Take/Put/HasValue/Count with no virtual destructor; constructors initialize full32-bit Filled after4/12-byte trivial Value, not bool. Take returns T by value using native hidden class-return pointer (EAX preserved); it has no explicit output reference. Own EAX tail proves returned buffer identity and explains the copy scheduling. Target reports invalid state using actual CString chain before CDebugBreak then still performs operation; no original element identity recoverable. Distinct descriptive trivial unsigned-word projections preserve own observed copy width without claiming recovered scalar/vector source types.
Layout: source-proven polymorphic queue Value+4, full32-bit Filled+16; no unsupported base or destructor invented. Actual CString/assert-hook/debug-break family reused, preserving EH lifetime and callback effects. No shared middleware header available. Element projections are explicit original-identity uncertainty allowed by owner rules.

## 00c3af40: CUnprovenComparisonAdapter_00C3AF40::EqualValue
Original manifest label: `_gapscan / sub_00c3af40`.
Identity: descriptive; Own concrete comparator table012b5530 slots0/1/2 are value equality/unsigned less/raw-key equality; four comparator slots then virtual deleting destructor. Original abstract table012b1ab0 has four pure slots and destructor00c09890 atslot4; destructor transitions prove genuine base. Actual getter00C3AB20 returns reference/pointer to record+4 and preservesEDX; same-TU visible noinline body retains its genuine effect. Constructors store collection pointer+4; record unusedprefix/complete extent and all library type/API names are explicit descriptive projections; original PDB names unavailable.
Layout: actual polymorphic comparator base and four operations plus destructor, collection pointer4; record prefix contains only observed word0/key4 and does not assert full original size. Same interface as PKCTRArray binary searches. No applicable shared engine header.

## 00c3b4d0: CUnprovenComparisonAdapter_00C3B4D0::EqualValue
Original manifest label: `_gapscan / sub_00c3b4d0`.
Identity: descriptive; Own concrete comparator table012b5608 slots0/1/2 are value equality/unsigned less/raw-key equality; four comparator slots then virtual deleting destructor. Original abstract table012b1ab0 has four pure slots and destructor00c09890 atslot4; destructor transitions prove genuine base. Actual getter00C3B0B0 returns reference/pointer to record+4 and preservesEDX; same-TU visible noinline body retains its genuine effect. Constructors store collection pointer+4; record unusedprefix/complete extent and all library type/API names are explicit descriptive projections; original PDB names unavailable.
Layout: actual polymorphic comparator base and four operations plus destructor, collection pointer4; record prefix contains only observed word0/key4 and does not assert full original size. Same interface as PKCTRArray binary searches. No applicable shared engine header.

## 00c3af70: CUnprovenComparisonAdapter_00C3AF40::LessValue
Original manifest label: `_gapscan / sub_00c3af70`.
Identity: descriptive; Own concrete comparator table012b5530 slots0/1/2 are value equality/unsigned less/raw-key equality; four comparator slots then virtual deleting destructor. Original abstract table012b1ab0 has four pure slots and destructor00c09890 atslot4; destructor transitions prove genuine base. Actual getter00C3AB20 returns reference/pointer to record+4 and preservesEDX; same-TU visible noinline body retains its genuine effect. Constructors store collection pointer+4; record unusedprefix/complete extent and all library type/API names are explicit descriptive projections; original PDB names unavailable.
Layout: actual polymorphic comparator base and four operations plus destructor, collection pointer4; record prefix contains only observed word0/key4 and does not assert full original size. Same interface as PKCTRArray binary searches. No applicable shared engine header.

## 00c3b500: CUnprovenComparisonAdapter_00C3B4D0::LessValue
Original manifest label: `_gapscan / sub_00c3b500`.
Identity: descriptive; Own concrete comparator table012b5608 slots0/1/2 are value equality/unsigned less/raw-key equality; four comparator slots then virtual deleting destructor. Original abstract table012b1ab0 has four pure slots and destructor00c09890 atslot4; destructor transitions prove genuine base. Actual getter00C3B0B0 returns reference/pointer to record+4 and preservesEDX; same-TU visible noinline body retains its genuine effect. Constructors store collection pointer+4; record unusedprefix/complete extent and all library type/API names are explicit descriptive projections; original PDB names unavailable.
Layout: actual polymorphic comparator base and four operations plus destructor, collection pointer4; record prefix contains only observed word0/key4 and does not assert full original size. Same interface as PKCTRArray binary searches. No applicable shared engine header.

## 00c3af90: CUnprovenComparisonAdapter_00C3AF40::EqualKey
Original manifest label: `_gapscan / sub_00c3af90`.
Identity: descriptive; Own concrete comparator table012b5530 slots0/1/2 are value equality/unsigned less/raw-key equality; four comparator slots then virtual deleting destructor. Original abstract table012b1ab0 has four pure slots and destructor00c09890 atslot4; destructor transitions prove genuine base. Actual getter00C3AB20 returns reference/pointer to record+4 and preservesEDX; same-TU visible noinline body retains its genuine effect. Constructors store collection pointer+4; record unusedprefix/complete extent and all library type/API names are explicit descriptive projections; original PDB names unavailable.
Layout: actual polymorphic comparator base and four operations plus destructor, collection pointer4; record prefix contains only observed word0/key4 and does not assert full original size. Same interface as PKCTRArray binary searches. No applicable shared engine header.

## 00c3b520: CUnprovenComparisonAdapter_00C3B4D0::EqualKey
Original manifest label: `_gapscan / sub_00c3b520`.
Identity: descriptive; Own concrete comparator table012b5608 slots0/1/2 are value equality/unsigned less/raw-key equality; four comparator slots then virtual deleting destructor. Original abstract table012b1ab0 has four pure slots and destructor00c09890 atslot4; destructor transitions prove genuine base. Actual getter00C3B0B0 returns reference/pointer to record+4 and preservesEDX; same-TU visible noinline body retains its genuine effect. Constructors store collection pointer+4; record unusedprefix/complete extent and all library type/API names are explicit descriptive projections; original PDB names unavailable.
Layout: actual polymorphic comparator base and four operations plus destructor, collection pointer4; record prefix contains only observed word0/key4 and does not assert full original size. Same interface as PKCTRArray binary searches. No applicable shared engine header.

## 00c40970: CListenerState::SetVector0C
Original manifest label: `_gapscan / sub_00c40970`.
Identity: descriptive; Full original node dtor00c4cf40 finalvptr store00c4cf69 proves common original virtual-object base012b5868 beneath CPooledNode. Own parameter constructor00c3cea0 calls original intrusive-node ctor00c4cf20 (Prev4/Next8, virtualdtor0 and table012b8778), stores012b5a84 and null EffectHandle7c; destructor00c3cec0 cleans backend00c40800 then original node00c4cf40. Actual default initialization00c40750 and individual setters00c40860..00c40cf0 establish two float3s+0c/+18, typed integer/float fields24..74 and full32-bit DirtyFlags78. Owner128-byte size proven original vector-deleting helper00c3d0f0 stride80h. Parameter/type/API spellings are descriptive and unproven; no original library PDB names or applicable shared engine headers; own target fieldVector0C and dirtybit0x100. Nonnull compares three float coordinates with ordinary unordered-aware !=, copies only on change; null compares all three to0 then writes0,0,1, preserving original unusual zero-test/default mismatch (not silently repaired).
Layout: actual original intrusive-list-node base and full128-byte typed parameter owner. Unproven word70 retains observed32-bit storage without claiming pointer/enum source identity.

## 00c40a70: CListenerState::SetVector18
Original manifest label: `_gapscan / sub_00c40a70`.
Identity: descriptive; Full original node dtor00c4cf40 finalvptr store00c4cf69 proves common original virtual-object base012b5868 beneath CPooledNode. Own parameter constructor00c3cea0 calls original intrusive-node ctor00c4cf20 (Prev4/Next8, virtualdtor0 and table012b8778), stores012b5a84 and null EffectHandle7c; destructor00c3cec0 cleans backend00c40800 then original node00c4cf40. Actual default initialization00c40750 and individual setters00c40860..00c40cf0 establish two float3s+0c/+18, typed integer/float fields24..74 and full32-bit DirtyFlags78. Owner128-byte size proven original vector-deleting helper00c3d0f0 stride80h. Parameter/type/API spellings are descriptive and unproven; no original library PDB names or applicable shared engine headers; own target fieldVector18 and dirtybit0x800. Nonnull compares three float coordinates with ordinary unordered-aware !=, copies only on change; null compares all three to0 then writes0,0,1, preserving original unusual zero-test/default mismatch (not silently repaired).
Layout: actual original intrusive-list-node base and full128-byte typed parameter owner. Unproven word70 retains observed32-bit storage without claiming pointer/enum source identity.

## 00c40a20: CListenerState::SetInteger44
Original manifest label: `_gapscan / sub_00c40a20`.
Identity: descriptive; Full original node dtor00c4cf40 finalvptr store00c4cf69 proves common original virtual-object base012b5868 beneath CPooledNode. Own parameter constructor00c3cea0 calls original intrusive-node ctor00c4cf20 (Prev4/Next8, virtualdtor0 and table012b8778), stores012b5a84 and null EffectHandle7c; destructor00c3cec0 cleans backend00c40800 then original node00c4cf40. Actual default initialization00c40750 and individual setters00c40860..00c40cf0 establish two float3s+0c/+18, typed integer/float fields24..74 and full32-bit DirtyFlags78. Owner128-byte size proven original vector-deleting helper00c3d0f0 stride80h. Parameter/type/API spellings are descriptive and unproven; no original library PDB names or applicable shared engine headers; own target fieldInteger44 and dirtybit0x200. Integer equality gates both store and dirtybit; unchanged values preserve existing dirty state.
Layout: actual original intrusive-list-node base and full128-byte typed parameter owner. Unproven word70 retains observed32-bit storage without claiming pointer/enum source identity.

## 00c40ca0: CListenerState::SetInteger6C
Original manifest label: `_gapscan / sub_00c40ca0`.
Identity: descriptive; Full original node dtor00c4cf40 finalvptr store00c4cf69 proves common original virtual-object base012b5868 beneath CPooledNode. Own parameter constructor00c3cea0 calls original intrusive-node ctor00c4cf20 (Prev4/Next8, virtualdtor0 and table012b8778), stores012b5a84 and null EffectHandle7c; destructor00c3cec0 cleans backend00c40800 then original node00c4cf40. Actual default initialization00c40750 and individual setters00c40860..00c40cf0 establish two float3s+0c/+18, typed integer/float fields24..74 and full32-bit DirtyFlags78. Owner128-byte size proven original vector-deleting helper00c3d0f0 stride80h. Parameter/type/API spellings are descriptive and unproven; no original library PDB names or applicable shared engine headers; own target fieldInteger6C and dirtybit0x80000. Integer equality gates both store and dirtybit; unchanged values preserve existing dirty state.
Layout: actual original intrusive-list-node base and full128-byte typed parameter owner. Unproven word70 retains observed32-bit storage without claiming pointer/enum source identity.

## 00c41880: CUnprovenExclusiveAudioObject_00C3D910::SetPair
Original manifest label: `_gapscan / sub_00c41880`.
Identity: descriptive; Full node dtor00c4cf40 finalvptr store00c4cf69 proves common virtual-object base012b5868 beneath CPooledNode. Own base CAudioObject constructor00c42d60 calls CPooledNode00c4cf20 and source-set ctor00c44250 at54; base dtor00c3cf40 calls shutdown44170/source-setdtor44550/node4cf40. Actual vtable012b5a88 has dtor0/bool1/bool2/Init3/Shutdown4/pure5/SetPair6/SetTriple7/pureAttach8. Own derived tables012b5ab0 exclusive object (ctor inheritance and destructor3d0e0->3cf40, stride1dc) and012b7b88 curve object(ctor44580->42d60,dtor3d0d0->3cf40,stride1bc) override6/7/8. Base typed properties/state/pointer24/28/8-byte Pair3c/12-byte Triple44/Backend50/source-set54/callbackdc/contexte0 agree existing descriptive CAudioObject. Unknown inactive ranges remain explicit projections, no original field/type names claimed; no shared middleware header. Unused derived suffixes omitted without derived sizeof claim; actual base helper00c43780 uses optional8-byte parameter24/3c. Own override marks alternative dirtybit0x100 and nulls alternative parameter pointer, after qualified original base setter.
Layout: genuine original CAudioObject/CPooledNode inheritance, typed base properties and source-set; inactive unknown members explicit.

## 00c418a0: CUnprovenExclusiveAudioObject_00C3D910::SetTriple
Original manifest label: `_gapscan / sub_00c418a0`.
Identity: descriptive; Full node dtor00c4cf40 finalvptr store00c4cf69 proves common virtual-object base012b5868 beneath CPooledNode. Own base CAudioObject constructor00c42d60 calls CPooledNode00c4cf20 and source-set ctor00c44250 at54; base dtor00c3cf40 calls shutdown44170/source-setdtor44550/node4cf40. Actual vtable012b5a88 has dtor0/bool1/bool2/Init3/Shutdown4/pure5/SetPair6/SetTriple7/pureAttach8. Own derived tables012b5ab0 exclusive object (ctor inheritance and destructor3d0e0->3cf40, stride1dc) and012b7b88 curve object(ctor44580->42d60,dtor3d0d0->3cf40,stride1bc) override6/7/8. Base typed properties/state/pointer24/28/8-byte Pair3c/12-byte Triple44/Backend50/source-set54/callbackdc/contexte0 agree existing descriptive CAudioObject. Unknown inactive ranges remain explicit projections, no original field/type names claimed; no shared middleware header. Unused derived suffixes omitted without derived sizeof claim; actual base helper00c437a0 uses optional12-byte parameter28/44. Own override marks alternative dirtybit0x80 and nulls alternative parameter pointer, after qualified original base setter.
Layout: genuine original CAudioObject/CPooledNode inheritance, typed base properties and source-set; inactive unknown members explicit.

## 00c418c0: CUnprovenExclusiveAudioObject_00C3D910::AttachBackend
Original manifest label: `_gapscan / sub_00c418c0`.
Identity: descriptive; Full node dtor00c4cf40 finalvptr store00c4cf69 proves common virtual-object base012b5868 beneath CPooledNode. Own base CAudioObject constructor00c42d60 calls CPooledNode00c4cf20 and source-set ctor00c44250 at54; base dtor00c3cf40 calls shutdown44170/source-setdtor44550/node4cf40. Actual vtable012b5a88 has dtor0/bool1/bool2/Init3/Shutdown4/pure5/SetPair6/SetTriple7/pureAttach8. Own derived tables012b5ab0 exclusive object (ctor inheritance and destructor3d0e0->3cf40, stride1dc) and012b7b88 curve object(ctor44580->42d60,dtor3d0d0->3cf40,stride1bc) override6/7/8. Base typed properties/state/pointer24/28/8-byte Pair3c/12-byte Triple44/Backend50/source-set54/callbackdc/contexte0 agree existing descriptive CAudioObject. Unknown inactive ranges remain explicit projections, no original field/type names claimed; no shared middleware header. Unused derived suffixes omitted without derived sizeof claim; own member helper00c43b30 receives source, globalcount0139c9fc, two exact24-byte configuration block addresses0139C9A0/0139C9D0, output pointer and propagates fullwidth return. Config record kind/names and argument source types are explicit descriptive ABI projections, no original named struct claimed.
Layout: original CAudioObject/node bases and exact global record addresses retained.

## 00c448a0: CUnprovenCurveAudioObject_00C3D8B0::AttachBackend
Original manifest label: `_gapscan / sub_00c448a0`.
Identity: descriptive; Full node dtor00c4cf40 finalvptr store00c4cf69 proves common virtual-object base012b5868 beneath CPooledNode. Own base CAudioObject constructor00c42d60 calls CPooledNode00c4cf20 and source-set ctor00c44250 at54; base dtor00c3cf40 calls shutdown44170/source-setdtor44550/node4cf40. Actual vtable012b5a88 has dtor0/bool1/bool2/Init3/Shutdown4/pure5/SetPair6/SetTriple7/pureAttach8. Own derived tables012b5ab0 exclusive object (ctor inheritance and destructor3d0e0->3cf40, stride1dc) and012b7b88 curve object(ctor44580->42d60,dtor3d0d0->3cf40,stride1bc) override6/7/8. Base typed properties/state/pointer24/28/8-byte Pair3c/12-byte Triple44/Backend50/source-set54/callbackdc/contexte0 agree existing descriptive CAudioObject. Unknown inactive ranges remain explicit projections, no original field/type names claimed; no shared middleware header. Unused derived suffixes omitted without derived sizeof claim; own member helper00c43b30 receives source, globalcount0139c9fc, two exact24-byte configuration block addresses0139C988/0139C9B8, output pointer and propagates fullwidth return. Config record kind/names and argument source types are explicit descriptive ABI projections, no original named struct claimed.
Layout: original CAudioObject/node bases and exact global record addresses retained.

## 00c44090: CAudioObject::Dispatch15
Original manifest label: `_gapscan / sub_00c44090`.
Identity: descriptive; Full node dtor00c4cf40 finalvptr store00c4cf69 proves common virtual-object base012b5868 beneath CPooledNode. Own base CAudioObject constructor00c42d60 calls CPooledNode00c4cf20 and source-set ctor00c44250 at54; base dtor00c3cf40 calls shutdown44170/source-setdtor44550/node4cf40. Actual vtable012b5a88 has dtor0/bool1/bool2/Init3/Shutdown4/pure5/SetPair6/SetTriple7/pureAttach8. Own derived tables012b5ab0 exclusive object (ctor inheritance and destructor3d0e0->3cf40, stride1dc) and012b7b88 curve object(ctor44580->42d60,dtor3d0d0->3cf40,stride1bc) override6/7/8. Base typed properties/state/pointer24/28/8-byte Pair3c/12-byte Triple44/Backend50/source-set54/callbackdc/contexte0 agree existing descriptive CAudioObject. Unknown inactive ranges remain explicit projections, no original field/type names claimed; no shared middleware header. Unused derived suffixes omitted without derived sizeof claim; target uses Backend50 or virtualAttachBackend slot8 with PreferredSource78 or current source from collection54 (Entries0,Count8,Index10), then forwards four native32-bit arguments to backend virtualslot15. Nonzero full32-bit errors report through singleton0143bb58 actual member00c3d090. Global5870 derived common5868 proven complete dtor3cb30; backend interface is explicitly an incomplete ABI contract, no concrete backend original class hierarchy/unused slot signatures claimed.
Layout: actual CAudioObject/common-object/node bases and typed active fields; backend four arguments and returns are descriptive native-width ABI projections, no original semantic argument types claimed. Inactive backend interface slots are abstract contract placeholders.

## 00c44100: CAudioObject::Dispatch16
Original manifest label: `_gapscan / sub_00c44100`.
Identity: descriptive; Full node dtor00c4cf40 finalvptr store00c4cf69 proves common virtual-object base012b5868 beneath CPooledNode. Own base CAudioObject constructor00c42d60 calls CPooledNode00c4cf20 and source-set ctor00c44250 at54; base dtor00c3cf40 calls shutdown44170/source-setdtor44550/node4cf40. Actual vtable012b5a88 has dtor0/bool1/bool2/Init3/Shutdown4/pure5/SetPair6/SetTriple7/pureAttach8. Own derived tables012b5ab0 exclusive object (ctor inheritance and destructor3d0e0->3cf40, stride1dc) and012b7b88 curve object(ctor44580->42d60,dtor3d0d0->3cf40,stride1bc) override6/7/8. Base typed properties/state/pointer24/28/8-byte Pair3c/12-byte Triple44/Backend50/source-set54/callbackdc/contexte0 agree existing descriptive CAudioObject. Unknown inactive ranges remain explicit projections, no original field/type names claimed; no shared middleware header. Unused derived suffixes omitted without derived sizeof claim; target uses Backend50 or virtualAttachBackend slot8 with PreferredSource78 or current source from collection54 (Entries0,Count8,Index10), then forwards four native32-bit arguments to backend virtualslot16. Nonzero full32-bit errors report through singleton0143bb58 actual member00c3d090. Global5870 derived common5868 proven complete dtor3cb30; backend interface is explicitly an incomplete ABI contract, no concrete backend original class hierarchy/unused slot signatures claimed.
Layout: actual CAudioObject/common-object/node bases and typed active fields; backend four arguments and returns are descriptive native-width ABI projections, no original semantic argument types claimed. Inactive backend interface slots are abstract contract placeholders.

## 00c44720: CUnprovenCurveAudioObject_00C3D8B0::SetPair
Original manifest label: `_gapscan / sub_00c44720`.
Identity: descriptive; Full node dtor00c4cf40 finalvptr store00c4cf69 proves common virtual-object base012b5868 beneath CPooledNode. Own base CAudioObject constructor00c42d60 calls CPooledNode00c4cf20 and source-set ctor00c44250 at54; base dtor00c3cf40 calls shutdown44170/source-setdtor44550/node4cf40. Actual vtable012b5a88 has dtor0/bool1/bool2/Init3/Shutdown4/pure5/SetPair6/SetTriple7/pureAttach8. Own derived tables012b5ab0 exclusive object (ctor inheritance and destructor3d0e0->3cf40, stride1dc) and012b7b88 curve object(ctor44580->42d60,dtor3d0d0->3cf40,stride1bc) override6/7/8. Base typed properties/state/pointer24/28/8-byte Pair3c/12-byte Triple44/Backend50/source-set54/callbackdc/contexte0 agree existing descriptive CAudioObject. Unknown inactive ranges remain explicit projections, no original field/type names claimed; no shared middleware header. Unused derived suffixes omitted without derived sizeof claim; actual base helper00c43780 uses optional8-byte parameter24/3c. Own curve-state bit10000 gates clearing alternative pointer and transitionwords110/114, dirtybits0x1100, then Statebit10000.
Layout: genuine original CAudioObject/CPooledNode inheritance, typed base properties and source-set; inactive unknown members explicit.

## 00c44760: CUnprovenCurveAudioObject_00C3D8B0::SetTriple
Original manifest label: `_gapscan / sub_00c44760`.
Identity: descriptive; Full node dtor00c4cf40 finalvptr store00c4cf69 proves common virtual-object base012b5868 beneath CPooledNode. Own base CAudioObject constructor00c42d60 calls CPooledNode00c4cf20 and source-set ctor00c44250 at54; base dtor00c3cf40 calls shutdown44170/source-setdtor44550/node4cf40. Actual vtable012b5a88 has dtor0/bool1/bool2/Init3/Shutdown4/pure5/SetPair6/SetTriple7/pureAttach8. Own derived tables012b5ab0 exclusive object (ctor inheritance and destructor3d0e0->3cf40, stride1dc) and012b7b88 curve object(ctor44580->42d60,dtor3d0d0->3cf40,stride1bc) override6/7/8. Base typed properties/state/pointer24/28/8-byte Pair3c/12-byte Triple44/Backend50/source-set54/callbackdc/contexte0 agree existing descriptive CAudioObject. Unknown inactive ranges remain explicit projections, no original field/type names claimed; no shared middleware header. Unused derived suffixes omitted without derived sizeof claim; actual base helper00c437a0 uses optional12-byte parameter28/44. Own curve-state bit10000 gates clearing alternative pointer and transitionwords110/114, dirtybits0x1080, then Statebit10000.
Layout: genuine original CAudioObject/CPooledNode inheritance, typed base properties and source-set; inactive unknown members explicit.

## 00c49e30: PKCStaticQueue<CUnprovenStaticQueueWord_00C49E30>::Put
Original manifest label: `_gapscan / sub_00c49e30`.
Identity: probable template owner from own assertion filename; Own filename PKCStaticQueue.h line57 and exact message This queue is full!. Concrete four-slot table012b80c8 has Take/Put/Count/IsFull, no virtual destructor; separate four-pure interface012b80d8 and constructor establish actual polymorphic base. Original constructor allocates capacity*4 and stores Capacity4/Entries8/Used12/WriteIndex16. Real OldestIndex00c49fe0 computes signed WriteIndex-Used, adds Capacity if negative. Actual element identity unknown; explicit unknown scalar32 representation uses a full-range enum projection, retaining proven raw EAX return ABI; neither original element kind nor enumerator identities are claimed.
Layout: genuine four-slot abstract queue interface and real typed ring storage. Unknown original library base/element/method names remain descriptive projections; no applicable shared engine header. Full CString diagnostic lifetime and unmodified debug-break ordering preserved.

## 00c4bb20: PKCStaticQueue<CUnprovenStaticQueueWord_00C4BB20>::Put
Original manifest label: `_gapscan / sub_00c4bb20`.
Identity: probable template owner from own assertion filename; Own filename PKCStaticQueue.h line57 and exact message This queue is full!. Concrete four-slot table012b8364 has Take/Put/Count/IsFull, no virtual destructor; separate four-pure interface012b8374 and constructor establish actual polymorphic base. Original constructor allocates capacity*4 and stores Capacity4/Entries8/Used12/WriteIndex16. Real OldestIndex00c4bf80 computes signed WriteIndex-Used, adds Capacity if negative. Actual element identity unknown; explicit unknown scalar32 representation uses a full-range enum projection, retaining proven raw EAX return ABI; neither original element kind nor enumerator identities are claimed.
Layout: genuine four-slot abstract queue interface and real typed ring storage. Unknown original library base/element/method names remain descriptive projections; no applicable shared engine header. Full CString diagnostic lifetime and unmodified debug-break ordering preserved.

## 00c49f10: PKCStaticQueue<CUnprovenStaticQueueWord_00C49E30>::Take
Original manifest label: `_gapscan / sub_00c49f10`.
Identity: probable template owner from own assertion filename; Own filename PKCStaticQueue.h line67 and exact message Nothing in this queue - you should have checked first!. Concrete four-slot table012b80c8 has Take/Put/Count/IsFull, no virtual destructor; separate four-pure interface012b80d8 and constructor establish actual polymorphic base. Original constructor allocates capacity*4 and stores Capacity4/Entries8/Used12/WriteIndex16. Real OldestIndex00c49fe0 computes signed WriteIndex-Used, adds Capacity if negative. Actual element identity unknown; explicit unknown scalar32 representation uses a full-range enum projection, retaining proven raw EAX return ABI; neither original element kind nor enumerator identities are claimed.
Layout: genuine four-slot abstract queue interface and real typed ring storage. Unknown original library base/element/method names remain descriptive projections; no applicable shared engine header. Full CString diagnostic lifetime and unmodified debug-break ordering preserved.

## 00c4beb0: PKCStaticQueue<CUnprovenStaticQueueWord_00C4BB20>::Take
Original manifest label: `_gapscan / sub_00c4beb0`.
Identity: probable template owner from own assertion filename; Own filename PKCStaticQueue.h line67 and exact message Nothing in this queue - you should have checked first!. Concrete four-slot table012b8364 has Take/Put/Count/IsFull, no virtual destructor; separate four-pure interface012b8374 and constructor establish actual polymorphic base. Original constructor allocates capacity*4 and stores Capacity4/Entries8/Used12/WriteIndex16. Real OldestIndex00c4bf80 computes signed WriteIndex-Used, adds Capacity if negative. Actual element identity unknown; explicit unknown scalar32 representation uses a full-range enum projection, retaining proven raw EAX return ABI; neither original element kind nor enumerator identities are claimed.
Layout: genuine four-slot abstract queue interface and real typed ring storage. Unknown original library base/element/method names remain descriptive projections; no applicable shared engine header. Full CString diagnostic lifetime and unmodified debug-break ordering preserved.

## 00c57f00: CUnprovenAudioBuffer_00C591F0::Clear
Original manifest label: `_gapscan / sub_00c57f00`.
Identity: descriptive; Own base constructor00c591f0 vtable012b8f38 and destructor00c57ef0 resetting original virtual-object base012b5868. Base table has destructor0/Reset1/Clear2/MixShortGain3/MixIntGain4/MixInt5/MixFloatGain6/MixFloatQ15Gain7/MixFloat8/Route9/Output10; adjacent012b8f64 is another class destructor, not slot11. Original base typed six planar int buffers+4..18, OutputShift1c, ChannelCount20 and unaligned raw storage24..C33; ctor rounds storage upward to16-byte alignment and sets six512-byte channel pointers. Actual filtered constructor00c3efa0 calls base and constructs six380-byte filters atC34; inactive derived suffix omitted without asserting derived size. MMX constructor00c3f660 and SSE00c3f7b0 call common filtered constructor and install012b5d40/012b5de0. Proven shared primary prefix modeled; any further MMX/SSE inheritance relationship is unproven, not recovered. All class/API/field names are descriptive projections, no library PDB identity claimed; original engine headers do not apply; own Clear targetslot2 and memset intrinsic zeroes ChannelCount*512 bytes from first planar buffer. No actual unknown helper or additional memory operation invented.
Layout: complete proven original buffer base chain and typed prefix/raw storage; omitted inactive filtered suffix explicit.

## 00c5c050: CUnprovenMMXAudioBuffer_00C3F660::Clear
Original manifest label: `_gapscan / sub_00c5c050`.
Identity: descriptive; Own base constructor00c591f0 vtable012b8f38 and destructor00c57ef0 resetting original virtual-object base012b5868. Base table has destructor0/Reset1/Clear2/MixShortGain3/MixIntGain4/MixInt5/MixFloatGain6/MixFloatQ15Gain7/MixFloat8/Route9/Output10; adjacent012b8f64 is another class destructor, not slot11. Original base typed six planar int buffers+4..18, OutputShift1c, ChannelCount20 and unaligned raw storage24..C33; ctor rounds storage upward to16-byte alignment and sets six512-byte channel pointers. Actual filtered constructor00c3efa0 calls base and constructs six380-byte filters atC34; inactive derived suffix omitted without asserting derived size. MMX constructor00c3f660 and SSE00c3f7b0 call common filtered constructor and install012b5d40/012b5de0. Proven shared primary prefix modeled; any further MMX/SSE inheritance relationship is unproven, not recovered. All class/API/field names are descriptive projections, no library PDB identity claimed; original engine headers do not apply; own Clear targetslot2 and memset intrinsic zeroes ChannelCount*512 bytes from first planar buffer. No actual unknown helper or additional memory operation invented.
Layout: complete proven original buffer base chain and typed prefix/raw storage; omitted inactive filtered suffix explicit.

## 00c58520: CUnprovenAudioBuffer_00C591F0::RouteFloat4
Original manifest label: `_global / AudioFormat_DecodeFloatChannels`.
Identity: descriptive; Own base constructor00c591f0 vtable012b8f38 and destructor00c57ef0 resetting original virtual-object base012b5868. Base table has destructor0/Reset1/Clear2/MixShortGain3/MixIntGain4/MixInt5/MixFloatGain6/MixFloatQ15Gain7/MixFloat8/Route9/Output10; adjacent012b8f64 is another class destructor, not slot11. Original base typed six planar int buffers+4..18, OutputShift1c, ChannelCount20 and unaligned raw storage24..C33; ctor rounds storage upward to16-byte alignment and sets six512-byte channel pointers. Actual filtered constructor00c3efa0 calls base and constructs six380-byte filters atC34; inactive derived suffix omitted without asserting derived size. MMX constructor00c3f660 and SSE00c3f7b0 call common filtered constructor and install012b5d40/012b5de0. Proven shared primary prefix modeled; any further MMX/SSE inheritance relationship is unproven, not recovered. All class/API/field names are descriptive projections, no library PDB identity claimed; original engine headers do not apply; own four-channel planar routing uses128 samples/512bytes per source plane, outputs mono4x quartergain/stereo alternating2x halfgain/otherwise four independent planes. Each virtualcall re-reads buffer pointers and the vtable, branch selection uses initial ChannelCount; float gains atslot6/plain8 or Q15int gains atslot4/plain5. Method source identity unknown; old DecodeFloatChannels label has no independent identity evidence.
Layout: original virtual-object/buffer hierarchy with six channel pointers and complete proven typed/raw-storage base. Inactive filtered suffix and original names explicitly unproven.

## 00c58630: CUnprovenAudioBuffer_00C591F0::RouteInt4
Original manifest label: `_global / Audio_RouteChannels4_c58630`.
Identity: descriptive; Own base constructor00c591f0 vtable012b8f38 and destructor00c57ef0 resetting original virtual-object base012b5868. Base table has destructor0/Reset1/Clear2/MixShortGain3/MixIntGain4/MixInt5/MixFloatGain6/MixFloatQ15Gain7/MixFloat8/Route9/Output10; adjacent012b8f64 is another class destructor, not slot11. Original base typed six planar int buffers+4..18, OutputShift1c, ChannelCount20 and unaligned raw storage24..C33; ctor rounds storage upward to16-byte alignment and sets six512-byte channel pointers. Actual filtered constructor00c3efa0 calls base and constructs six380-byte filters atC34; inactive derived suffix omitted without asserting derived size. MMX constructor00c3f660 and SSE00c3f7b0 call common filtered constructor and install012b5d40/012b5de0. Proven shared primary prefix modeled; any further MMX/SSE inheritance relationship is unproven, not recovered. All class/API/field names are descriptive projections, no library PDB identity claimed; original engine headers do not apply; own four-channel planar routing uses128 samples/512bytes per source plane, outputs mono4x quartergain/stereo alternating2x halfgain/otherwise four independent planes. Each virtualcall re-reads buffer pointers and the vtable, branch selection uses initial ChannelCount; float gains atslot6/plain8 or Q15int gains atslot4/plain5. Method source identity unknown; old DecodeFloatChannels label has no independent identity evidence.
Layout: original virtual-object/buffer hierarchy with six channel pointers and complete proven typed/raw-storage base. Inactive filtered suffix and original names explicitly unproven.

## 00c5c070: CUnprovenMMXAudioBuffer_00C3F660::MixShortGain
Original manifest label: `_gapscan / sub_00c5c070`.
Identity: descriptive; Own base constructor00c591f0 vtable012b8f38 and destructor00c57ef0 resetting original virtual-object base012b5868. Base table has destructor0/Reset1/Clear2/MixShortGain3/MixIntGain4/MixInt5/MixFloatGain6/MixFloatQ15Gain7/MixFloat8/Route9/Output10; adjacent012b8f64 is another class destructor, not slot11. Original base typed six planar int buffers+4..18, OutputShift1c, ChannelCount20 and unaligned raw storage24..C33; ctor rounds storage upward to16-byte alignment and sets six512-byte channel pointers. Actual filtered constructor00c3efa0 calls base and constructs six380-byte filters atC34; inactive derived suffix omitted without asserting derived size. MMX constructor00c3f660 and SSE00c3f7b0 call common filtered constructor and install012b5d40/012b5de0. Proven shared primary prefix modeled; any further MMX/SSE inheritance relationship is unproven, not recovered. All class/API/field names are descriptive projections, no library PDB identity claimed; original engine headers do not apply; own target virtualslot3 forwards source, fixed16 blocks,destination,gain to genuine cdecl MMX00c5be20/SSE00c5d150 helper. Helper reads input16-bit samples, multiplies high halves and accumulates32-bit destination, no this state.
Layout: original virtual buffer hierarchy and typed planar storage retained. Exact source/helper original names unknown and explicitly descriptive; actual target helper addresses/prototype effect recorded.

## 00c5d510: CUnprovenSSEAudioBuffer_00C3F7B0::MixShortGain
Original manifest label: `_gapscan / sub_00c5d510`.
Identity: descriptive; Own base constructor00c591f0 vtable012b8f38 and destructor00c57ef0 resetting original virtual-object base012b5868. Base table has destructor0/Reset1/Clear2/MixShortGain3/MixIntGain4/MixInt5/MixFloatGain6/MixFloatQ15Gain7/MixFloat8/Route9/Output10; adjacent012b8f64 is another class destructor, not slot11. Original base typed six planar int buffers+4..18, OutputShift1c, ChannelCount20 and unaligned raw storage24..C33; ctor rounds storage upward to16-byte alignment and sets six512-byte channel pointers. Actual filtered constructor00c3efa0 calls base and constructs six380-byte filters atC34; inactive derived suffix omitted without asserting derived size. MMX constructor00c3f660 and SSE00c3f7b0 call common filtered constructor and install012b5d40/012b5de0. Proven shared primary prefix modeled; any further MMX/SSE inheritance relationship is unproven, not recovered. All class/API/field names are descriptive projections, no library PDB identity claimed; original engine headers do not apply; own target virtualslot3 forwards source, fixed16 blocks,destination,gain to genuine cdecl MMX00c5be20/SSE00c5d150 helper. Helper reads input16-bit samples, multiplies high halves and accumulates32-bit destination, no this state.
Layout: original virtual buffer hierarchy and typed planar storage retained. Exact source/helper original names unknown and explicitly descriptive; actual target helper addresses/prototype effect recorded.

## 00c5c090: _gapscan::sub_00c5c090
Original manifest label: `_gapscan / sub_00c5c090`.

## 00c5d530: _gapscan::sub_00c5d530
Original manifest label: `_gapscan / sub_00c5d530`.

## 00c6e410: _gapscan::sub_00c6e410
Original manifest label: `_gapscan / sub_00c6e410`.

## 00c6f990: _gapscan::sub_00c6f990
Original manifest label: `_gapscan / sub_00c6f990`.

## 00c70480: CVtable_012bedcc::Process
Original manifest label: `_gapscan / sub_00c70480`.
Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Own Process slot2 copies128 short samples, locks section, fades out old voices/swaps/fades in new ones on crossfade, otherwise mixes current voices; fadeOut00c702e0/fadeIn00c70240/steady00c70390/copy00c70040. Copy helper catalog CPreTransposedBoneMatrix::Set is a propagated placeholder: own caller passes adjacent short audio history blocks and helper copies exactly128bytes. Distinct descriptive copy names prevent binding one symbol to two targets; no original copy identifier claimed.
Layout: actual root/base40, derived short history/scratch member and two typed voice pairs; complete active prefix, no artificial nonpolymorphic base.

## 00c70e50: CVtable_012bede0::Process
Original manifest label: `_gapscan / sub_00c70e50`.
Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Own Process slot2 copies128 short samples, locks section, fades out old voices/swaps/fades in new ones on crossfade, otherwise mixes current voices; fadeOut00c70cb0/fadeIn00c70c10/steady00c70d60/copy00c70a60. Copy helper catalog CPreTransposedBoneMatrix::Set is a propagated placeholder: own caller passes adjacent short audio history blocks and helper copies exactly128bytes. Distinct descriptive copy names prevent binding one symbol to two targets; no original copy identifier claimed.
Layout: actual root/base40, derived short history/scratch member and two typed voice pairs; complete active prefix, no artificial nonpolymorphic base.

## 00c705b0: CVtable_012bedcc::SetupVoice
Original manifest label: `_global / Audio_ProcessChannelVolume`.
Retail-asm: 0x00c705b0 uses three bare fistp dword conversions after float stores; ordinary C++ casts call __ftol2 and truncate rather than obey the current x87 rounding mode. Only that original conversion is preserved, C++ reconstructs all surrounding arithmetic/branches.
Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Global0143bc2c installed at00c4d3af after allocating1d8 and calling ctor00c3e520. True ctor chain00c3e520->00c4d000 initializes base012b5b80 and four20byte pool objects at4/18/2c/40; pool ctor00c3c8a0 and full dtor00c3caa0 show true commonroot012b5868. Full base dtor00c3e2a0 resets root at00c3e308; master dtor00c3f380 chains base. Own ctor zeros TapTable1b8 and outputChannels1bc; inactive derived fields54..1b7 and1c0..1d7 explicitly unproven. Descriptive master name retained from existing library models, no original PDB identity claimed. SetupVoice converts gains with original bare FISTP32, clamps unsigned results32767, splits impulse Delay word50 into quotient and low3 bits, prepares short coefficients through 00c78970. Original helper descriptive catalog spelling retained.
Layout: true root/mixer and master ancestry, typed short coefficients and impulse25shorts+ushortDelay.

## 00c70f80: CVtable_012bede0::SetupVoice
Original manifest label: `_global / Audio_ProcessChannelVolume_Alt`.
Retail-asm: 0x00c70f80 uses three bare fistp dword conversions after float stores; ordinary C++ casts call __ftol2 and truncate rather than obey the current x87 rounding mode. Only that original conversion is preserved, C++ reconstructs all surrounding arithmetic/branches.
Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Global0143bc2c installed at00c4d3af after allocating1d8 and calling ctor00c3e520. True ctor chain00c3e520->00c4d000 initializes base012b5b80 and four20byte pool objects at4/18/2c/40; pool ctor00c3c8a0 and full dtor00c3caa0 show true commonroot012b5868. Full base dtor00c3e2a0 resets root at00c3e308; master dtor00c3f380 chains base. Own ctor zeros TapTable1b8 and outputChannels1bc; inactive derived fields54..1b7 and1c0..1d7 explicitly unproven. Descriptive master name retained from existing library models, no original PDB identity claimed. SetupVoice converts gains with original bare FISTP32, clamps unsigned results32767, splits impulse Delay word50 into quotient and low7 bits, prepares short coefficients through 00c7ba00. Original helper descriptive catalog spelling retained.
Layout: true root/mixer and master ancestry, typed short coefficients and impulse25shorts+ushortDelay.

## 00c70840: CVtable_012bedcc::SetParameters
Original manifest label: `_gapscan / sub_00c70840`.
Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Target compares volume/gains, calls UpdateVoices00c70750 only when one changes.
Layout: true root/mixer base40 and lock member; derived history/scratch member and typed voice records, float gains; no active engine header.

## 00c71210: CVtable_012bede0::SetParameters
Original manifest label: `_gapscan / sub_00c71210`.
Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Target compares volume/gains, calls UpdateVoices00c71120 only when one changes.
Layout: true root/mixer base40 and lock member; derived history/scratch member and typed voice records, float gains; no active engine header.

## 00c708a0: CVtable_012bedcc::Update
Original manifest label: `_gapscan / sub_00c708a0`.
Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Global0143bc2c installed at00c4d3af after allocating1d8 and calling ctor00c3e520. True ctor chain00c3e520->00c4d000 initializes base012b5b80 and four20byte pool objects at4/18/2c/40; pool ctor00c3c8a0 and full dtor00c3caa0 show true commonroot012b5868. Full base dtor00c3e2a0 resets root at00c3e308; master dtor00c3f380 chains base. Own ctor zeros TapTable1b8 and outputChannels1bc; inactive derived fields54..1b7 and1c0..1d7 explicitly unproven. Descriptive master name retained from existing library models, no original PDB identity claimed. Own Update slot4 resolves impulse pair via00c400d0 then computes angular front/rear gains and conditionally calls UpdateVoices00c70750. Helper Animation_ComputeFrameOffset is an existing descriptive placeholder for tap-pair lookup, not recovered original animation API. Input arguments x,y,blend,volume,gains,crossfade,force.
Layout: audited real mixer/master ancestry, typed short history and voice records, member taps and front/rear gains; original library names unproven.

## 00c71270: CVtable_012bede0::Update
Original manifest label: `_gapscan / sub_00c71270`.
Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Global0143bc2c installed at00c4d3af after allocating1d8 and calling ctor00c3e520. True ctor chain00c3e520->00c4d000 initializes base012b5b80 and four20byte pool objects at4/18/2c/40; pool ctor00c3c8a0 and full dtor00c3caa0 show true commonroot012b5868. Full base dtor00c3e2a0 resets root at00c3e308; master dtor00c3f380 chains base. Own ctor zeros TapTable1b8 and outputChannels1bc; inactive derived fields54..1b7 and1c0..1d7 explicitly unproven. Descriptive master name retained from existing library models, no original PDB identity claimed. Own Update slot4 resolves impulse pair via00c400d0 then computes angular front/rear gains and conditionally calls UpdateVoices00c71120. Helper Animation_ComputeFrameOffset is an existing descriptive placeholder for tap-pair lookup, not recovered original animation API. Input arguments x,y,blend,volume,gains,crossfade,force.
Layout: audited real mixer/master ancestry, typed short history and voice records, member taps and front/rear gains; original library names unproven.

## 00c71e10: CAudioSource::EAXSet
Original manifest label: `_gapscan / sub_00c71e10`.
Identity: Descriptive library CAudioSource method at own vtable012bee40 slot15; identical inherited slots in012b8cc8/012b8d48. Actual constructor00c72600 calls00c59ba0 then installs012bee40. Base00c59ba0 calls CPooledNode ctor; base destructor00c3f3d0 chains node00c4cf40 which resets commonroot012b5868. OpenAL source ID generated by00c72930 into+58. Resolver00c4d740, own EAXSet literal, distinct cache0143bea0.
Layout: true root/node/source-base/source hierarchy. Base typed observer0c,64bit clock words10/18/20,Flags28/State2c/FadeGain30,unproven inactive34..4f,Context50; default8-byte alignment gives base58. Base Initialize00c596e0 writes Context50, clock helpers00c59770/00c597b0 use signed64-bit compares/copies. SourceId58 passed to OpenAL. Partial source prefix only, no full source size claimed. CAudioContext is an API-only projection; original library names remain unproven.

## 00c71e90: CAudioSource::EAXGet
Original manifest label: `_gapscan / sub_00c71e90`.
Identity: Descriptive library CAudioSource method at own vtable012bee40 slot16; identical inherited slots in012b8cc8/012b8d48. Actual constructor00c72600 calls00c59ba0 then installs012bee40. Base00c59ba0 calls CPooledNode ctor; base destructor00c3f3d0 chains node00c4cf40 which resets commonroot012b5868. OpenAL source ID generated by00c72930 into+58. Resolver00c4d740, own EAXGet literal, distinct cache0143bea4.
Layout: true root/node/source-base/source hierarchy. Base typed observer0c,64bit clock words10/18/20,Flags28/State2c/FadeGain30,unproven inactive34..4f,Context50; default8-byte alignment gives base58. Base Initialize00c596e0 writes Context50, clock helpers00c59770/00c597b0 use signed64-bit compares/copies. SourceId58 passed to OpenAL. Partial source prefix only, no full source size claimed. CAudioContext is an API-only projection; original library names remain unproven.

## 00ca1000: vorbis::res0_inverse
Original manifest label: `_gapscan / sub_00ca1000`.
Identity: Xiph Vorbis1.0.1 res0.c original res0_inverse packs active channels then _01inverse with vorbis_book_decodevs_add. Own helper00ca0df0 matches setup/partition decode algorithm and returns20 stack bytes; callback00c96450 receives codebook/float in ecx/edx with remaining two stack args. Own wrapper uses normal fastcall. No owner layout accessed.

## 00ca12b0: vorbis::res1_inverse
Original manifest label: `_gapscan / sub_00ca12b0`.
Identity: Xiph Vorbis1.0.1 res0.c original res1_inverse packs active channels then _01inverse with vorbis_book_decodev_add. Own helper00ca0df0 matches setup/partition decode algorithm and returns20 stack bytes; callback00c965c0 receives codebook/float in ecx/edx with remaining two stack args. Own wrapper uses normal fastcall. No owner layout accessed.

## 00ca7a80: CEnumPins::QueryInterface
Original manifest label: `_gapscan / sub_00ca7a80`.
Identity: proven. DirectShow amfilter CEnumPins::QueryInterface; own first GUID at012a9af4 =56a86892-0ad4-11ce-b03a-0020af0ba770(IEnumPins), second012c06c4 exact IID_IUnknown. Own caller/table and established clone/ctor confirm enumeration interface. Correct GetInterface00cacbe0 adds reference and returns same primary interface.
Layout: original IUnknown/enum inheritance and full enumerator fields; CEnumPins carries real CGenericList/CBaseList cache24. No applicable engine header.

## 00ca7bb0: CEnumMediaTypes::QueryInterface
Original manifest label: `_gapscan / sub_00ca7bb0`.
Identity: proven. DirectShow amfilter CEnumMediaTypes::QueryInterface; own first GUID at012a9ae4 =89c31040-846b-11ce-97d3-00aa0055595a(IEnumMediaTypes), second012c06c4 exact IID_IUnknown. Own caller/table and established clone/ctor confirm enumeration interface. Correct GetInterface00cacbe0 adds reference and returns same primary interface.
Layout: original IUnknown/enum inheritance and full enumerator fields; CEnumPins carries real CGenericList/CBaseList cache24. No applicable engine header.

## 00cabed0: CEnumPins::Clone
Original manifest label: `_gapscan / sub_00cabed0`.
Identity: DirectShow amfilter CEnumPins::Clone, same own enumeration vtable/constructor as QueryInterface. Scalar allocation48 bytes via00bfea1a; ctor00cabe00; GetPinVersion primary slot5. Standard E_POINTER/VFW_E_ENUM_OUT_OF_SYNC/E_OUTOFMEMORY and owned cloned enumerator.
Layout: complete enumeration fields, DirectShow real COM/base inheritance and primary owner virtual prefix. Unused owner fields and secondary interface methods omitted, no complete owner size claimed. CBaseObject is the empty DirectShow release type, not the unrelated engine class of the same spelling.

## 00cabfd0: CEnumMediaTypes::Clone
Original manifest label: `_gapscan / sub_00cabfd0`.
Identity: DirectShow amfilter CEnumMediaTypes::Clone, same own enumeration vtable/constructor as QueryInterface. Scalar allocation20 bytes via00bfea1a; ctor00cabf70; GetMediaTypeVersion primary slot4. Standard E_POINTER/VFW_E_ENUM_OUT_OF_SYNC/E_OUTOFMEMORY and owned cloned enumerator.
Layout: complete enumeration fields, DirectShow real COM/base inheritance and primary owner virtual prefix. Unused owner fields and secondary interface methods omitted, no complete owner size claimed. CBaseObject is the empty DirectShow release type, not the unrelated engine class of the same spelling.

## 00cad8f0: CPosPassThru::put_CurrentPosition
Original manifest label: `_gapscan / sub_00cad8f0`.
Identity: proven. Original DirectShow ctlutil CPosPassThru method in pass-through tables012bf8f8/012bf940 and derived renderer tables; exact COM interface slot and own GetPeer00cad400 or GetPeerSeeking00cad460 establish identity. Actual IMediaSeeking/CMediaPosition/CUnknown/INonDelegatingUnknown hierarchy explains secondary-this adjustment -4. Real noinline peer getter queries ConnectedTo and QueryInterface, releases connected pin, then forwards or returns E_NOTIMPL. Forwarded double or LONG output pointer and Release ordering exactly follow original interface.
Layout: original release-mode hierarchy, seeking0/media-position4/unknown8, pin24; no applicable engine header.

## 00cada00: CPosPassThru::put_PrerollTime
Original manifest label: `_gapscan / sub_00cada00`.
Identity: proven. Original DirectShow ctlutil CPosPassThru method in pass-through tables012bf8f8/012bf940 and derived renderer tables; exact COM interface slot and own GetPeer00cad400 or GetPeerSeeking00cad460 establish identity. Actual IMediaSeeking/CMediaPosition/CUnknown/INonDelegatingUnknown hierarchy explains secondary-this adjustment -4. Real noinline peer getter queries ConnectedTo and QueryInterface, releases connected pin, then forwards or returns E_NOTIMPL. Forwarded double or LONG output pointer and Release ordering exactly follow original interface.
Layout: original release-mode hierarchy, seeking0/media-position4/unknown8, pin24; no applicable engine header.

## 00cadae0: CPosPassThru::CanSeekForward
Original manifest label: `_gapscan / sub_00cadae0`.
Identity: proven. Original DirectShow ctlutil CPosPassThru method in pass-through tables012bf8f8/012bf940 and derived renderer tables; exact COM interface slot and own GetPeer00cad400 or GetPeerSeeking00cad460 establish identity. Actual IMediaSeeking/CMediaPosition/CUnknown/INonDelegatingUnknown hierarchy explains secondary-this adjustment -4. Real noinline peer getter queries ConnectedTo and QueryInterface, releases connected pin, then forwards or returns E_NOTIMPL. Forwarded double or LONG output pointer and Release ordering exactly follow original interface.
Layout: original release-mode hierarchy, seeking0/media-position4/unknown8, pin24; no applicable engine header.

## 00cadb50: CPosPassThru::CanSeekBackward
Original manifest label: `_gapscan / sub_00cadb50`.
Identity: proven. Original DirectShow ctlutil CPosPassThru method in pass-through tables012bf8f8/012bf940 and derived renderer tables; exact COM interface slot and own GetPeer00cad400 or GetPeerSeeking00cad460 establish identity. Actual IMediaSeeking/CMediaPosition/CUnknown/INonDelegatingUnknown hierarchy explains secondary-this adjustment -4. Real noinline peer getter queries ConnectedTo and QueryInterface, releases connected pin, then forwards or returns E_NOTIMPL. Forwarded double or LONG output pointer and Release ordering exactly follow original interface.
Layout: original release-mode hierarchy, seeking0/media-position4/unknown8, pin24; no applicable engine header.

## 00caf0e0: CPosPassThru::GetStopPosition
Original manifest label: `_gapscan / sub_00caf0e0`.
Identity: proven. Original DirectShow ctlutil CPosPassThru method in pass-through tables012bf8f8/012bf940 and derived renderer tables; exact COM interface slot and own GetPeer00cad400 or GetPeerSeeking00cad460 establish identity. Actual IMediaSeeking/CMediaPosition/CUnknown/INonDelegatingUnknown hierarchy explains secondary-this adjustment -4. Real noinline peer getter queries ConnectedTo and QueryInterface, releases connected pin, then forwards or returns E_NOTIMPL. Original GetSeekingLongLong member-pointer dispatch uses generated vcall thunk00caef60(slot11) or00caef40(slot10); do not replace with direct virtual-call source.
Layout: original release-mode hierarchy, seeking0/media-position4/unknown8, pin24; no applicable engine header.

## 00caf120: CPosPassThru::GetDuration
Original manifest label: `_gapscan / sub_00caf120`.
Identity: proven. Original DirectShow ctlutil CPosPassThru method in pass-through tables012bf8f8/012bf940 and derived renderer tables; exact COM interface slot and own GetPeer00cad400 or GetPeerSeeking00cad460 establish identity. Actual IMediaSeeking/CMediaPosition/CUnknown/INonDelegatingUnknown hierarchy explains secondary-this adjustment -4. Real noinline peer getter queries ConnectedTo and QueryInterface, releases connected pin, then forwards or returns E_NOTIMPL. Original GetSeekingLongLong member-pointer dispatch uses generated vcall thunk00caef60(slot11) or00caef40(slot10); do not replace with direct virtual-call source.
Layout: original release-mode hierarchy, seeking0/media-position4/unknown8, pin24; no applicable engine header.

## 00d16920: NScript::CQ_BountyHuntScript::WatchForHostage1Killed
Original manifest label: `_gapscan / sub_00d16920`.
Identity: Own CBH_Hostage1 Init builds spawned member callback00d16865 with literal WatchForHostage1Killed and ParentClass., passes parent+20; original PDB CQ_BountyHuntScript public method(CScriptThing by value). Real flag84 reconciles typed script base. CScriptThing MsgIsKilledBy const slot18, Game NewScriptFrame const slot7, CScriptBase IsActiveThreadTerminating00cb7940.
Layout: real typed script/level/container/handle ancestry; parent Game64 and flags84/85. Original CScriptThing vtable and counted-pointer destructor/copy semantics, no synthetic dispatch. Unused virtual slots descriptive. Shared headers aliased for real hierarchy.

## 00d16d00: NScript::CQ_BountyHuntScript::WatchForHostage2Killed
Original manifest label: `_gapscan / sub_00d16d00`.
Identity: Own CBH_Hostage2 Init builds spawned member callback00d16c85 with literal WatchForHostage2Killed and ParentClass., passes parent+20; original PDB CQ_BountyHuntScript public method(CScriptThing by value). Real flag85 reconciles typed script base. CScriptThing MsgIsKilledBy const slot18, Game NewScriptFrame const slot7, CScriptBase IsActiveThreadTerminating00cb7940.
Layout: real typed script/level/container/handle ancestry; parent Game64 and flags84/85. Original CScriptThing vtable and counted-pointer destructor/copy semantics, no synthetic dispatch. Unused virtual slots descriptive. Shared headers aliased for real hierarchy.

## 00d1cfc0: NScript::CQ_BreakSiegeScript::OnPersist
Original manifest label: `_gapscan / sub_00d1cfc0`.
Identity: Original PDB OnPersist and unique own persist literals; own vtable0x12cab18 slot4, constructor stores at0xd1cc28,0xd23ca7. All12 own strings and typed original owner fields mapped in order. Transfer<bool>004045c0, Transfer<long>00410be0.
Layout: real typed CScriptBase64/CLevelScriptBase72 with original fields and STLport containers. Shared handle headers aliased for real polymorphic hierarchy. CPersistContext API-only projection, no storage used.

## 00e57d20: NScript::CV_BeggarAndChildScript::OnPersist
Original manifest label: `_gapscan / sub_00e57d20`.
Identity: Original PDB OnPersist and unique own persist literals; own vtable0x12e65e0 slot4, constructor stores at0xe57bc8,0xe5d0d6. All12 own strings and typed original owner fields mapped in order. Transfer<bool>004045c0, Transfer<long>00410be0.
Layout: real typed CScriptBase64/CLevelScriptBase72 with original fields and STLport containers. Shared handle headers aliased for real polymorphic hierarchy. CPersistContext API-only projection, no storage used.

## 00d41ca0: NScript::CQ_GuildTrainingScript::CSkillTarget::Init
Original manifest label: `_gapscan / sub_00d41ca0`.
Identity: Own named entity registration00d3c358 ->factory00d41bf0 ->vtable012ce0cc slot2; original PDB private virtual Init signature and named fields. CScriptThing GetDataString const slot3; GFCharStringToInt fastcall0099e7f0; CCharString destructor0099eae0.
Layout: real CEntityScriptBase20 with Game4 and CScriptThing Me8; ParentClass20, PMasterData24; original field order. CCharString is a four-byte pointer to CCharStringData, not a CBasicString base. Inactive virtual slots are descriptive projections. Shared handle header aliased to restore real polymorphic inheritance.

## 00d8f4a0: NScript::CQ_HobbeCaveScript::CHobbeTorrent::Init
Original manifest label: `_gapscan / sub_00d8f4a0`.
Identity: Own named entity registration00d8dee2 ->factory00d8f3f0 ->vtable012d5960 slot2; original PDB private virtual Init signature and named fields. CScriptThing GetDataString const slot3; GFCharStringToInt fastcall0099e7f0; CCharString destructor0099eae0.
Layout: real CEntityScriptBase20 with Game4 and CScriptThing Me8; ParentClass20, PMasterData24; original field order. CCharString is a four-byte pointer to CCharStringData, not a CBasicString base. Inactive virtual slots are descriptive projections. Shared handle header aliased to restore real polymorphic inheritance.

## 00d42ff0: NScript::CQ_GuildTrainingScript::CBirdKiller::Init
Original manifest label: `_gapscan / sub_00d42ff0`.
Identity: Own named registration00d3c42c ->factory00d42f40 ->vtable012ce310 slot2; PDB original nested private virtual Init/field names. Game vtable01260f0c slot477 points0088e5a0, original const SetThingPersistent(const CScriptThing&,bool), corroborated PDB name set.
Layout: original CEntityScriptBase20, Game4/Me8, ParentClass20/PMasterData24 and named fields after28; actual script handle inheritance. Inactive virtual slots are descriptive projections.

## 00ec8210: NScript::CV_SickChildScript::CManInLove::Init
Original manifest label: `_gapscan / sub_00ec8210`.
Identity: Own named registration00ec56bc ->factory00ec8160 ->vtable012f1a94 slot2; PDB original nested private virtual Init/field names. Game vtable01260f0c slot477 points0088e5a0, original const SetThingPersistent(const CScriptThing&,bool), corroborated PDB name set.
Layout: original CEntityScriptBase20, Game4/Me8, ParentClass20/PMasterData24 and named fields after28; actual script handle inheritance. Inactive virtual slots are descriptive projections.

## 00d63d80: NScript::CQ_GuildTrainingWoodsDepartureScript::CScorpionHome::Init
Original manifest label: `_gapscan / sub_00d63d80`.
Identity: Own named ScorpionHome registration00d61ee2 ->factory00d63cc0 ->vtable012d19c0 slot2; original PDB nested private virtual Init, named ScorpionsLeft/FlourishHint and CScriptDef GUI_DepartureBeetles; global0143e90c and typed retail field+3852.
Layout: original CEntityScriptBase20, parent/master20/24, count28, hint32, full child36. Typed definition prefix through GUI_MeleeBeetles; original def base chain with packed parent57 and STLport12-byte vectors reconciles donor offsets. No opaque base prefix; inactive virtual slots are descriptive.

## 00d66c60: NScript::CQ_GuildTrainingWoodsMeleeScript::CScorpionHome::Init
Original manifest label: `_gapscan / sub_00d66c60`.
Identity: Own named ScorpionHome registration00d6670c ->factory00d66ba0 ->vtable012d1da0 slot2; original PDB nested private virtual Init, named ScorpionsLeft/FlourishHint and CScriptDef GUI_MeleeBeetles; global0143e90c and typed retail field+3856.
Layout: original CEntityScriptBase20, parent/master20/24, count28, hint32, full child36. Typed definition prefix through GUI_MeleeBeetles; original def base chain with packed parent57 and STLport12-byte vectors reconciles donor offsets. No opaque base prefix; inactive virtual slots are descriptive.

## 00d64080: NScript::CQ_GuildTrainingWoodsDepartureScript::TeleportOutHero
Original manifest label: `_global / Quest_GuildWoods_Teleport_Exit_First`.
Identity: Own DoMission callback registration00d63f41 stores this address with literal TeleportOutHero at012d19dc and parent pointer. PDB nameset independently confirms member TeleportOutHero; typed CScriptBase/CLevelScriptBase hierarchy yields Game64. Game slots70 GetHero,72 GetThingWithScriptName,264 GetHealth,472 EntityTeleportToThing,376 Pause,364 AddNewConversation,366 AddLineToConversation,263 ChangeHeroHealthBy resolved from own01260f0c vtable; health threshold0125a2a0 is6.0f.
Layout: actual typed script base and level base, inherited Game64; owned trailing quest state untouched. Real CScriptThing polymorphic handle/counting model and shared header evidence retained. Unused virtual slots descriptive.

## 00d66f50: NScript::CQ_GuildTrainingWoodsMeleeScript::TeleportOutHero
Original manifest label: `_global / Quest_GuildWoods_Teleport_Exit_Additional`.
Identity: Own DoMission callback registration00d66e21 stores this address with literal TeleportOutHero at012d19dc and parent pointer. PDB nameset independently confirms member TeleportOutHero; typed CScriptBase/CLevelScriptBase hierarchy yields Game64. Game slots70 GetHero,72 GetThingWithScriptName,264 GetHealth,472 EntityTeleportToThing,376 Pause,364 AddNewConversation,366 AddLineToConversation,263 ChangeHeroHealthBy resolved from own01260f0c vtable; health threshold0125a2a0 is6.0f.
Layout: actual typed script base and level base, inherited Game64; owned trailing quest state untouched. Real CScriptThing polymorphic handle/counting model and shared header evidence retained. Unused virtual slots descriptive.

## 00d7d8d0: NScript::CQ_HeroSoulsArenaScript::WatchArenaCellsToTurnOffVillage
Original manifest label: `_global / Script_Arena_Cells_Wait`.
Identity: Own Main callback registration00d7cfba stores this function with exact method literal WatchArenaCellsToTurnOffVillage and parent receiver; independent PDB nameset agrees. Own Game vtable01260f0c slot12 IsRegionLoaded,72 GetThingWithScriptName,440 SetVillageLimbo/447 SetThingAsUsable; real script base Game64. Counted handle CScriptThing lifetime and assigning returned handle explicit original copy semantics; late cancellation exits call destructor004aa840.
Layout: actual typed script ancestry and original counted handle model; inherited Game64; inactive owned trailing quest state unaccessed. Shared headers retained as evidence, unused virtual slots descriptive.

## 00d8f040: NScript::CQ_HobbeCaveScript::WatchForGrannysRegion
Original manifest label: `_global / Script_Grannys_House`.
Identity: Own Main callback registration00d8e61c stores this function with exact method literal WatchForGrannysRegion and parent receiver; independent PDB nameset agrees. Own Game vtable01260f0c slot12 IsRegionLoaded,72 GetThingWithScriptName,440 SetVillageLimbo/447 SetThingAsUsable; real script base Game64. Counted handle CScriptThing lifetime and assigning returned handle explicit original copy semantics; late cancellation exits call destructor004aa840.
Layout: actual typed script ancestry and original counted handle model; inherited Game64; inactive owned trailing quest state unaccessed. Shared headers retained as evidence, unused virtual slots descriptive.

## 00d9b8a0: NScript::CQ_HobbeToothContestScript::CQ_HobbeToothHobbe::OnPredicateFail
Original manifest label: `_gapscan / sub_00d9b8a0`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own literalbinding00d9a2fc ->factory00d9b7c0 installs exacttable012d6834 targetslot5. Own helper00d9b8f0 increments Hero count+4c, helper00d9bb50 increments Whisper count+50, reconciling original PDB fieldnames and exactpublicQAE HeroKilledHobbe()/WhisperKilledHobbe(). ScriptThing const MsgIsKilledBy charref atslot18 confirmed ownRTTI1238c8c target4aaaf0 and bothnamesets; string exactlyQ_HobbeToothWhisper. Original bool result survives destruction of the implicit CCharString temporary before parent dispatch.
Layout: complete typed original parent104/entity28 and original level/entity bases; shared headers and genuine STLport storage. No copied labels or invented call target identities.

## 00d9bf10: NScript::CQ_HobbeToothContestScript::CDrinkingHobbe::OnPredicateFail
Original manifest label: `_gapscan / sub_00d9bf10`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own literalbinding00d9a42f ->factory00d9be30 installs exacttable012d6b10 targetslot5. Own helper00d9b8f0 increments Hero count+4c, helper00d9bb50 increments Whisper count+50, reconciling original PDB fieldnames and exactpublicQAE HeroKilledHobbe()/WhisperKilledHobbe(). ScriptThing const MsgIsKilledBy charref atslot18 confirmed ownRTTI1238c8c target4aaaf0 and bothnamesets; string exactlyQ_HobbeToothWhisper. Original bool result survives destruction of the implicit CCharString temporary before parent dispatch.
Layout: complete typed original parent104/entity28 and original level/entity bases; shared headers and genuine STLport storage. No copied labels or invented call target identities.

## 00da4c30: NScript::CQ_MinionClifftopChaseScript::CBalverineFightingGuard::Init
Original manifest label: `_gapscan / sub_00da4c30`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own binding00da368f literalBalverineFightingGuard ->factory00da4b80 installs exacttable012d7490 targetslot2, privatevirtualInit signature matches exact PDB nameset. Original own fields bool ExplainedAboutSummoning;long HealthBarID; reconcile own 36-byte allocation.
Layout: original entity/base/typed members; parent uses correct original level-script prefix, unused own parent suffix omitted without sizeclaim. No inactive map/tree baggage.

## 00da5d40: NScript::CQ_MinionClifftopChaseScript::CCorneredGuard::Init
Original manifest label: `_gapscan / sub_00da5d40`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own binding00da38fb literalCorneredGuard ->factory00da5c90 installs exacttable012d7580 targetslot2, privatevirtualInit signature matches exact PDB nameset. Original own fields bool AskedForHerosHelp;long HealthBarID; reconcile own 36-byte allocation.
Layout: original entity/base/typed members; parent uses correct original level-script prefix, unused own parent suffix omitted without sizeclaim. No inactive map/tree baggage.

## 00da5bf0: NScript::CQ_MinionClifftopChaseScript::CMinionFieldGenerator::Init
Original manifest label: `_gapscan / sub_00da5bf0`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own binding00da3891 literalMinionFieldGenerator ->factory00da5b40 installs exacttable012d7564 targetslot2, privatevirtualInit signature matches exact PDB nameset. Original own fields bool DoneIntro; reconcile own 32-byte allocation.
Layout: original entity/base/typed members; parent uses correct original level-script prefix, unused own parent suffix omitted without sizeclaim. No inactive map/tree baggage.

## 00e38680: NScript::CV_AssassinAttacksScript::CChestHidingAssassin::Init
Original manifest label: `_gapscan / sub_00e38680`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own binding00e378e6 literalChestHidingAssassin ->factory00e385d0 installs exacttable012e3704 targetslot2, privatevirtualInit signature matches exact PDB nameset. Original own fields bool Screamed; reconcile own 32-byte allocation.
Layout: original entity/base/typed members; parent uses correct original level-script prefix, unused own parent suffix omitted without sizeclaim. No inactive map/tree baggage.

## 00dc3ea0: NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::scalar deleting destructor
Original manifest label: `_gapscan / sub_00dc3ea0`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own exactvtable012da494 slot0 and constructor00dc3df0 define original named owner; original PDB destructor and complete typed fields. Nested skeleton array skeleton_object[4]+32,returned[4]+28,collected_all48,52bytes. Compiler scalar deleting destructor destroys actual CCharString array through EH-vector destructor00401400 then original CEntityScriptBase00f35b40 destructor, optionally scalar delete00bfe9bc. Original implicit owner destructor and genuine own constructor body let compiler emit complete member cleanup; no hand-written flags wrapper.
Layout: complete original hierarchy/typed fields, protected STLport storage/base proxies and shared handle layout evidence.

## 00dcc000: NScript::CQ_OpeningGraveyardSecretPassageScript::scalar deleting destructor
Original manifest label: `_gapscan / sub_00dcc000`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own exactvtable012da11c slot0 and constructor00dc2570 define original named owner; original PDB destructor and complete typed fields. Root hero_object[4]+88, all original own bool/long fields to116bytes; donor136 minus20base delta. Compiler scalar deleting destructor destroys actual CCharString array through EH-vector destructor00401400 then original CScriptBase00cbd510 destructor, optionally scalar delete00bfe9bc. Original implicit owner destructor and genuine own constructor body let compiler emit complete member cleanup; no hand-written flags wrapper.
Layout: complete original hierarchy/typed fields, protected STLport storage/base proxies and shared handle layout evidence.

## 00e19390: NScript::CQ_WhiteBalverineWWScript::CKG_Chief::Init
Original manifest label: `_gapscan / sub_00e19390`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own binding00e18681 literalKG_Chief ->factory00e192e0 installs exacttable012e0e68 targetslot2, privatevirtualInit signature matches exact PDB nameset. Factory32-byte instance and ownboolInitialChatted identify WhiteBalverineWW::CKG_Chief, distinct from28-byteKnotholeGlade sibling.
Layout: original entity/base/typed members; parent uses correct original level-script prefix, unused own parent suffix omitted without sizeclaim. No inactive map/tree baggage.

## 00ec6860: NScript::CV_SickChildScript::CSickChildsSister::Init
Original manifest label: `_gapscan / sub_00ec6860`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own binding00ec55f4 literalSickChildsSister ->factory00ec67b0 installs exacttable012f1964 targetslot2, privatevirtualInit signature matches exact PDB nameset. Original own fields bool LeadHeroToSickChild; reconcile own 32-byte allocation.
Layout: original entity/base/typed members; parent uses correct original level-script prefix, unused own parent suffix omitted without sizeclaim. No inactive map/tree baggage.

## 00e3b8f0: NScript::CV_BordelloScript::OnPersist
Original manifest label: `_gapscan / sub_00e3b8f0`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own script factory00e46a00 installs table012e38b0 rootOnPersist slot4; exact publicvirtual UAEX PDB nameset and full transfer literal/order/typed own offsets agree. Original donor script base is20bytes larger, own members shifted-20. Bordello prefix includes all fields throughBeersDrunk; unused suffix deliberately omitted, no totalsizeclaim.
Layout: typed original level script, complete original list/vector storage/base ancestry; context44.

## 00e9e110: NScript::CV_IntroductionToTrophiesScript::OnPersist
Original manifest label: `_gapscan / sub_00e9e110`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own script factory00e9fb40 installs table012ed558 rootOnPersist slot4; exact publicvirtual UAEX PDB nameset and full transfer literal/order/typed own offsets agree. Original donor script base is20bytes larger, own members shifted-20. Full128-byte donor/108-byte own script with nested BriarRoseOpinion enum, actual CPersistContext::Transfer<enum> at00e9e270.
Layout: typed original level script, complete original list/vector storage/base ancestry; context44.

## 00e5f190: NScript::CV_BodyGuardScript::CBodyGuard::OnPersist
Original manifest label: `_gapscan / sub_00e5f190`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own literalbinding00e5d53f ->factory00e5dd60 installs table012e701c methodslot4. Original privatevirtual OnPersist signature and full own fields match PDB; actualBrainState+32. Exact template Transfer<long>(constchar*,long&,constlong&) defaultzero at00410be0, not a pointer-valued fallback or invented persistence wrapper.
Layout: real complete typed entity and context44, original Game/Me/Parent/Master storage, level parent prefix only and omitted unused parent suffix.

## 00e8d2a0: NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::OnPersist
Original manifest label: `_gapscan / sub_00e8d2a0`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own literalbinding00e8ba0c ->factory00e8bd30 installs table012ebb14 methodslot4. Original privatevirtual OnPersist signature and full own fields match PDB; actualBrainState+28. Exact template Transfer<long>(constchar*,long&,constlong&) defaultzero at00410be0, not a pointer-valued fallback or invented persistence wrapper.
Layout: real complete typed entity and context44, original Game/Me/Parent/Master storage, level parent prefix only and omitted unused parent suffix.

## 00e69f00: NScript::CV_ChapelOfEvilScript::WatchForTermination
Original manifest label: `_gapscan / sub_00e69f00`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own watcher construction stores exact target memberfunctionat00e696ed beside constructor literal WatchForTermination at012c84b0; original publicQAEX method unique PDBowner. Original typed fields reconcile QuestFinished own108 vs donor+20. Real CScriptBase::IsActiveThreadTerminating00cb7940 (PActiveThread->terminating flag), const GameNewScriptFrame slot7, DeactivateQuestLater slot281/00892f10 with unsignedlong, classbyvalue GetActiveQuestName slot655/00891880. Parent unused suffix omitted without sizeclaim..
Layout: original typed script prefix through QuestFinished; complete true bases/storage and non-POD string.

## 00edbc80: NScript::CV_TempleOfLightScript::WatchForTermination
Original manifest label: `_gapscan / sub_00edbc80`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own watcher construction stores exact target memberfunctionat00edb421 beside constructor literal WatchForTermination at012c84b0; original publicQAEX method unique PDBowner. Original typed fields reconcile QuestFinished own112 vs donor+20. Real CScriptBase::IsActiveThreadTerminating00cb7940 (PActiveThread->terminating flag), const GameNewScriptFrame slot7, DeactivateQuestLater slot281/00892f10 with unsignedlong, classbyvalue GetActiveQuestName slot655/00891880. Parent unused suffix omitted without sizeclaim..
Layout: original typed script prefix through QuestFinished; complete true bases/storage and non-POD string.

## 00ebcd70: NScript::CV_OracleScript::Main
Original manifest label: `_gapscan / sub_00ebcd70`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own global scriptbinding S_VO/S_VPAAWB points own factory, original entityliteral/factory pairs VO_OracleMarker->00ebce80,VO_Woman->00ebe540. Own table slot2 is Main; table1 launches script Main thread. Original PDB class/method/nested Alloc namesets. Genuine CEntityScriptBinding28 two-slot abstract interface (no dtor), string4,parent8,memberfunction12,fastcall non-POD classreturn allocation16,bool20,ulongFlags24. Actual AddEntityScriptBinding00cb8230,PostAddScriptedEntities00cb8930 and constgame slot64..
Layout: original level-script72 and binding28, complete STLport base/proxy storage; unused script suffix omitted.

## 00ec12e0: NScript::CV_PicnicAreaAfterWaspBossScript::Main
Original manifest label: `_gapscan / sub_00ec12e0`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own global scriptbinding S_VO/S_VPAAWB points own factory, original entityliteral/factory pairs PAAWB_Villager->00ec13f0,PAAWB_Guard->00ec14e0. Own table slot2 is Main; table1 launches script Main thread. Original PDB class/method/nested Alloc namesets. Genuine CEntityScriptBinding28 two-slot abstract interface (no dtor), string4,parent8,memberfunction12,fastcall non-POD classreturn allocation16,bool20,ulongFlags24. Actual AddEntityScriptBinding00cb8230,PostAddScriptedEntities00cb8930 and constgame slot64..
Layout: original level-script72 and binding28, complete STLport base/proxy storage; unused script suffix omitted.

## 00ec4540: NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Init
Original manifest label: `_gapscan / sub_00ec4540`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own binding00ec3dff literalRTFE_RockTroll ->factory00ec4490 installs exacttable012f16c4 targetslot2, privatevirtualInit signature matches exact PDB nameset. Original own fields none beyond Parent/Master reconcile own 28-byte allocation.
Layout: original entity/base/typed members; parent uses correct original level-script prefix, unused own parent suffix omitted without sizeclaim. No inactive map/tree baggage.

## 00ed51e0: NScript::CV_SwordInTheStoneScript::CSwordOfStrength::Init
Original manifest label: `_gapscan / sub_00ed51e0`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own binding00ed4d76 literalSwordOfStrength ->factory00ed5130 installs exacttable012f3158 targetslot2, privatevirtualInit signature matches exact PDB nameset. Original own fields none beyond Parent/Master reconcile own 28-byte allocation.
Layout: original entity/base/typed members; parent uses correct original level-script prefix, unused own parent suffix omitted without sizeclaim. No inactive map/tree baggage.

## 00ecbd50: NScript::CV_SickChildScript::CTalkingTrader1::Init
Original manifest label: `_gapscan / sub_00ecbd50`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own literalTalkingTrader1 binding00ec57eb->factory00ecbc80->table012f216c slot2; original TimesTalkedTo28/TimesHit32 complete36. Game ownRTTI1260f0c exact slots844/235/360/477/517/526 with PDB signatures (first takes actual non-POD CScriptThing byvalue), actual CCPPointerInfo12 RefCount/fastcall-DeleteFunc/Data (ownstoreddeleter00cdee00 consumesECX) plus inline original sharing copy..
Layout: full original bases and typed members, correct protected STLport storage; no unused tree.

## 00ecca30: NScript::CV_SickChildScript::CTalkingTrader2::Init
Original manifest label: `_gapscan / sub_00ecca30`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; own literalTalkingTrader2 binding00ec5851->factory00ecc960->table012f2348 slot2; original TimesTalkedTo28/TimesHit32 complete36. Game ownRTTI1260f0c exact slots844/235/360/477/517/526 with PDB signatures (first takes actual non-POD CScriptThing byvalue), actual CCPPointerInfo12 RefCount/fastcall-DeleteFunc/Data (ownstoreddeleter00cdee00 consumesECX) plus inline original sharing copy..
Layout: full original bases and typed members, correct protected STLport storage; no unused tree.

## 00eef5d0: NScript::CTestQuestCard2Script::Main
Original manifest label: `_gapscan / sub_00eef5d0`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; original global binding00cdae36 pairs TestQuestCard2 with ownfactory00eef6d0 and sharedCEmptyData::Alloc00cd4ac0 (ownvtable12c2724/Emptyregistration, independently established existing row). Exactoriginal CLevelScriptBase<NScript::CEmptyData> constructor PDB and ownGame64/MasterData68 reconcile72-byteallocation. Own table012f5bf4 targetslot2 is original publicvirtual Main (not entity Init); ownexactmessage You've chosen quest card 2! and duration2.5f; ShowOnScreenMessage constslot111 originalPDBsignatureUBEXcharref,float.
Layout: genuine original CEmptyData level-script type and original complete72-byte script, no invented data subtype or flattened base.

## 00eef760: NScript::CTestQuestCard3Script::Main
Original manifest label: `_gapscan / sub_00eef760`.
Identity: proven; Complete original CScriptBase64/CLevelScriptBase72 and CEntityScriptBase20 typed Game/Me/Parent/Master prefixes; real CBaseClassNonCopyable protected ctor/dtor and CBaseClass ancestry, non-POD counted-pointer special members, CCharString(constchar*,long). Original STLport list:_List_base/rebound-node allocatorproxy and vector:_Vector_base/end-storage proxy retained; unused map/tree declarations omitted. Unused virtual slots and owner suffixes explicit projections. Shared thing/resource headers aliased as data evidence. Game exact ownRTTI1260f0c and PDB publicconst APIs; child privatevirtualEAE signatures preserve original access. Field identities from PDB named layouts reconciled with actual factory allocation sizes. No donor address used asretailidentity; original global binding00cdaed3 pairs TestQuestCard3 with ownfactory00eef860 and sharedCEmptyData::Alloc00cd4ac0 (ownvtable12c2724/Emptyregistration, independently established existing row). Exactoriginal CLevelScriptBase<NScript::CEmptyData> constructor PDB and ownGame64/MasterData68 reconcile72-byteallocation. Own table012f5c40 targetslot2 is original publicvirtual Main (not entity Init); ownexactmessage You've chosen quest card 3! and duration2.5f; ShowOnScreenMessage constslot111 originalPDBsignatureUBEXcharref,float.
Layout: genuine original CEmptyData level-script type and original complete72-byte script, no invented data subtype or flattened base.

## 00f25980: std::vector<CArenaRoundDef>::operator=
Original manifest label: `_global / std_vector_Conversation_Assign2`.
Identity: proven; Original CArenaRoundDef type proven by own virtual Transfer literals/typed wrapper callers, original PDB namesets and original member record layout. Catalog Conversation/SymbolInfo names false. True CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase packed37 ancestry, private default Def ctor with required original derived friendship, public copy ctor. Actual original element56 and genuine STLport allocator/vector protectedbase/endproxy. Standard algorithm helper bodies/declarations reflect actual separate codegen at own targets, no invented barrier.
Layout: original full typed element and base chain, incomplete unused nested vector element declaration only.

## 00f26c30: std::vector<CHeroSoulsRoundDef>::operator=
Original manifest label: `_global / std_vector_Conversation_Assign3`.
Identity: proven; Original CHeroSoulsRoundDef type proven by own virtual Transfer literals/typed wrapper callers, original PDB namesets and original member record layout. Catalog Conversation/SymbolInfo names false. True CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase packed37 ancestry, private default Def ctor with required original derived friendship, public copy ctor. Actual original element56 and genuine STLport allocator/vector protectedbase/endproxy. Standard algorithm helper bodies/declarations reflect actual separate codegen at own targets, no invented barrier.
Layout: original full typed element and base chain, incomplete unused nested vector element declaration only.

## 00f25f90: std::vector<CArenaCreatureDef>::operator=
Original manifest label: `_global / std_vector_assign_CConversation`.
Identity: proven; Original CArenaCreatureDef type proven by own virtual Transfer literals/typed wrapper callers, original PDB namesets and original member record layout. Catalog Conversation/SymbolInfo names false. True CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase packed37 ancestry, private default Def ctor with required original derived friendship, public copy ctor. Actual original element56 and genuine STLport allocator/vector protectedbase/endproxy. Standard algorithm helper bodies/declarations reflect actual separate codegen at own targets, no invented barrier.
Layout: original full typed element and base chain, incomplete unused nested vector element declaration only.

## 00f26920: std::vector<CArenaWaveDef>::operator=
Original manifest label: `std / std::vector_assign_CConversation`.
Identity: proven; Original CArenaWaveDef type proven by own virtual Transfer literals/typed wrapper callers, original PDB namesets and original member record layout. Catalog Conversation/SymbolInfo names false. True CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase packed37 ancestry, private default Def ctor with required original derived friendship, public copy ctor. Actual original element60 and genuine STLport allocator/vector protectedbase/endproxy. Standard algorithm helper bodies/declarations reflect actual separate codegen at own targets, no invented barrier.
Layout: original full typed element and base chain, incomplete unused nested vector element declaration only.

## 00f27a10: std::vector<CHeroSoulsCreatureDef>::_M_insert_overflow
Original manifest label: `_global / vector_reallocate_insert_pair_CSymbolInfo`.
Identity: proven; Original CHeroSoulsCreatureDef type proven by own virtual Transfer literals/typed wrapper callers, original PDB namesets and original member record layout. Catalog Conversation/SymbolInfo names false. True CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase packed37 ancestry, private default Def ctor with required original derived friendship, public copy ctor. Actual original element52 and genuine STLport allocator/vector protectedbase/endproxy. Standard algorithm helper bodies/declarations reflect actual separate codegen at own targets, no invented barrier.
Layout: original full typed element and base chain, incomplete unused nested vector element declaration only.

## 00f34860: std::vector<CFeatDef>::_M_insert_overflow
Original manifest label: `_global / vector_reallocate_insert_CFeatDef`.
Identity: proven; Original CFeatDef type proven by own virtual Transfer literals/typed wrapper callers, original PDB namesets and original member record layout. Catalog Conversation/SymbolInfo names false. True CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase packed37 ancestry, private default Def ctor with required original derived friendship, public copy ctor. Actual original element100 and genuine STLport allocator/vector protectedbase/endproxy. Standard algorithm helper bodies/declarations reflect actual separate codegen at own targets, no invented barrier.
Layout: original full typed element and base chain, incomplete unused nested vector element declaration only.

## 012242b0: StaticInitialization::_$E1 -- initialize the scaled float at013D27DC.
Original manifest label: `_gapscan / sub_012242b0`.
Identity: descriptive data/function identity, proven static initializer role; Own native static initializer calls00a76320 (loads float0.5 from0122f59c with no native args), multiplies actual0.9f literal01253af0 and stores exactglobal013D27DC. Original global/provider names and provider highlevel role unknown; old GetCombatMinDistance is unsupported catalog propagation. Cdecl zeroarg float-return projection matches actual call ABI; not adopted from donor-address labels.
Layout: actual single float global; compiler emits initialization helper from ordinary dynamic global initializer. No applicable shared class header.

## 01224430: StaticInitialization::_$E1 -- initialize the scaled float at013D2D64.
Original manifest label: `_gapscan / sub_01224430`.
Identity: descriptive data/function identity, proven static initializer role; Own native static initializer calls00a76320 (loads float0.5 from0122f59c with no native args), multiplies actual0.9f literal01253af0 and stores exactglobal013D2D64. Original global/provider names and provider highlevel role unknown; old GetCombatMinDistance is unsupported catalog propagation. Cdecl zeroarg float-return projection matches actual call ABI; not adopted from donor-address labels.
Layout: actual single float global; compiler emits initialization helper from ordinary dynamic global initializer. No applicable shared class header.

## 012244f0: StaticInitialization::_$E1 -- construct CDynamicObjectHeap<24,853> at013D2E38.
Original manifest label: `_gapscan / sub_012244f0`.
Identity: original template/base identity proven; Own RTTI COL0135a54c type descriptor literal .?AV?$CDynamicObjectHeap@$0BI@$0DFF@@@ proves exact original CDynamicObjectHeap<24,853>; full RTTI base array has only CDynamicObjectHeapBase (itself no base), table0129f638 destructor0/privatevirtualRelease1. Both PDB exact constructor(bool) QAE and dtorUAE/ReleaseEAE confirm genuine template/API. Base20 ctor00f36c60/dtor00f36c90 and original fields AllocCount4,TotalAllocatorBlocks8,RefCountReleaseEnabledc,HeapNext10. Original FreeList14,StartBlock18,AllowHeapToGrow tail and nested CObjectEntry/AllocatorBlockEntry fields from PDB layouts and own releaseAF8780/AF8BC0; entrysize24 and count853 exactly match own loop bounds. ECX carries previous entry across iterations, proving reversed free-chain links, not independent null stores. Globals exact original spellings unproven/address-suffixed.
Layout: genuine original heap/base/nested storage, no opaque vptr or invented padding; Pad is original entry payload byte storage from PDB. Native static initialization and atexit callback compiler-generated from ordinary global object. Nested class keys and base-pure member access unproven projections; original proven ctor/dtor/private derived Release access retained. No available shared heap header.

## 01224590: StaticInitialization::_$E1 -- construct CDynamicObjectHeap<116,529> at01427E20.
Original manifest label: `_gapscan / sub_01224590`.
Identity: original template/base identity proven; Own RTTI COL0135a5dc type descriptor literal .?AV?$CDynamicObjectHeap@$0HE@$0CBB@@@ proves exact original CDynamicObjectHeap<116,529>; full RTTI base array has only CDynamicObjectHeapBase (itself no base), table0129f650 destructor0/privatevirtualRelease1. Both PDB exact constructor(bool) QAE and dtorUAE/ReleaseEAE confirm genuine template/API. Base20 ctor00f36c60/dtor00f36c90 and original fields AllocCount4,TotalAllocatorBlocks8,RefCountReleaseEnabledc,HeapNext10. Original FreeList14,StartBlock18,AllowHeapToGrow tail and nested CObjectEntry/AllocatorBlockEntry fields from PDB layouts and own releaseAF8780/AF8BC0; entrysize116 and count529 exactly match own loop bounds. ECX carries previous entry across iterations, proving reversed free-chain links, not independent null stores. Globals exact original spellings unproven/address-suffixed.
Layout: genuine original heap/base/nested storage, no opaque vptr or invented padding; Pad is original entry payload byte storage from PDB. Native static initialization and atexit callback compiler-generated from ordinary global object. Nested class keys and base-pure member access unproven projections; original proven ctor/dtor/private derived Release access retained. No available shared heap header.

## 01224c6d: StaticInitialization::_$E1 -- construct the STLport map at0143e910 and register its destruction.
Original manifest label: `_gapscan / sub_01224c6d`.
Identity: proven map/tree lifetime role, descriptive Key/Value/global; Own static initializer constructs global0143e910 via actual STLport map default ctor00cd3640; callback0122aea0 invokes tree destructor00cd31d2. Constructor00cd3640 passes real empty Compare/allocator references to tree ctor00cd2d37; that allocates header via00cd283c, clears node_count+4, bool headercolor0/parent4, self-links left8/rightc. Tree destructor clears and frees header. Actual global occupies12bytes before neighboring0143e91c. Original Key/Value identities and kinds unproven, represented only by incomplete distinct CUnproven types: no element layout/sizeof/copy semantics invented or accessed. Source uses genuine std::map/_Rb_tree/_Rb_tree_base, node/allocator proxy/rebound node allocator and comparator members, not a synthetic map_T class or arbitrary constructor helper.
Layout: real tree header/size/comparator storage and original bool color, original constructor/destructor declarations remain out of line. Map implicit destructor naturally forwards to actual tree destructor; no manual initializer or atexit wrapper. No applicable shared STLport source header in workspace.

## 01224c83: StaticInitialization::_$E1 -- construct the wide string at0143e908 and register its destruction.
Original manifest label: `_gapscan / sub_01224c83`.
Identity: proven CWideString/default lifetime, descriptive global; Own native static initializer loads global0143e908, calls default CWideString constructor0099aed0 then registers destructor callback0122ae6a, which calls CWideString destructor0099b510. Original default constructorbody clears storage_ and incrementswide-instance counter013bca20, independently distinguishing wide from narrow. Shared real fable_string.h reused unchanged; original global spelling unproven.
Layout: genuine original four-byte CWideString and native compiler-generated static lifetime; no handwritten initializer or atexit wrapper.
