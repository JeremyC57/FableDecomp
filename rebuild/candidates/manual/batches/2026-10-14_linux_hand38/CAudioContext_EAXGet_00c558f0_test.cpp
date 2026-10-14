#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif
static int g_resolves = 0; static const char* g_resolved = 0; static int g_calls = 0; static const void* g_a[5];
static long __cdecl FakeEAX(const void* guid, unsigned long prop, unsigned long src, void* value, unsigned long size) { ++g_calls; g_a[0] = guid; g_a[1] = (const void*)prop; g_a[2] = (const void*)src; g_a[3] = value; g_a[4] = (const void*)size; return 77; }
static void __cdecl FakeClose(void* device) { ++g_calls; g_a[0] = device; }
typedef long (__cdecl* PFN_EAX)(const void*, unsigned long, unsigned long, void*, unsigned long);
typedef void (__cdecl* PFN_alcCloseDevice)(void*);
struct CAudioContext { void* __vftable; void* Module; unsigned char _pad0[0xc]; PFN_alcCloseDevice pfnCloseDevice; unsigned char _pad1[0xd0]; void* pfnGetProcAddress; unsigned char _pad2[4]; PFN_EAX pfnEAXSet; PFN_EAX pfnEAXGet; unsigned char _pad3[4]; void* Device; void* Context; bool UseALGetProcAddress; unsigned char _pad4[0xa7]; long m_1ac; int resets;
    void* GetProcAddress(const char* name) { ++g_resolves; g_resolved = name; return (void*)&FakeEAX; }
    void Reset(bool keepDevice) { ++resets; }
    long EAXGet(const void* g, unsigned long p, unsigned long s, void* value, unsigned long size) { if (!pfnEAXGet) pfnEAXGet = (PFN_EAX)GetProcAddress("EAXGet"); return pfnEAXGet(g, p, s, value, size); }
};
int main()
{
    CAudioContext* c = (CAudioContext*)std::malloc(sizeof(CAudioContext)); std::memset(c, 0, sizeof(*c)); int v = 0;
    long r = c->EAXGet((const void*)1, 2, 3, &v, 4); bool ok = r == 77 && g_resolves == 1 && g_calls == 1 && std::strcmp(g_resolved, "EAXGet") == 0 && g_a[0] == (const void*)1 && g_a[1] == (const void*)2 && g_a[2] == (const void*)3 && g_a[3] == &v && g_a[4] == (const void*)4;
    r = c->EAXGet((const void*)1, 2, 3, &v, 4); ok = ok && r == 77 && g_resolves == 1 && g_calls == 2;
    std::free(c);
    if (!ok) { std::printf("00c558f0_TEST FAIL\n"); return 1; }
    std::printf("00c558f0_TEST PASS\n"); return 0;
}
