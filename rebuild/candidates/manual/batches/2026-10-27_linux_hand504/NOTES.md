# Hand chunk 504 (std::__introsort_loop .. PKCOneShotQueue<SFileReply>): 24 of 30 byte-exact

All 24 are RELOCATION_MATCH (no plain MATCH, no retail_asm). gall.sh: 24/24 RELOCATION_MATCH; tall.sh (g++) and
tall.py (VC7.1): 24/24 PASS; vland dry run: WINS 24, no RELOC_IDENTITY, no RELOC_DUPLICATE, no DIFFER.

## Names

### Proven
- 00a82380 std::vector<std::pair<unsigned __int64,bool> >::_M_insert_overflow -- 16-byte element; Ego_r.pdb CNavSwitchableLeafNode::UniqueIDsOpen is CVectorMap<unsigned __int64,bool>; its callers are that map's push_back/insert
- 00a82480 std::vector<std::pair<unsigned __int64,CANavQuadTreeNode*> >::_M_insert_overflow -- Ego_r.pdb CNavQuadTree::SwitchableNodes; caller CNavQuadTree::LoadFromFile push_back 0x00A837E0
- 00aacda0 std::vector<CActiveSoundInfo>::_M_insert_overflow -- named by the landed push_back 0x00AACEF0; the element stores RTTI vtable 0x0129E674 CActiveSoundInfo
- 00ac8720 QuadricOptimiserInternals::COptimisedVertex::InterpolateGroups(const COptimisedVertex&, float) -- Ego_r.pdb donor order and the Groups map at +0x9C
- 00ad83d0 std::vector<QuadricOptimiserInternals::COptimisedSubMesh>::_M_insert_overflow -- 0x18-byte element; callers push_back 0x00ACEB70 and insert 0x00AD8764
- 00adc650 std::__introsort_loop<CVertexGroupBlockOptimiser::CBlockInfo*,...> -- named by the landed std::sort 0x00ADC890 of the same instance
- 00adce60 std::vector<CVertexGroupBlock>::_M_insert_overflow -- Ego_r.pdb CVertexGroupBlock {vector<unsigned long>, map<unsigned long,long>}; the copy ctors 0x00535470 and 0x00A96E40 it calls
- 00af1e90 std::vector<NParticleEngine::CParticleEmitterInfo>::_M_insert_overflow -- Ego_r.pdb element; landed helpers 0x00AF1C10 / 0x00AF1AA0 / 0x00AF1D90 name this caller
- 00b0f9f0 std::vector<std::pair<unsigned long,NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long> > >::_M_insert_overflow -- the element copy stores RTTI vtable 0x0125FEE4 .?AV?$CParticleUpdateParamDesc@KK@NParticleEngine@@
- 00b6c120 std::vector<CVertexShader>::_M_insert_overflow -- the element stores RTTI vtable 0x0129B3D0 CVertexShader (base CShaderBase); 14 push_back callers in the landscape renderer
- 00b91e70 CEngineSceneGrid::GetTotalCellCount(unsigned long, unsigned long) -- Ego_r.pdb ?GetTotalCellCount@CEngineSceneGrid@@AAEKKK@Z (donor order)
- 00bd9ed0 CEngineLandscapeMemoryManager::~CEngineLandscapeMemoryManager() -- the first vtables it stores, 0x012A7CBC / 0x012A7CA8, are RTTI .?AVCEngineLandscapeMemoryManager@@; Ego_r.pdb donor order after FreeDisposalList; the only caller is its ??_G 0x00BDAF63
- 00bdb9f0 std::vector<CVertexShaderConstant>::operator= -- caller 0x00BDBD1B inside CVertexShaderConstantLayout's constructor 0x00BDBB70 passes this+0x140 = Ego_r.pdb ReservedPresets (CArray<CVertexShaderConstant>)
- 00bf94d0 std::vector<CEngineTexturePalette::CPaletteEntry>::_M_insert_overflow -- inline push_backs in CEngineTexturePalette's constructor 0x00BF90A0 and GetTexturePaletteIndex 0x00BF9150 on this+4 = Ego_r.pdb Palette; FableWin.pdb class key U (UCPaletteEntry@CEngineTexturePalette)
- parked 00a60410 GFRemoveComments(char*, const char*, const char*) -- Ego_r.pdb ?GFRemoveComments@@YIXPADPBD1@Z; 0x00A60200 is its LeavingNewlines sibling
- parked 00ac7200 QuadricOptimiserInternals::COptimisedPrimitive::MakeOrthogonal(const C3DVector&, const C3DVector&) const -- Ego_r.pdb donor order
- parked 00b67e30 CEngineLandscapeRenderer::RegisterSubPatchForZSorting(long&, CLandscapeBackgroundPatch*) -- Ego_r.pdb

