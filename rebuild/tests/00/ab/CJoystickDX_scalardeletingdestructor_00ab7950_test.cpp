#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = ""; struct IDevice { virtual unsigned long Release() { std::strcat(g_seq, "R"); return 0; } virtual long Acquire() { std::strcat(g_seq, "A"); return 0; } virtual long Unacquire() { std::strcat(g_seq, "U"); return 0; } };
struct CJoystick { virtual void EventScanStart() { } unsigned char _body[0xd14]; virtual ~CJoystick() { std::strcat(g_seq, "J"); } };
struct CJoystickDX : CJoystick { unsigned char _pad[0x20]; IDevice* DJoystick; bool Acquired; unsigned char _pad2[3]; void* InputManagerDX;
    virtual ~CJoystickDX() { if (DJoystick) { DJoystick->Unacquire(); DJoystick->Release(); DJoystick = 0; } } };

int main()
{
    IDevice dev; CJoystickDX* j = new CJoystickDX; j->DJoystick = &dev; CJoystick* base = j; delete base;
    if (std::strcmp(g_seq, "URJ")) { std::printf("00ab7950_TEST FAIL\n"); return 1; } g_seq[0] = 0; j = new CJoystickDX; j->DJoystick = 0; base = j; delete base; if (std::strcmp(g_seq, "J")) { std::printf("00ab7950_TEST FAIL\n"); return 1; }
    std::printf("00ab7950_TEST PASS\n");
    return 0;
}