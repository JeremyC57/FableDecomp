#include <cstdio>
// Behaviour of CTCInventoryWeapons::vfunc_36 (0x005C2E02): base hook then ResetInventoryAnim, same argument.
static long g_seq[4]; static long g_n = 0; static long g_arg[4];
struct CTCInventoryBase_Retail {
    void OnItemHighlighted(long item) { g_arg[g_n] = item; g_seq[g_n++] = 2; }
    void ResetInventoryAnim(long item) { g_arg[g_n] = item; g_seq[g_n++] = 3; }
};
class CTCInventoryWeapons : public CTCInventoryBase_Retail { public:
    void OnItemHighlighted(long item)
    {
        CTCInventoryBase_Retail::OnItemHighlighted(item);
        ResetInventoryAnim(item);
    }
};
int main()
{
    CTCInventoryWeapons w; w.OnItemHighlighted(-2);
    if (g_n != 2 || g_seq[0] != 2 || g_seq[1] != 3 || g_arg[0] != -2 || g_arg[1] != -2) { std::printf("005c2e02_TEST FAIL\n"); return 1; }
    std::printf("005c2e02_TEST PASS\n");
    return 0;
}