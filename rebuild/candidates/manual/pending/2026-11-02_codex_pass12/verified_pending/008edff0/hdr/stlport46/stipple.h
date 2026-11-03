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

#ifndef FABLE_STLPORT46_STIPPLE_H
#define FABLE_STLPORT46_STIPPLE_H
// Adapted original STLport4.6 _vector.h/_vector.c, _uninitialized.h and _algobase.h.
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t,void* p){return p;}
#else
#include <new>
#endif
namespace std {
struct __true_type {};
template<class T>class allocator {public:T* allocate(size_t n,const void* =0);void deallocate(T* p,size_t){if(p)free(p);}};
template<class T>T* allocator<T>::allocate(size_t n,const void*){return n?(T*)malloc(n*sizeof(T)):0;}
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T>inline const T&(min)(const T&a,const T&b){return b<a?b:a;}
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline void swap(T&a,T&b){T temp=a;a=b;b=temp;}
template<class T,class U>inline void _Construct(T*p,const U&v){new(p)T(v);}
inline void* __copy_trivial(const void*first,const void*last,void*result){return (last==first)?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);}
template<class T,class N>inline T*fill_n(T*first,N count,const T&v){for(;count>0;--count,++first)*first=v;return first;}

template<class T,class A>class _Vector_base {public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){} _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;} ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:
 typedef unsigned int size_type;typedef T*iterator;typedef const T*const_iterator;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 explicit vector(size_type n,const T&value=T(),const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=fill_n(this->_M_start,n,value);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,__true_type(),1,true);}
 iterator erase(iterator first,iterator last){T*out=(T*)__copy_trivial(last,this->_M_finish,first);this->_M_finish=out;return first;}
 void resize(size_type n,T x){if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
 void resize(size_type n){resize(n,T());}
 __declspec(noinline) void _M_fill_insert(iterator,size_type,const T&);
protected:
 void _M_insert_overflow(iterator pos,const T&x,const __true_type&,size_type fill_len,bool at_end){const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);T*start=this->_M_end_of_storage.allocate(len);T*finish=(T*)__copy_trivial(this->_M_start,pos,start);finish=fill_n(finish,fill_len,x);if(!at_end)finish=(T*)__copy_trivial(pos,this->_M_finish,finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=start+len;}
};
}
#endif
