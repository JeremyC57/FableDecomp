// C3DMeshFileTrianglesChunk::AddTriangle(const C3DTriangle2& triangle) -- append one 6-byte triangle (three vertex indices) to the chunk's triangle list (+0x2C)
// Leaf: ?AddTriangle@C3DMeshFileTrianglesChunk@@QAEXABVC3DTriangle2@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABCEC0, length 65 bytes (+15 int3), `ret 4`.
// Callees: std::vector<C3DTriangle2>::_M_insert_overflow(pointer, const C3DTriangle2&, const __false_type&, size_type,
//   bool) @ 0x00A71550 (the shared triangle-vector grow path).
// Identity: Ego_r.pdb `?AddTriangle@C3DMeshFileTrianglesChunk@@QAEXABVC3DTriangle2@@@Z`; the only caller is
//   COptimisedPrimitive::CopyTo (0x00ACCAB9, three uint16 indices built on its frame). The manifest's
//   "VertexData_AppendInterleaved" is descriptive.
// Layout: retail 12-byte STLport containers put TriangleList at +0x2C (the Ego_r header has +0x34), the same local
//   model as the landed GetList 0x00ABC440.
// Shape: the STLport push_back inlined on the member (`add ecx,2Ch` once, finish/end read through the owner first);
//   the POD triangle copies as a dword plus a word; the grow arm passes the tag homed on the value argument slot.

class C3DTriangle2 {
public:
    unsigned short VertexIndex[3];
};

#include "engine/CArray.h"


class C3DMeshFileTrianglesChunk {
public:
    void*                 __vftable;               // +0x00
    unsigned char         _chunkBase[0x28];        // +0x04 CChunkedFileChunk members
    CArray<C3DTriangle2>  TriangleList;            // +0x2C
    void AddTriangle(const C3DTriangle2& triangle);
};

void C3DMeshFileTrianglesChunk::AddTriangle(const C3DTriangle2& triangle)
{
    TriangleList.push_back(triangle);
}