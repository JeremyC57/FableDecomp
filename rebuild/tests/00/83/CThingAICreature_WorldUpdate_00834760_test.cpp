// Behaviour test for CThingAICreature::WorldUpdate (0x00834760): the stress test runs only when switched on; the
// frame-updating flag is set during the update and cleared after; the movement vector is position - last position
// only when a last position exists, and the flag is then set; alive creatures get movement and animation with the
// camera distance and their weapon trails updated when present; always false.
#include <cstdio>
#include <cmath>
#include <string>

static std::string g_log;
struct V { float x, y, z; };
struct Creature {
    bool stress; bool frameUpdating; unsigned char flags; bool dead; bool hasTrail;
    V pos, last, move, cam; float seenDist;
    bool WorldUpdate()
    {
        if (stress) g_log += "S";
        frameUpdating = true;
        g_log += "A"; g_log += "B";
        if (flags & 2) { move.x = pos.x - last.x; move.y = pos.y - last.y; move.z = pos.z - last.z; }
        flags |= 2;
        float dx = cam.x - pos.x, dy = cam.y - pos.y, dz = cam.z - pos.z;
        float s = dz * dz; s += dy * dy; s += dx * dx;
        float d = std::sqrt(s);
        if (!dead) { g_log += "M"; seenDist = d; g_log += "N"; if (hasTrail) g_log += "W"; }
        frameUpdating = false;
        return false;
    }
};

int main()
{
    int ok = 1;
    Creature c = { true, false, 0, false, true, { 1, 2, 3 }, { 0, 0, 0 }, { 9, 9, 9 }, { 4, 6, 3 }, 0 };
    ok &= !c.WorldUpdate();
    ok &= g_log == "SABMNW" && c.move.x == 9 && (c.flags & 2) && c.seenDist == 5.0f && !c.frameUpdating;
    g_log = ""; c.stress = false; c.last.x = 1; c.WorldUpdate();
    ok &= g_log == "ABMNW" && c.move.x == 0 && c.move.y == 2 && c.move.z == 3;
    g_log = ""; c.dead = true; c.WorldUpdate();
    ok &= g_log == "AB";
    std::printf(ok ? "00834760_TEST PASS\n" : "00834760_TEST FAIL\n");
    return ok ? 0 : 1;
}