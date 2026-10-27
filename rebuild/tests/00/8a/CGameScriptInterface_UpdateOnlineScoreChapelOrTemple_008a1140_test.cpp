#include <cstdio>
// Behaviour model: the activity code and score reach the scoreboard unchanged.
static int act; static float sc;
static void update(int a, float s, bool) { act = a; sc = s; }
int main()
{
    update(0xA, 12.5f, false);
    if (act != 0xA || sc != 12.5f) { std::printf("008a1140_TEST FAIL\n"); return 1; }
    std::printf("008a1140_TEST PASS\n");
    return 0;
}