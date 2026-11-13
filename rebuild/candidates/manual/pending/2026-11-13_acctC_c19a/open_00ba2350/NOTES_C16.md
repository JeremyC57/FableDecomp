# agentC16 NOTES (w22 acctC lane; continues agentC13 / C8 / C6 / C3 / C2)
Tools: tools/ (C13's, repointed) + tools/sib.sh (regrade the two matched cloth rows), tools/g1.sh (grade + lcs similarity, tsim.py),
tools/frc.sh (c2prio run, optional C2_FORCEID forcing, obj similarity via osim.py), tools/c2pf/ (hand883 c2prio.py with forcing).
Work sources in w/. Sibling matched rows (C8) copied to sib/src, sib/test.

## 0x00BA2350 SetupRenderModeShadersAndConstants -- IN PROGRESS
- Extent verified: code 0x00BA2350..0x00BA48A5 (`ret 1Ch` at 0x00BA48A3), 2-byte `mov edi,edi` pad 0x00BA48A6, 18-entry jump table
  0x00BA48A8..0x00BA48EF, next function 0x00BA48F0 (`sub esp,0Ch`). 9632 bytes = rows.tsv: oracle unchanged.
- Port of acctB's draft onto today's overlay: w/r1_2.cpp DIFFER(9500v9632) (= acctB). Opt-in mapping (stale -> today):
  _REFLECTIONMASK -> new _REFLECTIONTEXTURE + existing _2DALPHAMASK; RENDERUTIL_SETUPRENDERMODE -> existing _CENGINEPRIMITIVERENDERUTIL +
  _CLOTHRENDER + _CLOTH2D + new _SETUPBLANKTEXTURES; LIGHTING _SETUPSHADERSPOTLIGHT/_GETCOUNT -> existing _SPOTLIGHT; SHAREDDATA_MIPMAP ->
  existing _CLOTH (+_CLOTH2D) + new nested _SETUPRENDERMODE (Ego_r names: stale 2D names were wrong, 0x19C = PixelShader2DDiffuseOnly,
  0x1AC = PixelShader2DDiffuse); SHADOWRENDERER_SETUPSHADOWFUNC -> existing _SHADOWFUNC (generated branch, row no longer defines
  FABLE_RETAIL_CENGINESHADOWRENDERER) + new nested _SHADOWFUNC_CTOR (FableWin ??0CColorBufferDepthPackParameters@..@@QAE@JJJ@Z);
  RESOURCEMANAGER_GETZERORGBATEXTURE -> repo _ZERORGBATEXTURE; OUTLINEGLOW needs _MEMBERS for SetupDepthBufferTransform.
  Re-applied stale opt-ins (new to the overlay): C3DPRIMITIVE2_ISDP3BUMP(_MASK), CENGINECAMERA_PEEKWINDOW,
  CENGINEINTERNALPRIMITIVESTATICMESH_PEEKRENDERINFO / _PEEKLIGHTS, CENGINEPRIMITIVE2DVIEWPORTMANAGER_ALPHAMASK,
  CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_SETUPRENDERMODE, CENGINESUBPRIMITIVESTATICMESH_PARENTBASE.
- C13's flag on CEnginePrimitiveManagerStaticMeshesBase.h: checked -- the re-applied _RENDERMESHPRIMITIVE blocks declare members no repo
  opt-in declares (_SILHOUETTE: DrawImmediately/DrawSilhouette, _BLANKTEXTURES: SetupStaticBlocksBlankTextures); only a duplicate
  forward declaration overlaps (harmless). Nothing to reconcile.
- Best natural source: differ/00ba2350.cpp (= w/b2.cpp) DIFFER(9516v9632), lcs 1726/2419 (masked regs+frame 2219; acctB draft 1697/2042).
  Changes over the port (all code-neutral or improving, evidence in brackets): FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
  with `SetEnableFadedShadowFactor(true)` at the six sites (the guard `if (PeekGlobalShadowMode() != 1) enable = false;` is in the
  setter; retail `mov al,1; je; xor al,al` = a constant-true argument folded; replaces acctB's hand-inlined bool); FW-SAME opt-ins
  CRGBCOLOUR_CTOR4_INITIALISE, CRGBFLOATCOLOUR_INITIALISE_CTOR, CSHADERRENDERMANAGER_PSCONST_INDEX, C3DMESHINFO_PEEKTEXTUREBANK_MEMBER;
  SetTextureFactor pixel path `C4DVector(colour)` (cloth rows' text); tint as `tint_colour.A = alpha; SetTextureFactor(CRGBFloatColour
  (tint_colour))` (code-identical to acctB's named alpha_factor: the Initialise ctor evaluates A,B,G,R = retail's order naturally);
  tinted PS selects as if/else (retail pushes per arm, tail-merged call: +26 lcs); C4DVector(reflection); Set2DAlphaMaskTextureMode
  with CLAMP (cloth text, FW 24i SAME); GetCurrent2DAlphaMaskInfo via GFDebugPDynamicCast (cloth text, FW 16i SAME).
- THE residue (measured, c2prio --trace): sub_primitive prio 59 is coloured before mesh_primitive 49; sub takes edi because ebx costs
  it 2300 (the tint byte temps B/G #148/#149, allowed .cd..b./..d..b., would be left with one register). Forcing mesh first
  (C2_FORCEID=19=60, tools/frc.sh) reproduces retail's block 1 instruction for instruction (frame offsets aside): lcs 2103/2419.
  Forcing the tint bytes above sub instead does NOT help (sub then picks edi by table order). So retail had prio(mesh) > prio(sub).
- Priority sensitivity (c2prio): code-neutral texts of SetTextureFactor / GetRGBColour use / C4DVector temps, tint spellings,
  named locals, declaration order, TU flags (/Ob1 /G6 /Gr /Oi-; /Os breaks): sub 58..60, mesh 49 always. Diagnostics: removing
  the stipple block drops sub only to 55; any reference to sub in the else path / cases (GetScale, PeekObjectTransform) drops sub to
  28 / 9 (live through the switch) and gives mesh edi -- but those spellings change code (retail reads primitive in esi there).
- Plain `inline` instead of FABLE_FORCEINLINE per helper (13 variants): ApplyPixelShader plain inline gives 9632 bytes but 3
  out-of-line wrapper calls (retail has none) -- rejected; the others worse.
- Forced-flip residue (b2 + 19=60): frame-slot offsets (retail tint 0x18 / G-spill+tinted 0x10 / this 0x14 / bank 0x30 / flags 0x34 /
  block_on_load 0x38; ours 0x1c / 0x18,0x23 / ... ), case 15-18 tail merging (SetUserVertexShaderConstant + SetFogEnable copies),
  pass 8 and the outline-stencil/fill else tails (retail keeps both, hoists the 12/13 pushes; ours cross-jumps them).

## 0x00BA5260 RenderMeshPrimitive -- DIFFER(4056v4056), 2 residues (agentC16)
- differ/00ba5260.cpp = C13's port + (1) row 1's real body (b2) and helpers in the TU before this row (tools/comb.py builds it);
  (2) FlushRender `PD3DDevice->SetStreamSource(0, 0, 0, 0)` (the cloth rows' text; SelectVertexBuffer(0, 0) form = +records:
  4093v4056 with an extra uncross-jumped SetRenderState copy at 0x00BA5748); (3) declaration order `C3DTextureStages
  texture_stages; unsigned long shader_flags = 0; unsigned long render_flags = ...;` (fixes the [ebp-1Ch]/[ebp-20h] store pair).
- Residue A, 0x00BA558B `mov byte [ebp-38h],0` (UseBlankTextures=false of the inline C3DTextureStages ctor) scheduled early in ours:
  scheduler dump (tools/sched2 t11, region 18) shows the byte store carries a memory-dependence priority (10240) from the later
  AlphaRef byte load because texture_stages escapes into callees whose bodies the TU lacks. PROBE (probe/00ba5260_g1stubs_PROBE.cpp,
  never landable): non-capturing stand-in bodies of ALL FOUR callees SetupStaticBlocks 0x00BA50A0, SetupFirstTextures 0x00B9D670,
  RenderMeshPrimitiveBlocksIndexBuffer 0x00B9E5B0, RenderMeshPrimitiveBlocksDegenerateTriangles 0x00B9E750 (any 3 are not enough)
  put the store exactly where retail has it. All four precede this row in retail's TU. => row 2 needs their real bodies in its TU
  (0x00BA50A0 parked DIFFER 441v441 in pending/2026-10-27_linux_hand645; 0x00B9D670 hand429 draft; the two RenderMeshPrimitiveBlocks*
  unstarted). COORDINATOR DECISION: a row source that carries other rows' bodies only for IPA (as the cloth rows share one TU).
- Residue B, 0x00BA5EB3 cull SetRenderState(POLY_CULL_CCW) bookmark test/or registers (retail CBM->edx, ours esi): whole-function
  record count. Meter (tools/scan2.py, late rulers after EndOcclusionTest; probe g1 = stubs+fixes): window = count -1..-6 records
  (g1ng = no SHADOWGUARD = -9 records: k in [3,8] late rulers -> full RELOCATION_MATCH under the stubs, i.e. only A+B remain).
  Natural -1..-6 not found yet: tried GetParent/PeekParent, occlusion_handle position, render flags test forms, named mesh/material,
  composite_surfaces ternary, GetAppropriateMipmap qualification, decl orders (code-neutral, 0 units). Stand-in body of row 1 breaks
  much more (4045 bytes): row 2 depends on row 1's real body (IPA), so B may move once row 1's text is final.
- Row 1, more measurements (agentC16): else-path variants (w/x/): MeshEffect read per arm in the outline-stencil/fill else (else_effect_ref)
  9688 bytes, lcs 1772 (keeps its own tail, no cross-jump into pass 8; retail also keeps it but hoists the 12/13 pushes, which needs equal
  arm registers -> colouring first); named UV stage local, dp3 bool forms, degenerate-flag position: worse or neutral.
- Row 1 grades: 86 full grades + ~25 c2prio / forced builds (each 60-70 s). Levels: helper texts (fwsize), TU flags, statement structure
  (tint/alpha, PS selects, render_mode placement, else path), c2 allocator measurement (priority / cost trace, forced flips), inline
  decisions (plain inline per helper). Not yet: a systematic search of the else-path block structure for mesh_primitive's priority.

## 0x00B783F0 CEngineWaterRenderer::Render -- DIFFER(9343v9343), unchanged residue (agentC16, few grades: rows 1/2 first)
- Positions (tools/lo.py, 65 SetRenderState expansions): quartet L20-23 = exp2 (2nd SetCommonRenderStates) bump-enabled stage-1 block
  (0x7945D..0x79589), L60 = exp3 SrcBlend (0x7A565), pair L61/L62 = exp3 DestBlend / AlphaBlendEnable (0x7A5D3/0x7A638), O31 = exp2
  bump-else stage-2 (0x79938).
- A dead setter inside SetCommonRenderStates (probe w3/p/P1-P3): at its start or before the bump block fixes quartet + pair but breaks
  L60 (3 dead units before L60); at its end leaves L20-22 (1 unit before the quartet). So the wanted shape is exactly 2 units before the
  exp2 quartet and none between it and L60 -- a SetCommonRenderStates(bool) helper cannot give that (any per-call dead arm repeats in
  exp3, any arm live in exp3 adds code retail lacks): the coordinator's bool-parameter idea is ruled out by position, the 2 units must
  come from code between Capture and exp2's bump block that is not repeated after it.
- FableWin SetCommonRenderStates is 106i/333b v ours 75i/260b (+31i/+73b, consistent with one ASSERT + one 13-byte imm8 setter call,
  e.g. an empty PC stub; C13 measured both weigh 0 here).
- Natural region rewrites tried (w3/n): attaches as a loop (8690 b), attaches after the env-map block (8894 b), SetFogEnable(false) before
  the env-map block (same residue), SetCommonRenderStates through a local render-manager pointer (8654 b).
- KEY RESULT (agentC16, late): the stub-body PROBE + the DEFAULT (unnamed) C3DVector::operator- (i.e. without the overlay opt-in
  FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB, which C13 carried over from chunk 2060) grades RELOCATION_MATCH (4056v4056):
  probe/00ba5260_RELOCMATCH_stubs_unnamedsub_PROBE.cpp. So residue B is exactly the named-result operator- records.
  BUT the matched cloth row 0x00BB3E80 (same GetAppropriateMipmap text) goes DIFFER(4540v4540) without NAMED_RESULT_SUB.
  FableWin ??GC3DVector@@QBE?BV0@ABV0@@Z = 34i: the text `C3DVector result; result.Initialise(X - other.X, Y - other.Y, Z - other.Z);
  return result;` gives 34i/92b at /Od (named 32i, unnamed 28i) and keeps BOTH cloth rows RELOCATION_MATCH (probe/hv2 overlay), but row 2
  then still shows B. => one operator- text cannot serve 0x00BB3E80 and 0x00BA5260 with the other texts as they are; the coordinator
  must pick: (a) FW-SAME Initialise form (new opt-in, cloth rows OK) + find a natural -k record lever in row 2, or (b) unnamed default in
  row 2's TU (different text per TU -- not acceptable for one inline function unless the cloth row gets its +k elsewhere).
- Row 2 is therefore: [callee bodies in TU] + [operator- text decision] away from RELOCATION_MATCH. Behaviour test not written yet.
