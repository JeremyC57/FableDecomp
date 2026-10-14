#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DAnimationStats;
static long g_destroyed = 0; static void* g_names = 0; static long g_event = -1; static bool g_has = false;
struct SCountBlock { long m_references; void (*m_pfnDestroy)(const C3DAnimationStats*); const C3DAnimationStats* m_pObject; };
static void Destroy(const C3DAnimationStats*) { ++g_destroyed; }
struct CCountedPointer {
    const C3DAnimationStats* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(const C3DAnimationStats* o, SCountBlock* c) : m_pObject(o), m_pCount(c) { if (c) ++c->m_references; }
    CCountedPointer(const CCountedPointer& p) : m_pObject(p.m_pObject), m_pCount(p.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = 0; m_pCount = 0; }
    const C3DAnimationStats* operator->() const { return m_pObject; }
};
struct C3DAnimationStats { long m_id; void GetEmbeddedEventNames(void* names) const { g_names = names; } bool HasEmbeddedEvent(long event) const { g_event = event; return g_has; } };
struct C3DAnimationInfo { const C3DAnimationStats* m_pStats; SCountBlock* m_pCount; CCountedPointer GetAnimationStats() { return CCountedPointer(m_pStats, m_pCount); } };
struct CAnimationBank { C3DAnimationInfo* m_pAnimations[4]; C3DAnimationInfo* GetAnimation(long i) { return m_pAnimations[i]; } };
struct CQuery { void* __vftable; CAnimationBank* m_pBank; void GetEmbeddedEventNames(long animation, void* names) { CCountedPointer stats = m_pBank->GetAnimation(animation)->GetAnimationStats(); stats->GetEmbeddedEventNames(names); } };

int main()
{
    C3DAnimationStats stats; stats.m_id = 7; SCountBlock* block = new SCountBlock; block->m_references = 1; block->m_pfnDestroy = Destroy; block->m_pObject = &stats;
    C3DAnimationInfo info; info.m_pStats = &stats; info.m_pCount = block; CAnimationBank bank; bank.m_pAnimations[2] = &info; CQuery q; q.m_pBank = &bank; int out;
    q.GetEmbeddedEventNames(2, &out); if (g_names != &out || block->m_references != 1 || g_destroyed != 0) { std::printf("006f91e0_TEST FAIL\n"); return 1; }
    block->m_references = 0; q.GetEmbeddedEventNames(2, 0); if (g_names != 0 || g_destroyed != 1) { std::printf("006f91e0_TEST FAIL\n"); return 1; }
    std::printf("006f91e0_TEST PASS\n");
    return 0;
}
