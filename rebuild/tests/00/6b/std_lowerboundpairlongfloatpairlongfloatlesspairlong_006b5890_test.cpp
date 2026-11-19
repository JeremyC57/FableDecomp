// Behaviour test for std::__lower_bound<pair<long,float>*, pair<long,float>, less<pair<long,float> >, int> (0x006B5890): the STLport body
// (distance / advance / comp(*middle, val)) with the pair operator< (first, then second), against a linear reference over sorted arrays.
// Kills: the wrong half (upper_bound semantics), comp(val, *middle), the second member ignored or compared reversed, len update off by one.
#include <stdio.h>
#include <stdlib.h>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
template<class A,class B> struct pair { A first; B second; };
template<class A,class B> inline bool operator<(const pair<A,B>& x,pair<A,B> y)
{return x.first<y.first || (!(y.first<x.first) && x.second<y.second);}
template<class T> struct less { bool operator()(const T& x,const T& y) const { return x<y; } };
template<class I,class D> inline void advance(I& i,D n){i+=n;}
template<class I> inline int distance(const I& a,const I& b){return (int)(b-a);}
template<class _ForwardIter,class _Tp,class _Compare,class _Distance>
_ForwardIter __lower_bound(_ForwardIter first,_ForwardIter last,const _Tp& value,_Compare comp,_Distance*){
    _Distance len=distance(first,last);
    _Distance half;
    _ForwardIter middle;
    while(len>0){
        half=len>>1;
        middle=first;
        advance(middle,half);
        if(comp(*middle,value)){
            first=middle;
            ++first;
            len=len-half-1;
        }else len=half;
    }
    return first;
}
typedef pair<long,float> P;
int main(){
 P a[16];long cases=0;
 for(int n=0;n!=12;++n){
  for(int i=0;i!=n;++i){a[i].first=(i/3)-1;a[i].second=(float)(i%3)*0.5f;}   // sorted by (first, second)
  for(long f=-3;f!=5;++f)for(int s=0;s!=5;++s){
   P v;v.first=f;v.second=(float)s*0.25f;
   P* got=__lower_bound(a,a+n,v,less<P>(),(int*)0);
   int want=0;while(want<n&&(a[want].first<v.first||(a[want].first==v.first&&a[want].second<v.second)))++want;
   check(got==a+want);++cases;
  }
 }
 check(cases==480);puts("006b5890_TEST PASS");return 0;
}