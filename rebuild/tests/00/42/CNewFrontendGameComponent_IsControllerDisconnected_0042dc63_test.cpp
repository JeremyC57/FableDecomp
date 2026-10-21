// test for 0042dc63 IsControllerDisconnected
#include <stdio.h>
class CGameJoystickManager {
public:
    static CGameJoystickManager& __fastcall Get();       // 0x0049BB90
    bool HasExplicitMainPlayerJoystickDeviceNumber() const;   // 0x0049BB50
    long GetMainPlayerJoystickDeviceNumber() const;      // 0x0049BB40
};
class CInputManager {
public:
    bool IsJoystickDeviceNumberActive(long device_number) const;   // 0x009F5250
};
extern CInputManager* g_pInputManager;                   // 0x013B8388
class CNewFrontendGameComponent {
public:
    unsigned char _body[0x10];
private:
    bool IsControllerDisconnected();
    friend struct CNewFrontendGameComponent_Test;        // behaviour-test hook (the method is private)
};
bool CNewFrontendGameComponent::IsControllerDisconnected()
{
    if (CGameJoystickManager::Get().HasExplicitMainPlayerJoystickDeviceNumber())
        return !g_pInputManager->IsJoystickDeviceNumberActive(CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber());
    return false;
}

static bool g_has, g_active; static long g_dev, g_asked;
static CGameJoystickManager g_jm; static CInputManager g_im;
CInputManager* g_pInputManager = &g_im;
CGameJoystickManager& __fastcall CGameJoystickManager::Get() { return g_jm; }
bool CGameJoystickManager::HasExplicitMainPlayerJoystickDeviceNumber() const { return g_has; }
long CGameJoystickManager::GetMainPlayerJoystickDeviceNumber() const { return g_dev; }
bool CInputManager::IsJoystickDeviceNumberActive(long d) const { g_asked = d; return g_active; }
struct CNewFrontendGameComponent_Test { static bool Run(CNewFrontendGameComponent& c) { return c.IsControllerDisconnected(); } };
int main()
{
    CNewFrontendGameComponent c; g_dev = 3; g_asked = -1;
    g_has = false; g_active = false; int ok = !CNewFrontendGameComponent_Test::Run(c) && g_asked == -1;
    g_has = true; g_active = true; ok = ok && !CNewFrontendGameComponent_Test::Run(c) && g_asked == 3;
    g_active = false; ok = ok && CNewFrontendGameComponent_Test::Run(c);
    printf(ok ? "0042dc63_TEST PASS\n" : "0042dc63_TEST FAIL\n");
    return 0;
}