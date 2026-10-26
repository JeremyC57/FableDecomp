// Behaviour test for 004387f0 (CPlayerGui::RenderUpdateGuiTimers): GUI timer at 0.5/0.5, seven stat bars at the def's times,
// quest-info timer unless in mode 1, secondary fade with a forced mode 2 when flagged; the def is loaded once.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CCharString { public:
    const char* text;
    CCharString(const char* t, long) : text(t) {}
    ~CCharString() {}
};
class CPlayerGuiDef { public:
    unsigned char _pad000[0x4e4];
    float         StatBarOpenTime;
    unsigned char _pad4e8[0x4f0 - 0x4e8];
    float         StatBarCloseTime;
    unsigned char _pad4f4[0x584 - 0x4f4];
    float         QuestInfoFadeTime;
};
struct CPlayerGuiDefPointer { CPlayerGuiDef* Object; };
CPlayerGuiDefPointer g_PlayerGuiDef = { 0 };
static CPlayerGuiDef s_def;
static int s_loads = 0;
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CPlayerGuiDefPointer* out)
    { if (std::strcmp(name.text, "PLAYER_GUI_PC") == 0) { out->Object = &s_def; ++s_loads; } return true; }
};
static CDefinitionManager s_mgr;
class CGameDefinitionManager { public: static CDefinitionManager& __fastcall Get() { return s_mgr; } };
__forceinline const CPlayerGuiDef* GetPlayerGuiDef()
{
    if (!g_PlayerGuiDef.Object) {
        CCharString name("PLAYER_GUI_PC", -1);
        CGameDefinitionManager::Get().GetDef(name, &g_PlayerGuiDef);
    }
    return g_PlayerGuiDef.Object;
}
enum EPlayerGuiMode { GUI_MODE_CLOSED, GUI_MODE_1, GUI_MODE_OPENING, GUI_MODE_CLOSING };
namespace NPlayerGui {
class CStatBase { public:                                          // CStatBase (Ego_r.pdb offsets)
    unsigned char  _pad00[0x28];
    EPlayerGuiMode GuiMode;                                 // +0x28
    float          FadeInTimer;                             // +0x2c
};
class CDrawQuestInfo { public:
    unsigned char  _pad00[0x28];
    EPlayerGuiMode GuiMode;                                 // +0x28
    float          FadeInTimer;                             // +0x2c
    float          SecondaryFadeTimer;                      // +0x30 (descriptive)
    bool           ShowingSecondary;                        // +0x34 (descriptive)
};
}
template<class T> class CCountedPointer { public:
    T*    Ptr;
    void* Count;
    T* operator->() const { return Ptr; }
};
class CPlayerGui { public:                                         // retail-offset view (PDB - 8 / - 0x14)
    unsigned char _pad000[0x1c8];
    CCountedPointer<NPlayerGui::CDrawQuestInfo> PDrawQuestInfo;   // +0x1c8 (PDB +0x1d0)
    unsigned char _pad1d0[0x294 - 0x1d0];
    EPlayerGuiMode GuiMode;                                 // +0x294 (PDB +0x2a8)
    float          FadeInTimer;                             // +0x298
    unsigned char _pad29c[0x2cc - 0x29c];
    CCountedPointer<NPlayerGui::CStatBase> PMoney;          // +0x2cc
    CCountedPointer<NPlayerGui::CStatBase> PRenown;         // +0x2d4
    CCountedPointer<NPlayerGui::CStatBase> PMorality;       // +0x2dc
    CCountedPointer<NPlayerGui::CStatBase> PExperience;     // +0x2e4
    CCountedPointer<NPlayerGui::CStatBase> PCrime;          // +0x2ec
    CCountedPointer<NPlayerGui::CStatBase> PStatUpdates;    // +0x2f4
    CCountedPointer<NPlayerGui::CStatBase> PAutoPickups;    // +0x2fc
    struct Call { float dt, open, close; EPlayerGuiMode* mode; EPlayerGuiMode modeVal; float* timer; };
    Call calls[16]; int ncalls;
    void UpdateGuiTimer(float dt, float openTime, float closeTime, EPlayerGuiMode* mode, float* timer)
    { Call c = { dt, openTime, closeTime, mode, *mode, timer }; calls[ncalls++] = c; }
    void RenderUpdateGuiTimers(float dt);
};

