# Chunk 731 -- CEngine render-state defaults, Initialise, and the CEngineStateBlock Apply twins

Result: 5 of 6 matched (RELOCATION_MATCH, vland behaviour PASS on the full mirror `cp -r rebuild/include mirror; cp -r hdr/. mirror/`):
0x00B573A0 / 0x00B57860 / 0x00B57D20 / 0x00B581E0 CEngineStateBlock{Alpha,Alpha2X,AdditiveAlpha,AddSmoothAlpha}::Apply and
0x00B26360 CEngine::Initialise. Parked: 0x00B24850 CEngine::SetDefaultRenderStates DIFFER(4344v4344), 6 normalized diff lines,
the documented bookmark operand-order lane gap (differ/00b24850.cpp line 2).

Regrade: `FABLE_HDR_OVERLAY=$D/mirror LANDVERIFY_WORK=$D/lv python3 -u $T/vland.py payload.json oracle.tsv` (run per row:
pl/<addr>.json, 120 s each).

## Levers
- Apply twins: the inline CRenderStateManager setters (FABLE_RETAIL_CRENDERSTATEMANAGER) on NEngineInternal::GPRenderManager
  0x01436E18. The texture-op / arg / fog / dither / lighting / anisotropy / transform-mode setters have no out-of-line copy in
  Ego_r.pdb; FableWin.pdb has the debug bodies, so they are `__forceinline` in the retail branch, each through its private
  D3D table (addresses in the header comments).
