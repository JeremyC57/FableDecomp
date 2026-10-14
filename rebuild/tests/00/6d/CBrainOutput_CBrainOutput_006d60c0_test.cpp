#include <stdio.h>
static int g_destroyed, g_ipDtors, g_ipCtors, g_blocksFreed;
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtors; } ~CBaseIntelligentPointer() { ++g_ipDtors; } };
struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CCreatureActionBase { long m_tag; };
static void __fastcall DestroyAction(CCreatureActionBase* p) { ++g_destroyed; p->m_tag = -1; }
struct SCountBlock {
    long m_references; void (__fastcall* m_pfnDestroy)(CCreatureActionBase*); CCreatureActionBase* m_pObject;
    SCountBlock(CCreatureActionBase* o, long refs) : m_references(refs), m_pfnDestroy(DestroyAction), m_pObject(o) { }
    void operator delete(void* p) { ++g_blocksFreed; ::operator delete(p); }
};
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject; SCountBlock* m_pCount;
    __forceinline ~CCountedPointer_CCreatureActionBase()
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
        m_pObject = 0; m_pCount = 0;
    }
};
struct C3DVector { float X, Y, Z; };
struct CNavigationInput { unsigned char _body[0x18]; };

struct CBrainOutput {
    unsigned long Types; long LookFlags; CNavigationInput NavigationInput; CCountedPointer_CCreatureActionBase PAction; C3DVector MoveToPos; C3DVector LookAtPos; CIntelligentPointer LookAtThing;
    ~CBrainOutput();
};
CBrainOutput::~CBrainOutput() { }
int main()
{
    CCreatureActionBase act; act.m_tag = 1;
    bool ok = sizeof(CBrainOutput) == 72;
    {   // last reference: the action is destroyed and the block freed
        CBrainOutput* o = new CBrainOutput; o->PAction.m_pObject = &act; o->PAction.m_pCount = new SCountBlock(&act, 1);
        o->~CBrainOutput(); ::operator delete(o);
        ok = ok && g_destroyed == 1 && act.m_tag == -1 && g_blocksFreed == 1 && g_ipDtors == 1;
    }
    {   // shared: only the count drops
        SCountBlock* blk = new SCountBlock(&act, 2);
        CBrainOutput* o = new CBrainOutput; o->PAction.m_pObject = &act; o->PAction.m_pCount = blk;
        o->~CBrainOutput(); ::operator delete(o);
        ok = ok && g_destroyed == 1 && blk->m_references == 1 && g_blocksFreed == 1 && g_ipDtors == 2;
        delete blk;
    }
    {   // null: nothing happens
        CBrainOutput* o = new CBrainOutput; o->PAction.m_pObject = 0; o->PAction.m_pCount = 0;
        o->~CBrainOutput(); ::operator delete(o);
        ok = ok && g_destroyed == 1 && g_blocksFreed == 2 && g_ipDtors == 3;
    }
    if (!ok) { printf("006d60c0_TEST FAIL\n"); return 1; }
    printf("006d60c0_TEST PASS\n"); return 0;
}