// Behaviour test for CScriptedMapBrush::SetGameHeight (0x0082E2E0): writes the cell inside the game area only.
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
    b.SetGameHeight(12, 21, 7.5f);
    if (g_cells[6].Height != 7.5f) { std::printf("0082e2e0_TEST FAIL\n"); return 1; }
    b.SetGameHeight(14, 21, 99.0f); b.SetGameHeight(12, 23, 99.0f); b.SetGameHeight(9, 20, 99.0f);
    for (int i = 0; i < 12; ++i) if (g_cells[i].Height == 99.0f) { std::printf("0082e2e0_TEST FAIL\n"); return 1; }
    std::printf("0082e2e0_TEST PASS\n");
    return 0;
}