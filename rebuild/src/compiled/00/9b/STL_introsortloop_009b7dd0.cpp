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