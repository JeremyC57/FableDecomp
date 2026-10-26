// Behaviour test for 0x008078A0 STLport remove_if<CTargetPoint*, CThingFilter_IsInFOV>: find the first point in view, compact the rest after it.
#include <cstdio>
class CThing { public: int id; };
struct CPointer {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
    void SetPItem(CThing* item) { Item = item; }
    CPointer& operator=(const CPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
struct CTargetPoint { CPointer PThing; long Index; float X, Y, Z; long FrameRejected; };
struct CFilterInFOV { float MaxX; bool operator()(const CTargetPoint& p) const { return p.X <= MaxX; } };
static CTargetPoint* FindIf(CTargetPoint* first, CTargetPoint* last, CFilterInFOV pred)
{
    for (; first != last; ++first) if (pred(*first)) return first;
    return last;
}
static CTargetPoint* RemoveCopyIf(CTargetPoint* first, CTargetPoint* last, CTargetPoint* result, CFilterInFOV pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) { *result = *first; ++result; }
    return result;
}
static CTargetPoint* RemoveIf(CTargetPoint* first, CTargetPoint* last, CFilterInFOV pred)
{
    first = FindIf(first, last, pred);
    if (first == last)
        return first;
    CTargetPoint* next = first;
    return RemoveCopyIf(++next, last, first, pred);
}
int main()
{
    CThing things[7];
    CTargetPoint pts[7];
    const float xs[7] = { 8.0f, 9.0f, 1.0f, 7.0f, 2.0f, 6.0f, 3.0f };
    for (int i = 0; i < 7; ++i) {
        things[i].id = i; pts[i].PThing.Item = &things[i]; pts[i].Index = i;
        pts[i].X = xs[i]; pts[i].Y = 0.0f; pts[i].Z = 0.0f; pts[i].FrameRejected = 0;
    }
    CFilterInFOV pred; pred.MaxX = 4.0f;                    // 2, 4, 6 are in view and removed
    CTargetPoint* end = RemoveIf(pts, pts + 7, pred);
    bool ok = end == pts + 4;
    const int keep[4] = { 0, 1, 3, 5 };
    for (int i = 0; ok && i < 4; ++i)
        ok = pts[i].PThing.GetPItem() == &things[keep[i]] && pts[i].Index == keep[i] && pts[i].X == xs[keep[i]];
    pred.MaxX = -1.0f;                                      // nothing in view: last returned, nothing moved
    ok = ok && RemoveIf(pts, pts + 4, pred) == pts + 4 && pts[3].Index == 5;
    if (!ok) { std::printf("008078a0_TEST FAIL\n"); return 1; }
    std::printf("008078a0_TEST PASS\n");
    return 0;
}