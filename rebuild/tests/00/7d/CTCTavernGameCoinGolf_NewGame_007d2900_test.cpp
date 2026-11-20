// Behaviour test for 0x007D2900 CTCTavernGameCoinGolf::NewGame (rewritten by NF159 on the shared headers): the row's object is linked
// in; the coin-base NewGame and CTCTavernGame::SetState are stubbed here and the override is called non-virtually on raw storage.
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CTCTavernGameCoinGolf.h"
#include <cstdio>
static int g_base = 0, g_calls = 0;
static long g_state = -1;
void CTCTavernGameCoinBase::NewGame() { ++g_base; if (g_calls != 0) g_state = -2; }   // must run before SetState
void CTCTavernGame::SetState(ETavernGameStates state) { ++g_calls; g_state = state; }
static double g_storage[sizeof(CTCTavernGameCoinGolf) / sizeof(double) + 1];
int main()
{
    CTCTavernGameCoinGolf* game = (CTCTavernGameCoinGolf*)g_storage;
    game->CTCTavernGameCoinGolf::NewGame();
    if (g_base != 1 || g_calls != 1 || g_state != 7) { std::printf("BAD\n"); return 1; }
    std::printf("ENDCUT_007d2900_OK\n");
    return 0;
}
