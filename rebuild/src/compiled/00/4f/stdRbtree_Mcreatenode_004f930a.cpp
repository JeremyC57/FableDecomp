// std::_Rb_tree<long, pair<const long, CDefString>, ...>::_M_create_node(const value_type& x) -- STLport 4.6 _tree.c: allocate a 0x18-byte
//   node (__node_alloc / malloc) and copy-construct the pair<const long, CDefString> into its value field at +0x10
// Leaf: ?_M_create_node@?$_Rb_tree@JU?$pair@$$CBJVCDefString@@@std@@U?$_Select1st@U?$pair@$$CBJVCDefString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCDefString@@@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBJVCDefString@@@std@@@2@ABU?$pair@$$CBJVCDefString@@@2@@Z
// Flags: /d2inlS20
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F930A, 34 bytes, size TU.
// Identity (NF92, q11223 follow-on, claimed through the lock): the callee of the ported _M_copy 0x004F927A (map<long, CDefString> tree);
//   was the stand-in TreeNode_Allocate_24Byte (local TreeNodePayload model). Explicit instantiation of stlport/fable_map.h (the 0x004F9A6E
//   recipe). Descriptive-by-library.
#pragma optimize("s",on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
namespace std {
template _Rb_tree_node<pair<long const, CDefString> >* _Rb_tree<long, pair<long const, CDefString>, _Select1st<pair<long const, CDefString> >, less<long>, allocator<pair<long const, CDefString> > >::_M_create_node(const pair<long const, CDefString>&);
}
