#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CObj { }; static long g_destroyed = 0; static void DestroyObj(CObj*) { ++g_destroyed; } struct SCount { long m_references; void (*m_pfnDestroy)(CObj*); CObj* m_pObject; };
struct CPtr; static const CPtr* g_assigned = 0; struct CPtr { CObj* m_pObject; SCount* m_pCount; CPtr& operator=(const CPtr& o) { g_assigned = &o; return *this; }
    void Release() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pCount = 0; m_pObject = 0; } };
struct CMapInfo { unsigned char _pad[0xd4]; CPtr m_owner; CPtr m_file; unsigned char _p[4]; bool m_e8, m_e9; void Clear() { if (m_file.m_pObject) { m_owner = m_file; m_file.Release(); } m_e8 = false; m_e9 = false; } };

int main() {
    CObj o; SCount* c = new SCount; c->m_references = 1; c->m_pfnDestroy = DestroyObj; c->m_pObject = &o; CMapInfo m; std::memset(&m, 0, sizeof m); m.m_file.m_pObject = &o; m.m_file.m_pCount = c; m.m_e8 = true; m.m_e9 = true;
    m.Clear(); if (g_assigned != &m.m_file || m.m_file.m_pObject || m.m_file.m_pCount || g_destroyed != 1 || m.m_e8 || m.m_e9) { std::printf("00697c60_TEST FAIL\n"); return 1; } g_assigned = 0; m.m_e8 = true; m.Clear(); if (g_assigned || m.m_e8) { std::printf("00697c60_TEST FAIL\n"); return 1; }
    std::printf("00697c60_TEST PASS\n"); return 0;
}