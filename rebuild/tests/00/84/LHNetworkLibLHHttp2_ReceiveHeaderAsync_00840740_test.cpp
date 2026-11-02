#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <new>
#ifdef _MSC_VER
void* __cdecl operator new[](unsigned int);
#endif
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
static char* _itoa(int value,char* out,int radix) { if(radix!=10) abort();sprintf(out,"%d",value);return out; }
#endif
struct HttpFixture;

static time_t testNow=100;
static int timeCalls=0;
static time_t fixture_time(time_t* out) {++timeCalls;if(out)*out=testNow;return testNow;}
#define time fixture_time
// LHNetworkLib::LHHttp2::ReceiveHeaderAsync(void) -- receive and parse the pending HTTP response headers.
// Leaf: ?ReceiveHeaderAsync@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@XZ
// Flags: /GS
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_RECEIVE_HEADER
#include <new>
void __cdecl operator delete[](void*);
#if defined(FABLE_RETAIL_LHHTTP2)
// Retail opt-in (Codex pass 11): original LHNetworkLib TPI names and full native 0x10a8 field layout; 008400b0/00840740 operand offsets.
#ifndef FABLE_RETAIL_LHHTTP2_HEADER
#define FABLE_RETAIL_LHHTTP2_HEADER
#ifndef FABLE_STLPORT_STRING_H
#define FABLE_STLPORT_STRING_H
// STLport 4.6.2 _string.h/_string.c excerpts, adapted to this header subset.
// Copyright (c) 1994 Hewlett-Packard Company; 1996,1997 Silicon Graphics
// Computer Systems, Inc.; 1997 Moscow Center for SPARC Technology;
// 1999 Boris Fomitchev. Permission to use/copy/modify is granted without
// fee provided these notices are retained. Provided as is, without warranty.
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H
// STLport 4.6.2 excerpts: stl/_alloc.h, _vector.h, _list.h, _bitset.h,
// _pair.h and _function_base.h. Original inheritance, field names and keys.
// Only declarations and operations needed by these translation units appear.
#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#include <new>
#endif
#include <stdlib.h>
#ifdef FABLE_RETAIL_SHOP
#include <stdio.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
struct __true_type { };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
#endif
#ifdef FABLE_STLPORT_HTTP
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
struct forward_iterator_tag : public input_iterator_tag { };
#else
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
#endif
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
template<class I,class O,class D> inline O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*) {
    for(D n=last-first;n>0;--n) { *result=*first;++first;++result; } return result;
}
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_REGION_GRAPH)
template<class I,class O,class D>O __fastcall __copy(I,I,O,const random_access_iterator_tag&,D*);
template<class I,class C>void __fastcall sort(I,I,C);
#endif
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
template<class T> inline T* __copy_ptrs(T* first,T* last,T* result,const __false_type&) {return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class T> inline T* __copy_ptrs(T* first,T* last,T* result,const __true_type&) {return last==first?result:(T*)((char*)memmove(result,first,(char*)last-(char*)first)+((char*)last-(char*)first));}
#endif
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_REGION_GRAPH)
template<class I> void __fastcall __destroy_aux(I,I,const __false_type&);
#else
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
#endif
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#else
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
#endif
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_SHOP)
    typedef __false_type is_POD_type;
