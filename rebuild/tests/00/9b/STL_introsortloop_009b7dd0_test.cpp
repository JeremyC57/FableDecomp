// 009b7dd0 _STL::__introsort_loop
// Flags: /Gr
// Leaf: __introsort_loop
// Layout: 12byte value, unsigned key+0 and8byte trivially copied payload+4;4byte comparator; actual payload aliases unknown.
namespace FableSort_009b7dd0 {
template<class K,class V> struct pair {K first;V second;pair(){}pair(const pair& p):first(p.first),second(p.second){}};
typedef pair<unsigned int,pair<unsigned int,unsigned int> > CSortValue;
struct CKeyPairCompareLess {int State;bool operator()(const CSortValue&a,const CSortValue&b)const{return a.first<b.first;}};
template<class T,class C> __forceinline const T& __median(const T&a,const T&b,const T&c,C comp){
 if(comp(a,b)){if(comp(b,c))return b;else if(comp(a,c))return c;else return a;}
 else if(comp(a,c))return a;else if(comp(b,c))return c;else return b;
}
CSortValue* __fastcall __unguarded_partition(CSortValue*,CSortValue*,CSortValue,CKeyPairCompareLess);
void __fastcall __partial_sort(CSortValue*,CSortValue*,CSortValue*,CSortValue*,CKeyPairCompareLess);
template<class T,class C> void __introsort_loop(T* first,T* last,T*,int depth,C comp){
 while(last-first>16){
  if(depth==0){__partial_sort(first,last,last,(T*)0,comp);return;}
  --depth;
  T* cut=__unguarded_partition(first,last,T(__median(*first,*(first+(last-first)/2),*(last-1),comp)),comp);
  __introsort_loop(cut,last,(T*)0,depth,comp);last=cut;
 }
}
template void __introsort_loop<CSortValue,CKeyPairCompareLess>(CSortValue*,CSortValue*,CSortValue*,int,CKeyPairCompareLess);
}

#include <stdio.h>
#include <assert.h>
#include <algorithm>
namespace FableSort_009b7dd0 {
CSortValue* __fastcall __unguarded_partition(CSortValue* first,CSortValue* last,CSortValue pivot,CKeyPairCompareLess comp){for(;;){while(comp(*first,pivot))++first;--last;while(comp(pivot,*last))--last;if(!(first<last))return first;CSortValue value=*first;*first=*last;*last=value;++first;}}
void __fastcall __partial_sort(CSortValue* first,CSortValue* middle,CSortValue* last,CSortValue*,CKeyPairCompareLess comp){std::partial_sort(first,middle,last,comp);}
}
int main(){using namespace FableSort_009b7dd0;CSortValue values[40];CKeyPairCompareLess comp;comp.State=0;
for(int i=0;i<40;++i){values[i].first=39-i;values[i].second.first=i+100;values[i].second.second=i+200;}
__introsort_loop(values,values,(CSortValue*)0,6,comp);__introsort_loop(values,values+1,(CSortValue*)0,6,comp);
__introsort_loop(values,values+40,(CSortValue*)0,6,comp);
for(int i=0;i<40;++i){assert(values[i].first+values[i].second.first==139 && values[i].first+values[i].second.second==239);for(int j=i+16;j<40;++j)assert(!comp(values[j],values[i]));}
__introsort_loop(values,values+40,(CSortValue*)0,0,comp);for(int i=1;i<40;++i)assert(!comp(values[i],values[i-1]));puts("009b7dd0_TEST PASS");return 0;}