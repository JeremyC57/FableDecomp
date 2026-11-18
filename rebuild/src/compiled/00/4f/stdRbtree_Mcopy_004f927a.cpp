// std::_Rb_tree<long, pair<const long, CDefString>, ...>::_M_copy(_Rb_tree_node* x, _Rb_tree_node* p) -- STLport 4.6 _tree.c: clone a subtree,
//   _M_clone_node (_M_create_node 0x004F930A + colour, null links) per node, right subtrees by recursion, the left spine iteratively
// Leaf: ?_M_copy@?$_Rb_tree@JU?$pair@$$CBJVCDefString@@@std@@U?$_Select1st@U?$pair@$$CBJVCDefString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCDefString@@@std@@@2@@std@@AAEPAU?$_Rb_tree_node@U?$pair@$$CBJVCDefString@@@std@@@2@PAU32@0@Z
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F927A, 144 bytes, frame pointer kept, size TU.
// Identity (NF92, q11223): the callee of the ported tree operator= 0x004F9207 (map<long, CDefString>, CHeroExperienceDef members); was the
//   stand-in CRetailTree_004f930a::Copy (a local tree model, label CTCCarryable_CloneTree propagated). Explicit instantiation of
//   stlport/fable_map.h (TREE_MCOPY_BODY, the 0x004F99DE recipe). Descriptive-by-library.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_TREE_MCOPY_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
namespace std {
template _Rb_tree_node<pair<long const, CDefString> >* _Rb_tree<long, pair<long const, CDefString>, _Select1st<pair<long const, CDefString> >, less<long>, allocator<pair<long const, CDefString> > >::_M_copy(_Rb_tree_node<pair<long const, CDefString> >*, _Rb_tree_node<pair<long const, CDefString> >*);
}
