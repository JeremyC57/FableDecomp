#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <new>
namespace NUISystem {
static long g_owner = 0; static int g_baseCtor = 0, g_cbase = 0, g_init = 0;
struct IComponentA { virtual void va00() { } unsigned char _state[0x10]; };
struct IComponentB { virtual void vb00() { } };
struct CComponentRoot { virtual void v00() { } };
struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB { unsigned char _body[0x15c - 0x1c]; CChangingStateComponent(long owner) { g_owner = owner; ++g_baseCtor; } };
struct CBase { virtual void v00() { } CBase() { ++g_cbase; } };
struct CEnginePrimitiveHandle : CBase { void* m_pPrimitive; virtual void v00() { } CEnginePrimitiveHandle() : CBase(), m_pPrimitive(0) { } };
struct CComponentVector { void* m_pFirst; void* m_pLast; void* m_pEnd; CComponentVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct CViewport : CChangingStateComponent {
    virtual void v00() { } virtual void va00() { } virtual void vb00() { }
    CEnginePrimitiveHandle m_primitive; CComponentVector m_fakeChildren; void* m_pView;
    void InitialiseFromDef() { ++g_init; }
    CViewport(long owner) : CChangingStateComponent(owner), m_primitive(), m_fakeChildren(), m_pView(0) { InitialiseFromDef(); }
};
}
int main()
{
    unsigned char raw[sizeof(NUISystem::CViewport)]; for (unsigned k = 0; k < sizeof(raw); ++k) raw[k] = 0xAA;
    NUISystem::CViewport* v = new (raw) NUISystem::CViewport(77);
    if (NUISystem::g_owner != 77 || NUISystem::g_baseCtor != 1 || NUISystem::g_cbase != 1 || NUISystem::g_init != 1 || v->m_primitive.m_pPrimitive != 0 || v->m_fakeChildren.m_pFirst != 0 || v->m_fakeChildren.m_pLast != 0 || v->m_fakeChildren.m_pEnd != 0 || v->m_pView != 0) { std::printf("0054ea00_TEST FAIL\n"); return 1; }
    std::printf("0054ea00_TEST PASS\n"); return 0;
}
