// Behaviour test for 0x0055AF60 NUISystem::CClickable::OnLeftClicked: state saved, def's state applied, def released, actions run, event observed.
#include <cstdio>
struct CClickableDef {
    long m_count; long LeftClickedState; int destroyed;
    void Destroy() { ++destroyed; }
    void ReleaseTemporary() { --m_count; if (m_count == 0) Destroy(); }
};
struct CDefPointer {
    CClickableDef* m_p;
    CDefPointer(CClickableDef* p) : m_p(p) { if (m_p) ++m_p->m_count; }
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
    CClickableDef* operator->() const { return m_p; }
};
struct CClickable {
    CClickableDef* def; long CurrentState; long StateBeforeLeftClicked; void* PActionsOnLeftClicked;
    void* executed; long observed;
    CDefPointer GetDef() { return CDefPointer(def); }
    void ChangeState(long s) { CurrentState = s; }
    void ExecuteActions(void* a) { executed = a; }
    void ObserveEvent(long e) { observed = e; }
    void OnLeftClicked()
    {
        StateBeforeLeftClicked = CurrentState;
        ChangeState(GetDef()->LeftClickedState);
        ExecuteActions(PActionsOnLeftClicked);
        ObserveEvent(0x1c);
    }
};
int main()
{
    CClickableDef def = { 1, 4, 0 }; int actions = 0;
    CClickable c; c.def = &def; c.CurrentState = 2; c.PActionsOnLeftClicked = &actions; c.executed = 0; c.observed = 0;
    c.OnLeftClicked();
    if (c.StateBeforeLeftClicked != 2 || c.CurrentState != 4 || def.m_count != 1 || def.destroyed != 0 ||
        c.executed != &actions || c.observed != 0x1c) { std::printf("0055af60_TEST FAIL\n"); return 1; }
    std::printf("0055af60_TEST PASS\n");
    return 0;
}