#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_destroyed = 0; static void* g_destroyedObj = 0; static void Destroy(void* o) { ++g_destroyed; g_destroyedObj = o; }
struct SCountBlock_00be03a0 { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CCountedPointer_00be03a0 { void* m_pObject; SCountBlock_00be03a0* m_pCount; CCountedPointer_00be03a0() : m_pObject(0), m_pCount(0) { } CCountedPointer_00be03a0(const CCountedPointer_00be03a0& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    void Release() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
    CCountedPointer_00be03a0& operator=(const CCountedPointer_00be03a0& o) { if (m_pCount != o.m_pCount) { Release(); m_pObject = o.m_pObject; m_pCount = o.m_pCount; if (m_pCount) ++m_pCount->m_references; } return *this; }
    ~CCountedPointer_00be03a0() { Release(); m_pObject = 0; m_pCount = 0; } };
struct CEngineStaticMapFileBlockManager { CCountedPointer_00be03a0 m_bankFileEntry; void SetBankFileEntry(CCountedPointer_00be03a0 e) { m_bankFileEntry = e; } };
int main() {
    SCountBlock_00be03a0* oldb = new SCountBlock_00be03a0; oldb->m_references = 1; oldb->m_pfnDestroy = &Destroy; oldb->m_pObject = (void*)1; SCountBlock_00be03a0* newb = new SCountBlock_00be03a0; newb->m_references = 1; newb->m_pfnDestroy = &Destroy; newb->m_pObject = (void*)2;
    CEngineStaticMapFileBlockManager m; m.m_bankFileEntry.m_pObject = (void*)1; m.m_bankFileEntry.m_pCount = oldb; { CCountedPointer_00be03a0 e; e.m_pObject = (void*)2; e.m_pCount = newb; ++newb->m_references; m.SetBankFileEntry(e); }
    if (g_destroyed != 1 || g_destroyedObj != (void*)1 || m.m_bankFileEntry.m_pCount != newb || m.m_bankFileEntry.m_pObject != (void*)2 || newb->m_references != 2) { std::printf("00be03a0_TEST FAIL\n"); return 1; }
    { CCountedPointer_00be03a0 e; e.m_pObject = (void*)2; e.m_pCount = newb; ++newb->m_references; m.SetBankFileEntry(e); } if (g_destroyed != 1 || newb->m_references != 2) { std::printf("00be03a0_TEST FAIL\n"); return 1; }
    m.m_bankFileEntry.Release(); m.m_bankFileEntry.Release(); if (g_destroyed != 2 || g_destroyedObj != (void*)2) { std::printf("00be03a0_TEST FAIL\n"); return 1; }
    std::printf("00be03a0_TEST PASS\n"); return 0;
}