# DOSSIER 0x00BCC800 CEnginePrimitiveManagerWeaponTrails::RenderWeaponTrail (3205 B) -- identity scout

## 1. Retail shape (dis_all.txt)
- `push ebp ; lea ebp,[esp-74h] ; sub esp,114h` (00bcc800..05) = VC7.1 /O2 large-frame EBP-offset frame (NOT /Oy-, no `mov ebp,esp`).
  Epilogue `add ebp,74h ; mov esp,ebp ; pop ebp ; ret 4` (00bcd47c..82). Two early `ret 4` at 00bcca2b, 00bcca79.
- thiscall, `ret 4` = one stack arg (const CEngineInternalPrimitiveWeaponTrail&, at [ebp+7Ch]); `this` homed at [ebp+64h].
- NO SEH: `grep fs:` = 0 hits, although CGraphicFrame, two CArray<C3DVector> and the capture block have dtors -> the TU is built
  without /EHsc (or the dtors are all nothrow-inlined). Draft has no `// Flags:` line; keep it that way unless a grade says otherwise.
- Two bare-dword fistp sites: 00bcc97e (GFFloatToLongFloor inline asm block 00bcc96a..00bcc992) and 00bccf73 (GFFloatToLongNear
  in the vertex loop). Both come from shared headers engine/GFFloatToLongFloor.h / GFFloatToLongNear.h -> the row needs
  `// Retail-asm:` lines for each site (model: rebuild/src/compiled/00/64/NPlayerGuiCCrimeStatBar_DrawStatBarOnLeft_00646010.cpp). The draft has none.

## 2. Name / signature (manifest label is RIGHT)
- `?RenderWeaponTrail@CEnginePrimitiveManagerWeaponTrails@@AAEXABVCEngineInternalPrimitiveWeaponTrail@@@Z` in
  ghidra_out/egor_pdb_names.tsv (Ego_r 0x00a81ac0) AND ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv (0x02ec8470, 3124 B).
  = private, non-virtual, `void RenderWeaponTrail(const CEngineInternalPrimitiveWeaponTrail&)`. Not in the vtable
  (rtti-vtables.tsv 0x012A6AEC slots 0/1/2/5/7 = 00bcd7a0/00bcd600/00bcc7c0/00bcd490/00bcc700).
- Only caller: 00bcd5b8 inside CEnginePrimitiveManagerWeaponTrails::RenderSubPrimitive 0x00BCD490 (landed,
  rebuild/src/compiled/00/bc/CEnginePrimitiveManagerWeaponTrails_RenderSubPrimitive_00bcd490.cpp; passes subPrimitive Parent +0x0c).

