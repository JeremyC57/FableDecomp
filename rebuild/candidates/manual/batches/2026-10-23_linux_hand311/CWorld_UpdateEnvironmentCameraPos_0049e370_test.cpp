// Behaviour test for CWorld::UpdateEnvironmentCameraPos (0x0049E370): bilinear corner weights times blend/255 per theme.
#include <cstdio>
#include <cmath>
struct Cell { unsigned char themes[3]; unsigned char blends[3]; };
static long g_count; static long g_themes[12]; static float g_weights[12]; static int g_called;
static Cell g_cells[4]; static int g_present[4];
static const Cell* CellAt(float x, float y)
{
    int idx = (x >= 4.0f ? 1 : 0) + (y >= 4.0f ? 2 : 0);
    return g_present[idx] ? &g_cells[idx] : 0;
}
static void Update(float px, float py)
{
    const Cell* cells[4];
    cells[0] = CellAt(px, py); cells[1] = CellAt(px + 4.0f, py); cells[2] = CellAt(px, py + 4.0f); cells[3] = CellAt(px + 4.0f, py + 4.0f);
    float fx = (px - (float)(int)(px * 0.25f) * 4.0f) * 0.25f;
    float fy = (py - (float)(int)(py * 0.25f) * 4.0f) * 0.25f;
    int themes[12]; float weights[12]; int count = 0; float total = 0.0f;
    for (int corner = 0; corner < 4; ++corner) {
        float weight = 0.0f;
        if (corner == 0) weight = (1.0f - fy) * (1.0f - fx);
        else if (corner == 1) weight = (1.0f - fy) * fx;
        else if (corner == 2) weight = (1.0f - fx) * fy;
        else if (corner == 3) weight = fy * fx;
        const Cell* cell = cells[corner];
        if (cell)
            for (int i = 0; i != 3; ++i) {
                int theme = cell->themes[i];
                themes[count] = theme == 0 ? 100 : theme + 10;
                float w = (float)cell->blends[i] * weight * (1.0f / 255.0f);
                weights[count++] = w; total += w;
            }
    }
    if (total > 0.00390625f) { g_called = 1; g_count = count; for (int i = 0; i < count; ++i) { g_themes[i] = themes[i]; g_weights[i] = weights[i]; } }
}
int main()
{
    for (int c = 0; c < 4; ++c) { g_present[c] = 1; g_cells[c].themes[0] = 0; g_cells[c].themes[1] = (unsigned char)(c + 1); g_cells[c].themes[2] = 5; g_cells[c].blends[0] = 255; g_cells[c].blends[1] = 0; g_cells[c].blends[2] = 0; }
    g_present[3] = 0;
    Update(1.0f, 2.0f);   // fx 0.25, fy 0.5
    int ok = g_called && g_count == 9 && g_themes[0] == 100 && g_themes[1] == 11 && g_themes[4] == 12;
    ok = ok && std::fabs(g_weights[0] - 0.375f) < 1e-5f && std::fabs(g_weights[3] - 0.125f) < 1e-5f && std::fabs(g_weights[6] - 0.375f) < 1e-5f;
    g_called = 0; for (int c = 0; c < 4; ++c) g_present[c] = 0;
    Update(1.0f, 2.0f);
    ok = ok && !g_called;
    if (!ok) { std::printf("0049e370_TEST FAIL\n"); return 1; }
    std::printf("0049e370_TEST PASS\n");
    return 0;
}
