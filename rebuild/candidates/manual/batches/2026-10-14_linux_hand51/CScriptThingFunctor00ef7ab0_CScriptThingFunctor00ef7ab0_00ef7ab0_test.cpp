#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_destroyed = 0; static void* g_deleted = 0; static void DestroyThing(void* o) { g_destroyed = o; std::strcat(g_seq, "D"); } static void DeleteBlock(void* p) { g_deleted = p; std::strcat(g_seq, "d"); }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; }; struct CBase { virtual ~CBase() { std::strcat(g_seq, "B"); } };
struct CCountedPointer { void* m_pThing; SCountBlock* m_pCount; ~CCountedPointer() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); DeleteBlock(m_pCount); } } m_pThing = 0; m_pCount = 0; } };
struct CScriptThing : CBase { CCountedPointer m_thing; virtual ~CScriptThing() { } }; struct CFunctorBase { virtual void v00() { } ~CFunctorBase() { std::strcat(g_seq, "F"); } };
struct CFunctor : CFunctorBase { CScriptThing m_thing; virtual void v00() { } ~CFunctor() { } };

int main() {
    SCountBlock c; int thing; c.m_references = 1; c.m_pfnDestroy = DestroyThing; c.m_pObject = &thing; { CFunctor f; f.m_thing.m_thing.m_pThing = &thing; f.m_thing.m_thing.m_pCount = &c; } if (std::strcmp(g_seq, "DdBF") || g_destroyed != &thing || g_deleted != &c) { std::printf("00ef7ab0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; c.m_references = 2; { CFunctor f; f.m_thing.m_thing.m_pThing = &thing; f.m_thing.m_thing.m_pCount = &c; } if (std::strcmp(g_seq, "BF") || c.m_references != 1) { std::printf("00ef7ab0_TEST FAIL\n"); return 1; }
    std::printf("00ef7ab0_TEST PASS\n"); return 0;
}
