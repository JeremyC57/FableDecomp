#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CWorldMap
{
    bool PointIsWithinCellBL(const C3DVector* point, const C3DVector* cell) const;
};
bool CWorldMap::PointIsWithinCellBL(const C3DVector* point, const C3DVector* cell) const
{
    float blockX = (float)(long)point->X;
    return blockX == cell->X
        && cell->Y == (float)(long)point->Y
        && (point->X - blockX) < (point->Y - (float)(long)point->Y);
}
static bool T(const CWorldMap& w, float px, float py, float cx, float cy)
{
    C3DVector p = { px, py, 0.0f };
    C3DVector c = { cx, cy, 0.0f };
    return w.PointIsWithinCellBL(&p, &c);
}
int main()
{
    CWorldMap w;
    // fractions (0.2,0.3) -> TL & TR-side?; (0.8,0.7) -> BR; (0.2,0.7) -> BL; (0.7,0.2) -> TR
    if (T(w, 5.2f, 7.3f, 5.0f, 7.0f) != true) { std::printf("0063b9b0_TEST FAIL\n"); return 1; }
    if (T(w, 5.8f, 7.7f, 5.0f, 7.0f) != false) { std::printf("0063b9b0_TEST FAIL\n"); return 1; }
    if (T(w, 5.2f, 7.7f, 5.0f, 7.0f) != true) { std::printf("0063b9b0_TEST FAIL\n"); return 1; }
    if (T(w, 5.7f, 7.2f, 5.0f, 7.0f) != false) { std::printf("0063b9b0_TEST FAIL\n"); return 1; }
    if (T(w, 5.2f, 7.3f, 4.0f, 7.0f) || T(w, 5.2f, 7.3f, 5.0f, 6.0f)) { std::printf("0063b9b0_TEST FAIL\n"); return 1; }
    std::printf("0063b9b0_TEST PASS\n");
    return 0;
}
