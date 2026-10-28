# Hand chunk 519 (CNavNavigableLeafNode .. CLocalDetailPrimitiveRepeatedMesh): 36 of 40 byte-exact

7 MATCH + 29 RELOCATION_MATCH, no retail_asm rows, 4 parked, 0 skipped. Every src/ file grades clean in gall.sh (grades.txt),
its test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and the vland dry run reports WINS: 36 with no
RELOC_CONFLICT / RELOC_IDENTITY (only RELOC_NAME notes where the catalog label differs from the bound std/PDB symbol).

## What matched (36)
- CNavNavigableLeafNode: 00a787d0 AddToIndexTable -- `table.insert(std::make_pair(this, index)); ++index;` with the landed
  0x00A789F0 model (make_pair converted by the pair template ctor); the leaf only resolved after the PDB class keys
  (`class map`, `class allocator`: V, not U).
- C3DAnimationManager: 00aa0f60 ctor -- CBaseClassNonCopyable base, MeshBank(0), the real STLport _Vector_base(alloc)
  ctor, two CSimpleMemoryPool(0, 0x1000) members (retail offsets +0x14/+0x30: 12-byte vector).
- CChunkedFileSuperChunk: 00aa8400 RemoveAllSubChunks -- /Gr TU; unparent loop then STLport `clear()` =
  `erase(begin(), end())` (out-of-line __copy 0x00AA8F40 and __destroy_aux 0x00AA9140 under their bound std spellings;
  the two empty tags share the high byte of the `push ecx` slot).
- C3DMeshFileVerticesChunk: 00abbd70 GetChunkInfo -- /GS; `char buffer[128]; sprintf(buffer, "Number of vertices: %u",
  VertexList.size()); return CCharString(buffer);` (literal read from the retail image at 0x0129EBF4).
- CEnginePrimitive2DParticleGroup / CEnginePrimitiveParticleMeshGroup: 00af0880 / 00afead0 ctors (MATCH) -- the
  CEnginePrimitive base of the landed 0x00AF08D0 (type, interpolation set 0, three channel bytes), implicit bitwise
  CMatrix3x4 copy = `rep movsd` x 0xC. Real enum parameter types give the PDB mangling.
- CEngine: 00b25d20 / 00b25d50 PeekGlobalDiffuse/AmbientColour -- `return g_pEngineLightingManager->PeekGlobal...();`,
  the manager's inline accessor returning `ThemeChannels[0].DiffuseColour` / `.AmbientColour` by reference.
- CCompositeSurfaceCache: 00b2c400 ctor -- CResource base; CEngineTextureMorph default ctors zero 5 words each (12-byte
  vector); MipmapLevel and TimeStamp untouched; CManagedTexture inline ctor (CBaseClass() call + vptr + null Data).
- CEnginePrimitiveOcclusionManager / Info: 00b2f9f0 SetAsOccluded, 00b2fa90 Info ctor -- inline ClearHandles (STLport
  erase over __copy_trivial / memmove); TestFrame in the init list from g_pOcclusionManager->CurrentFrame.
- CEnginePrimitiveAttachmentManager: 00b3a0d0 UpdateSceneForPreload -- the test is the handle's inline
  `GetReferenceCount() > 1` over `CIVCountedPointer::GetRefCount()` (`Object ? Object->IVRefCount : 0`): the
  conditional puts the pointer in eax and retail's `mov ecx,eax` follows; the IsNull()+IVRefCount forms load ecx
  directly (3 grades). Enable is bit 0 of the +0x34 bit-field byte (`or byte,bl`).
- CEngineClothSimulationMeshBase: 00b80f90 GetGravityVector (named factor local; same-TU noinline ConvertVectorToLocal),
  00b84120 ??_G (user dtor `{ ClearParticleSims(); }` + noinline __destroy_aux, chaining to ~CBaseClassNonCopyable).
- CEnginePrimitiveSimulationInfo: 00b812c0 FindSimulation (MATCH), 00b81e00 ctor.
- CEngineSubPrimitiveRenderer: 00b84990 SortList -- /Gr; `if (list.empty()) return;` where `empty()` compares
  `_M_start == _M_finish` (begin loaded first; `begin() == end()` loads end first), STLport stable_sort inline wrapper
  -> __stable_sort_aux 0x00B86710, sort 0x00B868C0 under 256 elements, else RadixSort 0x00B847D0.
- CEnginePrimitiveTypeInfo: 00b8faa0 ctor (loop zero of 47 managers; memset misorders), 00b8fb60 implicit ??_G (emitted by
  the real ctor in the TU).
- CEngineSubPrimitiveMeshBase: 00b98b40 ctor (inline base ctor Parent/NextSubPrimitive).
- CEngineInternalPrimitiveStaticMesh / ParticleMesh: 00b9d1e0 / 00bc25b0 GetWorldPosition (MATCH) -- virtual local
  model; `position = ObjectTransform.GetTranslation(); return true;` with C3DVector's user copy ctor.
