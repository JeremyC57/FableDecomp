// map<CCharString, C2DVector, less<CCharString>, allocator<pair<const CCharString, C2DVector > > >::operator[](const CCharString& k) -- STLport subscript of map<CCharString, C2DVector>: lower_bound, insert (k, C2DVector()) when absent, return the mapped vector
// Leaf: ??A?$map@VCCharString@@VC2DVector@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VC2DVector@@@std@@@4@@std@@QAEAAVC2DVector@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BD6A0, 124 bytes, __fastcall `ret 4`.
// Identity: map<CCharString, C2DVector> (landed callers declare `C2DVector& operator[](const CCharString& key); //
//   0x006BD6A0`; CDefinitionManager / region-def code). The pair temporary is 12 bytes.
// Callees: _M_lower_bound @ 0x006BCEC0 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950, CCharString copy
//   ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, _Rb_tree::insert_unique(iterator, const value_type&) @ 0x006BD3A0.
// Shape: STLport 4.6 map::operator[] verbatim. C2DVector's default constructor is empty (`C2DVector() {}`), so
//   `__default_constructed((C2DVector*)0)` returns an uninitialised value: VC7.1 copies whatever its slots hold into the pair's
//   second (the `mov eax,[esp+0Ch] ; mov [esp+10h],eax ; mov ecx,[esp+10h]` chain) -- no zero stores.
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
#define FABLE_RETAIL_C2DVECTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/C2DVector.h"
template C2DVector& std::map<CCharString, C2DVector, std::less<CCharString>, std::allocator<std::pair<const CCharString, C2DVector > > >::operator[](const CCharString&);
