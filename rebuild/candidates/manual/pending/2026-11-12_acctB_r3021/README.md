# acctB hand-back, row 0x00A9EB90 C3DAnimationManager::BlendAnimations: matched (was parked with one operand differing)

> **SUPERSEDED for the row sources and tests of 0x00A9EB90 by `2026-11-12_acctB_u3022`** (one helper text set for the three
> C3DAnimationManager rows; land the rows from there). The notes and evidence below stay valid as history.

| address | result | notes |
|---|---|---|
| 0x00A9EB90 `?BlendAnimations@C3DAnimationManager@@AAEXPAVC3DKeyframe@@PBV2@KM@Z` | RELOCATION_MATCH 1337v1337, test PASS (g++ and VC7.1) | grade `retail_asm` through the shared rdtsc / fistp helpers, as parked. 65 grades this pass (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win, no warnings.

## What closed it

Only the text of the two table helpers; the row body is unchanged.

1. `GFFastSin` with chunk 2044's text (named table values `a` / `b`, proven by FableWin size there): fixed the
   parked SIB order at 0x00A9EC5A on the first grade and exposed an operand-order difference in the SSE loop's dot
   product (0x00A9EC40..0x00A9EC55).
2. `GFFastArcCos` written the same way
   (`float a = g_ArcCosTable[i]; float b = g_ArcCosTable[i + 1]; return ((1.0f - frac) * a + frac * b) * 0.15915494f;`).

## OPEN review item: two GFFastArcCos texts among the pending rows

The named-value `GFFastArcCos` text is FITTED (FableWin's copy carries asserts, so there is no size evidence). Under
it every landed user regrades unchanged (0x00A22530, 0x00AA3390, 0x00AA3560, 0x00A22590, 0x00AA3C50, 0x00B05620) and
the pending 0x00AA0090 still matches, **but the pending twin 0x00A9F540 (`2026-11-12_acctB_r2044`) then differs** in
the 4-term Dot order of its expanded BlendWith (0x00A9FA56..0x00A9FA87); that row matched with the one-line text.
Both rows keep the helper as a row-local copy, so each hand-back matches as staged, but retail had one text. Do not
land a shared-header body for `GFFastArcCos` from either row until this is settled; this is now settled in
`2026-11-12_acctB_u3022`.

## Other notes

- The match is not on an edge: it holds with 1 to 3 extra diagnostic dead locals and breaks at 8.
- Made-up names (unchanged): `g_BlendAnimationsCycles` 0x013D2D88, `g_BlendAnimationsKeyframes` 0x013D2D90.
- **Headers:** the chunk 3005 overlay unchanged. No name fixes.
