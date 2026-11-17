// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, unsigned __int64>, _Select1st<..>, less<unsigned __int64>, allocator<..> >::_M_erase(x)
//   -- STLport 4.6 _tree.c: erase the subtree under x without rebalancing (recurse right, walk left, free each node)
// Leaf: ?_M_erase@?$_Rb_tree@_KU?$pair@$$CB_K_K@std@@U?$_Select1st@U?$pair@$$CB_K_K@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_K_K@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CB_K_K@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00527950, 51 bytes.
// Identity (NF92, q11218 follow-on): the tree of NThingManager::UIDsToReplaceWithUID (Ego_d.pdb PUB32: std::map<unsigned __int64, unsigned
//   __int64>, 0x013B8A8C); its atexit destructor 0x01228DF0 (re-authored by NF92 on that static) calls this address under exactly this
//   spelling. Explicit instantiation of the stlport/fable_map.h member (the 0x004E6E43 recipe). Was the stand-in with unproven key / value
//   types (catalog std::_Rb_tree<...CUnproven_00527950...>::_M_erase). Descriptive-by-library (STLport; both PDBs are Dinkumware builds).
#include "stlport/fable_map.h"
namespace std {
template void _Rb_tree<unsigned __int64, pair<const unsigned __int64, unsigned __int64>, _Select1st<pair<const unsigned __int64, unsigned __int64> >, less<unsigned __int64>, allocator<pair<const unsigned __int64, unsigned __int64> > >::_M_erase(_Rb_tree_node<pair<const unsigned __int64, unsigned __int64> >* x);
}
