// behaviour test of EnsureWindowsVersionDetected_00c73770 0x00C73770: the probe runs once while the flag is clear, never once it is set; the
// object is returned unchanged. Self-contained model (the lane links the test alone).
#include <stdio.h>
unsigned char g_defStringInit;
static int g_probes = 0;
void __cdecl DetectWindowsVersion() { ++g_probes; }
void* __fastcall EnsureWindowsVersionDetected_00c73770(void* self)
{
    if (!g_defStringInit)
        DetectWindowsVersion();
    return self;
}
int main()
{
    int a;
    g_defStringInit = 0;
    if (EnsureWindowsVersionDetected_00c73770(&a) != &a || g_probes != 1) { printf("FAIL1\n"); return 1; }
    g_defStringInit = 1;
    if (EnsureWindowsVersionDetected_00c73770(&a) != &a || g_probes != 1) { printf("FAIL2\n"); return 1; }
    printf("OK_00c73770\n");
    return 0;
}
