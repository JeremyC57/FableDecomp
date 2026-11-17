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

#include <cstdio>
#include <cstdlib>
#include <cstring>
static unsigned cases;
static void check(bool p){if(!p){std::fprintf(stderr,"deque copy case %u failed\n",cases);std::abort();}}
int main(){
 typedef NUISystem::CComponent*Pointer;typedef std::ScreenIter Iter;
 const int starts[]={0,1,15,31,32,63};const int destinations[]={0,3,31,32,63};const int lengths[]={0,1,2,31,32,33,64,95};
 Pointer source[6][32],dest[6][32];Pointer*sm[6],*dm[6];
 for(int b=0;b<6;++b){sm[b]=source[b];dm[b]=dest[b];for(int k=0;k<32;++k)source[b][k]=reinterpret_cast<Pointer>(static_cast<unsigned int>(0x1000+(b*32+k)*4));}
 for(int si=0;si<6;++si)for(int di=0;di<5;++di)for(int ni=0;ni<8;++ni){
  int start=starts[si],out=destinations[di],count=lengths[ni];
  for(int b=0;b<6;++b)for(int k=0;k<32;++k)dest[b][k]=reinterpret_cast<Pointer>(static_cast<unsigned int>(0x8000+(b*32+k)*4));
  Iter first(source[start/32]+start%32,sm+start/32),last(source[(start+count)/32]+(start+count)%32,sm+(start+count)/32),result(dest[out/32]+out%32,dm+out/32);
  Iter oldFirst=first,oldLast=last,oldResult=result;
  Iter end=std::__copy(first,last,result,std::random_access_iterator_tag(),static_cast<int*>(0));
  check(end._M_cur==dest[(out+count)/32]+(out+count)%32&&end._M_node==dm+(out+count)/32);
  check(end._M_first==dm[(out+count)/32]&&end._M_last==dm[(out+count)/32]+32);
  check(std::memcmp(&first,&oldFirst,sizeof first)==0&&std::memcmp(&last,&oldLast,sizeof last)==0&&std::memcmp(&result,&oldResult,sizeof result)==0);
  for(int b=0;b<6;++b)for(int k=0;k<32;++k){int at=b*32+k;Pointer expected=at>=out&&at<out+count?source[(start+at-out)/32][(start+at-out)%32]:reinterpret_cast<Pointer>(static_cast<unsigned int>(0x8000+at*4));check(dest[b][k]==expected);check(source[b][k]==reinterpret_cast<Pointer>(static_cast<unsigned int>(0x1000+at*4)));}
  ++cases;
 }
 check(cases==240);std::puts("0059afd7_TEST PASS");return 0;
}