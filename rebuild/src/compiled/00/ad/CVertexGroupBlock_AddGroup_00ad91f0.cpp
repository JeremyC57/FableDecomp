// CVertexGroupBlock::AddGroup(unsigned long group) -- append a bone group to the block and record its index in the group -> index map
// Leaf: ?AddGroup@CVertexGroupBlock@@QAEXK@Z
// Inline-budget: __declspec(noinline) through FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE -- retail calls vector<unsigned long>::
//   _M_insert_overflow 0x00464ACE out of line with its body visible (the job of the old same-TU stand-in); recheck_final.log RELOCATION_MATCH PASS.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD91F0, 0x62 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: Ego_r.pdb ?AddGroup@CVertexGroupBlock@@QAEXK@Z; retail layout as the landed CVertexGroupBlock::IsSubsetOf
// 0x00AD8C30 (Groups vector<unsigned long> +0x00, GroupToIndexMap map<unsigned long, long> +0x0C). Callers: the mesh
// code at 0x00AA6D3C, CVertexGroupBlock::AddGroupsFrom 0x00AD9260 and CVertexGroupBlockOptimiser::InsertBlock 0x00AD92E0.
// Catalog "AnimBlockGroup_AddBlock" is descriptive.
// Callees: vector<unsigned long>::_M_insert_overflow(pos, x, __true_type, fill_len, at_end) @ 0x00464ACE (as in the
// landed 0x00AD9D70), map<unsigned long, long>::insert -> _Rb_tree::insert_unique(const value_type&) @ 0x0045CA09
// (pair<iterator,bool> through the hidden pointer).
// Shape: STLport push_back (`if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
// _M_insert_overflow(_M_finish, x, __true_type(), 1, true);` -- x binds to the parameter's own slot, the empty tag is
// homed at that same slot) then `GroupToIndexMap.insert(value_type(group, Groups.size() - 1))` with the pair built
// after the argument pushes.
// NF61 (round 7, q10554): moved onto the shared header engine/CVertexGroupBlock.h (FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS: Groups
//   CArray<unsigned long> +0, GroupToIndexMap std::map<unsigned long,long> +0xC; _ADDGROUP declares AddGroup), engine/CArray.h,
//   stlport/fable_containers.h (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE: STLport's _M_insert_overflow body visible but called out
//   of line, which lets VC7.1 home the empty __true_type tag on the `group` slot as retail -- the job the same-TU stand-in did) and
//   stlport/fable_map.h; the local vector / map / pair / iterator models are gone. The insert binds NF58's
//   _Rb_tree<unsigned long, pair<const unsigned long, long>, ...>::insert_unique 0x0045CA09. Self leaf unchanged (Ego_r.pdb).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_ADDGROUP
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"

void CVertexGroupBlock::AddGroup(unsigned long group)
{
    Groups.push_back(group);
    GroupToIndexMap.insert(std::map<unsigned long, long>::value_type(group, Groups.size() - 1));
}
