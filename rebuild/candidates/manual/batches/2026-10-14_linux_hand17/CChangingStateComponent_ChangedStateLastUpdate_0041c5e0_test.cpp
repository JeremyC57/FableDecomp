#include <cstdio>
struct SStateInfo { unsigned char Flags; };
struct CComponent;
struct CCountedComponentPointer { CComponent* Object; void* Info; };
static int g_baseInit = 0; static unsigned long g_stateChanged = (unsigned long)-1; static unsigned long g_setState = (unsigned long)-1;
static CCountedComponentPointer* g_added = 0;
struct CComponent {
    virtual void SetState(unsigned long state) { g_setState = state; }
    virtual bool IsChanged() { return m_changed; }
    virtual long GetComponentType() { return m_type; }
    virtual SStateInfo* GetStateInfo(unsigned long state) { return state == 5 ? &m_info : 0; }
    virtual void OnStateChanged(unsigned long state) { g_stateChanged = state; }
    bool m_changed; long m_type; SStateInfo m_info;
    float m_timer0, m_end0, m_timer1, m_end1, m_timer2, m_end2;
    void Initialise() { ++g_baseInit; }
    void AddChild(CCountedComponentPointer& child) { g_added = &child; }
    CComponent() : m_changed(false), m_type(0) { m_info.Flags = 0; }
};
struct CChangingStateComponent : CComponent {
    long StatesBeingDone; long StatesDone; void* StatesToDo; float ParentUpdateTime;
    unsigned long CurrentState, TargetState, RequestedState; float UpdateTime; bool PreviousUpdateChanged; unsigned long PreviousState;
    bool m_independent;
    bool IsIndependentByDefault() { return m_independent; }
    bool IsIndependentByDefault40() { return m_independent; }
    CChangingStateComponent() : StatesBeingDone(9), StatesDone(9), StatesToDo(0), ParentUpdateTime(9.0f), CurrentState(9), TargetState(9), RequestedState(9), UpdateTime(9.0f), PreviousUpdateChanged(false), PreviousState(9), m_independent(false) { }
    long ChangedStateLastUpdate()
    {
        if (PreviousUpdateChanged != IsChanged()) { if (IsChanged()) return 1; }
        return 0;
    }
};
int main()
{
    CChangingStateComponent c;
    c.PreviousUpdateChanged = true; c.m_changed = false;
    if (c.ChangedStateLastUpdate() != 0) { std::printf("0041c5e0_TEST FAIL\n"); return 1; }
    c.PreviousUpdateChanged = false; c.m_changed = true;
    if (c.ChangedStateLastUpdate() != 1) { std::printf("0041c5e0_TEST FAIL\n"); return 1; }
    c.PreviousUpdateChanged = true; c.m_changed = true;
    if (c.ChangedStateLastUpdate() != 0) { std::printf("0041c5e0_TEST FAIL\n"); return 1; }
    c.PreviousUpdateChanged = false; c.m_changed = false;
    if (c.ChangedStateLastUpdate() != 0) { std::printf("0041c5e0_TEST FAIL\n"); return 1; }
    std::printf("0041c5e0_TEST PASS\n"); return 0;
}
