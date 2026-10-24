#include <cstdio>
struct CChangingStateComponent {
    float ParentUpdateTime;
    unsigned long LastState;
    int Calls;
    CChangingStateComponent() : ParentUpdateTime(0.0f), LastState(0), Calls(0) {}
    virtual ~CChangingStateComponent() {}
    virtual void ChangeState(unsigned long state) { LastState = state; ++Calls; }
    virtual void ChangeState(unsigned long state, float parentUpdateTime) { ParentUpdateTime = parentUpdateTime; ChangeState(state); }
};
struct CDerived : CChangingStateComponent {
    using CChangingStateComponent::ChangeState;
    virtual void ChangeState(unsigned long state) { LastState = state + 100; ++Calls; }
};
int main()
{
    CDerived d;
    CChangingStateComponent* p = &d;
    p->ChangeState(3, 0.5f);
    if (d.ParentUpdateTime != 0.5f || d.LastState != 103 || d.Calls != 1) { std::printf("0041c5a0_TEST FAIL\n"); return 1; }
    std::printf("0041c5a0_TEST PASS\n");
    return 0;
}