// std::_Rb_tree<NUISystem::ETableSprites, pair<const NUISystem::ETableSprites, long>, ...>::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert for the UI def's sprite map: walk down to the insertion parent, step back once (_Rb_global<bool>::_M_decrement 0x004294E2) when the walk ended going left, and insert (_M_insert 0x00632A30) only when no equal key exists
// Leaf: ?insert_unique@?$_Rb_tree@W4ETableSprites@NUISystem@@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@U?$_Select1st@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@4@U?$less@W4ETableSprites@NUISystem@@@4@V?$allocator@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@4@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4ETableSprites@NUISystem@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00632990, 143 bytes (+ int3 padding), `ret 8` (hidden pair<iterator,bool> result, &v); /O2 TU.
// Identity: the only caller is the binary map reader 0x00632830 (GFSerialiseMapBinaryIn, call at 0x006328E4, reached from the
//   CUIDef Sprites transfer 0x006327A0), so the tree is map<NUISystem::ETableSprites, long> (Ego_r.pdb CUIDef::Sprites): a signed key
//   at node +0x10 (`setl`, `jge`). The catalog label "RBTree_find_insertion_bounds" is descriptive.
// NF58 (round 7): was a local namespace-std model whose one-parameter _Rb_tree_iterator<V> mangled both this row's own symbol and its
//   _M_insert call without STLport's _Nonconst_traits<V> argument; now the explicit instantiation of stlport/fable_map.h's insert_unique
//   (FABLE_RETAIL_STLP_MAP body; FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern, so
//   it binds the _M_insert 0x00632A30 defines. STLport 4.6.2 source, descriptive-by-library.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
namespace NUISystem { enum ETableSprites; }
namespace std {
template pair<_Rb_tree<NUISystem::ETableSprites, pair<NUISystem::ETableSprites const, long>, _Select1st<pair<NUISystem::ETableSprites const, long> >, less<NUISystem::ETableSprites >, allocator<pair<NUISystem::ETableSprites const, long> > >::iterator, bool> _Rb_tree<NUISystem::ETableSprites, pair<NUISystem::ETableSprites const, long>, _Select1st<pair<NUISystem::ETableSprites const, long> >, less<NUISystem::ETableSprites >, allocator<pair<NUISystem::ETableSprites const, long> > >::insert_unique(const pair<NUISystem::ETableSprites const, long>&);
}
