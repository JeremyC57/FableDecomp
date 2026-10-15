#include <cstdio>
// Behaviour of CTCInventoryTrade::vfunc_36 (0x0059B71C): three base calls in order with the same argument.
static long g_seq[4]; static long g_n = 0; static long g_arg[4];
struct CTCInventoryBase_Retail {
    void SetNumberOfSelectedItemOwned(long item) { g_arg[g_n] = item; g_seq[g_n++] = 1; }
    void vfunc_36(long item) { g_arg[g_n] = item; g_seq[g_n++] = 2; }
    void ResetInventoryAnim(long item) { g_arg[g_n] = item; g_seq[g_n++] = 3; }
};
struct CTCInventoryTrade : CTCInventoryBase_Retail {
    void vfunc_36(long item)
    {
        SetNumberOfSelectedItemOwned(item);
        CTCInventoryBase_Retail::vfunc_36(item);
        ResetInventoryAnim(item);
    }
};
int main()
{
    CTCInventoryTrade t; t.vfunc_36(5);
    if (g_n != 3 || g_seq[0] != 1 || g_seq[1] != 2 || g_seq[2] != 3 || g_arg[0] != 5 || g_arg[1] != 5 || g_arg[2] != 5) { std::printf("0059b71c_TEST FAIL\n"); return 1; }
    std::printf("0059b71c_TEST PASS\n");
    return 0;
}