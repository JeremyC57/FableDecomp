#include <stdio.h>
#include <string.h>
#include <new>
static int g_ipCtors, g_destroyed;
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtors; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CCreatureActionBase { long m_tag; };
static void __fastcall DestroyAction(CCreatureActionBase* p) { ++g_destroyed; }
struct SCountBlock { long m_references; void (__fastcall* m_pfnDestroy)(CCreatureActionBase*); CCreatureActionBase* m_pObject; };
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject; SCountBlock* m_pCount;
    CCountedPointer_CCreatureActionBase() : m_pObject(0), m_pCount(0) { }
    __forceinline void Reset(CCreatureActionBase* object)
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
        m_pCount = 0; m_pObject = object;
    }
};
struct C3DVector { float X, Y, Z; };
struct CNavigationInput { long m_00, m_04, m_08, m_0c, MovementType, m_14; CNavigationInput() : m_0c(0), MovementType(0), m_14(0) { } };
struct CBrainOutput {
    unsigned long Types; long LookFlags; CNavigationInput NavigationInput; CCountedPointer_CCreatureActionBase PAction; C3DVector MoveToPos; C3DVector LookAtPos; CIntelligentPointer LookAtThing;
    CBrainOutput();
};
CBrainOutput::CBrainOutput() : NavigationInput(), PAction(), LookAtThing() { Types = 0; LookFlags = 0; PAction.Reset(0); }
int main()
{
    unsigned char raw[sizeof(CBrainOutput)]; memset(raw, 0xCD, sizeof(raw));
    CBrainOutput* o = new (raw) CBrainOutput();
    bool ok = sizeof(CBrainOutput) == 72 && g_ipCtors == 1 && g_destroyed == 0 && o->Types == 0 && o->LookFlags == 0 && o->NavigationInput.m_0c == 0 && o->NavigationInput.MovementType == 0 && o->NavigationInput.m_14 == 0
        && o->NavigationInput.m_00 == (long)0xCDCDCDCD && o->PAction.m_pObject == 0 && o->PAction.m_pCount == 0 && o->LookAtThing.m_pItem == 0 && *(void**)&o->LookAtThing != 0;
    // Reset on a live block releases it
    CCreatureActionBase act; SCountBlock* blk = new SCountBlock; blk->m_references = 1; blk->m_pfnDestroy = DestroyAction; blk->m_pObject = &act;
    o->PAction.m_pObject = &act; o->PAction.m_pCount = blk; o->PAction.Reset(0);
    ok = ok && g_destroyed == 1 && o->PAction.m_pCount == 0 && o->PAction.m_pObject == 0;
    if (!ok) { printf("00835620_TEST FAIL\n"); return 1; }
    printf("00835620_TEST PASS\n"); return 0;
}