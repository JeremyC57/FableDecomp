// NShape::CBasicShape::CBasicShape(long id, const CPointList& points) -- CShape base (id, copied point list, empty list) then this class's empty subdivision list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00762440, length 61 bytes (+ int3 padding).
// Callees: 0x006F0810 copy constructor of a 12-byte retail vector (the CShape point list at +0x08).
// Vtables: 0x0126A208 NShape::CShape (base, stored first), 0x0126A31C NShape::CBasicShape.
// Shape: the inlined CShape constructor stores its vtable, the id at +0x04, copy-constructs the
// point list at +0x08 out of line and zeroes its own 12-byte vector at +0x14; then the derived
// vtable is stored and the subdivision vector at +0x20 zeroed. The vtable store sits ABOVE the
// zero stores because each zeroed block is a member object with an inline constructor.
template <class T>
struct CRetailVector {                          // VC6-era Dinkumware vector: first / last / end
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
    CRetailVector(const CRetailVector& other);  // 0x006F0810 (out of line)
};

namespace NShape {

struct C3DVector { float X, Y, Z; };
struct CSubdivisionTriangle;
template <class T> struct CCountedPointer { T* m_pObject; void* m_pCount; };

struct CShape {                                 // vtable 0x0126A208
    virtual void v00();
    long m_id;                                  // +0x04
    CRetailVector<C3DVector> m_points;          // +0x08
    CRetailVector<C3DVector> m_edges;           // +0x14
    CShape(long id, const CRetailVector<C3DVector>& points)
        : m_id(id), m_points(points), m_edges() { }
};

struct CBasicShape : CShape {                   // vtable 0x0126A31C
    CRetailVector< CCountedPointer<CSubdivisionTriangle> > Subdivisions;   // +0x20
    virtual void v00();
    CBasicShape(long id, const CRetailVector<C3DVector>& points);
};

CBasicShape::CBasicShape(long id, const CRetailVector<C3DVector>& points)
    : CShape(id, points), Subdivisions()
{
}

}