### Probable
- 00a5aaa0 std::__introsort_loop<float*,float,int,std::less<float> > -- STLport 4.6 body shape; float compares; the __partial_sort callee is 0x00A5A8A0
- 00a6bec0 std::__introsort_loop over std::pair<unsigned long,unsigned long> -- body shape plus the element compare; callees 0x00A6B420 and 0x00A6BBB0
- 00ae7be0 std::__introsort_loop over std::pair<long,unsigned long> -- signed first-key compare; callees 0x00AE5F00 and 0x00AE7760
- 00a80ee0 std::vector<CANavQuadTreeNode*>::operator= -- caller 0x00A81BA0 is a one-member operator= (CArray<T>) reached from CNavigationLayer's implicit operator= 0x00A85700, which fits the Ego_r.pdb CNavigationLayer layout
- 00bed690 std::swap<std::pair<long,CCompositeSurfaceInfo> > -- vptr 0x012A8154 = RTTI .?AVCCompositeSurfaceInfo@@ stored into the temporary; the pair layout of landed __adjust_heap 0x00BEDD70 (CVectorMap<long,CCompositeSurfaceInfo>)
- 00bef700 std::__partial_sort over std::pair<std::pair<long,long>,CEngineTextureDecal> -- it calls the landed decal-table heap trio __make_heap 0x00BEEE40, __pop_heap 0x00BEEDA0 and sort_heap 0x00BEF350
- parked 00c0ffe0 PKCProtectionInstance::~PKCProtectionInstance() -- the destructor of the landed guard 0x00C0FFC0; the class name comes from its own trace string ".\PKCProtectionInstance.cpp"

### Descriptive
- 00c186d0 CMorphableNode_00c185b0::Slot00 -- slot 0 of vtable 0x012B2DEC (libpk PKCTRMorphable.cpp; no PDB/RTTI); keeps the landed Slot01 0x00C187A0 names
- 00c1aec0 PKCStaticQueue<SFileRequest>::Push -- slot 1 of vtable 0x012B2FC8; the class name comes from the "libpk\PKCStaticQueue.h" trace (the landed sweep1 name, no address suffix)
- 00c1b370 PKCStaticQueue<SFileRequest>::Pop -- slot 0 of vtable 0x012B2FC8; same trace
- 00c1dd70 PKCOneShotQueue<SFileReply>::Pop -- slot 0 of vtable 0x012B332C; the "libpk\PKCOneShotQueue.h" trace
- parked 00c11230 CSoundEventManager::CanStartEvent -- the class comes from the landed libpk set; the method name is descriptive
- parked 00c1ca20 CMatrix3x4::RotateC -- the Z-axis sibling of the landed descriptive RotateB 0x00C1C900 / RotateA 0x00C1C990

## What matched (24)
- STLport sorts (/Gr, __fastcall): 00a5aaa0, 00a6bec0, 00ae7be0, 00adc650 (__introsort_loop) and 00bef700 (__partial_sort). For pairs, use struct pair with STLport's explicit memberwise copy ctor and struct less. In __partial_sort, make_heap is inline and __make_heap, __pop_heap and sort_heap stay out of line. A same-TU noinline __uninitialized_copy homes the __false_type tag in the dead value-type slot.
- vector::_M_insert_overflow (8 rows): the real STLport 4.x body. The tag-taking helpers (__uninitialized_copy, __uninitialized_fill_n, __destroy_aux) carry same-TU `__declspec(noinline)` bodies so that VC7.1 homes the `__false_type()` temporaries on the atend slot. Polymorphic elements destroy through a virtual-dtor loop, or call the base dtor directly when the derived class declares none (00b0f9f0).
- vector::operator= (00a80ee0, 00bdb9f0): STLport vector.c verbatim, with __copy_ptrs and __uninitialized_copy as same-TU noinline bodies (family L030).
- 00ac8720 InterpolateGroups: `Groups.insert(value_type(o->first, 0.0f)).first->second += weight;` with a named `float weight`. VC hoists the loop-invariant 0.0f store only when insert_unique's real body and a `_M_insert` stand-in that reads v (never writes it) are in the TU; operator-- is noinline.
- 00b91e70 GetTotalCellCount: a local model (vptr, Level, LevelUp, LevelDown, Size, CArray GridCells at +0x14) and the plain recursive loop.
- 00bd9ed0 ~CEngineLandscapeMemoryManager: the body is just `FreeDisposalList();`. The three `std::list<T*>[3]` members give the counted reverse loops through an inline ~_List_base (clear() inline, then a null-tested sentinel free); the inline empty virtual base dtors give the trailing vtable stores.
- 00bed690 std::swap: `_Tp __tmp = __a; __a = __b; __b = __tmp;` with the landed __forceinline CCountedPointer model.
- libpk traces (/EHsc): 00c186d0, 00c1aec0, 00c1b370, 00c1dd70. A `CString s;` in a nested block, the operator<< chain and the assert hook give EH state 0 -> -1, then the `CDebugBreak();` temporary on a dead frame byte. The queue bodies come from the retail registers: Pop returns its pointer argument (`mov eax,[arg] ; mov edi,eax` before `rep movsd`). Push's `jb`/`div` mean unsigned count and capacity. In OneShot Pop the flag store sits between the copy's loads, so the flag is cleared first.

