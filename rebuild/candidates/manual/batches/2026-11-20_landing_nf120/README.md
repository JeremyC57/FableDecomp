# 2026-11-20 landing nf120: PRIORITY slice 7, first hand-back (36 rows)

Records: `naming/2026-11-20_agentNF120/` (selfcheck 0 problems). 31 queue lines over 30 addresses: 22 done, 1 done-already (q4269),
1 blocked (q3942: family 0x00742260 / 0x00743220, byte-identical descriptive list dtors no exe reference names; the self row is staged in
the agent's work/), 7 open, all moved to NF124 (q4747, q4802, q6326, q6408, q6453, q6454, q6455). 25 rows settled as not-a-binder.

## Rows (36): 8 MATCH + 28 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 24 self renames (RENAMES s1; 24 files moved, funclet includes 1355 / 0): the CTCLook head/eye family (GetHeadPos, GetTurningAnglesAndCheck-
  ForTurningBody, CentreHead private per both PDBs), the CTCCameraPointDefinitionBase / CTCCameraPointScripted getters, the
  CScriptGameResourceObjectScriptedThingBase script-command virtuals (0x007E7360..0x007E7410), CTCHeroRangedAttacks GetPullBackAmount /
  GetProjectileTargetingZoomParameter, CTCWillLightning::SetLevel, CTCFishing::FinishFishing, CTCCreatureGenerator::OnSerialise,
  CCountedPointer<CCardinalSpline>::Reset, NShape::CTrack::GetTrackPosNearest, CTCUnholyPower::PlaySoundW (nf119's queued fix),
  CAnimFilterOfGroup scalar deleting dtor, CTCCreatureModeManager::RemoveAllModes.
- Three renames outside the entry list (NOTES): 0x007B0CC0 is CTCFishing::CancelButtonPressed (was a generated placeholder; its caller
  0x00690B20 and its test drop the made-up CastButtonPressed); 0x004C7CF0 CThing::OnKill is protected virtual (MAE); 0x00664370
  CThingCreatureBase::OnKill is UAE, the spelling its only referencing row 0x00830D80 already binds. All per Ego_r + FableWin.
- 12 binder respells in place (0x00690B20 + test, 0x00753080, 0x00753C70, 0x007744A0, 0x007A09B0, 0x0087B910, 0x008EA220, five quest-script
  Main rows).

## Headers (add-only opt-ins, default path token-identical; pin_asserts pinned 0)
CTCFishing.h CANCELBUTTONPRESSED; CTCCreatureModeManager.h REMOVEALLMODES; CThingCreatureBase.h ONKILL. assert_names_check: 0 mismatches.

## Checks
bindings_multi_check 1. 830 rows rechecked (gate referencing rows of 32 targets + renamed rows + 563 direct includers of the three
headers): 830 good / 0 bad. No-new-split gate 0 (`logs/split_gate.txt`).
- 0x0099EAE0 (`??1CCharString@@QAE@XZ`, referenced by 5263 landed rows) was left out of the gate recompile, a coordinator decision
  recorded here: the table already holds 58 spellings for it, the landing moves only 0x0073E970 off the stand-in `??1CListMember_0099eae0@@QAE@XZ`
  (no other row or header spells it) onto the canonical spelling other rows already bind, so the set can only shrink (58 -> 57).
- Table spelling sets: 0x0073E6A0, 0x007528A0, 0x00760DA0, 0x00778280, 0x0079F810, 0x007B0950, 0x007E7390, 0x007E73D0 2 -> 1, 0x0075C280
  3 -> 1. Grew: 0x00752200 1 -> 2 is a new table witness, not a created split: 0x00753080 bound `?ChangeEyeAngles@CTCLook@@QAEXMM@Z` before
  and after (HEAD and landing pairs), while 0x00753040 binds the AAE form (PDB: private). 0x0074DE50 0 -> 1 is a new witness.
- NAME_FIXES queued: 0x00751820 GetHeadSettingsSlowInFastOut (existing split), 0x00751E80 GetTurningAngles AAE, 0x00752190
  ChangeHeadAngleYZ AAE, 0x0076C850 CTCEnemy::RemoveAllyThing; plus 0x00752200 ChangeEyeAngles (0x00753080 binds QAE, PDB AAE).
