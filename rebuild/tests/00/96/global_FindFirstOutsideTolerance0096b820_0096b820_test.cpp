#include <assert.h>
#include <stdio.h>
// BEGIN exact source snapshot SHA256 7e151de4ab62edd572240984af0b81ac673664678c11ab89404a71459c9faccc
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

// END exact source snapshot
int main(){SearchEntry a[9];for(int i=0;i<9;++i)a[i].value=4.0f;WithinTolerance p={4.0f,0.5f};SearchTag tag;assert(FindFirstOutsideTolerance(a,a,p,tag)==a);assert(FindFirstOutsideTolerance(a,a+9,p,tag)==a+9);for(int n=1;n<=9;++n){a[n-1].value=5.0f;assert(FindFirstOutsideTolerance(a,a+n,p,tag)==a+n-1);a[n-1].value=4.0f;}p.tolerance=0;assert(FindFirstOutsideTolerance(a,a+9,p,tag)==a);puts("0096b820_TEST PASS");}