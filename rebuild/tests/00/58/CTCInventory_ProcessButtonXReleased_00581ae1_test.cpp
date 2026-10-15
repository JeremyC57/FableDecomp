#include <cstdio>
#include <cstring>
// Behaviour of 0x00581AE1 (chunk 901 piece): toggles +0x1cd (set only when +0x1cc is clear) and always calls the hook
static int g_hook = 0;
struct CTCInventory {
    virtual void v00() {}
    unsigned char _pad_0x04[0x1cc - 0x04];
    bool m_suppressA;
    bool m_suppressB;
    void Hook_005BC68F() { ++g_hook; }
    void ProcessButtonXReleased();
};
void CTCInventory::ProcessButtonXReleased()
{
    bool& flag = m_suppressB;
    if (flag) flag = false;
    else if (!m_suppressA) flag = true;
    Hook_005BC68F();
}
int main()
{
    CTCInventory inv; inv.m_suppressA = false; inv.m_suppressB = false;
    inv.ProcessButtonXReleased(); bool a = inv.m_suppressB;        // set
    inv.ProcessButtonXReleased(); bool b = inv.m_suppressB;        // cleared
    inv.m_suppressA = true; inv.ProcessButtonXReleased(); bool c = inv.m_suppressB;   // blocked
    inv.m_suppressB = true; inv.ProcessButtonXReleased(); bool d = inv.m_suppressB;   // cleared even when blocked
    if (!a || b || c || d || g_hook != 4) { std::printf("00581ae1_TEST FAIL\n"); return 1; }
    std::printf("00581ae1_TEST PASS\n"); return 0;
}