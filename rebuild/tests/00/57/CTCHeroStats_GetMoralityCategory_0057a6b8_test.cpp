#include <cstdio>
// Behaviour model of CTCHeroStats::GetMoralityCategory (0x0057A6B8).
static int Category(float m)
{
    if (m < 0.142857149f) return 0;
    if (m < 0.285714298f) return 1;
    if (m < 0.428571433f) return 2;
    if (m < 0.571428597f) return 3;
    if (m < 0.714285731f) return 4;
    if (m < 0.857142866f) return 5;
    return 6;
}
int main()
{
    if (Category(0.0f) != 0 || Category(0.2f) != 1 || Category(0.5f) != 3 || Category(0.8f) != 5 || Category(1.0f) != 6 || Category(0.3f) != 2 || Category(0.6f) != 4) { std::printf("0057a6b8_TEST FAIL\n"); return 1; }
    std::printf("0057a6b8_TEST PASS\n");
    return 0;
}