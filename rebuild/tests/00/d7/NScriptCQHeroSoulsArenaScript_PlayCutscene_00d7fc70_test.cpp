// behaviour test for 0x00D7FC70 NScript::CQ_HeroSoulsArenaScript::PlayCutscene(CCharString): the hero is taken under scripted control (priority 4) into a fresh handle, cast as HERO, and the named cut-scene runs (no flags, no args, a5 false, a6 true) between pause(true) / fix-camera(true) and fix-camera(false) / pause(false), inside a movie sequence started with an empty name
#include <cstdio>
#include <map>
#include <string>
struct Handle { int Id; };
struct Game {
    std::string Log; int HeroCalls; int Priority; Handle* Scripted;
    Game() : HeroCalls(0), Priority(-1), Scripted(0) { }
    int GetHero() { ++HeroCalls; Log += "H"; return 7; }
    bool StartScriptingEntity(int thing, Handle& h, int priority) { Log += "S"; h.Id = thing; Priority = priority; Scripted = &h; return true; }
    void StartMovieSequence(const std::string& name) { Log += name.empty() ? "M" : "m"; }
    void Pause(bool b) { Log += b ? "P" : "p"; }
    void Fix(bool b) { Log += b ? "F" : "f"; }
};
static std::string gRunName; static std::map<std::string, Handle> gRunCast; static int gRunArgs = -1;
static void RunCutsceneMacro(const std::string& name, std::map<std::string, Handle>& cast, void* flags, void* args, bool a5, bool a6, Game& g)
{
    g.Log += "R"; gRunName = name; gRunCast = cast; gRunArgs = (flags == 0) + 2 * (args == 0) + 4 * (a5 ? 1 : 0) + 8 * (a6 ? 1 : 0);
}
static void PlayCutscene(Game* game_, std::string name)      // the function under test
{
    Handle hero; hero.Id = 0;
    game_->StartScriptingEntity(game_->GetHero(), hero, 4);
    std::map<std::string, Handle> cast;
    cast["HERO"] = hero;
    game_->StartMovieSequence("");
    Game* game = game_;
    game->Pause(true);
    game_->Fix(true);
    RunCutsceneMacro(name, cast, 0, 0, false, true, *game_);
    game_->Fix(false);
    game->Pause(false);
}
int main()
{
    int fail = 0;
    Game g;
    PlayCutscene(&g, "CS_TEST");
    if (g.Log != "HSMPFRfp") fail |= 1;
    if (g.HeroCalls != 1 || g.Priority != 4) fail |= 2;
    if (gRunName != "CS_TEST" || gRunCast.size() != 1 || gRunCast.count("HERO") != 1 || gRunCast["HERO"].Id != 7) fail |= 4;
    if (gRunArgs != 1 + 2 + 8) fail |= 8;
    if (fail) { std::printf("00d7fc70_TEST FAIL %d\n", fail); return 1; }
    std::printf("00d7fc70_TEST PASS\n");
    return 0;
}