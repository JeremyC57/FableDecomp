# acctB hand-back, chunk 2048 (CEngineSkyRenderer): 1 of 2 rows matched, 1 near miss still in work

Three agents (Opus 5.5) worked this chunk in the same directory. Re-verified by the acctB local coordinator. Nothing
is landed.

| address | result | notes |
|---|---|---|
| 0x00B64550 `?RenderSunFlare@CEngineSkyRenderer@@AAEXXZ` | RELOCATION_MATCH 2631v2631, test PASS (g++ and VC7.1) | 23 full-row grades. No flags |
| 0x00B64FA0 `?RenderClouds@CEngineSkyRenderer@@AAEXM@Z` | DIFFER 2688v2688, 731/734 instructions equal (`rows/parked/`) | one `or` operand order left (SetTextureUVModeV(2): retail `test eax,ecx ; or ecx,eax ; mov [esi+8],ecx`). About 77 grades. A continuation agent is on it |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test
runs, read-only vland: 1 win, no warnings, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE). Purity and
padded-struct greps clean. Under the final overlay I regraded 0x00B65A20 RenderOuterSky (the row of
`2026-11-12_acctB_r3014`): RELOCATION_MATCH both with its own source and with every new helper opt-in switched on
(agent's `cand/ros_fits.cpp`). `grades.log` has one line per grade.

## What closed RenderSunFlare (all FITTED to retail bytes unless marked)

- The final sun sprite sits in its own block (`if (sun_texture != 0) { ... }`): its `CGraphicFrame` then shares the
  loop frame's stack slot (frame 0x7C as retail) and retail's three `RestoreCaptureBlock` exits appear.
- The flare ratio is computed on unnamed temporaries:
  `C2DVector(screen.X - centre_x, screen.Y - centre_y).GetMagnitude() / C2DVector(centre_x, centre_y).GetMagnitude()`
  (`GetMagnitude` and the constructor are PDB names; the statement shape is fitted).
- The existing repo opt-in `FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS` (already proven by an earlier row).
- `if / else` around the loop's frame where the draft had `continue`.

## New evidence source used here: FableWin debug sizes (`fwsize`)

Plain VC7.1 `/Od` reproduces the FableWin debug build's helper functions byte for byte when a helper's TEXT is right
(22 helpers of this TU measured exact). Helpers a few instructions off have a different text in the original, and
because VC7.1 release folds the difference away, the only visible effect is on tie-breaks that depend on how much
intermediate code the function has, which is exactly the RenderClouds residue. Helper opt-ins fitted this way (all in
`hdr/`, default texts unchanged; `hdr/README.md` has the table with sizes):

| opt-in | text | status |
|---|---|---|
| `FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB` / `_MUL` | `C2DVector result(...); return result;` in `operator-` / `operator*(float)` | size SAME (26i/68b, 24i/63b); code-neutral in all three rows |
| `FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_ADD` | same for `operator+` | size SAME, but it changes RenderClouds' code, so NO row uses it: either the 2004 source differed or the statement is spelled otherwise |
| `FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA` | `~CGraphicFrame` calls a private forced-inline `ReleaseData` | size SAME (9i/19b + 15i/38b); plain inline is refused in two rows (grades 95/96), hence forced |
| `FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT`, `FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT`, `FABLE_RETAIL_CIVCOUNTEDPOINTER_GET`, `FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET`, `FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX` | direct member reads, `Get()`, `PSConstTable[index]` | size SAME |
| `FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL` | `IsLoading` = `!FileData.IsNull()` | instruction count same, 2 bytes off (later compiler emits `add eax,1` for `inc eax`) |

Caveats: FableWin is a later build with asserts and was compiled with C++ exception handling (rows compare at
`/Od /EHsc`, leaf helpers at plain `/Od`), so its sizes are a hint for which text to try; the retail grade decides.
The tool is in the lane (`fwsize.py`), not in the repo: it reads `ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv`
(body bytes and instruction count per mangled name) and compiles the TU unoptimised; I can hand it over as a
`tools/` proposal if you want it.

## Review items

- RenderSunFlare's source defines the helper opt-ins so the three sky rows share one body per helper; it matches with
  or without them (agent's grades 94 / 98), so they can land later with RenderClouds if you prefer.
- `IsFrameDataLoaded` is a made-up name (no PDB name found); `g_pEngineLightingManager` 0x01436E9C follows the landed
  rows' spelling. The literal `0x1000000` passed to the occlusion query may be `GFMakeOccluderRetrivalID(...)`
  (FableWin lists it); left as a literal.
- `NAME_FIXES.tsv`: `BeginLoading@CGraphicInfoFrame` is const (some landed bindings spell it non-const or under
  NUISystem); one landed binding spells `CGraphicInfo::GetFrame` non-const with a pointer; the landed 0x00B63530 uses
  `CEngineSkyRenderer_Methods` with C2DVector arguments where the PDB has
  `RenderScreenSpaceSprite(const CTexture*, float, float, float, float, CRGBColour)`; 0x00989B00 is
  `?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKMMMM@Z`.
- Headers: the overlay is the c2031 / r3014 overlay plus this chunk's opt-ins and the new `engine/CSkyDef.h`
  (Ego_r layout minus 4, offsets +0x4C / +0x50 / +0x74 proven by retail reads). Land after c2031 / r3014.

## How to land

`rows/00b64550.cpp` + `rows/00b64550.test.cpp` with `hdr/engine/`, then `payload.json` through your usual vland.
`rows/parked/00b64fa0.*` is the current RenderClouds state for reference only.
