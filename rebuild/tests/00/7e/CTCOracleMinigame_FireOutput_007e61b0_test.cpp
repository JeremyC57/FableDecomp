#include <cstdio>
// Behaviour model of CTCOracleMinigame::FireOutput (0x007E61B0): LCG step + rotate-right-13, symbol = low two bits.
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
struct Game { unsigned long seed; long fired; long output[32]; long firing[4]; long frames[4]; int sounds; };
static void FireOutput(Game& g, long frame)
{
    g.seed = (g.seed * 0x24a1u + 0x24dfu) & 0xffffffffu;
    g.seed = Ror13(g.seed) & 0xffffffffu;
    long symbol = (long)(g.seed & 3);
    g.output[g.fired] = symbol;
    g.firing[symbol] = 1;
    g.frames[symbol] = frame;
    ++g.fired;
    g.sounds++;
}
int main()
{
    Game g = { 12345u, 0 };
    FireOutput(g, 7); FireOutput(g, 8);
    if (g.fired != 2 || g.sounds != 2 || g.output[0] < 0 || g.output[0] > 3 || g.firing[g.output[1]] != 1 || g.frames[g.output[1]] != 8) { std::printf("007e61b0_TEST FAIL\n"); return 1; }
    std::printf("007e61b0_TEST PASS\n");
    return 0;
}