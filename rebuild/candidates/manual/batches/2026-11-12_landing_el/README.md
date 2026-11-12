# 2026-11-12 landing el -- name-fix pass NF28: 44 queue entries, 80 landed rows changed, 18 renamed

Source: `naming/2026-11-12_agentNF28/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README). Run as the workflow
`lane_tools/workflows/name_fix_queue_pass.js`: one Opus agent applied the queue (tags land1011dt .. land1011ei), an independent Opus
verifier attacked the hand-back twice, with two repair passes between and after.

**Queue:** 34 done, 7 partial, 3 rejected with evidence (0x004460C0: offsets prove the landed name; 0x00463F3C: the claimed mangling is
in neither PDB; 0x00451ADD: folded tree constructor, element type not provable).
**Rows:** 80 landed rows changed, 18 of them renames of the row's own name (RENAMES_applied.tsv section 1), 70 caller respellings
(section 2). All 80 rechecked in this landing: 79 RELOCATION_MATCH + 1 MATCH, every test PASS (`logs/recheck.log`).
**Verifier findings and repairs:** round 1 -- two new local models of header classes (0x00A2F190, 0x009B4BD0) and one false evidence
line (0x0068EAF0): all three moved onto shared headers. Round 2 -- BLOCKER: 0x004083B0 had been renamed SetDefaultValuesGameplay,
which is 0x00408360; it is SetDefaultValuesAudio (audio volumes, Ego_r 0x00406350 / FableWin 0x01881DC0, shared header, binding table)
and now sits on engine/CUserProfileManager.h with a new test; the 0x00688A60 test lost its new local CInputProcessManager model.
**Headers** (6, additions only, defaults unchanged per hdrcheck2): CUserProfileManager.h (AUDIODEFAULTS), CDefinitionManager.h
(CINSTANTIATEDEFINFO_DECL), CGameCameraManager.h (GETRENDERCAMERA), CTCPhysicsBase.h, CInputProcessTargetLockRightStickTargetSelect.h,
CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h.
**Bindings:** rebind.py removed 139 and added 129 rows for the 80 rechecked rows; bindings_multi_check at baseline (the NullCoord
line only); funclet_includes_check 1317 checked, 0 broken.
**Carried over (queue tag land1011el):** the partial entries -- 0x01436EA8 g_pLandscapeRenderer (20 rows respelled, about 33 left),
0x0129BA40 GD3DFormats (9 respelled, about 15 rows with other pixel-format spellings left), 0x00669D30 GetEffectFromString (seven
stand-in binders), 0x00AE9450 (two callers with local CEmitterPointer models), 0x00A0D2B0 (self row on a stand-in), 0x007F70D0 (claim
proven, self rename DIFFER; `parked/007f70d0.cpp`), 0x00BE0810 (no caller TU) -- plus the verifier's notes: 0x009BEBB0's Leaf line
says ABE where the compiled symbol is QBE; 0x00408340 SetSoundAsMuted is mangled (char) where both PDBs have bool and still uses a
local CUserProfileManager model; 0x00406610 returns a local CWS stand-in instead of CWideString.
