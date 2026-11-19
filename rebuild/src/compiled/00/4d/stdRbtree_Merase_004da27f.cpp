// std::_Rb_tree<long, std::pair<const long, CDefString>, ...>::_M_erase(_Rb_tree_node<pair<const long, CDefString> >* x) -- STLport recursive subtree erase of a map<long,CDefString>: erase the right subtree, free the node, continue left
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJVCDefString@@@std@@U?$_Select1st@U?$pair@$$CBJVCDefString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCDefString@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJVCDefString@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA27F, 43 bytes, `ret 4` (thiscall, size-optimised TU). Callers (E8): itself
//   (0x004DA291), the map<long,CDefString> clears 0x004DB898 (row 0x004DB88A), 0x006DAED2 (row 0x006DAEC0) and 0x006DBA6F (row 0x006DBA50).
// Identity (NF96 q11443, descriptive-by-library: STLport 4.6 _tree.c _M_erase, in neither PDB): the body is `while (x) { _M_erase(right(x));
//   y = left(x); destroy_node(x); x = y; }` (right child +0xC, left +8; CDefString is trivially destructible, so the node goes straight to
//   free 0x00BFEA14). The tree is map<long,CDefString>'s: (1) 0x006DBA50 is FableWin.pdb ??$GFSerialiseMapBinaryIn@VCDefString@@JU?$less@J@std@@@@
//   (its inline clear calls this), (2) the ported 0x004DB88A clear of the same tree binds this spelling. Retail has no identical-code folding.
// Source: stlport/fable_map.h (the recipe of the landed 0x006EFCE0); was a free __fastcall stand-in `BuildTreeArray` over AutoTinyTree*
//   models (catalog label LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray, propagated).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
template void std::_Rb_tree<long, std::pair<const long, CDefString>, std::_Select1st<std::pair<const long, CDefString> >, std::less<long>, std::allocator<std::pair<const long, CDefString> > >::_M_erase(std::_Rb_tree_node<std::pair<const long, CDefString> >*);
