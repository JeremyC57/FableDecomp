// Behaviour test for 0x008C4990 CCreatureAction_ScorpionPrepareTailPlunge::FinishAction: runs the reconstructed body over
// stubbed callees: the flag gate, the mode-manager lookup, the new mode aimed at the target, AddMode receiving an add-ref'd
// copy, the counted pointer releasing (deleter called once) and the base FinishAction always running.
#include <cstdio>
#include <cstdlib>

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags6c;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
static int g_destroyed = 0;
void DestroyMode(void* object);
struct SCountBlock {
    long  m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(DestroyMode), m_pObject(object) { }
};
struct CModeBase { };
template <class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(T* object) : m_pObject(object) { if (object) m_pCount = new SCountBlock(m_pObject); else m_pCount = 0; }
    template <class U> CCountedPointer(const CCountedPointer<U>& other)
        : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }
    }
    T* operator->() const { return m_pObject; }
};
struct CModeX : CModeBase {
    CThing* m_owner; CThing* m_target;
    CModeX(CThing* owner) : m_owner(owner), m_target(0) { }
    void SetTarget(CThing* target) { m_target = target; }
};
void DestroyMode(void* object) { ++g_destroyed; delete (CModeX*)object; }
static CModeX* g_added = 0; static long g_refsSeen = 0; static CThing* g_addedTarget = 0; static CThing* g_addedOwner = 0;
struct CTCCreatureModeManager {
    bool AddMode(CCountedPointer<CModeBase> mode)
    {
        g_added = (CModeX*)mode.m_pObject; g_refsSeen = mode.m_pCount->m_references;
        g_addedTarget = g_added->m_target; g_addedOwner = g_added->m_owner; return true;
    }
};
static int g_baseFinish = 0;
class CCombatActionBase {
public:
    virtual void FinishAction() { ++g_baseFinish; }
    CBaseIntelligentPointer PCreature;
    CBaseIntelligentPointer PTarget;
    CTCCreatureModeManager* GetModeManager() const
    {
        CTCCreatureModeManager* modeManager;
        PCreature.GetPItem()->GetInterface(&modeManager, 0x31);
        return modeManager;
    }
};
class CAction : public CCombatActionBase {
public:
    virtual void FinishAction();
};
void CAction::FinishAction()
{
    if (PCreature.GetPItem()->m_flags6c & 8) {
        CThing* target = PTarget.GetPItem();
        CTCCreatureModeManager* modeManager = GetModeManager();
        CCountedPointer<CModeX> mode(new CModeX(PCreature.GetPItem()));
        mode->SetTarget(target);
        modeManager->AddMode(mode);
    }
    CCombatActionBase::FinishAction();
}

int main()
{
    CTCCreatureModeManager manager;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x31, (CTCBase*)&manager } };
    CThing owner = CThing(); owner.m_presentBits[1] = 1UL << (0x31 & 31);
    owner.m_interfaces.first = entries; owner.m_interfaces.last = entries + 2;
    CThing target = CThing();
    CThing* gate = &owner;
    CAction a; a.PCreature.m_pItem = &owner; a.PTarget.m_pItem = &target;
    gate->m_flags6c = 0;
    a.FinishAction();                                       // gate clear: no mode, base still runs
    bool ok = g_added == 0 && g_baseFinish == 1 && g_destroyed == 0;
    gate->m_flags6c = 8;
    a.FinishAction();
    ok = ok && g_added != 0 && g_refsSeen == 2 && g_addedTarget == &target && g_addedOwner == &owner &&
         g_destroyed == 1 && g_baseFinish == 2;
    if (!ok) { std::printf("008c4990_TEST FAIL\n"); return 1; }
    std::printf("008c4990_TEST PASS\n");
    return 0;
}
