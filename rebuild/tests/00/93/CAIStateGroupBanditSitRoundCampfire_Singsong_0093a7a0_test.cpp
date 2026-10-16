// behaviour test for CAIStateGroup_BanditSitRoundCampfire::Singsong (0x0093A7A0)
#include <cstdio>
#include <cstring>
static bool sheathed; static float lastDuration; static const char* lastInto; static const char* lastLoop; static const char* lastOut;
static float Rand(float range, unsigned long* seed) { *seed = *seed * 1103515245u + 12345u; return range * (float)((*seed >> 8) & 0xffff) / 65536.0f; }
static long Perform(const char* into, const char* loop, const char* out, float d, long r) { lastInto = into; lastLoop = loop; lastOut = out; lastDuration = d; return r; }
static long Singsong(unsigned long* seed, long actionResult)
{
    sheathed = true;
    float duration = Rand(5.0f, seed) + 5.0f;
    long result = Perform("ST_SITTING_SING_SONG_DRUNK_INTO", "ST_SITTING_SING_SONG_DRUNK_LOOP", "ST_SITTING_SING_SONG_DRUNK_LOOP", duration, actionResult);
    return result == 2;
}
int main()
{
    unsigned long seed = 7;
    bool ok = Singsong(&seed, 2) == 1 && sheathed && lastDuration >= 5.0f && lastDuration < 10.0f
        && std::strcmp(lastInto, "ST_SITTING_SING_SONG_DRUNK_INTO") == 0 && std::strcmp(lastOut, "ST_SITTING_SING_SONG_DRUNK_LOOP") == 0;
    ok = ok && Singsong(&seed, 3) == 0;
    if (!ok) { std::printf("0093a7a0_TEST FAIL\n"); return 1; }
    std::printf("0093a7a0_TEST PASS\n");
    return 0;
}