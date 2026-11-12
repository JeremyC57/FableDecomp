# acctB hand-back, chunk 2031 (2 reserved rows: 0 matched, 2 parked -- one is a proven pair-landing case)

Authored by an Opus 5.5 agent; the acctB local coordinator regraded both parked sources and the pair-landing diagnostic
and ran both tests. Nothing is landed and no row here is landable on its own.

| address | result | notes |
|---|---|---|
| 0x00B54E30 CEngineWeatherRenderer::RenderSnow | parked, DIFFER 1901v1901, 569 of 573 instructions | **Pair-landing, proven.** Only the two inline push_back C3DVector copies differ (retail 0x00B553BD..0x00B553CF and 0x00B55403..0x00B55414 load X, Y, Z then store; ours interleaves) -- the same residue as RenderRain 0x00B54890 (c2008). With non-capturing, side-effecting stub bodies of all three clip functions visible in the TU (ClipBoxToCamera 0x00B51F60, ClipBoxToWeatherMaskSphere 0x00B52090, ClipBoxToLandscape 0x00B52DD0) the same source grades RELOCATION_MATCH: `probe/diag2_clm.cpp` (diagnostic only, not a deliverable; I regraded it: RELOCATION_MATCH). Camera + mask only, or landscape only, stays 569/573. 11 grades |
| 0x00B65A20 CEngineSkyRenderer::RenderOuterSky | parked, DIFFER 1897v1893, 597 of 602 instructions | start address checked: a real function start (direct call from RenderSky 0x00B66284; the previous function's cold tail ends at 0x00B65A1E). Residue in the shader tail only: (a) the blend vector's dead Y store 0x00B65E42 comes after `mov eax,[GPShaderRenderManager]` in retail (a scheduler priority tie), (b) the `UpdateFlags \|= 2` temporary is ebx in retail, esi in ours (0x00B65E5A..67), (c) colour 13's first copy 0x00B65E97 `add eax,0D0h ; mov ecx,[eax]` v ours `mov ecx,[eax+0D0h]` before the add. 6 grades; **passes 2 and 3 not used (agent context)**. Queued for the acctB near-miss lane |

Coordinator checks (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): both parked sources regrade as recorded; both
`rows/parked/<addr>.test.cpp` print `<addr>_TEST PASS` under g++ (the agent also ran them under VC7.1). Purity grep clean.

What unblocks RenderSnow and RenderRain: ClipBoxToLandscape 0x00B52DD0 (4489 bytes, unlanded) **is in the acctB
reservation**; acctB is authoring it next and will then regrade 0x00B54890 and 0x00B54E30 with the three real bodies
(noinline, PDB names) in one TU. Those three rows will have to land together.

Review items:

- NAME_FIXES.tsv (6): 0x0041B9C0 is `CResource::TouchResource` (landed as `CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera`);
  0x009FBFB0 / 0x009FBF90 are the private `CResource::ListRemove` / `ListInsert` (landed as camera-spline stand-ins);
  0x00B52160 `SetCommonParticleShaderConstants(long&)`; 0x006B7350 `CEnvironment::PeekInterpolatedParams` (landed as
  `C_006b7350::M`); 0x00989B50 is the CRGBFloatColour overload of SetUserVertexShaderConstant.
- LABELS.tsv: 10 names incl. the uncalled COMDATs 0x0041BB70 TouchTexture, 0x0041C1F0 PeekGraphicInfoFrame, 0x0041C260
  GetFrame (the header body compiled out of line grades RELOCATION_MATCH against it per the agent), and 0x00A0ACA0.
- 0x01436E48 has 4 bound spellings and 0x01436E8C has 3: one address, one name -- pick at landing.
- Header debt (also reported by c2030): `rebuild/include/engine/CWorld.h` has CIDrawWorld slots 1 and 2 swapped against
  vtable 0x012390DC. This chunk and c2030 each wrote a `CIDrawWorld.h` / `_GAMEWORLD` opt-in: merge them into one.

`hdr/` is the overlay the parked sources need (opt-ins and new headers, evidence per item in `hdr/README.md`). No retail
bytes are in this folder.
