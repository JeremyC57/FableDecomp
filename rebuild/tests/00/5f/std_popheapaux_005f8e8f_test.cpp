// Behavior fixture: exact auxiliary body; traceable heap callee and portable standard pair.
#include <stddef.h>
namespace std {
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair():first(_T1()),second(_T2()){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
    template<class U1,class U2> pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
};
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef ptrdiff_t difference_type; };
}
#include <assert.h>
#include <stdio.h>
#ifdef _MSC_VER
#define FABLE_STLP_FASTCALL __fastcall
#else
#define FABLE_STLP_FASTCALL
#endif

template<class K,class V> class CKeyPairCompareLess {
public:
 bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
static void* ExpectedFirst;
static void* ExpectedLast;
static int Calls;
namespace std {
template<class I,class T,class C,class D>
void FABLE_STLP_FASTCALL __pop_heap(I first,I last,I result,T val,C comp,D* distance) {
 assert(first==ExpectedFirst && last==ExpectedLast && result==last);
 assert(distance==0);assert(val.first==last->first && val.second==last->second);
 ++Calls;
 *result=*first;
 if(first==last)return;
 D count=D(last-first),hole=0;
 while(hole*2+1<count) {
  D child=hole*2+1;
  if(child+1<count && comp(first[child],first[child+1]))++child;
  if(!comp(val,first[child]))break;
  first[hole]=first[child];hole=child;
 }
 first[hole]=val;
}
template<class _RandomAccessIterator,class _Tp,class _Compare>
inline void FABLE_STLP_FASTCALL __pop_heap_aux(_RandomAccessIterator __first,
                 _RandomAccessIterator __last,_Tp*,_Compare __comp) {
    __pop_heap(__first,__last-1,__last-1,_Tp(*(__last-1)),__comp,
               (typename iterator_traits<_RandomAccessIterator>::difference_type*)0);
}
}
int main() {
 typedef std::pair<long,long> E;
 const long sets[5][7]={{9,8,7,6,5,4,3},{-1,-2,-3,-4,-5,-6,-7},
                      {4,4,4,4,4,4,4},{10,7,9,2,6,1,8},{0,0,-1,-1,0,-2,-1}};
 for(int pattern=0;pattern<5;++pattern)for(int count=1;count<=7;++count) {
  E storage[9];E* a=storage+1;E original[7];
  storage[0]=E(123456,long(0));storage[8]=E(-123456,long(0));
  for(int i=0;i<count;++i){a[i]=E(sets[pattern][i],long(i+1));original[i]=a[i];}
  ExpectedFirst=a;ExpectedLast=a+count-1;Calls=0;
  std::__pop_heap_aux(a,a+count,(E*)0,CKeyPairCompareLess<long,long>());
  assert(Calls==1);assert(a[count-1].first==original[0].first && a[count-1].second==original[0].second);
  for(int j=1;j<count-1;++j)assert(a[(j-1)/2].first>=a[j].first);
  for(int j=0;j<count;++j){int seen=0;for(int k=0;k<count;++k)
   if(a[k].first==original[j].first && a[k].second==original[j].second)++seen;assert(seen==1);}
  assert(storage[0].first==123456 && storage[8].first==-123456);
 }
 puts("005f8e8f_TEST PASS");return 0;
}