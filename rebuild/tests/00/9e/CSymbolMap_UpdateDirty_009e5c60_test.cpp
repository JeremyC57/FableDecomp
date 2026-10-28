#include <stdio.h>
#include <string.h>
// CSymbolMap::UpdateDirty()const -- Sort pending map entries before lookup.
// Leaf: ?UpdateDirty@CSymbolMap@@QBEXXZ
// Identity: Own TransferIn009e2590 and canonical binding prove CSymbolMap::UpdateDirty()const; original nested CSymbolInfo GroupName/EntryName/Val gives16-byte pair. Own RTTI129c158 -> CBaseClassNonCopyable -> CBaseClass. Sort009e7e40.
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
class CSymbolMap:public CBaseClassNonCopyable{public:class CSymbolInfo{public:CCharString GroupName,EntryName;long Val;};mutable CVectorMap<CCharString,CSymbolInfo,CCompareString<CSymbolInfo> > SLONGMap;void UpdateDirty()const;};
void CSymbolMap::UpdateDirty()const{SLONGMap.Sort();}

typedef std::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;typedef CCompareString<CSymbolMap::CSymbolInfo> Compare;

class CCharStringData{public:const char* Text;};bool CCharString::FastCompare(const CCharString& o)const{return strcmp(PStringData->Text,o.PStringData->Text)<0;}
static Entry* expectedFirst;static Entry* expectedLast;static int sorts,bad;static bool* dirty;
namespace std{template<class Iter,class Cmp>void __fastcall sort(Iter first,Iter last,Cmp cmp){++sorts;if(first!=expectedFirst||last!=expectedLast||!*dirty)bad=1;for(Iter i=first;i!=last;++i){Iter j=i;while(j!=first&&cmp(*j,*(j-1))){Entry value=*j;*j=*(j-1);*(j-1)=value;--j;}}}}
int main(){CSymbolMap owner;Entry values[3];CCharStringData names[3];names[0].Text="c";names[1].Text="a";names[2].Text="b";for(int i=0;i<3;++i){values[i].first.PStringData=&names[i];memset(&values[i].second,0,sizeof(values[i].second));} owner.SLONGMap.Data._M_start=values;owner.SLONGMap.Data._M_finish=values+3;owner.SLONGMap.Data._M_end_of_storage._M_data=values+3;owner.SLONGMap.Dirty=false;expectedFirst=values;expectedLast=values+3;dirty=&owner.SLONGMap.Dirty;owner.UpdateDirty();bool ok=sorts==0;owner.SLONGMap.Dirty=true;owner.UpdateDirty();Compare cmp;ok=ok&&sorts==1&&!owner.SLONGMap.Dirty&&!cmp(values[1],values[0])&&!cmp(values[2],values[1]);owner.UpdateDirty();ok=ok&&sorts==1;owner.SLONGMap.Data._M_finish=values;expectedLast=values;owner.SLONGMap.Dirty=true;owner.UpdateDirty();ok=ok&&sorts==2&&!owner.SLONGMap.Dirty&&!bad;puts(ok?"009e5c60_TEST PASS":"009e5c60_TEST FAIL");return ok?0:1;}