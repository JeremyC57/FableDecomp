#include <cstdio>
// Behaviour of CTCInventoryQuests::vfunc_42 (0x0061A0E0): 0.5f in test-quest mode, else the base value.
struct CTCInventoryBase_Retail { float m_defValue; float vfunc_42() { return m_defValue; } };
struct CTCInventoryQuests : CTCInventoryBase_Retail {
    bool ActiveTestQuestMode;
    float vfunc_42()
    {
        if (ActiveTestQuestMode)
            return 0.5f;
        float value = CTCInventoryBase_Retail::vfunc_42();
        return value;
    }
};
int main()
{
    CTCInventoryQuests q; q.m_defValue = 2.25f; q.ActiveTestQuestMode = false;
    if (q.vfunc_42() != 2.25f) { std::printf("0061a0e0_TEST FAIL\n"); return 1; }
    q.ActiveTestQuestMode = true;
    if (q.vfunc_42() != 0.5f) { std::printf("0061a0e0_TEST FAIL\n"); return 1; }
    std::printf("0061a0e0_TEST PASS\n");
    return 0;
}
