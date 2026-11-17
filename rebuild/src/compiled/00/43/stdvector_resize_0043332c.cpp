#pragma optimize("s",on)
// std::vector<CCharString>::resize(size_type n, const CCharString& x) -- STLport 4.6.2: erase the tail when shrinking, else fill-insert
//   n - size() copies of x at the end
// Leaf: ?resize@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXIABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043332C, 62 bytes, thiscall, `ret 8` (size-optimised TU).
// Identity (NF48, round 7): the callees are the out-of-line vector<CCharString>::erase(iterator, iterator) 0x0043336A (landed:
//   CCharString::operator= 0x0099EFB0 per element) and vector<CCharString>::_M_fill_insert 0x004333B3; its landed callers resize
//   vector<CCharString> members (CV_BookCollectingScript::BookDef 0x00E54990, CFishCompOrganiser 0x00E81D10,
//   GFSerialiseVectorBinaryIn<CCharString> 0x00433273). STLport takes the value by const reference (Ego_r.pdb's by-value
//   ?resize@?$vector@VCCharString@@...QAEXIVCCharString@@@Z is the Dinkumware form). Name descriptive-by-library. Was the generic
//   stand-in CArrayElem::resize (catalog "CArray<pair<long,long>>::push_back", propagated) over a 4-byte Elem.
// Shape: the shared stlport/fable_containers.h resize over the declaration-only erase(first, last)
//   (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE) and _M_fill_insert, emitted by an explicit instantiation.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"

template void std::vector<CCharString>::resize(size_t, const CCharString&);
