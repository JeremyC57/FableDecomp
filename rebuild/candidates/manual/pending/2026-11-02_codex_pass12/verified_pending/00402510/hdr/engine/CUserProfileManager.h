#ifdef FABLE_RETAIL_GFMAIN
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_GFMAIN)
#ifndef FABLE_RETAIL_SCOREBOARD_PROFILE_HEADER
#define FABLE_RETAIL_SCOREBOARD_PROFILE_HEADER
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDeviceResetCallback.h"
#include "engine/CWideString.h"
class CTexture;
// Layout: original full field order; native vectors12 and queue40 give size140.
class CUserProfileManager : public CBaseClassNonCopyable, public CDeviceResetCallback {
public:
    static CUserProfileManager& __fastcall Get();
#ifdef FABLE_RETAIL_GFMAIN
    void SetProfileName(const CWideString&,bool);
    static void __fastcall Release();
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    bool GetAssignedInputForAction(EGameAction,bool,CActionInputControl&);
    bool GetAssignedInputForMovementAction(EGameAction,bool,const C2DVector&,CActionInputControl&);
#endif
    static bool __fastcall VerifySignature(const CWideString&);
    CWideString GetScoreboardUserName() { return ScoreBoardUserName; }
    CWideString GetScoreboardProxyIp() { return ScoreBoardProxyIp; }
    long GetScoreboardProxyPort() { return ScoreBoardProxyPort; }
    long GetScoreboardServerPort() { return ScoreBoardServerPort; }
private:
    CWideString ProfileName; // +0x8
    bool UseNewProfileValues; // +0xc
    bool SubtitlingEnabled; // +0xd
    bool VibrationEnabled; // +0xe
    bool CameraXYInverted; // +0xf
    bool CameraYZInverted; // +0x10
    bool CameraFPBowXYInverted; // +0x11
    bool CameraFPBowYZInverted; // +0x12
    bool CameraPCBowXYInverted; // +0x13
    bool CameraPCBowYZInverted; // +0x14
    bool CameraPCViewHeroXYInverted; // +0x15
    bool CameraPCViewHeroYZInverted; // +0x16
    bool TutorialSystemEnabled; // +0x17
    bool GuildMasterEnabled; // +0x18
    bool HUDTooltipEnabled; // +0x19
    unsigned char _pad_1a[2];
    float Gamma; // +0x1c
    float HUDAlpha; // +0x20
    float SoundVolume; // +0x24
    float MusicVolume; // +0x28
    float DialogueVolume; // +0x2c
    bool UsingHeadPhones; // +0x30
    bool SoundsMuted; // +0x31
    bool ShowingBuddyNames; // +0x32
    bool ApplyVideoValuesOnLoad; // +0x33
    bool DisplayDialogOnOptionsChanged; // +0x34
    unsigned char _pad_35[3];
    unsigned long Antialiasing; // +0x38
    float TextureDetail; // +0x3c
    float ShadowDetail; // +0x40
    float MeshDetail; // +0x44
    unsigned long Resolution; // +0x48
    unsigned long RefreshRate; // +0x4c
    bool VerticalSyncEnabled; // +0x50
    bool StereoReversed; // +0x51
    unsigned char _pad_52[2];
    unsigned char AssignedInputsPassive[12]; // std::vector<CActionInputControl,std::allocator<CActionInputControl> > // +0x54
    unsigned char AssignedInputsAggressive[12]; // std::vector<CActionInputControl,std::allocator<CActionInputControl> > // +0x60
    bool ShowTargetingStatus; // +0x6c
    unsigned char _pad_6d[3];
    float EffectsDetail; // +0x70
    unsigned long ScreenAspectRatio; // +0x74
    bool NewProfileToUseWASD; // +0x78
    bool CameraResetting; // +0x79
    unsigned char _pad_7a[2];
    float CameraSensitivity; // +0x7c
    unsigned char DisplayModes[12]; // std::vector<CDisplayModeDesc,std::allocator<CDisplayModeDesc> > // +0x80
    unsigned char GivenTutorialCategories[12]; // std::vector<enum ETutorialCategory,std::allocator<enum ETutorialCategory> > // +0x8c
    unsigned char PendingTutorialCategories[40]; // std::queue<enum ETutorialCategory,std::deque<enum ETutorialCategory,std::allocator<enum ETutorialCategory> > > // +0x98
    unsigned char SaveGameNames[12]; // std::vector<CWideString,std::allocator<CWideString> > // +0xc0
    bool DisplayTutorialsDuringCutscenes; // +0xcc
    bool SubtitlingEnabledNewProfile; // +0xcd
    bool VibrationEnabledNewProfile; // +0xce
    bool CameraXYInvertedNewProfile; // +0xcf
    bool CameraYZInvertedNewProfile; // +0xd0
    bool CameraFPBowXYInvertedNewProfile; // +0xd1
    bool CameraFPBowYZInvertedNewProfile; // +0xd2
    bool CameraPCBowXYInvertedNewProfile; // +0xd3
    bool CameraPCBowYZInvertedNewProfile; // +0xd4
    bool CameraPCViewHeroXYInvertedNewProfile; // +0xd5
    bool CameraPCViewHeroYZInvertedNewProfile; // +0xd6
    bool TutorialSystemEnabledNewProfile; // +0xd7
    bool GuildMasterEnabledNewProfile; // +0xd8
    bool HUDTooltipEnabledNewProfile; // +0xd9
    unsigned char _pad_da[2];
    float GammaNewProfile; // +0xdc
    float HUDAlphaNewProfile; // +0xe0
    float SoundVolumeNewProfile; // +0xe4
    float MusicVolumeNewProfile; // +0xe8
    float DialogueVolumeNewProfile; // +0xec
    bool UsingHeadPhonesNewProfile; // +0xf0
    bool ShowingBuddyNamesNewProfile; // +0xf1
    unsigned char _pad_f2[2];
    unsigned long AntialiasingNewProfile; // +0xf4
    float TextureDetailNewProfile; // +0xf8
    float ShadowDetailNewProfile; // +0xfc
    float MeshDetailNewProfile; // +0x100
    unsigned long ResolutionNewProfile; // +0x104
    unsigned long RefreshRateNewProfile; // +0x108
    bool VerticalSyncEnabledNewProfile; // +0x10c
    bool StereoReversedNewProfile; // +0x10d
    unsigned char _pad_10e[2];
    CWideString ScoreBoardUserName; // +0x110
    CWideString ScoreBoardPassword; // +0x114
    CWideString ScoreBoardProxyIp; // +0x118
    long ScoreBoardProxyPort; // +0x11c
    long ScoreBoardServerPort; // +0x120
    float EffectsDetailNewProfile; // +0x124
    unsigned long ScreenAspectRatioNewProfile; // +0x128
    bool CameraResettingNewProfile; // +0x12c
    unsigned char _pad_12d[3];
    float CameraSensitivityNewProfile; // +0x130
    bool ShowTargetingStatusNewProfile; // +0x134
    unsigned char _pad_135[3];
    CTexture* PBackBufferTexture; // +0x138
    bool VideoChangesAccepted; // +0x13c
};
FABLE_STATIC_ASSERT(sizeof(CUserProfileManager)==0x140);
#endif
#else
#ifndef FABLE_RETAIL_HEADER_CUSERPROFILEMANAGER
#define FABLE_RETAIL_HEADER_CUSERPROFILEMANAGER
class CWideString;
// Static member declaration only; no object layout is used by this translation unit.
class CUserProfileManager {
public:
    static bool __fastcall VerifySignature(const CWideString&);
};
#endif

