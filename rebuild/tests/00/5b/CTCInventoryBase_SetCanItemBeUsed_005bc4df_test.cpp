#include <cstdio>
// Behaviour test for CTCInventoryBase::SetCanItemBeUsed (005bc4df): same logic over a copy of the model.
static int g_lookups = 0;
static long LookUpInventoryItemDef(long defIndex) { ++g_lookups; return defIndex == 5 ? 0x1234 : 0; }
struct CTCInventoryBase_Retail {
    bool CanItemBeQuickAssigned; bool CanItemBeUsed;
    void SetCanItemBeUsed(long defIndex)
    {
        if (defIndex > 0)
            CanItemBeUsed = LookUpInventoryItemDef(defIndex) != 0;
        else
            CanItemBeUsed = false;
    }
};
int main()
{
    CTCInventoryBase_Retail b; b.CanItemBeQuickAssigned = false; b.CanItemBeUsed = false;
    b.SetCanItemBeUsed(5);
    if (!b.CanItemBeUsed || g_lookups != 1) { std::printf("005bc4df_TEST FAIL\n"); return 1; }
    b.SetCanItemBeUsed(-3);
    if (b.CanItemBeUsed || g_lookups != 1) { std::printf("005bc4df_TEST FAIL\n"); return 1; }
    b.SetCanItemBeUsed(5); b.SetCanItemBeUsed(6);
    if (b.CanItemBeUsed || g_lookups != 3) { std::printf("005bc4df_TEST FAIL\n"); return 1; }
    std::printf("005bc4df_TEST PASS\n");
    return 0;
}