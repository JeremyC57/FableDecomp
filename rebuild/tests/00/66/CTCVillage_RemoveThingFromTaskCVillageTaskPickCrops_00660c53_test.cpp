#ifndef _MSC_VER
#ifndef _GLIBCXX_INCLUDE_NEXT_C_HEADERS
#define _GLIBCXX_INCLUDE_NEXT_C_HEADERS
#endif
#endif
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
// Modified for project naming and a self-contained fixture.
// Self-contained exact-body fixture; 43 defined cases. Embedded shared C++ definitions; controlled external boundaries.
#ifndef FABLE_FIXTURE_EMBED_2AC39537E6CB49AE
#define FABLE_FIXTURE_EMBED_2AC39537E6CB49AE

#include <stdio.h>
#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_CVILLAGETASKPICKCROPS
#ifndef FABLE_FIXTURE_EMBED_789F63606FC6EC36
#define FABLE_FIXTURE_EMBED_789F63606FC6EC36
#ifndef FABLE_ENGINE_CISTASK_H
#define FABLE_ENGINE_CISTASK_H
#ifndef FABLE_FIXTURE_EMBED_2551CAD4DB920F4C
#define FABLE_FIXTURE_EMBED_2551CAD4DB920F4C
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H





#include <stddef.h>
#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE) || defined(FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE) || defined(FABLE_RETAIL_STLP_SORT_INTERNALS) || defined(FABLE_RETAIL_STLP_HEAP)
#ifdef _MSC_VER
#define FABLE_STLP_NOINLINE __declspec(noinline)
#define FABLE_STLP_FASTCALL __fastcall
#else
#define FABLE_STLP_NOINLINE
#define FABLE_STLP_FASTCALL
#endif
#endif
namespace std {




struct __true_type { __true_type() { } };
#ifdef FABLE_RETAIL_STLP_FALSE_TAG_POD


struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#ifdef FABLE_RETAIL_STLP_FIND_IF_INPUT

template<class I> struct iterator_traits {
    typedef typename I::iterator_category iterator_category;
    typedef typename I::value_type value_type;
    typedef typename I::difference_type difference_type;
    typedef typename I::pointer pointer;
    typedef typename I::reference reference;
};
#else
template<class I> struct iterator_traits;
#endif
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
template<class T> struct __type_traits {
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};
#define FABLE_STLP_SCALAR_TRAITS(T) template<> struct __type_traits<T> { \
    typedef __true_type has_trivial_default_constructor; typedef __true_type has_trivial_copy_constructor; \
    typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; typedef __true_type is_POD_type; };
FABLE_STLP_SCALAR_TRAITS(bool)
FABLE_STLP_SCALAR_TRAITS(char)
FABLE_STLP_SCALAR_TRAITS(signed char)
FABLE_STLP_SCALAR_TRAITS(unsigned char)
FABLE_STLP_SCALAR_TRAITS(short)
FABLE_STLP_SCALAR_TRAITS(unsigned short)
FABLE_STLP_SCALAR_TRAITS(int)
FABLE_STLP_SCALAR_TRAITS(unsigned int)
FABLE_STLP_SCALAR_TRAITS(long)
FABLE_STLP_SCALAR_TRAITS(unsigned long)
FABLE_STLP_SCALAR_TRAITS(__int64)
FABLE_STLP_SCALAR_TRAITS(unsigned __int64)
FABLE_STLP_SCALAR_TRAITS(float)
FABLE_STLP_SCALAR_TRAITS(double)
FABLE_STLP_SCALAR_TRAITS(long double)
#undef FABLE_STLP_SCALAR_TRAITS
template<class T> struct __type_traits<T*> {
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }

inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#ifdef FABLE_RETAIL_STLP_FILL_LOCAL_ITERATOR
template<class I,class T> inline void fill(I begin,I last,const T& x) { I first=begin; for(;first!=last;++first) *first=x; }
#else
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
#endif
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class I,class O,class D> inline O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*) {
    for(D n=last-first;n>0;--n) { *result=*first;++first;++result; } return result;
}
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __true_type&) {
    return (Output)__copy_trivial(first,last,result);
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
#ifdef FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD


template<class R,class B,class D> inline B __copy_backward(R first,R last,B result,const random_access_iterator_tag&,D*) {
    for(D n=last-first;n>0;--n) *--result=*--last;
    return result;
}
template<class I,class O> inline O __copy_backward_ptrs(I first,I last,O result,const __false_type&) {
    return __copy_backward(first,last,result,random_access_iterator_tag(),(typename iterator_traits<I>::difference_type*)0);
}
#else
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif

template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}

#ifdef FABLE_STLP_OUTOFLINE_UNINIT_COPY



#pragma auto_inline(off)
template<class I,class O> __declspec(noinline) O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
#pragma auto_inline(on)
#elif defined(FABLE_RETAIL_STLP_UNINIT_OUTOFLINE)



#pragma auto_inline(off)
template<class I,class O> O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
#pragma auto_inline(on)
#else
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
#endif
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
#ifdef FABLE_STLP_OUTOFLINE_UNINIT_FILL_N




#pragma auto_inline(off)
template<class I,class N,class T> __declspec(noinline) I __fastcall __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#pragma auto_inline(on)
#elif defined(FABLE_RETAIL_STLP_UNINIT_OUTOFLINE)
#pragma auto_inline(off)
template<class I,class N,class T> I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {   
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#pragma auto_inline(on)
#else
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __true_type&) {
    return std::fill_n(first,n,x);
}
template<class I,class N,class T> inline I uninitialized_fill_n(I first,N n,const T& x) {
    typedef typename iterator_traits<I>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,x,IsPOD());
}

template<class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template<class _Tp1> allocator(const allocator<_Tp1>&) { }
    ~allocator() { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
#if defined(FABLE_RETAIL_STLP_VECTOR_SWAP) || defined(FABLE_RETAIL_STLP_SORT_INTERNALS)




template<class _Tp> inline void swap(_Tp& a,_Tp& b) { _Tp tmp=a; a=b; b=tmp; }
#endif

template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=_M_end_of_storage.allocate(n);
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
    vector(size_type n,const _Tp& value,const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(n,a) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,value);
    }
#ifdef FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
    
    
    
    
    explicit vector(size_type n);
#else
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
#endif
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#ifdef FABLE_RETAIL_STLP_VECTOR_ASSIGN
    
    
    vector& operator=(const vector& x);
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_C
    
    
    
    
    vector& operator=(const vector& x);
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data-this->_M_start); }
    template <class _ForwardIter> _Tp* _M_allocate_and_copy(size_type n,_ForwardIter first,_ForwardIter last) {
        _Tp* result=this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first,last,result,_IsPODType());
        return result;
    }
#endif
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
#ifdef FABLE_RETAIL_STLP_VECTOR_SWAP_OUTOFLINE
    
    
    
    
    void swap(vector& x);
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_SWAP
    
    void swap(vector& x) { std::swap(this->_M_start,x._M_start); std::swap(this->_M_finish,x._M_finish); std::swap(this->_M_end_of_storage,x._M_end_of_storage); }
