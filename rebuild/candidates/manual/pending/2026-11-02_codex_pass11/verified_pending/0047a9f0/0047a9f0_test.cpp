// Self-contained C++03 behavior fixture with selected source and genuine headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <new>
#if !defined(_MSC_VER)
typedef unsigned int fixture_u64 __attribute__((mode(DI)));
#define __declspec(x)
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define long int
#define FABLE_STATIC_ASSERT(x) typedef char fixture_assert_##__LINE__[1]
#else
typedef unsigned __int64 fixture_u64;
#define FABLE_STATIC_ASSERT(x) typedef char fixture_assert_##__LINE__[(x)?1:-1]
#endif
#define FABLE_FASTCALL __fastcall
#define FABLE_CDECL __cdecl
#define FABLE_THISCALL
#define FABLE_STDCALL __stdcall
#define FABLE_VECTOR_INLINE inline
#define FABLE_DEFPTR_INLINE inline
typedef unsigned int fable_u32;typedef int fable_i32;typedef unsigned char fable_u8;typedef signed char fable_i8;
#define private public
#define protected public
// CHeroLogBook::AddEntry(CWideString const &,CWideString const &,CWideString const &,CHeroLogBook::CHeroLogBookEntry::ECategory,bool) -- add a unique hero-log entry and notify sound/map interfaces.
// Leaf: ?AddEntry@CHeroLogBook@@QAEXABVCWideString@@00W4ECategory@CHeroLogBookEntry@1@_N@Z
// Original PDB CWideString, class key V, one protected pointer at +0.
// STLport 4.6.2 _string.h/_string.c excerpts, adapted to this header subset.
// SCOREBOARD selects authenticated4.5.2 integral/null dispatch; see row provenance.
// Copyright (c) 1994 Hewlett-Packard Company; 1996,1997 Silicon Graphics
// Computer Systems, Inc.; 1997 Moscow Center for SPARC Technology;
// 1999 Boris Fomitchev. Permission to use/copy/modify is granted without
// fee provided these notices are retained. Provided as is, without warranty.
// STLport 4.6.2 excerpts: stl/_alloc.h, _vector.h, _list.h, _bitset.h,
// _pair.h and _function_base.h. Original inheritance, field names and keys.
// Only declarations and operations needed by these translation units appear.
#include <stddef.h>
#include <stdlib.h>
#include <new>
#include <string.h>
namespace std {
struct __true_type { };
struct __false_type { };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
template<class I> void __fastcall __destroy_aux(I,I,const __false_type&);
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor;
};
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
template<> struct __type_traits<unsigned short> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
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
template<class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    ~allocator(){}
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
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
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
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
    vector(const vector&);
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
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
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
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
};
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
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
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
    typedef _Tp value_type;
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
#include <string.h>
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
// Ego_r TPI d45a/d454: protected Data, public NoRefs and const equality.
// Native47aa32..44 loads both start/finish pairs; native refcount is +12.
class CWideStringData {
protected:
    std::basic_string<unsigned short> Data;
public:
    long NoRefs;
    bool operator==(const CWideStringData& other)const { const std::basic_string<unsigned short>& other_data=other.Data;return Data==other_data; }
};
FABLE_STATIC_ASSERT(sizeof(CWideStringData)==16);
class CCharString;
class CWideStringData;
class CWideString {
public:
    CWideString(const CWideString&);
    bool operator==(const CWideString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData || !other.PStringData)return false;
        return *PStringData==*other.PStringData;
    }
    bool Contains(const CWideString&)const;
    ~CWideString();
protected:
    static long NoStrings;
    void UnassignString() { if(PStringData) { if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0; } }
    CWideStringData* PStringData;
};

// STLport4.6.2 _tree.h86-113,140-170,197-218,220-265 and _map.h43-85.
// Genuine names, class keys, inheritance and full native three-word tree storage.
namespace std {
template<class A,class B,class R> struct binary_function { typedef A first_argument_type;typedef B second_argument_type;typedef R result_type; };
template<class T> struct less:public binary_function<T,T,bool>{bool operator()(const T& a,const T& b)const{return a<b;}};
template<class P> struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type& operator()(const P& p)const{return p.first;}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class V> struct _Rb_tree_node:public _Rb_tree_node_base { V _M_value_field; };
template<class Dummy> class _Rb_global {
    typedef _Rb_tree_node_base* _Base_ptr;
public:
    static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);
    static void __cdecl _Rotate_left(_Rb_tree_node_base*,_Rb_tree_node_base*&);
    static void __cdecl _Rotate_right(_Rb_tree_node_base*,_Rb_tree_node_base*&);
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* _M_node) {
  if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
    _M_node = _M_node->_M_right;
  else if (_M_node->_M_left != 0) {
    _Base_ptr __y = _M_node->_M_left;
    while (__y->_M_right != 0)
      __y = __y->_M_right;
    _M_node = __y;
  }
  else {
    _Base_ptr __y = _M_node->_M_parent;
    while (_M_node == __y->_M_left) {
      _M_node = __y;
      __y = __y->_M_parent;
    }
    _M_node = __y;
  }
  return _M_node;
}
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node) {
        if(node->_M_right!=0) {
            node=node->_M_right;
            while(node->_M_left!=0) node=node->_M_left;
        } else {
            _Rb_tree_node_base* parent=node->_M_parent;
            while(node==parent->_M_right) {node=parent;parent=parent->_M_parent;}
            if(node->_M_right!=parent) node=parent;
        }
        return node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
    bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {
    typedef typename Traits::reference reference;
    typedef typename Traits::pointer pointer;
    _Rb_tree_iterator(){_M_node=0;}
    _Rb_tree_iterator(_Rb_tree_node<V>* p){_M_node=p;}
    _Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >& p){_M_node=p._M_node;}
    reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}
    pointer operator->()const{return &operator*();}
    _Rb_tree_iterator& operator--(){this->_M_node=_Rb_global_inst::_M_decrement(this->_M_node);return *this;}
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
};
template<class V,class A>struct _Rb_tree_base {
public:
    _Rb_tree_base(const A&);
    ~_Rb_tree_base();
protected:
    _STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>
class _Rb_tree:public _Rb_tree_base<V,A> {
protected:
    size_t _M_node_count;
    Compare _M_key_compare;
public:
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
    _Rb_tree(const Compare&,const A&);
    ~_Rb_tree();
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
protected:
    typedef _Rb_tree_node<V>* _Link_type;
    _Link_type& _M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
    _Link_type& _M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
    static _Link_type& __fastcall _S_parent(_Link_type x){return (_Link_type&)x->_M_parent;}
    static _Link_type& __fastcall _S_left(_Rb_tree_node_base* x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Rb_tree_node_base* x){return (_Link_type&)x->_M_right;}
    static const K& __fastcall _S_key(_Rb_tree_node_base* x){return KeyOfValue()(static_cast<_Link_type>(x)->_M_value_field);}
    _Link_type _M_create_node(const V& x){_Link_type p=this->_M_header.allocate(1);std::_Construct(&p->_M_value_field,x);return p;}
private:
    iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const V&,_Rb_tree_node_base* =0);
public:
    size_t size()const{return _M_node_count;}
protected:
    _Link_type& _M_root()const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& __fastcall _S_left(_Link_type x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
    static V& __fastcall _S_value(_Link_type x){return x->_M_value_field;}
    static const K& __fastcall _S_key(_Link_type x){return KeyOfValue()(_S_value(x));}
private:
    template<class Key> _Rb_tree_node<V>* _M_find(const Key& key)const {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        if(y==this->_M_header._M_data || _M_key_compare(key,_S_key(y)))y=this->_M_header._M_data;
        return y;
    }
public:
    iterator find(const K& key){return iterator(_M_find(key));}
    pair<iterator,bool> insert_unique(const V&);
    iterator insert_unique(iterator,const V&);
    Compare key_comp()const{return _M_key_compare;}
    iterator lower_bound(const K& key) {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        return iterator(y);
    }
    const_iterator find(const K&) const;
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >
class map {
public:typedef pair<const K,V> value_type;
private:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
    typedef typename _Rep_type::iterator iterator;
    explicit map(const Compare& c=Compare(),const A& a=A()):_M_t(c,a){}
    iterator begin(){return _M_t.begin();}
    const_iterator begin()const{return _M_t.begin();}
    iterator end(){return _M_t.end();}
    V& operator[](const K&);
    iterator find(const K& key){return _M_t.find(key);}
    pair<iterator,bool> insert(const value_type& value){return _M_t.insert_unique(value);}
    iterator insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
    iterator lower_bound(const K& key){return _M_t.lower_bound(key);}
    Compare key_comp()const{return _M_t.key_comp();}
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
};
}
FABLE_STATIC_ASSERT(sizeof(std::map<unsigned long,long>)==12);
// Original Ego_r/FableWin classes and native47a9f3 LogEntries+10.
// Original GivenQuests vector shrinks16 to12 and LogEntries map16 to12.
class CHeroLogBook {
public:
    class CQuestDesc;
    class CHeroLogBookEntry {
    public:
        enum ECategory {CATEGORY_QUEST,CATEGORY_STORY,CATEGORY_TUTORIAL,CATEGORY_BASICS,CATEGORY_OBJECTS,CATEGORY_TOWNS,CATEGORY_HERO,CATEGORY_COMBAT};
        CHeroLogBookEntry(const CWideString& name_tag,const CWideString& abbreviated_name,const CWideString& text_tag):Name(name_tag),AbbreviatedName(abbreviated_name),Content(text_tag){}
        CHeroLogBookEntry(const CHeroLogBookEntry& entry):Name(entry.Name),AbbreviatedName(entry.AbbreviatedName),Content(entry.Content){}
        const CWideString& GetName()const{return Name;}
    private:
        CWideString Name,AbbreviatedName,Content;
    };
    void AddEntry(const CWideString&,const CWideString&,const CWideString&,CHeroLogBookEntry::ECategory,bool);
private:
    unsigned char GivenQuests[12]; // Complete vector<CQuestDesc> storage; unused by this member.
    long QuestBeingViewed;
    std::map<CHeroLogBookEntry::ECategory,std::vector<CHeroLogBookEntry> > LogEntries;
};
FABLE_STATIC_ASSERT(sizeof(CHeroLogBook)==28);
FABLE_STATIC_ASSERT(sizeof(CHeroLogBook::CHeroLogBookEntry)==12);
// Original CGameComponent base and native PWorld+24; caller0042f4b1
// allocates0x161e8 before constructor00418dca. The unused remainder is opaque.
// Original PDB bases at0/4, Quit+8, Running+9, Game+c; native size16.
// Retail layout (hand-written; no members). RTTI CBaseClassNonCopyable : CBaseClass; vtable 0x0129A7C4, slot 0 scalar
// deleting dtor 0x0099A470. Ego_r.pdb / FableWin.pdb: ??0CBaseClassNonCopyable@@IAE@XZ (protected),
// ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual). The copy operations are private and undefined (non-copyable).
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// The default ctor is out of line (0x0099A2D0: `mov eax,ecx; mov [eax],0x01231710; ret`): Ego_r.pdb places
// ??0CBaseClass@@QAE@XZ / ??1CBaseClass@@UAE@XZ side by side (0x00B01F20 / 0x00B01F30, one .cpp) and FableWin.pdb does the
// same (0x03044700 / 0x03044720), and 1,225 landed rows call 0x0099A2D0. The copy ctor is the implicit one: FableWin.pdb
// emits ??0CBaseClass@@QAE@ABV0@@Z beside the compiler-generated ??_E (0x018CD7E0), and retail expands it inline (a bare
// vtable store in a derived copy ctor, landed chunk 577 0x00DCEDF0 / chunk 578 0x00F05F30: write `: CBaseClass(other)`).

class CBaseClass {
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    // slot 0
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);

// Layout: original PDB classV, no fields, native secondary base at+4.
class CDeviceResetCallback {
public:
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
};

class CGame;
class CGameComponent : public CBaseClassNonCopyable, public CDeviceResetCallback {
public:
    CGameComponent(CGame&);
    virtual ~CGameComponent();
    virtual void Init();
    virtual bool Run(CGameComponent**)=0;
    virtual void ChangeTextureColourDepth(long)=0;
protected:
    virtual void SetQuit();
    bool Quit;
    bool Running;
    CGame& Game;
};
FABLE_STATIC_ASSERT(sizeof(CGameComponent)==16);

class CThingSearchTools; class CProceduralTexture; class CIDrawEnvironment; class CIDrawWorldMap;
// Ego_r CIDrawWorld TPI: three pure const queries and virtual destructor.
class CIDrawWorld {
public:
    virtual const CProceduralTexture* DrawGetProceduralTexture() const=0;
    virtual const CIDrawEnvironment* DrawGetEnvironment() const=0;
    virtual const CIDrawWorldMap* DrawGetWorldMap() const=0;
    virtual ~CIDrawWorld();
};
namespace boost {
template<class T> class scoped_ptr {
private:
    T* ptr;
public:
    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
};
}
class CWorldInit;class CAIGameCameraBase;class CMainGameComponent; class CPlayerManager; class CGameDefinitionManager;
class CWorldMap; class CEnvironment; class CGameTimeManager;
// Retail CWorld base chain from RTTI; original PDB first members. Remainder
// is kept opaque at its complete retail size, not shortened for this caller.
class CWorld : public CBaseClassNonCopyable, public CIDrawWorld {
protected:
    CMainGameComponent& Component;
    CPlayerManager& PlayerManager;
    const CGameDefinitionManager& DefinitionManager;
    boost::scoped_ptr<CWorldMap> PWorldMap;
    boost::scoped_ptr<CEnvironment> PEnvironment;
    boost::scoped_ptr<CGameTimeManager> PGameTimeManager;
    boost::scoped_ptr<CThingSearchTools> PThingSearchTools;
    unsigned char _pad_0x24[0x174];
public:
    CThingSearchTools& GetThingSearchTools() { return *PThingSearchTools; }
};
FABLE_STATIC_ASSERT(sizeof(CWorld)==0x198);
class CGameDefinitionManager;class CASoundBank; class CMainGameComponentInit; class CDisplayEngine;
class CPlayerManager;
class CMainGameComponent : public CGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;
    CASoundBank* PSampleBank;
    unsigned char PTextBank[8]; // CCountedPointer<NGameText::CDataBank> +14
    boost::scoped_ptr<CPlayerManager> PPlayerManager;
    unsigned char PPlayerInterface[4]; // boost::scoped_ptr<CGamePlayerInterface> +20
    boost::scoped_ptr<CWorld> PWorld;
    boost::scoped_ptr<CDisplayEngine> PDisplayEngine;
    unsigned char PLUA[8]; // CCountedPointer<CLUA> +2c
    bool ForceUpdateTick;
    float ForceUpdateTickSpeedMultipler;
    float ForceUpdateTickSpeedDesiredFramerate;
    long ForceUpdateNoFailedUpdates;
    bool FirstWorldFrameUpdate;
    long CurrentServerFrame;
    long InputServerFrame;
    long LastGameTurnForceRendered;
    double CurrentFrameStartGameTime;
    double GameStartTime;
    double LastFrameRenderDuration;
    unsigned char _pad_0x70[0x16178]; // All original members LastInterpolationInfo through DebugNoFramesUnableToRender.
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
    CMainGameComponent(CGame&,const CMainGameComponentInit&);
    virtual ~CMainGameComponent();
    virtual bool Run(CGameComponent**);
    virtual void ChangeTextureColourDepth(long);
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
    CPlayerManager& GetPlayerManager(){return *PPlayerManager;}
    const CWorld& PeekWorld() const { return *PWorld; }
};
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent)==0x161e8);

