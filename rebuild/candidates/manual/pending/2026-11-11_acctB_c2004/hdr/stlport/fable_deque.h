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
#ifdef FABLE_RETAIL_STLP_DEQUE_PUSHBACK
 // Retail opt-in (chunk 2004, row 0x00912BA0): STLport 4.6 _Deque_iterator_base::_M_decrement, expanded by back() (0x00912D7C:
 // `cmp cur,first ; jne` then the previous node's buffer + 0x60, and `sub eax,30h`).
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
#ifdef FABLE_RETAIL_STLP_DEQUE_PUSHBACK
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
#ifdef FABLE_RETAIL_STLP_DEQUE_PUSHBACK
 // Retail opt-in (chunk 2004, row 0x00912BA0 CAIStateGroup_FlyingThingBase::CalculatePrettyPath): the STLport 4.6 stl/_deque.h members
 // that row uses. back() and push_back() are expanded in retail (0x00912D7C..0x00912DA3; 0x00912C9F..0x00912D22 with the
 // `cur != last - 1` test, the placement copy and `add dword ptr [ebp],30h`); clear() (?clear@?$deque@..@std@@QAEXXZ, landed
 // 0x00913A20) and the protected _M_push_back_aux_v (?_M_push_back_aux_v@..@IAEXAB..@Z, landed 0x00913D60) are called out of line.
 typedef _Deque_iterator<T,_Nonconst_traits<T> > iterator;
 reference back(){iterator __tmp=this->_M_finish;--__tmp;return *__tmp;}
 void push_back(const value_type& __t){
  if(this->_M_finish._M_cur!=this->_M_finish._M_last-1){_Construct(this->_M_finish._M_cur,__t);++this->_M_finish._M_cur;}
  else _M_push_back_aux_v(__t);
 }
 void clear();
protected:
 void _M_push_back_aux_v(const value_type& __t);
#ifdef FABLE_RETAIL_STLP_DEQUE_PUSHBACK_AUX_BODY
 void _M_reserve_map_at_back(size_type __nodes_to_add=1){
  if(__nodes_to_add+1>this->_M_map_size._M_data-(this->_M_finish._M_node-this->_M_map._M_data))_M_reallocate_map(__nodes_to_add,false);
 }
 void _M_reallocate_map(size_type __nodes_to_add,bool __add_at_front);
#endif
public:
#endif
};
#ifdef FABLE_RETAIL_STLP_DEQUE_PUSHBACK_AUX_BODY
// Retail opt-in (chunk 2004, row 0x00912BA0): the STLport 4.6 stl/_deque.c body of _M_push_back_aux_v, visible to the TU that holds it
// (retail emits it in the flying-thing TU, 0x00913D60, landed) and kept out of line (noinline, as retail calls it). With the body
// visible VC7.1 knows the argument's address is not kept, so the inlined push_back's 12-dword copy of a local curve is scheduled
// load-ahead as retail (0x0091302B..0x00913093); with a declaration only it alternates load / store. Same copy, map reserve
// (_M_reallocate_map out of line, landed 0x00913A80), buffer allocation and node step as the landed body.
template<class T,class A>
#ifdef _MSC_VER
__declspec(noinline)
#endif
void deque<T,A>::_M_push_back_aux_v(const value_type& __t){
 value_type __t_copy=__t;
 _M_reserve_map_at_back();
 *(this->_M_finish._M_node+1)=this->_M_map_size.allocate(this->buffer_size());
 _Construct(this->_M_finish._M_cur,__t_copy);
 this->_M_finish._M_set_node(this->_M_finish._M_node+1);
 this->_M_finish._M_cur=this->_M_finish._M_first;
}
#endif
template<class T,class Sequence=deque<T> >class stack {protected:Sequence c;public:stack():c(){}explicit stack(const Sequence& s):c(s){}};
}

#endif
