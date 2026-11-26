# 0x00B6DF50 CEngineWaterRenderer::GetIceForegroundVertexShader

Landed 2026-11-26 on FullDecomp-B under VC7.1 RTM cl 13.10.3077. Grade MATCH, behaviour test `00b6df50_TEST PASS`. Retail extent 54 bytes (`ret 4`).

PDB spelling, both tables: `?GetIceForegroundVertexShader@CEngineWaterRenderer@@AAEAAVCVertexShader@@J@Z` (Ego_r 0x00A1B5B0, FableWin 0x02D4CDC0). The body reads `IceForegroundVertexShader` at +0x12C and clamps with the same unsigned `size() - 1` ternary as the landed twins 0x00B6DF90 and 0x00B6DFD0.

The manifest still says `CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader`. That spelling is the landed row 0x00BF5C60 (array at +0x4E8). The respell is queued in `NAME_FIXES.tsv` for Account A. The unmatched length estimate was 64.
