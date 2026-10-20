#include <cstdio>
#include <new>
// Behaviour model of CScriptGameResourceObjectScriptedThing::~CScriptGameResourceObjectScriptedThing @ 0x00903AC0.
static int g_log[32], g_n = 0;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
struct CThing_Retail {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool m_dead;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBase { virtual ~CBase() {} };
struct CActionBase : CBase { long m_actionState; bool m_started, m_finished; CActionBase() : m_actionState(0), m_started(false), m_finished(false) {} };
struct CActionStopScripting : CActionBase { };
struct CTCScriptedControl {
    bool Locked; CActionBase* last;
    void AddAction(CActionBase* a) { last = a; g_log[g_n++] = 1; }
    void ClearLocked() { Locked = false; g_log[g_n++] = 2; }
    void CleanupAllActions() { g_log[g_n++] = 3; }
};
static int g_destroyed = 0;
static void DestroyImp(void*) { ++g_destroyed; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template<class T> struct CCountedPointer {
    T* Data; SCountBlock* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) { Info->m_pfnDestroy(Info->m_pObject); operator delete(Info); }
        }
        Data = 0; Info = 0;
    }
};
struct CBaseObject { virtual ~CBaseObject() { g_log[g_n++] = 9; } };
struct CScriptGameResourceObjectBase : CBaseObject { virtual ~CScriptGameResourceObjectBase() {} };
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase { CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp; };
struct CIntelligentPointer {
    CThing_Retail* m_pItem;
    ~CIntelligentPointer() { g_log[g_n++] = 8; }
    CThing_Retail* GetPItem() { return m_pItem; }
};
struct CScriptGameResourceObjectScriptedThing : CScriptGameResourceObjectScriptedThingBase {
    CIntelligentPointer PThing;
    virtual ~CScriptGameResourceObjectScriptedThing()
    {
        if (PThing.GetPItem()) {
            if (!PThing.GetPItem()->m_dead) {
                CTCScriptedControl* control;
                PThing.GetPItem()->GetInterface(&control, 0x1f);
                if (control->Locked) {
                    control->AddAction(new CActionStopScripting());
                    control->ClearLocked();
                    control->CleanupAllActions();
                }
            }
        }
    }
};
static CScriptGameResourceObjectScriptedThing* Make(CThing_Retail* t, SCountBlock* b)
{
    CScriptGameResourceObjectScriptedThing* s = new CScriptGameResourceObjectScriptedThing;
    s->PThing.m_pItem = t; s->PImp.Data = 0; s->PImp.Info = b;
    return s;
}
int main()
{
    CTCScriptedControl control; control.Locked = true; control.last = 0;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x1f, (CTCBase*)&control } };
    CThing_Retail thing = { { 0x80000000UL, 0, 0, 0, 0, 0, 0, 0, 0 }, { entries, entries + 2 }, false };
    SCountBlock* b = new SCountBlock; b->m_references = 1; b->m_pfnDestroy = DestroyImp; b->m_pObject = 0;
    delete Make(&thing, b);
    int want[] = { 1, 2, 3, 8, 9 };
    int ok = g_n == 5 && g_destroyed == 1 && !control.Locked && control.last != 0;
    for (int i = 0; ok && i < 5; ++i) if (g_log[i] != want[i]) ok = 0;
    delete control.last;
    g_n = 0; delete Make(&thing, 0);                          // not locked any more
    ok = ok && g_n == 2 && g_log[0] == 8;
    thing.m_dead = true; control.Locked = true; g_n = 0;
    SCountBlock* b2 = new SCountBlock; b2->m_references = 2; b2->m_pfnDestroy = DestroyImp; b2->m_pObject = 0;
    delete Make(&thing, b2);                                  // dead thing: no actions, shared block survives
    ok = ok && g_n == 2 && control.Locked && g_destroyed == 1 && b2->m_references == 1;
    delete b2;
    if (!ok) { std::printf("00903ac0_TEST FAIL\n"); return 1; }
    std::printf("00903ac0_TEST PASS\n");
    return 0;
}
