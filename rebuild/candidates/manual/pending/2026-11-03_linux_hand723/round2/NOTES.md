# Chunk 723b (CEngineWaterRenderer render passes) -- hand-back notes

No row matched: `payload.json` has an empty `authored` list and `oracle.tsv` only its header. Four rows are parked in
`differ/` with every residue named; two rows were not attempted (see the end). Grade every `differ/` source with
`FABLE_HDR_OVERLAY=<this dir>/hdr`.

## Rows
| row | grade | residue |
|---|---|---|
| 0x00B70230 RenderSeaBackground | DIFFER(2138v2138), 24 diff lines | 3 of 7 SetRenderState bookmark test/or operand swaps (lane gap) |
| 0x00B761D0 RenderForeground | DIFFER(2531v2531), 48 | 6 of 8 test/or swaps (lane gap) |
| 0x00B751F0 RenderIceForeground | DIFFER(2722v2722), 48 | 6 of 7 test/or swaps (lane gap) |
| 0x00B77370 RenderSeaReflectiveForeground | DIFFER(2798v2734), 231 (247 on the final header) | retail calls ??_GCGraphicFrameData 0x0041BDF0 (`push 1`) for the last of three frame-data releases; VC7.1 here inlines all three; frame size, register swap and the rest follow from it |
| 0x00B70A90 RenderEnvironmentMap | not attempted | out of session budget |
| 0x00BC04F0 RenderSingleSprite | not attempted | out of session budget |
| 0x00B6EDF0 BuildConstantBuffers (task 3) | not attempted | |

## Header branches (hdr/ = full merged overlay; hdrcheck2: every CHANGED header default=same)
- `engine/CRenderStateManager.h` FABLE_RETAIL_CRENDERSTATEMANAGER: polymorphic class : CBaseClassNonCopyable with nested
  CStateInfo / CStateStackElement / CDeviceState (Ego_r layouts), all inline helpers `__forceinline`. Setters use the PDB
  parameter types: SetTextureUVModeU/V/W(long, TEXTURE_UV_MODE) map through the address-mode table 0x01396DC8
  (`g_textureUVModeToD3D`, descriptive; the enumerators are descriptive); SetAlphaRef(unsigned long), SetTextureCoordIndex(long,
  long), SetSpecularEnable(bool) added. The other enum setters (blend, cull, filter, mip, op, arg, transform) are left out until
  a row pins their tables. SetRenderState reads CurrentBookmarkMask directly (no `mask` local, see the lane gap below).
  CRenderStateCaptureBlock (RAII; static StateManager 0x013CB508) is in the same branch.
- `engine/CRenderManagerCore.h` FABLE_RETAIL_CRENDERMANAGERCORE (turns on CRENDERSTATEMANAGER / CTEXTURE / CINDEXBUFFER /
  CVERTEXBUFFERWIN32): : CRenderStateManager, typed own members +0x3A3C..+0x3D20; AttachTextureToStage /
  AttachD3DTextureToStage / SelectVertexBuffer / SelectIndexBuffer now `__forceinline` (plain inline left them out of line in
  RenderForeground: 2267v2531).
- `engine/CShaderRenderManager.h` (new): retail layout 0x3E8; SetPixelShaderConstant / SetBoneMatrixCount / PrepareForRender
  `__forceinline`.
- New this pass: `engine/CEngineShadowRenderer.h` FABLE_RETAIL_CENGINESHADOWRENDERER (RenderingTechnique +0x334 typed,
  nested CColorBufferDepthPackParameters, SetupShadowFuncRenderStates 0x00B50510; bytes below +0x334 opaque);
  `engine/CEngineLightingManager.h` retail branch gains GetLightsInArea / SetupShaderPointLights / DisablePointLights /
  UpdateGlobalLightingStates; `engine/CEngineWaterRenderer.h` GetIceForeground*VertexShader addresses.
