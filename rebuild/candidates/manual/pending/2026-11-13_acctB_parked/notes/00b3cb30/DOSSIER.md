# DOSSIER 0x00B3CB30 -- CEngineResourceManager::CEngineResourceManager (5399 bytes)

## 1. Retail shape (xdis / dis_all.txt)
- `sub esp,0C8h ; push ebx ; push ebp ; push esi ; push edi ; mov esi,ecx` ... `pop edi ; mov eax,esi ; pop esi ; pop ebp ; pop ebx ; add esp,0C8h ; ret 8` (0x00B3E044).
- __thiscall, 2 stack args (`ret 8`), returns `this` in eax (ctor). NO frame pointer (ebp is a scratch/long-lived reg), NO SEH
  prologue / no EH state stores, no /GS cookie -> TU built without /EHsc (same as sibling STLvector_Minsertoverflow 0x00B3EA70:
  "no EH: _STLP_TRY/_STLP_UNWIND compile away without /EHsc"). All local dtors run in a straight epilogue block (0x00B3DEF4..0x00B3E033).
- Registers: esi=this, edi=0 for the whole body (xor edi,edi 0x00B3CB68), ebx=1 from 0x00B3CDBC to the end (after the first
  RegisterVertexShaderConstantLayout used ebx=&VSConstantLayoutBasic), ebp = &init (0x00B3CCB2) until 0x00B3CE4E, then reused.
- Arg slots (E = esp after the 4 pushes): init = [E+0xDC], memory_init = [E+0xE0] (read at 0x00B3CD1B and 0x00B3D7FE).
- Unaligned/odd idioms: `lea esp,[esp]` / `lea ecx,[ecx]` loop-alignment pads (0x00B3CBBC, 0x00B3DD8D) = ordinary /O2 loop alignment.

## 2. Name / signature (two sources)
- Ego_r.pdb `egor_pdb_names.tsv` 0x009DB400 and FableWin `_fablewin_api.tsv` / `FableWin.exe.signatures.tsv` 0x01836314 / 0x02DD0200:
  `??0CEngineResourceManager@@QAE@ABVCEngineInit@@ABVCEngineMemoryInit@@@Z`
  -> `CEngineResourceManager::CEngineResourceManager(const CEngineInit& init, const CEngineMemoryInit& memory_init)` (public).
- Manifest label is RIGHT. Repo header engine/CEngineResourceManager.h (retail branch) already declares it (chunk 731 comment).
- Vtables stored: CAsyncLoadingProcessCallback 0x012A1AF8 (at +0xC, inline base ctor), then CEngineResourceManager 0x012A1B3C (+0)
  and 0x012A1B34 (+0xC) (rtti-vtables.tsv; dtor 0x00B3B8E0 stores the same pair).
- Caller: 0x00B2656E (CEngine::Initialise 0x00B26360: operator new 0x31E0, passes init + its memory-init local).

