// Behaviour test for CCreatureAction_LeapLong::Initialise (0x008B76B0): back 3 units along the facing when navigable,
// else forward 3 units (and non-solid to the target), else SPECIAL_IDLE with the flag set.
#include <cstdio>
#include <cstring>

struct Vec { float X, Y, Z; };
static int g_okMask;        // bit 0: back target navigable, bit 1: forward target navigable
static Vec g_lastTarget;
static int g_solidAdded;

static bool Navigable(const Vec& v, int bit) { g_lastTarget = v; return (g_okMask & bit) != 0; }

static const char* Initialise(const Vec& pos, const Vec& facing, bool* noRoom)
{
    Vec back = { pos.X - facing.X * 3.0f, pos.Y - facing.Y * 3.0f, pos.Z - facing.Z * 3.0f };
    if (Navigable(back, 1)) { *noRoom = false; return "MOVEMENT_LONG_BACK"; }
    Vec fwd = { facing.X * 3.0f + pos.X, facing.Y * 3.0f + pos.Y, facing.Z * 3.0f + pos.Z };
    if (Navigable(fwd, 2)) { *noRoom = false; ++g_solidAdded; return "MOVEMENT_LONG_FORWARD"; }
    *noRoom = true;
    return "SPECIAL_IDLE";
}

int main()
{
    Vec pos = { 10.0f, 20.0f, 0.0f }, facing = { 1.0f, 0.0f, 0.0f };
    bool noRoom = true;
    g_okMask = 1;
    int ok = std::strcmp(Initialise(pos, facing, &noRoom), "MOVEMENT_LONG_BACK") == 0 && !noRoom && g_lastTarget.X == 7.0f;
    g_okMask = 2;
    ok = ok && std::strcmp(Initialise(pos, facing, &noRoom), "MOVEMENT_LONG_FORWARD") == 0 && !noRoom
            && g_lastTarget.X == 13.0f && g_solidAdded == 1;
    g_okMask = 0;
    ok = ok && std::strcmp(Initialise(pos, facing, &noRoom), "SPECIAL_IDLE") == 0 && noRoom && g_solidAdded == 1;
    std::printf(ok ? "008b76b0_TEST PASS\n" : "008b76b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
