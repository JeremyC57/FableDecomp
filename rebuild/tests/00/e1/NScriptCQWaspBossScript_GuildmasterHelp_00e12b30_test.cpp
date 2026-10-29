// behaviour test for NScript::CQ_WaspBossScript::GuildmasterHelp (0x00E12B30): a copy of the control flow over a mock game
#include <cstdio>
#include <cstring>

class CScriptDef { public: long WB_LowHealth; };
static CScriptDef g_def;
static CScriptDef* pGlobalScriptDef = &g_def;

struct MockGame {
    int frames, phase, messages, pauses;
    float health;
    char last[64];
    const char* log[8];
    MockGame() : frames(0), phase(0), messages(0), pauses(0), health(500.0f) { last[0] = 0; }
    void NewScriptFrame()
    {
        ++frames;
        if (frames == 3) phase = 3;          // the wasp queen moves through her phases over time
        if (frames == 5) phase = 4;
        if (frames == 8) phase = 6;
        if (frames >= 10) health -= 100.0f;
    }
    long EntityGetBossPhase(int) { return phase; }
    float GetHealth(int) { return health; }
    void Pause(float) { ++pauses; }
    void HeroReceiveMessageFromGuildMaster(const char* text, const char*, bool, bool)
    {
        if (messages < 8) log[messages] = text;
        ++messages;
        std::strcpy(last, text);
    }
};

struct Script {
    MockGame* Game;
    bool CutsceneFinished;
    int terminateAtFrame;
    bool IsActiveThreadTerminating() { return terminateAtFrame >= 0 && Game->frames >= terminateAtFrame; }
    void GuildmasterHelp()
    {
        while (!CutsceneFinished) {
            Game->NewScriptFrame();
            CutsceneFinished = true;
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        int queen = 1;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_10", "", true, true);
        while (Game->EntityGetBossPhase(queen) != 3) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->Pause(1.0f);
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_20", "", true, true);
        while (Game->EntityGetBossPhase(queen) != 4) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->Pause(1.0f);
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_30", "", true, true);
        while (Game->EntityGetBossPhase(queen) != 6) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->Pause(1.0f);
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_40", "", true, true);
        CScriptDef* def = pGlobalScriptDef;
        while (Game->GetHealth(queen) != def->WB_LowHealth) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            def = pGlobalScriptDef;
        }
        if (IsActiveThreadTerminating()) return;
        Game->Pause(1.0f);
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_50", "", true, true);
    }
};

int main()
{
    g_def.WB_LowHealth = 200;
    MockGame g1;
    Script s1 = { &g1, false, -1 };
    s1.GuildmasterHelp();
    bool ok = g1.messages == 5 && g1.pauses == 4 && std::strcmp(g1.last, "TEXT_QST_072_GUILDMASTER_GUIDANCE_50") == 0
              && std::strcmp(g1.log[1], "TEXT_QST_072_GUILDMASTER_GUIDANCE_20") == 0 && g1.health == 200.0f;
    MockGame g2;
    Script s2 = { &g2, false, 6 };                    // the thread is terminated while waiting for phase 6
    s2.GuildmasterHelp();
    ok = ok && g2.messages == 3 && g2.pauses == 2;
    if (!ok) { std::printf("00e12b30_TEST FAIL\n"); return 1; }
    std::printf("00e12b30_TEST PASS\n");
    return 0;
}