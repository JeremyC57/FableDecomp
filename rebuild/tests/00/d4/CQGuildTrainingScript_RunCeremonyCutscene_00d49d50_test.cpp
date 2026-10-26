#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingScript::RunCeremonyCutscene -- wait for FrescoDome, create Whisper and the GM, script
// them and the hero, cast WHIS/GM/HERO, play the ceremony cut-scene, then remove both actors
static int g_frames = 0, g_endAt = 1000, g_created = 0, g_scripted = 0, g_faction = 0, g_removed = 0, g_played = 0, g_paused = 0, g_cast = 0;
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public: const char* name; C3DVector pos; const C3DVector& GetPos() const { return pos; } };
struct CHandle { const CScriptThing* thing; CHandle() : thing(0) {} bool IsScripting() { return thing != 0; } void Release() { thing = 0; } };
struct CCastMap { const char* keys[3]; const CScriptThing* things[3]; int n; CCastMap() : n(0) {}
    void Set(const char* k, const CHandle& h) { keys[n] = k; things[n] = h.thing; ++n; } };
class CGameScriptInterface { public:
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frames; }
    bool StartScriptingEntity(const CScriptThing& t, CHandle& h, int priority) { if (g_frames < 2 || priority != 4) return false; h.thing = &t; ++g_scripted; return true; }
    bool IsLevelLoaded(const char* l) const { return !std::strcmp(l, "FrescoDome") && g_frames >= 1; }
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.name = n; t.pos.X = (float)std::strlen(n); t.pos.Y = t.pos.Z = 0; return t; }
    CScriptThing CreateCreature(const char* def, const C3DVector& pos, const char* name, bool) const { CScriptThing t; t.name = name; t.pos = pos; if (def && pos.X > 0) ++g_created; return t; }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if ((!std::strcmp(t.name, "Whisper") || !std::strcmp(t.name, "GM")) && !a && b) ++g_removed; }
    void StartMovieSequence(const char*, int&) {}
    void FixMovieSequenceCamera(bool) const {}
    void PauseAllNonScriptedEntities(bool p) { g_paused += p ? 1 : -1; }
    void EntitySetInFaction(const CScriptThing& t, const char* f) const { if (!std::strcmp(t.name, "Whisper") && !std::strcmp(f, "FACTION_HERO")) ++g_faction; }
};
static void RunCutsceneMacro(const char* name, CCastMap& cast) { if (!std::strcmp(name, "CS_GUILD_CEREMONY") && cast.n == 3 && cast.things[1] && !std::strcmp(cast.things[1]->name, "Whisper")) ++g_played; g_cast = cast.n; }
struct CQ_GuildTrainingScript {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void RunCeremonyCutscene();
};
void CQ_GuildTrainingScript::RunCeremonyCutscene()
{
    while (!Game->IsLevelLoaded("FrescoDome")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    CScriptThing whisper = Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", Game->GetThingWithScriptName("MK_GTC_WHISSTART").GetPos(), "Whisper", false);
    CScriptThing guildmaster = Game->CreateCreature("CREATURE_GUILDKEEPER", Game->GetThingWithScriptName("MK_GTC_GMSTART").GetPos(), "GM", false);
    CHandle whisperHandle; if (whisperHandle.IsScripting()) whisperHandle.Release();
    while (!Game->StartScriptingEntity(whisper, whisperHandle, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetInFaction(whisper, "FACTION_HERO");
    CHandle guildmasterHandle; if (guildmasterHandle.IsScripting()) guildmasterHandle.Release();
    while (!Game->StartScriptingEntity(guildmaster, guildmasterHandle, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    CHandle heroHandle; if (heroHandle.IsScripting()) heroHandle.Release();
    while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    CCastMap cast; cast.Set("HERO", heroHandle); cast.Set("WHIS", whisperHandle); cast.Set("GM", guildmasterHandle);
    int movie = 0; Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro("CS_GUILD_CEREMONY", cast);
    Game->FixMovieSequenceCamera(false);
    Game->RemoveThing(guildmaster, false, true);
    Game->RemoveThing(whisper, false, true);
    game->PauseAllNonScriptedEntities(false);
}
int main()
{
    CGameScriptInterface game; game.hero.name = "Hero"; CQ_GuildTrainingScript s; s.Game = &game;
    s.RunCeremonyCutscene();
    bool ok = g_created == 2 && g_scripted == 3 && g_faction == 1 && g_played == 1 && g_cast == 3 && g_removed == 2 && g_paused == 0 && g_frames == 2;
    g_frames = 0; g_endAt = 1; g_created = 0;
    s.RunCeremonyCutscene();                            // terminated while waiting for the dome
    ok = ok && g_created == 0 && g_removed == 2;
    if (!ok) { std::printf("00d49d50_TEST FAIL\n"); return 1; }
    std::printf("00d49d50_TEST PASS\n");
    return 0;
}