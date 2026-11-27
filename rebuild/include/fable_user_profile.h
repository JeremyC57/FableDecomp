#pragma once
#include "fable_compiler.h"

#include "fable_string.h"

#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER
// Retail opt-in (movie row 0x006286F0): the retail class. RTTI (vtables 0x0122E23C / 0x0122E230) CUserProfileManager :
// CBaseClassNonCopyable (vptr +0x00), CDeviceResetCallback (vptr +0x04); Ego_r.pdb members (0x144 bytes with Dinkumware
// containers) at retail offsets, 0x140 in all (default model below). Members before the first container (+0x54) sit at
// the PDB offsets; from AssignedInputsPassive on, the STLport sizes (vector 0xC, queue<deque> 0x28) move them by -4 in
// all (5 vectors -0x14, the deque +0x10); the opaque block starts at the alignment gap +0x52. Read back by retail code: UseNewProfileValues +0x0C, SubtitlingEnabled +0x0D,
// SoundVolume +0x24, MusicVolume +0x28, DialogueVolume +0x2C, SubtitlingEnabledNewProfile +0xCD, SoundVolumeNewProfile
// +0xE4, MusicVolumeNewProfile +0xE8, DialogueVolumeNewProfile +0xEC (0x006286F0; the out-of-line getter COMDATs
// 0x0047CFC2 / 0x0047CFD3), ShadowDetail +0x40 / ShadowDetailNewProfile +0xFC (0x0054BBB0). The containers
// (+0x54 .. +0xCB) and the tail from +0x100 are opaque.
// Member functions: Ego_r.pdb + FableWin.pdb. Get 0x0040D2A0 is static __fastcall (?Get@CUserProfileManager@@SIAAV1@XZ);
// the getters are the `UseNewProfileValues ? <X>NewProfile : <X>` bodies of the retail COMDATs above (inline: 0x006286F0
// expands them; other callers call the COMDAT copies).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDeviceResetCallback.h"
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_DEFAULTS
#include "stlport/fable_deque.h"
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_STATE
#include "engine/CActionInputControl.h"
class CAFile;
#endif
enum ETutorialCategory { };
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VIDEODEFAULTS
#include "engine/CDisplayModeDesc.h"
#include "stlport/fable_containers.h"
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PCLASS
#include "engine/CCountedPointer.h"
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_RENDERUIBACKGROUND
class CTexture;
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_LOADGAMESTATE
class CAFile;
#endif
class CConfigOptionsDefaultsDef;                                         // chunk 719: GetConfigOptionsDefaultsDef's def type
template <class T> class CDefPointer;                                    // engine/CDefPointer.h (by-value return: complete at the definition)
class CUserProfileManager : public CBaseClassNonCopyable, public CDeviceResetCallback
{
#ifdef FABLE_RETAIL_TUTORIAL_GIVE_PASS29
public:
    bool GiveTutorialForCategory(ETutorialCategory category,bool ignore_fading);
    bool IsTutorialSystemEnabled() const;
    bool IsTutorialCategoryGiven(ETutorialCategory category) const;
private:
#endif
#ifdef FABLE_RETAIL_PROJECTILE_UPDATE_PASS28
public:
    // FableWin 029ccb70/029ccbb0 and the native Update profile accesses.
    bool IsCameraFPBowXYInverted() const { return UseNewProfileValues ? CameraFPBowXYInvertedNewProfile : CameraFPBowXYInverted; }
    bool IsCameraFPBowYZInverted() const { return UseNewProfileValues ? CameraFPBowYZInvertedNewProfile : CameraFPBowYZInverted; }
    float GetCameraSensitivity() const { return UseNewProfileValues ? CameraSensitivityNewProfile : CameraSensitivity; }
private:
#endif
public:
    CUserProfileManager();
    virtual ~CUserProfileManager();
    static CUserProfileManager& FABLE_FASTCALL Get();                    // 0x0040D2A0
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PCLASS
    // Nested opt-in (agentNF27, 2026-11-12, row 0x0040D2A0 CUserProfileManager::Get): the singleton's counted pointer at 0x013B7D4C,
    // Ego_r.pdb / FableWin.pdb `dynamic initializer for 'CUserProfileManager::PClass'' (and its atexit destructor), private static per
    // the Anniv360 Test PDB (third source) ?PClass@CUserProfileManager@@0V?$CCountedPointer@VCUserProfileManager@@@@A. Get tests its
    // Data word (`mov eax,[013B7D4Ch] ; test eax,eax`) and resets it through Reset 0x0040F1B0 (`mov ecx,013B7D4Ch`). Declaration only.
private:
    static CCountedPointer<CUserProfileManager> PClass;                  // 0x013B7D4C
public:
#endif
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
#ifdef FABLE_RETAIL_PROFILE_DEFAULT_NAME_PASS26
    // Both original PDBs identify this const public accessor, native 004065D0.
    CWideString GetDefaultProfileName() const;
#endif
    CDefPointer<const CConfigOptionsDefaultsDef> GetConfigOptionsDefaultsDef() const;   // 0x004083E0
    float GetMaxEffectsDetail() const;                                   // 0x004084B0
    float GetMaxTextureDetail() const;                                   // 0x00408500
    float GetMaxShadowDetail() const;                                    // 0x00408550
    float GetMaxMeshDetail() const;                                      // 0x004085A0
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VIDEODEFAULTS
    // Nested opt-in (wf21, row 0x004093E0 SetDefaultValuesVideo): Ego_r.pdb / FableWin.pdb ?SetDefaultValuesVideo@CUserProfileManager@@QAEXXZ,
    // the protected ?ApplyValues@..@IAEXXZ (0x00407370) and the private same-TU clamps (Ego_r.pdb AAEXAAMMMMM@Z / AAEXAAKJ@Z).
    void SetDefaultValuesVideo();                                        // 0x004093E0
    const std::vector<CDisplayModeDesc>& GetDisplayModes() const;        // Ego_r.pdb 0x004054D0 + FableWin.pdb 0x01889A50 ?GetDisplayModes@CUserProfileManager@@QBEABV?$vector@VCDisplayModeDesc@@..@std@@XZ; expanded in SetDefaultValuesVideo at 0x00409548..0x0040954E; no retail address of its own is bound (the 7-byte body at its ICF fold is landed as CModeBlock::GetAnimFilter)
protected:
    void ApplyValues();                                                  // 0x00407370
private:
    void ClampVideoOption(float& value, float newValue, float defaultValue, float maxValue, float defaultMaxValue);   // 0x004068D0
    void ClampAntialiasing(unsigned long& antialiasing, long maxAntialiasing);   // 0x00409140
    void ClampResolution(unsigned long& resolution, long maxWidth);      // 0x00406D90
    void ClampRefreshRate(unsigned long& refreshRate, long maxRefreshRate);   // 0x004069A0
public:
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_DEFAULTS
    void ResetAssignedInputs(); // Ego_r/FableWin QAEXXZ, retail004085F0
protected:
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_STATE
    void SaveGameState(CAFile&); // Ego_r/FableWin IAEXAAVCAFile@@@Z,00409FD0
#endif
    void ApplyVideoValues(); // Ego_r/FableWin IAEXXZ, retail0040A7F0
public:
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_RENDERUIBACKGROUND
    // Nested opt-in (w24B, row 0x004075C0; rebased from chunk 733): Ego_r.pdb / FableWin.pdb ?RenderUIBackground@CUserProfileManager@@QAEXM@Z
    // (public) and the captured back-buffer texture it draws, Ego_r.pdb PBackBufferTexture (retail +0x138: `mov eax,[ecx+138h]` at
    // 0x00407A2B; the landed CaptureBackBufferForUIBackground 0x00406CC0 writes the same word). Not the name of the
    // engine/CUserProfileManager.h declaration-only opt-in (..._UIBACKGROUND), which landed rows define. Layout unchanged (the word is carved out of the opaque tail).
    void RenderUIBackground(float alpha);                                // 0x004075C0
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_LOADGAMESTATE
    // Nested opt-in (w24B, row 0x0040BCA0; rebased from chunk 733): Ego_r.pdb / FableWin.pdb
    // ?LoadGameState@CUserProfileManager@@IAEXAAVCAFile@@@Z (protected, like SaveGameState 0x00409FD0). Declaration only; the row also
    // needs ..._VIDEODEFAULTS + ..._PASS22_DEFAULTS + ..._PASS22_STATE (the typed containers and the clamps).
protected:
    void LoadGameState(CAFile& file);                                    // 0x0040BCA0
public:
#endif
    void SetDefaultValues();                                             // 0x0040C790 (Ego_r / FableWin ?SetDefaultValues@CUserProfileManager@@QAEXXZ)
    void SetDefaultValuesForNewProfile();                                // 0x00409B70
    // Chunk 734 (row 0x004A5E90 CWorld::AutoLoadRespawn): Ego_r.pdb / FableWin.pdb QBE save-name getters (hidden CWideString result).
    CWideString GetAutoSaveFileName() const;                             // 0x00406690 ?GetAutoSaveFileName@CUserProfileManager@@QBE?AVCWideString@@XZ
    CWideString GetAutoSaveQuestStartFileName() const;                   // 0x004066B0 ?GetAutoSaveQuestStartFileName@..
    CWideString GetAutoSaveCheckPointFileName() const;                   // 0x004066F0 ?GetAutoSaveCheckPointFileName@..
    CWideString GetAutoSaveCheckPointPathName() const;                   // 0x00408040 ?GetAutoSaveCheckPointPathName@..
    CWideString GetPathNameFromFileName(const CWideString& fileName, bool b) const;   // 0x00408120 ?GetPathNameFromFileName@CUserProfileManager@@QBE?AVCWideString@@ABV2@_N@Z
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVEPATHS
    // Nested opt-in (chunk 1012, rows 0x004A0780 CWorld::SaveGameStateManual / 0x004A34F0 AutoLoadRegionLeaving): Ego_r.pdb QBE path / name
    // getters (hidden CWideString result), out of line in retail. Ego_r 0x00405F70 / 0x00406870 / 0x004068E0 / 0x00406950 =
    // GetAutoSavePathName / GetAutoSaveQuestStartPathName / GetAutoSaveCheckPointPathName / GetAutoSaveRegionLeavingPathName, retail
    // 0x00406F70 / 0x00407FD0 / 0x00408040 (declared above) / 0x004080B0 (same 0x70 spacing; the bodies build "AutoSave" + ""/".qs"/
    // ".cp"/".rl"); GetAutoSaveRegionLeavingFileName 0x00406730 (the landed CWorld::AutoSaveRegionLeaving 0x004A0180 binds these).
    CWideString GetAutoSavePathName() const;                             // 0x00406F70
    CWideString GetAutoSaveQuestStartPathName() const;                   // 0x00407FD0
    CWideString GetAutoSaveRegionLeavingPathName() const;                // 0x004080B0
    CWideString GetAutoSaveRegionLeavingFileName() const;                // 0x00406730
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVENAME
    // Nested opt-in (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay): Ego_r.pdb 0x00405860
    // ?GetAutoSaveDisplayName@CUserProfileManager@@QBE?AVCWideString@@XZ (hidden CWideString result), out of line in retail at 0x00406C20.
    CWideString GetAutoSaveDisplayName() const;                          // 0x00406C20
#endif
protected:
    virtual void OnPreDeviceReset();                                     // ?OnPreDeviceReset@CUserProfileManager@@MAEXXZ
    virtual bool OnPostDeviceReset();                                    // ?OnPostDeviceReset@CUserProfileManager@@MAE_NXZ
private:
    void Load();
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SAVE_PUBLIC
    // Add-only opt-in (NF297, 2026-11-23; row 0x0040C790 CUserProfileManager::SetDefaultValues): Ego_r.pdb + FableWin.pdb
    // ?Save@CUserProfileManager@@QAEXXZ (public, 0x0040A740; the self and 0x0040B690 / 0x0041E6D3 bind QAE). Declaration only.
public:
#endif
    void Save();
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SAVE_PUBLIC
private:
#endif
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
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_VIDEODEFAULTS
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_DEFAULTS
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_STATE
    unsigned char _pad_defaults_52[2];
    std::vector<CActionInputControl> AssignedInputsPassive; //+54
    std::vector<CActionInputControl> AssignedInputsAggressive; //+60
#else
    unsigned char _pad_defaults_52[0x6c-0x52];
#endif
    bool ShowTargetingStatus; // Ego_r+0x74 minus two vector-size differences
    unsigned char _pad_defaults_6d[3];
#else
    unsigned char _pad_0x52[0x70 - 0x52];                                // +0x52 padding, +0x54 AssignedInputsPassive/Aggressive, +0x6C ShowTargetingStatus (opaque)
#endif
    float         EffectsDetail;                                         // +0x70 (Ego_r.pdb +0x78)
    unsigned long ScreenAspectRatio;                                     // +0x74 (Ego_r.pdb +0x7C)
    bool          NewProfileToUseWASD;                                   // +0x78
    bool          CameraResetting;                                       // +0x79
    float         CameraSensitivity;                                     // +0x7C
    std::vector<CDisplayModeDesc> DisplayModes;                          // +0x80 (Ego_r.pdb +0x88)
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_DEFAULTS
    std::vector<ETutorialCategory> GivenTutorialCategories; // +0x8c
    std::queue<ETutorialCategory> PendingTutorialCategories; // +0x98, STLport deque0x28
    std::vector<CWideString> SaveGameNames; // +0xc0
#else
    unsigned char _pad_0x8c[0xcc - 0x8c];                                // GivenTutorialCategories .. SaveGameNames (opaque)
#endif
#else
    unsigned char _pad_0x52[0xcc - 0x52];                                // +0x52 padding, +0x54 AssignedInputsPassive .. SaveGameNames (opaque)
#endif
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
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_DEFAULTS
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_STATE
    unsigned char _pad_defaults_100[0x110-0x100];
    CWideString ScoreBoardUserName; //+110
    CWideString ScoreBoardPassword; //+114
    CWideString ScoreBoardProxyIp; //+118
#else
    unsigned char _pad_defaults_100[0x11c-0x100];
#endif
    long ScoreBoardProxyPort; // Ego_r+0x120 minus4, retail+0x11c
    long ScoreBoardServerPort; // retail+0x120
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_RENDERUIBACKGROUND
    unsigned char _pad_defaults_124[0x138-0x124];
    CTexture*     PBackBufferTexture;                                    // +0x138
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_CTOR_PASS26
    bool VideoChangesAccepted; // Ego_r +0x140; the retail constructor initializes +0x13c.
    unsigned char _pad_defaults_13d[3];
#else
    unsigned char _pad_defaults_13c[0x140-0x13c];
#endif
#else
    unsigned char _pad_defaults_124[0x140-0x124];
#endif
#else
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_RENDERUIBACKGROUND
    unsigned char _pad_0x100[0x138 - 0x100];                             // +0x100 MeshDetailNewProfile .. (opaque)
    CTexture*     PBackBufferTexture;                                    // +0x138
    unsigned char _pad_0x13c[0x140 - 0x13c];                             // VideoChangesAccepted (opaque)
#else
#ifdef FABLE_RETAIL_PROJECTILE_UPDATE_PASS28
    unsigned char _pad_camera_100[0x30];
    float CameraSensitivityNewProfile; // Ego_r +0x134, retail +0x130
    unsigned char _pad_camera_134[0xc];
#else
    unsigned char _pad_0x100[0x140 - 0x100];
#endif                             // +0x100 MeshDetailNewProfile .. (opaque)
#endif
#endif
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT_AT(176, offsetof(CUserProfileManager,UseNewProfileValues)==0x0c);
        FABLE_STATIC_ASSERT_AT(177, offsetof(CUserProfileManager,DialogueVolume)==0x2c);
        FABLE_STATIC_ASSERT_AT(178, offsetof(CUserProfileManager,StereoReversed)==0x51);
        FABLE_STATIC_ASSERT_AT(179, offsetof(CUserProfileManager,SubtitlingEnabledNewProfile)==0xcd);
        FABLE_STATIC_ASSERT_AT(180, offsetof(CUserProfileManager,SoundVolumeNewProfile)==0xe4);
        FABLE_STATIC_ASSERT_AT(181, offsetof(CUserProfileManager,ShadowDetailNewProfile)==0xfc);
    }
};
#else
class CUserProfileManager
{
public:
    CUserProfileManager();

