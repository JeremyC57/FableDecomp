// Behaviour test for 0x0092BF50 CAIStateGroup_MinionWander::FaceAwayFromBuildings: a portable model of the retail logic (nearest
// building within 10 units in 2D, alive and in view -> look at creature + (creature - building) with 0x11 frames, else 2).
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
struct Building { V Pos; bool Dead; bool InView; };
static long g_lookFrames = 0; static V g_lookAt = { 0, 0, 0 };
static long FaceAway(const V& creature, Building* buildings, int count) {
    Building* best = 0; float bestD = 0;
    for (int i = 0; i < count; ++i) {
        float dx = buildings[i].Pos.X - creature.X, dy = buildings[i].Pos.Y - creature.Y;
        if (dx * dx + dy * dy > 100.0f) continue;                     // In2DRange(10)
        float dz = buildings[i].Pos.Z - creature.Z, d = dx * dx + dy * dy + dz * dz;
        if (!best || d < bestD) { best = &buildings[i]; bestD = d; }  // CThingCompare_Nearest
    }
    if (best && !best->Dead && best->InView) {
        V target = { creature.X + (creature.X - best->Pos.X), creature.Y + (creature.Y - best->Pos.Y), creature.Z + (creature.Z - best->Pos.Z) };
        g_lookAt = target; g_lookFrames = 0x11;
        return 1;                                                    // the look action's result
    }
    return 2;
}
int main() {
    V c = { 0, 0, 0 };
    Building b[3] = { { { 3, 4, 0 }, false, true }, { { 1, 1, 0 }, false, true }, { { 20, 0, 0 }, false, true } };
    bool ok = FaceAway(c, b, 3) == 1 && g_lookAt.X == -1 && g_lookAt.Y == -1 && g_lookFrames == 0x11;
    b[1].Dead = true; ok = ok && FaceAway(c, b, 3) == 2;             // the nearest is dead: nothing to face away from
    b[1].Dead = false; b[1].InView = false; ok = ok && FaceAway(c, b, 3) == 2;
    Building far[1] = { { { 20, 0, 0 }, false, true } }; ok = ok && FaceAway(c, far, 1) == 2;
    std::printf(ok ? "0092bf50_TEST PASS\n" : "0092bf50_TEST FAIL\n");
    return ok ? 0 : 1;
}