class CThing; class CPlayer;class CMainGameComponent;class CGameDefinitionManager;
// TPI0x3585d original CBaseClassNonCopyable; complete members Component..HeroSwapPlayerScriptNames.
class CPlayerManager : public CBaseClassNonCopyable {
private: unsigned char _pad_0x04[0x28];
public: bool IsThingMainPlayer(const CThing&) const;
    CPlayerManager(CMainGameComponent&,const CGameDefinitionManager&);
    CPlayer& GetMainPlayer();
};
FABLE_STATIC_ASSERT(sizeof(CPlayerManager)==0x2c);


// Layout: original base class4 bytes plus private Valid+4, native size8.
class CInitBaseClass:public CBaseClass {private:bool Valid;public:CInitBaseClass();~CInitBaseClass();
};
class CThingPlayerCreature;
// Original CInitBaseClass base8; donor564 minus native container shrink8.
// Native player fields and constructors establish full retail size22c.
class CPlayer:public CInitBaseClass {
private: unsigned char _pad_0x08[0x224];
public: CPlayer(); CThingPlayerCreature* GetPControlledCreature() const;
};
FABLE_STATIC_ASSERT(sizeof(CPlayer)==0x22c);
// Original CThing hierarchy and members from Ego_r TPI 0xbbaf; native
// AutoPlace/OnHit confirm PhysicsTC+60, PBaseDef+70, Dead+91; ABI size98.
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Retail opt-in (chunk 708): define FABLE_RETAIL_CCLASS (with FABLE_RETAIL_BASE_OBJECT) before the include for the polymorphic
// retail class. RTTI .?AVCClass@@ : CBaseObject : CBaseClass (ghidra_out/rtti_bases.tsv). Vtable 0x0129CF60, 3 slots (slot 3 is
// data): slot 0 the deleting dtor 0x00A379E0 (calls ~CBaseObject 0x0099A430 directly, no CClass vtable store: the destructor is
// compiler-generated, FableWin's ??1CClass@@UAE@XZ is its debug copy), slot 1 CBaseObject::Validate 0x0099A400, slot 2 Save
// 0x00A19AB0 (`ret 4`, FableWin ?Save@CClass@@UAEXAAVCCharString@@@Z). Unregister 0x00A37160 (both PDBs
// ?Unregister@CClass@@QAEXAAVCClassFactory@@@Z; retail inlines CClassFactory::UnregisterAllocatedClass into it), PostConstructionInit
// (Ego_r 0x00B76790), GetType (FableWin inline QBEKXZ). Members: Ego_r names; the two list iterators are STLport
// _List_iterator<CClass*> (one node pointer each, kept opaque here); Type +0x10; sizeof 0x14 (unpacked: derived members start at
// +0x14, e.g. CNavigatorBase::PInterface 0x00A471AA, CThing chunk 705).
#include <stddef.h>
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Retail opt-in (2026-10-30, class-model migration wave 5): define FABLE_RETAIL_BASE_OBJECT for the polymorphic retail class.
// RTTI .?AVCBaseObject@@ : CBaseClass (ghidra_out/rtti_bases.tsv, 1 base). Vtable 0x0129A7D8, 2 slots (the next dword
// is string data): slot 0 scalar deleting dtor 0x0099A4D0, slot 1 Validate 0x0099A400 (`ret`). Members and spellings are
// the Ego_r.pdb and FableWin.pdb names, identical in both (Ego_r 0x00B01FD0-0x00B020F0, FableWin 0x03044AF0-0x03044E40):
// ??0CBaseObject@@IAE@XZ, ??0CBaseObject@@IAE@ABV0@@Z, ??4CBaseObject@@IAEAAV0@ABV0@@Z (protected), ??1CBaseObject@@MAE@XZ
// (protected virtual), ?Validate@CBaseObject@@UBEXXZ, ?ClearIntelligentPointers@CBaseObject@@QAEXXZ,
// ?InternalValidate@CBaseObject@@IBEXXZ. Retail bodies (CBaseClass.cpp's TU): ctor 0x0099A380 and copy ctor 0x0099A390
// both `mov [eax],0x0129A7D8; mov [eax+4],0` (the copy does not copy the pointer); operator= 0x0099A3B0 `mov eax,ecx;
// ret 4`; ClearIntelligentPointers 0x0099A3C0 (zeroes [p+0], deletes p when [p+4]==0, zeroes +4); ~CBaseObject 0x0099A430
// (the same body inline, then the CBaseClass vtable store: same-TU auto-inlining, wave 2 LESSONS item 2). All of them are out
// of line: no landed row outside that TU stores 0x0129A7D8, and the four spellings above are bound in reloc_bindings.tsv.
// Layout (retail bytes): IntelligentPointer +0x04 (read/zeroed at [ecx+4] by 0x0099A3C0); sizeof 8 (Ego_r.pdb agrees).
// unproven: InternalValidate has no retail body (debug only: Validate is `ret`); the class key of CBaseObjectPointer
// (no PDB mangled name carries it; 1,839 landed rows say `class`, 72 `struct`).
// The CBaseClass.cpp TU rows (0x0099A2D0-0x0099A4D0) model this class row-locally by design (auto-inlined bodies).

class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
protected:
    CBaseObject();                                                       // 0x0099A380
    CBaseObject(const CBaseObject&);                                     // 0x0099A390
    CBaseObject& operator=(const CBaseObject&);                          // 0x0099A3B0
public:
    void ClearIntelligentPointers();                                     // 0x0099A3C0
    virtual void Validate() const;                                       // slot 1  0x0099A400
protected:
    virtual ~CBaseObject();                                              // slot 0  0x0099A4D0 (scalar deleting); ~ 0x0099A430
    void InternalValidate() const;                                       // unproven: no retail body
public:
    CBaseObjectPointer* IntelligentPointer;                              // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
class CCharString;
class CClassFactory;
class CClass : public CBaseObject {
public:
    virtual void Save(CCharString&);                                     // slot 2  0x00A19AB0
    void Unregister(CClassFactory&);                                     // 0x00A37160
    void PostConstructionInit(CClassFactory&);                           // ?PostConstructionInit@CClass@@QAEXAAVCClassFactory@@@Z
    unsigned long GetType() const { return Type; }                       // FableWin ?GetType@CClass@@QBEKXZ (inline)
    unsigned char ClassListPos[0x4];                                     // +0x08 _List_iterator<CClass*> (STLport)
    unsigned char TypeListPos[0x4];                                      // +0x0c _List_iterator<CClass*> (STLport)
    unsigned char Type;                                                  // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CClass, Type) == 0x10);
// Retail declarations (Codex pass 11): Ego_r TPI 0x27dc, class key V,
// sizeof 4, protected PStringData +0; native array construction advances by 4.
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};
// Retail model (hand-written 2026-10-30, class-model migration wave 2): the counted def handle CDefPointer<T>.
// Layout: Ego_r.pdb CDefPointer<T> is 4 bytes, `CDefPointeeBase* Object` at +0 (ghidra_out/struct_layouts_egor.tsv; every
// instantiation). Ego_r inlines every member; the member set is FableWin.pdb's (debug, nothing inlined):
//   ??0..QAE@W4ENullDef@@@Z (332 instantiations), copy ??0..QAE@ABV0@@Z (52), the template converting ctor ??$?0 (e.g.
//   CDefPointer<const CDefClassBase>::CDefPointer<CDefClassBase>(const CDefPointer<CDefClassBase>&)), ??1 (249), ??4 (31),
//   the template ??$?4 operator=<U>, ?Release@..QAEXXZ (249), ?InternalGetObject@..QBEPAVCDefPointeeBase@@XZ (245),
//   ??$SetDynamicCast@$$CBVCDefClassBase@@@..QAEXABV?$CDefPointer@$$CBVCDefClassBase@@@@@Z (246), ??C operator-> (229), ??D
//   operator* (42), ?Get@..QBEPBV..XZ (73), ?IsNull@..QBE_NXZ (42), ?Reset@..QAEXPBV..@Z (18), ?GetRefCount@..QBEKXZ (1).
// Inline bodies (retail bytes): retail expands all of them. One template gives both release shapes the landed GetSubDef<T>
// rows show (0x008C79B0 and family): the manager's CDefPointer<CDefClassBase> temporary releases as `mov edi,[eax+4]; ...
// dec edi; ... mov [eax],edi; jne`, the CDefPointer<const T> assignment and destructor as `dec dword ptr [ecx+4]; jne`;
// Release() must hold the pointee in a local (the temporary's Delete call reuses ecx; reloading Object differs).
// The assignment shape (`cmp; test; dec [ecx+4]; jne; call [eax+4]; and [esi],0; inc [eax+4]`) is the out-of-line
// operator= rows (e.g. 0x00592844). SetDynamicCast<U>: FableWin.pdb pairs it with GFDebugPDynamicCast<CDefPointeeBase*,
// const T*> (a debug-only type check); release code is the plain assignment.
// Forced inline: retail never calls a CDefPointer member out of line (no relocation binding names one; the four
// out-of-line operator= bodies, e.g. 0x00592844, are unreferenced COMDATs), size-optimised TUs included (landed GetSubDef<T>
// rows 0x007BE88C / 0x007BE92E: plain `inline` is called out of line there, 112 vs 162 bytes); __forceinline reproduces that.
// ENullDef constructor (added 2026-10-31, owner request): FableWin.pdb's ??0?$CDefPointer@..@@QAE@W4ENullDef@@@Z (332
// instantiations) is the null constructor; no ??0..@@QAE@XZ default constructor exists in either PDB although classes
// default-construct CDefPointer members, so the enum parameter carries a default argument (inferred from that absence).
// The enumerator's name is in neither PDB name list: NULL_DEF is a placeholder spelling (naming queue), the enum's type
// name ENullDef is the PDB's. Retail bytes: `and/mov dword ptr [..], 0` — the body is `Object = 0`. Unproven: Reset's body.
// Retail layout (hand-written; no Ego_r layout row: the class adds no members). RTTI: CDefPointeeBase : CResource.
// Vtable 0x01230BA0: slot 0 scalar deleting dtor 0x0042D9A0, slots 1-9 inherited from CResource unchanged.
// Only the FABLE_RETAIL_DEF_CLASS_BASE opt-in uses it (included by engine/CDefClassBase.h). The dtor is inline: 0x0042D9A0
// stores this vtable and calls ~CResource 0x009FC550; only FableWin.pdb (debug) has ??1CDefPointeeBase@@UAE@XZ.
#include <stddef.h>
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CResource = 0x1c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in: define FABLE_RETAIL_DEF_CLASS_BASE for the polymorphic retail class (RTTI CResource :
// CIVCountedPointeeBase; vtable 0x012354A4, slots 2-9 below with their retail addresses; Ego_r.pdb signatures).
#include <stddef.h>
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CIVCountedPointeeBase = 0x8.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in: define FABLE_RETAIL_DEF_CLASS_BASE before the include for the polymorphic retail class (vtable
// 0x012354D0: slot 0 scalar deleting dtor 0x004664E0, slot 1 Delete 0x0042D930). The dtor is inline (0x004664E0 is only the
// vtable store + conditional delete; ~CResource 0x009FC550 ends with the same store; only FableWin.pdb has the ??1 symbol).
// The const reference-count accessors AddRef / Release (returns the new count, long) / GetRefCount are FableWin.pdb-only
// (Ego_r and retail inline them everywhere; no retail body). Retail shapes: `inc [eax+4]`, `dec [ecx+4]; jne` and, for the
// CDefPointer<CDefClassBase> temporary, `mov edi,[eax+4]; dec edi; mov [eax],edi` (engine/CDefPointer.h; landed GetSubDef<T>
// rows, wave 2). Whether Lionhead wrote IVRefCount `mutable` or const_cast in the accessors is not visible in the bytes (the
// two compile identically); `mutable` follows the landed local models.
// The flattened layout below stays the
// default so the landed includers are unchanged.
#include <stddef.h>
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }                                 // slot 0  0x004664E0 (scalar deleting; inline dtor)
    virtual void Delete() const;                                         // slot 1  0x0042D930
    void AddRef() const { ++IVRefCount; }                                // FableWin.pdb ?AddRef@CIVCountedPointeeBase@@QBEXXZ (inline)
    long Release() const { return --IVRefCount; }                        // FableWin.pdb ?Release@CIVCountedPointeeBase@@QBEJXZ (inline)
    long GetRefCount() const { return IVRefCount; }                      // FableWin.pdb ?GetRefCount@CIVCountedPointeeBase@@QBEJXZ (inline)
    mutable unsigned long IVRefCount;                                    // +0x04 (mutable: the const accessors count; see above)
};
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);