#endif
    _Tp& front() { return *this->_M_start; }
    _Tp& back() { return *(end()-1); }
    const _Tp& back() const { return *(end()-1); }
    void pop_back() { --this->_M_finish; _Destroy(this->_M_finish); }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
    
    
    
    
    void push_back(const _Tp& x);
#else
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#endif
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#ifdef FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
    
    
    
    
    iterator erase(iterator position);
#else
    iterator erase(iterator position) {
        if(position+1!=end()) __copy_ptrs(position+1,this->_M_finish,position,_IsPODType());
        --this->_M_finish;_Destroy(this->_M_finish);return position;
    }
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
    
    
    
    iterator erase(iterator first,iterator last);
#else
    iterator erase(iterator first,iterator last) {
        iterator result=__copy_ptrs(last,this->_M_finish,first,_IsPODType());
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
#endif
    void clear() { erase(begin(),end()); }
    
    
#ifdef FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
    
    
    
    
    
    
    __declspec(noinline) void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#else
    void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
    
    
    void resize(size_type n);
#else
    void resize(size_type n) { resize(n,_Tp()); }
#endif
    void _M_fill_insert(iterator position,size_type n,const _Tp& x);
#ifdef FABLE_RETAIL_STLP_VECTOR
    
    
    void reserve(size_type n);
#endif
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd);
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd);
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
};

#ifdef FABLE_RETAIL_STLP_VECTOR_C


template<class _Tp,class _Alloc> vector<_Tp,_Alloc>& vector<_Tp,_Alloc>::operator=(const vector<_Tp,_Alloc>& x) {
    if(&x!=this) {
        const size_type xlen=x.size();
        if(xlen>capacity()) {
            _Tp* tmp=_M_allocate_and_copy(xlen,(const _Tp*)x._M_start+0,(const _Tp*)x._M_finish+0);
            _M_clear();
            this->_M_start=tmp;
            this->_M_end_of_storage._M_data=this->_M_start+xlen;
        } else if(size()>=xlen) {
            _Tp* i=__copy_ptrs((const _Tp*)x._M_start+0,(const _Tp*)x._M_finish+0,(_Tp*)this->_M_start,_IsPODType());
            _Destroy(i,this->_M_finish);
        } else {
            __copy_ptrs((const _Tp*)x._M_start,(const _Tp*)x._M_start+size(),(_Tp*)this->_M_start,_IsPODType());
            __uninitialized_copy((const _Tp*)x._M_start+size(),(const _Tp*)x._M_finish+0,this->_M_finish,_IsPODType());
        }
        this->_M_finish=this->_M_start+xlen;
    }
    return *this;
}
#endif
#ifndef FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY

template<class _Tp,class _Alloc> void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
    newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
    if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
    else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
    if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
template<class _Tp,class _Alloc> void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    _Tp* newStart=this->_M_end_of_storage.allocate(len);
    _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_RESERVE





template<class _Tp,class _Alloc> __declspec(noinline) void vector<_Tp,_Alloc>::reserve(size_type n) {
    if(size_type(this->_M_end_of_storage._M_data-this->_M_start)<n) {
        const size_type old_size=size();
        _Tp* tmp;
        if(this->_M_start) {
            tmp=this->_M_end_of_storage.allocate(n);
            __uninitialized_copy((const _Tp*)this->_M_start,(const _Tp*)this->_M_finish,tmp,_IsPODType());
            _M_clear();
        } else {
            tmp=this->_M_end_of_storage.allocate(n);
        }
        _M_set(tmp,tmp+old_size,tmp+n);
    }
}
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR




template<class _Tp,class _Alloc> void vector<_Tp,_Alloc>::_M_fill_insert(iterator position,size_type n,const _Tp& x) {
    if(n!=0) {
        if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
            _Tp x_copy=x;
            const size_type elems_after=this->_M_finish-position;
            _Tp* old_finish=this->_M_finish;
            if(elems_after>n) {
                __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                this->_M_finish+=n;
                __copy_backward_ptrs(position,old_finish-n,old_finish,_IsPODType());
                fill(position,position+n,x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish,n-elems_after,x_copy);
                this->_M_finish+=n-elems_after;
                __uninitialized_copy(position,old_finish,this->_M_finish,_IsPODType());
                this->_M_finish+=elems_after;
                fill(position,old_finish,x_copy);
            }
        } else _M_insert_overflow(position,x,_IsPODType(),n,false);
    }
}
#endif

template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair():first(_T1()),second(_T2()){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
    template<class U1,class U2> pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
};
template<class _T1,class _T2> inline pair<_T1,_T2> __cdecl make_pair(const _T1& a,const _T2& b) { return pair<_T1,_T2>(a,b); }
#ifdef FABLE_RETAIL_STLP_MAKE_PAIR_ARRAYS




template<class _T1,class _T2,int _Sz> inline pair<_T1,_T2 const*> __cdecl make_pair(_T1 const& a,_T2 const (&b)[_Sz]) { return pair<_T1,_T2 const*>(a,static_cast<_T2 const*>(b)); }
template<class _T1,class _T2,int _Sz> inline pair<_T1 const*,_T2> __cdecl make_pair(_T1 const (&a)[_Sz],_T2 const& b) { return pair<_T1 const*,_T2>(static_cast<_T1 const*>(a),b); }
template<class _T1,class _T2,int _Sz1,int _Sz2> inline pair<_T1 const*,_T2 const*> __cdecl make_pair(_T1 const (&a)[_Sz1],_T2 const (&b)[_Sz2]) { return pair<_T1 const*,_T2 const*>(static_cast<_T1 const*>(a),static_cast<_T2 const*>(b)); }
#endif

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
#ifdef FABLE_RETAIL_STLP_LIST
    
    void _M_decr() { _M_node=_M_node->_M_prev; }
#endif
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
#if defined(FABLE_RETAIL_STLP_LIST_NOINLINE_NE) && defined(_MSC_VER)
    
    
    
    
    __declspec(noinline)
#endif
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
#ifdef FABLE_RETAIL_STLP_FIND_IF_INPUT
    typedef _Tp value_type;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
#endif
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#ifdef FABLE_RETAIL_STLP_LIST
    _List_iterator& operator--() { this->_M_decr(); return *this; }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_REVERSE
    
    
    _List_iterator operator--(int) { _List_iterator tmp=*this; this->_M_decr(); return tmp; }
#endif
};
#ifdef FABLE_RETAIL_STLP_LIST_REVERSE



