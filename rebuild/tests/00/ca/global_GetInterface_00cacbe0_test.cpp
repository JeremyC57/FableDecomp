// Behaviour test for 00cacbe0: GetInterface -- E_POINTER for a null out-pointer; else the pointer handed out with one AddRef
#include <cstdio>
static int g_ref = 0;
struct IU { virtual long __stdcall QueryInterface(const void*, void**) { return 0; } virtual unsigned long __stdcall AddRef() { return ++g_ref; } virtual unsigned long __stdcall Release() { return 0; } };
extern "C" long __stdcall GetInterface(IU* pUnk, void** ppv) { if (ppv == 0) return (long)0x80004003; *ppv = pUnk; pUnk->AddRef(); return 0; }
int main()
{
    IU u; void* p = 0;
    int ok = GetInterface(&u, 0) == (long)0x80004003 && g_ref == 0;
    ok = ok && GetInterface(&u, &p) == 0 && p == &u && g_ref == 1;
    std::printf(ok ? "00cacbe0_TEST PASS\n" : "00cacbe0_TEST FAIL\n");
    return ok ? 0 : 1;
}
