# 2026-11-11_acctB_c2021 -- acctB hand-back, chunk 2021 (2 reserved rows: 2 matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00B3F5E0 CEngineMap::PeekMapNormal(long,long) const | RELOCATION_MATCH 1624v1624, test PASS (g++ and VC7.1) | `// Flags: /d2inlT444` + `// Inline-budget:` line (four expanded PeekLandscapeHeight sites, c1 345, f=1, score 443). grade `retail_asm`: four expanded QuantiseLandscapeHeight roundings through the existing `engine/GFFloatToLongNear.h`, `// Retail-asm:` line cites the four `fld`/`fistp dword` sites (0x00B3F739, 0x00B3F89D, 0x00B3F9FA, 0x00B3FB3F). 3 grades |
| 0x00B40BD0 CEngineMap::PeekWaterDepth(long,long) const | RELOCATION_MATCH 1736v1736, test PASS (g++ and VC7.1) | `// Flags: /d2inlT576` + `// Inline-budget:` line (five expanded PeekThemeId / PeekThemeBlend sites, c1 308, scores 403 at f=1 up to 575 at f=0.25; T575 calls the last two). 9 grades |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grades, both test runs,
read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean.

Review items for landing:

- **Inline-budget values are large (444 and 576 against the default).** The agent measured that the header bodies as
  `__forceinline` also match both rows, and rejected that because CWaterPatchMesh::GetWaterType 0x00BEAB40 (another TU) calls
  the out-of-line PeekTheme* copy 0x00B43720 three times inside loops, which a forced inline would expand. Both rows also
  grade RELOCATION_MATCH at a shared T576 (agent's grade 3 of PeekMapNormal), so one TU-wide value is possible; the parked
  sibling 0x00B41770 needed T301. Owner / coordinator decision: accept per-row `/d2inlT`, or one TU value.
- **retail_asm via the existing helper** (PeekMapNormal): per-site addresses are cited in the row, but there is no new
  per-site attempts table beyond the helper header's own proof.
- **Possible lever for parked siblings** (not acctB rows): 0x00B43720, 0x00B43870, 0x00B439C0, 0x00B41770. The measured
  difference was reading `g_pEngineWorldMap` directly in the inline bodies instead of through a `worldMap` local (the local
  made the first expanded copy reload MapWidth instead of spilling it). Not graded here (not reserved by acctB).
- 0x01231724 is the compiler literal pool `__real@3b808081` (1/255); landed rows bind it as named globals
  (`g_OneOver255`, `g_fReciprocal255_01231724`). NAME_FIXES.tsv is otherwise empty.

To land: both rows need `hdr/` (opt-ins `FABLE_RETAIL_CENGINEMAP_PEEK_BODIES` in a copy of engine/CEngineMap.h and
`FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME` in a copy of engine/CEngineLandscapeRenderer.h; default paths unchanged;
evidence in `hdr/README.md`). Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