## Parked for Fable 5.1 (6)
- 00a60410 GFRemoveComments DIFFER(262v245): VC7.1 inverts the strstr loop (second strstr duplicated at the bottom, start kept in edi) where retail jumps back to one loop head with p in ebx | tried: for(;;), while, goto loop forms | next: a do/while with the first strstr hoisted, or a helper split
- 00ac7200 COptimisedPrimitive::MakeOrthogonal DIFFER(255v255): dot-product term order only; retail `fld [ecx+8] ; fmul [eax+8]` (z term) last, ours schedules z second | tried: 12 Dot spellings (+= chain, operator*, named locals) -- over budget through probes | next: lane gap: x87 scheduling sensitive to unrelated TU declarations
- 00b67e30 CEngineLandscapeRenderer::RegisterSubPatchForZSorting DIFFER(128v129): retail keeps the folded bitfield word 0xFFFFFFFD in esi for both the temporary and the element and uses `mov eax,[ecx+4] ; add eax,8 ; mov [ecx+4],eax`; ours reloads the word and uses `add [ecx+4],8` | tried: aggregate, field-assignment and constructor forms | next: model SortedSubPatchList's push_back with the real STLport vector (header debt: the member is opaque)
- 00c0ffe0 PKCProtectionInstance::~PKCProtectionInstance DIFFER(256v253): retail `call [GetTickCount] ; sub eax,[edi+4] ; mov esi,eax ; cmp esi,[0x0139C904]`, ours `mov ecx,[edi+4] ; mov esi,eax ; mov eax,[max] ; sub esi,ecx ; cmp esi,eax` | tried: one expression, split `elapsed -= m_tick`, inline GetElapsed() member, /Oa | next: lane gap: memory-operand folding (or a different global declaration, e.g. the maximum as a class static)
- 00c11230 CSoundEventManager::CanStartEvent DIFFER(242v242): block layout only; retail falls from `jb` into ResetLevel, Stop ends in `jmp` to a shared return-1 epilogue and the return-0 epilogue comes last | tried: early-return form, split ifs, else-if chain, nested positive if | next: a result variable with a single return, or goto-shaped source
- 00c1ca20 CMatrix3x4::RotateC DIFFER(140v140): first row only; retail s*X, s*Y, c*X; ours forms c*X before s*Y where X is at the zero displacement [ecx] (rows 2-4 match) | tried: swapped sum operands, swapped product operands, X read into a local | next: lane gap: zero-displacement operand ranking (try the free-function / pointer-argument form)

## Skipped
None.

