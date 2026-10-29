// CUserProfileManager::ApplyValues() -- push the profile's live values (the new-profile set when UseNewProfileValues) to the engine: vibration flag, gamma (renderer or display manager), the five sound-category volumes (zero when muted) plus the headphones flag, then a main-player refresh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00407370, length 569 bytes, plain `ret` (the last call is a tail jump).
// Callees: CDisplayManager::SetGamma 0x009BEAA0; the sound system's slot 46 RefreshCategory (+0xb8) and slot 7 (+0x1c,
//   descriptive SetUsingHeadPhones); CPlayerManager::GetMainPlayerNumber 0x00449700 / IsPlayer 0x00449880 / GetMainPlayer
//   0x00449970; the main player's +0x0c object's 0x00447390 (descriptive Refresh). The renderer pointer's safe-bool member
//   0x0040D870. Globals: vibration flag 0x013966F0, g_pMainGameComponent 0x013B86A0 (+0x1c player manager, +0x28 renderer),
//   g_pDisplayManager 0x013B8390, g_pSoundSystem 0x013B8394.
// Layout: CUserProfileManager (Ego_r.pdb; fields after the first vectors are retail = PDB - 4): UseNewProfileValues +0x0c,
//   VibrationEnabled +0x0e, Gamma +0x1c, SoundVolume +0x24, MusicVolume +0x28, DialogueVolume +0x2c, UsingHeadPhones
//   +0x30, SoundsMuted +0x31, VibrationEnabledNewProfile +0xce, GammaNewProfile +0xdc, SoundVolumeNewProfile +0xe4,
//   MusicVolumeNewProfile +0xe8, DialogueVolumeNewProfile +0xec, UsingHeadPhonesNewProfile +0xf0. The renderer's gamma at
//   +0xb4.
// Shape: every profile read is an inline Get* accessor choosing the new-profile copy.
struct CRenderer {
    unsigned char _pad00[0xb4];
    float         Gamma;                                    // +0xb4
};
struct CRendererPtr {                                       // a counted pointer with the safe-bool conversion
    CRenderer* Data;
    typedef void (CRendererPtr::*bool_type)() const;
    void true_value() const;                                // 0x0040D870
    operator bool_type() const { return Data ? &CRendererPtr::true_value : 0; }
    CRenderer* operator->() const { return Data; }
};
struct CPlayerObject {
    void Refresh();                                         // 0x00447390
};
class CPlayer { public:
    unsigned char  _pad00[0x0c];
    CPlayerObject* Object;                                  // +0x0c
};
class CPlayerManager { public:
    long GetMainPlayerNumber() const;                             // 0x00449700
    bool IsPlayer(long number);                             // 0x00449880
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                          // +0x1c
    unsigned char   _pad20[0x28 - 0x20];
    CRendererPtr    Renderer;                               // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
class CDisplayManager { public:
    void SetGamma(float gamma, bool b);                     // 0x009BEAA0
};
extern CDisplayManager* g_pDisplayManager;                  // 0x013B8390
struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void SetUsingHeadPhones(bool on);               // slot 7 (+0x1c)
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void RefreshCategory(long category, float volume, bool b);   // slot 46 (+0xb8)
};
extern CSoundSystem* g_pSoundSystem;                        // 0x013B8394
extern bool g_bVibrationEnabled;                            // 0x013966F0
class CUserProfileManager { public:
    unsigned char _pad00[0x0c];
    bool  UseNewProfileValues;                              // +0x0c
    unsigned char _pad0d;
    bool  VibrationEnabled;                                 // +0x0e
    unsigned char _pad0f[0x1c - 0x0f];
    float Gamma;                                            // +0x1c
    unsigned char _pad20[0x24 - 0x20];
    float SoundVolume;                                      // +0x24
    float MusicVolume;                                      // +0x28
    float DialogueVolume;                                   // +0x2c
    bool  UsingHeadPhones;                                  // +0x30
    bool  SoundsMuted;                                      // +0x31
    unsigned char _pad32[0xce - 0x32];
    bool  VibrationEnabledNewProfile;                       // +0xce
    unsigned char _padcf[0xdc - 0xcf];
    float GammaNewProfile;                                  // +0xdc
    unsigned char _pade0[0xe4 - 0xe0];
    float SoundVolumeNewProfile;                            // +0xe4
    float MusicVolumeNewProfile;                            // +0xe8
    float DialogueVolumeNewProfile;                         // +0xec
    bool  UsingHeadPhonesNewProfile;                        // +0xf0
    bool  GetVibrationEnabled() const { return UseNewProfileValues ? VibrationEnabledNewProfile : VibrationEnabled; }
    const float& GetGamma() const { return UseNewProfileValues ? GammaNewProfile : Gamma; }
    const float& GetSoundVolume() const { return UseNewProfileValues ? SoundVolumeNewProfile : SoundVolume; }
    const float& GetMusicVolume() const { return UseNewProfileValues ? MusicVolumeNewProfile : MusicVolume; }
    const float& GetDialogueVolume() const { return UseNewProfileValues ? DialogueVolumeNewProfile : DialogueVolume; }
    bool  GetUsingHeadPhones() const { return UseNewProfileValues ? UsingHeadPhonesNewProfile : UsingHeadPhones; }
    void ApplyValues();
};

