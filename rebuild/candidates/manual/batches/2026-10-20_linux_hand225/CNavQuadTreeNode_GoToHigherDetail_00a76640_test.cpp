// Behaviour test for 0x00A76640 CNavQuadTreeNode::GoToHigherDetail: true when any corner of the node square (centre,
// side = level size) lies in one of the areas (half-open on the far edges); false for no areas or no corner inside.
#include <cstdio>

struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    bool Contains(float x, float y) const { return x >= TLX && x < BRX && y >= TLY && y < BRY; }
    bool ContainsPoint(const C2DVector& p) const { return Contains(p.X, p.Y); }
};
static const float g_QuadTreeLevelSizes[4] = { 16.0f, 8.0f, 4.0f, 2.0f };
struct CNode {
    float PX, PY; unsigned char QuadTreeLevel;
    bool GoToHigherDetail(const C2DBoxF* area, const C2DBoxF* end) const
    {
        if (area == end) return false;
        float half = g_QuadTreeLevelSizes[QuadTreeLevel] * 0.5f;
        C2DBoxF box; box.TLX = PX - half; box.TLY = PY - half; box.BRX = half + PX; box.BRY = half + PY;
        for (; area != end; ++area)
            if (area->Contains(box.TLX, box.TLY) || area->Contains(box.BRX, box.TLY)
                || area->ContainsPoint(C2DVector(box.TLX, box.BRY)) || area->ContainsPoint(C2DVector(box.BRX, box.BRY)))
                return true;
        return false;
    }
};
int main()
{
    CNode n = { 10.0f, 10.0f, 1 };                       // square 6..14
    C2DBoxF far = { 20, 20, 30, 30 }, touchBR = { 13, 13, 20, 20 }, edge = { 14, 0, 20, 20 }, tl = { 0, 0, 6.5f, 6.5f };
    C2DBoxF list1[2] = { far, touchBR };
    if (n.GoToHigherDetail(list1, list1) || !n.GoToHigherDetail(list1, list1 + 2) || n.GoToHigherDetail(list1, list1 + 1)) { std::printf("00a76640_TEST FAIL\n"); return 1; }
    if (!n.GoToHigherDetail(&edge, &edge + 1) || !n.GoToHigherDetail(&tl, &tl + 1)) { std::printf("00a76640_TEST FAIL\n"); return 1; }
    C2DBoxF inner = { 7, 7, 13, 13 };                      // contains no corner
    if (n.GoToHigherDetail(&inner, &inner + 1)) { std::printf("00a76640_TEST FAIL\n"); return 1; }
    std::printf("00a76640_TEST PASS\n");
    return 0;
}
