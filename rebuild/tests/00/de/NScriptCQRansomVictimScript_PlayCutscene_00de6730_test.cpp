// Behaviour test for NScript::CQ_RansomVictimScript::PlayCutscene (0x00DE6730): a copy of the logic over a mock game and a mock
// cut-scene runner; checks the "$ARG1" argument (skipped only for "NULL", case-insensitive), the six cast entries and the
// camera fixed around the macro (no movie / pause in this script's variant).
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
    void FixMovieSequenceCamera(bool on) { log.push_back(on ? "fix" : "unfix"); }
};
static std::string lastMacro, lastCast, lastArgs;
static void RunCutsceneMacro(const std::string& name, std::map<std::string, std::string>& cast, int, std::map<std::string, std::string>* csargs, bool, bool, MockGame& g)
{
    lastMacro = name; lastCast.erase(); lastArgs.erase();
    for (std::map<std::string, std::string>::iterator i = cast.begin(); i != cast.end(); ++i) lastCast += i->first + "=" + i->second + ";";
    for (std::map<std::string, std::string>::iterator i = csargs->begin(); i != csargs->end(); ++i) lastArgs += i->first + "=" + i->second + ";";
    g.log.push_back("macro");
}
struct Script {
    MockGame* Game; std::string seh_Guard, seh_Father, seh_Victim, seh_Brother, seh_Ransommer;
    void PlayCutscene(std::string name, std::string arg)
    {
        std::map<std::string, std::string> csargs;
        std::string hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, 4);
        if (!EqualsCaseInsensitive(arg, "NULL"))
            csargs["$ARG1"] = arg;
        std::map<std::string, std::string> cast;
        cast["HERO"] = hero;
        cast["GUARD"] = seh_Guard;
        cast["CHIEF"] = seh_Father;
        cast["VICTIM"] = seh_Victim;
        cast["BROTHER"] = seh_Brother;
        cast["BANDIT"] = seh_Ransommer;
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro(name, cast, 0, &csargs, false, true, *Game);
        Game->FixMovieSequenceCamera(false);
    }
};
static std::string Join(const std::vector<std::string>& v) { std::string s; for (size_t i = 0; i < v.size(); ++i) { if (i) s += " "; s += v[i]; } return s; }
int main()
{
    MockGame g; Script s; s.Game = &g; s.seh_Guard = "guard"; s.seh_Father = "father"; s.seh_Victim = "victim"; s.seh_Brother = "brother"; s.seh_Ransommer = "ransommer";
    s.PlayCutscene("CS_RANSOM_INTRO", "NULL");
    const char* cast = "BANDIT=ransommer;BROTHER=brother;CHIEF=father;GUARD=guard;HERO=hero_entity;VICTIM=victim;";
    if (Join(g.log) != "fix macro unfix" || lastMacro != "CS_RANSOM_INTRO" || lastCast != cast || !lastArgs.empty()) { std::printf("00de6730_TEST FAIL\n"); return 1; }
    g.log.clear();
    s.PlayCutscene("CS_RANSOM_PAY", "Null");
    if (Join(g.log) != "fix macro unfix" || !lastArgs.empty()) { std::printf("00de6730_TEST FAIL\n"); return 1; }
    g.log.clear();
    s.PlayCutscene("CS_RANSOM_PAY", "Gold");
    if (Join(g.log) != "fix macro unfix" || lastArgs != "$ARG1=Gold;") { std::printf("00de6730_TEST FAIL\n"); return 1; }
    std::printf("00de6730_TEST PASS\n");
    return 0;
}