// Behaviour test for CScriptedMapBrush::GetGameHeight (0x0082E290): cell height inside the game area, 0.0f outside.
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
    if (b.GetGameHeight(10, 20) != 0.0f || b.GetGameHeight(13, 22) != 11.0f || b.GetGameHeight(11, 21) != 5.0f) { std::printf("0082e290_TEST FAIL\n"); return 1; }
    if (b.GetGameHeight(9, 21) != 0.0f || b.GetGameHeight(14, 21) != 0.0f || b.GetGameHeight(11, 19) != 0.0f || b.GetGameHeight(11, 23) != 0.0f) { std::printf("0082e290_TEST FAIL\n"); return 1; }
    std::printf("0082e290_TEST PASS\n");
    return 0;
}