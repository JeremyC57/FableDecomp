# 2026-11-11_acctB_c2008 -- acctB hand-back, chunk 2008 (4 reserved rows: 2 matched, 2 parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x004CA390 CThing::ConstructFromBinarySave (catalog `vfunc_16`; vtable 0x01239ABC slot 16) | RELOCATION_MATCH 1430v1430, test PASS | recipe of the landed SaveBinary 0x004C84B0 |
| 0x00B665E0 CEngineSkyRenderer::GetEnvironmentMap | RELOCATION_MATCH 1171v1171, test PASS | true length 1171 (`ret 8` at 0x00B66A70; the 1454 estimate ran over separate small functions from 0x00B66A80) |
| 0x00B54890 CEngineWeatherRenderer::RenderRain | parked, DIFFER 1431v1431 | only the two inline push_back C3DVector copies differ (0x00B54C8D..9F, 0x00B54CD3..E4: retail loads X, Y, Z then stores; ours alternates). Pair landing: retail's order appears only with the bodies of ClipBoxToCamera 0x00B51F60, ClipBoxToLandscape 0x00B52DD0 (unlanded, ~4.5 KB) and ClipBoxToWeatherMaskSphere 0x00B52090 visible. Source + passing test in `rows/parked/` |
| 0x00BB1F50 CEngineClothRenderer::CalculateVertexNormals | parked, DIFFER 1508v1508 after 30 grades (three passes); 420 of 425 instructions equal | register choice at 0x00BB1FDB / 0x00BB1FED and the W self-copy / n.X load order at 0x00BB21DF / 0x00BB21FC. Source + passing test in `rows/parked/` |

Tests of the matched rows pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine,
`FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grades, both test runs, read-only vland: 2 wins, no RELOC_IDENTITY; three RELOC_NAME
label warnings, all in NAME_FIXES.tsv). Purity and padded-struct greps are clean on rows and tests.

NOT READY TO LAND: `hdr/engine/CEngineClothRendererSSE.h` (`GF_SSE_ADD_TRIANGLE_NORMAL`, an `__asm` macro with a
FABLE_PORTABLE twin and a dual-path test in `hdr/tests_portable/`) is a proposal used only by the parked cloth row. It has no
ASM_RULES per-site attempts table (30 plain-C++ + 10 intrinsic spellings) yet and must not land without one. The two matched
rows do not include it.

To land the matched rows: `hdr/` (new headers and opt-ins listed in `hdr/README.md`; CEngineWeatherRenderer.h and
CEngineWeatherSettings.h start from c2005's copies; CEngineResourceManager.h and C4DVector.h are c2005's copies unchanged;
re-base on HEAD). Rebuild the oracle from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 11 landed spellings
proven wrong. 0x013BAB0C has two landed spellings (NOTES.md).
