#include <cstdio>

static int g_ref = 0;
struct IU { virtual long __stdcall QueryInterface(const void*, void**) { return 0; } virtual unsigned long __stdcall AddRef() { return ++g_ref; } virtual unsigned long __stdcall Release() { return 0; } };
long __stdcall AcquireInterfaceReference(IU* o, void** out) { if (!out) return (long)0x80004003; *out = o; o->AddRef(); return 0; }
int main()
{
    IU u; void* p = 0;
    if (AcquireInterfaceReference(&u, 0) != (long)0x80004003) { std::printf("00cacbe0_TEST FAIL\n"); return 1; }
    if (AcquireInterfaceReference(&u, &p) != 0 || p != &u || g_ref != 1) { std::printf("00cacbe0_TEST FAIL\n"); return 1; }
    std::printf("00cacbe0_TEST PASS\n");
    return 0;
}