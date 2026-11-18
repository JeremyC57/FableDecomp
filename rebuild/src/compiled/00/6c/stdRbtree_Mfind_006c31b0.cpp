// std::_Rb_tree::_M_find(const key_type& k) const -- (map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> >) STLport exact-key lookup over CCharString keys: walk from the root keeping the last node whose key is not less than k, then return it unless it is the header or k is less than its key (then the header, i.e. end())
// Leaf: ??$_M_find@VCCharString@@@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VC3DSkeletalMorphTarget@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VC3DSkeletalMorphTarget@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VC3DSkeletalMorphTarget@@@@@std@@@3@@std@@ABEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VC3DSkeletalMorphTarget@@@@@std@@@1@ABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C31B0, 112 bytes (+ int3 padding), thiscall, `ret 4`; returns the node pointer.
// Identity: `map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> >`; evidence: the landed CSkeletalMorphResourceManager::GetMorphTarget 0x006C3620 and PreloadBoneConfigs 0x006C37D0 call it on CachedMorphs (Ego_r CSkeletalMorphResourceManager +4: map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> >).
//   Family F012 (17 byte-identical bodies, one COMDAT per tree type): the manifest label "BinaryTree_FindNodeByString" is behavioural.
//   STLport 4.6 _tree.h `_Rb_tree::_M_find` (the out-of-line node lookup behind set/map/multimap find; map and multimap
//   share one `_Rb_tree<K, pair<const K, V>, _Select1st<...>, less<K>, allocator<...> >` instantiation).
// Callee: CBasicString<char>::operator< 0x00429950 (thiscall `ret 4`), reached through the inline CCharString::operator<
//   (equal data pointers -> false, null this -> true, null other -> false), once per step of the walk (node key vs k)
//   VC7.1's register allocation as retail (header in ebp, candidate in esi, node in edi, k in ebx).
// Node layout: color +0, parent +4, left +8, right +0xC, key at +0x10 (first word of the value); tree: header node
//   pointer +0, node count +4, comparator +8.
// Value: pair<const CCharString, CCountedPointer<C3DSkeletalMorphTarget>> (_Select1st); only the key is read, the mapped type
//   only names the instantiation.
// NF80 (q10763): moved onto the shared headers -- stlport/fable_map.h's member-template _M_find (NF64's 0x00579114 recipe) and
//   fable_string.h's CCharString::operator< (FABLE_RETAIL_CCHARSTRING_LESS_INLINE) over the one CBasicString<char>::operator< body
//   (FABLE_RETAIL_CBASICSTRING_LESS_BODY: an inline, never-inlined COMDAT of the 0x00429950 definition, so VC7.1 sees the call writes
//   no memory and keeps the header in ebp, candidate in esi, node in edi, k in ebx as retail). The local CBasicString / CCharString /
//   std tree models and the row's own operator< copy are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class C3DSkeletalMorphTarget;

template std::_Rb_tree_node<std::pair<CCharString const, CCountedPointer<C3DSkeletalMorphTarget> > >* std::_Rb_tree<CCharString, std::pair<CCharString const, CCountedPointer<C3DSkeletalMorphTarget> >, std::_Select1st<std::pair<CCharString const, CCountedPointer<C3DSkeletalMorphTarget> > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, CCountedPointer<C3DSkeletalMorphTarget> > > >::_M_find<CCharString>(const CCharString&) const;
