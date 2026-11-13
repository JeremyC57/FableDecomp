# DOSSIER 0x00B73760 CEngineWaterRenderer::CEngineWaterRenderer(const CEngineInit&)  (6799 bytes, 0xB73760..0xB751EF)

No earlier scout / draft existed ($W/prior empty; agent2093 dirs empty) -- this is a fresh scout.

## 1. Retail shape (xdis / dis_all.txt)
- `sub esp,0Ch ; push ebx ; push ebp ; push esi ; push edi ; mov esi,ecx` ... `pop edi ; mov eax,esi ; pop esi ; pop ebp ; pop ebx ;
  add esp,0Ch ; ret 4` (0xB751E3..0xB751EC). thiscall, returns this, one stack arg. No frame pointer, NO SEH frame (no fs:0, no
  EH states) although members have destructors -> TU built without /EHsc. No /GS cookie. Flags: none (as the landed sibling dtor
  0xB71880 and WorldFrameUpdate 0xB6D6D0: `// Flags: none`). No x87/SSE, no asm. Length ends exactly at the `ret 4` (0xB751EF).
- Single straight-line body, 2 forward conditionals + 1 if/else; no loops, no jump tables, no cold blocks.

## 2. Identity (two sources)
- ghidra_out/egor_pdb_names.tsv + ghidra_out/_fablewin_api.tsv: `??0CEngineWaterRenderer@@QAE@ABVCEngineInit@@@Z` (Ego_r 0x00A215E0,
  FableWin 0x02D47AC0); ghidra_out/bsim_port_audit_final.tsv maps retail b73760 -> that symbol (bsim 0.727, true).
- Body proof: stores vtable 0x012A3364 (= CEngineWaterRenderer vtable per engine/CEngineWaterRenderer.h) after the base ctor
  CEngineComponent::CEngineComponent 0x00B59710; reads the arg at +0x60 (CEngineInit::InitFlags, retail offset per CEngineInit.h).
- Only caller: 0x00B268EF inside CEngine::Initialise 0x00B26360 (new + ctor). Manifest label is RIGHT.
- Signature: `CEngineWaterRenderer::CEngineWaterRenderer(const CEngineInit& init)` (public; already declared in the shared header).
- Leaf: `??0CEngineWaterRenderer@@QAE@ABVCEngineInit@@@Z`.

