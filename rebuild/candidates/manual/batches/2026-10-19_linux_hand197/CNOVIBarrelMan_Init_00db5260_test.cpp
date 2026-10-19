#include <cstdio>
#include <cstring>
// Behaviour test for 00db5260 CNOVI_BarrelMan::Init: timer cleared on the global interface, flags/phase reset, the interface
// calls with their exact arguments in order, home position copied into the packed parent, sight radius 10.
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct C3DVector { float X, Y, Z; };
struct CScriptThing {
    int id;
    C3DVector home;
    C3DVector GetHomePos() const { Log("Home"); return home; }
};
struct CGameScriptInterface {
    virtual void SetTimer(long timer, long time) { char b[32]; std::sprintf(b, "T%ld,%ld", timer, time); Log(b); }
    virtual void SetThingHasInformation(const CScriptThing& t, bool a, bool b, bool c) const { Log(t.id == 5 && !a && b && !c ? "Info" : "InfoBAD"); }
    virtual void EntitySetAsDamageable(const CScriptThing& t, bool d) const { Log(t.id == 5 && !d ? "Dmg" : "DmgBAD"); }
    virtual void EntitySetAsKillable(const CScriptThing& t, bool k, bool b) const { Log(t.id == 5 && !k && !b ? "Kill" : "KillBAD"); }
    virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& t, bool a) const { Log(t.id == 5 && !a ? "Combo" : "ComboBAD"); }
    virtual void EntitySetSightRadius(const CScriptThing& t, float r) const { Log(t.id == 5 && r == 10.0f ? "Sight" : "SightBAD"); }
    virtual ~CGameScriptInterface() {}
};
CGameScriptInterface* g_pGameScriptInterface;
#pragma pack(push, 1)
struct CQ_NewOakValeIntroScript {
    unsigned char _pad0[0x85];
    C3DVector     BarrelBrokenPos;
    unsigned char _pad91[0x108 - 0x91];
    long          BarrelTimer;
};
#pragma pack(pop)
struct CNOVI_BarrelMan {
    CGameScriptInterface* GSI;
    CScriptThing Me;
    CQ_NewOakValeIntroScript* ParentClass;
    bool ComplainedAboutStock, HeroLetMeDown, OverheardYet;
    long MyPhase;
    void Init();
};
void CNOVI_BarrelMan::Init()
{
    g_pGameScriptInterface->SetTimer(ParentClass->BarrelTimer, 0);
    ComplainedAboutStock = false;
    MyPhase = 0;
    GSI->EntitySetAsDamageable(Me, false);
    GSI->EntitySetAsKillable(Me, false, false);
    GSI->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    GSI->SetThingHasInformation(Me, false, true, false);
    ParentClass->BarrelBrokenPos = Me.GetHomePos();
    HeroLetMeDown = false;
    OverheardYet = false;
    GSI->EntitySetSightRadius(Me, 10.0f);
}
int main()
{
    CGameScriptInterface global, own;
    g_pGameScriptInterface = &global;
    static CQ_NewOakValeIntroScript parent;
    parent.BarrelTimer = 42;
    CNOVI_BarrelMan m; m.GSI = &own; m.Me.id = 5; m.Me.home.X = 1.5f; m.Me.home.Y = -2.0f; m.Me.home.Z = 3.25f;
    m.ParentClass = &parent; m.ComplainedAboutStock = m.HeroLetMeDown = m.OverheardYet = true; m.MyPhase = 3;
    g_log[0] = 0;
    m.Init();
    bool ok = std::strcmp(g_log, "T42,0;Dmg;Kill;Combo;Info;Home;Sight;") == 0;
    ok = ok && !m.ComplainedAboutStock && !m.HeroLetMeDown && !m.OverheardYet && m.MyPhase == 0;
    C3DVector p; std::memcpy(&p, (char*)&parent + 0x85, sizeof(p));
    ok = ok && p.X == 1.5f && p.Y == -2.0f && p.Z == 3.25f;
    if (!ok) { std::printf("00db5260_TEST FAIL\n"); return 1; }
    std::printf("00db5260_TEST PASS\n");
    return 0;
}
