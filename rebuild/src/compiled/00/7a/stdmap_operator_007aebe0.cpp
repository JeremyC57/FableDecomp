// map<CCharString, vector<CCharString>, less<CCharString>, allocator<pair<const CCharString, vector<CCharString> > > >::operator[](const CCharString& k) -- STLport subscript of map<CCharString, vector<CCharString> >: lower_bound, insert (k, empty vector) when absent, return the mapped vector
// Leaf: ??A?$map@VCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@3@@std@@QAEAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@1@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AEBE0, 224 bytes, __fastcall `ret 4`.
// Identity: the value type is pair<const CCharString, vector<CCharString> > (landed _Rb_tree::_M_erase @ 0x007AE0E0 and
//   vector<CCharString> copy ctor @ 0x007ACEA0 of the same tree); 41 call sites (CGameScriptInterface::SetGossip
//   0x008A9E90, CTCAIScratchpad 0x007ABBD0, ...). The manifest's CArray<CMeshDataBank::CLipSyncEntry> is propagated.
// Callees: _M_lower_bound @ 0x007ACC00 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950, CCharString copy
//   ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, vector<CCharString> copy ctor @ 0x007ACEA0, _Rb_tree::insert_unique(iterator,
//   const value_type&) @ 0x007AE870 (result through the dead `k` slot), free @ 0x00BFEA14.
// Shape: STLport 4.6 map::operator[] verbatim. The default vector is three zero stores and its destructor folds away;
//   the value_type temporary's ~vector destroys [start, finish) and frees the null-tested start, then the key dies.
//   Same-TU stand-in (accepted precedent 0x0075D2B0): the vector copy constructor's body sits in this TU under
//   auto_inline(off); seeing that it only reads its argument lets VC7.1 fold the default temporary's ~vector, as
//   retail's whole-program knowledge did (declared-only, the temporary is re-read and freed: 255v224).
// NF85: moved off the row-local CBasicString / CCharString / STLport map and tree models onto the shared headers (the landed
//   0x009AC5D0 map<CCharString, CBankFileManager::CBankInfo>::operator[] recipe): stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[],
//   FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: lower_bound over the tree's private out-of-line _M_lower_bound) and fable_string.h. The own
//   symbol is the four-argument STLport map spelling (was the three-argument stand-in without the allocator), and _M_lower_bound binds the
//   private _Rb_tree spelling of its NF82 self row (was a map stand-in). FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: the default value is
//   STLport's `__default_constructed((_Tp*)0)`; FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE: the vector<CCharString> copy constructor 0x007ACEA0 with
//   its body visible but out of line (replaces the row's same-TU stand-in copy of it).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
template std::vector<CCharString>& std::map<CCharString, std::vector<CCharString>, std::less<CCharString>, std::allocator<std::pair<const CCharString, std::vector<CCharString> > > >::operator[](const CCharString&);
