# Hand chunk 2008 (CEngineWeatherRenderer .. CThing): 2 of 4 byte-exact

All grades were taken with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2008/hdr` (gg.sh exports it); hdr/README.md lists every header change.
Read-only verifier: `vland.py payload.json oracle.tsv` (no --land) -> both src rows RELOCATION_MATCH + behaviour PASS, no RELOC_IDENTITY
(three RELOC_NAME warnings, all for the label fixes below); tests PASS under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (2)
- **CThing** 0x004CA390 `?ConstructFromBinarySave@CThing@@UAEXAAVCMemoryDataInputStream@@ABVCSerialiseInfo@1@@Z` (catalog `CThing::vfunc_16`),
  RELOCATION_MATCH on the first grade. The load-side mirror of the landed CThing::SaveBinary 0x004C84B0 (same TU): the same inline
  one-argument Transfer<short / bool> specialisations and visible noinline traits readers, the repo's inline CDataInputStream::ReadSLONG /
  CMemoryDataInputStream::ReadConstData / IsEOF, `PCurrentThingManager->PeekWorld().IsSerialisingHeroState()`, the TU's out-of-line
  CThing::PeekComponent (0x004C7A70) defined in the TU so VC7.1 expands it as retail does, implicit `const char*` -> CCharString
  arguments for DisplayProgress / AddTC, and the component lookup through the inline CCharString operator==.
- **CEngineSkyRenderer** 0x00B665E0 `?GetEnvironmentMap@CEngineSkyRenderer@@QAEXPAVCTexture@@ABVCEngineComponentRenderParams@@@Z`,
  RELOCATION_MATCH in 3 grades. Shape levers: (1) the 120-degree FOV is a local constant (`const float fov`) and HorizontalFOV is
  assigned after the function-local static C3DVector, so the fptan sits before the static's guard and the FOV store after it
  (0x00B667E2 / 0x00B66844); (2) the virtual window is one temporary `SetVWindow(C2DBoxF(0, 0, GFAXToVX((float)surface.GetWidth()),
  GFAYToVY((float)surface.GetHeight())))`: VC7.1 builds both GetDimensions temporaries first (height, then width) in their own frame
  slots, and the box temporary shares the GetSurfaceLevel temporary's slot (a named box costs 8 frame bytes). The true length is 1171
  (ret 8 at 0x00B66A70; int3 padding; 0x00B66A80 starts a 12-byte copy function, then more small functions) -- the 1454-byte
  reservation ran over them; the oracle (1171) is right, so no SPLITS.tsv line.

## Parked for Fable 5.1 (2)
- 00b54890 CEngineWeatherRenderer::RenderRain DIFFER(1431v1431): the two inline STLport push_back copies of the box position --
  retail 0x00B54C8D..0x00B54C9F / 0x00B54CD3..0x00B54CE4 load X, Y, Z then store all three; ours load/store pairwise (everything
  else equal, 424/428 lines) | tried: 3 grades (shape fixed: a named `float radius` local gives retail's [esp+28h] store and the 0x44
  frame; SetCommonParticleShaderConstants' real body visible + noinline keeps the constant index in ebx; `long constant = 9` before the
  constant-8 call) and 10 diagnostic probes with stub bodies: the load-ahead copy appears only when ClipBoxToCamera, ClipBoxToLandscape
  and ClipBoxToWeatherMaskSphere are ALL visible (CLM yes; LAND alone, CAM+MASK+instantiated _M_insert_overflow, LAND+MASK+INST,
  CAM+LAND+INST no) | next: pair-landing -- ClipBoxToLandscape 0x00B52DD0 is unlanded (~4.5 KB, to 0x00B53F59); author it, then put
  the three real bodies (landed 0x00B51F60 / 0x00B52090 under their PDB names, noinline) before RenderRain in this TU.
- 00bb1f50 CEngineClothRenderer::CalculateVertexNormals DIFFER(1508v1508): 420/425 lines equal after 30 grades -- (1) 0x00BB1FDB
  `mov edi,[esi+98h]` + 0x00BB1FED `sub edx,edi` (ours eax, and the sub before `add esi,98h`): the register of the triangle-count
  begin temporary; (2) the triangle loop's 2nd / 3rd corner add: retail stores the W self-copy (0x00BB21DF) straight after
  `add eax,esi` and keeps `fld [n.X]` (0x00BB21FC) after reloading VertexNormals, ours delays the W store past the Z add and hoists
  the next fld | tried: named / unnamed edge and cross locals, const locals, triangle / quad references, while / do / pointer loops,
  signed indices and loop counters, statement orders, operator+= through operator+, CROSS_X / CROSS_XY / SUBTRACT_XY groupings (the
  CROSS_X_GROUP + SUBTRACT_XY_GROUP pair fixed the whole quad loop, 412 -> 420, but moved the triangle residue) | next: trace c2's
  scheduling regions of the triangle else-arm (hand890 sched/sim.py, t13.py) to place the 81-tuple cut; measure the begin temporary's
  register choice with c2prio. The SSE block goes through a new shared helper (below); its ASM_RULES attempts table is NOT complete
  (see "Asm helper").

## Asm helper (row 0x00BB1F50, parked)
- `engine/CEngineClothRendererSSE.h` GF_SSE_ADD_TRIANGLE_NORMAL (descriptive name; the block is inline in retail): the retail bytes
  0x00BB2079..0x00BB20ED re-read both pointers and the three corner indices from frame slots that 0x00BB205E..0x00BB206C stored just
  before the g_SSEAvailable test (the x87 path uses the registers), and the function has no `and esp,-16` frame -- the inline-asm
  signature the 2026-11-05 round-2 helpers (CParticleProgramSSE.h, NVectorMathSSE.h) document. FABLE_PORTABLE twin and a dual-path
  test (hdr/tests_portable/test_CEngineClothRendererSSE.cpp: PASS portable / VC7.1 asm / VC7.1 asm+twin).
- ASM_RULES rule 4 requires >= 30 plain-C++ and >= 10 intrinsic spellings recorded for this site before the helper is used. I did
  NOT run that table (all grades went into the x87 residue); the row is parked, so the helper must not land before the table exists.

## Skipped
None.

## Names
- Proven (two sources each; LABELS.tsv): the four rows, ThingTypeToTypeFlags, InitFromThingInfo, RenderSky, RefreshD3DTransforms,
  SetCameraState, the CEngineCameraDesc copy constructor, ClearRenderTarget, SetRenderTarget, CRenderTarget::Detach/Attach* and
  Uninitialise, CGraphicInfo / CGraphicInfoFrame::GetVolumeTexture, ClipBoxToCamera, ClipBoxToWeatherMaskSphere, ClipBoxToLandscape,
  SetCommonParticleShaderConstants, RenderRainBox, GetBoxBoundingSphereRadiusSquared, GetNormalBuffer, the NVectorMath pair.
- Label corrections: NAME_FIXES.tsv (11 lines; e.g. 0x0051E580 is ThingTypeToTypeFlags not GetThingTypeFlags, 0x00A10E20 is
  DetachZBuffer, 0x00B52090 is ClipBoxToWeatherMaskSphere not IsPointInPrecipitation, 0x004C7F40 takes CThing::CSerialiseInfo).
- Descriptive (searched Ego_r / FableWin / Ego_d name sets, strings, reloc_bindings; nothing names them): GF_SSE_ADD_TRIANGLE_NORMAL /
  fable_portable::AddTriangleNormalSSE; the enumerators WEATHER_BOX_VISIBLE / WEATHER_BOX_CLIPPED of CEngineWeatherRenderer::EWeatherBoxVisible
  (values 0 / 1 from the clip tests); the function-local static `sky_pos` of GetEnvironmentMap; all local variable names.
- Established spellings reused (no PDB data symbols): g_DefaultSerialisationSectionName + inline GetDefaultSerialisationSectionName()
  (as the landed same-TU 0x004CA010; the static initializer row 0x0121E160 names the same object g_CCharString_013bab0c -- two
  spellings for 0x013BAB0C, review at landing), g_pDisplayManager_01436e1c, g_skyZBufferStart_0139a704, g_pEngineEnvironmentDef,
  g_SSEAvailable, NEngineInternal::GP*.

## Header debt
- engine/CEngineCameraDesc.h declares `class`; the PDB key is `struct` (opt-in FABLE_RETAIL_CENGINECAMERADESC_STRUCT here).
- No repo header yet for CRenderTarget, CEngineComponentRenderParams / Params, CVolumeTexture, CWeatherMask, CEngineClothRenderer,
  C3DClothRenderVertex, NVectorMath (new overlay headers).
- The landed CEngineSkyRenderer::Render 0x00B662F0 and CRenderTarget rows still use local models / stand-in classes.