class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResource();                                                         // 0x009FBEC0 (??0CResource@@QAE@XZ, bound)
    virtual ~CResource();                                                // slot 0  0x004664FF (scalar deleting); ~CResource 0x009FC550
    virtual bool UnloadResource();                                       // slot 2  0x0042D940
    virtual unsigned long GetUnloadableMemory() const;                   // slot 3  0x009FBEF0
    virtual bool IsResourceLoaded() const;                               // slot 4  0x0042D950
    virtual void OnResourceLoaded();                                     // slot 5  0x0042D960
    virtual bool CanAbortResourceLoad(long) const;                       // slot 6  0x0042D970
    virtual void OnResourceLoadAborted();                                // slot 7  0x0042D980
    virtual unsigned long GetDebugMemoryInUse() const;                   // slot 8  0x009FBF00
    virtual void ResetDebugMemoryInUse();                                // slot 9  0x009FBF10
    CResourceList* ResourceList;                                         // +0x08
    CResource*     PrevResource;                                         // +0x0c
    CResource*     NextResource;                                         // +0x10
    unsigned long  ResourceSize;                                         // +0x14
    unsigned long  LastUsedFrame;                                        // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);


class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       // slot 0  0x0042D9A0 (scalar deleting; inline dtor)
};
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1c);


enum ENullDef { NULL_DEF };                                          // W4ENullDef@@ (global); enumerator name unproven

template <class T> class CDefPointer {
public:
    FABLE_DEFPTR_INLINE CDefPointer(ENullDef = NULL_DEF) : Object(0) {}             // ??0..QAE@W4ENullDef@@@Z
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};
FABLE_STATIC_ASSERT(sizeof(CDefPointer<CDefPointeeBase>) == 0x4);


// Ego_r TPI CArray derives std::vector; CVectorMap derives CArray.
// Retail LowerBound reads start+0/finish+4. Compare +12, Dirty +13.
template<class T> class CArray : public std::vector<T,std::allocator<T> > {
};
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    __declspec(noinline) iterator LowerBound(const K& key) {
        iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    __declspec(noinline) const_iterator LowerBound(const K& key) const {
        const_iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            const_iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
private:
    CompareType Compare;
    bool Dirty;
};
// Ego_r original enum names and values; native HasTC masks corroborate used values.
enum ETCInterfaceType {
    TCI_NULL = 0,
    TCI_PERFORM_ACTION = 1,
    TCI_PHYSICS = 2,
    TCI_APPEARANCE_MORPH = 3,
    TCI_HERO_STATS = 4,
    TCI_WOUND = 5,
    TCI_SMOKE_GENERATOR = 6,
    TCI_NAVIGATION = 7,
    TCI_TARGETING = 8,
    TCI_TARGETED = 9,
    TCI_CREATURE_STATS = 10,
    TCI_CREATURE_AI = 11,
    TCI_CREATURE_EXPRESSION = 12,
    TCI_ON_ACTION_USE = 13,
    TCI_CARRIED_ON_ACTION_USE = 14,
    TCI_TALK = 15,
    TCI_SOUND_PLAYER = 16,
    TCI_INVENTORY = 17,
    TCI_INVENTORY_CLOTHING = 18,
    TCI_INVENTORY_WEAPONS = 19,
    TCI_INVENTORY_STATS = 20,
    TCI_INVENTORY_MAGIC = 21,
    TCI_INVENTORY_EXPERIENCE = 22,
    TCI_INVENTORY_TRADE = 23,
    TCI_INVENTORY_AUGMENT = 24,
    TCI_INVENTORY_QUESTS = 25,
    TCI_INVENTORY_ITEM = 26,
    TCI_INVENTORY_MAP = 27,
    TCI_EDITOR = 28,
    TCI_EDITOR_ANIMATION_THING = 29,
    TCI_EDITOR_ANIMATION_CREATURE = 30,
    TCI_SCRIPTED_CONTROL = 31,
    TCI_MESH_AUTOMATIC_ENTITY_CREATOR = 32,
    TCI_THING_OWNER = 33,
    TCI_VILLAGE = 34,
    TCI_VILLAGE_MEMBER = 35,
    TCI_JACK_SHIELD = 36,
    TCI_JACK_PILLAR = 37,
    TCI_DOOR = 38,
    TCI_LIGHT = 39,
    TCI_CLOCK = 40,
    TCI_HERO = 41,
    TCI_ROTATE = 42,
    TCI_RESPAWN = 43,
    TCI_WEATHERVANE = 44,
    TCI_SINGLE_USER_ACTION_POINTS = 45,
    TCI_NAMED_USER_ACTION_POINTS = 46,
    TCI_EXCLUSIVE_USE = 47,
    TCI_COMBAT = 48,
    TCI_ENTITY_MODE_MANAGER = 49,
    TCI_CREATED_ENTITY = 50,
    TCI_OWNED_ENTITY = 51,
    TCI_PERCEIVE_THING = 52,
    TCI_BED = 53,
    TCI_STEALTH = 54,
    TCI_NOISE = 55,
    TCI_CUPBOARD = 56,
    TCI_TROPHY = 57,
    TCI_MAPWHO = 58,
    TCI_DRIVER_QUEST = 59,
    TCI_CONTAINER = 60,
    TCI_CREATURE_GENERATOR = 61,
    TCI_DRIVER_CAMERA_POINTS = 62,
    TCI_CAMERA_POINT = 63,
    TCI_DISCOVERY = 64,
    TCI_EXPLOSION = 65,
    TCI_ENTITY_EVENTS = 66,
    TCI_LOOK = 67,
    TCI_SHAPE_MANAGER = 68,
    TCI_EMOTEICON = 69,
    TCI_CARRYING = 70,
    TCI_CARRYABLE = 71,
    TCI_WEAPON = 72,
    TCI_ENEMY = 73,
    TCI_BANDIT = 74,
    TCI_PARTICLE_EMITTER = 75,
    TCI_WEAPON_TRAIL = 76,
    TCI_SOLID_TO_THING = 77,
    TCI_SMASHABLE = 78,
    TCI_CREATURE_OPINION_OF_HERO = 79,
    TCI_OPINION_SOURCE = 80,
    TCI_HERO_OPINION_DEED_LOG = 81,
    TCI_HERO_PODIUM = 82,
    TCI_SHOP = 83,
    TCI_STOCK_ITEM = 84,
    TCI_BLACKSMITHS = 85,
    TCI_TIME_APPEARANCE_FADE = 86,
    TCI_FAMILY_MEMBER = 87,
    TCI_NAVIGATION_SEED = 88,
    TCI_GIFT = 89,
    TCI_ANIMATION = 90,
    TCI_GRAPHIC_APPEARANCE_NEW = 91,
    TCI_GRAPHIC_APPEARANCE_BASIC_COMPONENT = 92,
    TCI_HERO_SUIT = 93,
    TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS = 94,
    TCI_HIGHLIGHT_ITEM = 95,
    TCI_EMPTY_DRIVER = 96,
    TCI_CREATURE_ITEM_LEVITATION = 97,
    TCI_CREATURE_HIVE = 98,
    TCI_REGION_ENTRANCE = 99,
    TCI_REGION_EXIT = 100,
    TCI_ACTIVATION_RECEPTOR = 101,
    TCI_ACTIVATION_TRIGGER = 102,
    TCI_DEGRADABLE = 103,
    TCI_HERO_EXPERIENCE = 104,
    TCI_EXPERIENCE_ORB = 105,
    TCI_HERO_CENTRE = 106,
    TCI_HERO_GUIDE = 107,
    TCI_QUEST_CARD = 108,
    TCI_FLAMMABLE = 109,
    TCI_HERO_CENTRE_DOOR_MARKER = 110,
    TCI_HERO_ABILITIES = 111,
    TCI_BOASTING_PODIUM = 112,
    TCI_BOASTING_POSITION = 113,
    TCI_BOASTING_AREA = 114,
    TCI_BOASTING_CROWD_CONTROL = 115,
    TCI_CREATURE_GROUP_BOASTING_CROWD = 116,
    TCI_VOLUME_CONTAINMENT_TRACKER = 117,
    TCI_DEAD_SHOT_IN_THING_TRACKER = 118,
    TCI_SHOT_DEFINITION = 119,
    TCI_SKELETAL_MORPH = 120,
    TCI_SWITCHABLE_NAVIGATION = 121,
    TCI_REGION_FOLLOWER = 122,
    TCI_GATHER_POINT = 123,
    TCI_DAY_OR_NIGHT_ONLY_SUPPORT = 124,
    TCI_TRAP = 125,
    TCI_UNDEAD_SOUL = 126,
    TCI_HIT_LOCATIONS = 127,
    TCI_MONEY_RECEIVER = 128,
    TCI_HERO_RANGED_ATTACKS = 129,
    TCI_PARTICLE_ATTACHER = 130,
    TCI_ANIMATING_OBJECT = 131,
    TCI_BERSERK = 132,
    TCI_DIVINE_WRATH = 133,
    TCI_UNHOLY_POWER = 134,
    TCI_MULTI_STRIKE = 135,
    TCI_MULTI_ARROW = 136,
    TCI_BURNT_EFFECT = 137,
    TCI_ELECTROCUTED_EFFECT = 138,
    TCI_CREATURE_TINT_EFFECT = 139,
    TCI_HASTE = 140,
    TCI_FORCE_PUSHED = 141,
    TCI_REGION_DISPLAY = 142,
    TCI_EXPRESSION = 143,
    TCI_SHOP_KEEPER = 144,
    TCI_SUMMON_SPELL = 145,
    TCI_BULLET_TIME = 146,
    TCI_PHYSICAL_SHIELD_SPELL = 147,
    TCI_WILL_LIGHTNING = 148,
    TCI_WILL_ENFLAME = 149,
    TCI_FORCE_PUSH_POWER = 150,
    TCI_DRAIN_LIFE = 151,
    TCI_LIFE_DRAINABLE = 152,
    TCI_HEAL_LIFE = 153,
    TCI_GHOST_SWORD = 154,
    TCI_EFFECT_ON_DIE = 155,
    TCI_WILL_RESPONSE = 156,
    TCI_FIREBALL_SPELL = 157,
    TCI_TURNCOAT_SPELL = 158,
    TCI_TURNCOAT = 159,
    TCI_SUMMONABLE_CREATURE = 160,
    TCI_SUMMONNED_CREATURE = 161,
    TCI_INTERNAL_MARKER = 162,
    TCI_CREATURE_GROUP = 163,
    TCI_CREATURE_GROUP_TAG = 164,
    TCI_MESH_REPLACER = 165,
    TCI_PRE_CALCULATED_NAVIGATION_ROUTE = 166,
    TCI_SPECIAL_ABILITIES = 167,
    TCI_BATTLE_CHARGE = 168,
    TCI_ASSASSIN_RUSH = 169,
    TCI_AI_SCRATCHPAD = 170,
    TCI_TAVERN = 171,
    TCI_SINGLE_PERSON_OCCUPIABLE = 172,
    TCI_BOSS = 173,
    TCI_ROCK_TROLL_SHIELD = 174,
    TCI_COMBAT_LEADER = 175,
    TCI_FISHING_SPOT = 176,
    TCI_FISHING = 177,
    TCI_STEALABLE_ITEM_LOCATION = 178,
    TCI_INTERESTING_TO_VILLAGERS = 179,
    TCI_GUARD = 180,
    TCI_CRATE_STACK = 181,
    TCI_WATER_WADER = 182,
    TCI_OVERHEAD_DISPLAY = 183,
    TCI_TAVERN_TABLE = 184,
    TCI_TAVERN_BAR = 185,
    TCI_TAVERN_TANKARD = 186,
    TCI_TAVERN_JUG = 187,
    TCI_FOLLOWED = 188,
    TCI_TRESPASSABLE = 189,
    TCI_TRESPASSER = 190,
    TCI_OBJECT_AUGMENTATIONS = 191,
    TCI_AUGMENTATION = 192,
    TCI_GOSSIP = 193,
    TCI_ACTION_USE_SCRIPTED_HOOK = 194,
    TCI_DRUNKENNESS = 195,
    TCI_ON_HERO_PUSH = 196,
    TCI_VILLAGE_CRIME = 197,
    TCI_IN_GAME_MENU = 198,
    TCI_COOP_SPIRIT = 199,
    TCI_HIT_ON_COLLISION = 200,
    TCI_GENERATES_EXPERIENCE_ON_KILLING = 201,
    TCI_CREATURE_GENERATOR_CREATOR = 202,
    TCI_HERO_RECEIVE_ITEMS = 203,
    TCI_GOLD = 204,
    TCI_CREATURE_HIT_NOTIFICATION = 205,
    TCI_AICREATURE_WILL_POWER_INDICATOR = 206,
    TCI_TAVERN_GAME = 207,
    TCI_GAME_PLAYER = 208,
    TCI_DECAPITATION = 209,
    TCI_TAVERN_GAME_MASTER = 210,
    TCI_IDLE_SCHEDULER = 211,
    TCI_SLEEP = 212,
    TCI_BUYABLE_HOUSE = 213,
    TCI_DUMMY_VILLAGER = 214,
    TCI_WIFE = 215,
    TCI_PARTICLE_ATTACHMENT_TRACKER = 216,
    TCI_CREATURE_FLY_ON_SPLINE = 217,
    TCI_WALL_MOUNT = 218,
    TCI_WALL_MOUNT_EFFECTS = 219,
    TCI_FISH = 220,
    TCI_TELEPORTER = 221,
    TCI_EXPLODING_OBJECT = 222,
    TCI_HERO_SPECIAL_MOVEMENT = 223,
    TCI_HERO_REMEMBER_CRIMES = 224,
    TCI_RESURRECTION_ITEM = 225,
    TCI_KRAKEN = 226,
    TCI_KRAKEN_TENTACLE = 227,
    TCI_CREATURE_FLY_IN_CIRCLES = 228,
    TCI_INFO_DISPLAY = 229,
    TCI_SPOT_LIGHT = 230,
    TCI_AMBIENT_BIRD = 231,
    TCI_BOAST_UI = 232,
    TCI_TEXTURE_DECAL = 233,
    TCI_FISHING_ROD = 234,
    TCI_THING_UID_DUMMY_FOR_SERIALISATION = 235,
    TCI_RUMBLE = 236,
    TCI_PHYSICAL_OBSTRUCTION = 237,
    TCI_ELECTROCUTE_ON_TOUCH = 238,
    TCI_DIGGING_SPOT = 239,
    TCI_DICE_GAME = 240,
    TCI_PLOT = 241,
    TCI_BOB = 242,
    TCI_FINE_DIALOGUE = 243,
    TCI_SOUND_ATMOSPHERE = 244,
    TCI_NYMPH = 245,
    TCI_QUICK_ACCESS_MENU = 246,
    TCI_WOBBLE = 247,
    TCI_CAMERA_COLLISION = 248,
    TCI_FADE_OUT_AND_IN = 249,
    TCI_AI_SUMMON = 250,
    TCI_FLASH_ON_HIT = 251,
    TCI_QUEST_COMPLETION_UI = 252,
    TCI_DESTROY_THING_ON_ACTION_FINISH = 253,
    TCI_OBSTACLE = 254,
    TCI_ATMOS_PLAYER = 255,
    TCI_FLOURISH_TARGET = 256,
    TCI_CREDITS_UI = 257,
    TCI_BETTING = 258,
    TCI_LIFE_TIMER = 259,
    TCI_FIRE_RANGED_WEAPON_ON_LEAVING_STATE_GROUP = 260,
    TCI_AVOID_REGION_EXIT = 261,
    TCI_GRAPHIC_APPEARANCE_STATIC_MESH_FADE_OUT_ON_PROXIMITY = 262,
    TCI_ORACLE_MINIGAME = 263,
    TCI_FIREHEART_MINIGAME = 264,
    TCI_DESTROY_THING_ON_SPECIAL_ABILITY_FINISH = 265,
    TCI_LIGHTNING_ORB = 266,
    TCI_LIGHTNING_NODE = 267,
    TCI_HAS_NAME = 268,
    TCI_HERO_ONLINE_SCOREBOARD = 269,
    TCI_PHOTOJOURNAL_CAPTURE = 270,
    TCI_EXPLOSIVE_TRAIL = 271,
    TCI_ABSORB_HITS = 272,
    TCI_GENERATED_CREATURE = 273,
    MAX_NO_TCI_TYPES = 274
};
class C3DVector; class CTCPhysicsBase; class CTCBase; class CTCDriverBase;
class CThingBaseDef; class CWorld; class CPlayerManager;class CTCParamsBase;
namespace NGraphicAppearance { class CTCGraphicAppearance; }
class CThing : public CClass {

protected:
    fixture_u64 UniqueID;
    std::bitset<274> UsingComponents;
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Components;
    std::list<CTCBase*> UpdateComponentsList;
    CCharString SerialisationSectionName;
    CTCDriverBase* PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;
    CDefPointer<const CThingBaseDef> PBaseDef;
    CCharString ScriptName;
    CCharString ScriptData;
public:
    CThing* RenderListPrev;
    CThing* RenderListNext;
    CThing* UpdateListPrev;
    CThing* UpdateListNext;
protected:
    unsigned short DefGlobalIndex;
    short CurrentMapNumber;
    signed char PlayerNumber;

protected:
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char ActivatedScript : 1;
    unsigned char LevelIsUnloading : 1;
    unsigned char IsAffectedByBulletTimeOverride : 1;
    unsigned char AddingTCsPermitted : 1;

private:
    unsigned char InitialisationStage : 3;
public:
    void Kill(bool); // Ego_r original public member; native004c9b80.
    CTCBase* AddTC(const CCharString&,bool,const CTCParamsBase*);
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    bool IsDead() const { return Dead!=0; }
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
    bool IsAlive() const { return !Dead; }
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThing,UniqueID)==0x18);
        FABLE_STATIC_ASSERT(offsetof(CThing,UsingComponents)==0x20);
        FABLE_STATIC_ASSERT(offsetof(CThing,Components)==0x44);
        FABLE_STATIC_ASSERT(offsetof(CThing,PhysicsTC)==0x60);
        FABLE_STATIC_ASSERT(offsetof(CThing,PBaseDef)==0x70);
    }
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& GetPos() const;
    C3DVector GetPhysPos() const;