#endif
};
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
template<> struct __type_traits<unsigned char> { typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;typedef __true_type is_POD_type; };
#endif
#ifdef FABLE_RETAIL_QUEST_COMPLETION
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<> struct __type_traits<unsigned short> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
template<> struct __type_traits<long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
template<> struct __type_traits<unsigned __int64> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
#ifdef FABLE_RETAIL_SHOP
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; typedef __true_type is_POD_type; };
#endif
#endif
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
#ifdef FABLE_STLPORT_CREATURE_POSITIONS
#ifndef FABLE_STLPORT_HTTP
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#endif
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input,Input,Output,const __false_type&);
template<class Input,class Size,class T>
Input __fastcall __uninitialized_fill_n(Input,Size,const T&,const __false_type&);
template<class T,class U> inline void _Construct(T* p,const U& value) { new(p) T(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
#endif
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    template<class U> allocator(const allocator<U>&){}
#endif
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) {
#ifdef FABLE_RETAIL_ENTITY_PERSIST
        if(p) free(p);
#else
        free(p);
#endif
    }
#endif
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHOP)
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
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
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    vector& operator=(const vector&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_SHOP)
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
#elif defined(FABLE_STLPORT_UI)
    vector(const vector&);
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_SHOP)
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#endif
#ifdef FABLE_STLPORT_HTTP
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x);++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fill_len,bool at_end=false) {
        const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
        _Tp* new_start=this->_M_end_of_storage.allocate(len);
        _Tp* new_finish=new_start;
        new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
        if(fill_len==1) { _Construct(new_finish,x);++new_finish; }
        else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());
        if(!at_end) new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
        _M_clear();_M_set(new_start,new_finish,new_start+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(_Tp* start,_Tp* finish,_Tp* end) {
        this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;
    }
public:
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
    void reserve(size_type);
    vector& operator=(const vector&);
    _Tp& back(){return *(this->_M_finish-1);}
#endif
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
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
#endif
    iterator erase(iterator first,iterator last) {
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
        iterator result=__copy_ptrs(last,this->_M_finish,first,_IsPODType());
#else
        iterator result=std::__copy(last,this->_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);
#endif
#else
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
#endif
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_LHHTTP2_RECEIVE_HEADER)
    void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#elif defined(FABLE_RETAIL_SCOREBOARD)
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#elif defined(FABLE_RETAIL_OAKVALE) || defined(FABLE_RETAIL_HANGING_TREE)
    void resize(size_type n,const _Tp& x);
#else
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
#endif
    void resize(size_type n) { resize(n,_Tp()); }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
protected:
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
#elif defined(FABLE_STLPORT_CREATURE_POSITIONS)
    // Original STLport4.6 _vector.h erase/clear and non-trivial push_back.
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            new((void*)this->_M_finish) _Tp(x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator i=std::__copy_ptrs(last,this->_M_finish,first,__false_type());
        for(iterator d=i;d!=this->_M_finish;++d) d->~_Tp();
        this->_M_finish=i;
        return first;
    }
    void clear() { erase(begin(),end()); }
protected:
#ifdef FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
    // STLport _vector.c non-trivial growth: copy prefix, fill, copy suffix, release.
    void _M_insert_overflow(iterator position,const _Tp& value,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type length=oldSize+(oldSize<fillLength?fillLength:oldSize);
        _Tp* newStart=(_Tp*)malloc(length*sizeof(_Tp));
        _Tp* newFinish=newStart;
        for(iterator first=this->_M_start;first!=position;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(size_type count=fillLength;count>0;--count,++newFinish) new((void*)newFinish) _Tp(value);
        if(!atEnd) for(iterator first=position;first!=this->_M_finish;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(iterator first=this->_M_start;first!=this->_M_finish;++first) first->~_Tp();
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+length;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __false_type&,size_type,bool);
#endif
#elif defined(FABLE_STLPORT_VISIBLE_PUSH_BACK)
    void push_back(const _Tp& x) {
        if (this->_M_finish!=this->_M_end_of_storage._M_data) {
            new ((void*)this->_M_finish) _Tp(x);
            ++this->_M_finish;
        } else {
            _M_insert_overflow(end(),x,__true_type(),size_type(1),true);
        }
    }
protected:
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
    // Original STLport _vector.h true-tag overflow path: trivial copy then fill.
    void _M_insert_overflow(iterator position,const _Tp& x,
                                                const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type len=oldSize+(oldSize<fillLength ? fillLength : oldSize);
        _Tp* newStart=(_Tp*)malloc(len*sizeof(_Tp));
        size_type before=position-this->_M_start;
        memmove(newStart,this->_M_start,before*sizeof(_Tp));
        _Tp* newFinish=newStart+before;
        for(size_type n=fillLength;n>0;--n,++newFinish) *newFinish=x;
        if(!atEnd) {
            size_type after=this->_M_finish-position;
            memmove(newFinish,position,after*sizeof(_Tp));
            newFinish+=after;
        }
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;
        this->_M_finish=newFinish;
        this->_M_end_of_storage._M_data=newStart+len;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
#endif
#else
#ifndef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp&);
#endif
#endif
};
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#if defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_QUICK_ACCESS)
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#if defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_COMBAT_TYPE)
    template<class U1,class U2> pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
#endif
#endif
};
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
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#endif
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    _Alloc get_allocator()const{return _Alloc(this->_M_node);}
#endif
#ifdef FABLE_STLPORT_CONSTRUCTORS
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    _List_base(const _Alloc& a):_M_node(allocator<_List_node<_Tp> >(a),(_List_node<_Tp>*)0) {
#else
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
#endif
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_PARTICLE_SERIALISE)
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
#else
    void clear();
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_RETAIL_REGION_GRAPH
    ~list();
#endif
#if defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_QUICK_ACCESS)
    ~list();
#endif
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _Tp value_type;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    _Tp& front() { return *begin(); }
    const _Tp& front() const { return *begin(); }
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_QUICK_ACCESS)
#ifndef FABLE_RETAIL_NAVIGATION_UPDATE
    _Alloc get_allocator()const { return _Alloc(); }
#endif
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) { insert(begin(),x.begin(),x.end()); }
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    list& operator=(const list&);
#endif
    template<class Input> void insert(iterator position,Input first,Input last) { _M_insert_dispatch(position,first,last,__false_type()); }
    template<class Input> void _M_insert_dispatch(iterator,Input,Input,const __false_type&);
#endif
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _Alloc get_allocator() const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) {
        insert(begin(),x.begin(),x.end());
    }
    ~list() { }
    template<class Input> void insert(iterator p,Input first,Input last) {
        _M_insert_dispatch(p,first,last,__false_type());
    }
    template<class Input> __declspec(noinline) void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) {
        for(;first!=last;++first) insert(position,*first);
    }
#endif
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS)
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
        _Construct(&p->_M_data,x);
#else
        new((void*)&p->_M_data) _Tp(x);
#endif
        return p;
    }
