# Naming pass wave 2, agent A -- report (2026-10-29)

Base: repo at 001d4f577 (wave 2B landed). All files re-copied from that tree, all scripts re-applied
(`tools/final.py data/all_cand_rows.txt final2`), and the 495 edited rows rechecked on the composed result
(dry run `--recheck-landed`, runs finA + finB): 495/495 MATCH or RELOCATION_MATCH with test PASS. No reverts left.

## Families (rows = landed rows whose source changed; files = sources + tests)
| script | target(s) | rows | files |
|---|---|---|---|
| fam_getframe.py | 0x0049D870 ?GetFrame@CWorld@@SIJXZ | 181 | 279 |
| fam_cbase_ns.py | 0x0099A2D0/2E0/2F0/300 (namespace N_0099a2d0 / N_0099a300 roots) | 130 | 198 |
| fam_cbase_global.py | same four (file-scope `CBase` with virtual dtor, ROLE table) | 84 | 123 |
| fam_ccrc.py | 0x004014A0 ?Calc@CCRC@@SIKKPBXK@Z | 17 | 17 |
| fam_uimanager.py | 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ | 11 | 20 |
| fam_profilemgr.py | 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ | 36 | 44 |
| fam_defmgr.py | 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ | 32 | 36 |
| fam_rwsco_private.py | 0x00993CA0 (local CDataInputStream: public -> private) | 9 | 9 |
Total 495 rows (5 rows carry two families), 721 files. Shared helpers: famlib.py, fam_getter_common.py
(must sit next to the scripts). File lists: fam_<name>.files.txt (repo-relative), rows: fam_<name>.rows.txt.
Apply: `cd <repo>; python3 fam_<name>.py $(cat fam_<name>.files.txt)` for each, any order.
Checked: forward and reverse order give identical trees; a second run edits 0 files; CLI application from the
repo reproduces final2/overlay byte for byte. overlay/ is empty (no hand edits).