protected:
    CWorld& GetWorld();
    const CWorld& PeekWorld() const;
    unsigned long* GetPWorldSeed() const;
    CPlayerManager& GetPlayerManager();
};
FABLE_STATIC_ASSERT(sizeof(CThing)==0x98);

class CBaseObject;
class CBaseObjectPointer;
// Ego_r TPI CBaseIntelligentPointer: independent polymorphic class, PData +4;
// retail GetPItem 00a01b50 receives this+0 and reads PData +4.
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
// PDB C3DVector complete TPI 0x2783: public X/Y/Z at 0/4/8, size 12.
// Embedded alignment is under review; this header introduces no packing directive.
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
// Retail model (hand-written 2026-10-30, class-model migration wave 9): the engine's reference-counted owning pointer and
// its shared count block. Not polymorphic (no vtable, no RTTI); every member is inline in the release build.
//
// CCPPointerInfo (Ego_r.pdb, 12 bytes): RefCount +0x0 (ulong), DeleteFunc +0x4, Data +0x8 (void*). DeleteFunc is
//   __fastcall: every retail release loads `mov ecx,[eax+8]; call [eax+4]` (0x006E7AB0, 0x00CE1000, 0x00CE1040, 0x008AB2B0)
//   and the deleters it points at are `SIXPAX@Z` (static __fastcall void(void*)) CCountedPointer<T>::DeleteData in
//   FableWin.pdb (343 instantiations); Ego_r.pdb's `_func___cdecl_void_void_ptr` is the PDB's generic spelling.
//   Ctor: FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z = (unsigned long, void (__fastcall*)(void*), void*); retail
//   Reset 0x008AB2B0 expands it after `new(0xc)`: [eax]=1, [eax+4]=deleter, [eax+8]=Data (member order).
//
// CCountedPointer<T> (Ego_r.pdb, 8 bytes for every T): Data +0x0 (T*), Info +0x4 (CCPPointerInfo*). Member set = FableWin.pdb
//   (the debug build emits them; Ego_r keeps out of line only ShareData / ReleaseData / Reset / DeleteData, the rest inline):
//   ??0 (), (T*), (const&), member template ??$?0 (const CCountedPointer<U>&); ??1; ??4 (const&), member template ??$?4;
//   ??C operator-> QBE, ??D operator* QBE; DeleteData SIXPAX@Z; Get / get QBE; GetReferenceCount QBEKXZ; IsNull QBE_NXZ;
//   ReleaseData QAEXXZ; Reset / reset QAEXPAV; ShareData QAEXPAV..PAVCCPPointerInfo@@@Z; Validate QBEXXZ (debug check);
//   member template ??$SetStaticCast (63). Not modelled: the free __fastcall templates ??$?8 / ??$?9 (operator== / !=
//   of two CCountedPointers, YI_NABV..0@Z) -- no landed includer compares two pointers.
// Bodies (retail bytes; no ICF in retail: 732 separate /O2 dtor COMDATs, one per T):
//   ~CCountedPointer = 0x006E7AB0 (MovieBase) / 0x00CE1000 (CEntityScriptBase) and the other 730: `if (Info) { dec [Info];
//     reload Info; cmp [Info],0; jne; ecx = Info->Data; call Info->DeleteFunc; operator delete(Info) }`, then Data = 0
//     and Info = 0 in that order. It is its own body, not a ReleaseData call (below).
//   ReleaseData: the same release, then Info = 0 and Data = 0 in THAT order. Retail never emits it out of line (no
//     release COMDAT with the Info-first tail exists); expanded at 0x00A45330 / 0x00A5F250 / 0x006633A0 (`mov [x+4],edi;
//     mov [x],edi`). Reset and ShareData are written over it (their dead zero stores vanish):
//   Reset(T*) = 0x008AB2B0 (and 197 more): <ReleaseData> leaves only `mov [esi+4],0`, then Data = data; if (data)
//     Info = new CCPPointerInfo(1, DeleteData, Data) (the block reads Data back; a failed new leaves Info 0).
//   ShareData = 0x00CE1040 (CCountedPointer<CScriptThing>, Ego_r.pdb name): `if (Info != info) { <release>; Data = data;
//     Info = info; if (info) ++info->RefCount; }`.
//   operator= = 0x00704580 (CScriptThing): loads o.Data / o.Info first, then the ShareData body, returns *this.
//   The (T*) ctor is the allocation without the release (out of line at 0x008C58F0 / 0x008C5980, `ret 4`).
//   Copy ctor: Data, Info copied, then `if (Info) ++Info->RefCount` (expanded in every CScriptThing copy 0x004ABE90).
//   DeleteData = `delete static_cast<T*>(data)`: 0x00CDEE00 (polymorphic T: `test ecx,ecx; je; mov eax,[ecx]; push 1;
//     call [eax]`), 0x00674E80 (CExpressionSet: the inline implicit dtor, then operator delete). Only its address is
//     stored (the count block); the landed DeleteData rows explicitly instantiate it.
// Model test (wave 9, 955 landed rows, logs/mc_cpvar_*.txt): this split (dtor own body, ReleaseData zeroing Info first)
//   gives 15 DIFFER; a dtor that calls a zeroing ReleaseData gives the same leaf bytes at more sites but names ReleaseData
//   where retail calls the Data-first dtor COMDAT, and `ReleaseData(); Data = 0; Info = 0;` gives 20 DIFFER.
// Inlining: every member is plain `inline` (in-class), never __forceinline / noinline. Retail calls the COMDAT copies at
//   some sites and expands them at others; VC7.1 RTM 3077 reproduces that per-site choice from these bodies on the landed
//   rows (wave 9 HEADERS.md lists the shapes it does not reproduce). unproven: the source order of Reset's two stores and
//   the bodies of GetReferenceCount / reset / Get / get / IsNull (FableWin names only, no retail out-of-line copy found).

