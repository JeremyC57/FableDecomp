# 2026-11-11_acctB_c2004 -- acctB hand-back, chunk 2004 (4 reserved rows, all matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x009609B0 CAIStateGroup_DeedReactionRoot::IsRunnable | RELOCATION_MATCH 1352v1352, test PASS | reaches the existing shared `engine/GFFloatToLongCeil.h` helper (retail inline ceiling blocks at 0x00960A31 / 0x00960AA3), so grade `retail_asm`; no asm in the row. REVIEW: the row cites the helper's own proof and carries no new per-site table of C++ attempts (ASM_RULES rule 4) |
| 0x00912BA0 CAIStateGroup_FlyingThingBase::CalculatePrettyPath | RELOCATION_MATCH 1314v1314, test PASS | STLport `_M_push_back_aux_v` body visible but out of line (new overlay opt-in) |
| 0x009FED30 CGraphicDataBank::LoadGraphicFrame | RELOCATION_MATCH 1206v1206, test PASS | true length 1206 (reservation estimated 1264): `ret 20h` at 0x009FF1E3, int3 padding, then a separate 48-byte function at 0x009FF1F0 |
| 0x00725500 CTCPhysicsControlled::PerformImpulseBasedMovement | RELOCATION_MATCH 1324v1324, test PASS | 10 graded variants + 1 confirmation grade; one unusual spelling (`0.0f - normal.Dot(C3DVector(0,0,1)) < 0.125f`) noted in LESSONS.md |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 4 wins, no RELOC_IDENTITY). Two non-blocking warnings: RELOC_CONFLICT on
`CTCPerceiveThing::GetExtendedSightRadius` (0x00744950 here; landed 0x00864620 binds that name to 0x007448F0, which the agent
proves is GetSightRadius -- NAME_FIXES.tsv), and RELOC_NAME on 0x0129BA3C (`DELTA_NOTIONAL_ZERO`, labelled `EPSILON`).
Purity and padded-struct greps are clean on rows and tests.

To land: the rows need `hdr/` (seven new class headers; opt-in blocks added to 16 copied repo headers, default paths
unchanged; `hdr/README.md`). Re-copy the HEAD headers and re-apply the blocks if the repo copies moved. `payload.json` holds
the four rows; rebuild the oracle from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 10 landed spellings
proven wrong.
