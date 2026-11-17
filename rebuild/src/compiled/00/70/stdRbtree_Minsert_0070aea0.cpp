// std::_Rb_tree<long, pair<long const, CEngineTextureDecal>, _Select1st<pair<long const, CEngineTextureDecal> >, less<long >, allocator<pair<long const, CEngineTextureDecal> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJVCEngineTextureDecal@@@std@@U?$_Select1st@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJVCEngineTextureDecal@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJVCEngineTextureDecal@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070AEA0, 226 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the map<long, CEngineTextureDecal> tree that CTCTextureDecal::PeekEngineDecals 0x0070A570 fills (through
//   its insert_unique 0x0070AFE0) before copying the decals into EngineDecals (Ego_r.pdb vector<CEngineTextureDecal>). Two sources: the
//   PDB name sets carry map<long, CEngineTextureDecal> members (`?$map@JVCEngineTextureDecal@@U?$less@J@std@@...`), and the 0x28-byte
//   node = 0x10 links + the long key + the 20-byte retail CEngineTextureDecal (two longs + the STLport vector, copied through the
//   out-of-line vector<CCompositeBlendDesc> copy constructor 0x005BB346, FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE).
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CEngineTextureDecalMap::_M_insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/CEngineTextureDecal.h"
namespace std {
template _Rb_tree<long, pair<long const, CEngineTextureDecal>, _Select1st<pair<long const, CEngineTextureDecal> >, less<long >, allocator<pair<long const, CEngineTextureDecal> > >::iterator _Rb_tree<long, pair<long const, CEngineTextureDecal>, _Select1st<pair<long const, CEngineTextureDecal> >, less<long >, allocator<pair<long const, CEngineTextureDecal> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, pair<long const, CEngineTextureDecal> const&, _Rb_tree_node_base*);
}
