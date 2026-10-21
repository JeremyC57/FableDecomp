#include <cstdio>
// Behaviour copy of CAIStateGroup_BanditSitRoundCampfire::Drink (0x0093A8C0).
static long g_frame = 0;
static float g_rate = 0.01f;
struct Drunk { float Drunkenness; };
struct Group {
    bool hasDrunk; Drunk drunk; long actionResult; int destroyed; int sheathed; float lastDuration;
    long Drink(float randomPart) {
        ++sheathed;
        float duration = randomPart + 5.0f;
        long start = g_frame;
        lastDuration = duration;
        g_frame += 120;                       // the action takes 120 frames
        long result = actionResult;
        ++destroyed;                          // DestroyItem("CARRY_SLOT_RIGHT_HAND")
        if (hasDrunk) {
            drunk.Drunkenness += (float)(g_frame - start) * g_rate;
            if (drunk.Drunkenness > 2.5f) drunk.Drunkenness = 2.5f;
        }
        return result == 2;
    }
};
int main() {
    Group g = { true, {1.0f}, 2, 0, 0, 0 };
    long r = g.Drink(2.0f);
    if (r != 1 || g.lastDuration != 7.0f || g.drunk.Drunkenness < 2.19f || g.drunk.Drunkenness > 2.21f || g.destroyed != 1) { std::printf("0093a8c0_TEST FAIL\n"); return 1; }
    r = g.Drink(0.0f);                        // 2.2 + 1.2 -> clamped to 2.5
    if (r != 1 || g.drunk.Drunkenness != 2.5f) { std::printf("0093a8c0_TEST FAIL\n"); return 1; }
    Group h = { false, {0.0f}, 4, 0, 0, 0 };
    if (h.Drink(1.0f) != 0 || h.drunk.Drunkenness != 0.0f) { std::printf("0093a8c0_TEST FAIL\n"); return 1; }
    std::printf("0093a8c0_TEST PASS\n"); return 0;
}
