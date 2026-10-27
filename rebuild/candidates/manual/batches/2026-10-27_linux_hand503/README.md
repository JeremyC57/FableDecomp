# Chunk 503 (agent503, 2026-10-27): 26 of 30 single functions byte-exact (128-256 bytes)

First queued chunk of single functions of 128-256 bytes (30 rows, address order, 0x004BC670..0x00A4AC60). One Opus 5.5
agent at high effort.

- **Landed:** 26 rows with `vland.py --land` (verify.log: 1 MATCH, 25 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). 0x00A49E00 and 0x00A4AC60 carry /Gr.
- **Coordinator fix:** to clear a RELOC_CONFLICT the agent had renamed calls to 0x00487DC0 in 0x006299C0, 0x006360A0 and
  0x006369B0 to `CPlayer::GetPPlayerCharacter` (a FableWin name). The Ego_r donor order puts GetPControlledCreature and
  PeekPControlledCreature 0x10 apart in that order, matching retail 0x00487DC0 / 0x00487DD0, and Peek returns a const
  pointer; family pass 6 and chunks 487-488 bind 0x00487DC0 as GetPControlledCreature. The three sources were switched
  back (same bytes, regraded RELOCATION_MATCH). The 3 RELOC_CONFLICT warnings are against landed 0x00896EE0, which binds
  GetPControlledCreature to 0x00487DD0 and is the row to fix.
- **Parked (2, pending/2026-10-27_linux_hand503/differ/):** 0x006BB860 CGameTimeManager::ConsoleSetTimeOfDay (173v181,
  x87 under /Op: retail reloads Time from [ecx+8], ours forwards the parameter slot), 0x00917E30
  CAIStateGroup_Clean::GetBench (140v134, frame only: `sub esp,8` vs retail's single `push ecx` slot).
- **Skipped (2):** 0x00A0F6EA and 0x00A0F98B are branch targets inside the deque insert at 0x00A0F4B0, not functions
  (they read esi/edi/ebx/ebp without setting them); drop them from the catalog.
- **Identities:** 24 proven (22 landed rows in ghidra_out/labels_chunk503_2026-10-27.tsv), 4 probable.

Label corrections (NOTES.md): landed 0x006359C0 / 0x00633670 are InitModeYesNoQuestion / UninitModeYesNoQuestion (mode
15), not ClickPastText; 0x008E97B0 is CModeStrafe::LockStrafingCombinationAnimsToMainAnim (bound
ResetStrafeAnims_008e97b0); the landed ability-component stub pairs 0x007A7BD0/BE0, 0x007C4410/420, 0x007C9160/170,
0x007CCE00/E10 are OnKill / OnRemove entries that jump to FinalCleanup; 0x00A49BC0 / 0x00A49C10 / 0x00A4A2A0 are
__uninitialized_copy / __uninitialized_fill_n / __destroy_aux over CLipSyncPhonemeSample; 0x004C3200 is
C3DMeshObject::~C3DMeshObject (no binding); 0x007F21C0 CMockSpline::GetPositionAtIndex; 0x0099E480 ToWideString,
0x00487150 EndSpecialMoveMode, 0x00A24270 GetMeshInfo. Review: 0x0099A140's key copies one byte (char) where the landed
family header says long; 0x0053B717 reuses the landed CList(long) vtable-install device.
