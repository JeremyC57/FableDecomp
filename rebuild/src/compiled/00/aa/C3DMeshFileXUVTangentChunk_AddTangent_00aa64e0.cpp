// C3DMeshFileXUVTangentChunk::AddTangent(const C3DVector& uTangent, const C3DVector& vTangent) -- append one U and one V tangent to the mesh file's UV-tangent chunk arrays
// Leaf: ?AddTangent@C3DMeshFileXUVTangentChunk@@QAEXABVC3DVector@@0@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA64E0, 137 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb `?AddTangent@C3DMeshFileXUVTangentChunk@@QAEXABVC3DVector@@0@Z`; no direct caller in retail (an
//   unreferenced out-of-line copy next to the chunk's other methods 0x00AA57F0/0x00AA6A60). The two pushes go to the
//   12-byte-element arrays at +0x10 and +0x1C, which the landed ReadFromFile 0x00AA6A60 reads as UTangents/VTangents.
//   The catalog's "Vector_PushBack_3Floats_Pair" is descriptive.
// Callees: vector<C3DVector>::_M_insert_overflow(pos, x, tag, 1, true) 0x0066E370 (shared by both pushes).
// Layout: retail chunk = CChunkedFileChunk {vptr, ChunkType[5] +4, Parent +0xC}, UTangents +0x10, VTangents +0x1C as
//   12-byte STLport vectors (engine/C3DMeshFileXUVTangentChunk.h carries the 16-byte Ego_r arrays at +0x10/+0x20, so the
//   class is modelled locally); C3DVector is 3 floats.
// Shape: two inline STLport push_backs: `lea ecx,[esi+member]`, the placement copy (three dword moves) under its null
//   test and `add [ecx+4],0Ch`, or the overflow call with the empty tag homed in the first argument's slot.

class C3DVector {
public:
    float X, Y, Z;
};

#include "engine/CArray.h"


class CChunkedFileSuperChunk;

class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();
    char                    ChunkType[5];               // +0x04
    CChunkedFileSuperChunk* Parent;                     // +0x0c
};

class C3DMeshFileXUVTangentChunk : public CChunkedFileChunk {
public:
    CArray<C3DVector> UTangents;                        // +0x10
    CArray<C3DVector> VTangents;                        // +0x1c
    void AddTangent(const C3DVector& uTangent, const C3DVector& vTangent);
};

void C3DMeshFileXUVTangentChunk::AddTangent(const C3DVector& uTangent, const C3DVector& vTangent)
{
    UTangents.push_back(uTangent);
    VTangents.push_back(vTangent);
}