# hdr/ overlay (chunk 3017; based on chunk 2008's overlay)

Grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3017/hdr`. Chunk 3017 added the three entries in "Chunk 3017 additions" below; everything else is chunk 2008's overlay unchanged. Every change to an existing header is an opt-in macro (the
default path is untouched, so landed includers keep their bytes). Each modified header started as a copy of the current repo header,
except the four marked "from c2005", which are the 2026-11-11 acctB chunk 2005 hand-back's overlay copies (their own opt-ins kept).

## New headers
| header | content | evidence |
|---|---|---|
| `engine/CRenderTarget.h` | CRenderTarget : CBaseClass; RenderToSurface +0x04, ZBufferSurface +0x14 (CSurface), sizeof 0x24; inline default ctor, implicit dtor; DetachTarget / DetachZBuffer / Uninitialise / AttachTarget / AttachZBuffer / two-surface ctor declarations, GetPRenderToSurface / GetPZBuffer | Ego_r.pdb layout (36 bytes) + name set; retail 0x00A10E10..0x00A10E90 in Ego_r donor order; 0x00B66632.. builds it inline, 0x00B66A2B..0x00B66A56 destroys it with no own vtable store |
| `engine/CEngineComponentRenderParams.h` | CEngineComponentParams (FrameInterpolation +0, opaque InterpolationInfo, FrameID, SceneFilterFlags, OffWorldTickProcessFlag; 0x80) and CEngineComponentRenderParams (Params, RenderLayer, RenderLayerFlags; 0xC) | Ego_r.pdb layouts; 0x00B667C2 reads Params->FrameInterpolation |
| `engine/CVolumeTexture.h` | polymorphic CVolumeTexture: PD3DVolumeTexture +4, ByteLength +8, PData +0xC | Ego_r.pdb layout (16 bytes) + ??_E UAE; 0x00B54926 reads +4 |
| `engine/CWeatherMask.h` | BSphere +0, BBox +0x10, InvTransform +0x28, Scale +0x58 (0x5C) | Ego_r.pdb layout; SetCommonParticleShaderConstants 0x00B52160 / ClipBoxToWeatherMaskSphere 0x00B52090 offsets |
| `engine/CEngineClothRenderer.h` | polymorphic class, VertexNormals +0x84, PositionBuffer +0xC4, NormalBufferPool +0xCC, NormalBuffer +0xE8, NormalBufferSize +0xEC (sizeof 0xF0); GetNormalBuffer / CalculateVertexNormals | Ego_r.pdb layout with 12-byte STLport CArrays (six arrays, -0x18); GetNormalBuffer 0x00BB1E90 body (+0xEC / +0xCC / +0xE8) |
| `engine/C3DClothRenderVertex.h` | PositionIndex +0, TexCoordIndex +2 | Ego_r.pdb layout; 0x00BB204B `movzx edi,word ptr [ecx+eax*4]` |
| `engine/NVectorMath.h` | NVectorMath::GFClear4DVectors / GFNormalise3DVectors (cdecl), `extern bool g_SSEAvailable` | Ego_r.pdb / FableWin.pdb spellings; reloc_bindings (established) |
| `engine/CEngineClothRendererSSE.h` | **asm helper** GF_SSE_ADD_TRIANGLE_NORMAL (macro-wrapped `__asm`, `FABLE_PORTABLE` twin fable_portable::AddTriangleNormalSSE) | retail 0x00BB2079..0x00BB20ED; see the header block and NOTES.md (row parked; attempts table incomplete) |
| `tests_portable/test_CEngineClothRendererSSE.cpp` | dual-path test for the helper (for rebuild/tests/portable/) | PASS: g++ portable, VC7.1 asm, VC7.1 asm+twin |

## Opt-ins in existing headers
| header | opt-in | content | default path |
|---|---|---|---|
| `engine/CThing.h` | `FABLE_RETAIL_CTHING_CONSTRUCTFROMBINARYSAVE` (inside LOADSINGLE) | private `bool InitFromThingInfo(const CSerialiseInfo&)` 0x004C7F40 | unchanged |
| `engine/CThingManager.h` | `FABLE_RETAIL_CTHINGMANAGER_THINGTYPEFLAGS` | `unsigned long ThingTypeToTypeFlags(long) const` 0x0051E580; inline `PeekComponent() const` (FableWin) | unchanged |
| `engine/CEngineCameraDesc.h` | `FABLE_RETAIL_CENGINECAMERADESC_STRUCT` | `struct` key (PDB U: ??0CEngineCameraDesc@@QAE@ABU0@@Z, SetCameraState ABU) | `class` (header debt: the key is wrong by default) |
| `engine/CEngineCamera.h` (POSITION branch) | `FABLE_RETAIL_CENGINECAMERA_CAMERASTATE` | inline PeekCameraState (FableWin), SetCameraState 0x00B30B50, RefreshD3DTransforms 0x00B2FC50 | unchanged |
| `engine/CEngineCamera.h` (POSITION branch) | `FABLE_RETAIL_CENGINECAMERA_NONHOMOGENOUS` | inline PeekNonHomogenousViewTransform / PeekInverseNonHomogenousViewTransform (FableWin; +0x114 / +0x144) | unchanged |
| `engine/CSurface.h` | `FABLE_RETAIL_CSURFACE_ASSIGN` | `CSurface& operator=(const CSurface&)` 0x009F2E70 | unchanged |
| `engine/CDisplayManager.h` | `FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET` | `bool SetRenderTarget(CRenderTarget&)` 0x009BF160 (+ SetRenderTargetAsBackScreen when BACKSCREEN is off) | unchanged |
| `engine/CEngineSkyRenderer.h` | `FABLE_RETAIL_CENGINESKYRENDERER_ENVMAP` | private RenderSky(float, const C3DVector&, bool) 0x00B66190, public GetEnvironmentMap 0x00B665E0 | unchanged |
| `engine/CEngineWeatherRenderer.h` (from c2005) | `FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERRAIN` (inside BUFFERS) | typed shaders +0xD0..+0x108, WeatherMaskList[3], CurrentMask, SavedBoxVisibility[729] +0x784, nested enum EWeatherBoxVisible (descriptive enumerators), ClipBoxToCamera / ClipBoxToWeatherMaskSphere / ClipBoxToLandscape / SetCommonParticleShaderConstants / RenderRainBox / RenderRain | c2005 BUFFERS layout unchanged |
| `engine/CEngineWeatherSettings.h` (from c2005) | `FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL` | typed tail ParticleBoxPos[81] .. SnowSpeedFactors[5] (Ego_r, no containers) | opaque tail |
| `engine/CEngineResourceManager.h` (from c2005) | -- | copy only (c2005's VBPOOL opt-in) | -- |
| `engine/C4DVector.h` (from c2005) | -- | copy only (c2005's ADDASSIGN used by the cloth row) | -- |
| `engine/CGraphicInfo.h` | `FABLE_RETAIL_CGRAPHICINFO_GETVOLUMETEXTURE` | `bool GetVolumeTexture(CVolumeTexture**, unsigned long) const` 0x009FFB20 | unchanged |
| `engine/CRenderManagerCore.h` | `FABLE_RETAIL_CRENDERMANAGERCORE_VOLUMETEXTURE_INLINE` | FABLE_FORCEINLINE AttachVolumeTextureToStage through AttachD3DTextureToStage | declaration (unchanged) |
| `engine/CParticleProgram.h` | `FABLE_RETAIL_CPARTICLEPROGRAM_RENDERACCESS` | typed RenderVertices (CArray<C3DClothRenderVertex>), FableWin inline GetRenderTriangleCount / PeekRenderTriangles / GetRenderQuadCount / PeekRenderQuads / PeekRenderVertices / GetVertexPositionCount | opaque RenderVertices |

## Chunk 3017 additions (row 0x00BB1F50 CEngineClothRenderer::CalculateVertexNormals)
| header | opt-in | content | default path | evidence |
|---|---|---|---|---|
| `engine/C3DVector.h` (copy of the repo header) | `FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP` | `operator^` with each component grouped: `C3DVector((Y*o.Z-Z*o.Y),(Z*o.X-X*o.Z),(X*o.Y-Y*o.X))` (no code; three PAREN tuples) | unchanged | FITTED: c2 scheduler DAG dump + diagnostic paren ruler place the 81-tuple cuts of the triangle arm (0x00BB21CC..0x00BB221B) and the quad loop; the only per-helper distribution that fits both loops. Regrades all 6 landed operator^ users unchanged (NOTES.md): candidate single text for operator^ |
| `engine/C3DTriangle2.h`, `engine/C3DQuad2.h` (copies of the repo headers) | `FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS` | inline `unsigned long GetA() const` .. `GetC()` / `GetD()` returning VertexIndex[0..3] | unchanged | FableWin.pdb ?GetA@C3DTriangle2@@QBEKXZ .. ?GetD@C3DQuad2@@QBEKXZ (debug COMDATs 0x02E77990..0x02E77A50 in the cloth TU's run after CalculateVertexNormals; fwsize SAME 9i/17-18b) |

`C4DVector.h` and the other chunk 2008 copies are unchanged. No shared default body changed, so no landed row needs a regrade.

Re-base before landing: the copies were taken on 2026-11-11; diff each against HEAD and re-apply the opt-in blocks if HEAD moved.
