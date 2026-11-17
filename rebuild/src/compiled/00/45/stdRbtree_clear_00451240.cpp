// std::_Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, float>, ...>::clear() -- STLport 4.6: if the tree is not empty,
//   _M_erase(_M_root()) (0x004508D6), then reset the header (leftmost = header, root = 0, rightmost = header) and the node count
// Leaf: ?clear@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00451240, 41 bytes, a COMDAT in a size-optimised TU.
// Identity (NF86, q11100): the clear of the map<EObjectAugmentationType, float> tree. Its landed callers already bind this spelling here
//   (reloc_bindings: GFSerialiseMapBinaryIn<float, EObjectAugmentationType, less<..>> 0x004616D6; also called by that tree's operator= 0x0046139B),
//   and the tree destructor 0x00451A85 (q11100, CArmourDef::AugmentationResponse) calls it first. Byte shape = the EDamageAttribute twin 0x0045128D
//   (NF81). Descriptive by library (STLport 4.6; both PDBs are Dinkumware builds). Was the stand-in _global::RbTree_Clear_00451240 (leaf
//   ?CTCCarryable_OnKill@@YIXPAVCTCCarryable@@@Z over a local node model; _M_erase bound as ?ReleaseThing_004508d6@@YGXPAX@Z).
// Port (NF86): one explicit instantiation on stlport/fable_map.h (the 0x0045128D recipe).
#pragma optimize("s", on)
#include "engine/EObjectAugmentationType.h"
#include "stlport/fable_map.h"
namespace std {
template void _Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, float>, _Select1st<pair<const EObjectAugmentationType, float> >, less<EObjectAugmentationType>, allocator<pair<const EObjectAugmentationType, float> > >::clear();
}
