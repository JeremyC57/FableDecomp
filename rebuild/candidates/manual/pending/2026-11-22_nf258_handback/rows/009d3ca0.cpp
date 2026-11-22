// std::_Construct<pair<const CCharString, CArray<unsigned long> >, pair<const CCharString, CArray<unsigned long> > >(p, value) -- STLport 4.6 placement copy-construction of a CBankFile symbol-map node value
// Leaf: ??$_Construct@U?$pair@$$CBVCCharString@@V?$CArray@K@@@std@@U12@@std@@YIXPAU?$pair@$$CBVCCharString@@V?$CArray@K@@@0@ABU10@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3CA0, 31 bytes (`__fastcall`: p in ecx, the value in edx; `ret`).
// Identity: STLport 4.6 _construct.h _Construct(p, v) = `new (p) T(v)` over pair<const CCharString, CArray<unsigned long> >: the
//   placement-new null test, then CCharString(const CCharString&) 0x0099EC30 on +0 and CArray<unsigned long>'s inline copy, i.e.
//   vector<unsigned long>'s copy constructor 0x00535470, on +4. The value type is the map<CCharString, CArray<unsigned long> > of
//   CBankFile::WriteSymbolFile (0x009D4810, both PDBs); both PDBs also carry the Dinkumware construct helper for this exact pair
//   (`_Cons_val<allocator<pair<CCharString const, CArray<unsigned long> > >, ...>`, the catalog's label). Descriptive-by-library: no PDB
//   holds STLport instances. No retail code calls or references it (no E8/E9 site, no absolute reference). The 0x007AE0C0 recipe.
// NF258 (2026-11-22, L37): moved off the July row-local PairT / CArrayUL / CCharString models (`?Cons_val_pair@@YIXPAUPairT@@0@Z`,
//   binding the stand-ins `?copy@CCharString@@QAEXPBV1@@Z` and `?copy_00535470@CArrayUL@@QAEXPBU1@@Z`) onto fable_string.h /
//   engine/CArray.h / stlport/fable_containers.h (FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL); explicit instantiation. The vector copy
//   constructor is declaration-only (FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY), so +4 binds ??0?$vector@KV?$allocator@K@std@@@std@@QAE@ABV01@@Z.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CArray.h"
namespace std {
template void __fastcall _Construct<pair<CCharString const, CArray<unsigned long> >, pair<CCharString const, CArray<unsigned long> > >(
    pair<CCharString const, CArray<unsigned long> >*, const pair<CCharString const, CArray<unsigned long> >&);
}
