// C3DGroup2::AddVertexBlend(const C3DVertexBlend2& blend) -- append one {vertex index, blend value} pair to the group's blend list (+4)
// Leaf: ?AddVertexBlend@C3DGroup2@@QAEXABVC3DVertexBlend2@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC4110, length 63 bytes (+1 int3), `ret 4`.
// Callees: std::vector<C3DVertexBlend2>::_M_insert_overflow(pointer, const C3DVertexBlend2&, const __false_type&,
//   size_type, bool) @ 0x00ABEB40 (the landed C3DMeshFileGroupChunk::AddVertex calls the same grow path).
// Identity: Ego_r.pdb `?AddVertexBlend@C3DGroup2@@QAEXABVC3DVertexBlend2@@@Z`; callers 0x00AE38B7 / 0x00AE38F2 are the
//   landed CParticleProgram::AddGroup 0x00AE37D0, which already names 0x00AC4110 C3DGroup2::AddVertexBlend. The
//   manifest's "C3DGroup2_InsertVertexInline" is descriptive.
// Layout: C3DGroup2 (retail 16 bytes) {BoneIndex +0, VertexBlends +4}; C3DVertexBlend2 {VertexIndex, BlendValue}.
// Shape: STLport push_back inlined on the member (`add ecx,4`), the 8-byte record copied as two dwords, the grow arm
//   passing the tag homed on the value argument slot.

class C3DVertexBlend2 {
public:
    unsigned long VertexIndex;                     // +0
    float         BlendValue;                      // +4
};

#include "engine/CArray.h"


class C3DGroup2 {
public:
    long                    BoneIndex;             // +0
    CArray<C3DVertexBlend2> VertexBlends;          // +4
    void AddVertexBlend(const C3DVertexBlend2& blend);
};

void C3DGroup2::AddVertexBlend(const C3DVertexBlend2& blend)
{
    VertexBlends.push_back(blend);
}