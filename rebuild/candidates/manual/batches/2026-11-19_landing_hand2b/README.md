# Landing hand2b (2026-11-19): 2 of hand1's parked rows (3,081 bytes)

vland --land, LANDVERIFY_STRICT_HEADERS=1: RELOCATION_MATCH + PASS for both (`logs/vland_land.log`); no new header opt-ins, no flags.

| row | what closed it |
|---|---|
| 0x00B6DC40 CEngineWaterRenderer::ProcessSeaLoading (771) | Fable advisor fable7's q4 shape (from Ego_d's line table / block extents) plus `LoadingSea.Reset(NULL)` / `LoadingSeaEntry.Reset(NULL)` pointer drops: FableWin lines 591/592, 613/614, 644/645 are 16 bytes each, and a VC7.1 /Od measurement gives Reset(NULL) 2 bytes per site more than ReleaseData(). Replaces the earlier /d2inlT196 form (no inline-budget flag now). |
| 0x00AE3C30 CParticleProgram::ReadFromFile (2310) | the faithful r4 shape (Ego_d line tables) with the existing opt-in FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE (CDataInputStream::Read's original locals) |

Still with hand2b: 0x005AC10E (escalated; Fable review: read FableWin's NotInStock loops) and 0x00F3ABF0 (reference-local match not
accepted -- coordinator decision delegated by the owner).
