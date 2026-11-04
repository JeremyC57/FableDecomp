// Behaviour test for CJoystickDX::~CJoystickDX 0x00AB6A90: a live DirectInput device is unacquired, then released, exactly once each,
// before the CJoystick base destructor runs; a joystick without a device destroys cleanly.
#include <cstdio>
static int g_order[8]; static int g_n = 0;
struct IDevice { int unacq, rel; void Unacquire() { ++unacq; g_order[g_n++] = 1; } void Release() { ++rel; g_order[g_n++] = 2; } };
struct CJoystick { virtual ~CJoystick() { g_order[g_n++] = 3; } };
struct CJoystickDX : CJoystick {
    IDevice* DJoystick;
    ~CJoystickDX() { if (DJoystick) { DJoystick->Unacquire(); DJoystick->Release(); DJoystick = 0; } }
};
int main()
{
    IDevice dev = { 0, 0 };
    { CJoystickDX j; j.DJoystick = &dev; }
    if (dev.unacq != 1 || dev.rel != 1 || g_n != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3) { std::printf("00ab6a90_TEST FAIL\n"); return 1; }
    g_n = 0;
    { CJoystickDX j; j.DJoystick = 0; }
    if (g_n != 1 || g_order[0] != 3) { std::printf("00ab6a90_TEST FAIL\n"); return 1; }
    std::printf("00ab6a90_TEST PASS\n");
    return 0;
}