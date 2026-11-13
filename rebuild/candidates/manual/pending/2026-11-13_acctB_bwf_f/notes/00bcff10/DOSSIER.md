# DOSSIER 0x00BCFF10 (8140 bytes) -- CEnginePrimitiveManagerAnimatedMeshesBase::CEnginePrimitiveManagerAnimatedMeshesBase
Scout 2026-11-12. $W/prior/ was EMPTY and /opt/fable_lane/w22/agent4013 (the earlier chunk for this row) holds only its brief: no
earlier scouting existed for this row. BUT agent4010 (row 0x00BD3070 RenderMeshPrimitive, same class) built the class header:
copied here as $W/hdr_4010/ (whole overlay; the one needed is hdr_4010/engine/CEnginePrimitiveManagerAnimatedMeshesBase.h).
Probes: $W/probe/collapse.py (block-collapsed listing -> probe/collapsed.txt), probe/table.py (shader table -> probe/shader_table.txt),
probe/body.inc (the 61 statements, generated). Write-up of the whole body: $W/OUTLINE.md.

## 1. Retail shape
- thiscall, 1 stack arg, `ret 4` (0x00BD1ED9), returns this. Prologue `mov eax,[esp+4]; sub esp,0Ch; push ebx/ebp/esi/edi; push eax;
  mov esi,ecx; call base`. No frame pointer, NO SEH/EH frame, no /GS -> build with NO /EHsc ("// Flags: none"), like the landed
  siblings 0x00BC14E0, 0x00B99D90, 0x00BD1EF0, 0x00BD1F30 (all `ret 4`, no EH frame). Member arrays construct with inline counted
  loops (consistent with no EH). Length confirmed: first instr 0x00BCFF10, last `ret 4` at 0x00BD1ED9 ends at 0x00BD1EDC = 0x00BCFF10+0x1FCC = 8140 bytes (oracle length right; nothing after the ret belongs to the row).
- Size-opt idioms: none; /O2-style (loop-head alignment fillers `jmp+3; lea ecx,[ecx]`, `mov edi,edi`, `lea ebx,[ebx+0]` are
  compiler-emitted).

## 2. Name (proven, two sources + vtable + callers)
- Ego_r.pdb `??0CEnginePrimitiveManagerAnimatedMeshesBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z` (egor_pdb_names.tsv @0x00A84830,
  Ego_r size 8130 ~ retail 8140); FableWin.pdb same leaf (fablewin_pdb_names.tsv @0x02E7AA30; FableWin.exe.signatures.tsv size 8577);
  bsim_port_audit_final.tsv matches 0x00BCFF10 to that leaf (score 0.915); engine_api.tsv: __thiscall (this, CArray<CEngineScene*>*).
- Vtable stored at 0x00BCFF23 = 0x012A78DC = RTTI CEnginePrimitiveManagerAnimatedMeshesBase (rebuild/manifest/rtti-vtables.tsv).
- Callers (xref.py): 0x00BD1EF8 in landed CEnginePrimitiveManagerAnimatedMeshes::CEnginePrimitiveManagerAnimatedMeshes 0x00BD1EF0 and
  0x00BD1F38 in landed CEnginePrimitiveManager2DAnimatedMeshes ctor 0x00BD1F30 -- both pass their scenes arg through.
- The manifest label is RIGHT (class + ctor). Signature: `CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>& scenes)`,
  public, non-const, no return type. Mangled leaf: `??0CEnginePrimitiveManagerAnimatedMeshesBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z`.

