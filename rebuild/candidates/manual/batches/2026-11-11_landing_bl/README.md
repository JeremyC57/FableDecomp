# 2026-11-11 landing bl -- chunk 1025: 3 of 8 (three parked, two not attempted)

Source: `pending/2026-11-11_linux_hand1025/` (NOTES, STATUS, LESSONS, LABELS, NAME_FIXES, ATTEMPTS, differ/). vland --land: 3 wins
(RELOCATION_MATCH), tests PASS.
- 0x007BF000 NThingShot::GetCreatureFirePosAndOrientation (Ego_r + FableWin): named bool for the helper-point result; store-then-raise clamp.
- 0x008028D0 `anonymous namespace'::GetTargetingRangeForTargetType (?A0x58ed6966; Ego_r): new engine/CTargetingDef.h, same-TU GetTargetingRange.
- 0x006B1D30 CAtmosProcess::SetAtmosSoundsAsMuted (Ego_r): new engine/CAtmosProcess.h; same-TU by-name GetDef<CSoundThemeDef> body.
Parked (differ/): 0x00802B60 GetTargetingRangeForTarget DIFFER(410v424) (register priority, c2prio def 37 v target 28);
0x007C0AD0 NThingShot::CreateShot DIFFER(936v938) (else-if block order, ebx/ebp swap; same-TU CBasicString<char>::operator== helps);
0x0062FE60 NUISystem::CUIDef::CUIDef DIFFER(829v817) (zero / 1.0f stores of the vector and box members).
Not attempted (agent budget; decode notes in NOTES.md): 0x00716B30 CActionTalkBase::SetUpTextAndSpeech, 0x00BD2D90
GetBoneTransformations (SSE matrix multiply; retail_asm at best).
Headers: base 8f1b0fe48, clean merge, hdrcheck2 default=same; new CTargetingDef.h, CAtmosProcess.h, CThingAICreatureInit.h.
Queued land1011bl: 3 name fixes (GetDef<CSoundThemeDef>, SHOT_CREATION_DUMMY / SHOT_CREATION_POINT).
