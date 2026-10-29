// Behaviour test for CUserProfileManager::ApplyNewProfileValues @ 0x0040CCA0: every setting takes its new-profile value
// (the new-profile copy is left as is), ApplyValues runs once per setter, the WASD reset follows the flag, and only the
// final targeting-status setter saves.
#include <stdio.h>
#include <string.h>
class CWideStringData;

class CUserProfileManager { public:
    void*            _bases[2];                          // +0x00 CBaseClassNonCopyable vptr, +0x04 CDeviceResetCallback vptr
    CWideStringData* ProfileName;                       // +0x08 CWideString
    bool             UseNewProfileValues;               // +0x0C
    bool             SubtitlingEnabled;                 // +0x0D
    bool             VibrationEnabled;                  // +0x0E
    bool             CameraXYInverted;                  // +0x0F
    bool             CameraYZInverted;                  // +0x10
    bool             CameraFPBowXYInverted;             // +0x11
    bool             CameraFPBowYZInverted;             // +0x12
    bool             CameraPCBowXYInverted;             // +0x13
    bool             CameraPCBowYZInverted;             // +0x14
    bool             CameraPCViewHeroXYInverted;        // +0x15
    bool             CameraPCViewHeroYZInverted;        // +0x16
    bool             TutorialSystemEnabled;             // +0x17
    bool             GuildMasterEnabled;                // +0x18
    bool             HUDTooltipEnabled;                 // +0x19
    float            Gamma;                             // +0x1C
    float            HUDAlpha;                          // +0x20
    float            SoundVolume;                       // +0x24
    float            MusicVolume;                       // +0x28
    float            DialogueVolume;                    // +0x2C
    bool             UsingHeadPhones;                   // +0x30
    bool             SoundsMuted;                       // +0x31
    bool             ShowingBuddyNames;                 // +0x32
    bool             ApplyVideoValuesOnLoad;            // +0x33
    bool             DisplayDialogOnOptionsChanged;     // +0x34
    unsigned long    Antialiasing;                      // +0x38
    float            TextureDetail;                     // +0x3C
    float            ShadowDetail;                      // +0x40
    float            MeshDetail;                        // +0x44
    unsigned long    Resolution;                        // +0x48
    unsigned long    RefreshRate;                       // +0x4C
    bool             VerticalSyncEnabled;               // +0x50
    bool             StereoReversed;                    // +0x51
    unsigned long    AssignedInputsPassive[3];          // +0x54 std::vector<CActionInputControl>
    unsigned long    AssignedInputsAggressive[3];       // +0x60 std::vector<CActionInputControl>
    bool             ShowTargetingStatus;               // +0x6C
    float            EffectsDetail;                     // +0x70
    unsigned long    ScreenAspectRatio;                 // +0x74
    bool             NewProfileToUseWASD;               // +0x78
    bool             CameraResetting;                   // +0x79
    float            CameraSensitivity;                 // +0x7C
    unsigned long    DisplayModes[3];                   // +0x80 std::vector<CDisplayModeDesc>
    unsigned long    GivenTutorialCategories[3];        // +0x8C std::vector<ETutorialCategory>
    unsigned long    PendingTutorialCategories[10];     // +0x98 std::queue<ETutorialCategory> (STLport deque)
    unsigned long    SaveGameNames[3];                  // +0xC0 std::vector<CWideString>
    bool             DisplayTutorialsDuringCutscenes;   // +0xCC
    bool             SubtitlingEnabledNewProfile;       // +0xCD
    bool             VibrationEnabledNewProfile;        // +0xCE
    bool             CameraXYInvertedNewProfile;        // +0xCF
    bool             CameraYZInvertedNewProfile;        // +0xD0
    bool             CameraFPBowXYInvertedNewProfile;   // +0xD1
    bool             CameraFPBowYZInvertedNewProfile;   // +0xD2
    bool             CameraPCBowXYInvertedNewProfile;   // +0xD3
    bool             CameraPCBowYZInvertedNewProfile;   // +0xD4
    bool             CameraPCViewHeroXYInvertedNewProfile; // +0xD5
    bool             CameraPCViewHeroYZInvertedNewProfile; // +0xD6
    bool             TutorialSystemEnabledNewProfile;   // +0xD7
    bool             GuildMasterEnabledNewProfile;      // +0xD8
    bool             HUDTooltipEnabledNewProfile;       // +0xD9
    float            GammaNewProfile;                   // +0xDC
    float            HUDAlphaNewProfile;                // +0xE0
    float            SoundVolumeNewProfile;             // +0xE4
    float            MusicVolumeNewProfile;             // +0xE8
    float            DialogueVolumeNewProfile;          // +0xEC
    bool             UsingHeadPhonesNewProfile;         // +0xF0
    bool             ShowingBuddyNamesNewProfile;       // +0xF1
    unsigned long    AntialiasingNewProfile;            // +0xF4
    float            TextureDetailNewProfile;           // +0xF8
    float            ShadowDetailNewProfile;            // +0xFC
    float            MeshDetailNewProfile;              // +0x100
    unsigned long    ResolutionNewProfile;              // +0x104
    unsigned long    RefreshRateNewProfile;             // +0x108
    bool             VerticalSyncEnabledNewProfile;     // +0x10C
    bool             StereoReversedNewProfile;          // +0x10D
    CWideStringData* ScoreBoardUserName;                // +0x110 CWideString
    CWideStringData* ScoreBoardPassword;                // +0x114 CWideString
    CWideStringData* ScoreBoardProxyIp;                 // +0x118 CWideString
    long             ScoreBoardProxyPort;               // +0x11C
    long             ScoreBoardServerPort;              // +0x120
    float            EffectsDetailNewProfile;           // +0x124
    unsigned long    ScreenAspectRatioNewProfile;       // +0x128
    bool             CameraResettingNewProfile;         // +0x12C
    float            CameraSensitivityNewProfile;       // +0x130
    bool             ShowTargetingStatusNewProfile;     // +0x134

