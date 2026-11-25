// Behaviour test for 0x006448A4 DrawMoralityNative::Draw: sets the bar's update graphic; a non-zero (retail's pooled 0.0f, 0x0122DEDC)
// morality draws the bar on the good (> 0) or evil side with alpha 255.0f (0x01230014) * the gui's alpha.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); callees are defined
// here under the row spellings (or as thiscall-compatible __fastcall fixtures), so the test observes the float literal(s) the row uses:
// a mutated literal (-> 0.5f) changes the behaviour and fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CEnginePrimitivePrepareInfo;
struct DrawMoralityNative { void Draw(const CEnginePrimitivePrepareInfo&); };
static unsigned char g_def[0x560]; static long g_graphics[8];
struct PlayerGuiDefNative;
PlayerGuiDefNative* __fastcall PeekGuiDef() { return (PlayerGuiDefNative*)g_def; }
static char g_log[128]; static const void* g_bank = 0;
// StatBarNative slot 0: DrawStatBarOnLeft(info, bank, pos, offset, colour, alpha, positive)
extern "C" void __fastcall NF420_DrawStatBarOnLeft(void*, int, const void*, const void* bank, const float* pos, const float* offset, const void*, int alpha, int positive)
{ char b[64]; sprintf(b, "draw(%g,%g,%d,%d)", pos[0], offset[0], alpha & 0xff, positive & 0xff); strcat(g_log, b); if (bank != g_bank) exit(5); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static void* barVtbl[1] = { (void*)NF420_DrawStatBarOnLeft };
    static unsigned char bar[0x54]; *(void**)bar = barVtbl;
    static unsigned char gui[0x2e0]; *(float*)(gui + 0x2a0) = 0.5f; *(void**)(gui + 0x2dc) = bar;
    static unsigned char bank[4]; g_bank = bank;
    static unsigned char self[0x1c]; *(void**)(self + 4) = gui; *(void**)(self + 0x14) = bank;
    g_graphics[5] = 55; *(long**)(g_def + 0x54c) = g_graphics;
    *(float*)(g_def + 0x4fc) = 1.0f; *(float*)(g_def + 0x504) = 2.0f; *(float*)(g_def + 0x4ac) = 3.0f; *(float*)(g_def + 0x4b4) = 4.0f;
    *(long*)(g_def + 0x530) = 66; *(long*)(g_def + 0x534) = 77;
    DrawMoralityNative* d = (DrawMoralityNative*)self;
    *(float*)(bar + 0x10) = 0.0f; d->Draw(*(const CEnginePrimitivePrepareInfo*)0);
    bool ok = g_log[0] == 0 && *(long*)(bar + 0x50) == 55 && self[0x18] == 1;
    *(float*)(bar + 0x10) = -0.25f; d->Draw(*(const CEnginePrimitivePrepareInfo*)0);
    ok = ok && strcmp(g_log, "draw(2,3,127,0)") == 0 && *(long*)(bar + 0x50) == 77;
    g_log[0] = 0; *(float*)(bar + 0x10) = 0.25f; d->Draw(*(const CEnginePrimitivePrepareInfo*)0);
    ok = ok && strcmp(g_log, "draw(1,4,127,1)") == 0 && *(long*)(bar + 0x50) == 66;
    if (ok) printf("006448a4_TEST PASS\n");
    else printf("FAIL log=%s\n", g_log);
    return ok ? 0 : 1;
}
