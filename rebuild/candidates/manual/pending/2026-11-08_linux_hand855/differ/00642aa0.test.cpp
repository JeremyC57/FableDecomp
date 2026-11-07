// Behaviour model of CMinimapData::PopulateMinimapThemeList (0x00642AA0): every minimap cell takes the minimap theme of the strongest
// engine theme at its world position (engine EMinimapThemeType 1..7 -> minimap theme 1,6,4,5,2,7,3, anything else 0); then for every
// building overlapping the map, the cells under its bounding circle's square (clamped to the map) whose downward probe from 100 above
// the ground hits the building's physics mesh become theme 8.
#include <cstdio>
#include <vector>
struct Building { float CX, CY, R; bool Hit(float x, float y) const { return (x - CX) * (x - CX) + (y - CY) * (y - CY) <= R * R; } };
struct Grid { long W, H; std::vector<int> Cells; int& At(long x, long y) { return Cells[y * W + x]; } };
static int Map(int engine)
{
    switch (engine) { case 1: return 1; case 2: return 6; case 3: return 4; case 4: return 5; case 5: return 2; case 6: return 7; case 7: return 3; default: return 0; }
}
static void Populate(Grid& g, int (*strongest)(long, long), const std::vector<Building>& buildings)
{
    for (long y = 0; y < g.H; ++y)
        for (long x = 0; x < g.W; ++x) g.At(x, y) = Map(strongest(x, y));
    for (size_t i = 0; i < buildings.size(); ++i) {
        const Building& b = buildings[i];
        long cx = (long)b.CX, cy = (long)b.CY, r = (long)b.R;
        long left = cx - r, top = cy - r, right = cx + r, bottom = cy + r;
        if (left < 0) left = 0; else if (left > g.W) left = g.W;
        if (top < 0) top = 0; else if (top > g.H) top = g.H;
        if (right < 0) right = 0; else if (right > g.W) right = g.W;
        if (bottom < 0) bottom = 0; else if (bottom > g.H) bottom = g.H;
        for (long y = top; y < bottom; ++y)
            for (long x = left; x < right; ++x)
                if (b.Hit((float)x, (float)y)) g.At(x, y) = 8;
    }
}
static int Strongest(long x, long y) { return (int)((x + y) % 9); }
int main()
{
    Grid g; g.W = 8; g.H = 8; g.Cells.assign(64, -1);
    std::vector<Building> b; Building one = { 6.0f, 6.0f, 1.0f }; b.push_back(one);
    Populate(g, Strongest, b);
    if (g.At(0, 0) != 0 || g.At(1, 0) != 1 || g.At(2, 0) != 6 || g.At(7, 0) != 3 || g.At(8 % 8, 1) != 1) { std::printf("00642aa0_TEST FAIL themes\n"); return 1; }
    if (g.At(6, 6) != 8 || g.At(5, 6) != 8 || g.At(7, 7) == 8 || g.At(4, 4) == 8) { std::printf("00642aa0_TEST FAIL buildings\n"); return 1; }
    std::printf("00642aa0_TEST PASS\n");
    return 0;
}
