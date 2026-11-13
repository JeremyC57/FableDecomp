# 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive (bwf1 author)
- grades ~25: best DIFFER(4540v4540) 102 hunks (c/v1.cpp = differ/00bb3e80.cpp). Moved it: AddOcclusionTest/AllocateOccluderHandle force-inlined
  (FableWin names), vtable slot fix (RESETOCCLUSIONINFO), occlusion info pointer held in a local, nested block for preloading/texture
  (preloading lands in the dead param slot +0xF), tinted pixel shader as if/else (two calls tail-merged), graphic_bank read BEFORE material
  (frame 0x48 and size equal), render_flags + light_count locals with per-arm base/index for the vertex shader.
- Did not move: scale*radius v radius*scale (c2 canonicalises), SETRENDERSTATE_DESIRED (worse), occlusion info as reference v pointer.
- grades ~50: best DIFFER(4540v4540) 37 hunks. Moved it: FableWin ?ApplyTriVertexShader@CEngineClothRenderer@@AAEXK_NK@Z (68 debug
  instructions, no retail copy) as a FABLE_FORCEINLINE private member (light_count, force, render_flags) with per-arm ternary clamp,
  together with the retail statement order lights / material / bank / composite (41 hunks); GetRGBColour named-result opt-in
  (CRGBFLOATCOLOUR_GETRGBCOLOUR_NAMED_RESULT + FABLE_RETAIL_CRGBCOLOUR: R,G,B,A converted into named bytes, built, returned) fixes the
  delayed R/G/B stores; SETRENDERSTATE_DESIRED (agent2048) now helps (37).
- Did not move: `?: true : false` / bool select / enable-first body for SetEnableFadedShadowFactor; std::min in ApplyTriVertexShader
  (address-taken temps: wrong); scale*radius.
## Hand-back (bwf1 author, ~58 grades) -- row NOT matched
- Best: differ/00bb3e80.cpp = c/v1.cpp, DIFFER(4540v4540), 37 hunks. Overlay: hdr/ (README.md lists every opt-in). No Flags, no pragma.
- Residue (exact): (1) CRenderStateManager::SetRenderState `test esi,edx ; or edx,esi` (retail) v `test edx,esi ; or esi,edx` at the
  specular site 0xBB4000 and alpha-state sites 0xBB49F2 / 0xBB4A69 / 0xBB4AD9 (fog, texture-factor and the other sites match);
  (2) TransformPoint rows 2/3 and GetMagnitude add-chain term order 0xBB42F7..0xBB435A (retail rows: X*E12 first / Y*E23 first,
  magnitude X,Z,Y; ours Z..X): c2 linearizer key = record-id hash => the function still has the wrong AMOUNT of IL somewhere;
  (3) SetEnableFadedShadowFactor's bool argument: retail `cmp dword ptr [eax+4694h],1 ; mov cl,1 ; je ; xor cl,cl ; cmp cl,[eax+0CEh]`,
  ours `mov esi,[..] ; mov dl,[eax+0CEh] ; cmp esi,1 ; sete cl ; cmp dl,cl` (both sites); (4) loop pad 0xBB4F3E (follows from 3);
  (5) one `mov eax,[eax+2814h]` / `mov esi,[edx+8]` scheduled one slot off in the alpha states.
- What moved it (in order): force-inline AllocateOccluderHandle + AddOcclusionTest/ClearHandles (FableWin names); vtable slots via
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO (PeekRenderInfoMeshBase +0x74, GetMorphInfo +0x80); the outline
  effect read inside each pass arm; occlusion info pointer in a local; tinted shadow pixel shader as if/else (two calls, tail-merged);
  texture in a block after the mipmap and preloading in an inner block ending before the dispatch (frame map == retail);
  FableWin ?ApplyTriVertexShader@CEngineClothRenderer@@AAEXK_NK@Z (light_count, force, render_flags) FABLE_FORCEINLINE with a per-arm
  `n < 5 ? n : 5` clamp + the retail statement order lights / material / bank / composite (71 -> 41); GetRGBColour named-result text
  (delayed R/G/B stores); SETRENDERSTATE_DESIRED (41 -> 37).
- Tried, no gain: scale*radius order (c2 canonicalises); occlusion info as a reference; `?: true : false`, `bool f=false; if(..) f=true`,
  `bool f=true; if(!=) f=false`, if/else two calls (worse, 4592) and an enable-first body for the faded-shadow argument; std::min in
  ApplyTriVertexShader (address-taken temps); named intermediates in GetAppropriateMipmap (39); CRGBFloatColour(tint) + GetAlphaAsFloat
  (120: retail reads the tint bytes, so the CRGBColour(R,G,B,alpha) temp is right); CRGBColour ctor through Initialise (no change).
- Best next ideas: (a) fit the remaining DIFF helpers to FableWin sizes (fwsize: GetAppropriateMipmap 75 v 81, C3DVector operator- 32 v
  34, CRGBFloatColour(const CRGBColour&) 36 v 46, CEnginePrimitiveRenderUtil::SetTextureFactor 39 v 35, SetEnableFadedShadowFactor 20 v
  24, GetRGBColour v 54, GFFloatToLongFloor 19 v 21) -- the x87 / test-or orders are record-id ties that move with the IL amount
  (agent2050 brief r3010; agent3010 tools/c2h7/model.py predicts them); fwsize probes hung >5 min here (shared compile lock?), so run
  them when the lane is idle; (b) the faded-shadow argument: look for the real call-site text (FableWin PeekGlobalShadowMode 9i exists;
  maybe a by-value enum compare inside another inline); (c) then the residual SetRenderState orders.
- Names: 0x00BF0B20 is SetupOutlineFillStates and 0x00BF0740 SetupOutlineStencilStates (NAME_FIXES.tsv); the landed occlusion stubs
  0x00B8D090 / 0x00B8D0A0 are StartOcclusionTest / EndOcclusionTest (Ego_r / FableWin). Made-up names: none (model members only:
  RenderPolygons' anonymous bool / fvf parameter names are descriptive).
- Tools here: tools/g.sh (grade + dump ours), tools/sbs.py (side by side), tools/hl.py (hunk list), tools/calls.py (call targets),
  tools/pk.sh (frame-packer trace, copy of agent3016/4007).
- Last probes (after hand-back block): dead outer locals (+1 / +3, diagnostic only) do NOT move the x87 add-chain orders (39 hunks, same
  orders); C3DVector operator- as `C3DVector result; result.Initialise(...); return result;` (34i/92b v FableWin 34i/90b; opt-in
  FABLE_RETAIL_C3DVECTOR_SUB_INITIALISE in hdr/C3DVector.h) leaves the row at 37 with the same orders. fwsize probes on a TINY file
  run in ~1 s (probe/q*.cpp pattern); only the full-TU probe hung. TransformPoint is fwsize SAME, so its row orders (retail row 2
  X,Z,Y / row 3 Y,X,Z; ours Z,Y,X) are pure c2 record-id ties: next step is agent3010's tools/c2h7/model.py on this row.
