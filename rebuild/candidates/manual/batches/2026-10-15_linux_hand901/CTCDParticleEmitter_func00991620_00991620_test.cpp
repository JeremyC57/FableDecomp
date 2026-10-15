#include <cstdio>
#include <cstring>
// Behaviour of 0x00991620 (chunk 901 piece): stores and notifies (slot 2) only when the flag changes
static int g_notify = 0; static bool g_last = false;
struct CFlaggedObject_Retail {
    virtual void v00() {}
    virtual void v01() {}
    virtual void OnFlagChanged(bool enabled) { ++g_notify; g_last = enabled; }
    unsigned char _pad_0x04[0x26 - 0x04];
    bool m_flag26;
    void func_00991620(bool enabled);
};
void CFlaggedObject_Retail::func_00991620(bool enabled) { if (m_flag26 != enabled) { m_flag26 = enabled; OnFlagChanged(enabled); } }
int main()
{
    CFlaggedObject_Retail o; o.m_flag26 = false;
    o.func_00991620(false);                 // no change
    o.func_00991620(true);                  // change -> notify(true)
    o.func_00991620(true);                  // no change
    o.func_00991620(false);                 // change -> notify(false)
    if (g_notify != 2 || g_last || o.m_flag26) { std::printf("00991620_TEST FAIL\n"); return 1; }
    std::printf("00991620_TEST PASS\n"); return 0;
}
