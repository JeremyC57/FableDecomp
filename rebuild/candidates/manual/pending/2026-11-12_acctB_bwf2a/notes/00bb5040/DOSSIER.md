# DOSSIER 0x00BB5040 -- CEngineClothRenderer::CEngineClothRenderer()  (4824 bytes, 0x00BB5040..0x00BB6317)

## 1. Retail shape  (python3 $S/w17/xdis.py 00bb5040 0x12e8 ; $W/dis_all.txt)
- `__thiscall`, no stack params, plain `ret` (0x00BB6317), returns `this` (`mov eax,edi` 0x00BB630E). Last byte is the ret (len 0x12D8).
- Prologue: `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,18h ; push ebx ; push ebp ; push esi ; push edi`.
  This is the VC7.1 **8-byte-aligned frame** (NOT /Oy-): ebp is re-pushed and then used as a general register (holds the
  CShaderResource*), epilogue `pop edi ; pop esi ; pop ebp ; pop ebx ; mov esp,ebp ; pop ebp ; ret`.
- **No SEH frame / no EH states** although members have destructors -> TU built without /EHsc (same as every sibling shader-ctor
  row: 0x00BC14E0 `// Flags: none`, 0x00BB6E00 "no EH frame"). No /GS cookie. Not size-optimised (/O2 idioms: `xor ebx,ebx`
  zero register, registers hoisted).
- ebx = 0 for the whole body; edi = this; ebp = the vertex-shader resource (loaded ONCE at 0x00BB5114, before the first resize);
  esi = &current CArray (then reused as scratch).
- Aligned frame cause (open question, see OUTLINE "frame"): the only 8-byte local is the by-value `CVertexShader` temporary at
  aligned-8 (F+0x20). Same temporaries in sibling rows (max 21 GetVertexShader calls, 0x00BC14E0) give NO aligned frame; this row has
  34. Lane precedents for `and esp,-8`: 0x0098F3A0 (an 8-byte `const double&` temporary), 0x00544520 (8-byte C2DVector by-value
  temporaries), 0x00BE3110 (C2DCoordI array). Hypothesis A: VC7.1's dynamic-alignment heuristic fires on the 8-byte temp's
  reference count (34 temporaries) -> plain source reproduces it. Hypothesis B: an inline helper with an `unsigned __int64`
  (CSimpleMemoryPool::AllocateType / the PoolNewArray guard of landed 0x00BB1E30) folded away in the body. Try A first.

## 2. Identity (two sources)
- Name: `CEngineClothRenderer::CEngineClothRenderer()`, leaf **`??0CEngineClothRenderer@@QAE@XZ`** (public, no params, void).
  - Ego_r.pdb: ghidra_out/egor_pdb_names.tsv `00a6b020 ??0CEngineClothRenderer@@QAE@XZ` (Ego_r address space; dtor ??1 at 0x00a6c420).
  - FableWin.pdb: ghidra_out/_fablewin_api.tsv / naming_stragglers/FableWin.exe.signatures.tsv `02e71900 ??0CEngineClothRenderer@@QAE@XZ`
    (5171 bytes debug, followed by ??_E 02e73240 and ??1 02e73280 -- same TU order as retail ctor / ??_G 0x00BB6710 / ??1 0x00BB6730).
  - Retail: stores vtable 0x012A56EC = CEngineClothRenderer (rebuild/manifest/rtti-vtables.tsv: slot0 ??_G 0x00BB6710, slot1 FrameStart 0x00BB1EF0).
  - Only caller 0x00B3462C inside 0x00B33B50 (CEnginePrimitiveRenderer, not landed): `operator new(0xF0)` 0x00BFEA1A, null-check,
    `call 0x00BB5040`, then CCountedPointer<CEngineClothRenderer>::Reset into +0x2F0 (ClothRenderer). sizeof retail = 0xF0.
  - Manifest label `CEngineClothRenderer::CEngineClothRenderer` is CORRECT.
  - NB egor_pdb_names.tsv has `00bb5040 ?IsPosDraggable@CGuiWindowBox...` -- that is an Ego_r address collision, ignore.

