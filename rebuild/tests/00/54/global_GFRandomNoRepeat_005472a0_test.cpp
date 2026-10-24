// Behaviour test for 005472a0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

static int g_debugMessages = 0;
static bool g_stuck = false;
static void OutputDebugStringW(const unsigned short* text) { ++g_debugMessages; CHECK(text[0] == 'G'); }   // KERNEL32 stub
static unsigned long GFROR13(unsigned long value)       // stub for 0x00497890: rotate right by 13 (or stuck)
{
    if (g_stuck) return 0x12345u;
    value &= 0xffffffffu;
    return ((value >> 13) | (value << 19)) & 0xffffffffu;
}
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = (*seed * 0x24a1 + 0x24df) & 0xffffffffu;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
static const unsigned short g_message[] = { 'G', 'F', 'R', 0 };
static unsigned long GFRandomNoRepeat(unsigned long range, unsigned long previous, unsigned long* seed)
{
    unsigned long result = GFRandom(range, seed);
    long attempts = 0;
    while (range > 1 && result == previous) {
        result = GFRandom(range, seed);
        if (++attempts > 100) {
            OutputDebugStringW(g_message);
            break;
        }
    }
    return result;
}
static void RunTest()
{
    unsigned long seed = 12345;
    unsigned long previous = 0;
    for (int i = 0; i < 200; ++i) {
        unsigned long r = GFRandomNoRepeat(3, previous, &seed);
        CHECK(r < 3 && r != previous);                     // never repeats while range > 1
        previous = r;
    }
    CHECK(GFRandomNoRepeat(1, 0, &seed) == 0);             // range 1: the repeat is allowed
    CHECK(GFRandomNoRepeat(0, 0, &seed) == 0);             // range 0: 0 without dividing
    CHECK(g_debugMessages == 0);
    g_stuck = true;                                        // a generator stuck on one value: give up after 101 retries
    unsigned long stuckValue = 0x12345u % 7;
    CHECK(GFRandomNoRepeat(7, stuckValue, &seed) == stuckValue);
    CHECK(g_debugMessages == 1);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("005472a0_TEST FAIL\n");
        return 1;
    }
    std::printf("005472a0_TEST PASS\n");
    return 0;
}