class CCPPointerInfo {
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
};
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);

template <class T> class CCountedPointer {
public:
    T* Data;                                                             // +0x0
    CCPPointerInfo* Info;                                                // +0x4

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
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
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   // unproven body (FableWin name only)
    void Validate() const;                                               // FableWin.pdb only (debug build check)
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
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   // FableWin.pdb ??$SetStaticCast@..@?$CCountedPointer@..@@QAEXABV?$CCountedPointer@..@@@Z (63)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              // landed 0x00A32620 (C3DMeshFile::GetRoot) expands it
    }
    void reset(T* data) { Reset(data); }                                 // unproven body (FableWin name only)
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    // per-T deleter, address stored in the count block
};

class C2DLineF;
class CThingPhysical : public CThing {

private:
    unsigned char HealthOverTimeEffects[4];
protected:
    float Scale;
    CIntelligentPointer<CThingPhysical> PLastCreator;
    CIntelligentPointer<CThingPhysical> POriginalCreator;
    float MaxHealth;
    float Health;
    long CanComeBetweenCameraAndObservedThing;
    unsigned char Damageable:1, Killable:1, RespondToHits:1, EvadeAllHits:1,
        NegateAllHits:1, AbleToBeEngagedInCombat:1;
public:
    const C3DVector& PeekPos() const;
    float GetHealth() const { return Health; }
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysical)==0xc0);



class CHitParameters;
class CThingGameObject : public CThingPhysical {
protected:
    CIntelligentPointer<CThing> PThingStandingOn;
private:
    unsigned char AddToComboMultiplierOnHit:1,AddToComboMultiplierOnHitOverrideSet:1,
        GiveHeroStatChangesOnBeingHit:1,GiveHeroStatChangesOnBeingHitOverrideSet:1;
public:
    virtual void OnHit(CHitParameters&);
};
FABLE_STATIC_ASSERT(sizeof(CThingGameObject)==0xd0);



class CThingBuilding; class CThingCreatureDef; class CCreatureActionBase;
class CCreatureInteraction; class CTCCreatureModeManager;
class CHeroCombatDef;
class CThingCreatureBase : public CThingGameObject {


private:
    unsigned long LastMessageEventICreatedID;
    unsigned char CombatCollisionDebugGraphics[12];
protected:
    CDefPointer<const CThingCreatureDef> PDef;
    long ShotAccuracyPercentage;
    C3DVector InitialPos;
    CIntelligentPointer<CThingCreatureBase> PLastAttackedByCreature;
    unsigned long WFLastAttackedByCreature;
    CCountedPointer<CCreatureActionBase> PCurrentAction;
    std::list<CCountedPointer<CCreatureActionBase> > PQueuedActions;
    C3DVector MovementVector;
    C3DVector HeadPosOffset;
    long IdleCounter;
    float TurnSpeed;
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;
    CTCCreatureModeManager* PTCModeManager;
    unsigned char PreviousActionHandedness[4];
    long PreviousActionHandednessWF;
    unsigned char BodyReorienter[4];
    unsigned char CombatDebugGraphics[12];
    CIntelligentPointer<CThing> PItemToUnsheatheAfterCutscene;
    char* DebugText;
    bool CurrentlyFrameUpdating:1,Blinking:1,MeleeAttacker:1,MeleeAttacked:1,
        LeaveDeadBodyOverride:1,LeaveExperienceOrbsOverride:1,FollowHeroThroughTeleporters:1,HeadPosCached:1;
    bool UseMovementInActions:1,UpdateMovementVectorThisFrame:1,AbortingCurrentAction:1,DemonDoor:1,
        Oracle:1,HadFacingAngleYZSetByBodyOrientation:1,UnderInfluenceOfEpicSpell:1,HiddenOnMiniMap:1;
    bool IsShowingDebugInfo:1;
    void ReactToGenericStrike(const CHitParameters&);
public:
    bool AutoPlaceNearBuilding(const CThingBuilding*,float);
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PDef)==0xe0);
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PCurrentAction)==0x100);
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PQueuedActions)==0x108);
    }
    virtual void OnHit(CHitParameters&);
    bool SetCurrentAction(const CCreatureActionBase&);
};
FABLE_STATIC_ASSERT(sizeof(CThingCreatureBase)==0x160);
// Original sole base CThingCreatureBase; native Create006ac91c allocates208.
// Complete original tail ControlPos through MovementBandType is unused here.
class CThingPlayerCreature:public CThingCreatureBase {
private: unsigned char _pad_0x160[0xa8];
public: CThingPlayerCreature();
};
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreature)==0x208);
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCBase = 0xc.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in (naming wave 31, DECISIONS_W30 D5): define FABLE_RETAIL_CTCBASE before the include for the polymorphic retail
// class. RTTI .?AVCTCBase@@ : CBaseClassNonCopyable : CBaseClass (ghidra_out/rtti_bases.tsv); ctor 0x00686800 calls
// CBaseClassNonCopyable() and stores vtable 0x0125BE0C, ~CTCBase 0x00686830 stores it and tail-jumps ~CBaseClassNonCopyable
// 0x0099A300. Vtable 0x0125BE0C = 28 slots (Fable.exe; slot 28 is the string "End"): slot 0 scalar deleting dtor 0x00687050,
// slots 2-27 the CTCBase bodies 0x006867F0 .. 0x006869B0 in the Ego_r.pdb / FableWin.pdb definition order of the same names
// (each body's `ret` / `ret 4` / `ret 8` and `mov al,1` / `xor al,al` fits that signature), with the PDB editor virtuals
// (EditFrameUpdate, EditorDrawPreparePrimitivesForRendering, Get/SetFromPropertiesStruct) absent from retail. Slot 1 is the
// protected OnSerialise 0x00686960 (`ret 8`: the only two-argument CTCBase virtual; its body sits where both PDBs define it,
// between IsSerialisableForHeroState and IsComponentSerialisable). Slots 21 / 22 are _purecall 0x00BFEA08: GetInterfaceType /
// GetClassTypeName, the two pure virtuals every CTC override pair fills (chunk 695: CTCAssassinRush 0x0126CF2C slots 21 / 22;
// 259 / 323 FableWin.pdb overrides). Signatures are the PDB spellings. The flattened layout below stays the landed default.
#include <stddef.h>




class CThing;
class CEnginePrimitivePrepareInfo;class CWorld;
class CInterfaceType;
class CPersistContext;
class CTCParamsBase;
namespace NTCMessages { class CMessageBase; }
class CTCBase : public CBaseClassNonCopyable {
public:
protected:
    virtual void OnSerialise(CPersistContext &,bool);
public:
    virtual ~CTCBase();
public:
    virtual void OnRemove();
public:
    virtual void ConstructFromParams(const CTCParamsBase &);
public:
    virtual void OnCreate();
public:
    virtual void InitialActivate();
public:
    virtual void OnInitialActivate();
public:
    virtual void RenderUpdate(float);
public:
    virtual bool HasFrameUpdate() const;
public:
    virtual void FrameUpdate();
public:
    virtual void BulletTimeFrameUpdate();
public:
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &);
public:
    virtual void Copy(const CTCBase &);
public:
    virtual void OnDie();
public:
    virtual void OnKill();
public:
    virtual void OnLevelUnload();
public:
    virtual void OnSetInLimbo(bool);
public:
    virtual bool Validate(CCharString &) const;
public:
    virtual bool IsFreeForDeletion() const;
public:
    virtual bool IsSerialiseCreatable() const;
public:
    virtual bool IsSerialisableForHeroState() const;
public:
    virtual ETCInterfaceType GetInterfaceType() const=0;
public:
    virtual CCharString GetClassTypeName() const=0;
public:
    virtual bool IsComponentSerialisable() const;
public:
    virtual bool IsDriver() const;
public:
    virtual bool IsCopyable() const;
public:
    virtual void GlobalUpdate(bool);
public:
    virtual void HandleMessage(const NTCMessages::CMessageBase &);
protected:
protected:
    CThing& ParentThing;
private:
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
FABLE_STATIC_ASSERT(sizeof(CTCBase)==12);


class CThingSoundEmitter;
// Original complete36-byte class and native component16 receiver.
class CTCSoundPlayer : public CTCBase {
public:
    unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long);
protected:
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;
    CCharString VoiceName,ExtraTag;
    short AnimCriteriaClipDistance;
    long LastMaterialIndex;
};
FABLE_STATIC_ASSERT(sizeof(CTCSoundPlayer)==36);

// Ego_r original class 0x9ebe: public float X+0,Y+4, sizeof8.
// Native CTCPhysicsBase::GetOffsetVector slot28 at006b13d0 writes both words.
// Retail opt-in (Codex pass11): original lib_coord.hpp100..103,314..322;
// user copy ctor and arithmetic confirmed by UI hidden-result call sites.
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};


namespace NInventory {
// External API projection; native RTTI places the next base at304.
class CTCInventoryBase : public CTCBase { unsigned char _pad_0x0c[292]; };
}
namespace NHeroInformationScreens {
// Original first pure virtual and four-byte secondary receiver, never instantiated.
class CBase { public:virtual CWideString GetScreenName()const=0; };
}
// Original classV owner, used only as a nested-type declaration projection.
// No outer instance, member offset or sizeof(CTCInventoryMap) is modeled here.
class CTCInventoryMap
 : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase
{
public:
 // External member-call projection only: no claim about outer sizeof or tail.
 void AddLogBookSaveInfo(const CWideString&,const CWideString&,const CWideString&,CHeroLogBook::CHeroLogBookEntry::ECategory);
 // Layout: original TPI2aa7e and native reset/caller stride prove all28bytes.
 class CUIMiniMapMarker {
 public:
  long Region;CCharString UIDefName;C2DVector Position;
  long DefNameLength;long GraphicIndex;float Angle;
  CUIMiniMapMarker():Region(0),UIDefName(""),Position(0.0f,0.0f),DefNameLength(0),GraphicIndex(0),Angle(0.0f){}
 };
};
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap::CUIMiniMapMarker)==28);

// Ego_r TPI0xb5fe; retail vtable0129b874.
class CFailedAllocationHandler {
public:
    virtual ~CFailedAllocationHandler();
    virtual bool HandleFailedAllocation(unsigned long)=0;
};
FABLE_STATIC_ASSERT(sizeof(CFailedAllocationHandler)==4);

// Ego_r TPI0xb181; original base and complete native members, size3c.
// Native vtable0129c808 and CDefinitionManager base destructor prove inheritance.
class CResourceList : public CFailedAllocationHandler {
public:
    virtual ~CResourceList();
    virtual bool HandleFailedAllocation(unsigned long);
private:
    CResource Head;
    unsigned long ResourceCount;
    long AllocatedMemory,MaximumMemory;
    unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;
};
FABLE_STATIC_ASSERT(sizeof(CResourceList)==0x3c);

// Original Ego_r TPI0x9fbd: public CResourceList base; retail009b02e0
// tears down complete native0xd0 object, with 12-byte STLport containers.
class CDefClassBase;
class CDefinitionManager : public CResourceList {
    friend class CDefClassBase;
public:
    virtual ~CDefinitionManager();
    long GetGlobalIndexFromClassIndex(const CCharString&,long) const;
    template<class T> bool GetDef(long,CDefPointer<const T>*) const;
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long) const;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long);
    unsigned char PDefTypeDebugInfoMap[4];
    unsigned char SymbolPathList[12],CompilePathList[12];
    unsigned char PSymbolMap[8],PDefinitionSymbolMap[8];
    unsigned char NameCRCToIndexMap[16],DefClassInfo[16];
    unsigned char CompressedDefData[12],CompressedOffsets[16],InstantiatedDefs[12];
    unsigned char ErrorCallback[4],CurrentFilename[4],CompiledFileName[4];
    unsigned long DependencyCheckCRC;
    bool UseSafeBinaryFormat;
    unsigned char _pad_0xc1[3];
    unsigned char RecompiledDefs[12];
};
FABLE_STATIC_ASSERT(sizeof(CDefinitionManager)==0xd0);
// Original class V and public CDefinitionManager base: both PDB name sets,
// native RTTI. Native ctor0044c6c2 constructs that base and zeroes d0..dc.
// Both donor TPI sets retain only a forward record for this class; therefore
// the extra native storage is deliberately unnamed until its fields are proven.
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();
    CGameDefinitionManager();
    virtual ~CGameDefinitionManager();
    virtual void InitAndCompile();
    template<class T> bool GetDef(long,CDefPointer<T>&) const;