    void ApplyNewProfileValues();
    void SetSubtitlingEnabled(bool enabled, bool save);
    void SetVibrationEnabled(bool enabled, bool save);
    void SetCameraXYInverted(bool inverted, bool save);
    void SetCameraYZInverted(bool inverted, bool save);
    void SetCameraFPBowXYInverted(bool inverted, bool save);
    void SetCameraFPBowYZInverted(bool inverted, bool save);
    void SetCameraPCBowXYInverted(bool inverted, bool save);
    void SetCameraPCBowYZInverted(bool inverted, bool save);
    void SetCameraPCViewHeroXYInverted(bool inverted, bool save);
    void SetCameraPCViewHeroYZInverted(bool inverted, bool save);
    void SetTutorialSystemEnabled(bool enabled, bool save);
    void SetGuildMasterEnabled(bool enabled, bool save);
    void SetHUDTooltipEnabled(bool enabled, bool save);
    void SetGamma(float gamma, bool save);
    void SetHUDAlpha(float alpha, bool save);
    void SetMusicVolume(float volume, bool save);
    void SetSoundVolume(float volume, bool save);
    void SetDialogueVolume(float volume, bool save);
    void SetUsingHeadPhones(bool usingHeadPhones, bool save);
    void SetShowingBuddyNames(bool showing, bool save);
    void SetShowTargetingStatus(bool showing, bool save);
    void SetAntialiasing(unsigned long antialiasing, bool save);
    void SetCameraResetting(bool resetting, bool save);
    void SetCameraSensitivity(float sensitivity, bool save);
    void SetShadowDetail(float detail, bool save);
    void SetTextureDetail(float detail, bool save);
    void SetMeshDetail(float detail, bool save);
    void SetEffectsDetail(float detail, bool save);
    void SetResolution(unsigned long resolution, bool save);
    void SetRefreshRate(unsigned long rate, bool save);
    void SetScreenAspectRatio(unsigned long ratio, bool save);
    void SetVerticalSyncEnabled(bool enabled, bool save);
    void ResetAssignedInputsWASD();   // 0x00408820
    void Save();                      // 0x0040A740
protected:
    void ApplyValues();               // 0x00407370
};

