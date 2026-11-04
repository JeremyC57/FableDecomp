// N3DPrimitive::CDeviceIndependent::GetTotalIndexCount() -- index count of the primitive: each triangle list contributes its strip indices, or three indices per triangle when it has no strip
// Leaf: ?GetTotalIndexCount@CDeviceIndependent@N3DPrimitive@@QBEKXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8ED30, 111 bytes. Ego_r.pdb `?GetTotalIndexCount@CDeviceIndependent@N3DPrimitive@@QBEKXZ`
//   (donor 0x00BD1560, directly after GetTotalTriangleCount 0x00BD1520 = retail 0x00A8ECE0); called at 0x00A92222 right
//   after GetTotalTriangleCount on the same primitive.
// Layout: Ego_r.pdb N3DPrimitive::CDeviceIndependent with retail 12-byte CArrays: TriangleLists +0x10; CTriangleList:
//   Triangles +8 (6-byte C3DTriangle2), StripIndices +0x14 (unsigned short). `test edx,0FFFFFFFEh` is `size() != 0`
//   on the 2-byte strip; `lea edx,[eax+esi] ; lea esi,[edx+eax*2]` adds three per triangle.
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

unsigned long N3DPrimitive::CDeviceIndependent::GetTotalIndexCount() const
{
    unsigned long total = 0;
    for (unsigned long i = 0; i < TriangleLists.size(); ++i) {
        const CTriangleList* list = TriangleLists[i].Object;
        if (list->StripIndices.size() != 0)
            total += list->StripIndices.size();
        else
            total += list->Triangles.size() * 3;
    }
    return total;
}