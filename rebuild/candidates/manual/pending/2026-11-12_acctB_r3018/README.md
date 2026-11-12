# acctB hand-back, row 0x007D6870 CTCTavernGameCardPairs::ProcessInput: matched (was parked at about 540 / 571)

| address | result | notes |
|---|---|---|
| 0x007D6870 `?ProcessInput@CTCTavernGameCardPairs@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z` | RELOCATION_MATCH 1778v1778, test PASS (g++ and VC7.1) | vtable 0x0126F6FC slot 28; catalog name `vfunc_28`. 536 full-row compiles over two sessions, about 140 of them diagnostics (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win, no RELOC_CONFLICT / RELOC_IDENTITY / NOT_GENUINE; three `WARN RELOC_NAME` lines (below).

## What closed it

- **Session 1 (408 to 566 equal), proven by the bytes:** `GetRight` as the default `Forward ^ Up` (same size as
  FableWin; the earlier text was 43 instructions against 15); the movement block inside the switch with MAIN_GAME
  falling through into the TURNING cases (retail's 0x38 frame); `if (aligned) break; if (dot > 0.0f) return;`; two
  named references for the second and the final dot.
- **Session 2 (the last 5 instructions, the final dot's term order at 0x007D6ED1):** the order follows hash keys
  over compiler record ids, so the function needed more intermediate code. Closed by writing the same logic step by
  step with named intermediate values (the list is in `NOTES.md`). Each piece was graded alone first to confirm it
  changes no code.

## Review items

- **FITTED: the set of named intermediates.** Retail's bytes prove how much intermediate code the function has, not
  which names the original used. The match sits in a one-unit window: one more dead local flips 0x007D6C22. Any
  later change to the `C3DVector`, `C2DVector`, `CRightHandedSet` or `CDefPointer` inline bodies needs this row
  regraded.
- **vland RELOC_NAME** (landed labels differ from the spellings bound here): 0x00A0D390
  `CProcessedInput::SetAsPerformedEvent` (label `NScript::CV_SickChildScript::CWomanToAttract::Init`), 0x00687D00
  `CInputProcessGameBase::GetControlMovementFromGameActionEvent` (label `GetEventGameActionVector`), 0x0129BA3C
  `DELTA_NOTIONAL_ZERO` (label `EPSILON`). Evidence in `NOTES.md` / `NAME_FIXES.tsv`.
- **Made-up name:** `UseCardBaseInput` 0x01383C3F stays descriptive; locals are descriptive.
- **Headers:** the chunk 2016 overlay unchanged.
