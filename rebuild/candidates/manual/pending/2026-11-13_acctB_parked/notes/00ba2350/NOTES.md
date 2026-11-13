# 0x00BA2350 SetupRenderModeShadersAndConstants (bwf2 author, 2026-11-12)
- grades 1-12: full draft written (v/head.inc + v/body*.inc, overlay opt-ins listed in hdr/README.md), DIFFER(9316->9336 v 9632). Moved it: case layout order 2..13,1,16,17,18,15 (retail block order), named `alpha_factor` float (A evaluated first), camera window as named floats left/top/right/bottom (right spills as in retail). No effect: `?true:false` / PeekGlobalShadowMode for the faded-shadow bool (retail `mov al,1; je; xor al,al` still sete).
- grades 13-30 (best: differ/00ba2350.cpp = v/a9.cpp, DIFFER(9340v9632)). Moved it / kept: effect local declared first in pass 9 and in the
  outline-stencil/fill else path (retail loads MeshEffect into esi / pushes it before the pass-12 test), case 17 reuses ONE bool for both
  Get*Texture out params (retail: same slot). No effect / worse: `?true:false` and PeekGlobalShadowMode() for the faded-shadow bool,
  direct GPShaderRenderManager->ApplyPixelShader instead of the static wrapper, removing the mesh_primitive local (this->ebp but +124 bytes),
  pass-10 transparent arm as if/else with the PS inside (worse with the current colouring; retail reads IsDP3Bump once there -- retry AFTER
  the colouring is fixed), camera window by value (copies through memory; named floats left/top/right/bottom is right).
- KEY FINDING (c2prio, v/c2pf + v/a9.prio, a9b.prio = --blocks): c2 colours sub_primitive (prio 59) before mesh_primitive (51), so ours gets
  sub=edi, mesh=ebp, this=memory, stages=edi. Forcing mesh_primitive above sub (`C2_FORCEUID=28=60`, uid = creation ordinal from --json)
  gives retail's prologue exactly (ebp=this + [esp+14h], ebx=sub, esi=primitive, edi=mesh loaded after the setne) AND retail's 0xB0 frame,
  and pass 10 then matches except one IsDP3Bump re-read. So the residue is ONE allocator decision. Arithmetic (a9b.prio): sub = 4*K1 - 5,
  mesh = 2*K1 + 19, K1 = candidates in prologue block 1 (16 now: 4 CRGBColour::Initialise byte temps, consts 0xffffff/2/1, pass, flags,
  tinted, bank, primitive, sub, mesh, this, #25) -> mesh wins only if K1 <= 11, or sub loses >=9 elsewhere (its live-through penalty over the
  SetTextureFactor blocks is -57; the lights GetParent ref +10), or mesh gains (its biggest penalty: -30 live through pass 1's
  SetTextureFactor(1,1,1,1) blocks, -13 at the pass-10 transparent PS select, -10 at ApplyStaticVertexShader(0)).
- Remaining structure after the colouring fix (from v/cpos.sh call map): case 4 -> case 7's PS call and case 6 -> case 7's PS (retail
  cross-jumps, ours does not), case 16 keeps its own SetUserVertexShaderConstant call and case 18 jumps into case 15's (ours the other way),
  pass 9 arms and the stencil/fill else path keep their own PS / OSASM / blank-texture copies in retail (ours cross-jumps them) -- these
  are register-dependent, so re-check them only after the colouring matches.
- Best next idea: find the source fact that lowers sub_primitive's c2 priority below mesh_primitive's (or raises mesh): run
  `FABLE_HDR_OVERLAY=$W/hdr C2PRIO_RUNS=$W/v/runs python3 $W/v/c2pf/c2prio.py <src> --func SetupRenderMode --blocks` (55 s) per spelling;
  candidates: how the tint copy is spelled (fewer block-1 temps), where `lights` is read, whether pass 1's SetTextureFactor(1,1,1,1) /
  ApplyStaticVertexShader(0) sit where mesh is live, a `dp3_bump` local in pass 1. Tools: v/cpos.sh (call map with offsets, OBJ=...),
  v/rv.sh <ours_from> <ours_to> <retail_from> <retail_to> (side by side), v/cseq.sh.
- Names: 0xBF0B20 is SetupOutlineFillStates (pass 13), 0xBF0740 SetupOutlineStencilStates (pass 12) -- NAME_FIXES.tsv; 0xB89070 =
  CEngineScreenEffectOutlineGlow::SetupDepthBufferTransform (Ego_r, one source + call shape). 2D pixel-shader member names in the
  SharedData opt-in are descriptive (FableWin getters take a blend mode). Headers: hdr/README.md. No Flags needed so far.
- c2prio probes with no effect on the sub(59) v mesh(51) priorities: ternary alpha clamp (v/q1), a named render_flags local (v/q2), mesh_primitive declared before primitive (v/q3; this changes only the tie-break key).

