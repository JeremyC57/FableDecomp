#include <stdio.h>
struct C3DVector { float X, Y, Z; };
namespace NShape {
struct CSubdivisionTriangle {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual unsigned char IsPosInShape(const C3DVector& pos) { return pos.X > m_min; }
    float m_min;
};
template <class T> struct CCountedPointer { T* m_pObject; void* m_pCount; };
template <class T> struct CRetailVector { T* first; T* last; T* end; };
struct CBasicShape {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual unsigned char IsPosInShape(const C3DVector& pos);
    unsigned char _pad[0x1c];
    CRetailVector< CCountedPointer<CSubdivisionTriangle> > Subdivisions;
};
unsigned char CBasicShape::IsPosInShape(const C3DVector& pos)
{
    for (CCountedPointer<CSubdivisionTriangle>* p = Subdivisions.first; p != Subdivisions.last; ++p) {
        if (p->m_pObject->IsPosInShape(pos)) return true;
    }
    return false;
}
}
int main()
{
    NShape::CSubdivisionTriangle t0, t1, t2; t0.m_min = 10.0f; t1.m_min = 5.0f; t2.m_min = 20.0f;
    NShape::CCountedPointer<NShape::CSubdivisionTriangle> items[3];
    items[0].m_pObject = &t0; items[1].m_pObject = &t1; items[2].m_pObject = &t2;
    NShape::CBasicShape shape; shape.Subdivisions.first = items; shape.Subdivisions.last = items + 3; shape.Subdivisions.end = items + 3;
    C3DVector a = { 7.0f, 0, 0 }, b = { 1.0f, 0, 0 };
    bool ok = shape.IsPosInShape(a) == 1 && shape.IsPosInShape(b) == 0;
    shape.Subdivisions.last = items;
    ok = ok && shape.IsPosInShape(a) == 0;
    if (!ok) { printf("007611c0_TEST FAIL\n"); return 1; }
    printf("007611c0_TEST PASS\n"); return 0;
}
