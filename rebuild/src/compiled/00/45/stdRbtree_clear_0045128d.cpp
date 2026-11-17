// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, float>, ...>::clear() -- STLport tree clear: when the tree holds nodes,
// erase them all (_M_erase 0x00450903 from the root) and reset the header to an empty tree
// Leaf: ?clear@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@M@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045128D, a COMDAT in a size-optimised TU (`#pragma optimize("s", on)`).
// Identity (NF81, round 7, q10986 binder): the clear() of the map<EDamageAttribute, float> tree: its callee 0x00450903 is that tree's
//   _M_erase (q10986), and the landed tree operator= 0x004614C0 already binds this address under this spelling. Descriptive-by-library
//   (STLport; both PDBs are Dinkumware). Was the stand-in _global::RbTree_Clear_0045128d (leaf OnKill: a global __fastcall
//   CTCCarryable_OnKill over a local Node model calling `FreeCarry`).
// Port (NF81): one explicit instantiation of the STLport clear on stlport/fable_map.h (the 0x004E7720 recipe).
#pragma optimize("s", on)
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"
namespace std {
template void _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, float>, _Select1st<pair<const EDamageAttribute, float> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, float> > >::clear();
}