## 3. Callees (call site -> landed file / PDB)
| site | target | landed row | PDB (FableWin) | note |
|---|---|---|---|---|
| 8c4/8f8 | 009fbfb0 | 00/9f/CResource_ListRemove_009fbfb0.cpp | CResource::ListRemove | via TouchResource (inline) |
| 8d6/90a | 009fbf90 | 00/9f/CResource_ListInsert_009fbf90.cpp | CResource::ListInsert | |
| 938,a1d,a6b | 009fde80 | NOT LANDED (manifest: CGraphicInfoFrame::CFrameData::BeginLoading) | | inside GetFrame |
| 9cf,d443 | 009fcba0 | 00/9f/CGraphicFrameData_Uninitialise_009fcba0.cpp | | CGraphicFrame dtor/ReleaseData |
| 9e8,a58,d45b | vslot +4 | | | counted pointer / deleting dtor |
| a30,ce15,d3bf | 00bfea1a operator new; 9ef,d462,d471 00bfe9bc operator delete; d41c,d42c 00bfea14 free | global_* rows | CRT | |
| a3f | 009fe380 | CGraphicInfoFrameCFrameData_CFrameData_009fe380.cpp | ??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z | |
| a94 | 00988020 | CShaderRenderManager_ApplyVertexShader_00988020.cpp | ApplyVertexShader | |
| a9f | 00988290 | CShaderRenderManager_ResetWorldTransform_00988290.cpp | ResetWorldTransform | |
| ad1,ada | 007c2ab0 | CSTLportVectorC3DVector_CSTLportVector_007c2ab0.cpp | CArray<C3DVector>(size) ctor (out of line) | tangents0 at [ebp-64h], tangents1 at [ebp-58h] |
| cd1..d21 | 0098b930/00988a20/00989bf0/00989760/009897c0/0098a760/00988a50 | CShaderRenderManager_SetVSConstants / UpdatePixelShader / UpdatePixelShaderConstants / UpdateAmbient / UpdateFogColour / UpdateLightGlobals / UpdateCombinedProjectionTransform | (PrepareForRender inline) | 00989760's egor_pdb_names entry is a folded unrelated name (CAIStateGroup_...IsRunnable) -- ignore |
| cdb0,d37f | 00bc9b30 | NOT LANDED (manifest FindNode) | ?FindNode@CEngineTreeFilerBase@@AAEPAUCNodeHeader@1@PAU21@PAK@Z | first recursion level inlined at both sites |
| cdfd | 00bf3030 | 00/bf/CEngineTreeFilerBase_AddNewKey_00bf3030.cpp | ?AddNewKey@CEngineTreeFilerBase@@AAEKPAK@Z | **landed spelling WRONG**: `CNodeHeader* AddNewKey(const unsigned int*)`; PDB = private, returns unsigned long, takes `unsigned long*` (draft spelling is the PDB one -> NAME_FIXES candidate) |
| ce39 | 00ac20c0 | CThingFilterIsValid_operator_00ac20c0.cpp (manifest label CThingFilter_IsValid::operator()) | ?IsBusy@CVertexBufferBase@@QBE_NXZ | ICF-folded `mov eax,[ecx]; jmp [eax+0Ch]` thunk; header opt-in FABLE_RETAIL_CVERTEXBUFFERBASE_ISBUSY_SIZEOF |
| ce57,ceb1,d3ff | 00ac20d0 | CThingFilterIsValid_func00ac20d0_00ac20d0.cpp | ?Sizeof@CVertexBufferBase@@QBEKXZ | same opt-in |
| ce8a | 00bf3380 | CEngineVertexBufferPool_BuildNewBuffer_00bf3380.cpp | ?BuildNewBuffer@CEngineVertexBufferPool@@QAEPAV?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@PAKW4EVertexType@@JJKW4EVertexBufferPool@@@Z | landed uses int for the two enums; draft matches PDB |
| cecb | 00ac20a0 | CVertexBuffer_Lock_00ac20a0.cpp | ?Lock@CVertexBufferBase@@QAEPAEKKK@Z | args pushed 0,0,flags -> flags is the FIRST arg; header names params (offset,flags,size) -- the draft's `Lock(GetDynamicVBLockFlags(),0,0)` is byte-right; param naming unverified |
| d209 | 00ac20b0 | CVertexBuffer_Unlock_00ac20b0.cpp | ?Unlock@CVertexBufferBase@@QAEXXZ | |
| d24a..d30d | D3D device vslots +0x104 SetTexture, +0x190 SetStreamSource, +0x144 DrawPrimitive, +0x1a0 SetIndices | inline DrawBuffersNonIndexed / FlushRender | |
| d289 | 00a058c0 | CRenderStateManager_RealiseRenderState_00a058c0.cpp | RealiseRenderState | |
| d410 | 00a05840 | CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | RestoreCaptureBlock | capture-block dtor; runs AFTER ReturnResource, before the two tangent frees and frame dtor |
Other PDB-confirmed inline names (FableWin.exe.signatures.tsv): CEngineTreeFilerBase::FindID `QAEKPAK`, AddKey `QAEKPAK`,
GetGenericObject `IAEPAXK` (protected), SetGenericObject `IAEXKPAX`; CEngineVertexBufferPool::GetBuffer `QAEPAV..W4EVertexType@@JJKW4EVertexBufferPool@@`,
GetKey `QAEXPAKW4EVertexType@@JJKW4EVertexBufferPool@@`; CEnginePrimitiveRenderUtil::GetDynamicVBUsage/Pool/LockFlags `SI` (static __fastcall);
CShaderRenderManager::DrawBuffersNonIndexed (both overloads), CRenderManagerCore::FlushRender `QAEXXZ`, CGraphicInfoFrame::GetFrame `QBE_NAAVCGraphicFrame@@KK`,
CGraphicDataBank::PeekGraphicInfoFrame `QBEPBVCGraphicInfoFrame@@KK`. All agree with the draft's spellings.

