#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CObj { }; static long g_destroyed = 0; static void DestroyObj(CObj*) { ++g_destroyed; } struct SCount { long m_references; void (*m_pfnDestroy)(CObj*); CObj* m_pObject; };
struct CCountedPointer { CObj* m_pObject; SCount* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { } ~CCountedPointer() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = 0; m_pCount = 0; } };
class CCharString { public: void* m_p; CCharString() : m_p(0) { } const CCharString& operator=(const CCharString& o) { m_p = o.m_p; return *this; } };
struct CSample { CCharString m_name; long m_4; CCountedPointer m_data; CSample() : m_4(0) { }
    CSample& operator=(const CSample& o) { m_name = o.m_name; m_4 = o.m_4; SCount* block = o.m_data.m_pCount; CObj* obj = o.m_data.m_pObject; if (m_data.m_pCount != block) { if (m_data.m_pCount) { --m_data.m_pCount->m_references; if (m_data.m_pCount->m_references == 0) { m_data.m_pCount->m_pfnDestroy(m_data.m_pCount->m_pObject); delete m_data.m_pCount; } } m_data.m_pObject = obj; m_data.m_pCount = block; if (block) ++block->m_references; } return *this; } };

int main() {
    CObj o1, o2; SCount* c1 = new SCount; c1->m_references = 1; c1->m_pfnDestroy = DestroyObj; c1->m_pObject = &o1; SCount* c2 = new SCount; c2->m_references = 1; c2->m_pfnDestroy = DestroyObj; c2->m_pObject = &o2;
    CSample a, b; a.m_4 = 1; a.m_data.m_pObject = &o1; a.m_data.m_pCount = c1; b.m_4 = 2; b.m_data.m_pObject = &o2; b.m_data.m_pCount = c2; b.m_name.m_p = (void*)9;
    CSample& r = (a = b); if (&r != &a || a.m_4 != 2 || a.m_name.m_p != (void*)9 || a.m_data.m_pObject != &o2 || a.m_data.m_pCount != c2 || c2->m_references != 2 || g_destroyed != 1) { std::printf("00a49ce0_TEST FAIL\n"); return 1; }
    a = b; if (c2->m_references != 2 || g_destroyed != 1) { std::printf("00a49ce0_TEST FAIL\n"); return 1; } CSample e; e = a; if (e.m_data.m_pCount != c2 || c2->m_references != 3) { std::printf("00a49ce0_TEST FAIL\n"); return 1; } a.m_data.m_pObject = 0; a.m_data.m_pCount = 0; b.m_data.m_pCount = 0; e.m_data.m_pCount = 0; delete c2;
    std::printf("00a49ce0_TEST PASS\n"); return 0;
}