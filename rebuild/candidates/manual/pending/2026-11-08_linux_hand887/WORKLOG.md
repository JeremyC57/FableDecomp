# Chunk 887 worklog
Extents verified (xdis, last ret + 0xCC padding, no inner padding):
- 0x00BA6240 ends 0x00BA6DB9 (ret 4 at 0xBA6DB6) = 2937 bytes, one function
- 0x00BAFA00 ends 0x00BB096A (ret 0xC) = 3946, one function
- 0x00BB0970 ends 0x00BB15F2 (ret 0xC) = 3202, one function
- 0x00BBAC60 ends 0x00BBBAFF (ret 4) = 3743, one function

## 0x00BA6240 RenderPrimitiveToShadowBuffer (session 2)
Probe line: v7 (351 dl) -> v9 GetAppropriateMipmap helper (228) -> v11 GETASCARRAY_START + explicit ClipSphere sum (221)
-> v12 CCountedPointer dtor called (overlay opt-in) (196) -> b1/v13 UseBlank store after the mipmap call + single-return
mipmap (149, length 2937) -> c5/v14 C3DTextureStages ctor leaves DiffuseTextureStage unset, else-branch store (141)
-> s1/v15 same-TU SetupStaticBlocks body (95: the bool out-param no longer escapes, frame top now radius/mipmap)
-> s2/v16 same-TU SetupFirstTextures/RMPBIB (88) -> f1/v17 PeekSceneData() temporary instead of a named copy (80; also
fixes the SetRenderState test/or operand order) -> v19 realistic same-TU bodies (87; `mov ecx,ebx` reload right, technique esi lost).
Findings: x87 product order, test/or operand order and frame slots all follow VC7.1 symbol numbering (a temporary vs a named
local moves them); member-function inlines (GetMagnitude/Dot) fix the product order, a named-local explicit sum controls it;
/d2inlS56..100 needed for capture ctor + shaders dtor + GetHiddenMaterials + PeekGraphicInfo (S50-54 too small).
Open: tol vs centre slot order, p vs induction-offset slot sharing, plane Dot order, Type/RenderInfo store order, C4DVector int copy.

## Session 3 (2D sprite rows)
- 0x00BB0970 RenderSpritePrim (20 grades): v1 1047 -> v3 351 (landed GetFrame body, vertex layout X,Y,Z,DiffuseColour,U1,V1,U2,V2)
  -> v12 PeekGraphicInfoFrame inline (sbb select) -> v14 named mask region 359 / 182 frame-blind (ndx.py). Open: flags/half-width
  packed into the dead `primitive` argument home (+67 bytes of disp32), rep movsd source, ebp zero register vs mask index.
- 0x00BAFA00 RenderClockPrim: full decode, one compiled draft (DIFFER 3884v3946, 1373 lines).
- 0x00BBAC60 RenderLandscapeDecal: not drafted (callee map in differ/00bbac60.cpp).
- Header overlay: CTexture FORMATQUERIES + CPixelFormat ISLINEAR added; C3DVector opt-ins and chunk887/sprite_common.h retired.
