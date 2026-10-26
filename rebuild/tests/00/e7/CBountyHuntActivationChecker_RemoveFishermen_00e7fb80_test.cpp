#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
// Behaviour model of CBountyHuntActivationChecker::RemoveFishermen: the organiser is removed only while alive, then
// each living competitor is removed (dead ones are skipped); RemoveThing always gets (false, true).
static std::string g_log;
class CScriptThing { public: std::string name; bool alive; bool IsAlive() const { return alive; } };
class CGame { public:
    bool organiserAlive; std::vector<CScriptThing> competitors;
    CScriptThing GetThingWithScriptName(const char* name) const { CScriptThing t; t.name = name; t.alive = organiserAlive; return t; }
    long GetAllThingsWithScriptName(const char* name, std::vector<CScriptThing>& out) const
    { if (std::strcmp(name, "FishCompCompetitor") == 0) out = competitors; return (long)out.size(); }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { char x[8]; std::sprintf(x, "%d%d;", a, b); g_log += t.name + x; }
};
struct CChecker {
    CGame* Game;
    void RemoveFishermen()
    {
        if (Game->GetThingWithScriptName("FishCompOrganiser").IsAlive())
            Game->RemoveThing(Game->GetThingWithScriptName("FishCompOrganiser"), false, true);
        std::vector<CScriptThing> competitors;
        Game->GetAllThingsWithScriptName("FishCompCompetitor", competitors);
        long count = (long)competitors.size();
        if (count != 0) {
            for (long i = 0; i < count; ++i)
                if (competitors[i].IsAlive())
                    Game->RemoveThing(competitors[i], false, true);
        }
    }
};
int main()
{
    CGame g; CChecker c; c.Game = &g;
    g.organiserAlive = true;
    CScriptThing a; a.name = "A"; a.alive = true; CScriptThing b; b.name = "B"; b.alive = false; CScriptThing d; d.name = "D"; d.alive = true;
    g.competitors.push_back(a); g.competitors.push_back(b); g.competitors.push_back(d);
    c.RemoveFishermen();
    if (g_log != "FishCompOrganiser01;A01;D01;") { std::printf("00e7fb80_TEST FAIL\n"); return 1; }
    g_log = ""; g.organiserAlive = false; g.competitors.clear();
    c.RemoveFishermen();
    if (g_log != "") { std::printf("00e7fb80_TEST FAIL\n"); return 1; }
    std::printf("00e7fb80_TEST PASS\n");
    return 0;
}