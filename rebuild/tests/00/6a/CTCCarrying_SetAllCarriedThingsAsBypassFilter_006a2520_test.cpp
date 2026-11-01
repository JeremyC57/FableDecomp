#include "rebuild_abi.h"
#include <cstdio>
static int g_reset = 0;
class CTCCarrying { public:
    unsigned char _pad[0x1c];
    bool  BypassFilters;
    void* Key;
    void ResetAllCarriedThingsAsBypassFilter();
    void SetAllCarriedThingsAsBypassFilter(void* key);
};
void CTCCarrying::ResetAllCarriedThingsAsBypassFilter() { g_reset++; }
int main()
{
    static CTCCarrying c;
    static int key;
    c.BypassFilters = false; c.Key = 0; g_reset = 0;
    c.SetAllCarriedThingsAsBypassFilter(&key);
    if (!c.BypassFilters) { std::printf("BAD_FLAG\n"); return 1; }
    if (c.Key != &key) { std::printf("BAD_VAL\n"); return 1; }
    if (g_reset != 1) { std::printf("BAD_HELP\n"); return 1; }
    std::printf("SETCARRY_006a2520_OK\n");
    return 0;
}
