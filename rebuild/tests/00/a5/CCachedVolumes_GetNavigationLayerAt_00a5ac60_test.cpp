#include <cstdio>
// Behaviour test for CCachedVolumes::GetNavigationLayerAt (0x00A5AC60).
struct Vec { float X, Y, Z; };
struct Volume {
    float MinX, MaxX;
    bool IsContainingPoint(const Vec& p) const { return p.X >= MinX && p.X <= MaxX; }
};
struct Entry { long first; Volume* second; };
struct Cached {
    Entry* start; Entry* finish;
    long GetNavigationLayerAt(const Vec& p) const
    {
        for (const Entry* it = start; it != finish; ++it) {
            long layer = it->first;
            if (it->second->IsContainingPoint(p)) return layer;
        }
        return 0;
    }
};
int main()
{
    Volume a = { 0, 10 }, b = { 20, 30 }, c = { 5, 25 };
    Entry e[3] = { { 3, &a }, { 7, &b }, { 9, &c } };
    Cached cv = { e, e + 3 };
    Vec p1 = { 4, 0, 0 }, p2 = { 22, 0, 0 }, p3 = { 15, 0, 0 }, p4 = { 40, 0, 0 };
    bool ok = cv.GetNavigationLayerAt(p1) == 3 && cv.GetNavigationLayerAt(p2) == 7 && cv.GetNavigationLayerAt(p3) == 9
           && cv.GetNavigationLayerAt(p4) == 0;
    Cached empty = { e, e };
    ok = ok && empty.GetNavigationLayerAt(p1) == 0;
    std::printf(ok ? "00a5ac60_TEST PASS\n" : "00a5ac60_TEST FAIL\n");
    return ok ? 0 : 1;
}