#include <cstdio>
static int g_order = 0, g_start = 0, g_end = 0;
struct C3DAnimationStateBase { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void Validate() = 0; };
struct SStart : C3DAnimationStateBase { virtual void Validate() { g_start = ++g_order; } };
struct SEnd : C3DAnimationStateBase { virtual void Validate() { g_end = ++g_order; } };
struct C3DAnimationTransitionState { virtual void v00() { } unsigned long IVRefCount; C3DAnimationStateBase* StartState; C3DAnimationStateBase* EndState; float Progress;
    virtual void Validate() { StartState->Validate(); EndState->Validate(); } };
int main()
{
    SStart s; SEnd e; C3DAnimationTransitionState t; t.StartState = &s; t.EndState = &e;
    t.Validate();
    if (g_start != 1 || g_end != 2) { std::printf("00a3be30_TEST FAIL\n"); return 1; }
    std::printf("00a3be30_TEST PASS\n"); return 0;
}
