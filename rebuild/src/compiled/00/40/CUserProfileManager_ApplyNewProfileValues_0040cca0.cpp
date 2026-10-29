// CUserProfileManager::ApplyNewProfileValues() -- copy every "new profile" option into the live option, re-applying the options after each copy; switch to the WASD key set when the new profile asks for it, then save the profile
// Leaf: ?ApplyNewProfileValues@CUserProfileManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040CCA0, 535 bytes; single caller 0x005969FD.
// Identity: Ego_r.pdb `?ApplyNewProfileValues@CUserProfileManager@@QAEXXZ`; every copy pairs an Ego_r `<X>NewProfile`
//   member with its live `<X>` member at the retail offsets (see the layout below), and the existing binding agrees.
// Callees:
//   CUserProfileManager::ApplyValues 0x00407370 (protected; reads UseNewProfileValues +0x0C to pick the new or live set),
//   CUserProfileManager::ResetAssignedInputsWASD 0x00408820 (loads FABLE_PC_CONTROL_SCHEME_GDD_WASD[_NO_CAPSLOCK]),
//   CUserProfileManager::Save 0x0040A740 (the out-of-line setters SetX(bool value, bool save) call it when `save`;
//   its body writes the profile file when UseNewProfileValues is false). Retail tail-jumps to Save.
// Shape: one inlined setter call per option, `SetX(XNewProfile, false)`, in retail's order (Sound, Dialogue, Music;
//   EffectsDetail between MeshDetail and Resolution; ScreenAspectRatio, VerticalSync, CameraSensitivity,
//   CameraResetting), then `SetShowTargetingStatus(ShowTargetingStatusNewProfile, true)`, whose `if (save) Save()`
//   becomes the tail jump. The setters are the retail TU's own bodies (out-of-line copies at 0x0040AF90.., Ego_r
//   declaration order: `if (UseNewProfileValues) XNewProfile = v; X = v; ApplyValues(); if (save) Save();`). Inlined
//   with v = XNewProfile the guarded store is a self-assignment, so VC7.1 drops it together with its
//   UseNewProfileValues test, and the by-value parameter is what keeps every bool copy in al (a plain
//   `X = XNewProfile;` rotates al/cl/dl and reorders the dword copies). They are written `inline` here: the TU's
//   out-of-line copies are other catalog rows.
// Layout: retail offsets = Ego_r offsets, minus 4 per STLport vector (0xC vs 0x10) before the member, plus 0x10 for the
//   STLport deque (0x28 vs 0x18) under PendingTutorialCategories. Bases per RTTI: CBaseClassNonCopyable @0,
//   CDeviceResetCallback @4 (no shared header for CUserProfileManager; the bases are kept opaque here).
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