- Rebased on main (the repo gained these since the hand723 overlay): `engine/C4DVector.h` (FABLE_RETAIL_C4DVECTOR: the
  CRGBFloatColour ctor and Initialise(x,y,z,w)), `engine/CMatrix3x4.h` (FABLE_RETAIL_CMATRIX3X4: 12-float ctor, chained
  SetIdentity, SetTranslateOnly), `engine/C3DTriangle2.h` (FABLE_RETAIL_C3DTRIANGLE2 = the hand723 class used by 0x00B6EDF0;
  default = main). The retail headers that need them define the macros before their includes.

## Regrade under the final overlay (work/regrade/summary2.txt)
Siblings 0x00B77E20, 0x00B75CA0, 0x00B76BC0 and the hand723 rows 0x00B36EC0, 0x00B67920, 0x00B6D6D0, 0x00B71880,
0x00BC14E0, 0x00C2A890, plus 0x006286F0, 0x00B677D0, 0x00BC9800: RELOCATION_MATCH (see the final report for the last run).

## Levers found
- Same-TU `__forceinline` member definitions for the Set*TexturesAndUVModes / Setup*ShaderConstantsAndFog helpers.
- GetGraphicInfo double lookup: index and bank in locals, declared index first; `GetGraphicInfo(i) ? GetGraphicInfo(i)->Frames : 0`
  gives retail's `cmp/sbb/and` select.
- GetFrame touch: read `GraphicInfo->Bank->CurrentFrame` twice (no `frame` local) for retail's register choice.
- CGraphicFrame as a local with the FableWin.pdb ctor / dtor / private ReleaseData: VC7.1 drops the dtor's null test after
  the frame was dereferenced (GetTexture() for the stage attach), which is retail's unchecked release.
- Ice loop: copy the bounding box, then the mesh, then zero the theme-channel bytes (frame slots and store order).
- SetRenderState without a `mask` local: a constant-value setter (SetSpecularEnable(true)) loads the state's mask first.

## Lane gap (not source-movable)
The bookmark `test` / `or` operand order in the inlined SetRenderState varies block by block within one retail function
(0x00B70230: blocks 1-3 `test eax,esi`, 4-7 `test esi,eax`; 0x00B761D0: 1-6 vs 7-8) and every spelling (mask local or not,
either operand order, `x = mask | x`, value by const& or through the enum table) compiles identically.

## Header / naming debt
- The graphic-bank chain (CGraphicDataBank, CGraphicInfo, CGraphicInfoFrame + CFrameData, CGraphicFrameData, CManagedTexture) has
  no engine header; the rows reuse the landed 0x0041C260 / 0x00BA9360 local models. CResource / CResourceList / CGraphicFrame
  headers exist but their retail branches keep the list head and frame counter private and CIVCountedPointeeBase slot 1 is
  Delete(): a header pass should make CResource::TouchResource / CGraphicFrame's ctor / dtor / ReleaseData / operator= /
  GetTexture and CGraphicFrameData::Release / AddReference / GetTexture header members.
- CEnvironmentDef: no header; retail graphic indices are 4 bytes below Ego_r (IceTexture +0xCC, WaterEdgeAlphaMap +0xD0,
  WaterSurfMap +0xD4; read by 0x00B751F0 / 0x00B761D0 / 0x00B77370).
- CEngineLocalLightArray / CEnginePrimitiveThemeChannelDesc: no headers (Ego_r layouts in 0x00B751F0).
- CWaterPatchMesh::Render(long) const 0x00BEACA0 is added by a layout rename in the rows (header lacks it).
- Globals: 0x01436E98 kept `g_pResourceManager` (no PDB name; main also binds g_pEngine_01436e98 / g_pEngineData ...);
  0x01436E9C `g_pEngineLightingManager` and 0x01436E60 `g_pShadowRenderer_01436e60` follow the majority landed spelling;
  0x01436E8C is `g_pEngineWorldMap` here but `g_pEngineState_01436e8c` in 8 landed files (descriptive both; unify).
