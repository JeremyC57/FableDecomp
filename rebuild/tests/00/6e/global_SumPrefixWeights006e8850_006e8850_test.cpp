#include <assert.h>
#include <stdio.h>
// BEGIN exact source snapshot SHA256 560da40e683a86305bf59376612fab2bc75948fcaafa95adc905f038cb55de05
// 006e8850 SumPrefixWeights_006e8850 (owner uncertain)
// Flags: /O2 /Oy
// Layout: retail vector +0/+4 bounds, stride 28 and weight +12 at 006e88a4.
struct WeightEntry { int data[3]; float weight; int tail[3]; };
struct WeightArray {
 WeightEntry *first,*last,*limit;
 __forceinline WeightEntry* Get(int index) const {return index>=0 && (unsigned)index<(unsigned)(last-first) ? first+index : 0;}
 float SumPrefixWeights(int index) const;
};
float WeightArray::SumPrefixWeights(int index) const {
 float sum=0.0f;
 for(int i=index;i>=0;--i) sum+=Get(i)->weight;
 return sum;
}

// END exact source snapshot
int main(){WeightEntry a[9];for(int i=0;i<9;++i)a[i].weight=(float)(i+1);WeightArray v={a,a+9,a+9};assert(v.SumPrefixWeights(-1)==0);assert(v.SumPrefixWeights(0)==1);assert(v.SumPrefixWeights(3)==10);assert(v.SumPrefixWeights(8)==45);puts("006e8850_TEST PASS");}