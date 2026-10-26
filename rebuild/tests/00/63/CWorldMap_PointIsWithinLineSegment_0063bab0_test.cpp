#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CWorldMap
{ public:
    bool PointIsWithinLineSegment(const C3DVector* point, const C3DVector* a, const C3DVector* pos) const;
};
bool CWorldMap::PointIsWithinLineSegment(const C3DVector* point, const C3DVector* a, const C3DVector* pos) const
{
    return (a->X < pos->X ? a->X : pos->X) <= point->X
        && (a->X > pos->X ? a->X : pos->X) >= point->X
        && (a->Y < pos->Y ? a->Y : pos->Y) <= point->Y
        && (a->Y > pos->Y ? a->Y : pos->Y) >= point->Y;
}
int main()
{
    CWorldMap w;
    C3DVector a = { 4.0f, 1.0f, 0.0f }, pos = { 1.0f, 3.0f, 0.0f };
    C3DVector in = { 2.0f, 2.0f, 0.0f }, edge = { 4.0f, 3.0f, 0.0f }, outx = { 4.5f, 2.0f, 0.0f }, outy = { 2.0f, 0.5f, 0.0f };
    if (!w.PointIsWithinLineSegment(&in, &a, &pos) || !w.PointIsWithinLineSegment(&edge, &a, &pos)
        || w.PointIsWithinLineSegment(&outx, &a, &pos) || w.PointIsWithinLineSegment(&outy, &pos, &a))
    { std::printf("0063bab0_TEST FAIL\n"); return 1; }
    std::printf("0063bab0_TEST PASS\n");
    return 0;
}