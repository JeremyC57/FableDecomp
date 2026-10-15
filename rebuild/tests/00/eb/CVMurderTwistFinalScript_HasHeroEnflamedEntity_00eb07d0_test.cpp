#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_ctors = 0, g_dtors = 0, g_termCalls = 0; static unsigned char g_terminating = 0, g_enflamed = 0; static long g_kind = 0; static const char* g_name = 0;
struct CCharString {
    const char* m_pStringData;
    CCharString(const char* text, long length = -1) : m_pStringData(text) { ++g_ctors; if (length != -1) g_ctors = -100; }
    ~CCharString() { ++g_dtors; }
};
struct CScriptThing { virtual void v000() { } virtual bool IsEnflamedBy(long kind, const CCharString& name) { g_kind = kind; g_name = name.m_pStringData; return g_enflamed != 0; } };
struct CV_MurderTwistFinalScript {
    virtual void v00() { }
    unsigned char IsActiveThreadTerminating() { ++g_termCalls; return g_terminating; }
    unsigned char HasHeroEnflamedEntity(CScriptThing* thing)
    {
        bool enflamed = thing->IsEnflamedBy(3, "SCRIPT_NAME_HERO");
        if (enflamed) return !IsActiveThreadTerminating();
        IsActiveThreadTerminating();
        return 0;
    }
};
static bool SameText(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
int main()
{
    CScriptThing thing; CV_MurderTwistFinalScript s;
    g_enflamed = 0; if (s.HasHeroEnflamedEntity(&thing) != 0 || g_termCalls != 1 || g_kind != 3 || !SameText(g_name, "SCRIPT_NAME_HERO") || g_ctors != 1 || g_dtors != 1) { std::printf("00eb07d0_TEST FAIL\n"); return 1; }
    g_enflamed = 1; if (s.HasHeroEnflamedEntity(&thing) != 1 || g_termCalls != 2 || g_dtors != 2) { std::printf("00eb07d0_TEST FAIL\n"); return 1; }
    g_terminating = 1; if (s.HasHeroEnflamedEntity(&thing) != 0 || g_termCalls != 3) { std::printf("00eb07d0_TEST FAIL\n"); return 1; }
    g_enflamed = 0; if (s.HasHeroEnflamedEntity(&thing) != 0 || g_termCalls != 4) { std::printf("00eb07d0_TEST FAIL\n"); return 1; }
    std::printf("00eb07d0_TEST PASS\n"); return 0;
}