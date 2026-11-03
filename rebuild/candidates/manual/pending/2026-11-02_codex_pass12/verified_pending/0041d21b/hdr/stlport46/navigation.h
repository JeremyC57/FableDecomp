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

// Modified from genuine STLport4.6 _list.h/_list.c for this native ABI.
#ifndef FABLE_STLPORT46_NAVIGATION_H
#define FABLE_STLPORT46_NAVIGATION_H
#include "stlport46/vector_roots.h"
namespace std {
template<class T>struct less;
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map;
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T>struct _Nonconst_traits{typedef T* pointer;typedef T& reference;};
template<class T>struct _Const_traits{typedef const T* pointer;typedef const T& reference;};
struct _List_iterator_base{
 _List_node_base* _M_node;
 _List_iterator_base(_List_node_base* x):_M_node(x){} _List_iterator_base(){}
 void _M_incr(){_M_node=_M_node->_M_next;} void _M_decr(){_M_node=_M_node->_M_prev;}
 bool operator==(const _List_iterator_base& x)const{return _M_node==x._M_node;}
 bool operator!=(const _List_iterator_base& x)const{return _M_node!=x._M_node;}
};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 typedef _List_iterator<T,_Nonconst_traits<T> > iterator;
 _List_iterator(_List_node<T>* x):_List_iterator_base(x){} _List_iterator(){}
 _List_iterator(const iterator& x):_List_iterator_base(x._M_node){}
 typename Traits::reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;}
 typename Traits::pointer operator->()const{return &operator*();}
 _List_iterator& operator++(){_M_incr();return *this;}
 _List_iterator& operator--(){_M_decr();return *this;}
};
template<class T,class A>class _List_base {
public:
 typedef _List_node<T> _Node;
#if defined(FABLE_RETAIL_NAV_REGIONS) && !defined(FABLE_RETAIL_NAV_OUTPUT)
 // Original _list.h ownership and VC7.1 _alloc.h conversion configuration.
 A get_allocator()const{return (const allocator<_Node>&)this->_M_node;}
 _List_base(const A& a):_M_node(a,(_Node*)0){_Node* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;}
#else
 _List_base(const A& a):_M_node(allocator<_Node>(a),(_Node*)0){_Node* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;}
#endif
 ~_List_base(){clear();_M_node.deallocate(_M_node._M_data,1);}
 void clear();
 _STLP_alloc_proxy<_Node*,_Node,allocator<_Node> > _M_node;
};
template<class T,class A>void _List_base<T,A>::clear(){_List_node<T>* cur=(_List_node<T>*)this->_M_node._M_data->_M_next;while(cur!=this->_M_node._M_data){_List_node<T>* tmp=cur;cur=(_List_node<T>*)cur->_M_next;_Destroy(&tmp->_M_data);this->_M_node.deallocate(tmp,1);}this->_M_node._M_data->_M_next=this->_M_node._M_data;this->_M_node._M_data->_M_prev=this->_M_node._M_data;}
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{
public:
 typedef _List_node<T> _Node;typedef _List_iterator<T,_Nonconst_traits<T> > iterator;typedef _List_iterator<T,_Const_traits<T> > const_iterator;
 explicit list(const A& a=A()):_List_base<T,A>(a){}
 list(const list& x):_List_base<T,A>(x.get_allocator()){insert(begin(),x.begin(),x.end());}
 ~list(){}
#if !defined(FABLE_RETAIL_NAV_REGIONS) || defined(FABLE_RETAIL_NAV_OUTPUT)
 A get_allocator()const{return A((const allocator<_Node>&)this->_M_node);}
#endif
 iterator begin(){return (_Node*)this->_M_node._M_data->_M_next;}const_iterator begin()const{return (_Node*)this->_M_node._M_data->_M_next;}
 iterator end(){return this->_M_node._M_data;}const_iterator end()const{return this->_M_node._M_data;}
 T& back(){return *--end();}const T& back()const{return *--end();}
protected:
 _Node* _M_create_node(const T& x){_Node* p=this->_M_node.allocate(1);_Construct(&p->_M_data,x);return p;}
public:
 iterator insert(iterator position,const T& x){_Node* tmp=_M_create_node(x);_List_node_base* n=position._M_node;_List_node_base* p=n->_M_prev;tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;return tmp;}
 void push_back(const T& x){insert(end(),x);}
 template<class I>void insert(iterator pos,I first,I last){_M_insert_dispatch(pos,first,last,__false_type());}
 template<class I>void _M_insert_dispatch(iterator pos,I first,I last,const __false_type&){for(;first!=last;++first)insert(pos,*first);}
};
}
#endif
