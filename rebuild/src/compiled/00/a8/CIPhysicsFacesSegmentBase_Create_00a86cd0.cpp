// CIPhysicsFacesSegmentBase::Create(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices) -- builds the root of a mesh's face segmentation tree: index every triangle, then either split the set by a dividing plane into a branch or keep it whole in a leaf
// Leaf: ?Create@CIPhysicsFacesSegmentBase@@SIPBV1@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@VC3DVector@@@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A86CD0, 331 bytes, static `__fastcall` (triangles in ecx, vertices in edx),
//   `ret`, returns the new segment.
// Identity (proven): Ego_r.pdb ?Create@CIPhysicsFacesSegmentBase@@SIPBV1@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@VC3DVector@@@@@Z
//   (the only static of the class with that signature; the donor order is Calculate, ..., the two constructors, Create).
//   Callers: CPhysicsMesh::CalculateSegmentation 0x00A6C850 and the CPhysicsMesh constructor 0x00A6DA40 (RootSegment).
//   Its callees are CIPhysicsFacesSegmentBase::Calculate 0x00A868C0, the CPhysicsFacesSegmentBranch constructor
//   0x00A86B30 (landed, PDB spelling) and the inlined CPhysicsFacesSegmentLeaf constructor (vtable 0x0129DEA4,
//   .?AVCPhysicsFacesSegmentLeaf@@). The catalog's bare "Create" had no class.
// Callees: vector<unsigned short>::_M_insert_overflow(pos, x, const __true_type&, 1, true) 0x005135D0 (push_back),
//   Calculate 0x00A868C0 (protected static __fastcall, Ego_r.pdb spelling KI... with references; the landed binding is an
//   invented CPhysicsFacesSegmentBranch / pointer spelling -- NAME_FIXES), operator new 0x00BFEA1A, the branch
//   constructor 0x00A86B30, vector<unsigned short>::vector(const vector&) 0x00A87090 (the leaf's member copy), free 0x00BFEA14.
// Shape: STLport push_back inline in the index loop (finish cached in ebx, the converted index temporary in a frame word;
//   same-TU noinline bodies of _M_insert_overflow, the vector copy constructor and a Calculate STAND-IN keep the face
//   array from escaping, else VC reloads finish after the element store);
//   one result variable so the three CArray destructors (inline frees) are shared by both arms.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A

#include "engine/CArray.h"


class C3DVector { public: float X, Y, Z; };
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
class CPlane {
public:
    C3DVector Normal;                                      // +0x00
    float Distance;                                        // +0x0C
};

class CIPhysicsFacesSegmentBase {
public:
    virtual ~CIPhysicsFacesSegmentBase();
    static const CIPhysicsFacesSegmentBase* __fastcall Create(const CArray<C3DTriangle2>& triangles,
                                                             const CArray<C3DVector>& vertices);
protected:
    static inline __declspec(noinline) bool __fastcall Calculate(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices,
                                     const CArray<unsigned short>& faces, long depth, CPlane& plane,
                                     CArray<unsigned short>& positive, CArray<unsigned short>& negative)   // 0x00A868C0
    {   // same-TU stand-in (NOT the retail body): lets VC see that the face array's address does not escape
        return faces.size() > (unsigned int)depth;
    }
};
class CPhysicsFacesSegmentLeaf : public CIPhysicsFacesSegmentBase {      // vtable 0x0129DEA4, 0x10 bytes
public:
    CPhysicsFacesSegmentLeaf(const CArray<unsigned short>& faces) : TriangleListIndex(faces) { }
protected:
    virtual ~CPhysicsFacesSegmentLeaf();
    CArray<unsigned short> TriangleListIndex;              // +0x04
};
class CPhysicsFacesSegmentBranch : public CIPhysicsFacesSegmentBase {    // vtable 0x0129DE88, 0x1C bytes
public:
    CPhysicsFacesSegmentBranch(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices,
                               const CPlane& plane, const CArray<unsigned short>& positive,
                               const CArray<unsigned short>& negative, long depth);   // 0x00A86B30
protected:
    virtual ~CPhysicsFacesSegmentBranch();
    CPlane ChildDividingPlane;                             // +0x04
    CIPhysicsFacesSegmentBase* PositiveChild;              // +0x14
    CIPhysicsFacesSegmentBase* NegativeChild;              // +0x18
};

const CIPhysicsFacesSegmentBase* __fastcall CIPhysicsFacesSegmentBase::Create(const CArray<C3DTriangle2>& triangles,
                                                                            const CArray<C3DVector>& vertices)
{
    CArray<unsigned short> faces;
    for (unsigned long i = 0; i < triangles.size(); ++i)
        faces.push_back((unsigned short)i);
    CArray<unsigned short> positive;
    CArray<unsigned short> negative;
    CPlane plane;
    const CIPhysicsFacesSegmentBase* result;
    if (Calculate(triangles, vertices, faces, 0, plane, positive, negative))
        result = new CPhysicsFacesSegmentBranch(triangles, vertices, plane, positive, negative, 0);
    else
        result = new CPhysicsFacesSegmentLeaf(faces);
    return result;
}