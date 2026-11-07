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
#ifdef FABLE_RETAIL_STLP_FASTCALL_DESTROY_AUX
// Opt-in (chunk 855, row 0x00507070 CWorldMap::BuildStaticMapGenerationVisibilityInfo): the retail TU's non-trivial
// __destroy_aux instantiations are __fastcall (first in ecx, last in edx, the tag pushed): 0x00519EB0 (CSeaBodyDesc) and
// 0x0051AC70 (CMapDesc) are called that way where VC7.1 leaves them out of line, and expand where it inlines them.
template<class I> inline void __fastcall __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
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
#ifdef FABLE_RETAIL_STLP_VECTOR_SWAP
// Retail opt-in (chunk 849, row 0x00AE2E20 CParticleProgram::OptimiseSpringOrder): STLport 4.6 _algobase.h swap(_Tp&, _Tp&), used by
// the inline vector::swap below (retail expands `particle_list.swap(next_particle_list)` in place: the words exchanged through
// registers at 0x00AE30C7..0x00AE30DC, then the old storage freed at 0x00AE30E3; no call).
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
    // 0x0075B8D0) but, seeing that it never captures its argument, keeps the caller's local start pointer in a register across
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
    // VC7.1 then knows resize does not keep the vector's address, holds the start in a register across the later calls and reuses
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
#ifdef FABLE_RETAIL_STLP_VECTOR
// Retail branch (pass 12 header wave): STLport 4.6 _vector.h includes _vector.c, so every retail TU that calls resize() /
// insert(pos, n, x) saw _M_fill_insert's body (stl/_vector.c lines 61-86, reproduced here) and VC7.1 still kept the call out of
// line. The visible body changes the caller's register allocation: 0x00AC2BE0 PartitionSpace matches only with it (2775v2791
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
};
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
    ~list() { }
    _Alloc get_allocator() const { return _Alloc(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
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
    iterator insert(iterator position,const _Tp& x) {
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
    // cmp ecx,eax`, 0x0057EBA3, before the vector reserve); the for-loop form above swaps the two registers.
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
// _bitset.h / _range_errors.h (no-exceptions branch)
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
    // Per-TU opt-in (plan phase 3, owner-approved 2026-11-04): STLport 4.6 _bitset.h test(), declared only. In the size-optimised
    // TUs retail calls the out-of-line std::bitset<274>::test at 0x00410DE0 (63 bytes, ?test@?$bitset@$0BBC@@std@@QBE_NI@Z; 337
    // landed callers bind it) instead of expanding the range check and the word test inline.
    bool test(size_t n) const;
#else
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
#endif
#ifdef FABLE_RETAIL_STLP_BITSET_INDEX
    // Per-TU opt-in (plan phase 4, 2026-11-05): STLport 4.6 _bitset.h bitset<_Nb>::reference and operator[]. Retail expands the
    // indexed bit test inline as a word test on the bitset, with no call: 0x0062D032 `test dword ptr [ebx+0x24],0x10000`
    // (CWorld::EACreatureBlock, CThing::UsingComponents +0x20 bit 0x30) and 0x0044BFA1 `mov al,byte ptr [ecx+0x38]; test al,al;
    // jns` (bit 0xC7). The rows that index the bitset define the macro; the non-const operator[] goes through the reference
    // proxy (operator bool), the const one through _Unchecked_test, as in STLport.
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
// _function_base.h / _algo.h
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
// Retail opt-in (chunk 796, row 0x008E18D0): the std::sort instantiations are __fastcall in retail (first in ecx, last in edx, the
// comparator pushed: 0x0076C000 / 0x0076C1A0 are bound as ??$sort@...@std@@YIX...; the landed 0x00767DA0 local model spells the
// same). A declaration only.
template<class _RandomAccessIter,class _Compare> void __fastcall sort(_RandomAccessIter,_RandomAccessIter,_Compare);
#else
template<class _RandomAccessIter,class _Compare> void sort(_RandomAccessIter,_RandomAccessIter,_Compare);
#endif
#ifdef FABLE_RETAIL_STLP_SORT2
// Retail opt-in (chunk 849, row 0x00AE2E20): STLport 4.6 _algo.h `template <class _RandomAccessIter> void sort(_RandomAccessIter,
// _RandomAccessIter)`, out of line in retail (sort<pair<long,unsigned long>*> 0x00AE7DC0, first in ecx, last in edx under /Gr).
// A declaration only.
template<class _RandomAccessIter> void sort(_RandomAccessIter,_RandomAccessIter);
#endif
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
#ifdef FABLE_RETAIL_STLP_BACK_INSERTER
// Per-TU opt-in (chunk 849, row 0x0057E2F1 CTCHeroStats::OnSerialise; the landed 0x007B3760 CTCGuard::OnSerialise models the same
// locally): STLport 4.6 _iterator.h back_insert_iterator / back_inserter, and _algobase.h copy() declared only. Retail calls the copy
// instantiations out of line as __fastcall with the result through a hidden pointer in ecx (0x0057EBDE -> 0x00581376 list to
// back_insert_iterator<vector>, 0x0057EC1C -> 0x005813A0 pointer range in edx to back_insert_iterator<list>).
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
}
#endif
