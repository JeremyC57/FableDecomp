// Behaviour test for CDarkwoodRockTroll::OnPredicateFail (0x00E03B00): the objective is redirected only when the
// troll was killed AND the parent's flag is set; the flag is then cleared.
#include <cstdio>
#include <cstring>
struct CCharString {
    char text[64];
    CCharString(const char* t, long = -1) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
    ~CCharString() {}
};
static int g_objectiveCalls = 0;
static char g_args[4][64];
struct CGameScriptInterface {
    virtual void SetQuestCardObjective(const CCharString& q, const CCharString& t, const CCharString& r, const CCharString& r2) const
    {
        ++g_objectiveCalls;
        std::strcpy(g_args[0], q.text); std::strcpy(g_args[1], t.text); std::strcpy(g_args[2], r.text); std::strcpy(g_args[3], r2.text);
    }
    virtual CCharString GetActiveQuestName() const { return CCharString("Q_ESCORT"); }
};
struct CScriptThing {
    bool killed;
    virtual bool MsgIsKilledBy(const CCharString& killer) const { return killed && killer.text[0] == 0; }
};
struct CQ_TraderEscortScript { bool TrollAttackPending; };
class CDarkwoodRockTroll {
public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    CQ_TraderEscortScript* ParentClass;
    void OnPredicateFail();
};
void CDarkwoodRockTroll::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("") && ParentClass->TrollAttackPending)
    {
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields");
        ParentClass->TrollAttackPending = false;
    }
}
int main()
{
    CGameScriptInterface game; CQ_TraderEscortScript parent; CDarkwoodRockTroll t;
    t.Game = &game; t.ParentClass = &parent;
    int bad = 0;
    t.Me.killed = false; parent.TrollAttackPending = true; t.OnPredicateFail();
    if (g_objectiveCalls != 0 || !parent.TrollAttackPending) bad = 1;
    t.Me.killed = true; parent.TrollAttackPending = false; t.OnPredicateFail();
    if (g_objectiveCalls != 0) bad = 2;
    parent.TrollAttackPending = true; t.OnPredicateFail();
    if (g_objectiveCalls != 1 || parent.TrollAttackPending) bad = 3;
    if (std::strcmp(g_args[0], "Q_ESCORT") || std::strcmp(g_args[1], "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03")
        || std::strcmp(g_args[2], "BarrowFields") || std::strcmp(g_args[3], "BarrowFields")) bad = 4;
    if (bad) { std::printf("00e03b00_TEST FAIL\n"); return 1; }
    std::printf("00e03b00_TEST PASS\n");
    return 0;
}