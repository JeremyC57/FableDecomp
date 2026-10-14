// N3DPrimitive::CDeviceIndependent::CDeviceIndependent() -- CBase, own vtable, five empty arrays and a zeroed bounding sphere
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98100, length 77 bytes.
// Callees: CBase::CBase() @ 0x0099A2F0. Vtable 0x0129DFA8 = N3DPrimitive::CDeviceIndependent (rtti-vtables.tsv).
// PDB CDeviceIndependent (100 bytes): Vertices +4, TriangleLists +20, Groups +36, BoundingSphere +52, UTangents +68,
// VTangents +84 with 16-byte CArrays; retail CArray is 12 bytes so the members sit at +4, +0x10, +0x1c, +0x28 (sphere,
// 16 bytes), +0x38, +0x44 and the object is 0x50 bytes. Shape: the vtable store precedes the nineteen zero stores
// because every zeroed word belongs to an embedded member with an inline constructor; the sphere's four words are
// zeroed through the same integer zero register, so they are modelled as dwords.
struct CBase {
    virtual void v00();
    CBase();                           // 0x0099A2F0
};

template <class T>
struct CArray {                        // retail CArray<T>: begin / end / capacity
    T* m_begin;
    T* m_end;
    T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) { }
};

struct C3DBoundingSphere {             // centre + radius, zeroed as dwords
    long m_x, m_y, m_z, m_radius;
    C3DBoundingSphere() : m_x(0), m_y(0), m_z(0), m_radius(0) { }
};

struct C3DVertex2; struct C3DVector; struct CTriangleListRef; struct CGroupRef;

namespace N3DPrimitive {

struct CDeviceIndependent : CBase {    // vtable 0x0129DFA8
    virtual void v00();
    CArray<C3DVertex2>       Vertices;        // +0x04
    CArray<CTriangleListRef> TriangleLists;   // +0x10
    CArray<CGroupRef>        Groups;          // +0x1c
    C3DBoundingSphere        BoundingSphere;  // +0x28
    CArray<C3DVector>        UTangents;       // +0x38
    CArray<C3DVector>        VTangents;       // +0x44
    CDeviceIndependent();
};

CDeviceIndependent::CDeviceIndependent()
{
}

}