public:
    iterator insert(iterator position,const _Tp& x) {
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
#ifdef FABLE_RETAIL_SHOP
// Original STLport _range_errors.h no-exceptions branch.
inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
#else
void __stl_throw_out_of_range(const char*);
#endif
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif

#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(strlen,memcmp,memcpy,memset)
#endif
namespace std {
#ifdef FABLE_RETAIL_HERO_LOGBOOK
// Authentic STLport4.6.2 char_traits.h125..150,207..209; native403c60.
template<class C,class I> class __char_traits_base {
public:static int __cdecl compare(const C*,const C*,size_t);
};
template<class C> class char_traits:public __char_traits_base<C,C> {};
#else
template<class C> class char_traits;
#endif
template<> class char_traits<char> {
public:
    typedef char char_type;
    static void assign(char& a,const char& b) { a=b; }
    static size_t length(const char* p) { return strlen(p); }
    static int compare(const char* a,const char* b,size_t n) { return memcmp(a,b,n); }
    static char* copy(char* a,const char* b,size_t n) { return (char*)memmove(a,b,n); }
};
inline char* uninitialized_copy(const char* first,const char* last,char* result) {
    return (char*)__copy_trivial(first,last,result);
}
template<class T,class A> class _String_base {
public:
    typedef A allocator_type;
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    size_t max_size() const { return size_t(-1)/sizeof(T)-1; }
    void _M_throw_length_error() const;
    void _M_throw_out_of_range() const;
    void _M_allocate_block(size_t n) {
        if(n<=max_size()+1 && n>0) {
            _M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;
        } else _M_throw_length_error();
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start); }
    _String_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0) { }
    _String_base(const A& a,size_t n):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0) { _M_allocate_block(n); }
    ~_String_base() { _M_deallocate_block(); }
};
struct _String_reserve_t { };
template<class C,class Traits=char_traits<C>,class A=allocator<C> >
class basic_string:protected _String_base<C,A> {
    typedef basic_string<C,Traits,A> _Self;
    typedef _String_base<C,A> _Base;
public:
    typedef C* iterator;
    typedef const C* const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef A allocator_type;
    typedef _String_reserve_t _Reserve_t;
    static C _M_null() { return C(); }
    enum { npos=-1 };
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    basic_string();
    basic_string(const C* s,const A& a=A());
    basic_string(const _Self& s);
    basic_string(_Reserve_t,size_t n,const A& a=A()):_Base(a,n+1) { _M_terminate_string(); }
    #ifdef FABLE_RETAIL_SCOREBOARD
    template<class I> basic_string(I f,I l,const A& a=A()):_Base(a) { _M_initialize_dispatch(f,l,__false_type()); }
#else
    template<class I> basic_string(I f,I l,const A& a=A()):_Base(a) { _M_range_initialize(f,l); }
#endif
    ~basic_string() { _Destroy(this->_M_start,this->_M_finish+1); }
    size_type size() const { return this->_M_finish-this->_M_start; }
    size_type capacity() const { return this->_M_end_of_storage._M_data-this->_M_start-1; }
    size_type max_size() const { return _Base::max_size(); }
    const C* c_str() const { return this->_M_start; }
    const C* data() const { return this->_M_start; }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    C& operator[](size_type n) { return *(this->_M_start+n); }
    const C& operator[](size_type n) const { return *(this->_M_start+n); }
    _Self& operator=(const _Self& s) { if(&s!=this) assign(s._M_start,s._M_finish); return *this; }
    _Self& operator=(const C* s) { return assign(s,s+Traits::length(s)); }
    _Self& assign(const C* first,const C* last);
    template<class I> _Self& assign(I first,I last) { return _M_assign_dispatch(first,last,__false_type()); }
    _Self& append(const _Self& s) { return append(s._M_start,s._M_finish); }
    _Self& append(const C* s) { return append(s,s+Traits::length(s)); }
    template<class I> _Self& append(I f,I l) { return _M_append_dispatch(f,l,__false_type()); }
    _Self& operator+=(const _Self& s) { return append(s); }
    _Self& operator+=(const C* s) { return append(s); }
    _Self& operator+=(C c) { push_back(c);return *this; }
    _Self substr(size_type pos=0,size_type n=npos) const {
        if(pos>size()) this->_M_throw_out_of_range();
        return _Self(this->_M_start+pos,this->_M_start+pos+(min)(n,size()-pos));
    }
    void reserve(size_type n=0);
    void push_back(C c) {
        if(this->_M_finish+1==this->_M_end_of_storage._M_data) reserve(size()+(max)(size(),size_type(1)));
        _M_construct_null(this->_M_finish+1);Traits::assign(*this->_M_finish,c);++this->_M_finish;
    }
private:
    void _M_construct_null(C* p) { *p=C(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
    template<class I> void _M_range_initialize(I f,I l,const forward_iterator_tag&) {
        difference_type n=l-f;this->_M_allocate_block(n+1);
        this->_M_finish=uninitialized_copy(f,l,this->_M_start);_M_terminate_string();
    }
#ifdef FABLE_RETAIL_SCOREBOARD
    template<class I> void _M_initialize_dispatch(I f,I l,const __false_type&) { _M_range_initialize(f,l); }
    template<class I> void _M_range_initialize(I f,I l) { _M_range_initialize(f,l,random_access_iterator_tag()); }
#else
    template<class I> void _M_range_initialize(I f,I l) { _M_range_initialize(f,l,forward_iterator_tag()); }
#endif
    template<class I> _Self& _M_assign_dispatch(I f,I l,const __false_type&);
    template<class I> _Self& append(I f,I l,const forward_iterator_tag&);
    template<class I> _Self& _M_append_dispatch(I f,I l,const __false_type&) { return append(f,l,forward_iterator_tag()); }
};
template<class C,class T,class A> basic_string<C,T,A>::basic_string():_Base(allocator_type()) {
    this->_M_start=this->_M_end_of_storage.allocate(8);this->_M_finish=this->_M_start;
    this->_M_end_of_storage._M_data=this->_M_start+8;_M_terminate_string();
}
template<class C,class T,class A> basic_string<C,T,A>::basic_string(const C* s,const A& a):_Base(a) {
    _M_range_initialize(s,s+T::length(s));
}
template<class C,class T,class A> basic_string<C,T,A>::basic_string(const basic_string<C,T,A>& s):_Base(s.get_allocator()) {
    _M_range_initialize(s._M_start,s._M_finish);
}
template<class C,class T,class A> inline bool __fastcall operator==(const basic_string<C,T,A>& x,const basic_string<C,T,A>& y) {
    return x.size()==y.size()&&T::compare(x.data(),y.data(),x.size())==0;
}
template<class C,class T,class A> inline bool operator==(const basic_string<C,T,A>& s,const C* c) {
    size_t n=T::length(c);return s.size()==n&&T::compare(s.data(),c,n)==0;
}
template<class C,class T,class A> inline bool operator!=(const basic_string<C,T,A>& s,const C* c) { return !(s==c); }
template<class C,class T,class A> basic_string<C,T,A> operator+(const C*,const basic_string<C,T,A>&);
template<class C,class T,class A> basic_string<C,T,A> operator+(const basic_string<C,T,A>&,const C*);
typedef basic_string<char> string;
}
#endif

#ifndef FABLE_LHNETWORK_TYPES_H
#define FABLE_LHNETWORK_TYPES_H
// Original Ego_r CodeView enum names and values; native HTTP state constants.
namespace LHNetworkLib {
enum LH_HTTP_SERVER_CODES {
    HTTP_NOT_DEFINED = -1,
    HTTP_DOCUMENT_FOUND = 200,
    HTTP_DOCUMENT_CREATED = 201,
    HTTP_DOCUMENT_ACCPETED = 202,
    HTTP_DOCUMENT_NO_CONTENT = 204,
    HTTP_DOCUMENT_RESET = 205,
    HTTP_DOCUMENT_PARTIAL_CONTENT = 206,
    HTTP_DOCUMENT_MULTIPLE_CHOICES = 300,
    HTTP_DOCUMENT_MOVED_PERMANENTLY = 301,
    HTTP_DOCUMENT_MOVED_TEMPORARILY = 302,
    HTTP_DOCUMENT_SEE_OTHER = 303,
    HTTP_DOCUMENT_NOT_MODIFIED = 304,
    HTTP_DOCUMENT_USE_PROXY = 305,
    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_NOT_ACCEPTABLE = 406,
    HTTP_REQUEST_TIMEOUT = 408,
    HTTP_CONFLICT = 409,
    HTTP_GONE = 410,
    HTTP_LENGTH_REQUIRED = 411,
    HTTP_REQUEST_ENTITY_TOO_LARGE = 413,
    HTTP_REQUEST_URI_TOO_LONG = 414,
    HTTP_UNSUPPORTED_MEDIA_TYPE = 415,
    HTTP_SERVER_ERROR = 500,
    HTTP_NOT_IMPLEMENTED = 501
};
enum HTTP_REQUEST_TYPE {
    GET = 0,
    PUT = 1,
    POST = 2
};
enum LH_RETURN {
    LH_OK = 0,
    LH_FALSE = 1,
    LH_ERROR = 2,
    LH_FAIL = 3
};
enum LH_HTTP_STATUS {
    HTTP_STATUS_NONE = 0,
    HTTP_STATUS_SENDING_REQUEST = 1,
    HTTP_STATUS_REQUEST_SENT = 2,
    HTTP_STATUS_WAITING_FOR_HEADER = 3,
    HTTP_STATUS_RECEIVING_HEADER = 4,
    HTTP_STATUS_HEADER_RECEIVED = 5,
    HTTP_STATUS_RECEIVING_DOCUMENT = 6,
    HTTP_STATUS_DOCUMENT_RECEIVED = 7,
    HTTP_STATUS_DOCUMENT_PREPARED = 8,
    HTTP_STATUS_SENDING_ERROR = -100,
    HTTP_STATUS_RECEIVING_ERROR = -101,
    HTTP_STATUS_TIMEOUT_ERROR = -102,
    HTTP_STATUS_PREPARE_ERROR = -103,
    HTTP_STATUS_NO_REQUEST_PREPARED = -104,
    HTTP_STATUS_INCORRECT_ACTION = -105,
    HTTP_STATUS_OK = 1000,
    HTTP_STATUS_FAIL = 1001,
    HTTP_STATUS_ERROR = 1002
};
enum HTTP_TRANSFER_ENCODING {
    NORMAL = 0,
    CHUNKED = 1,
    ENCODING_NONE = 2
};
enum LSOCKET_PROTOCOL {
    LSP_NONE = 0,
    LSP_TCP = 1,
    LSP_UDP = 2,
    LSP_TCP_PACKET = 3
};
enum LSOCKET_FLAGS {
    LSF_NONE = 0,
    LSF_LISTENING = 1
};
enum LSOCKET_SELECT {
    LSS_NONE = 0,
    LSS_READ = 1,
    LSS_WRITE = 2,
    LSS_EXCEPTION = 4
};
}
#endif

namespace LHNetworkLib {
class LSocket;
struct LHHttpDocumentParts {
    char* Buffer;
    unsigned long BufferLength;
    unsigned long CurrBufPos;
    ~LHHttpDocumentParts() { if(Buffer) { delete[] Buffer;Buffer=0; } BufferLength=0; }
};
class LHHttpHeaders {
public:
    std::vector<std::string> HdrDocumentHeader;
    std::string HeaderString;
#ifdef FABLE_RETAIL_SCOREBOARD
    ~LHHttpHeaders() { Reset(); }
    void Reset();
#else
    void Reset() { HeaderString="";HdrDocumentHeader.clear(); }
#endif
    void DeleteHeaderString() { HeaderString=""; }
    void AddHeader(const std::string& line) { HdrDocumentHeader.push_back(line); }
    void AddHeader(const std::string& name,const std::string& value);
    std::string GetHeaderString();
};
class LHHttp2 {
    friend struct ::HttpFixture;
    LSocket* Host;
    std::string Hostname;
    unsigned short Port;
    std::string PreparedRequest;
    unsigned long RequestBytesSent;
    unsigned long RequestLength;
    LHHttpHeaders MyHeaders;
    LHHttpHeaders ReceivedHeaders;
    unsigned long TimeOutValue;
    unsigned long ActionStartTime;
    bool ActionStarted;
    unsigned short CurrentAction;
    unsigned long CurrDownSize;
    unsigned long Forwardings;
    unsigned long CurrForwarding;
    char CurrentHeaderPart[2048];
    unsigned long CurrentHeaderPos;
    std::string ProxyHost;
    unsigned long ProxyPort;
    std::vector<LHHttpDocumentParts*> DocumentParts;
    HTTP_TRANSFER_ENCODING DocumentEncoding;
    unsigned long DocumentEstimatedLength;
    unsigned long DocumentLength;
    char CurrentDocumentPart[2048];
    unsigned long CurrentDocumentPos;
    bool InChunkMode;
    LHHttpDocumentParts* CurrentDocumentChunk;
public:
#ifdef FABLE_RETAIL_SCOREBOARD
    LHHttp2();
    ~LHHttp2();
    void SetProxySettings(const std::string&,unsigned short);
    LH_RETURN Open(const std::string&,unsigned short);
#endif
    LH_HTTP_STATUS PrepareRequest(HTTP_REQUEST_TYPE,const std::string&,LHHttpHeaders*,const std::string&,unsigned long);
    LH_HTTP_STATUS ReceiveHeaderAsync();
    void ResetCurrentHeaderPart() { CurrentHeaderPos=0;memset(CurrentHeaderPart,0,sizeof CurrentHeaderPart); }
    void ResetCurrentDocumentPart() {
        DocumentEstimatedLength=0;DocumentLength=0;memset(CurrentDocumentPart,0,sizeof CurrentDocumentPart);
        CurrentDocumentPos=0;InChunkMode=false;
        if(CurrentDocumentChunk) { delete CurrentDocumentChunk;CurrentDocumentChunk=0; }
    }
};
bool __fastcall StartsWith(const std::string&,const std::string&);
}
#ifdef _MSC_VER
typedef char LHHttp2SizeCheck[sizeof(LHNetworkLib::LHHttp2)==0x10a8?1:-1];
#endif
#endif

#endif

#ifndef FABLE_RETAIL_LSOCKET_HEADER
#define FABLE_RETAIL_LSOCKET_HEADER


#include <stdio.h>
// SDK Winsock structures: original CodeView fields and native 16-byte copies.
struct in_addr {
    union {
        struct { unsigned char s_b1,s_b2,s_b3,s_b4; } S_un_b;
        struct { unsigned short s_w1,s_w2; } S_un_w;
        unsigned long S_addr;
    } S_un;
};
struct sockaddr_in { short sin_family;unsigned short sin_port;in_addr sin_addr;char sin_zero[8]; };
namespace LHNetworkLib {
struct LAddress {
    std::string ServerName;
    unsigned long ServerIP;
    unsigned short ServerPort;
    LSOCKET_PROTOCOL TransportProtocol;
    LSOCKET_FLAGS TransportFlags;
};
class LSocketServer;
class LSocket {
    friend struct ::HttpFixture;
    LAddress RemoteAddress;
    LAddress LocalAddress;
    unsigned int Socket;
    sockaddr_in SocketInformation;
    sockaddr_in SocketInterfaceInformation;
    bool ListeningSocket;
    LSOCKET_PROTOCOL ConnectionMode;
    std::vector<char> SendOutputQueue;
    std::vector<char> InternalPacketBuffer;
    bool NoListeningBinding;
    LSocketServer* ServerObject;
public:
    bool ActivityPending;
    unsigned long NrTotalDataSent;
    unsigned long NrTotalDataReceived;
    unsigned long NrLastDataSent;
    unsigned long NrLastDataReceived;
    FILE* _DEBUGFP;
    bool IsReadable(long);
    bool Receive(std::vector<char>&);
};
}
#ifdef _MSC_VER
typedef char LSocketSizeCheck[sizeof(LHNetworkLib::LSocket)==0x9c?1:-1];
#endif
#endif

#include <time.h>
#ifndef FABLE_STLPORT_STRING_BODIES_H
#define FABLE_STLPORT_STRING_BODIES_H
// Real STLport 4.6.2 string member bodies (_string.h and _string.c),
// with allocator and iterator configuration resolved for the retail build.
// Original notices are retained in fable_string.h.

#include <stdio.h>
namespace std {
#pragma auto_inline(off)
template<class T,class A> void _String_base<T,A>::_M_throw_length_error() const { puts("basic_string");abort(); }
template<class T,class A> void _String_base<T,A>::_M_throw_out_of_range() const { puts("basic_string");abort(); }
template<class C,class T,class A>
basic_string<C,T,A>& basic_string<C,T,A>::assign(const C* first,const C* last) {
    ptrdiff_t n=last-first;
    if(size_type(n)<=size()) {
        T::copy(this->_M_start,first,n);
        if(this->_M_start+n!=this->_M_finish) {
            T::assign(this->_M_start[n],_M_null());this->_M_finish=this->_M_start+n;
        }
    } else {
        T::copy(this->_M_start,first,size());append(first+size(),last);
    }
    return *this;
}
template<class C,class T,class A> template<class I>
basic_string<C,T,A>& basic_string<C,T,A>::_M_assign_dispatch(I first,I last,const __false_type&) {
    C* cur=this->_M_start;
    while(first!=last && cur!=this->_M_finish) { T::assign(*cur,*first);++first;++cur; }
    if(first==last) {
        if(cur!=this->_M_finish) {T::assign(*cur,_M_null());this->_M_finish=cur;}
    } else append(first,last);
    return *this;
}
template<class C,class T,class A> template<class I>
basic_string<C,T,A>& basic_string<C,T,A>::append(I first,I last,const forward_iterator_tag&) {
    if(first!=last) {
        const size_type oldSize=size();difference_type n=last-first;
        if(size_type(n)>max_size() || oldSize>max_size()-size_type(n)) this->_M_throw_length_error();
        if(oldSize+n>capacity()) {
            const size_type len=oldSize+(max)(oldSize,size_type(n))+1;
            C* newStart=this->_M_end_of_storage.allocate(len);
            C* newFinish=newStart;
            newFinish=uninitialized_copy(this->_M_start,this->_M_finish,newStart);
            newFinish=uninitialized_copy(first,last,newFinish);_M_construct_null(newFinish);
            _Destroy(this->_M_start,this->_M_finish+1);this->_M_deallocate_block();
            this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+len;
        } else {
            I next=first;++next;uninitialized_copy(next,last,this->_M_finish+1);
            _M_construct_null(this->_M_finish+n);T::assign(*end(),*first);this->_M_finish+=n;
        }
    }
    return *this;
}
template<class C,class T,class A> void basic_string<C,T,A>::reserve(size_type n) {
    if(n>=capacity()) {
        if(n>max_size()) this->_M_throw_length_error();
        size_type len=n+1;
        C* newStart=this->_M_end_of_storage.allocate(len);
        C* newFinish=uninitialized_copy(this->_M_start,this->_M_finish,newStart);
        _M_construct_null(newFinish);_Destroy(this->_M_start,this->_M_finish+1);this->_M_deallocate_block();
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+len;
    }
}
template<class C,class T,class A> basic_string<C,T,A> operator+(const C* s,const basic_string<C,T,A>& y) {
    typedef basic_string<C,T,A> Str;typedef typename Str::_Reserve_t Reserve;
    size_t n=T::length(s);Str result(Reserve(),n+y.size());result.append(s,s+n);result.append(y);return result;
}
template<class C,class T,class A> basic_string<C,T,A> operator+(const basic_string<C,T,A>& x,const C* s) {
    typedef basic_string<C,T,A> Str;typedef typename Str::_Reserve_t Reserve;
    size_t n=T::length(s);Str result(Reserve(),x.size()+n,x.get_allocator());result.append(x);result.append(s,s+n);return result;
}
#pragma auto_inline(on)
}
#endif

#ifndef FABLE_HTTP_STRING_FUNCTIONS_BODIES_H
#define FABLE_HTTP_STRING_FUNCTIONS_BODIES_H

#include <ctype.h>
namespace LHNetworkLib {
#pragma auto_inline(off)
std::string __fastcall toLowerCase(const std::string& text) {
    std::string result;int length=text.size();
    for(int i=0;i<length;++i) result+=(char)tolower(text[i]);
    return result;
}
bool __fastcall StartsWith(const std::string& text,const std::string& prefix) {
    return toLowerCase(text).substr(0,prefix.size())==toLowerCase(prefix);
}
#pragma auto_inline(on)
}
#endif

#ifndef FABLE_HTTP_HEADER_BODIES_H
#define FABLE_HTTP_HEADER_BODIES_H

namespace LHNetworkLib {
#pragma auto_inline(off)
void LHHttpHeaders::AddHeader(const std::string& name,const std::string& value) {
    std::string header(name);
    if(name[name.size()-1]!=':') header+=":";
    header+=" ";header+=value;
    HdrDocumentHeader.push_back(header);
}
#pragma auto_inline(on)
}
#endif

// Layout: complete native LHHttp2 and LSocket fields, with original names.
namespace LHNetworkLib {
LH_HTTP_STATUS LHHttp2::ReceiveHeaderAsync()
{
    unsigned long receivesize=1;
    unsigned short bcounter;
    bool newline=false;
    LH_HTTP_STATUS returnstatus=HTTP_STATUS_RECEIVING_HEADER;
    if(CurrentAction!=1) return HTTP_STATUS_INCORRECT_ACTION;
    if(CurrentHeaderPos==0 && ReceivedHeaders.HdrDocumentHeader.empty() && !ActionStarted) {
        ActionStarted=true;
        time((time_t*)&ActionStartTime);
    }
    if(Host && Host->IsReadable(0)) {
        std::vector<char> tmpbuffer;
        bcounter=0;
        for(;bcounter<1024;++bcounter) {
            if(receivesize==0) break;
            tmpbuffer.resize(receivesize);
            if(!Host->Receive(tmpbuffer)) return HTTP_STATUS_TIMEOUT_ERROR;
            receivesize=Host->NrLastDataReceived;
            memcpy(CurrentHeaderPart+CurrentHeaderPos,&tmpbuffer[0],receivesize);
            if(receivesize!=0) {
                if(CurrentHeaderPart[CurrentHeaderPos]=='\r') newline=true;
                if(CurrentHeaderPart[CurrentHeaderPos]=='\n' && newline) {
                    CurrentHeaderPart[CurrentHeaderPos-1]=0;
                    if(strcmp(CurrentHeaderPart,"")==0) {
                        CurrentAction=2;
                        ActionStarted=false;
                        DocumentEstimatedLength=0;DocumentLength=0;
                        memset(CurrentDocumentPart,0,sizeof CurrentDocumentPart);
                        CurrentDocumentPos=0;InChunkMode=false;
                        if(CurrentDocumentChunk) { delete CurrentDocumentChunk;CurrentDocumentChunk=0; }
                        return HTTP_STATUS_HEADER_RECEIVED;
                    }
                    if(StartsWith(CurrentHeaderPart,"http/1."))
                        ReceivedHeaders.AddHeader("ServerCode",CurrentHeaderPart);
                    else
                        ReceivedHeaders.AddHeader(CurrentHeaderPart);
                    CurrentHeaderPos=0;
                    memset(CurrentHeaderPart,0,sizeof CurrentHeaderPart);
                } else CurrentHeaderPos+=receivesize;
            } else if((unsigned long)time(0)>ActionStartTime+TimeOutValue) {
                ReceivedHeaders.Reset();
                CurrentHeaderPos=0;
                DocumentEstimatedLength=0;
                DocumentLength=0;
                CurrentDocumentPos=0;
                memset(CurrentHeaderPart,0,sizeof CurrentHeaderPart);
                InChunkMode=false;
                returnstatus=HTTP_STATUS_TIMEOUT_ERROR;
            }
        }
        if(bcounter!=0) time((time_t*)&ActionStartTime);
    } else if((unsigned long)time(0)>ActionStartTime+TimeOutValue) {
        ReceivedHeaders.Reset();
        ResetCurrentHeaderPart();
        returnstatus=HTTP_STATUS_TIMEOUT_ERROR;
    }
    return returnstatus;
}
}


static int checks=0;
static void check(bool ok,int n) { ++checks;if(!ok){fprintf(stderr,"HTTP fixture check %d failed\n",n);exit(1);} }
namespace LHNetworkLib {
std::string LHHttpHeaders::GetHeaderString() {
    HeaderString="";
    for(std::vector<std::string>::iterator i=HdrDocumentHeader.begin();i!=HdrDocumentHeader.end();++i) {
        HeaderString+=*i;HeaderString+="\r\n";
    }
    return HeaderString;
}
}
struct HttpFixture {
    static void initialise(LHNetworkLib::LHHttp2& h) {
#ifdef _MSC_VER
        check(sizeof(LHNetworkLib::LHHttp2)==0x10a8,1);
        check(offsetof(LHNetworkLib::LHHttp2,PreparedRequest)==0x14&&offsetof(LHNetworkLib::LHHttp2,ReceivedHeaders)==0x40,2);
        check(offsetof(LHNetworkLib::LHHttp2,CurrentHeaderPart)==0x70&&offsetof(LHNetworkLib::LHHttp2,CurrentHeaderPos)==0x870,3);
        check(offsetof(LHNetworkLib::LHHttp2,CurrentDocumentPart)==0x89c&&offsetof(LHNetworkLib::LHHttp2,CurrentDocumentChunk)==0x10a4,4);
#endif
        h.Host=0;h.Hostname="example.test";h.Port=80;h.RequestBytesSent=77;h.RequestLength=99;
        h.TimeOutValue=10;h.ActionStartTime=90;h.ActionStarted=false;h.CurrentAction=1;
        h.CurrDownSize=h.Forwardings=h.CurrForwarding=0;
        memset(h.CurrentHeaderPart,0,sizeof h.CurrentHeaderPart);h.CurrentHeaderPos=0;
        h.ProxyHost="";h.ProxyPort=0;h.DocumentEncoding=LHNetworkLib::ENCODING_NONE;
        h.DocumentEstimatedLength=8;h.DocumentLength=9;
        memset(h.CurrentDocumentPart,0x5a,sizeof h.CurrentDocumentPart);
        h.CurrentDocumentPos=10;h.InChunkMode=true;h.CurrentDocumentChunk=0;
    }
    static void prepare();
    static void receive();
};

#undef time
static const char* incoming="";
static unsigned int incomingIndex=0;
static bool readable=true;
static bool readSuccess=true;
static int readableCalls=0,receiveCalls=0;
namespace LHNetworkLib {
bool LSocket::IsReadable(long timeout) {check(timeout==0,101);++readableCalls;return readable;}
bool LSocket::Receive(std::vector<char>& out) {
    ++receiveCalls;
    if(!readSuccess)return false;
    if(incoming[incomingIndex]) {check(!out.empty(),102);out[0]=incoming[incomingIndex++];NrLastDataReceived=1;}
    else NrLastDataReceived=0;
    return true;
}
}
static void input(const char* text) { incoming=text;incomingIndex=0;readable=true;readSuccess=true;readableCalls=receiveCalls=timeCalls=0;testNow=100; }
void HttpFixture::receive() {
    using namespace LHNetworkLib;
    {LHHttp2 h;initialise(h);input("");h.CurrentAction=0;check(h.ReceiveHeaderAsync()==HTTP_STATUS_INCORRECT_ACTION,110);check(timeCalls==0&&readableCalls==0,111);}
    {LHHttp2 h;initialise(h);input("");check(h.ReceiveHeaderAsync()==HTTP_STATUS_RECEIVING_HEADER,112);check(h.ActionStarted&&h.ActionStartTime==100&&timeCalls==2,113);}
    {LHHttp2 h;initialise(h);input("");h.ActionStarted=true;h.ActionStartTime=1;h.ReceivedHeaders.AddHeader("Old: yes");h.CurrentHeaderPos=5;memcpy(h.CurrentHeaderPart,"hello",6);check(h.ReceiveHeaderAsync()==HTTP_STATUS_TIMEOUT_ERROR,114);check(h.CurrentHeaderPos==0&&h.ReceivedHeaders.HdrDocumentHeader.empty(),115);for(unsigned int i=0;i<sizeof h.CurrentHeaderPart;++i)check(h.CurrentHeaderPart[i]==0,116);}
    {LHHttp2 h;LSocket socket;initialise(h);input("");h.Host=&socket;readSuccess=false;check(h.ReceiveHeaderAsync()==HTTP_STATUS_TIMEOUT_ERROR,120);check(receiveCalls==1,121);}
    {LHHttp2 h;LSocket socket;initialise(h);input("abc");h.Host=&socket;check(h.ReceiveHeaderAsync()==HTTP_STATUS_RECEIVING_HEADER,122);check(h.CurrentHeaderPos==3&&!memcmp(h.CurrentHeaderPart,"abc",3),123);check(receiveCalls==4&&timeCalls==3,124);}
    {LHHttp2 h;LSocket socket;initialise(h);input("HTTP/1.1 200 OK\r\nX-Mode: test\r\n\r\n");h.Host=&socket;
      h.CurrentDocumentChunk=new LHHttpDocumentParts;h.CurrentDocumentChunk->Buffer=new char[4];h.CurrentDocumentChunk->BufferLength=4;h.CurrentDocumentChunk->CurrBufPos=2;
      check(h.ReceiveHeaderAsync()==HTTP_STATUS_HEADER_RECEIVED,130);check(h.CurrentAction==2&&!h.ActionStarted,131);
      check(h.ReceivedHeaders.HdrDocumentHeader.size()==2,132);
      check(h.ReceivedHeaders.HdrDocumentHeader[0]=="ServerCode: HTTP/1.1 200 OK",133);
      check(h.ReceivedHeaders.HdrDocumentHeader[1]=="X-Mode: test",134);
      check(h.DocumentEstimatedLength==0&&h.DocumentLength==0&&h.CurrentDocumentPos==0&&!h.InChunkMode&&h.CurrentDocumentChunk==0,135);
      for(unsigned int i=0;i<sizeof h.CurrentDocumentPart;++i)check(h.CurrentDocumentPart[i]==0,136);
    }
    {LHHttp2 h;LSocket socket;initialise(h);input("");h.Host=&socket;h.ActionStarted=true;h.ActionStartTime=1;h.ReceivedHeaders.AddHeader("Old: value");h.CurrentHeaderPos=4;
      check(h.ReceiveHeaderAsync()==HTTP_STATUS_TIMEOUT_ERROR,140);check(h.ReceivedHeaders.HdrDocumentHeader.empty()&&h.CurrentHeaderPos==0,141);check(h.DocumentEstimatedLength==0&&h.DocumentLength==0&&h.CurrentDocumentPos==0&&!h.InChunkMode,142);
    }
    {LHHttp2 h;LSocket socket;initialise(h);static char text[1201];memset(text,'a',1200);text[1200]=0;input(text);h.Host=&socket;
      check(h.ReceiveHeaderAsync()==HTTP_STATUS_RECEIVING_HEADER,150);check(receiveCalls==1024&&h.CurrentHeaderPos==1024,151);check(h.CurrentHeaderPart[1024]==0,152);
    }
}
int main(){HttpFixture::receive();puts("00840740_TEST PASS");return 0;}