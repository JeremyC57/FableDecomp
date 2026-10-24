// Behaviour test for 0x00806CF0 STLport remove_copy_if<CTargetPoint*, CTargetPoint*, CThingFilter_IsInFOV>: keep the points the filter rejects, in order.
#include <cstdio>
struct CThing { int id; };
struct CPointer {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
    void SetPItem(CThing* item) { Item = item; }
    CPointer& operator=(const CPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
struct C3DVector { float X, Y, Z; };
struct CTargetPoint { CPointer PThing; long Index; C3DVector TargetingPosition; long FrameRejected; };
struct CFilterInFOV { float MaxX; bool operator()(const CTargetPoint& p) const { return p.TargetingPosition.X <= MaxX; } };
static CTargetPoint* RemoveCopyIf(CTargetPoint* first, CTargetPoint* last, CTargetPoint* result, CFilterInFOV pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
    return result;
}
int main()
{
    CThing things[6];
    CTargetPoint pts[6];
    const float xs[6] = { 5.0f, 1.0f, 7.0f, 2.0f, 9.0f, 3.0f };
    for (int i = 0; i < 6; ++i) {
        things[i].id = i;
        pts[i].PThing.Item = &things[i]; pts[i].Index = 10 + i;
        pts[i].TargetingPosition.X = xs[i]; pts[i].TargetingPosition.Y = (float)i; pts[i].TargetingPosition.Z = 0.0f;
        pts[i].FrameRejected = 100 + i;
    }
    CFilterInFOV pred; pred.MaxX = 4.0f;                  // 1, 2, 3 are "in view" and dropped
    CTargetPoint* end = RemoveCopyIf(pts, pts + 6, pts, pred);   // in place, as remove_if uses it
    bool ok = end == pts + 3;
    const int keep[3] = { 0, 2, 4 };
    for (int i = 0; ok && i < 3; ++i)
        ok = pts[i].PThing.GetPItem() == &things[keep[i]] && pts[i].Index == 10 + keep[i]
          && pts[i].TargetingPosition.X == xs[keep[i]] && pts[i].TargetingPosition.Y == (float)keep[i]
          && pts[i].FrameRejected == 100 + keep[i];
    ok = ok && RemoveCopyIf(pts, pts, pts, pred) == pts;  // empty range
    if (!ok) { std::printf("00806cf0_TEST FAIL\n"); return 1; }
    std::printf("00806cf0_TEST PASS\n");
    return 0;
}