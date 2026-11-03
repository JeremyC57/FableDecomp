#if defined(FABLE_RETAIL_ANIMATION_RETRY)
#include "stlport46/animation_retry.h"
#else
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

// Modified STLport 4.6 vector excerpts for original animation instantiations.
#ifndef FABLE_STLPORT46_ANIMATION_H
#define FABLE_STLPORT46_ANIMATION_H
#define FABLE_STLPORT_CONTAINERS_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{};struct __false_type{};
template<class T>struct __type_traits{typedef __false_type is_POD_type;typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;};
template<>struct __type_traits<unsigned char>{typedef __true_type is_POD_type;typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T>class allocator{public:allocator(){} T*allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void __destroy_aux(I a,I b,const __false_type&){for(;a!=b;++a)_Destroy(&*a);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
inline void*__copy_trivial(const void*a,const void*b,void*d){return b==a?d:(char*)memmove(d,a,(const char*)b-(const char*)a)+((const char*)b-(const char*)a);}
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};struct random_access_iterator_tag:public bidirectional_iterator_tag{};
template<class I,class O,class D>inline O __copy(I a,I b,O d,const random_access_iterator_tag&,D*){for(D n=b-a;n>0;--n){*d=*a;++a;++d;}return d;}
template<class I,class O>inline O __copy_ptrs(I a,I b,O d,const __false_type&){return __copy(a,b,d,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class I,class O,class D>inline O __copy_backward(I a,I b,O d,const random_access_iterator_tag&,D*){for(D n=b-a;n>0;--n)*--d=*--b;return d;}
template<class I,class O>inline O __copy_backward_ptrs(I a,I b,O d,const __false_type&){return __copy_backward(a,b,d,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class I,class O>inline O __copy_backward_ptrs(I a,I b,O d,const __true_type&){ptrdiff_t n=b-a;return n?(O)memmove(d-n,a,n*sizeof(*a)):d;}
template<class I,class T>inline void fill(I a,I b,const T&x){for(;a!=b;++a)*a=x;}
template<class I,class O>inline O __copy_ptrs(I a,I b,O d,const __true_type&){return(O)__copy_trivial(a,b,d);}
template<class T>inline T*copy(const T*a,const T*b,T*d){return __copy_ptrs(a,b,d,typename __type_traits<T>::has_trivial_assignment_operator());}
template<class T>inline T*copy(T*a,T*b,T*d){return __copy_ptrs(a,b,d,typename __type_traits<T>::has_trivial_assignment_operator());}
template<class I,class O>inline O __uninitialized_copy(I a,I b,O d,const __false_type&){O cur=d;for(;a!=b;++a,++cur)_Construct(&*cur,*a);return cur;}
template<class I,class O>inline O __uninitialized_copy(I a,I b,O d,const __true_type&){return(O)__copy_trivial(a,b,d);}
template<class T>inline T*fill_n(T*p,size_t n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __false_type&){I cur=p;for(;n>0;--n,++cur)_Construct(&*cur,x);return cur;}
template<class T,class A>class _Vector_base{public:
 explicit _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T*iterator;typedef const T*const_iterator;typedef size_t size_type;typedef A allocator_type;typedef typename __type_traits<T>::is_POD_type _IsPODType;
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T*s,T*f,T*e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 void _M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end=false);
 void _M_insert_overflow(T*position,const T&x,const __false_type&,size_type fill_len,bool at_end=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 T*new_start=this->_M_end_of_storage.allocate(len);T*new_finish=new_start;
 new_finish=__uninitialized_copy(this->_M_start,position,new_start,_IsPODType());
 if(fill_len==1){_Construct(new_finish,x);++new_finish;}else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,_IsPODType());
 if(!at_end)new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,_IsPODType());
 _M_clear();_M_set(new_start,new_finish,new_start+len);
 }
 template<class I>pointer _M_allocate_and_copy(size_type n,I first,I last){pointer result=this->_M_end_of_storage.allocate(n);__uninitialized_copy(first,last,result,_IsPODType());return result;}

public:
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 explicit vector(size_type n):_Vector_base<T,A>(n,A()){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,T(),_IsPODType());}
 vector(size_type n,const T&x,const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,x,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 vector&operator=(const vector&);
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
 iterator erase(iterator first,iterator last){iterator i=__copy_ptrs(last,this->_M_finish,first,typename __type_traits<T>::has_trivial_assignment_operator());_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
 void clear(){erase(begin(),end());}
 void _M_fill_insert(iterator,size_type,const T&);
 void resize(size_type n,T x){if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
 void resize(size_type n){resize(n,T());}
};
template<class T,class A>vector<T,A>&vector<T,A>::operator=(const vector& x){
 if(&x!=this){const size_type xlen=x.size();if(xlen>capacity()){pointer tmp=_M_allocate_and_copy(xlen,(const_pointer)x._M_start+0,(const_pointer)x._M_finish+0);_M_clear();this->_M_start=tmp;this->_M_end_of_storage._M_data=this->_M_start+xlen;}
 else if(size()>=xlen){pointer i=__copy_ptrs((const_pointer)x._M_start+0,(const_pointer)x._M_finish+0,(pointer)this->_M_start,typename __type_traits<T>::has_trivial_assignment_operator());_Destroy(i,this->_M_finish);}
 else{__copy_ptrs((const_pointer)x._M_start,(const_pointer)x._M_start+size(),(pointer)this->_M_start,typename __type_traits<T>::has_trivial_assignment_operator());__uninitialized_copy((const_pointer)x._M_start+size(),(const_pointer)x._M_finish+0,this->_M_finish,_IsPODType());}this->_M_finish=this->_M_start+xlen;}return *this;
}
template<class T,class A>void vector<T,A>::_M_fill_insert(iterator position,size_type n,const T&x){
 if(n!=0){if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T x_copy=x;const size_type elems_after=this->_M_finish-position;pointer old_finish=this->_M_finish;
 if(elems_after>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());this->_M_finish+=n;__copy_backward_ptrs(position,old_finish-n,old_finish,typename __type_traits<T>::has_trivial_assignment_operator());fill(position,position+n,x_copy);}
 else{__uninitialized_fill_n(this->_M_finish,n-elems_after,x_copy,_IsPODType());this->_M_finish+=n-elems_after;__uninitialized_copy(position,old_finish,this->_M_finish,_IsPODType());this->_M_finish+=elems_after;fill(position,old_finish,x_copy);}}
 else _M_insert_overflow(position,x,_IsPODType(),n);}
}
template<class T,class A>void vector<T,A>::_M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);pointer new_start=this->_M_end_of_storage.allocate(len);pointer new_finish=(pointer)__copy_trivial(this->_M_start,position,new_start);
 new_finish=fill_n(new_finish,fill_len,x);if(!at_end)new_finish=(pointer)__copy_trivial(position,this->_M_finish,new_finish);_M_clear();_M_set(new_start,new_finish,new_start+len);
}

}
#endif

#endif
