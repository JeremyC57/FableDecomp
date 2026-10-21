// Behaviour test for CCombatSequenceJacksCombo::OnCreate (0x0090A1F0): three interruptable mid attacks (code 150)
// on the sequence's owner and target are queued, then the base OnCreate runs; the queue holds the only references.
#include <cstdio>

struct CThing { int id; };
struct CThingCreatureBase : CThing {};
struct CSoundPair { long m_first, m_second; CSoundPair() : m_first(0), m_second(0) {} };
struct CCombatActionBase {
    CThingCreatureBase* Owner; CThing* Target; long Code; long Sound0;
    CCombatActionBase(CThingCreatureBase* o, CThing* t, long code, const CSoundPair& s) : Owner(o), Target(t), Code(code), Sound0(s.m_first) {}
    virtual ~CCombatActionBase() {}
};
static int g_deleted = 0;
struct SCountBlock { long m_references; CCombatActionBase* m_pObject; };
struct CCountedPointer {
    CCombatActionBase* m_pObject; SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    void Release() { if (m_pCount && --m_pCount->m_references == 0) { delete m_pCount->m_pObject; ++g_deleted; delete m_pCount; } m_pObject = 0; m_pCount = 0; }
    void Reset(CCombatActionBase* o) { Release(); m_pObject = o; m_pCount = o ? new SCountBlock : 0; if (m_pCount) { m_pCount->m_references = 1; m_pCount->m_pObject = o; } }
    CCountedPointer& operator=(CCombatActionBase* o) { Reset(o); return *this; }
    ~CCountedPointer() { Release(); }
};
struct CCombatSequenceBase {
    CThingCreatureBase* m_pOwner; CThing* m_pTarget;
    CCountedPointer Queue[4]; int Count; bool Created;
    void AddAction(const CCountedPointer& a) { Queue[Count].m_pObject = a.m_pObject; Queue[Count].m_pCount = a.m_pCount; if (a.m_pCount) ++a.m_pCount->m_references; ++Count; }
    virtual void OnCreate() { Created = true; }
};
struct CCreatureAction_InterruptableMidAttack : CCombatActionBase {
    CCreatureAction_InterruptableMidAttack(CThingCreatureBase* owner, CThing* thing) : CCombatActionBase(owner, thing, 150, CSoundPair()) {}
};
struct CCombatSequenceJacksCombo : CCombatSequenceBase { virtual void OnCreate(); };

void CCombatSequenceJacksCombo::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_InterruptableMidAttack(m_pOwner, m_pTarget);
    AddAction(action);
    action = new CCreatureAction_InterruptableMidAttack(m_pOwner, m_pTarget);
    AddAction(action);
    action = new CCreatureAction_InterruptableMidAttack(m_pOwner, m_pTarget);
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}

int main()
{
    CThingCreatureBase jack; CThing foe;
    CCombatSequenceJacksCombo s; s.m_pOwner = &jack; s.m_pTarget = &foe; s.Count = 0; s.Created = false;
    s.OnCreate();
    bool ok = s.Count == 3 && s.Created && g_deleted == 0;
    for (int i = 0; i < 3; ++i)
        ok = ok && s.Queue[i].m_pObject && s.Queue[i].m_pObject->Owner == &jack && s.Queue[i].m_pObject->Target == &foe
            && s.Queue[i].m_pObject->Code == 150 && s.Queue[i].m_pObject->Sound0 == 0 && s.Queue[i].m_pCount->m_references == 1;
    if (!ok) { std::printf("0090a1f0_TEST FAIL\n"); return 1; }
    std::printf("0090a1f0_TEST PASS\n");
    return 0;
}