private:
    unsigned char _pad_0xd0[16];
};
FABLE_STATIC_ASSERT(sizeof(CGameDefinitionManager)==0xe0);
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CParentDefClassBase = 0x3d.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CParentDefClassBase = 0x39.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in (2026-10-30, class-model migration wave 2): define FABLE_RETAIL_DEF_CLASS_BASE for the polymorphic retail
// class. RTTI .?AVCParentDefClassBase@@ : CDefClassBase (ghidra_out/rtti_bases.tsv); vtable 0x01230CBC, 25 slots: slot 0
// scalar deleting dtor 0x004303F0, overrides at slots 10-16, 21, 22 and 24 (addresses per line; spellings are the Ego_r.pdb
// and FableWin.pdb names, identical in both), slots 1-9, 17, 23 inherited, 18-20 _purecall 0x00BFEA08.
// Layout (retail bytes): GetInstantiationName 0x004303B0 reads [ecx+0x25], SetInstantiationName 0x004303C0 writes it,
// GetPSubDefInfoMap 0x004303E0 is `lea eax,[ecx+0x29]`, the ctor 0x00430370 stores CDefString -1 at +0x25 and zeroes the
// CVectorMap at +0x29/+0x2D/+0x31 and its Dirty byte at +0x36: 0x39 bytes, packed like its base (Ego_r.pdb 0x3D carries a
// Dinkumware 0x14 vector). Out-of-line members: ctor 0x00430370 and ~CParentDefClassBase 0x00430300 (one TU: the deleting
// dtor 0x004303F0 auto-inlines the dtor body; derived deleting dtors call 0x00430300), copy ctor 0x0046EE98 and operator=
// 0x00431ED0 (both PDBs; bound in rebuild/oracles/reloc_bindings.tsv).
// Copy members (class-model migration wave 3 experiment, that wave's LESSONS.md item 1): they
// stay declared out of line. Undeclared (implicit) members over typed CDefString / CVectorMap models reproduce the 39 /Os
// callers of 0x00431ED0 tested and the standalone 0x00431ED0 body, but at /O2 RTM 3077 then inlines operator= into the
// callers retail makes call it (0x00434370 CEngineDef, 0x00625350) and still calls it from 0x00432E60 CFrontEndDef, which
// The 5 /O2 expanders (0x00432C70, 0x00432E60, 0x00434450, 0x004345A0, 0x0067C980) keep row-local __forceinline models.
// Members: CDefString / CVectorMap have no shared header. An includer that has the complete types (CDefString and
// CVectorMap<K,V,C> defined before the include) defines FABLE_PDCB_TYPED_MEMBERS for the typed members; otherwise the two
// members are opaque with the same offsets.
#include <stddef.h>
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDefClassBase = 0x25.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in: define FABLE_RETAIL_DEF_CLASS_BASE for the polymorphic retail class. RTTI: CDefClassBase :
// CDefPointeeBase : CResource : CIVCountedPointeeBase. Vtable 0x01230BCC, 25 slots; slots 18-20 are _purecall 0x00BFEA08
// and are named from the derived overrides (CFireHeartPatternDef vtable 0x012CCC50: Transfer 0x00D35070, Copy 0x00D34FB0,
// GetSizeofClass 0x00D34EF0). Signatures are the Ego_r.pdb / FableWin.pdb spellings. Size 0x25, packed (corrected
// 2026-10-29 from 0x28): the class is declared under #pragma pack(1) (Ego_r.pdb 0x25), so a derived def's first member
// sits at +0x25 when it is a byte (landed CDegradableDef::operator= 0x00454434: Degradable bool +0x25, GraphicType +0x28)
// and at +0x28 when it is 4-aligned (CFireHeartPatternDef: _Construct 0x00D34EC0 copies its first member at +0x28).
// Slots 14/15 (0x0042DA60 / 0x0042DA50) are the two GetPSubDefInfo overloads; their bodies are identical
// (`xor eax,eax; ret 4`), but slot 14 is the const one: the landed const GetSubDef<T> rows (e.g. 0x008C79B0) call [vptr+0x38]
// from a const member, and the CParentDefClassBase overrides (0x009AD4A0 / 0x009AD510) are bound UBE / UAE. Declared
// non-const first, VC7.1 emits the const overload first; a probe build reproduces all 25 retail slot names (HEADERS.md).
// Non-virtual members (2026-10-29 migration wave 1): operator= 0x00431F10 (catalog + Ego_r ??4CDefClassBase@@QAEAAV0@ABV0@@Z),
// the GetSubDef member template (Ego_r spelling), CSubDefInfo {DefIndex, OriginalParentDefIndex} (Ego_r layout, 8 bytes) and
// the FableWin.pdb accessors AreDefaultValsApplied / SetDefaultValsApplied (Ego_r inlines them away: inline bodies here).
// ~CDefClassBase is inline: only FableWin.pdb (debug) has ??1CDefClassBase@@UAE@XZ, and the retail scalar deleting dtor
// 0x0042DAB0 stores the CDefPointeeBase vtable 0x01230BA0 and calls ~CResource 0x009FC550 (landed vector<CArenaRoundDef>
// ::_M_fill_insert 0x00F34D70 destroys its by-value copy with that inline sequence). FableWin.pdb also has a private
// default ctor ??0CDefClassBase@@AAE@XZ (derived defs reach it through friendship); it is not declared here yet.
#include <stddef.h>

class CCharString;
class CDefString;
class CDefinitionManager;
class CPersistContext;
class CDataInputStream;
class CDataOutputStream;
class CValidateDefinitionInitBase;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <class T> class CDefPointer;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    class CSubDefInfo {                                                  // Ego_r.pdb CSubDefInfo, 8 bytes
    public:
        long DefIndex;                                                   // +0x00
        long OriginalParentDefIndex;                                     // +0x04
    };
    CDefClassBase(const CDefClassBase&);                                 // 0x0046644C
    CDefClassBase& operator=(const CDefClassBase&);                      // 0x00431F10 (Ego_r ??4CDefClassBase@@QAEAAV0@ABV0@@Z; out of line: an implicit one inlines the 0x00431F60 base copy, retail calls it)
    template <class T> bool GetSubDef(CDefPointer<const T>*) const;      // Ego_r ??$GetSubDef@V..@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBV..@@@@@Z
    bool AreDefaultValsApplied() const { return DefaultValsApplied; }    // FableWin.pdb ?AreDefaultValsApplied@CDefClassBase@@QBE_NXZ
    void SetDefaultValsApplied(bool value) { DefaultValsApplied = value; }   // FableWin.pdb ?SetDefaultValsApplied@CDefClassBase@@QAEX_N@Z
    virtual ~CDefClassBase() { }                                         // slot 0  0x0042DAB0 (scalar deleting; the dtor is inline)
    virtual bool UnloadResource();                                       // slot 2  0x009ACE90
    virtual CDefString GetInstantiationName() const;                     // slot 10 0x0042DA10
    virtual void SetInstantiationName(CDefString);                       // slot 11 0x0042DA20
    virtual void AddSubDef(const CCharString&, long, long);              // slot 12 0x0042DA30
    virtual void RemoveSubDef(const CCharString&);                       // slot 13 0x0042DA40
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             // slot 15 0x0042DA50 (declared first: VC7.1 reverses overloads)
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; // slot 14 0x0042DA60
    virtual bool IsParentDef() const;                                    // slot 16 0x0042DA70
    virtual void PostTransfer();                                         // slot 17 0x0042DA80
    virtual void Transfer(CPersistContext&) = 0;                         // slot 18
    virtual void Copy(const CDefClassBase*) = 0;                         // slot 19
    virtual unsigned long GetSizeofClass() = 0;                          // slot 20
    virtual void InternalSaveBinary(CDataOutputStream&);                 // slot 21 0x009ACAB0
    virtual void InternalLoadBinary(CDataInputStream&);                  // slot 22 0x009ACB20
    virtual void Validate(const CValidateDefinitionInitBase&, const CCharString&) const;  // slot 23 0x0042DA90
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             // slot 24 0x0042DAA0
    CDefinitionManager* PDefManager;                                     // +0x1c
    unsigned long       GlobalIndex;                                     // +0x20
    unsigned char       Setup : 1;                                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CDefClassBase) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, PDefManager) == 0x1c);

class CDataInputStream;
class CDataOutputStream;
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CParentDefClassBase();                                               // 0x00430370 (??0CParentDefClassBase@@QAE@XZ)
    CParentDefClassBase(const CParentDefClassBase&);                     // 0x0046EE98 (??0CParentDefClassBase@@QAE@ABV0@@Z)
    CParentDefClassBase& operator=(const CParentDefClassBase&);          // 0x00431ED0 (??4CParentDefClassBase@@QAEAAV0@ABV0@@Z)
    virtual ~CParentDefClassBase();                                      // slot 0  0x004303F0 (scalar deleting); ~ 0x00430300
    virtual CDefString GetInstantiationName() const;                     // slot 10 0x004303B0
    virtual void SetInstantiationName(CDefString);                       // slot 11 0x004303C0
    virtual void AddSubDef(const CCharString&, long, long);              // slot 12 0x009B00D0
    virtual void RemoveSubDef(const CCharString&);                       // slot 13 0x009B0170
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             // slot 15 0x009AD510 (declared first: VC7.1 reverses overloads)
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; // slot 14 0x009AD4A0
    virtual bool IsParentDef() const;                                    // slot 16 0x004303D0 (`mov al,1; ret`)
    virtual void InternalSaveBinary(CDataOutputStream&);                 // slot 21 0x009B0200
    virtual void InternalLoadBinary(CDataInputStream&);                  // slot 22 0x009AED70
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             // slot 24 0x004303E0
    long          InstantiationName_TablePos;                            // +0x25 CDefString InstantiationName (TablePos)
    unsigned char SubDefInfoMap[0x10];                                   // +0x29 CVectorMap<unsigned long,CSubDefInfo,CKeyPairCompareLess<..> >
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase) == 0x39);




