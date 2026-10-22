#include <cstdio>
#include <cstring>
// behaviour test: CQ_HangingTreeGoodScript::WatchForHeroNearExit -- gather the REGION_EXIT_POINT things, then each
// frame check whether the hero is within 8 of any of them while more than 3 from the prisoner; the first time, the
// guard leader tells him not to leave and the watch ends
static int g_frames = 0, g_endAt = 1000, g_lines = 0, g_heroAt = 4;   // frame at which the hero reaches exit #1
struct CScriptThing { int id; };
struct CThings {
    CScriptThing Items[4]; unsigned int Count;
    CThings() : Count(0) { }
    void push_back(int id) { CScriptThing t; t.id = id; Items[Count++] = t; }
    unsigned int size() const { return Count; }
    const CScriptThing& operator[](unsigned int i) const { return Items[i]; }
};
static bool g_farFromPrisoner = true;
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { return a.id == 0 && b.id == 21 && d == 8.0f && g_frames >= g_heroAt; }
static bool IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float d) { return a.id == 0 && b.id == 5 && d == 3.0f && g_farFromPrisoner; }
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing GetHero() const { CScriptThing t; t.id = 0; return t; }
    void GetAllThingsWithDefName(const char* n, CThings& v) const { if (!std::strcmp(n, "REGION_EXIT_POINT")) { v.push_back(20); v.push_back(21); } }
    void AddLineToConversation(const CScriptThing& who, const char* t) const { if (who.id == 7 && !std::strcmp(t, "TEXT_QST_030_GUARD_DONT_EXIT")) ++g_lines; }
};
struct CQ_HangingTreeGoodScript {
    CGameScriptInterface* Game; CScriptThing Leader, Prisoner;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void WatchForHeroNearExit();
};
void CQ_HangingTreeGoodScript::WatchForHeroNearExit()
{
    bool warned = false;
    CThings exits;
    Game->GetAllThingsWithDefName("REGION_EXIT_POINT", exits);
    while (!warned) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        for (unsigned int i = 0; i < exits.size(); ++i) {
            if (IsActiveThreadTerminating()) return;
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), exits[i], 8.0f) && IsDistanceBetweenThingsOver(Game->GetHero(), Prisoner, 3.0f)) {
                Game->AddLineToConversation(Leader, "TEXT_QST_030_GUARD_DONT_EXIT");
                warned = true;
            }
        }
    }
}
int main()
{
    CGameScriptInterface game; CQ_HangingTreeGoodScript s; s.Game = &game; s.Leader.id = 7; s.Prisoner.id = 5;
    s.WatchForHeroNearExit();
    bool ok = g_lines == 1 && g_frames == 4;
    g_frames = 0; g_lines = 0; g_farFromPrisoner = false; g_endAt = 9;
    s.WatchForHeroNearExit();                           // escorting the prisoner: never warned, ends with the thread
    ok = ok && g_lines == 0 && g_frames == 9;
    if (!ok) { std::printf("00d76490_TEST FAIL\n"); return 1; }
    std::printf("00d76490_TEST PASS\n");
    return 0;
}