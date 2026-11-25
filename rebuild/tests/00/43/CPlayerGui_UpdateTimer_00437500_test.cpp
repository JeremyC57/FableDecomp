// Behaviour test for 0x00437500 CPlayerGui_UpdateTimer_00437500: steps a fade timer up (mode 2) or down (mode 3) and clamps it at
// 0.0 (mode 1; retail's pooled 0.0f, 0x0122DEDC) and 1.0 (mode 0).
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); callees are defined
// here under the row spellings (or as thiscall-compatible __fastcall fixtures), so the test observes the float literal the row uses:
// a mutated literal (0.0f -> 0.5f) changes the result and fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
void __stdcall CPlayerGui_UpdateTimer_00437500(const float* pRate, float stepUp, float stepDown, int* pMode, float* pTimer);
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    float rate = 0.1f, timer = 0.3f; int mode = 3;
    CPlayerGui_UpdateTimer_00437500(&rate, 1.0f, 1.0f, &mode, &timer);          // 0.3 - 0.1: still above zero
    bool ok = mode == 3 && timer > 0.19f && timer < 0.21f;
    timer = 0.05f; CPlayerGui_UpdateTimer_00437500(&rate, 1.0f, 1.0f, &mode, &timer);   // below zero: clamped
    ok = ok && mode == 1 && timer == 0.0f;
    mode = 2; timer = 0.95f; CPlayerGui_UpdateTimer_00437500(&rate, 1.0f, 1.0f, &mode, &timer);
    ok = ok && mode == 0 && timer == 1.0f;
    if (ok) printf("PASS CPlayerGui::UpdateTimer 0x00437500\n");
    else printf("FAIL mode=%d timer=%f\n", mode, timer);
    return ok ? 0 : 1;
}
