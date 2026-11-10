// CParticleProgram::RemapBoneIndices(std::map<unsigned long,unsigned long>& remap, CArray<C3DGroup2>& groups) -- keep only the skinning groups whose bone is in the remap table, each with its bone index rewritten through the table, and assign the survivors back to the array
// Grade: DIFFER(270v270) -- scheduling only (4 difflines): retail loads the map header (mov ecx,[edx]) before the bone index and stores the key word [esp+18h] after the root test; ours loads the bone first and stores it before the root load (the address-taken named key orders the store ahead of the tree loads). Register colouring, frame, destructor and every call match. 5 grades; 12 compile-only probes of the key spelling (const, iterator local, end() hoisted, assignment in the condition, key declared outside the loop, key read before the group reference, temp key for find or operator[]) either keep the early store or sink it into the then-block (260 bytes).
// Leaf: ?RemapBoneIndices@CParticleProgram@@CIXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@AAV?$CArray@VC3DGroup2@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE3920, 270 bytes, private static __fastcall (remap in ecx, groups in edx), `ret`.
// Identity: Ego_r.pdb and FableWin.pdb list the pair ?RemapBoneIndices@CParticleProgram@@QAEXAAV?$map@KK...@Z (the landed 0x00AE3A30,
//   which calls this row on SimGroups and NonSimGroups) and this private static `CI` overload; the registers fit (`mov ebx,edx` =
//   groups, ecx spilled = remap) and the body walks 16-byte C3DGroup2 records (BoneIndex +0, VertexBlends +4).
// Callees: vector<C3DGroup2>::reserve 0x00AE80F0, map<unsigned long,unsigned long>::operator[] 0x00553E60,
//   C3DGroup2::SetBoneIndex 0x00AC4070, CArray<C3DGroup2>::push_back 0x00AE83D0, vector<C3DGroup2>::operator= 0x00AE7FE0, free
//   0x00BFEA14 (the local array's inline destructor: each group's blend block, then the group block).
// Graded with FABLE_HDR_OVERLAY=$A/hdr (new engine/C3DGroup2.h; CArray.h FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE; CParticleProgram.h
//   FABLE_RETAIL_CPARTICLEPROGRAM_REMAPBONEINDICES).
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_REMAPBONEINDICES
#define FABLE_RETAIL_C3DGROUP2
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_C
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/C3DGroup2.h"
#include "engine/CParticleProgram.h"

void __fastcall CParticleProgram::RemapBoneIndices(std::map<unsigned long, unsigned long>& remap, CArray<C3DGroup2>& groups)
{
    CArray<C3DGroup2> new_groups;
    new_groups.reserve(groups.size());
    for (unsigned long i = 0; i < groups.size(); ++i) {
        C3DGroup2& group = groups[i];
        unsigned long bone_index = group.GetBoneIndex();
        if (remap.find(bone_index) != remap.end()) {
            group.SetBoneIndex(remap[bone_index]);
            new_groups.push_back(group);
        }
    }
    groups = new_groups;
}
