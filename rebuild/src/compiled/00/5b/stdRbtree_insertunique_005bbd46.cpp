// std::_Rb_tree<long, pair<long const, CEnginePrimitiveHandle>, _Select1st<pair<long const, CEnginePrimitiveHandle> >, less<long >, allocator<pair<long const, CEnginePrimitiveHandle> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x005BBCBE) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@JU?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@U?$_Select1st@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBJVCEnginePrimitiveHandle@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BBD46, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<long, CEnginePrimitiveHandle> tree; Ego_r.pdb members of this type:
//   CHeroPostcardGenerator::PrimitiveHandles, CTCInventoryClothing::MorphPrimitiveHandles. Landed callers of its insert_unique
//   0x005BBD46: 0x005BBE32 _global::CMemoryAllocatorVariableSize_InsertOrFind_ScriptRight. Landed callers of _M_insert besides
//   0x005BBD46: 0x005BBE32 _global::CMemoryAllocatorVariableSize_InsertOrFind_ScriptRight.
//   Its callee _M_insert 0x005BBCBE is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins005bbcbe::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
namespace std {
template pair<_Rb_tree<long, pair<long const, CEnginePrimitiveHandle>, _Select1st<pair<long const, CEnginePrimitiveHandle> >, less<long >, allocator<pair<long const, CEnginePrimitiveHandle> > >::iterator, bool> _Rb_tree<long, pair<long const, CEnginePrimitiveHandle>, _Select1st<pair<long const, CEnginePrimitiveHandle> >, less<long >, allocator<pair<long const, CEnginePrimitiveHandle> > >::insert_unique(const pair<long const, CEnginePrimitiveHandle>&);
}
