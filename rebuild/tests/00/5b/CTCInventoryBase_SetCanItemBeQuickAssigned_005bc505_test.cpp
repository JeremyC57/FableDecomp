#include <cstdio>
// Behaviour test for CTCInventoryBase::SetCanItemBeQuickAssigned (005bc505): same logic over a copy of the model.
static int g_lookups = 0;
static long LookUpInventoryItemDef(long defIndex) { ++g_lookups; return defIndex == 5 ? 0x1234 : 0; }
struct CTCInventoryBase_Retail {
    bool CanItemBeQuickAssigned; bool CanItemBeUsed;
    void SetCanItemBeQuickAssigned(long defIndex)
    {
        if (defIndex > 0)
            CanItemBeQuickAssigned = LookUpInventoryItemDef(defIndex) != 0;
        else
            CanItemBeQuickAssigned = false;
    }
};
int main()
{
    CTCInventoryBase_Retail b; b.CanItemBeQuickAssigned = false; b.CanItemBeUsed = false;
    b.SetCanItemBeQuickAssigned(5);
    if (!b.CanItemBeQuickAssigned || g_lookups != 1) { std::printf("005bc505_TEST FAIL\n"); return 1; }
    b.SetCanItemBeQuickAssigned(-3);
    if (b.CanItemBeQuickAssigned || g_lookups != 1) { std::printf("005bc505_TEST FAIL\n"); return 1; }
    b.SetCanItemBeQuickAssigned(5); b.SetCanItemBeQuickAssigned(6);
    if (b.CanItemBeQuickAssigned || g_lookups != 3) { std::printf("005bc505_TEST FAIL\n"); return 1; }
    std::printf("005bc505_TEST PASS\n");
    return 0;
}