// behaviour test for 0x008066a0 std::__find<NPlayerTargeting::CTargetPoint*, CTargetPoint> (equal = same thing and same index): STLport random-access find -- four tests per pass over (last - first) >> 2
// passes, then the 3/2/1 remainder; returns the first hit or last.
#include <cstdio>
#include <cmath>
struct CTargetPoint { void* thing; long Index; float pos[3]; long FrameRejected;
    bool operator==(const CTargetPoint& o) const { return thing == o.thing && Index == o.Index; } };
struct Eq { const CTargetPoint* v; bool operator()(CTargetPoint& p) const { return p == *v; } };
template <class It, class P> static It find_body(It first, It last, P hit)
{
    int trip = (last - first) >> 2;
    for (; trip > 0; --trip) {
        if (hit(*first)) return first; ++first;
        if (hit(*first)) return first; ++first;
        if (hit(*first)) return first; ++first;
        if (hit(*first)) return first; ++first;
    }
    switch (last - first) {
    case 3: if (hit(*first)) return first; ++first;
    case 2: if (hit(*first)) return first; ++first;
    case 1: if (hit(*first)) return first;
    default: return last;
    }
}
int main()
{
    bool ok = true;
    int things[3]; CTargetPoint pts[11];
    for (int i = 0; i < 11; ++i) { pts[i].thing = &things[i % 3]; pts[i].Index = i; pts[i].FrameRejected = 0; }
    for (int n = 0; n <= 11; ++n)
        for (int k = 0; k < 11; ++k) {
            CTargetPoint key = pts[k]; Eq e; e.v = &key;
            CTargetPoint* r = find_body(pts, pts + n, e);
            if (r != (k < n ? pts + k : pts + n)) ok = false;
        }
    { CTargetPoint key = pts[4]; key.thing = &things[2]; Eq e; e.v = &key;   // same index, other thing: no hit
      if (find_body(pts, pts + 11, e) != pts + 11) ok = false; }
    if (!ok) { std::printf("008066a0_TEST FAIL\n"); return 1; }
    std::printf("008066a0_TEST PASS\n");
    return 0;
}