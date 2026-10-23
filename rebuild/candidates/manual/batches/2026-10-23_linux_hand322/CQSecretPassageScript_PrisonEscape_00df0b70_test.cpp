#include <cstdio>
#include <cstring>
// behaviour test: CQ_SecretPassageScript::PrisonEscape -- wait for the passage and the marker, cutscene, remove live
// followers only, region-follow + remove the mother, give the quest card
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frame = 0;
struct Thing { const char* name; bool alive; };
static Thing g_followers[3] = { { "f0", true }, { "f1", false }, { "f2", true } };
struct Game {
    void NewScriptFrame() { ++g_frame; }
    bool IsLevelLoaded() const { return g_frame >= 2; }
    bool HeroNearMarker() const { return g_frame >= 4; }
    void EntityStopFollowing(const char* n) { L("stop"); L(n); }
    void Cutscene(const char* n) { L(n); }
    void FadeScreenOut(float t, float h, unsigned char r, unsigned char g, unsigned char b, unsigned char a) { if (t == 0.5f && h == 0.0f && !r && !g && !b && a == 255) L("fade"); }
    int GetFollowingEntityList(Thing** out) { *out = g_followers; return 3; }
    void RemoveThing(const char* n, bool a, bool b) { if (!a && b) { L("remove"); L(n); } }
    void SetEntityAsRegionFollowing(const char* leader, const char* follower, bool on) { L(leader); L(follower); L(on ? "on" : "off"); }
    void GiveHeroQuestCardDirectly(const char* card, const char* quest, bool show) { L(card); L(quest); if (show) L("bad"); }
};
struct Script {
    Game* G; long HenchmanOverrideState;
    bool IsActiveThreadTerminating() { return false; }
    void Run()
    {
        while (!G->IsLevelLoaded()) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        while (!G->HeroNearMarker()) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        const char* mother = "MotherTorture";
        G->EntityStopFollowing(mother);
        G->Cutscene("CS_JACK_CAPTURES");
        G->FadeScreenOut(0.5f, 0.0f, 0, 0, 0, 255);
        Thing* f; int n = G->GetFollowingEntityList(&f);
        HenchmanOverrideState = 2;
        for (int i = 0; i < n; ++i) if (f[i].alive) G->RemoveThing(f[i].name, false, true);
        G->SetEntityAsRegionFollowing("hero", mother, false);
        G->RemoveThing(mother, false, true);
        G->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_PRISON_ESCAPE", "Q_PrisonEscapeRescueMother", false);
    }
};
int main()
{
    Game g; Script s; s.G = &g; s.HenchmanOverrideState = 0;
    s.Run();
    if (std::strcmp(g_log, "stop;MotherTorture;CS_JACK_CAPTURES;fade;remove;f0;remove;f2;hero;MotherTorture;off;remove;MotherTorture;OBJECT_QUEST_CARD_PRISON_ESCAPE;Q_PrisonEscapeRescueMother;") != 0
        || s.HenchmanOverrideState != 2 || g_frame != 4) {
        std::printf("00df0b70_TEST FAIL\n"); return 1;
    }
    std::printf("00df0b70_TEST PASS\n");
    return 0;
}
