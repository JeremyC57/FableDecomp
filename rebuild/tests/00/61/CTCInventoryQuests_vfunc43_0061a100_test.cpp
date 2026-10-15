#include <cstdio>
// Behaviour of CTCInventoryQuests::vfunc_43 (0x0061A100): 0.035f in test-quest mode, else the base value.
struct CTCInventoryBase_Retail { float m_defValue; float vfunc_43() { return m_defValue; } };
struct CTCInventoryQuests : CTCInventoryBase_Retail {
    bool ActiveTestQuestMode;
    float vfunc_43()
    {
        if (ActiveTestQuestMode)
            return 0.035f;
        float value = CTCInventoryBase_Retail::vfunc_43();
        return value;
    }
};
int main()
{
    CTCInventoryQuests q; q.m_defValue = 2.25f; q.ActiveTestQuestMode = false;
    if (q.vfunc_43() != 2.25f) { std::printf("0061a100_TEST FAIL\n"); return 1; }
    q.ActiveTestQuestMode = true;
    if (q.vfunc_43() != 0.035f) { std::printf("0061a100_TEST FAIL\n"); return 1; }
    std::printf("0061a100_TEST PASS\n");
    return 0;
}