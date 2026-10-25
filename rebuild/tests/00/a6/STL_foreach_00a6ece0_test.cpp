// Behaviour test for 0x00A6ECE0: std::for_each with CAddElements<C3DVector>
#include <cstdio>
#include <cstdlib>
#include <new>

class C3DVector {
public:
    float x, y, z;
};

template <class T> class CAddElements {
public:
    T* Sum;
    CAddElements(T& sum) : Sum(&sum) { }
    void operator()(const T& value)
    {
        Sum->x += value.x;
        Sum->y += value.y;
        Sum->z += value.z;
    }
};

namespace std {
template <class _InputIter, class _Function>
_Function for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for (; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
}

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    int ok = 1;
    C3DVector v[3];
    for (int i = 0; i < 3; ++i) {
        v[i].x = 1.0f + i;
        v[i].y = -2.0f * i;
        v[i].z = 0.5f;
    }
    C3DVector sum;
    sum.x = 10.0f;
    sum.y = 0.0f;
    sum.z = 0.0f;
    CAddElements<C3DVector> f = std::for_each(v, v + 3, CAddElements<C3DVector>(sum));
    if (f.Sum != &sum || !Near(sum.x, 16.0f) || !Near(sum.y, -6.0f) || !Near(sum.z, 1.5f))
        ok = 0;
    std::for_each(v, v, CAddElements<C3DVector>(sum));
    if (!Near(sum.x, 16.0f))
        ok = 0;

    if (!ok) {
        std::printf("00a6ece0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a6ece0_TEST PASS\n");
    return 0;
}