void CUserProfileManager::ApplyValues()
{
    g_bVibrationEnabled = UseNewProfileValues ? VibrationEnabledNewProfile : VibrationEnabled;
    if (g_pMainGameComponent) {
        if (g_pMainGameComponent->Renderer) {
            if (UseNewProfileValues)
                g_pMainGameComponent->Renderer->Gamma = GammaNewProfile;
            else
                g_pMainGameComponent->Renderer->Gamma = Gamma;
        }
    } else if (g_pDisplayManager) {
        if (UseNewProfileValues)
            g_pDisplayManager->SetGamma(GammaNewProfile, false);
        else
            g_pDisplayManager->SetGamma(Gamma, false);
    }
    if (g_pSoundSystem) {
        if (SoundsMuted) {
            g_pSoundSystem->RefreshCategory(0x10, 0.0f, false);
            g_pSoundSystem->RefreshCategory(0x20, 0.0f, false);
            g_pSoundSystem->RefreshCategory(0x40, 0.0f, false);
            g_pSoundSystem->RefreshCategory(0x80, 0.0f, false);
            g_pSoundSystem->RefreshCategory(0x100, 0.0f, false);
        } else {
            if (UseNewProfileValues) {
                g_pSoundSystem->RefreshCategory(0x10, SoundVolumeNewProfile, false);
                g_pSoundSystem->RefreshCategory(0x20, SoundVolumeNewProfile, false);
                g_pSoundSystem->RefreshCategory(0x40, DialogueVolumeNewProfile, false);
                g_pSoundSystem->RefreshCategory(0x80, SoundVolumeNewProfile, false);
                g_pSoundSystem->RefreshCategory(0x100, MusicVolumeNewProfile, false);
                g_pSoundSystem->SetUsingHeadPhones(UsingHeadPhonesNewProfile);
            } else {
                g_pSoundSystem->RefreshCategory(0x10, SoundVolume, false);
                g_pSoundSystem->RefreshCategory(0x20, SoundVolume, false);
                g_pSoundSystem->RefreshCategory(0x40, DialogueVolume, false);
                g_pSoundSystem->RefreshCategory(0x80, SoundVolume, false);
                g_pSoundSystem->RefreshCategory(0x100, MusicVolume, false);
                g_pSoundSystem->SetUsingHeadPhones(UsingHeadPhones);
            }
        }
    }
    if (g_pMainGameComponent) {
        CPlayerManager* players = g_pMainGameComponent->PlayerManager;
        if (players->IsPlayer(players->GetMainPlayerNumber()))
            players->GetMainPlayer().Object->Refresh();
    }
}