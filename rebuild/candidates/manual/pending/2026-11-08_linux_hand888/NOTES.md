# Chunk 887 notes

## Extents (all four checked with xdis: last `ret` then 0xCC padding, no inner function starts)
0x00BA6240..0x00BA6DB9 = 2937 (ret 4); 0x00BAFA00..0x00BB096A = 3946 (ret 0xC); 0x00BB0970..0x00BB15F2 = 3202 (ret 0xC);
0x00BBAC60..0x00BBBAFF = 3743 (ret 4). One function each.

## Results
None matched. Session 1 parked 0x00BA6240 after 92 graded sources (74 diff lines). Session 2 (chunk 888) worked the other three:
0x00BAFA00 3884v3946/1373 lines -> 3905v3946/1091 lines (118 register+frame-blind), 0x00BB0970 359 -> 313 lines (108 blind),
0x00BBAC60 first draft 682 -> 87 lines (73 blind). Each differ/ file records what is left and the next idea.

## Parked for Fable 5.1 (4)
- 0x00BA6240 CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer DIFFER(2935v2937): frame-slot order (tol/centre/p), Dot order, Type hoist | tried: 92 sources (see differ/00ba6240.cpp) | next: c2prio --trace for the slot/priority order
- 0x00BAFA00 CEnginePrimitiveManager2DSprites::RenderClockPrim DIFFER(3905v3946): primitive pointer split edi->ebx (reload [ebp+74h] at 0x00BAFFB9), scene in edx without the MaskBankIndex CSE, flags memory-only at [ebp+68h], z/BL slot order, `list` in memory, lerp `fld st(1) ; fmulp`, index-loop zero register | tried: textures[2], reference-bound quad temps (biased frame = retail), fraction compare chain, pointer-local index loop, debug CRGBColour temporary | next: read the late sprite fields through `primitive` so the parameter is the late live range; c2prio --trace for colouring
- 0x00BB0970 CEnginePrimitiveManager2DSprites::RenderSpritePrim DIFFER(3170v3202): frame 0x80 v 0x74 -- with a plain quad temporary VC7.1 maps the flags/half_width/red/z slot onto the dead `primitive` argument home (frame 0x6c), with a reference-bound one the temporary keeps its own 32-byte slot | tried: reference-bound / block-scoped / plain quad, parameter used directly, CRGBColour temporary, 31 sources | next: RE the c2 parameter-home join (0x10790ad6 / 0x10790e34) to find the interference edge retail has
- 0x00BBAC60 CEnginePrimitiveManagerDecal::RenderLandscapeDecal DIFFER(3823v3743): default/REVSUBTRACT SetBlendOp expansion commutes the bookmark test (`mov esi,[edx+8] ; mov eax,[eax+2814h] ; test eax,esi`) so the three BlendOp cases do not cross-jump; colour CRGBFloatColour temporary copied (+80 bytes); index else-arm `lea eax,[ebx+eax+2]` | tried: switch layouts, bool-threaded premultiply (the retail block layout), channel-desc copy, light_count order, PeekGraphicBank accessor, C4DVector-outer colour (fixes the colour, re-packs the frame: `vertex` weight 3 -> 2) | next: keep the C4DVector-outer colour and restore `vertex`'s packer weight; lane gap for the bookmark commutation

## Label corrections
- 0x00A8D750 ghidra label `Vector_Construct_uint16` (forge-autoname) = `std::vector<unsigned short>::vector(size_type)` (landed STLvectorunsignedshort_vector_00a8d750.cpp; `CArray<unsigned short>(n)` in RenderLandscapeDecal 0x00BBB1CE).
- 0x00BB9FD0 ghidra label `std_vector_initialize_with_fill` = `std::vector<CTVertexNormTex1>::vector(size_type)` (landed STLvectorCTVertexNormTex1_vector_00bb9fd0.cpp; called at 0x00BBB0F4).
- 0x004372C0 still labelled `std::vector<bool>::operator[]` in labels_*.tsv; it is CTexture::GetHeight (NAME_FIXES.tsv).

## Header debt / local models
- 0x00BBAC60 (session 2): shared headers for the render state / shaders / lighting / capture block / CArray / vectors / colours; local
  models for CGraphicFrame(+Data, release inline), CGraphicInfo (GetFrame 0x009FF8D0), the bank (EntryExists, PeekGraphic), the
  resource manager (GraphicBanks +0x954, PeekGraphicBank), CEngineInternalPrimitiveDecal (Ego_r 0x94), CEnginePrimitiveManagerDecal
  (PointLightShaderIndex +0x54, LandscapeDecalVertexShader +0x6C, LandscapeDecalPixelShader +0xB4), CTVertexNormTex1(Base),
  CEngineLocalLightArray (LightCount). EGraphicBankNo has no recovered enumerator: the source passes `(EGraphicBankNo)0`.
