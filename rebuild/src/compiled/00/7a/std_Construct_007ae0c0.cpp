// std::_Construct<pair<const CCharString, vector<CCharString> >, pair<const CCharString, vector<CCharString> > >(p, value) -- STLport 4.6 placement copy-construction of a gossip-map node value
// Leaf: ??$_Construct@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@U12@@std@@YIXPAU?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@0@ABU10@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AE0C0, 31 bytes (`__fastcall`: p in ecx, the value in edx; `ret`).
// Identity: the body is STLport 4.6 _construct.h _Construct(p, v) = `new (p) T(v)` over pair<const CCharString, vector<CCharString> >: the
//   placement-new null test, then CCharString(const CCharString&) 0x0099EC30 on +0 and vector<CCharString>'s copy constructor 0x007ACEA0
//   (q11134) on +4. Descriptive-by-library: no PDB holds STLport instances (the catalog's Dinkumware _Cons_val<.. CArray<unsigned long> ..>
//   label has the wrong value type; the second member is the vector the callee proves). No retail code calls or references it.
// NF87: moved off the row-local Pair / CArrayUL / CCharString models onto stlport/fable_containers.h (overlay opt-in
//   FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL), so 0x007ACEA0 binds the STLport vector copy-ctor spelling; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL
#include "stlport/fable_containers.h"
#include "fable_string.h"
namespace std {
template void __fastcall _Construct<pair<CCharString const, vector<CCharString, allocator<CCharString> > >, pair<CCharString const, vector<CCharString, allocator<CCharString> > > >(pair<CCharString const, vector<CCharString, allocator<CCharString> > >*, const pair<CCharString const, vector<CCharString, allocator<CCharString> > >&);
}
