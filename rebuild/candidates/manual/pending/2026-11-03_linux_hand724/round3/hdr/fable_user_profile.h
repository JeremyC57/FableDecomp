#pragma once

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
    // chunk 724c: FableWin ?IsCameraFPBowXYInverted@CUserProfileManager@@QBE_NXZ / ?IsCameraFPBowYZInverted@...@QBE_NXZ /
    // ?GetCameraSensitivity@CUserProfileManager@@QBEMXZ (COMDAT 0x0054A510), inlined by 0x00879900 (`cmp UseNewProfileValues`).
    bool IsCameraFPBowXYInverted() const { return UseNewProfileValues ? CameraFPBowXYInvertedNewProfile : CameraFPBowXYInverted; }
    bool IsCameraFPBowYZInverted() const { return UseNewProfileValues ? CameraFPBowYZInvertedNewProfile : CameraFPBowYZInverted; }
    float GetCameraSensitivity() const { return UseNewProfileValues ? CameraSensitivityNewProfile : CameraSensitivity; }
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
protected:
    virtual void OnPreDeviceReset();                                     // ?OnPreDeviceReset@CUserProfileManager@@MAEXXZ
    virtual bool OnPostDeviceReset();                                    // ?OnPostDeviceReset@CUserProfileManager@@MAE_NXZ
private:
    void Load();
    void Save();
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
    unsigned char _pad_0x52[0x7c - 0x52];                                // +0x52 padding, +0x54 AssignedInputsPassive .. CameraResetting (opaque)
    float         CameraSensitivity;                                     // +0x7C (Ego_r +0x84; 0x00879900 reads it in place)
    unsigned char _pad_0x80[0xcc - 0x80];                                // +0x80 DisplayModes .. SaveGameNames (opaque)
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
    unsigned char _pad_0x100[0x130 - 0x100];                             // +0x100 MeshDetailNewProfile .. CameraResettingNewProfile (opaque)
    float         CameraSensitivityNewProfile;                           // +0x130 (Ego_r +0x134)
    unsigned char _pad_0x134[0x140 - 0x134];                             // +0x134 ShowTargetingStatusNewProfile .. (opaque)
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,UseNewProfileValues)==0x0c);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,DialogueVolume)==0x2c);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,StereoReversed)==0x51);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,SubtitlingEnabledNewProfile)==0xcd);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,SoundVolumeNewProfile)==0xe4);
        FABLE_STATIC_ASSERT(offsetof(CUserProfileManager,ShadowDetailNewProfile)==0xfc);
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
