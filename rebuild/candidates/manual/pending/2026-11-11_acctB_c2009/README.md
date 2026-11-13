# 2026-11-11_acctB_c2009 -- acctB hand-back, chunk 2009 (4 reserved rows: 2 matched, 2 not attempted)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00745120 CTCPerceiveThing::UpdateLineOfSight | RELOCATION_MATCH 1538v1538, test PASS | 5 grades |
| 0x00952890 CAIStateGroup_JackDragon::UpdateGround | RELOCATION_MATCH 1509v1509, test PASS | 6 grades |
| 0x0097FEA0 CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp | not attempted (STATUS.tsv says SKIP) | the agent ran out of working budget; re-queued by acctB as chunk 2015; outline in NOTES.md |
| 0x007A0D90 CTCWillLightning::GetTargets | not attempted (STATUS.tsv says SKIP) | same; re-queued as chunk 2015 |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 2 wins, no RELOC_IDENTITY). Warnings: RELOC_CONFLICT on
`CTCPerceiveThing::GetExtendedSightRadius` (landed 0x00864620 binds it to 0x007448F0 = GetSightRadius; already filed by
c2004) and four label-only RELOC_NAME (0x00437900, 0x00733190, 0x00686D90, 0x009531A0; NAME_FIXES.tsv). Purity and
padded-struct greps are clean.

To land: the rows need `hdr/` (3 new headers; opt-in blocks in copies of 9 repo headers, default paths unchanged;
`hdr/README.md`; re-copy HEAD and re-apply). Both row objects also carry same-TU member COMDATs (GetFOV, three setters,
IsStillRunnable, GetSubDef<CJackDragonDef>, reserve / search bodies); only the leaf is graded. Rebuild the oracle from
Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 11 entries.
