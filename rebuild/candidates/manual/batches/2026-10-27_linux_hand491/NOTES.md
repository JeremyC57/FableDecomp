# Hand chunk 491 (std::__uninitialized_fill_n<CCompositeBlendDesc> .. CSuspendableProcessMicrothread): 39 of 40 byte-exact

17 MATCH + 22 RELOCATION_MATCH, 0 retail_asm, 0 parked, 1 skipped. Every test prints `<addr>_TEST PASS` under tall.sh (g++)
and tall.py (VC7.1/Wine). Read-only vland dry run: WINS 39, no RELOC_IDENTITY; the only warnings are three RELOC_NAME
(the new names differ from propagated catalog labels; see "Callee names").

## What matched
| addr | name | grade |
|---|---|---|
| 006cacb0 | std::__uninitialized_fill_n<CCompositeBlendDesc*, unsigned int, CCompositeBlendDesc> | MATCH |
| 008d4680 | CGameScriptThing::DecrementScriptCounter | MATCH |
| 0099cf70 | CSystemRegistry::~CSystemRegistry | RELOCATION_MATCH |
| 009a76c0 | NBankFileManager::SetHeaderDir | RELOCATION_MATCH |
| 009af6e0 | CDefinitionManager::RecompileDefType | RELOCATION_MATCH |
| 009af720 | CDefinitionManager::RecompileAllDefs | RELOCATION_MATCH |
| 009bc7d0 | CXMVPlaybackInfo::CXMVPlaybackInfo | MATCH |
| 009d47e0 | CBankCreatorException::CBankCreatorException(const CBankCreatorException&) | RELOCATION_MATCH |
| 009d8240 | NProfileTimer::StartProfile | MATCH |
| 009e7380 | std::remove_if<pair<CCharString, CSymbolMap::CSymbolInfo>*, CPred_HasGroupName> | RELOCATION_MATCH |
| 009eaae0 | CConsole::SetConsoleHelpWindowColour | MATCH |
| 009ecc70 | CConsole::ConsoleAddKeyBinding | RELOCATION_MATCH |
| 009f2ac0 | CMemoryAllocator::InternalAllocate(long, unsigned long, const char*, long, const char*) (/GS) | RELOCATION_MATCH |
| 009f76a0 | CFourierAnalysis::CFourierAnalysis | MATCH |
| 009f8860 | std::list<CGraphicBankManager::CGraphicDataBankInfo>::_M_create_node | RELOCATION_MATCH |
| 009f9df0 | CTexture::GetNoLevels | MATCH |
| 009fc150 | CResourceList::FrameEnd | MATCH |
| 00a000a0 | std::_Destroy<CUnprovenPolymorphic_00a000a0> | MATCH |
| 00a01d40 | CHandleBasedMemoryPool::GetHandleFromPointer | MATCH |
| 00a03b80 | CInputEvent::GetChar | MATCH |
| 00a14e50 | CNavigatorManager::SaveNavigationMapToFile | RELOCATION_MATCH |
| 00a19b90 | CANavQuadTreeNode::GetPreferabilityCost | MATCH |
| 00a20410 | std::pair<const long, CNavigatorAStar::CAStarState>::pair | RELOCATION_MATCH |
| 00a222b0 | std::_Rb_tree<float, pair<const float, const CANavQuadTreeNode*>, ...>::insert_equal | RELOCATION_MATCH |
| 00a25e60 | CPhysicsMeshInfo::CMeshData::OnResourceLoaded | RELOCATION_MATCH |
| 00a2f170 | C3DMeshStats::GetHelperPointNamesStartingWith | RELOCATION_MATCH |
| 00a31150 | std::__copy<pair<unsigned long, C3DHelperPoint>*, ..., int> | MATCH |
| 00a31880 | CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess<..>>::LowerBound | MATCH |
| 00a318d0 | CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess<..>>::UpperBound | MATCH |
| 00a32bc0 | std::__pop_heap<pair<unsigned long, C3DDummyObject>*, ..., CKeyPairCompareLess<..>, int> | RELOCATION_MATCH |
| 00a345d0 | std::vector<C3DGeneratorObject>::push_back | RELOCATION_MATCH |
| 00a35360 | std::vector<pair<unsigned long, C3DMeshVolume>>::push_back | RELOCATION_MATCH |
| 00a35b10 | C3DMeshObject::GetHelperPointNamesStartingWith | RELOCATION_MATCH |
| 00a36fc0 | CClassFactory::GetNoAllocatedOfType | MATCH |
| 00a3a9c0 | CCompressorZlib::Alloc | RELOCATION_MATCH |
| 00a3aa00 | CCompressorZlib::Free | RELOCATION_MATCH |
| 00a3e420 | CSoundManagerLH::GetNextEmitterID_00a3e420 (descriptive) | MATCH |
| 00a44130 | CCountedPointer<CLipSyncSampleData>::DeleteData | RELOCATION_MATCH |
| 00a44690 | CSuspendableProcessMicrothread::InternalSuspend | RELOCATION_MATCH |

