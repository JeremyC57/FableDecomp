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
    long InternalChanged()
    {
        OnStateChanged(TargetState);
        if (m_timer0 >= m_end0 && m_timer1 >= m_end1 && m_timer2 >= m_end2) return 1;
        return 0;
    }
};
int main()
{
    CChangingStateComponent c; c.TargetState = 3;
    c.m_timer0 = 1.0f; c.m_end0 = 0.5f; c.m_timer1 = 2.0f; c.m_end1 = 2.0f; c.m_timer2 = 3.0f; c.m_end2 = 2.5f;
    if (c.InternalChanged() != 1 || g_stateChanged != 3) { std::printf("0052c780_TEST FAIL\n"); return 1; }
    c.m_timer1 = 1.5f;
    if (c.InternalChanged() != 0) { std::printf("0052c780_TEST FAIL\n"); return 1; }
    c.m_timer1 = 2.0f; c.m_timer2 = 0.0f;
    if (c.InternalChanged() != 0) { std::printf("0052c780_TEST FAIL\n"); return 1; }
    c.m_timer2 = 2.5f; c.m_timer0 = 0.0f;
    if (c.InternalChanged() != 0) { std::printf("0052c780_TEST FAIL\n"); return 1; }
    std::printf("0052c780_TEST PASS\n"); return 0;
}