template<class _Iterator> class reverse_iterator {
protected:
    _Iterator current;
public:
    typedef _Iterator iterator_type;
    typedef typename _Iterator::reference reference;
    typedef typename _Iterator::pointer pointer;
    reverse_iterator() { }
    explicit reverse_iterator(iterator_type x) : current(x) { }
    reverse_iterator(const reverse_iterator& x) : current(x.current) { }
    iterator_type base() const { return current; }
    reference operator*() const { _Iterator tmp=current; return *--tmp; }
    pointer operator->() const { return &(operator*()); }
    reverse_iterator& operator++() { --current; return *this; }
    reverse_iterator operator++(int) { reverse_iterator tmp=*this; --current; return tmp; }
    reverse_iterator& operator--() { ++current; return *this; }
    reverse_iterator operator--(int) { reverse_iterator tmp=*this; ++current; return tmp; }
    bool operator==(const reverse_iterator& y) const { return current==y.current; }
    bool operator!=(const reverse_iterator& y) const { return current!=y.current; }
};
#endif
#ifdef FABLE_RETAIL_STLP_ADVANCE



template<class _Tp,class _Traits> inline bidirectional_iterator_tag __ITERATOR_CATEGORY(const _List_iterator<_Tp,_Traits>&) { return bidirectional_iterator_tag(); }
template<class _BidirectionalIterator,class _Distance> inline void __advance(_BidirectionalIterator& i,_Distance n,const bidirectional_iterator_tag&) {
    if(n>0) while(n--) ++i; else while(n++) --i;
}
template<class _InputIterator,class _Distance> inline void advance(_InputIterator& i,_Distance n) { __advance(i,n,__ITERATOR_CATEGORY(i)); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE


template<class _InputIterator> inline ptrdiff_t __distance(const _InputIterator& first,const _InputIterator& last,const input_iterator_tag&) {
    ptrdiff_t n=0; _InputIterator it(first); while(it!=last) { ++it; ++n; } return n;
}
template<class _InputIterator> inline ptrdiff_t distance(const _InputIterator& first,const _InputIterator& last) {
    return __distance(first,last,input_iterator_tag());
}
#endif
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
    
    
    
    
    _List_base(const _Alloc&);
    ~_List_base();
#else
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#ifdef FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
    
    
    
    
    void clear();
#else
    void clear() {
        _List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;
        while(cur!=_M_node._M_data) {
            _List_node<_Tp>* tmp=cur;
            cur=(_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp,1);
        }
        _M_node._M_data->_M_next=_M_node._M_data;
        _M_node._M_data->_M_prev=_M_node._M_data;
    }
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
    typedef _Tp value_type;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) { insert(begin(),x.begin(),x.end()); }
#ifdef FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
    
    
    
    FABLE_STLP_NOINLINE ~list() { }
#else
    ~list() { }
#endif
    _Alloc get_allocator() const { return _Alloc(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
#ifdef FABLE_RETAIL_STLP_LIST_REVERSE
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
#endif
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
    template<class Input> void insert(iterator position,Input first,Input last) { _M_insert_dispatch(position,first,last,__false_type()); }
    template<class Input> void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) { for(;first!=last;++first) insert(position,*first); }
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
#ifdef FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
    
    
    
    FABLE_STLP_NOINLINE iterator insert(iterator position,const _Tp& x) {
#else
    iterator insert(iterator position,const _Tp& x) {
#endif
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#ifdef FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT
    
    
    
    
    
    iterator insert(iterator position);
    void push_back() { insert(end()); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST
    
    
    
    
#ifdef FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
    
    
    
    size_t size() const { size_t result=distance(begin(),end()); return result; }
#else
    size_t size() const { ptrdiff_t n=0; for(const_iterator first=begin(),last=end();first!=last;++first) ++n; return n; }
#endif
    _Tp& front() { return *begin(); }
    void push_front(const _Tp& x) { insert(begin(),x); }
    
    
    _Tp& back() { iterator tmp=end(); return *(--tmp); }
    const _Tp& back() const { const_iterator tmp=end(); return *(--tmp); }
    
    
    iterator erase(iterator position) {
        _List_node_base* next_node=position._M_node->_M_next;
        _List_node_base* prev_node=position._M_node->_M_prev;
        _List_node<_Tp>* n=(_List_node<_Tp>*)position._M_node;
        prev_node->_M_next=next_node;
        next_node->_M_prev=prev_node;
        _Destroy(&n->_M_data);
        this->_M_node.deallocate(n,1);
        return iterator(next_node);
    }
    void pop_front() { erase(begin()); }
#ifdef FABLE_RETAIL_STLP_LIST_ERASE_RANGE
    
    
    
    iterator erase(iterator first,iterator last) {
        while(first!=last) erase(first++);
        return last;
    }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_POPBACK
    
    
    void pop_back() { iterator tmp=end(); erase(--tmp); }
#endif
#endif
#ifdef FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
    
    
    template<class _Predicate> void remove_if(_Predicate pred);
#endif
};

template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
#ifdef FABLE_RETAIL_STLP_BITSET_INDEX
    _WordT& _M_getword(size_t n) { return _M_w[_S_whichword(n)]; }
#endif
};
inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
#ifdef FABLE_RETAIL_STLP_BITSET_TEST_DECL
    
    
    
    bool test(size_t n) const;
#else
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
#endif
#ifdef FABLE_RETAIL_STLP_BITSET_INDEX
    
    
    
    
    
    struct reference {
        typedef _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> _Bitset_base;
        typedef bitset<_Nb> _Bitset;
        unsigned long* _M_wp;
        size_t _M_bpos;
        reference() {}
        reference(_Bitset& b,size_t pos) { _M_wp=&b._M_getword(pos); _M_bpos=_Bitset_base::_S_whichbit(pos); }
        ~reference() {}
        reference& operator=(bool x) { if(x) *_M_wp|=_Bitset_base::_S_maskbit(_M_bpos); else *_M_wp&=~_Bitset_base::_S_maskbit(_M_bpos); return *this; }
        reference& operator=(const reference& j) { if((*(j._M_wp)&_Bitset_base::_S_maskbit(j._M_bpos))) *_M_wp|=_Bitset_base::_S_maskbit(_M_bpos); else *_M_wp&=~_Bitset_base::_S_maskbit(_M_bpos); return *this; }
        bool operator~() const { return (*(_M_wp)&_Bitset_base::_S_maskbit(_M_bpos))==0; }
        operator bool() const { return (*(_M_wp)&_Bitset_base::_S_maskbit(_M_bpos))!=0; }
        reference& flip() { *_M_wp^=_Bitset_base::_S_maskbit(_M_bpos); return *this; }
    };
    reference operator[](size_t pos) { return reference(*this,pos); }
    bool operator[](size_t pos) const { return _Unchecked_test(pos); }
#endif
};

template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _Arg1,class _Arg2,class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template<class _Tp> struct less : public binary_function<_Tp,_Tp,bool> {
    bool operator()(const _Tp& x,const _Tp& y) const { return x<y; }
};
#ifdef FABLE_RETAIL_STLP_SORT_FASTCALL



template<class _RandomAccessIter,class _Compare> void __fastcall sort(_RandomAccessIter,_RandomAccessIter,_Compare);
#else
template<class _RandomAccessIter,class _Compare> void sort(_RandomAccessIter,_RandomAccessIter,_Compare);
#endif
#ifdef FABLE_RETAIL_STLP_SORT2



template<class _RandomAccessIter> void sort(_RandomAccessIter,_RandomAccessIter);
#endif
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
#ifdef FABLE_RETAIL_STLP_BACK_INSERTER




template<class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    typedef _Container container_type;
    explicit back_insert_iterator(_Container& x) : container(&x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& value) { container->push_back(value); return *this; }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
    back_insert_iterator<_Container> operator++(int) { return *this; }
};
template<class _Container> inline back_insert_iterator<_Container> back_inserter(_Container& x) { return back_insert_iterator<_Container>(x); }
template<class _InputIter,class _OutputIter> _OutputIter __fastcall copy(_InputIter first,_InputIter last,_OutputIter result);
#endif
#ifdef FABLE_RETAIL_STLP_SORT_INTERNALS






