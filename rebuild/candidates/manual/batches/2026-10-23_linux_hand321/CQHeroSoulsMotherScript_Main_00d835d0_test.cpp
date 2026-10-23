#include <cstdio>
#include <cstring>
// behaviour test: CQ_HeroSoulsMotherScript::Main -- bind two entity scripts, wait for OakBay or the garden; in Oak Bay
// Jack talks until the hero nears the chat marker (re-opening the conversation if it ended), then wait for the garden,
// play the start cutscene once, quest card / start screen / objective, spawn CreateScreamers, fade in, wait for the
// soul and (unless Briar is dead) play the success cutscene and complete the quest
static int g_frames, g_endAt, g_oakBayAt, g_gardenAt, g_nearAt, g_soulAt;
static bool g_convActive, g_briarDead;
static int g_bound, g_lines, g_convs, g_cutStart, g_cutSuccess, g_card, g_screen, g_obj9, g_obj10, g_spawned, g_fades, g_completed, g_log;
struct CScriptThing { int id; };
struct CGameScriptInterface {
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frames; }
    bool IsRegionLoaded(const char* n) const
    {
        if (std::strcmp(n, "OakBay") == 0) return g_oakBayAt >= 0 && g_frames >= g_oakBayAt;
        if (std::strcmp(n, "OakvaleMemorialGarden") == 0) return g_frames >= g_gardenAt;
        return false;
    }
    void PostAddScriptedEntities() const {}
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.id = std::strcmp(n, "MotherJackChatMarker") == 0 ? 5 : 0; return t; }
    long AddNewConversation(const CScriptThing&, bool a, bool b) { ++g_convs; g_convActive = true; return (a && !b) ? 40 + g_convs : -1; }
    void AddLineToConversation(long c, const char* t, bool, const CScriptThing&, const CScriptThing&) { if (c > 40 && std::strncmp(t, "TEXT_QST_B04_JACK_MSG_PRE_MOTHER_", 33) == 0) ++g_lines; }
    bool IsConversationActive(long) const { return g_convActive; }
    void GiveHeroQuestCardDirectly(const char* c, const char* q, bool f) const { if (!std::strcmp(c, "OBJECT_QUEST_CARD_HERO_SOULS_MOTHER") && !std::strcmp(q, "Q_HeroSouls") && f) ++g_card; }
    void KickOffQuestStartScreen(const char* q, bool a, bool b) const { if (!std::strcmp(q, "Q_HeroSouls") && a && !b) ++g_screen; }
    void SetQuestCardObjective(const char* q, const char* t, const char* r, const char* l) const
    {
        if (std::strcmp(q, "Q_HeroSouls") != 0) return;
        if (!std::strcmp(t, "TEXT_QUEST_HERO_SOULS_OBJECTIVE_09") && !std::strcmp(r, "OakvaleMemorialGarden") && !std::strcmp(l, "OakBay")) ++g_obj9;
        if (!std::strcmp(t, "TEXT_QUEST_HERO_SOULS_OBJECTIVE_10") && !std::strcmp(r, "NorthernWastes3") && !std::strcmp(l, "NorthernWastes3")) ++g_obj10;
    }
    void SetQuestAsCompleted(const char* q, bool a, bool b, bool c) const { if (!std::strcmp(q, "Q_HeroSouls") && a && b && c) ++g_completed; }
    void FadeScreenIn() { ++g_fades; }
    void Pause(float s) const { if (s > 1.7f && s < 1.9f) ++g_frames; }
    const char* GetActiveQuestName() const { return "Q_HeroSouls"; }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing&, float d) { return a.id == 5 && d == 50.0f && g_frames >= g_nearAt; }
