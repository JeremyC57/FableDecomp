// Behaviour test for 0x00EB3D10 CMurderTwistAssassin::OnPredicateFail: only when the assassin is killed: quest state 4,
// morality, renown and the reward object from the script def, then the active quest card's objective is set.
#include <cstdio>
#include <cstring>

static int g_log[8]; static int g_n; static float g_morality; static long g_renown; static const char* g_object; static long g_count; static bool g_show;
static const char* g_obj[4];
struct CScriptDef { float MurderTwistMorality; long MurderTwistRenown; const char* MurderTwistReward; };
static CScriptDef g_def = { -25.0f, 40, "OBJECT_REWARD" };
static CScriptDef* g_pScriptDef = &g_def;
struct CGameScriptInterface {
    void GiveHeroMorality(float m) { g_log[g_n++] = 156; g_morality = m; }
    void GiveHeroRenownPoints(long r) { g_log[g_n++] = 161; g_renown = r; }
    void GiveHeroObject(const char* o, long c, bool s) { g_log[g_n++] = 121; g_object = o; g_count = c; g_show = s; }
    const char* GetActiveQuestName() const { return "Q_MURDER_TWIST"; }
    void SetQuestCardObjective(const char* q, const char* a, const char* b, const char* c) { g_log[g_n++] = 296; g_obj[0] = q; g_obj[1] = a; g_obj[2] = b; g_obj[3] = c; }
};
struct CScriptThing { bool killed; bool MsgIsKilledBy(const char* who) const { return killed && !std::strcmp(who, ""); } };
struct CScript { long State; };
struct CMurderTwistAssassin {
    CGameScriptInterface* Game; CScriptThing Me; CScript* ParentClass;
    void OnPredicateFail()
    {
        if (Me.MsgIsKilledBy("")) {
            ParentClass->State = 4;
            Game->GiveHeroMorality(g_pScriptDef->MurderTwistMorality);
            Game->GiveHeroRenownPoints(g_pScriptDef->MurderTwistRenown);
            Game->GiveHeroObject(g_pScriptDef->MurderTwistReward, -1, false);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MURDER_TWIST_OBJECTIVE_RETURN", "OakBay", "");
        }
    }
};
int main()
{
    CGameScriptInterface gsi; CScript quest = { 1 };
    CMurderTwistAssassin a; a.Game = &gsi; a.Me.killed = false; a.ParentClass = &quest;
    a.OnPredicateFail();
    if (g_n != 0 || quest.State != 1) { std::printf("00eb3d10_TEST FAIL\n"); return 1; }
    a.Me.killed = true; a.OnPredicateFail();
    if (g_n != 4 || g_log[0] != 156 || g_log[1] != 161 || g_log[2] != 121 || g_log[3] != 296 || quest.State != 4) { std::printf("00eb3d10_TEST FAIL\n"); return 1; }
    if (g_morality != -25.0f || g_renown != 40 || std::strcmp(g_object, "OBJECT_REWARD") || g_count != -1 || g_show) { std::printf("00eb3d10_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_obj[0], "Q_MURDER_TWIST") || std::strcmp(g_obj[1], "TEXT_QUEST_MURDER_TWIST_OBJECTIVE_RETURN") || std::strcmp(g_obj[2], "OakBay") || std::strcmp(g_obj[3], "")) { std::printf("00eb3d10_TEST FAIL\n"); return 1; }
    std::printf("00eb3d10_TEST PASS\n");
    return 0;
}
