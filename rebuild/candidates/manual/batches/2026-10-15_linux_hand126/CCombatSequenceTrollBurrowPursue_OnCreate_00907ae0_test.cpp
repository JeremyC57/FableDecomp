// Behaviour test for 00907ae0 CCombatSequence_TrollBurrowPursue::OnCreate -- the counted-pointer OnCreate: a new action is
// built from the sequence's owner/target, adopted by a counted pointer, handed to AddAction (which
// keeps its own reference), the base OnCreate runs, and the local pointer's release leaves the
// action alive with one reference. A second run where AddAction keeps nothing must destroy it.
#include <cstdio>

struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };

static int g_destroyed = 0;
struct CAction {
    void* m_owner; void* m_target; long m_code; float m_extra; bool m_flag;
    CAction() : m_owner(0), m_target(0), m_code(0), m_extra(0), m_flag(false) { }
};
static void DestroyAction(void* p) { ++g_destroyed; delete (CAction*)p; }

struct CCountedPointer {
    CAction* m_pObject; SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    void Reset(CAction* object)
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
        m_pCount = 0; m_pObject = object;
        if (object) { m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyAction; m_pCount->m_pObject = object; }
    }
    CCountedPointer& operator=(CAction* object) { Reset(object); return *this; }
    ~CCountedPointer()
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
        m_pObject = 0; m_pCount = 0;
    }
};

static bool g_keep = true;
static SCountBlock* g_keptBlock = 0;
static CAction* g_added = 0;
static int g_baseCalls = 0;

static CAction* MakeAction(void* owner) { CAction* a = new CAction; a->m_owner = owner; return a; }

struct CSequence {
    void* m_pOwner; void* m_pTarget; long m_28;
    void AddAction(const CCountedPointer& a) { g_added = a.m_pObject; if (g_keep) { g_keptBlock = a.m_pCount; ++a.m_pCount->m_references; } }
    void BaseOnCreate() { ++g_baseCalls; }
    void* GetOwner() const { return m_pOwner; }
    void OnCreate()
    {
        CCountedPointer action;
        action = MakeAction(GetOwner());
        AddAction(action);
        BaseOnCreate();
    }
};

int main()
{
    int owner = 1, target = 2;
    CSequence s; s.m_pOwner = &owner; s.m_pTarget = &target; s.m_28 = 77;
    s.OnCreate();
    bool ok = g_added != 0 && g_baseCalls == 1 && g_destroyed == 0 && g_keptBlock && g_keptBlock->m_references == 1
        && g_added->m_owner == &owner ;
    if (g_keptBlock) { g_keptBlock->m_pfnDestroy(g_keptBlock->m_pObject); delete g_keptBlock; }
    g_keep = false; g_destroyed = 0;
    s.OnCreate();
    ok = ok && g_destroyed == 1 && g_baseCalls == 2;
    if (!ok) { std::printf("00907ae0_TEST FAIL\n"); return 1; }
    std::printf("00907ae0_TEST PASS\n");
    return 0;
}
