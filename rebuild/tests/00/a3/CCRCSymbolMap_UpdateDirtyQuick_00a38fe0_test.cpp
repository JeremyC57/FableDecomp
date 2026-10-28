#include <stdio.h>
#include <string.h>
// CCRCSymbolMap::UpdateDirtyQuick()const -- Sort pending map entries before lookup.
// Leaf: ?UpdateDirtyQuick@CCRCSymbolMap@@QBEXXZ
// Identity: PDB original UpdateDirtyQuick()const; adjacent UpdateDirty00a38fb0 also trims through00a39a00, this quick form only sorts. SLONGMap+4 pair<ulong,long>, own RTTI129cf84 original base chain. Sort009b8520.
// Layout: original polymorphic bases and STLport containers; no active owner header. CVectorMap Data12, empty Compare byte+12, Dirty+13.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
class CBaseClass{public:virtual ~CBaseClass(){}};class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable(){}~CBaseClassNonCopyable(){}private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);};
class CCharStringData;class CCharString{public:CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
// Fixture access only: expose protected storage for existing initialization/checks; original fields, bases and candidate body unchanged.
namespace std{
template<class T>class allocator{};template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T,class A>class _List_base{public:typedef _List_node<T> _Node;typedef allocator<_Node> _NodeAlloc;_STLP_alloc_proxy<_Node*,_Node,_NodeAlloc> _M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{};
template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}};
template<class Iter,class Compare>void __fastcall sort(Iter,Iter,Compare);
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;Cmp Compare;bool Dirty;__forceinline void Sort(){if(Dirty){std::sort(Data.begin(),Data.end(),Compare);Dirty=false;}}};
class CCRCSymbolMap:public CBaseClassNonCopyable{public:mutable CVectorMap<unsigned long,long> SLONGMap;void UpdateDirtyQuick()const;};
void CCRCSymbolMap::UpdateDirtyQuick()const{SLONGMap.Sort();}

typedef std::pair<unsigned long,long> Entry;typedef CKeyPairCompareLess<unsigned long,long> Compare;

class CCharStringData{public:const char* Text;};bool CCharString::FastCompare(const CCharString& o)const{return strcmp(PStringData->Text,o.PStringData->Text)<0;}
static Entry* expectedFirst;static Entry* expectedLast;static int sorts,bad;static bool* dirty;
namespace std{template<class Iter,class Cmp>void __fastcall sort(Iter first,Iter last,Cmp cmp){++sorts;if(first!=expectedFirst||last!=expectedLast||!*dirty)bad=1;for(Iter i=first;i!=last;++i){Iter j=i;while(j!=first&&cmp(*j,*(j-1))){Entry value=*j;*j=*(j-1);*(j-1)=value;--j;}}}}
int main(){CCRCSymbolMap owner;Entry values[3];values[0].first=3;values[1].first=1;values[2].first=2;for(int i=0;i<3;++i)values[i].second=i+7; owner.SLONGMap.Data._M_start=values;owner.SLONGMap.Data._M_finish=values+3;owner.SLONGMap.Data._M_end_of_storage._M_data=values+3;owner.SLONGMap.Dirty=false;expectedFirst=values;expectedLast=values+3;dirty=&owner.SLONGMap.Dirty;owner.UpdateDirtyQuick();bool ok=sorts==0;owner.SLONGMap.Dirty=true;owner.UpdateDirtyQuick();Compare cmp;ok=ok&&sorts==1&&!owner.SLONGMap.Dirty&&!cmp(values[1],values[0])&&!cmp(values[2],values[1]);owner.UpdateDirtyQuick();ok=ok&&sorts==1;owner.SLONGMap.Data._M_finish=values;expectedLast=values;owner.SLONGMap.Dirty=true;owner.UpdateDirtyQuick();ok=ok&&sorts==2&&!owner.SLONGMap.Dirty&&!bad;puts(ok?"00a38fe0_TEST PASS":"00a38fe0_TEST FAIL");return ok?0:1;}