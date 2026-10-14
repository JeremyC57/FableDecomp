#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_destroyed = 0; static void* g_deleted = 0; static void* g_freed = 0;
static void DestroyThing(void* o) { g_destroyed = o; std::strcat(g_seq, "D"); } static void DeleteBlock(void* p) { g_deleted = p; std::strcat(g_seq, "d"); } static void FreeBuffer(void* p) { g_freed = p; std::strcat(g_seq, "f"); }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CBase { virtual ~CBase() { std::strcat(g_seq, "B"); } };
struct CCountedPointer { void* m_pThing; SCountBlock* m_pCount;
    ~CCountedPointer() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); DeleteBlock(m_pCount); } } m_pThing = 0; m_pCount = 0; } };
struct CScriptThing : CBase { CCountedPointer m_thing; virtual ~CScriptThing() { } };
struct CScriptThingListBase { void* m_pBuffer; void Clear() { std::strcat(g_seq, "C"); } ~CScriptThingListBase() { Clear(); if (m_pBuffer) FreeBuffer(m_pBuffer); } };
struct CGameScriptThingManager : CScriptThingListBase { CScriptThing m_nullThing; ~CGameScriptThingManager() { } };

int main() {
    SCountBlock c; int thing; c.m_references = 1; c.m_pfnDestroy = DestroyThing; c.m_pObject = &thing; int buffer;
    { CGameScriptThingManager m; m.m_pBuffer = &buffer; m.m_nullThing.m_thing.m_pThing = &thing; m.m_nullThing.m_thing.m_pCount = &c; }
    if (std::strcmp(g_seq, "DdBCf") || g_destroyed != &thing || g_deleted != &c || g_freed != &buffer) { std::printf("004bb600_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; c.m_references = 2; { CGameScriptThingManager m; m.m_pBuffer = 0; m.m_nullThing.m_thing.m_pThing = &thing; m.m_nullThing.m_thing.m_pCount = &c; }
    if (std::strcmp(g_seq, "BC") || c.m_references != 1) { std::printf("004bb600_TEST FAIL\n"); return 1; }
    std::printf("004bb600_TEST PASS\n"); return 0;
}
