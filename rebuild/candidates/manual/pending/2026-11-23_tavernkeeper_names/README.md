# Pending (2026-11-23): TavernKeeper 0x009687B0 / 0x00968860 onto their PDB names, waiting on the lane

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

`PATCH.diff` (apply with `git apply --index`): the two self rows and their tests (renamed), the callers OnReplace 0x00968BC0 and Update
0x00969470, build_candidates.ps1, reloc_bindings.tsv, the labels file. Also rename the two rows' name column in
`rebuild/oracles/auto-re-candidates.tsv` (`CAIStateGroup_TavernServer::IsCarryingJug` -> `CAIStateGroup_TavernKeeper::IsHoldingJug` for
009687b0; `CAIStateGroup_TavernKeeper::GetOldJugPoint` -> `CAIStateGroup_TavernKeeper::GetPaymentPoint` for 00968860); that file carries
retail bytes, so its hunk is not in the patch.

Not landed yet: the container restart of 2026-11-23 wiped the lane. To land, rebuild the lane, objsec HEAD v new for the four rows (only the
two symbol names change), recheck 009687b0 00968860 00968bc0 00969470 through vland, then commit as a landing.
