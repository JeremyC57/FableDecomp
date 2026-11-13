# 2026-11-11_acctB_c2020 -- acctB hand-back, chunk 2020 (the 3 rows chunk 2012 left: 2 matched, 1 parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed. This folder supersedes the
parked source of 0x006D1250 in `2026-11-11_acctB_c2012/rows/parked/`.

| address | result | notes |
|---|---|---|
| 0x00949AB0 CAIStateGroup_GoToPub::IsRunnable | RELOCATION_MATCH 1572v1572, test PASS | 2 grades. Defines the same-TU GetSeat 0x009482A0 with NULL default arguments (retail expands the constant-argument call and calls the scripted one) |
| 0x006414A0 CTCLight::DrawLight | RELOCATION_MATCH 1596v1596, test PASS | 3 grades; jump table checked case by case; uses the existing FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN opt-in |
| 0x006D1250 CTCWife::CheckAppearanceChanges | parked, DIFFER 1629v1608 (0.7953 exact share, 0.9001 masked) | 3 more grades only (working budget); frame-slot order from 0x006D1269, loop-1 load order 0x006D132E, `this` reload 0x006D15ED, tattoo loop 0x006D16B0..0x006D1769; `rows/parked/`, STATUS.tsv; no test yet |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT; four label-only RELOC_NAME warnings,
all in NAME_FIXES.tsv). Purity and padded-struct greps are clean.

BONUS for the coordinator (not an acctB row): the GetSeat body in `rows/00949ab0.cpp` grades RELOCATION_MATCH against retail
0x009482A0 (1072 bytes) with leaf `?GetSeat@CAIStateGroup_GoToPub@@IAE_NPAVCThing@@PBVCCharString@@@Z` -- checked by the
acctB coordinator with q.py on a scratch oracle row. That row is parked in `pending/2026-10-30_parked_retry2_sec4`.

To land: the rows need `hdr/` (2 new headers; opt-ins in copies of 8 repo headers; `hdr/README.md`; the c2012 / c2007
overlay headers are carried unchanged). One fix to an existing block: in NGraphicAppearance_CTCGraphicAppearance.h,
LIGHTINGCHANNEL + MOVEMENT used to redefine `LightingChannel`; check no landed file defines both without
VOLUMECONTAINMENTTRACKER. Rebuild the oracle from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 5 entries.
Descriptive: global 0x013BA85C `g_LightsDrawnThisFrame`; building property passed as `(ESimBuildingProperty)0x20`.
