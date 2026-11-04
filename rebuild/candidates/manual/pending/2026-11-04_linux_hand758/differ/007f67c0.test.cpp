// Behaviour test for CTCTargetingAI::GetPBestCreatureForInteractionTypeWithinRangeLimit (0x007F67C0), g++ model: nearest conscious, visible, available creature with min <= 2D distance <= max (box = centre +/- max).
#include <cstdio>
#include <vector>
struct Creature { float x, y, z; bool available, canSee, unconscious; };
static const Creature* Best(float px, float py, float pz, long type, float minR, float maxR, const std::vector<Creature>& cs)
{
    (void)type;
    long cx = (long)px, cy = (long)py, r = (long)maxR;
    const Creature* best = 0; float bestD = 0;
    for (size_t i = 0; i < cs.size(); ++i) {
        const Creature& c = cs[i];
        if (c.x < cx - r || c.x > cx + r || c.y < cy - r || c.y > cy + r) continue;
        float dx = c.x - px, dy = c.y - py, d2 = dx * dx + dy * dy;
        if (!c.available || d2 < minR * minR || d2 > maxR * maxR || !c.canSee || c.unconscious) continue;
        float d3 = d2 + (c.z - pz) * (c.z - pz);
        if (!best || d3 < bestD) { best = &c; bestD = d3; }
    }
    return best;
}
int main()
{
    std::vector<Creature> cs;
    Creature tooClose = { 1, 0, 0, true, true, false }, ok1 = { 6, 0, 0, true, true, false }, ok2 = { 0, 4, 0, true, true, false },
             blind = { 3, 0, 0, true, false, false }, ko = { 3.5f, 0, 0, true, true, true }, far = { 30, 0, 0, true, true, false };
    cs.push_back(tooClose); cs.push_back(ok1); cs.push_back(ok2); cs.push_back(blind); cs.push_back(ko); cs.push_back(far);
    const Creature* b = Best(0, 0, 0, 1, 2.0f, 10.0f, cs);
    int ok = b == &cs[2];
    ok = ok && Best(0, 0, 0, 1, 2.0f, 3.0f, cs) == 0;
    std::printf(ok ? "007f67c0_TEST PASS\n" : "007f67c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
