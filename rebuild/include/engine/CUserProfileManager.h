#ifndef FABLE_ENGINE_CUSERPROFILEMANAGER_H
#define FABLE_ENGINE_CUSERPROFILEMANAGER_H
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#include "fable_string.h"
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
#include "fable_string.h"
#endif
// CUserProfileManager (new, chunk 765): only the two methods row 0x006C9FD0 calls are modelled. Ego_r.pdb and FableWin.pdb
// both spell the singleton accessor ?Get@CUserProfileManager@@SIAAV1@XZ (static __fastcall, returns a reference; retail
// 0x0040D2A0 lazily news 0x140 bytes into 0x013B7D4C); ?ProcessPendingTutorials@CUserProfileManager@@QAEXXZ = retail 0x0040BB30.
// Landed rows (e.g. NUISystemCSlider_GetValueFromProfile_0054bbb0.cpp) declare the same Get locally.
class CUserProfileManager {
public:
    static CUserProfileManager& __fastcall Get();                         // 0x0040D2A0
    void ProcessPendingTutorials();                                      // 0x0040BB30
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
    // Retail opt-in (chunk 795, row 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate): the profile volumes and their Ego_r.pdb accessors
    // ?GetSoundVolume@CUserProfileManager@@QBEMXZ / ?GetDialogueVolume@CUserProfileManager@@QBEMXZ (out of line at 0x0047CFC2 /
    // 0x0047CFD3 for the slider TU, expanded in place here: `cmp byte [eax+0Ch] ; mov [eax+0E4h] / [eax+24h]`, 0x005E7980).
    // Retail offsets = Ego_r.pdb before the first vector (UseNewProfileValues +0x0C, SoundVolume +0x24, MusicVolume +0x28,
    // DialogueVolume +0x2C) and Ego_r.pdb - 4 after it (SoundVolumeNewProfile +0xE4, MusicVolumeNewProfile +0xE8,
    // DialogueVolumeNewProfile +0xEC). Combinable with FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD.
    float GetSoundVolume() const { return UseNewProfileValues ? SoundVolumeNewProfile : SoundVolume; }
    float GetDialogueVolume() const { return UseNewProfileValues ? DialogueVolumeNewProfile : DialogueVolume; }
    unsigned char _pad_vol_0x00[0xc];                                    // vptr, CBaseClass, ProfileName
    bool UseNewProfileValues;                                            // +0x0c
    unsigned char _pad_vol_0x0d[0x24 - 0xd];                             // SubtitlingEnabled .. HUDAlpha
    float SoundVolume;                                                   // +0x24
    float MusicVolume;                                                   // +0x28
    float DialogueVolume;                                                // +0x2c
    unsigned char _pad_vol_0x30[0xe4 - 0x30];                            // UsingHeadPhones .. HUDAlphaNewProfile
    float SoundVolumeNewProfile;                                         // +0xe4
    float MusicVolumeNewProfile;                                         // +0xe8
    float DialogueVolumeNewProfile;                                      // +0xec
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_TARGETINGSTATUS
    // Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the targeting-status pair the row
    // selects by UseNewProfileValues (`cmp byte [eax+0Ch],0 ; mov al,[eax+134h] / [eax+6Ch]`): Ego_r.pdb ShowTargetingStatus (+0x74
    // there, after two 16-byte vectors that are 12 in retail: +0x6C) and ShowTargetingStatusNewProfile (+0x138 there; retail +0x134,
    // the VOLUME block's -4 rule for the new-profile members). Not combinable with the VOLUME layout block above (own pads).
    unsigned char _pad_ts_0x00[0xc];                                     // vptr, CBaseClass, ProfileName
    bool UseNewProfileValues;                                            // +0x0c
    unsigned char _pad_ts_0x0d[0x6c - 0xd];                              // SubtitlingEnabled .. AssignedInputsAggressive
    bool ShowTargetingStatus;                                            // +0x6c
    unsigned char _pad_ts_0x6d[0x134 - 0x6d];                            // EffectsDetail .. CameraSensitivityNewProfile
    bool ShowTargetingStatusNewProfile;                                  // +0x134
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
    // Nested opt-in (chunk 777, row 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest): the scoreboard settings and
    // their FableWin.pdb accessors (?GetScoreboardUserName@CUserProfileManager@@QAE?AVCWideString@@XZ, ..ProxyIp.., ?GetScoreboardProxyPort
    // @CUserProfileManager@@QAEJXZ, ?GetScoreboardServerPort@..@QAEJXZ; inline in retail, which copies the CWideString member with the
    // copy constructor 0x0099B720 and reads the ports in place). Retail offsets = Ego_r.pdb - 4 (STLport vectors 12, the deque-backed
    // queue 0x28): ScoreBoardUserName +0x110 (0x0055F8A8), ScoreBoardProxyIp +0x118 (0x0055F759), ScoreBoardProxyPort +0x11C
    // (0x0055F7B9), ScoreBoardServerPort +0x120 (0x0055F830). The members before them stay opaque.
    CWideString GetScoreboardUserName() { return ScoreBoardUserName; }
    CWideString GetScoreboardPassword() { return ScoreBoardPassword; }
    CWideString GetScoreboardProxyIp() { return ScoreBoardProxyIp; }
    long GetScoreboardProxyPort() { return ScoreBoardProxyPort; }
    long GetScoreboardServerPort() { return ScoreBoardServerPort; }
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
    unsigned char _pad_0[0x110 - 0xf0];                                  // UsingHeadPhonesNewProfile .. StereoReversedNewProfile
#else
    unsigned char _pad_0[0x110];                                         // ProfileName .. StereoReversedNewProfile
#endif
    CWideString ScoreBoardUserName;                                      // +0x110
    CWideString ScoreBoardPassword;                                      // +0x114
    CWideString ScoreBoardProxyIp;                                       // +0x118
    long ScoreBoardProxyPort;                                            // +0x11c
    long ScoreBoardServerPort;                                           // +0x120
#endif
};
#endif
