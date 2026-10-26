#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
class CCreatureActionBase { public: int x; }; static long g_destroyed = 0; static void* g_output = 0;
struct SCountBlock { long m_references; void (*m_pfnDestroy)(CCreatureActionBase*); CCreatureActionBase* m_pObject; };
static void DestroyAction(CCreatureActionBase*) { ++g_destroyed; note("D"); }
struct CCountedPointer { CCreatureActionBase* m_pObject; SCountBlock* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { }
    ~CCountedPointer() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = 0; m_pCount = 0; } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } ~CBaseIntelligentPointer() { note("I"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { };
class C3DVector { public: float X, Y, Z; }; class CNavigationInput { public: long m_00, m_04, m_08, m_0c, m_10, m_14; };
class CBrainOutput { public: unsigned long Types; long LookFlags; CNavigationInput NavigationInput; CCountedPointer PAction; C3DVector MoveToPos; C3DVector LookAtPos; CIntelligentPointer LookAtThing;
    CBrainOutput() : Types(0), LookFlags(0) { note("C"); } };
static CCreatureActionBase g_action;
class CAIBrain { public: bool ForceReset(CBrainOutput* output) { note("R"); g_output = output; SCountBlock* b = new SCountBlock; b->m_references = 1; b->m_pfnDestroy = DestroyAction; b->m_pObject = &g_action; output->PAction.m_pObject = &g_action; output->PAction.m_pCount = b; return true; }
    void ForceResetWithOutput() { CBrainOutput output; ForceReset(&output); } };

int main()
{
    CAIBrain brain; brain.ForceResetWithOutput(); if (std::strcmp(g_seq, "CRID") || g_destroyed != 1 || g_output == 0) { std::printf("0088bb60_TEST FAIL\n"); return 1; }
    std::printf("0088bb60_TEST PASS\n");
    return 0;
}