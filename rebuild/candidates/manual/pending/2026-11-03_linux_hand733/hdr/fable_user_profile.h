#pragma once

#include "fable_string.h"

#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER
// Retail opt-in (movie row 0x006286F0): the retail class. RTTI (vtables 0x0122E23C / 0x0122E230) CUserProfileManager :
// CBaseClassNonCopyable (vptr +0x00), CDeviceResetCallback (vptr +0x04); Ego_r.pdb members (0x144 bytes with Dinkumware
// containers) at retail offsets, 0x140 in all. Members before the first container (+0x54) sit at the PDB offsets; from
// AssignedInputsPassive on, the STLport sizes (vector 0xC, queue<deque> 0x28) move them by -4 in all (5 vectors -0x14, the
// deque +0x10). Chunk 733 replaced the two opaque blocks (+0x52..+0xCB, +0x100..+0x13F) with the Ego_r members at the
// retail offsets the landed rows prove (ApplyNewProfileValues 0x0040CCA0, ~CUserProfileManager 0x00408990, SetDefaultValues
// 0x0040C790, SetDefaultValuesForNewProfile 0x00409B70); every offset is asserted below. Read back by retail code:
// UseNewProfileValues +0x0C, SubtitlingEnabled +0x0D, SoundVolume +0x24, MusicVolume +0x28, DialogueVolume +0x2C,
// SubtitlingEnabledNewProfile +0xCD, SoundVolumeNewProfile +0xE4, MusicVolumeNewProfile +0xE8, DialogueVolumeNewProfile
// +0xEC (0x006286F0; the out-of-line getter COMDATs 0x0047CFC2 / 0x0047CFD3), ShadowDetail +0x40 / ShadowDetailNewProfile
// +0xFC (0x0054BBB0).
// Member functions: Ego_r.pdb + FableWin.pdb spellings (access and calling convention as both PDBs mangle them). Get 0x0040D2A0
// is static __fastcall (?Get@CUserProfileManager@@SIAAV1@XZ); the getters are the `UseNewProfileValues ? <X>NewProfile : <X>`
// bodies of the retail COMDATs above (inline: 0x006286F0 expands them; other callers call the COMDAT copies).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDeviceResetCallback.h"
#include "engine/CDisplayModeDesc.h"
#include "engine/CActionInputControl.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_deque.h"
enum ETutorialCategory;                                                  // Ego_r.pdb global enum (MS forward declaration)
enum EEngineVideoOptionQuality;                                          // Ego_r.pdb global enum (engine/CEngineVideoOptions.h)
class CAFile;
class CTexture;
class CConfigOptionsDefaultsDef;                                         // chunk 719: GetConfigOptionsDefaultsDef's def type
template <class T> class CDefPointer;                                    // engine/CDefPointer.h (by-value return: complete at the definition)
class CUserProfileManager : public CBaseClassNonCopyable, public CDeviceResetCallback
{
public:
    CUserProfileManager();
    virtual ~CUserProfileManager();
    static CUserProfileManager& FABLE_FASTCALL Get();                    // 0x0040D2A0
    // Chunk 719: Ego_r.pdb + FableWin.pdb ?VerifySignature@CUserProfileManager@@SI_NABVCWideString@@@Z (public static __fastcall).
    static bool FABLE_FASTCALL VerifySignature(const CWideString& filename);   // 0x00409730
    void SetProfileName(const CWideString& profileName, bool loadProfile);
    bool IsSubtitlingEnabled() const { return UseNewProfileValues ? SubtitlingEnabledNewProfile : SubtitlingEnabled; }
    float GetSoundVolume() const { return UseNewProfileValues ? SoundVolumeNewProfile : SoundVolume; }         // COMDAT 0x0047CFC2
    float GetMusicVolume() const { return UseNewProfileValues ? MusicVolumeNewProfile : MusicVolume; }         // COMDAT 0x0054A4F0
    float GetDialogueVolume() const { return UseNewProfileValues ? DialogueVolumeNewProfile : DialogueVolume; } // COMDAT 0x0047CFD3
    // Retail opt-in addition (chunk 719): the config-options defaults def (landed 0x004083E0) and the four detail caps that read it,
    // Ego_r.pdb / FableWin.pdb QBE members in Ego_r order (0x00406D40 / D80 / DC0 / E00 = retail 0x004084B0 / 0x00408500 /
    // 0x00408550 / 0x004085A0; fields MaxEffectsDetail +0x70, MaxTextureDetail +0x58, MaxShadowDetail +0x60, MaxMeshDetail +0x68).
    CDefPointer<const CConfigOptionsDefaultsDef> GetConfigOptionsDefaultsDef() const;   // 0x004083E0
    float GetMaxEffectsDetail() const;                                   // 0x004084B0
    float GetMaxTextureDetail() const;                                   // 0x00408500
    float GetMaxShadowDetail() const;                                    // 0x00408550
    float GetMaxMeshDetail() const;                                      // 0x004085A0
    void SetDefaultValues();                                             // 0x0040C790 (Ego_r / FableWin ?SetDefaultValues@CUserProfileManager@@QAEXXZ)
    void SetDefaultValuesForNewProfile();                                // 0x00409B70
    // Chunk 733 (Ego_r.pdb + FableWin.pdb, QAE): the profile file (Save 0x0040A740 / Load 0x0040D350 are public in both PDBs:
    // ?Save@CUserProfileManager@@QAEXXZ, ?Load@CUserProfileManager@@QAEXXZ), the default key set (0x004085F0) and the UI
    // background pass (?RenderUIBackground@CUserProfileManager@@QAEXM@Z, 0x004075C0).
    void Save();                                                         // 0x0040A740
    void Load();                                                         // 0x0040D350
    void ResetAssignedInputs();                                          // 0x004085F0
    void RenderUIBackground(float alpha);                                // 0x004075C0
    // Ego_r.pdb / FableWin.pdb ?GetPathNameFromFileName@CUserProfileManager@@QBE?AVCWideString@@ABV2@_N@Z (retail 0x00408120):
    // the save-slot path for a slot file name (LoadGameState keeps a loaded slot name only when it resolves to a path).
    CWideString GetPathNameFromFileName(const CWideString& fileName, bool flag) const;   // 0x00408120 (flag: role not established; LoadGameState passes false)
protected:
    virtual void OnPreDeviceReset();                                     // ?OnPreDeviceReset@CUserProfileManager@@MAEXXZ
    virtual bool OnPostDeviceReset();                                    // ?OnPostDeviceReset@CUserProfileManager@@MAE_NXZ
    // Chunk 733: protected in both PDBs (IAE).
    void ApplyValues();                                                  // 0x00407370 ?ApplyValues@CUserProfileManager@@IAEXXZ
    void ApplyVideoValues();                                             // 0x0040A7F0 ?ApplyVideoValues@CUserProfileManager@@IAEXXZ
    void SaveGameState(CAFile& file);                                    // 0x00409FD0 ?SaveGameState@CUserProfileManager@@IAEXAAVCAFile@@@Z
    void LoadGameState(CAFile& file);                                    // 0x0040BCA0 ?LoadGameState@CUserProfileManager@@IAEXAAVCAFile@@@Z
private:
    // Chunk 733: private in both PDBs (AAE), reference parameters (AAM / AAK).
    void ClampVideoOption(float& value, float configValue, float defaultValue, float configMax, float defaultMax);   // 0x004068D0 ?ClampVideoOption@CUserProfileManager@@AAEXAAMMMMM@Z
    void ClampAntialiasing(unsigned long& antialiasing, long maxAntialiasing);   // 0x00409140 ?ClampAntialiasing@CUserProfileManager@@AAEXAAKJ@Z
    // ?ClampResolution@CUserProfileManager@@AAEXAAKJ@Z (Ego_r 0x00405DA0; retail out-of-line copy 0x00406D90) and
    // ?ClampRefreshRate@CUserProfileManager@@AAEXAAKJ@Z (Ego_r 0x004055E0; no retail copy survives /OPT:REF): ordinary members of
    // the retail TU that VC7.1 expands in SetDefaultValues / SetDefaultValuesForNewProfile (rows that need that define them).
    void ClampResolution(unsigned long& resolution, long maxWidth);      // 0x00406D90
    // ?ConvertFloatToVideoOptionQuality@CUserProfileManager@@AAE?AW4EEngineVideoOptionQuality@@M@Z (Ego_r + FableWin; retail 0x00406820, landed).
    EEngineVideoOptionQuality ConvertFloatToVideoOptionQuality(float value);   // 0x00406820
    void ClampRefreshRate(unsigned long& refreshRate, long maxRefreshRate);
    CWideString   ProfileName;                                           // +0x08
    bool          UseNewProfileValues;                                   // +0x0C
    bool          SubtitlingEnabled;                                     // +0x0D
    bool          VibrationEnabled;                                      // +0x0E
    bool          CameraXYInverted;                                      // +0x0F
    bool          CameraYZInverted;                                      // +0x10
    bool          CameraFPBowXYInverted;                                 // +0x11
    bool          CameraFPBowYZInverted;                                 // +0x12
    bool          CameraPCBowXYInverted;                                 // +0x13
    bool          CameraPCBowYZInverted;                                 // +0x14
    bool          CameraPCViewHeroXYInverted;                            // +0x15
    bool          CameraPCViewHeroYZInverted;                            // +0x16
    bool          TutorialSystemEnabled;                                 // +0x17
    bool          GuildMasterEnabled;                                    // +0x18
    bool          HUDTooltipEnabled;                                     // +0x19
    float         Gamma;                                                 // +0x1C
    float         HUDAlpha;                                              // +0x20
    float         SoundVolume;                                           // +0x24
    float         MusicVolume;                                           // +0x28
    float         DialogueVolume;                                        // +0x2C
    bool          UsingHeadPhones;                                       // +0x30
    bool          SoundsMuted;                                           // +0x31
    bool          ShowingBuddyNames;                                     // +0x32
    bool          ApplyVideoValuesOnLoad;                                // +0x33
    bool          DisplayDialogOnOptionsChanged;                         // +0x34
    unsigned long Antialiasing;                                          // +0x38
    float         TextureDetail;                                         // +0x3C
    float         ShadowDetail;                                          // +0x40
    float         MeshDetail;                                            // +0x44
    unsigned long Resolution;                                            // +0x48
    unsigned long RefreshRate;                                           // +0x4C
    bool          VerticalSyncEnabled;                                   // +0x50
    bool          StereoReversed;                                        // +0x51
    std::vector<CActionInputControl> AssignedInputsPassive;              // +0x54
    std::vector<CActionInputControl> AssignedInputsAggressive;           // +0x60
    bool          ShowTargetingStatus;                                   // +0x6C
    float         EffectsDetail;                                         // +0x70
    unsigned long ScreenAspectRatio;                                     // +0x74
    bool          NewProfileToUseWASD;                                   // +0x78
    bool          CameraResetting;                                       // +0x79
    float         CameraSensitivity;                                     // +0x7C
    std::vector<CDisplayModeDesc>   DisplayModes;                        // +0x80
    std::vector<ETutorialCategory>  GivenTutorialCategories;             // +0x8C
    std::queue<ETutorialCategory>   PendingTutorialCategories;           // +0x98 (STLport deque, 0x28)
    std::vector<CWideString>        SaveGameNames;                       // +0xC0
    bool          DisplayTutorialsDuringCutscenes;                       // +0xCC
    bool          SubtitlingEnabledNewProfile;                           // +0xCD
    bool          VibrationEnabledNewProfile;                            // +0xCE
    bool          CameraXYInvertedNewProfile;                            // +0xCF
    bool          CameraYZInvertedNewProfile;                            // +0xD0
    bool          CameraFPBowXYInvertedNewProfile;                       // +0xD1
    bool          CameraFPBowYZInvertedNewProfile;                       // +0xD2
    bool          CameraPCBowXYInvertedNewProfile;                       // +0xD3
    bool          CameraPCBowYZInvertedNewProfile;                       // +0xD4
    bool          CameraPCViewHeroXYInvertedNewProfile;                  // +0xD5
    bool          CameraPCViewHeroYZInvertedNewProfile;                  // +0xD6
    bool          TutorialSystemEnabledNewProfile;                       // +0xD7
    bool          GuildMasterEnabledNewProfile;                          // +0xD8
    bool          HUDTooltipEnabledNewProfile;                           // +0xD9
    float         GammaNewProfile;                                       // +0xDC
    float         HUDAlphaNewProfile;                                    // +0xE0
    float         SoundVolumeNewProfile;                                 // +0xE4
    float         MusicVolumeNewProfile;                                 // +0xE8
    float         DialogueVolumeNewProfile;                              // +0xEC
    bool          UsingHeadPhonesNewProfile;                             // +0xF0
    bool          ShowingBuddyNamesNewProfile;                           // +0xF1
    unsigned long AntialiasingNewProfile;                                // +0xF4
    float         TextureDetailNewProfile;                               // +0xF8
    float         ShadowDetailNewProfile;                                // +0xFC
    float         MeshDetailNewProfile;                                  // +0x100
    unsigned long ResolutionNewProfile;                                  // +0x104
    unsigned long RefreshRateNewProfile;                                 // +0x108
    bool          VerticalSyncEnabledNewProfile;                         // +0x10C
    bool          StereoReversedNewProfile;                              // +0x10D
    CWideString   ScoreBoardUserName;                                    // +0x110
    CWideString   ScoreBoardPassword;                                    // +0x114
    CWideString   ScoreBoardProxyIp;                                     // +0x118
    long          ScoreBoardProxyPort;                                   // +0x11C
    long          ScoreBoardServerPort;                                  // +0x120
    float         EffectsDetailNewProfile;                               // +0x124
    unsigned long ScreenAspectRatioNewProfile;                           // +0x128
    bool          CameraResettingNewProfile;                             // +0x12C
    float         CameraSensitivityNewProfile;                           // +0x130
    bool          ShowTargetingStatusNewProfile;                         // +0x134
    CTexture*     PBackBufferTexture;                                    // +0x138
    bool          VideoChangesAccepted;                                  // +0x13C
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,UseNewProfileValues)==0x0c);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,DialogueVolume)==0x2c);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,StereoReversed)==0x51);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,AssignedInputsPassive)==0x54);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,ShowTargetingStatus)==0x6c);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,CameraSensitivity)==0x7c);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,DisplayModes)==0x80);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,GivenTutorialCategories)==0x8c);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,PendingTutorialCategories)==0x98);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,SaveGameNames)==0xc0);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,SubtitlingEnabledNewProfile)==0xcd);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,SoundVolumeNewProfile)==0xe4);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,ShadowDetailNewProfile)==0xfc);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,StereoReversedNewProfile)==0x10d);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,ScoreBoardUserName)==0x110);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,ScoreBoardServerPort)==0x120);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,ShowTargetingStatusNewProfile)==0x134);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,VideoChangesAccepted)==0x13c);
    }
};
#else
class CUserProfileManager
{
public:
    CUserProfileManager();

    __forceinline static void* operator new(unsigned int size)
    {
        return ::operator new(size);           // 0x00BFEA1A MSVCR71 operator new
    }

    void SetProfileName(
        const CWideString& profileName,
        bool loadProfile);

private:
    void Load();
    void Save();

    void* primaryVTable_;
    void* secondaryVTable_;
    CWideString profileName_;
    fable_u8 opaque0C_[0x134];
};
#endif // FABLE_RETAIL_CUSERPROFILEMANAGER

struct CCountedUserProfileManager
{
    CUserProfileManager* object;
    void* reference;

    void operator=(CUserProfileManager* value);
};

struct CUserProfileManagerSingleton
{
    static CUserProfileManager& FABLE_FASTCALL Get();
};

extern CCountedUserProfileManager
    g_CUserProfileManagerSingleton_013B7D4C;

FABLE_STATIC_ASSERT(sizeof(CUserProfileManager) == 0x140);
FABLE_STATIC_ASSERT(sizeof(CCountedUserProfileManager) == 0x08);
