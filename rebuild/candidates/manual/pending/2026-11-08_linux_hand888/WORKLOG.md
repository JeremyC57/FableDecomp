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

## Session 2 (chunk 888, 2026-11-08) -- 0x00BAFA00 RenderClockPrim
- Frame solved by probe, not by grades: `fas.sh` (/FAs listing: `_name$ = off ; size` per local), `pk.sh` (c2 frame-packer list:
  size, weight w14 = reference count) and `ord.sh` (packer processing order). Rules learned (c2 13.10.3077, regime 0):
  * a frame slot is a set of non-interfering symbols; slot size = largest member, smaller members sit at the slot's TOP end
    (retail `textures[2]` at box+8, `half` at TR+4, flags at BL+4);
  * a block-scoped debug `C2DBoxF box` is tracked and packs; the GetTextureQuadUVs result spelled as a plain assignment temp
    has weight 1 and packs with the box, which pushed `uvs` to the frame bottom (frame 0x7c);
  * VC7.1 picks the biased ebp frame (`lea ebp,[esp-N]`) from the WEIGHT of the frame symbols out of disp8 range, not from the
    frame size: probe fc20/fc22 (1 ref out of range: plain `mov ebp,esp`; 3 refs: biased); RenderClockPrim at frame 0x8c stays
    unbiased with two weight-1 temps and becomes `lea ebp,[esp-0x6c]` with `const CTextureQuadUVs& quad = frame->GetTextureQuadUVs();`
    (temps `$S1`/`$S2`, weight 2 each) -- the landed Draw2DGraphic 0x009DCA80 spelling.
- f1: `const CTexture* textures[2]` (retail [ebp+0x18]/[ebp+0x1c] = box+8/box+12, one 8-byte symbol right-aligned in the 16-byte
  box slot) + the fraction chain `if (angle <= 0.0 || angle >= 1.0f) clamp else if (angle < 0.125f || angle > 0.875f) fmod8 else fmod4`
  (retail's jnp/je/jnp/je compare encodings) -> frame 0x8c, layout = retail except the z / BL+flags slot order.
- f2 = f1 + reference-bound quad temps: biased frame `lea ebp,[esp-0x6c] ; sub esp,0x8c` = retail. DIFFER(3886v3946), 1130 diff
  lines (was 1373). Open: primitive edi->ebx split, scene in edx without the MaskBankIndex CSE, flags memory-only, `following`
  in a register, z slot below the BL/flags slot.
- l2 (loop): `unsigned short* indices = sub->Indices;` + index arithmetic on the pointer (no strength reduction) gives retail's
  index eax / i ecx / following edx; DIFFER(3905v3946), 1091 diff lines, 118 register+frame-blind. Best = differ/00bafa00.cpp.
  Mini probes (exp/loop/): an `index++` store in both if-arms is always hoisted as a common head; only differing arm IL avoids it.

## Session 2 -- 0x00BB0970 RenderSpritePrim
- sp_q1 = session-1 draft + `const CTextureQuadUVs& quad = frame->GetTextureQuadUVs(); uvs = quad;`: flags/half_width/red/z leave
  the dead `primitive` argument home (DIFFER(3170v3202), 313 lines, 108 register+frame-blind) but the reference keeps the 32-byte
  temporary alive to the end of the bank block (own slot, frame 0x80 v 0x74). Retail slot map (fpomap.py): uvs -0x20, target -0x28,
  quad/box/corner3/offset -0x48, centre/cosine -0x50, corner0 -0x58, corner1/across -0x60, this -0x64, texture -0x68,
  corner2/half_height -0x70, flags/red/z/half_width -0x74.
- Plain temporary (sp_b0), block-scoped reference (sp_q2), CRGBColour(255,0,0,255) temporary (sp_q3/q4), the parameter used
  directly everywhere (sp_p1) or in the sub-primitive constructor (sp_v1): all give the 0x6c frame with the flags slot on the
  argument home. pk.sh lists are identical for b0 and q1: the difference is an interference edge, not the weights. Open.

## Session 2 -- 0x00BBAC60 RenderLandscapeDecal (first draft this session, 20 grades)
- Draft from the shared headers (CRenderManager / CRenderStateScopedCaptureBlock / CScopedShadersEnable / CShaderRenderManager::
  PrepareForRender(9) / CEngineLightingManager::RestoreDefaultPrimitiveParameters / GFFloatToLongFloor/Ceil / CArray) + local
  frame / bank / decal models: a0 DIFFER(3906v3743) 682 lines.
- Block layout: retail's blend-mode cases 3/4 sit after the index loop's then-arm and share one dest-set + premultiply tail.
  Neither per-case colour code (a0/a1), `case 3: case 4:` with a conditional dest (a3, setne index), nor an `if (mode==3||mode==4)`
  after the switch (a4, not threaded) gives it; a `bool premultiply` set in the cases and tested right after the switch IS
  jump-threaded by c2 (a5: 425 lines): the dest-sets of 3/4 then cross-jump at machine level (`mov esi,[K] ; jmp`).
- Colour: `CRGBFloatColour colour; if (premultiply) { colour = CRGBFloatColour(decal.Colour); colour.R *= colour.A; ... } else
  colour = CRGBFloatColour(decal.Colour); SetPixelShaderConstant(2, C4DVector(colour));` keeps the threading (a9b: 97 lines);
  constructing the colour before the `if` (a8) loses it.
- `CEnginePrimitiveThemeChannelDesc channels; channels = CEnginePrimitiveThemeChannelDesc();` (the landed SetUpPointLights
  0x00BBC0B0 spelling) gives retail's extra 4-byte copy and frame 0x64; `light_count = 0;` before SetSelfIlluminationAmbient(100);
  `width + corner + 1` in the then-arm; PeekGraphicBank accessor for the bank (prologue order). Best c2/c6 = differ/00bbac60.cpp:
  DIFFER(3823v3743), 87 lines, 73 register+frame-blind.
- Open: the default/REVSUBTRACT SetBlendOp copy commutes the bookmark test (CRenderStateManager.h lane gap) and so does not
  cross-jump; the colour temporary copy (+80 bytes); a C4DVector-outer colour (dc_c3) fixes that region but drops `vertex` to packer
  weight 2 and re-packs the frame.
