#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CObj { }; static long g_destroyed = 0; static void DestroyObj(CObj*) { ++g_destroyed; } struct SCount { long m_references; void (*m_pfnDestroy)(CObj*); CObj* m_pObject; };
struct CPtr { CObj* m_pObject; SCount* m_pCount; CPtr() : m_pObject(0), m_pCount(0) { } CPtr(const CPtr& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CPtr() { if (m_pCount) { if (--m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
    CPtr& operator=(const CPtr& o) { SCount* block = o.m_pCount; if (m_pCount != block) { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = o.m_pObject; m_pCount = block; if (block) ++block->m_references; } return *this; } };
struct CBank { long m_0; CPtr m_pool; void SetAnimationMemoryPool(CPtr pool) { m_pool = pool; } };

int main() {
    CObj o1, o2; SCount* c1 = new SCount; c1->m_references = 1; c1->m_pfnDestroy = DestroyObj; c1->m_pObject = &o1; SCount* c2 = new SCount; c2->m_references = 1; c2->m_pfnDestroy = DestroyObj; c2->m_pObject = &o2;
    CBank b; b.m_pool.m_pObject = &o1; b.m_pool.m_pCount = c1; { CPtr arg; arg.m_pObject = &o2; arg.m_pCount = c2; ++c2->m_references; b.SetAnimationMemoryPool(arg); } if (b.m_pool.m_pCount != c2 || b.m_pool.m_pObject != &o2 || c2->m_references != 2 || g_destroyed != 1) { std::printf("007046d0_TEST FAIL\n"); return 1; }
    { CPtr arg; arg.m_pObject = &o2; arg.m_pCount = c2; ++c2->m_references; b.SetAnimationMemoryPool(arg); } if (c2->m_references != 2 || g_destroyed != 1) { std::printf("007046d0_TEST FAIL\n"); return 1; } b.m_pool.m_pCount = 0; b.m_pool.m_pObject = 0; c2->m_references = 0; delete c2;
    std::printf("007046d0_TEST PASS\n"); return 0;
}