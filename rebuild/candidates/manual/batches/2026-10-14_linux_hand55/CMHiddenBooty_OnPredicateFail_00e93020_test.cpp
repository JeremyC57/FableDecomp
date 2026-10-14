#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CThing { int x; }; static long g_destroyed = 0; static void DestroyThing(CThing*) { ++g_destroyed; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(CThing*); CThing* m_pObject; };
struct CCountedPointer { CThing* m_pObject; SCountBlock* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(const CCountedPointer& other) { SCountBlock* count = other.m_pCount; CThing* object = other.m_pObject; if (m_pCount != count) { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = object; m_pCount = count; if (count) ++count->m_references; } return *this; } };
struct CScriptThing { virtual void v00() { } virtual void* GetInfo(long which) { g_which = which; return &PImp; } CCountedPointer PImp; static long g_which; }; long CScriptThing::g_which = -1;
static void* g_info = 0; struct CQuest { virtual void OnBootyFailed(void* info) { g_info = info; } };
struct CM_HiddenBooty { void* __vftable; CQuest* PQuest; CScriptThing Thing; void OnPredicateFail() { PQuest->OnBootyFailed(Thing.GetInfo(0)); } };

int main()
{
    CQuest q; CM_HiddenBooty b; b.PQuest = &q; b.OnPredicateFail(); if (CScriptThing::g_which != 0 || g_info != &b.Thing.PImp) { std::printf("00e93020_TEST FAIL\n"); return 1; }
    std::printf("00e93020_TEST PASS\n");
    return 0;
}
