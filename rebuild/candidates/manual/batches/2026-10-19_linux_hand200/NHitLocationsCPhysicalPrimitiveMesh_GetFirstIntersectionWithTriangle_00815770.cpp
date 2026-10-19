// NHitLocations::CPhysicalPrimitiveMesh::GetFirstIntersectionWithTriangle(CThing* thing, const std::vector<C3DVector>& triangle, C3DVector& position, C3DVector& normal, float radius) -- sweep the triangle's three edges through the line test and report the first hit
// Leaf: GetFirstIntersectionWithTriangle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00815770, 174 bytes, `ret 14h`; vtable 0x0125B9B0 slot 8.
// Callees: the primitive's own virtual GetFirstIntersectionWithLine (slot 7, +0x1C, 0x00815070 for the mesh).
// Shape: the edges are taken as (v[(i+2)%3], v[(i+3)%3]) for i = 0..2 -- (2,0), (0,1), (1,2) -- VC strength-reduces the
//   index to i+2 in esi (`lea edi,[esi+1]`, two `idiv` by 3, exit test `lea eax,[esi-2] ; cmp eax,3 ; jne`: the loop is written `i != 3`, `i < 3` gives `jl`); the vector's
//   start and the vtable are re-read every pass and `this` is spilled across the call. The hit position and normal
//   are frame C3DVectors copied out dword by dword only on a hit. CPhysicalPrimitiveMesh is the 4-byte polymorphic
//   base only (Ego_r.pdb).
struct CThing;
struct C3DVector { float X, Y, Z; };
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](long index) const { return _M_start[index]; }
};

namespace NHitLocations {
class CPhysicalPrimitiveMesh {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual bool GetFirstIntersectionWithLine(CThing* thing, const C3DVector& start, const C3DVector& end,
                                              C3DVector& position, C3DVector& normal, float radius) const;   // slot 7 (+0x1C)
    virtual bool GetFirstIntersectionWithTriangle(CThing* thing, const vector<C3DVector>& triangle,
                                                  C3DVector& position, C3DVector& normal, float radius) const;   // slot 8
};

bool CPhysicalPrimitiveMesh::GetFirstIntersectionWithTriangle(CThing* thing, const vector<C3DVector>& triangle,
                                                              C3DVector& position, C3DVector& normal, float radius) const
{
    C3DVector hitPosition;
    C3DVector hitNormal;
    for (long i = 0; i != 3; ++i) {
        if (GetFirstIntersectionWithLine(thing, triangle[(i + 2) % 3], triangle[(i + 3) % 3], hitPosition, hitNormal, radius)) {
            position = hitPosition;
            normal = hitNormal;
            return true;
        }
    }
    return false;
}
}
