# chunk 1026 (agent1026) -- notes

All grades ran with `export FABLE_HDR_OVERLAY=$A/hdr` (tools/gg.sh exports it; gall.sh must be run with it exported too -- 00716b30
is SRC_FAIL without the overlay). 0x00C400D0 grades with `QLEN=256` (SPLITS.tsv: the 528-byte catalog extent holds the next function
0x00C401D0). Grader: $A/tools/q.py = $S/tools/q.py plus the QLEN cut. c2prio probes are logged in ATTEMPTS.log as "(probe)" and are not grades.

## Matched (4)
| row | identity | grade | lever |
|---|---|---|---|
| 0x006BD9B0 | ?InternalUpdate@CMusicManager@@IAEXABVCState@1@_N@Z (FableWin + Ego_r name set; the only CMusicManager member taking (const CState&, bool); its 8 callers are the landed CState setters 0x006BDE50.. passing a modified copy of CurrentState) | RELOCATION_MATCH g4 | inline `CState::HasAnyOverride()` (retail COMDAT 0x006BD7E0) instead of an open-coded `\|\|` |
| 0x00C400D0 | LookupDirectionFilterPair (descriptive, libpk; landed caller 0x00C6D150 spelling) | RELOCATION_MATCH g4 (QLEN=256) | named `index` + `&filters[index]` per arm; switch 1 / 4 / 8,64 with the azimuth fold falling into case 4 |
| 0x00C43B30 | CAudioObject::AttachConfiguredBackend (descriptive, landed callers' spelling) | RELOCATION_MATCH g3 | manager local after the useSoftware test, one function-scope voice, per-block result, software error last |
| 0x00716B30 | ?SetUpTextAndSpeech@CActionTalkBase@NTCScriptedControl@@AAEXXZ (Ego_r donor 0x0072AFB0; private) | RELOCATION_MATCH g5 | `double gesture_delay` spelling; inline GetNarrator(); Retail-asm line for the two GFFloatToLongCeil expansions (0x00716CA5 / 0x00716D02 `fistp dword ptr [ebp-4]`) |

Callees named in the rows: CMusicManager::PlayMusic 0x006BD900, CState::GetActiveSet 0x006BD760 (inline body), GetDanger 0x006BD780,
CASoundManager::StopMusic via PSoundManager; LookupAzimuthElevation (landed callee spelling); voice manager ApplySlotA 0x00C4D440 /
ApplySlotB 0x00C4D4B0 / AcquireVoiceA 0x00C4D520 / AcquireVoiceB 0x00C4D5F0 / Release 0x00C4D070 / slot 2 AcquireStreamVoice
(landed descriptive spellings); CTCTalk::PlaySpeech 0x006E3580, PlayRandomLipMovement 0x006E2A50 (probable, NAME_FIXES), 
GetCurrentSpeechDuration 0x006E2840, CTCBase::GetWorld 0x00686A70, CGameCameraManager::IsUsingScriptedCameraMode 0x00697AE0,
CActionTalkBase::GetNextCameraTagIndex 0x00712E40 / SetUpCamera 0x00715A70, GFFloatRandom 0x004978A0, GFGetTime 0x009E1BC0.
Descriptive data names: MusicFadeTimes 0x0137FAE8; SpeakerUniqueID 0x013BAD08, ListenerUniqueID 0x013BAD10, CurrentCameraTagIndex
0x01380C34, ConversationCameraSide 0x013BAD00 (DefaultCameraTags 0x013BAD18 is the Ego_r name).

## Header debt
- engine/CMusicManager.h is new (overlay). Land it with the row; the landed setters 0x006BDE50.. can then use it instead of local models.
- 00716b30 still models NTCScriptedControl::CActionTalkBase (derives CTCScriptedControl::CActionBase; virtual slot 6 DisplayText; offsets
  from Ego_r) and CSubtitledMessage (0x44 bytes) locally: they need engine/ headers (NTCScriptedControl_CActionTalkBase.h,
  CSubtitledMessage.h) generated from struct_layouts_egor.tsv.
- Overlay opt-ins (hdr/README.md): CSOUNDDEF_MUSICSETENTRIES, CTCTALK_CONVERSATION, CTCBASE_TALKACTION_FRIEND,
  CGAMECAMERAMANAGER_SCRIPTEDMODE, CMUSICMANAGER_OVERRIDE_TESTS.

## Parked (3)
- 0x0062FE60 NUISystem::CUIDef::CUIDef -- DIFFER(821v817), 6 grades (3 prior + 3 here) + c2prio. g6 wrote PositionOffset member-wise
  (829 -> 821). Left: retail copies each vector's zero through one register (edx for TextWindow/Up/Forward/RotationAxis, ecx/edx shared
  by Min/Max/Step/Dimensions); ours alternates eax/edx. c2prio shows these copies are not allocator candidates (the 0.0f is
  `_real@00000000`, 19 x87 refs, turned into integer moves after colouring), so next: find the spelling of the C2DBoxF / C3DVector /
  C2DVector ctors that gives one value per vector (e.g. an engine ctor taking one zero, or a retail `Zero` helper) -- check EgoCore /
  Ego_r for C2DBoxF / C3DVector ctor overloads first.
- 0x007C0AD0 NThingShot::CreateShot -- DIFFER(936v938), 5 grades (3 prior + 2 here). (1) OBJECT/CREATURE block order comes from the
  expanded CCharString::operator==(const char*) null arm; the lever (null arm `return strcmp(text, "") == 0;`) needs a repo opt-in
  FABLE_RETAIL_CCHARSTRING_TEXT_EQ_RETURN_STRCMP in rebuild/include/fable_string.h -- an overlay copy is impossible (g5 SRC_FAIL:
  `#pragma once`, repo copy also reached). Coordinator: add the opt-in and regrade the differ file with it defined. (2) shot / shot_def
  ebx/ebp swap (c2prio shot_def 71 > shot 66) to work after (1).
- 0x00802B60 GetTargetingRangeForTarget -- DIFFER(410v424), 4 grades (chunk 1025) + 3 c2prio probes here: def priority 37 > map 30 >
  target 28 under every spelling (prior, block-scoped def with early return, const-ref temporary); targeter (prio -3) takes ebp where
  retail homes it to memory. Next: a spelling that lowers def's reference weight (def read through a helper taking it by reference, or
  the targeting def fetched twice) -- read the decision with c2prio --trace before grading.

## Name corrections
See NAME_FIXES.tsv: 0x006BD760 is CState::GetActiveSet (landed as CMusicManager_IsDangerMusicEnabled); 0x00C400D0 spelled
Animation_ComputeFrameOffset by landed callers 0x00C71270 / 0x00C708A0; 0x006E2A50 probably PlayRandomLipMovement (landed as
CTCTalk::SetUpSpeechSample); catalog labels CMusicManager_CopyAssign (0x006BD9B0) and CShaderConstantRange_ValidateAndLoad (0x00C43B30).
