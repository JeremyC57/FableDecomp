// Behaviour test for CAIStateGroup_MazeTeleport::Update (0x00954F40): the marker named "MazeTeleportPos<boss phase>" is searched in
// the 400x400 box around the creature (truncated position +- 200); found, its position becomes the teleport target and the boss's
// phase start position; otherwise a valid hiding position within 20 units is picked with the world seed (seed = ror13(seed * 0x24A1 +
// 0x24DF), index = seed % count), or, when there is none, the state group terminates first. The teleport-out action is then performed
// (no wait) and the state group terminates.
#include <cstdio>
#include <cstring>
#include <vector>
struct V { float X, Y, Z; };
struct Marker { const char* name; V pos; };
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed *= 0x24a1; *seed += 0x24df; *seed = ror13(*seed);
    if (range) return *seed % range;
    return 0;
}
struct World { std::vector<Marker> markers; std::vector<V> valid; unsigned long seed; };
struct State {
    World* world; V creature; long boss_phase; V phase_start; bool phase_start_set; int terminates; int actions; V action_pos;
    void Update()
    {
        const Marker* pMarkerThing = 0;
        char marker_name[32];
        std::sprintf(marker_name, "MazeTeleportPos%d", (int)boss_phase);
        long x = (long)creature.X, y = (long)creature.Y;
        for (size_t i = 0; i < world->markers.size() && !pMarkerThing; ++i) {
            const Marker& m = world->markers[i];
            if (m.pos.X >= x - 200 && m.pos.X <= x + 200 && m.pos.Y >= y - 200 && m.pos.Y <= y + 200 && std::strcmp(m.name, marker_name) == 0)
                pMarkerThing = &m;
        }
        V teleport_pos = { -1, -1, -1 };
        if (pMarkerThing) {
            teleport_pos = pMarkerThing->pos;
            phase_start = teleport_pos; phase_start_set = true;
        } else {
            std::vector<V> validPositions = world->valid;
            if (validPositions.size() == 0)
                ++terminates;
            else
                teleport_pos = validPositions[GFRandom((unsigned long)validPositions.size(), &world->seed)];
        }
        ++actions; action_pos = teleport_pos;
        ++terminates;
    }
};
static int fail(int n) { std::printf("00954f40_TEST FAIL %d\n", n); return 1; }
int main()
{
    World w; w.seed = 12345;
    Marker m1 = { "MazeTeleportPos1", { 50.0f, 60.0f, 7.0f } }, m2 = { "MazeTeleportPos2", { 10.0f, 10.0f, 1.0f } }, far2 = { "MazeTeleportPos2", { 900.0f, 0.0f, 0.0f } };
    w.markers.push_back(far2); w.markers.push_back(m1); w.markers.push_back(m2);
    V v0 = { 1, 2, 3 }, v1 = { 4, 5, 6 }, v2 = { 7, 8, 9 };
    w.valid.push_back(v0); w.valid.push_back(v1); w.valid.push_back(v2);
    State s = { &w, { 0.0f, 0.0f, 0.0f }, 2, { 0, 0, 0 }, false, 0, 0, { 0, 0, 0 } };
    s.Update();                                            // phase 2: the near marker (the far one is outside the box)
    if (!s.phase_start_set || s.action_pos.X != 10.0f || s.phase_start.Z != 1.0f || s.terminates != 1 || s.actions != 1) return fail(1);
    if (w.seed != 12345) return fail(2);                   // no random draw
    State t = { &w, { 0.0f, 0.0f, 0.0f }, 3, { 0, 0, 0 }, false, 0, 0, { 0, 0, 0 } };
    unsigned long seed = 12345; unsigned long expect = GFRandom(3, &seed);
    t.Update();                                            // phase 3: no marker -> random valid position
    if (t.phase_start_set || w.seed != seed || t.action_pos.X != w.valid[expect].X || t.terminates != 1) return fail(3);
    State u = { &w, { 1000.0f, 0.0f, 0.0f }, 1, { 0, 0, 0 }, false, 0, 0, { 0, 0, 0 } };
    u.Update();                                            // phase 1 marker exists but out of range: random again
    if (u.phase_start_set || u.terminates != 1) return fail(4);
    w.valid.clear();
    State e = { &w, { 0.0f, 0.0f, 0.0f }, 5, { 0, 0, 0 }, false, 0, 0, { 0, 0, 0 } };
    e.Update();                                            // nothing anywhere: terminates before the action, and after
    if (e.terminates != 2 || e.actions != 1) return fail(5);
    unsigned long z = 7; if (GFRandom(0, &z) != 0) return fail(6);
    std::printf("00954f40_TEST PASS\n");
    return 0;
}