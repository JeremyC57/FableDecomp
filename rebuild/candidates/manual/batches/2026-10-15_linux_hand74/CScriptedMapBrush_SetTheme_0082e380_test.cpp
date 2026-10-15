// Behaviour test for CScriptedMapBrush::SetTheme (0x0082E380): writes Theme[layer] of the cell inside the game area only.
#include <cstdio>
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    bool Contains(long x, long y) const { return x >= TLX && x < BRX && y >= TLY && y < BRY; }
    long Width() const { return BRX - TLX; }
};
struct CScriptedMapCell { float Height; unsigned long Theme[3]; unsigned char ThemeBlend[4]; };
struct CScriptedMapBrush {
    CScriptedMapCell* Cells;
    C2DBoxI GameArea;
    float GetGameHeight(long x, long y) const
    {
        if (GameArea.Contains(x, y))
            return Cells[(y - GameArea.TLY) * GameArea.Width() - GameArea.TLX + x].Height;
        return 0.0f;
    }
    void SetGameHeight(long x, long y, float height)
    {
        if (GameArea.Contains(x, y))
            Cells[(y - GameArea.TLY) * GameArea.Width() - GameArea.TLX + x].Height = height;
    }
    void SetTheme(long x, long y, unsigned long theme, unsigned long layer)
    {
        if (GameArea.Contains(x, y))
            Cells[(y - GameArea.TLY) * GameArea.Width() - GameArea.TLX + x].Theme[layer] = theme;
    }
};
// 4x3 brush covering game cells x 10..13, y 20..22; cell (x, y) lives at (y-20)*4 + (x-10).
static CScriptedMapCell g_cells[12];
static void Setup(CScriptedMapBrush& b)
{
    for (int i = 0; i < 12; ++i) { g_cells[i].Height = (float)i; g_cells[i].Theme[0] = g_cells[i].Theme[1] = g_cells[i].Theme[2] = 0; }
    b.Cells = g_cells; b.GameArea.TLX = 10; b.GameArea.TLY = 20; b.GameArea.BRX = 14; b.GameArea.BRY = 23;
}
int main()
{
    CScriptedMapBrush b; Setup(b);
    b.SetTheme(13, 20, 42, 2);
    if (g_cells[3].Theme[2] != 42 || g_cells[3].Theme[0] != 0 || g_cells[3].Theme[1] != 0) { std::printf("0082e380_TEST FAIL\n"); return 1; }
    b.SetTheme(10, 19, 77, 0); b.SetTheme(14, 22, 77, 1);
    for (int i = 0; i < 12; ++i) for (int l = 0; l < 3; ++l) if (g_cells[i].Theme[l] == 77) { std::printf("0082e380_TEST FAIL\n"); return 1; }
    std::printf("0082e380_TEST PASS\n");
    return 0;
}
