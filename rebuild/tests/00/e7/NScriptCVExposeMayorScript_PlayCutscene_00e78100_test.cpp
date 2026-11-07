// Behaviour test for NScript::CV_ExposeMayorScript::PlayCutscene (0x00E78100): a copy of the logic over a mock game and a mock
// cut-scene runner; checks the "$ARG1" argument (skipped only for "NULL", case-insensitive), the five cast entries, and the
// fade / no-fade split (movie + pause only when fading, camera fixed around the macro in both).
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <vector>

static bool EqualsCaseInsensitive(const std::string& a, const char* b)
{
    if (a.size() != std::strlen(b)) return false;
    for (size_t i = 0; i < a.size(); ++i) { char x = a[i], y = b[i]; if (x >= 'a' && x <= 'z') x -= 32; if (y >= 'a' && y <= 'z') y -= 32; if (x != y) return false; }
    return true;
}
struct MockGame {
    std::vector<std::string> log;
    long GetHero() { return 7; }
    void StartScriptingEntity(long, std::string& out, int priority) { out = priority == 4 ? "hero_entity" : "wrong_priority"; }
    void StartMovieSequence(const char*, int) { log.push_back("movie"); }
    void PauseAllNonScriptedEntities(bool on) { log.push_back(on ? "pause" : "unpause"); }
    void FixMovieSequenceCamera(bool on) { log.push_back(on ? "fix" : "unfix"); }
};
struct Pause { MockGame* g; Pause(MockGame& game) : g(&game) { g->PauseAllNonScriptedEntities(true); } ~Pause() { g->PauseAllNonScriptedEntities(false); } };
static std::string lastMacro, lastCast, lastArgs;
static void RunCutsceneMacro(const std::string& name, std::map<std::string, std::string>& cast, int, std::map<std::string, std::string>* csargs, bool, bool, MockGame& g)
{
    lastMacro = name; lastCast.erase(); lastArgs.erase();
    for (std::map<std::string, std::string>::iterator i = cast.begin(); i != cast.end(); ++i) lastCast += i->first + "=" + i->second + ";";
    for (std::map<std::string, std::string>::iterator i = csargs->begin(); i != csargs->end(); ++i) lastArgs += i->first + "=" + i->second + ";";
    g.log.push_back("macro");
}
struct Script {
    MockGame* Game; std::string seh_Suspicious, seh_Sheriff, seh_LadyGrey, seh_Rhodri;
    void PlayCutscene(std::string name, bool fade, std::string arg)
    {
        std::map<std::string, std::string> csargs;
        std::string hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, 4);
        if (!EqualsCaseInsensitive(arg, "NULL"))
            csargs["$ARG1"] = arg;
        std::map<std::string, std::string> cast;
        cast["HERO"] = hero;
        cast["PRIS"] = seh_Suspicious;
        cast["SHER"] = seh_Sheriff;
        cast["LADY"] = seh_LadyGrey;
        cast["RHODRI"] = seh_Rhodri;
        if (fade) {
            int movie = 0; Game->StartMovieSequence("", movie);
            Pause pause(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro(name, cast, 0, &csargs, false, true, *Game);
            Game->FixMovieSequenceCamera(false);
        } else {
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro(name, cast, 0, &csargs, false, true, *Game);
            Game->FixMovieSequenceCamera(false);
        }
    }
};
static std::string Join(const std::vector<std::string>& v) { std::string s; for (size_t i = 0; i < v.size(); ++i) { if (i) s += " "; s += v[i]; } return s; }
int main()
{
    MockGame g; Script s; s.Game = &g; s.seh_Suspicious = "pris"; s.seh_Sheriff = "sher"; s.seh_LadyGrey = "lady"; s.seh_Rhodri = "rhodri";
    s.PlayCutscene("CS_MAYOREXPOSE_GHOST", true, "NULL");
    const char* cast = "HERO=hero_entity;LADY=lady;PRIS=pris;RHODRI=rhodri;SHER=sher;";
    if (Join(g.log) != "movie pause fix macro unfix unpause" || lastMacro != "CS_MAYOREXPOSE_GHOST" || lastCast != cast || !lastArgs.empty()) { std::printf("00e78100_TEST FAIL\n"); return 1; }
    g.log.clear();
    s.PlayCutscene("CS_MAYOREXPOSE_GREYREFUSE", false, "null");
    if (Join(g.log) != "fix macro unfix" || lastMacro != "CS_MAYOREXPOSE_GREYREFUSE" || lastCast != cast || !lastArgs.empty()) { std::printf("00e78100_TEST FAIL\n"); return 1; }
    g.log.clear();
    s.PlayCutscene("CS_X", false, "Evidence");
    if (Join(g.log) != "fix macro unfix" || lastArgs != "$ARG1=Evidence;") { std::printf("00e78100_TEST FAIL\n"); return 1; }
    std::printf("00e78100_TEST PASS\n");
    return 0;
}