## Label corrections
- 0x00A5AAA0 "Quicksort_Float", 0x00A6BEC0 "QuickSort_Median3_Partition", 0x00AE7BE0 "Quicksort_Partition_64Bit" and 0x00ADC650 "Quicksort_Partition_Recursive" -> std::__introsort_loop instances.
- 0x00A80EE0 "Vector_AssignWithRealloc" -> std::vector<CANavQuadTreeNode*>::operator=; 0x00A81BA0 -> CArray<CANavQuadTreeNode*>::operator=; 0x00A85700 -> CNavigationLayer's implicit operator=.
- 0x00A82380 "Vector_InsertN_16byte", 0x00A82480 "Vector_InsertN_16ByteElements", 0x00AACDA0 "Vector_Reallocate_CopyFill", 0x00AD83D0 "Vector_ReallocateElements_6Way", 0x00ADCE60 "vector_insert", 0x00AF1E90 "Vector_InsertRange", 0x00B0F9F0 "Vector_ReallocateWithNew", 0x00B6C120 "VectorReallocateAndCopy" and 0x00BF94D0 "Vector_ReallocateForPairs" -> std::vector<T>::_M_insert_overflow (element types above).
- 0x00AC7200 "Vector_ProjectOntoPlane" -> COptimisedPrimitive::MakeOrthogonal; 0x00AC8720 "QuadricOptimiser_UpdateQuadricCosts" -> COptimisedVertex::InterpolateGroups.
- 0x00BD9ED0 "ParticleSpriteGroup_Destroy" -> ~CEngineLandscapeMemoryManager; its callee 0x00BD9CE0 "TexturePool_Clear" -> CEngineLandscapeMemoryManager::FreeDisposalList.
- 0x00BDB9F0 "std_vector_assign_copy_stride" -> std::vector<CVertexShaderConstant>::operator=; 0x00BDB750 `_Copy_impl<C2DLineF*,C2DLineF*>` (BSim) -> __uninitialized_copy<const CVertexShaderConstant*, CVertexShaderConstant*>.
- 0x00BED690 "swap<CCompositeSurfaceInfo>" -> std::swap<std::pair<long,CCompositeSurfaceInfo> >.
- 0x00BEF700 "Std_Make_Heap_ProcessPairs" -> std::__partial_sort.
- 0x00BF90A0 "CParticleMorphs::Initialize" -> CEngineTexturePalette (constructor/initialiser: Palette at +4, RetailVersion at +0x14); 0x00BF8F30 / 0x00BF8F70 (landed with invented SPairLongCSecond_0099ec30 names) -> __uninitialized_copy / __uninitialized_fill_n of CEngineTexturePalette::CPaletteEntry.
- 0x00C0FFE0 "EventQueue_ProcessWithTimeout" -> PKCProtectionInstance::~PKCProtectionInstance.
- 0x00C11230 "CTCHero_OnActionInterrupt" -> a CSoundEventManager method (sole caller 0x00C12940). Its callees' labels are all propagated game names:
  - C3DGroup2 0xC25C50 -> the 16-byte handle's zeroing ctor
  - CAnimateThing::GetPMeshAnims 0xC25C70 -> Bind
  - CTCStealth::OnCreate 0xC25CB0 -> Release
  - $E2 0xC25CA0 -> the empty dtor
  - InputEvent_ValidateForContext 0xC20C10 -> CSoundEvent::IsAudible
  - CTCInventory::OnCreate 0xC21620 -> CSoundEvent::Stop
  - Thiscall_ActionLightningStateHandler 0xC20DF0 -> CSoundEvent::ResetLevel
- 0x00C1B460 "CAIStateGroup_HoverHornetQueen::GetNextCirclePoint" -> PKCStaticQueue<SFileRequest>::GetTailIndex (head - count, wrapped).
- 0x00C0DEC0 / 0x00C0DF00 / 0x00C0DF10 "operator&&<NScript::...>" -> CString::operator<< (text / long / unsigned long); 0x00C16750 "list<CMessageEvent const*>" -> CDebugBreak::CDebugBreak; 0x00C0BA40 "CCharStringData::Release" -> CString::~CString.
- Gap-scan placeholders named: 0x00B91E70, 0x00C186D0, 0x00C1AEC0, 0x00C1B370, 0x00C1CA20 and 0x00C1DD70.

## Header debt
- 00bd9ed0: engine/CEngineLandscapeMemoryManager.h flattens both bases (CFailedAllocationHandler @0, CMovableResource @4) into `__vftable` + pad and keeps TextureDisposalList / VertexBufferDisposalList / IndexBufferDisposalList ([3] STLport lists each) opaque. The TU uses a local model (vland: SHARED_HEADER_AVAILABLE CMovableResource).
- 00af1e90: local CIVCountedPointer / CIVCountedPointeeBase model next to an existing engine header (vland: SHARED_HEADER_AVAILABLE CIVCountedPointeeBase).
- 00b67e30 (parked): CEngineLandscapeRenderer::SortedSubPatchList (+0x5E4) is opaque in the header; the donor-include model needs it typed.
- 00b91e70: no engine header for CEngineSceneGrid; local model from the Ego_r.pdb layout (vptr, Level, LevelUp, LevelDown, Size, GridCells +0x14).
- libpk: PKCStaticQueue<T> count/capacity are unsigned (retail `jb` / `div`), and the queue interface's slot 0 is `T* Pop(T*)` (returns item). The landed models (ctor 0x00C1B340, PKCDefaultFileManager 0x00C1A230, PKCOneShotQueue 0x00C1DD50 / 0x00C1DF30) spell `long` and `void Pop`.
