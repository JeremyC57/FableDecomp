# agent1026 header overlay (grade with `export FABLE_HDR_OVERLAY=$A/hdr`)

Every change is behind a new opt-in; with the opt-in undefined each copied header is byte-identical in effect to the repo copy.

| header | opt-in | what it adds | evidence | used by |
|---|---|---|---|---|
| engine/CMusicManager.h | (new header) + `FABLE_RETAIL_CMUSICMANAGER_OVERRIDE_TESTS` | class CMusicManager (CurrentState +0, PSoundDef +0x18, RegisteredMusic +0x1C, CurrentMusicID +0x2C, PendingMusicID +0x30, PPendingFadeTimes +0x34, PendingLoopCount +0x38, Paused +0x3C), nested CState (inline GetActiveSet / GetDanger bodies), CFadeTimes {FadeOutMS, FadeInMS}, CRegisteredMusicEntry, InternalUpdate / PlayMusic decls; the opt-in adds the inline override tests HasUIOverride / HasCinematicOverride / HasOverride / HasAnyOverride (COMDATs 0x006BD7B0..0x006BD7E0, descriptive names) | FableWin.pdb / Ego_r.pdb CMusicManager name set; struct_layouts_egor.tsv; offsets read by 0x006BD9B0 | 006bd9b0 |
| engine/CSoundDef.h | `FABLE_RETAIL_CSOUNDDEF_MUSICSETENTRIES` | class CMusicSetEntry {NormalMusicID, DangerMusicID, LoopCount} and `std::vector<CMusicSetEntry> MusicSetEntries` at +0x60 (offset asserts) | Ego_r layout; 0x006BDA4F.. reads [def+0x64]-[def+0x60] / 12 | 006bd9b0 |
| engine/CTCTalk.h | `FABLE_RETAIL_CTCTALK_CONVERSATION` | Attitude +0x18, PlaySpeech(CASoundBank&, long, bool, const CCharString&, NSpeechGainManager::EDialogueLayer, bool, bool) 0x006E3580, PlayRandomLipMovement() 0x006E2A50, GetCurrentSpeechDuration() const 0x006E2840, inline SetAttitude | FableWin.pdb CTCTalk name set; call shapes at 0x00716C5A / 0x00716C63 / 0x00716C80 | 00716b30 |
| engine/CTCBase.h | `FABLE_RETAIL_CTCBASE_TALKACTION_FRIEND` | `friend class NTCScriptedControl::CActionTalkBase;` so the action calls the protected GetWorld 0x00686A70 (retail calls it directly at 0x00716D7B) | Ego_r access of GetWorld (IBE) | 00716b30 |
| engine/CGameCameraManager.h | `FABLE_RETAIL_CGAMECAMERAMANAGER_SCRIPTEDMODE` | `bool IsUsingScriptedCameraMode() const;` 0x00697AE0 | FableWin ?IsUsingScriptedCameraMode@CGameCameraManager@@QBE_NXZ | 00716b30 |

Not in the overlay: a fable_string.h opt-in (FABLE_RETAIL_CCHARSTRING_TEXT_EQ_RETURN_STRCMP) for 0x007C0AD0 -- fable_string.h is
`#pragma once` and also reached through rebuild/include, so an overlay copy redefines CCharString (SRC_FAIL, g5). It has to be made in the
repo header (see NOTES.md, 007c0ad0).
