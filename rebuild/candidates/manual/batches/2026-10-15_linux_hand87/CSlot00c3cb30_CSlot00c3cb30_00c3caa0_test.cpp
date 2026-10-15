#include <cstdio>
static int g_clears = 0;
struct CSlotBase { virtual void v00() { } ~CSlotBase() { } };
struct CSlot : CSlotBase {
    void* m_pHead; void* m_pTail; long m_index; long m_count;
    virtual void v00() { }
    bool IsOpen() const { return m_index >= 0; }
    void Clear() { ++g_clears; m_count = 0; m_index = -1; }
    ~CSlot() { if (m_count != 0 || IsOpen()) Clear(); }
};
int main()
{
    { CSlot a; a.m_index = -1; a.m_count = 0; }
    bool idle = g_clears == 0;
    { CSlot b; b.m_index = -1; b.m_count = 3; }
    bool counted = g_clears == 1;
    { CSlot c; c.m_index = 0; c.m_count = 0; }
    if (!idle || !counted || g_clears != 2) { std::printf("00c3caa0_TEST FAIL\n"); return 1; }
    std::printf("00c3caa0_TEST PASS\n");
    return 0;
}
