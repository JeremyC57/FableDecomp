# 2026-11-11_acctB_c2015 -- acctB hand-back, chunk 2015 (the 2 rows chunk 2009 did not reach; both matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x0097FEA0 CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp | RELOCATION_MATCH 1533v1533, test PASS | plain C++, 7 grades |
| 0x007A0D90 CTCWillLightning::GetTargets | RELOCATION_MATCH 1517v1517, test PASS | 7 grades. `// Inline-budget:` line: `#pragma inline_depth(0)` around two statements of the new-beam branch (retail calls GFRandom and the counted-pointer push_back there but expands them elsewhere; the inline trace shows no single `/d2inlT<N>` fits: N > 61 and N <= 47) -- review at landing |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 2 wins, no RELOC_IDENTITY; three label-only RELOC_NAME warnings on 0x007A3CA0,
0x007A3200, 0x006691C0, all in NAME_FIXES.tsv). Purity and padded-struct greps are clean.

To land: the rows need `hdr/` (6 new headers; opt-ins in 8 copied repo headers; `hdr/README.md`). Header debt:
`NEntityEvents::CEventBase` is defined in the new NEntityEvents_CEventExpressionPerformedTo.h (slots 0..4) and, smaller, in
the repo's NEntityEvents_CEventHitBase.h: merge before any TU includes both. Rebuild the oracle from Fable.exe (no retail
bytes are in this folder). NAME_FIXES.tsv: 12 entries.
