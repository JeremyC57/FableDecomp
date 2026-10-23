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