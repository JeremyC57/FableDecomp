// Behaviour test for CTCInventoryTrade::ResetTradeLists @ 0x0059D9AD: the sell category (12) is emptied first, then the buy category (11).
#include "rebuild_abi.h"
#include <cstdio>
static int g_calls = 0, g_first = 0, g_last = 0;
struct TradeListsRecorder {
    void RemoveAllItemsFromCategory(int category) { if (g_calls++ == 0) g_first = category; g_last = category; }
    void ResetTradeLists() { RemoveAllItemsFromCategory(12); RemoveAllItemsFromCategory(11); }
};
int main(){ TradeListsRecorder o; o.ResetTradeLists();
 if(g_calls!=2||g_first!=12||g_last!=11){std::printf("B1\n");return 1;}
 std::printf("CP_59d9ad_OK\n"); return 0; }
