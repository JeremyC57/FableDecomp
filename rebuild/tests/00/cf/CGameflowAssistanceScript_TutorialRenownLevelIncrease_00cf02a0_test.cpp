// behaviour test for CGameflowAssistanceScript::Tutorial_RenownLevelIncrease (0x00CF02A0): level-up announcements and quest-card removal
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* Text;
    CCharString() : Text("") {}
    CCharString(const char* t) : Text(t) {}
};
struct CGameflowMasterData { bool StopAssistance; };
static int g_frame, g_renown[32], g_limit, g_removed, g_msgs; static const char* g_msg[16]; static CGameflowMasterData* g_data;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frame; if (g_frame >= g_limit) g_data->StopAssistance = true; }
    long GetHeroRenownLevel() const { return g_renown[g_frame]; }
    void HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString&, bool, bool) const { g_msg[g_msgs++] = text.Text; }
    void RemoveQuestCardFromGuild(const CCharString& n) const { if (std::strcmp(n.Text, "DUMMY_QUEST_HAS_NO_SCRIPT") == 0) ++g_removed; }
    bool IsHeroControlledByPlayer() const { return true; }
};
struct CGameflowAssistanceScript {
    CGameScriptInterface* GSI; CGameflowMasterData* PMasterData; long SavedRenownLevel;
    bool IsActiveThreadTerminating() { return false; }
    void Run()
    {
        CCharString messages[8] = { "TEXT_QST_080_RENOWN_LEVEL_ONE", "TEXT_QST_080_RENOWN_LEVEL_TWO", "TEXT_QST_080_RENOWN_LEVEL_THREE",
            "TEXT_QST_080_RENOWN_LEVEL_FOUR", "TEXT_QST_080_RENOWN_LEVEL_FIVE", "TEXT_QST_080_RENOWN_LEVEL_SIX", "TEXT_QST_080_RENOWN_LEVEL_SEVEN" };
        if (!IsActiveThreadTerminating())
        for (;;) {
            if (PMasterData->StopAssistance) { if (IsActiveThreadTerminating()) return; return; }
            long renown = GSI->GetHeroRenownLevel();
            if (GSI->IsHeroControlledByPlayer()) {
                if (IsActiveThreadTerminating()) return;
                if (renown > SavedRenownLevel) {
                    if (IsActiveThreadTerminating()) return;
                    SavedRenownLevel = renown;
                    if (renown < 8) {
                        if (IsActiveThreadTerminating()) return;
                        GSI->HeroReceiveMessageFromGuildMaster(messages[renown - 1], "", true, true);
                    }
                    if (renown > 1) {
                        if (IsActiveThreadTerminating()) return;
                        GSI->RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT");
                        GSI->RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT");
                        GSI->RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT");
                    }
                }
            }
            GSI->NewScriptFrame();
            if (!IsActiveThreadTerminating()) continue;
            break;
        }
    }
};
int main()
{
    for (int i = 0; i < 32; ++i) g_renown[i] = i < 2 ? 0 : i < 5 ? 1 : i < 9 ? 3 : 9;
    CGameScriptInterface gsi; CGameflowMasterData data = { false }; g_data = &data; g_limit = 12;
    CGameflowAssistanceScript s; s.GSI = &gsi; s.PMasterData = &data; s.SavedRenownLevel = 0;
    s.Run();
    // level 1 at frame 2 (message ONE), level 3 at frame 5 (THREE + 3 removals), level 9 at frame 9 (no message, 3 removals)
    if (g_msgs != 2 || std::strcmp(g_msg[0], "TEXT_QST_080_RENOWN_LEVEL_ONE") || std::strcmp(g_msg[1], "TEXT_QST_080_RENOWN_LEVEL_THREE")
        || g_removed != 6 || s.SavedRenownLevel != 9 || g_frame != 12) { std::printf("00cf02a0_TEST FAIL\n"); return 1; }
    std::printf("00cf02a0_TEST PASS\n");
    return 0;
}