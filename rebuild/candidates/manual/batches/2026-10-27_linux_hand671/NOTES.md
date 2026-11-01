# Hand chunk 671 (CTCHitOnCollision .. CEngineShadowRenderer): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (each object's relocation table checked with `objdump -dr`: every call / data target is the
intended PDB symbol) and pass their behaviour tests under g++ and the lane's VC7.1 cl.exe; the read-only vland run reports 10
WINS. No row was parked, so the 12-probe parking rule never applied (probe counts: 1,5,1,1,2,1,1,1,3,1; logs in probe/).
No row needed `/EHsc`, a pragma, `__forceinline` or a flag.

## What matched (10)
- **CTCHitOnCollision::~CTCHitOnCollision 0x007C8560** (catalog: CTCInventoryItem::OnDie) -- compiler-generated (no vtable
  store): STLport vector<CThingGameObject*> CollidedList `if (_M_start) free`, jmp ~CTCBase. Emitted by defining the PDB ctor
  ??0CTCHitOnCollision@@AAE@AAVCThing@@@Z (private) with an empty body.
- **CNavSwitchableLeafNode::~CNavSwitchableLeafNode 0x00A84F00** (catalog: CTCInventoryItem::OnDie) -- implicit; UniqueIDsOpen
  CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<..> > = STLport vector<pair> + Compare + Dirty (landed Insert model),
  jmp ~CNavNavigableLeafNode. PDB ctor (C2DVector const&, uchar x3, vector<unsigned __int64> const&) emits it.
- **C3DAnimFileBoneMaskChunk::~C3DAnimFileBoneMaskChunk 0x00A9A7C0** (catalog: CTCInventoryItem::OnDie) -- implicit; CArray<bool>
  EnableMask (retail: a byte-per-element 12-byte vector, NOT a bit vector: the ctor zeroes 3 words and WriteToFile reads bytes),
  jmp ~CChunkedFileChunk. PDB ctor ??0C3DAnimFileBoneMaskChunk@@QAE@XZ emits it.
- **CStaticFontBank::~CStaticFontBank 0x00AB7A70** -- `if (Glyphs) { delete [] Glyphs; Glyphs = 0; }` over GlyphBanks[1024];
  then `if (GFGetSystemManager() && ->IsValid() && ->GetDisplayManager()) { delete PTexture; PTexture = 0; }`. The PDB accessor
  CSystemManager::GetDisplayManager() (inline `return PDisplayManager.Get();`) is what matches; PDisplayManager.Get() written at
  the call site leaves a dead `add eax,60h` (probe 1). CFontBank's inline user dtor gives the 0x0129AB38 store + ~CCharString.
- **CEngineLocalDetailGenerator::~CEngineLocalDetailGenerator 0x00B59BE0** (catalog: AddLocalDetailDynamic) -- user dtor:
  `for (long i = 0; i < g_pEngineWorldMap->GetMapCount(); ++i) if (GetMap(i)) { delete GetMap(i)->LocalDetailCacheMap;
  GetMap(i)->LocalDetailCacheMap = 0; }` with the PDB accessors (long count: signed `jl`; re-evaluation gives the global reload);
  ObjectCollectionTypes vector freed; jmp ~CEngineComponent. (Not CEngineLocalDetailGenerator::DeleteMap inlined: DeleteMap
  0x00B59840 zeroes inside the null test, this body zeroes unconditionally.)
- **CEngineScreenEffectColourFilter::~CEngineScreenEffectColourFilter 0x00B5A0D0** -- empty user body; members in reverse order:
  CPixelShader / CVertexShader inline user dtors (local Data copy, `dec ; jne ; call [vt]` with 1) and the protected empty
  ~CShaderBase store; CRenderTarget implicit (no store): ZBufferSurface, RenderToSurface (CSurface inline user dtor =
  Uninitialise), ~CBaseClass.
- **CInputEvent::GetChecksum 0x00A04160** (catalog: _Dest_val<allocator<CMacro>,CMacro>) -- `unsigned long checksum = Key +
  DeviceType + DeviceNumber + Type;` then `checksum += a + b (+ c);` for HeldMouseMovement, MouseMovement, MousePos, JoystickPos.
  VC7.1 adds the floats onto the converted checksum last-first; each `+=` ends in __ftol2 (the last as the tail jump).
- **NScript::CV_SwordInTheStoneScript::~CV_SwordInTheStoneScript 0x00ED7640** -- implicit: ~CTimer (DeregisterTimer slot 88),
  the two CScriptThing members (engine/CScriptThing.h with an inline ~CScriptThing, engine/CCountedPointer.h PImp release),
  jmp ~CScriptBase; CLevelScriptBase<CQ_SunnyvaleMasterData> without a dtor declaration (chunks 664/665). PDB ctor emits it.
