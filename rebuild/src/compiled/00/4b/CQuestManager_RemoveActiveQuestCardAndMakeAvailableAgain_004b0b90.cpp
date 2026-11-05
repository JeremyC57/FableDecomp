// 004b0b90 CQuestManager::RemoveActiveQuestCardAndMakeAvailableAgain
// Leaf: ?RemoveActiveQuestCardAndMakeAvailableAgain@CQuestManager@@QAEXABVCCharString@@_N@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Flags:
#define FABLE_RETAIL_BASE_CLASS
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CCountedPointer.h"
#define CQuestManager CQuestManagerFields
#include "engine/CQuestManager.h"
#undef CQuestManager
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CTCQuestCard CTCQuestCardFields
#include "engine/CTCQuestCard.h"
#undef CTCQuestCard
#define CPlayerGui CPlayerGuiFields
#include "engine/CPlayerGui.h"
#undef CPlayerGui
// Layout: original string data's accessed prefix is PData +0 and StringLength +4.
// No complete string-data object is constructed; remaining allocation fields are unaccessed.
template<class T>class CBasicString {protected:T*PData;unsigned long StringLength;public:bool operator==(const CBasicString&x)const{if(StringLength!=x.StringLength)return false;return Compare(PData,x.PData)==0;}protected:static long __fastcall Compare(const T*,const T*);};
class CCharStringData {public:CBasicString<char>Data;};
class CCharString {protected:CCharStringData*PStringData;public:bool operator==(const CCharString&x)const{if(PStringData==x.PStringData)return true;if(PStringData&&x.PStringData)return PStringData->Data==x.PStringData->Data;return false;}};
// Layout: original PDB CBaseIntelligentPointer is polymorphic, PData +4, total8.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
extern "C" void __cdecl free(void*);
#ifdef _MSC_VER
#define FABLE_STLP_CDECL __cdecl
#else
#define FABLE_STLP_CDECL
#endif
extern "C" int FABLE_STLP_CDECL puts(const char*);
extern "C" void FABLE_STLP_CDECL abort(void);
namespace std {
inline void FABLE_STLP_CDECL __stl_throw_out_of_range(const char*msg){puts(msg);abort();}
template<class T>class allocator {};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class A,class B>struct pair {typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};
template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool> {bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Select1st:public unary_function<T,typename T::first_type> {const typename T::first_type&operator()(const T&x)const{return x.first;}};
template<class T>struct _Identity:public unary_function<T,T> {const T&operator()(const T&x)const{return x;}};
struct _Rb_tree_node_base {bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base {T _M_value_field;};
template<class T,class A>struct _Rb_tree_base {protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > >_M_header;};
template<class K,class V,class Select,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A> {protected:unsigned int _M_node_count;Compare _M_key_compare;};
template<class K,class Compare=less<K>,class A=allocator<K> >class set {private:_Rb_tree<K,K,_Identity<K>,Compare,A>_M_t;};
template<class T,class A>class _Vector_base {protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:typedef unsigned int size_type;typedef T*iterator;typedef const T*const_iterator;iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator begin()const{return this->_M_start;}const_iterator end()const{return this->_M_finish;}size_type size()const{return size_type(this->_M_finish-this->_M_start);}};
struct _List_node_base {_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base {T _M_data;};
struct _List_iterator_base {_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}_List_iterator_base(){}void _M_incr(){_M_node=_M_node->_M_next;}bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits {typedef T value_type;typedef T*pointer;typedef T&reference;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base {typedef _List_iterator<T,_Nonconst_traits<T> >iterator;_List_iterator(_List_node<T>*n):_List_iterator_base(n){}_List_iterator(){}_List_iterator(const iterator&x):_List_iterator_base(x._M_node){}typename Traits::reference operator*()const{return static_cast<_List_node<T>*>(this->_M_node)->_M_data;}_List_iterator&operator++(){this->_M_incr();return *this;}};
template<class T,class A>class _List_base {public:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A> {public:typedef _List_iterator<T,_Nonconst_traits<T> >iterator;iterator begin(){return iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}iterator end(){return iterator(this->_M_node._M_data);}iterator erase(iterator pos){_List_node_base*next=pos._M_node->_M_next;_List_node_base*prev=pos._M_node->_M_prev;_List_node<T>*n=static_cast<_List_node<T>*>(pos._M_node);prev->_M_next=next;next->_M_prev=prev;n->_M_data.~T();free(n);return iterator(static_cast<_List_node<T>*>(next));}};
template<unsigned int N>struct _Base_bitset {unsigned long _M_w[N];};
template<unsigned int N>class bitset:public _Base_bitset<(N+31)/32> {public:bool test(unsigned int pos)const{if(pos>=N)__stl_throw_out_of_range("bitset");return (this->_M_w[pos/32]&(1UL<<(pos%32)))!=0;}};
}
template<class T>class CArray:public std::vector<T> {};
template<class K,class V>class CKeyPairCompareLess {};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {public:CompareType Compare;bool Dirty;typedef typename std::vector<std::pair<K,V> >::const_iterator const_iterator;const_iterator LowerBound(const K&)const;const_iterator Find(const K&key)const{const_iterator i=LowerBound(key);if(i==this->end()||key<i->first)return this->end();return i;}};
// Layout: flattened supplied class fields retain native vptr and offsets. Methods add no storage.
class CTCBase;
enum ETCInterfaceType {};
class CThing:public CThingFields {public:
 void Kill(bool);bool HasTC(ETCInterfaceType t)const{return reinterpret_cast<const std::bitset<274>&>(UsingComponents).test(static_cast<unsigned int>(t));}template<class T>bool GetTC(T**);template<class T>bool GetTC(ETCInterfaceType,T**);
};
class CTCQuestCard:public CTCQuestCardFields {};
class CQuestManager:public CQuestManagerFields {public:void RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString&,bool);};
#pragma auto_inline(off)
template<class K,class V,class Compare>typename CVectorMap<K,V,Compare>::const_iterator CVectorMap<K,V,Compare>::LowerBound(const K&key)const
{
 const_iterator first=this->begin();int len=this->end()-first;
 while(len>0){int half=len>>1;const_iterator middle=first+half;if(middle->first<key){first=middle+1;len=len-half-1;}else len=half;}
 return first;
}
#pragma auto_inline(on)
template<class T>__forceinline bool CThing::GetTC(ETCInterfaceType t,T**out)
{
 if(HasTC(t)){const ETCInterfaceType key=t;const CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<const CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);*out=reinterpret_cast<T*>(map.Find(key)->second);return true;}return false;
}
template<>__forceinline bool CThing::GetTC<CTCQuestCard>(CTCQuestCard**out){return GetTC(static_cast<ETCInterfaceType>(0x6c),out);}
template<class T>class CTBaseSingletonNoDefaultAlloc {public:static T&__fastcall Get(){return *PClass;}private:static CCountedPointer<T>PClass;};
class CPlayerGui:public CPlayerGuiFields {public:void UpdateMiniMapMarkers(bool);};
void CQuestManager::RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString&scriptName,bool available)
{
 for(std::list<CIntelligentPointer<CThing> >::iterator i=reinterpret_cast<std::list<CIntelligentPointer<CThing> >&>(ActiveQuestCards).begin();i!=reinterpret_cast<std::list<CIntelligentPointer<CThing> >&>(ActiveQuestCards).end();++i){CThing*card=reinterpret_cast<CThing*>((*i).GetPItem());if(card){CTCQuestCard*tc;card->GetTC(&tc);if(tc&&*reinterpret_cast<CCharString*>(&tc->ScriptQuestName_PStringData)==scriptName){bool completed=false;for(std::list<CIntelligentPointer<CThing> >::iterator j=reinterpret_cast<std::list<CIntelligentPointer<CThing> >&>(CompletedQuestCards).begin();j!=reinterpret_cast<std::list<CIntelligentPointer<CThing> >&>(CompletedQuestCards).end();++j)if((*j).GetPItem()==reinterpret_cast<CBaseObject*>(card)){completed=true;break;}if(!completed)card->Kill(false);reinterpret_cast<std::list<CIntelligentPointer<CThing> >&>(ActiveQuestCards).erase(i);CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);return;}}}
}