inline void CUserProfileManager::SetSubtitlingEnabled(bool enabled, bool save)
{
    if (UseNewProfileValues)
        SubtitlingEnabledNewProfile = enabled;
    SubtitlingEnabled = enabled;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetVibrationEnabled(bool enabled, bool save)
{
    if (UseNewProfileValues)
        VibrationEnabledNewProfile = enabled;
    VibrationEnabled = enabled;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraXYInverted(bool inverted, bool save)
{
    if (UseNewProfileValues)
        CameraXYInvertedNewProfile = inverted;
    CameraXYInverted = inverted;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraYZInverted(bool inverted, bool save)
{
    if (UseNewProfileValues)
        CameraYZInvertedNewProfile = inverted;
    CameraYZInverted = inverted;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraFPBowXYInverted(bool inverted, bool save)
{
    if (UseNewProfileValues)
        CameraFPBowXYInvertedNewProfile = inverted;
    CameraFPBowXYInverted = inverted;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraFPBowYZInverted(bool inverted, bool save)
{
    if (UseNewProfileValues)
        CameraFPBowYZInvertedNewProfile = inverted;
    CameraFPBowYZInverted = inverted;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraPCBowXYInverted(bool inverted, bool save)
{
    if (UseNewProfileValues)
        CameraPCBowXYInvertedNewProfile = inverted;
    CameraPCBowXYInverted = inverted;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraPCBowYZInverted(bool inverted, bool save)
{
    if (UseNewProfileValues)
        CameraPCBowYZInvertedNewProfile = inverted;
    CameraPCBowYZInverted = inverted;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraPCViewHeroXYInverted(bool inverted, bool save)
{
    if (UseNewProfileValues)
        CameraPCViewHeroXYInvertedNewProfile = inverted;
    CameraPCViewHeroXYInverted = inverted;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraPCViewHeroYZInverted(bool inverted, bool save)
{
    if (UseNewProfileValues)
        CameraPCViewHeroYZInvertedNewProfile = inverted;
    CameraPCViewHeroYZInverted = inverted;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetTutorialSystemEnabled(bool enabled, bool save)
{
    if (UseNewProfileValues)
        TutorialSystemEnabledNewProfile = enabled;
    TutorialSystemEnabled = enabled;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetGuildMasterEnabled(bool enabled, bool save)
{
    if (UseNewProfileValues)
        GuildMasterEnabledNewProfile = enabled;
    GuildMasterEnabled = enabled;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetHUDTooltipEnabled(bool enabled, bool save)
{
    if (UseNewProfileValues)
        HUDTooltipEnabledNewProfile = enabled;
    HUDTooltipEnabled = enabled;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetGamma(float gamma, bool save)
{
    if (UseNewProfileValues)
        GammaNewProfile = gamma;
    Gamma = gamma;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetHUDAlpha(float alpha, bool save)
{
    if (UseNewProfileValues)
        HUDAlphaNewProfile = alpha;
    HUDAlpha = alpha;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetMusicVolume(float volume, bool save)
{
    if (UseNewProfileValues)
        MusicVolumeNewProfile = volume;
    MusicVolume = volume;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetSoundVolume(float volume, bool save)
{
    if (UseNewProfileValues)
        SoundVolumeNewProfile = volume;
    SoundVolume = volume;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetDialogueVolume(float volume, bool save)
{
    if (UseNewProfileValues)
        DialogueVolumeNewProfile = volume;
    DialogueVolume = volume;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetUsingHeadPhones(bool usingHeadPhones, bool save)
{
    if (UseNewProfileValues)
        UsingHeadPhonesNewProfile = usingHeadPhones;
    UsingHeadPhones = usingHeadPhones;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetShowingBuddyNames(bool showing, bool save)
{
    if (UseNewProfileValues)
        ShowingBuddyNamesNewProfile = showing;
    ShowingBuddyNames = showing;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetShowTargetingStatus(bool showing, bool save)
{
    if (UseNewProfileValues)
        ShowTargetingStatusNewProfile = showing;
    ShowTargetingStatus = showing;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetAntialiasing(unsigned long antialiasing, bool save)
{
    if (UseNewProfileValues)
        AntialiasingNewProfile = antialiasing;
    Antialiasing = antialiasing;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraResetting(bool resetting, bool save)
{
    if (UseNewProfileValues)
        CameraResettingNewProfile = resetting;
    CameraResetting = resetting;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetCameraSensitivity(float sensitivity, bool save)
{
    if (UseNewProfileValues)
        CameraSensitivityNewProfile = sensitivity;
    CameraSensitivity = sensitivity;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetShadowDetail(float detail, bool save)
{
    if (UseNewProfileValues)
        ShadowDetailNewProfile = detail;
    ShadowDetail = detail;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetTextureDetail(float detail, bool save)
{
    if (UseNewProfileValues)
        TextureDetailNewProfile = detail;
    TextureDetail = detail;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetMeshDetail(float detail, bool save)
{
    if (UseNewProfileValues)
        MeshDetailNewProfile = detail;
    MeshDetail = detail;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetEffectsDetail(float detail, bool save)
{
    if (UseNewProfileValues)
        EffectsDetailNewProfile = detail;
    EffectsDetail = detail;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetResolution(unsigned long resolution, bool save)
{
    if (UseNewProfileValues)
        ResolutionNewProfile = resolution;
    Resolution = resolution;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetRefreshRate(unsigned long rate, bool save)
{
    if (UseNewProfileValues)
        RefreshRateNewProfile = rate;
    RefreshRate = rate;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetScreenAspectRatio(unsigned long ratio, bool save)
{
    if (UseNewProfileValues)
        ScreenAspectRatioNewProfile = ratio;
    ScreenAspectRatio = ratio;
    ApplyValues();
    if (save)
        Save();
}
inline void CUserProfileManager::SetVerticalSyncEnabled(bool enabled, bool save)
{
    if (UseNewProfileValues)
        VerticalSyncEnabledNewProfile = enabled;
    VerticalSyncEnabled = enabled;
    ApplyValues();
    if (save)
        Save();
}

void CUserProfileManager::ApplyNewProfileValues()
{
    SetSubtitlingEnabled(SubtitlingEnabledNewProfile, false);
    SetVibrationEnabled(VibrationEnabledNewProfile, false);
    SetCameraXYInverted(CameraXYInvertedNewProfile, false);
    SetCameraYZInverted(CameraYZInvertedNewProfile, false);
    SetCameraFPBowXYInverted(CameraFPBowXYInvertedNewProfile, false);
    SetCameraFPBowYZInverted(CameraFPBowYZInvertedNewProfile, false);
    SetCameraPCBowXYInverted(CameraPCBowXYInvertedNewProfile, false);
    SetCameraPCBowYZInverted(CameraPCBowYZInvertedNewProfile, false);
    SetCameraPCViewHeroXYInverted(CameraPCViewHeroXYInvertedNewProfile, false);
    SetCameraPCViewHeroYZInverted(CameraPCViewHeroYZInvertedNewProfile, false);
    SetTutorialSystemEnabled(TutorialSystemEnabledNewProfile, false);
    SetGuildMasterEnabled(GuildMasterEnabledNewProfile, false);
    SetHUDTooltipEnabled(HUDTooltipEnabledNewProfile, false);
    SetUsingHeadPhones(UsingHeadPhonesNewProfile, false);
    SetShowingBuddyNames(ShowingBuddyNamesNewProfile, false);
    SetGamma(GammaNewProfile, false);
    SetHUDAlpha(HUDAlphaNewProfile, false);
    SetSoundVolume(SoundVolumeNewProfile, false);
    SetDialogueVolume(DialogueVolumeNewProfile, false);
    SetMusicVolume(MusicVolumeNewProfile, false);
    SetAntialiasing(AntialiasingNewProfile, false);
    SetTextureDetail(TextureDetailNewProfile, false);
    SetShadowDetail(ShadowDetailNewProfile, false);
    SetMeshDetail(MeshDetailNewProfile, false);
    SetEffectsDetail(EffectsDetailNewProfile, false);
    SetResolution(ResolutionNewProfile, false);
    SetRefreshRate(RefreshRateNewProfile, false);
    SetScreenAspectRatio(ScreenAspectRatioNewProfile, false);
    SetVerticalSyncEnabled(VerticalSyncEnabledNewProfile, false);
    SetCameraSensitivity(CameraSensitivityNewProfile, false);
    SetCameraResetting(CameraResettingNewProfile, false);
    if (NewProfileToUseWASD)
        ResetAssignedInputsWASD();
    SetShowTargetingStatus(ShowTargetingStatusNewProfile, true);
}

static int g_apply = 0, g_save = 0, g_wasd = 0;
void CUserProfileManager::ApplyValues() { g_apply++; }
void CUserProfileManager::Save() { g_save++; }
void CUserProfileManager::ResetAssignedInputsWASD() { g_wasd++; }
static int fail(int line) { printf("0040cca0_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    static CUserProfileManager m;
    memset(&m, 0, sizeof(m));
    m.SubtitlingEnabledNewProfile = true; m.CameraYZInvertedNewProfile = true; m.HUDTooltipEnabledNewProfile = true;
    m.ShowingBuddyNamesNewProfile = true; m.GammaNewProfile = 1.25f; m.HUDAlphaNewProfile = 0.5f;
    m.SoundVolumeNewProfile = 0.75f; m.DialogueVolumeNewProfile = 0.25f; m.MusicVolumeNewProfile = 0.125f;
    m.AntialiasingNewProfile = 4; m.TextureDetailNewProfile = 2.0f; m.ShadowDetailNewProfile = 3.0f;
    m.MeshDetailNewProfile = 4.0f; m.EffectsDetailNewProfile = 5.0f; m.ResolutionNewProfile = 7;
    m.RefreshRateNewProfile = 60; m.ScreenAspectRatioNewProfile = 2; m.VerticalSyncEnabledNewProfile = true;
    m.CameraSensitivityNewProfile = 0.9f; m.CameraResettingNewProfile = true; m.ShowTargetingStatusNewProfile = true;
    m.VibrationEnabled = true;   // new-profile value false must win
    m.NewProfileToUseWASD = false;
    m.UseNewProfileValues = false;
    m.ApplyNewProfileValues();
    if (!m.SubtitlingEnabled || m.VibrationEnabled || !m.CameraYZInverted || m.CameraXYInverted || !m.HUDTooltipEnabled)
        return fail(__LINE__);
    if (!m.ShowingBuddyNames || m.Gamma != 1.25f || m.HUDAlpha != 0.5f || m.SoundVolume != 0.75f) return fail(__LINE__);
    if (m.DialogueVolume != 0.25f || m.MusicVolume != 0.125f || m.Antialiasing != 4 || m.TextureDetail != 2.0f)
        return fail(__LINE__);
    if (m.ShadowDetail != 3.0f || m.MeshDetail != 4.0f || m.EffectsDetail != 5.0f || m.Resolution != 7) return fail(__LINE__);
    if (m.RefreshRate != 60 || m.ScreenAspectRatio != 2 || !m.VerticalSyncEnabled || m.CameraSensitivity != 0.9f)
        return fail(__LINE__);
    if (!m.CameraResetting || !m.ShowTargetingStatus) return fail(__LINE__);
    if (g_apply != 32 || g_save != 1 || g_wasd != 0) return fail(__LINE__);
    // with the WASD flag and UseNewProfileValues set: the reset runs and the new-profile copies keep their values
    m.NewProfileToUseWASD = true; m.UseNewProfileValues = true;
    m.ApplyNewProfileValues();
    if (g_wasd != 1 || g_apply != 64 || g_save != 2) return fail(__LINE__);
    if (m.GammaNewProfile != 1.25f || !m.SubtitlingEnabledNewProfile || m.Gamma != 1.25f) return fail(__LINE__);
    printf("0040cca0_TEST PASS\n");
    return 0;
}