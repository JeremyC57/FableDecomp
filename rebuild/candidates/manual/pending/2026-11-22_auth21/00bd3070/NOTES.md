# 0x00BD3070 CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive: AUTH21 NOTES
Base: AUTH20's `b6.cpp` (AUTH1 / p25S), DIFFER(11288v11288), stock flags. Final: `00bd3070.cpp` (= c6), DIFFER(11288v11288), rc 151.
The state is ESCALATED (ESCALATE.md). Overlay `hdr/`: AUTH1's 10 headers (MorphInfo rebased onto the current repo header, which had gained
_DECALS_PASS26; CEngineInternalPrimitiveBase's unused ALPHAFADE block dropped) plus CRenderManagerCore.h, GFGetNextPowerOfTwo.h and
CEnginePrimitiveManagerMeshBase.h. `hdr_00bd3070.patch` is the whole overlay-versus-repo diff.

## Where each former row-local body now lives (macro the row defines; N = new add-only opt-in, E = existing)
| member | header | macro | text |
|---|---|---|---|
| CGraphicDataBank::PeekGraphic | CGraphicInfo.h | E _PEEKGRAPHIC + N _PEEKGRAPHIC_FORCEINLINE (keyword only) | landed 0x00BB8EF0 `&GraphicList[index]` (b6: `.begin()[index]`, +30 rc) |
| CGraphicDataBank::PeekGraphicInfoFrame | CGraphicInfo.h | E _PEEKGRAPHICINFOFRAME | same |
| CGraphicInfoFrame::GetTexture | CGraphicInfo.h | E _GETTEXTURE_INLINE | wfA5 text (b6's spelling differs, bytes equal) |
| CRenderStateManager::SetTextureFactor | CRenderStateManager.h | E _SETTEXTUREFACTOR_INLINE | same |
| CShaderRenderManager::SelectVertexBuffer | CShaderRenderManager.h | E _SELECTVERTEXBUFFER (plain in-class) | same |
| CRenderManagerCore::FlushRender | CRenderManagerCore.h | N _FLUSHRENDER_FORCEINLINE (in-class) | 7 landed rows' text |
| CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade | CEngineInternalPrimitiveBase.h | E _GETALPHAINCLUDINGDISTANCEFADE + _FORCEINLINE | landed 0x00BA7B30 |
| CEnginePrimitive2DViewportManager::GetCurrentAlphaMask | CEnginePrimitive2DViewportManager.h | E _ALPHAMASK | same |
| CEnginePrimitiveRenderUtil::ApplyPixelShader / SetTextureFactor / Set2DAlphaMaskTextureMode / DisableTextureFiltering / SetupAlphaBlendingStatesFromRenderFlags | CEnginePrimitiveRenderUtil.h | N _APPLYPIXELSHADER / _SETTEXTUREFACTOR / _ALPHAMASKTEXTUREMODE / _ALPHABLENDSTATES _FORCEINLINE (in-class) | b6 = landed in-row text (SetTextureFactor: the 0x00BA1B00 `C4DVector(colour.R, ..)` form; 5 other rows use `C4DVector(colour)`) |
| CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo | CEnginePrimitiveRenderUtil.h | N _GETCURRENT2DALPHAMASKINFO_FORCEINLINE (after the class: include cycle via CEnginePrimitive2DViewportManager) | landed text |
| CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier (dependency) | CEnginePrimitiveRenderUtil.h | N _GETMIPMAPSIZEMULTIPLIER_INLINE (plain, after the class) | same |
| GFGetTopBitInByte (dependency) | GFGetNextPowerOfTwo.h | N FABLE_RETAIL_GFGETTOPBITINBYTE | landed text. **Needs header claim**: no shared home existed; GFGetNextPowerOfTwo.h holds the sibling GFGetTopBit |
| CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap | CEnginePrimitiveManagerMeshBase.h | E _GETAPPROPRIATEMIPMAP + N _GETAPPROPRIATEMIPMAP_FORCEINLINE (after the class) | b6 text. The 5-row landed text measures 11272 (rc 397). FableWin's locals match neither |
| C3DVector(float,float,float) | C3DVector.h | E _CTOR_FORCEINLINE | closes the one unexpanded call at 0x00BD4D13 |

Still row-local (plain `inline`, not FABLE_FORCEINLINE, and no moved body needs them):
- the CEnginePrimitiveRenderUtil GetPixelShader* getters and GetPixelShader2DDiffuse* / SetWorldTransform;
- CEngineSubPrimitiveAnimatedMesh::PeekObjectTransform;
- CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocks.
These are listed for a later pass, as the advisor's scope note asks.

## Names (fixed in the row; no NAME_FIXES needed for landed code)
- 0x01436E84: b6 spelled it `g_pEnginePrimitiveTypeInfo` ("no PDB name found"). The final row uses `NEngineInternal::GPPrimitiveTypeInfo`, the
  spelling 40+ landed rows use. It also matches the NEngineInternal::GP* family (GPRenderManager 0x01436E18, GPShaderRenderManager
  0x01436E14). The PDB symbol dumps carry no globals, so the name stays the landed one.
- 0x0143B990: b6 spelled it `g_highestSetBitTable`. Now `GTopBitInByteTable` (9 landed rows; the Anniversary PDB name cited there).
  GFGetTopBitInByte is a FableWin.pdb S_GPROC32.
- The new macro names are descriptive opt-in names. Every member name is the PDB spelling.

## Proof of the header changes
- `pin_asserts.py rebuild/include hdr <all 12>`: 1 pinned. That is CEngineSubPrimitiveAnimatedMesh.h, where AUTH1's addition had moved
  `sizeof==36` (now `_AT(12, ...)`). The row regrades to the same bytes (506c5c674d66).
- The patch removes nothing else. Its only other `-` line is PeekGraphic's `inline const CGraphicInfo* ...` head, split into an
  `#ifdef .._FORCEINLINE FABLE_FORCEINLINE #else inline #endif` keyword switch (the default tokens are unchanged).
- `equiv_check.py rebuild/include <repo include + overlay> equiv.tsv` over every landed row: result in `equiv.tsv` / `x/equiv.out` (see STATUS).
