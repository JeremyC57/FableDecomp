// Behaviour test for CNavQuadTree::GetPreferabilityCostForNode (0x00A76E00): the node square's top-left corner, relative to the map and
// nudged by 1/8, is floored (GFFloatToLongFloor) to a weight-map cell; the row width is MapWidth rounded (GFFloatToLongNear); other
// layers or an empty map give 0x40, as does a negative cell index.
#include <stdio.h>
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongNear.h"
#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif
static const float g_QuadTreeLevelSizes[3] = { 8.0f, 4.0f, 2.0f };
struct Node { float X, Y; unsigned char Level, Layer; };
struct Tree {
    float MapX, MapY, MapWidth; const unsigned char* Weights; long Count;
    unsigned char Cost(const Node& n) const
    {
        if (n.Layer != 0 || Count == 0) return 0x40;
        float half = g_QuadTreeLevelSizes[n.Level] * 0.5f;
        float relX = (n.X - half) - MapX, relY = (n.Y - half) - MapY;
        long cellY = GFFloatToLongFloor(relY + 0.125f);
        long cellX = GFFloatToLongFloor(relX + 0.125f);
        Int64 index = (Int64)GFFloatToLongNear(MapWidth) * cellY + cellX;
        if (index < 0 || index > 0x7fffffff) return 0x40;
        return Weights[GFFloatToLongNear(MapWidth) * cellY + cellX];
    }
};
int main()
{
    unsigned char w[16]; for (int i = 0; i < 16; ++i) w[i] = (unsigned char)(i * 3);
    Tree t = { 10.0f, 20.0f, 4.0f, w, 16 };
    Node a = { 13.0f, 23.0f, 2, 0 };          // corner (12, 22) -> cell (2, 2) -> index 10
    Node b = { 13.0f, 23.0f, 2, 1 };          // other layer
    Node c = { 10.0f, 20.0f, 2, 0 };          // corner (9, 19) -> cell (-1, -1) -> negative index
    bool ok = t.Cost(a) == 30 && t.Cost(b) == 0x40 && t.Cost(c) == 0x40;
    printf(ok ? "00a76e00_TEST PASS\n" : "00a76e00_TEST FAIL\n");
    return ok ? 0 : 1;
}