- **C3DMesh2::~C3DMesh2 0x00A895F0** -- `if (MemoryHandle != -1) { GFExplicitDelete(Materials, MaterialCount);
  GFExplicitDelete(Primitives, PrimitiveCount); MemoryPool->FreeResource(MemoryHandle); MemoryHandle = -1; }` + the two other
  handles. GFExplicitDelete<T>(T*, unsigned long) is the Ego_r/FableWin template (??$GFExplicitDelete@VC3DMeshMaterial@@@@YIX..K@Z),
  defined in the TU; VC7.1 inlines it and its COMDATs come out under the exact PDB spellings. The loops written on the members
  re-read them each pass (221v175, probe 1).
- **CEngineShadowRenderer::~CEngineShadowRenderer 0x00B4D210** -- body `OnReleaseDefaultPoolResources();` (virtual, bound directly
  in the dtor); member dtors in reverse Ego_r order (CTexture inline dtor, CSurface user dtor, CRenderTarget implicit), then
  ~CRenderStateCaptureBlock `if (Open) { Open = false; g_pRenderStateManager->RestoreCaptureBlock(); }` (plain inline).

## Parked for Fable 5.1 (0)
(none)

## Skipped
(none)

## Label corrections
- 0x007C8560 CTCInventoryItem::OnDie -> CTCHitOnCollision::~CTCHitOnCollision
- 0x00A84F00 CTCInventoryItem::OnDie -> CNavSwitchableLeafNode::~CNavSwitchableLeafNode
- 0x00A9A7C0 CTCInventoryItem::OnDie -> C3DAnimFileBoneMaskChunk::~C3DAnimFileBoneMaskChunk
- 0x00B59BE0 CEngineLocalDetailGenerator::AddLocalDetailDynamic -> CEngineLocalDetailGenerator::~CEngineLocalDetailGenerator
- 0x00A04160 _Dest_val<std::_Dest_val<std::allocator<CMacro>,CMacro> -> CInputEvent::GetChecksum
- 0x00BDED60 CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip -> CLocalDetailCacheMap::~CLocalDetailCacheMap (callee)
- 0x0099A330 CTCInventoryBase::GetCanItemBeQuickAssigned / GetMember_byte0x4 -> CInitBaseClass::IsValid (callee)
- 0x00ED7620 CVtable_012f301c ??_G -> NScript::CV_SwordInTheStoneScript ??_G; the eight other slot-0 ??_G rows (all landed as
  `*_scalar_deleting_destructor` / `ScalarDeletingDestructor` stand-ins) get their PDB ??_G spellings (NAME_FIXES.tsv).
- Destructor labels in this chunk: 5 of 5 right (~CStaticFontBank, ~CEngineScreenEffectColourFilter, ~CV_SwordInTheStoneScript,
  ~C3DMesh2, ~CEngineShadowRenderer); all five non-dtor labels were wrong.

