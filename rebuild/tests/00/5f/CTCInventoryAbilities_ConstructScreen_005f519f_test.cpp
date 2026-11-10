// Standalone behavior test of the selected ConstructScreen body with production containers and counted pointers.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
#define FABLE_FASTCALL __fastcall
#define FABLE_STATIC_ASSERT(x)
class CCharString {
public: char value[256];
    CCharString(){value[0]=0;}
    CCharString(const char* text,long=-1){strcpy(value,text);}
};
class CWideString {
public: char value[256];
    CWideString(){value[0]=0;}
    CWideString(const char* text){strcpy(value,text);}
    CWideString(const unsigned short* text){unsigned int n=0;while(text[n]){assert(text[n]<128);value[n]=(char)text[n];++n;}value[n]=0;}
    const CWideString& operator+=(const CWideString& other){strcat(value,other.value);return *this;}
};
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H
// STLport 4.6.2 excerpts: stl/type_traits.h, _iterator_base.h, _algobase.h, _construct.h, _uninitialized.h,
// _alloc.h, _vector.h/_vector.c, _pair.h, _list.h, _bitset.h, _range_errors.h and _function_base.h.
// One model for every translation unit (Codex pass 11 consolidation): original names, keys and inheritance;
// the members retail keeps out of line (_M_fill_insert, list::_M_insert_dispatch, ~list, vector copy
// constructor) are declared without a body here, as everywhere else in rebuild/include.
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
// Owner decision 2026-11-03 (D2, "faithfully recreate what retail did"): the dispatch tags carry an empty user constructor, so a
// tag temporary (push_back's _IsPODType() argument, the __destroy / __uninitialized_* dispatch) is never zero-filled -- retail passes
// the tag's address without storing to it (UpdateShadowedSpotLights 0x00B47F82..0x00B47F91, SwitchDoorsOnAndOff 0x00DEFB10;
// 558 landed rows declared the tags this way locally before the default path did).
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template<class I> struct iterator_traits;
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
// _algobase.h
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
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
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
// _construct.h
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
// _uninitialized.h
#ifdef FABLE_STLP_OUTOFLINE_UNINIT_COPY
// Opt-in (chunk 760, row 0x00CBC130 vector::_M_insert_overflow): like FABLE_STLP_OUTOFLINE_UNINIT_FILL_N below, the retail TU keeps
// the non-POD __uninitialized_copy instantiation out of line as a __fastcall COMDAT (0x00CB9B00, called twice with the tag
// temporary passed by address); the same-TU noinline body gives that shape.
#pragma auto_inline(off)
template<class I,class O> __declspec(noinline) O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
#pragma auto_inline(on)
#elif defined(FABLE_RETAIL_STLP_UNINIT_OUTOFLINE)
// Per-TU opt-in (chunk 892, row 0x0076E4D0 CTCWeaponTrail::GetLastWeaponTrailPoints): the non-POD __uninitialized_copy and
// __uninitialized_fill_n instantiations out of line with the default convention (that TU is not /Gr; 0x0076F780 / 0x0076F7D0,
// reached from vector::_M_insert_overflow 0x007700D0). With both bodies visible c2 knows push_back's temporary is never captured.
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
// Opt-in (2026-11-03, chunk 719 row 0x00501150): the retail TU keeps this instantiation out of line as a __fastcall COMDAT
// (0x00513570 ??$__uninitialized_fill_n@PAVCLevelFiles@CObject@CLevelLoader@@IV123@@std@@YI...ABU__false_type@0@@Z, called
// from the inlined vector(n) with the tag homed in a dead argument slot); the same-TU noinline body gives that shape (EXTRA_RULES
// chunk 422 / GOTCHAS 0xB3EA70: inlined, the fill makes vector(n) too large and VC7.1 calls the whole ctor instead).
#pragma auto_inline(off)
template<class I,class N,class T> __declspec(noinline) I __fastcall __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#pragma auto_inline(on)
#elif defined(FABLE_RETAIL_STLP_UNINIT_OUTOFLINE)
#pragma auto_inline(off)
template<class I,class N,class T> I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {   // see FABLE_RETAIL_STLP_UNINIT_OUTOFLINE above
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
// _alloc.h (malloc-based allocator)
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
// (also FABLE_RETAIL_STLP_SORT_INTERNALS, chunk 892: iter_swap below swaps through it)
// Retail opt-in (chunk 849, row 0x00AE2E20 CParticleProgram::OptimiseSpringOrder): STLport 4.6 _algobase.h swap(_Tp&, _Tp&), used by
// the inline vector::swap below (retail expands `particle_list.swap(next_particle_list)` in place: the words exchanged through
// CPU locations at 0x00AE30C7..0x00AE30DC, then the old storage freed at 0x00AE30E3; no call).
template<class _Tp> inline void swap(_Tp& a,_Tp& b) { _Tp tmp=a; a=b; b=tmp; }
#endif
// _vector.h / _vector.c
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
    // Per-TU opt-in (chunk 779, row 0x00BEAE50 CWaterPatchMesh::Load; like FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE): STLport's
    // vector(size_type n) is one inline body, which VC7.1 expanded in some TUs and called as a COMDAT in others; that TU calls
    // vector<unsigned char>::vector(size_type) 0x00514E20 (??0?$vector@EV?$allocator@E@std@@@std@@QAE@I@Z, bound by the landed
    // 0x00996610) for both CArray<unsigned char> locals (0x00BEAFDF, 0x00BEAFE9). A declaration only here, so the row calls it.
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
    // Retail branch (chunk 735, row 0x00577AC0): STLport 4.6 _vector.c operator=(const vector&), defined out of class (not
    // inline): retail calls its vector<NUISystem::CComponent*> instance at 0x005798C8 for `ExpSliders = ...`.
    vector& operator=(const vector& x);
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_C
    // Retail branch (chunk 741, row 0x00B149D0 CPSCEmitterGeneric::ReadBinary): STLport 4.6 _vector.h includes _vector.c, so the
    // TU sees operator='s body (below) and _M_allocate_and_copy; VC7.1 still calls operator= out of line (vector<C3DVector>
    // 0x0075B8D0) but, seeing that it never captures its argument, keeps the caller's local start pointer in a CPU location across
    // the call (retail 0x00B15840 `mov ebx,[esp+14h]` ... 0x00B1587C `test ebx,ebx; push ebx; call free`; declaration-only: reloaded, 3784v3779).
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
    // Retail opt-in (chunk 798, row 0x00F0C140 CQ_AmbushTradersScript::PhaseTwo): STLport 4.6 _vector.h swap(_Self&) is one inline
    // body (three _STLP_STD::swap calls), which VC7.1 called as a COMDAT in that TU: vector<CQ_AmbushTradersScript::TeleportData>::swap
    // 0x00F0CA30 (Ego_r.pdb ?swap@?$vector@UTeleportData@CQ_AmbushTradersScript@NScript@@...@std@@QAEXAAV12@@Z; call at 0x00F0C823 with
    // ecx = &TeleportInfo and an empty local vector). A declaration only here, so the row calls it.
    void swap(vector& x);
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_SWAP
    // (chunk 849) STLport 4.6 _vector.h swap(_Self&): three _STLP_STD::swap calls, inline (0x00AE2E20 expands it).
    void swap(vector& x) { std::swap(this->_M_start,x._M_start); std::swap(this->_M_finish,x._M_finish); std::swap(this->_M_end_of_storage,x._M_end_of_storage); }
#endif
    _Tp& front() { return *this->_M_start; }
    _Tp& back() { return *(end()-1); }
    const _Tp& back() const { return *(end()-1); }
    void pop_back() { --this->_M_finish; _Destroy(this->_M_finish); }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
    // Per-TU opt-in (chunk 760, row 0x0086BE40 CRandomAppearanceMorph::GetRandomBodyParts; like
    // FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN): STLport's push_back is one inline body, which VC7.1
    // expanded in some TUs and called as a COMDAT in others; that TU calls push_back<CMorphSet::CMeshTextureSet> 0x0086E7C0
    // (whose own body expands _M_insert_overflow). A declaration only here, so the row calls it.
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
    // Retail opt-in (chunk 782, row 0x00DFD600 CQ_TraderConflictGoodScript::AttackPeople; like
    // FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE): STLport's erase(position) is one inline body, which VC7.1 expanded in some TUs
    // and called as a COMDAT in others; that TU calls vector<CScriptThing>::erase(iterator) 0x00CD3152 (call at 0x00DFDAB6, the
    // landed UpdateLiveEnemies 0x00DFC320 binds it too) and re-reads the vector's words after it. Declaration only here.
    iterator erase(iterator position);
#else
    iterator erase(iterator position) {
        if(position+1!=end()) __copy_ptrs(position+1,this->_M_finish,position,_IsPODType());
        --this->_M_finish;_Destroy(this->_M_finish);return position;
    }
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
    // Per-TU opt-in (chunk 773, row 0x00671600 CThingCreatureDef::Transfer; like FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE): STLport's
    // erase(first, last) is one inline body, which VC7.1 expanded in some TUs and called as a COMDAT in others; that TU calls
    // vector<ENavigatorType>::erase 0x00672070 (landed, the body above) for `erase(unique(begin(), end()), end())`. Declaration only here.
    iterator erase(iterator first,iterator last);
#else
    iterator erase(iterator first,iterator last) {
        iterator result=__copy_ptrs(last,this->_M_finish,first,_IsPODType());
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
#endif
    void clear() { erase(begin(),end()); }
    // Retail takes the value by const reference (STLport 4.5.2 form; 0x004428F0 resize<long> forwards the pointer to
    // _M_fill_insert, which dereferences it); FableWin.pdb's later build passes it by value.
#ifdef FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
    // Per-TU opt-in (chunk 777, row 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest; like
    // FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE): VC7.1 expanded STLport's inline resize in some TUs and called it as a COMDAT
    // in others; that TU calls vector<char>::resize 0x004122D0 (landed, ?resize@?$vector@DV?$allocator@D@std@@@std@@QAEXIABD@Z) with
    // the char() temporary in a dead argument slot. The body is visible (STLport's own) but kept out of line, as in the retail TU:
    // VC7.1 then knows resize does not keep the vector's address, holds the start in a CPU location across the later calls and reuses
    // the vector's frame slot (0x0055FA79 `mov edi,[esp+28h]` ... 0x0055FB34 `push edi; call free`); declaration-only re-reads it.
    __declspec(noinline) void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#else
    void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#endif
    void resize(size_type n) { resize(n,_Tp()); }
    void _M_fill_insert(iterator position,size_type n,const _Tp& x);
#ifdef FABLE_RETAIL_STLP_VECTOR
    // Retail branch (chunk 728, row 0x006D2BD0 CTCWife::OnSerialise): STLport 4.6 vector::reserve (stl/_vector.c), out of line in
    // retail (reserve<CCharString> 0x004BA7A0, reserve<pair<CCharString,long> > 0x006D4680). A declaration only.
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
// _vector.c: the growth paths are out-of-class, non-inline template members.
#ifdef FABLE_RETAIL_STLP_VECTOR_C
// STLport 4.6 stl/_vector.c operator= (see the in-class note): grow (allocate-and-copy, clear, new storage), shrink (copy, destroy
// the tail) or extend (copy over size(), uninitialized-copy the rest), then _M_finish = _M_start + xlen.
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
#ifdef FABLE_RETAIL_STLP_VECTOR_RESERVE
// Retail opt-in (chunk 857, row 0x00AC8C80 COptimisedPrimitive::ComputeVertexCost): STLport 4.6 stl/_vector.c reserve, the body the
// landed out-of-line copy 0x00920FF0 (vector<C3DVector>) reproduces, visible but kept out of line as in the retail TU (call at
// 0x00AC8CCB). Seeing that reserve never captures the vector, VC7.1 keeps the local vector's _M_finish in a CPU location across the
// push_back loop (0x00AC8CE1 `mov ebx,[esp+7Ch]` ... 0x00AC8D45 `add ebx,0Ch`); declaration-only reloads it after every store.
// Pair with FABLE_RETAIL_STLP_VECTOR (the in-class declaration).
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
// Retail branch (pass 12 header wave): STLport 4.6 _vector.h includes _vector.c, so every retail TU that calls resize() /
// insert(pos, n, x) saw _M_fill_insert's body (stl/_vector.c lines 61-86, reproduced here) and VC7.1 still kept the call out of
// line. The visible body changes the caller's CPU assignment: 0x00AC2BE0 PartitionSpace matches only with it (2775v2791
// declaration-only; retail's _M_fill_insert<bool> is 0x004676EF).
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
// _pair.h
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
// Retail branch (chunk 880, row 0x0058AF10 CTCInventory::OpenPCOptionsMenu): STLport 4.6 _pair.h's string-literal make_pair
// overloads (_STLP_FUNCTION_TMPL_PARTIAL_ORDER, defined for VC7.1): a literal decays to `const char*`, so
// `make_pair("A", "B")` is a pair<const char*, const char*> that converts to a pair<CCharString, CCharString> through the
// member-template constructor (out of line at 0x00592495).
template<class _T1,class _T2,int _Sz> inline pair<_T1,_T2 const*> __cdecl make_pair(_T1 const& a,_T2 const (&b)[_Sz]) { return pair<_T1,_T2 const*>(a,static_cast<_T2 const*>(b)); }
template<class _T1,class _T2,int _Sz> inline pair<_T1 const*,_T2> __cdecl make_pair(_T1 const (&a)[_Sz],_T2 const& b) { return pair<_T1 const*,_T2>(static_cast<_T1 const*>(a),b); }
template<class _T1,class _T2,int _Sz1,int _Sz2> inline pair<_T1 const*,_T2 const*> __cdecl make_pair(_T1 const (&a)[_Sz1],_T2 const (&b)[_Sz2]) { return pair<_T1 const*,_T2 const*>(static_cast<_T1 const*>(a),static_cast<_T2 const*>(b)); }
#endif
// _list.h
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
    // Retail branch (chunk 736, row 0x00A7A0A0 CNavQuadTree::SetUpRegions): STLport 4.6 _list.h _M_decr, for list::back().
    void _M_decr() { _M_node=_M_node->_M_prev; }
#endif
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
#if defined(FABLE_RETAIL_STLP_LIST_NOINLINE_NE) && defined(_MSC_VER)
    // Per-TU inline opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): that TU calls the list iterator comparison
    // out of line at all three of its sites (0x0041EAA3 / 0x0041EBD6 AUGMENT, 0x00420CED CHANGE_ALPHA -> COMDAT 0x0041C7C0) where
    // VC7.1's caller-size inline budget would expand it; the end() in the same loop condition stays expanded, so an
    // inline_depth region (per full-expression) cannot separate them.
    __declspec(noinline)
#endif
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
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
    // Per-TU opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): STLport 4.6 _list.h postfix decrement (retail calls
    // it out of line at 0x0042899B from the ADD_CHILD case).
    _List_iterator operator--(int) { _List_iterator tmp=*this; this->_M_decr(); return tmp; }
#endif
};
#ifdef FABLE_RETAIL_STLP_LIST_REVERSE
// Per-TU opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): STLport 4.6 _iterator.h reverse_iterator, the part
// list::rbegin() users need. operator* decrements a copy of `current` (`mov eax,[hdr+4] ; mov eax,[eax+8]`), operator++
// decrements `current` itself; the action's last parameters are read as `*it`, `*++it` (0x0041F9B3 / 0x0041F9ED).
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
// Per-TU opt-in (chunk 892, row 0x0076E4D0 CTCWeaponTrail::GetLastWeaponTrailPoints): STLport 4.6 _iterator_base.h advance() for a
// list iterator -- the bidirectional __advance (`if (n > 0) while (n--) ++i; else while (n++) --i;`), dispatched on the iterator's
// category; the trail's `advance(it, -n)` from end() walks the prev links (0x0076E520..).
template<class _Tp,class _Traits> inline bidirectional_iterator_tag __ITERATOR_CATEGORY(const _List_iterator<_Tp,_Traits>&) { return bidirectional_iterator_tag(); }
template<class _BidirectionalIterator,class _Distance> inline void __advance(_BidirectionalIterator& i,_Distance n,const bidirectional_iterator_tag&) {
    if(n>0) while(n--) ++i; else while(n++) --i;
}
template<class _InputIterator,class _Distance> inline void advance(_InputIterator& i,_Distance n) { __advance(i,n,__ITERATOR_CATEGORY(i)); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
// Per-TU opt-in (chunk 849, row 0x0057E2F1 CTCHeroStats::OnSerialise): STLport 4.6 _iterator_base.h distance() / __distance() (const
// reference arguments, the input-iterator loop on a copy of first), for list::size() below.
template<class _InputIterator> inline ptrdiff_t __distance(const _InputIterator& first,const _InputIterator& last,const input_iterator_tag&) {
    ptrdiff_t n=0; _InputIterator it(first); while(it!=last) { ++it; ++n; } return n;
}
template<class _InputIterator> inline ptrdiff_t distance(const _InputIterator& first,const _InputIterator& last) {
    return __distance(first,last,input_iterator_tag());
}
#endif
template<class _Tp,class _Alloc> class _List_base {
public:
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#ifdef FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
    // Per-TU opt-in (chunk 801, row 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering): STLport 4.6 _list.c
    // _List_base::clear(), declared only. Retail calls the list<CEnginePrimitiveHandle> instantiation out of line at 0x00834F90
    // (`lea edi,[esi+194h] ; mov ecx,edi ; call 0x00834F90`, 0x00833C0C; the landed ~CThingAICreature 0x008320C0 binds the same
    // address as ?clear@?$_List_base@VCEnginePrimitiveHandle@@..@std@@QAEXXZ).
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
    // Per-TU opt-in (chunk 892, row 0x005A887D CTCInventoryTrade::RefreshBuyPossibilityPC): STLport's inline destructor, kept out of
    // line with its body visible, as the retail TU calls it (~list<CComponent*> 0x0042ABCA at 0x005A8A33). __declspec(noinline) is
    // MSVC's spelling (FABLE_STLP_NOINLINE is empty elsewhere).
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
    // Per-TU opt-in (chunk 892, row 0x005A887D): STLport's insert(iterator, const T&), body visible but kept out of line, as the
    // retail TU calls it (list<CComponent*>::insert 0x0042AB8A at 0x005A8A14, from push_back): VC7.1 then knows it keeps neither
    // the list's nor the value's address.
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
    // Per-TU opt-in (chunk 801, row 0x00833C00): STLport 4.6 _list.h `iterator insert(iterator __pos)` (the no-default-parameter
    // overload, `return insert(__pos, _Tp());`), declared only: retail calls it out of line at 0x007464A0 (the landed body builds the
    // default CEnginePrimitiveHandle and links the node), with the iterator copied in place into the argument slot (`push ecx ;
    // mov ecx,[edi] ; mov eax,esp ; mov [eax],ecx`, 0x00833C74) and the result through a hidden pointer. push_back() is the
    // STLport inline `insert(end())`.
    iterator insert(iterator position);
    void push_back() { insert(end()); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST
    // Retail branch (chunk 726): STLport 4.6 _list.h size() / front() / push_front(). size() is distance(begin(), end())
    // with the input-iterator __distance loop inlined (NUISystem::CList::ChangeState 0x0053A0A1 counts the
    // GPreviousSelected nodes in place: mov ecx,[ecx] ; inc edi ; cmp ecx,eax ; jne); push_front(x) is insert(begin(), x)
    // (out of line in that TU at 0x0053CA3C).
#ifdef FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
    // chunk 849 (row 0x0057E2F1): STLport 4.6's own spelling, `size_type __result = distance(begin(), end()); return __result;`. The
    // copy-of-first loop keeps the head in eax and walks ecx (`mov eax,[ebx+1D8h] ; mov ecx,[eax] ; .. mov ecx,[ecx] ; inc edx ;
    // cmp ecx,eax`, 0x0057EBA3, before the vector reserve); the for-loop form above swaps the two CPU locations.
    size_t size() const { size_t result=distance(begin(),end()); return result; }
#else
    size_t size() const { ptrdiff_t n=0; for(const_iterator first=begin(),last=end();first!=last;++first) ++n; return n; }
#endif
    _Tp& front() { return *begin(); }
    void push_front(const _Tp& x) { insert(begin(),x); }
    // chunk 736 (row 0x00A7A0A0): STLport 4.6 _list.h back(): `iterator __tmp = end(); return *(--__tmp);` (SetUpRegions reads
    // Regions.back() as the list head's _M_prev before each MakeRegion call: `mov eax,[edx+4] ; add eax,8`).
    _Tp& back() { iterator tmp=end(); return *(--tmp); }
    const _Tp& back() const { const_iterator tmp=end(); return *(--tmp); }
    // chunk 749 (row 0x00549440 CTextSlider::ProcessEvent): STLport 4.6 _list.h erase(iterator) and pop_front() (`erase(begin())`);
    // retail calls the list<NUISystem::CList*> instances out of line (pop_front 0x0053C7D9 -> erase 0x0053C7B9).
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
    // Retail opt-in (chunk 857, row 0x00A1C010 CNavigatorAStar::CullWayPoints): STLport 4.6 _list.h erase(first, last),
    // `while (__first != __last) erase(__first++); return __last;`, expanded in retail (0x00A1C661: next / prev of the node, the
    // iterator advanced, prev->next / next->prev relinked, free, until the CullBack node).
    iterator erase(iterator first,iterator last) {
        while(first!=last) erase(first++);
        return last;
    }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_POPBACK
    // chunk 760 (row 0x00683B00 CTCCreatureOpinionOfHero::ClearPendingDeeds): STLport 4.6 _list.h pop_back():
    // `iterator __tmp = end(); erase(--__tmp);` (0x00683BA9: the head's _M_prev, its next/prev, prev->next first, then free).
    void pop_back() { iterator tmp=end(); erase(--tmp); }
#endif
#endif
#ifdef FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
    // chunk 767 (row 0x0082A380 CMiniMapDisplay::RemoveMarker): STLport 4.6 _list.h member template remove_if(_Predicate), declared
    // only: retail tail-jumps to the out-of-line instantiation (0x0082C770, ??$remove_if@VCIsMarkerBoundToThing@CMiniMapDisplay@@@..).
    template<class _Predicate> void remove_if(_Predicate pred);
#endif
};
}
#endif
#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H
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
//   ShareData = 0x00CE1040 (CCountedPointer<CEntityScriptBase>: every landed caller binds that T, FableWin has the symbol; the method name is Ego_r.pdb's): `if (Info != info) { <release>; Data = data;
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
// Inlining: every member is plain `inline` (in-class), never FABLE_FORCEINLINE / noinline. Retail calls the COMDAT copies at
//   some sites and expands them at others; VC7.1 RTM 3077 reproduces that per-site choice from these bodies on the landed
//   rows (wave 9 HEADERS.md lists the shapes it does not reproduce). unproven: the source order of Reset's two stores and
//   the bodies of GetReferenceCount / reset / Get / get / IsNull (FableWin names only, no retail out-of-line copy found).


static void* last_pointer_info_data=0;
static int control_blocks_alive=0;
class CCPPointerInfo {
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { last_pointer_info_data=data;++control_blocks_alive; }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
    ~CCPPointerInfo(){--control_blocks_alive;}
};
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);

template <class T> class CCountedPointer {
public:
    T* Data;                                                             // +0x0
    CCPPointerInfo* Info;                                                // +0x4

    CCountedPointer() : Data(0), Info(0) { }
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR) && !defined(FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR)
    // Chunk 764 (row 0x0096DBC0 CCreatureAction_ScreamerIntoAggressive::Initialise): there retail calls the (T*) constructor's COMDAT
    // (0x00718A00, ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAVCModeBase@CTCCreatureModeManager@@@Z) where VC7.1
    // expands the in-class body; the landed same-module rows (0x008C1A10, 0x008C1900) declare it without a body. Declaration only.
    CCountedPointer(T* data);
#else
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR) && defined(_MSC_VER)
    // Inline-budget (row 0x0041E6D3 NUISystem::CManager::Action): retail CALLS this ctor's same-TU COMDATs at every site
    // (CComponent 0x00429C15 x25, CAction 0x0042AA29 x17, CTexture 0x0041C030 in LOAD_PHOTO; body = the default one below:
    // 0x0041C030 `test eax,eax; mov [esi],eax; je` then `new CCPPointerInfo(1, DeleteData, Data)` reading the stored member).
    // A declaration-only ctor (FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR alone) exposes each object's address above c2's
    // 10000-tuple limit, so it never shares a frame slot (LOAD_PHOTO's texture: ours -3C8h alone, retail -268h shared).
    // A visible body that is not expanded keeps the object tracked. Default path unchanged.
    __declspec(noinline)
