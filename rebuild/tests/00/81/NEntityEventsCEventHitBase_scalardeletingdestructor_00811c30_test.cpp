#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_destroyed = 0; static void* g_destroyedObj = 0; static int g_freed = 0; static void* g_freedBlock[4];
struct SCountBlock_00811c30 { long m_references; void (*m_pfnDestroy)(void* object); void* m_pObject; };
static void DestroyObj(void* o) { ++g_destroyed; g_destroyedObj = o; }
struct CCountedPointer_00811c30 { void* m_pObject; SCountBlock_00811c30* m_pCount;
    ~CCountedPointer_00811c30() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); g_freedBlock[g_freed++] = m_pCount; } } m_pObject = 0; m_pCount = 0; } };
namespace NEntityEvents { struct CEventBase { virtual ~CEventBase() { } long m_eventType; }; struct CEventHitBase : CEventBase { CCountedPointer_00811c30 m_hitInfo; virtual ~CEventHitBase() { } }; }
int main() {
    int obj; SCountBlock_00811c30 blk; blk.m_references = 2; blk.m_pfnDestroy = DestroyObj; blk.m_pObject = &obj;
    NEntityEvents::CEventHitBase* e = new NEntityEvents::CEventHitBase; e->m_hitInfo.m_pObject = &obj; e->m_hitInfo.m_pCount = &blk; delete e; if (blk.m_references != 1 || g_destroyed != 0 || g_freed != 0) { std::printf("00811c30_TEST FAIL\n"); return 1; }
    e = new NEntityEvents::CEventHitBase; e->m_hitInfo.m_pObject = &obj; e->m_hitInfo.m_pCount = &blk; delete e; if (blk.m_references != 0 || g_destroyed != 1 || g_destroyedObj != &obj || g_freed != 1 || g_freedBlock[0] != &blk) { std::printf("00811c30_TEST FAIL\n"); return 1; }
    std::printf("00811c30_TEST PASS\n"); return 0;
}