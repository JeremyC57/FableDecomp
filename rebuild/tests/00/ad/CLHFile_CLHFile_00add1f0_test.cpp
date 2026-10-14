#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CObj { }; static long g_destroyed = 0; static void DestroyObj(CObj*) { ++g_destroyed; } struct SCount { long m_references; void (*m_pfnDestroy)(CObj*); CObj* m_pObject; };
struct CCountedPointer { CObj* m_pObject; SCount* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { } ~CCountedPointer() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = 0; m_pCount = 0; } };
struct CBaseEv { virtual void v00() { } ~CBaseEv() { std::strcat(g_seq, "B"); } }; struct CHolder : CBaseEv { CCountedPointer m_pointer; ~CHolder() { } };

int main() {
    CObj o; SCount* c = new SCount; c->m_references = 2; c->m_pfnDestroy = DestroyObj; c->m_pObject = &o; { CHolder h; h.m_pointer.m_pObject = &o; h.m_pointer.m_pCount = c; } if (g_destroyed != 0 || c->m_references != 1 || std::strcmp(g_seq, "B")) { std::printf("00add1f0_TEST FAIL\n"); return 1; }
    { CHolder h; h.m_pointer.m_pObject = &o; h.m_pointer.m_pCount = c; } if (g_destroyed != 1 || std::strcmp(g_seq, "BB")) { std::printf("00add1f0_TEST FAIL\n"); return 1; } { CHolder h; } if (g_destroyed != 1 || std::strcmp(g_seq, "BBB")) { std::printf("00add1f0_TEST FAIL\n"); return 1; }
    std::printf("00add1f0_TEST PASS\n"); return 0;
}