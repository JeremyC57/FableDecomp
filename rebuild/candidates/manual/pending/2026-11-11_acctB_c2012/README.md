# 2026-11-11_acctB_c2012 -- acctB hand-back, chunk 2012 (4 reserved rows: 1 matched, 1 parked, 2 not attempted)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x006DE280 CTCTrophy::TerminateMinigame | RELOCATION_MATCH 1581v1581, test PASS (g++ and VC7.1) | `// Flags: /d2inlT77` + `// Inline-budget:` line: with the witness loop behind `if (it != end)` the inline trace scores are emote 77, hero-stats 74, PlaySoundW 76; T77 expands exactly the three sites retail expands -- review at landing. Defines the landed CTCTrophy::PlaySoundW (0x006DDCD0) body in the TU, as c2007 did |
| 0x006D1250 CTCWife::CheckAppearanceChanges | parked, DIFFER 1629v1608 after one pass (6 grades) | `this` / `hero` in swapped registers from the end of the modifier block on (retail 0x006D15ED / 0x006D15F1); `rows/parked/`. Re-queued by acctB as chunk 2020 for its remaining passes |
| 0x00949AB0 CAIStateGroup_GoToPub::IsRunnable | not attempted | outline in NOTES.md; re-queued as chunk 2020 |
| 0x006414A0 CTCLight::DrawLight | not attempted | outline in NOTES.md; re-queued as chunk 2020 |

Coordinator re-verification of the matched row (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade,
both test runs, read-only vland: 1 win, no warnings). Purity and padded-struct greps are clean.

To land: the row depends on c2007's headers (CTCTrophy.h, CTrophyDef.h, CTCCarryable.h, CThingAICreature.h; `hdr/` here is
c2007's overlay unchanged plus new headers / opt-ins, `hdr/README.md`): land with or after c2007. Rebuild the oracle from
Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 0x00682310 and 0x009C7BA0. Open naming question (NOTES.md):
0x00616580 and 0x00B2EB20 are byte-identical set<long> finds and only 0x00B2EB20 is bound.
