#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __declspec(x)
#define __cdecl
#endif
// std::list::list(const list& x) -- copy elements into a newly allocated sentinel list
// Grade: pending owner naming preflight; historical exact grade3 preserved
// Leaf: ??0?$list@JV?$allocator@J@std@@@std@@QAE@ABV01@@Z
// Flags:
// Retail 00579a1c, 47 bytes. Element/callsite evidence retained in ROW_DETAILS.
// Adapted from STLport 4.6.2 _list.h and _alloc.h; malloc route proven by retail.
// Copyright (c) 1994 Hewlett-Packard Company; 1996,1997 Silicon Graphics Computer Systems, Inc.;
// 1997 Moscow Center for SPARC Technology; 1999 Boris Fomitchev.
// Permission to use, copy, modify, distribute and sell this software and documentation for any purpose is granted
// without fee, provided this copyright notice appears in all copies. Provided as is, without warranty.
#pragma optimize("y", off)
#pragma optimize("s", on)
#ifdef _MSC_VER
typedef unsigned int list_size_type;
#else
typedef __SIZE_TYPE__ list_size_type;
#endif
extern "C" void* __cdecl malloc(list_size_type);
extern "C" void __cdecl free(void*);
namespace std {
template<class T> class allocator { public:
 allocator(){}
 allocator(const allocator&){}
 ~allocator(){}
 template<class U> allocator(const allocator<U>&){}
 T* allocate(list_size_type n,const void* =0) { return n ? (T*)malloc(n*sizeof(T)) : 0; }
 void deallocate(T*p,list_size_type) { if(p)free(p); }
};
template<class V,class T,class A> class _STLP_alloc_proxy: public A { public:
 V _M_data;
 _STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}
};
template<class T> struct _Nonconst_traits {typedef T* pointer;typedef T& reference;};
template<class T> struct _Const_traits {typedef const T* pointer;typedef const T& reference;};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template<class T> struct _List_node: public _List_node_base { T _M_data; };
struct _List_iterator_base {
 _List_node_base* _M_node;
 _List_iterator_base(_List_node_base*p):_M_node(p){}
 _List_iterator_base(){}
 void _M_incr(){_M_node=_M_node->_M_next;}
 void _M_decr(){_M_node=_M_node->_M_prev;}
 bool operator==(const _List_iterator_base&x)const{return _M_node==x._M_node;}
 bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}
};
template<class T,class Traits> struct _List_iterator: public _List_iterator_base {
 typedef _List_iterator<T,_Nonconst_traits<T> > iterator;
 typedef typename Traits::reference reference;
 typedef _List_node<T> _Node;
 _List_iterator(_Node*p):_List_iterator_base(p){}
 _List_iterator(){}
 _List_iterator(const iterator&x):_List_iterator_base(x._M_node){}
 reference operator*()const{return ((_Node*)_M_node)->_M_data;}
 _List_iterator& operator++(){_M_incr();return *this;}
};
template<class T,class A> class _List_base {
protected:
 typedef _List_node<T> _Node;
 typedef allocator<_Node> _Node_allocator_type;
public:
 typedef A allocator_type;
 allocator_type get_allocator()const{return allocator_type((const _Node_allocator_type&)_M_node);}
 __declspec(noinline) _List_base(const allocator_type&);
 ~_List_base(){clear();_M_node.deallocate(_M_node._M_data,1);}
 void clear(){
 _Node*cur=(_Node*)_M_node._M_data->_M_next;
 while(cur!=_M_node._M_data){_Node*tmp=cur;cur=(_Node*)cur->_M_next;tmp->_M_data.~T();_M_node.deallocate(tmp,1);}
 _M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;
 }
 _STLP_alloc_proxy<_Node*,_Node,_Node_allocator_type> _M_node;
};
template<> __declspec(noinline)
_List_base<long,allocator<long> >::_List_base(const allocator<long>&a)
 :_M_node(allocator<_Node>(a),(_Node*)0)
{
 _Node*n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
}
template<class T,class A=allocator<T> > class list: public _List_base<T,A> {
 typedef _List_base<T,A> _Base;
public:
 typedef _List_node<T> _Node;
 typedef typename _Base::allocator_type allocator_type;
 typedef _List_iterator<T,_Nonconst_traits<T> > iterator;
 typedef _List_iterator<T,_Const_traits<T> > const_iterator;
 explicit list(const allocator_type&a=allocator_type()):_Base(a){}
 iterator begin(){return iterator((_Node*)this->_M_node._M_data->_M_next);}
 const_iterator begin()const{return const_iterator((_Node*)this->_M_node._M_data->_M_next);}
 iterator end(){return this->_M_node._M_data;}
 const_iterator end()const{return this->_M_node._M_data;}
 list(const list&x):_Base(x.get_allocator()){insert(begin(),x.begin(),x.end());}
 template<class I> void insert(iterator pos,I first,I last);
};
template list<long>::list(const list<long>&);
}

namespace std {
template<class T,class A> template<class I>
void list<T,A>::insert(iterator pos,I first,I last) {
 while(first!=last){
 _Node*n=this->_M_node.allocate(1);new(n) _Node;n->_M_data=*first;
 _List_node_base*p=pos._M_node;_List_node_base*prev=p->_M_prev;
 n->_M_next=p;n->_M_prev=prev;prev->_M_next=n;p->_M_prev=n;++first;
 }
}
}
template<class T> void append(std::list<T>&l,const T&value){
 typename std::list<T>::_Node*n=l._M_node.allocate(1);new(n)typename std::list<T>::_Node;n->_M_data=value;
 std::_List_node_base*p=l._M_node._M_data;std::_List_node_base*prev=p->_M_prev;
 n->_M_next=p;n->_M_prev=prev;prev->_M_next=n;p->_M_prev=n;
}
int main(){
 long v1=3,v2=1,v3=4;
 std::list<long> original;append(original,v1);append(original,v2);append(original,v3);
 std::list<long> copy(original);
 int ok=copy._M_node._M_data!=original._M_node._M_data;
 std::list<long>::const_iterator i=copy.begin();
 ok=ok && i!=copy.end() && *i==3;++i;
 ok=ok && i!=copy.end() && *i==1;++i;
 ok=ok && i!=copy.end() && *i==4;++i;
 ok=ok && i==copy.end();
 std::list<long> empty;std::list<long> emptycopy(empty);
 ok=ok && emptycopy.begin()==emptycopy.end() && emptycopy._M_node._M_data!=empty._M_node._M_data;
 copy.clear();ok=ok && copy.begin()==copy.end() && original.begin()!=original.end();
 if(!ok){std::printf("00579a1c_TEST FAIL\n");return 1;}std::printf("00579a1c_TEST PASS\n");return 0;
}