## 3. Callees
| retail | landed row | landed name | PDB name | note |
|---|---|---|---|---|
| 0x0099A2F0 | 00/99/CBaseClassNonCopyable_CBaseClassNonCopyable_0099a2f0.cpp | CBaseClassNonCopyable::CBaseClassNonCopyable | ??0CBaseClassNonCopyable@@IAE@XZ | out of line (engine/CBaseClassNonCopyable.h declares it non-inline) |
| 0x00A62FB0 x6 | 00/a6/CVertexShader_CVertexShader_00a62fb0.cpp | CVertexShader::CVertexShader | ??0CVertexShader@@QAE@XZ | members +0x54..+0x7C |
| 0x00AC1EC0 | 00/ac/CIndexBuffer_CIndexBuffer_00ac1ec0.cpp | CIndexBuffer::CIndexBuffer | ??0CIndexBuffer@@QAE@XZ | +0x8C |
| 0x00A5C570 x2 | 00/a5/CSimpleMemoryPool_CSimpleMemoryPool_00a5c570.cpp | CSimpleMemoryPool::CSimpleMemoryPool(ulong capacity, ulong growthGranularity) | ??0CSimpleMemoryPool@@QAE@KK@Z | args (0, 0x1000) |
| 0x00B6CBD0 x5 | 00/b6/stdvectorCVertexShader_resize_00b6cbd0.cpp | std::vector<CVertexShader>::resize(n) | ?resize@?$vector@VCVertexShader@@...@std@@QAEXI@Z | STLport COMDAT out of line, `ret 4`, arg 6 |
| 0x0099EBF0 x34 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString::CCharString(const char*, long) | ??0CCharString@@QAE@PBDJ@Z | `push -1 ; push str` (default length -1) |
| 0x00A5D5F0 x34 | 00/a5/CShaderResource_GetPixelShader_00a5d5f0.cpp | **GetPixelShader (WRONG)** | ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z | NAME FIX: landed row spells it GetPixelShader (a `__fastcall` free function); engine/CShaderResource.h line 20 + landed 0x00BC14E0 / 0x00BB6E00 + the VSHADER_* literals say GetVertexShader (GetPixelShader is 0x00A5D720). Hidden result pointer, `ret 8`. |
| 0x0099EAE0 x34 | 00/99/CCharString_CCharString_0099eae0.cpp | CCharString::~CCharString | ??1CCharString@@QAE@XZ | (ghidra coverage.tsv label `_Cons_val<...>` is wrong; landed name is right) |
| 0x009BEE00 | 00/9b/CDisplayManager_GetPixelShaderVersion_009bee00.cpp | CDisplayManager::GetPixelShaderVersion() const | ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ | unsigned long, 1400 = ps_1_4 (0x578) |
| `call [vptr+0]` push 1 | -- | CVertexShaderData scalar deleting dtor (slot 0) | -- | inside inline CVertexShader::operator= and ~CVertexShader |
Globals: 0x01436E98 `NEngineInternal::GPResourceManager` (CEngineResourceManager*; spelled in landed 00/bb/...SetupPixelShader_00bbcc70.cpp,
00/ba/..._00badb40.cpp; other rows use descriptive g_pResourceManager / g_pEngineData -- use the NEngineInternal spelling);
0x01436E1C `NEngineInternal::GPDisplayManager` (`?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A`, landed 00/be/..._00be1e70.cpp).
+0x29C0 on the resource manager = `VShaderResources[0]` (engine/CEngineResourceManager.h line 100, retail opt-in FABLE_RETAIL_CENGINERESOURCEMANAGER:
`GetVShaderResource(EShaderBankNo bank) const { return VShaderResources[bank].Get(); }`) -> `GetVShaderResource((EShaderBankNo)0)`.
Vtables seen in inline expansions: 0x0129B3D0 CVertexShader, 0x0129A030 CShaderBase.

