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
| 0x0093D270 IsRunnable | terminal (A), header form contradicts the PDBs (oracle matches) | AUTH18 |
| 0x00B4D370 UpdateShadowSpace | open, DIFFER(7162v7176) | AUTH19-AUTH21 |
| 0x00BD3070 RenderMeshPrimitive | open, DIFFER(11288v11288) | AUTH20 (resume point) |
| 0x00502E90 PopulateRegionWithVillagers | open, DIFFER(13587v13622) | not started |
Also: 0x00B30B50 SetCameraState landed (AUTH9, Codex p26 text); AUTH17 default-flag sweep of 18 rows: 0 new matches.
Opus scorecard on the 10: 3 matched by authoring (0x00C73050, 0x007602F0, 0x008E18D0), 1 landed from Codex p26 text (0x00763060),
1 matched only by Codex (0x00A4EFC0), 2 proven terminal, 3 open.

## Fable 5.2 runs
| agent | rows | outcome | grades used | notes |
|---|---|---|---|---|

## Fresh rows for the trial (coordinator pick, 2026-11-22)
The three open rows above are the hardest left, so they alone are a poor sample. AUTH22 also gets five unreserved parked near misses
not worked this week (smallest residue first, excluding rows from the last Codex passes and 0x006AA720): 0x0066B370, 0x00A68DE0,
0x00839420, 0x00651273, 0x00816410. For an Opus comparison on fresh rows, the next Opus authoring run can take the next five on the same
list (0x005A94BD, 0x00409730, 0x00A15890, 0x00CFBDF0, 0x006B97E0).
