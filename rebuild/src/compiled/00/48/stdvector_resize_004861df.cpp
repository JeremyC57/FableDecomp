// std::vector<unsigned char>::resize(size_type n) -- STLport 4.6 `resize(n, _Tp())`: the zero byte built in the frame, its address
// and n passed to the out-of-line resize(size_type, const unsigned char&) 0x00486189
// Leaf: ?resize@?$vector@EV?$allocator@E@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004861DF, size TU with a frame pointer (`push ebp ; mov ebp,esp`), `ret 4`.
// Identity (NF104, 2026-11-19, queue q4767): Ego_r.pdb ?resize@?$vector@EV?$allocator@E@std@@@std@@QAEXI@Z (the one-argument resize),
//   bound so by the landed callers 0x0047F58B and 0x00597228; the body builds `unsigned char()` (0) in a frame slot and calls
//   0x00486189 = resize(size_type, const unsigned char&) with (n, &tmp). STLport 4.6.2, descriptive-by-library. Was the stand-in
//   `_Umove_EWeaponClass` (free __fastcall) calling `helper_486189`; now an explicit instantiation over stlport/fable_containers.h with
//   FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (the two-argument resize visible but out of line, as this TU calls it).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "stlport/fable_containers.h"
template void std::vector<unsigned char>::resize(std::vector<unsigned char>::size_type);
