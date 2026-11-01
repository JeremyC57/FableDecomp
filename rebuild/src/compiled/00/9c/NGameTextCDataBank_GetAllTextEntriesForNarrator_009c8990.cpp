// 009c8990 NGameText::CDataBank::GetAllTextEntriesForNarrator
// Leaf: ?GetAllTextEntriesForNarrator@CDataBank@NGameText@@QBEXABVCCharString@@AAV?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@K@Z
// Flags:
#include <new>
#include <string.h>
namespace std{
typedef unsigned int size_t;typedef int ptrdiff_t;
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class B,class C>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef C result_type;};
template<class A,class B>struct unary_function{typedef A argument_type;typedef B result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}};
template<class T,class A>struct _Alloc_traits{typedef typename A::template rebind<T>::other allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base{typedef _Rb_tree_node_base*_Base_ptr;_Rb_tree_Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;
 _Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&p){_M_node=p._M_node;}
 reference operator*()const{return _Link_type(_M_node)->_M_value_field;}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}_Self&operator--(){_M_node=_Rb_global_inst::_M_decrement(_M_node);return*this;}
};
template<class V,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<V>_Node;typedef typename _Alloc_traits<V,A>::allocator_type allocator_type;
 _Rb_tree_base(const allocator_type&);~_Rb_tree_base();
protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;
};
template<class K,class V,class Key,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{
 typedef _Rb_tree_base<V,A>_Base;
protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;typedef _Rb_tree_Color_type _Color_type;
public:typedef K key_type;typedef V value_type;typedef V*pointer;typedef const V*const_pointer;typedef V&reference;typedef const V&const_reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag _Iterator_category;typedef typename _Base::allocator_type allocator_type;
protected:size_type _M_node_count;Compare _M_key_compare;
 _Link_type&_M_rightmost()const{return(_Link_type&)this->_M_header._M_data->_M_right;}
 static _Link_type&__cdecl _S_right(_Link_type p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Link_type p){return p->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type p){return Key()(_S_value(p));}
 static _Link_type&__cdecl _S_right(_Base_ptr p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Base_ptr p){return((_Link_type)p)->_M_value_field;}
 static const K&__cdecl _S_key(_Base_ptr p){return Key()(_S_value(_Link_type(p)));}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
private:void _M_erase(_Link_type);iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
public:void clear(){if(_M_node_count){_M_erase((_Link_type)this->_M_header._M_data->_M_parent);this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_right=this->_M_header._M_data;_M_node_count=0;}}size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef pair<const K,V>value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;
};
template<class K,class C=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;
public:typedef _Rb_tree_iterator<K,_Const_traits<K> >iterator;void clear(){_M_t.clear();}
pair<iterator,bool>insert(const K&x){pair<typename _Rep_type::iterator,bool>p=_M_t.insert_unique(x);return pair<iterator,bool>(p.first,p.second);}
};
}
// Layout: original char-string data is17 packed bytes (13-byte Data followed by unaccessed reference count).
// Only the address of Data is used here; byte storage preserves the original size without new packing.
template<class T>class CBasicString{unsigned char unobservedStringState[13];public:bool operator==(const T*)const;};
class CCharStringData{public:CBasicString<char>Data;private:unsigned char unobservedReferenceCount[4];};
class CCharString{protected:CCharStringData*PStringData;public:bool operator==(const char*text)const{if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}};
namespace std{
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:unsigned int size()const{return this->_M_finish-this->_M_start;}const T&operator[](unsigned int n)const{return this->_M_start[n];}};
}
template<class T>class CArray:public std::vector<T>{public:const T&operator[](unsigned int n)const{return std::vector<T>::operator[](n);}};
#define CIVCountedPointeeBase CIVCountedPointeeBaseFields
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResourceFields
#include "engine/CResource.h"
#undef CResource
class CIVCountedPointeeBase{private:mutable unsigned long IVRefCount;public:virtual~CIVCountedPointeeBase();virtual void Delete()const;void AddRef()const{++IVRefCount;}long Release()const{return --IVRefCount;}long GetRefCount()const{return IVRefCount;}};
class CResource:public CIVCountedPointeeBase{unsigned char unobservedResourceState[sizeof(CResourceFields)-sizeof(CIVCountedPointeeBase)];};
template<class T>class CIVCountedPointer{private:CIVCountedPointeeBase*Object;public:CIVCountedPointer(T*p=0):Object(const_cast<CIVCountedPointeeBase*>(static_cast<const CIVCountedPointeeBase*>(p))){if(Object)Object->AddRef();}
 CIVCountedPointer(const CIVCountedPointer&p):Object(p.Object){if(Object)Object->AddRef();}
 ~CIVCountedPointer(){Release();}void Release(){if(Object){CIVCountedPointeeBase*object=Object;if(object->Release()==0)object->Delete();Object=0;}}T*Get()const{return static_cast<T*>(Object);}T*operator->()const{return Get();}};
