#include <cstdio>
#include <cstring>
// Behaviour test for 00daed30 CNOVI_Bully::Init: member reset values, parent flag cleared, interface calls in order with args.
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing { int id; };
struct CGameScriptInterface {
    CScriptThing hero;
    virtual CScriptThing& GetHero() { Log("Hero"); return hero; }
    virtual void SetThingHasInformation(const CScriptThing& t, bool a, bool b, bool c) const { Log(t.id == 5 && !a && !b && !c ? "Info" : "InfoBAD"); }
    virtual void EntitySetAsDamageable(const CScriptThing& t, bool d) const { Log(t.id == 5 && !d ? "Dmg" : "DmgBAD"); }
    virtual void EntitySetAsKillable(const CScriptThing& t, bool k, bool b) const { Log(t.id == 5 && !k && !b ? "Kill" : "KillBAD"); }
    virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& t, bool a) const { Log(t.id == 5 && !a ? "Combo" : "ComboBAD"); }
    virtual void EntitySetThingAsAllyOfThing(const CScriptThing& t, const CScriptThing& a) const { Log(t.id == 5 && a.id == 9 ? "Ally" : "AllyBAD"); }
    virtual void SetIsPushableByHero(CScriptThing t, bool p) { Log(t.id == 5 && !p ? "Push" : "PushBAD"); }
    virtual ~CGameScriptInterface() {}
};
struct CQ_NewOakValeIntroScript { bool BullySubdued; };
struct CNOVI_Bully {
    CGameScriptInterface* GSI;
    CScriptThing Me;
    CQ_NewOakValeIntroScript* ParentClass;
    long InitialHealth, HitsTaken;
    bool DoneIntro, SaidPieceAboutAttackingVictim, SpokenOnFirstProximity;
    long IntimidateSpeechLoop;
    void Init();
};
void CNOVI_Bully::Init()
{
    DoneIntro = false;
    HitsTaken = 0;
    InitialHealth = 4;
    SpokenOnFirstProximity = false;
    ParentClass->BullySubdued = false;
    SaidPieceAboutAttackingVictim = false;
    IntimidateSpeechLoop = 10;
    GSI->EntitySetAsDamageable(Me, false);
    GSI->EntitySetAsKillable(Me, false, false);
    GSI->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    GSI->SetThingHasInformation(Me, false, false, false);
    GSI->EntitySetThingAsAllyOfThing(Me, GSI->GetHero());
    GSI->SetIsPushableByHero(Me, false);
}
int main()
{
    CGameScriptInterface g; g.hero.id = 9;
    CQ_NewOakValeIntroScript parent; parent.BullySubdued = true;
    CNOVI_Bully b; b.GSI = &g; b.Me.id = 5; b.ParentClass = &parent;
    b.InitialHealth = 1; b.HitsTaken = 3; b.DoneIntro = b.SaidPieceAboutAttackingVictim = b.SpokenOnFirstProximity = true; b.IntimidateSpeechLoop = 0;
    g_log[0] = 0;
    b.Init();
    bool ok = std::strcmp(g_log, "Dmg;Kill;Combo;Info;Hero;Ally;Push;") == 0;
    ok = ok && b.InitialHealth == 4 && b.HitsTaken == 0 && !b.DoneIntro && !b.SaidPieceAboutAttackingVictim && !b.SpokenOnFirstProximity;
    ok = ok && b.IntimidateSpeechLoop == 10 && !parent.BullySubdued;
    if (!ok) { std::printf("00daed30_TEST FAIL\n"); return 1; }
    std::printf("00daed30_TEST PASS\n");
    return 0;
}