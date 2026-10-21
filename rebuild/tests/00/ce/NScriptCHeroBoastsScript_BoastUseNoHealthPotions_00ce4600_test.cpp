// Behaviour test for CHeroBoastsScript::BoastUseNoHealthPotions @ 0x00CE4600 (copy of the model, scripted stub world).
#include <cstdio>
#include <cstring>
static int g_frame, g_doneAt, g_failed, g_completed, g_infos, g_clickAt, g_questFailAt, g_questDoneAt, g_potionAt;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frame; }
    bool MsgOnQuestCompletedBeforeScreenShown(const char* q) const { return !std::strcmp(q, "Q") && g_frame >= g_questDoneAt; }
    bool MsgOnQuestFailedBeforeScreenShown(const char* q) const { return !std::strcmp(q, "Q") && g_frame >= g_questFailAt; }
    bool MsgIsGameInfoClickedPast() const { return g_frame >= g_clickAt; }
    void DisplayGameInfo(const char* t) const { if (!std::strcmp(t, "TEXT_QST_BOAST_STATE_FAILED_USENOHEALTHPOTIONS")) ++g_infos; }
    float GetHeroHealth() const { return g_frame >= g_potionAt ? 90.0f : 50.0f - g_frame; }   // damage, then a potion
    void SetBoastAsFailed(long b, const char* q) const { if (b == 8 && !std::strcmp(q, "Q")) ++g_failed; }
    void SetBoastAsCompleted(long b, const char* q) const { if (b == 8 && !std::strcmp(q, "Q")) ++g_completed; }
};
struct CHeroBoastsScript {
    CGameScriptInterface* GSI;
    bool IsActiveThreadTerminating() { return g_frame >= g_doneAt; }
    void BoastUseNoHealthPotions(const char* questName)
    {
        float lastHealth = GSI->GetHeroHealth();
        while (!IsActiveThreadTerminating()) {
            float health = GSI->GetHeroHealth();
            if (health > lastHealth) {
                if (IsActiveThreadTerminating()) return;
                GSI->DisplayGameInfo("TEXT_QST_BOAST_STATE_FAILED_USENOHEALTHPOTIONS");
                while (!GSI->MsgIsGameInfoClickedPast()) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                GSI->SetBoastAsFailed(8, questName);
                return;
            }
            if (IsActiveThreadTerminating()) return;
            lastHealth = health;
            if (GSI->MsgOnQuestFailedBeforeScreenShown(questName)) { if (IsActiveThreadTerminating()) return; GSI->SetBoastAsFailed(8, questName); return; }
            if (GSI->MsgOnQuestCompletedBeforeScreenShown(questName)) { if (IsActiveThreadTerminating()) return; GSI->SetBoastAsCompleted(8, questName); return; }
            GSI->NewScriptFrame();
        }
    }
};
static void reset(int done, int potion, int click, int fail, int complete)
{ g_frame = 0; g_doneAt = done; g_potionAt = potion; g_clickAt = click; g_questFailAt = fail; g_questDoneAt = complete; g_failed = g_completed = g_infos = 0; }
int main()
{
    int bad = 0;
    CGameScriptInterface gsi; CHeroBoastsScript s; s.GSI = &gsi;
    reset(100, 1000, 0, 1000, 5); s.BoastUseNoHealthPotions("Q"); if (g_completed != 1 || g_failed || g_infos) bad++;
    reset(100, 1000, 0, 4, 1000); s.BoastUseNoHealthPotions("Q"); if (g_failed != 1 || g_completed) bad++;
    reset(100, 3, 7, 1000, 1000); s.BoastUseNoHealthPotions("Q"); if (g_failed != 1 || g_infos != 1 || g_frame != 7) bad++;
    reset(5, 3, 50, 1000, 1000); s.BoastUseNoHealthPotions("Q"); if (g_failed || g_infos != 1) bad++;
    reset(0, 1000, 0, 0, 0); s.BoastUseNoHealthPotions("Q"); if (g_failed || g_completed) bad++;
    if (bad) { std::printf("00ce4600_TEST FAIL\n"); return 1; }
    std::printf("00ce4600_TEST PASS\n");
    return 0;
}