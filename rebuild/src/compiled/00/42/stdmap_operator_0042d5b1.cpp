// std::map<unsigned long, NUISystem::CUIState>::operator[](const unsigned long& k) -- STLport map subscript: lower_bound, insert a default
// CUIState when the key is missing, return the mapped value
// Leaf: ??A?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@QAEAAVCUIState@NUISystem@@ABK@Z
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D5B1, 83 bytes. Pragmas: frame pointer kept in a
// size-optimised TU (`optimize("y",off)` + `("s",on)`, the tree-TU settings).
// Callees: 0x0042AD6C _Rb_tree::_M_lower_bound (node pointer in eax), 0x0052E5D0 CUIState::CUIState(),
// 0x0042AA96 pair<CComponent* const, CUIState>::pair(const key&, const CUIState&), 0x0042D4D3
// _Rb_tree::insert_unique(iterator hint, const value_type&) (iterator by hidden pointer).
//
// The manifest label `CPersistTraits::Transfer` is propagated and wrong. This is STLport's
//     T& operator[](const key_type& k) {
//         iterator i = lower_bound(k);
//         if (i == end() || key_comp()(k, (*i).first)) i = insert(i, value_type(k, T()));
//         return (*i).second; }
// STLport tree layout: the map holds the header node pointer at +0 (`cmp esi,[edi]` is end()), nodes
// are {color, parent, left, right} with the value at +0x10: the key (+0x10, a pointer: unsigned
// `jae`) and the 0x24-byte mapped CUIState (+0x14, returned with `lea eax,[esi+14h]`). The mapped
// default (frame -0x24) and the pair (frame -0x4C) have trivial destructors; the iterator is a
// 4-byte class with a user copy constructor (built in a reserved argument slot: `push ecx ; mov
// eax,esp ; mov [eax],esi`), and the insert's hidden result reuses the dead key-argument slot
// (`lea eax,[ebp+8]`). The default CUIState is a NAMED local: retail re-addresses it with
// `lea eax,[ebp-24h]` after its constructor, where a bare `CUIState()` temporary pushes the
// constructor's returned eax (80 v 83 bytes).
namespace NUISystem { class CComponent; }

// NF42 (round 5): the CComponentStateMap / SComponentStatePair / local CUIState stand-ins replaced by the shared
// engine/NUISystem_CUIState.h and stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[], FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE), as
// the map<unsigned long, NUISystem::CUIState> of the NUISystem state lists (the key is an unsigned long, 0x0042AD6C's _M_lower_bound
// spelling; the landed caller 0x005416B0 binds this full STLport spelling). Descriptive-by-library (STLport 4.6.2).
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#include "stlport/fable_map.h"
namespace std {
template NUISystem::CUIState& map<unsigned long, NUISystem::CUIState>::operator[](const unsigned long&);
}
