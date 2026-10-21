#include <cstdio>

extern long g_WorldFramesPerSecond;               // 0x01375550
struct CInputManager { char _pad[0x38]; bool b38; };
extern CInputManager* g_pInputManager;            // 0x013B8388
extern bool g_b13B860C;                           // 0x013B860C
extern bool g_b13B8629;                           // 0x013B8629
extern bool g_b13B8688;                           // 0x013B8688

class CMainGameComponent
{
public:
    void* _vfptr;                                 // +0x00 CBaseClass
    void* _vfptrEntry;                            // +0x04 CComponentListEntry
    bool m_b08;                                   // +0x08
    bool m_b09;                                   // +0x09
    double GetGameTimeOfNextPresentCompletion() const;   // 0x004166E2
    bool IsTimeForServerUpdate(long frame) const;
protected:
    double ConvertGTToWF(double gameTime) const { return gameTime / (1.0f / g_WorldFramesPerSecond); }
};

bool CMainGameComponent::IsTimeForServerUpdate(long frame) const
{
    if (m_b09) {
        if ((g_b13B860C && g_pInputManager->b38 && !g_b13B8629) || g_b13B8688) return true;
        float framesAhead = ConvertGTToWF(GetGameTimeOfNextPresentCompletion()) - frame;
        if (framesAhead > 1.0f) return true;
    }
    return false;
}

long g_WorldFramesPerSecond = 10;
CInputManager g_input;
CInputManager* g_pInputManager = &g_input;
bool g_b13B860C, g_b13B8629, g_b13B8688;
double g_next;
double CMainGameComponent::GetGameTimeOfNextPresentCompletion() const { return g_next; }
static int n_fail;
static void check(bool got, bool want, int line) { if (got != want) { ++n_fail; printf("FAIL line %d\n", line); } }
int main()
{
    CMainGameComponent c; c.m_b08 = true; c.m_b09 = false;
    g_b13B8688 = true; g_next = 100.0;
    check(c.IsTimeForServerUpdate(0), false, __LINE__);          // inactive
    c.m_b09 = true;
    check(c.IsTimeForServerUpdate(5000), true, __LINE__);        // forced flag
    g_b13B8688 = false;
    g_b13B860C = true; g_input.b38 = true; g_b13B8629 = false;
    check(c.IsTimeForServerUpdate(5000), true, __LINE__);        // input gate
    g_b13B8629 = true;
    g_next = 1.0;                                                // 10 world frames
    check(c.IsTimeForServerUpdate(8), true, __LINE__);           // 2 ahead
    check(c.IsTimeForServerUpdate(9), false, __LINE__);          // exactly 1 ahead
    check(c.IsTimeForServerUpdate(10), false, __LINE__);
    g_b13B860C = false; g_b13B8629 = false;
    check(c.IsTimeForServerUpdate(3), true, __LINE__);
    g_input.b38 = false; g_b13B860C = true;
    check(c.IsTimeForServerUpdate(9), false, __LINE__);
    if (!n_fail) printf("0041674a_TEST PASS\n");
    return 0;
}