## 3. Callees (retail addr -> landed row -> PDB name; MISMATCH flagged)
| addr | landed file (rebuild/src/compiled/..) | PDB / proper name | note |
|---|---|---|---|
| 00B59710 | CEngineComponent_CEngineComponent_00b59710 | ??0CEngineComponent | base ctor out of line |
| 00A27030 | CMeshDataBank_CMeshDataBank_00a27030 | ??0CMeshDataBank@@QAE@XZ | x2 (+0x20,+0x480) |
| 00AA0F60 | C3DAnimationManager_C3DAnimationManager_00aa0f60 | ??0C3DAnimationManager@@QAE@XZ | |
| 00A62020 | CShaderDataBank_CShaderDataBank_00a62020 | ??0CShaderDataBank | 27-loop + pixel bank |
| 0099AED0 | CWideString_CWideString_0099aed0 | ??0CWideString@@QAE@XZ | |
| 00BDBB70/00BDB400/00BDB260/00BDB080/00BDAFF0 | CEngineVSConstantLayout{Basic,Lights,LightsBones,RepeatedMesh,Particles}_ctor | ??0... | |
| 00F37520 | CGPUScratchBufferTextureManager ctor | ??0CGPUScratchBufferTextureManager@@QAE@XZ | |
| 00BFEA0E / 00BFEA14 / 00BFE9BC | global_malloc / global_free / operator delete (toolchain_library) | malloc/free (STLport alloc), operator delete (CCountedPointer info) | |
| 00A23B50 | **CObservable_AddExclusiveObserver_00a23b50.cpp** (manifest "SetMember_dword0x3d4") | **?SetTextureBank@CMeshDataBank@@QAEXPBVCGraphicDataBank@@@Z** (Ego_r) | **NAME WRONG** -> NAME_FIXES. Body = `TextureBank(+0x3D4)=arg` (CMeshDataBank.h +0x3d4 TextureBank) |
| 00B3B240 | CEngineResourceManager_AllocatePools_00b3b240 | ?AllocatePools@CEngineResourceManager@@AAEX_NABVCEngineMemoryInit@@@Z (private) | landed row models arg2 as `CEngineResourcePoolSizes` and members +0x8E0.. as GeneralPool/GeneralAllocator/StaticPool/IndexPool/ScratchPool and +0x1C as `Budgets` -- **all wrong vs PDB** (LoadingMemoryPool, LoadingMemoryFailureHandler, AnimationMemoryPool, ClothMemoryPool, MeshDataMemoryPool, MainMeshBank). Leaf line is just "AllocatePools". |
| 0099B7D0 | CWideString_operatorassign_0099b7d0 | ??4CWideString | x2 |
| 0099B6B0 | CWideString_CWideString_0099b6b0 | ??0CWideString@@QAE@PBG@Z | |
| 0099BE70 | global_operator_0099be70 | ??H@YI?AVCWideString@@ABV0@0@Z (fastcall, ecx=ret, edx=left) | |
| 0099BF30 | global_operator_0099bf30 | ??H@YI?AVCWideString@@ABV0@PBG@Z | |
| 009A8840 | CBankFileManager_OpenRetailBank_009a8840 | ?OpenRetailBank@CBankFileManager@@QAEXABVCWideString@@_N@Z | on GBankFileManager 0x013CA79C |
| 0099B510 | CWideString_Destructor_0099b510 | ??1CWideString@@QAE@XZ | |
| 0098B230 | CShaderRenderManager_RegisterVertexShaderConstantLayout_0098b230 | RegisterVertexShaderConstantLayout(long, const CVertexShaderConstantLayout*) | (1..5) |
| 009979E0 | global_ConvertDirectoryAndFilenameToPath_009979e0 | CAFile::ConvertDirectoryAndFilenameToPath (static fastcall, hidden ret; landed comment) | |
| vtbl+0x10 on EditorMeshBank | CBankFile slot 4 = 009D1160 | ?OpenFileReadWrite@CBankFile@@UAEXABVCWideString@@KK@Z | virtual call through vptr |
| vtbl+0x04 on EngineMeshBank / ShaderBankPixel | CBankFile slot 1 = 009D06F0 | ?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z | result discarded |
| 00A240E0 | CMeshDataBank_SetClothMemoryPool_00a240e0 | ?SetClothMemoryPool@CMeshDataBank@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z | by-value CCountedPointer (sub esp,8 copy+inc) |
| 00A24050 | CMeshDataBank_SetAnimationMemoryPool_00a24050 | ?SetAnimationMemoryPool@CMeshDataBank@@QAEXV?$CCountedPointer@...@@_N@Z | |
| 00A24150 | CMeshDataBank_SetMeshDataMemoryPool_00a24150 | ?SetMeshDataMemoryPool@CMeshDataBank@@QAEXV?$CCountedPointer@...@@_N@Z | |
| 00B3E820 | CBankFileAsync_SetLoadingMemoryPool_00b3e820 | ?SetLoadingMemoryPool@CBankFileAsync@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z | |
| 009D66A0 | CBankFileAsync_SetLoadingMemoryFailureHandler_009d66a0 | ?SetLoadingMemoryFailureHandler@CBankFileAsync@@QAEXV?$CCountedPointer@VCLoadingMemoryFailureHandler@@@@@Z | |
| 0099E4B0 | CCharString_CCharString_0099e4b0 | ??0CCharString@@QAE@XZ | |
| 0099EBF0 | CCharString_Constructor_0099ebf0 | CCharString(const char*, long = -1) | push -1 |
| 0099EAE0 | CCharString_CCharString_0099eae0 | ??1CCharString | |
| 0098B390 | CShaderRenderManager_SetPixelShaderConstant_0098b390 | ?SetPixelShaderConstant(const CCharString&, const C4DVector&) | out-of-line overload (the index overload is the inline one) |
| 00AB3B10 | CVertexShaderInput_CVertexShaderInput_00ab3b10 | ??0CVertexShaderInput | base of Static/Animated, derived ctors inline (vptr 0x0129DFB8 Static, 0x0129DFB0 Animated) |
| 00A90560 | N3DPrimitiveCVertexShaderInputStatic_Initialise_00a90560 | N3DPrimitive::CVertexShaderInputStatic::Initialise(unsigned long, bool) | |
| 00A90630 | CVertexShaderInputAnimated_Initialise_00a90630 | N3DPrimitive::CVertexShaderInputAnimated::Initialise(unsigned long, bool) | |
| 0099F690 | global_operator_0099f690 | ??H@YI?AVCCharString@@PBDABV0@@Z (fastcall: ecx=ret, edx=const char*, push right) | |
| 00B3B6D0 | CEngineResourceManager_SetupVertexShaderResource_00b3b6d0 (models `CEngineResourceManagerNative`) | ?SetupVertexShaderResource@CEngineResourceManager@@AAEXW4EShaderBankNo@@ABVCCharString@@ABVCVertexShaderInput@@@Z | landed class spelling `CEngineResourceManagerNative` is wrong |
| 00B3B5D0 | CEngineResourceManager_SetupVertexShaderResource_00b3b5d0 (manifest module "_global") | ?SetupVertexShaderResource@CEngineResourceManager@@AAEXW4EShaderBankNo@@ABVCCharString@@PBU_D3DVERTEXELEMENT9@@W4EVertexType@@@Z | |
| 0098B860 | CShaderRenderManager_CreateResource_0098b860 | CCountedPointer<CShaderResource> CreateResource() (hidden ret) | |
| 00A5D7C0 | CShaderResource_OpenShaderBank_00a5d7c0 | ?OpenShaderBank@CShaderResource@@QAEXPBVCShaderDataBank@@@Z | |
| 009E3830 | CPixelFormat_InitialiseD3DFormat_009e3830 | ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z | DXT1/DXT3/A8R8G8B8(0x15) |
| 00A698F0 | CTextureManager_CalculateTextureSize_00a698f0 | ?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z (static fastcall) | |
| 00B244F0 | **CTextureManager_GetPoolTextureSize_00b244f0.cpp** | **?PeekVideoOptionsDef@CEngine@@QAEABVCEngineVideoOptionsDef@@W4EEngineVideoOptionQuality@@@Z** (Ego_r 0x009AEB40) | **NAME WRONG**: ecx = GPEngine 0x01436EA4, body returns CEngine+0x120+4*i = PEngineVideoOptionsDefs[i] (CEngine.h +0x120). Result +0x3C = CEngineVideoOptionsDef::HiresTextureMemory (Ego_r +0x40 minus 4) |
| 00B3EA70 | STLvector_Minsertoverflow_00b3ea70 | vector<CTextureManagerPoolInfo>::_M_insert_overflow (fastcall-like `ret 14h`) | push_back slow path x3 |
| 00A6AAA0 | CTextureManager_Initialise_00a6aaa0 | ?Initialise@CTextureManager (const vector<...>&) | guarded by TextureManager->Initialised (+0x04, CTextureManager.h) |
| 00B3E820 / 009D66A0 / 00A24050 / 00A240E0 | see above | | |
| 00B3C5D0 | NOT LANDED | ?SetupAlphaStippleTexture@CEngineResourceManager@@AAEX_N@Z (Ego_r 0x009DACD0) | private, declare it |
| 009BE6C0 | CDisplayManager_GetBestAlphaTextureFormat_009be6c0 | ?GetBestAlphaTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z (Ego_r: K = unsigned long; landed leaf says J) | on GPDisplayManager 0x01436E1C |
| 009FA280 | CTexture_Initialise_009fa280 | ?Initialise@CTexture (C2DExtentsI const&, long, CPixelFormat const&, ulong, ESurfacePool, bool, bool) | via inline CTexture sized ctor |
| 009FA1C0 | CTexture_Assignment_009fa1c0 | ??4CTexture@@QAEABV0@ABV0@@Z | |
| 009F9F70 | CTexture_Uninitialise_009f9f70 | ?Uninitialise@CTexture@@QAEXXZ (= inline ~CTexture) | |
| 009F9E00 | CTexture_GetSurfaceLevel_009f9e00 | ?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z | |
| 009F3FF0 | CSurface_Clear_009f3ff0 | CSurface::Clear(const CRGBColour&) | |
| 009F2E20 | CSurface_Uninitialise_009f2e20 | CSurface::Uninitialise (inline ~CSurface: vptr 0x0122F84C store then call) | |
| 00F370C0 | CGPUScratchBufferTextureManager_AddScratchPool_00f370c0 | AddScratchPool(long) | arg 0 |
| 00A14990 | CAsyncLoadingMemoryPool_AddLoadingCallback_00a14990 | ?AddLoadingCallback@CAsyncLoadingMemoryPool@@SIXPAVCAsyncLoadingProcessCallback@@@Z (static fastcall; ecx=this+0xC) | |
| 00A5E900 | std_destroyaux_00a5e900 | std::__destroy_aux(pair<unsigned long,CArrayHolder>*, ..., const __false_type&) fastcall | in the 4 CVertexShaderInput dtors |
| 0099A2E0 | CBaseClass_CBaseClass_0099a2e0 | ??1CBaseClass@@UAE@XZ | end of each CVertexShaderInput dtor |