## Proven
- 0x007C8560 ??1CTCHitOnCollision@@UAE@XZ -- sole caller slot-0 ??_G 0x007C8540 of vtable 0x0126E7A4 (RTTI CTCHitOnCollision : CTCBase); no store (implicit); Ego_r member CollidedList at +0x0C; Ego_r.pdb name set
- 0x007C8540 ??_GCTCHitOnCollision@@UAEPAXI@Z -- slot 0 of 0x0126E7A4; Ego_r.pdb name set
- 0x00A84F00 ??1CNavSwitchableLeafNode@@UAE@XZ -- sole caller slot-0 ??_G 0x00A84EE0 of vtable 0x0129DE04 (RTTI CNavSwitchableLeafNode : CNavNavigableLeafNode); UniqueIDsOpen at +0x20; Ego_r.pdb name set
- 0x00A84EE0 ??_GCNavSwitchableLeafNode@@UAEPAXI@Z -- slot 0 of 0x0129DE04
- 0x00A76A80 ??1CNavNavigableLeafNode@@UAE@XZ -- called by ??_G 0x00A80340 (slot 0 of 0x0129DDA4, CNavNavigableLeafNode) and tail of 0x00A84F00
- 0x00A80340 ??_GCNavNavigableLeafNode@@UAEPAXI@Z -- slot 0 of 0x0129DDA4
- 0x00A9A7C0 ??1C3DAnimFileBoneMaskChunk@@UAE@XZ -- sole caller slot-0 ??_G 0x00AB0B50 of vtable 0x0129E980 (RTTI C3DAnimFileBoneMaskChunk : CChunkedFileChunk); EnableMask at +0x10; Ego_r.pdb name set
- 0x00AB0B50 ??_GC3DAnimFileBoneMaskChunk@@UAEPAXI@Z -- slot 0 of 0x0129E980
- 0x00AA75C0 ??1CChunkedFileChunk@@UAE@XZ -- stores CChunkedFileChunk vtable 0x0129E474, jmp ~CBaseClassNonCopyable; PDB virtual dtor
- 0x00AB7A70 ??1CStaticFontBank@@UAE@XZ -- first store vtable 0x0129EAD4 (RTTI CStaticFontBank); sole caller ??_G 0x00AB9990 (slot 0); Ego_r/FableWin name set
- 0x00AB9990 ??_GCStaticFontBank@@UAEPAXI@Z -- slot 0 of 0x0129EAD4
- 0x00B59BE0 ??1CEngineLocalDetailGenerator@@UAE@XZ -- first store vtable 0x012A2588 (RTTI CEngineLocalDetailGenerator : CEngineComponent); sole caller ??_G 0x00B5A090 (slot 0); Ego_r/FableWin name set
- 0x00B5A090 ??_GCEngineLocalDetailGenerator@@UAEPAXI@Z -- slot 0 of 0x012A2588
- 0x00BDED60 ??1CLocalDetailCacheMap@@QAE@XZ -- non-virtual dtor called before operator delete of CEngineMap::LocalDetailCacheMap in DeleteMap 0x00B59840 and in 0x00B59BE0; PDB QAE
- 0x00B5A0D0 ??1CEngineScreenEffectColourFilter@@UAE@XZ -- first store vtable 0x012A25CC (RTTI ... : CEngineComponent); sole caller ??_G 0x00B5BB00 (slot 0); members at Ego_r offsets
- 0x00B5BB00 ??_GCEngineScreenEffectColourFilter@@UAEPAXI@Z -- slot 0 of 0x012A25CC
- 0x00A04160 ?GetChecksum@CInputEvent@@QBEKXZ -- sole caller CInputManager::CalculateInternalChecksum 0x009F5170 sums it over the CInputEvent EventStore (0x34 stride); Ego_r/FableWin name set
- 0x00ED7640 ??1CV_SwordInTheStoneScript@NScript@@UAE@XZ -- slot-0 ??_G 0x00ED7620 of vtable 0x012F301C, stored only by ctor 0x00ED4AA0 <- Alloc 0x00ED75F0 (new 0x80) <- registration 0x00CD968A after "V_SwordInTheStone" (0x012C5B20) / "S_VSITS" (0x012F763C); Ego_r/FableWin name set
- 0x00ED7620 ??_GCV_SwordInTheStoneScript@NScript@@UAEPAXI@Z -- slot 0 of 0x012F301C
- 0x00ED4AA0 ??0CV_SwordInTheStoneScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z -- stores 0x012F301C; called only by the Alloc; PDB name set
- 0x00ED75F0 ?Alloc@CV_SwordInTheStoneScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z -- new 0x80 + ctor; stored by the registration with "V_SwordInTheStone"
- 0x00A895F0 ??1C3DMesh2@@UAE@XZ -- first store vtable 0x0129DEF0 (RTTI C3DMesh2 : C3DMeshStats); sole caller ??_G 0x00A8D700 (slot 0); tail ??1C3DMeshStats
- 0x00A8D700 ??_GC3DMesh2@@UAEPAXI@Z -- slot 0 of 0x0129DEF0, scalar-deleting body (PDB sets list ??_EC3DMesh2 only: confirm at landing)
- 0x00B4D210 ??1CEngineShadowRenderer@@UAE@XZ -- first store vtable 0x012A2374 (RTTI ... : CEngineComponent); sole caller ??_G 0x00B50B80 (slot 0); Ego_r offsets
- 0x00B50B80 ??_GCEngineShadowRenderer@@UAEPAXI@Z -- slot 0 of 0x012A2374
- 0x0099A330 ?IsValid@CInitBaseClass@@QBE_NXZ -- `mov al,[ecx+4]`; called on the CSystemManager (RTTI : CInitBaseClass); the class's only bool getter in both PDBs
- 0x00A62ED0 ?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z -- called on C3DMeshStats::MemoryPool with a handle; PDB name set

Descriptive names: none for functions. Data-symbol spellings g_pEngineWorldMap (0x01436E8C), g_pRenderStateManager (0x013CB508)
and g_pGameScriptInterface (0x0143E8F8) are the established landed spellings (no PDB data-symbol dump). The CInputEvent union arm
for the +0x00 C3DVector is named HeldMouseMovement from the PDB getter + SetAsLeftMouseButtonHeld's store (evidence in source).

## Header debt
engine/CTCBase.h, CChunkedFileChunk.h, C3DAnimFileBoneMaskChunk.h, CNavSwitchableLeafNode.h, CNavNavigableLeafNode.h,
CStaticFontBank.h, CFontBank.h, CTexture.h, CEngineComponent.h, CEngineWorldMap.h, CLocalDetailCacheMap.h,
CEngineScreenEffectColourFilter.h, CEngineShadowRenderer.h, CSurface.h, CRenderStateManager.h, CMovableResource.h,
CMovableResourceMemoryPool.h are flattened (vptr as data / byte-array members / no member functions), so every row carries a
local polymorphic model with the PDB member names (each source names the header in a `// Layout:` line). engine/CLevelScriptBase.h
still declares `virtual ~CLevelScriptBase();` (retail's is implicit; same debt as chunks 664/665). engine/CCountedPointer.h,
CBaseClass.h, CBaseClassNonCopyable.h, CScriptThing.h, CScriptBase.h, CTimer.h, CGameScriptInterface.h used as is.
