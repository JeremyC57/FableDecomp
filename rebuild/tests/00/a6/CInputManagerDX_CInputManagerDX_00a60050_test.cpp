// Behaviour test for CInputManagerDX::CInputManagerDX 0x00A60050 (model copy: which devices the init record creates).
#include <cstdio>
static int g_created[3], g_deleted[3], g_dinput;
struct Device { int kind; Device(int k) : kind(k) { ++g_created[k]; } virtual ~Device() { ++g_deleted[kind]; } };
struct Init { bool UseMouse, UseJoystick, UseKeyboard; };
struct SystemManager { void* WinInstanceHandle; void* GetWinInstanceHandle() { return WinInstanceHandle; } };
static SystemManager g_sys;
SystemManager* GFGetSystemManager() { return &g_sys; }
long DirectInput8Create(void* instance, unsigned long version, void** out) { if (instance == &g_sys && version == 0x800) { *out = &g_dinput; return 0; } return -1; }
template <class T> struct scoped_ptr { T* ptr; void reset(T* p) { T* old = ptr; ptr = p; delete old; } };
struct Manager {
    void* DInput; scoped_ptr<Device> PMouse, PKeyboard; Device* Joysticks[4]; int JoystickCount;
    Manager(const Init& init)
    {
        DInput = 0; PMouse.ptr = 0; PKeyboard.ptr = 0; JoystickCount = 0;
        void* instance = GFGetSystemManager()->GetWinInstanceHandle();
        DirectInput8Create(instance, 0x800, &DInput);
        if (init.UseJoystick) Joysticks[JoystickCount++] = new Device(0);
        if (init.UseKeyboard) PKeyboard.reset(new Device(1));
        if (init.UseMouse) PMouse.reset(new Device(2));
    }
};
int main()
{
    g_sys.WinInstanceHandle = &g_sys;
    Init a = { true, false, true };
    Manager m(a);
    bool ok = m.DInput == &g_dinput && m.JoystickCount == 0 && m.PKeyboard.ptr && m.PMouse.ptr && g_created[1] == 1 && g_created[2] == 1;
    Init b = { false, true, false };
    Manager n(b);
    if (n.JoystickCount != 1 || n.PKeyboard.ptr || n.PMouse.ptr || g_created[0] != 1) ok = false;
    m.PMouse.reset(0);
    if (g_deleted[2] != 1) ok = false;
    if (!ok) { std::printf("00a60050_TEST FAIL\n"); return 1; }
    std::printf("00a60050_TEST PASS\n");
    return 0;
}