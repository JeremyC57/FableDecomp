# acctB hand-back, chunk 2048 (CEngineSkyRenderer): 2 of 2 rows matched

Four agents (Opus 5.5) worked this chunk in the same directory. Re-verified by the acctB local coordinator. Nothing
is landed. This folder replaces its earlier state (RenderSunFlare matched, RenderClouds parked at 731/734).

| address | result | notes |
|---|---|---|
| 0x00B64550 `?RenderSunFlare@CEngineSkyRenderer@@AAEXXZ` | RELOCATION_MATCH 2631v2631, test PASS (g++ and VC7.1) | 23 full-row grades. Its source now also defines the three helper opt-ins RenderClouds needs (same bytes with or without them) |
| 0x00B64FA0 `?RenderClouds@CEngineSkyRenderer@@AAEXM@Z` | RELOCATION_MATCH 2688v2688 (734/734 instructions), test PASS (g++ and VC7.1) | about 116 full-row grades over four sessions |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (both grades, both
test runs, read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; one `WARN RELOC_NAME`, below).
Purity and padded-struct greps clean; no flags. Under the final overlay I regraded 0x00B65A20 RenderOuterSky (the row
of `2026-11-12_acctB_r3014`): RELOCATION_MATCH with its own source and with every helper opt-in switched on (agent's
`cand/ros_fits.cpp`). `grades.log` has one line per grade (155).

## RenderClouds: what the residue was and what closed it

The last difference was the operand order of the bookmark `or` in the twelve expanded `SetRenderState` blocks (retail
pattern BBAAABBBBBBB). It has two flip points. The back one moves with how the pixel constants are spelled; the front
one moves with how many named locals the function carries, counted across every expanded inline. Steps, each graded:

1. Helper bodies fitted to FableWin debug sizes (session 3; table in `hdr/README.md`): 721 -> 731.
2. Constants 5 and 6 back to `C4DVector(blend, blend, blend, blend)`, the spelling the matched sibling RenderOuterSky
   uses: fixes the back flip.
3. `CIVCountedPointer<CFrameData>::Release` through `IsNull()` and `CIVCountedPointeeBase::Release` with a named
   `unsigned long count` that deletes itself: FableWin instruction counts 17 and 24 reproduced.
4. One named local in the shared forced-inline `CRenderStateManager::SetRenderState`:
   `const unsigned long desired_state = state.DesiredState; if (desired_state != value) { ... }`. It is expanded twelve
   times, which supplies the twelve named symbols the front flip needs. Same code bytes.

## Review items

- **`FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED` is FITTED and touches a widely used inline.** It is an
  opt-in (default text unchanged), it is code-neutral in all three sky rows, and FableWin's debug `SetRenderState`
  carries assert code so its size proves nothing either way. A dead-local ruler confirms the mechanism (about twelve
  extra named symbols give retail's pattern), but which statement the original had is not known. The agent notes that
  the rows listed in GOTCHAS as parked on this same or-order (0x00B70230, 0x00B761D0, 0x00B751F0; not acctB rows)
  probably have the same cause and could be tried with this opt-in.
- **Helper opt-ins PROVEN by FableWin size** (plain VC7.1 `/Od` reproduces the debug function): named-result
  `C2DVector` `-` and `*`, `~CGraphicFrame` -> `ReleaseData`, direct-member `GetBank` / `GetCurrentFrame`,
  `CIVCountedPointer::Get`, `TouchTexture` through `Get()`, `PSConstTable[index]`; by instruction count only (bytes
  differ, later compiler): `IsLoading` = `!FileData.IsNull()`, the two `Release` bodies above. The named-result
  `C2DVector` `+` reproduces the FableWin size but changes RenderClouds' code, so no row uses it.
- **Unused opt-ins left in `hdr/`** (measured, not needed): `FABLE_RETAIL_CIVCOUNTEDPOINTER_CASTTOBASE`,
  `FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME_EARLYRETURN`, `FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_ADD`.
- **Fitted statement spellings:** RenderSunFlare's sibling-scope sun sprite, unnamed `C2DVector(...).GetMagnitude()`
  ratio and if / else around the loop frame; RenderClouds' constant 7 as
  `C4DVector(C3DVector(colour.GetR(), colour.GetG(), colour.GetB()), 1.0f)`.
- **Label:** 0x01396DC8 is labelled `g_textureUVModeToD3D` (a descriptive name from an earlier chunk); the header's
  PDB spelling is `?D3DTextureUVModeTable@CRenderStateManager@@0PAKA`. One source only, so it is a review note, not a
  NAME_FIXES entry.
- **Made-up name:** `IsFrameDataLoaded` (no PDB name found). `NAME_FIXES.tsv` as before (`BeginLoading` const,
  `CGraphicInfo::GetFrame`, `RenderScreenSpaceSprite` signature, 0x00989B00 `SetUserVertexShaderConstant`).
- **Headers:** the c2031 / r3014 overlay plus this chunk's opt-ins, the new `engine/CSkyDef.h` and a copy of
  `CIVCountedPointeeBase.h` with its opt-in. Land after c2031 / r3014; RenderOuterSky may adopt the same helper
  opt-ins (it matches either way).

## How to land

Both rows together: `rows/00b64550.*` and `rows/00b64fa0.*` with `hdr/engine/`, then `payload.json` through your
usual vland.
