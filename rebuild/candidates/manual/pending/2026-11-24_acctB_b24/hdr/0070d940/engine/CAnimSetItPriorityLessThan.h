#ifndef FABLE_ENGINE_CANIMSETITPRIORITYLESSTHAN_H
#define FABLE_ENGINE_CANIMSETITPRIORITYLESSTHAN_H
#include "rebuild_abi.h"
// New header (B24C7 overlay, corrected by B24C8; row 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim): the stable_sort predicate over
// list<CAnimSet>::const_iterator. Ego_r.pdb struct_layouts: CAnimSetItPriorityLessThan size 1, no members (an EMPTY functor), next to
// binary_function<_List_const_iterator<..CAnimSet..>, _List_const_iterator<..>, bool> (size 1): the base. FableWin.pdb 0x01EAB0B0
// ??RCAnimSetItPriorityLessThan@@QBE_NABV?$_List_const_iterator@V?$_List_val@VCAnimSet@@V?$allocator@VCAnimSet@@@std@@@std@@@std@@0@Z
// (public const, two const iterator references). Its body (compare CAnimSet::Priority +0x4C) is expanded inside the landed
// __stable_sort_aux 0x00711B80 chain (empty functor there too); declared only here (Priority is private to CAnimSet). Retail passes
// the empty by-value comparator as an uninitialised 4-byte slot (0x0070DF9F / 0x0070E16A `mov eax,[esp+5Ch] ; push eax`).
#ifdef FABLE_RETAIL_CANIMSETITPRIORITYLESSTHAN
#include "stlport/fable_containers.h"
#include "engine/CAnimSet.h"
class CAnimSetItPriorityLessThan : public std::binary_function<std::list<CAnimSet>::const_iterator, std::list<CAnimSet>::const_iterator, bool> {
public:
    bool operator()(const std::list<CAnimSet>::const_iterator& a, const std::list<CAnimSet>::const_iterator& b) const;
};
#endif
#endif
