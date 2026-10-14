#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = ""; struct IDevice { virtual unsigned long Release() { std::strcat(g_seq, "R"); return 0; } virtual long Acquire() { std::strcat(g_seq, "A"); return 0; } virtual long Unacquire() { std::strcat(g_seq, "U"); return 0; } };
struct CJoystickDX; static IDevice* g_pNewDevice = 0; static unsigned long g_class = 0, g_flags = 0; static void* g_ref = 0;
static long EnumJoysticksCallback(const void*, void*) { return 0; }
struct CDirectInput { virtual long EnumDevices(unsigned long devClass, void* callback, void* ref, unsigned long flags); };
struct CInputManagerDX { CDirectInput* m_pDirectInput; };
struct CJoystickDX { bool ExclusiveMode; IDevice* DJoystick; bool Acquired; CInputManagerDX* InputManagerDX;
    bool SetExclusiveMode(bool exclusive) { if (DJoystick) { DJoystick->Unacquire(); DJoystick->Release(); DJoystick = 0; } ExclusiveMode = true;
        InputManagerDX->m_pDirectInput->EnumDevices(4, (void*)EnumJoysticksCallback, this, 1); return DJoystick != 0 ? true : false; } };
long CDirectInput::EnumDevices(unsigned long devClass, void* callback, void* ref, unsigned long flags) { std::strcat(g_seq, "E"); g_class = devClass; g_flags = flags; g_ref = ref; (void)callback; ((CJoystickDX*)ref)->DJoystick = g_pNewDevice; return 0; }

int main()
{
    IDevice dev; CDirectInput di; CInputManagerDX m; m.m_pDirectInput = &di; CJoystickDX j; j.DJoystick = &dev; j.ExclusiveMode = false; j.InputManagerDX = &m; g_pNewDevice = &dev;
    if (!j.SetExclusiveMode(false) || std::strcmp(g_seq, "URE") || !j.ExclusiveMode || g_class != 4 || g_flags != 1 || g_ref != &j || j.DJoystick != &dev) { std::printf("00ab6de0_TEST FAIL\n"); return 1; }
    g_pNewDevice = 0; g_seq[0] = 0; if (j.SetExclusiveMode(true) || std::strcmp(g_seq, "URE") || j.DJoystick != 0) { std::printf("00ab6de0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; if (j.SetExclusiveMode(true) || std::strcmp(g_seq, "E")) { std::printf("00ab6de0_TEST FAIL\n"); return 1; }
    std::printf("00ab6de0_TEST PASS\n");
    return 0;
}
