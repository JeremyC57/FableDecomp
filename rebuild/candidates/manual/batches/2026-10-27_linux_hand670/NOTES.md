# Hand chunk 670 (C3DMeshLODInfo .. CStreamingFontDataBank): 10 of 10 byte-exact

All ten rows are destructors, all RELOCATION_MATCH (tail-jump and call targets checked by name with `objdump -dr`), all
tests PASS under g++ and the lane's VC7.1. Six of the ten catalog labels were wrong (both duplicated labels were half wrong).
Every row was settled first from its slot-0 `??_G` caller (callers = exactly one `call` at `??_G+3`), the vtable it stores
and the RTTI base-class array (read straight from the COL in Fable.exe), then authored; no row needed more than 3 probes,
so the 12-probe parking rule never applied.

## What matched (10)
- **C3DMeshLODInfo** 0x00A24470 `??1C3DMeshLODInfo@@UAE@XZ`: user-declared empty virtual dtor over the retail CResource
  (engine/CResource.h with FABLE_RETAIL_DEF_CLASS_BASE); Mesh / FileData counted pointers (engine/CCountedPointer.h) released
  in reverse order with one shared zero register; tail ~CResource.
- **CSoundBankLH** 0x00A0B9A0 `??1CSoundBankLH@@UAE@XZ` (catalog said ~C3DMeshLODInfo): the dtor calls its own slot 2
  (0x00A0B6C0, `if (BankObject) { BankObject->slot0(); BankObject = 0; }`), devirtualised in the dtor and expanded from the
  same-TU body; then the +0x14 counted pointer and the IMPLICIT ~CASoundBank (no vtable store; Ego_r CASoundBank PSymbolMap +4)
  inline; tail ~CBaseClassNonCopyable.
- **CSoundManagerLH** 0x00A3EDE0 `??1CSoundManagerLH@@UAE@XZ` (catalog said CIconText::ClearIcons): body = CASoundManager
  0x009918F0 (clear banks) + release of the sound system (+0x40, slot 13); members from the retail ctor 0x00A3EF00 and the landed
  slots. The real STLport 4.6 text decides per member exactly as retail: NameMap's ~vector inline, NamePairMap's ~vector and the
  Sounds list's ~_List_base out of line (0x00A40BD0 / 0x00A3FF60); tail ~CASoundManager 0x00991880.