Globals: g_BankFileManager / NBankFileManager::GBankFileManager 0x013CA79C (UsingRetailBanks byte +0x14 = 0x013CA7B0, read via
inline FableWin ?IsUsingRetailBanks@CBankFileManager@@QBE_NXZ -- 0x02DD27A0, no opt-in in fable_retail_banks.h yet; member is
protected), GPShaderRenderManager 0x01436E14 (NEngineInternal), GPDisplayManager 0x01436E1C (?GPDisplayManager@@3PAVCDisplayManager@@A),
GPEngine 0x01436EA4, video options 0x01436EB0 (a CEngineVideoOptions* -- +0x24 = TextureDetail per Ego_r CEngineVideoOptions;
landed spellings vary: g_pVideoOptions / g_pTexturePoolOwner_01436eb0 / g_pEngineCapabilities_01436eb0 -- no PDB name found),
double 0.001 at 0x0126F430 (MaxLoadingTime), D3DVERTEXELEMENT9 arrays 0x0129D6B0.. (only 0x0129D708 = g_Declaration2D is named; rest
descriptive), strings listed in OUTLINE.

## 4. Members (retail offsets; shared header engine/CEngineResourceManager.h retail branch, FABLE_RETAIL_CENGINERESOURCEMANAGER)
Ego_r names (struct_layouts_egor.tsv; retail sizes from the header / dtor 0x00B3B8E0):
+0x00 CEngineComponent base (vptr, GlobalPointerAddress +4, RenderEnabled +8, KeepAlive +9), +0x0C CAsyncLoadingProcessCallback vptr,
+0x10 MeshBanks[3] (CMeshDataBank*), +0x1C MainMeshBank, +0x20 EditorMeshBank (CMeshDataBank 0x460), +0x480 EngineMeshBank,
+0x8E0 LoadingMemoryPool, +0x8E8 LoadingMemoryFailureHandler, +0x8F0 AnimationMemoryPool, +0x8F8 ClothMemoryPool, +0x900 MeshDataMemoryPool
(CCountedPointer<..> Data/Info), +0x908 AnimationManager (C3DAnimationManager 0x4C), +0x954 GraphicBanks[1], +0x958 MainGraphicBank,
+0x95C EngineFont (const CFontBank*), +0x960 VShaderBanks[27] (CShaderDataBank stride 0x128), +0x2898 ShaderBankPixel,
+0x29C0 VShaderResources[27], +0x2A98 ShaderResourcePixel, +0x2AA0 MaxLoadingTime (double), +0x2AA8 VertexBufferUsageFlags (NOT touched),
+0x2AAC StipleFadeTexture (CTexture), +0x2AB4 HWInverseStipplePatterns (CArray<ulong>), +0x2AC0 DitherTextureList (CArray<CTexture>),
+0x2ACC EngineCachePath, +0x2AD0 WriteableEngineCachePath (CWideString), +0x2AD4 ZeroRGBATexture, +0x2ADC SatRGBATexture,
+0x2AE4 VSConstantLayoutBasic, +0x2C30 ..Lights, +0x2D7C ..LightsBones, +0x2EC8 ..RepeatedMesh, +0x3044 ..Particles,
+0x3190 GPUScratchBufferTextureManager, +0x31D4 ExternalTexturesAllocatedFromEnginePools (map; _M_header +0x31D4, count +0x31D8). sizeof 0x31E0.
- The repo retail-branch header keeps every composite OPAQUE (`unsigned char X[N]`) -- a ctor cannot be compiled against it as is:
  every member needs its real type so the compiler emits the member-ctor sequence. The landed dtor 0x00B3B8E0 models the full typed
  layout LOCALLY (classes CTexture, CWideString, CMeshDataBank, CShaderDataBank, C3DAnimationManager, CSimpleMemoryPool, CVertexShaderConstantLayout
  + 5 derived, CGPUScratchBufferTextureManager, CExternalTextureMap, CEngineComponent, CAsyncLoadingProcessCallback, CEngineResourceManager).
  Shared headers exist for: CEngineResourceManager, CEngineComponent (FABLE_RETAIL_CENGINECOMPONENT), CMeshDataBank, CCountedPointer,
  CArray, CTexture, CSurface, CPixelFormat, C2DExtentsI, CRGBColour, C4DVector, CShaderRenderManager, CDisplayManager, CTextureManager,
  CGraphicDataBank, CEngine, CEngineInit, CEngineMemoryInit; fable_string.h (CCharString / CWideString); fable_retail_banks.h
  (CBankFileManager, FABLE_RETAIL_CBANKFILEMANAGER[_HEADERPATHS] for GBankFileManager); stlport/fable_map.h (FABLE_RETAIL_STLP_MAP).
  NO header: CVertexShaderInput{,Static,Animated}, C3DAnimationManager, CTextureManagerPoolInfo (model in STLvector_Minsertoverflow_00b3ea70.cpp).
