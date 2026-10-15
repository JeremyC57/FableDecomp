#include <cstdio>
// Behaviour of CTCInventoryMap::ProcessButtonBReleased (0x005FD0B0).
struct CTCInventoryBase_Retail {
    long InventorySelectionState; long m_closeCalls; long m_baseCalls; bool m_closeArg;
    virtual void Close(bool immediate) { ++m_closeCalls; m_closeArg = immediate; }
    void ProcessButtonBReleased() { ++m_baseCalls; }
};
struct CTCInventoryMap : CTCInventoryBase_Retail {
    bool m_closing; long m_clears;
    void ClearMapScreenPrimitives() { ++m_clears; }
    void ProcessButtonBReleased()
    {
        if (InventorySelectionState == 0 && !m_closing) {
            m_closing = true;
            ClearMapScreenPrimitives();
            Close(true);
            return;
        }
        CTCInventoryBase_Retail::ProcessButtonBReleased();
    }
};
int main()
{
    CTCInventoryMap m; m.InventorySelectionState = 0; m.m_closeCalls = 0; m.m_baseCalls = 0; m.m_closeArg = false;
    m.m_closing = false; m.m_clears = 0;
    m.ProcessButtonBReleased();
    int bad = (m.m_closeCalls != 1 || !m.m_closeArg || !m.m_closing || m.m_clears != 1 || m.m_baseCalls != 0);
    m.ProcessButtonBReleased();                         // latched: goes to the base
    bad |= (m.m_closeCalls != 1 || m.m_baseCalls != 1);
    m.m_closing = false; m.InventorySelectionState = 2; m.ProcessButtonBReleased();
    bad |= (m.m_closeCalls != 1 || m.m_baseCalls != 2 || m.m_closing);
    if (bad) { std::printf("005fd0b0_TEST FAIL\n"); return 1; }
    std::printf("005fd0b0_TEST PASS\n");
    return 0;
}