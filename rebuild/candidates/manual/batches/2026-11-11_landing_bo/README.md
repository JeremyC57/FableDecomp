# 2026-11-11 landing bo -- chunk 1026: 4 of 7 (CMusicManager::InternalUpdate, CActionTalkBase::SetUpTextAndSpeech, two libpk audio rows)

Source: `pending/2026-11-11_linux_hand1026/` (STATUS / NOTES / LESSONS / LABELS / NAME_FIXES / SPLITS / BOUNDS_FIX / differ).
vland --land: 4 RELOCATION_MATCH, tests PASS.
- 0x006BD9B0 CMusicManager::InternalUpdate (FableWin + Ego_r; catalog label CMusicManager_CopyAssign dropped).
- 0x00716B30 NTCScriptedControl::CActionTalkBase::SetUpTextAndSpeech (Ego_r, private; `// Retail-asm:` cites the GFFloatToLongCeil fistp
  at 0x00716CA5 / 0x00716D02; defines CActionTalkBase / CSubtitledMessage locally until headers exist -- debt).
- 0x00C400D0 LookupDirectionFilterPair (descriptive, libpk; 256 bytes incl. jump + index tables, BOUNDS_FIX).
- 0x00C43B30 CAudioObject::AttachConfiguredBackend (descriptive, libpk; propagated CShaderConstantRange_ValidateAndLoad dropped).
Headers (base fe0e0079d, hdrcheck2 default=same): CGameCameraManager (SCRIPTEDMODE), CSoundDef (MUSICSETENTRIES), CTCBase
(TALKACTION_FRIEND), CTCTalk (CONVERSATION); new CMusicManager.h.
Parked (differ/): 0x0062FE60 CUIDef::CUIDef 821v817, 0x007C0AD0 NThingShot::CreateShot 936v938 (needs a fable_string.h opt-in
FABLE_RETAIL_CCHARSTRING_TEXT_EQ_RETURN_STRCMP), 0x00802B60 GetTargetingRangeForTarget 410v424.
Queued land1011bo: GetActiveSet (0x006BD760), LookupDirectionFilterPair caller respellings, PlayRandomLipMovement (0x006E2A50, probable).
