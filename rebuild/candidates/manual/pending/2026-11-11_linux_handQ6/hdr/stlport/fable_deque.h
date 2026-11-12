#ifndef FABLE_STLPORT_FABLE_DEQUE_H
#define FABLE_STLPORT_FABLE_DEQUE_H
// STLport4.6.2 stl/_deque.h and _deque.c: native iterator/base hierarchy.
#include "stlport/fable_containers.h"
namespace std {
template<class T>struct _Deque_iterator_base {
 enum _Constants {_blocksize=128,__buffer_size=sizeof(T)<_blocksize?_blocksize/sizeof(T):1};
 typedef T value_type;typedef T** _Map_pointer;
 T* _M_cur;T* _M_first;T* _M_last;T** _M_node;
 _Deque_iterator_base():_M_cur(0),_M_first(0),_M_last(0),_M_node(0){}
 void _M_set_node(_Map_pointer node){_M_last=(_M_first=*(_M_node=node))+__buffer_size;}
 void _M_increment(){if(++_M_cur==_M_last){_M_set_node(_M_node+1);_M_cur=_M_first;}}
#ifdef FABLE_STLPORT_DEQUE_PUSH_BACK
 // Opt-in (chunk Q6, row 0x00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath): STLport 4.6 _deque.h _M_decrement.
 void _M_decrement(){if(_M_cur==_M_first){_M_set_node(_M_node-1);_M_cur=_M_last;}--_M_cur;}
#endif
};
template<class T,class Traits>struct _Deque_iterator:public _Deque_iterator_base<T> {
 typedef T value_type;typedef random_access_iterator_tag iterator_category;typedef ptrdiff_t difference_type;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 _Deque_iterator(){}
 _Deque_iterator(const _Deque_iterator<T,_Nonconst_traits<T> >& x):_Deque_iterator_base<T>(x){}
 reference operator*()const{return *this->_M_cur;}
 _Deque_iterator& operator++(){this->_M_increment();return *this;}
 bool operator!=(const _Deque_iterator& other)const{return this->_M_cur!=other._M_cur;}
#ifdef FABLE_STLPORT_DEQUE_PUSH_BACK
 _Deque_iterator& operator--(){this->_M_decrement();return *this;}
#endif
};
template<class T,class Traits>struct iterator_traits<_Deque_iterator<T,Traits> > {
 typedef T value_type;typedef random_access_iterator_tag iterator_category;typedef ptrdiff_t difference_type;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
};
template<class T,class A>class _Deque_base {
public:
 typedef T value_type;typedef A allocator_type;typedef allocator<T*> _Map_alloc_type;
 typedef _Deque_iterator<T,_Nonconst_traits<T> > iterator;
 static size_t buffer_size(){return _Deque_iterator_base<T>::__buffer_size;}
 _Deque_base(const A& a,size_t count):_M_start(),_M_finish(),_M_map(allocator<T*>(),0),_M_map_size(a,0){_M_initialize_map(count);}
 _Deque_base(const A& a):_M_start(),_M_finish(),_M_map(allocator<T*>(),0),_M_map_size(a,0){}
 ~_Deque_base(){if(_M_map._M_data){_M_destroy_nodes(_M_start._M_node,this->_M_finish._M_node+1);_M_map.deallocate(_M_map._M_data,_M_map_size._M_data);}}
protected:
 void _M_initialize_map(size_t count){
  size_t nodes=count/buffer_size()+1;
  _M_map_size._M_data=(std::max)((size_t)8,nodes+2);
  _M_map._M_data=_M_map.allocate(_M_map_size._M_data);
  T** first=_M_map._M_data+(_M_map_size._M_data-nodes)/2;T** last=first+nodes;
  _M_create_nodes(first,last);_M_start._M_set_node(first);_M_finish._M_set_node(last-1);
  _M_start._M_cur=_M_start._M_first;_M_finish._M_cur=_M_finish._M_first+count%buffer_size();
 }
 void _M_create_nodes(T** first,T** last){for(T** n=first;n<last;++n)*n=_M_map_size.allocate(buffer_size());}
 void _M_destroy_nodes(T** first,T** last){for(T** n=first;n<last;++n)_M_map_size.deallocate(*n,buffer_size());}
 iterator _M_start,_M_finish;
 _STLP_alloc_proxy<T**,T*,_Map_alloc_type> _M_map;
 _STLP_alloc_proxy<size_t,T,A> _M_map_size;
};
template<class T,class A=allocator<T> >class deque:protected _Deque_base<T,A> {
public:
 typedef T value_type;typedef size_t size_type;typedef T& reference;typedef const T& const_reference;
 deque():_Deque_base<T,A>(A(),0){}
 ~deque(){std::_Destroy(this->_M_start,this->_M_finish);}
#ifdef FABLE_STLPORT_DEQUE_PUSH_BACK
 // Opt-in (chunk Q6, row 0x00912840): STLport 4.6 _deque.h back() / push_back() / _M_reserve_map_at_back(), inline as in the
 // library; _M_push_back_aux_v (_deque.c) and _M_reallocate_map are out of line (retail 0x00913D60 for deque<CBezierCurve>):
 // declarations only here, a row whose retail TU holds the _M_push_back_aux_v body defines it (see the row).
 typedef typename _Deque_base<T,A>::iterator iterator;
 reference back(){iterator tmp=this->_M_finish;--tmp;return *tmp;}
 void push_back(const value_type& t){
  if(this->_M_finish._M_cur!=this->_M_finish._M_last-1){_Construct(this->_M_finish._M_cur,t);++this->_M_finish._M_cur;}
  else _M_push_back_aux_v(t);
 }
protected:
 void _M_push_back_aux_v(const value_type& t);
 void _M_reserve_map_at_back(size_type nodes_to_add=1){
  if(nodes_to_add+1>this->_M_map_size._M_data-(this->_M_finish._M_node-this->_M_map._M_data))_M_reallocate_map(nodes_to_add,false);
 }
 void _M_reallocate_map(size_type nodes_to_add,bool add_at_front);
public:
#endif
};
#if defined(FABLE_STLPORT_DEQUE_PUSH_BACK) && defined(FABLE_STLPORT_DEQUE_PUSH_BACK_AUX_NOINLINE)
// Per-TU opt-in (chunk Q6, row 0x00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath): STLport 4.6 _deque.c
// _M_push_back_aux_v (exceptions off), body visible but kept out of line, as the retail TU calls it (deque<CBezierCurve> 0x00913D60):
// c2 then knows push_back keeps no address of the value (the 12-dword software-pipelined curve copy at 0x00912B19).
#pragma auto_inline(off)
template<class T,class A> void deque<T,A>::_M_push_back_aux_v(const value_type& t){
 value_type t_copy=t;
 _M_reserve_map_at_back();
 *(this->_M_finish._M_node+1)=this->_M_map_size.allocate(this->buffer_size());
 _Construct(this->_M_finish._M_cur,t_copy);
 this->_M_finish._M_set_node(this->_M_finish._M_node+1);
 this->_M_finish._M_cur=this->_M_finish._M_first;
}
#pragma auto_inline(on)
#endif
template<class T,class Sequence=deque<T> >class stack {protected:Sequence c;public:stack():c(){}explicit stack(const Sequence& s):c(s){}};
}

#endif
