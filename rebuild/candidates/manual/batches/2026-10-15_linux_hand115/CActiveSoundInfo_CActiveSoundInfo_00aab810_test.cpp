#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct __declspec(novtable) CActiveSoundInfoBase : CBase { void* m_pSound; CActiveSoundInfoBase(void* s) : CBase(), m_pSound(s) { } };
struct CActiveSoundInfo : CActiveSoundInfoBase {
    void* m_pOwner; void* m_pInfo; virtual void v00() { }
    CActiveSoundInfo(void* o, void* s, void* i) : CActiveSoundInfoBase(s), m_pOwner(o), m_pInfo(i) { }
};
int main()
{
    int a, b, c; CActiveSoundInfo v(&a, &b, &c);
    if (g_base != 1 || v.m_pOwner != &a || v.m_pSound != &b || v.m_pInfo != &c) { std::printf("00aab810_TEST FAIL\n"); return 1; }
    std::printf("00aab810_TEST PASS\n"); return 0;
}
