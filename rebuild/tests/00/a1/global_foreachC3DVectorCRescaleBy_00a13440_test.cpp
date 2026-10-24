// Behaviour test for for_each<C3DVector*, CRescaleBy> (0x00A13440): every component of every vector in [first, last)
// is scaled, vectors outside the range are untouched, an empty range is a no-op, and the functor comes back intact.
#include <cstdio>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};

struct CRescaleBy {
    float Scale;
    CRescaleBy(float scale) : Scale(scale) {}
    void operator()(C3DVector& v) const { v *= Scale; }
};

namespace _STL {
template <class _InputIter, class _Function>
_Function for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for ( ; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
}

static int nearf(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    int ok = 1;
    C3DVector v[4];
    for (int i = 0; i < 4; ++i) { v[i].X = 1.0f + i; v[i].Y = -2.0f * i; v[i].Z = 0.5f; }
    CRescaleBy r = _STL::for_each(v, v + 3, CRescaleBy(2.5f));
    if (!nearf(r.Scale, 2.5f)) ok = 0;
    for (int i = 0; i < 3; ++i)
        if (!nearf(v[i].X, 2.5f * (1.0f + i)) || !nearf(v[i].Y, 2.5f * (-2.0f * i)) || !nearf(v[i].Z, 1.25f)) ok = 0;
    if (!nearf(v[3].X, 4.0f) || !nearf(v[3].Y, -6.0f) || !nearf(v[3].Z, 0.5f)) ok = 0;
    CRescaleBy e = _STL::for_each(v, v, CRescaleBy(10.0f));
    if (!nearf(e.Scale, 10.0f) || !nearf(v[0].X, 2.5f)) ok = 0;

    if (!ok) { std::printf("00a13440_TEST FAIL\n"); return 1; }
    std::printf("00a13440_TEST PASS\n");
    return 0;
}