// Original complete named fields; native parent occupies39bytes, aligned to3c.
// Map storage is the genuine12-byte STLport representation.
namespace NUISystem {
class CUIMiscThingsDef : public CParentDefClassBase {
public:
    CWideString SpaceSeparator;
    CWideString CommaSeparator;
    CWideString ColonSeparator;
    CWideString NewLineSeparator;
    CWideString OpenBracket;
    CWideString CloseBracket;
    CWideString Positive;
    CWideString WeaponValueString;
    CWideString WeaponAugString;
    CWideString WeaponAugNone;
    CWideString WeaponWeightString;
    CWideString WeaponLightString;
    CWideString WeaponHeavyString;
    CWideString WeaponKillsString;
    CWideString WeaponCatMeleeString;
    CWideString WeaponCatRangedString;
    CWideString WeaponDamageString;
    CWideString TradeCostString;
    CWideString TradeProfitString;
    CWideString TradeLossString;
    CWideString TradeAlreadyOwnsString;
    CWideString TradeNumberInStockString;
    CWideString TradeDeliveryString;
    CWideString TradeNoDeliveryString;
    CWideString TradeDaysString;
    CWideString TradeBuyString;
    CWideString TradeSellString;
    CWideString TradeWantedString;
    CWideString QuestFailedString;
    CWideString FailedString;
    CWideString SucceededString;
    CWideString Plus;
    CWideString Minus;
    unsigned long CoreGraphic;
    unsigned long VignetteGraphic;
    unsigned long OptionalGraphic;
    unsigned long FeatGraphic;
    CWideString ObjectsRewardString;
    CWideString NoneString;
    CWideString CheckGuildString;
    CWideString QuestStartingString;
    C2DVector RingCenter;
    C2DVector PCRingCenter;
    C2DVector WorldMapOffset;
    float WorldMapWidth;
    float WorldMapHeight;
    CWideString YouString;
    CWideString OwnString;
    CWideString NoString;
    CWideString HousesString;
    CWideString HouseString;
    CWideString InString;
    CWideString ShopsString;
    CWideString ShopString;
    CWideString ThereString;
    CWideString AreString;
    CWideString IsString;
    CWideString ForString;
    CWideString SaleString;
    CWideString GeneralString;
    CWideString TitleString;
    CWideString BarberString;
    CWideString TatooString;
    CWideString LevelString;
    unsigned long TotalSpellsInPalettes;
    unsigned long TotalSpellsInContainer;
    CWideString LogBookBasicsCategoryString;
    CWideString LogBookObjectsCategoryString;
    CWideString LogBookTownsCategoryString;
    CWideString LogBookHeroCategoryString;
    CWideString LogBookCombatCategoryString;
    CWideString LogBookQuestCategoryString;
    CWideString LogBookStoryCategoryString;
    CWideString LogBookBasicsCategoryNameString;
    CWideString LogBookObjectsCategoryNameString;
    CWideString LogBookTownsCategoryNameString;
    CWideString LogBookHeroCategoryNameString;
    CWideString LogBookCombatCategoryNameString;
    CWideString LogBookQuestCategoryNameString;
    CWideString LogBookStoryCategoryNameString;
    unsigned char MapPaths[12]; // std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > >
    CCharString SoundUpDown;
    CCharString SoundSlider;
    CCharString SoundBack;
    CCharString SoundForward;
    CCharString SoundError;
    CCharString SoundExit;
    CCharString CountUpSound;
    CCharString SoundKeyboardUp;
    CCharString SoundKeyboardDown;
    CCharString SoundKeyboardLeft;
    CCharString SoundKeyboardRight;
    CCharString SoundKeyboardEnterCharacter;
    CCharString SoundKeyboardDeleteCharacter;
    CCharString SoundKeyboardDone;
    CWideString FrontEndMusic;
    C2DVector HeroDollTL;
    C2DVector HeroDollBR;
    float HeroDollSphereRadius;
    C2DVector HeroDollTL_PC;
    C2DVector HeroDollBR_PC;
    float HeroDollSphereRadius_PC;
    C2DVector HeroDollFrameTL_PC;
    float HeroDollFrameEmulateListOffset;
    unsigned char QuestStartScreenMusic[4]; // Complete original EMusicSetType storage.
    unsigned char QuestCompleteScreenMusic[4]; // Complete original EMusicSetType storage.
    unsigned char QuestFailureScreenMusic[4]; // Complete original EMusicSetType storage.
    unsigned char DeathScreenMusic[4]; // Complete original EMusicSetType storage.
    float DigitCountTime;
    unsigned long SaveHeroGraphicIndex;
    unsigned char MiniMapGraphics[12]; // std::map<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString const ,long> > >
    long KeyboardSmallKeyGraphic;
    long KeyboardLargeKeyGraphic;
    unsigned long TotalAssignableThings;
    float TimeInSecsForFade;
    float BackBufferFilterSaturation;
    float BackBufferFilterContrast;
    float BackBufferFilterBrightness;
    float BackBufferFilterTintR;
    float BackBufferFilterTintG;
    float BackBufferFilterTintB;
    float BackBufferFilterTintScale;
    float BackBufferDiffuseScale;
    float BackBufferAmbientScale;
    float MinimumFilterColor;
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CUIMiscThingsDef)==0x260);

#pragma auto_inline(off)
namespace std {
// Original STLport4.6.2 _tree.c345..421; native47c500/47c720.
template<class K,class V,class KeyOfValue,class Compare,class A>
typename _Rb_tree<K,V,KeyOfValue,Compare,A>::iterator
_Rb_tree<K,V,KeyOfValue,Compare,A>::_M_insert(_Rb_tree_node_base* x0,_Rb_tree_node_base* y0,const V& value,_Rb_tree_node_base* w0) {
    _Link_type w=(_Link_type)w0,x=(_Link_type)x0,y=(_Link_type)y0,z;
    if(y==this->_M_header._M_data || (w==0 && (x!=0 || _M_key_compare(KeyOfValue()(value),_S_key(y))))) {
        z=_M_create_node(value);_S_left(y)=z;
        if(y==this->_M_header._M_data){_M_root()=z;_M_rightmost()=z;}
        else if(y==_M_leftmost())_M_leftmost()=z;
    } else {
        z=_M_create_node(value);_S_right(y)=z;
        if(y==_M_rightmost())_M_rightmost()=z;
    }
    _S_parent(z)=y;_S_left(z)=0;_S_right(z)=0;
    _Rb_global_inst::_Rebalance(z,this->_M_header._M_data->_M_parent);++_M_node_count;
    return iterator(z);
}
template<class K,class V,class KeyOfValue,class Compare,class A>
pair<_Rb_tree_iterator<V,_Nonconst_traits<V> >,bool>
_Rb_tree<K,V,KeyOfValue,Compare,A>::insert_unique(const V& value) {
    _Link_type y=this->_M_header._M_data,x=_M_root();bool comp=true;
    while(x!=0){y=x;comp=_M_key_compare(KeyOfValue()(value),_S_key(x));x=comp?_S_left(x):_S_right(x);}
    iterator j=iterator(y);
    if(comp) {if(j==begin())return pair<iterator,bool>(_M_insert(y,y,value),true);else --j;}
    if(_M_key_compare(_S_key(j._M_node),KeyOfValue()(value)))return pair<iterator,bool>(_M_insert(x,y,value),true);
    return pair<iterator,bool>(j,false);
}
}
// Original UnassignString body is inlined by native99b510; NoStrings decrements at99b54c.
CWideString::~CWideString(){UnassignString();--NoStrings;}
template<class K,class V,class C,class A> V& std::map<K,V,C,A>::operator[](const K& key) {
    iterator i=lower_bound(key);
    if(i==end() || key_comp()(key,i->first))i=insert(i,value_type(key,V()));
    return i->second;
}
template<class I> __declspec(noinline) void __fastcall std::__destroy_aux(I first,I last,const std::__false_type&) {
    for(;first!=last;++first)std::_Destroy(&*first);
}
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* result)const {
    if(index<=0)return false;
    CDefPointer<const CDefClassBase> definition=GetPDefFromGlobalIndex(index);
    if(!definition.IsNull()) { result->SetDynamicCast(definition);return true; }
    return false;
}
template<class T,class A> std::vector<T,A>::vector(const std::vector<T,A>& other)
 : std::_Vector_base<T,A>(other.size(),other.get_allocator()) {
    this->_M_finish=std::__uninitialized_copy(other.begin(),other.end(),this->_M_start,std::__false_type());
}
template<class C,class I> int __cdecl std::__char_traits_base<C,I>::compare(const C* first,const C* second,size_t count) {
    for(size_t i=0;i<count;++i)if(first[i]!=second[i])return first[i]<second[i]?-1:1;
    return 0;
}
#pragma auto_inline(on)