- Opt-ins useful here: FABLE_RETAIL_CENGINERESOURCEMANAGER (+ _EXTERNALTEXTURES for the map's value type), FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
  (inline GetTextureBank(): the `cmp [ecx+3D4h],0` test), FABLE_RETAIL_CTEXTURE_SIZED (inline CTexture(size,levels,format,usage,pool):
  `PD3DTexture=0; AllocationSource=0` = the `and ebp,0FFFFFFFh` + Initialise(...,false,false) -- exactly 0x00B3DDCF..0x00B3DDFC; also
  declares GetSurfaceLevel), FABLE_RETAIL_C2DEXTENTSI_INITIALISE (C2DExtentsI(x,y)), FABLE_RETAIL_CRGBCOLOUR (ctor(r,g,b,a) stores R,G,B,A
  = +2,+1,+0,+3 matching 0x00B3DE29..38), FABLE_RETAIL_CSURFACE / _CLEAR, FABLE_RETAIL_CPIXELFORMAT(_INIT), FABLE_RETAIL_CCOUNTEDPOINTER,
  FABLE_RETAIL_C4DVECTOR, FABLE_RETAIL_STLP_MAP, FABLE_RETAIL_CBANKFILEMANAGER(_HEADERPATHS). Needs NEW opt-ins (overlay in $W/hdr):
  typed members for CEngineResourceManager (a FABLE_RETAIL_CENGINERESOURCEMANAGER_CTOR-style typed layout), CBankFileManager::IsUsingRetailBanks() const.
- CEngineInit (engine/CEngineInit.h, retail 0x64): Font +0x10, MainGraphicBank +0x14, MeshBank +0x1C, MeshResourcePath +0x20,
  ShaderResourcePath +0x28, EngineCacheResourcePath +0x2C, WriteableEngineCacheResourcePath +0x30, InitFlags +0x60 (bits 0x20 editor bank, 0x04 stipple arg).
- CEngineMemoryInit (engine/CEngineMemoryInit.h): HiresTextureMemory +0x18.
- CGraphicDataBank: +0x280 TextureManager (CCountedPointer<CTextureManager>.Data; Ego_r +0x2FC -> retail -0x7C), +0x2A0/+0x2A4 =
  PixelFormats[6] / PixelFormats[7] (Ego_r PixelFormats[11] at +0x304 -> retail +0x288; inferred by the same 0x7C shift -- one source).
  CTextureManager +0x04 Initialised (bool).
- CVertexShaderInput (0x1C): vptr, +0x04 vector<pair<ulong,CArrayHolder>> (destroyed via __destroy_aux + free), +0x10 POD vector (free) --
  from the epilogue; derived Static/Animated add no data (locals 0x1C apart).

## 5. Visibility
- Inline in retail (no call): CAsyncLoadingProcessCallback ctor (vptr 0x012A1AF8), CCountedPointer default ctor/copy/operator=/dtor,
  CArray<> default ctors, CTexture default + sized ctors, ~CTexture, ~CSurface, C2DExtentsI(x,y), CRGBColour(r,g,b,a), C4DVector(x,y,z,w),
  CPixelFormat() (FormatIndex=-1) and CPixelFormat inline Initialise? NO -- Initialise is called (0x009E3830), STLport map ctor (malloc(0x1C)
  header inline although ??0map 0x00B3E990 exists out of line), vector<CTextureManagerPoolInfo>(10) / push_back fast path / dtor (free),
  CVertexShaderInputStatic/Animated ctors+dtors (only base ctor 0x00AB3B10, __destroy_aux, ~CBaseClass called), GetTextureBank,
  IsUsingRetailBanks, PeekVideoOptionsDef is CALLED (out of line), Get-texture-manager / pixel-format peeks on CGraphicDataBank inline.
- Out-of-line COMDAT helpers at neighbouring addresses in this TU: 0x00B3EA70 _M_insert_overflow (called), 0x00B3E990 map ctor / 0x00B3E9D0 map
  dtor (NOT called here: ctor inlined), 0x00B3EA40 CArray<CTextureManagerPoolInfo>(n) and 0x00B3E920 vector(n) (NOT called here: inlined).

## 6. Sibling rows (same class / TU)
- CEngineResourceManager_CEngineResourceManager_00b3b8e0.cpp (dtor): no pragmas, no Flags line, full LOCAL typed model -- copy its member types.
- CEngineResourceManager_OnReleaseDefaultPoolResources_00b3afe0.cpp: `// Flags: none`, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CENGINERESOURCEMANAGER, _ONRELEASE.
- CEngineResourceManager_BuildAlphaStippleTexture_00b3be10.cpp: FABLE_RETAIL_CENGINERESOURCEMANAGER, FABLE_RETAIL_C2DVECTOR.
- STLvector_Minsertoverflow_00b3ea70.cpp: CTextureManagerPoolInfo / CPixelFormat / allocator model, `#pragma auto_inline(off)` around its two noinline callees.
- stdmap_map_00b3e990.cpp: FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES, stlport/fable_map.h.
- CEngineResourceManager_ResizeHiresTextureMemory_00b3b4f0.cpp: same DXT1 CalculateTextureSize(0x200,0x200,true,fmt) and (kb<<10)/size idiom; graphic bank +0x280.
None use /Oy- or optimize("s"); build flags default (/O2, no /EHsc).
