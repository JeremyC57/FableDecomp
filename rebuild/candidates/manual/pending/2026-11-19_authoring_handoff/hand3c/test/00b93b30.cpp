// Behaviour test for CEngineSceneMap::AddPrimitive (0x00B93B30), a portable model of the row. The position is normalised over the map
// area, scaled by the level size (1 << level), floored and clamped to the level; the primitive keeps that full grid position, and the cell
// it is filed in is the same position shifted down to the deepest existing level.
#include <stdio.h>
#include <math.h>
struct GridPos { signed char Level; unsigned char X, Y; };
struct Result { GridPos stored; long fileLevel; unsigned long cellX, cellY; };
static Result AddPrimitive(float tlx, float tly, float brx, float bry, long levels, long calcLevel, float wx, float wy)
{
    float mx = (wx - tlx) / (brx - tlx), my = (wy - tly) / (bry - tly);
    GridPos g; g.Level = (signed char)calcLevel;
    unsigned long level_size = 1ul << g.Level;
    g.X = (unsigned char)(long)floor((float)level_size * mx);
    g.Y = (unsigned char)(long)floor((float)level_size * my);
    if (g.X > level_size - 1) g.X = (unsigned char)(level_size - 1);
    if (g.Y > level_size - 1) g.Y = (unsigned char)(level_size - 1);
    Result r; r.stored = g;
    unsigned long level = (unsigned long)g.Level < (unsigned long)levels - 1 ? (unsigned long)g.Level : (unsigned long)levels - 1;
    r.fileLevel = (long)level;
    r.cellX = (unsigned char)(g.X >> (g.Level - level));
    r.cellY = (unsigned char)(g.Y >> (g.Level - level));
    return r;
}
int main()
{
    int bad = 0;
    Result a = AddPrimitive(0.0f, 0.0f, 64.0f, 64.0f, 4, 2, 40.0f, 10.0f);      // level 2 (4x4), cell (2, 0), level exists
    if (a.stored.Level != 2 || a.stored.X != 2 || a.stored.Y != 0 || a.fileLevel != 2 || a.cellX != 2 || a.cellY != 0) ++bad;
    Result b = AddPrimitive(0.0f, 0.0f, 64.0f, 64.0f, 2, 3, 63.0f, 33.0f);      // level 3 (8x8) cell (7, 4); only levels 0..1 exist
    if (b.stored.Level != 3 || b.stored.X != 7 || b.stored.Y != 4 || b.fileLevel != 1 || b.cellX != 1 || b.cellY != 1) ++bad;
    Result c = AddPrimitive(-32.0f, 100.0f, 32.0f, 164.0f, 5, 3, 32.0f, 164.0f); // the far edge clamps to size - 1
    if (c.stored.X != 7 || c.stored.Y != 7 || c.cellX != 7 || c.cellY != 7) ++bad;
    Result d = AddPrimitive(0.0f, 0.0f, 64.0f, 64.0f, 3, 0, 50.0f, 50.0f);       // level 0: one cell
    if (d.stored.Level != 0 || d.stored.X != 0 || d.stored.Y != 0 || d.fileLevel != 0) ++bad;
    printf(bad ? "00b93b30_TEST FAIL\n" : "00b93b30_TEST PASS\n");
    return bad ? 1 : 0;
}
