// N3DPrimitive::CDeviceIndependent::GetTotalTriangleCount() -- sum of the triangle counts of every triangle list of the primitive
// Leaf: ?GetTotalTriangleCount@CDeviceIndependent@N3DPrimitive@@QBEKXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8ECE0, 80 bytes. Ego_r.pdb `?GetTotalTriangleCount@CDeviceIndependent@N3DPrimitive@@QBEKXZ`
//   (donor 0x00BD1520, directly before GetTotalIndexCount 0x00BD1560 -- retail 0x00A8ECE0 / 0x00A8ED30). Its caller
//   0x00A92218 (the "CLTH" cloth-primitive loader 0x00A91640) stores the result and then calls GetTotalIndexCount.
// Layout: Ego_r.pdb N3DPrimitive::CDeviceIndependent with retail 12-byte CArrays: vptr +0, Vertices +4, TriangleLists +0x10
//   (CArray<CIVCountedPointer<CTriangleList> >); CTriangleList: CIVCountedPointee vptr/count, Triangles +8
//   (CArray<C3DTriangle2>, 6-byte triangles: the `imul 2AAAAAABh` /6), StripIndices +0x14 (CArray<unsigned short>).
// Shape: a plain indexed loop over size(); VC hoists the bound into ebx and strength-reduces the element pointer (esi);
//   `add eax,8` is the dead `&list->Triangles` of the inlined size().
template <class T> class CIVCountedPointer {            // retail 4 bytes
public:
    T* Object;                                          // +0x00
    T* operator->() const { return Object; }
};

#include "engine/CArray.h"


class C3DTriangle2 { public: unsigned short Index[3]; };   // 6 bytes
class C3DVertex2;
class C3DGroup2;

namespace N3DPrimitive {

class CTriangleList {                                   // CIVCountedPointee: vptr +0, count +4
public:
    virtual ~CTriangleList();
    unsigned long             RefCount;                 // +0x04
    CArray<C3DTriangle2>      Triangles;                // +0x08
    CArray<unsigned short>    StripIndices;             // +0x14
};

class CDeviceIndependent {
public:
    virtual ~CDeviceIndependent();
    CArray<C3DVertex2>                          Vertices;        // +0x04
    CArray<CIVCountedPointer<CTriangleList> >   TriangleLists;   // +0x10
    unsigned long GetTotalTriangleCount() const;
    unsigned long GetTotalIndexCount() const;
};

}   // namespace N3DPrimitive

unsigned long N3DPrimitive::CDeviceIndependent::GetTotalTriangleCount() const
{
    unsigned long total = 0;
    for (unsigned long i = 0; i < TriangleLists.size(); ++i)
        total += TriangleLists[i]->Triangles.size();
    return total;
}