template<class _ForwardIter1,class _ForwardIter2> inline void iter_swap(_ForwardIter1 a,_ForwardIter2 b) { swap(*a,*b); }
template<class _RandomAccessIter,class _BidirectionalIter,class _Distance>
FABLE_STLP_NOINLINE _BidirectionalIter FABLE_STLP_FASTCALL __copy_backward(_RandomAccessIter first,_RandomAccessIter last,
                                                                           _BidirectionalIter result,const random_access_iterator_tag&,_Distance*) {
    for(_Distance n=last-first;n>0;--n) *--result=*--last;
    return result;
}
template<class _BI1,class _BI2> inline _BI2 copy_backward(_BI1 first,_BI1 last,_BI2 result) {
    return __copy_backward(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
template<class _RandomAccessIter,class _Tp,class _Compare>
_RandomAccessIter FABLE_STLP_FASTCALL __unguarded_partition(_RandomAccessIter first,_RandomAccessIter last,_Tp pivot,_Compare comp) {
    for(;;) {
        while(comp(*first,pivot)) ++first;
        --last;
        while(comp(pivot,*last)) --last;
        if(!(first<last)) return first;
        iter_swap(first,last);
        ++first;
    }
}
template<class _RandomAccessIter,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __unguarded_linear_insert(_RandomAccessIter last,_Tp val,_Compare comp);
template<class _RandomAccessIter,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __linear_insert(_RandomAccessIter first,_RandomAccessIter last,_Tp val,_Compare comp) {
    if(comp(val,*first)) {
        copy_backward(first,last,last+1);
        *first=val;
    } else
        __unguarded_linear_insert(last,val,comp);
}
#endif
#ifdef FABLE_RETAIL_STLP_HEAP



template<class _RandomAccessIterator,class _Distance,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __adjust_heap(_RandomAccessIterator first,_Distance holeIndex,_Distance len,_Tp val,_Compare comp);
template<class _RandomAccessIterator,class _Tp,class _Compare,class _Distance>
void FABLE_STLP_FASTCALL __pop_heap(_RandomAccessIterator first,_RandomAccessIterator last,_RandomAccessIterator result,
                                    _Tp val,_Compare comp,_Distance*) {
    *result=*first;
    __adjust_heap(first,_Distance(0),_Distance(last-first),val,comp);
}
#endif
}
#endif

#endif
#ifndef FABLE_FIXTURE_EMBED_9C19DD117EC1E964
#define FABLE_FIXTURE_EMBED_9C19DD117EC1E964
#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H









































#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif

class CCPPointerInfo {
public:
    unsigned long RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);

template <class T> class CCountedPointer {
public:
    T* Data;                                                             
    CCPPointerInfo* Info;                                                

    CCountedPointer() : Data(0), Info(0) { }
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR) && !defined(FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR)
    
    
    
    CCountedPointer(T* data);
#else
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR) && defined(_MSC_VER)
    
    
    
    
    
    
    __declspec(noinline)
#endif
    CCountedPointer(T* data) : Data(data)
    {
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER
        
        
        if (Data)
#else
        if (data)
#endif
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
#endif
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
    
    
    
    ~CCountedPointer();
#else
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
#endif
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
    void Validate() const;                                               
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR
    
    
    
    
    void Reset(T* data)
    {
        this->~CCountedPointer();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
#else
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
#endif
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              
    }
    void reset(T* data) { Reset(data); }                                 
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    
};

#endif 

#endif
#ifndef FABLE_FIXTURE_EMBED_5F396F1F3166DA58
#define FABLE_FIXTURE_EMBED_5F396F1F3166DA58
#ifndef FABLE_ENGINE_CVILLAGETASKBASE_H
#define FABLE_ENGINE_CVILLAGETASKBASE_H
#ifndef FABLE_FIXTURE_EMBED_E13B60B466CDF77A
#define FABLE_FIXTURE_EMBED_E13B60B466CDF77A
#ifndef FABLE_ENGINE_CBASEOBJECT_H
#define FABLE_ENGINE_CBASEOBJECT_H
















#ifdef FABLE_RETAIL_BASE_OBJECT
#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif
#ifndef FABLE_FIXTURE_EMBED_01C0CF4FD353D899
#define FABLE_FIXTURE_EMBED_01C0CF4FD353D899
#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H







#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif

class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);

#endif 

#endif
class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
protected:
    CBaseObject();                                                       
    CBaseObject(const CBaseObject&);                                     
    CBaseObject& operator=(const CBaseObject&);                          
public:
    void ClearIntelligentPointers();                                     
    virtual void Validate() const;                                       
protected:
    virtual ~CBaseObject();                                              
    void InternalValidate() const;                                       
public:
    CBaseObjectPointer* IntelligentPointer;                              
};
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
#else 




#include <stddef.h>
#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif

struct CBaseObjectPointer;

#pragma pack(push, 1)
class CBaseObject {
public:
    void*               __vftable;                           
    CBaseObjectPointer* IntelligentPointer;                  
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, IntelligentPointer) == 0x4);

#endif 
#endif 

#endif
class CThing;

enum EVillageTaskType { VILLAGE_TASK_TORCH, VILLAGE_TASK_PATROL, VILLAGE_TASK_PICK_CROPS, VILLAGE_TASK_SECURITY_SWEEP, VILLAGE_TASK_HORN, VILLAGE_TASK_CARRY_CRATES };

class CVillageTaskBase : public CBaseObject {
public:
    virtual ~CVillageTaskBase();
    virtual EVillageTaskType GetTaskType() const = 0;
    virtual bool IsValid() const = 0;
    virtual bool IsTaskAvailable() const = 0;
    virtual bool IsThingInvolvedWithTask(const CThing&) const = 0;
    virtual void RemoveThingFromTask(const CThing&, bool&) = 0;
    virtual bool IsCreatureAssignedToTask(const CThing&) const = 0;
    virtual void AssignCreatureToTask(const CThing&) = 0;
    CVillageTaskBase();
    CVillageTaskBase(const CVillageTaskBase&);
    CVillageTaskBase& operator=(const CVillageTaskBase&);
};
FABLE_STATIC_ASSERT(sizeof(CVillageTaskBase)==8);
#endif

