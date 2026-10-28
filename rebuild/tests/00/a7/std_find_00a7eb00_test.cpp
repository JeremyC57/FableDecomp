// Behaviour test for 00a7eb00: std::__find over const CNavigationPosition* for a C2DVector (STLport random-access 4-way unrolled find).
#include <cstdio>
class C2DVector { public: float X, Y; };
class CNavigationPosition { public:                              // compares equal to a C2DVector on its position
    C2DVector Position; int Extra;
    bool operator==(const C2DVector& v) const { return Position.X == v.X && Position.Y == v.Y; }
};
static const CNavigationPosition* find4(const CNavigationPosition* first, const CNavigationPosition* last, const C2DVector& val)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
    }
    switch (last - first) {
    case 3: if (*first == val) return first; ++first;
    case 2: if (*first == val) return first; ++first;
    case 1: if (*first == val) return first; ++first;
    case 0: default: return last;
    }
}
int main() {
    CNavigationPosition p[11];
    for (int i = 0; i < 11; ++i) { p[i].Position.X = (float)i; p[i].Position.Y = (float)(i * 2); p[i].Extra = i; }
    int fail = 0;
    for (int n = 0; n <= 11; ++n)
        for (int k = 0; k < 12; ++k) {
            C2DVector v; v.X = (float)k; v.Y = (float)(k * 2);
            const CNavigationPosition* r = find4(p, p + n, v);
            const CNavigationPosition* want = k < n ? p + k : p + n;
            if (r != want) fail = 1;
        }
    C2DVector off; off.X = 3.0f; off.Y = 5.0f;              // X matches, Y does not
    if (find4(p, p + 11, off) != p + 11) fail = 1;
    if (fail) { std::printf("00a7eb00_TEST FAIL\n"); return 1; }
    std::printf("00a7eb00_TEST PASS\n");
    return 0;
}