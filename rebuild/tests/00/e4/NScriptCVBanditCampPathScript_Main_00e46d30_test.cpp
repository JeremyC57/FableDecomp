// Behaviour test for NScript::CV_BanditCampPathScript::Main (0x00E46D30): bind BCP_Bandit, wait for BanditCampPath_1, take the hero,
// fade out, drop the raid quest, save marker + auto-save, pick the two eavesdroppers out of the bandit list by data string, play the
// overheard cut-scene once while both live (OverheardBanditConversation), release the hero, wait for the full five-piece outfit, wait
// for the level to unload, deactivate the quest. A state model, run with and without the cut-scene.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Thing { const char* Data; bool Alive; };
struct Village {
    bool OverheardBanditConversation;
    Thing Bandits[4]; unsigned int BanditCount;
    Thing Bandit1, Bandit2;
    int Frames; bool Terminate;
    bool IsLevelLoaded(const char*) { return Frames >= 1 && Frames < 6; }
    bool HasItem(int piece) { return Frames >= 3 + (piece == 4 ? 1 : 0); }
    void NewScriptFrame() { ++Frames; }
    bool IsActiveThreadTerminating() { return Terminate; }
    void Main()
    {
        Log("bind"); Log("post");
        while (!IsLevelLoaded("BanditCampPath_1")) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("seh_hero"); Log("fade_out");
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Log("-raid"); Log("marker"); Log("autosave");
        for (unsigned int i = 0; i < BanditCount; ++i) {
            if (!std::strcmp(Bandits[i].Data, "BCP_EavesDropBandit1")) { if (IsActiveThreadTerminating()) return; Bandit1 = Bandits[i]; }
            else if (!std::strcmp(Bandits[i].Data, "BCP_EavesDropBandit2")) { if (IsActiveThreadTerminating()) return; Bandit2 = Bandits[i]; }
        }
        if (Bandit1.Alive && Bandit2.Alive && !OverheardBanditConversation) {
            if (IsActiveThreadTerminating()) return;
            Log("CS_BANDITPATH_OVERHEARD");
            OverheardBanditConversation = true;
        }
        Log("release");
        while (!(HasItem(0) && HasItem(1) && HasItem(2) && HasItem(3) && HasItem(4))) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        while (!(!IsLevelLoaded("BanditCampPath_1"))) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("deactivate_later");
    }
};
int main()
{
    Village v = { false, { { "BCP_Bandit", true }, { "BCP_EavesDropBandit2", true }, { "BCP_EavesDropBandit1", true }, { "", true } }, 4,
                  { "", false }, { "", false }, 0, false };
    v.Main();
    bool ok = !std::strcmp(g_log, "bind;post;seh_hero;fade_out;-raid;marker;autosave;CS_BANDITPATH_OVERHEARD;release;deactivate_later;")
        && v.OverheardBanditConversation && !std::strcmp(v.Bandit1.Data, "BCP_EavesDropBandit1")
        && !std::strcmp(v.Bandit2.Data, "BCP_EavesDropBandit2") && v.Frames == 6;
    g_log[0] = 0;
    Village w = { true, { { "BCP_EavesDropBandit1", true }, { "BCP_EavesDropBandit2", true }, { "", true }, { "", true } }, 2,
                  { "", false }, { "", false }, 0, false };
    w.Main();
    ok = ok && !std::strcmp(g_log, "bind;post;seh_hero;fade_out;-raid;marker;autosave;release;deactivate_later;");
    g_log[0] = 0;
    Village x = { false, { { "BCP_EavesDropBandit1", false }, { "BCP_EavesDropBandit2", true }, { "", true }, { "", true } }, 2,
                  { "", false }, { "", false }, 0, false };
    x.Main();
    ok = ok && !std::strcmp(g_log, "bind;post;seh_hero;fade_out;-raid;marker;autosave;release;deactivate_later;") && !x.OverheardBanditConversation;
    std::printf(ok ? "00e46d30_TEST PASS\n" : "00e46d30_TEST FAIL\n");
    return ok ? 0 : 1;
}