## 3. Callees
| retail | landed row | landed spelling | PDB spelling | flag |
|---|---|---|---|---|
| 0x00BEBC30 | 00/be/CEnginePrimitiveManagerMeshBase_CEnginePrimitiveManagerMeshBase_00bebc30.cpp | CEnginePrimitiveManagerMeshBase(void*) | ??0CEnginePrimitiveManagerMeshBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z (header engine/CEnginePrimitiveManagerMeshBase.h retail branch declares `(CArray<CEngineScene *>&)`) | landed row param type wrong (void*): NAME_FIXES candidate (type, not name). engine_api.tsv mislabels it CEnginePrimitiveGroupManager (and landed 0x00B99D90 copies that wrong name) |
| 0x00A62FB0 | 00/a6/CVertexShader_CVertexShader_00a62fb0.cpp | CVertexShader::CVertexShader() | ??0CVertexShader@@QAE@XZ | ok (engine_api says CBaseIntelligentPointer: stale) |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1) | ??0CCharString@@QAE@PBDJ@Z | ok (file name says "Constructor") |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ??1CCharString@@QAE@XZ | ok |
| 0x00A5D5F0 | 00/a5/CShaderResource_GetPixelShader_00a5d5f0.cpp | `GetPixelShader` (free __fastcall fn, CPixelShader*) | ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z (engine/CShaderResource.h line 20 pins 0x00A5D5F0; GetPixelShader(name) is 0x00A5D720; the result here is assigned to CVertexShader members) | WRONG landed spelling -> NAME_FIXES.tsv: 0x00A5D5F0 = CShaderResource::GetVertexShader |
| 0x009BEE00 | 00/9b/CDisplayManager_GetPixelShaderVersion_009bee00.cpp | CDisplayManager::GetPixelShaderVersion() const | ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ | ok; but landed 0x00B99D90 spells it CEngineDisplay::CalculateTextureMemoryFromQuality (and engine_api.tsv says Engine_CalculateTextureMemoryFromQuality): wrong, NAME_FIXES candidate for that caller |
| [vtbl+0](1) on CVertexShaderData | -- | CVertexShaderData scalar deleting dtor via `delete data` | ??1CVertexShaderData (private virtual) | inline from CVertexShader.h |
- Globals: 0x01436E98 `g_pResourceManager` (CEngineResourceManager*; descriptive, no PDB name; spelled exactly so in
  ghidra_out/labels_*.tsv and the landed 0x00BC14E0). 0x01436E1C `NEngineInternal::GPDisplayManager`
  (?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A; 15 landed rows declare it locally as
  `namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }`; other landed spellings g_pEngineDisplay /
  g_pDisplayManager_01436e1c are wrong).
- Literals: 61 VSHADER_* strings 0x012A78B8 .. 0x012A6C60 (table in OUTLINE / probe/shader_table.txt). Exact text matters (relocs).

## 4. Members (all `this`, Ego_r = retail; class sizeof 0x690)
No repo header for the class. Use $W/hdr_4010/engine/CEnginePrimitiveManagerAnimatedMeshesBase.h (agent4010; Ego_r names/types,
offsets verified against this row: every assigned offset and the pointer block +0x2E8..+0x304 and +0x32C land on the right member).
It needs FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS (retail branch of engine/CEnginePrimitiveManagerMeshBase.h, which declares the
`(CArray<CEngineScene *>&)` base ctor) and FABLE_RETAIL_CVERTEXSHADER (engine/CVertexShader.h retail branch: CShaderBase base,
inline operator= and ~CVertexShader through CVertexShaderData::Release -- the exact expansions seen here; the same texts matched in
landed 0x00BC14E0). Its ctor declaration `CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>&);` is already there.
Its _HELPERS opt-in is NOT needed for this row (the ctor calls no Apply* helper); don't define it.
Members written: PointLightShaderIndex[6] +0x20 (unsigned long); 61 CVertexShader members (see OUTLINE); the CVertexShader* selectors
+0x2E8..+0x304; float TextureOverlayFadeDistandeFactor +0x32C (sic PDB spelling) = 2.0f.
External members: CEngineResourceManager::VShaderResources[27] (CCountedPointer<CShaderResource>, +0x29C0, Data at +0 of each 8-byte
element) read through the inline `GetVShaderResource(EShaderBankNo)` (engine/CEngineResourceManager.h, FABLE_RETAIL_CENGINERESOURCEMANAGER):
bank 3 = +0x29D8, bank 4 = +0x29E0. No enumerator names recovered: write `(EShaderBankNo)3` / `(EShaderBankNo)4` like 0x00BC14E0's `(EShaderBankNo)21`.
CShaderResource.h (no opt-in needed) declares `CVertexShader GetVertexShader(const CCharString& name) const;`.
CDisplayManager: engine/CDisplayManager.h retail branch declares GetVertexShaderVersion() const (line 65) but NOT
GetPixelShaderVersion -> add `unsigned long GetPixelShaderVersion() const; // 0x009BEE00 ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ`
next to it in the hdr/ overlay (opt-in block), or include CDisplayManager.h under FABLE_RETAIL_CDISPLAYMANAGER (heavy includes) --
alternative: model `this` only, a local CDisplayManager declaration is a verifier warning since a header exists; prefer the overlay line.
Classes no draft exists to model locally; the landed siblings 0x00B99D90 / 0x00BD1EF0 model CCharString / CVertexShader / managers
locally -- do NOT copy that; use the shared headers above (fable_string.h via FABLE_RETAIL_CCHARSTRING).

