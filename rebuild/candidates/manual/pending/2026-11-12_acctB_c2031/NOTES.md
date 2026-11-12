# Hand chunk 2031 (CEngineSkyRenderer .. CEngineWeatherRenderer): 0 of 2 byte-exact

All grades were taken with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2031/hdr` (hdr/README.md lists every header change; all are opt-ins or
new headers). Both rows are parked in differ/ with passing behaviour tests (g++ and VC7.1). Row extents were checked from Fable.exe: both
catalog lengths are right (no SPLITS.tsv).

## What matched (0)
None.

## Parked for Fable 5.1 (2)
- 00b54e30 CEngineWeatherRenderer::RenderSnow DIFFER(1901v1901): 569/573 instructions; only the two inline STLport push_back C3DVector
  copies differ -- retail 0x00B553BD..0x00B553CF and 0x00B55403..0x00B55414 load X, Y, Z then store all three, ours interleaves
  load/store (`mov eax,[esp+30h] ; mov [ebp],eax ; mov ecx,[esp+34h] ; mov [ebp+4],ecx ; ...`) | tried: 5 source grades to reach the
  residue (frame fetch through the shared GetFrame, forced helpers, SetCommonParticleShaderConstants' real body noinline, a named
  texture-index local for the head registers, the GetFrame touch block without a named bank-frame local) and 4 diagnostic grades: with
  non-capturing, side-effecting bodies (each calls an unknown function with the coordinates by value) of ClipBoxToCamera 0x00B51F60,
  ClipBoxToWeatherMaskSphere 0x00B52090 AND ClipBoxToLandscape 0x00B52DD0 visible the row grades RELOCATION_MATCH (probe/diag2_clm.cpp);
  camera + mask bodies alone or the landscape body alone stay at 569/573 (probe/diag2_cm.cpp, probe/diag2_l.cpp); pure stub bodies (no
  call) change far more (frame 0x40, reloads of the saved visibility dropped: probe/diag_clm.cpp) | next: lane gap: pair-landing --
  ClipBoxToLandscape 0x00B52DD0 is unlanded (4489 bytes of x87 around CEngineMap::PeekLandscapeHeight 0x00B439C0, never attempted; hand727
  NOT_ATTEMPTED). Author it, respell the landed 0x00B51F60 / 0x00B52090 under their PDB names (c2008 NAME_FIXES.tsv), and put the three
  real bodies (noinline) before RenderRain / RenderSnow in one TU: both 0x00B54890 (c2008) and 0x00B54E30 should then match.
- 00b65a20 CEngineSkyRenderer::RenderOuterSky DIFFER(1897v1893): 597/602 instructions; everything before the shader tail matches (both
  frame fetches, the fallback texture, the destructor); the residue is in the last block: (1) the blend vector's dead Y store
  0x00B65E42 `mov [esp+28h],ecx` (retail after `mov eax,[GPShaderRenderManager]`, ours right after the X store); (2) the
  `UpdateFlags |= 2` read-modify-write register 0x00B65E5A..0x00B65E67 (retail ebx, ours esi); (3) colour 13's first copy 0x00B65E97
  (retail `add eax,0D0h ; mov ecx,[eax]`, ours `mov ecx,[eax+0D0h]` before the add, 4 bytes longer in all) | tried (5 grades + 1
  re-verification compile): the colour copies straight from `params...Colours[12]` (params reload took eax, 567/602), a named blend
  vector (no change), copies through named const references (597/602, kept), a CRGBFloatColour(R, G, B, A) rebuild (x87 copies, 549/602);
  free probes: c2prio traces of six colour-copy spellings, scheduler DAG dumps (tools/sched.sh + tools/skyfit.py) of five variants and a
  SetPixelShaderConstant `PSConstTable[index]` form (no change). Measured: the Y / W dead stores tie at scheduler priority 80 (Y: height
  70, 10 successors incl. memory edges to the three table stores; W: height 72, 8 successors) and the tie goes to the lower tuple index (Y);
  the flags register is not an allocator candidate and not an 0x10775ACC expression temporary (no c2prio entry) | next: trace the c2 path
  that splits `or [mem],imm` into load/or/store and picks the free register (retail ebx, ours esi: probably the first dead register in some
  order when eax/ecx/edx are busy); then re-run tools/skyfit.py: the Y/W tie and the colour-13 fold may follow from the same DAG change.

## Skipped
None. Both starts were confirmed (below).

## Row extents
- 0x00B54E30: starts after RenderRain's int3 padding (RenderRain ends `ret 4` at 0x00B54E24); its only caller is CEngineWeatherRenderer::Render
  (0x00B55E4F); last instruction `ret 4` at 0x00B5559A, int3 padding after. 1901 bytes = the catalog length.
- 0x00B65A20 (the start flagged for checking): a real function start. RenderSky 0x00B66190 calls it directly (0x00B66284
  `call 0x00B65A20`, xref.py); the preceding function's out-of-line cold tail ends with the 2-byte `jmp 0x00B659C4` at 0x00B65A1E, so there
  is no int3 padding before 0x00B65A20 (it is 16-byte aligned and starts with `sub esp,44h`, the frame its epilogues undo). Its own cold
  tail (the second fetch's create-and-load path 0x00B66143..0x00B66184, `jmp 0x00B65DF6`) is inside the row; int3 padding starts at
  0x00B66185. 1893 bytes = the catalog length.

## Names (two sources each; LABELS.tsv)
- 0x00B54E30 `?RenderSnow@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z`: Ego_r.pdb 0x009FB0F0 + FableWin.pdb; Ego_r donor
  order RenderRain 0x009FAAD0 -> RenderSnow 0x009FB0F0 = retail 0x00B54890 -> 0x00B54E30; the body reads Settings.Snow* and draws through
  RenderSnowBox 0x00B54210 (Ego_r ?RenderSnowBox@CEngineWeatherRenderer@@AAEXABVC3DVector@@@Z, landed).
- 0x00B65A20 `?RenderOuterSky@CEngineSkyRenderer@@AAEXM@Z`: Ego_r.pdb 0x00A169E0 + FableWin.pdb; Ego_r donor order RenderClouds 0x00A15F60 ->
  RenderOuterSky 0x00A169E0 = retail 0x00B64FA0 -> 0x00B65A20; one float argument (`ret 4`), called by RenderSky (which passes its
  interpolation; the landed RenderSky 0x00B66190 comment names it the same way).
- Callees / helpers proven here: CResource::TouchResource (FableWin ?TouchResource@CResource@@QAEXXZ = retail uncalled copy 0x0041B9C0, this
  exact body), CManagedTexture::TouchTexture (?TouchTexture@CManagedTexture@@QAEXXZ = uncalled 0x0041BB70), CGraphicDataBank::
  PeekGraphicInfoFrame (?PeekGraphicInfoFrame@CGraphicDataBank@@QBEPBVCGraphicInfoFrame@@KK@Z = uncalled 0x0041C1F0, two PeekGraphic range
  tests, `ret 8`), CGraphicInfoFrame::GetFrame (uncalled 0x0041C260, landed), CRenderManagerCore::DrawCurrentBuffersIndexed(const CTexture**,
  ...) 0x00A0ACA0, CShaderRenderManager::SetUserVertexShaderConstant(unsigned long, const CRGBFloatColour&) 0x00989B50 (Ego_r order C3DVector
  0x00AE62B0 / C4DVector 0x00AE6300 / KMMMM 0x00AE6350 / CRGBFloatColour 0x00AE63A0 = retail 0x00989A60 / 0x00989AB0 / 0x00989B00 /
  0x00989B50; the two 16-byte overloads have identical bodies), CEnvironment::PeekInterpolatedParams 0x006B7350 (CEnvironment vtable
  0x0125D53C slot 1), CWorld::DrawGetEnvironment 0x0049E350 (CIDrawWorld slot 1), CEngineEnvironment::GetClampedScaledCurrentWindVector
  (FableWin, inline), CEngineWorldMap::PeekGameWorld (FableWin, inline), CRenderStateManager::SetPointSpriteEnable / SetPointScaleEnable
  (FableWin, inline), C2DBoxI::GetHeight (FableWin, inline).
- Label corrections: NAME_FIXES.tsv (0x0041B9C0, 0x009FBFB0, 0x009FBF90, 0x00B52160, 0x006B7350, 0x00989B50).
- Established spellings reused (no PDB data symbols): NEngineInternal::GPResourceManager / GPRenderManager / GPShaderRenderManager,
  g_pEngineEnvironmentDef 0x01436E24, g_pDisplayManager_01436e1c 0x01436E1C, g_pEngineEnvironment_01436e48 0x01436E48 (the CEngineEnvironment
  spelling of CEngine::Initialise 0x00B26360; the address also has g_pEngineSystem_01436e48 / g_pEngineGlobals_01436e48 / g_envHost_b23b90
  bindings -- review at landing), g_pEngineWorldMap 0x01436E8C (also bound as g_pEngineState_01436e8c / g_pEngineState -- review).
- Descriptive (searched Ego_r / FableWin / Ego_d name sets, strings, reloc_bindings; nothing names them): all local variable names
  (texture, wind, constant, boxes, clipped_boxes, level, params, texture0 / texture1, blend, bank, textures, sky_colour / horizon_colour,
  colour0 / colour1); the diagnostic-only WeatherDiagnosticExternalTest (probe files, never delivered). The colour indices 12 / 13 and the
  texture-set index 0 are plain numbers (no enumerator names in the PDB name sets).

## Header debt
- engine/CWorld.h: its CIDrawWorld declares slot 1 DrawGetWorldMap and slot 2 DrawGetEnvironment; retail's vtable 0x012390DC has
  DrawGetEnvironment (0x0049E350) at slot 1 and DrawGetWorldMap (0x0049E1A0) at slot 2 (new engine/CIDrawWorld.h, same guard macro).
- No repo header yet for CIDrawEnvironment / CIDrawEnvironmentParams (new overlay header); CEngineWorldMap's retail branch types only
  GameWorld (+0x198) besides MapList.
- engine/CGraphicInfoFrame.h has two models (STATICBLOCKS and c2018's SURFACECOMPOSITION); GetFrame now exists in both (this one in the
  STATICBLOCKS branch, matching the landed 0x0041C260 spelling).
- The landed CEngineWeatherRenderer rows around this TU (ClipBoxToCamera 0x00B51F60, IsPointInPrecipitation 0x00B52090,
  SetCommonParticleShaderConstants 0x00B52160, WorldFrameUpdate 0x00B52B30, RenderMist 0x00B54310) and the sky rows (RenderSky, Render,
  RenderBaseBand) still use local stand-in models.

## C++ attempts before an asm helper
Not applicable: neither row needs an asm helper.
