/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */
// Modified excerpts for the original bank-symbol instantiations.
#ifndef FABLE_STLPORT_BANK_SYMBOLS_H
#define FABLE_STLPORT_BANK_SYMBOLS_H
#include <stddef.h>
#include <stdlib.h>
#include <new>
#include <string.h>
// Original STLport4.6.2 allocator/vector/tree names and complete native storage.
namespace std {
struct __true_type{};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
inline void*__copy_trivial(const void*first,const void*last,void*result){return first==last?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);}
template<class I,class N,class T>inline I fill_n(I first,N n,const T&value){for(;n>0;--n,++first)*first=value;return first;}
template<class T>class allocator {public:T*allocate(size_t n){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T,class A>class _Vector_base {public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T*iterator;typedef const T*const_iterator;typedef size_t size_type;explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){for(iterator p=this->_M_start;p!=this->_M_finish;++p)p->~T();}iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}size_type size()const{return size_type(this->_M_finish-this->_M_start);}bool empty()const{return this->_M_start==this->_M_finish;}T&operator[](size_type n){return *(begin()+n);}const T&operator[](size_type n)const{return *(begin()+n);}void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,__true_type(),1,true);}protected:__declspec(noinline) void _M_insert_overflow(iterator __position,const T&__x,const __true_type&,size_type __fill_len,bool __atend){const size_type __old_size=size();const size_type __len=__old_size+(max)(__old_size,__fill_len);iterator __new_start=this->_M_end_of_storage.allocate(__len);iterator __new_finish=(iterator)__copy_trivial(this->_M_start,__position,__new_start);__new_finish=fill_n(__new_finish,__fill_len,__x);if(!__atend)__new_finish=(iterator)__copy_trivial(__position,this->_M_finish,__new_finish);for(iterator p=this->_M_start;p!=this->_M_finish;++p)p->~T();this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);this->_M_start=__new_start;this->_M_finish=__new_finish;this->_M_end_of_storage._M_data=__new_start+__len;}};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(),second(){}pair(const A&a,const B&b):first(a),second(b){}};
template<class A,class B,class R>struct binary_function{};template<class A,class R>struct unary_function{};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};template<class T>struct _Const_traits{typedef const T*pointer;typedef const T&reference;};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:static _Rb_tree_node_base*__cdecl _M_increment(_Rb_tree_node_base*node){if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}else{_Rb_tree_node_base*parent=node->_M_parent;while(node==parent->_M_right){node=parent;parent=parent->_M_parent;}if(node->_M_right!=parent)node=parent;}return node;}};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;_Rb_tree_iterator(){} _Rb_tree_iterator(_Rb_tree_node<V>*p){_M_node=p;}reference operator*()const{return ((_Rb_tree_node<V>*)_M_node)->_M_value_field;}pointer operator->()const{return &operator*();}_Rb_tree_iterator&operator++(){_M_node=_Rb_global<bool>::_M_increment(_M_node);return *this;}};
template<class V,class A>struct _Rb_tree_base{_Rb_tree_base(const A&):_M_header(allocator<_Rb_tree_node<V> >(),0){_M_header._M_data=_M_header.allocate(1);}~_Rb_tree_base(){if(_M_header._M_data)_M_header.deallocate(_M_header._M_data,1);}protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;};
template<class K,class V,class E,class C,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:size_t _M_node_count;C _M_key_compare;private:void _M_erase(_Rb_tree_node<V>*);public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;_Rb_tree(const C&c,const A&a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}~_Rb_tree(){clear();}void clear(){if(_M_node_count){_M_erase((_Rb_tree_node<V>*)this->_M_header._M_data->_M_parent);this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_right=this->_M_header._M_data;_M_node_count=0;}}iterator begin(){return iterator((_Rb_tree_node<V>*)this->_M_header._M_data->_M_left);}iterator end(){return iterator(this->_M_header._M_data);}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{public:typedef pair<const K,V>value_type;private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::iterator iterator;map():_M_t(C(),A()){}iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}V&operator[](const K&);};
}
#endif
