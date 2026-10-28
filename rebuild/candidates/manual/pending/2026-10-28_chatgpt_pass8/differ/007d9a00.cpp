// std::iter_swap(CUnprovenSwapRecord* a, CUnprovenSwapRecord* b) -- exchange the complete16-byte values.
// Leaf: ??$iter_swap@PAVCUnprovenSwapRecord_007d9a00@@PAV1@@std@@YIXPAVCUnprovenSwapRecord_007d9a00@@0@Z
// Identity: descriptive; unreferenced 16-byte value swap, ECX/EDX receivers; no actual owning record/type or swap-versus-iter_swap role is proved. Catalog NearestBuilding constructor is false; address-specific descriptive record retained.
#ifndef _MSC_VER
#define __fastcall
#endif
class CUnprovenSwapRecord_007d9a00 { public: unsigned long UnprovenWords[4]; };
namespace std { template<class I1,class I2> void __fastcall iter_swap(I1 a,I2 b) { CUnprovenSwapRecord_007d9a00 temporary=*a; *a=*b; *b=temporary; } }
template void __fastcall std::iter_swap<CUnprovenSwapRecord_007d9a00*,CUnprovenSwapRecord_007d9a00*>(CUnprovenSwapRecord_007d9a00*,CUnprovenSwapRecord_007d9a00*);