void CPlayerGui::RenderUpdateGuiTimers(float dt)
{
    UpdateGuiTimer(dt, 0.5f, 0.5f, &GuiMode, &FadeInTimer);
    const CPlayerGuiDef* def = GetPlayerGuiDef();
    float openTime = def->StatBarOpenTime;
    float closeTime = def->StatBarCloseTime;
    UpdateGuiTimer(dt, openTime, closeTime, &PMoney->GuiMode, &PMoney->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PRenown->GuiMode, &PRenown->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PMorality->GuiMode, &PMorality->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PExperience->GuiMode, &PExperience->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PCrime->GuiMode, &PCrime->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PStatUpdates->GuiMode, &PStatUpdates->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PAutoPickups->GuiMode, &PAutoPickups->FadeInTimer);
    if (PDrawQuestInfo->GuiMode != GUI_MODE_1) {
        UpdateGuiTimer(dt, def->QuestInfoFadeTime, def->QuestInfoFadeTime, &PDrawQuestInfo->GuiMode, &PDrawQuestInfo->FadeInTimer);
        if (PDrawQuestInfo->ShowingSecondary) {
            EPlayerGuiMode mode = GUI_MODE_OPENING;
            UpdateGuiTimer(dt, def->QuestInfoFadeTime, def->QuestInfoFadeTime, &mode, &PDrawQuestInfo->SecondaryFadeTimer);
        }
    }
}

static int fails = 0;
static void check(bool c, int n) { if (!c) { std::printf("fail %d\n", n); ++fails; } }
int main()
{
    s_def.StatBarOpenTime = 1.5f; s_def.StatBarCloseTime = 2.5f; s_def.QuestInfoFadeTime = 0.75f;
    static NPlayerGui::CStatBase bars[7];
    static NPlayerGui::CDrawQuestInfo qi;
    static CPlayerGui gui;
    std::memset(bars, 0, sizeof(bars)); std::memset(&qi, 0, sizeof(qi)); std::memset(&gui, 0, sizeof(gui));
    CCountedPointer<NPlayerGui::CStatBase>* ptrs[7] = { &gui.PMoney, &gui.PRenown, &gui.PMorality, &gui.PExperience, &gui.PCrime, &gui.PStatUpdates, &gui.PAutoPickups };
    for (int i = 0; i < 7; ++i) ptrs[i]->Ptr = &bars[i];
    gui.PDrawQuestInfo.Ptr = &qi;
    qi.GuiMode = GUI_MODE_CLOSED; qi.ShowingSecondary = true;
    gui.RenderUpdateGuiTimers(0.1f);
    check(gui.ncalls == 10 && s_loads == 1, 1);
    check(gui.calls[0].open == 0.5f && gui.calls[0].close == 0.5f && gui.calls[0].mode == &gui.GuiMode && gui.calls[0].timer == &gui.FadeInTimer, 2);
    for (int i = 0; i < 7; ++i)
        check(gui.calls[1 + i].open == 1.5f && gui.calls[1 + i].close == 2.5f && gui.calls[1 + i].mode == &bars[i].GuiMode && gui.calls[1 + i].timer == &bars[i].FadeInTimer && gui.calls[1 + i].dt == 0.1f, 10 + i);
    check(gui.calls[8].open == 0.75f && gui.calls[8].mode == &qi.GuiMode && gui.calls[8].timer == &qi.FadeInTimer, 3);
    check(gui.calls[9].modeVal == GUI_MODE_OPENING && gui.calls[9].timer == &qi.SecondaryFadeTimer && gui.calls[9].close == 0.75f, 4);
    gui.ncalls = 0; qi.ShowingSecondary = false;
    gui.RenderUpdateGuiTimers(0.2f);
    check(gui.ncalls == 9 && s_loads == 1, 5);
    gui.ncalls = 0; qi.GuiMode = GUI_MODE_1;
    gui.RenderUpdateGuiTimers(0.2f);
    check(gui.ncalls == 8, 6);
    if (fails == 0) std::printf("004387f0_TEST PASS\n");
    return fails;
}