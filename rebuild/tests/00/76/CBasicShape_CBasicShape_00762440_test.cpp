#include <new>
#include <stdio.h>
template <class T> struct CRetailVector {
    T* first; T* last; T* end;
    CRetailVector() : first(0), last(0), end(0) { }
    CRetailVector(const CRetailVector& other) : first(other.first), last(other.last), end(other.end) { }
};
namespace NShape {
struct C3DVector { float X, Y, Z; };
class CSubdivisionTriangle;
template <class T> struct CCountedPointer { T* m_pObject; void* m_pCount; };
class CShape { public:
    virtual void v00() {}
    long m_id; CRetailVector<C3DVector> m_points; CRetailVector<C3DVector> m_edges;
    CShape(long id, const CRetailVector<C3DVector>& points) : m_id(id), m_points(points), m_edges() { }
};
struct CBasicShape : CShape {
    CRetailVector< CCountedPointer<CSubdivisionTriangle> > Subdivisions;
    virtual void v00() {}
    CBasicShape(long id, const CRetailVector<C3DVector>& points);
};
CBasicShape::CBasicShape(long id, const CRetailVector<C3DVector>& points) : CShape(id, points), Subdivisions() { }
}
int main()
{
    NShape::C3DVector pts[2]; CRetailVector<NShape::C3DVector> src; src.first = pts; src.last = pts + 2; src.end = pts + 2;
    unsigned char raw[sizeof(NShape::CBasicShape)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    NShape::CBasicShape* s = new (raw) NShape::CBasicShape(42, src);
    bool ok = s->m_id == 42 && s->m_points.first == pts && s->m_points.last == pts + 2 && s->m_edges.first == 0 && s->m_edges.end == 0
        && s->Subdivisions.first == 0 && s->Subdivisions.last == 0 && s->Subdivisions.end == 0;
    if (!ok) { printf("00762440_TEST FAIL\n"); return 1; }
    printf("00762440_TEST PASS\n"); return 0;
}