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
// LHNetworkLib::LHHttp2::PrepareRequest(LHNetworkLib::HTTP_REQUEST_TYPE,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,LHNetworkLib::LHHttpHeaders *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,unsigned long) -- construct the HTTP request and its headers.
// Leaf: ?PrepareRequest@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@W4HTTP_REQUEST_TYPE@2@ABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@PAVLHHttpHeaders@2@1K@Z
// Flags: /GS
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_LHHTTP2
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
#include <stdlib.h>
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
struct __true_type { };
#ifdef FABLE_STLPORT_HTTP
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
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
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) { free(p); }
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
#ifdef FABLE_STLPORT_UI
    vector(const vector&);
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
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
    void resize(size_type n) { resize(n,_Tp()); }
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
    __declspec(noinline) void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
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
    void push_back(const _Tp&);
#endif
};
template<class _T1,class _T2> struct pair {
    _T1 first;
    _T2 second;
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
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
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
template<class C> class char_traits;
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
    template<class I> basic_string(I f,I l,const A& a=A()):_Base(a) { _M_range_initialize(f,l); }
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
    template<class I> void _M_range_initialize(I f,I l) { _M_range_initialize(f,l,forward_iterator_tag()); }
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
    void Reset() { HeaderString="";HdrDocumentHeader.clear(); }
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

#include <stdlib.h>
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

// Layout: engine/LHHttp2.h restores all PDB fields with native STLport widths.
namespace LHNetworkLib {
LH_HTTP_STATUS LHHttp2::PrepareRequest(HTTP_REQUEST_TYPE Method,const std::string& URI,LHHttpHeaders* requestheaders,const std::string& body,unsigned long bodylength)
{
    std::string requesttype;
    std::string reqheaderstring="";
    std::string myheaderstring="";
    PreparedRequest="";
    RequestBytesSent=0;
    switch(Method) {
    case GET: requesttype="GET ";break;
    case PUT: requesttype="PUT ";break;
    case POST: requesttype="POST ";break;
    default: requesttype="GET ";break;
    }
    if(ProxyHost!="") {
        requesttype+="HTTP://"+Hostname;
        if(Port!=80) {
            requesttype+=":";
            char number[20];
            _itoa(Port,number,10);
            requesttype+=number;
        }
        requesttype+="/";
    }
    requesttype+=URI;
    requesttype+=" HTTP/1.1\r\n";
    if(requestheaders) reqheaderstring=requestheaders->GetHeaderString();
    MyHeaders.AddHeader("Host",Hostname);
    MyHeaders.AddHeader("Connection: close");
    if(body!="") {
        std::string content_length_str="Content-Length: ";
        char number[20];
        _itoa(bodylength,number,10);
        content_length_str+=number;
        MyHeaders.AddHeader(content_length_str);
    }
    myheaderstring=MyHeaders.GetHeaderString();
    PreparedRequest=requesttype;
    if(reqheaderstring!="") PreparedRequest+=reqheaderstring;
    PreparedRequest+=myheaderstring;
    PreparedRequest+="\r\n";
    RequestLength=PreparedRequest.size();
    if(body!="") {
        PreparedRequest+=body;
        RequestLength+=body.size();
    }
    MyHeaders.DeleteHeaderString();
    if(requestheaders) requestheaders->DeleteHeaderString();
    return HTTP_STATUS_DOCUMENT_PREPARED;
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

void HttpFixture::prepare() {
    using namespace LHNetworkLib;
    const HTTP_REQUEST_TYPE methods[4]={GET,PUT,POST,(HTTP_REQUEST_TYPE)9};
    const char* words[4]={"GET ","PUT ","POST ","GET "};
    for(int method=0;method<4;++method)for(int proxy=0;proxy<3;++proxy)for(int withBody=0;withBody<2;++withBody)for(int withExtra=0;withExtra<2;++withExtra) {
        LHHttp2 h;initialise(h);
        LHHttpHeaders extra;extra.AddHeader("X-Trace","yes");
        if(proxy) {h.ProxyHost="proxy.test";h.Port=proxy==2?8080:80;}
        std::string body=withBody?"payload":"";
        LH_HTTP_STATUS status=h.PrepareRequest(methods[method],"path",withExtra?&extra:0,body,withBody?7:0);
        std::string expected=words[method];
        if(proxy) { expected+="HTTP://example.test";if(proxy==2)expected+=":8080";expected+="/"; }
        expected+="path HTTP/1.1\r\n";
        if(withExtra)expected+="X-Trace: yes\r\n";
        expected+="Host: example.test\r\nConnection: close\r\n";
        if(withBody)expected+="Content-Length: 7\r\n";
        expected+="\r\n";expected+=body;
        check(status==HTTP_STATUS_DOCUMENT_PREPARED,10);
        check(h.PreparedRequest==expected,11);
        check(h.RequestBytesSent==0&&h.RequestLength==expected.size(),12);
        check(h.MyHeaders.HeaderString=="",13);
        if(withExtra)check(extra.HeaderString=="",14);
        check(h.MyHeaders.HdrDocumentHeader.size()==(withBody?3u:2u),15);
    }
}
int main(){HttpFixture::prepare();puts("008400b0_TEST PASS");return 0;}