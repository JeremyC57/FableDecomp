// 00416231 CMainGameComponent::GetCurrentGameTime -- behaviour test (agentNF6: portable, GFGetTime spelling, exact PASS line)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CMainGameComponent { public:
    char pad[0x60];
    double gameTimeBase;
    double GetCurrentGameTime() const;
};
static double g_now;
double __fastcall GFGetTime() { return g_now; }
double CMainGameComponent::GetCurrentGameTime() const { return GFGetTime() - gameTimeBase; }
int main() {
    CMainGameComponent game;
    g_now = 17.5; game.gameTimeBase = 6.25;
    if (game.GetCurrentGameTime() != 11.25) return 1;
    g_now = 1.0; game.gameTimeBase = 3.5;
    if (game.GetCurrentGameTime() != -2.5) return 2;
    printf("00416231_TEST PASS\n");
    return 0;
}
