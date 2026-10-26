#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingMeleeScript::Main -- bind three entity scripts, teleport the guildmaster, create Whisper
// (hero faction, friends with everything), script her and the hero, play CS_GUILD_MELEE_INTRO, set the objective, log
// story entry 40 and wait for the melee tutorial to be clicked past
static int g_frames = 0, g_endAt = 1000, g_bindings = 0, g_post = 0, g_teleport = 0, g_created = 0, g_scripted = 0, g_faction = 0;
static int g_friends = 0, g_boss = 0, g_played = 0, g_paused = 0, g_objective = 0, g_story = 0, g_tutorialShown = 0, g_clicked = 0;
static bool g_tutorial = true;
struct C3DVector { float X, Y, Z; };
struct CThingImpl { bool friends; CThingImpl() : friends(false) {} void SetFriendsWithEverythingFlag(bool on) { friends = on; ++g_friends; } };
struct CScriptThing {
    const char* name; C3DVector pos; CThingImpl* data;
    CScriptThing() : name(""), data(0) { pos.X = pos.Y = pos.Z = 0; }
    const C3DVector& GetPos() const { return pos; }
    void SetFriendsWithEverythingFlag(bool on) { if (data) data->SetFriendsWithEverythingFlag(on); }
};
static CThingImpl g_whisperImpl;
struct CHandle { const CScriptThing* thing; CHandle() : thing(0) {} bool IsScripting() { return thing != 0; } void Release() { thing = 0; } };
struct CCastMap { const char* keys[2]; const CScriptThing* things[2]; int n; CCastMap() : n(0) {}
    void Set(const char* k, const CHandle& h) { keys[n] = k; things[n] = h.thing; ++n; } };
struct CGameScriptInterface {
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frames; }
    bool StartScriptingEntity(const CScriptThing& t, CHandle& h, int priority) { if (g_frames < 2 || priority != 4) return false; h.thing = &t; ++g_scripted; return true; }
    bool MsgIsTutorialClickedPast() const { if (g_frames >= 4) { ++g_clicked; return true; } return false; }
    void PostAddScriptedEntities() const { ++g_post; }
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.name = n; t.pos.X = (float)std::strlen(n); return t; }
    CScriptThing CreateCreature(const char* def, const C3DVector& pos, const char* name, bool) const
    { CScriptThing t; t.name = name; t.pos = pos; if (!std::strcmp(def, "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE") && pos.X == 15) { ++g_created; t.data = &g_whisperImpl; } return t; }
    bool DisplayTutorial(int category) const { if (category == 26 && g_tutorial) { ++g_tutorialShown; return true; } return false; }
    void SetQuestCardObjective(const char* q, const char* t, const char*, const char*) const { if (!std::strcmp(q, "Q_GuildTraining") && !std::strcmp(t, "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_03")) ++g_objective; }
    void StartMovieSequence(const char*, int&) {}
    void FixMovieSequenceCamera(bool) const {}
    void PauseAllNonScriptedEntities(bool p) { g_paused += p ? 1 : -1; }
    void EntityTeleportToThing(const CScriptThing& t, const CScriptThing& to, bool) const { if (!std::strcmp(t.name, "TheRealGuildmaster") && !std::strcmp(to.name, "M_MeleeTeacherStand")) ++g_teleport; }
    void EntitySetAllowBossPhaseChanges(const CScriptThing& t, bool allow) const { if (!std::strcmp(t.name, "MeleeOpponent") && !allow) ++g_boss; }
    void EntitySetInFaction(const CScriptThing& t, const char* f) const { if (!std::strcmp(t.name, "MeleeOpponent") && !std::strcmp(f, "FACTION_HERO")) ++g_faction; }
};
static void RunCutsceneMacro(const char* name, CCastMap& cast) { if (!std::strcmp(name, "CS_GUILD_MELEE_INTRO") && cast.n == 2 && !std::strcmp(cast.keys[1], "WHISPER") && !std::strcmp(cast.things[1]->name, "MeleeOpponent")) ++g_played; }
static void AddLogbookStoryEntry(long index) { if (index == 40) ++g_story; }
struct CQ_GuildTrainingMeleeScript {
    CGameScriptInterface* Game;
    bool TalkedToWhisper;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void AddEntityScriptBinding(const char*) { ++g_bindings; }
    void PostAddScriptedEntities() { ++g_post; }
    void Main();
};
void CQ_GuildTrainingMeleeScript::Main()
{
    AddEntityScriptBinding("TheRealGuildmaster");
    AddEntityScriptBinding("MeleeOpponent");
    AddEntityScriptBinding("MeleeThunder");
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->EntityTeleportToThing(Game->GetThingWithScriptName("TheRealGuildmaster"), Game->GetThingWithScriptName("M_MeleeTeacherStand"), false);
    CScriptThing whisper = Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", Game->GetThingWithScriptName("MK_GTWU_WHISPER").GetPos(), "MeleeOpponent", false);
    Game->EntitySetInFaction(whisper, "FACTION_HERO");
    whisper.SetFriendsWithEverythingFlag(true);
    CHandle whisperHandle; if (whisperHandle.IsScripting()) whisperHandle.Release();
    while (!Game->StartScriptingEntity(whisper, whisperHandle, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAllowBossPhaseChanges(whisper, false);
    CHandle heroHandle; if (heroHandle.IsScripting()) heroHandle.Release();
    while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    {
        CCastMap cast; cast.Set("HERO", heroHandle); cast.Set("WHISPER", whisperHandle);
        int movie = 0; Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_MELEE_INTRO", cast);
        Game->FixMovieSequenceCamera(false);
        TalkedToWhisper = true;
        game->PauseAllNonScriptedEntities(false);
    }
    Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_03", "", "");
    AddLogbookStoryEntry(40);
    if (Game->DisplayTutorial(26)) {
        if (IsActiveThreadTerminating()) return;
        while (!Game->MsgIsTutorialClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
    }
}
int main()
{
    CGameScriptInterface game; game.hero.name = "Hero"; CQ_GuildTrainingMeleeScript s; s.Game = &game; s.TalkedToWhisper = false;
    s.Main();
    bool ok = g_bindings == 3 && g_post == 2 && g_teleport == 1 && g_created == 1 && g_faction == 1 && g_whisperImpl.friends && g_friends == 1
        && g_scripted == 2 && g_boss == 1 && g_played == 1 && g_paused == 0 && s.TalkedToWhisper && g_objective == 1 && g_story == 1
        && g_tutorialShown == 1 && g_clicked == 1 && g_frames == 4;
    g_frames = 0; g_endAt = 1; g_played = 0; s.TalkedToWhisper = false;
    s.Main();                                           // terminated while waiting to script Whisper
    ok = ok && g_played == 0 && !s.TalkedToWhisper && g_boss == 1;
    g_frames = 0; g_endAt = 1000; g_tutorial = false; g_clicked = 0;
    s.Main();                                           // tutorial not shown: no wait
    ok = ok && g_played == 1 && g_clicked == 0 && g_frames == 2;
    if (!ok) { std::printf("00d55e90_TEST FAIL\n"); return 1; }
    std::printf("00d55e90_TEST PASS\n");
    return 0;
}