// std::map::operator[](const CCharString& k) -- STLport subscript of map<CCharString, CArray<unsigned long> >: lower_bound, insert (k, empty array) when absent, return the mapped array
// Leaf: ??A?$map@VCCharString@@V?$CArray@K@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V?$CArray@K@@@std@@@4@@std@@QAEAAV?$CArray@K@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D4810, 166 bytes, __fastcall `ret 4`.
// Identity: only caller CBankFile::WriteSymbolFile (0x009CF573) push_backs a loop index into the result (in-place store,
//   else vector<unsigned long>::_M_insert_overflow @ 0x00464ACE, landed). The manifest's map<CCharString, C2DVector> is
//   propagated.
// Callees: _M_lower_bound @ 0x009D2BA0 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950, CCharString copy
//   ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, vector<unsigned long> copy ctor @ 0x00535470, _Rb_tree::insert_unique(iterator,
//   const value_type&) @ 0x009D4380 (result through the dead `k` slot), free @ 0x00BFEA14.
// NF258 (2026-11-22, L37): ported off the row-local CBasicString / CCharString / vector / pair / map / tree models onto the shared
//   headers, the 0x009AC5D0 recipe (fable_string.h CCHARSTRING_LESS_INLINE + CBASICSTRING_LESS_BODY, stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP operator[] + FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE for the out-of-line _M_lower_bound), with
//   engine/CArray.h CArray<unsigned long> as the mapped type: both PDBs spell this function
//   ??A?$map@VCCharString@@V?$CArray@K@@...@std@@QAEAAV?$CArray@K@@ABVCCharString@@@Z, and the landed caller 0x009CF3A0
//   (CBankFile::WriteSymbolFile) binds that spelling here. The vector copy constructor's body stays visible but out of line
//   (FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE, replacing the HEAD same-TU auto_inline(off) stand-in body): VC7.1 then sees that it
//   only reads its argument and folds the default temporary's destructor (declaration-only gave 187v166 at HEAD). The copy binds
//   ??0?$vector@KV?$allocator@K@std@@@std@@QAE@ABV01@@Z at 0x00535470; its pick-any copy must equal 0x00535470 (pk.py).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "stlport/fable_map.h"

template CArray<unsigned long>& std::map<CCharString, CArray<unsigned long>, std::less<CCharString>,
    std::allocator<std::pair<const CCharString, CArray<unsigned long> > > >::operator[](const CCharString&);
