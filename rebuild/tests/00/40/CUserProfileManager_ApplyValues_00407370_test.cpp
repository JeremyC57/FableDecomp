#include <cstdio>
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
struct CRenderer { float Gamma; };
struct CRendererPtr {
    CRenderer* Data;
    operator bool() const { return Data != 0; }
    CRenderer* operator->() const { return Data; }
};
static int g_refreshes = 0;
struct CPlayerObject { void Refresh() { ++g_refreshes; } };
class CPlayer { public: CPlayerObject* Object; };
class CPlayerManager { public:
    CPlayer player; bool exists;
    long GetMainPlayerNumber() { return 3; }
    bool IsPlayer(long number) { return exists && number == 3; }
    CPlayer* GetMainPlayer() { return &player; }
};
class CMainGameComponent { public: CPlayerManager* PlayerManager; CRendererPtr Renderer; };
static CMainGameComponent* g_pMainGameComponent;
static float g_dmGamma = -1.0f;
class CDisplayManager { public: void SetGamma(float gamma, bool b) { g_dmGamma = b ? -2.0f : gamma; } };
static CDisplayManager* g_pDisplayManager;
static float g_vol[9];
static bool g_head = false;
struct CSoundSystem {
    void SetUsingHeadPhones(bool on) { g_head = on; }
    void RefreshCategory(long category, float volume, bool b)
    {
        int i = 0; while ((1L << (i + 4)) != category) ++i;
        g_vol[i] = b ? -9.0f : volume;
    }
};
static CSoundSystem* g_pSoundSystem;
static bool g_bVibrationEnabled;
class CUserProfileManager { public:
    bool  UseNewProfileValues;
    bool  VibrationEnabled;
    float Gamma;
    float SoundVolume;
    float MusicVolume;
    float DialogueVolume;
    bool  UsingHeadPhones;
    bool  SoundsMuted;
    bool  VibrationEnabledNewProfile;
    float GammaNewProfile;
    float SoundVolumeNewProfile;
    float MusicVolumeNewProfile;
    float DialogueVolumeNewProfile;
    bool  UsingHeadPhonesNewProfile;
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
            players->GetMainPlayer()->Object->Refresh();
    }
}

int main()
{
    CUserProfileManager m;
    m.UseNewProfileValues = false; m.VibrationEnabled = true; m.Gamma = 1.5f; m.SoundVolume = 0.25f; m.MusicVolume = 0.5f;
    m.DialogueVolume = 0.75f; m.UsingHeadPhones = true; m.SoundsMuted = false; m.VibrationEnabledNewProfile = false;
    m.GammaNewProfile = 2.0f; m.SoundVolumeNewProfile = 0.1f; m.MusicVolumeNewProfile = 0.2f; m.DialogueVolumeNewProfile = 0.3f;
    m.UsingHeadPhonesNewProfile = false;
    CRenderer r; r.Gamma = 0.0f;
    CPlayerObject po; CPlayerManager pm; pm.player.Object = &po; pm.exists = true;
    CMainGameComponent mgc; mgc.PlayerManager = &pm; mgc.Renderer.Data = &r;
    CDisplayManager dm; CSoundSystem ss;
    g_pMainGameComponent = &mgc; g_pDisplayManager = &dm; g_pSoundSystem = &ss;
    m.ApplyValues();
    CHECK(g_bVibrationEnabled && r.Gamma == 1.5f && g_dmGamma == -1.0f && g_refreshes == 1);
    CHECK(g_vol[0] == 0.25f && g_vol[1] == 0.25f && g_vol[2] == 0.75f && g_vol[3] == 0.25f && g_vol[4] == 0.5f && g_head);
    m.UseNewProfileValues = true; g_pMainGameComponent = 0;
    m.ApplyValues();
    CHECK(!g_bVibrationEnabled && g_dmGamma == 2.0f && g_refreshes == 1);
    CHECK(g_vol[0] == 0.1f && g_vol[2] == 0.3f && g_vol[4] == 0.2f && !g_head);
    m.SoundsMuted = true;
    m.ApplyValues();
    CHECK(g_vol[0] == 0.0f && g_vol[2] == 0.0f && g_vol[4] == 0.0f);
    if (g_fail) { std::printf("00407370_TEST FAIL\n"); return 1; }
    std::printf("00407370_TEST PASS\n");
    return 0;
}