## 5. Callee-body visibility
- Inline in retail (expanded in this body): CVertexShader::operator= and ~CVertexShader (+~CShaderBase vptr store), CVertexShaderData::Release,
  CEngineResourceManager::GetVShaderResource, CCountedPointer::Get. All already inline in the shared headers (CVertexShader.h retail branch,
  CEngineResourceManager.h). Out of line: CVertexShader() 0x00A62FB0, CCharString ctor/dtor, GetVertexShader 0x00A5D5F0,
  GetPixelShaderVersion 0x009BEE00 (another TU), base ctor 0x00BEBC30 (another TU, MeshBase). No STLport.
- Same TU neighbours (defined earlier in retail): FrameStart 0x00BCEA60, OOL helper copies 0x00BCE8A0/0x00BCE900/0x00BCE940,
  RenderPrimitiveBlocksIndexBuffer 0x00BCFB00; after: derived ctors 0x00BD1EF0/0x00BD1F30, GetSupportedTypes 0x00BD27F0. None of them is
  called here, so no visibility lever is expected.

## 6. Sibling landed rows (idioms to copy)
- 0x00BC14E0 CEnginePrimitiveManagerParticleSpriteGroup ctor (rebuild/src/compiled/00/bc/...ParticleSpriteGroup_00bc14e0.cpp): THE model.
  `// Flags: none`; `#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP / CENGINERESOURCEMANAGER / CCHARSTRING / CARRAY`;
  includes engine/<Class>.h, engine/CEngineResourceManager.h, engine/CShaderResource.h; `extern CEngineResourceManager* g_pResourceManager;`;
  statements `Member = g_pResourceManager->GetVShaderResource((EShaderBankNo)21)->GetVertexShader("...");` (it re-reads the global each
  time; THIS row hoists both bank pointers into locals -- see OUTLINE).
- 0x00B99D90 CEnginePrimitiveManagerRepeatedStaticMeshes ctor: same `>= 1400` pixel-shader-version guard and the "selector = &depth set"
  tail (local models, wrong callee names: do not copy names).
- 0x00BCEA60 AnimatedMeshesBase::FrameStart (same class/TU): selector assignment texts (array-decay `= VertexShadersPalSkinDepthBufferShadow;`
  and `= &VertexShadersPalSkinDepthShadowBufferRender;`).
- 0x00BD1EF0 / 0x00BD1F30 derived ctors (no flags, no EH).
- 0x00BC34D0 ParticleMeshGroup ctor (`ret 4`, no EH frame).

## Risks / levers for the author
1. Local declaration order of the two bank pointers (retail loads bank 4 into ecx first, then bank 3 into ebx; bank 4 is spilled
   into the dead arg slot). Try `shader_resource` (3) first then `dp3_shader_resource` (4), and the reverse.
2. Selector store order (retail: 2EC,2FC,2E8,2F0,300,2F4,304,2F8) differs from FrameStart's -> write them in retail order.
3. PointLightShaderIndex stores interleave (eax=1 stores first, then [0x20]=0): plain `[0]=0..[5]=3` in index order should schedule that way;
   if not, try the order 1,2,0,3,4,5.
4. If FABLE_RETAIL_CENGINERESOURCEMANAGER's GetVShaderResource returns `const CShaderResource*`, GetVertexShader must be const (it is).