- **CStreamingFontDataBankEntry** 0x00A5F150 / **CFontDataBankEntry** 0x00A63480 (masked twins; catalog gave both the
  CFontDataBankEntry name): `~T() { Unload(); }` with Unload defined after the dtor in the TU (as retail's 0x00A5F250 / 0x00A63580):
  VC7.1 devirtualises the call in the dtor and expands ReleaseData (Info then Data cleared); the member ~CCountedPointer then runs
  on the propagated zero (`mov eax,edi ; cmp eax,edi ; je`) and keeps its dead block. ReleaseData directly and a qualified call
  give the same bytes (probes 1-2 of 0x00A63480).
- **CMouseDX** 0x00AB5320 `??1CMouseDX@@UAE@XZ`: `if (DMouse) { DMouse->Unacquire(); DMouse->Release(); }` over Ego_r CMouseDX at
  retail offsets (Ego_r - 8 after CMouse's 4-byte STLport list); CursorGraphic counted pointer +0x4858; tail ??1CMouse 0x00A66F80.
- **CEngineSubPrimitiveRenderer** 0x00B84390 `??1CEngineSubPrimitiveRenderer@@UAE@XZ` (catalog said
  ~CEngineWaterBackgroundSubPatch): body `PFrameMemoryPool_0143bac4 = 0;`, then SortBuffer, the CSubPrimitiveList[15] array
  (VC7.1's counted reverse loop) and the CEngineComponent base at +4 (ebx) before the primary base.
- **CEngineBillboardGenerator** 0x00B8C7C0 `??1CEngineBillboardGenerator@@UAE@XZ`: compiler-generated (no own vtable store),
  emitted by defining the PDB ctor in the TU; PDB-shaped CPixelShader / CVertexShader user dtors (`if (Data) Data->Release();`
  with the private `if (--ReferenceCount == 0) delete this;`) and CShaderBase's protected virtual `{}`; VC7.1's generated dtor
  null-checks the CEngineComponent base adjustment (`test esi,esi ; je -> xor ecx,ecx`) and duplicates the tail.
- **CEngineSceneGrid** 0x00B93E40 `??1CEngineSceneGrid@@UAE@XZ` (catalog said ~CLocalDetailGeneratorTheme):
  `for (unsigned long i = 0; i < GridCells.size(); ++i) delete GridCells[i];` over CArray<CEngineSceneGridCell*> (STLport vector,
  `*(begin()+n)`), then `if (_M_start) free`.
- **CStreamingFontDataBank** 0x00A5F3A0 `??1CStreamingFontDataBank@@UAE@XZ` (catalog said ~CChunkedFile): `CloseFonts();` (out of
  line, landed 0x00A5F2F0), then streamingEntry, Filename, Fonts, and the StateBlock's implicit ~CStreamingFontBankStateBlock
  expanding ~CBankStateBlock's user `{}` (its vtable store at +0x164); tail ~CBankFileAsync 0x009D6490.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Proven
- 0x00A24470 ??1C3DMeshLODInfo@@UAE@XZ -- vtable 0x0129CDA4 store (RTTI C3DMeshLODInfo : CResource : CIVCountedPointeeBase); sole caller ??_G 0x00A2B690 (slot 0); Ego_r ??1C3DMeshLODInfo@@UAE@XZ; tail ??1CResource@@UAE@XZ.
- 0x00A0B9A0 ??1CSoundBankLH@@UAE@XZ -- vtable 0x0129C9DC store (RTTI .?AVCSoundBankLH@@ : CASoundBank : CBaseClassNonCopyable); sole caller ??_G 0x00A0BDF0 (slot 0). Class name from RTTI only (absent from both PDBs).
- 0x00A3EDE0 ??1CSoundManagerLH@@UAE@XZ -- vtable 0x0129D2C4 store (RTTI .?AVCSoundManagerLH@@ : CASoundManager); sole caller ??_G 0x00A41A50 (slot 0); tail ??1CASoundManager@@UAE@XZ 0x00991880. RTTI name only.
- 0x00A5F150 ??1CStreamingFontDataBankEntry@@UAE@XZ -- vtable 0x0129D5B4 store (RTTI); sole caller ??_G 0x00A5F800; Ego_r name; Unload 0x00A5F250 expanded.
- 0x00A63480 ??1CFontDataBankEntry@@UAE@XZ -- vtable 0x0129DA70 store (RTTI); sole caller ??_G 0x00A63970; Ego_r name; Unload 0x00A63580 expanded.
- 0x00AB5320 ??1CMouseDX@@UAE@XZ -- vtable 0x0129EA14 store (RTTI CMouseDX : CMouse); sole caller ??_G 0x00AB5FA0; Ego_r name.
- 0x00B84390 ??1CEngineSubPrimitiveRenderer@@UAE@XZ -- vtables 0x012A3A60 / 0x012A3A28 (RTTI COL offsets 0 / 4, CEngineComponent at mdisp 4); sole caller ??_G 0x00B866A0; Ego_r name + donor order = retail order (0x00B84320 / 0x00B84390 / 0x00B84400 / 0x00B84470).
- 0x00B8C7C0 ??1CEngineBillboardGenerator@@UAE@XZ -- sole caller ??_GCEngineBillboardGenerator 0x00B8C7A0 (slot 0 of 0x012A3BF4); body walks Ego_r VertexShader +0x10 / PixelShader +0x18 (CPixelShader / CVertexShader / CShaderBase vtable stores); no own store = compiler-generated; Ego_r ??1 / ??_G.
- 0x00B93E40 ??1CEngineSceneGrid@@UAE@XZ -- vtable 0x012A3E58 store (RTTI); sole caller ??_G 0x00B97A60; deletes GridCells through ??1CEngineSceneGridCell@@QAE@XZ 0x00B93660; Ego_r name.
- 0x00A5F3A0 ??1CStreamingFontDataBank@@UAE@XZ -- vtable 0x0129D5CC store (RTTI : CBankFileAsync : CBankFile); sole caller ??_G 0x00A5FA30; CloseFonts 0x00A5F2F0 then tail ??1CBankFileAsync@@UAE@XZ 0x009D6490 (stores 0x0129B8D4 = RTTI CBankFileAsync).
- 0x00A2B690 / 0x00A0BDF0 / 0x00A41A50 / 0x00A5F800 / 0x00A63970 / 0x00AB5FA0 / 0x00B866A0 / 0x00B8C7A0 / 0x00B97A60 / 0x00A5FA30 ??_G<Class>@@UAEPAXI@Z -- slot 0 of each class's RTTI vtable, calling the row above.
- 0x00A40BD0 std::vector<std::pair<std::pair<CCharString,CCharString>,CSoundBankLH*> >::~vector -- the COMDAT the real STLport text emits for NamePairMap (+0x54) in the ~CSoundManagerLH TU; body matches.
- 0x00A3FF60 std::_List_base<CSoundObjectLH*, std::allocator<CSoundObjectLH*> >::~_List_base -- the COMDAT the real _list.c text emits for Sounds (+0x4C); CSoundObjectLH is the landed descriptive element name.
- 0x00A66F80 ??1CMouse@@UAE@XZ -- stores CMouse vtable 0x0129DBC4; callers ??_GCMouse 0x00A673B0 and the ~CMouseDX tail.
- 0x009D5F80 ??0CBankFileAsync@@QAE@XZ -- stores vtable 0x0129B8D4 (RTTI CBankFileAsync) after the CBankFile ctor.
- 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ -- tail of six rows here (already in chunk 662/667 NAME_FIXES).

Descriptive (no PDB / RTTI name exists): CSoundBankLH slot 2 `Unload_00a0b6c0` (0x00A0B6C0), its slots 1 `Load_00a0b630`, the
bank object class `CSoundBankObject_00a0b6c0` and its slot 0 `Release`, `CSoundBankResource_00a0b9a0` (+0x14 target),
CASoundManager 0x009918F0 `ClearBanks_009918f0`, the sound system slot 13 `Shutdown_00a3edf7`, CSoundManagerLH members
`m_pSoundSystem` / `m_pMain` / `m_48` / `m_nextEmitterID` / `NamePairMap` / `NameMap` / `Sounds` / `m_vector` (as the landed
slots), the global `PFrameMemoryPool_0143bac4` (0x0143BAC4, type proven CSimpleMemoryPool*), `ERenderPassType`'s enumerator.

## Label corrections
- 0x00A0B9A0 C3DMeshLODInfo::~C3DMeshLODInfo -> CSoundBankLH::~CSoundBankLH.
- 0x00A3EDE0 CIconText::ClearIcons -> CSoundManagerLH::~CSoundManagerLH.
- 0x00A5F150 CFontDataBankEntry::~CFontDataBankEntry -> CStreamingFontDataBankEntry::~CStreamingFontDataBankEntry.
- 0x00B84390 CEngineWaterBackgroundSubPatch::~CEngineWaterBackgroundSubPatch -> CEngineSubPrimitiveRenderer::~CEngineSubPrimitiveRenderer.
- 0x00B93E40 CLocalDetailGeneratorTheme::~CLocalDetailGeneratorTheme -> CEngineSceneGrid::~CEngineSceneGrid.
- 0x00A5F3A0 CChunkedFile::~CChunkedFile -> CStreamingFontDataBank::~CStreamingFontDataBank.
- Neighbours met on the way: 0x00A40BD0 CCombatWheel::ResetRings -> std::vector<pair<pair<CCharString,CCharString>,CSoundBankLH*>>::~vector;
  0x00A3FF60 CGuiControlTreePane::SortTreeRecursively -> std::_List_base<CSoundObjectLH*>::~_List_base; 0x00A66F80
  CGuiControlTreePane_Destructor -> CMouse::~CMouse; 0x00A0B6C0 CEnginePrimitiveRenderer::ClearAllChildPrimitives -> CSoundBankLH
  slot 2; 0x00A3EF00 CMainGameComponent_Initialize -> CSoundManagerLH::CSoundManagerLH (base ctor 0x00991910 then vtable 0x0129D2C4);
  0x009918F0 Unwind@00fbdab0 / CSoundManager::Shutdown -> CASoundManager member (Banks erase); 0x00B84470
  NHeroInformationScreens::CBase::CBase -> CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer (stores 0x012A3A60/0x012A3A28;
  Ego_r ??0CEngineSubPrimitiveRenderer@@QAE@XZ).

## Header debt
- engine/CFontDataBankEntry.h, CStreamingFontDataBankEntry.h, C3DMeshLODInfo.h, CMouse.h, CEngineBillboardGenerator.h,
  CVertexShader.h, CEngineSubPrimitiveRenderer.h, CEngineComponent.h, CStreamingFontDataBank.h, CBankFile.h are flattened donor
  layouts (`void* __vftable`, byte arrays, `#pragma pack`), so every row re-declares its class over the real base
  (engine/CBaseClassNonCopyable.h, engine/CResource.h retail opt-in) with the PDB member names; each source names the header in
  its `// Layout:` line. No engine header exists for CSoundBankLH / CASoundBank / CSoundManagerLH / CASoundManager /
  CEngineSceneGrid / CPixelShader / CShaderBase / CBankStateBlock.
- STLport vector / list / pair / type-traits are local copies of the 4.6 text in each TU (no shared STLport header in the lane).
