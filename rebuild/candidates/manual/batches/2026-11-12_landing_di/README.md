# 2026-11-12 landing di -- chunk Q19: 5 of Q18's 14 parked rows matched

Source: `pending/2026-11-12_linux_handQ19/` (README, STATUS, NOTES, LESSONS, LABELS, NAME_FIXES, ATTEMPTS, differ/, hdr README).
The oracle was rebuilt from the re-downloaded Fable.exe (SHA-1 39fd416b...) with chunk_pool/oracle_from_exe.py.

Landed (RELOCATION_MATCH, test PASS): 0x0065C5BB UpdateMirroredCrimeData, 0x0079B6F0 ResetMinionPos, 0x008C4FD0 CreateBabies,
0x008CDD40 ReleaseTimeSpell::FinishAction, 0x008E16A0 CModeScreamerSuperAttack::FrameUpdate.

Coordinator fixes at landing:
- The agent's oracle for four rows carried retail's trailing int3 padding (736 / 432 / 560 bytes). These graded DIFFER(732v736) and
  similar under vland, which keeps padding although q.py strips it. The oracle was re-cut without the padding (`logs/grade2.log`).
- 0x0079B6F0's test passed under g++ but FAILed under VC7.1: `Physics p = Physics();` relies on C++03 value-initialisation, which
  VC7.1 lacks, so SetCount started undefined. It now sets the field explicitly; the source is unchanged.

Headers (opt-ins only; hdrcheck2 default=same for every changed header; the existing UNSHEATHEHASTE opt-in is token-identical apart
from a line split): C3DVector.h (NEWTON_SQUARE_PAREN, NEWTON_STEP_PAREN), CVectorMap.h (INSERT_FORCEINLINE), CTCVillage.h
(MIRROREDCRIMES), CTCSummonSpell.h, CCreatureAction_ScorpionSummon.h, CCreatureAction_ReleaseTimeSpell.h,
CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h (CTOR), CGameCameraManager.h (RELEASESCRIPTEDCONTROL); new
CCreatureAction_RiseUp.h and NCreatureMode_CModeScreamerSuperAttack.h.

Checks: bindings_multi_check at baseline (NullCoord only); funclet_includes_check 1317 checked, 0 broken.
Queued (land1011di): 0x0079C740 is CTCBulletTime::GetTimeMultiplier (landed `_unattributed::GetFloatThrough_...`); 0x004FB150 is
CWorldMap::GetLoadedRegion (24 landed local models spell it GetCurrentRegionNumber).
