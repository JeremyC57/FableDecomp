// Behaviour test for CQ_ArenaScript::GivePrizeFund @ 0x00F21300: GoldMultiplier 0..7 adds 100/500/1000/2000/3000/4000/
// 8000/10000 and picks ROUNDn prize lines; any other multiplier adds nothing and plays empty lines.
#include <cstdio>
#include <string>
static const long Prize[8] = { 100, 500, 1000, 2000, 3000, 4000, 8000, 10000 };
static void Give(long mult, long& total, std::string& prize, std::string& later)
{
    prize = ""; later = "";
    if (mult >= 0 && mult <= 7) {
        total += Prize[mult];
        char b[80];
        std::sprintf(b, "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND%d_PRIZE", (int)mult + 1);
        prize = b; later = prize + "_LATER";
    }
}
int main()
{
    long total = 0; std::string p, l;
    Give(0, total, p, l); bool a = total == 100 && p == "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND1_PRIZE";
    Give(7, total, p, l); bool b = total == 10100 && l == "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND8_PRIZE_LATER";
    Give(8, total, p, l); bool c = total == 10100 && p.empty() && l.empty();
    Give(5, total, p, l); bool d = total == 14100;
    if (!(a && b && c && d)) { std::printf("00f21300_TEST FAIL\n"); return 1; }
    std::printf("00f21300_TEST PASS\n");
    return 0;
}