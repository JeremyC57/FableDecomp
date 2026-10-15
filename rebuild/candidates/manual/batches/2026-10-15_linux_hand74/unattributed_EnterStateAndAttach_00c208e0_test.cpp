// Behaviour test for the 0x00C208E0 state body: state 4 first, owner registers us, link binds owner + record, final state word 1.
#include <cstdio>
struct CStateRecord { int m_tag; };
static int g_order = 0;
static int g_seenState = -1;
struct CObserverList {
    void* m_registered; int m_when;
    virtual void Register(void* observer) { m_registered = observer; m_when = ++g_order; }
};
struct COwnerObject { CObserverList m_observers; };
struct CStateLink {
    COwnerObject* m_owner; CStateRecord* m_record; int m_when;
    virtual void Bind(COwnerObject* owner, CStateRecord* record) { m_owner = owner; m_record = record; m_when = ++g_order; }
};
struct CStateMachineObject {
    CStateRecord m_record;
    CStateLink m_link;
    unsigned short m_state;
    int m_changeWhen;
    void ChangeState(long state) { g_seenState = (int)state; m_changeWhen = ++g_order; }
    void EnterStateAndAttach(COwnerObject* owner)
    {
        ChangeState(4);
        owner->m_observers.Register(this);
        m_link.Bind(owner, &m_record);
        m_state = 1;
    }
};
int main()
{
    COwnerObject owner; owner.m_observers.m_registered = 0;
    CStateMachineObject obj; obj.m_state = 7;
    obj.EnterStateAndAttach(&owner);
    if (g_seenState != 4 || obj.m_changeWhen != 1 || owner.m_observers.m_registered != &obj || owner.m_observers.m_when != 2) { std::printf("00c208e0_TEST FAIL\n"); return 1; }
    if (obj.m_link.m_owner != &owner || obj.m_link.m_record != &obj.m_record || obj.m_link.m_when != 3 || obj.m_state != 1) { std::printf("00c208e0_TEST FAIL\n"); return 1; }
    std::printf("00c208e0_TEST PASS\n");
    return 0;
}
