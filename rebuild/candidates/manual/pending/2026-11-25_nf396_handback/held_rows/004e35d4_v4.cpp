// std::_Rb_tree<long, pair<const long, CEnginePrimitiveHandle>, ...>::_M_erase(_Rb_tree_node*) -- (map<long, CEnginePrimitiveHandle>) STLport 4.6 _tree.c _M_erase: erase the right subtree recursively, then walk left, destroying each node's value pair and freeing the node
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@U?$_Select1st@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E35D4, 53 bytes, thiscall, `ret 4`; size TU. Callees: itself (right subtree), the value pair's
//   destructor 0x004DD728 (??1?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@QAE@XZ, `lea ecx,[esi+10h]`), free 0x00BFEA14 (the node).
// Identity (NF391 trace, NF396 re-check): its references are its own recursive call 0x004E35E6 and the call at 0x004E4116 in the
//   41-byte tree clear 0x004E4108 (`_M_erase(root)`, then the header reset); the node value is the pair<const long,
//   CEnginePrimitiveHandle> whose destructor 0x004DD728 the table already binds (witness map<long, CEnginePrimitiveHandle>::operator[]
//   0x005BBFD6). FableWin: the map<long, CEnginePrimitiveHandle> members of CTCInventoryClothing are destroyed in its destructor. Both
//   PDBs are Dinkumware builds, so the spelling is the one the repo STLport emits (descriptive-by-library).
// NF396: was the stand-in CTextTreeWalkThrough::BuildTreeArray(TreeNode*) calling SubObject::Cleanup_004dd728 (catalog / manifest
//   LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray, propagated). Model: the shared stlport/fable_map.h _M_erase (the
//   0x00527950 form) over engine/CEnginePrimitiveHandle.h (the defines of the landed same-tree _M_insert row 0x005BBCBE).
#pragma optimize("s", on)
#include "stlport/fable_map.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR
#include "engine/CEnginePrimitiveHandle.h"
namespace std {
template void _Rb_tree<long, pair<const long, CEnginePrimitiveHandle>, _Select1st<pair<const long, CEnginePrimitiveHandle> >, less<long>, allocator<pair<const long, CEnginePrimitiveHandle> > >::_M_erase(_Rb_tree_node<pair<const long, CEnginePrimitiveHandle> >* x);
}
