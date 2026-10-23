#include <cstdio>
#include <cstring>
struct CCharString {
    const char* p;
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString(const CCharString& o) : p(o.p) {}
    bool is(const char* t) const { return std::strcmp(p, t) == 0; }
};
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
static int g_frame = 0;
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CScriptThing {
    C3DVector pos;
    CScriptThing() : pos(5.0f, 6.0f, 7.0f) {}
    const C3DVector& GetPos() const { return pos; }
    bool MsgIsUsedBy(const CCharString& user) const { return user.is("SCRIPT_NAME_HERO") && (g_frame == 3 || g_frame == 8); }
};
static int g_active = 0, g_orbs = 0, g_deact = 0;
static float g_orbX = 0, g_orbZ = 0; static long g_orbXP = 0;
struct GameCalls {
    void NewScriptFrame() const { ++g_frame; }
    CScriptThing CreateExperienceOrb(const C3DVector& p, long xp) const { ++g_orbs; g_orbX = p.X; g_orbZ = p.Z; g_orbXP = xp; return CScriptThing(); }
    void ActivateQuest(const CCharString& q) const { if (q.is("TestTheInventory")) g_active = 1; }
    void DeactivateQuest(const CCharString& q, unsigned long) const { if (q.is("TestTheInventory")) { g_active = 0; ++g_deact; } }
};
static int g_preds = 0;
struct CIsThingAlive { CIsThingAlive(const CScriptThing&) {} };
struct CEntityScriptBase {
    GameCalls*   Game;
    CScriptThing Me;
    void SetPredicate(const CIsThingAlive&) { ++g_preds; }
    bool IsActiveThreadTerminating() { return g_frame >= 12; }
};
struct CTriggerCrate : CEntityScriptBase {
    void Main();
};

void CTriggerCrate::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!IsActiveThreadTerminating()) {
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->ActivateQuest("TestTheInventory");
        Game->CreateExperienceOrb(Me.GetPos() + C3DVector(1.0f, 1.0f, 0.0f), 10);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->DeactivateQuest("TestTheInventory", 0);
        Game->NewScriptFrame();
    }
}

int main()
{
    GameCalls g; CTriggerCrate c; c.Game = &g;
    c.Main();
    CHECK(g_preds == 1 && g_orbs == 1 && g_orbXP == 10 && g_orbX == 6.0f && g_orbZ == 7.0f);
    CHECK(g_deact == 1 && g_active == 0 && g_frame == 12);
    if (g_fail) { std::printf("00ef25e0_TEST FAIL\n"); return 1; }
    std::printf("00ef25e0_TEST PASS\n");
    return 0;
}
