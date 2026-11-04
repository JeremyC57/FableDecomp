#include <cstdio>
#include <cstdlib>
#include <cstring>
class CThing { public: int x; }; static long g_destroyed = 0; static void DestroyThing(CThing*) { ++g_destroyed; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(CThing*); CThing* m_pObject; };
struct CCountedPointer { CThing* m_pObject; SCountBlock* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(const CCountedPointer& other) { SCountBlock* count = other.m_pCount; CThing* object = other.m_pObject; if (m_pCount != count) { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = object; m_pCount = count; if (count) ++count->m_references; } return *this; } };
class CScriptThing { public: virtual void v00() { } virtual void* GetInfo(long which) { g_which = which; return &PImp; } CCountedPointer PImp; static long g_which; }; long CScriptThing::g_which = -1;
static CScriptThing* g_thing = 0; static bool g_flag = false;
struct CQuest { virtual void SetThingState(CScriptThing* t, bool f) { g_thing = t; g_flag = f; } };
struct CMansLover { void* __vftable; CQuest* PQuest; CScriptThing Thing; unsigned char _pad14[8]; bool m_flag1c; void Init() { PQuest->SetThingState(&Thing, true); m_flag1c = false; } };

int main()
{
    CQuest q; CMansLover l; l.PQuest = &q; l.m_flag1c = true; l.Init(); if (l.m_flag1c || !g_flag || g_thing != &l.Thing) { std::printf("00ec9cb0_TEST FAIL\n"); return 1; }
    std::printf("00ec9cb0_TEST PASS\n");
    return 0;
}