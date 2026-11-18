// map<CCharString, CCharString, less<CCharString>, allocator<pair<const CCharString, CCharString > > >::operator[](const CCharString& k) -- STLport subscript of map<CCharString, CCharString>: lower_bound, insert (k, CCharString()) when absent, return the mapped string
// Leaf: ??A?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@std@@QAEAAVCCharString@@ABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AC700, 149 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: map<CCharString, CCharString> (the landed callers declare `CCharString& operator[](const CCharString&)
//   // 0x009AC700`: the script argument maps, the bank-file name maps); same TU as the landed
//   map<CCharString, NBankFileManager::CContainedBank>::operator[] 0x009AC530, whose model this reuses.
// Callees: _M_lower_bound @ 0x009AAE00 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950,
//   CCharString() @ 0x0099E4B0 (the default value), copy ctor @ 0x0099EC30 (key and value into the pair temporary),
//   ~CCharString @ 0x0099EAE0 (pair second, pair first, default value), _Rb_tree::insert_unique(iterator, const value_type&)
//   @ 0x009ABDB0 (iterator argument built in its slot, result in a frame local).
// Shape: STLport 4.6 map::operator[] verbatim with the inline less<CCharString> / CCharString::operator<; the default
//   value is `__default_constructed((_Tp*)0)` (_STLP_DEFAULT_CONSTRUCTED), built in the dead `k` argument slot before the
//   pair; map::insert is the inline forwarder to insert_unique (its by-value result re-read from the frame).
// NF85: moved off the row-local CBasicString / CCharString / STLport map and tree models onto the shared headers (the landed
//   0x009AC5D0 map<CCharString, CBankFileManager::CBankInfo>::operator[] recipe): stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[],
//   FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: lower_bound over the tree's private out-of-line _M_lower_bound) and fable_string.h. The own
//   symbol is the four-argument STLport map spelling (was the three-argument stand-in without the allocator), and _M_lower_bound binds the
//   private _Rb_tree spelling of its NF82 self row (was a map stand-in). FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: the default value is
//   STLport's `__default_constructed((_Tp*)0)`, as the old row had it (and as 0x0042D5B1 does).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
template CCharString& std::map<CCharString, CCharString, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCharString > > >::operator[](const CCharString&);