- 0x00BA6240: the shared-header devices are in hdr/ (CMatrix3x4 MULVECTOR, CCountedPointer CALLED_DTOR). The source still has local
  stand-ins for SetupStaticBlocks / SetupFirstTextures / RenderMeshPrimitiveBlocksIndexBuffer, all same-TU bodies under
  `auto_inline(off)`. v20 uses fake non-clobbering bodies. v19 uses honest bodies: the real SetupFirstTextures through
  `GPRenderManager->AttachTextureToStage`, CStaticBlock DiffuseTexture +0x14 / BumpTexture +0x18, and
  C3DPrimitive2::StaticBlocks +0x44. It is 87 lines, gets the `mov ecx,ebx` reload right but loses the technique-in-esi load. A
  landed source should use the honest bodies.
- Conflict to resolve: in this TU the `C3DTextureStages` constructor leaves DiffuseTextureStage unset. The row sets it to 1 or 0 per
  branch, and retail's DSE does not remove a -1 store. That disagrees with the 0x00A90700 model (-1, -1, false).
- 2D sprite rows: engine/CGraphicFrame.h and engine/CGraphicInfoFrame.h are flat. CEngineInternalPrimitive2DSprite.h and
  CEngineInternalPrimitive2DClockSprite.h are generated flattened layouts with no base and split C2DBoxF / C2DVector members. So the
  differ sources carry local models (Ego_r.pdb layouts):
  - the landed GetFrame 0x0041C260 body;
  - the landed 0x00BA9A70 resource / frame-data models;
  - the 2D scene clip region (0x01436E84 -> +0x94 -> +0xC);
  - the resource manager's SpriteBank (+0x954, descriptive) and SatRGBATexture (+0x2ADC);
  - the 2D primitive renderer's LayerEnabled (+0x320);
  - the display manager's Viewport (+0x1F4, descriptive) and RenderTargetDimensions (+0x194);
  - the engine-settings displacement byte (+0xEE, descriptive);
  - the primitive renderer's debug-box byte (+0x25C, descriptive);
  - the interpolation-set table 0x0143B930 (Factor at +4, stride 12).

  Retail branches for those headers are the follow-up.
- Descriptive names to replace when the PDB gives them:
  - `SpriteBank` (+0x954)
  - `Viewport` (+0x1F4)
  - `DisplacementPassEnabled` (+0xEE)
  - `DrawDebug2DBoxes` (+0x25C)
  - `C2DClipRegion` / `CEngine2DScene` / `CEngine2DSceneOwner` and their fields
  - `CEngineInterpolationSet`
  - `CGraphicFrameData::GetTextureQuadUVs` (from the landed row)

## Decoded behaviour (RenderSpritePrim 0x00BB0970)
1. Reject against the viewport, then against the 2D clip-region box when it is masked and enabled.
2. Return when the colour's alpha is 0 or the layer is disabled.
3. flags = BlockOnLoad ? 0x11 : 0.
4. Frame 0 of graphic BankIndex through `PeekGraphicInfoFrame`. GetFrame fails → return. Otherwise texture and UVs, half-texel
   (0.5/Width, 0.5/Height) on U1, U2, V2, V3, plus the lerped UV offset. With no bank the UVs are the unit quad.
5. A linear-format texture scales U by its width and V by its height.
6. Alpha mask = the clip region's mask graphic (frame 0), or SatRGBATexture.
7. Rotate the box's half extents by the lerped angle (GFFastCos/GFFastSin) about the centre, convert to clip space, halve the colour
   (keeping alpha), take z from the layer.
8. Fill a CEngineSubPrimitive2DSprite.
9. With flag 0x400 and the displacement pass, queue it on lists 5 and 6 (as a copy). Otherwise queue it on `type`.
10. Optional debug box (Draw2DBoxOutline, red).

## Decoded behaviour (RenderClockPrim 0x00BAFA00)
1. Same rejects; flags = BlockOnLoad (0/1).
2. Mask frame first.
3. Frame of (BankIndex, Frame), then of (BankIndex2, Frame2); the second supplies the UVs only when the first is absent.
4. UV offset lerp, then the first linear texture's scale.
5. Seven vertices: centre, top-middle, TR, BR, BL, TL, and the sweep point.
6. Sweep: t = GFClamp(lerp(AngleStart, AngleEnd), 0, 1); segment = GFFloatToLongCeil(t*4+0.5). The fraction is
   `fmod(t-0.125, 0.25)*4` inside [0.125, 0.875], else `fmod(t, 0.125)*8`; outside (0, 1) it is the clamp.
   Vertex 6 = lerp(vertex[segment], vertex[segment==5 ? 1 : segment+1]).
7. 18 indices: the segment's triangle is split at vertex 6, and IndexOffset marks the split.
8. Texture[0] = second texture, Texture[1] = first; Colour/Colour2 are swapped.
9. Queue on list 5 if flag 0x400, else on `type`.
10. Debug box.
