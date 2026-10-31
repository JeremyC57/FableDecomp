// Behaviour test for CWorld::ResetToFrontEnd @ 0x0049DCC0: mute sounds, build legacy/new front end, Init it, queue it, quit main.
#include <cstdio>
static int g_log[16]; static int g_n;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
class CGame { public: int id; };
static CGame g_Game;
static bool g_bUseLegacyFrontend;
struct CGameScriptInterface_SoundProvider { void** vtable; };
struct CFlaggedObject_Retail {
    void** vtable; bool Enabled;
    void func_00991620(bool enable) { if (Enabled != enable) { Enabled = enable; Log(enable ? 11 : 10); } }
};
static CGameScriptInterface_SoundProvider* DAT_013B8394;
class CGameComponent { public:
    int kind; int inited; CGame* game;
    CGameComponent() : kind(0), inited(0), game(0) {}
    virtual ~CGameComponent() {}
    virtual void Init() { inited = 1; Log(20 + kind); }
    virtual void SetQuit() { Log(30); }
};
class CFrontendGameComponentInit { public: long Nowt; };
class CNewFrontendGameComponentInit { public: long Nothing; };
class CFrontendGameComponent : public CGameComponent { public:
    CFrontendGameComponent(CGame& g, const CFrontendGameComponentInit& init) { kind = 1; game = &g; if (init.Nowt != 0) kind = 99; }
};
class CNewFrontendGameComponent : public CGameComponent { public:
    CNewFrontendGameComponent(CGame& g, const CNewFrontendGameComponentInit& init) { kind = 2; game = &g; if (init.Nothing != 0) kind = 99; }
};
class CMainGameComponent : public CGameComponent { public:
    CGameComponent* NextComponentToRun; int quit;
    CMainGameComponent() : NextComponentToRun(0), quit(0) {}
    virtual void SetQuit() { quit = 1; Log(30); }
};
class CWorld { public:
    CMainGameComponent* Component;
    void ResetToFrontEnd()
    {
        if (DAT_013B8394)
            reinterpret_cast<CFlaggedObject_Retail*>(DAT_013B8394)->func_00991620(false);
        CGameComponent* frontEnd;
        if (g_bUseLegacyFrontend) {
            CFrontendGameComponentInit init; init.Nowt = 0;
            frontEnd = new CFrontendGameComponent(g_Game, init);
            frontEnd->Init();
        } else {
            CNewFrontendGameComponentInit init; init.Nothing = 0;
            frontEnd = new CNewFrontendGameComponent(g_Game, init);
            frontEnd->Init();
        }
        CMainGameComponent* component = Component;
        component->NextComponentToRun = frontEnd;
        component->SetQuit();
    }
};
int main()
{
    CFlaggedObject_Retail snd; snd.vtable = 0; snd.Enabled = true; DAT_013B8394 = reinterpret_cast<CGameScriptInterface_SoundProvider*>(&snd);
    CMainGameComponent main1; CWorld w; w.Component = &main1;
    g_bUseLegacyFrontend = true; g_n = 0;
    w.ResetToFrontEnd();
    bool ok = snd.Enabled == false && main1.quit == 1 && main1.NextComponentToRun && main1.NextComponentToRun->kind == 1 &&
              main1.NextComponentToRun->inited == 1 && main1.NextComponentToRun->game == &g_Game &&
              g_n == 3 && g_log[0] == 10 && g_log[1] == 21 && g_log[2] == 30;
    delete main1.NextComponentToRun;
    DAT_013B8394 = 0; CMainGameComponent main2; w.Component = &main2;
    g_bUseLegacyFrontend = false; g_n = 0;
    w.ResetToFrontEnd();
    ok = ok && main2.NextComponentToRun && main2.NextComponentToRun->kind == 2 && main2.quit == 1 && g_n == 2 && g_log[0] == 22;
    delete main2.NextComponentToRun;
    if (!ok) { std::printf("0049dcc0_TEST FAIL\n"); return 1; }
    std::printf("0049dcc0_TEST PASS\n");
    return 0;
}