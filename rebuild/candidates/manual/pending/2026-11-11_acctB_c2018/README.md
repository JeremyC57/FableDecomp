# 2026-11-11_acctB_c2018 -- acctB hand-back, chunk 2018 (3 CEngineSurfaceCompositionManager rows, all matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed. 0x00B2B180 is the row chunk
2011 did not reach.

| address | result | notes |
|---|---|---|
| 0x00B2CB40 CEngineSurfaceCompositionManager::CEngineSurfaceCompositionManager() | RELOCATION_MATCH 1637v1637, test PASS | 7 grades; no flags (`FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE`) |
| 0x00B2B180 CEngineSurfaceCompositionManager::RenderToTexture | RELOCATION_MATCH 1539v1539, test PASS | 4 grades |
| 0x00B2D330 CEngineSurfaceCompositionManager::UpdateDiskCacheTexture | RELOCATION_MATCH 1723v1723, test PASS | 8 grades. PAIR LANDING with 0x00B2B180: its TU defines RenderToTexture (same body as rows/00b2b180.cpp) and a noinline GetDimensionsAndAdjustMipmapLevel (landed 0x00B2AEF0); without those bodies the 0x48 frame and registers do not match |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 3 wins, no RELOC_IDENTITY / RELOC_CONFLICT; label-only RELOC_NAME warnings, covered
by NAME_FIXES.tsv). Purity and padded-struct greps are clean.

REVIEW at landing: 0x00B2D330 forces four Lionhead header helpers inline with FABLE_FORCEINLINE (GFGetTopBit,
CGraphicInfoFrame::GetFrame, CGraphicFrameData::Release, CGraphicFrame::operator=). The `// Inline-budget:` line records the
c2 trace scores without forcing (125, 142, 85, 115-127 against the threshold 74); the agent reports their retail out-of-line
copies have no caller.

To land: the rows need `hdr/` (opt-ins in 17 existing headers, default text unchanged; new headers CGraphicDataBankInit,
CEngineTextureMorph, CCompositeSurfaceCache, CSurfaceCompositor, CBankStateBlock; `hdr/README.md`). The SURFACECOMPOSITION
branches overlap the STATICBLOCKS models and should be merged. Rebuild the oracle from Fable.exe (no retail bytes are in this
folder). NAME_FIXES.tsv: 17 rows. reloc_bindings.tsv has several names for 0x00B59710 and 0x0099B7D0 (left unchanged;
possibly folded bodies).
