// Behaviour test for 0x008060C0 STLport __find_if<CTargetPoint*, CThingFilter_IsInFOV>: first hit or last, all trip/remainder paths.
#include <cstdio>
struct CTargetPoint { int Thing; int Index; float X, Y, Z; int FrameRejected; };
struct CFilterInFOV {                                   // stand-in: "in view" when X lies in [MinX, MaxX]
    float MinX, MaxX; bool Inclusive;
    bool operator()(const CTargetPoint& p) const { return Inclusive ? (p.X >= MinX && p.X <= MaxX) : (p.X > MinX && p.X < MaxX); }
};
struct Tag { };
static CTargetPoint* FindIfImpl(CTargetPoint* first, CTargetPoint* last, CFilterInFOV pred, const Tag&)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3: if (pred(*first)) return first; ++first;
    case 2: if (pred(*first)) return first; ++first;
    case 1: if (pred(*first)) return first;
    default: return last;
    }
}
static CTargetPoint* FindIf(CTargetPoint* first, CTargetPoint* last, CFilterInFOV pred) { return FindIfImpl(first, last, pred, Tag()); }
static CTargetPoint* Naive(CTargetPoint* first, CTargetPoint* last, const CFilterInFOV& pred)
{
    for (; first != last; ++first) if (pred(*first)) return first;
    return last;
}
int main()
{
    CTargetPoint pts[11];
    for (int i = 0; i < 11; ++i) { pts[i].Thing = i; pts[i].Index = i; pts[i].X = (float)i; pts[i].Y = 0.0f; pts[i].Z = 0.0f; pts[i].FrameRejected = 0; }
    bool ok = true;
    for (int n = 0; n <= 11 && ok; ++n)                         // every range length (trip count and 0..3 remainder)
        for (int hit = -1; hit < 11 && ok; ++hit) {             // hit == -1: nothing in view
            CFilterInFOV pred;
            pred.MinX = (float)hit; pred.MaxX = hit < 0 ? -100.0f : 100.0f; pred.Inclusive = true;
            CTargetPoint* first = pts; CTargetPoint* last = pts + n;
            ok = FindIfImpl(first, last, pred, Tag()) == Naive(first, last, pred);
        }
    if (!ok) { std::printf("008060c0_TEST FAIL\n"); return 1; }
    std::printf("008060c0_TEST PASS\n");
    return 0;
}