## Parked
(none)

## Skipped
- 00a446a0 `_global::ThreadEntry` (oracle 46 bytes): the row is cut wrong. The real function is
  ?ThreadEntry@CSuspendableProcessMicrothread@@CAXPAX@Z, 0x00A446A0..0x00A4473D (158 bytes, /Od, an SEH catch funclet at
  0x00A446F4). The catalog splits it into five rows: ThreadEntry, NScript_CIngredientOwner_RunThread, Catch@00a446f4, Init
  and NScript_MicroThread_Idle. It needs re-cutting as one row with /EHsc + /Od before anyone authors it.

## Label corrections
- 0x00A01D40: catalog `CWorld::DrawGetEnvironment` -> `CHandleBasedMemoryPool::GetHandleFromPointer(void*)`. Evidence:
  `mov eax,[ecx-1Ch]` = (CHeader*)p - 1 ->Handle (+4 of the 0x20-byte CHeader in Ego_r.pdb); caller ~CContainmentVolume
  0x00A752F0.
- 0x00A3E420: catalog `CGameScriptThing::DecrementScriptCounter` (propagated) -> `CSoundManagerLH::GetNextEmitterID_00a3e420`
  (descriptive). Body `return m_nextEmitterID++` (+0x50); callers are the CSoundEmitterLH constructors.
- 0x009D8240: catalog `_global::$E2` -> `NProfileTimer::StartProfile(const CCharString&, long)`, an empty __fastcall; its
  twin 0x009D8250 is EndProfile.
- 0x00A44690: catalog `_Cons_val<allocator<pair<EHeroMorphType, CParticleMorphs::CEntry>>, ...>` (propagated) ->
  `CSuspendableProcessMicrothread::InternalSuspend()` (Ego_r.pdb protected static __fastcall; /Od body calling
  MicroThread::MicroThreadSleep 0x009D8650). The `_Cons_val` / `_Dest_val` labels in this region are propagated and
  wrong (retail has no ICF).
- 0x00A25E60: catalog `vfunc_5` -> `CPhysicsMeshInfo::CMeshData::OnResourceLoaded` (vtable 0x0129CD78, slot 5).
- 0x00A3AA10 (not in the chunk): labelled `CVO_OracleMarker::Init`, but it is CCompressorZlib::Initialise (next to Alloc
  and Free, and stores both as zalloc/zfree).
- 0x00A2F120 (not in the chunk): labelled GetAllDummyObjectNames, but it is C3DMeshStats::GetAllHelperPointNames (it calls
  UnpackNames with dummyObjects = false, the same as 0x00A2F170).
- 0x00A43F80: labelled `_global::Destroy_StringRecordVector_00a43f80` ->
  `std::__destroy_aux<CLipSyncFrameData*>(first, last, const __false_type&)` (12-byte CLipSyncFrameData elements, called
  from DeleteData 0x00A44130).