struct CMasterData { bool KilledBriar; bool CollectedSoulFromMother; };
struct CQ_HeroSoulsMotherScript {
    CGameScriptInterface* Game; CMasterData* PMasterData; bool PlayedMotherCS; bool QuestStartScreened;
    bool IsActiveThreadTerminating() { if (g_frames >= g_soulAt) PMasterData->CollectedSoulFromMother = true; if (g_frames >= 3 && g_nearAt > 0) g_convActive = false; return g_frames >= g_endAt; }
    void AddEntityScriptBinding(const char*) { ++g_bound; }
    void PostAddScriptedEntities() {}
    void SpawnFunction(const char* n) { if (!std::strcmp(n, "CreateScreamers")) ++g_spawned; }
    void PlayCutscene(const char* n) { if (!std::strcmp(n, "CS_HEROSOULS_SOUL2_MOTHER_START")) ++g_cutStart; if (!std::strcmp(n, "CS_HEROSOULS_SOUL2_MOTHER_SUCCESS")) ++g_cutSuccess; }
    void Main();
};
static void AddLogbookStoryEntryString(const char* t) { if (!std::strcmp(t, "TEXT_QST_LOG_STORY_PLATINUM_360")) ++g_log; }
void CQ_HeroSoulsMotherScript::Main()
{
    AddEntityScriptBinding("SummonerToKill");
    AddEntityScriptBinding("HS_Screamer");
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!(Game->IsRegionLoaded("OakBay") || Game->IsRegionLoaded("OakvaleMemorialGarden"))) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (Game->IsRegionLoaded("OakBay")) {
        if (IsActiveThreadTerminating()) return;
        long conversation = Game->AddNewConversation(Game->GetHero(), true, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_PRE_MOTHER_10", false, Game->GetHero(), Game->GetHero());
        CScriptThing marker = Game->GetThingWithScriptName("MotherJackChatMarker");
        while (!IsDistanceBetweenThingsUnder(marker, Game->GetHero(), 50.0f)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        if (!Game->IsConversationActive(conversation)) {
            if (!IsActiveThreadTerminating()) conversation = Game->AddNewConversation(Game->GetHero(), true, false);
            else return;
        }
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_PRE_MOTHER_20", false, Game->GetHero(), Game->GetHero());
    }
    while (!Game->IsRegionLoaded("OakvaleMemorialGarden")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (!PlayedMotherCS) { if (IsActiveThreadTerminating()) return; PlayCutscene("CS_HEROSOULS_SOUL2_MOTHER_START"); PlayedMotherCS = true; }
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS_MOTHER", Game->GetActiveQuestName(), true);
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    QuestStartScreened = true;
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_09", "OakvaleMemorialGarden", "OakBay");
    SpawnFunction("CreateScreamers");
    Game->FadeScreenIn();
    while (!PMasterData->CollectedSoulFromMother) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->Pause(1.8f);
    if (PMasterData->KilledBriar) { if (IsActiveThreadTerminating()) return; return; }
    if (IsActiveThreadTerminating()) return;
    PlayCutscene("CS_HEROSOULS_SOUL2_MOTHER_SUCCESS");
    AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_360");
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_10", "NorthernWastes3", "NorthernWastes3");
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
    Game->FadeScreenIn();
}
static void Reset(int oak, int garden, int near_, int soul, int end, bool briar)
{
    g_frames = 0; g_oakBayAt = oak; g_gardenAt = garden; g_nearAt = near_; g_soulAt = soul; g_endAt = end; g_briarDead = briar; g_convActive = false;
    g_bound = g_lines = g_convs = g_cutStart = g_cutSuccess = g_card = g_screen = g_obj9 = g_obj10 = g_spawned = g_fades = g_completed = g_log = 0;
}
int main()
{
    bool ok = true;
    CGameScriptInterface game; game.hero.id = 1;
    CMasterData data;
    CQ_HeroSoulsMotherScript s; s.Game = &game; s.PMasterData = &data;
    // Oak Bay first: Jack's two lines (conversation re-opened once it ended), then the garden and full success
    Reset(1, 6, 5, 9, 1000, false); data.KilledBriar = false; data.CollectedSoulFromMother = false; s.PlayedMotherCS = false; s.QuestStartScreened = false;
    s.Main();
    ok = ok && g_bound == 2 && g_convs == 2 && g_lines == 2 && g_cutStart == 1 && g_card == 1 && g_screen == 1 && s.QuestStartScreened
            && g_obj9 == 1 && g_spawned == 1 && g_fades == 2 && g_cutSuccess == 1 && g_log == 1 && g_obj10 == 1 && g_completed == 1;
    // straight to the garden, cutscene already played, Briar dead: no success part
    Reset(-1, 2, 0, 4, 1000, true); data.KilledBriar = true; data.CollectedSoulFromMother = false; s.PlayedMotherCS = true;
    s.Main();
    ok = ok && g_convs == 0 && g_cutStart == 0 && g_card == 1 && g_fades == 1 && g_cutSuccess == 0 && g_completed == 0;
    // the thread ends while waiting for the regions
    Reset(-1, 50, 0, 100, 3, false); data.KilledBriar = false; data.CollectedSoulFromMother = false; s.PlayedMotherCS = false;
    s.Main();
    ok = ok && g_bound == 2 && g_card == 0 && g_cutStart == 0;
    if (!ok) { std::printf("00d835d0_TEST FAIL\n"); return 1; }
    std::printf("00d835d0_TEST PASS\n");
    return 0;
}
