#include <cstdio>
// Behaviour model of 0x0084F4D0: copy two vectors, empty point vector, zero trailing word.
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct SAxisPoint { C3DVector Position; float Weight; };
struct CAxisPointList {
    C3DVector Origin; C3DVector Axis; SAxisPoint* m_pFirst; SAxisPoint* m_pLast; SAxisPoint* m_pEnd; long m_count;
    CAxisPointList(const C3DVector& origin, const C3DVector& axis) : Origin(origin), Axis(axis), m_pFirst(0), m_pLast(0), m_pEnd(0), m_count(0) {}
};
int main()
{
    C3DVector o(1.0f, 2.0f, 3.0f), up(0.0f, 0.0f, 1.0f);
    CAxisPointList l(o, up);
    if (l.Origin.X != 1.0f || l.Origin.Y != 2.0f || l.Origin.Z != 3.0f || l.Axis.Z != 1.0f || l.Axis.X != 0.0f
        || l.m_pFirst != 0 || l.m_pLast != 0 || l.m_pEnd != 0 || l.m_count != 0)
        { std::printf("0084f4d0_TEST FAIL\n"); return 1; }
    std::printf("0084f4d0_TEST PASS\n");
    return 0;
}
