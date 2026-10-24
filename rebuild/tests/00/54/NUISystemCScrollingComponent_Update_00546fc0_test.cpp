#include <cstdio>
struct CChangingStateComponent {
    unsigned long CurrentState; bool Changed; long Requested; int BaseCalls;
    CChangingStateComponent() : CurrentState(0), Changed(false), Requested(-1), BaseCalls(0) {}
    virtual ~CChangingStateComponent() {}
    virtual void Update(float) { ++BaseCalls; }
    virtual void ChangeState(unsigned long s) { Requested = (long)s; }
    virtual bool ChangedStateLastUpdate() { return Changed; }
};
struct CScrollingComponent : CChangingStateComponent {
    virtual void Update(float dt)
    {
        CChangingStateComponent::Update(dt);
        if (ChangedStateLastUpdate()) {
            switch (CurrentState) {
            case 1: ChangeState(3); break;
            case 3: ChangeState(1); break;
            }
        }
    }
};
int main()
{
    CScrollingComponent c;
    c.Changed = true; c.CurrentState = 1; c.Update(0.1f);
    if (c.Requested != 3) { std::printf("00546fc0_TEST FAIL\n"); return 1; }
    c.Requested = -1; c.CurrentState = 3; c.Update(0.1f);
    if (c.Requested != 1) { std::printf("00546fc0_TEST FAIL\n"); return 1; }
    c.Requested = -1; c.CurrentState = 2; c.Update(0.1f);
    if (c.Requested != -1) { std::printf("00546fc0_TEST FAIL\n"); return 1; }
    c.Changed = false; c.CurrentState = 1; c.Update(0.1f);
    if (c.Requested != -1 || c.BaseCalls != 4) { std::printf("00546fc0_TEST FAIL\n"); return 1; }
    std::printf("00546fc0_TEST PASS\n");
    return 0;
}