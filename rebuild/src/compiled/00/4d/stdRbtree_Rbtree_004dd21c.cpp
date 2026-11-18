// std::_Rb_tree<long, pair<const long, CDefString>, _Select1st<..>, less<long>, allocator<..> >::~_Rb_tree() -- STLport 4.6 _tree.h: clear()
//   (out of line, 0x004DB88A) and free the header node
// Leaf: ??1?$_Rb_tree@JU?$pair@$$CBJVCDefString@@@std@@U?$_Select1st@U?$pair@$$CBJVCDefString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCDefString@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD21C, 23 bytes, size TU.
// Identity (NF92, q11221): the tree of the std::map<long, CDefString> members of CHeroExperienceDef (Ego_r.pdb SoundIntensityThresholds /
//   ComboMultiplierIncrease / Lock / Lost / RunningHitsIncrease); FableWin.pdb 0x01B9BFDD names the map's own destructor (debug), and in
//   retail's STLport the map destructor is inline and this COMDAT is the tree's (as the landed 0x004E81FB / 0x004E81E4). The ported
//   CHeroExperienceDef destructor 0x004EBB1C binds it under this spelling (x5). Explicit instantiation of stlport/fable_map.h (the 0x004E81E4
//   recipe: TREE_CLEAR_OUTOFLINE). Was the propagated stand-in CActiveFile::OnReadFinished (?OnReadFinished_5@@YIXPAUCActiveFile@@@Z over a
//   local model). Descriptive-by-library (STLport; both PDBs are Dinkumware builds).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
template std::_Rb_tree<long, std::pair<const long, CDefString>, std::_Select1st<std::pair<const long, CDefString> >, std::less<long>, std::allocator<std::pair<const long, CDefString> > >::~_Rb_tree();
