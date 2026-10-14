#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
struct CThing { int x; }; static long g_destroyed = 0; static void DestroyThing(CThing*) { ++g_destroyed; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(CThing*); CThing* m_pObject; };
struct CCountedPointer { CThing* m_pObject; SCountBlock* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(const CCountedPointer& other) { SCountBlock* count = other.m_pCount; CThing* object = other.m_pObject; if (m_pCount != count) { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = object; m_pCount = count; if (count) ++count->m_references; } return *this; } };
struct CScriptThing { virtual void v00() { } virtual void* GetInfo(long which) { g_which = which; return &PImp; } CCountedPointer PImp; static long g_which; }; long CScriptThing::g_which = -1;
static CScriptThing* g_thing = 0;
struct CQuest { virtual void RegisterThing(CScriptThing* t, bool f) { note(f ? "R1" : "R0"); g_thing = t; } virtual void SetThingMarker(CScriptThing* t, bool f) { note(f ? "M1" : "M0"); g_thing = t; } };
struct CLostTraderBrother { void* __vftable; CQuest* PQuest; CScriptThing Thing; unsigned char _pad14[8]; long m_state; bool m_flag20; long m_counter; bool m_flag28;
    void Init() { m_state = 1; m_flag20 = false; PQuest->RegisterThing(&Thing, true); PQuest->SetThingMarker(&Thing, false); m_counter = 0; m_flag28 = false; } };

int main()
{
    CQuest q; CLostTraderBrother b; b.PQuest = &q; b.m_state = 7; b.m_flag20 = true; b.m_counter = 9; b.m_flag28 = true; b.Init();
    if (b.m_state != 1 || b.m_flag20 || b.m_counter != 0 || b.m_flag28 || std::strcmp(g_seq, "R1M0") || g_thing != &b.Thing) { std::printf("00ea50a0_TEST FAIL\n"); return 1; }
    std::printf("00ea50a0_TEST PASS\n");
    return 0;
}
