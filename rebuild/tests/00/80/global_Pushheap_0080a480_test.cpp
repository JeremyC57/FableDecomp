// behaviour test for 0x0080A480 _Push_heap<CTargetPoint, CThingCompare_BestForTargeting2>
#include <cstdio>
struct CThing { int id; };
struct ThingPointer {
    CThing* item;
    CThing* GetPItem() const { return item; }
    void SetPItem(CThing* p) { item = p; }
    ThingPointer& operator=(const ThingPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
struct C3DVector { float X, Y, Z; };
struct CTargetPoint {
    ThingPointer PThing;
    int Index;
    C3DVector TargetingPosition;
    int FrameRejected;
    CTargetPoint& operator=(const CTargetPoint& o)
    {
        PThing = o.PThing; Index = o.Index; TargetingPosition = o.TargetingPosition; FrameRejected = o.FrameRejected;
        return *this;
    }
};
// stand-in for the out-of-line comparator: prefers the point nearest the hero's X (larger distance = "less")
struct CThingCompare_BestForTargeting2 {
    float HeroX;
    float pad[12];
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const
    {
        float da = a.TargetingPosition.X - HeroX; if (da < 0) da = -da;
        float db = b.TargetingPosition.X - HeroX; if (db < 0) db = -db;
        return da > db;
    }
};
static void _Push_heap(CTargetPoint* first, long hole, long top, CTargetPoint value, CThingCompare_BestForTargeting2 comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
int main()
{
    static const float xs[6] = { 10.0f, 4.0f, 7.5f, 1.0f, 12.0f, 5.5f };
    CThing things[6];
    CTargetPoint heap[6];
    CThingCompare_BestForTargeting2 c; c.HeroX = 5.0f;
    for (int i = 0; i < 6; ++i) {
        things[i].id = i;
        CTargetPoint v;
        v.PThing.item = &things[i]; v.Index = i; v.TargetingPosition.X = xs[i]; v.TargetingPosition.Y = (float)-i; v.TargetingPosition.Z = 0.0f; v.FrameRejected = 10 * i;
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("0080a480_TEST FAIL\n"); return 1; }
    }
    // the best target is the one closest to x = 5 (5.5, pushed last: it climbs 5 -> 2 -> 0)
    if (heap[0].PThing.item != &things[5] || heap[0].Index != 5 || heap[0].TargetingPosition.Y != -5.0f || heap[0].FrameRejected != 50 || heap[2].Index != 1 || heap[5].Index != 2) { std::printf("0080a480_TEST FAIL\n"); return 1; }
    std::printf("0080a480_TEST PASS\n");
    return 0;
}