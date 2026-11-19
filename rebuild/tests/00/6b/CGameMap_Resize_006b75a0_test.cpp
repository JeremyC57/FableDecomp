// Behaviour test for CGameMap::Resize (0x006B75A0), a portable model: a size whose cell count overflows a long is refused (nothing
// changes); otherwise every cell of the new map is cleared (themes 0, blend 0 = 0xFF, blend 1 = 0, sound theme -1, music 0) and
// the old cell at (x, y) moves to (x - x_offset, y - y_offset) where both maps have it. Rows are stored bottom-up:
// index (size_y - 1 - y) * size_x + x.
#include <stdio.h>
#include <vector>
struct Cell { unsigned char themes[3], blends[2]; signed char sound; unsigned char music; };
static Cell Cleared() { Cell c = { {0, 0, 0}, {0xFF, 0}, -1, 0 }; return c; }
struct Map {
    std::vector<Cell> cells; long size_x, size_y;
    Cell& At(long x, long y) { return cells[(size_y - y - 1) * size_x + x]; }
    void Resize(long width, long height, long x_offset, long y_offset)
    {
        long long overflow_check = (long long)width * height;
        if (overflow_check < 0 || overflow_check > 0x7fffffffLL) return;
        std::vector<Cell> new_cells(width * height, Cleared());
        for (long from_x = x_offset > 0 ? x_offset : 0, to_x = x_offset < 0 ? -x_offset : 0;
             from_x < (size_x < width + x_offset ? size_x : width + x_offset); ++from_x, ++to_x)
            for (long from_y = y_offset > 0 ? y_offset : 0, to_y = y_offset < 0 ? -y_offset : 0;
                 from_y < (size_y < height + y_offset ? size_y : height + y_offset); ++from_y, ++to_y)
                new_cells[(height - 1 - to_y) * width + to_x] = At(from_x, from_y);
        cells = new_cells; size_x = width; size_y = height;
    }
};
static bool Same(const Cell& a, const Cell& b) { return a.themes[0] == b.themes[0] && a.music == b.music && a.sound == b.sound && a.blends[0] == b.blends[0]; }
int main()
{
    int bad = 0;
    Map m; m.size_x = 3; m.size_y = 2; m.cells.resize(6);
    for (long y = 0; y < 2; ++y) for (long x = 0; x < 3; ++x) { Cell c = { {(unsigned char)(10 * y + x), 0, 0}, {1, 2}, 3, 4 }; m.At(x, y) = c; }
    Map grow = m; grow.Resize(4, 3, -1, 0);                      // shift right by one, one row taller
    if (grow.size_x != 4 || grow.size_y != 3 || grow.cells.size() != 12) ++bad;
    if (!Same(grow.At(0, 0), Cleared()) || grow.At(1, 0).themes[0] != 0 || grow.At(3, 1).themes[0] != 12 || !Same(grow.At(2, 2), Cleared())) ++bad;
    Map shrink = m; shrink.Resize(2, 1, 1, 1);                   // keep (1..2, 1)
    if (shrink.cells.size() != 2 || shrink.At(0, 0).themes[0] != 11 || shrink.At(1, 0).themes[0] != 12) ++bad;
    Map huge = m; huge.Resize(0x10000, 0x10000, 0, 0);           // 2^32 cells: refused
    if (huge.size_x != 3 || huge.cells.size() != 6) ++bad;
    Map neg = m; neg.Resize(-2, 3, 0, 0);                        // negative count: refused
    if (neg.size_x != 3) ++bad;
    printf(bad ? "006b75a0_TEST FAIL\n" : "006b75a0_TEST PASS\n");
    return bad ? 1 : 0;
}