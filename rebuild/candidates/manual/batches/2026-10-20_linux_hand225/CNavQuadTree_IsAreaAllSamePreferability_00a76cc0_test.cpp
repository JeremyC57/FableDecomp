// Behaviour test for 0x00A76CC0 CNavQuadTree::IsAreaAllSamePreferability: true for other layers or an empty map; else
// every weight cell under the node square (map-relative, row-major by MapWidth) must equal the first one.
#include <cstdio>

static const float g_QuadTreeLevelSizes[3] = { 8.0f, 4.0f, 2.0f };
struct CNode { float X, Y; unsigned char QuadTreeLevel, NavLayer; };
struct CTree {
    float MapX, MapY, MapWidth; unsigned char* Weights; int Count;
    bool IsAreaAllSamePreferability(const CNode* node) const
    {
        float half = g_QuadTreeLevelSizes[node->QuadTreeLevel] * 0.5f;
        float tlx = node->X - half, tly = node->Y - half, brx = half + node->X, bry = half + node->Y;
        if (node->NavLayer == 0 && Count != 0) {
            unsigned char preferability = 0xff; bool found = false;
            for (long x = (long)(tlx - MapX); x < brx - MapX; ++x)
                for (long y = (long)(tly - MapY); y < bry - MapY; ++y) {
                    long index = (long)(y * MapWidth + x);
                    if (!found) { preferability = Weights[index]; found = true; }
                    else if (Weights[index] != preferability) return false;
                }
        }
        return true;
    }
};
int main()
{
    unsigned char w[64]; for (int i = 0; i < 64; ++i) w[i] = 1;
    CTree t = { 0.0f, 0.0f, 8.0f, w, 64 };
    CNode n = { 4.0f, 4.0f, 1, 0 };                          // cells x,y in [2,6)
    if (!t.IsAreaAllSamePreferability(&n)) { std::printf("00a76cc0_TEST FAIL\n"); return 1; }
    w[5 * 8 + 3] = 2;                                         // inside the square
    if (t.IsAreaAllSamePreferability(&n)) { std::printf("00a76cc0_TEST FAIL\n"); return 1; }
    n.NavLayer = 1; if (!t.IsAreaAllSamePreferability(&n)) { std::printf("00a76cc0_TEST FAIL\n"); return 1; }
    n.NavLayer = 0; w[5 * 8 + 3] = 1; w[7 * 8 + 7] = 3;        // outside the square
    if (!t.IsAreaAllSamePreferability(&n)) { std::printf("00a76cc0_TEST FAIL\n"); return 1; }
    t.Count = 0; w[5 * 8 + 3] = 2; if (!t.IsAreaAllSamePreferability(&n)) { std::printf("00a76cc0_TEST FAIL\n"); return 1; }
    std::printf("00a76cc0_TEST PASS\n");
    return 0;
}
