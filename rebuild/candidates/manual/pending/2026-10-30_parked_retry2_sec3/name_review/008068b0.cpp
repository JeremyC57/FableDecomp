// std::iter_swap(pair<C3DVector, long>* left, pair<C3DVector, long>* right) -- exchange two (position, value) pairs
// Grade: MATCH -- VC7.1 Dinkumware iter_swap -> swap over pair<C3DVector, long> with C3DVector's user copy constructor (three words of the temporary to the frame, the long in eax), /Gr
// NAME REVIEW: byte-identical twin of 0x00806220 (same PDB instantiation); one address = one name, so this row is held
//   for the coordinator (a second copy of the same COMDAT in another link-time-codegen TU, or a different pair<C3DVector,X>).
// Flags: /Gr
// Leaf: ??$iter_swap@PAU?$pair@VC3DVector@@J@std@@PAU12@@std@@YIXPAU?$pair@VC3DVector@@J@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008068B0, 81 bytes (the manifest extent runs past the int3 padding into the next
//   function: BOUNDS_FIX.tsv), __fastcall (left in ecx, right in edx), plain `ret`. No direct E8/E9 callers.
// Identity: Ego_r.pdb ??$iter_swap@PAU?$pair@VC3DVector@@J@std@@PAU12@@std@@YIXPAU?$pair@VC3DVector@@J@0@0@Z, the only
//   iter_swap over a 16-byte pair whose first member is a C3DVector (the bytes copy the C3DVector words through the frame
//   and the fourth word in a register, so the vector comes first); its family (_Adjust_heap / _Make_heap / _Insertion_sort
//   / _Median over pair<C3DVector,long> with CCompareTargetedPositionsForFirstPersonTargeting) is the 0x00807620
//   _Push_heap TU. The manifest label CNearestBuildingWithProperty (a 24-byte class) does not fit; the parked
//   CUnprovenSwapRecord model (four unsigned words) kept the temporary in registers.
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> inline void swap(T& left, T& right) { T temporary = left; left = right; right = temporary; }
template <class I1, class I2> inline void iter_swap(I1 left, I2 right) { std::swap(*left, *right); }
}
template void std::iter_swap<std::pair<C3DVector, long>*, std::pair<C3DVector, long>*>(std::pair<C3DVector, long>*, std::pair<C3DVector, long>*);
