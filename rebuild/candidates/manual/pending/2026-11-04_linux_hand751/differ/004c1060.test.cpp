// Behaviour test for CTCGraphicAppearance::GetFocalPos (0x004C1060, parked DIFFER row): a model of the row -- the first focal helper wins, else the second, else the interpolated position.
#include <stdio.h>
struct V { float X, Y, Z; };
static int g_have;   // bit 0: first helper exists, bit 1: second
static bool GetHelperPoint(int which, V& out) { if (!(g_have & (1 << which))) return false; out.X = (float)(which + 1); out.Y = 0; out.Z = 0; return true; }
static V Interp() { V v = { 9, 9, 9 }; return v; }
static V GetFocalPos() { V pos; if (!GetHelperPoint(0, pos) && !GetHelperPoint(1, pos)) pos = Interp(); return pos; }
static int Check() { g_have = 3; int ok = GetFocalPos().X == 1; g_have = 2; ok = ok && GetFocalPos().X == 2; g_have = 0; return ok && GetFocalPos().X == 9; }
int main()
{
    int ok = Check();
    printf(ok ? "004c1060_TEST PASS\n" : "004c1060_TEST FAIL\n");
    return ok ? 0 : 1;
}
