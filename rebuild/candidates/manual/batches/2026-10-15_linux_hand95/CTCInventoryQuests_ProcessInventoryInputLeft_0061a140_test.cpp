#include <cstdio>
// Behaviour of CTCInventoryQuests::ProcessInventoryInputLeft (0x0061A140).
struct CTCInventoryBase_Retail {
    long m_categories; long m_baseCalls;
    long GetNumberOfCategories() { return m_categories; }
    void ProcessInventoryInputLeft() { ++m_baseCalls; }
};
struct CTCInventoryQuests : CTCInventoryBase_Retail {
    bool ActiveTestQuestMode;
    void ProcessInventoryInputLeft()
    {
        if (!ActiveTestQuestMode && GetNumberOfCategories() > 0)
            CTCInventoryBase_Retail::ProcessInventoryInputLeft();
    }
};
int main()
{
    CTCInventoryQuests q; q.m_baseCalls = 0; q.m_categories = 2; q.ActiveTestQuestMode = false;
    q.ProcessInventoryInputLeft();
    int bad = q.m_baseCalls != 1;
    q.ActiveTestQuestMode = true; q.ProcessInventoryInputLeft(); bad |= q.m_baseCalls != 1;
    q.ActiveTestQuestMode = false; q.m_categories = 0; q.ProcessInventoryInputLeft(); bad |= q.m_baseCalls != 1;
    q.m_categories = -1; q.ProcessInventoryInputLeft(); bad |= q.m_baseCalls != 1;
    if (bad) { std::printf("0061a140_TEST FAIL\n"); return 1; }
    std::printf("0061a140_TEST PASS\n");
    return 0;
}