## 3. Callees (landed file / landed name / PDB)
| addr | count | landed row | landed name | PDB / note |
|---|---|---|---|---|
| 0x00B59710 | 1 | 00/b5/CEngineComponent_CEngineComponent_00b59710.cpp | CEngineComponent::CEngineComponent() | ??0CEngineComponent@@QAE@XZ ok |
| 0x00AC1EC0 | 3 | 00/ac/CIndexBuffer_CIndexBuffer_00ac1ec0.cpp | CIndexBuffer::CIndexBuffer() | ok (+0x0C,+0x20,+0x68) |
| 0x0099A2F0 | 2 | 00/99/CBaseClassNonCopyable_..._0099a2f0.cpp | CBaseClassNonCopyable() | ok; from the INLINE CVertexBufferWin32 ctor (+0x34,+0x7C) |
| 0x00A62FB0 | 14 | 00/a6/CVertexShader_CVertexShader_00a62fb0.cpp | CVertexShader::CVertexShader() | ??0CVertexShader@@QAE@XZ ok |
| 0x00A5EC00 | 17 | 00/a5/CPixelShader_CPixelShader_00a5ec00.cpp | **CBaseIntelligentPointer::CBaseIntelligentPointer()** | WRONG: stores vtable 0x0129A054 = CPixelShader; PDB ??0CPixelShader@@QAE@XZ (shared CPixelShader.h already says 0x00A5EC00) -> NAME_FIXES |
| 0x0099E4B0 | 3 | 00/99/CCharString_CCharString_0099e4b0.cpp | CCharString::CCharString() | ok (+0x278,+0x27C,+0x5AC) |
| 0x00BE9920 | 1 | NOT landed | (manifest CEngineWaterSettings ctor) | ??0CEngineWaterSettings@@QAE@XZ (Ego_r+FableWin); header does NOT declare it -> must add a declaration or the implicit inline ctor is generated |
| 0x0099EBF0 | 41 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long = -1) | `push -1 ; push str` |
| 0x0099EAE0 | 41 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString() | ok |
| 0x00A5D5F0 | 25 | 00/a5/**CShaderResource_GetPixelShader_00a5d5f0.cpp** | `__fastcall GetPixelShader(...)` | WRONG spelling: it is ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z (manifest + CShaderResource.h + sprite row 0xBC14E0) -> NAME_FIXES |
| 0x00A5D720 | 16 | 00/a5/CShaderResource_GetPixelShader_00a5d720.cpp | CShaderResource::GetPixelShader | ?GetPixelShader@CShaderResource@@QBE?AVCPixelShader@@ABVCCharString@@@Z (landed signature odd but name ok) |
| 0x00B6C120 | 12 | 00/b6/stdvector_Minsertoverflow_00b6c120.cpp | vector<CVertexShader>::_M_insert_overflow | ok, `ret 14h`; from the inline push_back |
| 0x009BEE00 | 2 | 00/9b/CDisplayManager_GetPixelShaderVersion_009bee00.cpp | CDisplayManager::GetPixelShaderVersion() const | ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ ok; NOT declared in engine/CDisplayManager.h (only GetVertexShaderVersion line 65) -> add in overlay |
| 0x00B6D510 | 1 | 00/b6/CEngineWaterRenderer_InitialiseDynamicTextures_00b6d510.cpp | Leaf says **CWaterDynamicTextures::InitialiseDynamicTextures** | WRONG: PDB ?InitialiseDynamicTextures@CEngineWaterRenderer@@AAEXXZ (private) -> NAME_FIXES |
| 0x00B6EDF0 | 1 | NOT landed | (manifest BuildConstantBuffers) | ?BuildConstantBuffers@CEngineWaterRenderer@@AAEXXZ, declared private in shared header |
| vcall [reg] slot0 push 1 | 70 | -- | CVertexShaderData / CPixelShaderData deleting dtor | from inline Release() / operator= |

Globals (no PDB name for either; use landed descriptive spellings):
- 0x01436E98 `extern CEngineResourceManager* g_pResourceManager;` (spelling used by the sibling dtor 0xB71880 and sprite ctor 0xBC14E0;
  labels_*.tsv: g_pResourceManager). Reads: +0x29F8 = VShaderResources[7], +0x2A00 = [8], +0x2A08 = [9], +0x2A28 = [13],
  +0x2A98 = ShaderResourcePixel (CEngineResourceManager.h line 100: VShaderResources[27] at +0x29C0, stride 8). Use
  `g_pResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("...")` and `GetPShaderResource()->GetPixelShader("...")`
  exactly as row 0xBC14E0 (`(EShaderBankNo)21`). Retail reloads the global for every call (`mov ecx,[01436E98h]` each time).
- 0x01436E1C CDisplayManager* (landed spellings vary: g_pDisplayManager_01436e1c / g_pEngineDisplay / g_pResourceOwner_01436e1c;
  no PDB name). Pick `extern CDisplayManager* g_pDisplayManager_01436e1c; // 0x01436E1C (descriptive)` (matches the class).
- Strings 0x012A33A8..0x012A38B4 = the 41 shader-name literals (table in OUTLINE.md). Vtables: 0x012A3364 this, 0x0129BA20
  CVertexBufferWin32, 0x012A3334 CWaterPatchTesselatorBase<CTVertexWaterBackground>, 0x012A334C <CTVertexSeaBackground>,
  0x0129B3D0 CVertexShader, 0x0129A054 CPixelShader, 0x0129A030 CShaderBase (written by the inline ~CShaderBase of every temporary).

## 4. Members (shared header rebuild/include/engine/CEngineWaterRenderer.h, branch FABLE_RETAIL_CENGINEWATERRENDERER)
The retail branch already types EVERY offset this ctor touches (sizeof 0x5C4, static asserts). Turn on:
`#define FABLE_RETAIL_CENGINEWATERRENDERER` (auto-enables CENGINECOMPONENT / CINDEXBUFFER / CVERTEXBUFFERWIN32 / CVERTEXSHADER),
`#define FABLE_RETAIL_CENGINERESOURCEMANAGER`, `#define FABLE_RETAIL_CARRAY` (+ `FABLE_RETAIL_CARRAY_PUSHBACK`, see 5).
Offsets written: +0x0C/+0x20/+0x68 CIndexBuffer; +0x34/+0x7C CVertexBufferWin32 (inline ctor); +0xBC..+0x1D0 CVertexShader /
CPixelShader members (ctor calls in declaration order), +0x12C/+0x138/+0x144 CArray<CVertexShader> (IceForegroundVertexShader /
IceForegroundShadowedVertexShader / IceForegroundColbuffShadowedVertexShader; inline vector ctor = 3 zero dwords); +0x1FC..+0x26C ten
CArray render lists (zero stores, ctor order); +0x274 HighendWaterEnabled, +0x275 HighendWaterSupported, +0x276 HaveBackgroundSea;
+0x278 LoadedOrLoadingSeaBodyID, +0x27C RequestedSeaBodyID, +0x5AC StaticMapSeaEntryPrefix (CCharString() 0x0099E4B0);
+0x280 Settings (0x00BE9920); +0x564 Tesselator, +0x584 SeaTesselator; +0x5A4 WaterGenerator, +0x5A8 StaticMapBankFile;
+0x5B0/+0x5B8 CCountedPointer (inline ctor: 4 zero dwords); +0x5C0 WaterBumpFactor = 1.5f (0x3FC00000);
body end also zeroes +0x1D8 DynamicTexturePageHandle, +0x1E0 EnvironmentMap, +0x1E8 SeaBumpMap, +0x1F0 WaterBumpMap, +0x1F8 IceBumpMap
(the CTexture* members; the *Handle members +0x1DC/+0x1E4/+0x1EC/+0x1F4 are NOT written).
NOT written: +0xB0/+0xB4/+0xB8 (longs), +0x574 (Tesselator base CLandscapeTesselatorBase::Data), +0x594 (SeaTesselator Data), +0x277.
CEngineInit (engine/CEngineInit.h, no macro): InitFlags +0x60 (retail; Ego_r has it at +0x64 because of the debug member).
Old draft: none, so no local models to retire. The sibling 0xBBA480 (decal ctor) models CVertexShader/CPixelShader/CEngineData
locally -- do NOT copy that; use the shared headers like 0xBC14E0 / 0xB71880.

## 5. Header gaps / visibility (what the hdr/ overlay must add; overlay rule: current header + #ifdef FABLE_RETAIL_* additions)
a. **CVertexShader copy constructor** (PDB ??0CVertexShader@@QAE@ABV0@@Z, public). Retail push_back expansion constructs in place:
   `mov [ecx],0129B3D0h ; mov eax,[eax+4] ; cmp eax,ebx ; mov [ecx+4],eax ; je ; inc [eax+10h]` (0xB73C43..0xB73C53) =
   `CVertexShader(const CVertexShader& o) : Data(o.Data) { if (Data) Data->AddReference(); }` inline. engine/CVertexShader.h has NO
   copy ctor (the implicit one would not add-ref) -> add under a new opt-in, e.g. FABLE_RETAIL_CVERTEXSHADER_COPYCTOR. Same text as
   the landed _M_insert_overflow 0xB6C120 / uninitialized_copy 0xB6BAB0 notes ("CVertexShader's user copy constructor").
b. **CArray<CVertexShader>::push_back**: PDB has ?push_back@?$CArray@VCVertexShader@@@@QAEXABVCVertexShader@@@Z; retail expands it AND
   STLport vector::push_back inline (only _M_insert_overflow 0xB6C120 called, args `push 1 ; push 1 ; push &tag ; push &tmp ; push finish`).
   Use FABLE_RETAIL_CARRAY_PUSHBACK (CArray.h line 69: `void push_back(const T& x) { std::vector<T>::push_back(x); }`) and do NOT
   define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE/NOINLINE. The default __false_type has an empty user ctor (fable_containers.h
   line 46) = retail passes the tag address without a store (lea edx,[esp+28h] -> the CCharString slot). Do NOT define
   FABLE_RETAIL_STLP_FALSE_TAG_POD.
c. **CEngineWaterSettings()**: add `CEngineWaterSettings();  // 0x00BE9920 ??0CEngineWaterSettings@@QAE@XZ` (declaration only).
d. **CWaterPatchTesselatorBase<T>() / CLandscapeTesselatorBase()**: both headers DECLARE the ctor only; retail expands both inline
   (0xB73A54..0xB73AA4: CArray zeros at +4/+8/+0xC, then the derived vtable store, then BridgeVertexBuffer zeros at +0x14..+0x1C;
   base vptr store dead-eliminated, Data +0x10 untouched). Overlay: inline `CLandscapeTesselatorBase() { }` and
   `CWaterPatchTesselatorBase() { }` (base / member default ctors do the work). No out-of-line copy of either exists in the manifest.
   Note retail order: base CArray zeros (0x568..0x570) BEFORE the vtable store 0x564, derived CArray after.
e. **CDisplayManager::GetPixelShaderVersion() const** declaration (0x009BEE00) next to GetVertexShaderVersion in CDisplayManager.h.
f. CShaderResource.h already declares GetVertexShader/GetPixelShader(const CCharString&) const by value. CVertexShader::operator=
   (returns const&, no Data=0) and CPixelShader::operator= (void, nulls Data) in the shared headers match retail exactly
   (0xB73B25..0xB73B4B and 0xB74AE2..0xB74B08). Temp dtors: ~CVertexShader -> CVertexShaderData::Release (`mov edx,[eax+10h] ;
   dec edx ; mov [eax],edx ; cmp edx,ebx`), then ~CShaderBase vtable store 0x0129A030, then ~CCharString -- matches the shared
   headers (proven in 0xBC14E0).
g. CVertexBufferWin32 inline ctor (CVertexBufferWin32.h line 14 / CVertexBufferBase.h line 16) matches retail order
   (base call 0x99A2F0, Length/Type, compression 1,1,1,1,0,0,0,0, vtable 0x0129BA20, PVertexBuffer=0, Stride=0) -- proven by 0xBC14E0.
h. Same-TU visibility: retail TU = CEngineWaterRenderer.cpp (rows 0xB6D420..0xB7F600). InitialiseDynamicTextures 0xB6D510 and
   BuildConstantBuffers 0xB6EDF0 are defined earlier in the TU but called out of line (big bodies) -> declarations only are fine;
   if a body is visible and VC7.1 inlines it, keep it declaration-only.

## 6. Siblings to copy idioms from
- 0x00BC14E0 CEnginePrimitiveManagerParticleSpriteGroup ctor (00/bc/..._00bc14e0.cpp): RELOCATION_MATCH, Flags none; the SAME
  idiom (`Member = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("...")`, GetPShaderResource()->
  GetPixelShader, inline CVertexBufferWin32 ctor). Macros: FABLE_RETAIL_CENGINERESOURCEMANAGER, FABLE_RETAIL_CCHARSTRING,
  FABLE_RETAIL_CARRAY + its hdr overlay. BEST TEMPLATE.
- 0x00B71880 ~CEngineWaterRenderer (00/b7/CEngineWaterRenderer_CEngineWaterRenderer_00b71880.cpp): same class/TU, RELOCATION_MATCH,
  Flags none, `#define FABLE_RETAIL_CENGINEWATERRENDERER` + `FABLE_RETAIL_CENGINERESOURCEMANAGER`, extern g_pResourceManager.
- 0x00B6D6D0 WorldFrameUpdate (Flags none, FABLE_RETAIL_CENGINEWATERRENDERER).
- 0x00BBA480 CEnginePrimitiveManagerDecal ctor / 0x00BB6E00 particle decal group ctor: same shader-lookup shape, `ret 4`, no EH (local models; idioms only).
- Lane family overlays: /opt/fable_lane/w22/agent2048, agent2059, agent2060 (render rows, hdr/ overlays).

## 7. Open questions for the author
- InitFlags test 0xB73ABB..0xB73AF4: `mov cl,[eax+60h] ; test cl,cl ; jns` then two byte stores (0x275 then 0x274) in each arm.
  Bit 0x80 of InitFlags. Try in order: `if (init.InitFlags & 0x80) { HighendWaterSupported = true; HighendWaterEnabled = true; }
  else { HighendWaterSupported = false; HighendWaterEnabled = false; }`; if VC7.1 emits `test byte ptr [eax+60h],80h` instead, try
  `(char)init.InitFlags < 0` or a local bool. Note the read happens AFTER all member ctors (param slot then reused for the CCharString).
- Body order of the trailing zero stores (0x1D8,0x1E0,0x1E8,0x1F0,0x1F8,0x276,0x5A4,0x5A8,0x5C0) is source order (they follow the
  last call and precede two calls; VC7.1 keeps stores in order).
