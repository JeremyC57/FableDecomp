# acctB hand-back, chunk 2037 (3 reserved rows: 2 matched -- pair-landing, read the caveat -- and 1 parked, still being worked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00B54890 CEngineWeatherRenderer::RenderRain | RELOCATION_MATCH 1431v1431, test PASS (g++ and VC7.1) | **supersedes `2026-11-11_acctB_c2008/rows/parked/00b54890.cpp`.** The c2008 source unchanged, plus the three clip-function bodies in the TU (see caveat) |
| 0x00B54E30 CEngineWeatherRenderer::RenderSnow | RELOCATION_MATCH 1901v1901, test PASS (g++ and VC7.1) | **supersedes `2026-11-12_acctB_c2031/rows/parked/00b54e30.cpp`.** The c2031 source unchanged, plus the same three bodies |
| 0x00B52DD0 CEngineWeatherRenderer::ClipBoxToLandscape (4489 bytes) | parked, DIFFER 4482v4489, 1351 of 1528 instructions | corners 1-3 (to 0x00B53AEA) and the frame are byte-exact; every difference is register allocation in corner 4, the last of four expanded copies (0x00B53B65 `mov edi,[ebp+8]` v ours esi; 0x00B53BA2; the neighbour path 0x00B53E04..0x00B53F13). `// Flags: /d2inlT497` + forced CEngineWorldMap wrappers. 7 grades; an acctB continuation agent is working it now. Source + passing test in `rows/parked/` |

**Caveat on the two matched rows (pair-landing).** RenderRain / RenderSnow only schedule their two inline push_back
C3DVector copies as retail does when the bodies of the three clip functions they call are visible in the TU. Each row
therefore carries, `__declspec(noinline)` and in retail address order: ClipBoxToCamera 0x00B51F60 and
ClipBoxToWeatherMaskSphere 0x00B52090 (ported onto the shared headers; the agent reports each grades RELOCATION_MATCH
against its own retail bytes when compiled out of line, `probe/pc.cpp` on its side -- I did not regrade those two), and the
**current, not yet matching** ClipBoxToLandscape body. Only each row's own leaf is graded and matches. What the callers
need is a visible callee with unknown side effects (c2031's stub diagnostic showed the same), so the matches should
survive the final ClipBoxToLandscape body, but they must be regraded with it, and the rows should land together with
0x00B52DD0 (acctB will hand back updated copies when it matches). Each row has a `// Retail-asm:` line only because the
purity check reads the whole TU text and the ClipBoxToLandscape body calls the GFFloatToLong helpers; the rows' own leaves
contain no asm-helper site.

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grades, both test
runs, read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; RELOC_NAME on the respelled 0x00B52090 and on
0x01436EA0). Purity and padded-struct greps are clean. Parked row regraded with its flag: DIFFER(4482v4489).

Review items for landing:

- **Header conflict, unresolved:** `hdr/engine/CEngineMap.h` (copied from c2021) adds opt-in
  `FABLE_RETAIL_CENGINEMAP_MAPHASDATA_IFRETURN`, which changes the body of the shared inline
  CEngineMap::MapHasDataAtPoint (if-return form) and writes out PeekLandscapeHeight's neighbour test. With it, retail's
  uncalled COMDATs 0x00B56880 / 0x00B566B0 grade MATCH / RELOCATION_MATCH, but c2021's row 0x00B3F5E0 grades DIFFER under it
  (c2021's rows use the `&&` chain form, as does retail's out-of-line 0x00B439C0). Retail compiled one header text: the
  continuation agent is instructed to find the one body that serves all rows. Do not land this opt-in as it stands.
- NAME_FIXES.tsv (4): 0x00B51F60 `?ClipBoxToCamera@CEngineWeatherRenderer@@ABE...` (landed stand-in name and binding
  differ); 0x00B52090 `ClipBoxToWeatherMaskSphere` (landed as IsPointInPrecipitation); 0x00B30610
  `?ClipBox@CEngineCamera@@QBE_NABVCMatrix3x4@@@Z`; 0x00B50E50
  `?GetBoxBoundingSphereRadiusSquared@CEngineWeatherRenderer@@ABEMXZ`.
- 0x01436EA0 is spelled `NEngineInternal::GPCamera` here, bound as `gWeatherCamera` in the landed 0x00B51F60: one address,
  one name -- pick at landing. Descriptive names: enumerators WEATHER_BOX_VISIBLE / WEATHER_BOX_CLIPPED, locals.
- The parked row has no per-site attempts table for its asm-helper sites yet (ASM_RULES requires it before landing).

To land (later, with 0x00B52DD0): `hdr/` per `hdr/README.md` (opt-ins in CEngineWorldMap.h, CEngineWeatherRenderer.h
`_CLIPBODIES`, CEngineCamera.h `_CLIPBOX`; copies from c2008 / c2021 / c2031). Rebuild the oracle from Fable.exe (no retail
bytes are in this folder).
