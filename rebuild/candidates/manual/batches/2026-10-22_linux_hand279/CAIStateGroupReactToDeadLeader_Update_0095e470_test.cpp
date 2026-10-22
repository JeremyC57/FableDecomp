// Behaviour test for CAIStateGroup_ReactToDeadLeader::Update (0x0095E470): model copy, scripted stubs.
#include <cstdio>

struct V { float X, Y, Z; };
struct CThing { unsigned char m_flags91; V pos; };
static CThing* g_enemy;
static CThing g_self;
static int g_fleeOk;
static int g_frames;
static int g_budget = 100;
static int g_moveScript[16];
static int g_moveCalls;
static int g_stopping;
static int g_terminated;
static V g_lastTarget;

static bool Alive() { return g_enemy && !(g_enemy->m_flags91 & 1); }
static bool GetNearestFleePosition(const V& centre, float radius, V* out)
{
    if (!g_fleeOk) return false;
    out->X = centre.X + radius; out->Y = centre.Y; out->Z = centre.Z;
    return true;
}
static int MoveTo(V pos) { g_lastTarget = pos; return g_moveScript[g_moveCalls++]; }

static void Update()
{
    while (Alive()) {
        V fleePos;
        if (!GetNearestFleePosition(g_enemy->pos, 20.0f, &fleePos))
            break;
        int result;
        do {
            result = MoveTo(fleePos);
            ++g_frames;
        } while (!g_stopping && Alive() && g_frames < g_budget && result == 4);
        if (result != 2)
            g_enemy = 0;
        if (!g_stopping && Alive() && g_frames < g_budget) {
            float dx = g_enemy->pos.X - g_self.pos.X, dy = g_enemy->pos.Y - g_self.pos.Y, dz = g_enemy->pos.Z - g_self.pos.Z;
            if (dx * dx + dy * dy + dz * dz > 400.0f)
                g_enemy = 0;
        }
    }
    ++g_terminated;
}

int main()
{
    int ok = 1;
    CThing enemy = { 0, { 0.0f, 0.0f, 0.0f } };
    // pending, pending, success; enemy still 25 away (> 20): forget it and stop
    g_enemy = &enemy; g_self.pos.X = 25.0f; g_fleeOk = 1;
    g_moveScript[0] = 4; g_moveScript[1] = 4; g_moveScript[2] = 2;
    Update();
    if (g_moveCalls != 3 || g_frames != 3 || g_enemy != 0 || g_terminated != 1 || g_lastTarget.X != 20.0f) ok = 0;
    // success but enemy within 20: flee again; second move fails -> forget the enemy
    g_enemy = &enemy; g_self.pos.X = 10.0f; g_moveCalls = 0; g_frames = 0;
    g_moveScript[0] = 2; g_moveScript[1] = 3;
    Update();
    if (g_moveCalls != 2 || g_enemy != 0 || g_terminated != 2) ok = 0;
    // no flee position: just terminate, enemy kept
    g_enemy = &enemy; g_fleeOk = 0; g_moveCalls = 0;
    Update();
    if (g_moveCalls != 0 || g_enemy != &enemy || g_terminated != 3) ok = 0;
    // frame budget exhausted: one move per pass with no distance check, until a move fails
    g_fleeOk = 1; g_frames = 99; g_moveScript[0] = 2; g_moveScript[1] = 3;
    Update();
    if (g_moveCalls != 2 || g_enemy != 0 || g_terminated != 4) ok = 0;
    g_enemy = &enemy;
    // dead enemy: nothing
    enemy.m_flags91 = 1; g_moveCalls = 0;
    Update();
    if (g_moveCalls != 0 || g_terminated != 5) ok = 0;
    if (!ok) { std::printf("0095e470_TEST FAIL\n"); return 1; }
    std::printf("0095e470_TEST PASS\n");
    return 0;
}