#endif

class CIsTask : public std::unary_function<CCountedPointer<CVillageTaskBase>,bool> {
    const CVillageTaskBase& Task;
public:
    CIsTask(const CVillageTaskBase& task) : Task(task) { }
    
    bool operator()(const CCountedPointer<CVillageTaskBase>& p) const { return p.Get()==&Task; }
};
FABLE_STATIC_ASSERT(sizeof(CIsTask)==4);
#endif

#endif
#ifndef FABLE_FIXTURE_EMBED_A3ECA271B5F9A65B
#define FABLE_FIXTURE_EMBED_A3ECA271B5F9A65B
#ifndef FABLE_ENGINE_CVILLAGETASKPICKCROPS_H
#define FABLE_ENGINE_CVILLAGETASKPICKCROPS_H





#include <stddef.h>
#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif

#ifdef FABLE_RETAIL_CVILLAGETASKPICKCROPS
class CThingObject;
#endif
#ifdef FABLE_RETAIL_CVILLAGETASKPICKCROPS

#ifndef FABLE_FIXTURE_EMBED_5F396F1F3166DA58
#define FABLE_FIXTURE_EMBED_5F396F1F3166DA58
#ifndef FABLE_ENGINE_CVILLAGETASKBASE_H
#define FABLE_ENGINE_CVILLAGETASKBASE_H
#ifndef FABLE_FIXTURE_EMBED_E13B60B466CDF77A
#define FABLE_FIXTURE_EMBED_E13B60B466CDF77A
#ifndef FABLE_ENGINE_CBASEOBJECT_H
#define FABLE_ENGINE_CBASEOBJECT_H
















#ifdef FABLE_RETAIL_BASE_OBJECT
#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif
#ifndef FABLE_FIXTURE_EMBED_01C0CF4FD353D899
#define FABLE_FIXTURE_EMBED_01C0CF4FD353D899
#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H







#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif

class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);

#endif 

#endif
class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
protected:
    CBaseObject();                                                       
    CBaseObject(const CBaseObject&);                                     
    CBaseObject& operator=(const CBaseObject&);                          
public:
    void ClearIntelligentPointers();                                     
    virtual void Validate() const;                                       
protected:
    virtual ~CBaseObject();                                              
    void InternalValidate() const;                                       
public:
    CBaseObjectPointer* IntelligentPointer;                              
};
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
#else 




#include <stddef.h>
#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif

struct CBaseObjectPointer;

#pragma pack(push, 1)
class CBaseObject {
public:
    void*               __vftable;                           
    CBaseObjectPointer* IntelligentPointer;                  
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, IntelligentPointer) == 0x4);

#endif 
#endif 

#endif
class CThing;

enum EVillageTaskType { VILLAGE_TASK_TORCH, VILLAGE_TASK_PATROL, VILLAGE_TASK_PICK_CROPS, VILLAGE_TASK_SECURITY_SWEEP, VILLAGE_TASK_HORN, VILLAGE_TASK_CARRY_CRATES };

class CVillageTaskBase : public CBaseObject {
public:
    virtual ~CVillageTaskBase();
    virtual EVillageTaskType GetTaskType() const = 0;
    virtual bool IsValid() const = 0;
    virtual bool IsTaskAvailable() const = 0;
    virtual bool IsThingInvolvedWithTask(const CThing&) const = 0;
    virtual void RemoveThingFromTask(const CThing&, bool&) = 0;
    virtual bool IsCreatureAssignedToTask(const CThing&) const = 0;
    virtual void AssignCreatureToTask(const CThing&) = 0;
    CVillageTaskBase();
    CVillageTaskBase(const CVillageTaskBase&);
    CVillageTaskBase& operator=(const CVillageTaskBase&);
};
FABLE_STATIC_ASSERT(sizeof(CVillageTaskBase)==8);
#endif

#endif
#ifndef FABLE_FIXTURE_EMBED_C4249C1292A07C93
#define FABLE_FIXTURE_EMBED_C4249C1292A07C93
#ifndef FABLE_ENGINE_CINTELLIGENTPOINTER_H
#define FABLE_ENGINE_CINTELLIGENTPOINTER_H
#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif
class CBaseObject;
class CBaseObjectPointer;
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER











#pragma pack(push, 1)
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                                           
    virtual ~CBaseIntelligentPointer();                                  
    CBaseObject* GetPItem() const;                                       
    void SetPItem(CBaseObject* item);                                    
private:
    CBaseObjectPointer* PData;                                           
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    
    
    
    CIntelligentPointer() { }
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
    
    
    
    CIntelligentPointer(T* pointer);
#else
    CIntelligentPointer(T* pointer) { SetPointer(pointer); }
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
    
    CIntelligentPointer& operator=(const CIntelligentPointer& other);
#endif
    void Set(T* pointer) { SetPointer(pointer); }
    T* Get() const { return static_cast<T*>(GetPItem()); }             
    T* operator->() const { return Get(); }                             
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
    
    
    
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
    
    
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
    
    
    
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
    
    
    
    
    CIntelligentPointer(const CIntelligentPointer& other);
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT
    
    
    
    template<class U> CIntelligentPointer(const CIntelligentPointer<U>& other);
#endif
private:
    void SetPointer(const T* pointer) { SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer))); }
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE



template<class T> bool FABLE_FASTCALL operator!=(const CBaseObject* item, const CIntelligentPointer<T>& pointer);
#endif
#else


class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
private:
    CBaseObjectPointer* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
};
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
#endif
#endif

#endif
#ifndef FABLE_FIXTURE_EMBED_171944BE6855B8BB
#define FABLE_FIXTURE_EMBED_171944BE6855B8BB
#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H
#ifndef FABLE_FIXTURE_EMBED_3A0EA43A47735ACB
#define FABLE_FIXTURE_EMBED_3A0EA43A47735ACB







#ifndef FABLE_COMPILER_H
#define FABLE_COMPILER_H

#if defined(_MSC_VER)
#define FABLE_FORCEINLINE __forceinline
#else
#define FABLE_FORCEINLINE inline
#endif

#endif 

#endif




#if defined(_MSC_VER)
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#include <math.h>
#ifndef FABLE_FIXTURE_EMBED_334ED73EFC59D3D2
#define FABLE_FIXTURE_EMBED_334ED73EFC59D3D2
#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H
#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif

#ifdef FABLE_RETAIL_C2DCOORDI_TOFLOAT




class C2DVector;
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}
    C2DVector ToC2DCoordF() const;
};
#ifndef FABLE_FIXTURE_EMBED_AFB680C8A676E5A2
#define FABLE_FIXTURE_EMBED_AFB680C8A676E5A2
#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H
#ifndef FABLE_FIXTURE_EMBED_3A0EA43A47735ACB
#define FABLE_FIXTURE_EMBED_3A0EA43A47735ACB







#ifndef FABLE_COMPILER_H
#define FABLE_COMPILER_H

