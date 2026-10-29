// 0059afd7 std::__copy
// Leaf: ??$__copy@U?$_Deque_iterator@PAVCComponent@NUISystem@@U?$_Nonconst_traits@PAVCComponent@NUISystem@@@std@@@std@@U12@H@std@@YI?AU?$_Deque_iterator@PAVCComponent@NUISystem@@U?$_Nonconst_traits@PAVCComponent@NUISystem@@@std@@@0@U10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
#ifndef _MSC_VER
#define __fastcall
#endif
#pragma optimize("y",off)
#pragma optimize("s",on)
namespace NUISystem {class CComponent;}
namespace std {
struct input_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:public forward_iterator_tag{};
struct random_access_iterator_tag:public bidirectional_iterator_tag{};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Deque_iterator_base{
 enum _Constants{_blocksize=128,__buffer_size=sizeof(T)<128?128/sizeof(T):1};
 typedef random_access_iterator_tag iterator_category;
 typedef T value_type;typedef unsigned int size_type;typedef int difference_type;
 typedef value_type**_Map_pointer;typedef _Deque_iterator_base<T>_Self;
 value_type*_M_cur;value_type*_M_first;value_type*_M_last;_Map_pointer _M_node;
 _Deque_iterator_base(value_type*x,_Map_pointer y):_M_cur(x),_M_first(*y),_M_last(*y+__buffer_size),_M_node(y){}
 _Deque_iterator_base():_M_cur(0),_M_first(0),_M_last(0),_M_node(0){}
 difference_type _M_subtract(const _Self&x)const;
 void _M_increment();
 void _M_set_node(_Map_pointer node){_M_last=(_M_first=*(_M_node=node))+difference_type(__buffer_size);}
};
template<class T,class Traits>struct _Deque_iterator:public _Deque_iterator_base<T>{
 typedef random_access_iterator_tag iterator_category;typedef T value_type;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 typedef unsigned int size_type;typedef int difference_type;typedef value_type**_Map_pointer;
 typedef _Deque_iterator_base<T>_Base;typedef _Deque_iterator<T,Traits>_Self;
 typedef _Deque_iterator<T,_Nonconst_traits<T> >_Nonconst_self;
 _Deque_iterator(value_type*x,_Map_pointer y):_Deque_iterator_base<T>(x,y){}
 _Deque_iterator(){}
 _Deque_iterator(const _Nonconst_self&x):_Deque_iterator_base<T>(x){}
 reference operator*()const{return *this->_M_cur;}
 difference_type operator-(const _Self&x)const{return this->_M_subtract(x);}
 _Self&operator++(){this->_M_increment();return *this;}
};
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<class T>typename _Deque_iterator_base<T>::difference_type _Deque_iterator_base<T>::_M_subtract(const _Self&x)const{
 return difference_type(__buffer_size)*(_M_node-x._M_node-1)+(_M_cur-_M_first)+(x._M_last-x._M_cur);
}
template<class T>void _Deque_iterator_base<T>::_M_increment(){if(++_M_cur==_M_last){_M_set_node(_M_node+1);_M_cur=_M_first;}}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
template<class In,class Out,class Distance>Out __fastcall __copy(In first,In last,Out result,const random_access_iterator_tag&,Distance*){
 for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;
}
typedef _Deque_iterator<NUISystem::CComponent*,_Nonconst_traits<NUISystem::CComponent*> > ScreenIter;
template ScreenIter __fastcall __copy(ScreenIter,ScreenIter,ScreenIter,const random_access_iterator_tag&,int*);
}