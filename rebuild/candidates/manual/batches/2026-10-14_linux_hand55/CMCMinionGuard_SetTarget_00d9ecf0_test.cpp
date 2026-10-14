#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CThing { int x; }; static long g_destroyed = 0; static void DestroyThing(CThing*) { ++g_destroyed; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(CThing*); CThing* m_pObject; };
struct CCountedPointer { CThing* m_pObject; SCountBlock* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(const CCountedPointer& other) { SCountBlock* count = other.m_pCount; CThing* object = other.m_pObject; if (m_pCount != count) { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = object; m_pCount = count; if (count) ++count->m_references; } return *this; } };
struct CScriptThing { virtual void v00() { } virtual void* GetInfo(long which) { g_which = which; return &PImp; } CCountedPointer PImp; static long g_which; }; long CScriptThing::g_which = -1;
static CScriptThing* g_thing = 0; static const CScriptThing* g_target = 0;
struct CQuest { virtual void SetThingTarget(CScriptThing* thing, const CScriptThing* target) { g_thing = thing; g_target = target; } };
struct CMC_MinionGuard { void* __vftable; CQuest* PQuest; CScriptThing Thing; unsigned char _pad14[8]; CScriptThing Target; void SetTarget(const CScriptThing& target) { PQuest->SetThingTarget(&Thing, &target); Target = target; } };

int main()
{
    CQuest q; CMC_MinionGuard g; g.PQuest = &q; CScriptThing target; CThing thing; SCountBlock* block = new SCountBlock; block->m_references = 1; block->m_pfnDestroy = DestroyThing; block->m_pObject = &thing; target.PImp.m_pObject = &thing; target.PImp.m_pCount = block;
    g.SetTarget(target); if (g_thing != &g.Thing || g_target != &target || g.Target.PImp.m_pObject != &thing || g.Target.PImp.m_pCount != block || block->m_references != 2) { std::printf("00d9ecf0_TEST FAIL\n"); return 1; }
    g.SetTarget(target); if (block->m_references != 2) { std::printf("00d9ecf0_TEST FAIL\n"); return 1; } CScriptThing none; g.SetTarget(none); if (block->m_references != 1 || g.Target.PImp.m_pCount != 0 || g_destroyed != 0) { std::printf("00d9ecf0_TEST FAIL\n"); return 1; }
    block->m_references = 0; g.Target.PImp.m_pObject = &thing; g.Target.PImp.m_pCount = block; ++block->m_references; g.SetTarget(none); if (g_destroyed != 1) { std::printf("00d9ecf0_TEST FAIL\n"); return 1; }
    std::printf("00d9ecf0_TEST PASS\n");
    return 0;
}
