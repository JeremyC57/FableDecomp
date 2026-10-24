// Behaviour test for 0x0080A850 STLport remove_copy_if with CFilter_Bind2<IsInFOV, unary_negate<IsTargetableBehind> >: keep points out of view or targetable behind.
#include <cstdio>
struct CThing { int id; bool TargetableBehind; };
struct CTargetPoint { CThing* PThing; long Index; float X, Y, Z; long FrameRejected; };
struct CFilterInFOV { float MaxX; bool operator()(const CTargetPoint& p) const { return p.X <= MaxX; } };
struct CFilterTargetableBehind { bool operator()(const CTargetPoint& p) const { return p.PThing->TargetableBehind; } };
struct CNegate { CFilterTargetableBehind Pred; bool operator()(const CTargetPoint& p) const { return !Pred(p); } };
struct CBind2 {
    CFilterInFOV* F1; CNegate* F2;
    bool operator()(const CTargetPoint& t) const { return (*F1)(t) && (*F2)(t); }
};
static CTargetPoint* RemoveCopyIf(CTargetPoint* first, CTargetPoint* last, CTargetPoint* result, CBind2 pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) { *result = *first; ++result; }
    return result;
}
int main()
{
    CThing things[6] = { { 0, false }, { 1, true }, { 2, false }, { 3, false }, { 4, true }, { 5, false } };
    const float xs[6] = { 1.0f, 2.0f, 9.0f, 3.0f, 8.0f, 4.0f };
    CTargetPoint pts[6];
    for (int i = 0; i < 6; ++i) { pts[i].PThing = &things[i]; pts[i].Index = i; pts[i].X = xs[i]; pts[i].Y = 0.0f; pts[i].Z = 0.0f; pts[i].FrameRejected = 0; }
    CFilterInFOV fov; fov.MaxX = 5.0f;
    CNegate negate;
    CBind2 pred; pred.F1 = &fov; pred.F2 = &negate;
    // removed: in view and NOT targetable behind -> 0, 3, 5; kept: 1 (behind), 2 and 4 (out of view)
    CTargetPoint* end = RemoveCopyIf(pts, pts + 6, pts, pred);
    bool ok = end == pts + 3 && pts[0].Index == 1 && pts[1].Index == 2 && pts[2].Index == 4;
    ok = ok && pts[1].PThing == &things[2] && pts[2].X == 8.0f;
    if (!ok) { std::printf("0080a850_TEST FAIL\n"); return 1; }
    std::printf("0080a850_TEST PASS\n");
    return 0;
}