#define CBankFile CBankFileFields
#include "engine/CBankFile.h"
#undef CBankFile
class CBankFile:public CBankFileFields{public:unsigned long GetEntryDataType(unsigned long)const;};

#pragma auto_inline(off)
namespace std{
template<class K,class V,class Key,class Compare,class A>
__declspec(noinline)pair<_Rb_tree_iterator<V,_Nonconst_traits<V> >,bool>_Rb_tree<K,V,Key,Compare,A>::insert_unique(const V&v){
 _Link_type y=this->_M_header._M_data;_Link_type x=(_Link_type)y->_M_parent;bool comp=true;
 while(x){y=x;comp=_M_key_compare(Key()(v),_S_key(x));x=(_Link_type)(comp?x->_M_left:x->_M_right);}
 iterator j(y);
 if(comp){if(j._M_node==this->_M_header._M_data->_M_left)return pair<iterator,bool>(_M_insert(y,y,v),true);else --j;}
 if(_M_key_compare(_S_key(j._M_node),Key()(v)))return pair<iterator,bool>(_M_insert(x,y,v),true);
 return pair<iterator,bool>(j,false);
}
}
#pragma auto_inline(on)
namespace NGameText{
class CTextEntry;
class CDataBankEntry{};
class CDataBankTextEntry:public CDataBankEntry{private:CIVCountedPointer<CTextEntry>PText;long NarratorIndex;public:long GetNarratorIndex()const{return NarratorIndex;}};
class CGroupEntry:public CResource{private:CArray<long>Entries;public:const CArray<long>&GetTextEntries()const{return Entries;}};
// Layout: original CBankFileAsync first base and CResourceBank second base flatten into this1e0-byte prefix.
// Original Entries is CArray<CDataBankEntry*>; donor254 becomes native1e0 after container/base shrinkage.
class CDataBank:public CBankFile{private:unsigned char unobservedBankState[0x1e0-sizeof(CBankFile)];CArray<CDataBankEntry*>Entries;
public:long ConvertNarratorNameToIndex(const CCharString&)const;CIVCountedPointer<const CGroupEntry>GetPGroupEntry(unsigned long,bool)const;
 const CDataBankTextEntry*GetTextInfo(unsigned long n)const{if(GetEntryDataType(n)==0)return static_cast<const CDataBankTextEntry*>(Entries[n]);return 0;}
 void GetAllTextEntriesForNarrator(const CCharString&,std::set<unsigned long>&,unsigned long)const;
};


void CDataBank::GetAllTextEntriesForNarrator(const CCharString& narrator, std::set<unsigned long>& entries, unsigned long minIndex) const
{
    entries.clear();
    long narratorIndex = ConvertNarratorNameToIndex(narrator);
    if (narratorIndex == -1 && !(narrator == ""))
        return;
    long count = Entries.size();
    for (long i = 1; i < count; ++i) {
        if (GetEntryDataType(i) == 0) {
            const CDataBankTextEntry* entry = GetTextInfo(i);
            if (entry) {
                long speaker = entry->GetNarratorIndex();
                if ((speaker == narratorIndex || (narrator == "" && speaker == -1)) && (unsigned long)i >= minIndex)
                    entries.insert(i);
            }
        } else if (GetEntryDataType(i) == 1) {
            const CArray<long>& members = GetPGroupEntry(i, true)->GetTextEntries();
            for (unsigned long j = 0; j < members.size(); ++j) {
                const CDataBankTextEntry* entry = GetTextInfo(members[j]);
                if (entry) {
                    long speaker = entry->GetNarratorIndex();
                    if (speaker == narratorIndex || (narrator == "" && speaker == -1)) {
 long value=members[j];if((unsigned long)value>=minIndex)entries.insert((unsigned long)value);
 }
                }
            }
        }
    }
}
}