- 0x00A33C50 / 0x00A34F00: labelled `Vector_Reallocate_*` -> `vector<C3DGeneratorObject>::_M_insert_overflow` /
  `vector<pair<unsigned long, C3DMeshVolume>>::_M_insert_overflow` (STLport push_back's overflow arm).

## Header debt
- Local polymorphic models, where the engine header flattens the vptr:
  - CGameScriptThing (008d4680): virtual DecrementScriptCounter, ScriptCounter +0x14.
  - CANavQuadTreeNode (00a19b90).
  - CResource base of CPhysicsMeshInfo::CMeshData (00a25e60): 0x1C bytes; m_pSourceData +0x1C is descriptive.
- Local classes with no usable header:
  - CConsole: colours +0x80..+0x90 (009eaae0); KeyBindingMap +4 (009ecc70).
  - CSymbolMap, CGenericVar, CPred_HasGroupName (009af6e0 / 009af720 / 009e7380).
  - CSoundManagerLH +0x50 (00a3e420).
  - CMemoryAllocator (009f2ac0): Ego_r layout, laid out by hand like the landed 0x009F2A40.
  - CLipSyncSampleData / CLipSyncFrameData / CLipSyncPhoneme (00a44130).
  - CCompositeBlendDesc / CRGBColour (006cacb0): the same model as the landed 0x006CAC20.
- Donor-pattern rows that use engine headers:
  - CTexture, CResourceList, CHandleBasedMemoryPool, CClassFactory, CDefinitionManager.
  - CNavigatorManager + CNavQuadTree.
  - CCompressorZlib: these rows had a SHARED_HEADER_AVAILABLE warning and were moved onto the headers.
  - For CDefinitionManager, CNavigatorManager and CClassFactory, accessors reinterpret the opaque container bytes as STLport
    containers.

## Callee names (new spellings vs reloc_bindings)
- 0x00A43F80: `??$__destroy_aux@PAVCLipSyncFrameData@@@std@@YIXPAVCLipSyncFrameData@@0ABU__false_type@0@@Z`. New; the
  catalog label is descriptive.
- 0x00A33C50 / 0x00A34F00: `_M_insert_overflow` of the two vectors. Protected, with `__false_type` and atend.
- 0x00A32500: `std::__adjust_heap<pair<unsigned long, C3DDummyObject>*, int, pair<..>, CKeyPairCompareLess<..>>`. Declared
  only.
- 0x009E6A70: `std::__find_if<pair<CCharString, CSymbolInfo>*, CPred_HasGroupName>` (random access). 009e7380 carries a
  same-TU stand-in body for it under auto_inline(off) + noinline; review this at landing.
- 0x009E6990: `std::remove_copy_if<...>`. Declared only.
- 0x009EFCF0: `std::map<char, CCharString>::operator[]`. Out of line.
- 0x00A2ED10: `C3DMeshStats::UnpackNames(vector<CCharString>&, bool, const CCharString&, bool) const`. Private.
- 0x00A21E80: `_Rb_tree<float, ...>::_M_insert`. Private.
- 0x00A25D20: `CPhysicsMeshInfo::CMeshData::BuildMesh()`. Descriptive.
- 0x009AE6B0 / 0x009AEF60 / 0x009AF480: CDefinitionManager::ClearDefsForRecompile / CreateSymbolsFromPathList /
  DoCompilePathList. Protected.
- 0x009F2A40 / 0x009F2390: CMemoryAllocator::InternalAllocate(long, unsigned long, bool) / GetAllocationSize.
- 0x009D8650: MicroThread::MicroThreadSleep.
- 0x009D8250: NProfileTimer::EndProfile (the twin of 0x009D8240; not declared).
- 0x00A78B20: CNavQuadTree::SaveToFile(CAFile&) const.
- Globals:
  - 0x013CA7D4: `g_CCharString_013ca7d4`, the CWideString bank header directory; this is the landed name.
  - 0x013D27B8/BC/C0: CNavigatorAStar::CAStarState::NoOfStates / NoOfStatesOnOpenList / NoOfStatesOnClosedList.

## Proven
00a01d40
008d4680
0099cf70
009a76c0
009af6e0
009af720
009bc7d0
009d47e0
009d8240
009e7380
009eaae0
009ecc70
009f2ac0
009f76a0
009f9df0
009fc150
00a03b80
00a14e50
00a19b90
00a2f170
00a31880
00a318d0
00a35b10
00a36fc0
00a3a9c0
00a3aa00
00a44130
00a44690

## Probable
006cacb0
009f8860
00a20410
00a222b0
00a25e60
00a31150
00a32bc0
00a345d0
00a35360

## Descriptive
00a000a0
00a3e420