## 4. Members
- CEngineInternalPrimitiveWeaponTrail (struct_layouts_egor.tsv, PDB 132 B; retail 0x7c): TrailType +0x48 (EWeaponTrailType), GraphicIndex +0x4c,
  GraphicFrame +0x50, Alpha +0x54 uchar, SkipPoints +0x58, Trail +0x5c CArray<CArray<C3DVector>>, TrailElementCreationTime +0x68, CurrentTime +0x74,
  FallOffTime +0x78 (retail = PDB-4 past Trail: 12-byte CArray). NO shared header (no rebuild/include/engine/CEngineInternalPrimitiveWeaponTrail.h) -> local model OK.
  EWeaponTrailType's real values live nested in engine/CEnginePrimitiveWeaponTrail.h (CEnginePrimitiveWeaponTrail::EWeaponTrailType: WEAPON/PROJECTILE/CREATURE/TRAP); draft invents `WEAPON_TRAIL_TYPE_0`.
- CEnginePrimitiveManagerWeaponTrails (PDB 48 B): VertexShader +0x0c, VertexBufferPool +0x14. No shared header -> local OK.
- CEngineVertexBufferPool (PDB 28 B): vptr, AllocCount +4, FreePoolMem +8, TotalMemAllocated +0xc, ResourceMappingTree +0x10, SizeGranularity +0x18.
  **SHARED HEADERS EXIST and the draft models them locally**: rebuild/include/engine/CEngineVertexBufferPool.h (CEngineDynamic3DResourceManager<T>
  with 3 virtuals GetTotalBytesAllocated/GetTotalBytesFree/FreeMemory, CListHeader {Head,Tail}) and rebuild/include/engine/CEngineTreeFiler.h
  (CEngineTreeFilerBase {BaseNode, KeySize}, CNodeHeader forward-declared private). Neither offers any FABLE_RETAIL_* opt-in yet: the author must add
  `#ifdef FABLE_RETAIL_...` blocks (inline FindNode/FindID/AddKey/Get/SetGenericObject, GetResource/AddNewResource/ReturnResource, GetKey/GetBuffer)
  in hdr/ overlays of those two headers instead of the local classes (strict verifier rejects local models of header classes).
- **Name errors in the draft's local model**: CListHeader members are `Head`/`Tail` (PDB, struct_layouts + shared header), draft `First`/`Last`.
  CNodeHeader layout (28 B: IsLeaf, KeyOffset, KeyBitStart, KeyBitCount, Entry, PrevSibling, NextSibling, ChildListHead, ChildListTail, Parent) matches.
  CEngineDynamic3DResource<T,3>: Key[3] +0x34, PoolNext +0x40 (PDB CVertexBufferWin32 variant 68 B; retail 0x44).
- Other members via opt-ins the draft already enables: CGraphicDataBank GraphicList (+0x1e0/+0x1e4, sizeof CGraphicInfo 0x2c), resource manager +0x954;
  CEnginePrimitiveRenderer DynamicVBUsage +0xc4 / DynamicVBPool +0xc8 / DynamicVBLockFlags +0xcc (FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICVB);
  render core TextureStagesInUse +0x3cb8; capture manager 0x013CB508 +0x2808/+0x2814/+0x3a24.
- Coordinator flagged 4 STALE overlay headers (CSurface, CIVCountedPointer, C2DVector, CRenderStateManager) to re-base by hand.

