# 2026-11-11_acctB_c2006 -- acctB hand-back, chunk 2006 (4 reserved rows: 3 matched, 1 parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00E621F0 NScript::CV_BodyGuardScript::CBodyGuard::SetHenchmanAsHired(bool) | RELOCATION_MATCH 1340v1340, test PASS | true length 1340 (code + pad + two jump tables); the 1384 estimate ran into the Alloc at 0x00E62730 |
| 0x00D91E00 NScript::CQ_HobbeCaveScript::CHobbeSpawn::Main | RELOCATION_MATCH 1399v1399, test PASS | `// Inline-budget:` line: a folded `&& true` on one terminate check (inline trace 64 -> 74, threshold 74); retail's construct there is unproven -- review at landing |
| 0x00B399A0 CEngineInternalAttachedPrimitive::UpdateOutputPrimitive(float) | RELOCATION_MATCH 1414v1414, test PASS | defines same-TU bodies of RedoMatrixConversion / GetParentTransform / GetChildInverseTransform (landed 0x00B38E30, 0x00B39900): check for duplicate symbols at link |
| 0x00550DC0 NUISystem::CTable::Draw | parked, DIFFER 1399v1399, 20 masked lines | two swapped frame-slot pairs of CRGBFloatColour temporaries (0x00550EFE / 0x00550F53, 0x00550F59 / 0x00550F63); `rows/parked/` has the source and a passing test |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 3 wins, no RELOC_IDENTITY; RELOC_NAME on 0x00E5FC70, landed as GetHenchmanGoldCost,
proven `CBodyGuard::GetGoldCost` -- NAME_FIXES.tsv). Purity and padded-struct greps are clean.

Grade counts: the agent reports one "official" grade per row and did its searching with probe compiles (about 70 on the
parked row), so the per-row counts in STATUS.tsv understate the compiles spent. The parked row has not had three passes.

To land: the rows need `hdr/` (additions only, opt-in; `hdr/README.md`). Merge order: the pending hand-back H40g's
CV_BodyGuardScript.h / CQ_HobbeCaveScript.h blocks first (the copies here are H40g's plus one new `_CHOBBESPAWN_MAIN` block).
`FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY` cannot be combined with chunk 764's `_PREMULTIPLY` opt-in. Rebuild the oracle from
Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 3 landed spellings proven wrong.
