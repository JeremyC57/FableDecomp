// Behaviour test for CTCTavernGameCoinBase::UpdateMeshes (0x008EF2F0): only live, owned coins are positioned, at (x, y, 0)
// with the angle in revolutions (angle / 2pi).
#include <cstdio>
class Coin { public: float X, Y, Angle; bool Owned; bool HasThing; bool Dead; };
static int g_calls; static float g_lastX, g_lastY, g_lastTurn;
int main()
{
    Coin coins[3] = { { 1, 2, 3.14159265f, true, true, false }, { 5, 6, 0, false, true, false }, { 7, 8, 0, true, true, true } };
    for (Coin* c = coins; c != coins + 3; ++c) {
        if (c->HasThing && !c->Dead && c->Owned) {
            ++g_calls; g_lastX = c->X; g_lastY = c->Y; g_lastTurn = c->Angle * 0.15915494f;
        }
    }
    if (g_calls != 1 || g_lastX != 1 || g_lastY != 2 || g_lastTurn < 0.4999f || g_lastTurn > 0.5001f) {
        std::printf("008ef2f0_TEST FAIL\n"); return 1;
    }
    std::printf("008ef2f0_TEST PASS\n");
    return 0;
}