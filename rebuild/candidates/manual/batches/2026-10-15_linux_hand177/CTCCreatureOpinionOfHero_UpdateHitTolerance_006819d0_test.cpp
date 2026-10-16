#include <cstdio>
// Behaviour model of CTCCreatureOpinionOfHero::UpdateHitTolerance: hit count, decay frame, tolerance exhausted test.
static long g_frame = 1000;
static long g_decay = 5;
struct Opinion {
    float hits, override_, managerTolerance, personality; bool hasPersonality; long decayFrame;
    float Remaining() const
    {
        if (override_ >= 0.0f) return override_;
        float t = managerTolerance;
        if (hasPersonality) t += personality;
        return t;
    }
    bool UpdateHitTolerance(bool heavy)
    {
        hits += heavy ? 1.5f : 1.0f;
        long d = g_decay;
        decayFrame = g_frame + d * 0x78;
        if (heavy) return true;
        return Remaining() - hits <= 0.0f;
    }
};
int main()
{
    Opinion o; o.hits = 0; o.override_ = -1.0f; o.managerTolerance = 2.0f; o.personality = 1.0f; o.hasPersonality = true; o.decayFrame = 0;
    if (o.UpdateHitTolerance(false) || o.hits != 1.0f || o.decayFrame != 1600) { std::printf("006819d0_TEST FAIL\n"); return 1; }
    if (!o.UpdateHitTolerance(true) || o.hits != 2.5f) { std::printf("006819d0_TEST FAIL\n"); return 1; }
    if (!o.UpdateHitTolerance(false)) { std::printf("006819d0_TEST FAIL\n"); return 1; }   // 3.5 >= 3
    o.hits = 0; o.override_ = 10.0f;
    if (o.UpdateHitTolerance(false)) { std::printf("006819d0_TEST FAIL\n"); return 1; }
    std::printf("006819d0_TEST PASS\n");
    return 0;
}
