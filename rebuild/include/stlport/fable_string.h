#ifndef FABLE_STLPORT_STRING_H
#define FABLE_STLPORT_STRING_H
// STLport 4.6.2 _string.h/_string.c excerpts, adapted to this header subset.
// SCOREBOARD selects authenticated4.5.2 integral/null dispatch; see row provenance.
// Copyright (c) 1994 Hewlett-Packard Company; 1996,1997 Silicon Graphics
// Computer Systems, Inc.; 1997 Moscow Center for SPARC Technology;
// 1999 Boris Fomitchev. Permission to use/copy/modify is granted without
// fee provided these notices are retained. Provided as is, without warranty.
#include "fable_containers.h"
#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(strlen,memcmp,memcpy,memset)
#endif
namespace std {
// Authentic STLport4.6.2 char_traits.h125..150,207..209; native403c60.
template<class C,class I> class __char_traits_base {
public:static int __cdecl compare(const C*,const C*,size_t);
};
template<class C> class char_traits:public __char_traits_base<C,C> {};
template<> class char_traits<char>
{
public:
    typedef char char_type;
    static void assign(char& a,const char& b) { a=b; }
    static size_t length(const char* p) { return strlen(p); }
    static int compare(const char* a,const char* b,size_t n) { return memcmp(a,b,n); }
    static char* copy(char* a,const char* b,size_t n) { return (char*)memmove(a,b,n); }
#ifdef FABLE_RETAIL_STLP_STRING_FIND
    // Opt-in (chunk 865, row 0x0083F650 LHHttpHeaders::GetHeader): STLport 4.6.2 char_traits.h eq(), used by the find predicates below.
    static bool eq(const char& a,const char& b) { return a==b; }
#endif
};
inline char* uninitialized_copy(const char* first,const char* last,char* result) {
    return (char*)__copy_trivial(first,last,result);
}
#ifdef FABLE_RETAIL_STLP_STRING_FIND
// Opt-in (chunk 865, row 0x0083F650 LHHttpHeaders::GetHeader): STLport 4.6.2 _function_base.h / _string.h / _algo.h pieces behind
// basic_string::find and find_first_not_of. search<const char*,const char*,_Eq_traits> is out of line in retail (0x00567120, declared);
// __find_if<const char*,_Not_within_traits> is out of line too (0x00841900, landed as std_findif_00841900.cpp) but its body is in the
// TU as in STLport, so c2's mod/ref summary knows it never writes through its tag reference (the row's two dispatch-tag temporaries
// then share one argument slot as in retail); auto_inline(off) keeps it a call.
template<class _Traits> struct _Eq_traits                            // binary_function: fable_containers.h
    : public binary_function<typename _Traits::char_type,typename _Traits::char_type,bool> {
    typedef typename _Traits::char_type char_type;
    bool operator()(const char_type& x,const char_type& y) const { return _Traits::eq(x,y); }
};
template<class _Traits> struct _Eq_char_bound {
    typedef typename _Traits::char_type char_type;
    char_type __val;
    _Eq_char_bound(char_type c) : __val(c) { }
    bool operator()(const char_type& c) const { return _Traits::eq(c,__val); }
};
template<class _ForwardIter1,class _ForwardIter2,class _BinaryPred>
_ForwardIter1 __fastcall search(_ForwardIter1 first1,_ForwardIter1 last1,_ForwardIter2 first2,_ForwardIter2 last2,
                                _BinaryPred predicate);                                               // 0x00567120
#pragma auto_inline(off)
template<class _RandomAccessIter,class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter first,_RandomAccessIter last,_Predicate pred,
                                       const random_access_iterator_tag&) {                           // 0x00841900 (landed)
    int tripCount=(last-first)>>2;
    for(;tripCount>0;--tripCount) {
        if(pred(*first)) return first;
        ++first;
        if(pred(*first)) return first;
        ++first;
        if(pred(*first)) return first;
        ++first;
        if(pred(*first)) return first;
        ++first;
    }
    switch(last-first) {
    case 3:
        if(pred(*first)) return first;
        ++first;
    case 2:
        if(pred(*first)) return first;
        ++first;
    case 1:
        if(pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)
template<class _InputIter,class _Predicate>
inline _InputIter find_if(_InputIter first,_InputIter last,_Predicate pred) {
    return __find_if(first,last,pred,random_access_iterator_tag());
}
template<class _Traits> struct _Not_within_traits {
    typedef const typename _Traits::char_type* _Pointer;
    const _Pointer _M_first;
    const _Pointer _M_last;
    _Not_within_traits(_Pointer f,_Pointer l) : _M_first(f),_M_last(l) { }
    bool operator()(const typename _Traits::char_type& x) const {
        return find_if((_Pointer)_M_first,(_Pointer)_M_last,_Eq_char_bound<_Traits>(x))==(_Pointer)_M_last;
    }
};
#endif
template<class T,class A> class _String_base {
public:
    typedef A allocator_type;
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    size_t max_size() const { return size_t(-1)/sizeof(T)-1; }
#ifdef FABLE_RETAIL_STLP_STRING_THROW_INLINE
    // Opt-in (chunk 865, row 0x0083F650): STLport _string.c's helpers are plain members (fable_string_bodies.h keeps them out of line,
    // 0x00565A30 / 0x00565360); in the LHHttpHeaders::GetHeader TU VC7.1 expanded the length error in place (`push "basic_string";
    // call puts; call abort` at 0x0083F7F8). In-class bodies here; do not include fable_string_bodies.h with this opt-in.
    void _M_throw_length_error() const { puts("basic_string");abort(); }
    void _M_throw_out_of_range() const { puts("basic_string");abort(); }
#else
    void _M_throw_length_error() const;
    void _M_throw_out_of_range() const;
#endif
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
#ifdef FABLE_RETAIL_STLP_STRING_FIND
    // Opt-in (chunk 865): STLport 4.6.2 _string.h find(const C*, pos) / find_first_not_of(const C*, pos) and their _string.c bodies.
    size_type find(const C* s,size_type pos=0) const { return find(s,pos,Traits::length(s)); }
    size_type find(const C* s,size_type pos,size_type n) const {
        if(pos+n>size()) return npos;
        const C* result=search((const C*)this->_M_start+pos,(const C*)this->_M_finish,s,s+n,_Eq_traits<Traits>());
        return result!=this->_M_finish?result-this->_M_start:npos;
    }
    size_type find_first_not_of(const C* s,size_type pos=0) const { return find_first_not_of(s,pos,Traits::length(s)); }
    size_type find_first_not_of(const C* s,size_type pos,size_type n) const {
        if(pos>size()) return npos;
        const C* result=find_if((const C*)this->_M_start+pos,(const C*)this->_M_finish,_Not_within_traits<Traits>((const C*)s,(const C*)s+n));
        return result!=this->_M_finish?result-this->_M_start:npos;
    }
#endif
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
        difference_type n=l-f;
        this->_M_allocate_block(n+1);
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
template<class C,class T,class A> inline bool
__cdecl
operator==(const basic_string<C,T,A>& x,const basic_string<C,T,A>& y) {
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
