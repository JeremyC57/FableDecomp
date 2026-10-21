// Behaviour test for CTestPos::Main (0x00EF3840): the filter is registered and destroyed first; then the script waits
// until the hero is within 1.4 of Me, fires slot 508 on the hero with false, waits until the hero is beyond 3.0,
// fires slot 509, yields, and repeats until IsDone; every wait yields one frame per failed test and stops as soon as
// the script is done.
#include <cstdio>

struct CScriptThing { int id; };
static int g_log[256];
static int g_n = 0;
static void Log(int v) { if (g_n < 256) g_log[g_n++] = v; }

static int g_frame = 0, g_doneAt = 0;
static float g_distance[64];
static CScriptThing g_hero = { 7 };

struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; Log(1); }
    CScriptThing& GetHero() { return g_hero; }
    void EntityUnsheatheMeleeWeapon(const CScriptThing& thing, bool flag) { Log(flag ? 99 : 508 + thing.id * 0); }
    void EntityUnsheatheRangedWeapon(const CScriptThing& thing, bool flag) { Log(flag ? 99 : 509 + thing.id * 0); }
};
bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d)
{ return a.id == 7 && b.id == 3 && g_distance[g_frame] < d; }
bool IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float d)
{ return a.id == 7 && b.id == 3 && g_distance[g_frame] > d; }

struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { Log(2); }
};

class CTestPos {
public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    void RegisterFilter(CIsThingAlive& filter) { Log(filter.m_thing.id == 3 ? 3 : 98); }
    bool IsDone() { return g_frame >= g_doneAt; }
    void Main();
};

void CTestPos::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!IsDone()) {
        while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 1.4f)) {
            Game->NewScriptFrame();
            if (IsDone())
                return;
        }
        if (IsDone())
            return;
        Game->EntityUnsheatheMeleeWeapon(Game->GetHero(), false);
        while (!IsDistanceBetweenThingsOver(Game->GetHero(), Me, 3.0f)) {
            Game->NewScriptFrame();
            if (IsDone())
                return;
        }
        if (IsDone())
            return;
        Game->EntityUnsheatheRangedWeapon(Game->GetHero(), false);
        Game->NewScriptFrame();
    }
}

static bool Expect(const int* e, int count)
{
    if (g_n != count) return false;
    for (int i = 0; i < count; ++i) if (g_log[i] != e[i]) return false;
    return true;
}

int main()
{
    CGameScriptInterface game;
    CTestPos s; s.Game = &game; s.Me.id = 3;
    // frame:   0    1    2    3    4    5    6    7
    float d[] = { 9, 5.0f, 2.0f, 1.0f, 2.0f, 4.0f, 1.0f, 9.0f };
    for (int i = 0; i < 64; ++i) g_distance[i] = i < 8 ? d[i] : 0.5f;
    g_doneAt = 7;
    s.Main();
    // filter(3,2), frame->1, wait under: f1 5.0 no -> frame 2 (2.0 no) -> frame 3 (1.0 yes) -> 508,
    // wait over: f3 1.0 no -> frame 4 (2.0) no -> frame 5 (4.0 yes) -> 509, frame -> 6, loop: under 1.0 yes -> 508,
    // wait over: 1.0 no -> frame 7 -> done -> return
    static const int e1[] = { 3, 2, 1, 1, 1, 508, 1, 1, 509, 1, 508, 1 };
    int ok = Expect(e1, 12);
    g_n = 0; g_frame = 0; g_doneAt = 1;                      // done right after the first frame
    s.Main();
    static const int e2[] = { 3, 2, 1 };
    ok = ok && Expect(e2, 3);
    if (!ok) { std::printf("00ef3840_TEST FAIL\n"); return 1; }
    std::printf("00ef3840_TEST PASS\n");
    return 0;
}
