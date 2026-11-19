# 2026-11-19 landing nf106: naming round 10, GetAllThingMapsInArea ports (16 rows)

Records: `naming/2026-11-19_agentNF106/` (selfcheck 0 problems). q11280 / q11283 (0x0049C6F0) blocked: port with 40 rows left
(`q11280_rows_left.txt`; NOTES says what each group needs: the 0x0074D710 vector define set first for 0x00969FB0 / 0x0095DB90, a fitted
GetPFirst / GetPBest body opt-in, a StatGetTypeFlags opt-in); q11281 open. No header changed.

## Rows (16): explicit instantiations of the engine/CThingSearchTools.h templates on a landed twin's define set
Vector instances on the 0x0095C7C0 set (0x007197C0, 0x006B2720, 0x0074D710, 0x008AD710); `std::list` instances on the 0x0085B2A0 /
0x006FE7C0 set (0x0079BF30, 0x007C3C20, 0x0078DB20, 0x00785DD0, 0x0091A750, 0x00792E50, 0x007930B0, 0x007A5190, 0x007A63D0, 0x0091ADB0,
0x0080E8E0); 0x0091BA40 GetAllTCsInRoughArea<CTCPerceiveThing, ..., list> on the 0x00518340 set (Ego_r.pdb + FableWin.pdb). 14 self-leaf
renames (13 QAE -> the PDB const QBE; 0x0091BA40 off the `_Calls` stand-in, file moves) and 32 callee respells on 0x0049C6F0 / 0x0049C700.

## Checks
16 rows RELOCATION_MATCH, every test PASS (`logs/recheck.log`); bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0.
Exe-scan gate: 88 other landed rows reference the 16 targets, all recheck good, 0 created splits (`logs/split_gate.txt`).
