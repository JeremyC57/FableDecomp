#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>
// behaviour test: OakValeFire waits for the "fire" flag, creates a burning-patch effect at every fire point, then
// waits for the flag to drop and removes every effect
static int Fail(const char* what, const char* log) { std::printf("00ee8870_TEST FAIL %s [%s]\n", what, log); return 1; }
#define CHECK(cond) do { if (!(cond)) return Fail(#cond, g_log); } while (0)
static char g_log[4096];
static int g_frame = 0, g_termAt = 1000, g_fireOn = 3, g_fireOff = 7;
static void Log(const char* s) { char b[16]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
struct C3DVector { float X, Y, Z; };
struct CScriptThing {
    std::string Name; C3DVector Pos;
    CScriptThing() : Name("") { Pos.X = Pos.Y = Pos.Z = 0.0f; }
    const C3DVector& GetPos() const { return Pos; }
};
struct CGame {
    std::vector<CScriptThing> Points;
    void NewScriptFrame() const { ++g_frame; }
    long GetAllThingsWithScriptName(const char* n, std::vector<CScriptThing>& out) const { if (std::strcmp(n, "Q_REVISITED_FIREPOINT") == 0) out = Points; return (long)out.size(); }
    CScriptThing CreateEffect(const char* name, const C3DVector& pos, const char* group, float, bool, bool) const
    { CScriptThing t; char x[96]; std::sprintf(x, "%s/%s/%d", name, group, (int)pos.X); t.Name = x; Log(("create " + t.Name).c_str()); return t; }
    void RemoveThing(const CScriptThing& t, bool, bool b) const { char x[128]; std::sprintf(x, "remove %s %d", t.Name.c_str(), (int)b); Log(x); }
};
struct CCS_OakValeRevisitedScript {
    CGame* GSI; std::vector<CScriptThing> FirePoint, Fires; std::map<std::string, bool> OakValeFlag;
    bool IsActiveThreadTerminating() { OakValeFlag["fire"] = g_frame >= g_fireOn && g_frame < g_fireOff; return g_frame >= g_termAt; }
    void OakValeFire();
};
void CCS_OakValeRevisitedScript::OakValeFire()
{
    while (OakValeFlag["fire"] != true) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->GetAllThingsWithScriptName("Q_REVISITED_FIREPOINT", FirePoint);
    Fires.resize(FirePoint.size());
    for (unsigned int i = 0; i < FirePoint.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Fires[i] = GSI->CreateEffect("OAKVALE_BURNING_PATCH", FirePoint[i].GetPos(), "FirePoints", 0.0f, false, false);
    }
    if (IsActiveThreadTerminating())
        return;
    while (OakValeFlag["fire"] != false) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Fires.resize(FirePoint.size());
    for (unsigned int j = 0; j < Fires.size(); ++j) {
        if (IsActiveThreadTerminating())
            return;
        GSI->RemoveThing(Fires[j], false, true);
    }
}
int main()
{
    CGame game; CScriptThing p; p.Pos.X = 4.0f; game.Points.push_back(p); p.Pos.X = 9.0f; game.Points.push_back(p);
    CCS_OakValeRevisitedScript s; s.GSI = &game;
    s.OakValeFire();
    CHECK(std::strcmp(g_log, "create OAKVALE_BURNING_PATCH/FirePoints/4@3;create OAKVALE_BURNING_PATCH/FirePoints/9@3;"
        "remove OAKVALE_BURNING_PATCH/FirePoints/4 1@7;remove OAKVALE_BURNING_PATCH/FirePoints/9 1@7;") == 0);
    g_log[0] = 0; g_frame = 0; g_termAt = 5; s.Fires.clear(); s.OakValeFlag.clear();
    s.OakValeFire();
    CHECK(std::strcmp(g_log, "create OAKVALE_BURNING_PATCH/FirePoints/4@3;create OAKVALE_BURNING_PATCH/FirePoints/9@3;") == 0 && g_frame == 5);
    std::printf("00ee8870_TEST PASS\n");
    return 0;
}