- Initialise: def names are string literals converted at the call; the explicit `CCharString("...")` form is 5151v5174 (see
  GOTCHAS). The inline GetDef(name, index, def) body is in the TU (as in landed 0x004084B0 and chunk 727's 0x00B625E0).
- SetDefaultRenderStates: stage 0's TextureTransformMode is set inside loop 2 (retail quirk). Every block but that one matches;
  tried: loop forms and counters, an alias of GPRenderManager, unused locals, index spellings, header mask-operand swaps,
  a CStateInfo& local in the setter, /G6 /G7 /GB /Oa /Ow (LOG.tsv).

## Header overlay (hdr/engine; hdrcheck2 default=same for every changed header, run from a scratch copy so the stubs stay
## in tools/hc/stubinc -- nothing written under the tree, no git)
Changed retail branches: CRenderStateManager (inline setters), CRenderManagerCore (SetTextureEnable inline, EnableXBoxShaderConstants
0x00A0AA20), CDisplayManager (AddDeviceResetCallback 0x009C0190, GetVertexShaderVersion 0x009BEDE0), CIEngine (slots 17..84 with PDB
signatures; the 4-arg SetChildPrimitive is declared before the 5-arg one because retail lays the pair out in reverse), CDefinitionManager
(chunk 727's GetDef(name,index,def) + GetPDefFromClassIndex, plus GetDef(name,def) 0x00B28E40 and GetPDefFromInstantiationName
0x009ADA40), CEngineResourceManager (tree + ctor 0x00B3CB30), CEngineWorldMap (sizeof 0x1C0 from retail `new`; members after
MapList +0x20 opaque; ctor 0x00B423F0; the unproven GameWorld/StaticMapOpenState/CommonHeaderPosLookupTable offset asserts dropped --
landed 0x00B67920 uses MapList only and still matches), CEngineLandscapeRenderer (+PeekSceneFilterFlags), CEngineLightingManager
(chunk 727's + SetupDefaultFogStates 0x00B47630, ctor 0x00B482A0, the overrides).
New FABLE_RETAIL_<CLASS> branches: CEngine (full retail layout, sizeof 0x178, CIEngine +0 / CDeviceResetCallback +0xC), CEngineStateBlocks
(the ten CEngineStateBlock<Mode> classes + CEngineStateBlocks : CEngineComponent, 0x34), CDefaultEngineMemoryInit (0x28), and
generated ctor + PeekSceneFilterFlags branches with RTTI bases and the retail `new` sizes for CEnginePrimitiveTypeInfo 0xCC,
CEngineOcclusionManager 0x240C0, CEnginePrimitiveOcclusionManager 0x38, CEngineEnvironment 0x654, CEngineCamera 0x228,
CEngineBillboardGenerator 0x20, CEngineScreenEffectOutlineGlow 0x1E4, CEngineScreenEffectDisplacementRenderer 0x1AC,
CEngineSubPrimitiveRenderer 0x190, CEnginePrimitiveRenderer 0x350, CEnginePrimitiveRenderer2D 0x374,
CEnginePrimitiveAttachmentManager 0x14, CEngineSurfaceCompositionManager 0x10DC, CEngineSimulationManager 0x1078,
CEngineShadowRenderer 0x340, CEngineSkyRenderer 0x194, CEngineWeatherRenderer 0x12FC, CEngineScreenEffectColourFilter 0xEC,
CEngineScreenEffectGlowRenderer 0x280, CEngineScreenEffectRadialBlurRenderer 0x22C (members opaque `_pad_0xNN`).
New headers (one model, no macro): CEngineInit (retail 0x64, no DebugString), CEngineMemoryInit (0x28), CEngineLayerRenderer (0x1C;
inline ctor and operator<< over AddComponent 0x00B2AC80), CEngineLocalDetailGenerator (0x7C), CEnginePrimitiveRendererSharedData
(0x268), CEngineDef (0x80), CEngineVideoOptionsDef (0x78), CEnvironmentDef (0x134) -- the three def classes are : CDefClassBase with
one opaque block, sizes from their GetSizeofClass bodies (0x00433FD0 / 0x00434110 / 0x004558D9).

## Merge notes for the coordinator
Other scratch overlays touch the same headers: CEngineSkyRenderer.h (chunk727: full typed retail branch with the same 0x194 and ctor --
prefer it, add PeekSceneFilterFlags), CEngineShadowRenderer.h (agent723b/c, chunk727: a base-less member subset -- this chunk needs the
RTTI bases CEngineComponent for `layer << g_pEngineShadowRenderer`), CEngineCamera.h (723b/c, 727: typed members, same 0x228; needs the
`CEngineCamera(bool)` ctor), CEngineResourceManager.h / CEngineWorldMap.h / CEngineLandscapeRenderer.h / CEngineLightingManager.h /
CRenderManagerCore.h / CRenderStateManager.h (723*, 727), CDisplayManager.h (719*, 723c), CIEngine.h (724b/c, 725/b, 728/b),
CDefinitionManager.h (727, 728b), CEngineScreenEffectDisplacementRenderer.h (723c). Chunk 727's 0x00B625E0 has local CEngineInit /
CEnvironmentDef models; with this chunk's engine/CEngineInit.h and engine/CEnvironmentDef.h landed it should include them (its
CEnvironmentDef members +0x118..+0x128 go into the new header's opaque block).

## Siblings (SIBLINGS.tsv)
24 landed rows that define a changed retail macro (directly or through a header that defines one) regraded with --recheck-landed on
the final mirror: all RELOCATION_MATCH / MATCH and PASS (incl. 0x009DA9F0, 0x00434A10/20, 0x00B67920, 0x00B3BE10, 0x0047A9F0).

## Names
Globals 0x01436E10 / 0x01436E20 / 0x01436E24 / 0x01436E28 stay descriptive (no PDB data symbol); the component pointers keep the
established spellings (NAME_FIXES_QUEUE c722/c723/c639). NAME_FIXES.tsv: SetTextureTransformCount, EnableXBoxShaderConstants,
SetUVCompressionVertexShaderConstant, GetVertexShaderVersion, the 0x01436E3C outline-glow pointer (bound as g_pWaterRenderer), the
CHandleBasedMemoryPool / CEnginePrimitiveTypeInfo ctor swap in landed 0x00A148D0, SetDefaultRenderStates' private spelling (landed
0x00B25C00 declares it public), and GetPDefFromClassIndex (confirms chunk 727).
