// Behaviour test for 0x0042DEC5 CNewFrontendGameComponent::InitialiseInput.
#include <cstdio>
class CInputManager { public: int id; };
template <class T> struct CCountedPointer { T* Data; long* RefCount; };
struct CSystemManager { unsigned char pad[0x58]; CCountedPointer<CInputManager> PInputManager; };
static CSystemManager g_system;
static int g_calls = 0;
CSystemManager* GFGetSystemManager() { ++g_calls; return &g_system; }
class CNewFrontendGameComponent {
public:
    void InitialiseInput() { m_pInputManager = GFGetSystemManager()->PInputManager.Data; }
    CInputManager* m_pInputManager;
};
int main()
{
    CInputManager input; input.id = 7;
    g_system.PInputManager.Data = &input; g_system.PInputManager.RefCount = 0;
    CNewFrontendGameComponent c; c.m_pInputManager = 0;
    c.InitialiseInput();
    if (c.m_pInputManager != &input || g_calls != 1) { std::printf("0042dec5_TEST FAIL\n"); return 1; }
    g_system.PInputManager.Data = 0;
    c.InitialiseInput();
    if (c.m_pInputManager != 0 || g_calls != 2) { std::printf("0042dec5_TEST FAIL\n"); return 1; }
    std::printf("0042dec5_TEST PASS\n");
    return 0;
}
