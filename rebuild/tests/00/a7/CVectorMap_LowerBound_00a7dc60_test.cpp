#include <stdio.h>
// CVectorMap::LowerBound(const RetailUID& key) -- Return the first entry not less than an unsigned 64-bit key.
// Leaf: ?LowerBound@?$CVectorMap@_KPAVCANavQuadTreeNode@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@@QAEPAU?$pair@_KPAVCANavQuadTreeNode@@@std@@AB_K@Z
// Identity: Own CNavQuadTree::SetSwitchableNodesAsBlocked00a79240 and SwitchableNodes+64 prove mutable map<UID,CANavQuadTreeNode*>; prior pair class-key V corrected to original struct U.
// Layout: original CArray/STLport vector12 with pair16, 8-byte-aligned UID. Comparer+12/Dirty+13.
#ifdef _MSC_VER
typedef unsigned __int64 RetailUID;
#else
typedef unsigned long RetailUID;
#endif
class CANavQuadTreeNode;class CActiveEntityScriptBase;class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
namespace std{template<class A,class B>struct pair{A first;B second;};template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& item,const K& key)const{return item.first<key;}};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap{public:typedef std::pair<K,V> value_type;typedef value_type* iterator;typedef const value_type* const_iterator;CArray<value_type> Data;C Compare;bool Dirty;iterator LowerBound(const K& key){iterator first=Data._M_start;int count=Data._M_finish-first;C comp;while(count>0){int half=count/2;iterator middle=first+half;if(comp(*middle,key)){first=middle+1;count-=half+1;}else count=half;}return first;}};
typedef CVectorMap<RetailUID,CANavQuadTreeNode* > Map;
template Map::iterator Map::LowerBound(const RetailUID&);

int main(){Map map;Map::value_type entries[6];RetailUID high=RetailUID(1)<<32;RetailUID top=RetailUID(1)<<63;RetailUID keys[6]={0,high-1,high,high,top,~RetailUID(0)};for(int i=0;i<6;++i)entries[i].first=keys[i];map.Data._M_start=entries;map.Data._M_finish=entries+6;map.Data._M_end_of_storage._M_data=entries+6;RetailUID probes[9]={0,1,high-1,high,high+1,top-1,top,top+1,~RetailUID(0)};int wants[9]={0,1,1,2,4,4,4,5,5};bool ok=true;for(int j=0;j<9;++j)ok=ok&&map.LowerBound(probes[j])==entries+wants[j];map.Data._M_finish=entries+4;ok=ok&&map.LowerBound(top)==entries+4;map.Data._M_finish=entries;ok=ok&&map.LowerBound(top)==entries;puts(ok?"00a7dc60_TEST PASS":"00a7dc60_TEST FAIL");return ok?0:1;}