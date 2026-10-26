// Behaviour test for 0x006D6210 CCountedPointer<CBrainOutput>::DeleteData: deleting the brain output releases
// its action count block (destroying the action on the last reference) and runs LookAtThing's destructor.
#include <cstdio>

static int g_pointerDtors = 0;
static int g_actionsDestroyed = 0;

struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() { ++g_pointerDtors; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {};

class CCreatureActionBase { public: int m_id; };

struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(CCreatureActionBase*);
    CCreatureActionBase* m_pObject;
};

struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject;
    SCountBlock*         m_pCount;
    ~CCountedPointer_CCreatureActionBase()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

class C3DVector { public: float X, Y, Z; };
class CNavigationInput { public: unsigned char _body[0x18]; };

class CBrainOutput { public:
    unsigned long Types;
    long LookFlags;
    CNavigationInput NavigationInput;
    CCountedPointer_CCreatureActionBase PAction;
    C3DVector MoveToPos;
    C3DVector LookAtPos;
    CIntelligentPointer LookAtThing;
};

template<class T> class CCountedPointer { public:
    static void DeleteData(void* data) { delete static_cast<T*>(data); }
};

static void DestroyAction(CCreatureActionBase* a) { ++g_actionsDestroyed; delete a; }

int main()
{
    // last reference: the action is destroyed
    CBrainOutput* out = new CBrainOutput;
    out->PAction.m_pObject = new CCreatureActionBase;
    out->PAction.m_pCount = new SCountBlock;
    out->PAction.m_pCount->m_references = 1;
    out->PAction.m_pCount->m_pfnDestroy = DestroyAction;
    out->PAction.m_pCount->m_pObject = out->PAction.m_pObject;
    CCountedPointer<CBrainOutput>::DeleteData(out);
    if (g_actionsDestroyed != 1 || g_pointerDtors != 1) { std::printf("006d6210_TEST FAIL\n"); return 1; }

    // shared reference: the block survives with one reference less
    SCountBlock shared;
    shared.m_references = 2; shared.m_pfnDestroy = DestroyAction; shared.m_pObject = 0;
    out = new CBrainOutput;
    out->PAction.m_pObject = 0;
    out->PAction.m_pCount = &shared;
    CCountedPointer<CBrainOutput>::DeleteData(out);
    if (shared.m_references != 1 || g_actionsDestroyed != 1 || g_pointerDtors != 2) { std::printf("006d6210_TEST FAIL\n"); return 1; }

    // null data is a no-op
    CCountedPointer<CBrainOutput>::DeleteData(0);
    if (g_pointerDtors != 2) { std::printf("006d6210_TEST FAIL\n"); return 1; }
    std::printf("006d6210_TEST PASS\n");
    return 0;
}