// Behaviour test for 0x00A58A60 NBBBMaths::IsPointInsideTriangleInThePlane(const vector<C2DVector>&, const C2DVector&)
#include <cstdio>

namespace _STL {
template <class T> class vector {
public:
    typedef unsigned int size_type;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T& operator[](size_type n) const { return *(begin() + n); }
};
}   // namespace _STL

class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
};

namespace NBBBMaths {
bool IsPointInsideTriangleInThePlane(const _STL::vector<C2DVector>& points, const C2DVector& pos)
{
    C2DVector edge = points[0] - points[2];
    C2DVector toPos = pos - points[2];
    float sign = edge.X * toPos.Y - edge.Y * toPos.X;
    for (long i = 0; i != 2; ++i) {
        edge = points[i + 1] - points[i];
        toPos = pos - points[i];
        float cross = edge.X * toPos.Y - edge.Y * toPos.X;
        if ((sign > 0.0f && cross < 0.0f) || (sign < 0.0f && cross > 0.0f))
            return false;
    }
    return true;
}
}   // namespace NBBBMaths

int main()
{
    C2DVector ccw[3] = { C2DVector(0.0f, 0.0f), C2DVector(4.0f, 0.0f), C2DVector(0.0f, 4.0f) };
    C2DVector cw[3] = { C2DVector(0.0f, 0.0f), C2DVector(0.0f, 4.0f), C2DVector(4.0f, 0.0f) };
    _STL::vector<C2DVector> a, b;
    a._M_start = ccw; a._M_finish = a._M_end_of_storage = ccw + 3;
    b._M_start = cw;  b._M_finish = b._M_end_of_storage = cw + 3;
    bool ok = NBBBMaths::IsPointInsideTriangleInThePlane(a, C2DVector(1.0f, 1.0f)) &&
              !NBBBMaths::IsPointInsideTriangleInThePlane(a, C2DVector(5.0f, 5.0f)) &&
              !NBBBMaths::IsPointInsideTriangleInThePlane(a, C2DVector(-1.0f, 1.0f)) &&
              NBBBMaths::IsPointInsideTriangleInThePlane(b, C2DVector(1.0f, 2.0f)) &&      // winding does not matter
              !NBBBMaths::IsPointInsideTriangleInThePlane(b, C2DVector(3.0f, 3.0f));
    if (!ok) { std::printf("00a58a60_TEST FAIL\n"); return 1; }
    std::printf("00a58a60_TEST PASS\n");
    return 0;
}