#endif
    CCountedPointer(T* data) : Data(data)
    {
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER
        // Retail (Codex pass 11): the ctor tests the member it just stored -- 0x00626E40 spills the pointer and reloads it
        // for the test and the CCPPointerInfo store, which `if (data)` (the landed default) does not reproduce.
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

#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H

namespace NGameText {
class CDataBank {public: const CWideString GetTextBySymbol(const CCharString& symbol)const{return CWideString(symbol.value);} };
}
static NGameText::CDataBank test_bank;
class CMainGameComponent {public:const NGameText::CDataBank& GetTextBank()const{return test_bank;} };
static CMainGameComponent game;
class CTCInventoryAbilities;
namespace NUISystem {
class CComponent {
public:
    char kind[128],name[128],caption[256];
    std::vector<CCountedPointer<CComponent> > children;
    bool unavailable;float x,y;
    CComponent(const char* type="component"):unavailable(false),x(0),y(0){strcpy(kind,type);name[0]=0;caption[0]=0;}
    virtual ~CComponent(){}
    void SetName(const CCharString& text){strcpy(name,text.value);}
    void AddChild(CCountedPointer<CComponent>& child){children.push_back(child);}
    void MakeUnavailable(){unavailable=true;}
    CComponent* FindChild(CCharString text);
};
class CText:public CComponent {
public: CText(const char* type):CComponent(type){}
    void SetValue(const CWideString& text,bool a,bool b){assert(a && b);strcpy(caption,text.value);}
};
class CViewport:public CComponent {
public:CComponent* fake[8];int fake_count;
    CViewport():fake_count(0){}
    void AddFakeChild(CComponent* component){fake[fake_count++]=component;}
};
static CViewport* current_viewport;
CComponent* CComponent::FindChild(CCharString text){assert(strcmp(text.value,"UI_SCROLLING_VIEWPORT_TEXT_AREA")==0);return current_viewport;}
class CList:public CComponent {
public:
    std::vector<CCountedPointer<CComponent> > scrolling,headers;
    unsigned long offsets;
    CList():CComponent("UI_SUB_LIST"),offsets(99){}
    void AddNonScrollingChild(CCountedPointer<CComponent>& component){headers.push_back(component);}
    void AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>& component){scrolling.push_back(component);}
    void InitialiseOffsets(unsigned long count){offsets=count;}
};
class CManager {
public:
    CTCInventoryAbilities* inventory;int registrations;
    CManager():inventory(0),registrations(0){}
    static CManager* GetInstance();
    void SetAbilitiesInventory(CTCInventoryAbilities* value){inventory=value;++registrations;}
    CComponent* CreateComponent(const CCharString& text,bool flag){
        assert(!flag);
        if(strcmp(text.value,"UI_SUB_LIST")==0)return new CList;
        if(strcmp(text.value,"UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF")==0 || strcmp(text.value,"UI_ONE_OFF_SUBTITLE")==0)return new CText(text.value);
        return new CComponent(text.value);
    }
    CComponent* CreateMenuEntryComponent(const CWideString& title,float x,float y,const CCharString& template_name,float a,const CCharString empty,float b,CCharString button){
        assert(a==0 && b==0 && empty.value[0]==0 && strcmp(button.value,"PC_BUTTON_BASTARD")==0);
        CComponent* component=new CComponent(template_name.value);strcpy(component->caption,title.value);component->x=x;component->y=y;return component;
    }
};
static CManager manager;
CManager* CManager::GetInstance(){return &manager;}
}
class CTCInventoryStats {public:static CWideString GetXString(){return CWideString(" x");} };
CWideString GFIntToWideString(long value){char text[32];sprintf(text,"%ld",value);return CWideString(text);}
class CTCInventoryAbilities {
public:
    bool has_magic,has_expressions;long magic_count,expression_count;int magic_calls,expression_calls;
    NUISystem::CComponent* last_magic;
    const CMainGameComponent& PeekComponent()const{return game;}
    NUISystem::CComponent* GetMagicListUIComponent(long& count,NUISystem::CComponent*){assert(expression_calls==0 && count==0);++magic_calls;count=magic_count;last_magic=has_magic?new NUISystem::CComponent("magic_body"):0;return last_magic;}
    NUISystem::CComponent* GetExpressionsListUIComponent(long& count,NUISystem::CComponent*){assert(magic_calls==1 && count==0);if(has_magic)assert(last_pointer_info_data==last_magic);++expression_calls;count=expression_count;return has_expressions?new NUISystem::CComponent("expressions_body"):0;}
    NUISystem::CComponent* ConstructScreen(NUISystem::CComponent* parent);
};
NUISystem::CComponent* CTCInventoryAbilities::ConstructScreen(NUISystem::CComponent* parent)
{
    const NGameText::CDataBank& textBank=PeekComponent().GetTextBank();
    NUISystem::CManager::GetInstance()->SetAbilitiesInventory(this);
    std::list<CWideString> titles;
    titles.push_back(textBank.GetTextBySymbol("TEXT_GUI_MAGIC"));
    titles.push_back(textBank.GetTextBySymbol("TEXT_GUI_EXPRESSIONS"));
    long expressionCount=0;
    long magicCount=0;
    NUISystem::CManager& ui=*NUISystem::CManager::GetInstance();
    NUISystem::CList* list=static_cast<NUISystem::CList*>(ui.CreateComponent("UI_SUB_LIST",false));
    CCountedPointer<NUISystem::CComponent> magic(GetMagicListUIComponent(magicCount,parent));
    CCountedPointer<NUISystem::CComponent> expressions(GetExpressionsListUIComponent(expressionCount,parent));
    CCountedPointer<NUISystem::CComponent> back(ui.CreateComponent("UI_BACK_FOR_LIST",false));
    CCountedPointer<NUISystem::CComponent> select(ui.CreateComponent("UI_SELECT_FOR_LIST",false));
    list->AddNonScrollingChild(back);
    list->AddNonScrollingChild(select);
    CCountedPointer<NUISystem::CComponent> title(ui.CreateComponent("UI_SKILLS_TITLE",false));
    CCharString titleName("SKILLS TITLE");
    title->SetName(titleName);
    list->AddNonScrollingChild(title);
    unsigned long itemCount=0;
    if(!magic.IsNull()) ++itemCount;
    if(!expressions.IsNull()) ++itemCount;
    list->InitialiseOffsets(itemCount);
    CWideString caption=textBank.GetTextBySymbol("TEXT_GUI_MENU_MAGIC");
    CCountedPointer<NUISystem::CComponent> magicEntry;
    if(magicCount>0) {
        caption+=CTCInventoryStats::GetXString();
        caption+=GFIntToWideString(magicCount);
    }
    if(!magic.IsNull()) {
        magicEntry.Reset(ui.CreateMenuEntryComponent(caption,0.0f,1.0f,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        magicEntry->AddChild(magic);
    } else {
        magicEntry.Reset(ui.CreateMenuEntryComponent(caption,0.0f,1.0f,"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        magicEntry->MakeUnavailable();
    }
    CCountedPointer<NUISystem::CComponent> magicDescription(ui.CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF",false));
    NUISystem::CText* magicText=static_cast<NUISystem::CText*>(magicDescription.Get());
    magicText->SetValue(PeekComponent().GetTextBank().GetTextBySymbol("TEXT_GUI_MENUGUIDE_SKILLS_MAGIC"),true,true);
    magicEntry->AddChild(magicDescription);
    NUISystem::CViewport* viewport=static_cast<NUISystem::CViewport*>(parent->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    viewport->AddFakeChild(magicDescription.Get());
    CCountedPointer<NUISystem::CComponent> subtitle(ui.CreateComponent("UI_ONE_OFF_SUBTITLE",false));
    static_cast<NUISystem::CText*>(subtitle.Get())->SetValue(caption,true,true);
    magicEntry->AddChild(subtitle);
    list->AddScrollingChildAndSetOffsets(magicEntry);
    CCountedPointer<NUISystem::CComponent> magicRing(ui.CreateComponent("UI_RING_PIC_SKILLS_MAGIC",false));
    magicEntry->AddChild(magicRing);
    caption=textBank.GetTextBySymbol("TEXT_GUI_MENU_EXPRESSIONS");
    if(expressionCount>0) {
        caption+=CTCInventoryStats::GetXString();
        caption+=GFIntToWideString(expressionCount);
    }
    CCountedPointer<NUISystem::CComponent> expressionEntry;
    if(!expressions.IsNull()) {
        expressionEntry.Reset(ui.CreateMenuEntryComponent(caption,25.0f,0.6666667f,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        expressionEntry->AddChild(expressions);
    } else {
        expressionEntry.Reset(ui.CreateMenuEntryComponent(caption,25.0f,0.6666667f,"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        expressionEntry->MakeUnavailable();
    }
    CCountedPointer<NUISystem::CComponent> expressionDescription(ui.CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF",false));
    NUISystem::CText* expressionText=static_cast<NUISystem::CText*>(expressionDescription.Get());
    expressionText->SetValue(PeekComponent().GetTextBank().GetTextBySymbol("TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS"),true,true);
    expressionEntry->AddChild(expressionDescription);
    viewport->AddFakeChild(expressionDescription.Get());
    subtitle.Reset(ui.CreateComponent("UI_ONE_OFF_SUBTITLE",false));
    static_cast<NUISystem::CText*>(subtitle.Get())->SetValue(caption,true,true);
    expressionEntry->AddChild(subtitle);
    list->AddScrollingChildAndSetOffsets(expressionEntry);
    CCountedPointer<NUISystem::CComponent> expressionRing(ui.CreateComponent("UI_RING_PIC_SKILLS_EXPRESSION",false));
    expressionEntry->AddChild(expressionRing);
    CCountedPointer<NUISystem::CComponent> button(ui.CreateComponent("UI_BACK_FOR_LIST",false));
    list->AddNonScrollingChild(button);
    button.Reset(ui.CreateComponent("UI_SELECT_FOR_LIST",false));
    list->AddNonScrollingChild(button);
    return list;
}


static void check(bool magic,bool expressions,long magic_count,long expression_count){
    last_pointer_info_data=0;
    NUISystem::CComponent parent;
    NUISystem::CViewport viewport;NUISystem::current_viewport=&viewport;
    CTCInventoryAbilities inventory;inventory.has_magic=magic;inventory.has_expressions=expressions;
    inventory.magic_count=magic_count;inventory.expression_count=expression_count;inventory.magic_calls=0;inventory.expression_calls=0;
    NUISystem::CList* list=static_cast<NUISystem::CList*>(inventory.ConstructScreen(&parent));
    assert(NUISystem::manager.inventory==&inventory && inventory.magic_calls==1 && inventory.expression_calls==1);
    assert(list->offsets==(unsigned long)((magic?1:0)+(expressions?1:0)));
    assert(list->headers.size()==5 && list->scrolling.size()==2);
    assert(strcmp(list->headers[0]->kind,"UI_BACK_FOR_LIST")==0 && strcmp(list->headers[1]->kind,"UI_SELECT_FOR_LIST")==0);
    assert(strcmp(list->headers[2]->name,"SKILLS TITLE")==0);
    assert(strcmp(list->headers[3]->kind,"UI_BACK_FOR_LIST")==0 && strcmp(list->headers[4]->kind,"UI_SELECT_FOR_LIST")==0);
    for(unsigned int n=0;n<2;++n){
        bool exists=n?expressions:magic;long count=n?expression_count:magic_count;
        NUISystem::CComponent* entry=list->scrolling[n].Get();
        assert(entry->unavailable==!exists);
        assert(strcmp(entry->kind,exists?"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW":"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW")==0);
        char expected[256];strcpy(expected,n?"TEXT_GUI_MENU_EXPRESSIONS":"TEXT_GUI_MENU_MAGIC");
        if(count>0){char number[32];sprintf(number," x%ld",count);strcat(expected,number);}
        assert(strcmp(entry->caption,expected)==0);
        assert(entry->children.size()==(unsigned int)(exists?4:3));
        unsigned int description=exists?1:0;
        assert(viewport.fake[n]==entry->children[description].Get());
        assert(strcmp(entry->children[description]->caption,n?"TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS":"TEXT_GUI_MENUGUIDE_SKILLS_MAGIC")==0);
        assert(strcmp(entry->children[description+1]->caption,expected)==0);
        assert(strcmp(entry->children[description+2]->kind,n?"UI_RING_PIC_SKILLS_EXPRESSION":"UI_RING_PIC_SKILLS_MAGIC")==0);
        if(n==0)assert(entry->x==0 && entry->y==1);else assert(entry->x==25 && entry->y>0.6666f && entry->y<0.6667f);
    }
    delete list;
}
// Test map supplies associative storage only; production uses the shared STLport map.
namespace std {
template<class K,class T> class map {
    vector<pair<K,T> > values;
public:
    typedef pair<K,T>* iterator;
    iterator begin(){return values.begin();}
    iterator end(){return values.end();}
    iterator find(const K& key){for(iterator p=begin();p!=end();++p)if(p->first==key)return p;return end();}
    T& operator[](const K& key){iterator p=find(key);if(p!=end())return p->second;values.push_back(pair<K,T>(key,T()));return values[values.size()-1].second;}
    void clear(){values.clear();}
    unsigned long size()const{return values.size();}
};
}
static CWideString operator+(const CWideString& a,const CWideString& b){CWideString result(a);result+=b;return result;}
namespace MagicTest {
static int live_components=0,live_actions=0,live_params=0;
enum EHeroAbility { ABILITY_NULL=0,ABILITY_LAST=19 };
enum ETCInterfaceType { TCI_EXPERIENCE=22 };
template<class T> class CDefPointer {public:T* Object;CDefPointer(T* p=0):Object(p){}T* operator->()const{return Object;} };
class CHeroAbilityDef {public:long IconGraphicIndex;std::vector<long> LevDescription;};
namespace NGameText {class CDataBank {public:const CWideString GetTextBySymbol(const CCharString& s)const{return CWideString(s.value);} };}
class CMainGameComponent {public:const NGameText::CDataBank& GetTextBank()const{static NGameText::CDataBank bank;return bank;} };
static CMainGameComponent game;
struct Observation {
    unsigned long offsets;
    int containers,spellCount,graphics[32],emptyDefinitions,fakeChildren,deletedComponents;
    bool experience,viewport,descriptions,texts,spriteIcons,arrows;
    Observation():offsets(0),containers(0),spellCount(0),emptyDefinitions(0),fakeChildren(0),deletedComponents(0),experience(true),viewport(true),descriptions(true),texts(true),spriteIcons(true),arrows(true){for(int n=0;n<32;++n)graphics[n]=0;}
};
static Observation observed;
namespace NUISystem {
enum EType { SPRITE=0,OTHER=1,TEXT=6 };
enum EActionType { UI_ACTION_TYPE_GIVE_EXCLUSIVE_INPUT=202,UI_ACTION_TYPE_SHOW_SPELL_SELECTION=205,UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN=206 };
class CAction {public:EActionType ActionType;CAction(){++live_actions;}~CAction(){--live_actions;} };
class CActionParam {public:bool component;long value;CActionParam(bool c=false,long v=0):component(c),value(v){if(!component)++live_params;}virtual ~CActionParam(){if(!component)--live_params;} };
class CActionParamSLONG:public CActionParam {public:CActionParamSLONG(long v):CActionParam(false,v){} };
class CActionParamULONG:public CActionParam {public:CActionParamULONG(unsigned long v):CActionParam(false,v){} };
class CDeletion {public:enum EDeletionMethod{UI_DELETE_THIS_INSTANCE=1};EDeletionMethod Method;};
class CComponent:public CActionParam {
public:
    char kind[128],caption[256];
    bool unavailable,deleted;
    std::vector<CCountedPointer<CComponent> > children;
    std::vector<CCountedPointer<CAction> > actions;
    CComponent(const char* k="component"):CActionParam(true),unavailable(false),deleted(false){strcpy(kind,k);caption[0]=0;++live_components;}
    virtual ~CComponent(){++observed.deletedComponents;--live_components;}
    virtual EType GetType()const{return OTHER;}
    CComponent* FindChild(const CCharString& name){for(unsigned int n=0;n<children.size();++n)if(strcmp(children[n]->kind,name.value)==0)return children[n].Get();return 0;}
    CComponent* FindFirstChildOfTypeNonRecursive(EType type){for(unsigned int n=0;n<children.size();++n)if(children[n]->GetType()==type)return children[n].Get();return 0;}
    void AddChild(CCountedPointer<CComponent>& p){children.push_back(p);}
    void AddAction(CCountedPointer<CAction>& p){actions.push_back(p);}
    void MakeUnavailable(){unavailable=true;}
    void SetDeletion(CDeletion d){assert(d.Method==CDeletion::UI_DELETE_THIS_INSTANCE);deleted=true;}
};
class CText:public CComponent {public:
    CWideString text;unsigned long textId;bool firstFlag,secondFlag;
    CText(const char* k):CComponent(k),textId(0),firstFlag(false),secondFlag(false){}
    virtual EType GetType()const{return TEXT;}
    const CWideString& GetValue()const{return text;}
    void SetValue(const CWideString& v,bool a,bool b){text=v;strcpy(caption,v.value);firstFlag=a;secondFlag=b;}
    void SetValue(unsigned long id,const CWideString& value,bool a){textId=id;text=value;firstFlag=a;}
};
class CSprite:public CComponent {public:long graphic;CSprite(const char* k):CComponent(k),graphic(-1){}virtual EType GetType()const{return SPRITE;}void SetGraphicIndex(unsigned long n){graphic=n;} };
class CMenuEntry:public CComponent {public:
    std::vector<CActionParam*> params;
    CMenuEntry(const char* k):CComponent(k){}
    virtual ~CMenuEntry(){for(unsigned int n=0;n<params.size();++n)if(!params[n]->component)delete params[n];}
    void AddUIActionParam(CActionParam* p){params.push_back(p);}
};
class CList:public CComponent {public:
    unsigned long offsets;std::vector<CCountedPointer<CComponent> > scrolling,headers;
    CList(const char* k):CComponent(k),offsets(0){}
    void InitialiseOffsets(unsigned long n){offsets=n;}
    void AddNonScrollingChild(CCountedPointer<CComponent>& c){headers.push_back(c);}
    void AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>& c){scrolling.push_back(c);}
};
class CViewport:public CComponent {public:
    CViewport():CComponent("UI_SCROLLING_VIEWPORT_TEXT_AREA"){}
    void AddFakeChild(CComponent*){++observed.fakeChildren;}
};
class CSpellContainer:public CComponent {public:
    CSpellContainer():CComponent("UI_SPELL_CONTAINER_TEMPLATE"){}
    void SetEmptySlotDef(unsigned long value){assert(value==900);++observed.emptyDefinitions;}
    void AddChildAndSetOffsets(CCountedPointer<CComponent>& c){AddChild(c);}
};
class CSpellContainerList:public CComponent {public:
    CSpellContainerList():CComponent("UI_SPELL_CONTAINER_LIST_TEMPLATE"){}
    void InitialiseOffsets(unsigned long n){observed.offsets=n;}
    void AddSpellSelectionChild(CCountedPointer<CComponent>& c){AddChild(c);}
    void AddSpellIndicationChild(CCountedPointer<CComponent>& c){AddChild(c);}
    void AddChildAndSetOffsets(CCountedPointer<CComponent>& c){
        ++observed.containers;
        for(unsigned int n=0;n<c->children.size();++n){CSprite* s=static_cast<CSprite*>(c->children[n].Get());observed.graphics[observed.spellCount++]=s->graphic;}
        AddChild(c);
    }
};
class CUIMiscThingsDef {public:unsigned long TotalSpellsInPalettes,TotalSpellsInContainer;CWideString SpaceSeparator;CUIMiscThingsDef():TotalSpellsInPalettes(6),TotalSpellsInContainer(3),SpaceSeparator("|"){} };
static void append(CComponent* parent,CComponent* child){CCountedPointer<CComponent> p(child);parent->AddChild(p);}
class CManager {public:
    static CManager* GetInstance(){static CManager ui;return &ui;}
    CComponent* CreateComponent(const CCharString& s,bool flag){
        assert(!flag);const char* name=s.value;
        if(strcmp(name,"UI_SUB_LIST")==0)return new CList(name);
        if(strcmp(name,"UI_SPELL_CONTAINER_LIST_TEMPLATE")==0)return new CSpellContainerList;
        if(strcmp(name,"UI_SPELL_CONTAINER_TEMPLATE")==0)return new CSpellContainer;
        if(strcmp(name,"UI_SPELL_TEMPLATE")==0||strcmp(name,"UI_SPELL_EMPTY_TEMPLATE")==0)return new CSprite(name);
        if(strcmp(name,"UI_TEXT_TRADE_NAME_TEMPLATE_NEW")==0||strcmp(name,"UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW")==0)return new CText(name);
        if(strcmp(name,"UI_ICON_TRADE_TEMPLATE")==0)return observed.spriteIcons?static_cast<CComponent*>(new CSprite(name)):new CComponent(name);
        if(strcmp(name,"UI_LIST_MAGIC_ACTIONS")==0){CComponent* c=new CComponent(name);append(c,new CMenuEntry("UI_MENU_ENTRY_MAGIC_ACTION_ASSIGN"));return c;}
        if(strcmp(name,"UI_MAGIC_CATEGORIES")==0){
            CComponent* root=new CComponent(name);
            const char* names[3]={"attack","surround","physical"};
            const char* subs[3]={"UI_ATTACK_SUBTITLE","UI_SURROUND_SUBTITLE","UI_PHYSICAL_SUBTITLE"};
            for(int n=0;n<3;++n){CMenuEntry* entry=new CMenuEntry(names[n]);
                if(observed.texts){CText* title=new CText("title");title->SetValue(CWideString(names[n]),true,true);append(entry,title);CText* sub=new CText(subs[n]);sub->SetValue(CWideString(subs[n]),true,true);append(entry,sub);}
                if(observed.arrows)append(entry,new CComponent("UI_SELECTION_ARROW_RIGHT"));append(root,entry);
            }
            if(observed.descriptions){append(root,new CComponent("UI_ATTACK_SPELLS_DESC"));append(root,new CComponent("UI_PHYSICAL_SPELLS_DESC"));append(root,new CComponent("UI_SURROUND_SPELLS_DESC"));}
            return root;
        }
        return new CComponent(name);
    }
    CComponent* CreateMenuEntryComponent(const CWideString& caption,float x,float y,const CCharString& kind,float a,CCharString empty,float b,CCharString button){
        assert(x==0&&y==1&&a==0&&b==0&&empty.value[0]==0&&strcmp(button.value,"PC_BUTTON_BASTARD")==0);
        assert(strcmp(kind.value,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW")==0);
        CMenuEntry* entry=new CMenuEntry(kind.value);strcpy(entry->caption,caption.value);return entry;
    }
};
}
class CGameDefinitionManager {public:static CGameDefinitionManager& Get(){static CGameDefinitionManager d;return d;}long GetDefGlobalIndexFromName(const CCharString& name)const{assert(strcmp(name.value,"UI_SPELL_EMPTY_TEMPLATE")==0);return 900;} };
class CTCInventoryExperience {public:CWideString GetExperienceAbilityName(long n){char value[64];sprintf(value,"Ability%ld",n);return CWideString(value);} };
class CThing {public:CTCInventoryExperience experience;bool GetTC(ETCInterfaceType type,CTCInventoryExperience** out){assert(type==TCI_EXPERIENCE);if(!observed.experience)return false;*out=&experience;return true;} };
class CTCInventoryAbilities {public:
    enum EUISpellCategory { SPELL_CATEGORY_ATTACK=0, SPELL_CATEGORY_SURROUND=1, SPELL_CATEGORY_PHYSICAL=2 };
    CThing* ParentThing;
    NUISystem::CUIMiscThingsDef misc;
    CHeroAbilityDef defs[20];
    std::vector<std::pair<long,CDefPointer<const CHeroAbilityDef> > > Abilities;
    std::vector<EHeroAbility> ReceivedAbilityOrder,AssignedAbilities,AssignedAbilitiesSave;
    std::map<EUISpellCategory,NUISystem::CComponent*> CategoriesLists;
    std::map<EHeroAbility,NUISystem::CComponent*> UnassignedIcons,AbilitiesMenuEntries;
    CTCInventoryAbilities(CThing* parent):ParentThing(parent){
        for(long n=0;n<20;++n){defs[n].IconGraphicIndex=100+n;for(long level=0;level<4;++level)defs[n].LevDescription.push_back(1000+n*10+level);Abilities.push_back(std::pair<long,CDefPointer<const CHeroAbilityDef> >(-1,CDefPointer<const CHeroAbilityDef>(&defs[n])));}
    }
    const CMainGameComponent& PeekComponent()const{return game;}
    const CDefPointer<const NUISystem::CUIMiscThingsDef> GetMiscThingsDef()const{return CDefPointer<const NUISystem::CUIMiscThingsDef>(&misc);}
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility n)const{return Abilities[n].second;}
    EUISpellCategory GetCategory(EHeroAbility n){return static_cast<EUISpellCategory>((static_cast<long>(n)-1)%3);}
    NUISystem::CComponent* GetMenuEntryFromCategory(EUISpellCategory category,NUISystem::CComponent* root){const char* names[3]={"attack","surround","physical"};return root->FindChild(names[category]);}
    NUISystem::CComponent* GetMagicListUIComponent(long& count,NUISystem::CComponent* owner);
};
// Complete behavioral reconstruction of005eeff2; not selected until review and compile validation.
NUISystem::CComponent* CTCInventoryAbilities::GetMagicListUIComponent(long& count,NUISystem::CComponent* owner)
{
    const unsigned short emptyWide[]={0};
    const unsigned short spaceWide[]={0x20,0};
    const unsigned short levelPrefix[]={0x20,0x2d,0x20,0};
    const NGameText::CDataBank& bank=PeekComponent().GetTextBank();
    std::list<EHeroAbility> owned;
    NUISystem::CManager* ui=NUISystem::CManager::GetInstance();
    const CDefPointer<const NUISystem::CUIMiscThingsDef> misc=GetMiscThingsDef();
    for(long n=1;n<20;++n)
        if(Abilities[n].first>=0) owned.push_back(static_cast<EHeroAbility>(n));
    AssignedAbilities.clear();
    AssignedAbilitiesSave.clear();
    unsigned long received=std::min<unsigned long>(ReceivedAbilityOrder.size(),misc->TotalSpellsInPalettes);
    for(unsigned long n=0;n<received;++n) {
        AssignedAbilities.push_back(ReceivedAbilityOrder[n]);
        AssignedAbilitiesSave.push_back(ReceivedAbilityOrder[n]);
    }
    unsigned long assigned=AssignedAbilities.size();
    if(assigned<misc->TotalSpellsInPalettes) {
        EHeroAbility empty=static_cast<EHeroAbility>(0);
        EHeroAbility savedEmpty=static_cast<EHeroAbility>(0);
        for(unsigned long n=0;n<misc->TotalSpellsInPalettes-assigned;++n) {
            AssignedAbilities.push_back(empty);
            AssignedAbilitiesSave.push_back(savedEmpty);
        }
    }
    for(unsigned long n=0;n<AssignedAbilities.size() && n<AssignedAbilitiesSave.size();++n) {
        if(n%misc->TotalSpellsInContainer==1 && n<AssignedAbilities.size()-1 && n<AssignedAbilitiesSave.size()-1) {
            EHeroAbility swapped=AssignedAbilities[n];
            AssignedAbilities[n]=AssignedAbilities[n+1];
            AssignedAbilities[n+1]=swapped;
            swapped=AssignedAbilitiesSave[n];
            AssignedAbilitiesSave[n]=AssignedAbilitiesSave[n+1];
            AssignedAbilitiesSave[n+1]=swapped;
        }
    }
    CCountedPointer<NUISystem::CComponent> palette(ui->CreateComponent("UI_SPELL_CONTAINER_LIST_TEMPLATE",false));
    CCountedPointer<NUISystem::CComponent> container(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE",false));
    CCountedPointer<NUISystem::CComponent> selection(ui->CreateComponent("UI_SPELL_SELECTION_TEMPLATE",false));
    CCountedPointer<NUISystem::CComponent> indication(ui->CreateComponent("UI_UNSELECT_FOR_MAGIC_ASSIGNMENT_LIST",false));
    NUISystem::CSpellContainerList* paletteList=static_cast<NUISystem::CSpellContainerList*>(palette.Get());
    paletteList->InitialiseOffsets(AssignedAbilities.size()/misc->TotalSpellsInContainer+1);
    paletteList->AddSpellSelectionChild(selection);
    paletteList->AddSpellIndicationChild(indication);
    static_cast<NUISystem::CSpellContainer*>(container.Get())->SetEmptySlotDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));
    std::vector<EHeroAbility> reordered(AssignedAbilities.size());
    for(unsigned long n=0;n<AssignedAbilities.size();++n) {
        unsigned long row=n/misc->TotalSpellsInContainer;
        unsigned long column=n%misc->TotalSpellsInContainer;
        reordered[AssignedAbilities.size()-(row+1)*misc->TotalSpellsInContainer+column]=AssignedAbilities[n];
    }
    unsigned long paletteSize=reordered.size();
    for(unsigned long n=0;n<paletteSize;++n) {
        if(n%misc->TotalSpellsInContainer==0 && n!=0) {
            paletteList->AddChildAndSetOffsets(container);
            container.Reset(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE",false));
            static_cast<NUISystem::CSpellContainer*>(container.Get())->SetEmptySlotDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));
        }
        CCountedPointer<NUISystem::CComponent> spell;
        if(reordered[n]!=static_cast<EHeroAbility>(0)) {
            spell.Reset(ui->CreateComponent("UI_SPELL_TEMPLATE",false));
            static_cast<NUISystem::CSprite*>(spell.Get())->SetGraphicIndex(Abilities[reordered[n]].second->IconGraphicIndex);
        } else spell.Reset(ui->CreateComponent("UI_SPELL_EMPTY_TEMPLATE",false));
        static_cast<NUISystem::CSpellContainer*>(container.Get())->AddChildAndSetOffsets(spell);
    }
    paletteList->AddChildAndSetOffsets(container);
    CCountedPointer<NUISystem::CComponent> actions(NUISystem::CManager::GetInstance()->CreateComponent("UI_LIST_MAGIC_ACTIONS",false));
    NUISystem::CComponent* assignment=actions->FindChild("UI_MENU_ENTRY_MAGIC_ACTION_ASSIGN");
    CCountedPointer<NUISystem::CAction> exclusive(new NUISystem::CAction);
    exclusive->ActionType=NUISystem::UI_ACTION_TYPE_GIVE_EXCLUSIVE_INPUT;
    CCountedPointer<NUISystem::CAction> show(new NUISystem::CAction);
    show->ActionType=NUISystem::UI_ACTION_TYPE_SHOW_SPELL_SELECTION;
    CCountedPointer<NUISystem::CAction> set(new NUISystem::CAction);
    set->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN;
    assignment->AddAction(exclusive);
    assignment->AddAction(show);
    assignment->AddAction(set);
    CategoriesLists.clear();
    unsigned long attackCount=0,physicalCount=0,surroundCount=0;
    for(std::list<EHeroAbility>::iterator it=owned.begin();it!=owned.end();++it) {
        EUISpellCategory category=GetCategory(*it);
        if(category==SPELL_CATEGORY_SURROUND) ++surroundCount;
        if(category==SPELL_CATEGORY_PHYSICAL) ++physicalCount;
        if(category==SPELL_CATEGORY_ATTACK) ++attackCount;
    }
    const EUISpellCategory categoryOrder[3]={SPELL_CATEGORY_SURROUND,SPELL_CATEGORY_ATTACK,SPELL_CATEGORY_PHYSICAL};
    const unsigned long categoryCounts[3]={surroundCount,attackCount,physicalCount};
    const char* categoryTitles[3]={"UI_SURROUND_CATEGORIES_TITLE","UI_ATTACK_CATEGORIES_TITLE","UI_PHYSICAL_CATEGORIES_TITLE"};
    for(unsigned long n=0;n<3;++n) {
        if(categoryCounts[n]>0) {
            NUISystem::CList* list=static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST",false));
            list->InitialiseOffsets(categoryCounts[n]);
            CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent("UI_RING_PIC_SPELL_ASSIGNMENT",false));
            list->AddNonScrollingChild(ring);
            CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent(categoryTitles[n],false));
            list->AddNonScrollingChild(title);
            CCountedPointer<NUISystem::CComponent> button(ui->CreateComponent("UI_BACK_FOR_LIST",false));
            list->AddNonScrollingChild(button);
            button.Reset(ui->CreateComponent("UI_SELECT_FOR_LIST",false));
            list->AddNonScrollingChild(button);
            CategoriesLists[categoryOrder[n]]=list;
        }
    }
    count=physicalCount+attackCount+surroundCount;
    NUISystem::CComponent* root=ui->CreateComponent("UI_MAGIC_CATEGORIES",false);
    NUISystem::CViewport* viewport=static_cast<NUISystem::CViewport*>(owner->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    if(viewport) {
        NUISystem::CComponent* description=root->FindChild("UI_ATTACK_SPELLS_DESC");
        if(description) viewport->AddFakeChild(description);
        description=root->FindChild("UI_PHYSICAL_SPELLS_DESC");
        if(description) viewport->AddFakeChild(description);
        description=root->FindChild("UI_SURROUND_SPELLS_DESC");
        if(description) viewport->AddFakeChild(description);
    }
    UnassignedIcons.clear();
    AbilitiesMenuEntries.clear();
    for(std::list<EHeroAbility>::iterator it=owned.begin();it!=owned.end();++it) {
        CWideString caption,displayName;
        CTCInventoryExperience* experience;
        if(ParentThing->GetTC(static_cast<ETCInterfaceType>(22),&experience)) {
            displayName=experience->GetExperienceAbilityName(*it);
            caption=displayName;
            caption+=misc->SpaceSeparator;
            caption+=GFIntToWideString(Abilities[*it].first+1);
            displayName+=CWideString(levelPrefix)+bank.GetTextBySymbol("TEXT_GUI_MENU_LEVEL")+CWideString(spaceWide);
            displayName+=GFIntToWideString(Abilities[*it].first+1);
        }
        CCountedPointer<NUISystem::CComponent> entry(ui->CreateMenuEntryComponent(caption,0.0f,1.0f,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        AbilitiesMenuEntries[*it]=entry.Get();
        bool isAssigned=false;
        for(std::vector<EHeroAbility>::iterator a=AssignedAbilities.begin();a!=AssignedAbilities.end();++a)
            if(*a==*it) { isAssigned=true;break; }
        if(!isAssigned) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_UNASSIGNED_SPELL_ICON",false));
            entry->AddChild(icon);
            UnassignedIcons[*it]=icon.Get();
        }
        CCountedPointer<NUISystem::CMenuEntry> menuEntry;
        menuEntry.SetStaticCast(entry);
        entry->AddChild(palette);
        menuEntry->AddUIActionParam(new NUISystem::CActionParamSLONG(*it));
        menuEntry->AddUIActionParam(new NUISystem::CActionParamULONG(Abilities[*it].second->IconGraphicIndex));
        menuEntry->AddUIActionParam(palette.Get());
        CCountedPointer<NUISystem::CComponent> name(ui->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW",false));
        CCountedPointer<NUISystem::CText> text;
        text.SetStaticCast(name);
        text->SetValue(displayName,true,true);
        entry->AddChild(name);
        CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_ICON_TRADE_TEMPLATE",false));
        if(icon->GetType()==static_cast<NUISystem::EType>(0))
            static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        entry->AddChild(icon);
        entry->AddChild(actions);
        CCountedPointer<NUISystem::CComponent> description(ui->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW",false));
        static_cast<NUISystem::CText*>(description.Get())->SetValue(GetAbilityPDef(*it)->LevDescription[Abilities[*it].first],CWideString(emptyWide),true);
        entry->AddChild(description);
        if(viewport) viewport->AddFakeChild(description.Get());
        static_cast<NUISystem::CList*>(CategoriesLists[GetCategory(*it)])->AddScrollingChildAndSetOffsets(entry);
    }
    const char* subtitles[3]={"UI_ATTACK_SUBTITLE","UI_SURROUND_SUBTITLE","UI_PHYSICAL_SUBTITLE"};
    const unsigned long counts[3]={attackCount,surroundCount,physicalCount};
    for(long n=0;n<3;++n) {
        EUISpellCategory category=static_cast<EUISpellCategory>(n);
        std::map<EUISpellCategory,NUISystem::CComponent*>::iterator found=CategoriesLists.find(category);
        if(found!=CategoriesLists.end()) {
            CCountedPointer<NUISystem::CComponent> children(found->second);
            NUISystem::CComponent* entry=GetMenuEntryFromCategory(category,root);
            entry->AddChild(children);
            NUISystem::CText* title=static_cast<NUISystem::CText*>(entry->FindFirstChildOfTypeNonRecursive(static_cast<NUISystem::EType>(6)));
            if(title) title->SetValue(title->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(counts[n]),true,true);
            NUISystem::CText* subtitle=static_cast<NUISystem::CText*>(entry->FindChild(subtitles[n]));
            if(subtitle) subtitle->SetValue(subtitle->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(counts[n]),true,true);
        } else {
            NUISystem::CComponent* entry=GetMenuEntryFromCategory(category,root);
            entry->MakeUnavailable();
            NUISystem::CComponent* arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if(arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method=NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    return root;
}
static void runCase(int scenario)
{
    observed=Observation();
    observed.experience=scenario!=2;
    observed.viewport=scenario!=3;
    observed.descriptions=scenario!=4;
    observed.texts=scenario!=4;
    observed.spriteIcons=scenario!=3;
    observed.arrows=scenario!=4;
    {
        NUISystem::CComponent owner;
        if(observed.viewport)NUISystem::append(&owner,new NUISystem::CViewport);
        CThing thing;
        CTCInventoryAbilities inventory(&thing);
        if(scenario==5)inventory.misc.TotalSpellsInPalettes=0;
        const int countsByScenario[6]={0,6,2,2,1,3};
        const int choices[6][6]={{0,0,0,0,0,0},{1,2,3,4,5,6},{1,4,0,0,0,0},{2,3,0,0,0,0},{1,0,0,0,0,0},{1,2,3,0,0,0}};
        const int owned=countsByScenario[scenario];
        long expectedCategory[3]={0,0,0};
        for(int n=0;n<owned;++n){int ability=choices[scenario][n];inventory.Abilities[ability].first=(ability-1)%3;++expectedCategory[(ability-1)%3];}
        if(scenario==1){const int order[7]={1,2,3,4,5,0,7};for(int n=0;n<7;++n)inventory.ReceivedAbilityOrder.push_back(static_cast<EHeroAbility>(order[n]));}
        if(scenario==5)inventory.ReceivedAbilityOrder.push_back(static_cast<EHeroAbility>(1));
        long count=-123;
        NUISystem::CComponent* root=inventory.GetMagicListUIComponent(count,&owner);
        assert(count==owned && strcmp(root->kind,"UI_MAGIC_CATEGORIES")==0);
        unsigned long total=inventory.misc.TotalSpellsInPalettes;
        assert(inventory.AssignedAbilities.size()==total && inventory.AssignedAbilitiesSave.size()==total);
        const int swapped[6]={1,3,2,4,0,5};
        for(unsigned long n=0;n<total;++n){int expected=scenario==1?swapped[n]:0;assert(inventory.AssignedAbilities[n]==expected && inventory.AssignedAbilitiesSave[n]==expected);}
        if(scenario==1)assert(inventory.ReceivedAbilityOrder.size()==7&&inventory.ReceivedAbilityOrder[6]==7);
        assert(observed.offsets==total/3+1);
        assert(observed.containers==(total==0?1:2) && observed.spellCount==(int)total && observed.emptyDefinitions==observed.containers);
        const int reorderedGraphics[6]={104,-1,105,101,103,102};
        for(unsigned long n=0;n<total;++n)assert(observed.graphics[n]==(scenario==1?reorderedGraphics[n]:-1));
        assert(inventory.AbilitiesMenuEntries.size()==(unsigned long)owned);
        assert(inventory.UnassignedIcons.size()==(unsigned long)(scenario==1?1:owned));
        if(scenario==1)assert(inventory.UnassignedIcons.find(static_cast<EHeroAbility>(6))!=inventory.UnassignedIcons.end());
        const char* names[3]={"attack","surround","physical"};
        const char* subtitles[3]={"UI_ATTACK_SUBTITLE","UI_SURROUND_SUBTITLE","UI_PHYSICAL_SUBTITLE"};
        const char* titleKinds[3]={"UI_ATTACK_CATEGORIES_TITLE","UI_SURROUND_CATEGORIES_TITLE","UI_PHYSICAL_CATEGORIES_TITLE"};
        for(int category=0;category<3;++category){
            NUISystem::CComponent* entry=root->FindChild(names[category]);
            assert(entry->unavailable==(expectedCategory[category]==0));
            NUISystem::CComponent* arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT");if(observed.arrows)assert(arrow->deleted==(expectedCategory[category]==0));else assert(!arrow);
            if(expectedCategory[category]){
                NUISystem::CList* list=static_cast<NUISystem::CList*>(entry->FindChild("UI_SUB_LIST"));
                assert(list && list->offsets==(unsigned long)expectedCategory[category] && list->scrolling.size()==(unsigned long)expectedCategory[category]);
                assert(list->headers.size()==4);
                assert(strcmp(list->headers[0]->kind,"UI_RING_PIC_SPELL_ASSIGNMENT")==0);
                assert(strcmp(list->headers[1]->kind,titleKinds[category])==0);
                assert(strcmp(list->headers[2]->kind,"UI_BACK_FOR_LIST")==0 && strcmp(list->headers[3]->kind,"UI_SELECT_FOR_LIST")==0);
                if(observed.texts){char expected[256];sprintf(expected,"%s x%ld",names[category],expectedCategory[category]);assert(strcmp(entry->FindChild("title")->caption,expected)==0);sprintf(expected,"%s x%ld",subtitles[category],expectedCategory[category]);assert(strcmp(entry->FindChild(subtitles[category])->caption,expected)==0);}
            }
        }
        NUISystem::CComponent* sharedPalette=0;
        NUISystem::CComponent* sharedActions=0;
        for(int n=0;n<owned;++n){
            int ability=choices[scenario][n];long level=(ability-1)%3;
            NUISystem::CMenuEntry* entry=static_cast<NUISystem::CMenuEntry*>(inventory.AbilitiesMenuEntries[static_cast<EHeroAbility>(ability)]);
            char expected[256];expected[0]=0;if(observed.experience)sprintf(expected,"Ability%d|%ld",ability,level+1);assert(strcmp(entry->caption,expected)==0);
            assert(entry->params.size()==3 && entry->params[0]->value==ability && entry->params[1]->value==100+ability && entry->params[2]->component);
            NUISystem::CComponent* palette=entry->FindChild("UI_SPELL_CONTAINER_LIST_TEMPLATE");assert(palette && entry->params[2]==static_cast<NUISystem::CActionParam*>(palette));if(sharedPalette)assert(sharedPalette==palette);sharedPalette=palette;
            expected[0]=0;if(observed.experience)sprintf(expected,"Ability%d - TEXT_GUI_MENU_LEVEL %ld",ability,level+1);assert(strcmp(entry->FindChild("UI_TEXT_TRADE_NAME_TEMPLATE_NEW")->caption,expected)==0);
            NUISystem::CText* description=static_cast<NUISystem::CText*>(entry->FindChild("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW"));assert(description->textId==(unsigned long)(1000+ability*10+level) && description->firstFlag);
            NUISystem::CComponent* icon=entry->FindChild("UI_ICON_TRADE_TEMPLATE");assert(icon);if(observed.spriteIcons)assert(static_cast<NUISystem::CSprite*>(icon)->graphic==100+ability);
            NUISystem::CComponent* actions=entry->FindChild("UI_LIST_MAGIC_ACTIONS");assert(actions);if(sharedActions)assert(sharedActions==actions);sharedActions=actions;
            NUISystem::CComponent* assignment=actions->FindChild("UI_MENU_ENTRY_MAGIC_ACTION_ASSIGN");assert(assignment->actions.size()==3);assert(assignment->actions[0]->ActionType==202&&assignment->actions[1]->ActionType==205&&assignment->actions[2]->ActionType==206);
        }
        assert(observed.fakeChildren==(observed.viewport?owned+(observed.descriptions?3:0):0));
        delete root;
    }
    assert(live_components==0 && live_actions==0 && live_params==0 && control_blocks_alive==0);
}
static void runAll(){for(int n=0;n<6;++n)runCase(n);}
}
int main(){
    check(false,false,0,0);check(true,false,3,0);check(false,true,0,2);check(true,true,4,5);
    check(true,true,0,0);check(true,true,-1,-2);
    assert(control_blocks_alive==0);MagicTest::runAll();
    puts("005f519f_TEST PASS");return 0;
}