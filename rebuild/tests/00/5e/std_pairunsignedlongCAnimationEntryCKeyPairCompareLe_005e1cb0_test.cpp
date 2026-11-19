// Behaviour test for std::__adjust_heap<pair<unsigned long, CAnimationEntry>*, ...> (0x005E1CB0): the STLport sift-down + push-up of the
// animation table entries (unsigned key; the entry's bank index and two def-string words copied, its CIVCountedPointer<CAnimComponentMap>
// with the counted pointer's assignment semantics). Adapted from the landed 0x005E1210 test. Every heap size 1..9, every hole, every key pattern over 3 values: the result
// must equal a reference model (array contents and pointer identity), every counted object's reference count must equal the number of
// pointers to it, and no object may be deleted. Kills: compare direction, the `--secondChild` choice, the `secondChild == len` bottom
// case, a missing AddRef / Release (counts and deletions with and without outside references), the topIndex bound of the push. (The
// operator='s same-pointer early return is unobservable here: the algorithm never assigns a slot to itself.)
#include <stdio.h>
#include <stdlib.h>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
static int deletes;
class CAnimComponentMap { public: unsigned long IVRefCount; int id;
 long Release() const { --const_cast<CAnimComponentMap*>(this)->IVRefCount; if (IVRefCount==0) { ++deletes; return 0; } else { return IVRefCount; } }
 void AddRef() const { ++const_cast<CAnimComponentMap*>(this)->IVRefCount; } };
template<class T> class CIVCountedPointer { public:
 T* Object;
 CIVCountedPointer():Object(0){}
 CIVCountedPointer(const CIVCountedPointer& o):Object(o.Object){ if (Object) Object->AddRef(); }
 ~CIVCountedPointer(){ Release(); }
 T* InternalGetObject() const { return Object; }
 void Release(){ if (Object) { Object->Release(); Object = 0; } }
 CIVCountedPointer& operator=(const CIVCountedPointer& other){
  if (InternalGetObject() == other.InternalGetObject()) return *this;
  Release(); Object = other.Object; if (Object) Object->AddRef(); return *this; } };
template<class T1,class T2> struct pair { T1 first; T2 second; pair(){} pair(const pair& p):first(p.first),second(p.second){} };
class CDefString { public: long TablePos; };
class CAnimationEntry { public: long BankIndex; CDefString AnimName, GroupName; CIVCountedPointer<CAnimComponentMap> Components; };
typedef pair<unsigned long, CAnimationEntry> E;
template<class K,class V> class CKeyPairCompareLess { public: bool operator()(const pair<K,V>& a,const pair<K,V>& b) const { return a.first<b.first; } };
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> C;
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
// reference model on plain (key, id) arrays
// keys 0 < 1 < 2 map to 5 < 0x80000000 < 0xF0000000: an unsigned compare (a signed one would order them differently)
static unsigned long KEY(int k){ static const unsigned long v[3]={5UL,0x80000000UL,0xF0000000UL}; return v[k]; }
static void model(int* k,int* id,int hole,int len,int vk,int vid){
 int top=hole,sc=2*hole+2;
 while(sc<len){ if(k[sc]<k[sc-1]) sc--; k[hole]=k[sc]; id[hole]=id[sc]; hole=sc; sc=2*(sc+1); }
 if(sc==len){ k[hole]=k[sc-1]; id[hole]=id[sc-1]; hole=sc-1; }
 int parent=(hole-1)/2;
 while(hole>top && k[parent]<vk){ k[hole]=k[parent]; id[hole]=id[parent]; hole=parent; parent=(hole-1)/2; }
 k[hole]=vk; id[hole]=vid;
}
int main(){
 long cases=0; CAnimComponentMap obj[12];
 for(int len=1;len<=9;++len)for(int hole=0;hole<len;++hole){
  int combos=1; for(int i=0;i<len;++i) combos*=3; if(combos>2187) combos=2187;
  for(int c=0;c<combos;++c)for(int vk=0;vk<3;++vk)for(int same=0;same<2;++same)for(unsigned long base=0;base<=1000;base+=1000){
   for(int i=0;i<12;++i){ obj[i].IVRefCount=base; obj[i].id=i; }
   E arr[9]; int k[9],id[9]; int cc=c;
   for(int i=0;i<len;++i){ k[i]=cc%3; cc/=3; id[i]=(i%3==2&&same)?0:i; arr[i].first=KEY(k[i]); arr[i].second.BankIndex=100+id[i]; arr[i].second.AnimName.TablePos=200+id[i]; arr[i].second.GroupName.TablePos=300+id[i]; arr[i].second.Components.Object=&obj[id[i]]; obj[id[i]].AddRef(); }
   int vid=same?id[hole]:10;
   { E val; val.first=KEY(vk); val.second.BankIndex=100+vid; val.second.AnimName.TablePos=200+vid; val.second.GroupName.TablePos=300+vid; val.second.Components.Object=&obj[vid]; obj[vid].AddRef();
     deletes=0; __adjust_heap(arr,hole,len,val,C()); }
   model(k,id,hole,len,vk,vid);
   unsigned long count[12]={0};
   for(int i=0;i<len;++i){ check(arr[i].first==KEY(k[i])); check(arr[i].second.Components.Object==&obj[id[i]]); check(arr[i].second.BankIndex==100+id[i] && arr[i].second.AnimName.TablePos==200+id[i] && arr[i].second.GroupName.TablePos==300+id[i]); ++count[id[i]]; }
   int want_deletes=0;
   { bool init[12]={false}; for(int i=0;i<len;++i) init[(i%3==2&&same)?0:i]=true; init[vid]=true; for(int i=0;i<12;++i) if(init[i]&&base==0&&count[i]==0) ++want_deletes; }
   check(deletes==want_deletes);
   for(int i=0;i<12;++i) check(obj[i].IVRefCount==base+count[i]);
   for(int i=0;i<len;++i) arr[i].second.Components.Object=0;
   ++cases;
  }
 }
 printf("%ld cases\n",cases);
 puts("005e1cb0_TEST PASS"); return 0;
}