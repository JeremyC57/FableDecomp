// C3DPrimitive2::AddMirroredTextureVertexIndex(const unsigned long* indices, unsigned long count) -- append the given vertex indices to the primitive's mirrored-texture index list
// Leaf: ?AddMirroredTextureVertexIndex@C3DPrimitive2@@AAEXPBKK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A90510, 75 bytes, `ret 8`. Ego_r.pdb
//   `?AddMirroredTextureVertexIndex@C3DPrimitive2@@AAEXPBKK@Z` (private); its caller 0x00A92476 (the C3DPrimitive2
//   loader) passes CMirroredTextureHighlightsChunk::GetList() (0x00AC4F30) and GetCount() (0x00AC4F20).
// Callee: std::vector<unsigned long>::_M_insert_overflow(unsigned long*, const unsigned long&, const __true_type&,
//   unsigned int, bool) @ 0x00464ACE (the binding of the landed callers' std spelling).
// Layout: Ego_r.pdb C3DPrimitive2 with retail 12-byte CArrays (ClothPrimitives +0x20, PartitionedMeshSourceIndexBuffer
//   +0x80): MirroredTextureVertexIndexList at +0x94 (Ego_r +156). engine has no C3DPrimitive2 header.
// Shape: STLport 4.6 push_back inline (placement copy with its null test, else the overflow with a __true_type tag homed in
//   the dead `count` slot); the indexed loop becomes a down-counter (ebx) over a stepping pointer (edi).

#include "engine/CArray.h"


class C3DPrimitive2 {
public:
    unsigned char         _pad00[0x94];                 // vptr (CMovableResource), MaterialIndex .. PartitionTree
    CArray<unsigned long> MirroredTextureVertexIndexList;   // +0x94
private:
    void AddMirroredTextureVertexIndex(const unsigned long* indices, unsigned long count);
};

void C3DPrimitive2::AddMirroredTextureVertexIndex(const unsigned long* indices, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        MirroredTextureVertexIndexList.push_back(indices[i]);
}