## 5. Callee visibility
- FindNode 0x00BC9B30 and FindID 0x00BC9B80 (landed, CEngineTreeFilerBase_FindID_00bc9b80.cpp, no callers) sit just before this TU's
  primitive-manager code: the out-of-line copies of inline members; FindID inlined here, FindNode inlined one level (FABLE_FORCEINLINE, recursion = call).
  Copy the FindID row's exact text (named `nodeKey` local, `(node + 1)` casts) -- it byte-matched.
- CShaderRenderManager::DrawBuffersNonIndexed(4-arg) is out of line at 0x00BC9F50 (landed) yet inlined here -> visible inline member.
- CArray<C3DVector>(n) ctor called out of line 007c2ab0 (STLport COMDAT), not inlined; operator new/delete/free out of line.
- CVertexBufferBase Lock/Unlock/IsBusy/Sizeof: out-of-line thunks (FABLE_RETAIL_CVERTEXBUFFERBASE_ISBUSY_SIZEOF for the last two).

## 6. Siblings (same class / neighbourhood)
- 00/bc/CEnginePrimitiveManagerWeaponTrails_RenderSubPrimitive_00bcd490.cpp (caller; capture block + tracked-state idioms).
- 00/bc/CEngineTreeFilerBase_FindID_00bc9b80.cpp (FindNode/FindID text; FABLE_FORCEINLINE FindNode).
- 00/bc/CShaderRenderManager_DrawBuffersNonIndexed_00bc9f50.cpp (SelectVertexBuffer inline, device re-read).
- 00/bc/CEnginePrimitiveManagerWeaponTrails_RenderPrimitive_00bcc700.cpp (`#define FABLE_RETAIL_CARRAY`, Trail[0] through const ref; `// Flags: none`).
- 00/bc/CEngineInternalPrimitiveWeaponTrail_Update_00bcd660.cpp (layout evidence for the 0x7c trail object).
- Family overlays: /opt/fable_lane/w22/agent2048, agent2059, agent2060 (see HINTS.md). RenderSpriteTrail 0x00BCA950 (not landed) used the same pool model.
None of the bc rows carries pragmas; no `#pragma optimize` needed (frame shape is plain /O2).

## 7. Draft model errors / residues (c29 = 953 equal, 3207 v 3205)
- Control flow, loops and calls already match; residue is almost all FRAME SLOT ASSIGNMENT (VC7.1 packs slots by scope/lifetime):
  retail [ebp+34h] is shared by GFFloatToLongFloor's 1.0f constant (00bcc971) and vertex_count (00bccace, reread at 00bccd40) -> in retail
  vertex_count lives in a slot whose earlier tenant ended -- ours puts both at 0x54. fraction retail 0x38 (ours 0x34). The FIRST tangent's
  scaled temp C3DVector is at 0x3c/0x40/0x44 (00bccb6b..86) = the same slots as key[3] in GetBuffer (0x3c..0x44, 00bccd71..7f); the second
  tangent's temp at 0x28/0x2c/0x30 and Z spill at [ebp-38h]. Ours shifts these by 4 (key 0x38..0x40). So one 4-byte local is missing/ordered
  differently below 0x3c: try scoping (tangent loop temps inside its block, `vertex_count` computed later / declared in a narrower scope) rather than texts.
- [ebp+48h] retail = the frame-data pointer (00bcc945), ours 0x44; [ebp+5ch]<->[ebp+60h] swap (GFFloatToLongFloor sum vs `usage` 00bccd4a).
- GetBuffer FindNode push: retail `lea edx,[key]` (00bccda9), ours ecx.
- ReturnResource tail 00bcd32d..3df: retail tree in ebx (spilled to [ebp+7Ch], the dead arg slot), key ptr (resource+0x34) in edi, node in [ebp+7Ch];
  leaf path `mov [ebp+7Ch],eax ; jmp` laid out first. `id > 0` is an unsigned `jbe` (draft right). GetGenericObject null test folded away there.
- Naming: CListHeader Head/Tail; EWeaponTrailType nesting; local classes duplicating CEngineTreeFiler.h / CEngineVertexBufferPool.h must move to opt-ins.
- Missing `// Retail-asm:` lines for 00bcc96a..92 (Floor) and the GFFloatToLongNear site at 00bccf73.
