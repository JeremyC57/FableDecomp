# 0x00BD3070 RenderMeshPrimitive -- workflow bwf1 author notes (2026-11-12)

## Progress (grades 1..~45 of this session; ATTEMPTS.log has one line each)
- Best: v/k01.cpp = differ/00bd3070.cpp: DIFFER 11288v11288 (SAME SIZE), 2138 of 3160 instructions equal (prior best s03: 2071, 11336).
- What moved it:
  1. `abort_render` flag KEPT (dossier point 1 is wrong: removing it un-threads arms 8/12/13, +48 bytes).
  2. The alpha / blend block is the FableWin inline `CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromRenderFlags(render_flags, pass,
     sub_primitive.PeekAlpha(), sub_primitive.GetParent()->AlphaRef)` (the static twin 0x00BA5260 uses it; fwsize SAME 71i/268b): gives retail's
     4-aligned alpha-ref byte, `mov eax,[pass] ; cmp eax,1`, load order. New hdr opt-in FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ALPHABLENDSTATES.
  3. `unsigned long mipmap = 0;` in GetAppropriateMipmap AFTER the sphere copy / TransformPoint (two immediate zero stores at 0xBD3250 / 0xBD325D).
  4. Default SetRenderState text (NOT ..._SETRENDERSTATE_DESIRED): size 11352 -> 11288.
- What did not: StartCaptureBlock statement spellings (6 variants, hdr opt-in FABLE_RETAIL_CRENDERSTATEMANAGER_STARTCAPTUREBLOCK_STATEMENTS kept but
  unused), SetTextureFactor early-return forms, `scale` local, num_textures declaration positions (after the mipmap call gives frame 0xE0 but
  fewer equal instructions; before composite_surfaces: ebx zero store, wrong).
- Tools left in v/: g1.sh (grade + structure tokens + eq + SRS pattern), mk.sh, sbs.sh, hk.py (hunks by address range), srs.py (per-SetRenderState
  load / test operand order), cd.py (call counts), od.py (disassemble a fwsize /Od function), ruler/ (dead-local sweep, diagnostic only).

## Workflow author 2 (2026-11-12, session B) -- running notes
- grades 1-20: H1 (per-arm `if (failed) { FlushRender; Restore; return; }`) REFUTED: 11808 bytes, the four return blocks never cross-jump
  (each copy starts with a different register holding GPRenderManager). Best: v/m03.cpp = differ (eq 2309/3160, 315 hunks, 11288 same size):
  `abort_render = failed;` written BEFORE SetTextureFactor in the combined and Z_WRITE arms (gives retail's block layout: TF-then falls into
  the abort test, TF-else after the return block, Z_WRITE's copy cross-jumps back) + `Distance * TextureOverlayFadeDistandeFactor` order.
- Did not help: abort flag inverse (render = !failed, eq 2196), flag=true + `if (flag && failed)` (= k01), only-combined / only-Z before TF,
  forceinline CEnginePrimitiveRenderUtil::ApplyPixelShader (m14: all 19 APS expanded like retail, but arms 8/12/13 then cross-jump, eq 2289),
  direct GPS->ApplyPixelShader in arms 8/12/13 (no change), reflection-colour named ref / named C4DVector (no change), member-init C4DVector ctor (worse).
- grades 21-37: decal loop named blend ref / `!= false` (no change), named CRGBColour (worse); combined TF colour with A from
  GetAlphaAsFloat (3 spellings, eq 2300-2301 < 2309); AlphaRef via GetParentBase (no change) / via primitive (worse); lights via
  primitive->PeekLights() (eq 2260); abort_render declaration position (no change); m06 (m03 without the flag, test `failed`): 11368, eq 1955.
- Header overlay: all 9 STALE headers RE-BASED onto the current repo text + additions only (hdr/README.md lines); the row now defines
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO instead of the old _VIRTUALS (GetAsOcclusionPrimitive is
  inherited now). Grade unchanged (315 hunks) -> v/m29.cpp = differ/00bd3070.cpp. hdr_bak/ = the coordinator's overlay before the re-base.

## Hand-back state (session B)
- Best: differ/00bd3070.cpp (= v/m29.cpp): DIFFER 11288v11288, eq 2309/3160 (k01 was 2138), 315 hunks.
- What moved it: `abort_render = failed;` BEFORE SetTextureFactor in the combined and Z_WRITE arms (+171 eq: retail's TF-then -> abort
  test -> return block -> TF-else layout, Z_WRITE cross-jumps back into the combined copy); `Distance * TextureOverlayFadeDistandeFactor`.
- Remaining residue (by size): frame 0xE4 v 0xE0 + slot renames (~140 hunks: this +54 v +58, primitive +30 v +0C, decal +04 v +18,
  textures +14 v +1C ...); abort_render is a MEMORY byte in m29 (arms 9/10-alpha store `mov [ebp+68h],al`, retail tests failed
  [ebp+6Fh] directly at 0xBD4E7B) -- retail = k01 semantics with m03 layout, no spelling found yet; arms 6/7 + 4/8 APS tail not
  cross-jumped (ours loads GPShaderRenderManager before the push, arm 7 uses `push 1` / `cmp ..,1` not esi=1); reflection-colour copy
  scheduling in arms 7/10/13 (8 v 11); decal loop SIB base/index order and decal kept in ebx (retail reloads [ebp+18h]).
- Packer facts (pk tool /opt/fable_lane/w22/agent2009/tmp/c2/pk.py with FABLE_HDR_OVERLAY=$W/hdr; pk/m03.txt, sim.py replays it at
  frame 224): retail slot order implies `this` heavier than graphic_bank and force_load lighter than bank; retail +0x30 = lights
  (ebx spill, `mov ebx,[ebp+30h]` at 0xBD4DA3) shared with scale and bone_transforms; GetFadeDistance return temp at [ebp+8] in retail
  (ours joins it to the lights slot, making it an 8-byte slot).
- FableWin COMDATs first referenced by this function (signatures.tsv after 0x02E7E830): GetAlphaIncludingDistanceFade, GetAlphaAsFloat,
  GetShowPreloadColours (!), GetLOD (!), GetParentBase, RenderPrimitiveBlocks, FailIfXbox ... -- GetShowPreloadColours / GetLOD are not in
  the draft (debug-only paths or helper bodies; unchecked).
- Best next idea: get k01's register-flag semantics (test reads failed, arms 8/12/13 threaded past) WITH m03's layout -- e.g. a
  textual `if (failed)` check only in the combined arm right after SetTextureFactor plus the flag for the other arms, or a tail helper;
  then attack the frame with the packer trace (sim.py --w / --merge to find which weights give retail's slot order: this > bank >
  force_load > num_textures > mipmap > composite).
- grades 38-39: combined arm textual `if (failed) { FlushRender; Restore; return; }` after SetTextureFactor + flag for the other arms:
  11416 / 11392 (two return blocks never merge). Refuted.
