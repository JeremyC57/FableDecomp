// std::iter_swap(pair<C3DVector, long>* left, pair<C3DVector, long>* right) -- exchange two (position, value) pairs
// Grade: MATCH -- VC7.1 Dinkumware iter_swap -> swap over pair<C3DVector, long> with C3DVector's user copy constructor (three words of the temporary to the frame, the long in eax), /Gr
// Flags: /Gr
// Leaf: ??$iter_swap@PAU?$pair@VC3DVector@@J@std@@PAU12@@std@@YIXPAU?$pair@VC3DVector@@J@0@0@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00806220, 81 bytes (the manifest extent runs past the int3 padding into the next
//   function: BOUNDS_FIX.tsv), __fastcall (left in ecx, right in edx), plain `ret`. No direct E8/E9 callers.
// Identity: Ego_r.pdb ??$iter_swap@PAU?$pair@VC3DVector@@J@std@@PAU12@@std@@YIXPAU?$pair@VC3DVector@@J@0@0@Z, the only
//   iter_swap over a 16-byte pair whose first member is a C3DVector (the bytes copy the C3DVector words through the frame
//   and the fourth word in a register, so the vector comes first); its family (_Adjust_heap / _Make_heap / _Insertion_sort
//   / _Median over pair<C3DVector,long> with CCompareTargetedPositionsForFirstPersonTargeting) is the 0x00807620
//   _Push_heap TU. The manifest label CNearestBuildingWithProperty (a 24-byte class) does not fit; the parked
//   CUnprovenSwapRecord model (four unsigned words) kept the temporary in registers.
#include "engine/C3DVector.h"
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> inline void swap(T& left, T& right) { T temporary = left; left = right; right = temporary; }
template <class I1, class I2> inline void iter_swap(I1 left, I2 right) { std::swap(*left, *right); }
}
template void std::iter_swap<std::pair<C3DVector, long>*, std::pair<C3DVector, long>*>(std::pair<C3DVector, long>*, std::pair<C3DVector, long>*);