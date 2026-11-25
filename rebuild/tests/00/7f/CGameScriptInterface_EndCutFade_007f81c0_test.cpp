#include "rebuild_abi.h"
#include <cstdio>
static int g_h1 = 0, g_h2 = 0, g_v = 0;
struct FableEndCut_007f81c0
{
    void FABLE_FASTCALL h1();
    void EndCutFade();
};
void FABLE_FASTCALL FableEndCut_007f81c0::h1() { g_h1++; }
// Naming (NF443, 2026-11-25; NF438 q2): the callee stub is renamed with the row to its PDB spelling ?SetTriggerState@CTCActivationReceptorRelay@@AAEX_N@Z (was ?h2@FableEndCut_007f81c0@@QAEXH@Z).
class CTCActivationReceptorRelay { private: void SetTriggerState(bool v); };
void CTCActivationReceptorRelay::SetTriggerState(bool v) { g_h2++; g_v = v ? 1 : 0; }
int main()
{
    FableEndCut_007f81c0 c;
    c.EndCutFade();
    if (g_h1 != 1 || g_h2 != 1 || g_v != 1) { std::printf("BAD\n"); return 1; }
    std::printf("ENDCUT_007f81c0_OK\n");
    return 0;
}