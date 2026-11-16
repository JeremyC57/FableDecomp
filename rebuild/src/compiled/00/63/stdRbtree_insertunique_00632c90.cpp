// std::_Rb_tree<unsigned long,pair<const unsigned long,CCharString>,_Select1st<..>,less<unsigned long>,allocator<..> >::insert_unique(const value_type& v) -- STLport unique insert for the UI def's action map: walk down to the insertion parent, step back once when the walk ended going left, and insert only when no equal key exists
// Leaf: ?insert_unique@?$_Rb_tree@KU?$pair@$$CBKVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBKVCCharString@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCCharString@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBKVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBKVCCharString@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBKVCCharString@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00632C90, 143 bytes (+ int3 padding), `ret 8` (hidden pair<iterator,bool>
// result, &v).
// Identity: callers are the binary map reader 0x00632B90 (call at 0x00632C63, reached from the CUIDef ActionMap transfer
// 0x00632AF0, inserting a pair<const unsigned long, CCharString>) and two sites in another module (0x00A49F8B, 0x00A4A14F)
// whose trees fold onto this COMDAT (every map with an unsigned key at node +0x10 has these bytes: `setb`, `jae`). The
// catalog label "RBTree_LookupOrInsert" is descriptive.
// Callees: `_Rb_global<bool>::_M_decrement` @ 0x004294E2 (__cdecl, `add esp,4`), _Rb_tree::_M_insert(x, y, v, w = 0)
// @ 0x00632D30 (out of line, iterator by hidden pointer).
// Shape: the STLport 4.x insert_unique of the signed twin 0x00632990, with the unsigned key compare.
// Port (round 4, NF40): the local std::_Rb_tree / pair / iterator / CCharString models are replaced by the explicit
//   instantiation of stlport/fable_map.h's insert_unique(const value_type&) (the Codex pass 13 body under FABLE_RETAIL_STLP_MAP) over fable_string.h, with the opt-ins of the landed
//   0x00A495F0 TU (Codex pass 22 typed chain: map<unsigned long, CCharString> of the lip-sync sample data). The iterator is
//   _Rb_tree_iterator<V, _Nonconst_traits<V> >, as the queue claim and the PDB-shaped callers spell it.
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"

typedef std::_Rb_tree<unsigned long, std::pair<const unsigned long, CCharString>, std::_Select1st<std::pair<const unsigned long, CCharString> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, CCharString> > > TTree;

template std::pair<TTree::iterator, bool> TTree::insert_unique(const std::pair<const unsigned long, CCharString>&);
