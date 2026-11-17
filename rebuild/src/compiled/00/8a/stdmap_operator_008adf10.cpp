// map<CCharString, bool, less<CCharString>, allocator<pair<const CCharString, bool > > >::operator[](const CCharString& k) -- STLport subscript of map<CCharString, bool>: lower_bound, insert (k, false) when absent, return the mapped flag
// Leaf: ??A?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@QAEAA_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ADF10, 115 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: map<CCharString, bool> (landed callers declare `bool& operator[](const CCharString& key); // 0x008ADF10`; the
//   pair's second is a byte store). The manifest's map<CCharString, C2DVector> is propagated.
// Callees: _M_lower_bound @ 0x007ACBB0 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950, CCharString copy
//   ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, _Rb_tree::insert_unique(iterator, const value_type&) @ 0x008ACA90.
// Shape: as map<CCharString, long>::operator[] 0x00519230 (STLport 4.6 verbatim, `__default_constructed`, inline insert
//   forwarder, result through the dead `k` slot).
// NF85: moved off the row-local CBasicString / CCharString / STLport map and tree models onto the shared headers (the landed
//   0x009AC5D0 map<CCharString, CBankFileManager::CBankInfo>::operator[] recipe): stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[],
//   FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: lower_bound over the tree's private out-of-line _M_lower_bound) and fable_string.h. The own
//   symbol is the four-argument STLport map spelling (was the three-argument stand-in without the allocator), and _M_lower_bound binds the
//   private _Rb_tree spelling of its NF82 self row (was a map stand-in).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
template bool& std::map<CCharString, bool, std::less<CCharString>, std::allocator<std::pair<const CCharString, bool > > >::operator[](const CCharString&);