    FABLE_FORCEINLINE static void* operator new(unsigned int size)
    {
        return ::operator new(size);           // 0x00BFEA1A MSVCR71 operator new
    }

    void SetProfileName(
        const CWideString& profileName,
        bool loadProfile);

private:
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_LOAD_PUBLIC
    // Add-only opt-in (NF293, 2026-11-23; row 0x0040D400 CUserProfileManager::SetProfileName): Ego_r.pdb + FableWin.pdb
    // ?Load@CUserProfileManager@@QAEXXZ (public, 0x0040D350; the self and 0x00556470 / 0x0059686D / 0x00597BF2 bind QAE). Declaration only.
public:
#endif
    void Load();
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_LOAD_PUBLIC
private:
#endif
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SAVE_PUBLIC
    // Add-only opt-in (NF297, 2026-11-23; row 0x0040D400 CUserProfileManager::SetProfileName): Ego_r.pdb + FableWin.pdb
    // ?Save@CUserProfileManager@@QAEXXZ (public, 0x0040A740; the self and 0x0040B690 / 0x0041E6D3 bind QAE). Declaration only.
public:
#endif
    void Save();
#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_SAVE_PUBLIC
private:
#endif

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

FABLE_STATIC_ASSERT_AT(226, sizeof(CUserProfileManager) == 0x140);
FABLE_STATIC_ASSERT_AT(227, sizeof(CCountedUserProfileManager) == 0x08);
