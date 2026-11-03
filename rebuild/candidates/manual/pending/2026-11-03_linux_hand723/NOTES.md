# Chunk 723: CEngineWaterRenderer group (13 rows)

Result: 6 RELOCATION_MATCH (tests PASS), 1 parked in differ/ after 18 graded attempts, and 6 large float/render-state rows not
drafted (budget; see below). Every matched row except 0x00C2A890 needs the hand-back header overlay `hdr/`
(grade with `FABLE_HDR_OVERLAY=$A/hdr`). `hdrcheck2.py hdr/` reports default=same for every changed header.

| row | name | grade | attempts |
|---|---|---|---|
| 00b36ec0 | CEnginePrimitiveRendererSharedData::~CEnginePrimitiveRendererSharedData | RELOCATION_MATCH | 3 |
| 00b67920 | CEngineLandscapeRenderer::~CEngineLandscapeRenderer | RELOCATION_MATCH | 3 |
| 00b6d6d0 | CEngineWaterRenderer::WorldFrameUpdate | RELOCATION_MATCH | 2 |
| 00b71880 | CEngineWaterRenderer::~CEngineWaterRenderer (catalog said ~CEnginePrimitiveRendererSharedData) | RELOCATION_MATCH | 2 |
| 00bc14e0 | CEnginePrimitiveManagerParticleSpriteGroup ctor | RELOCATION_MATCH | 3 |
| 00c2a890 | CNavigatorSwitch::SendStateAndReset (descriptive, libpk; catalog said LoadSea) | RELOCATION_MATCH | 1 |
| 00b6edf0 | CEngineWaterRenderer::BuildConstantBuffers (catalog said GenerateWaterMesh) | DIFFER 3144v3144, parked | 18 |
| 00b70230 / 00b70a90 / 00b751f0 / 00b761d0 / 00b77370 | RenderSeaBackground / RenderEnvironmentMap / RenderIceForeground / RenderForeground / RenderSeaReflectiveForeground (catalog names, not re-proven) | not drafted | 0 |
| 00bc04f0 | CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite (catalog name) | not drafted | 0 |

## Levers that worked
- **Implicit destructor emitted out of line** (0x00B36EC0): a user `~X() {}` adds the class vtable store (1351 v 1345). Leave the
  destructor implicit and define the PDB constructor in the TU so VC7.1 emits it.
- **`size()` not `end() - begin()`** (0x00B67920): STLport `size()` loads `_M_finish` first; `end()-begin()` loads begin and keeps
  it across the loop (1296 v 1294).
- **CArray index operators through `begin()`** (0x00BC14E0): `return *(this->begin() + i);` lets VC7.1 fold the element offset into
  the inlined `CVertexShader::operator=` (`[esi+0Ch]`); the default `this->_M_start[i]` materialises the element address
  (`add esi,8`, 3008 v 2970). Put under the existing `FABLE_RETAIL_CARRAY` branch; the 3 landed rows that define it
  (0x00520670, 0x00522530, 0x00522720) still RELOCATION_MATCH with it.
- **Shader handle temporaries release through `C*ShaderData::Release()`**: `--ReferenceCount; if (CountReferences() == 0) delete this;`
  gives retail's `mov esi,[eax+10h] ; dec esi ; mov [eax],esi` for by-value temporaries; `operator=` keeps the local-copy `dec [mem]`
  form. Both are in the hand-back CVertexShader.h / CPixelShader.h (same as the landed 0x00B8B630 local model).
- **CVertexBufferWin32 construction order**: the base ctor sets Length / Type (and the CVertexCompressionParams unit scale / zero
  bias); the CVertexBufferWin32 ctor clears PVertexBuffer and Stride after its vtable store.
- 0x00B6EDF0 (parked): `C3DTriangle2::SetVertexIndex(i, v)` per index gives retail's in-order word stores, and the CGenericVertex
  setters in the order SetPosition, SetU1V1, SetDiffuseColour give the lazy constant loads; together they took the diff from 74
  to 16 lines. The residue is store scheduling only (see the Grade line in differ/00b6edf0.cpp).

## Headers handed back (hdr/engine/)
Opt-in branches (default path unchanged; `hdrcheck2`: default=same):
- `CArray.h`: FABLE_RETAIL_CARRAY also switches the index operators to `*(begin()+i)`.
- `CEngineComponent.h` (FABLE_RETAIL_CENGINECOMPONENT): the polymorphic root with the 14 retail slots and GlobalPointerAddress /
  RenderEnabled / KeepAlive.
- `CEngineWaterRenderer.h` (FABLE_RETAIL_CENGINEWATERRENDERER): `: CEngineComponent`, typed members at retail offsets, the virtual
  overrides and private helpers (now including BuildConstantBuffers).
