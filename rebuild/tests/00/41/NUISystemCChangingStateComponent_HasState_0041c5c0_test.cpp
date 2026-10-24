#include <cstdio>
struct CUIState { int v; };
struct CChangingStateComponent {
    CUIState states[2];
    virtual ~CChangingStateComponent() {}
    virtual CUIState* GetState(unsigned long index) { return index < 2 ? &states[index] : 0; }
    virtual bool HasState(unsigned long index) { return GetState(index) != 0; }
};
int main()
{
    CChangingStateComponent c;
    if (!c.HasState(0) || !c.HasState(1) || c.HasState(2) || c.HasState(77)) { std::printf("0041c5c0_TEST FAIL\n"); return 1; }
    std::printf("0041c5c0_TEST PASS\n");
    return 0;
}