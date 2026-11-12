// Behaviour test for CTCTargetingAI::GetPBestCreatureForInteractionTypeWithinRangeLimit (0x007F67C0): the rough search box is
// the parent position truncated to integer cells grown by the truncated max range; the best creature is the nearest one that
// passes all four filters (available for the interaction type, inside [min,max] 2D range, can see each other, not unconscious).
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Box { long TLX, TLY, BRX, BRY; };
struct Creature { Vec Pos; long Interactions; bool Sees; bool Unconscious; };
static Box MakeArea(const Vec& pos, float max_range)
{
    long x = (long)pos.X, y = (long)pos.Y, r = (long)max_range;
    Box b = { x - r, y - r, x + r, y + r };
    return b;
}
static const Creature* Best(const Vec& pos, const Creature* cs, int n, long type, float min_range, float max_range)
{
    Box area = MakeArea(pos, max_range);
    const Creature* best = 0;
    float best_d = 0.0f;
    for (int i = 0; i < n; ++i) {
        const Creature& c = cs[i];
        if ((long)c.Pos.X < area.TLX || (long)c.Pos.X >= area.BRX || (long)c.Pos.Y < area.TLY || (long)c.Pos.Y >= area.BRY) continue;
        float dx = c.Pos.X - pos.X, dy = c.Pos.Y - pos.Y;
        float d2 = dx * dx + dy * dy;
        if (!(c.Interactions & type)) continue;
        if (d2 < min_range * min_range || d2 > max_range * max_range) continue;
        if (!c.Sees || c.Unconscious) continue;
        float dz = c.Pos.Z - pos.Z;
        float d3 = d2 + dz * dz;
        if (!best || d3 < best_d) { best = &c; best_d = d3; }
    }
    return best;
}
int main()
{
    Vec me = { 10.7f, -3.2f, 0.0f };
    Box a = MakeArea(me, 5.9f);
    if (a.TLX != 5 || a.TLY != -8 || a.BRX != 15 || a.BRY != 2) { std::printf("007f67c0_TEST FAIL\n"); return 1; }
    Creature cs[4] = {
        { { 11.0f, -3.0f, 0.0f }, 1, true, false },     // too close (inside min range)
        { { 13.0f, -3.0f, 0.0f }, 1, true, true },      // unconscious
        { { 14.0f, -3.0f, 0.0f }, 1, true, false },     // valid
        { { 13.5f, -3.2f, 0.0f }, 2, true, false },     // wrong interaction type
    };
    const Creature* b = Best(me, cs, 4, 1, 1.0f, 5.0f);
    if (b != &cs[2]) { std::printf("007f67c0_TEST FAIL\n"); return 1; }
    if (Best(me, cs, 4, 8, 1.0f, 5.0f) != 0) { std::printf("007f67c0_TEST FAIL\n"); return 1; }
    std::printf("007f67c0_TEST PASS\n");
    return 0;
}