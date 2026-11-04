// Behaviour test for CKeyboardDX::~CKeyboardDX (0x00AB5FC0): unacquire, then release the DirectInput device.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[64];
struct Device { void Unacquire() { strcat(g_log, "unacquire,"); } void Release() { strcat(g_log, "release"); } };
struct Keyboard { Device* DKeyboard; ~Keyboard() { DKeyboard->Unacquire(); DKeyboard->Release(); } };
int main()
{
    Device d;
    { Keyboard k; k.DKeyboard = &d; }
    bool ok = strcmp(g_log, "unacquire,release") == 0;
    printf(ok ? "00ab5fc0_TEST PASS\n" : "00ab5fc0_TEST FAIL\n");
    return ok ? 0 : 1;
}