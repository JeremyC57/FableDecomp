// Behaviour test for CTCTargetingAI::GetPBestCreatureForInteractionType (0x007F68B0), g++ model: nearest available, aware, not incapacitated AI creature inside the extended-sight box.
#include <cstdio>
#include <vector>
struct Creature { float x, y; bool available, aware, incapacitated; };
static const Creature* Best(float px, float py, float sight, const std::vector<Creature>& cs)
{
    long x = (long)px, y = (long)py, r = (long)sight;
    const Creature* best = 0; float bestD = 0;
    for (size_t i = 0; i < cs.size(); ++i) {
        const Creature& c = cs[i];
        if (c.x < x - r || c.x > x + r || c.y < y - r || c.y > y + r) continue;
        if (!c.available || !c.aware || c.incapacitated) continue;
        float d = (c.x - px) * (c.x - px) + (c.y - py) * (c.y - py);
        if (!best || d < bestD) { best = &c; bestD = d; }
    }
    return best;
}
int main()
{
    std::vector<Creature> cs;
    Creature a = { 3, 3, true, true, false }, b = { 1, 0, true, false, false }, c = { 2, 0, true, true, true }, d = { 50, 0, true, true, false };
    cs.push_back(a); cs.push_back(b); cs.push_back(c); cs.push_back(d);
    int ok = Best(0, 0, 10, cs) == &cs[0] && Best(0, 0, 2.5f, cs) == 0 && Best(48, 0, 5, cs) == &cs[3];
    std::printf(ok ? "007f68b0_TEST PASS\n" : "007f68b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
