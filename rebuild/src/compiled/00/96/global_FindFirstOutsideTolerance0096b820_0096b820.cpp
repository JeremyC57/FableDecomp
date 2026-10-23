// 0096b820 FindFirstOutsideTolerance_0096b820 (owner uncertain)
// Flags: /O2 /Oy /Gr
// Leaf: FindFirstOutsideTolerance
#include <math.h>
#pragma intrinsic(fabs)
// Layout: retail 0096b840 reads float +8; iterator increments 12 bytes.
struct SearchEntry { int first, second; float value; };
struct WithinTolerance { float target, tolerance; bool operator()(const SearchEntry& e) const { return !(fabs(e.value-target)<tolerance); } };
struct SearchTag {};
template<class Iterator, class Predicate> Iterator __fastcall FindFirstOutsideTolerance(Iterator first, Iterator last, Predicate pred, SearchTag) {
 int trips=(last-first)>>2;
 for(; trips>0; --trips) {
  if(pred(*first)) return first; ++first;
  if(pred(*first)) return first; ++first;
  if(pred(*first)) return first; ++first;
  if(pred(*first)) return first; ++first;
 }
 switch(last-first) {
 case 3: if(pred(*first)) return first; ++first;
 case 2: if(pred(*first)) return first; ++first;
 case 1: if(pred(*first)) return first; ++first;
 default: return last;
 }
}
template SearchEntry* __fastcall FindFirstOutsideTolerance(SearchEntry*,SearchEntry*,WithinTolerance,SearchTag);