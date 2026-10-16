#include <cstdio>
// Behaviour model of CTCInventoryAbilities::OnInitialActivate (0x005F6B47): after the base activation and the morph
// refresh, both row cursors step back one row but not below minus one row, then both lists are re-cycled forward.
static long Max(long a, long b) { return a < b ? b : a; }
struct Inv {
    long ability, expression, perRow; int cycles;
    void Activate()
    {
        ability = Max(ability - perRow, -perRow);
        expression = Max(expression - perRow, -perRow);
        cycles += 2;
    }
};
int main()
{
    Inv v = { 8, 1, 4, 0 };
    v.Activate();
    if (v.ability != 4 || v.expression != -3 || v.cycles != 2) { std::printf("005f6b47_TEST FAIL\n"); return 1; }
    v.Activate();
    if (v.ability != 0 || v.expression != -4) { std::printf("005f6b47_TEST FAIL\n"); return 1; }
    std::printf("005f6b47_TEST PASS\n");
    return 0;
}