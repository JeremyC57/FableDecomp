#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CWorldMap
{
    bool PointIsWithinLineSegment(const C3DVector* point, const C3DVector* a, const C3DVector* b) const;
};
bool CWorldMap::PointIsWithinLineSegment(const C3DVector* point, const C3DVector* a, const C3DVector* b) const
{
    return (a->X < b->X ? a->X : b->X) <= point->X
        && (a->X > b->X ? a->X : b->X) >= point->X
        && (a->Y < b->Y ? a->Y : b->Y) <= point->Y
        && (a->Y > b->Y ? a->Y : b->Y) >= point->Y;
}
int main()
{
    CWorldMap w;
    C3DVector a = { 4.0f, 1.0f, 0.0f }, b = { 1.0f, 3.0f, 0.0f };
    C3DVector in = { 2.0f, 2.0f, 0.0f }, edge = { 4.0f, 3.0f, 0.0f }, outx = { 4.5f, 2.0f, 0.0f }, outy = { 2.0f, 0.5f, 0.0f };
    if (!w.PointIsWithinLineSegment(&in, &a, &b) || !w.PointIsWithinLineSegment(&edge, &a, &b)
        || w.PointIsWithinLineSegment(&outx, &a, &b) || w.PointIsWithinLineSegment(&outy, &b, &a))
    { std::printf("0063bab0_TEST FAIL\n"); return 1; }
    std::printf("0063bab0_TEST PASS\n");
    return 0;
}
