// Behaviour test for 0x00A169B0: STLport random-access __find over C2DVector (componentwise 0.0001 tolerance).
#include <cstdio>
#include <math.h>

class C2DVector {
public:
    float X, Y;
    bool operator==(const C2DVector& v) const { return fabs(X - v.X) <= 0.0001f && fabs(Y - v.Y) <= 0.0001f; }
};
struct random_access_iterator_tag { };

static C2DVector* Find(C2DVector* __first, C2DVector* __last, const C2DVector& __val, const random_access_iterator_tag&)
{
    int __trip_count = (int)(__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}

int main()
{
    C2DVector v[7];
    for (int i = 0; i < 7; ++i) { v[i].X = (float)i; v[i].Y = (float)(i * 2); }
    random_access_iterator_tag tag;
    for (int i = 0; i < 7; ++i) {
        C2DVector key; key.X = (float)i + 0.00005f; key.Y = (float)(i * 2);
        for (int n = i + 1; n <= 7; ++n)                       // every tail length: unrolled body and the 3/2/1 switch
            if (Find(v, v + n, key, tag) != v + i) { std::printf("00a169b0_TEST FAIL\n"); return 1; }
        if (Find(v, v + i, key, tag) != v + i) { std::printf("00a169b0_TEST FAIL\n"); return 1; }   // not in range: last
    }
    C2DVector miss; miss.X = 3.0f; miss.Y = 6.001f;            // outside the tolerance
    if (Find(v, v + 7, miss, tag) != v + 7) { std::printf("00a169b0_TEST FAIL\n"); return 1; }
    std::printf("00a169b0_TEST PASS\n");
    return 0;
}