- `CEngineResourceManager.h` (FABLE_RETAIL_CENGINERESOURCEMANAGER): retail CShaderDataBank is 0x128 (Ego_r 0x13C), so everything from
  VShaderResources on sits 0x230 lower. VShaderResources +0x29C0 (CCountedPointer<CShaderResource>[27]), ShaderResourcePixel
  +0x2A98, GPUScratchBufferTextureManager +0x3190; inline GetVShaderResource / GetPShaderResource; GetIndexBufferMemPool /
  GetIndexBufferUsage / GetVertexBufferUsage declared. This replaces the local resource-manager model 0x00B71880 had.
- `CEngineLandscapeRenderer.h`, `CEngineWorldMap.h` (`GetMapCount(){return MapList.size();}`), `CEngineMap.h` (inline
  Get/SetLandscapeMap), `CLandscapePatchTesselator.h`, `CVertexShader.h`, `CVertexBufferWin32.h` (inline ctor, Initialise),
  `CTriangleStripifier2.h` (CTriangleStripifierBase : CBaseClassNonCopyable per RTTI, implicit dtors, the index-array overload),
  `CEnginePrimitiveManagerParticleSpriteGroup.h` (typed retail members).
New headers: `CPixelShader.h`, `CShaderBase.h`, `CShaderResource.h`, `CVertexBufferBase.h` (inline protected ctor, Lock / Unlock,
inline GetSize), `CVertexCompressionParams.h`, `CEnginePrimitiveManager.h`, `CEnginePrimitiveRenderUtil.h`, `C3DTriangle2.h`,
`CEngineWaterSettings.h`, `CLandscapeTesselatorBase.h`, `CWaterPatchTesselatorBase.h`, `CLandscapeGPUUpdateQueue.h`,
`CEngineLandscapeMeshBuilder.h`.

## Names
- NAME_FIXES.tsv: 23 addresses. Row names (0x00B71880, 0x00B6D6D0, 0x00B6EDF0, 0x00C2A890) and the callees whose bound
  spellings are invented: the IB / VB Initialise / Lock / Unlock / GetSize family, the resource-manager buffer getters,
  CShaderResource::GetVertexShader / GetPixelShader (const, by value), vector<CVertexShader>::resize, CTriangleStripifier2's
  index overload, ??4CPixelShader (landed as CPixelShader::Assign).
- Global 0x01436E98: no PDB name exists. Landed code uses at least `g_pResourceManager` and `g_pEngineResourceManager_01436e98`
  (and many view-typed spellings). These rows and chunk 722 use `g_pResourceManager` (the CEngineResourceManager-typed binding);
  the coordinator should pick one spelling for the address.
- Descriptive: 0x00C2A890 `CNavigatorSwitch::SendStateAndReset` and its two sender callees (libpk, no PDB/RTTI).

## Not drafted (budget)
The six remaining rows (2.1 to 3.7 KB each) are render passes built from inline render-state helpers: capture-block push on
0x013CB508, the core's SetState / SetTexture / SetIndices caches, CShaderRenderManager::PrepareForRender, and for 0x00B77370 two
inline graphic-bank texture fetches (the 0x2E8BA2E9 divide-by-0x2C walk of the main graphic bank). The landed siblings
0x00B77E20 (RenderSeaNonReflectiveForeground), 0x00B75CA0 (RenderIceBackground) and 0x00B76BC0 (RenderBackground) model all of
these, but locally as pad structs. Reusing them needs retail branches of engine/CRenderManagerCore.h, CRenderStateManager.h, a new
CShaderRenderManager.h and the typed CEngineWaterRenderer branch. Disassemblies are in work/d_<addr>.txt.

## GOTCHAS (one line each)
- A user-declared empty destructor stores the vtable; to match a retail implicit destructor, leave it implicit and define the
  PDB constructor in the TU.
- STLport `size()` loads finish first; `end() - begin()` loads begin first and can keep it live.
- CArray `operator[]` written as `*(begin()+i)` folds constant indices into the addressing mode; `_M_start[i]` does not.
- Retail CShaderDataBank is 0x128 bytes, so CEngineResourceManager members after the shader banks sit 0x230 below Ego_r.
- CLandscapeGPUUpdateQueue is 4 bytes in retail (the dtor proves there is no TaskList); MeshBuilder is at +0x10.
- CEngine CIEngine forwarders need a local polymorphic CEngine; the flattened header cannot emit `UAE` virtuals.
- Constant index/vertex fills: inline setters (C3DTriangle2::SetVertexIndex, CGenericVertex::Set*) keep retail's in-order stores;
  plain `p[i] = k` lets VC7.1 group stores of the same value.
