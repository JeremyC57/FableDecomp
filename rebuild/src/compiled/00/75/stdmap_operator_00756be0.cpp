// map<CCharString, pair<long, long>, less<CCharString>, allocator<pair<const CCharString, pair<long, long> > > >::operator[](const CCharString& k) -- STLport subscript of map<CCharString, pair<long, long> >: lower_bound, insert (k, (0, 0)) when absent, return the mapped pair
// Leaf: ??A?$map@VCCharString@@U?$pair@JJ@std@@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QAEAAU?$pair@JJ@1@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756BE0, 127 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: Ego_r.pdb CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine (+0xC) is map<CCharString, pair<long, long> >;
//   both callers sit in the one setter 0x00755680 (`m[k].first = duration; m[k].second = fine;` on this +0xC). The
//   manifest's map<CCharString, C2DVector> is propagated.
// Callees: _M_lower_bound @ 0x00756100 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950, CCharString copy
//   ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, _Rb_tree::insert_unique(iterator, const value_type&) @ 0x00756790.
// Shape: STLport 4.6 map::operator[] verbatim; STLport's pair default ctor value-initialises both longs (the two zero stores).
// NF85: moved off the row-local CBasicString / CCharString / STLport map and tree models onto the shared headers (the landed
//   0x009AC5D0 map<CCharString, CBankFileManager::CBankInfo>::operator[] recipe): stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[],
//   FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: lower_bound over the tree's private out-of-line _M_lower_bound) and fable_string.h. The own
//   symbol is the four-argument STLport map spelling (was the three-argument stand-in without the allocator), and _M_lower_bound binds the
//   private _Rb_tree spelling of its NF82 self row (was a map stand-in). FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: the default value is
//   STLport's `__default_constructed((_Tp*)0)`, as the old row had it.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
template std::pair<long, long>& std::map<CCharString, std::pair<long, long>, std::less<CCharString>, std::allocator<std::pair<const CCharString, std::pair<long, long> > > >::operator[](const CCharString&);