#endif

#else
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE)
#ifndef FABLE_RETAIL_SCOREBOARD_PROFILE_HEADER
#define FABLE_RETAIL_SCOREBOARD_PROFILE_HEADER
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDeviceResetCallback.h"
#include "engine/CWideString.h"
class CTexture;
// Layout: original full field order; native vectors12 and queue40 give size140.
class CUserProfileManager : public CBaseClassNonCopyable, public CDeviceResetCallback {
public:
    static CUserProfileManager& __fastcall Get();
#ifdef FABLE_RETAIL_SUBTITLE
    bool GetAssignedInputForAction(EGameAction,bool,CActionInputControl&);
    bool GetAssignedInputForMovementAction(EGameAction,bool,const C2DVector&,CActionInputControl&);
#endif
    static bool __fastcall VerifySignature(const CWideString&);
    CWideString GetScoreboardUserName() { return ScoreBoardUserName; }
    CWideString GetScoreboardProxyIp() { return ScoreBoardProxyIp; }
    long GetScoreboardProxyPort() { return ScoreBoardProxyPort; }
    long GetScoreboardServerPort() { return ScoreBoardServerPort; }
private:
    CWideString ProfileName; // +0x8
    bool UseNewProfileValues; // +0xc
    bool SubtitlingEnabled; // +0xd
    bool VibrationEnabled; // +0xe
    bool CameraXYInverted; // +0xf
    bool CameraYZInverted; // +0x10
    bool CameraFPBowXYInverted; // +0x11
    bool CameraFPBowYZInverted; // +0x12
    bool CameraPCBowXYInverted; // +0x13
    bool CameraPCBowYZInverted; // +0x14
    bool CameraPCViewHeroXYInverted; // +0x15
    bool CameraPCViewHeroYZInverted; // +0x16
    bool TutorialSystemEnabled; // +0x17
    bool GuildMasterEnabled; // +0x18
    bool HUDTooltipEnabled; // +0x19
    unsigned char _pad_1a[2];
    float Gamma; // +0x1c
    float HUDAlpha; // +0x20
    float SoundVolume; // +0x24
    float MusicVolume; // +0x28
    float DialogueVolume; // +0x2c
    bool UsingHeadPhones; // +0x30
    bool SoundsMuted; // +0x31
    bool ShowingBuddyNames; // +0x32
    bool ApplyVideoValuesOnLoad; // +0x33
    bool DisplayDialogOnOptionsChanged; // +0x34
    unsigned char _pad_35[3];
    unsigned long Antialiasing; // +0x38
    float TextureDetail; // +0x3c
    float ShadowDetail; // +0x40
    float MeshDetail; // +0x44
    unsigned long Resolution; // +0x48
    unsigned long RefreshRate; // +0x4c
    bool VerticalSyncEnabled; // +0x50
    bool StereoReversed; // +0x51
    unsigned char _pad_52[2];
    unsigned char AssignedInputsPassive[12]; // std::vector<CActionInputControl,std::allocator<CActionInputControl> > // +0x54
    unsigned char AssignedInputsAggressive[12]; // std::vector<CActionInputControl,std::allocator<CActionInputControl> > // +0x60
    bool ShowTargetingStatus; // +0x6c
    unsigned char _pad_6d[3];
    float EffectsDetail; // +0x70
    unsigned long ScreenAspectRatio; // +0x74
    bool NewProfileToUseWASD; // +0x78
    bool CameraResetting; // +0x79
    unsigned char _pad_7a[2];
    float CameraSensitivity; // +0x7c
    unsigned char DisplayModes[12]; // std::vector<CDisplayModeDesc,std::allocator<CDisplayModeDesc> > // +0x80
    unsigned char GivenTutorialCategories[12]; // std::vector<enum ETutorialCategory,std::allocator<enum ETutorialCategory> > // +0x8c
    unsigned char PendingTutorialCategories[40]; // std::queue<enum ETutorialCategory,std::deque<enum ETutorialCategory,std::allocator<enum ETutorialCategory> > > // +0x98
    unsigned char SaveGameNames[12]; // std::vector<CWideString,std::allocator<CWideString> > // +0xc0
    bool DisplayTutorialsDuringCutscenes; // +0xcc
    bool SubtitlingEnabledNewProfile; // +0xcd
    bool VibrationEnabledNewProfile; // +0xce
    bool CameraXYInvertedNewProfile; // +0xcf
    bool CameraYZInvertedNewProfile; // +0xd0
    bool CameraFPBowXYInvertedNewProfile; // +0xd1
    bool CameraFPBowYZInvertedNewProfile; // +0xd2
    bool CameraPCBowXYInvertedNewProfile; // +0xd3
    bool CameraPCBowYZInvertedNewProfile; // +0xd4
    bool CameraPCViewHeroXYInvertedNewProfile; // +0xd5
    bool CameraPCViewHeroYZInvertedNewProfile; // +0xd6
    bool TutorialSystemEnabledNewProfile; // +0xd7
    bool GuildMasterEnabledNewProfile; // +0xd8
    bool HUDTooltipEnabledNewProfile; // +0xd9
    unsigned char _pad_da[2];
    float GammaNewProfile; // +0xdc
    float HUDAlphaNewProfile; // +0xe0
    float SoundVolumeNewProfile; // +0xe4
    float MusicVolumeNewProfile; // +0xe8
    float DialogueVolumeNewProfile; // +0xec
    bool UsingHeadPhonesNewProfile; // +0xf0
    bool ShowingBuddyNamesNewProfile; // +0xf1
    unsigned char _pad_f2[2];
    unsigned long AntialiasingNewProfile; // +0xf4
    float TextureDetailNewProfile; // +0xf8
    float ShadowDetailNewProfile; // +0xfc
    float MeshDetailNewProfile; // +0x100
    unsigned long ResolutionNewProfile; // +0x104
    unsigned long RefreshRateNewProfile; // +0x108
    bool VerticalSyncEnabledNewProfile; // +0x10c
    bool StereoReversedNewProfile; // +0x10d
    unsigned char _pad_10e[2];
    CWideString ScoreBoardUserName; // +0x110
    CWideString ScoreBoardPassword; // +0x114
    CWideString ScoreBoardProxyIp; // +0x118
    long ScoreBoardProxyPort; // +0x11c
    long ScoreBoardServerPort; // +0x120
    float EffectsDetailNewProfile; // +0x124
    unsigned long ScreenAspectRatioNewProfile; // +0x128
    bool CameraResettingNewProfile; // +0x12c
    unsigned char _pad_12d[3];
    float CameraSensitivityNewProfile; // +0x130
    bool ShowTargetingStatusNewProfile; // +0x134
    unsigned char _pad_135[3];
    CTexture* PBackBufferTexture; // +0x138
    bool VideoChangesAccepted; // +0x13c
};
FABLE_STATIC_ASSERT(sizeof(CUserProfileManager)==0x140);
#endif
#else
#ifndef FABLE_RETAIL_HEADER_CUSERPROFILEMANAGER
#define FABLE_RETAIL_HEADER_CUSERPROFILEMANAGER
class CWideString;
// Static member declaration only; no object layout is used by this translation unit.
class CUserProfileManager {
public:
    static bool __fastcall VerifySignature(const CWideString&);
};
#endif

#endif

#endif
