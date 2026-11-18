// std::_Rb_tree<long, pair<const long, CDefString>, _Select1st<..>, less<long>, allocator<..> >::operator=(const _Rb_tree& x) -- STLport 4.6 _tree.c:
//   unless self-assignment, clear(), then either an empty header or _M_copy(x root) 0x004F927A with leftmost / rightmost walks, then the count
// Leaf: ??4?$_Rb_tree@JU?$pair@$$CBJVCDefString@@@std@@U?$_Select1st@U?$pair@$$CBJVCDefString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCDefString@@@std@@@2@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9207, 115 bytes, size TU.
// Identity (NF92, q11223): the tree of the std::map<long, CDefString> members of CHeroExperienceDef (Ego_r.pdb SoundIntensityThresholds /
//   ComboMultiplier{Increase,Lock,Lost,RunningHitsIncrease}): the implicit map operator= is `_M_t = x._M_t`, this STLport tree operator= out
//   of line; the ported CHeroExperienceDef::operator= 0x004F914A binds it under this spelling (x5). Explicit instantiation of stlport/fable_map.h
//   (TREE_ASSIGN_BODY, the 0x004F9660 recipe). Was the stand-in CRetailTree_004f927a::operator= (a local tree model). Descriptive-by-library.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
namespace std {
template _Rb_tree<long, pair<long const, CDefString>, _Select1st<pair<long const, CDefString> >, less<long>, allocator<pair<long const, CDefString> > >& _Rb_tree<long, pair<long const, CDefString>, _Select1st<pair<long const, CDefString> >, less<long>, allocator<pair<long const, CDefString> > >::operator=(const _Rb_tree<long, pair<long const, CDefString>, _Select1st<pair<long const, CDefString> >, less<long>, allocator<pair<long const, CDefString> > >&);
}
