// behaviour test for 0x00416296
#include <stdio.h>
static long g_asked = -1;
class CGameJoystickManager {
public:
    long Device;
    static CGameJoystickManager& __fastcall Get() { static CGameJoystickManager m; return m; }
    long GetMainPlayerJoystickDeviceNumber() const { return Device; }
};
class CInputManager {
public:
    long Active;
    bool IsJoystickDeviceNumberActive(long device) const { g_asked = device; return device == Active; }
};
static CInputManager g_im; CInputManager* g_pInputManager = &g_im;
class CMainGameComponent {
public:
    bool IsControllerDisconnected() const;
};
bool CMainGameComponent::IsControllerDisconnected() const
{
    return g_pInputManager->IsJoystickDeviceNumberActive(CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber()) ? false : true;
}

int main()
{
    CMainGameComponent c; CGameJoystickManager::Get().Device = 2; g_im.Active = 2;
    int ok = c.IsControllerDisconnected() == false && g_asked == 2;
    g_im.Active = 1; ok &= c.IsControllerDisconnected() == true;
    printf(ok ? "00416296_TEST PASS\n" : "00416296_TEST FAIL\n");
    return ok ? 0 : 1;
}