void CHeroLogBook::AddEntry(const CWideString& name_tag,const CWideString& abbreviated_name,const CWideString& text_tag,CHeroLogBookEntry::ECategory log_category,bool add_to_map)
{
    typedef std::vector<CHeroLogBookEntry> EntryVector;
    typedef std::map<CHeroLogBookEntry::ECategory,EntryVector> EntryMap;
    for(EntryMap::iterator category=LogEntries.begin();category!=LogEntries.end();++category)
        for(EntryVector::iterator entry=category->second.begin();entry!=category->second.end();++entry)
            if(entry->GetName()==name_tag)return;
    CThingPlayerCreature* creature=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if(creature && !creature->IsDead() && creature->HasTC(TCI_SOUND_PLAYER)) {
        CTCSoundPlayer& sound_player=static_cast<CTCSoundPlayer&>(creature->GetTC(TCI_SOUND_PLAYER));
        sound_player.PlayAttachedCriteria("LOG_ENTRY",false,true,false,-1);
    }
    if(add_to_map && creature->HasTC(TCI_INVENTORY_MAP)) {
        CTCInventoryMap* inventory_map=static_cast<CTCInventoryMap*>(&creature->GetTC(TCI_INVENTORY_MAP));
        if(inventory_map)inventory_map->AddLogBookSaveInfo(name_tag,abbreviated_name,text_tag,log_category);
    }
    CDefPointer<const NUISystem::CUIMiscThingsDef> misc_def;
    long index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF",1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index,&misc_def);
    CHeroLogBookEntry::ECategory category_from_tag=log_category==CHeroLogBookEntry::CATEGORY_TUTORIAL?CHeroLogBookEntry::CATEGORY_BASICS:log_category;
    if(text_tag.Contains(misc_def->LogBookBasicsCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_BASICS;
    if(text_tag.Contains(misc_def->LogBookObjectsCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_OBJECTS;
    if(text_tag.Contains(misc_def->LogBookTownsCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_TOWNS;
    if(text_tag.Contains(misc_def->LogBookHeroCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_HERO;
    if(text_tag.Contains(misc_def->LogBookCombatCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_COMBAT;
    if(text_tag.Contains(misc_def->LogBookQuestCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_QUEST;
    if(text_tag.Contains(misc_def->LogBookStoryCategoryString))category_from_tag=CHeroLogBookEntry::CATEGORY_STORY;
    CHeroLogBookEntry new_entry(name_tag,abbreviated_name,text_tag);
    EntryMap::iterator existing=LogEntries.find(category_from_tag);
    if(existing!=LogEntries.end())existing->second.push_back(new_entry);
    else {
        EntryVector new_vector;
        EntryMap::value_type new_value(category_from_tag,new_vector);
        LogEntries.insert(new_value);
        LogEntries[category_from_tag].push_back(new_entry);
    }
}
namespace std {
template <class _Dummy> void __cdecl
_Rb_global<_Dummy>::_Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_right;
  __x->_M_right = __y->_M_left;
  if (__y->_M_left !=0)
    __y->_M_left->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_left)
    __x->_M_parent->_M_left = __y;
  else
    __x->_M_parent->_M_right = __y;
  __y->_M_left = __x;
  __x->_M_parent = __y;
}

template <class _Dummy> void __cdecl 
_Rb_global<_Dummy>::_Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_left;
  __x->_M_left = __y->_M_right;
  if (__y->_M_right != 0)
    __y->_M_right->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_right)
    __x->_M_parent->_M_right = __y;
  else
    __x->_M_parent->_M_left = __y;
  __y->_M_right = __x;
  __x->_M_parent = __y;
}

template <class _Dummy> void __cdecl
_Rb_global<_Dummy>::_Rebalance(_Rb_tree_node_base* __x, 
			       _Rb_tree_node_base*& __root)
{
  __x->_M_color = false;
  while (__x != __root && __x->_M_parent->_M_color == false) {
    if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
      if (__y && __y->_M_color == false) {
        __x->_M_parent->_M_color = true;
        __y->_M_color = true;
        __x->_M_parent->_M_parent->_M_color = false;
        __x = __x->_M_parent->_M_parent;
      }
      else {
        if (__x == __x->_M_parent->_M_right) {
          __x = __x->_M_parent;
          _Rotate_left(__x, __root);
        }
        __x->_M_parent->_M_color = true;
        __x->_M_parent->_M_parent->_M_color = false;
        _Rotate_right(__x->_M_parent->_M_parent, __root);
      }
    }
    else {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
      if (__y && __y->_M_color == false) {
        __x->_M_parent->_M_color = true;
        __y->_M_color = true;
        __x->_M_parent->_M_parent->_M_color = false;
        __x = __x->_M_parent->_M_parent;
      }
      else {
        if (__x == __x->_M_parent->_M_left) {
          __x = __x->_M_parent;
          _Rotate_right(__x, __root);
        }
        __x->_M_parent->_M_color = true;
        __x->_M_parent->_M_parent->_M_color = false;
        _Rotate_left(__x->_M_parent->_M_parent, __root);
      }
    }
  }
  __root->_M_color = true;
}


}

namespace std { void __cdecl __stl_throw_out_of_range(const char*){assert(false);abort();} }
static int live_chars,sound_calls,map_calls,def_calls,player_calls;
long CWideString::NoStrings=0;
static const char* chars(const CCharString& value){return value.PStringData?(const char*)value.PStringData:"";}
CCharString::CCharString(const char* text,long length):PStringData(0){
 if(length<0)length=(long)strlen(text);++live_chars;
 if(length){char* copy=(char*)malloc(length+1);memcpy(copy,text,length);copy[length]=0;PStringData=(CCharStringData*)copy;}
}
CCharString::~CCharString(){if(PStringData)free(PStringData);--live_chars;}
CWideString::CWideString(const CWideString& other):PStringData(other.PStringData){if(PStringData)++PStringData->NoRefs;++NoStrings;}
static bool wide_equal(const CWideString& value,const char* expected){
 if(!value.PStringData)return expected[0]==0;
 const unsigned short* p=value.PStringData->Data.data();size_t n=value.PStringData->Data.size();
 if(n!=strlen(expected))return false;for(size_t i=0;i<n;++i)if(p[i]!=(unsigned char)expected[i])return false;return true;
}
bool CWideString::Contains(const CWideString& needle)const{
 const unsigned short* p=PStringData?PStringData->Data.data():0;size_t n=PStringData?PStringData->Data.size():0;
 const unsigned short* q=needle.PStringData?needle.PStringData->Data.data():0;size_t m=needle.PStringData?needle.PStringData->Data.size():0;
 if(!m)return true;if(m>n)return false;
 for(size_t i=0;i+m<=n;++i){size_t j=0;while(j<m&&p[i+j]==q[j])++j;if(j==m)return true;}return false;
}
union WideMemory {void* align;unsigned char bytes[sizeof(CWideString)];};
static WideMemory zero_wide;
struct FixtureWide:public CWideString {
 FixtureWide(const char* text,bool backing=true):CWideString(*(CWideString*)zero_wide.bytes){
  if(backing){PStringData=new CWideStringData;PStringData->NoRefs=1;size_t n=strlen(text);free(PStringData->Data._M_start);PStringData->Data._M_start=(unsigned short*)malloc((n+1)*sizeof(unsigned short));PStringData->Data._M_finish=PStringData->Data._M_start+n;PStringData->Data._M_end_of_storage._M_data=PStringData->Data._M_finish+1;for(size_t i=0;i<n;++i)PStringData->Data._M_start[i]=(unsigned char)text[i];*PStringData->Data._M_finish=0;}
 }
};
static CPlayer* expected_player;static CPlayerManager* expected_player_manager;static CThingPlayerCreature* expected_creature;
static CGameDefinitionManager* expected_manager;static NUISystem::CUIMiscThingsDef* expected_def;
static CTCSoundPlayer* expected_sound;static CTCInventoryMap* expected_map;
static CHeroLogBook::CHeroLogBookEntry::ECategory expected_category;
static const CWideString *expected_name,*expected_abbreviation,*expected_content;
CMainGameComponent* CMainGameComponent::PMainGameComponent=0;
CPlayer& CPlayerManager::GetMainPlayer(){assert(this==expected_player_manager);++player_calls;return *expected_player;}
CThingPlayerCreature* CPlayer::GetPControlledCreature()const{assert(this==expected_player);return expected_creature;}
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get(){return *expected_manager;}
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& name,long index)const{assert(this==expected_manager&&strcmp(chars(name),"UI_MISC_THINGS_DEF")==0&&index==1);++def_calls;return 17;}
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)const{assert(this==expected_manager&&index==17);CDefPointer<const CDefClassBase> result;result.Object=expected_def;result.Object->AddRef();return result;}
unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& name,bool a,bool b,bool c,long index){assert(this==expected_sound&&strcmp(chars(name),"LOG_ENTRY")==0&&!a&&b&&!c&&index==-1);++sound_calls;return 23;}
void CTCInventoryMap::AddLogBookSaveInfo(const CWideString& name,const CWideString& abbreviation,const CWideString& content,CHeroLogBook::CHeroLogBookEntry::ECategory category){assert(this==expected_map&&&name==expected_name&&&abbreviation==expected_abbreviation&&&content==expected_content&&category==expected_category);++map_calls;}
template<class K,class V,class O,class C,class A> typename std::_Rb_tree<K,V,O,C,A>::iterator std::_Rb_tree<K,V,O,C,A>::insert_unique(iterator,const V& value){return insert_unique(value).first;}
typedef CHeroLogBook::CHeroLogBookEntry Entry;typedef Entry::ECategory Category;typedef std::vector<Entry> Entries;typedef std::map<Category,Entries> EntryMap;
template<class T> union Memory{double alignment;void* pointer;unsigned char bytes[sizeof(T)];};
static void init_map(EntryMap& map){typedef EntryMap::value_type Value;std::_Rb_tree_node<Value>* h=(std::_Rb_tree_node<Value>*)malloc(sizeof(std::_Rb_tree_node<Value>));h->_M_parent=0;h->_M_left=h->_M_right=h;h->_M_color=false;map._M_t._M_header._M_data=h;map._M_t._M_node_count=0;}
static void clear_node(std::_Rb_tree_node_base* p){if(!p)return;clear_node(p->_M_left);clear_node(p->_M_right);typedef EntryMap::value_type Value;std::_Rb_tree_node<Value>* node=static_cast<std::_Rb_tree_node<Value>*>(p);node->_M_value_field.~Value();free(node);}
static size_t entry_count(EntryMap& map){size_t n=0;for(EntryMap::iterator i=map.begin();i!=map.end();++i)n+=i->second.size();return n;}
static void add_existing(CHeroLogBook& book,Category category,const CWideString& name){Entries entries;Entry entry(name,name,name);entries.push_back(entry);book.LogEntries.insert(EntryMap::value_type(category,entries));}
static void run_case(int mode,int input_category,int mask,int duplicate,int growth){
 const char* tags[]={"[B]","[O]","[T]","[H]","[C]","[Q]","[S]"};char text[96]="content";
 for(int i=0;i<7;++i)if(mask&(1<<i))strcat(text,tags[i]);
 FixtureWide name("new-entry"),abbreviation("short"),content(text),oldname(duplicate==1?"new-entry":"old-entry"),nullname("",false),emptyname("");
 Memory<CHeroLogBook> book_memory;memset(&book_memory,0,sizeof(book_memory));CHeroLogBook& book=*(CHeroLogBook*)book_memory.bytes;init_map(book.LogEntries);
 Memory<CMainGameComponent> component_memory;memset(&component_memory,0,sizeof(component_memory));CMainGameComponent& component=*(CMainGameComponent*)component_memory.bytes;
 Memory<CPlayerManager> player_manager_memory;Memory<CPlayer> player_memory;Memory<CThingPlayerCreature> creature_memory;Memory<CGameDefinitionManager> manager_memory;Memory<NUISystem::CUIMiscThingsDef> def_memory;Memory<CTCSoundPlayer> sound_memory;Memory<CTCInventoryMap> map_memory;
 memset(&creature_memory,0,sizeof(creature_memory));memset(&def_memory,0,sizeof(def_memory));
 expected_player_manager=(CPlayerManager*)player_manager_memory.bytes;expected_player=(CPlayer*)player_memory.bytes;expected_creature=mode==0?0:(CThingPlayerCreature*)creature_memory.bytes;expected_manager=(CGameDefinitionManager*)manager_memory.bytes;expected_def=(NUISystem::CUIMiscThingsDef*)def_memory.bytes;expected_sound=(CTCSoundPlayer*)sound_memory.bytes;expected_map=(CTCInventoryMap*)map_memory.bytes;
 component.PPlayerManager.ptr=expected_player_manager;CMainGameComponent::PMainGameComponent=&component;
 expected_def->IVRefCount=1;
 CWideString* categories[]={&expected_def->LogBookBasicsCategoryString,&expected_def->LogBookObjectsCategoryString,&expected_def->LogBookTownsCategoryString,&expected_def->LogBookHeroCategoryString,&expected_def->LogBookCombatCategoryString,&expected_def->LogBookQuestCategoryString,&expected_def->LogBookStoryCategoryString};
 for(int j=0;j<7;++j){FixtureWide tag(tags[j]);new(categories[j])CWideString(tag);}
 std::pair<ETCInterfaceType,CTCBase*> components[2];components[0].first=TCI_SOUND_PLAYER;components[0].second=expected_sound;components[1].first=TCI_INVENTORY_MAP;components[1].second=mode==5?0:static_cast<NInventory::CTCInventoryBase*>(expected_map);
 if(expected_creature){expected_creature->Components._M_start=components;expected_creature->Components._M_finish=components+2;expected_creature->Components._M_end_of_storage._M_data=components+2;expected_creature->Dead=mode==2;expected_creature->UsingComponents._M_w[0]=(mode>=2?1UL<<TCI_SOUND_PLAYER:0)|(mode>=3?1UL<<TCI_INVENTORY_MAP:0);}
 int category=input_category==Entry::CATEGORY_TUTORIAL?Entry::CATEGORY_BASICS:input_category;const int mapped[]={3,4,5,6,7,0,1};for(int k=0;k<7;++k)if(mask&(1<<k))category=mapped[k];
 if(growth||duplicate)add_existing(book,(Category)(duplicate?7:category),oldname);
 if(duplicate==2){name.UnassignString();name.PStringData=oldname.PStringData;++name.PStringData->NoRefs;}
 if(duplicate==3){name.UnassignString();oldname.UnassignString();clear_node(book.LogEntries._M_t._M_header._M_data->_M_parent);free(book.LogEntries._M_t._M_header._M_data);init_map(book.LogEntries);add_existing(book,(Category)7,nullname);}
 if(duplicate==4){name.UnassignString();name.PStringData=emptyname.PStringData;++name.PStringData->NoRefs;clear_node(book.LogEntries._M_t._M_header._M_data->_M_parent);free(book.LogEntries._M_t._M_header._M_data);init_map(book.LogEntries);add_existing(book,(Category)7,emptyname);}
 size_t before=entry_count(book.LogEntries);long live_before=CWideString::NoStrings;int refs_before=expected_def->IVRefCount;
 sound_calls=map_calls=def_calls=player_calls=0;expected_category=(Category)input_category;expected_name=&name;expected_abbreviation=&abbreviation;expected_content=&content;
 bool add_to_map=mode>=3;book.AddEntry(name,abbreviation,content,(Category)input_category,add_to_map);
 assert(entry_count(book.LogEntries)==before+(duplicate?0:1));assert(def_calls==(duplicate?0:1)&&player_calls==(duplicate?0:1));
 assert(sound_calls==(!duplicate&&mode>=3?1:0));assert(map_calls==(!duplicate&&mode>=3&&mode!=5?1:0));
 assert(expected_def->IVRefCount==refs_before&&live_chars==0&&CWideString::NoStrings==live_before+(duplicate?0:3));
 if(!duplicate){EntryMap::iterator at=book.LogEntries.find((Category)category);assert(at!=book.LogEntries.end());Entry& entry=at->second[at->second.size()-1];assert(wide_equal(entry.Name,"new-entry")&&wide_equal(entry.AbbreviatedName,"short")&&wide_equal(entry.Content,text));}
 clear_node(book.LogEntries._M_t._M_header._M_data->_M_parent);free(book.LogEntries._M_t._M_header._M_data);for(int m=0;m<7;++m)categories[m]->~CWideString();
}
static int check_tree_node(std::_Rb_tree_node_base* p,std::_Rb_tree_node_base* parent,int lower,int upper,int& count){
 if(!p)return 1;assert(p->_M_parent==parent);typedef EntryMap::value_type Value;std::_Rb_tree_node<Value>* node=static_cast<std::_Rb_tree_node<Value>*>(p);int key=node->_M_value_field.first;assert(key>lower&&key<upper);++count;
 if(!p->_M_color){assert(!p->_M_left||p->_M_left->_M_color);assert(!p->_M_right||p->_M_right->_M_color);}
 int left=check_tree_node(p->_M_left,p,lower,key,count),right=check_tree_node(p->_M_right,p,key,upper,count);assert(left==right);return left+(p->_M_color?1:0);
}
static void run_tree_case(int start){
 FixtureWide text("tree-entry");Memory<CHeroLogBook> storage;memset(&storage,0,sizeof(storage));CHeroLogBook& book=*(CHeroLogBook*)storage.bytes;init_map(book.LogEntries);
 for(int i=0;i<8;++i){int key=(start+i*3)%8;add_existing(book,(Category)key,text);std::_Rb_tree_node_base* header=book.LogEntries._M_t._M_header._M_data;assert(header->_M_parent->_M_color);int count=0;check_tree_node(header->_M_parent,header,-1,8,count);assert(count==i+1);}
 for(int j=0;j<8;++j){Entries values;Entry entry(text,text,text);values.push_back(entry);std::pair<EntryMap::iterator,bool> added=book.LogEntries.insert(EntryMap::value_type((Category)j,values));assert(!added.second&&added.first->first==j&&added.first->second.size()==1);}
 int expected=0;for(EntryMap::iterator it=book.LogEntries.begin();it!=book.LogEntries.end();++it){assert(it->first==expected);++expected;}assert(expected==8);
 clear_node(book.LogEntries._M_t._M_header._M_data->_M_parent);free(book.LogEntries._M_t._M_header._M_data);
}
int main(){memset(&zero_wide,0,sizeof(zero_wide));int cases=0;
 for(int mode=0;mode<6;++mode)for(int category=0;category<8;++category)for(int pattern=0;pattern<10;++pattern){int mask=pattern==0?0:(pattern<=7?1<<(pattern-1):(pattern==8?127:85));run_case(mode,category,mask,0,pattern&1);++cases;}
 for(int duplicate=1;duplicate<=4;++duplicate)for(int mode=0;mode<6;++mode){run_case(mode,2,127,duplicate,0);++cases;}
 for(int start=0;start<8;++start){run_tree_case(start);++cases;}
 assert(cases==512&&CWideString::NoStrings==0&&live_chars==0);puts("0047a9f0_TEST PASS");return 0;}
