// CThingSearchTools::GetAllThings(const Filter& filter,Container& result) const -- Append scan-visible objects accepted by the filter.
// Leaf: ??$GetAllThings@VCThingObject@@V?$list@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@VCThingFilter_HasTC@@@CThingSearchTools@@QBEXABVCThingFilter_HasTC@@AAV?$list@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Identity: original PDB CThingObject specialization and actual list<CThing*> output; own mask0x807 and filter006a80a0 prove instance. Propagated CThingMarker/vector label is wrong.
#include <new>
#include <stdlib.h>
#include "engine/CClass.h"
#define CThingSearchTools CThingSearchTools_HeaderLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
// Layout: real CClass prefix from shared header; CThing header quarantined. ThingTypeFlags+0x6c, state byte+0x91 from retail; original scanner flag meanings retained.
class CThing:public CClass {public:unsigned char Pad11[0x5b];unsigned long ThingTypeFlags;unsigned char Pad70[0x21];unsigned char StateBits;
bool IsIncludedInThingScans()const{unsigned char f=StateBits;unsigned char nf=~f;if((f&4)&&(f&8)&&(nf&1)&&!(f&0x20))return true;return false;}};
class CThingObject:public CThing {public:enum{ThingTypeMask=0x807};};
class CCharString {public:void* PStringData;};enum ETCInterfaceType {TC_TEST=1};
class CThingFilter_HasDefName {public:CCharString Name;bool operator()(const CThing&)const;};
class CThingFilter_HasTC {public:ETCInterfaceType Type;bool operator()(const CThing&)const;};
class CThingFilter_HasScriptName {public:CCharString Name;bool operator()(const CThing&)const;};
namespace std {
template<class T>class allocator{};
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base {T _M_data;};
template<class T>struct _Const_traits {typedef const T& reference;};template<class T>struct _Nonconst_traits {typedef T& reference;};
template<class T,class Traits>struct _List_iterator {typedef typename Traits::reference reference;_List_node_base* _M_node;_List_iterator(_List_node_base* p):_M_node(p){} _List_iterator(const _List_iterator& p):_M_node(p._M_node){}reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;} _List_iterator& operator++(){_M_node=_M_node->_M_next;return *this;}bool operator!=(const _List_iterator& p)const{return _M_node!=p._M_node;}};
template<class T,class A=allocator<T> >class list {public:_List_node_base* _M_node;typedef _List_iterator<T,_Nonconst_traits<T> > iterator;typedef _List_iterator<T,_Const_traits<T> > const_iterator;
iterator end(){return iterator(_M_node);}const_iterator begin()const{return const_iterator(_M_node->_M_next);}const_iterator end()const{return const_iterator(_M_node);}
_List_node<T>* _M_create_node(const T& value){_List_node<T>* p=(_List_node<T>*)malloc(sizeof(_List_node<T>));new(&p->_M_data)T(value);return p;}
iterator insert(iterator position,const T& value){_List_node<T>* tmp=_M_create_node(value);_List_node_base* n=position._M_node;_List_node_base* prev=n->_M_prev;tmp->_M_next=n;tmp->_M_prev=prev;prev->_M_next=tmp;n->_M_prev=tmp;return iterator(tmp);}
void push_back(const T& value){insert(end(),value);}
};
}
template<class T>inline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
class CThingSearchTools:public CThingSearchTools_HeaderLayout {protected:const std::list<CClass*>& PeekClassList()const;public:template<class TThing,class TContainer,class TFilter>void GetAllThings(const TFilter&,TContainer&)const;};
template<class TThing,class TContainer,class TFilter>void CThingSearchTools::GetAllThings(const TFilter& filter,TContainer& result)const{const std::list<CClass*>& things=PeekClassList();for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it){CThing* thing=(CThing*)*it;if(thing->IsIncludedInThingScans() && IsThingOfType<TThing>(thing) && filter(*(TThing*)thing))result.push_back((TThing*)thing);}}
template void CThingSearchTools::GetAllThings<CThingObject,std::list<CThing*>, CThingFilter_HasTC>(const CThingFilter_HasTC&,std::list<CThing*>&)const;