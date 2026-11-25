// std::_Rb_tree<CCharString, pair<const CCharString, CTCHeroStats::MirroredCriminalStatus>, ...>::_M_erase(_Rb_tree_node*) -- (map<CCharString, CTCHeroStats::MirroredCriminalStatus>, CTCHeroStats::OutstandingCrimes) STLport 4.6 _tree.c _M_erase: erase the right subtree recursively, then walk left, destroying each node's key string and freeing the node
// Leaf: ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057FEFB, 53 bytes, thiscall, `ret 4`; size TU. Callees: itself (right subtree),
//   CCharString::~CCharString 0x0099EAE0 (`lea ecx,[esi+10h]`: the pair's destructor inlined, MirroredCriminalStatus is trivially
//   destructible), free 0x00BFEA14 (the node).
// Identity (NF391 trace, NF396 re-check): its references are its own recursive call 0x0057FF0D and the call at 0x0058061B in the
//   41-byte tree clear 0x0058060D (`_M_erase(root)`, then the header reset). The tree is CTCHeroStats::OutstandingCrimes (Ego_r.pdb
//   map<CCharString, CTCHeroStats::MirroredCriminalStatus> at +0x0C, the class's only map), whose operator[] 0x005812DF and insert
//   chain the table already binds under the same tree spelling; FableWin ~CTCHeroStats destroys that map. Both PDBs are Dinkumware
//   builds, so the spelling is the one the repo STLport emits (descriptive-by-library).
// NF396: was the stand-in CTextTreeWalkThrough::BuildTreeArray(TreeNode*) calling SubObject::Cleanup (catalog / manifest
//   LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray, propagated). Model: the shared stlport/fable_map.h _M_erase (the
//   0x00527950 form) over engine/CTCHeroStats.h's MirroredCriminalStatus (the defines of the landed 0x007CF4C0).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CTCHeroStats.h"
namespace std {
template void _Rb_tree<CCharString, pair<const CCharString, CTCHeroStats::MirroredCriminalStatus>, _Select1st<pair<const CCharString, CTCHeroStats::MirroredCriminalStatus> >, less<CCharString>, allocator<pair<const CCharString, CTCHeroStats::MirroredCriminalStatus> > >::_M_erase(_Rb_tree_node<pair<const CCharString, CTCHeroStats::MirroredCriminalStatus> >* x);
}
