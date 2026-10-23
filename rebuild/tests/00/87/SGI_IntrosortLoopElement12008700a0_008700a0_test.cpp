#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 33b036deab132a209f5e73dd29c23d03f4baa0f474246d41a32e3070330d6b6c
// 008700a0 SGI::IntrosortLoop_Element12_008700a0
// Flags: /O2 /Oy /Gr
// Leaf: IntrosortLoop
// Layout: retail element stride12; compare object4 bytes; introsort threshold16, depth argument and dummy type pointer.
struct Element_008700a0{int key,a,b;Element_008700a0(){}Element_008700a0(const Element_008700a0& p):key(p.key),a(p.a),b(p.b){}};
struct Compare_008700a0{int state;bool operator()(const Element_008700a0&a,const Element_008700a0&b)const{return a.key<b.key;}};
template<class T,class C> __forceinline const T& Median(const T&a,const T&b,const T&c,C comp){
 if(comp(a,b)){if(comp(b,c))return b;else if(comp(a,c))return c;else return a;}
 else if(comp(a,c))return a;else if(comp(b,c))return c;else return b;
}
Element_008700a0* __fastcall Partition(Element_008700a0*,Element_008700a0*,Element_008700a0,Compare_008700a0);
void __fastcall PartialSort(Element_008700a0*,Element_008700a0*,Element_008700a0*,Element_008700a0*,Compare_008700a0);
template<class T,class C> void IntrosortLoop(T* first,T* last,T*,int depth,C comp){
 while(last-first>16){
  if(depth==0){PartialSort(first,last,last,(T*)0,comp);return;}
  --depth;
  T* cut=Partition(first,last,T(Median(*first,*(first+(last-first)/2),*(last-1),comp)),comp);
  IntrosortLoop(cut,last,(T*)0,depth,comp);last=cut;
 }
}
template void IntrosortLoop<Element_008700a0,Compare_008700a0>(Element_008700a0*,Element_008700a0*,Element_008700a0*,int,Compare_008700a0);

// END exact source snapshot
Element_008700a0* __fastcall Partition(Element_008700a0* first,Element_008700a0* last,Element_008700a0 pivot,Compare_008700a0 comp){for(;;){while(comp(*first,pivot))++first;--last;while(comp(pivot,*last))--last;if(!(first<last))return first;Element_008700a0 value=*first;*first=*last;*last=value;++first;}}
void __fastcall PartialSort(Element_008700a0* first,Element_008700a0*,Element_008700a0* last,Element_008700a0*,Compare_008700a0 comp){for(Element_008700a0* i=first;i!=last;++i)for(Element_008700a0* j=i;j!=last;++j)if(comp(*j,*i)){Element_008700a0 temp=*i;*i=*j;*j=temp;}}
int main(){Element_008700a0 values[40];int keys[40];Compare_008700a0 comp;comp.state=0;for(int i=0;i<40;++i){keys[i]=39-i;values[i].key=keys[i];values[i].a=0;values[i].b=0;}
 IntrosortLoop(values,values,(Element_008700a0*)0,6,comp);IntrosortLoop(values,values+1,(Element_008700a0*)0,6,comp);
 IntrosortLoop(values,values+40,(Element_008700a0*)0,6,comp);
 for(int i=0;i<40;++i)for(int j=i+16;j<40;++j)assert(!comp(values[j],values[i]));
 IntrosortLoop(values,values+40,(Element_008700a0*)0,0,comp);for(int i=1;i<40;++i)assert(!comp(values[i],values[i-1]));
 puts("008700a0_TEST PASS");return 0;}