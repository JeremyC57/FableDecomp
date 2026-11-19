// Behaviour test for std::__adjust_heap<pair<unsigned __int64, list<C2DLineF> >*, int, pair<..>, CKeyPairCompareLess<..> > (0x00A82BE0):
// the STLport sift-down + push-up over the navigation layer's switchable line lists. Every heap size 1..8, every hole, every key pattern over
// three 64-bit keys chosen so that a signed compare, a low-dword-only or a high-dword-only compare all order them differently from the
// unsigned 64-bit one; each slot carries a list whose length and contents identify it. The result must equal a reference model (keys and
// list contents), and the lists must be deep copies (no shared nodes).
#include <stdio.h>
#include <stdlib.h>
#include <list>
#ifndef _MSC_VER
#define __int64 long long
#endif
typedef unsigned __int64 U64;
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
class C2DVector { public: float X, Y; };
class C2DLineF { public: C2DVector Point1, Point2; };
template<class T1,class T2> struct pair { T1 first; T2 second; };
typedef std::list<C2DLineF> L;
typedef pair<U64, L> E;
template<class K,class V> class CKeyPairCompareLess { public: bool operator()(const pair<K,V>& a,const pair<K,V>& b) const { return a.first<b.first; } };
typedef CKeyPairCompareLess<U64, L> C;
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
  _Distance __parent = (__holeIndex - 1) / 2;
  while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
    *(__first + __holeIndex) = *(__first + __parent);
    __holeIndex = __parent;
    __parent = (__holeIndex - 1) / 2;
  }
  *(__first + __holeIndex) = __val;
}
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
  _Distance __topIndex = __holeIndex;
  _Distance __secondChild = 2 * __holeIndex + 2;
  while (__secondChild < __len) {
    if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
      __secondChild--;
    *(__first + __holeIndex) = *(__first + __secondChild);
    __holeIndex = __secondChild;
    __secondChild = 2 * (__secondChild + 1);
  }
  if (__secondChild == __len) {
    *(__first + __holeIndex) = *(__first + (__secondChild - 1));
    __holeIndex = __secondChild - 1;
  }
  __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
// keys k=0<1<2 (unsigned 64-bit): 0x00000001_FFFFFFFF < 0x00000002_00000000 < 0x80000000_00000001.
// Low dword alone orders them 1,0,2 differently; a signed 64-bit compare puts key 2 first.
static U64 KEY(int k){ U64 v[3]; v[0]=((U64)1<<32)|0xFFFFFFFFu; v[1]=(U64)2<<32; v[2]=((U64)0x80000000u<<32)|1u; return v[k]; }
static void fill(L& l,int id){ l.clear(); for(int i=0;i<=id%4;++i){ C2DLineF f; f.Point1.X=(float)id; f.Point1.Y=(float)i; f.Point2.X=(float)(id*10+i); f.Point2.Y=-1.0f; l.push_back(f);} }
static bool same(const L& l,int id){ if((int)l.size()!=id%4+1) return false; int i=0; for(L::const_iterator it=l.begin();it!=l.end();++it,++i){ if(it->Point1.X!=(float)id||it->Point1.Y!=(float)i||it->Point2.X!=(float)(id*10+i)) return false;} return true; }
static void model(int* k,int* id,int hole,int len,int vk,int vid){
 int top=hole,sc=2*hole+2;
 while(sc<len){ if(k[sc]<k[sc-1]) sc--; k[hole]=k[sc]; id[hole]=id[sc]; hole=sc; sc=2*(sc+1); }
 if(sc==len){ k[hole]=k[sc-1]; id[hole]=id[sc-1]; hole=sc-1; }
 int parent=(hole-1)/2;
 while(hole>top && k[parent]<vk){ k[hole]=k[parent]; id[hole]=id[parent]; hole=parent; parent=(hole-1)/2; }
 k[hole]=vk; id[hole]=vid;
}
int main(){
 long cases=0;
 for(int len=1;len<=8;++len)for(int hole=0;hole<len;++hole){
  int combos=1; for(int i=0;i<len;++i) combos*=3;
  for(int c=0;c<combos;++c)for(int vk=0;vk<3;++vk){
   E arr[8]; int k[8],id[8]; int cc=c;
   for(int i=0;i<len;++i){ k[i]=cc%3; cc/=3; id[i]=i; arr[i].first=KEY(k[i]); fill(arr[i].second,i); }
   int vid=9;
   { E val; val.first=KEY(vk); fill(val.second,vid); __adjust_heap(arr,hole,len,val,C()); }
   model(k,id,hole,len,vk,vid);
   for(int i=0;i<len;++i){ check(arr[i].first==KEY(k[i])); check(same(arr[i].second,id[i])); }
   for(int i=0;i<len;++i)for(int j=i+1;j<len;++j) check(arr[i].second.empty()||&arr[i].second.front()!=&arr[j].second.front());
   ++cases;
  }
 }
 printf("%ld cases\n",cases);
 puts("00a82be0_TEST PASS"); return 0;
}