- CEngineScenePrimitiveStatsEnumerator: 00baca20 ctor -- init list; C2DBoxF needs its user copy ctor.
- CEngineClothRenderer: 00bb1ef0 FrameStart -- `GetAsCArray()` as `size() > 0 ? start : zero-size array` (a bare
  `size() ?` folds to `test esi,0FFFFFFF8h`).
- CEngineParticleSimulationRenderData: 00bb6520 ??_G -- via `vector destructor iterator` 0x00401400.
- CEngineInternalPrimitiveChanger: 00bc5620 / 00bc5660 SetAnimation / SetPosition (MATCH) -- count local, reference to
  the handle, IsNull()/GetData() inline (a pointer static_cast adds `mov eax,ecx`).
- CEnginePrimitiveGroupManager: 00bc6210 RenderPrimitiveToShadowBuffer (signed max), 00bc6270 PreloadPrimitive (`bool
  result = false;` declared first gives the `push ecx` frame byte).
- CEngineSubPrimitiveText: 00bc7a80 ??_G -- static __fastcall class operator delete (FableWin ??3CEngineSubPrimitiveBase@@SIXPAX@Z).
- CLocalDetailPrimitiveMesh / RepeatedMesh: 00bdfb50 / 00bfaa80 ctors -- base ctor, then retail-order zero stores.
- CEngineStaticMapFileBlockManager: 00be0420 SetBlockToHighPriority -- `CFileBlock& block = Blocks.find(index)->second;`
  (real STLport _Rb_tree find through the hidden iterator, no end() test), ChangeLoadingPriority(global word 0x0139A728).
- CEnginePrimitive2DViewportManager: 00be2190 RenderSubPrimitive -- implicit C2DAlphaMaskInfo copy (`rep movsd` x 0xB),
  Render(), then the inline `C2DAlphaMaskInfo::Reset()` (Ego_r.pdb): `AlphaMaskPositionFactors = C4DVector(0,0,0,0)`
  keeps the dead 16-byte temporary's zero stores (the old "dead temp" park).
- CEngineTreeFilerBase: 00bf3180 Initialise -- inline AllocNode branch root, DeleteTree under its PDB spelling.
- CEngineDynamic3DResource<CVertexBufferWin32,3>: 00bf3250 ctor -- template on T with Key[N], explicit instantiation.

