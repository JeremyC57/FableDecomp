# Authoring model trial (owner, 2026-11-22): "promote hand author to fable 5.2 to see if match rate improves"

From AUTH22 on, the hand-authoring agent runs on Fable 5.2 (high effort) instead of Opus 5.5 (high). Same briefs, same lane, same rules,
same advisor route. Naming agents (NF*) stay on Opus. Score each authoring agent run here when it hands back.

## Opus 5.5 baseline: the ten released AUTH1 rows (AUTH1-AUTH21, 2026-11-21 / 22)
All ten are rows earlier rounds (acctB / acctC) had parked as hard near misses.

| row | outcome | agent(s) |
|---|---|---|
| 0x00C73050 CAudioSource::UpdateStream | landed RELOCATION_MATCH | AUTH3 |
| 0x007602F0 NShape::IsShapeConvex | landed RELOCATION_MATCH | AUTH5 + AUTH6 |
| 0x00763060 FractureShape | landed (from Codex p26 text) | AUTH9 |
| 0x008E18D0 UpdateLightningBeams | landed RELOCATION_MATCH | AUTH9-AUTH15 |
| 0x00A4EFC0 InitialiseFromFileData | not matched by authoring (escalated); Codex matched it, landed as Inferred | AUTH4 / AUTH7 / AUTH11 |
| 0x00CF4EC0 CFanMarker::Main | terminal (A), proven inline tie | AUTH10-AUTH16 |
| 0x0093D270 IsRunnable | terminal (A) (oracle matched); landed as Inferred by owner override 2026-11-22 | AUTH18 |
| 0x00B4D370 UpdateShadowSpace | open, DIFFER(7162v7176); residue = x87 operand order + a 10-B loop pad it costs; waits on C2T1 | AUTH19-AUTH21 |
| 0x00BD3070 RenderMeshPrimitive | escalated, DIFFER(11288v11288): 13 operand-order picks after all 14 forceinlines moved to headers; waits on C2T1 | AUTH20, AUTH21 |
| 0x00502E90 PopulateRegionWithVillagers | open, DIFFER(13587v13622) | not started |
Also: 0x00B30B50 SetCameraState landed (AUTH9, Codex p26 text); AUTH17 default-flag sweep of 18 rows: 0 new matches.
Opus scorecard on the 10: 3 matched by authoring (0x00C73050, 0x007602F0, 0x008E18D0), 1 landed from Codex p26 text (0x00763060),
1 matched only by Codex (0x00A4EFC0), 2 proven terminal, 3 open.

## Fresh rows for the trial (coordinator pick, 2026-11-22)
The three open rows above are the hardest left, so they alone are a poor sample. AUTH22 also gets five unreserved parked near misses
not worked this week (smallest residue first, excluding rows from the last Codex passes and 0x006AA720): 0x0066B370, 0x00A68DE0,
0x00839420, 0x00651273, 0x00816410. For an Opus comparison on fresh rows, the next Opus authoring run can take the next five on the same
list (0x005A94BD, 0x00409730, 0x00A15890, 0x00CFBDF0, 0x006B97E0).

## Fable 5.2 runs
| agent | rows | outcome | grades used | notes |
|---|---|---|---|---|
| AUTH22 | 0x00502E90 | escalated, DIFFER(13606v13622) (record was 13587v13622 with nine inline_depth devices; device-free 13971) | 5 | Fable 5.2 high; advisor: not terminal |
| AUTH22 | 0x0066B370 | **matched (RELOCATION_MATCH), pending owner**: same mechanism as Opus's 0x0093D270 oracle (by-value float accessor read like an lvalue; text departs from Ego_d line 208); one pick-any copy holds it | 17 | not counted as a landed match until the owner rules |
| AUTH22 | 0x00A68DE0, 0x00839420, 0x00651273, 0x00816410 | not started (baseline regrades only) | 3 | context ran out on rows 1-2 |
| AUTH23 | 0x0066B370 checks | `const float&` accessor oracle RELOCATION_MATCH (one form covers 0x0093D270 too); `friend` not attested (0 friend records in any PDB); row-tail pragma rejected (re-modes every template copy) | 3 | owner: land as Inferred once the GetSubDef copy is solved |
| AUTH23 | 0x00A68DE0 | escalated, DIFFER(923v919) after the Ego_d port (from 939): retail keeps a return-slot local (`locked_box`) live across calls, which VC7.1 kills at every real call in every spelling tried | 4 | Fable 5.2; the old "cl hang" was a missing include |
| AUTH23 | 0x0066B370 | **landed** RELOCATION_MATCH as Inferred (owner), pick-any device per advisor | 4 (+17 AUTH22) | `batches/2026-11-22_landing_auth23_playallsoundevents/` |
| AUTH23 | 0x00A68DE0 | escalated, still moving: DIFFER(911v919) (same-TU Lock body as the landed 2D twin moved it) | 6 | |
| AUTH23 | 0x00839420 | working: DIFFER(1316v1308), 29 masked lines (from 75) | 3 | HANDOFF.md |
| AUTH23 | 0x00651273, 0x00816410, 0x00502E90 | not started | 0 | context |
| AUTH24 | 0x00839420 | **landed** RELOCATION_MATCH (Inferred: GFDebugPDynamicCast release dead-arm assert, Ego_d line 129 = 13 B) | 6 (+3 AUTH23) | `batches/2026-11-22_landing_auth24_createeffectonhit/` |
| AUTH24 | 0x00651273, 0x00816410, 0x00A68DE0, 0x00502E90 | not started | 0 | context |

**Running score (fresh rows, Fable 5.2):** of the five fresh near misses, 2 landed (0x0066B370 with an owner-approved Inferred accessor
form; 0x00839420 with an Inferred assert arm), 1 escalated (0x00A68DE0), 2 not started; 0x00502E90 (old hard row) escalated. Opus on its 10 hard rows:
3 authored matches (+1 from Codex text, +1 Inferred by owner override). Not yet comparable: different rows; run the Opus control set.
