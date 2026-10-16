#include <cstdio>
// Behaviour model of CTCInventoryAbilities::GetNumAssignedAbilityRows (0x005EE183): for every row index below the
// ability count, the row counts when any in-range cell of it holds a non-zero ability.
static long Rows(const long* order, unsigned int n, long perRow)
{
    long rows = 0;
    for (unsigned int row = 0; row < n; ++row)
        for (long col = 0; col < perRow; ++col) {
            unsigned int cell = (unsigned int)(perRow * row + col);
            if (cell < n && order[cell] != 0) { ++rows; break; }
        }
    return rows;
}
int main()
{
    long o[6] = { 0, 0, 3, 0, 0, 0 };
    if (Rows(o, 6, 2) != 1) { std::printf("005ee183_TEST FAIL\n"); return 1; }
    o[5] = 7;
    if (Rows(o, 6, 2) != 2 || Rows(o, 0, 2) != 0) { std::printf("005ee183_TEST FAIL\n"); return 1; }
    if (Rows(o, 6, 1) != 2) { std::printf("005ee183_TEST FAIL\n"); return 1; }
    std::printf("005ee183_TEST PASS\n");
    return 0;
}