#if defined(_MSC_VER)
#define FABLE_FORCEINLINE __forceinline
#else
#define FABLE_FORCEINLINE inline
#endif

#endif 

#endif




#ifdef FABLE_RETAIL_C2DVECTOR
#include <math.h>





extern const float DELTA_NOTIONAL_ZERO;                 
#pragma pack(push, 1)
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
#ifdef FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
    
    
    
    FABLE_FORCEINLINE
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
#ifdef FABLE_RETAIL_C2DVECTOR_INITIALISE
    
    
    void Initialise(float x,float y) { X=x; Y=y; }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D
    
    
    C2DVector(const class C3DVector& v);
#endif
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
    
    const C2DVector operator-() const { return C2DVector(-X,-Y); }
    const C2DVector operator*(float scalar) const { return C2DVector(X*scalar,Y*scalar); }
    const C2DVector& operator+=(const C2DVector& vector) { X+=vector.X; Y+=vector.Y; return *this; }
    
    
    const C2DVector operator/(float scalar) const { float inverse = 1.0f/scalar; return C2DVector(X*inverse,Y*inverse); }
    
    const C2DVector& operator*=(const C2DVector& vector) { X*=vector.X; Y*=vector.Y; return *this; }
    float Dot(const C2DVector& vector) const { return X*vector.X+Y*vector.Y; }
    
    
#ifdef FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
    
    
    
    float GetSquaredMagnitude() const { float squared=Y*Y; squared+=X*X; return squared; }
#else
    float GetSquaredMagnitude() const { return X*X+Y*Y; }
#endif
    
    
    
    float GetMagnitude() const { return sqrtf(X*X+Y*Y); }
    
    
    
    
    
    
    FABLE_FORCEINLINE bool operator==(const C2DVector& vector) const
    {
        return fabs(X-vector.X)<=DELTA_NOTIONAL_ZERO && fabs(Y-vector.Y)<=DELTA_NOTIONAL_ZERO;
    }
#ifdef FABLE_RETAIL_C2DVECTOR_NORMALISE
    
    
    
    
    C2DVector& Normalise();                                                  
    
    const C2DVector& operator*=(float scalar) { X*=scalar; Y*=scalar; return *this; }
    bool IsNonZero() const { return GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO; }
#endif
};
#pragma pack(pop)
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D
inline C2DVector::C2DVector(const C3DVector& v) : X(v.X),Y(v.Y) { }
#endif
#else
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
#ifdef FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
    
    
    
    FABLE_FORCEINLINE
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#endif
#endif

#endif
inline C2DVector C2DCoordI::ToC2DCoordF() const { return C2DVector((float)X,(float)Y); }
#else
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
#endif
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
#endif

#endif
#ifdef FABLE_RETAIL_C3DVECTOR
extern const float DELTA_NOTIONAL_ZERO;                                 



extern unsigned long GFastOneOverSqrtTable[128];                        
#ifdef FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN





FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-(r*r*value))*r*0.5f;}
#else
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-r*r*value)*r*0.5f;}
#endif
#endif
class C3DVector {
public:
    float X,Y,Z;
#ifdef FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
    
    bool operator!=(const C3DVector& v) const {
        return fabs(X-v.X)>DELTA_NOTIONAL_ZERO || fabs(Y-v.Y)>DELTA_NOTIONAL_ZERO || fabs(Z-v.Z)>DELTA_NOTIONAL_ZERO;
    }
#endif
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
#ifdef FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
    
    
    
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector((X*scale),Y*scale,Z*scale);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR
    
    FABLE_VECTOR_INLINE const C3DVector operator-()const{return C3DVector(-X,-Y,-Z);}
    
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#endif
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
#ifdef FABLE_RETAIL_C3DVECTOR
    
    
    
    
#ifdef FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
    
    
    
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Z*Z+Y*Y);}
#endif
    
    
    
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{float squared=X*X;squared+=Y*Y;return squared;}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
#endif
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#ifdef FABLE_RETAIL_C3DVECTOR
    
    
    
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    
    
#ifdef FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
    
    
    
    FABLE_VECTOR_INLINE float GetAngleXY() const {
        float angle=(float)atan2(X,Y)*0.15915494f;
        if(angle<0.0f || angle>=1.0f) {
            angle=(float)fmod((double)angle,1.0);
            if(angle<0.0f) angle+=1.0f;
        }
        return angle;
    }
    FABLE_VECTOR_INLINE C3DVector CrossWithUp() const {return C3DVector(Y,-X,0.0f);}
    FABLE_VECTOR_INLINE const C3DVector& InitialiseFromAngles(float angleXY,float angleYZ,float length) {
        float cosYZ=(float)cos(angleYZ*(double)6.2831855f);
        X=(float)(sin(angleXY*(double)6.2831855f)*(cosYZ*length));
        Y=(float)(cos(angleXY*(double)6.2831855f)*(cosYZ*length));
        Z=(float)(sin(angleYZ*(double)6.2831855f)*length);
        return *this;
    }
#else
    float GetAngleXY() const;
#endif
    
    
    void RotateXY(float angle);                                          
    
    
    static C3DVector __fastcall GetRandomUnitVector(unsigned long* seed);  
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
    
    
    
    
    
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(X*X+Z*Z+Y*Y);X*=factor;Y*=factor;Z*=factor;return *this;}
#else
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(GetSquaredMagnitude());X*=factor;Y*=factor;Z*=factor;return *this;}
#endif
    
    
    
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
    
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=X*X;squared+=Z*Z+Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#else
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=X*X;squared+=Z*Z;squared+=Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#endif
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

#endif
class CThingTrackNode; class CThingObject;
class CVillageTaskPickCrops : public CVillageTaskBase {
    CIntelligentPointer<CThingObject> Crop;
    CIntelligentPointer<const CThing> CreatureAssignedToTask;
public:
    CVillageTaskPickCrops(const CVillageTaskPickCrops &);
    CVillageTaskPickCrops(CThingObject &);
    virtual bool IsCreatureAssignedToTask(const CThing &) const;
    virtual void AssignCreatureToTask(const CThing &);
    virtual EVillageTaskType GetTaskType() const;
    virtual bool IsValid() const;
    virtual bool IsTaskAvailable() const;
    virtual bool IsThingInvolvedWithTask(const CThing &) const;
    virtual void RemoveThingFromTask(const CThing &,bool &);
    bool IsBetterThan(const CVillageTaskPickCrops *,const CThing &) const;
    CThingObject * GetCrop();
    virtual ~CVillageTaskPickCrops();
    CVillageTaskPickCrops & operator=(const CVillageTaskPickCrops &);
};
FABLE_STATIC_ASSERT(sizeof(CVillageTaskPickCrops)==0x18);
#else
#pragma pack(push, 1)
class CVillageTaskPickCrops {
public:
#ifdef FABLE_RETAIL_CVILLAGETASKPICKCROPS
    
    
    CVillageTaskPickCrops(CThingObject& crop);
#endif
    void*         __vftable;                                 
    unsigned char _pad_0x04[0x4];                            
    unsigned char Crop[0x8];                                 
    unsigned char CreatureAssignedToTask[0x8];               
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CVillageTaskPickCrops) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CVillageTaskPickCrops, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CVillageTaskPickCrops, Crop) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CVillageTaskPickCrops, CreatureAssignedToTask) == 0x10);

