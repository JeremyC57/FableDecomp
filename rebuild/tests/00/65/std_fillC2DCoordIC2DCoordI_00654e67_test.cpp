// Behaviour test for 0x00654E67 std::fill<C2DCoordI*,C2DCoordI> (model copy)
#include <cstdio>
class C2DCoordI { public: long X; long Y; };
template <class It, class T> void fill_model(It first, It last, const T& v) { for (; first != last; ++first) *first = v; }
int main()
{
    C2DCoordI a[5];
    for (int i = 0; i < 5; ++i) { a[i].X = i; a[i].Y = -i; }
    C2DCoordI v; v.X = 77; v.Y = 88;
    fill_model(a + 1, a + 4, v);
    if (a[0].X != 0 || a[4].X != 4 || a[4].Y != -4) { std::printf("00654e67_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 4; ++i) if (a[i].X != 77 || a[i].Y != 88) { std::printf("00654e67_TEST FAIL\n"); return 1; }
    fill_model(a, a, v);              // empty range touches nothing
    if (a[0].X != 0) { std::printf("00654e67_TEST FAIL\n"); return 1; }
    std::printf("00654e67_TEST PASS\n");
    return 0;
}