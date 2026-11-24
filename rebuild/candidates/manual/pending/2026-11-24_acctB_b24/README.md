# 2026-11-24 acctB B24 hand-backs (staged, NOT landed)

Agent B24C's four matched rows, re-verified by the B coordinator with read-only `vland.py` (strict headers, each row's own overlay in
`hdr/<addr>/`; logs in `coordverify/`). No oracle bytes here. Each row still has open checks before landing:

| address | function | coordinator re-verify | open before landing |
|---|---|---|---|
| 0x012133C0 | EH unwind funclet of CPhysicalPlayer::Play 0x00C271F0 | RELOCATION_MATCH PASS, no warnings | its 18-byte extent covers the parent's `__ehhandler` thunk at 0x012133C8; other Unwind rows stop before it: follow their precedent (probably an 8-byte row) |
| 0x00407A70 | CUserProfileManager::SetAssignedInputKeyboard | stock vland OUTSIDE_MANIFEST (agent: PASS with the start added in memory) | add manifest starts 0x00407A70 / 0x00407BA0 by the project's route; check the 0x004075C0 extent; then stock strict vland |
| 0x00407BA0 | CUserProfileManager::SetAssignedInputMouse | same | same |
| 0x00402510 | GFMain | RELOCATION_MATCH PASS, 5 warnings (4 RELOC_NAME on manifest labels the agent proves wrong, 1 expected RELOC_ALIAS `_exit`) | data globals spelled differently from landed rows (switch to the landed spellings, leave the proofs in NAME_FIXES.tsv for Account A); `Body_38[8]` placeholder in a new CSystemManagerInit.h; twelve `_GFMAIN` opt-ins and six new headers to check against the merged headers; `/Gr /GS` flags; test must be able to fail |

Agent notes, escalations and name fixes are in `notes/<addr>/`.