#endif 
#endif 

#endif
#ifndef FABLE_FIXTURE_EMBED_F3A331D73E07138B
#define FABLE_FIXTURE_EMBED_F3A331D73E07138B
#ifndef FABLE_STLPORT_ALGORITHM_H
#define FABLE_STLPORT_ALGORITHM_H





#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif
namespace std {
template <class R, class T> class mem_fun_ref_t {
public:
    explicit mem_fun_ref_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T& object) const { return (object.*m_pfn)(); }
private:
    R (T::*m_pfn)();
};
template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pfn)())
{
    return mem_fun_ref_t<R, T>(pfn);
}
template <class It, class F> F FABLE_FASTCALL for_each(It first, It last, F f);   
#ifdef FABLE_RETAIL_STLP_FIND_IF_INPUT


























template<class Input,class Predicate>
inline Input FABLE_FASTCALL __find_if(Input first,Input last,Predicate pred,const input_iterator_tag&) {
    while(first!=last && !pred(*first)) ++first;
    return first;
}
template<class Input,class Predicate>
Input FABLE_FASTCALL find_if(Input first,Input last,Predicate pred) {
    return __find_if(first,last,pred,typename iterator_traits<Input>::iterator_category());
}
#endif
}
#endif

#endif
class CThing { public: int id; };
class CThingTrackNode : public CThing {};
class CThingObject : public CThing {};
#ifndef FABLE_FIXTURE_EMBED_171944BE6855B8BB
#define FABLE_FIXTURE_EMBED_171944BE6855B8BB
#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H
#ifndef FABLE_FIXTURE_EMBED_3A0EA43A47735ACB
#define FABLE_FIXTURE_EMBED_3A0EA43A47735ACB







#ifndef FABLE_COMPILER_H
#define FABLE_COMPILER_H

#if defined(_MSC_VER)
#define FABLE_FORCEINLINE __forceinline
#else
#define FABLE_FORCEINLINE inline
#endif

#endif 

#endif




#if defined(_MSC_VER)
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#include <math.h>
#ifndef FABLE_FIXTURE_EMBED_334ED73EFC59D3D2
#define FABLE_FIXTURE_EMBED_334ED73EFC59D3D2
#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H
#ifndef FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#define FABLE_FIXTURE_EMBED_E82AA292DC456FD4
#include <stddef.h>
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __fastcall
#define FABLE_TEST_JOIN_(a,b) a##b
#define FABLE_TEST_JOIN(a,b) FABLE_TEST_JOIN_(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_TEST_JOIN(test_assert_,__LINE__)[(x)?1:-1]
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define FABLE_STATIC_ASSERT(x)
#define __int64 long long
#define __fastcall
#define __cdecl
#endif

#endif

#ifdef FABLE_RETAIL_C2DCOORDI_TOFLOAT




class C2DVector;
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}
    C2DVector ToC2DCoordF() const;
};
#ifndef FABLE_FIXTURE_EMBED_AFB680C8A676E5A2
#define FABLE_FIXTURE_EMBED_AFB680C8A676E5A2
#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H
#ifndef FABLE_FIXTURE_EMBED_3A0EA43A47735ACB
#define FABLE_FIXTURE_EMBED_3A0EA43A47735ACB







#ifndef FABLE_COMPILER_H
#define FABLE_COMPILER_H

#if defined(_MSC_VER)
#define FABLE_FORCEINLINE __forceinline
#else
#define FABLE_FORCEINLINE inline
#endif

#endif 

#endif




#ifdef FABLE_RETAIL_C2DVECTOR
#include <math.h>





extern const float DELTA_NOTIONAL_ZERO;                 
#pragma pack(push, 1)
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
#ifdef FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
    
    
    
    FABLE_FORCEINLINE
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
#ifdef FABLE_RETAIL_C2DVECTOR_INITIALISE
    
    
    void Initialise(float x,float y) { X=x; Y=y; }
#endif
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D
    
    
    C2DVector(const class C3DVector& v);
#endif
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
    
    const C2DVector operator-() const { return C2DVector(-X,-Y); }
    const C2DVector operator*(float scalar) const { return C2DVector(X*scalar,Y*scalar); }
    const C2DVector& operator+=(const C2DVector& vector) { X+=vector.X; Y+=vector.Y; return *this; }
    
    
    const C2DVector operator/(float scalar) const { float inverse = 1.0f/scalar; return C2DVector(X*inverse,Y*inverse); }
    
    const C2DVector& operator*=(const C2DVector& vector) { X*=vector.X; Y*=vector.Y; return *this; }
    float Dot(const C2DVector& vector) const { return X*vector.X+Y*vector.Y; }
    
    
#ifdef FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
    
    
    
    float GetSquaredMagnitude() const { float squared=Y*Y; squared+=X*X; return squared; }
#else
    float GetSquaredMagnitude() const { return X*X+Y*Y; }
#endif
    
    
    
    float GetMagnitude() const { return sqrtf(X*X+Y*Y); }
    
    
    
    
    
    
    FABLE_FORCEINLINE bool operator==(const C2DVector& vector) const
    {
        return fabs(X-vector.X)<=DELTA_NOTIONAL_ZERO && fabs(Y-vector.Y)<=DELTA_NOTIONAL_ZERO;
    }
#ifdef FABLE_RETAIL_C2DVECTOR_NORMALISE
    
    
    
    
    C2DVector& Normalise();                                                  
    
    const C2DVector& operator*=(float scalar) { X*=scalar; Y*=scalar; return *this; }
    bool IsNonZero() const { return GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO; }
#endif
};
#pragma pack(pop)
#ifdef FABLE_RETAIL_C2DVECTOR_FROM3D
inline C2DVector::C2DVector(const C3DVector& v) : X(v.X),Y(v.Y) { }
#endif
#else
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
#ifdef FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
    
    
    
    FABLE_FORCEINLINE
#endif
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#endif
#endif

#endif
inline C2DVector C2DCoordI::ToC2DCoordF() const { return C2DVector((float)X,(float)Y); }
#else
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
#endif
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
#endif

#endif
#ifdef FABLE_RETAIL_C3DVECTOR
extern const float DELTA_NOTIONAL_ZERO;                                 



extern unsigned long GFastOneOverSqrtTable[128];                        
#ifdef FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN





FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-(r*r*value))*r*0.5f;}
#else
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-r*r*value)*r*0.5f;}
#endif
#endif
class C3DVector {
public:
    float X,Y,Z;
#ifdef FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
    
