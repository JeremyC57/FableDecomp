// Behaviour test for 00a5c510 (std::fill_n<C4DVector*, unsigned int, C4DVector>): n vectors get the value member by member,
// the returned iterator is first + n, n == 0 writes nothing, the neighbours are untouched.
#include <cstdio>
class C4DVector { public:
    float X, Y, Z, W;
};
template <class _OutputIter, class _Size, class _Tp>
_OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}
#define FAIL() { std::printf("00a5c510_TEST FAIL\n"); return 1; }
int main()
{
    C4DVector buf[6];
    for (int i = 0; i < 6; ++i) { buf[i].X = (float)i; buf[i].Y = buf[i].Z = buf[i].W = -1.0f; }
    C4DVector v;
    v.X = 1.5f; v.Y = 2.5f; v.Z = -3.0f; v.W = 1.0f;
    if (fill_n(buf + 1, 0u, v) != buf + 1 || buf[1].X != 1.0f) FAIL();
    C4DVector* r = fill_n(buf + 1, 4u, v);
    if (r != buf + 5) FAIL();
    for (int i = 1; i < 5; ++i)
        if (buf[i].X != 1.5f || buf[i].Y != 2.5f || buf[i].Z != -3.0f || buf[i].W != 1.0f) FAIL();
    if (buf[0].X != 0.0f || buf[5].X != 5.0f) FAIL();
    std::printf("00a5c510_TEST PASS\n");
    return 0;
}