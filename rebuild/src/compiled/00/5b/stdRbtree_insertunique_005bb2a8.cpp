// std::_Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, bool>, _Select1st<pair<EClothingSuitPart const, bool> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, bool> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x005BB220) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EClothingSuitPart@@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@U?$_Select1st@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@@3@U?$less@W4EClothingSuitPart@@@3@V?$allocator@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EClothingSuitPart@@_N@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BB2A8, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<EClothingSuitPart, bool> tree; no Ego_r.pdb member of this exact type; its two sources: the landed
//   CTCInventoryClothing::ConstructScreen 0x005B76D1 / ConstructScreenPC 0x005B8743 call this map<EClothingSuitPart, bool>'s operator[]
//   0x005BBC4E -> forwarder 0x005BBB7D (bound ?insert@?$map@W4EClothingSuitPart@@_N...) -> hinted insert_unique 0x005BB85A (binds this
//   spelling), and the node creator 0x005BAF06 copies the key dword and a single byte at +4 (the bool). Landed callers of its
//   insert_unique 0x005BB2A8: 0x005BB85A _global::CMemoryAllocatorVariableSize_InsertOrFind_24Byte. Landed callers of _M_insert besides
//   0x005BB2A8: 0x005BB85A _global::CMemoryAllocatorVariableSize_InsertOrFind_24Byte.
//   Its callee _M_insert 0x005BB220 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins005bb220::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#include "engine/EClothingSuitPart.h"
namespace std {
template pair<_Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, bool>, _Select1st<pair<EClothingSuitPart const, bool> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, bool> > >::iterator, bool> _Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, bool>, _Select1st<pair<EClothingSuitPart const, bool> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, bool> > >::insert_unique(const pair<EClothingSuitPart const, bool>&);
}
