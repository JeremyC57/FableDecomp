#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CW { long v; bool operator<(const CW& o) const { return v < o.v; } }; struct SLess { SLess() { } bool operator()(const CW& a, const CW& b) const { return a < b; } };
static CW* Median(CW& a, CW& b, CW& c, SLess comp) { if (comp(a, b)) { if (comp(b, c)) return &b; else if (comp(a, c)) return &c; else return &a; } else if (comp(a, c)) return &a; else if (comp(b, c)) return &c; else return &b; }

int main() {
    CW a, b, c; a.v = 1; b.v = 2; c.v = 3; SLess comp; CW* m[6]; m[0] = Median(a, b, c, comp); m[1] = Median(a, c, b, comp); m[2] = Median(b, a, c, comp); m[3] = Median(b, c, a, comp); m[4] = Median(c, a, b, comp); m[5] = Median(c, b, a, comp); for (int i = 0; i < 6; ++i) if (m[i] != &b) { std::printf("009b2930_TEST FAIL\n"); return 1; }
    std::printf("009b2930_TEST PASS\n"); return 0;
}
