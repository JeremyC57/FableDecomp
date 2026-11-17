// map<CCharString, pair<long, CDateAndTime>, less<CCharString>, allocator<pair<const CCharString, pair<long, CDateAndTime> > > >::operator[](const CCharString& k) -- STLport subscript of map<CCharString, pair<long, CDateAndTime> >: lower_bound, insert (k, (0, CDateAndTime())) when absent, return the mapped pair
// Leaf: ??A?$map@VCCharString@@U?$pair@JVCDateAndTime@@@std@@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JVCDateAndTime@@@std@@@std@@@3@@std@@QAEAAU?$pair@JVCDateAndTime@@@1@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00721640, 166 bytes, __fastcall `ret 4`.
// Identity: Ego_r.pdb CTCHeroMorph::CustomToBankIndex is map<CCharString, pair<long, CDateAndTime> > (only caller 0x0071D682,
//   CTCHeroMorph code). The pair temporary is 0x24 bytes: key +0, long +4, CDateAndTime (Ego_r.pdb, 28 bytes: Year .. Millisecond) +8.
// Callees: _M_lower_bound @ 0x0071F890 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950, CDateAndTime() @
//   0x009E9AB0 (landed), CCharString copy ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, _Rb_tree::insert_unique(iterator, const
//   value_type&) @ 0x00720960 (iterator argument built in its slot, the result through the dead `k` slot).
// Shape: STLport 4.6 map::operator[] verbatim; STLport's pair default ctor (`first(_T1()), second(_T2())`) builds the default
//   value at the frame top (zero long, CDateAndTime ctor); STLport's user pair copy ctor (`first(__o.first),
//   second(__o.second)`) copies it memberwise into the value_type temporary: the long, then CDateAndTime's implicit 7-dword
//   `rep movsd` (the implicit pair copy would move all eight dwords at once).
// NF85: moved off the row-local CBasicString / CCharString / STLport map and tree models onto the shared headers (the landed
//   0x009AC5D0 map<CCharString, CBankFileManager::CBankInfo>::operator[] recipe): stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[],
//   FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: lower_bound over the tree's private out-of-line _M_lower_bound) and fable_string.h. The own
//   symbol is the four-argument STLport map spelling (was the three-argument stand-in without the allocator), and _M_lower_bound binds the
//   private _Rb_tree spelling of its NF82 self row (was a map stand-in). FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: the default value is
//   STLport's `__default_constructed((_Tp*)0)`, as the old row had it; FABLE_RETAIL_CDATEANDTIME_CTOR: the out-of-line CDateAndTime() 0x009E9AB0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CDATEANDTIME_CTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CDateAndTime.h"
template std::pair<long, CDateAndTime>& std::map<CCharString, std::pair<long, CDateAndTime>, std::less<CCharString>, std::allocator<std::pair<const CCharString, std::pair<long, CDateAndTime> > > >::operator[](const CCharString&);
