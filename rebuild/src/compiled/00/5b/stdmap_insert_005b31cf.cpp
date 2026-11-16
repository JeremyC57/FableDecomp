// std::map<NUISystem::CComponent*, std::pair<long,long> >::insert(iterator position, const value_type& x) -- hinted insert: forward to
// the tree's insert_unique(position, x) and return its iterator
// Leaf: ?insert@?$map@PAVCComponent@NUISystem@@U?$pair@JJ@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@2@@2@U32@ABU?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B31CF, 29 bytes, thiscall `ret 0Ch` (hidden result, position, x); calls
//   _Rb_tree::insert_unique(iterator, const value_type&) @ 0x005B2EFE.
// Identity: STLport 4.6 _map.h `iterator insert(iterator, const value_type&) { return _M_t.insert_unique(position, x); }`; the only
//   caller is map::operator[] 0x005B322B. Descriptive-by-library (STLport 4.6.2; both PDBs are Dinkumware builds).
// NF42 (round 5): the local three-parameter map / one-parameter iterator model (a truncated mangling) replaced by the shared
// stlport/fable_map.h, so the self spelling and its insert_unique callee carry the full STLport template arguments.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
namespace std {
template map<NUISystem::CComponent*, pair<long, long> >::iterator
    map<NUISystem::CComponent*, pair<long, long> >::insert(iterator, const value_type&);
}
