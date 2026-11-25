# Landing nf404 b4: 0x007FB120 is CTCKraken::HeadReadyToStrike

The table's descriptive `?IsTargetAlive@CTCKraken@@QBE_NXZ` was wrong. The real name is `?HeadReadyToStrike@CTCKraken@@QAE_NXZ`:
- FableWin.pdb 0x02778AA0 and Ego_r.pdb 0x007F6450;
- retail's call at 0x00924EDE maps to the donor's HeadReadyToStrike call.

Changes:
- TABLE_CHANGES replaces the line.
- The joined caller 0x00924C70 (`?Update@CAIStateGroup_KrakenAttack@@MAEXXZ`) now calls the real name. Its self is unchanged, so it gets no self line.

Both rows are RELOCATION_MATCH PASS. No source or table uses `IsTargetAlive@CTCKraken` any more; only a history comment mentions it.

Coordinator: 2/2 landed, 0 LITERAL_DIFF, 007fb120 self respelt with column 4 aligned, T3 nf401 0x007FB120 closed, baselines OK.
