# Chunk 2031 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00B54E30) RenderSnow is RenderRain's twin down to the residue: the two inline push_back C3DVector copies are scheduled load-ahead only
  with the bodies of ClipBoxToCamera 0x00B51F60, ClipBoxToWeatherMaskSphere 0x00B52090 AND ClipBoxToLandscape 0x00B52DD0 visible. A
  diagnostic stub body must keep the callee's memory side effects (call an unknown function with the coordinates by value): a pure stub
  lets c2 drop the saved-visibility reloads and shrink the frame (0x44 -> 0x40), which hides the real lever.
- (0x00B54E30 / 0x00B65A20) VC7.1 drops a pointer's null test after a dereference only when the object holding it never escapes: the
  frame copied by the expanded GetFrame and then read by `frame.GetTexture()` loses ~CGraphicFrame's `if (FrameData)` (0x00B55542,
  0x00B660EB); the same code with the frame passed to an out-of-line function keeps it (probe/frag/nt1.cpp v nt2.cpp).
- (~CGraphicFrame) Two decisions for one cost in one function prove a forced inline: ~CFrameData 0x009FE6C0 calls
  CGraphicFrame::UnInitialise 0x00A00B90 but expands the member ~CGraphicFrame with the same body (c1 88 > 74): ~CGraphicFrame is
  FABLE_FORCEINLINE with the release in its own body, UnInitialise stays plain inline.
- Check an inline header body for free against an uncalled retail COMDAT: take the member's address at file scope (forcing the out-of-line
  copy) and grade the probe against the COMDAT's row -- CGraphicInfoFrame::GetFrame vs 0x0041C260 graded RELOCATION_MATCH (probe/frag/gf1.cpp).
  Uncalled retail COMDATs found this way: TouchResource 0x0041B9C0, TouchTexture 0x0041BB70, PeekGraphicInfoFrame 0x0041C1F0.
- (0x00B54E60) Register rotation follows generation order, not issue order: retail loads GPResourceManager into ecx and the def into eax
  although the bank load issues first; `long texture = def->SnowTexture;` before the bank expression generates the def load first and
  reproduces it (inline in the call the def is generated after the bank: eax / edx).
- (GetFrame touch block) A named `bank_frame` local becomes an allocator candidate (eax); the retail touch block re-evaluates
  `GraphicInfo->GetBank()->GetCurrentFrame()` in the compare and the store (CSE temp, edx): same out-of-line bytes, different registers once
  the compare folds away (mip level 0).
- (0x00B65E60) A spilled reference reloaded in a later block is a split piece chosen "all costs 0, first allowed register" (c2prio --trace);
  to move it off eax, something else must own eax there: copying CRGBFloatColour values through a named `const CRGBFloatColour&` makes the
  source address an allocator candidate (eax), which pushes the reload into ecx as retail. Copying straight from
  `params.ThemeChannels[0].Colours[12]` keeps the address an expression temporary (reload in eax).
- (0x00B65A20) engine/CWorld.h's CIDrawWorld has slots 1 and 2 swapped: retail vtable 0x012390DC is {DrawGetProceduralTexture,
  DrawGetEnvironment 0x0049E350, DrawGetWorldMap 0x0049E1A0, dtor}.
- (scheduler) tools/sched.sh (hand892 t13 hook) + tools/skyfit.py replay c2's list scheduler for one region: two dead stores with equal
  priority ((height + successors) << 10) issue in tuple-index order; a store that conflicts with later heap stores gets extra memory
  successors (here the C4DVector temporary's Y store v the pixel-constant table stores).

Review at landing:
- 0x00B54E30 RenderSnow and 0x00B54890 RenderRain (c2008) are one pair-landing case: author ClipBoxToLandscape 0x00B52DD0 (unlanded, 4489
  bytes, x87 around PeekLandscapeHeight 0x00B439C0) and put it with the PDB-named ClipBoxToCamera / ClipBoxToWeatherMaskSphere bodies
  (noinline) in the TU of both rows.
- 0x0041B9C0 is landed as CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera (with callees bound as FableCameraPointScriptedSpline_0041B9C0
  StepBackward / ApplyKey): it is CResource::TouchResource (NAME_FIXES.tsv).
- 0x006B7350 is landed as the placeholder `C_006b7350::M`; it is CEnvironment::PeekInterpolatedParams (NAME_FIXES.tsv).
- 0x00B52160 is landed as WeatherRendererMethods::SetCommonParticleShaderConstants(long*); the PDB form is private (long&) (NAME_FIXES.tsv).
- 0x01436E48 has four bound spellings (g_pEngineEnvironment_01436e48 / g_pEngineSystem_01436e48 / g_pEngineGlobals_01436e48 /
  g_envHost_b23b90) and 0x01436E8C three (g_pEngineWorldMap / g_pEngineState_01436e8c / g_pEngineState): pick one each.
- The overlay copies of c2005 / c2008 / c2018 headers carry their opt-ins; merge them with those hand-backs' landings (hdr/README.md).
