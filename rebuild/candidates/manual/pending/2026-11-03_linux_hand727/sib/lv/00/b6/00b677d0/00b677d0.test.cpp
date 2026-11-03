// Behaviour test for CEngineLandscapeRenderer::SetupForegroundStates (0x00B677D0): three cached sampler states for stage 1
// (U, V address, min filter); an unchanged value is skipped, a changed one is bookmarked once per capture mask, stored and queued.
#include <stdio.h>
struct State { unsigned long Desired, Mask; bool Dirty; };
struct Core {
    State U[4], V[4], Min[4]; unsigned long CurMask; State* Stack[8]; unsigned long StackVal[8]; long StackSize;
    State* Update[8]; long UpdateSize;
    void Set(const unsigned long& value, State& s) {
        if (s.Desired != value) {
            if ((CurMask & s.Mask) == 0) { s.Mask |= CurMask; Stack[StackSize] = &s; StackVal[StackSize] = s.Desired; ++StackSize; }
            s.Desired = value;
            if (!s.Dirty) { s.Dirty = true; Update[UpdateSize++] = &s; }
        }
    }
};
static Core core;
static unsigned long valueAddr = 3, valueFilter = 2;
static void SetupForegroundStates() { core.Set(valueAddr, core.U[1]); core.Set(valueAddr, core.V[1]); core.Set(valueFilter, core.Min[1]); }
int main()
{
    core.CurMask = 4; core.V[1].Desired = 3; core.Min[1].Mask = 4; core.Min[1].Desired = 1;
    SetupForegroundStates();
    if (core.U[1].Desired != 3 || core.StackSize != 1 || core.Stack[0] != &core.U[1] || core.StackVal[0] != 0) { printf("00b677d0_TEST FAIL\n"); return 1; }
    if (core.UpdateSize != 2 || core.Update[1] != &core.Min[1] || core.Min[1].Desired != 2 || core.U[1].Mask != 4) { printf("00b677d0_TEST FAIL\n"); return 1; }
    SetupForegroundStates();
    if (core.UpdateSize != 2 || core.StackSize != 1) { printf("00b677d0_TEST FAIL\n"); return 1; }
    printf("00b677d0_TEST PASS\n"); return 0;
}