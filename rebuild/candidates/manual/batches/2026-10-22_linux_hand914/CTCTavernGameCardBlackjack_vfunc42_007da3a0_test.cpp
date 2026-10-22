// Behaviour test for 007da3a0 CTCTavernGameCardBlackjack::vfunc_42 (fetch and release the game def, return an empty wide string)
#include <cstdio>
#include <cstring>
static int g_destroyed = 0;
static int g_lookups = 0;
struct CWideString { int length; };
struct CCharString {
    const char* text;
    CCharString(const char* t, long length) : text(t) { (void)length; }
    ~CCharString() {}
    CWideString ToWideString() const { CWideString w; w.length = (int)std::strlen(text); return w; }
};
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
struct CDefVirtual { virtual void v00() {} virtual void Destroy() { ++g_destroyed; } virtual ~CDefVirtual() {} };
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer {
    CDefBase* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
};
static CDefBase g_def;
struct CTCTavernGameCardBlackjack {
    bool GetGameDef(CDefPointer& out) { ++g_lookups; out.m_p = &g_def; ++g_def.m_count; return true; }
    CWideString vfunc_42();
};
CWideString CTCTavernGameCardBlackjack::vfunc_42()
{
    CDefPointer def;
    GetGameDef(def);
    return CCharString("", -1).ToWideString();
}
int main()
{
    g_def.m_count = 1;
    CTCTavernGameCardBlackjack card;
    CWideString w = card.vfunc_42();
    if (w.length != 0 || g_lookups != 1 || g_def.m_count != 1 || g_destroyed != 0) { std::printf("007da3a0_TEST FAIL\n"); return 1; }
    g_def.m_count = 0;   // the lookup's reference was the last one: releasing destroys it
    card.vfunc_42();
    if (g_destroyed != 1) { std::printf("007da3a0_TEST FAIL\n"); return 1; }
    std::printf("007da3a0_TEST PASS\n");
    return 0;
}
