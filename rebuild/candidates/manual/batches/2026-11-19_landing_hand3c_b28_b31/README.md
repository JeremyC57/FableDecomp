# Landing hand3c b28 / b29 / b30 / b31 (Account B, 2026-11-19)

Four new rows from the hand3c hand-off (Account B's chunk-1 author) and three re-ports. All seven passed strict vland (`--land`)
with RELOCATION_MATCH and their tests PASS. An independent audit was done first: `audit_REPORT.md` (it also covers b24, which is not
in this batch).

| address | row | grade | notes |
|---|---|---|---|
| 0x0063F4B0 | CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind2<...>, CThingFindNavigationLayerAt> | RELOCATION_MATCH | audit fix: `operator()(const CThing&)` (Ego_d / FableWin `??R...@@QBE_NABVCThing@@@Z`), not `const CThingPhysical&` |
| 0x008A8260 | CGameScriptInterface::GetHeroSummonedCreaturesList | RELOCATION_MATCH | as authored |
| 0x00B51AD0 | CEngineWeatherRenderer::BuildBoxPositionMatrix | RELOCATION_MATCH | as authored; new C3DCoordI.h, BOXMATRIX / BOXCENTER opt-ins |
| 0x00B53F60 | CEngineWeatherRenderer::FrameStart | RELOCATION_MATCH | `/d2inlT94` (window 94..422, audit-confirmed; inline-budget rule 2026-11-08); audit fix: `index`, `old_mipmap_delay`, `old_reuse_busy` not const (Ego_d types) |
| 0x00B52990 | CEngineWeatherRenderer::UpdateBoxPositions | re-port, RELOCATION_MATCH | the old row had a local padded class model, the QAE spelling, made-up locals and a redundant `#pragma optimize("y", off)`; it is now on the shared header, AAE (PDB), with Ego_d locals in one block under the lock test |
| 0x00A13BE0 | C3DCoordI::operator+= | re-port, RELOCATION_MATCH | the local `class C3DCoordI` model is replaced by the new shared engine/C3DCoordI.h (FABLE_RETAIL_C3DCOORDI_OPERATOR_PLUS_EQUALS); otherwise the same text |
| 0x00A13C20 | C3DCoordI::operator-= | re-port, RELOCATION_MATCH | the same (FABLE_RETAIL_C3DCOORDI_OPERATOR_MINUS_EQUALS); the shared stray-maxps helper is unchanged |

## Same-TU strong copies (GOTCHAS line 455)
FrameStart's TU carries strong copies of UpdateBoxPositions (0x00B52990) and BuildBoxPositionMatrix (0x00B51AD0) plus the inline
GetBoxCoord, each under `// Fidelity: same-TU strong definition of 0x... (retail TU-mate)`. The UpdateBoxPositions copy is the re-ported
0x00B52990 body verbatim, and BuildBoxPositionMatrix + GetBoxCoord match b30 byte for byte (audit). Removing either body measures
416v411. The landed 0x00B52990 text could not be used verbatim: it had a local class model, and its pragma gives FrameStart an ebp
frame (403v411). That is why it is re-ported here, and its new body is the copy's text. These copies give duplicate external symbols at a
whole-program link, as the ten landed copies before them do (e.g. the CTCTavernGameCardBase rows); the owner should note this.

## Headers (additive, opt-in only; hdrcheck2 default=same for all)
- NEW engine/C3DCoordI.h: the Ego_r layout (C2DCoordI base, long Z) plus the two operator opt-ins. Without the re-ports, the two landed
  C3DCoordI rows' local class models would trip SHARED_HEADER_AVAILABLE once the header exists.
- engine/CEngineWeatherRenderer.h: FABLE_RETAIL_CENGINEWEATHERRENDERER_BOXMATRIX and _FRAMESTART.
- engine/CEngineWeatherSettings.h: FABLE_RETAIL_CENGINEWEATHERSETTINGS_BOXCENTER (ParticleBoxCenter typed C3DCoordI); asserts pinned (_AT 87/88).
- engine/CTextureManager.h: FABLE_RETAIL_CTEXTUREMANAGER_REUSEBUSY (static ReuseBusyTextures 0x013D28E8 + FableWin accessors).
- Includer regrade (audit): all 30 CEngineWeather* / CTextureManager includers. 19 MATCH / RELOCATION_MATCH + PASS. 11 give strict
  HEADER_POLICY with or without the overlay; all 11 pass non-strict, so this is pre-existing (list in audit_REPORT.md).

## Mutants
The audit added mutants: b28 5 (4 killed, 1 equivalent: `new_layer >` to `>=`), b29 5/5, b30 6/6, b31 7/7, 0x00B52990 6/6 (logs/).

## For Account A
NAME_FIXES.tsv: the 0x0063F7C0 operator() signature (same error as b28 had), GPCamera, BuildBoxPositionMatrix AAE, and the label
`?ReuseBusyTextures@CTextureManager@@0_NA` for 0x013D28E8. 0x00B52990's AAE fix is done here.

## Review items (owner)
- GOTCHAS line 446 ("summary only from an earlier definition"): b31 also matches with FrameStart defined first (Ego_d debug order).
  Here the rule holds only as a sufficiency, not a necessity.
- b30's 0..side loop wording is selected by the bytes but has no source. It is marked `// Shape:`; arguably it is `// Inferred:`.
