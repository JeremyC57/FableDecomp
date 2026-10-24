#include <cstdio>
static unsigned long g_seq[8]; static int g_n = 0;
struct CChangingStateComponent {
    virtual ~CChangingStateComponent() {}
    virtual void ChangeState(unsigned long s) { if (g_n < 8) g_seq[g_n++] = s; }
};
struct CScrollingComponent : CChangingStateComponent {
    virtual void ChangeState(unsigned long state)
    {
        CChangingStateComponent::ChangeState(state);
        if (state == 5) ChangeState(1);
        else if (state == 6) ChangeState(0);
    }
};
int main()
{
    CScrollingComponent c;
    CChangingStateComponent* p = &c;
    p->ChangeState(5);
    if (g_n != 2 || g_seq[0] != 5 || g_seq[1] != 1) { std::printf("00547010_TEST FAIL\n"); return 1; }
    g_n = 0; p->ChangeState(6);
    if (g_n != 2 || g_seq[0] != 6 || g_seq[1] != 0) { std::printf("00547010_TEST FAIL\n"); return 1; }
    g_n = 0; p->ChangeState(2);
    if (g_n != 1 || g_seq[0] != 2) { std::printf("00547010_TEST FAIL\n"); return 1; }
    std::printf("00547010_TEST PASS\n");
    return 0;
}