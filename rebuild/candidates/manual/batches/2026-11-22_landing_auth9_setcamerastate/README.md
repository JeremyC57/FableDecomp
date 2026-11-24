# Landing auth9 (Account A, 2026-11-22): 0x00B30B50 CEngineCamera::SetCameraState (1 row)

Authoring agent AUTH9. Hand-back: `pending/2026-11-22_auth9/00b30b50/` (NOTES, LOG) and `00b31160/` (P1 patch).

- 0x00B30B50 lands RELOCATION_MATCH (1499 B, no flags), test PASS, under LANDVERIFY_STRICT_HEADERS=1. The oracle bytes equal Fable.exe
  (next bytes `cccccccc`). The text is the copy Codex pass 26 matched inside 0x00B31160's TU. The leaf is in Ego_r.pdb and FableWin.pdb.
  The new row needs no TU-mate copy, and there is no header change.
- 0x00B31160 gets AUTH9's P1, a comment only. Its SetCameraState stays a strong same-TU copy: declaring it measured DIFFER(886v886).
- Recheck (`logs/recheck.log`): all 14 CEngineCamera rows in 00/b2-00/b3 pass (3 MATCH, 11 RELOCATION_MATCH, every test PASS).
- Link (`logs/link_check.txt`): one duplicate, SetCameraState (accepted same-TU kind, GOTCHAS nf96a).
- reloc_bindings: first witness `?SetupGamut@CEngineCamera@@AAEXXZ` -> 0x00B2FD60 (both PDBs). The one recheck warning is RELOC_NAME
  against the Ghidra label `CPlane_InitializeFromCorners` (labels_forge_autoname4.tsv), which is wrong; queued.
