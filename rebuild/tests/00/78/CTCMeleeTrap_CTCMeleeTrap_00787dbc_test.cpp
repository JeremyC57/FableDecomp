// behaviour test for 0x00787DBC CTCMeleeTrap::CTCMeleeTrap (model: the member defaults)
#include <cstdio>
struct Obj { bool PlayingContinuous; bool StrikeAnimation; Obj() : PlayingContinuous(false), StrikeAnimation(false) { } };
int main()
{
    Obj o;
    int ok = o.PlayingContinuous == false && o.StrikeAnimation == false;
    if (!ok) { std::printf("00787dbc_TEST FAIL\n"); return 1; }
    std::printf("00787dbc_TEST PASS\n");
    return 0;
}