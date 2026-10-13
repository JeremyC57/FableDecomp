#include <cstdio>

struct P { unsigned char pad[0x18]; float AwarenessTimer; long Is() { if (AwarenessTimer != 0.0f) return 1; return 0; } };
int main()
{
    P p; p.AwarenessTimer = 0.0f; if (p.Is() != 0) { std::printf("00643b3b_TEST FAIL\n"); return 1; }
    p.AwarenessTimer = 2.5f; if (p.Is() != 1) { std::printf("00643b3b_TEST FAIL\n"); return 1; }
    std::printf("00643b3b_TEST PASS\n");
    return 0;
}