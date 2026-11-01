# Hand chunk 668 (CEngineMap .. CNavQuadTreeNode): 10 of 10 byte-exact

All ten never-authored rows matched (1 MATCH, 9 RELOCATION_MATCH), each in 1-3 probes; every call target was checked
against retail with `w22/calls.py` / `objdump -r`, and both switch jump tables (0x00558C70, 0x00A75E30) were paired with
the retail tables by hand. Seven of the ten catalog labels were wrong or placeholders; every row's name is now proven.

## Proven
- 0x00B42A90 ??1CEngineMap@@QAE@XZ -- callees CEngineMap::CloseStaticMap 0x00B3EF40, CEngineLandscapeRenderer::DeleteMap(CEngineMap*) 0x00B67180 and CEngineLocalDetailGenerator::DeleteMap(CEngineMap*) 0x00B59840, all passed `this`; the members it destroys sit at the Ego_r CEngineMap offsets (StaticMapName +0x1C, AsyncFileEntry +0x3C, FileBlockManager +0x44); callers are ??_GCEngineMap 0x00B44420 and the landed CEngineWorldMap::DeleteMap 0x00B42B10 (`delete MapList[index]`); name in both Ego_r and FableWin.
- 0x00991880 ??1CASoundManager@@UAE@XZ -- its first store is vtable 0x0129A574 (RTTI .?AVCASoundManager@@); called by that vtable's slot-0 ??_G 0x009937B0 and by the tail jump of the derived CSoundManagerLH destructor 0x00A3EE43; ends in ??1CBaseClassNonCopyable@@MAE@XZ (RTTI base). Neither PDB has the sound module, so the name comes from RTTI plus the dtor shape.
- 0x008D7A90 ?OnDeactivate@CModeGuildmasterPowerUpShield@NCreatureMode@@UAEXXZ -- the only reference is slot 6 of vtable 0x012685F0 (RTTI ..PowerUpShield). Slot 6 is OnDeactivate (landed Heal sibling 0x008D69A0). Ego_r donor 0x00818E40.
- 0x008D7B20 ?OnDeactivate@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UAEXXZ -- the only reference is slot 6 of vtable 0x01268624 (RTTI ..PowerUpSpeed). Ego_r donor 0x00818ED0, 0x90 after Shield's, the same gap as in retail.
- 0x00B52870 ?FadeRainToTarget@CEngineWeatherRenderer@@AAEXM@Z -- Ego_r name, private. Tail jump to StartRaining 0x00B51EA0. Called from WorldFrameUpdate's rain block (0x00B52BA6 / 0x00B52BD9).
- 0x00B52900 ?FadeSnowToTarget@CEngineWeatherRenderer@@AAEXM@Z -- Ego_r name, private, after FadeRain in the donor as in retail. Tail jump to StartSnowing 0x00B51F00. Called from WorldFrameUpdate's snow block (0x00B52C1D / 0x00B52C50).
- 0x00558C70 ?ChangeState@CNavButton@NUISystem@@UAEXK@Z -- slot 48 of vtable 0x0124B04C (RTTI NUISystem::CNavButton), the UI family's ChangeState slot. Its first call is the qualified base CChangingStateComponent::ChangeState 0x0052CF40. Name in Ego_r and FableWin.
- 0x00A3BD00 ?QueryInterface@CBaseVideoRenderer@@UAGJABU_GUID@@PAPAX@Z -- slot 0 of the IUnknown-derived vtables 0x0129D04C (CTextureRenderer) and 0x012BF598 (CBaseVideoRenderer's own). The body is DECLARE_IUNKNOWN's GetOwner()->QueryInterface. The retail adjustor thunks (-4 / -0xD4 / -0xD8) are exactly the W3 / WNE@ / WNI@ thunks the compiler emits for CBaseVideoRenderer::QueryInterface.
- 0x009C2FD0 ??1CRenderManager2D@@UAE@XZ -- called by ??_GCRenderManager2D 0x009E16E0 and ??_GCRenderManager 0x009C3100 (RTTI CRenderManager : CRenderManager2D : CRenderManagerCore). Tail jump to ~CRenderManagerCore 0x00A0B060. It stores no vtable of its own, so it is compiler-generated. Members are Ego_r CRenderManager2D. Name in Ego_r and FableWin.
- 0x00A75E30 ?SetUpNeighbours@CNavQuadTreeNode@@MAEXW4ENavQuadTreeNeighbours@@PAVCANavQuadTreeNode@@@Z -- slot 17 of vtable 0x0129DCD4 (RTTI CNavQuadTreeNode). It calls slot 16 SetUpNeighboursWithLeaf (landed 0x00A75F10) and itself. Ego_r donor 0x00BBBB20, directly before SetUpNeighboursWithLeaf 0x00BBBC10, the retail order.
- Proven callees: 0x009C2F50 ??1?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@std@@@std@@QAE@XZ; 0x00A620D0 ??1CShaderDataBank@@UAE@XZ (stores vtable 0x0129D644, RTTI CShaderDataBank); 0x00B44420 ??_GCEngineMap@@QAEPAXI@Z (FableWin); 0x00558C10 ?IsAcceptButton@CNavButton@NUISystem@@QBE_NXZ (const in both PDBs); 0x00B59840 ?DeleteMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z (PDB name; catalog "CEngineResourceManager::DestroyMapStrip_00b59840").

## What matched (10)
- CEngineMap (0x00B42A90): a user destructor with four statements. The members' implicit destructors follow: the manager's dtor, out of line in retail, is declared without a body; engine/CCountedPointer.h gives the release with immediate zero stores; CCharString is the tail call. First try.
- CASoundManager (0x00991880): a user-declared empty virtual dtor keeps the vtable store. Real STLport 4.6 vector text inside CVectorMap<long, CSoundBankRecord> plus engine/CCountedPointer.h give retail's shared zero register. First try.
- NCreatureMode::CModeGuildmasterPowerUpShield / Speed (0x008D7A90 / 0x008D7B20): `long count = v.size(); for (long i...) if (IS_THING_ALIVE(v[i])) v[i]->Kill(true);` then the Heal sibling's SuspendAI(false) and base tail. The signed count gives `sar ; jle/jl`. Only mistake on the way: CThing members must start after CClass's padded 0x14, not at 0x11.
- CEngineWeatherRenderer Fade{Rain,Snow}ToTarget (0x00B52870 / 0x00B52900): an if / else-if chain on a float local. `(float)fabs(...)` turns retail's dword compare into `fcomp dword` (plain fabs compares a double). VC duplicates the tail call into each arm on its own.
- NUISystem::CNavButton::ChangeState (0x00558C70): the qualified base call, then a switch whose 1/5 and 0/6 case pairs give retail's index table {0,1,2,2,2,1,0}. `(CObserver*)this` is the null-tested +4 conversion.
- CBaseVideoRenderer::QueryInterface (0x00A3BD00): DirectShow BaseClasses DECLARE_IUNKNOWN placed inline in CBaseVideoRenderer, emitted through the CTextureRenderer vtable (its ctor is defined with retail's CLSID / name arguments). MATCH.
- CRenderManager2D (0x009C2FD0): an implicit dtor emitted by defining the class's constructor. With the real STLport text, VC calls ~vector<CQuickDrawTriInfo> out of line and frees the trivial vertex vectors inline, the same per-type choice as retail. ~CVertexShader needs the landed ??_G's local copy of Data.
- CNavQuadTreeNode::SetUpNeighbours (0x00A75E30): the leaf test returns first, then a switch with cases written in arm order 3, 2, 1, 0; the second call of each arm is a tail jump.

## Parked for Fable 5.1 (0)
(none)

## Skipped
(none) -- 0x00558C70's "SPLIT" extent is the function's own jump table (0x00558CF4) and index table (0x00558D00), not a second function; 0x00A3BD00's 1 filler byte precedes the AddRef forwarder 0x00A3BD10.

## Label corrections
- 0x00B42A90 "CBankFileEntryUpdateData::~CBankFileEntryUpdateData" -> CEngineMap::~CEngineMap.
- 0x00991880 "CBankFileEntryUpdateData::~CBankFileEntryUpdateData" -> CASoundManager::~CASoundManager.
- 0x008D7A90 / 0x008D7B20 "vfunc_6" -> OnDeactivate of their own classes (the class labels were right).
- 0x00B52900 "FadeRainToTarget" -> FadeSnowToTarget (0x00B52870 is FadeRainToTarget, as labelled).
- 0x00558C70 "vfunc_48" -> NUISystem::CNavButton::ChangeState.
- 0x00A3BD00 "CTextureRenderer::vfunc_0" (brief: a scalar deleting destructor) -> CBaseVideoRenderer::QueryInterface. The neighbours 0x00A3BD10 / 0x00A3BD20 are CBaseVideoRenderer::AddRef / Release (`jmp [ecx+4]` / `[ecx+8]`), and 0x00A3BD30..0x00A3BDB0 are their adjustor thunks (`?QueryInterface@CBaseVideoRenderer@@W3AG..`, `..WNE@AG..`, `..WNI@AG..`).
- 0x009C2FD0 "CEnginePrimitiveManagerOcclusionBase::~CEnginePrimitiveManagerOcclusionBase" -> CRenderManager2D::~CRenderManager2D.
- 0x00A75E30 "CNavQuadTreeNode::CNavQuadTreeNode::SetUpNeighbours" -> CNavQuadTreeNode::SetUpNeighbours (the class prefix was doubled).
- Callees: 0x009C2F50 manifest "CCombatWheel::ResetRings" / landed "CRetailVector_SOcclusionEntry" -> std::vector<CQuickDrawTriInfo>::~vector; 0x00A620D0 manifest "CParticleEffect::~CParticleEffect" -> CShaderDataBank::~CShaderDataBank; 0x00B59840 manifest "CEngineResourceManager::WorldFrameUpdate" -> CEngineLocalDetailGenerator::DeleteMap (already bound so); coverage.tsv calls 0x00B51F00 "StartRaining" (it is StartSnowing, as the catalog says).

## Retail quirk (keep)
- FadeSnowToTarget (0x00B52900) reads Settings.CurrentRainStrength (+0x5C8), like FadeRainToTarget: the bytes say `mov eax,[ecx+5C8h]` in both. StartSnowing writes +0x5CC. This is Lionhead's copy-paste bug, and the source keeps it.

## Callee names
- Globals 0x01436EA8 / 0x01436E88: spelt g_pLandscapeRenderer / g_pLocalDetailGenerator, as in the landed caller CEngineWorldMap::DeleteMap 0x00B42B10. reloc_bindings has about ten spellings for 0x01436EA8 (g_pEngine_01436ea8, gStaticMapEngineState, ...) and there is no PDB data-symbol dump. They are descriptive and wait for the data-symbol respell wave.
- CTextureRenderer's ctor (TU emitter only): CLSID_TextureRenderer as `__declspec(uuid)` with retail's GUID 71771540-2017-11cf-ae26-0020afd79767 (0x0129D150) and the L"Texture Renderer" name (0x0129D160), the arguments retail's ctor 0x00A3B510 passes.
- Descriptive member names (no PDB): CASoundManager members (as the landed ctor 0x00991910), CSoundBankRecord members (as landed 0x00992510), CThing::m_flags91 (dead bit, as the landed OnActivate 0x008E0060).
- Enumerators unproven: NUISystem::EEvent (0x19 cast), ENavQuadTreeNeighbours (numbered, as the landed sibling), CEngineMap::STATIC_MAP_OPEN_STATE (declared with no enumerators).

## Header debt
(Each source names the shared header it does not use in a `// Layout:` line with the reason; vland only warns SHARED_HEADER_AVAILABLE.)
- engine/CEngineWeatherRenderer.h: Settings flattened to bytes, and the Ego_r layout after it (the retail StartRaining clamps against +0x12F8). Local model with CEngineWeatherSettings::CurrentRainStrength at +0x4B4 (renderer +0x5C8).
- engine/CModeStandBase.h / CClass.h: flattened (vptr as data, pack(1)). The mode chain is local, as the landed Heal sibling.
- engine/CObserver.h (flattened) and no CComponent / CChangingStateComponent / CNavButton headers: the NUISystem chain is local.
- engine/CRenderManagerCore.h, CShaderDataBank.h (its 0x13C STLport size vs 0x128 implied here by VertexShaderResource at +0x3E48), CVertexShader.h: flat records, so the polymorphic chain is local.
- engine/CNavQuadTreeNode.h: flat record. The polymorphic pair is local, with Ego_r names and offsets.
- No CEngineMap header. Retail destroys nothing after FileBlockManager, so the Ego_r CArray members after +0x64 (ShorePoints, ...) are not in TLC's CEngineMap, or have trivial destructors there.

## TU emitters (not graded, not retail bodies)
- 0x00A3BD00: CTextureRenderer::CTextureRenderer(IUnknown*, long*, CTexture*) defined with an empty body (retail's 0x00A3B510 body is not reconstructed).
- 0x009C2FD0: CRenderManager2D::CRenderManager2D(CDisplayManager&, const CWideString&, unsigned long) defined with an empty body.
