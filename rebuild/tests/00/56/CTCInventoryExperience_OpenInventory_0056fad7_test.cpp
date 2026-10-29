// Behaviour test for CTCInventoryExperience::OpenInventory 0x0056FAD7 (model copy with stub engine objects)
#include <cstdio>

static int g_menus = 0, g_notified = 0, g_prepared = 0, g_hudOff = 0, g_refresh = 0, g_cleared = 0, g_active = 0;
void OnOpenMenus() { ++g_menus; }
class CTCHeroExperience { public:
    long GeneralExperience;
    long Stat[3];
    long GetStatExperience(long stat) { return Stat[stat]; }
};
class CThing { public:
    CTCHeroExperience* m_experience;
    template<class T> bool GetInterface(T** out, long wanted)
    {
        if (wanted == 0x68 && m_experience) { *out = (T*)m_experience; return true; }
        return false;
    }
};
namespace NUISystem { class CManager { public:
    virtual void OnThingInventoryOpened(CThing* thing) { if (thing) ++g_notified; }
    static CManager* __fastcall GetInstance() { static CManager m; return &m; }
}; }
struct CHudOwner { virtual void SetEnabled(bool enabled) { if (!enabled) ++g_hudOff; } };
struct CWorldPart { CHudOwner* Hud; };
class CMainGameComponent { public: CWorldPart* World; };
static CHudOwner g_hud; static CWorldPart g_worldPart = { &g_hud }; static CMainGameComponent g_mgc = { &g_worldPart };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
struct CSoundSystem {
    virtual void RefreshCategory(long category, float a, float b) { if ((category == 0x10 || category == 0x80) && a == 0.0f && b == 500.0f) ++g_refresh; }
};
static CSoundSystem g_sound; static CSoundSystem* g_pSoundSystem = &g_sound;
struct CTutorialSoundHandleMap { void clear() { ++g_cleared; } };

class CTCInventoryExperience { public:
    CThing* ParentThing;
    CTutorialSoundHandleMap TutorialSoundHandles;
    long OrigGeneral, OrigStrength, OrigSkill, OrigWill;
    float AccumFadeTime;
    virtual void SetScreenActive(long active) { g_active = (int)active; }
    void PrepareScreen() { ++g_prepared; }
    void OpenInventory();
};

void CTCInventoryExperience::OpenInventory()
{
    OnOpenMenus();
    NUISystem::CManager::GetInstance()->OnThingInventoryOpened(ParentThing);
    PrepareScreen();
    AccumFadeTime = 0.0f;
    g_pMainGameComponent->World->Hud->SetEnabled(false);
    CTCHeroExperience* experience;
    ParentThing->GetInterface(&experience, 0x68);
    if (g_pSoundSystem) {
        g_pSoundSystem->RefreshCategory(0x10, 0.0f, 500.0f);
        g_pSoundSystem->RefreshCategory(0x80, 0.0f, 500.0f);
        TutorialSoundHandles.clear();
    }
    OrigGeneral = experience->GeneralExperience;
    OrigStrength = experience->GetStatExperience(0);
    OrigSkill = experience->GetStatExperience(1);
    OrigWill = experience->GetStatExperience(2);
    SetScreenActive(1);
}

int main()
{
    CTCHeroExperience exp = { 1000, { 10, 20, 30 } };
    CThing hero = { &exp };
    CTCInventoryExperience screen;
    screen.ParentThing = &hero; screen.AccumFadeTime = 3.0f;
    screen.OpenInventory();
    if (g_menus != 1 || g_notified != 1 || g_prepared != 1 || g_hudOff != 1 || g_refresh != 2 || g_cleared != 1 ||
        g_active != 1 || screen.AccumFadeTime != 0.0f || screen.OrigGeneral != 1000 || screen.OrigStrength != 10 ||
        screen.OrigSkill != 20 || screen.OrigWill != 30) {
        std::printf("0056fad7_TEST FAIL\n"); return 1;
    }
    g_pSoundSystem = 0;
    screen.OpenInventory();
    if (g_refresh != 2 || g_cleared != 1 || g_menus != 2) { std::printf("0056fad7_TEST FAIL\n"); return 1; }
    std::printf("0056fad7_TEST PASS\n");
    return 0;
}