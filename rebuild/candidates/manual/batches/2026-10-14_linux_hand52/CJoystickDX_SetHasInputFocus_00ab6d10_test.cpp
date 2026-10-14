#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = ""; struct IDevice { virtual unsigned long Release() { std::strcat(g_seq, "R"); return 0; } virtual long Acquire() { std::strcat(g_seq, "A"); return 0; } virtual long Unacquire() { std::strcat(g_seq, "U"); return 0; } };
struct CJoystickDX { IDevice* DJoystick; bool Acquired;
    void SetHasInputFocus(bool focus) { if (DJoystick) { if (!focus) { if (Acquired == true) { DJoystick->Unacquire(); Acquired = false; } } else { if (!Acquired) { DJoystick->Acquire(); Acquired = true; } } } } };

int main()
{
    IDevice dev; CJoystickDX j; j.DJoystick = &dev; j.Acquired = false;
    j.SetHasInputFocus(true); if (std::strcmp(g_seq, "A") || !j.Acquired) { std::printf("00ab6d10_TEST FAIL\n"); return 1; }
    j.SetHasInputFocus(true); if (std::strcmp(g_seq, "A") || !j.Acquired) { std::printf("00ab6d10_TEST FAIL\n"); return 1; }
    j.SetHasInputFocus(false); if (std::strcmp(g_seq, "AU") || j.Acquired) { std::printf("00ab6d10_TEST FAIL\n"); return 1; }
    j.SetHasInputFocus(false); if (std::strcmp(g_seq, "AU") || j.Acquired) { std::printf("00ab6d10_TEST FAIL\n"); return 1; }
    j.DJoystick = 0; j.SetHasInputFocus(true); if (std::strcmp(g_seq, "AU") || j.Acquired) { std::printf("00ab6d10_TEST FAIL\n"); return 1; }
    std::printf("00ab6d10_TEST PASS\n");
    return 0;
}
