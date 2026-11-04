// CParticleProgram::GetUsedBones(CArray<bool>& used, const CArray<C3DGroup2>& groups) -- private static helper: flag every bone that a group with at least one vertex blend is skinned to
// Leaf: ?GetUsedBones@CParticleProgram@@CIXAAV?$CArray@_N@@ABV?$CArray@VC3DGroup2@@@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE1210, 79 bytes, static __fastcall (used in ecx, groups in edx), plain `ret`.
// Identity: Ego_r.pdb `?GetUsedBones@CParticleProgram@@CIXAAV?$CArray@_N@@ABV?$CArray@VC3DGroup2@@@@@Z` (private static
//   __fastcall); its only callers are the public CParticleProgram::GetUsedBones(CArray<bool>&) const @ 0x00AE15D0 (landed,
//   which already carries this body as its same-TU stand-in), once for SimGroups and once for NonSimGroups.
// Layout: retail CArray<T> is an STLport vector of three pointers (12 bytes); C3DGroup2 is {long BoneIndex,
//   CArray<C3DVertexBlend2> VertexBlends} = 16 bytes; C3DVertexBlend2 8 bytes.
// Shape: `for (i = 0; i < groups.size(); ++i) if ((long)groups[i].VertexBlends.size() > 0) used[groups[i].BoneIndex] = true;`
//   -- the signed blend-count test folds to `test ebx,0FFFFFFF8h ; jle`, and the group array (and its size) is re-read
//   after every byte store (a bool store may alias the CArray's pointers).
#include "engine/CArray.h"
class C3DVertexBlend2 { public: long Vertex; float Weight; };   // 8 bytes
class C3DGroup2 {                                      // 16 bytes in retail
public:
    long                     BoneIndex;                // +0x0
    CArray<C3DVertexBlend2>  VertexBlends;             // +0x4
};
class CParticleProgram {
public:
    void GetUsedBones(CArray<bool>& used) const;       // 0x00AE15D0
private:
    static void __fastcall GetUsedBones(CArray<bool>& used, const CArray<C3DGroup2>& groups);
};

void __fastcall CParticleProgram::GetUsedBones(CArray<bool>& used, const CArray<C3DGroup2>& groups)
{
    for (unsigned int i = 0; i < groups.size(); ++i)
        if ((long)groups[i].VertexBlends.size() > 0)
            used[groups[i].BoneIndex] = true;
}