// Behaviour test for CRegionMinimap::BuildRegionMinimapTGA (0x0066CD50), a portable model of the row: every cell (x, y) of a
// size x size minimap is drawn at (x, size - 1 - y); a theme whose lighting flag is clear keeps its colour; a lit theme blends its
// colour towards a 0.8-darkened copy with alpha 0x80 by weight round((light + 1) * 0.5 * 256) (GFGetInterpolatedColour:
// (a * (256 - w) + b * w) >> 8 per channel); the surface is saved once after all pixels and the cell list is then released.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct Col { unsigned char B, G, R, A; };
struct Cell { long theme; float light; };
static Col MakeCol(int r, int g, int b, int a) { Col c; c.R = (unsigned char)r; c.G = (unsigned char)g; c.B = (unsigned char)b; c.A = (unsigned char)a; return c; }
static Col Blend(Col a, Col b, unsigned long w)
{
    Col o;
    o.B = (unsigned char)((a.B * (256 - w) + b.B * w) >> 8);
    o.G = (unsigned char)((a.G * (256 - w) + b.G * w) >> 8);
    o.R = (unsigned char)((a.R * (256 - w) + b.R * w) >> 8);
    o.A = (unsigned char)((a.A * (256 - w) + b.A * w) >> 8);
    return o;
}
struct Minimap {
    long Size; Cell* Cells; Col Themes[4]; bool Lit[4];
    Col Pixels[3][3]; int Writes; int Saves; int WritesAtSave;
    void SetPixel(long x, long y, const Col& c) { Pixels[y][x] = c; ++Writes; }
    void SaveAsTGA() { ++Saves; WritesAtSave = Writes; }
    void Build()
    {
        long tga_width = Size, tga_height = Size;
        for (long y = 0; y < tga_height; ++y)
            for (long x = 0; x < tga_width; ++x) {
                const Cell& cell = Cells[Size * y + x];
                Col themeCol = Themes[cell.theme], col;
                if (Lit[cell.theme]) {
                    float interpRange = 0.8f;
                    Col interpCol = MakeCol((unsigned char)(themeCol.R * interpRange), (unsigned char)(themeCol.G * interpRange),
                                            (unsigned char)(themeCol.B * interpRange), 0x80);
                    float interpVal = (cell.light + 1.0f) * 0.5f;
                    col = Blend(themeCol, interpCol, (unsigned long)floor(interpVal * 256.0f + 0.5f));
                } else
                    col = themeCol;
                SetPixel(x, tga_height - 1 - y, col);
            }
        SaveAsTGA();
        free(Cells); Cells = 0;
    }
};
static bool Eq(Col a, Col b) { return a.R == b.R && a.G == b.G && a.B == b.B && a.A == b.A; }
int main()
{
    int bad = 0;
    Minimap m; m.Size = 3; m.Writes = 0; m.Saves = 0; m.WritesAtSave = -1;
    m.Cells = (Cell*)malloc(9 * sizeof(Cell));
    m.Themes[0] = MakeCol(10, 20, 30, 255); m.Lit[0] = false;
    m.Themes[1] = MakeCol(100, 50, 200, 255); m.Lit[1] = true;
    m.Themes[2] = MakeCol(0, 0, 0, 0); m.Lit[2] = false;
    m.Themes[3] = MakeCol(40, 80, 120, 0); m.Lit[3] = true;
    for (int i = 0; i < 9; ++i) { m.Cells[i].theme = 0; m.Cells[i].light = 0.0f; }
    m.Cells[1 * 3 + 2].theme = 1; m.Cells[1 * 3 + 2].light = 0.5f;     // (x 2, y 1): weight 192
    m.Cells[0 * 3 + 0].theme = 1; m.Cells[0 * 3 + 0].light = -1.0f;    // (x 0, y 0): weight 0 = theme colour
    m.Cells[2 * 3 + 1].theme = 3; m.Cells[2 * 3 + 1].light = 1.0f;     // (x 1, y 2): weight 256 = darkened copy
    m.Cells[0 * 3 + 2].theme = 2;                                      // unlit theme 2
    m.Build();
    if (m.Writes != 9 || m.Saves != 1 || m.WritesAtSave != 9 || m.Cells != 0) ++bad;
    // (2, 1) -> pixel row 3 - 1 - 1 = 1; dark copy (80, 40, 160, 0x80); (a * 64 + b * 192) >> 8
    if (!Eq(m.Pixels[1][2], MakeCol((100 * 64 + 80 * 192) >> 8, (50 * 64 + 40 * 192) >> 8, (200 * 64 + 160 * 192) >> 8, (255 * 64 + 0x80 * 192) >> 8))) ++bad;
    if (!Eq(m.Pixels[2][0], MakeCol(100, 50, 200, 255))) ++bad;                        // (0, 0) -> row 2, weight 0
    if (!Eq(m.Pixels[0][1], MakeCol(32, 64, 96, 0x80))) ++bad;                         // (1, 2) -> row 0, weight 256: 40*0.8, 80*0.8, 120*0.8
    if (!Eq(m.Pixels[2][2], MakeCol(0, 0, 0, 0))) ++bad;                               // (2, 0) -> row 2, unlit theme 2 as is
    if (!Eq(m.Pixels[1][0], MakeCol(10, 20, 30, 255))) ++bad;                          // (0, 1) -> row 1, unlit theme 0
    printf(bad ? "0066cd50_TEST FAIL %d\n" : "0066cd50_TEST PASS\n", bad);
    return bad != 0;
}
