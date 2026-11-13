# acctB hand-back, chunk 2037 (3 reserved rows: 3 matched; land them together)

Authored by two successive Opus 5.5 agents (the second continued in the first one's directory), re-verified by the acctB
local coordinator. Nothing is landed. This replaces the earlier version of this folder (which had 0x00B52DD0 parked).

| address | result | notes |
|---|---|---|
| 0x00B52DD0 CEngineWeatherRenderer::ClipBoxToLandscape(const C3DVector&) const | RELOCATION_MATCH 4489v4489, test PASS (g++ and VC7.1) | `// Flags: /d2inlT507` + `// Inline-budget:` line (PeekLandscapeHeight c1 345, score 506 at its four sites; the two CEngineWorldMap wrappers are FABLE_FORCEINLINE because c2 never plain-inlines a callee costing over 450 and MapHasDataAtPoint costs 533). Grade `retail_asm`: the GFFloatToLongFloor / Ceil / Near helper sites, with a `// Retail-asm:` block and a per-site attempts table (32 / 30 / 31 C++ spellings + 12 intrinsics under /O2 and /O2 /QIfist; none emits a bare `fistp dword`; `probe/asm/` on the agent side, summary in the row). 10 grades in all |
| 0x00B54890 CEngineWeatherRenderer::RenderRain | RELOCATION_MATCH 1431v1431, test PASS (g++ and VC7.1) | supersedes `2026-11-11_acctB_c2008/rows/parked/00b54890.cpp`. c2008's source plus the three clip-function bodies in the TU |
| 0x00B54E30 CEngineWeatherRenderer::RenderSnow | RELOCATION_MATCH 1901v1901, test PASS (g++ and VC7.1) | supersedes `2026-11-12_acctB_c2031/rows/parked/00b54e30.cpp`. c2031's source plus the same three bodies |

**Pair-landing.** RenderRain / RenderSnow schedule their inline push_back C3DVector copies as retail does only with the
bodies of the three clip functions visible in the TU. Each row therefore carries, `__declspec(noinline)` and in retail
address order, ClipBoxToCamera 0x00B51F60, ClipBoxToWeatherMaskSphere 0x00B52090 (ported onto the shared headers) and the
now matching ClipBoxToLandscape body (the same text as `rows/00b52dd0.cpp`). Grade each row's own leaf. The rows'
`// Retail-asm:` line exists because the purity check reads the whole TU text; their own leaves have no asm-helper site.

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (three grades, both
test runs, read-only vland: 3 wins, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; RELOC_NAME on the respelled 0x00B52090
and on 0x01436EA0). Purity and padded-struct greps are clean.

What closed 0x00B52DD0: nothing in the function body (it is the first agent's). The neighbour-map branch of the inline
`CEngineMap::PeekLandscapeHeight` now passes its own argument expressions to the recursive call
(`map->PeekLandscapeHeight(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y)`) instead of the
named cells of the test above it; that halves the cells' references in the last of the four expanded corners and gives
retail's register choices there (measured with c2prio; NOTES.md).

Review items for landing:

- **Header debt, still open: `FABLE_RETAIL_CENGINEMAP_MAPHASDATA_IFRETURN` in `hdr/engine/CEngineMap.h`** changes the
  bodies of the shared inlines CEngineMap::MapHasDataAtPoint / PeekLandscapeHeight for this row only. What I checked:
  with c2021's overlay merged under this one and the opt-in NOT defined, c2021's rows 0x00B3F5E0 (`/d2inlT444`) and
  0x00B40BD0 (`/d2inlT576`) both still grade RELOCATION_MATCH -- the default text is unchanged, so landing this opt-in
  breaks nothing. What the agent reports with the opt-in DEFINED: retail's uncalled copies 0x00B56880 MATCH and 0x00B566B0
  RELOCATION_MATCH, c2021's 0x00B3F5E0 RELOCATION_MATCH, but c2021's 0x00B40BD0 PeekWaterDepth DIFFER(1780v1736). So no single
  header text serves every row yet; retail compiled one. The agent's analysis of why the facts conflict and its ranked
  next ideas are in NOTES.md. Owner / coordinator decision whether to land with the row opt-in as debt.
- NAME_FIXES.tsv (4): 0x00B51F60 `?ClipBoxToCamera@CEngineWeatherRenderer@@ABE...`; 0x00B52090
  `ClipBoxToWeatherMaskSphere` (landed as IsPointInPrecipitation); 0x00B30610
  `?ClipBox@CEngineCamera@@QBE_NABVCMatrix3x4@@@Z`; 0x00B50E50
  `?GetBoxBoundingSphereRadiusSquared@CEngineWeatherRenderer@@ABEMXZ`. LABELS.tsv adds the two retail COMDATs 0x00B566B0 /
  0x00B56880.
- 0x01436EA0 is `NEngineInternal::GPCamera` here, bound as `gWeatherCamera` in the landed 0x00B51F60: pick one.
  Descriptive names: WEATHER_BOX_VISIBLE / WEATHER_BOX_CLIPPED, locals.
- Not checked by me: the agent's statement that the ported ClipBoxToCamera / ClipBoxToWeatherMaskSphere bodies grade
  RELOCATION_MATCH against their own retail bytes when compiled out of line.

To land: `hdr/` per `hdr/README.md` (opt-ins in CEngineWorldMap.h, CEngineMap.h, CEngineWeatherRenderer.h `_CLIPBODIES`,
CEngineCamera.h `_CLIPBOX`; copies from c2008 / c2021 / c2031 -- merge after those; c2021's CEngineLandscapeRenderer.h is
not in this overlay). Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