    bool operator!=(const C3DVector& v) const {
        return fabs(X-v.X)>DELTA_NOTIONAL_ZERO || fabs(Y-v.Y)>DELTA_NOTIONAL_ZERO || fabs(Z-v.Z)>DELTA_NOTIONAL_ZERO;
    }
#endif
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
#ifdef FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
    
    
    
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector((X*scale),Y*scale,Z*scale);}
#else
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
#endif
#ifdef FABLE_RETAIL_C3DVECTOR
    
    FABLE_VECTOR_INLINE const C3DVector operator-()const{return C3DVector(-X,-Y,-Z);}
    
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#endif
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
#ifdef FABLE_RETAIL_C3DVECTOR
    
    
    
    
#ifdef FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
    
    
    
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Z*Z+Y*Y);}
#endif
    
    
    
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{float squared=X*X;squared+=Y*Y;return squared;}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
#endif
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#ifdef FABLE_RETAIL_C3DVECTOR
    
    
    
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    
    
#ifdef FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
    
    
    
    FABLE_VECTOR_INLINE float GetAngleXY() const {
        float angle=(float)atan2(X,Y)*0.15915494f;
        if(angle<0.0f || angle>=1.0f) {
            angle=(float)fmod((double)angle,1.0);
            if(angle<0.0f) angle+=1.0f;
        }
        return angle;
    }
    FABLE_VECTOR_INLINE C3DVector CrossWithUp() const {return C3DVector(Y,-X,0.0f);}
    FABLE_VECTOR_INLINE const C3DVector& InitialiseFromAngles(float angleXY,float angleYZ,float length) {
        float cosYZ=(float)cos(angleYZ*(double)6.2831855f);
        X=(float)(sin(angleXY*(double)6.2831855f)*(cosYZ*length));
        Y=(float)(cos(angleXY*(double)6.2831855f)*(cosYZ*length));
        Z=(float)(sin(angleYZ*(double)6.2831855f)*length);
        return *this;
    }
#else
    float GetAngleXY() const;
#endif
    
    
    void RotateXY(float angle);                                          
    
    
    static C3DVector __fastcall GetRandomUnitVector(unsigned long* seed);  
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
    
    
    
    
    
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(X*X+Z*Z+Y*Y);X*=factor;Y*=factor;Z*=factor;return *this;}
#else
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float factor=GFFastOneOverSqrt(GetSquaredMagnitude());X*=factor;Y*=factor;Z*=factor;return *this;}
#endif
    
    
    
#ifdef FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
    
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=X*X;squared+=Z*Z+Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#else
    FABLE_VECTOR_INLINE bool IsNonZero()const{float squared=X*X;squared+=Z*Z;squared+=Y*Y;return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
#endif
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

#endif
class CTCVillage { public:
    template<class T> void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>,const CThing&,std::list<CCountedPointer<T> >&) const;
};
template<class T>
void CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing, std::list<CCountedPointer<T> >& task_list) const
{
    if (task->IsThingInvolvedWithTask(thing)) {
        bool remove_task;
        task->RemoveThingFromTask(thing,remove_task);
        if (remove_task) {
            typename std::list<CCountedPointer<T> >::iterator task_it = std::find_if(task_list.begin(),task_list.end(),CIsTask(*task.Get()));
            task_list.erase(task_it);
        }
    }
}

static int pointer_destroys, task_destroys;
struct TaskState { CVillageTaskPickCrops* task; int owner; bool remove; int calls; };
static TaskState states[4];
static TaskState& state(const CVillageTaskPickCrops* t) { for(int i=0;i<4;++i) if(states[i].task==t)return states[i]; throw 1; }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseObject::CBaseObject() : IntelligentPointer(0) { }
CBaseObject::~CBaseObject() { }
void CBaseObject::Validate() const { }
CVillageTaskBase::CVillageTaskBase() { }
CVillageTaskBase::~CVillageTaskBase() { }
class CBaseObjectPointer { public: CBaseObject* item; };
static CBaseObjectPointer pointer_slots[512];
static int next_pointer;
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(&pointer_slots[next_pointer++]) { PData->item=0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++pointer_destroys; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PData->item; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* p) { PData->item=p; }
CVillageTaskPickCrops::CVillageTaskPickCrops(CThingObject&) { }
CVillageTaskPickCrops::~CVillageTaskPickCrops() { ++task_destroys; }
void CVillageTaskPickCrops::AssignCreatureToTask(const CThing&) { }
bool CVillageTaskPickCrops::IsCreatureAssignedToTask(const CThing&) const { return false; }
EVillageTaskType CVillageTaskPickCrops::GetTaskType() const { return VILLAGE_TASK_PICK_CROPS; }
bool CVillageTaskPickCrops::IsValid() const { return true; }
bool CVillageTaskPickCrops::IsTaskAvailable() const { return true; }
bool CVillageTaskPickCrops::IsThingInvolvedWithTask(const CThing& t) const { return state(this).owner==t.id; }
void CVillageTaskPickCrops::RemoveThingFromTask(const CThing&,bool& remove_task) { TaskState& s=state(this);++s.calls;remove_task=s.remove; }
#define CHECK(x) do { if(!(x)){printf("FAIL line %d\n",__LINE__);return 1;} }while(0)
int main() {
    int cases=0;
    for(int count=0;count<=4;++count)for(int selected=0;selected<(count?count:1);++selected)for(int involved=0;involved<2;++involved)for(int remove=0;remove<2;++remove) {
        if(!count && involved && remove) continue;
        CThing thing;thing.id=7;
        CThingTrackNode node; CThingObject crop; C3DVector position;
        int initial_dead=task_destroys;
        {
            CCountedPointer<CVillageTaskPickCrops> owners[4];
            std::list<CCountedPointer<CVillageTaskPickCrops> > tasks;
            for(int i=0;i<4;++i){owners[i].Reset(new CVillageTaskPickCrops(crop));states[i].task=owners[i].Get();states[i].owner=(involved?7:8);states[i].remove=remove!=0;states[i].calls=0;if(i<count)tasks.push_back(owners[i]);CHECK(owners[i].GetReferenceCount()==(i<count?2u:1u));}
            CIntelligentPointer<CVillageTaskBase> task(owners[selected].Get());
            int before=pointer_destroys;
            CTCVillage village;village.RemoveThingFromTask<CVillageTaskPickCrops>(task,thing,tasks);
            CHECK(pointer_destroys==before+1);
            CHECK(states[selected].calls==involved);
            CHECK(tasks.size()==(unsigned)(count-((involved&&remove)?1:0)));
            int n=0;
            for(std::list<CCountedPointer<CVillageTaskPickCrops> >::iterator it=tasks.begin();it!=tasks.end();++it){if(involved&&remove&&n==selected)++n;CHECK(it->Get()==owners[n].Get());++n;}
            for(int i=0;i<4;++i)CHECK(owners[i].GetReferenceCount()==((i<count && !(involved&&remove&&i==selected))?2u:1u));
        }
        CHECK(task_destroys==initial_dead+4);++cases;
    }
    printf("00660c53_TEST PASS\n");
    return 0;
}

#endif