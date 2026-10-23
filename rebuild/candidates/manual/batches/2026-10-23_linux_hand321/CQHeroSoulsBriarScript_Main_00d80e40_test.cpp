#include <cstdio>
#include <cstring>
// behaviour test: CQ_HeroSoulsBriarScript::Main -- bindings, first-run objective/activation, the Northern Wastes wait,
// Jack's lines, the boss-phase waits (phase or health under 50), Briar's death and the quest completion
static int g_frames, g_endAt, g_csAt, g_screenAt, g_phase1At, g_phase2At, g_deadAt;
static int g_bound, g_obj7, g_activated, g_saves, g_fades, g_lines, g_teleOn, g_teleOff, g_cut, g_deact, g_log, g_obj11, g_completed;
static float g_morality;
static bool g_wastesLoaded;
struct CScriptThing { int id; };
struct CMasterData { bool KilledBriar; long DeliveredSoul; };
struct CScriptDef { float HS_GoodSoulTaken; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;
struct CGameScriptInterface {
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frames; }
    bool IsRegionLoaded(const char* n) const { return g_wastesLoaded && std::strcmp(n, "NorthernWastes3") == 0; }
    void PostAddScriptedEntities() const {}
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.id = std::strcmp(n, "BriarRose") == 0 ? 7 : 0; return t; }
    void GiveHeroMorality(float m) const { g_morality += m; }
    float GetHealth(const CScriptThing&) const { return 100.0f; }
    void ActivateQuest(const char* q) const { if (!std::strcmp(q, "Q_HeroSoulsMother")) ++g_activated; }
    void DeactivateQuest(const char* q, unsigned long f) const { if (!std::strcmp(q, "Q_HeroSoulsBriarMagicBarrier") && f == 0) ++g_deact; }
    void SetQuestAsCompleted(const char* q, bool a, bool b, bool c) const { if (!std::strcmp(q, "Q_HeroSouls") && a && b && c) ++g_completed; }
    void SetQuestCardObjective(const char* q, const char* t, const char* r, const char* l) const
    {
        if (std::strcmp(q, "Q_HeroSouls") != 0) return;
        if (!std::strcmp(t, "TEXT_QUEST_HERO_SOULS_OBJECTIVE_07") && !std::strcmp(r, "OakvaleMemorialGarden") && !std::strcmp(l, "OakBay")) ++g_obj7;
        if (!std::strcmp(t, "TEXT_QUEST_HERO_SOULS_OBJECTIVE_11") && !std::strcmp(r, "HeroGuildComplexInside") && !std::strcmp(l, "HeroGuildComplexInside")) ++g_obj11;
    }
    long AddNewConversation(const CScriptThing&, bool a, bool b) { return (!a && !b) ? 3 : -1; }
    void AddLineToConversation(long c, const char* t, bool, const CScriptThing&, const CScriptThing&) { if (c == 3 && std::strncmp(t, "TEXT_QST_B04_JACK_MSG_", 22) == 0) ++g_lines; }
    void FadeScreenIn() { ++g_fades; }
    void Pause(float) const { ++g_frames; }
    long EntityGetBossPhase(const CScriptThing& t) const { if (t.id != 7) return 0; return g_frames >= g_phase2At ? 2 : g_frames >= g_phase1At ? 1 : 0; }
    void SetTeleportingAsActive(bool a) const { if (a) ++g_teleOn; else ++g_teleOff; }
    const char* GetActiveQuestName() const { return "Q_HeroSouls"; }
    void AutoSaveQuestStart() const { ++g_saves; }
};
struct CQ_HeroSoulsBriarScript {
    CGameScriptInterface* Game; CMasterData* PMasterData; bool PlayedBriarCS; bool KickOffQuestStartScreen;
    bool IsActiveThreadTerminating()
    {
        if (g_frames >= g_csAt) PlayedBriarCS = true;
        if (g_frames >= g_screenAt) KickOffQuestStartScreen = true;
        if (g_frames >= g_deadAt) PMasterData->KilledBriar = true;
        return g_frames >= g_endAt;
    }
    void AddEntityScriptBinding(const char*) { ++g_bound; }
    void PostAddScriptedEntities() {}
    void PlayCutscene(const char* n) { if (!std::strcmp(n, "CS_HEROSOULS_SOUL3_INTRO_JACK")) ++g_cut; }
    void Main();
};
static void AddLogbookStoryEntryString(const char* t) { if (!std::strcmp(t, "TEXT_QST_LOG_STORY_PLATINUM_365")) ++g_log; }
void CQ_HeroSoulsBriarScript::Main()
{
    AddEntityScriptBinding("BriarRose");
    AddEntityScriptBinding("BRIAR_ROSE_IMPOSTER");
    AddEntityScriptBinding("HSB_MagicBarrier");
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (!PlayedBriarCS) {
        if (IsActiveThreadTerminating()) return;
        while (!PlayedBriarCS) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_07", "OakvaleMemorialGarden", "OakBay");
        Game->ActivateQuest("Q_HeroSoulsMother");
        Game->AutoSaveQuestStart();
        Game->FadeScreenIn();
    }
    while (Game->IsRegionLoaded("NorthernWastes3") && !KickOffQuestStartScreen) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (!KickOffQuestStartScreen) {
        if (IsActiveThreadTerminating()) return;
        Game->Pause(1.0f);
        long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_SECOND_SOUL", false, Game->GetHero(), Game->GetHero());
    }
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_07", "OakvaleMemorialGarden", "OakBay");
    while (!KickOffQuestStartScreen) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->SetTeleportingAsActive(false);
    CScriptThing briar = Game->GetThingWithScriptName("BriarRose");
    while (Game->EntityGetBossPhase(briar) != 1 && !(Game->GetHealth(briar) < 50.0f)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->Pause(4.0f);
    long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_BRIAR_TAUNT_10", false, Game->GetHero(), Game->GetHero());
    while (Game->EntityGetBossPhase(briar) != 2 && !(Game->GetHealth(briar) < 50.0f)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    conversation = Game->AddNewConversation(Game->GetHero(), false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_BRIAR_TAUNT_20", false, Game->GetHero(), Game->GetHero());
    while (!PMasterData->KilledBriar) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->SetTeleportingAsActive(true);
    if (!PMasterData->KilledBriar) return;
    if (IsActiveThreadTerminating()) return;
    PlayCutscene("CS_HEROSOULS_SOUL3_INTRO_JACK");
    Game->DeactivateQuest("Q_HeroSoulsBriarMagicBarrier", 0);
    ++PMasterData->DeliveredSoul;
    AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_365");
    Game->GiveHeroMorality(g_pScriptDef->HS_GoodSoulTaken);
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_11", "HeroGuildComplexInside", "HeroGuildComplexInside");
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
    Game->FadeScreenIn();
}
static void Reset(int cs, int screen, int p1, int p2, int dead, int end)
{
    g_frames = 0; g_csAt = cs; g_screenAt = screen; g_phase1At = p1; g_phase2At = p2; g_deadAt = dead; g_endAt = end;
    g_bound = g_obj7 = g_activated = g_saves = g_fades = g_lines = g_teleOn = g_teleOff = g_cut = g_deact = g_log = g_obj11 = g_completed = 0;
    g_morality = 0.0f;
}
int main()
{
    bool ok = true;
    g_def.HS_GoodSoulTaken = 2.5f;
    CGameScriptInterface game; game.hero.id = 1;
    CMasterData data;
    CQ_HeroSoulsBriarScript s; s.Game = &game; s.PMasterData = &data;
    g_wastesLoaded = false; Reset(2, 5, 8, 11, 14, 1000); data.KilledBriar = false; data.DeliveredSoul = 1; s.PlayedBriarCS = false; s.KickOffQuestStartScreen = false;
    s.Main();
    ok = ok && g_bound == 3 && g_obj7 == 2 && g_activated == 1 && g_saves == 1 && g_lines == 3 && g_teleOff == 1 && g_teleOn == 1
            && g_cut == 1 && g_deact == 1 && data.DeliveredSoul == 2 && g_log == 1 && g_morality == 2.5f && g_obj11 == 1
            && g_completed == 1 && g_fades == 2;
    // second run: cutscene already played and the start screen already kicked off -> no first-run block, no second-soul line
    g_wastesLoaded = true; Reset(0, 0, 0, 1, 0, 1000); data.KilledBriar = false; s.PlayedBriarCS = true; s.KickOffQuestStartScreen = true;
    s.Main();
    ok = ok && g_activated == 0 && g_obj7 == 1 && g_lines == 2 && g_completed == 1 && data.DeliveredSoul == 3;
    // the thread ends during the boss fight
    Reset(0, 0, 50, 60, 70, 4); data.KilledBriar = false; s.PlayedBriarCS = true; s.KickOffQuestStartScreen = true;
    s.Main();
    ok = ok && g_teleOff == 1 && g_teleOn == 0 && g_completed == 0 && data.DeliveredSoul == 3;
    if (!ok) { std::printf("00d80e40_TEST FAIL\n"); return 1; }
    std::printf("00d80e40_TEST PASS\n");
    return 0;
}
