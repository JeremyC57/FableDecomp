# Landing (2026-11-23): TavernKeeper 0x009687B0 / 0x00968860 onto their PDB names

Owner (2026-11-23): "If you can figure out what the function does, then name it based on how the original devs name other functions and
what it does." Both turned out to have real PDB names, so nothing is made up:

| address | was | now (Ego_r.pdb + FableWin.pdb) |
|---|---|---|
| 0x009687B0 | `CAIStateGroup_TavernServer::IsCarryingJug` (the TavernServer twin's name, so one name at two addresses) | `?IsHoldingJug@CAIStateGroup_TavernKeeper@@QAE_NXZ` |
| 0x00968860 | `CAIStateGroup_TavernKeeper::GetOldJugPoint` (descriptive, the TavernServer twin's name) | `?GetPaymentPoint@CAIStateGroup_TavernKeeper@@QAE_NXZ` |

Evidence: 16 of TavernKeeper's 18 ordinary PDB members already map to other retail rows, leaving exactly these two names for these two
addresses; signatures agree (`QAE_NXZ`); Ego_r sizes match the TavernServer twins (IsHoldingJug 193 = TavernServer::IsCarryingJug 193;
GetPaymentPoint 245 v TavernServer::GetOldJugPoint 249; retail 172 v 174 and 229 v 233); IsHoldingJug's only caller is TavernKeeper::OnReplace
0x00968BC0, and Update 0x00969470 calls 0x00968860 between GetPaymentRequest and ReceivePayment. Labels: in PATCH.diff
(`ghidra_out/labels_landing_2026-11-23_tavernkeeper.tsv`).

Changed: the two self rows and their tests (renamed), the callers OnReplace 0x00968BC0 and Update 0x00969470, build_candidates.ps1,
reloc_bindings.tsv (2 lines respelled), auto-re-candidates.tsv (2 names), `ghidra_out/labels_landing_2026-11-23_tavernkeeper.tsv`.

## Recheck
`logs/recheck.log` (landed tree, rebuilt lane after the 2026-11-23 container restart): 009687b0 / 00968860 / 00968bc0 / 00969470
4 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT. `?IsCarryingJug@CAIStateGroup_TavernServer@@QAE_NXZ` now names one address (0x00966CF0).
