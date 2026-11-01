// Behaviour test for the dynamic initializer of CTCFireheartMinigame::BlockNames (0x0121F2D0): a 4x4 static array of
// CCharStrings "BLOCKrc" in row-major order.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static long g_liveStrings = 0;
class CCharString { public:
    CCharString(const char* text, long length = -1) { PStringData = strdup(text); (void)length; ++g_liveStrings; }
    ~CCharString() { free(PStringData); --g_liveStrings; }
    char* PStringData;
};
class CTCFireheartMinigame { public: static CCharString BlockNames[4][4]; };
CCharString CTCFireheartMinigame::BlockNames[4][4] = {
    { "BLOCK00", "BLOCK01", "BLOCK02", "BLOCK03" },
    { "BLOCK10", "BLOCK11", "BLOCK12", "BLOCK13" },
    { "BLOCK20", "BLOCK21", "BLOCK22", "BLOCK23" },
    { "BLOCK30", "BLOCK31", "BLOCK32", "BLOCK33" }
};
int main()
{
    if (g_liveStrings != 16) { printf("0121f2d0_TEST FAIL live=%ld\n", g_liveStrings); return 1; }
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            char want[16]; sprintf(want, "BLOCK%d%d", r, c);
            if (strcmp(CTCFireheartMinigame::BlockNames[r][c].PStringData, want) != 0) { printf("0121f2d0_TEST FAIL [%d][%d]\n", r, c); return 1; }
        }
    printf("0121f2d0_TEST PASS\n");
    return 0;
}