## Parked for Fable 5.1 (4)
- 00b2f760 CEnginePrimitiveOcclusionManager::CEnginePrimitiveOcclusionManager DIFFER(101v95): retail reloads only the X word of the C3DVector(1,0,0) temporary (Y/Z forwarded zeros), VC reloads all three | tried: assignment from a temporary, init-list, copy-ctor, pack(1), implicit operator= (the last four fold the temporary away entirely) | next: a named C3DVector local assigned memberwise through a user operator= taking the X by reference
- 00b84190 CEngineClothSimulationAnimatedMesh::`scalar deleting destructor' DIFFER(36v75): retail inlines the whole ~CEngineClothSimulationMeshBase body (ClearParticleSims, __destroy_aux, free, ~CBaseClassNonCopyable call) into the derived ??_G; ours calls ??1CEngineClothSimulationAnimatedMesh / ??1CEngineClothSimulationMeshBase | tried: implicit derived dtor, empty inline user dtor, inline / __forceinline base dtor | next: lane gap: cross-TU dtor inlining (the base dtor body defined inline in the class with the ??_G-emitting ctor of the derived class in the same TU)
- 00bba3e0 CEngineInternalPrimitiveDecal::GetBoundingSphereWorldSpace DIFFER(87v87): `faddp` / `fsqrt` scheduled one slot later than retail (before `mov [esp+8],eax` / `mov [ecx],edx`) | tried: named locals, inline C2DVector length helper, single expression | next: lane gap: x87 scheduling
- 00bc9b30 CEngineTreeFilerBase::FindNode DIFFER(66v66): key and child index registers swapped (retail key esi / index edx) | tried: while, do-while, for(;;), named index/mask/word locals, inline GetIndex | next: lane gap: register allocation

## Skipped
None (no split fragments in this chunk).

## Names
### Proven
00a787d0
00aa0f60
00aa8400
00abbd70
00af0880
00afead0
00b25d20
00b25d50
00b2c400
00b2f760
00b2f9f0
00b2fa90
00b3a0d0
00b80f90
00b812c0
00b81e00
00b84120
00b84190
00b84990
00b8faa0
00b8fb60
00b98b40
00b9d1e0
00baca20
00bb1ef0
00bb6520
00bba3e0
00bc25b0
00bc5620
00bc5660
00bc6210
00bc6270
00bc7a80
00bc9b30
00bdfb50
00be0420
00be2190
00bf3180
00bf3250
00bfaa80
### Probable
(none)
### Descriptive
0139a728 (global word, `g_HighLoadingPriority`: the EThreadedFilePriority passed by SetBlockToHighPriority; no binding or PDB name exists)

## Label corrections
- 0x00AF0880: catalog "C3DMeshFileBoneChunk::SetLocalizationMatrix", bound "CEnginePrimitiveParticleGroup2D(long, ...)" -> ??0CEnginePrimitive2DParticleGroup@@QAE@W4EEngine2DLayer@@ABVCMatrix3x4@@ABVC2DVector@@_N3@Z.
- 0x00AFEAD0: catalog "C3DMeshFileBoneChunk::SetLocalizationMatrix", bound "SMeshCreateInfo(long, ...)" -> ??0CEnginePrimitiveParticleMeshGroup@@QAE@KABVCMatrix3x4@@W4EEnginePrimitiveBlendMode@@W4EEnginePrimitiveBlendOp@@@Z.
- 0x00BE0420: bound "?PrioritiseBlock@CEngineStaticMapFileBlockManager@@QAEXJ@Z" -> ?SetBlockToHighPriority@CEngineStaticMapFileBlockManager@@QAEXJ@Z.
- 0x00BE08E0 / 0x00BE0930: bound "CRetailBlockSet::FindInsertionPoint" / landed "Find" are STLport _Rb_tree<long, pair<const long, CEngineStaticMapFileBlockManager::CFileBlock>, ...>::find (non-const / const).
- 0x00AA8400: bound "?RemoveAllSubChunks@CChunkedFileSuperChunk_Retail@@QAEXXZ" -> ?RemoveAllSubChunks@CChunkedFileSuperChunk@@QAEXXZ.
- 0x0099A300: catalog "CBaseClass::~CBaseClass" -> ??1CBaseClassNonCopyable@@MAE@XZ (0x0099A2E0 is ??1CBaseClass@@UAE@XZ; identical bodies, separate COMDATs).
- 0x00B2F760: "CCompressedMemoryDataInputStream" ctor -> CEnginePrimitiveOcclusionManager ctor.
- 0x00B98B40: "CMemoryDataInputStream" ctor -> CEngineSubPrimitiveMeshBase ctor.
- 0x00B8FB60: "CEngineSubPrimitiveText scalar deleting destructor" -> ??_GCEnginePrimitiveTypeInfo@@UAEPAXI@Z.
- 0x00BDFB50: "CLocalDetailPrimitiveRepeatedMesh" ctor -> CLocalDetailPrimitiveMesh ctor.
- 0x00BF3250: "CDataInputStream" ctor -> CEngineDynamic3DResource<CVertexBufferWin32,3> ctor.
- 0x00B84190: "~CEngineClothSimulationMeshBase_Alt" -> ??_GCEngineClothSimulationAnimatedMesh@@UAEPAXI@Z.
- 0x00BB6520: "vfunc_0" -> ??_GCEngineParticleSimulationRenderData@@UAEPAXI@Z.
- 0x00B564C0: catalog "CEngineDynamic3DResource<...>::~" -> ~CVertexBufferWin32.
- 0x00B2FA40: landed "CV_BanditTollScript::Init" is CEnginePrimitiveOcclusionManager::SetAsVisible (uses an invented CRetailVector).
- 0x00BC7A50: the landed ctor model is invented (CCharString where retail has CWideString Text).
- 0x01436E5C is the CEngineOcclusionManager pointer; 0x01436E68 the CEngineSimulationManager pointer; 0x01436E9C the
  CEngineLightingManager pointer (several landed spellings: CMistParamsView, CEngineLightingView, ...).
- 0x00BEB550: label "Nop_ret0" is ??3CEngineSubPrimitiveBase@@SIXPAX@Z (FableWin.pdb).

## Header debt
- engine/CChunkedFileSuperChunk.h, C3DMeshFileVerticesChunk.h, CEngineLightingManager.h, CEnginePrimitiveAttachmentManager.h,
  CEnginePrimitive2DViewportManager.h, CEngineSubPrimitiveRenderer.h, CEnginePrimitiveTypeInfo.h, CEngineInternalPrimitiveStaticMesh.h,
  CEngineInternalPrimitiveParticleMesh.h: generated flat layouts (vptr as a data member, STLport containers and bit-fields
  as opaque bytes) cannot carry the virtual / inheritance / container shapes these bodies need, so typed local models
  sit beside a donor include (`#define X X_HeaderLayout`) or carry a `// Layout:` / `// Header debt:` comment.
- vland SHARED_HEADER_AVAILABLE notes for CANavQuadTreeNode, CSimpleMemoryPool, CIEngine, CResource, CGraphicFrame,
  CIVCountedPointeeBase, CEngineComponent, CVertexShader, CEngineInternalPrimitiveBase, CMovableResource: same reason.
- C3DAnimationManager, CCompositeSurfaceCache, CEngineTextureMorph: PDB offsets are 4 (per container) above retail
  (Ego_r 16-byte vector vs retail 12).
