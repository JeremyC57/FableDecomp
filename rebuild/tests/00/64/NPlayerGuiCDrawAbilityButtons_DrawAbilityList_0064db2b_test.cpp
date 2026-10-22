// Behaviour test for NPlayerGui::CDrawAbilityButtons::DrawAbilityList (0x0064DB2B): a dead hero or one without the abilities
// component draws nothing; otherwise rows after the scroll row that hold an ability are drawn first, then the rows before it,
// with consecutive draw rows, each column using its slot offset, at 0.75 of the given scale.
#include <cstdio>

struct Call { long col, index, row; float scale; };
static Call calls[64];
static int ncalls;

void DrawAbilityList(bool alive, bool hasComponent, const long* abilities, long count, const long* offsets, long columns,
                     long scrollIndex, float scale)
{
    if (!alive || !hasComponent)
        return;
    scale *= 0.75f;
    long firstRow = scrollIndex / columns + 1;
    long rows = (count + 2) / columns * columns / columns;
    long scrollRow = firstRow - 1;
    long drawn = firstRow - 1;
    for (int pass = 0; pass < 2; ++pass) {
        long from = pass == 0 ? firstRow : 0;
        long to = pass == 0 ? rows : scrollRow;
        for (long row = from; row < to; ++row) {
            bool any = false;
            for (long col = 0; col < columns; ++col) {
                unsigned long index = (unsigned long)(offsets[col] + row * columns - 1);
                if (index < (unsigned long)count && abilities[index] != 0) { any = true; break; }
            }
            if (any) {
                ++drawn;
                for (long col = 0; col < columns; ++col) {
                    Call c; c.col = col; c.index = offsets[col] + row * columns - 1; c.row = drawn - firstRow; c.scale = scale;
                    calls[ncalls++] = c;
                }
            }
        }
    }
}

int main()
{
    int ok = 1;
    long offsets[3] = { 1, 2, 3 };
    // 4 rows of 3: row 0 has abilities, row 1 is empty, row 2 has one, row 3 has one
    long abilities[12] = { 5, 0, 7,  0, 0, 0,  0, 9, 0,  1, 0, 0 };
    ncalls = 0;
    DrawAbilityList(false, true, abilities, 12, offsets, 3, 3, 1.0f);
    DrawAbilityList(true, false, abilities, 12, offsets, 3, 3, 1.0f);
    if (ncalls != 0) ok = 0;
    // scroll row 1: draw rows 2,3 (rows = (12+2)/3*3/3 = 4), then wrap to row 0; row 1 itself is skipped
    DrawAbilityList(true, true, abilities, 12, offsets, 3, 3, 2.0f);
    if (ncalls != 9) ok = 0;
    if (calls[0].index != 6 || calls[0].row != 0 || calls[0].scale != 1.5f) ok = 0;
    if (calls[3].index != 9 || calls[3].row != 1) ok = 0;
    if (calls[6].index != 0 || calls[6].row != 2 || calls[8].col != 2 || calls[8].index != 2) ok = 0;
    std::printf(ok ? "0064db2b_TEST PASS\n" : "0064db2b_TEST FAIL\n");
    return ok ? 0 : 1;
}