## attempt 2 (bwf2 author 2, 2026-11-12) -- work dir b/ (b0 = attempt-1 best; c*/d*/g*/h*/m* = this attempt)
- grades: b0 9340 h322 -> c1 9284 h312 (VS index helper `unsigned long index = light_count; if (index > 5) index = 5;` = FableWin 20i/60b SAME)
  -> d2 9492 h308 (pass-10 alpha arm: one IsDP3Bump read) -> g1 9488 h307 (faded-shadow bool as `bool f = true; if (ShadowMode != 1) f = false;`:
  retail's `mov al,1; je; xor al,al` shape). differ/ = g1. m2 = g1 + graphic_bank declared BEFORE block_on_load (fixes retail's block-1 temp
  registers under forcing: equal-priority tie-break by definition order) + `long alpha_value = alpha; long solid_alpha = StippleAlphaSolidFactor * alpha_value;`
  (retail's `movzx X,al; mov eax,[ebp+314h]; imul eax,X`).
- New tools: b/frc.sh <src> [id=prio] (forced c2 build via C2_FORCEID -- NOTE the candidate id of mesh_primitive changes with the source, read it
  from the first line of output; 19 for g1/m2), b/om.py <obj> [n] (instruction similarity v retail, relocs masked). Forced m2: 1877/2419 match.
- FableWin helper texts fixed to SAME size (no release/priority effect): IsDP3Bump `(InitFlags&2)!=0`, PeekLights via PeekRenderInfo,
  GetParent via GetParentBase (overlay opt-ins _ISDP3BUMP_MASK, _PEEKRENDERINFO, CENGINESUBPRIMITIVESTATICMESH_PARENTBASE).
- Priority probes (sub v mesh, c2prio): t2 `bool tinted=false; if(...) tinted=true;` FLIPS it (mesh 27 > sub 23: a block split in block 1
  cuts sub's 4*K1 term) but leaves a branch retail lacks; lights read before ThemeChannel -> sub 79 (so sub's ThemeChannel live-through costs ~21);
  DWORD tint copy K1 13 -> 48 v 43; __assume / empty do-while / ERenderMode / named CRGBFloatColour / `!= 0` flag test: no effect.
  FableWin static PS getters GetPixelShader*() at every site: gap 46 v 43 but frame shrinks to ~0x5C and forced similarity drops (1666): wrong globally.
- (attempt 2, cont.) best draft now differ/ = b/m5.cpp (real 9500v9632 h347; forced flip 1904/2419, best of all forced builds): m2 + BOTH pass-10
  PS selections as `if (IsDP3Bump()) [Bump=1;] ApplyPixelShader(A); else ApplyPixelShader(B);` (retail pushes the shader in each arm with its own
  register and tail-merges the call; the VS selections stay ternaries -- retail computes them into one register). The `bool alpha_dp3_bump`
  form (d2) spills the bool under forcing: wrong.
- Under the forced flip block 1 now matches retail instruction for instruction except frame slots (retail block_on_load 38h, G-byte spill and
  `tinted` share 10h, tint 18h, graphic_bank 30h, flags 34h; ours 40h/18h/23h/1Ch/28h/3Ch). The block-2 differences all follow from ONE
  colouring fact: retail keeps the tint G byte (Initialise param) in dl from the end of block 1 to the float conversion, so render_mode /
  flags temps take eax, material + the zero-extended alpha take edi and the alpha load is scheduled after `test al,8`; ours re-reads G from
  its spill slot. Probably the same root as the sub/mesh order (both are block-1/2 K counts).
- FableWin size checks that stay DIFF (FW is a later revision -- treat as hints): SetTextureFactor 39i v 35i, GetRGBColour 30i v 54i
  (with a GFFloatToLongTrunc per channel 38i; FW likely clamps: retail does not), CRGBFloatColour(MMMM) 20 v 22, SetPixelShaderConstant 29 v 28,
  SetEnableFadedShadowFactor 20 v 24, Set2DAlphaMaskTextureMode 32 v 24, GetCurrent2DAlphaMaskInfo 12 v 16. Inline-text changes so far never
  moved c2 priorities (GetParentBase, PeekRenderInfo, IsDP3Bump mask, GFFloatToLongTrunc: 58 v 49 unchanged), EXCEPT the 14 PS getters (G1),
  which also changed the inliner's treatment of the tint copy (copy scalar-replaced, frame 0x5C) -- an inline-budget effect worth knowing.
- Best next ideas: (1) find what keeps K2 / the block-2 competition for edx lower in retail (render_mode or the flags temp not being a
  candidate; or the stipple constants 0/5/0xFF not being referenced in block 2) -- c2prio --blocks shows block 2 = {flags temp, material,
  const 0, const 5, const 0xff, alpha, render_mode, sub}; (2) the inline budget: which helpers are plain `inline` v FABLE_FORCEINLINE changes
  the tint copy / block 1 (see G1); try demoting single helpers (GetCurrent2DAlphaMaskInfo, Set2DAlphaMaskTextureMode, SetupBlankTextures,
  DisableTextureFiltering) to plain inline and re-run c2prio; (3) per-site PS spelling (direct GPShaderRenderManager->ApplyPixelShader v the
  static wrapper v a FableWin getter) only after the colouring flips: retail's ecx-pattern sites are pass 1, the else path and pass 10.
- (attempt 2) i1: DisableTextureFiltering / Set2DAlphaMaskTextureMode / GetCurrent2DAlphaMaskInfo as plain inline -> called out of line (7236v9632): they must stay FABLE_FORCEINLINE; priorities unchanged.

## attempt 3 (bwf2 author 3, 2026-11-13) -- work dir r3/ (a0 = differ/ = attempt-2 m5)
- grades: a0 9500 h347 (baseline); a1 (no `primitive` local, every use `sub_primitive.GetParent()->`) 9504 h242 but WRONG in principle
  (the CSE temp dies at the ThemeChannel stores; retail keeps primitive in esi through the cases: `push esi` / `add esi,70h`);
  SetTextureFactor text variants (named C4DVector / named CRGBColour / both; C4DVector(colour)) no gain; TU flags /G7 9520 h339,
  /G5 /GB /Ox /Ob2 unchanged, /Op 9252; QFE-4035 compiler (FABLE_VC71=vc71) unchanged; tinted as if/else-both-arms / !(==) / ?1:0:
  c2 turns them all into setne in IL (no block split, priorities 58 v 49 unchanged); `const ... & sub = sub_primitive` alias for the
  block-2 reads: copy-propagated, no change.
- FableWin-size header texts added (overlay opt-ins, no priority change): CRGBColour(EEEE) via Initialise (FW 18i = SAME),
  C3DMeshInfo::PeekTextureBank = PeekBank()->TextureBank (FW 10i/25b SAME; f1 = a0 + this, 9500 h349),
  CRGBFloatColour(MMMM) via Initialise (18i v FW 22i: still DIFF -- not it; opt-in left unused).
- Analysis (c2prio a0.prio --blocks): sub_primitive is the only thing that decides edi; it conflicts with every combined-path temp
  up to the lights read (mesh does not: mesh is dead in the combined path). Retail's SetRenderState temps and the ThemeChannel
  `lea edi,[ebp+0E0h]` take edi because sub is ebx -- consistent with "mesh coloured before sub" OR "a combined-path candidate with
  priority > sub took edi first"; in ours every such temp has priority 24-28 (< sub 58). om.py's `mov esi,G` v `mov esi,20` rows
  are reloc-addend artefacts (D3DTextureTransformModeTable+14h etc.), not differences.
- (attempt 3, cont.) h1 = f1 + SetPixelShaderConstant body `PSConstTable[index] = value;` (FableWin 28i/79b SAME; begin()+index is
  29i/83b): real 9548v9632 h353 (size closer, hunks +6), priorities 59 v 49 unchanged, forced 1884 (f1 1903) -- kept as r3/h1.cpp, NOT
  copied to differ/ (differ/ stays attempt-2 m5, 9500 h347). Stencil/fill else path as ternary call (k1 = h1), per-arm effect (k2 9720),
  named effect ref + scale (k3 9556): retail's hoisted OS/OF pushes not reproduced.
- c2prio facts (r3/*.prio): sub = +64 (block 1, K16) +48 (block 2, K8) -57 (live through SetTextureFactor + ThemeChannel blocks 3..20)
  +10 (lights GetParent); mesh = +32 (block 1) + else-path refs (IsDP3Bump, +4..+16 each) - live-through (-30 in Z_WRITE's
  SetTextureFactor(1,1,1,1)). None of: FW-size helper texts (CRGBColour/CRGBFloatColour ctors via Initialise, PeekTextureBank,
  PSConst index, SetTextureFactor text), TU flags (/G7 /G5 /GB /Ox /Ob2 /Op), the QFE compiler, bool spellings, aliases moved it.
- Best next idea (attempt 4): stop K-guessing; get the pass-12/13 push hoist and the case 4/6 -> 7 PS cross-jumps first under forcing
  (b/frc.sh r3/h1.cpp 19=60, om.py <obj> 200 lists them), because a source shape that makes VC hoist identical pushes out of an
  if/else changes the else-path block structure that mesh's priority is summed over; then re-measure mesh v sub. Also untested: the
  FW CRGBColour copy-ctor byte gap (22i both, 51b v 55b: FW pushes the four bytes with movzx -- a different arg/member type), and
  GetRGBColour's FW 54i text (named-locals form changes the combined-path block K's that sub is penalised over).