## 4. Members (retail offsets = Ego_r.pdb minus 4 per preceding CArray: retail CArray is 12 bytes, Ego_r 16)
No shared header `rebuild/include/engine/CEngineClothRenderer.h` exists -> the class must be modelled in the row (as the landed
siblings 0x00BB6730 / 0x00BB1E30 do). Ego_r names (struct_layouts_egor.tsv line 150084..150110), retail offsets proven by the dtor 0x00BB6730 and this ctor:
| off | member | type | touched here | shared header / opt-in |
|---|---|---|---|---|
| +0x00 | vptr (CEnginePrimitiveRenderUtil : CBaseClassNonCopyable) | | base ctor + `mov [edi],012A56ECh` | engine/CEnginePrimitiveRenderUtil.h (inline `{}` ctor), engine/CBaseClassNonCopyable.h |
| +0x04 | TriVertexShaders | CArray<CVertexShader> | zero 3 words, resize(6), [0..5] | engine/CArray.h `FABLE_RETAIL_CARRAY` (operator[] through begin()) |
| +0x10 | TriVertexShadersDepthBufferShadow | CArray<CVertexShader> | same | " |
| +0x1C | TriVertexShadersSpotDepthBufferShadow | CArray<CVertexShader> | same | " |
| +0x28 | TriVertexShadersColourBufferShadow | CArray<CVertexShader> | same (if PS >= 1400) | " |
| +0x34 | TriVertexShadersSpotColourBufferShadow | CArray<CVertexShader> | same (if PS >= 1400) | " |
| +0x40 | TriVertexShadersShadow | CVertexShader* | NOT written | |
| +0x44 | TriVertexShadersSpotShadow | CVertexShader* | NOT written | |
| +0x48 | PatchVertexShaders | CArray<CVertexShader> | zero only | " |
| +0x54 | TriVertexShaderOutline | CVertexShader | ctor; = "VSHADER_STATIC_OUTLINE" (Data +0x58) | engine/CVertexShader.h `FABLE_RETAIL_CVERTEXSHADER` (operator=, ~ inline) |
| +0x5C | TriVertexShaderOutlineSmoothFill | CVertexShader | ctor; = ..._SMOOTH_FILL (Data +0x60) | " |
| +0x64 | TriVertexShaders2D | CVertexShader | ctor; = VSHADER_2D_STATIC_DIRLIGHT (Data +0x68) | " |
| +0x6C | TriBackfaceVertexShaders2D | CVertexShader | ctor; = ..._BACKFACE (Data +0x70) | " |
| +0x74 | PatchVertexShaders2D | CVertexShader | ctor only | " |
| +0x7C | DebugVertexShader | CVertexShader | ctor only | " |
| +0x84 | VertexNormals | C4DVector* | NOT written | |
| +0x88 | ClothDebug | bool | `= false` last store (0x00BB6308) | |
| +0x8C | IndexBuffer | CIndexBuffer (0x14) | ctor 0x00AC1EC0 | engine/CIndexBuffer.h `FABLE_RETAIL_CINDEXBUFFER` (+FABLE_RETAIL_CMOVABLERESOURCE) |
| +0xA0 | IndexBufferSize | long | `= -1` (0x00BB50DC, between the first pool's pushes and its call -> init list) | |
| +0xA4 | EnableCloth | bool | `= true` (0x00BB6301) | |
| +0xA8 | PositionBufferPool | CSimpleMemoryPool (0x1C) | (0, 0x1000) | engine/CSimpleMemoryPool.h `FABLE_RETAIL_CSIMPLEMEMORYPOOL` |
| +0xC4 | PositionBuffer | C4DVector* | = 0 (after both pool ctors) | |
| +0xC8 | PositionBufferSize | unsigned long | = 0 | |
| +0xCC | NormalBufferPool | CSimpleMemoryPool | (0, 0x1000) | " |
| +0xE8 | NormalBuffer | C4DVector* | = 0 | |
| +0xEC | NormalBufferSize | unsigned long | = 0 | |
sizeof = 0xF0 (operator new(0xF0) in the caller).
Classes the landed dtor 0x00BB6730 models LOCALLY although a shared header exists: CVertexShaderData/CVertexShader/CShaderBase
(engine/CVertexShader.h + CShaderBase.h), CIndexBuffer/CMovableResource (engine/CIndexBuffer.h), CSimpleMemoryPool
(engine/CSimpleMemoryPool.h), CEnginePrimitiveRenderUtil (engine/CEnginePrimitiveRenderUtil.h). Use the headers in the new row.
Also: engine/CShaderResource.h (GetVertexShader by value, no opt-in macro needed), engine/CEngineResourceManager.h
(`FABLE_RETAIL_CENGINERESOURCEMANAGER`), fable_string.h (`FABLE_RETAIL_CCHARSTRING`). CDisplayManager.h does NOT declare
GetPixelShaderVersion (grep) -- the landed 0x009BEE00 row declares it locally; add `unsigned long GetPixelShaderVersion() const;` under a
hdr/ overlay opt-in or check FABLE_RETAIL_CDISPLAYMANAGER first.

## 5. Visibility
- Out of line in retail (do NOT make visible / inline): CBaseClassNonCopyable ctor, CVertexShader ctor, CIndexBuffer ctor,
  CSimpleMemoryPool ctor, vector<CVertexShader>::resize (STLport COMDAT 0x00B6CBD0, the CArray.h STLport vector resize already
  emits a call -- matched in 0x00BC14E0), CCharString ctor/dtor, GetVertexShader, GetPixelShaderVersion.
- Inline (expanded every time): CVertexShader::operator= (header FABLE_RETAIL_CVERTEXSHADER text: `data = Data; if (data) { if
  (--data->ReferenceCount == 0) delete data; } Data = other.Data; Data->AddReference();` -> `mov ecx,[x+4]; cmp ecx,ebx; je;
  dec [ecx+10h]; jne; call [vptr] push 1`, then `mov eax,[res+4]; mov [x+4],eax; inc [eax+10h]`), ~CVertexShader (vtable store
  0x0129B3D0, Release `mov r,[eax+10h]; dec r; mov [eax+10h],r; jne`), ~CShaderBase (store 0x0129A030), CArray operator[]
  (begin()+i), CEngineResourceManager::GetVShaderResource / CCountedPointer::Get, CEnginePrimitiveRenderUtil(){}.
- Same TU (retail EngineClothRenderer.cpp): GetPositionBuffer 0x00BB1E30, GetNormalBuffer 0x00BB1E90, FrameStart 0x00BB1EF0,
  CalculateVertexNormals 0x00BB1F50, RenderPolygons 0x00BB2540, Render2DClothPrimitive 0x00BB30A0, RenderClothPrimitive 0x00BB3E80,
  this ctor, then ??_G 0x00BB6710, ??1 0x00BB6730. None is called by the ctor.

## 6. Sibling landed rows to copy idioms from
- 0x00BC14E0 CEnginePrimitiveManagerParticleSpriteGroup ctor (00/bc/..._00bc14e0.cpp): `// Flags: none`, defines
  FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP / CENGINERESOURCEMANAGER / CCHARSTRING / CARRAY; includes engine/CShaderResource.h;
  `X.resize(5); X[i] = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("...");` -- exactly our idiom, BUT it
  reloads the resource per statement; ours needs a LOCAL (see next).
- 0x00BB6E00 CEnginePrimitiveManagerParticleDecalGroup ctor (00/bb/..._00bb6e00.cpp): "the decal vertex resource (+0x29C0) is read
  once into a named local ahead of the first name temporary" -- `CShaderResource* vertexResource = g_pEngineData->DecalVertexShaderResource;`
  (local class models; no flags). Same +0x29C0 bank as ours.
- 0x00BB6730 ~CEngineClothRenderer (00/bb/CEngineClothRenderer_CEngineClothRenderer_00bb6730.cpp): retail layout of this exact class
  (local models), `PositionBufferPool(0, 0x1000), NormalBufferPool(0, 0x1000)` init list; no flags line.
- 0x00BB1E30 GetPositionBuffer / 0x00BB1E90 GetNormalBuffer / 0x00BB1EF0 FrameStart: same class, local partial models.
- 0x00B99D90 CEnginePrimitiveManagerRepeatedStaticMeshes ctor: colour-buffer-shadow shaders conditional on device capability.

## Name fixes to hand back (NAME_FIXES.tsv)
- 0x00A5D5F0: landed `GetPixelShader` (free __fastcall) -> `CShaderResource::GetVertexShader(const CCharString&) const`
  `?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z` (CShaderResource.h + 0x00BC14E0 + 0x00BB6E00).
- 0x01436E98 global: several spellings landed (g_pResourceManager, g_pEngineData, g_pEngine_01436e98); evidence spelling is
  `NEngineInternal::GPResourceManager`.
