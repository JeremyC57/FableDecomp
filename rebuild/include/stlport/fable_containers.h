#ifdef FABLE_RETAIL_STLP_BOOL_FILL_OWNER_PASS26
#include "fable_compiler.h"
#endif
#ifdef FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL
#include "fable_compiler.h"
#endif
// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H
// STLport 4.6.2 excerpts: stl/type_traits.h, _iterator_base.h, _algobase.h, _construct.h, _uninitialized.h,
// _alloc.h, _vector.h/_vector.c, _pair.h, _list.h, _bitset.h, _range_errors.h and _function_base.h.
// One model for every translation unit (Codex pass 11 consolidation): original names, keys and inheritance;
// the members retail keeps out of line (_M_fill_insert, list::_M_insert_dispatch, ~list, vector copy
// constructor) are declared without a body here, as everywhere else in rebuild/include.
#if defined(FABLE_RETAIL_STLP_VECTOR_CHECKED_SUBSCRIPT_PASS26) && defined(_DEBUG)
#include <assert.h>
#endif
#include <stddef.h>
#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#include "fable_compiler.h"
#endif
#ifdef FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "fable_compiler.h"
#endif
#ifdef FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
// Per-TU inline control (agentNF54, row 0x005BC0D3 ~vector<CEngineTextureMorph>): the size-optimised retail TU expands the range
// _Destroy -> __destroy dispatch (only the /Gr __destroy_aux COMDAT 0x005BBF80 is called); under /Os VC7.1 otherwise calls _Destroy
// out of line. The same FABLE_FORCEINLINE as FABLE_RETAIL_AUGMENTATION_PARTICLES applies to those two wrappers, nothing else.
#include "fable_compiler.h"
#endif
#if defined(FABLE_RETAIL_STLP_CUISTATE_MAP_GETSIZE_OUTOFLINE) || defined(FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE) || defined(FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE) || defined(FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE) || defined(FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE) || defined(FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE) || defined(FABLE_RETAIL_STLP_SORT_INTERNALS) || defined(FABLE_RETAIL_STLP_HEAP) || defined(FABLE_RETAIL_STLP_INTROSORT_LOOP) || defined(FABLE_RETAIL_STLP_LIST_SORT) || defined(FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22)
#ifdef _MSC_VER
#define FABLE_STLP_NOINLINE __declspec(noinline)
#define FABLE_STLP_FASTCALL __fastcall
#else
#define FABLE_STLP_NOINLINE
#define FABLE_STLP_FASTCALL
#endif
#endif
#if (defined(FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT_NOINLINE) || defined(FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE)) && !defined(FABLE_STLP_NOINLINE)
// Opt-ins (chunk Q6, row 0x00A66FD0): the visible-but-out-of-line list insert(iterator) / push_back() / erase(iterator) below.
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
#if !(defined(FABLE_RETAIL_STLP_HOST_TAGS_PASS23) && defined(_CPP_TYPE_TRAITS_H) && !defined(_MSC_VER))
struct __true_type { __true_type() { } };
#ifdef FABLE_RETAIL_STLP_FALSE_TAG_POD
// Retail opt-in (Codex 2026-11-09): original STLport4.6.2 type_traits.h line65.
// 00b803e9 writes a zero byte for the value-initialised tag argument.
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
#endif // Host libstdc++ already supplies these two empty dispatch tags.
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#ifdef FABLE_RETAIL_STLP_FIND_IF_INPUT
// Original STLport _iterator_base.h metadata, required by the search dispatch.
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
#ifdef FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
// Original iterator_traits<const _Tp*> removes const from value_type.
template<class T> struct iterator_traits<const T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
};
#endif
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
#ifdef FABLE_RETAIL_STLP_NATIVE_WCHAR_TRAITS
// Original STLport scalar traits for native wchar_t (this TU uses /Zc:wchar_t).
FABLE_STLP_SCALAR_TRAITS(wchar_t)
#endif
FABLE_STLP_SCALAR_TRAITS(int)
FABLE_STLP_SCALAR_TRAITS(unsigned int)
FABLE_STLP_SCALAR_TRAITS(long)
FABLE_STLP_SCALAR_TRAITS(unsigned long)
// The native compiler has two additional scalar integer types. ISO C++03 hosts use the scalar specializations above.
#if defined(_MSC_VER) || !defined(FABLE_RETAIL_STLP_ISO_SCALAR_TRAITS_PASS26)
FABLE_STLP_SCALAR_TRAITS(__int64)
FABLE_STLP_SCALAR_TRAITS(unsigned __int64)
#endif
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
#ifdef FABLE_RETAIL_STLP_MAX_FASTCALL
// Retail opt-in (Codex pass 17): original std::max<long> fastcall,
// FableWin 018ACFBC and its caller 018A801D; otherwise identical STLport body.
template<class T> inline const T& FABLE_FASTCALL max(const T& a,const T& b) { return a<b?b:a; }
#else
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
#endif
#ifdef FABLE_RETAIL_STLP_MIN_FASTCALL
// FableWin std::min<long> 0x01DE6FE0 and the CHeightMap::Resize call site use the original fastcall template.
template<class T> inline const T& FABLE_FASTCALL min(const T& a,const T& b) { return b<a?b:a; }
#else
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#endif
#ifdef FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
// Original STLport4.6.2 type_traits.h pointer/copy dispatch, partial-specialization path.
template<int _Is> struct __bool2type { typedef __false_type _Ret; };
template<> struct __bool2type<1> { typedef __true_type _Ret; };
template<> struct __bool2type<0> { typedef __false_type _Ret; };
template<class _P1,class _P2,class _P3> struct _Land3 { typedef __false_type _Ret; };
template<> struct _Land3<__true_type,__true_type,__true_type> { typedef __true_type _Ret; };
template<class _Tp1,class _Tp2> struct _IsSame { enum { _Ret=0 }; };
template<class _Tp> struct _IsSame<_Tp,_Tp> { enum { _Ret=1 }; };
template<class _Tp1,class _Tp2> struct _BothPtrType { static __false_type _Ret() { return __false_type(); } };
template<class _Tp1,class _Tp2> struct _BothPtrType<_Tp1*,_Tp2*> { static __true_type _Ret() { return __true_type(); } };
template<class _Tp1,class _Tp2> struct _OKToMemCpy {
    enum { _Same=_IsSame<_Tp1,_Tp2>::_Ret };
    typedef typename __type_traits<_Tp1>::has_trivial_assignment_operator _Tr1;
    typedef typename __type_traits<_Tp2>::has_trivial_assignment_operator _Tr2;
    typedef typename __bool2type<_Same>::_Ret _Tr3;
    typedef typename _Land3<_Tr1,_Tr2,_Tr3>::_Ret _Type;
    static _Type _Ret() { return _Type(); }
};
template<class _Tp1,class _Tp2> inline _OKToMemCpy<_Tp1,_Tp2> _IsOKToMemCpy(_Tp1*,_Tp2*) { return _OKToMemCpy<_Tp1,_Tp2>(); }
#endif
// _algobase.h
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#ifdef FABLE_RETAIL_STLP_FILL_CHAR
// Retail opt-in (chunk 1013b, row 0x00507650 CWorldMap::LoadWorldFromBinaryFile): STLport 4.6 _algobase.h's plain-char fill /
// fill_n overloads (memset), next to the unsigned char pair above. vector<char>(n) then fills through an inline memset as retail
// does (0x005076A4 `shr ecx,2 ; rep stosd ; and ecx,3 ; rep stosb`) and VC7.1 expands the constructor; the generic loop made it
// too costly to inline (an out-of-line ??0?$vector@DV?$allocator@D@std@@@std@@QAE@I@Z call).
inline void fill(char* first,char* last,const char& value) {
    char tmp=value;memset(first,(unsigned char)tmp,last-first);
}
template<class Size> inline char* fill_n(char* first,Size n,const char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
#ifdef FABLE_RETAIL_STLP_FILL_LOCAL_ITERATOR
template<class I,class T> inline void fill(I begin,I last,const T& x) { I first=begin; for(;first!=last;++first) *first=x; }
#elif defined(FABLE_RETAIL_STLP_FILL_NOINLINE) && defined(_MSC_VER)
// NF68 opt-in (row 0x00AD85D0): the same STLport body, kept out of line as a __fastcall COMDAT (0x00AD78D0: first in ecx, last in
// edx, `ret 4`), as that retail TU calls it.
#pragma auto_inline(off)
template<class I,class T> __declspec(noinline) void __fastcall fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
#pragma auto_inline(on)
#elif defined(FABLE_RETAIL_STLP_FILL_FASTCALL)
// Opt-in (egod e07, row 0x00681E60): STLport 4.6 _algobase.h generic fill as retail's __fastcall instance ??$fill@PA_N_N@std@@YIXPA_N0AB_N@Z
// (0x00685430, first / last in ecx / edx, the value by reference on the stack; reloc_bindings). Same body.
template<class I,class T> inline void FABLE_FASTCALL fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
#else
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
#endif
#ifdef FABLE_RETAIL_STLP_FILL_N_FASTCALL
// Original STLport generic fill_n; native pointer instance 0x0048514C receives first/count in ECX/EDX.
template<class I,class N,class T> inline I FABLE_FASTCALL fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
#else
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT
// (row 0x00523CF0 opt-in) STLport 4.6 _algobase.h __copy for input iterators (the tree-iterator copy of _M_range_insert), and
// declarations of the _iterator_base.h distance / advance defined further down.
template<class _InputIterator> inline ptrdiff_t distance(const _InputIterator& first,const _InputIterator& last);
template<class _InputIterator,class _Distance> inline void advance(_InputIterator& i,_Distance n);
template<class I,class O,class D> inline O __copy(I first,I last,O result,const input_iterator_tag&,D*) {
    for(;first!=last;++result,++first) *result=*first; return result;
}
#endif
#ifdef FABLE_RETAIL_STLP_COPY_INPUT_NOINLINE
// Per-TU opt-in (NF62, row 0x006C7370 CTCHeroOpinionDeedLog::OnSerialise): STLport 4.6 _algobase.h __copy for input iterators, kept
// out of line as retail calls it (0x006C9B70, `__fastcall`: hidden result in ecx, the two tree iterators by value, the tag reference
// and the (int*)0 distance pushed) for a map range copied into a back_insert_iterator. Not with FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT.
template<class I,class O,class D> __declspec(noinline) O __fastcall __copy(I first,I last,O result,const input_iterator_tag&,D*) {
    for(;first!=last;++result,++first) *result=*first; return result;
}
#endif
#ifdef FABLE_RETAIL_STLP_COPY_NOINLINE
// Per-TU opt-in (row 0x00956060 CAIStateGroup_MazeTutorialBattleTeleport::Update): that TU calls the random-access __copy COMDAT
// (0x0074CA20, landed) from all three inline vector::erase bodies instead of expanding it; the body stays visible, kept out of line.
template<class I,class O,class D> __declspec(noinline) O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*) {
#else
template<class I,class O,class D> inline O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*) {
#endif
    for(D n=last-first;n>0;--n) { *result=*first;++first;++result; } return result;
}
#ifdef FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE
// Original Ego_r / FableWin ?__copy_trivial@std@@YIPAXPBX0PAX@Z, native 0x00405BA0.
void* FABLE_FASTCALL __copy_trivial(const void* first,const void* last,void* result);
#else
#if defined(FABLE_RETAIL_STLP_VECTOR_ULONG_ERASE_COPY_PASS27) && defined(_MSC_VER)
// 00A90AE0: the 52-byte landed vector<unsigned long>::erase owner at 0046496F
// expands this STLport _algobase.h helper along with the POD overload below.
__forceinline void* __copy_trivial(const void* first,const void* last,void* result) {
#else
#if defined(FABLE_RETAIL_STLP_BOOL_VECTOR_SIZE_OWNERS_PASS26) && defined(_MSC_VER)
// Inline-budget: the bool erase owner at 004676BB expands the original POD copy dispatch.
FABLE_FORCEINLINE void* __copy_trivial(const void* first,const void* last,void* result) {
#else
#ifdef FABLE_RETAIL_CTHREADEDFILE_ULONG_OVERFLOW_PASS30
// Inline-budget: the size-built growth owner expands both POD copies.
FABLE_FORCEINLINE void* __copy_trivial(const void* first,const void* last,void* result) {
#else
inline void* __copy_trivial(const void* first,const void* last,void* result) {
#endif
#endif
#endif
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
#endif
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)
template<class Input,class Output> inline Output FABLE_FASTCALL __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
#elif defined(FABLE_RETAIL_STLP_COPY_PTRS_FALSE_NOINLINE) && defined(_MSC_VER)
// NF68 opt-in (row 0x009F6890 vector<CInputEvent>::operator=): the same STLport body, kept out of line as a __fastcall COMDAT
// (0x009F63B0: first in ecx, last in edx, result + tag on the stack, `ret 8`), as that retail TU calls it.
#pragma auto_inline(off)
template<class Input,class Output> __declspec(noinline) Output __fastcall __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
#else
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
#endif
#ifdef FABLE_RETAIL_ANIMATION_BUILDER_LONG_DISTANCE_PASS30
    // The settled 00AB1480 random-access copy uses a signed long distance in the 00AB1F30 caller.
    return __copy(first,last,result,random_access_iterator_tag(),(long*)0);
#else
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
#endif
}
#if defined(FABLE_RETAIL_STLP_COPY_PTRS_FALSE_NOINLINE) && !(defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)) && defined(_MSC_VER)
#pragma auto_inline(on)
#endif
#if defined(FABLE_RETAIL_STLP_VECTOR_ULONG_ERASE_COPY_PASS27) && defined(_MSC_VER)
template<class Input,class Output> __forceinline Output __copy_ptrs(Input first,Input last,Output result,const __true_type&) {
#else
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)
template<class Input,class Output> inline Output FABLE_FASTCALL __copy_ptrs(Input first,Input last,Output result,const __true_type&) {
#else
#ifdef FABLE_RETAIL_WATER_BYTE_RESIZE_SPELLINGS_PASS29
// Inline-budget: range erase at 00485B18 expands POD tag dispatch before __copy_trivial.
template<class Input,class Output> FABLE_FORCEINLINE Output __copy_ptrs(Input first,Input last,Output result,const __true_type&) {
#else
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __true_type&) {
#endif
#endif
#endif
    return (Output)__copy_trivial(first,last,result);
}
#ifdef FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
// Original _algobase.h __copy_aux; preserve the actual type/tag call chain.
template<class I,class O> inline O __copy_aux(I first,I last,O result,const __true_type&) {
    return __copy_ptrs(first,last,result,_IsOKToMemCpy((typename iterator_traits<I>::value_type*)0,(typename iterator_traits<O>::value_type*)0)._Ret());
}
template<class I,class O> inline O __copy_aux(I first,I last,O result,const __false_type&) {
    return __copy(first,last,result,typename iterator_traits<I>::iterator_category(),(typename iterator_traits<I>::difference_type*)0);
}
#endif
#ifdef FABLE_RETAIL_STLP_COPY
// Retail opt-in (wfE8, row 0x00A2F740 C3DMeshStats::InitialiseFromFileData; chunk 868's text): STLport 4.6 _algobase.h copy() for
// pointer ranges, dispatched on the element's trivial-assignment trait: a POD range through __copy_trivial (the inline `last == first`
// test and memmove 0x00BFEAE6 at 0x00A30084), a non-POD one through __copy<I,O,int> (0x00A31150 / 0x00A311B0 for the
// pair<unsigned long, C3DHelperPoint / C3DDummyObject> ranges).
#ifdef FABLE_RETAIL_WATER_BYTE_RESIZE_SPELLINGS_PASS29
// Inline-budget: range erase expands the pointer copy wrapper at 00485B18.
template<class T> FABLE_FORCEINLINE T* copy(T* first,T* last,T* result) {
#else
template<class T> inline T* copy(T* first,T* last,T* result) {
#endif
    return __copy_ptrs(first,last,result,typename __type_traits<T>::has_trivial_assignment_operator());
}
#ifdef FABLE_RETAIL_WATER_BYTE_RESIZE_SPELLINGS_PASS29
// Inline-budget: range erase expands the pointer copy wrapper at 00485B18.
template<class T> FABLE_FORCEINLINE T* copy(const T* first,const T* last,T* result) {
#else
template<class T> inline T* copy(const T* first,const T* last,T* result) {
#endif
    return __copy_ptrs(first,last,result,typename __type_traits<T>::has_trivial_assignment_operator());
}
#endif
#if defined(FABLE_RETAIL_PASS22_SCOREBOARD_COPY_BACKWARD_ORIGINAL) || defined(FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22)
// Actual STLport4.6.2 _algobase.h182..185 and224..226, including empty-range guard.
inline void* __copy_trivial_backward(const void* __first,const void* __last,void* __result) {
    const ptrdiff_t _Num=(const char*)__last-(const char*)__first;
    return (_Num>0)?memmove((char*)__result-_Num,__first,_Num):__result;
}
template<class I,class O> inline O __copy_backward_ptrs(I __first,I __last,O __result,const __true_type&) {
    return (O)__copy_trivial_backward(__first,__last,__result);
}
#else
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
#endif
#ifdef FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
// Retail opt-in (Codex 2026-11-09): STLport4.6.2 _algobase.h lines170-180,220-222.
// Pointer iterators dispatch to the counted random-access loop, not the bidirectional loop.
#ifdef FABLE_RETAIL_STLP_PASS22_COPY_BACKWARD_FASTCALL
// Actual native00B8EF60 has first/last in ECX/EDX andRET12; original STLport loop.
template<class R,class B,class D> inline B FABLE_FASTCALL __copy_backward(R first,R last,B result,const random_access_iterator_tag&,D*) {
#elif defined(FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE) && defined(_MSC_VER)
// NF68 opt-in (row 0x00AD85D0): the same STLport loop, kept out of line as a __fastcall COMDAT (0x00AD7120: first in ecx, last in
// edx, `ret 0Ch`), as that retail TU calls it. (auto_inline is restored after the body below.)
#pragma auto_inline(off)
template<class R,class B,class D> __declspec(noinline) B __fastcall __copy_backward(R first,R last,B result,const random_access_iterator_tag&,D*) {
#else
template<class R,class B,class D> inline B __copy_backward(R first,R last,B result,const random_access_iterator_tag&,D*) {
#endif
    for(D n=last-first;n>0;--n) *--result=*--last;
    return result;
}
#if defined(FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE) && !defined(FABLE_RETAIL_STLP_PASS22_COPY_BACKWARD_FASTCALL) && defined(_MSC_VER)
#pragma auto_inline(on)
#endif
#if defined(FABLE_RETAIL_THEME_FILL_HELPERS_PASS30) && defined(_MSC_VER)
// The pair<long,float> fill-insert owner 00786020 calls the original fastcall instantiation 006B5540.
template<class I,class O> __declspec(noinline) O FABLE_FASTCALL __copy_backward_ptrs(I first,I last,O result,const __false_type&) {
#else
template<class I,class O> inline O __copy_backward_ptrs(I first,I last,O result,const __false_type&) {
#endif
    return __copy_backward(first,last,result,random_access_iterator_tag(),(typename iterator_traits<I>::difference_type*)0);
}
#else
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
// _construct.h
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)
template<class T,class U> inline void FABLE_FASTCALL _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void FABLE_FASTCALL _Destroy(T* p) { p->~T(); }
inline void FABLE_FASTCALL _Destroy(char*,char*) { }
#elif defined(FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL)
// NF87 opt-in (row 0x007AE0C0, the out-of-line _Construct of pair<const CCharString, vector<CCharString> >): the /Gr TU's _construct.h
// _Construct as a __fastcall COMDAT (p in ecx, the value in edx; the placement-new null test, then the pair's two copy constructors).
// The default branch's text with __fastcall on _Construct only.
template<class T,class U> inline void __fastcall _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#elif defined(FABLE_RETAIL_STLP_CONSTRUCT_BYVAL)
// Per-TU opt-in (row 0x00754050 CTCLook::UpdateTurningAnims): _Construct taking its value BY VALUE. The inlined push_back then
// copies the element out of the argument temporary into register-held fields before the placement store (retail 0x007541E0:
// `mov ecx,edx ; mov [eax],ecx ; mov [eax+4],edi`, no reload of the temporary's second member after the first store).
template<class T,class U> inline void _Construct(T* p,U v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#elif defined(FABLE_RETAIL_STLP_DESTROY_FASTCALL)
// NF398 opt-in (2026-11-25, the map<unsigned __int64, CGroupMemberData> tree rows 0x004DFB8F _M_erase / 0x00813460 erase(first,
// last)): the /Gr TU's _construct.h _Destroy as a __fastcall COMDAT (p in ecx): retail keeps that instance's out-of-line copy at
// 0x004DD6F3 (`push 0 ; call ??_G?$pair@$$CB_KVCGroupMemberData@@@std@@QAEPAXI@Z 0x004DBE39 ; ret`, unreferenced, 8 bytes), the
// destroy both rows expand inline (the /O2 row also takes engine/CGroupMemberData.h's FABLE_RETAIL_STLP_GROUP_MEMBER_DESTROY_SIZE_OWNER).
// The cdecl default emits a 12-byte copy that retail has nowhere. The default branch's text with __fastcall on _Destroy only (the
// FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL pattern).
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void __fastcall _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#else
#if defined(FABLE_RETAIL_QUICK_MENU_PAIR_CONSTRUCT_PASS27)
// Inline-budget: native 005947B6 and 0059400C expand the real pair placement construction.
template<class T,class U> FABLE_FORCEINLINE void _Construct(T* p,const U& v) { new(p) T(v); }
#else
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
#endif
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#endif
#ifdef FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
// Retail opt-in (NF19, 2026-11-11): the nontrivial range destroy as the out-of-line /Gr COMDAT the caller only calls --
// first/last in ecx/edx, the tag by reference on the stack (e.g. 0x006F2300 std::__destroy_aux<CCountedPointer<
// CCombatSequenceBase>*>, called from CCombatManager::RegisterAttackingCombatant 0x006EDA80). Declaration only.
#include "rebuild_abi.h"
template<class I> void FABLE_FASTCALL __destroy_aux(I first,I last,const __false_type&);
#else
#if defined(FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE) && defined(_MSC_VER)
#endif
#if defined(FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL) && defined(_MSC_VER)
// Retail opt-in (chunk 2005, row 0x00AB2520 C3DAnimationBuilder::BuildMeshAnimation): the range destructor kept as its out-of-line
// __fastcall COMDAT (0x00AB2BD0 ??$__destroy_aux@...@std@@YIXPAV...0ABU__false_type@0@@Z: first in ecx, last in edx, the tag pushed)
// with its body visible in the TU, so the tag temporary is homed in a dead argument slot (0x00AB2975 `lea eax,[esp+7Ch]`), as the
// landed same-TU sibling 0x00AB1F30 models it.
template<class I> __declspec(noinline) void __fastcall __destroy_aux(I first,I last,const __false_type&) {
#elif defined(FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE) && defined(_MSC_VER)
// Inline-budget: retain the actual nontrivial range destructor COMDAT call.
template<class I> __declspec(noinline) void __destroy_aux(I first,I last,const __false_type&) {
#else
#ifdef FABLE_RETAIL_AUGMENTATION_PARTICLES
// Original native 0076a2a0 takes its range in ecx/edx and the tag on the stack.
template<class I> inline void FABLE_FASTCALL __destroy_aux(I first,I last,const __false_type&) {
#elif defined(FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL) && defined(_MSC_VER)
// Retail opt-in (chunk 2016, row 0x0061AD60 CTCInventoryQuests::DrawActiveTestQuests): the same __fastcall convention alone (the /Gr
// Lionhead TUs' default for free template functions), without the AUGMENTATION_PARTICLES forceinline wrappers or its fable_map.h
// changes: ~vector<CTestQuest> calls 0x004ABD90 with ecx = first, edx = last and the tag pushed (0x0061B37A..0x0061B389).
// (Spelled __fastcall under _MSC_VER: rebuild_abi.h may not be included yet when this header comes first.)
template<class I> inline void __fastcall __destroy_aux(I first,I last,const __false_type&) {
#else
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)
template<class I> inline void FABLE_FASTCALL __destroy_aux(I first,I last,const __false_type&) {
#else
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
#endif
#endif
#endif
    for(;first!=last;++first) _Destroy(&*first);
}
#endif
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)
template<class I> inline void FABLE_FASTCALL __destroy_aux(I,I,const __true_type&) { }
#else
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
#endif
#if defined(FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL) && defined(_MSC_VER)
// NF416 opt-in (2026-11-25, the NInventory::CTCInventoryBase TU rows 0x005BF91A / 0x005BF969, vector<NInventory::CItem>::erase
// 0x005C1ECC): the dispatch expanded at every call (retail erase calls __destroy_aux 0x005C18C6 directly) in the /Gr TU's __fastcall
// convention. Retail keeps the unreferenced out-of-line copies 0x005C1937 (__destroy: `push ecx ; lea eax,[esp+3] ; push eax ;
// call 0x005C18C6 ; pop ecx ; ret 4`) and 0x005C1996 (_Destroy(first, last): the same with a plain `ret`). Pair with
// FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL. The FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE text with __fastcall added.
template<class I,class T> FABLE_FORCEINLINE void __fastcall __destroy(I first,I last,T*) {
#elif defined(FABLE_RETAIL_AUGMENTATION_PARTICLES) || defined(FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE)
// Inline-budget: native helpers expand this original destruction dispatch wrapper.
template<class I,class T> FABLE_FORCEINLINE void __destroy(I first,I last,T*) {
#else
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)
template<class I,class T> inline void FABLE_FASTCALL __destroy(I first,I last,T*) {
#else
template<class I,class T> inline void __destroy(I first,I last,T*) {
#endif
#endif
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
#if defined(FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL) && defined(_MSC_VER)
// NF416 opt-in: see __destroy above (retail copy 0x005C1996).
template<class I> FABLE_FORCEINLINE void __fastcall _Destroy(I first,I last) {
#elif defined(FABLE_RETAIL_AUGMENTATION_PARTICLES) || defined(FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE)
// Inline-budget: native helpers expand this original destruction dispatch wrapper.
template<class I> FABLE_FORCEINLINE void _Destroy(I first,I last) {
#else
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)
template<class I> inline void FABLE_FASTCALL _Destroy(I first,I last) {
#elif defined(FABLE_RETAIL_STLP_DESTROY_RANGE_NOINLINE) && defined(_MSC_VER)
// NF68 opt-in (row 0x009F6890 vector<CInputEvent>::operator=): the same STLport range _Destroy, kept out of line as a __fastcall
// COMDAT (0x009F6720: first in ecx, last in edx, plain `ret`), as that retail TU calls it.
#pragma auto_inline(off)
template<class I> __declspec(noinline) void __fastcall _Destroy(I first,I last) {
#else
template<class I> inline void _Destroy(I first,I last) {
#endif
#endif
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#if defined(FABLE_RETAIL_STLP_DESTROY_RANGE_NOINLINE) && !(defined(FABLE_RETAIL_AUGMENTATION_PARTICLES) || defined(FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE)) && !(defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)) && defined(_MSC_VER)
#pragma auto_inline(on)
#endif
// _uninitialized.h
#if defined(FABLE_RETAIL_QUEST_SECTION_OVERFLOW_PASS29)
// Inline-budget: the native 316-byte non-POD overflow expands the guarded STLport placement-copy loops.
template<class I,class O> FABLE_FORCEINLINE O FABLE_FASTCALL __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) _Construct(&*cur, *first);
    return cur;
}
#elif defined(FABLE_STLP_OUTOFLINE_UNINIT_COPY)
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
#elif defined(FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE)
// NF41 opt-in (2026-11-16, row 0x00618420 vector<CTCInventoryMap::CUIRegionInfo>::_M_insert_overflow): retail expands all three
// non-POD __uninitialized_copy calls of the 40-byte element's overflow path in line (prefix, value, suffix; 303 bytes, no call);
// plain `inline` leaves the suffix copy as a call (295 bytes) once VC7.1's inline budget is spent. Like the /Os
// FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE above.
template<class I,class O> FABLE_FORCEINLINE O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
#else
#ifdef FABLE_RETAIL_AUGMENTATION_PARTICLES
// Native 00769930/007698c0 take first/last in ecx/edx and result/tag on the stack.
template<class I,class O> inline O FABLE_FASTCALL __uninitialized_copy(I first,I last,O result,const __false_type&) {
#else
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI) || defined(FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26)
// Original native const-C3DVector uninitialized-copy takes first/last in ecx/edx.
template<class I,class O> inline O FABLE_FASTCALL __uninitialized_copy(I first,I last,O result,const __false_type&) {
#else
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
#endif
#endif
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
#endif
#ifdef FABLE_RETAIL_STLP_ORIGINAL_TRUE_UNINIT_COPY_PASS22
// Original _uninitialized.h's two-iterator true-tag signature and pointer dispatch.
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __true_type&) {
#ifdef FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
    return __copy_aux(first,last,result,_BothPtrType<I,O>::_Ret());
#else
    return __copy_ptrs(first,last,result,__true_type());
#endif
}
#else
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)
template<class T> inline T* FABLE_FASTCALL __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
#else
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
#endif
    return (T*)__copy_trivial(first,last,result);
}
#endif
#if defined(FABLE_RETAIL_QUEST_SECTION_OVERFLOW_PASS29)
// Inline-budget: the native 316-byte non-POD overflow expands the guarded STLport placement-copy loops.
template<class I,class N,class T> FABLE_FORCEINLINE I FABLE_FASTCALL __uninitialized_fill_n(I first,N n,const T& value,const __false_type&) {
    for(;n>0;--n,++first) if(first) _Construct(first,value);
    return first;
}
#elif defined(FABLE_STLP_OUTOFLINE_UNINIT_FILL_N)
// Opt-in (2026-11-03, chunk 719 row 0x00501150): the retail TU keeps this instantiation out of line as a __fastcall COMDAT
// (0x00513570 ??$__uninitialized_fill_n@PAVCLevelFiles@CObject@CLevelLoader@@IV123@@std@@YI...ABU__false_type@0@@Z, called
// from the inlined vector(n) with the tag homed in a dead argument slot); the same-TU noinline body gives that shape (EXTRA_RULES
// chunk 422 / GOTCHAS 0xB3EA70: inlined, the fill makes vector(n) too large and VC7.1 calls the whole ctor instead).
#pragma auto_inline(off)
#ifdef FABLE_RETAIL_STLP_UNINIT_FILL_N_WRAPPER_EXPANSION_PASS27
// Native 00A95790 is called by _M_insert_overflow, while the public
// uninitialized_fill_n at 00A959E0 expands this same helper in its body.
template<class I,class N,class T> inline I __fastcall __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
#else
template<class I,class N,class T> __declspec(noinline) I __fastcall __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
#endif
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#pragma auto_inline(on)
#elif defined(FABLE_RETAIL_STLP_UNINIT_OUTOFLINE)
#pragma auto_inline(off)
template<class I,class N,class T> I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {   // see FABLE_RETAIL_STLP_UNINIT_OUTOFLINE above
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#pragma auto_inline(on)
#elif defined(FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE)
// Retail opt-in (agentFAM02 2026-11-11, the /Os vector TUs, e.g. 0x00469FA5 / 0x004768F8 uninitialized_fill_n): the size-optimised
// TU expands the non-POD __uninitialized_fill_n into the out-of-line uninitialized_fill_n COMDAT (`ret 4`, one loop, no call);
// plain `inline` under /Os keeps it as a call (EXTRA_RULES: under /Os the uninitialized/fill helpers are __forceinline).
template<class I,class N,class T> FABLE_FORCEINLINE I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#else
#ifdef FABLE_RETAIL_STLP_PASS22_FLARE_ABI
// Native00B8EE20 has first/count in ECX/EDX and value/tag on stack,RET8.
template<class I,class N,class T> inline I FABLE_FASTCALL __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
#else
#if defined(FABLE_RETAIL_GROUP_REMAP_PASS29) || defined(FABLE_RETAIL_CHUNK_READER_PASS29)
template<class I,class N,class T> inline I FABLE_FASTCALL __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
#else
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
#endif
#endif
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __true_type&) {
    return std::fill_n(first,n,x);
}
#ifdef FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY
// NF212 opt-in (2026-11-21, row 0x004333B3 vector<CCharString>::_M_fill_insert): declaration only. That size TU (frame pointer on)
// calls uninitialized_fill_n<CCharString*, unsigned int, CCharString> 0x00433504 out of line as a __fastcall COMDAT (first in ecx,
// n in edx, `ret 4`), the landed owner row's body. The visible noinline body (..._NOINLINE below) would emit a 41-byte pick-any
// copy that differs from the owner's 38 bytes (pick-any rule); same function, declaration only.
#ifdef _MSC_VER
template<class I,class N,class T> I __fastcall uninitialized_fill_n(I first,N n,const T& x);
#else
template<class I,class N,class T> I uninitialized_fill_n(I first,N n,const T& x);
#endif
#else
#if defined(FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_NOINLINE) && defined(_MSC_VER)
// NF68 opt-in (row 0x00AD85D0 vector<QuadricOptimiserInternals::COptimisedVertex>::_M_fill_insert): the same STLport body, kept out
// of line as a __fastcall COMDAT (0x00AD7B90: first in ecx, n in edx, `ret 4`), as that retail TU calls it.
#pragma auto_inline(off)
template<class I,class N,class T> __declspec(noinline) I __fastcall uninitialized_fill_n(I first,N n,const T& x) {
    typedef typename iterator_traits<I>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,x,IsPOD());
}
#pragma auto_inline(on)
#else
#if defined(FABLE_RETAIL_GROUP_REMAP_PASS29) || defined(FABLE_RETAIL_CHUNK_READER_PASS29)
template<class I,class N,class T> inline I FABLE_FASTCALL uninitialized_fill_n(I first,N n,const T& x) {
#else
template<class I,class N,class T> inline I uninitialized_fill_n(I first,N n,const T& x) {
#endif
    typedef typename iterator_traits<I>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,x,IsPOD());
}
#endif
#endif
// _alloc.h (malloc-based allocator)
#if defined(FABLE_RETAIL_STLP_MICROTHREAD_MALLOC_PASS30) && !defined(FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE)
// STLport 4.6.2 _alloc.h gives the class, method and local names.
// The allocation at 00A44B75 uses the malloc result directly.
template<int __inst> class __malloc_alloc {
public:
    static void* allocate(size_t __n) {
        void* __result = malloc(__n);
        return __result;
    }
};
#endif
#ifdef FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE
// Per-TU opt-in (row 0x00D30030): STLport's allocator<T>::deallocate forwards to __sgi_alloc = __malloc_alloc<0>::deallocate,
// whose body is `free(p)` (stl/_alloc.h); the extra inline layer is invisible in /O2 code but adds to the caller size VC7.1's
// inliner scores later sites with.
template<int __inst> class __malloc_alloc {
public:
    static void deallocate(void* __p, size_t) { free((char*)__p); }
};
#endif
#ifdef FABLE_RETAIL_STLP_MALLOC_ALLOC_ALLOCATE
// (acctC agentC31) allocation half of STLport's malloc-based __sgi_alloc; a separate class template here so that the existing
// FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE block above stays untouched (descriptive name: STLport's is __malloc_alloc<0>::allocate).
template<int __inst> class __malloc_alloc_allocate {
public:
    static void* allocate(size_t __n) { void* __result = malloc(__n); return __result; }
};
#endif
template<class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template<class _Tp1> allocator(const allocator<_Tp1>&) { }
    ~allocator() { }
#ifdef FABLE_RETAIL_STLP_MALLOC_ALLOC_ALLOCATE
    // Retail opt-in (acctC agentC31, row 0x00601310 CTCInventoryMap::ConstructQuestsScreenPC): STLport 4.6 stl/_alloc.h
    // allocator<T>::allocate forwards to __sgi_alloc::allocate(n * sizeof(T)) (the malloc-based __malloc_alloc<0>, body `malloc(n)`),
    // the mirror of FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE. Same code; the extra inline layer adds to the caller size VC7.1's
    // inliner scores later sites with.
    _Tp* allocate(size_t n,const void* =0) { return n != 0 ? (_Tp*)__malloc_alloc_allocate<0>::allocate(n * sizeof(_Tp)) : 0; }
#else
#if defined(FABLE_RETAIL_STLP_MICROTHREAD_MALLOC_PASS30) && !defined(FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE)
    _Tp* allocate(size_t __n, const void* = 0) {
        return __n != 0 ? (_Tp*)__malloc_alloc<0>::allocate(__n * sizeof(_Tp)) : 0;
    }
#else
#if defined(FABLE_RETAIL_QUICK_MENU_PAIR_ALLOCATE_PASS27)
// Inline-budget: native 0059400C expands allocation, including the zero-length test and malloc.
    FABLE_FORCEINLINE _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#else
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
#endif
#endif
#ifdef FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE
    void deallocate(_Tp* p,size_t n) { if (p != 0) __malloc_alloc<0>::deallocate((void*)p, n * sizeof(_Tp)); }
#else
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#endif
#ifdef FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
// Retail opt-in (chunk 883, row 0x006096E0 CTCInventoryMap::ConstructLogbookListPC; reused for row 0x00D30030): STLport 4.6 _alloc.h, where
// _STLP_alloc_proxy::allocate / deallocate go through __stl_alloc_rebind (a reference cast of the stored allocator). Same code; the
// extra inline layer is what spends the row's auto-inline budget as retail does.
template<class _Tp1,class _Tp2> inline allocator<_Tp2>& __stl_alloc_rebind(allocator<_Tp1>& a,const _Tp2*) { return (allocator<_Tp2>&)(a); }
#endif
};
#ifdef FABLE_RETAIL_ICON_REVERSE_ITERATOR_PASS29
// STLport 4.6.2 _iterator_base.h, _iterator.h and _vector.h; FableWin 02264640 uses rbegin/rend.
template<class _Category, class _Tp, class _Distance, class _Pointer, class _Reference> struct iterator {
    typedef _Category iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Pointer pointer;
    typedef _Reference reference;
};
template<class _Iterator> class reverse_iterator :
    public iterator<typename iterator_traits<_Iterator>::iterator_category,
                    typename iterator_traits<_Iterator>::value_type,
                    typename iterator_traits<_Iterator>::difference_type,
                    typename iterator_traits<_Iterator>::pointer,
                    typename iterator_traits<_Iterator>::reference> {
protected:
    _Iterator current;
    typedef reverse_iterator<_Iterator> _Self;
public:
    typedef typename iterator_traits<_Iterator>::reference reference;
    typedef _Iterator iterator_type;
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type __x) : current(__x) {}
    reverse_iterator(const _Self& __x) : current(__x.current) {}
    iterator_type base() const { return current; }
    reference operator*() const { _Iterator __tmp = current; return *--__tmp; }
    _Self& operator++() { --current; return *this; }
};
template<class _Iterator> inline bool FABLE_FASTCALL operator==(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y) {
    return __x.base() == __y.base();
}
template<class _Iterator> inline bool FABLE_FASTCALL operator!=(const reverse_iterator<_Iterator>& __x,const reverse_iterator<_Iterator>& __y) {
    return !(__x == __y);
}
#endif
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
#ifdef FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
    _Tp* allocate(size_t n) { return __stl_alloc_rebind(static_cast<_MaybeReboundAlloc&>(*this),(_Tp*)0).allocate(n,0); }
    void deallocate(_Tp* p, size_t n) { __stl_alloc_rebind(static_cast<_MaybeReboundAlloc&>(*this),(_Tp*)0).deallocate(p,n); }
#endif
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
#ifdef FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_FREE_UNTESTED
    // NF71 opt-in (round 7, row 0x008F95D0 LSocket::SendBlock): that TU releases its local vector<char> with a bare `push ecx ; call free`
    // (0x008F96B8; ReceiveBlock 0x008F9B20 `push esi ; call free` too) while the LAddress strings next to it keep their null test.
    // FITTED (owner acceptance test 2026-11-14): retail-shaped release, the null test of ~_Vector_base / deallocate dropped; free(0) is a no-op.
    ~_Vector_base() { free(_M_start); }
#elif defined(FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_DECL_ONLY)
    // NF399 opt-in (2026-11-25, row 0x004DF54F ~vector<const CSteeringAgent*>; like FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY and
    // FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY): retail's ~vector of a pointer vector is the 5-byte tail jump
    // `jmp 0x004DF554` into the out-of-line ~_Vector_base (landed there, nf396_b1); with the inline body below VC7.1 expands it
    // (14 bytes). Declaration only: the TU calls the base destructor out of line.
    ~_Vector_base();
#else
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
#endif
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
#if defined(FABLE_RETAIL_STRIPIFIER_VERTEX_MAPS_PASS30) || defined(FABLE_RETAIL_PALETTE_VECTOR_BASE_SIZE_PASS30)
// Fidelity: scaffolding, not retail. 005D82F3 is the 52-byte /Os allocator owner.
// The pragma block ends with optimize("", on). Use the original inline allocation.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline _Vector_base<long,allocator<long> >::_Vector_base(size_t n,const allocator<long>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=_M_end_of_storage.allocate(n);
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#if defined(FABLE_RETAIL_PALETTE_VECTOR_BASE_SIZE_PASS30)
// Fidelity: scaffolding, not retail. 00450E3A uses the same 52-byte /Os body.
template<> inline _Vector_base<float,allocator<float> >::_Vector_base(size_t n,const allocator<float>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=_M_end_of_storage.allocate(n);
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#endif
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
#ifdef FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
    // Real vector reference aliases also preserve CArray<bool>'s donor API.
    typedef _Tp& reference;
    typedef const _Tp& const_reference;
#endif
    typedef size_t size_type;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
protected:
    // Original STLport4.6.2 _vector.h lines125..168: these are in-class members.
    
#ifdef FABLE_RETAIL_SHARED_CONTROLS_PASS29
    // Inline-budget: retail expands this vector operation inside IsControlInputSharedBetweenGameActions.
    FABLE_FORCEINLINE
#endif
    void _M_insert_overflow(iterator __position,const _Tp& __x,const __false_type&,size_type __fill_len,bool __atend) {
        const size_type __old_size=size();
        const size_type __len=__old_size+(max)(__old_size,__fill_len);
        _Tp* __new_start=this->_M_end_of_storage.allocate(__len);
        _Tp* __new_finish=__new_start;
        {
        __new_finish=__uninitialized_copy(this->_M_start,__position,__new_start,__false_type());
        if(__fill_len==1) {_Construct(__new_finish,__x);++__new_finish;}
        else __new_finish=__uninitialized_fill_n(__new_finish,__fill_len,__x,__false_type());
        if(!__atend) __new_finish=__uninitialized_copy(__position,this->_M_finish,__new_finish,__false_type());
        }
        _M_clear();_M_set(__new_start,__new_finish,__new_start+__len);
    }
    void _M_insert_overflow(iterator __position,const _Tp& __x,const __true_type&,size_type __fill_len,bool __atend) {
        const size_type __old_size=size();
        const size_type __len=__old_size+(max)(__old_size,__fill_len);
        _Tp* __new_start=this->_M_end_of_storage.allocate(__len);
        _Tp* __new_finish=(_Tp*)__copy_trivial(this->_M_start,__position,__new_start);
        __new_finish=fill_n(__new_finish,__fill_len,__x);
        if(!__atend) __new_finish=(_Tp*)__copy_trivial(__position,this->_M_finish,__new_finish);
        _M_clear();_M_set(__new_start,__new_finish,__new_start+__len);
    }
public:
#endif
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#if defined(FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE) && defined(_MSC_VER)
    // Per-TU opt-in (row 0x0070CE10 CAnimSet::UpdateDistanceMoved): the fill constructor's body is visible to the TU but called
    // out of line (COMDAT vector<float> 0x004516E7).
    __declspec(noinline) vector(size_type n,const _Tp& value,const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(n,a) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,value);
    }
#elif defined(FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE)
    // Per-TU opt-in (agentFAM10, 2026-11-12, row 0x004538EB COpinionSet::COpinionSet; like FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE): that
    // size-optimised TU calls the fill constructor's COMDAT (vector<float> 0x004516E7, `ret 0Ch`, with the value and allocator
    // temporaries in the caller's frame) where the body below would be expanded. Declaration only.
    vector(size_type n,const _Tp& value,const _Alloc& a=_Alloc());
#else
    vector(size_type n,const _Tp& value,const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(n,a) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,value);
    }
#endif
#if defined(FABLE_RETAIL_STLP_VECTOR_SIZECTOR_VISIBLE_NOINLINE)
    // Per-TU opt-in (acctC agentC12, row 0x005EEFF2 CTCInventoryAbilities::GetMagicListUIComponent; like
    // FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE): the vector(size_type n) body below, visible but kept out of line, as that TU calls
    // the COMDAT (vector<EHeroAbility>::vector(size_type), call in the row after `push [n]`). Unlike the declaration-only
    // FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE, c2 knows the constructor keeps no address of its allocator temporary, which it then homes
    // in the caller's live argument slot as retail does.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
#elif defined(FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE)
    // Per-TU opt-in (chunk 779, row 0x00BEAE50 CWaterPatchMesh::Load; like FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE): STLport's
    // vector(size_type n) is one inline body, which VC7.1 expanded in some TUs and called as a COMDAT in others; that TU calls
    // vector<unsigned char>::vector(size_type) 0x00514E20 (??0?$vector@EV?$allocator@E@std@@@std@@QAE@I@Z, bound by the landed
    // 0x00996610) for both CArray<unsigned char> locals (0x00BEAFDF, 0x00BEAFE9). A declaration only here, so the row calls it.
#ifdef FABLE_RETAIL_PASS22_WATER_PATCH_LOAD
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
#else
    explicit vector(size_type n);
#endif
#else
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
    // Per-TU opt-in (chunk 1012, row 0x008A8260 CGameScriptInterface::GetHeroSummonedCreaturesList; like
    // FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE): STLport's copy constructor is one inline body, which VC7.1 called as a COMDAT in
    // that TU: vector<CIntelligentPointer<CThingAICreature> > copy 0x008AC540 (call at 0x008A8387, ecx = the local, the source pushed:
    // the inline CTCGhostSword::GetSummonedCreatures() return). Declared here; the body (below) is visible but kept out of line, so
    // VC7.1 knows the copy never captures the new vector and keeps its start/finish in CPU locations across the caller's loop
    // (0x008A838C `mov ebp,[esp+18h] ; mov edi,[esp+14h]`, never reloaded).
    vector(const vector& x);
#elif defined(FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY)
    // Per-TU opt-in (NF211, 2026-11-21, row 0x006EAC50 CCombatManager::GetCombatDebugInfo): the copy constructor declared only.
    // That TU calls vector<C3DVector>'s copy constructor 0x006F0810 out of line (0x006EADCA / 0x006EADD7, from the expanded implicit
    // CCombatDebugInfo copy constructor, whose out-of-line retail copy is 0x006F3000); with the inline body visible VC7.1 emits a pick-any COMDAT
    // copy of it inside the row's object, which the copy-constructor copy then reaches as a symbol defined in that object (a
    // different target under the nf205 amendment). Declaration only (the FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY pattern); same function.
    vector(const vector& x);
#else
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_STLP_PASS22_FLARE_ABI)
    FABLE_FORCEINLINE vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
#else
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
#endif
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_RANGE_CTOR
    // Retail opt-in (chunk 1018, row 0x00465C54 CCombatAbilityBlockDefBase::Transfer): STLport 4.6 _vector.h's range constructor
    // `vector(_InputIterator __first, _InputIterator __last)`, which for a non-integral iterator forwards to the category-dispatched
    // _M_range_initialize(first, last, const forward_iterator_tag&); retail expands the constructor (three zero stores) and calls
    // _M_range_initialize out of line (vector<EWeaponClass> 0x00465E44 with a set<EWeaponClass>::const_iterator pair, the tag homed
    // in a dead argument byte). Declaration only for _M_range_initialize.
    template <class _InputIter> vector(_InputIter first,_InputIter last,const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) {
        _M_range_initialize(first,last,forward_iterator_tag());
    }
    template <class _ForwardIter> void _M_range_initialize(_ForwardIter first,_ForwardIter last,const forward_iterator_tag&);
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
    // Per-TU opt-in (NF48 round 7, rows 0x00D2D900 / 0x00D61C00 / 0x00DC2540 deleting destructors and 0x00DC8890
    // CSpeakingMarker::Main; like FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE): STLport's ~vector is one inline body, which VC7.1
    // called as a COMDAT in some TUs: ~vector<CScriptThing> 0x008AC970 (landed,
    // ??1?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QAE@XZ). Declaration only.
    ~vector();
#elif defined(FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE)
    // Per-TU inline control (NF48 round 7; like FABLE_RETAIL_CARRAY_SIZE_CTOR_FORCEINLINE, owner decision 2026-11-08, inline budget):
    // the CScriptThing-list script threads 0x00EE6A40 / 0x00EDD060 / 0x00D18B20 expand ~vector<CScriptThing> at most exits and call the
    // out-of-line COMDAT 0x008AC970 at one (an inline_depth(0) region there), where VC7.1 otherwise calls the COMDAT at every exit;
    // CActiveEntity::~CActiveEntity 0x00CB5B90 expands ~vector<CCharString> (no COMDAT call). Same body as the default.
    FABLE_FORCEINLINE ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#else
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
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
#if !defined(FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22) && !defined(FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL)
    template <class _ForwardIter> _Tp* _M_allocate_and_copy(size_type n,_ForwardIter first,_ForwardIter last) {
        _Tp* result=this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first,last,result,_IsPODType());
        return result;
    }
#endif
#endif
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#ifdef FABLE_RETAIL_ICON_REVERSE_ITERATOR_PASS29
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
#ifdef FABLE_RETAIL_STLP_VECTOR_AT_OUTOFLINE
    // agentC29 (row 0x0074E860 CTCContainerRewardHero::GetRewardItems): STLport 4.6 vector::at (`_M_range_check(n)`, i.e. without
    // exceptions puts("vector") + abort, then `*(begin()+n)`), called out of line in retail: 0x0074CBC0 for vector<CDefIndex> (call at
    // 0x0074EA59, `push index ; mov ecx,&ObjectFamilies`; byte-identical copy 0x0074D120 in the CTCCreatureGenerator TU). Declaration only.
    const _Tp& at(size_type n) const;
#endif
#if defined(FABLE_RETAIL_STLP_VECTOR_AT_INLINE) && !defined(FABLE_RETAIL_STLP_VECTOR_AT_OUTOFLINE)
    // agentC36 (row 0x0074AF50 CTCCreatureGenerator::CreateCreaturesUndead): the same STLport 4.6 at() with its body, for a TU that
    // expands it (0x0074AFBC..0x0074AFE1: size test, `push "vector" ; call puts ; call abort`, then `[begin + n*4]`).
    void _M_range_check(size_type n) const { if (n >= size_type(this->_M_finish-this->_M_start)) { puts("vector"); abort(); } }
    const _Tp& at(size_type n) const { _M_range_check(n); return *(this->_M_start+n); }
#ifdef FABLE_RETAIL_GENERATOR_CREATE_PASS29
    _Tp& at(size_type n) { _M_range_check(n); return (*this)[n]; }
#endif
#endif
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
#ifdef FABLE_RETAIL_STLP_VECTOR_ORIGINAL_FRONT_PASS22
    // STLport4.6.2 _vector.h190..191; original CArray<unsigned char>::GetAsCArray
    // FableWin01AB9542 calls nonconst front, then returns its address.
    _Tp& front() { return *begin(); }
    const _Tp& front() const { return *begin(); }
#else
    _Tp& front() { return *this->_M_start; }
#endif
    _Tp& back() { return *(end()-1); }
    const _Tp& back() const { return *(end()-1); }
    void pop_back() { --this->_M_finish; _Destroy(this->_M_finish); }
#ifdef FABLE_RETAIL_STLP_VECTOR_CHECKED_SUBSCRIPT_PASS26
    // Inferred release form of the checked subscript. Both donor instantiations
    // check size against the original index before accessing their array.
    // FableWin 01A70AB0 handles primitive handles, 02103F8F handles exit pairs.
    _Tp& operator[](size_type __n) {
        if (size() <= __n) {
#ifdef _DEBUG
            assert(__n < size());
#endif
        }
        return *(begin() + __n);
    }
    const _Tp& operator[](size_type __n) const {
        if (size() <= __n) {
#ifdef _DEBUG
            assert(__n < size());
#endif
        }
        return *(begin() + __n);
    }
#else
#ifdef FABLE_RETAIL_STLP_VECTOR_INDEX_START
    // Retail opt-in (row 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions): operator[] reads _M_start directly instead of through
    // begin() (same address arithmetic, one inline level less). With begin() VC7.1 drops that row's second induction slot
    // ([esp+14h] index, 0x0075A7BA / 0x0075A7E0, beside the [esp+10h] byte offset, 0x0075A7D4) and stops holding &config.AngularMomentum /
    // &config.AngularVelocity in ebx / ebp (0x0075A7CB / 0x0075A7CE): 729 v 726 bytes. Body only: no layout change.
    _Tp& operator[](size_type n) { return *(this->_M_start+n); }
    const _Tp& operator[](size_type n) const { return *(this->_M_start+n); }
#elif defined(FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22)
    reference operator[](size_type __n) { return *(begin()+__n); }
    const_reference operator[](size_type __n) const { return *(begin()+__n); }
#else
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#endif // FABLE_RETAIL_STLP_VECTOR_INDEX_START
#endif // FABLE_RETAIL_STLP_VECTOR_CHECKED_SUBSCRIPT_PASS26
#if defined(FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE) && defined(_MSC_VER)
    // Per-TU opt-in (chunk Q6, row 0x00542780 NUISystem::CIconText::AddIcon): the STLport push_back body below, visible but kept out
    // of line, as the retail TU calls it (push_back<CEmbeddedIcon> 0x00543C90, whose body calls _M_insert_overflow 0x00543B70).
    // Unlike the declaration-only FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE, c2 then knows push_back keeps no address of the value.
    __declspec(noinline)
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
    // Per-TU opt-in (chunk 760, row 0x0086BE40 CRandomAppearanceMorph::GetRandomBodyParts; like
    // FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN): STLport's push_back is one inline body, which VC7.1
    // expanded in some TUs and called as a COMDAT in others; that TU calls push_back<CMorphSet::CMeshTextureSet> 0x0086E7C0
    // (whose own body expands _M_insert_overflow). A declaration only here, so the row calls it.
    void push_back(const _Tp& x);
#else
#ifdef FABLE_RETAIL_STLP_VECTOR_PUSHBACK_FINISH_LOCAL_PASS24
    // Keep the append position across construction. The sky load loop uses the same pointer for the write and increment.
    void push_back(const _Tp& x) {
        iterator finish = this->_M_finish;
        if (finish != this->_M_end_of_storage._M_data) {
            _Construct(finish, x);
            this->_M_finish = finish + 1;
        } else
            _M_insert_overflow(finish, x, _IsPODType(), 1, true);
    }
#else
    
#ifdef FABLE_RETAIL_SHARED_CONTROLS_PASS29
    // Inline-budget: retail expands this vector operation inside IsControlInputSharedBetweenGameActions.
    FABLE_FORCEINLINE
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#endif
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE
    // Per-TU opt-in (chunk 1021, row 0x006B3E60 CEnvironment::SetWeightedThemes; like FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE): STLport's
    // insert(position, x) is one inline body, which VC7.1 expanded in some TUs and called as a COMDAT in others; that TU calls
    // vector<pair<long, float> >::insert 0x006B5ED0 (landed; call at 0x006B3F4B). Declaration only here.
    iterator insert(iterator position,const _Tp& x);
#else
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
#if defined(FABLE_RETAIL_STLP_VECTOR_ERASE_NOINLINE) && defined(_MSC_VER)
    // Per-TU opt-in (acctC wfE9-8, rows 0x005A4CDD ConstructActualSellInventoryPC / 0x005AC10E ConstructActualBuyInventoryPC): the
    // STLport erase(position) body below, visible but kept out of line, as the retail TU calls it (vector<pair<long,long> >::erase
    // 0x00593139). Unlike the declaration-only FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE, c2 then knows erase keeps no address of the
    // vector, so a local vector's words survive later calls (retail loop heads 0x005A4E49 / 0x005AC189). Mirror of PUSHBACK_NOINLINE.
    __declspec(noinline)
#endif
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
#if defined(FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE) && defined(_MSC_VER)
    // NF73 opt-in (rows 0x00596CCE / 0x00597006, through FABLE_RETAIL_CFRONTENDMANAGER_GETPROFILENAMES_BODY): the STLport
    // erase(first, last) body below, visible but kept out of line, as the retail CFrontEndManager TU calls it (vector<CWideString>::erase
    // 0x00412450 from GetProfileNames 0x005955AB). Unlike the declaration-only FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE, c2 then
    // knows erase keeps no address of the vector. Mirror of FABLE_RETAIL_STLP_VECTOR_ERASE_NOINLINE.
    __declspec(noinline)
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
#ifdef FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE
    // Per-TU opt-in (chunk 1021, row 0x006B3E60; like FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE): that TU calls
    // vector<pair<long, float> >::clear 0x006B5B50 (landed; call at 0x006B3FDA) instead of expanding erase(begin(), end()).
    void clear();
#else
#ifdef FABLE_RETAIL_OCCLUSION_HANDLE_CLEAR_PASS30
    // Inline-budget: 00B91140 expands clear with its retail POD memmove arm.
    FABLE_FORCEINLINE
#endif
    void clear() { erase(begin(),end()); }
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
    // Authenticated4.5.2 _vector.h459..460;4.6.2 has the same public forwarding overload.
    void insert(iterator __pos,size_type __n,const _Tp& __x) { _M_fill_insert(__pos,__n,__x); }
#endif
    // Retail takes the value by const reference (STLport 4.5.2 form; 0x004428F0 resize<long> forwards the pointer to
    // _M_fill_insert, which dereferences it); FableWin.pdb's later build passes it by value.
#ifdef FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY
    // Per-TU opt-in (NF195, 2026-11-20, q4567: rows 0x00B7F600 .. CEngineWaterRenderer shader-constant setup): resize(n, value) declared
    // only. Those TUs call vector<CPreTransposedBoneMatrix>::resize 0x00B7EDF0 out of line (`push value ; push 4 ; call`) and keep no
    // value across the call; the visible noinline body (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE) would emit a 94-byte COMDAT copy
    // that differs from the owner's 102 bytes (pick-any rule). Declaration only; same function.
    void resize(size_type n,const _Tp& x);
#else
#ifdef FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
    // Per-TU opt-in (chunk 777, row 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest; like
    // FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE): VC7.1 expanded STLport's inline resize in some TUs and called it as a COMDAT
    // in others; that TU calls vector<char>::resize 0x004122D0 (landed, ?resize@?$vector@DV?$allocator@D@std@@@std@@QAEXIABD@Z) with
    // the char() temporary in a dead argument slot. The body is visible (STLport's own) but kept out of line, as in the retail TU:
    // VC7.1 then knows resize does not keep the vector's address, holds the start in a CPU location across the later calls and reuses
    // the vector's frame slot (0x0055FA79 `mov edi,[esp+28h]` ... 0x0055FB34 `push edi; call free`); declaration-only re-reads it.
#ifdef FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
    __declspec(noinline) void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else insert(end(),n-size(),x);}
#else
    __declspec(noinline) void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#endif
#else
#ifdef FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
    void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else insert(end(),n-size(),x);}
#else
    void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#endif
#endif
#endif
#if defined(FABLE_RETAIL_PALETTE_BUILD_PASS30)
    // Inline-budget: retail 00888A60 expands the original one-argument STLport wrapper.
    FABLE_FORCEINLINE void resize(size_type n) { resize(n,_Tp()); }
#elif defined(FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL)
    // Retail opt-in (Codex 2026-11-09): native00443f90 is the one-argument
    // vector<CEnginePrimitiveHandle>::resize called by00651293.
    void resize(size_type n);
#elif defined(FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_OUTOFLINE)
    // Per-TU opt-in (chunk 2001, row 0x00B3C5D0 CEngineResourceManager::SetupAlphaStippleTexture; pair with
    // FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE): that TU calls vector<CTexture>::resize(size_type) 0x0049B7F0 (`push 4 ; call`, Ego_r.pdb /
    // FableWin.pdb ?resize@?$vector@VCTexture@@V?$allocator@VCTexture@@@std@@@std@@QAEXI@Z) and vector<unsigned long>::resize(size_type,
    // const unsigned long&) 0x00464931 out of line. STLport's body stays visible (the same TU's BuildAlphaStippleTexture resizes its
    // CArray<unsigned char> argument through it, and VC7.1 must see that the callee keeps no address), only kept out of line.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    void resize(size_type n) { resize(n,_Tp()); }
#else
    void resize(size_type n) { resize(n,_Tp()); }
#endif
#if defined(FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE) || defined(FABLE_RETAIL_GROUP_REMAP_PASS29) || defined(FABLE_RETAIL_CHUNK_READER_PASS29)
protected: // Original Ego_r/FableWin _M_fill_insert access; native0075BB00 is IAEX.
#endif
    void _M_fill_insert(iterator position,size_type n,const _Tp& x);
#ifdef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
public:
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT
    // Per-TU opt-in (row 0x00523CF0, for the same-TU CThingManager::SaveEntitiesToScript / SaveEntitiesBinary bodies): STLport 4.6
    // _vector.h `insert(iterator __pos, _InputIterator __first, _InputIterator __last)` -> _M_insert_dispatch -> _M_range_insert(...,
    // forward_iterator_tag) (stl/_vector.c), the body visible to the TU but kept out of line as in retail
    // (vector<CCharString>::_M_range_insert 0x0052B6A0, landed). VC7.1 then knows it keeps no address of the vector.
    template<class _InputIterator> void insert(iterator position,_InputIterator first,_InputIterator last) {
        _M_range_insert(position,first,last,forward_iterator_tag());
    }
    template<class _ForwardIterator> __declspec(noinline) void _M_range_insert(iterator position,_ForwardIterator first,_ForwardIterator last,const forward_iterator_tag&) {
        if(first!=last) {
            size_type n=distance(first,last);
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                const size_type elemsAfter=this->_M_finish-position;
                iterator oldFinish=this->_M_finish;
                if(elemsAfter>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    __copy(first,last,position,input_iterator_tag(),(ptrdiff_t*)0);
                } else {
                    _ForwardIterator mid=first;
                    advance(mid,elemsAfter);
                    __uninitialized_copy(mid,last,this->_M_finish,_IsPODType());
                    this->_M_finish+=n-elemsAfter;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=elemsAfter;
                    __copy(first,mid,position,input_iterator_tag(),(ptrdiff_t*)0);
                }
            } else {
                const size_type oldSize=size();
                const size_type len=oldSize+(max)(oldSize,n);
                iterator newStart=this->_M_end_of_storage.allocate(len);
                iterator newFinish=newStart;
                newFinish=__uninitialized_copy(this->_M_start,position,newStart,_IsPODType());
                newFinish=__uninitialized_copy(first,last,newFinish,_IsPODType());
                newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,_IsPODType());
                _M_clear();
                _M_set(newStart,newFinish,newStart+len);
            }
        }
    }
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR
    // Retail branch (chunk 728, row 0x006D2BD0 CTCWife::OnSerialise): STLport 4.6 vector::reserve (stl/_vector.c), out of line in
    // retail (reserve<CCharString> 0x004BA7A0, reserve<pair<CCharString,long> > 0x006D4680). A declaration only.
    void reserve(size_type n);
#endif
protected:
#ifndef FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd);
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd);
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL
    // NF68 opt-in (row 0x009F6890): the same protected STLport member, called out of line in that retail TU (vector<CInputEvent>::_M_clear
    // 0x009F6850, landed ?_M_clear@...@IAEXXZ). Declaration only.
    void _M_clear();
#else
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
#endif
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
#if defined(FABLE_RETAIL_STLP_VECTOR_C) && defined(FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL) && !defined(FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22)
    // NF68 opt-in (row 0x009F6890): STLport 4.6 _vector.h's protected member template, called out of line in that retail TU
    // (vector<CInputEvent>::_M_allocate_and_copy<const CInputEvent*> 0x009F65E0, thiscall `ret 0Ch`). Declaration only.
    template <class _ForwardIter> _Tp* _M_allocate_and_copy(size_type n,_ForwardIter first,_ForwardIter last);
#endif
#if defined(FABLE_RETAIL_STLP_VECTOR_C) && defined(FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22)
    // Authenticated STLport4.6.2 _vector.h491..525: protected, in-class, after _M_clear/_M_set.
    template <class _ForwardIter> _Tp* _M_allocate_and_copy(size_type n,_ForwardIter first,_ForwardIter last) {
        _Tp* result=this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first,last,result,_IsPODType());
        return result;
    }
#endif
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
#ifdef FABLE_RETAIL_PALETTE_MUTABLE_VECTOR_COPY_PASS30
            // In 00889F00 these ranges use the original mutable iterator instance at 00889950.
            _Tp* i=__copy_ptrs(x._M_start,x._M_finish,this->_M_start,_IsPODType());
#else
            _Tp* i=__copy_ptrs((const _Tp*)x._M_start+0,(const _Tp*)x._M_finish+0,(_Tp*)this->_M_start,_IsPODType());
#endif
            _Destroy(i,this->_M_finish);
        } else {
#ifdef FABLE_RETAIL_PALETTE_MUTABLE_VECTOR_COPY_PASS30
            // In 00889F00 these ranges use the original mutable iterator instance at 00889950.
            __copy_ptrs(x._M_start,x._M_start+size(),this->_M_start,_IsPODType());
#else
            __copy_ptrs((const _Tp*)x._M_start,(const _Tp*)x._M_start+size(),(_Tp*)this->_M_start,_IsPODType());
#endif
            __uninitialized_copy((const _Tp*)x._M_start+size(),(const _Tp*)x._M_finish+0,this->_M_finish,_IsPODType());
        }
        this->_M_finish=this->_M_start+xlen;
    }
    return *this;
}
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
// (chunk 1012) STLport 4.6 _vector.h vector(const _Self&): allocate x.size() elements, uninitialized-copy the range; out of line.
template<class _Tp,class _Alloc>
#ifdef _MSC_VER
__declspec(noinline)
#endif
vector<_Tp,_Alloc>::vector(const vector<_Tp,_Alloc>& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
    this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
}
#endif
#ifndef FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
// The separate overflow instantiation can be supplied by another translation unit.
// Opt-in FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE (chunk 2055, row 0x00821A80 CMap::LoadFromFile): the two bodies stay
// visible but are kept out of line, as STLport's stl/_vector.c definitions are in the retail TU (vector<long>::_M_insert_overflow
// 0x00440990 is called at 0x00821DCC / 0x00821F3C / 0x00821F56, never expanded). With the body visible VC7.1 knows the call
// writes the vector only through `this`: retail keeps the local vector's _M_finish in a CPU location across push_back's construct
// store (0x00821EE7 `mov edx,[esp+30h] ; cmp edx,ecx ; ... ; mov ecx,edx`); with the declaration alone (..._DECL_ONLY) it is
// reloaded after every store and the frame grows by 8 bytes (0x98 v retail 0x90).
#if defined(FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE) && defined(_MSC_VER)
#define FABLE_STLP_OVERFLOW_OUTOFLINE __declspec(noinline)
#else
#define FABLE_STLP_OVERFLOW_OUTOFLINE
#endif
#ifndef FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
template<class _Tp,class _Alloc> FABLE_STLP_OVERFLOW_OUTOFLINE void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
    newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
    if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
    else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
    if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
template<class _Tp,class _Alloc> FABLE_STLP_OVERFLOW_OUTOFLINE void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    _Tp* newStart=this->_M_end_of_storage.allocate(len);
    _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
#endif
#endif
#ifdef FABLE_RETAIL_CTHREADEDFILE_ULONG_OVERFLOW_PASS30
// Native vector<unsigned long> growth owner uses the size-optimised STLport body.
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
// Inline-budget: the owner expands the unsigned-long fill loop.
template<> FABLE_FORCEINLINE unsigned long* fill_n<unsigned long*,unsigned int,unsigned long>(unsigned long* first,unsigned int n,const unsigned long& x) { for(;n>0;--n,++first) *first=x;return first; }
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<> inline void vector<unsigned long>::_M_insert_overflow(iterator position,const unsigned long& x,const __true_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();
    const size_type len=oldSize+(max)(oldSize,fillLength);
    iterator newStart=this->_M_end_of_storage.allocate(len);
    iterator newFinish=static_cast<iterator>(__copy_trivial(this->_M_start,position,newStart));
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=static_cast<iterator>(__copy_trivial(position,this->_M_finish,newFinish));
    _M_clear();
    _M_set(newStart,newFinish,newStart+len);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#pragma optimize("",on)
#endif
#endif
#if defined(FABLE_RETAIL_OCCLUSION_HANDLE_CLEAR_PASS30) && defined(_MSC_VER)
// Inline-budget: 0046496F emits the same range erase in size/frame mode.
#pragma optimize("s",on)
#pragma optimize("y",off)
template<> inline unsigned long* vector<unsigned long>::erase(iterator first,iterator last)
{
    iterator result=__copy_ptrs(last,this->_M_finish,first,_IsPODType());
    _Destroy(result,this->_M_finish);
    this->_M_finish=result;
    return first;
}
#pragma optimize("",on)
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_RESERVE
// Retail opt-in (chunk 857, row 0x00AC8C80 COptimisedPrimitive::ComputeVertexCost): STLport 4.6 stl/_vector.c reserve, the body the
// landed out-of-line copy 0x00920FF0 (vector<C3DVector>) reproduces, visible but kept out of line as in the retail TU (call at
// 0x00AC8CCB). Seeing that reserve never captures the vector, VC7.1 keeps the local vector's _M_finish in a CPU location across the
// push_back loop (0x00AC8CE1 `mov ebx,[esp+7Ch]` ... 0x00AC8D45 `add ebx,0Ch`); declaration-only reloads it after every store.
// Pair with FABLE_RETAIL_STLP_VECTOR (the in-class declaration).
#ifdef FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
// NF56 opt-in (row 0x0077E580 vector<CIntelligentPointer<CThing> >::reserve, the explicit instantiation): STLport 4.6 stl/_vector.c
// reserve as written there -- `capacity() < __n`, the old size taken inside, the copy through the member template
// _M_allocate_and_copy (needs FABLE_RETAIL_STLP_VECTOR_C) -- whose inlined allocate + copy gives retail's frame (the `push ecx` word,
// the copy's tag homed over the `n` slot: 0x0077E580..0x0077E616); the body below allocates and copies in place and schedules
// the size computation first (134 v 151 bytes). Same out-of-line placement.
template<class _Tp,class _Alloc> __declspec(noinline) void vector<_Tp,_Alloc>::reserve(size_type n) {
    if(capacity()<n) {
        const size_type old_size=size();
        _Tp* tmp;
        if(this->_M_start) {
            tmp=_M_allocate_and_copy(n,this->_M_start,this->_M_finish);
            _M_clear();
        } else {
            tmp=this->_M_end_of_storage.allocate(n);
        }
        _M_set(tmp,tmp+old_size,tmp+n);
    }
}
#else
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
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR
// Retail branch (pass 12 header wave): STLport 4.6 _vector.h includes _vector.c, so every retail TU that calls resize() /
// insert(pos, n, x) saw _M_fill_insert's body (stl/_vector.c lines 61-86, reproduced here) and VC7.1 still kept the call out of
// line. The visible body changes the caller's CPU location allocation: 0x00AC2BE0 PartitionSpace matches only with it (2775v2791
// declaration-only; retail's _M_fill_insert<bool> is 0x004676EF).
#ifdef FABLE_RETAIL_ARENA_FILL_UNINITIALIZED_COPY_PASS29
// STLport 4.6.2 _uninitialized.h. Dispatch on the value type of the output iterator.
template<class I, class O> inline O uninitialized_copy(I first, I last, O result) {
    typedef typename iterator_traits<O>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_copy(first, last, result, IsPOD());
}
#endif
#ifdef FABLE_RETAIL_ARENA_INSERT_ADVANCE_PASS29
// STLport 4.6.2 _iterator_base.h. Raw pointers take the random-access overload.
template<class I, class N> inline void __advance(I& it, N n, const random_access_iterator_tag&) {
    it += n;
}
#ifndef FABLE_RETAIL_STLP_ADVANCE
template<class I, class N> inline void advance(I& it, N n) {
    __advance(it, n, typename iterator_traits<I>::iterator_category());
}
#else
template<class I, class N> inline void advance(I&, N);
#endif
#endif
#ifdef FABLE_RETAIL_LANDSCAPE_INLINE_COPIES_PASS30
// Fidelity: the ordinary STLport unsigned-long methods. Keep the bound size-optimized
// copies at 0046496F and 00464931 when they also expand in the landscape renderer.
#if defined(_MSC_VER) && !defined(__clang__)
#pragma optimize("s", on)
#endif
template<> inline unsigned long* vector<unsigned long>::erase(unsigned long* first, unsigned long* last)
{
    iterator result = __copy_ptrs(last, this->_M_finish, first, _IsPODType());
    _Destroy(result, this->_M_finish);
    this->_M_finish = result;
    return first;
}
template<> inline void vector<unsigned long>::resize(size_type n, const unsigned long& value)
{
    if (n < size())
        erase(begin() + n, end());
    else
        insert(end(), n - size(), value);
}
#if defined(_MSC_VER) && !defined(__clang__)
#pragma optimize("", on)
#endif
#endif
#ifndef FABLE_RETAIL_STLP_VECTOR_FILL_INSERT_DECL_ONLY
// The 00B68070 TU calls the separately emitted unsigned-long instance at 004649A3.
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
#ifdef FABLE_RETAIL_ARENA_FILL_UNINITIALIZED_COPY_PASS29
                std::uninitialized_copy(position,old_finish,this->_M_finish);
#else
                __uninitialized_copy(position,old_finish,this->_M_finish,_IsPODType());
#endif
#ifdef FABLE_RETAIL_ARENA_INSERT_ADVANCE_PASS29
                std::advance(this->_M_finish, elems_after);
#else
                this->_M_finish+=elems_after;
#endif
                fill(position,old_finish,x_copy);
            }
        } else _M_insert_overflow(position,x,_IsPODType(),n,false);
    }
}
#endif
#endif
// _pair.h
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#if defined(FABLE_RETAIL_STLP_PAIR_DEFAULT_CTOR_NOINLINE) && defined(_MSC_VER)
    // NF61 opt-in (round 7, row 0x00469381 GFSerialiseMapBinaryIn<CBlendedParticleEffectSet, long, ..>): the same STLport body, visible
    // but kept out of line -- retail calls pair<long, CBlendedParticleEffectSet>() as a COMDAT (0x00469435, Ego_r.pdb
    // ??0?$pair@JVCBlendedParticleEffectSet@@@std@@QAE@XZ) in that size TU, where VC7.1 expands it (also under /d2inlS5).
    __declspec(noinline) pair():first(_T1()),second(_T2()){}
#else
    pair():first(_T1()),second(_T2()){}
#endif
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#if defined(FABLE_RETAIL_UPDATE_ACTION_ANIMATION_PASS29) && defined(_MSC_VER)
    template<class U1,class U2> __declspec(noinline) pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
#else
    template<class U1,class U2> pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
#endif
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
#ifdef FABLE_RETAIL_STLP_DECAL_HEAP
// Retail opt-in (Codex 2026-11-10): original STLport4.6 _pair.h comparison.
#ifdef FABLE_RETAIL_STLP_PAIR_LESS_RIGHT_BYVALUE
// FITTED nested opt-in (hand3a w24, row 0x006B5890 __lower_bound<pair<long,float>*, pair<long,float>, less<..>, int>): the same comparison
// with the right operand taken BY VALUE. Retail loads val.second onto the x87 stack before the loop (0x006B58A0 `fld [eax+4]`, compared
// with `fcom [middle+4]`, popped at 0x006B58D3): VC7.1 hoists the scalar-replaced copy's loads; with both operands by const reference
// it keeps &val and reloads (`fld [middle+4] ; fcomp [ebx+4]`). No source evidence for the by-value spelling (STLport 4.6.2 _pair.h is
// const&; no other retail float-pair comparison to cross-check). Same value for every input. Default path unchanged.
template<class A,class B> inline bool operator<(const pair<A,B>& x,pair<A,B> y)
{return x.first<y.first || (!(y.first<x.first) && x.second<y.second);}
#else
template<class A,class B> inline bool operator<(const pair<A,B>& x,const pair<A,B>& y)
{return x.first<y.first || (!(y.first<x.first) && x.second<y.second);}
#endif
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
    // Per-TU opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): STLport 4.6 _list.h postfix decrement (retail calls
    // it out of line at 0x0042899B from the ADD_CHILD case).
    _List_iterator operator--(int) { _List_iterator tmp=*this; this->_M_decr(); return tmp; }
#endif
};
#if defined(FABLE_RETAIL_STLP_LIST_REVERSE) && !defined(FABLE_RETAIL_ICON_REVERSE_ITERATOR_PASS29)
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
#if defined(FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE) && !defined(FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE)
// NF82 opt-in (row 0x004BB270, set<CCharString>'s _Rb_tree::erase(const key_type&)): the same STLport 4.6 _iterator_base.h distance() /
// input-iterator __distance(), the body visible but kept out of line as a __cdecl COMDAT, as retail calls it (0x004B84A0, `call` at
// 0x004BB2A6 with the two tree iterators by reference and the tag pushed); seeing the body, VC7.1 knows it writes nothing and keeps the
// equal_range iterators in registers across the call, as retail does.
template<class _InputIterator> __declspec(noinline) ptrdiff_t __cdecl __distance(const _InputIterator& first,const _InputIterator& last,const input_iterator_tag&) {
    ptrdiff_t n=0; _InputIterator it(first); while(it!=last) { ++it; ++n; } return n;
}
template<class _InputIterator> inline ptrdiff_t distance(const _InputIterator& first,const _InputIterator& last) {
    return __distance(first,last,input_iterator_tag());
}
#endif
template<class _Tp,class _Alloc> class _List_base {
public:
#if defined(FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE)
    // Per-TU opt-in (acctC agentC12, row 0x005EEFF2 CTCInventoryAbilities::GetMagicListUIComponent; like
    // FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE): the _List_base constructor / destructor bodies below, visible but kept out of line,
    // as that TU calls the list<EHeroAbility> COMDATs (??0 0x005F88ED, ??1 0x005F8908). Unlike the declaration-only
    // FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE, c2 knows the constructor keeps no address of its allocator temporary.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#elif defined(FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE)
    // Per-TU opt-in (Codex name fix 2026-11-10, list<long>): retail calls the _List_base constructor and destructor COMDATs
    // out of line (??0?$_List_base@JV?$allocator@J@std@@@std@@QAE@ABV?$allocator@J@1@@Z 0x0042AB58, ??1.. 0x0042AB73) from the
    // owning classes' constructors and destructors (NUISystem::CAction 0x0042BE50, CTCInventoryTrade 0x0059EB0C, ...).
    // Declarations only; the bodies are the #else ones.
    _List_base(const _Alloc&);
    ~_List_base();
#else
#ifdef FABLE_RETAIL_STLP_LIST_BASE_CONVERT_ALLOCATOR
    // Retail opt-in (acctC agentC31, row 0x00601310 CTCInventoryMap::ConstructQuestsScreenPC): STLport 4.6 _list.h
    // `_List_base(const allocator_type& __a) : _M_node(_STLP_CONVERT_ALLOCATOR(__a, _Node), (_Node*)0)`, where for a compiler with
    // member templates _STLP_CONVERT_ALLOCATOR(__a, _Tp) is `__a` (the node allocator is built by allocator's converting
    // constructor from the element allocator, not default-constructed). Same code; one more inline layer in the caller size.
    _List_base(const _Alloc& a) : _M_node(a,(_List_node<_Tp>*)0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
#else
#ifdef FABLE_RETAIL_STLP_LIST_CTORS_PASS22
    // Retail opt-in (Codex pass22): original list-base constructor is called by native0042bed4.
    FABLE_STLP_NOINLINE
#endif
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
#endif
#if defined(FABLE_RETAIL_SUBTITLE_LIST_BASE_PASS29)
    // Inline-budget: the list and its base both expand this destruction sequence at native 004046B0.
    FABLE_FORCEINLINE ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#elif defined(FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE)
    // Retail opt-in (Codex pass19): native008fada0 calls0048ca60 base destructor.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#elif defined(FABLE_RETAIL_CONSOLE_COMMENT_LIST_OWNER_PASS29)
    // Inline-budget: both 004046b0 destructor spellings expand clear/free in full.
    FABLE_FORCEINLINE ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#elif !defined(FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST)
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#ifdef FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFCLASS
    // Per-TU opt-in (chunk 4007, row 0x0061B610 CTCInventoryQuests::ConstructQuestList): STLport 4.6 declares clear() in the class
    // (_list.h) and defines it after the class as a plain template member with no `inline` (_list.c); the body is the one below.
    void clear();
#elif defined(FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE)
    // Per-TU opt-in (chunk 801, row 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering): STLport 4.6 _list.c
    // _List_base::clear(), declared only. Retail calls the list<CEnginePrimitiveHandle> instantiation out of line at 0x00834F90
    // (`lea edi,[esi+194h] ; mov ecx,edi ; call 0x00834F90`, 0x00833C0C; the landed ~CThingAICreature 0x008320C0 binds the same
    // address as ?clear@?$_List_base@VCEnginePrimitiveHandle@@..@std@@QAEXXZ).
    void clear();
#else
#ifdef FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
    // Per-TU opt-in (chunk 1011, row 0x005FCBE0 CTCInventoryMap::ConstructPastQuestsList): the same STLport 4.6 _list.c body, visible
    // but kept out of line, as retail calls it (_List_base<CIntelligentPointer<CThing> >::clear 0x004B8050). Unlike the
    // declaration-only FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE, c2 then knows the call leaves the list's head word alone: retail keeps
    // the head node in esi across the clear and frees it with no reload and no null test (`call 0x004B8050 ; push esi ; call free`,
    // 0x005FCCE3; the landed 0x005FD410 GetRegionsToDisplay has the same shape with a visible body).
    FABLE_STLP_NOINLINE
#endif
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
#ifdef FABLE_RETAIL_STLP_LIST_SORT
// Per-TU opt-in (wf9, row 0x006F3A30 _S_sort<const CThingCreatureBase*, allocator, CThingCompare_Nearest>): STLport 4.6 _list.h
// _List_global / _list.c _Transfer (the six-store splice under `position != last`), for list::splice below.
template<class _Dummy> class _List_global {
public:
    typedef _List_node_base _Node;
    static void _Transfer(_List_node_base* __position,_List_node_base* __first,_List_node_base* __last);
};
template<class _Dummy> void _List_global<_Dummy>::_Transfer(_List_node_base* __position,_List_node_base* __first,_List_node_base* __last) {
    if(__position!=__last) {
        __last->_M_prev->_M_next=__position;
        __first->_M_prev->_M_next=__last;
        __position->_M_prev->_M_next=__first;
        _Node* __tmp=__position->_M_prev;
        __position->_M_prev=__last->_M_prev;
        __last->_M_prev=__first->_M_prev;
        __first->_M_prev=__tmp;
    }
}
typedef _List_global<bool> _List_global_inst;
#endif
#ifdef FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFCLASS
template<class _Tp,class _Alloc> void _List_base<_Tp,_Alloc>::clear() {
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
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
    typedef _Tp value_type;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#ifdef FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE
    // Per-TU opt-in (NF42 round 5): STLport's inline default constructor kept out of line with its body visible. 0x0042ABAF is
    // _List_base<CComponent*>'s constructor (list's own 0x0042B6A6 calls it; the Adjust*OptionsSliders rows use _BASE_VISIBLE_NOINLINE
    // since NF241 2026-11-22). Needs FABLE_STLP_NOINLINE (e.g. FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE).
    FABLE_STLP_NOINLINE
#endif
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#ifdef FABLE_RETAIL_STLP_LIST_CTORS_PASS22
    FABLE_STLP_NOINLINE
#endif
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) { insert(begin(),x.begin(),x.end()); }
#ifdef FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
    // Per-TU opt-in (chunk 892, row 0x005A887D CTCInventoryTrade::RefreshBuyPossibilityPC): STLport's inline destructor, kept out of
    // line with its body visible, as the retail TU calls it (~list<CComponent*> 0x0042ABCA at 0x005A8A33). __declspec(noinline) is
    // MSVC's spelling (FABLE_STLP_NOINLINE is empty elsewhere).
    FABLE_STLP_NOINLINE ~list() { }
#elif defined(FABLE_RETAIL_PERSONAL_SCRIPT_LIST_DTOR_PASS26)
    // Inline-budget: the native personal script calls list::~list on its first exit and
    // otherwise expands clear and deallocate, including its normal loop backedge.
    FABLE_FORCEINLINE ~list() { this->clear();this->_M_node.deallocate(this->_M_node._M_data,1); }
#elif defined(FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST)
    // Per-TU opt-in: the clear+free body in list::~list itself and no ~_List_base; bytes only. The PDBs' ??1?$list is Dinkumware
    // (_List_val, no _List_base type), not STLport evidence: retail's list<CComponent*> `~list() {}` 0x0042B6D2 is a 5-byte
    // `jmp 0x0042ABCA`, ~_List_base (NF239 / NF241, 2026-11-22; row 0x00607330 now uses the default path).
    ~list() { this->clear();this->_M_node.deallocate(this->_M_node._M_data,1); }
#elif defined(FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY)
    // Per-TU opt-in (agentNF187, row 0x01228E44 `dynamic atexit destructor for 'NUISystem::CList::GPreviousSelected''): ~list declared
    // only, so the TU calls the landed out-of-line copy (0x0053C9BC, ~_List_base's clear + free expanded in it) and emits no pick-any
    // copy of it (the size-optimised `~list() { }` copy is a 5-byte `jmp ~_List_base`, not 0x0053C9BC's body).
    ~list();
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
#ifdef FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
    // Per-TU opt-in (row 0x00607330): STLport's range-insert body visible but kept out of line, as the retail TU calls it at every
    // site (list<CComponent*>::_M_insert_dispatch 0x0042B4C3 at 0x0060780B / 0x006095B3, the CDeletion copies).
    template<class Input>
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) { for(;first!=last;++first) insert(position,*first); }
#else
    template<class Input> void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) { for(;first!=last;++first) insert(position,*first); }
#endif
protected:
#ifdef FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE
    // Per-TU opt-in (hand4b w24, row 0x006E31E0 NSpeechGainManager::OnDialogueStart): STLport 4.6 _list.h _M_create_node, body
    // visible but kept out of line, as the retail TU calls the list<CActiveDialogueInfo> COMDAT 0x006E4CB0 from the inlined
    // insert(iterator, const T&) (`push edx ; mov ecx,13BABA0h ; call 0x006E4CB0`, 0x006E33C5..0x006E33E0, then the four link
    // stores in place). Same body as the default; no layout change.
    FABLE_STLP_NOINLINE
#endif
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
#ifdef FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
        _Construct(&p->_M_data,x);
#else
        new((void*)&p->_M_data) _Tp(x);
#endif
        return p;
    }
public:
#ifdef FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
    // Per-TU opt-in (chunk 892, row 0x005A887D): STLport's insert(iterator, const T&), body visible but kept out of line, as the
    // retail TU calls it (list<CComponent*>::insert 0x0042AB8A at 0x005A8A14, from push_back): VC7.1 then knows it keeps neither
    // the list's nor the value's address.
    FABLE_STLP_NOINLINE iterator insert(iterator position,const _Tp& x) {
#else
#ifdef FABLE_RETAIL_BUY_FROM_SELLER_LIST_INSERT_PASS30
    // Inline-budget: 004BA4E0 expands insert in the 73-byte push_back of a CIntelligentPointer<CThing>.
    FABLE_FORCEINLINE
#endif
    iterator insert(iterator position,const _Tp& x) {
#endif
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
#ifdef FABLE_RETAIL_STLP_LIST_PUSHBACK_DECL_PASS26
    // Quest completion 0x004B1EE4 calls the existing STLport owner at 0x004BA4E0.
    void push_back(const _Tp& x);
#else
#ifdef FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
    FABLE_STLP_NOINLINE
#endif
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT
    // Per-TU opt-in (chunk 801, row 0x00833C00): STLport 4.6 _list.h `iterator insert(iterator __pos)` (the no-default-parameter
    // overload, `return insert(__pos, _Tp());`), declared only: retail calls it out of line at 0x007464A0 (the landed body builds the
    // default CEnginePrimitiveHandle and links the node), with the iterator copied in place into the argument slot (`push ecx ;
    // mov ecx,[edi] ; mov eax,esp ; mov [eax],ecx`, 0x00833C74) and the result through a hidden pointer. push_back() is the
    // STLport inline `insert(end())`.
    iterator insert(iterator position);
    void push_back() { insert(end()); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT_NOINLINE
    // Per-TU opt-in (chunk Q6, row 0x00A66FD0 CMouse::ProcessMaintainedEventsFromEvent): STLport 4.6 _list.h `iterator
    // insert(iterator __pos) { return insert(__pos, _Tp()); }` and `void push_back() { insert(end()); }`, bodies visible but kept
    // out of line, as the retail TU calls them (list<CMaintainedEvent>::insert 0x009E4A20: the default value built in the frame,
    // the node copy-constructed and linked, the value destroyed; push_back 0x009E4AA0). VC7.1 then knows neither keeps the list's
    // nor the iterator's address.
    FABLE_STLP_NOINLINE iterator insert(iterator position) { return insert(position,_Tp()); }
    FABLE_STLP_NOINLINE void push_back() { insert(end()); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST
    // Retail branch (chunk 726): STLport 4.6 _list.h size() / front() / push_front(). size() is distance(begin(), end())
    // with the input-iterator __distance loop inlined (NUISystem::CList::ChangeState 0x0053A0A1 counts the
    // GPreviousSelected nodes in place: mov ecx,[ecx] ; inc edi ; cmp ecx,eax ; jne); push_front(x) is insert(begin(), x)
    // (out of line in that TU at 0x0053CA3C).
#if defined(FABLE_RETAIL_STLP_LIST_SIZE_DECL)
    // NF76 per-TU opt-in (row 0x006D53C0 CTCFollowed::FrameUpdate): STLport 4.6 _list.h size(), declared only. That TU calls the
    // list<CIntelligentPointer<CThingAICreature> > instance out of line (`mov ecx,edi ; call 0x0065F17B` at 0x006D562F, the GFRandom
    // range; ?size@?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@..@std@@@std@@QBEIXZ, queue entry nf41:e53).
    size_t size() const;
#elif defined(FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE)
    // chunk 849 (row 0x0057E2F1): STLport 4.6's own spelling, `size_type __result = distance(begin(), end()); return __result;`. The
    // copy-of-first loop keeps the head in eax and walks ecx (`mov eax,[ebx+1D8h] ; mov ecx,[eax] ; .. mov ecx,[ecx] ; inc edx ;
    // cmp ecx,eax`, 0x0057EBA3, before the vector reserve); the for-loop form above swaps the two CPU locations.
    size_t size() const { size_t result=distance(begin(),end()); return result; }
#else
    size_t size() const { ptrdiff_t n=0; for(const_iterator first=begin(),last=end();first!=last;++first) ++n; return n; }
#endif
    _Tp& front() { return *begin(); }
#ifdef FABLE_RETAIL_STLP_LIST_CONST_FRONT
    // Retail opt-in (Codex pass 24). Original STLport 4.6 const overload.
    const _Tp& front() const { return *begin(); }
#endif
    void push_front(const _Tp& x) { insert(begin(),x); }
    // chunk 736 (row 0x00A7A0A0): STLport 4.6 _list.h back(): `iterator __tmp = end(); return *(--__tmp);` (SetUpRegions reads
    // Regions.back() as the list head's _M_prev before each MakeRegion call: `mov eax,[edx+4] ; add eax,8`).
    _Tp& back() { iterator tmp=end(); return *(--tmp); }
    const _Tp& back() const { const_iterator tmp=end(); return *(--tmp); }
    // chunk 749 (row 0x00549440 CTextSlider::ProcessEvent): STLport 4.6 _list.h erase(iterator) and pop_front() (`erase(begin())`);
    // retail calls the list<NUISystem::CList*> instances out of line (pop_front 0x0053C7D9 -> erase 0x0053C7B9).
#ifdef FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE
    // Per-TU opt-in (chunk Q6, row 0x00A66FD0): the same body, kept out of line as the retail TU calls it
    // (list<CMaintainedEvent>::erase 0x009E47E0: unlink, the element's virtual destructor, free; iterator by value, result by
    // hidden pointer).
    FABLE_STLP_NOINLINE
#endif
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
#ifdef FABLE_RETAIL_STLP_LIST_ASSIGN_BODY
    // NF78 opt-in (round 7, row 0x00ADD020 list<CVertexGroupBlock>::operator=): STLport 4.6 _list.c operator=(const list&) (body after
    // the class) and _list.h erase(first, last), declared only: retail calls that instance's erase COMDAT (0x00ADC4E0) from it.
    iterator erase(iterator first,iterator last);
    list& operator=(const list& x);
#endif
#ifdef FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL
    // Per-TU opt-in (wf20, row 0x00AD9F10 CVertexGroupBlockOptimiser::AddBlocks): STLport 4.6 _list.c operator=(const list&) and
    // _list.h pop_back(), called out of line by that row (list<CVertexGroupBlock> 0x00ADD020 / 0x00ADC790). Declarations only.
    list& operator=(const list& x);
    void pop_back();
#endif
#ifdef FABLE_RETAIL_STLP_LIST_REMOVE_DECL
    // Per-TU opt-in (agentNF 2026-11-10, rows 0x00846100 / 0x00BBCE10): STLport 4.6 _list.h `void remove(const _Tp& __val);`, declared
    // only (its body is in _list.c). Retail calls the list<long> instance out of line (?remove@?$list@JV?$allocator@J@std@@@std@@QAEXABJ@Z
    // 0x0084ACE0, FableWin.pdb name set); a row that needs the same-TU body defines it as an explicit specialisation.
    void remove(const _Tp& __val);
#endif
#if defined(FABLE_RETAIL_STLP_LIST_REMOVE_NOINLINE) && !defined(FABLE_RETAIL_STLP_LIST_REMOVE_DECL)
    // NF92 opt-in (round 7, q11218 row 0x004C8050 CThing::WorldUpdate): the STLport 4.6 _list.c remove(const _Tp&) body, visible but
    // called out of line, as retail calls list<CTCBase*>::remove 0x004CC150 (?remove@?$list@PAVCTCBase@@V?$allocator@PAVCTCBase@@@std@@@std@@
    // QAEXABQAVCTCBase@@@Z) from its implicit instantiation in the caller's TU. Seeing that the body keeps no reference to the value, VC7.1
    // leaves the caller's component in a register across the call (0x004C8129: ebp reused for GetInterfaceType, the compare and the delete)
    // and shares the argument's frame word with the later Find key. Needs the list erase(iterator) body (FABLE_RETAIL_STLP_LIST).
#ifdef _MSC_VER
    __declspec(noinline) void remove(const _Tp& __val) {
#else
    void remove(const _Tp& __val) {
#endif
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__val == *__first) erase(__first);
            __first = __next;
        }
    }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_SORT
    // Per-TU opt-in (wf9, row 0x006F3A30): STLport 4.6 _list.h swap(list&), splice(iterator, list&, iterator) and the member
    // template sort(_StrictWeakOrdering) (`_S_sort(*this, __comp);`, retail forwarder 0x006F4040).
    void swap(list<_Tp,_Alloc>& __x) { _List_node<_Tp>* __tmp=this->_M_node._M_data; this->_M_node._M_data=__x._M_node._M_data; __x._M_node._M_data=__tmp; }
    void splice(iterator __position,list<_Tp,_Alloc>&,iterator __i) {
        iterator __j=__i;
        ++__j;
        if(__position==__i || __position==__j) return;
        _List_global_inst::_Transfer(__position._M_node,__i._M_node,__j._M_node);
    }
    template<class _StrictWeakOrdering> void sort(_StrictWeakOrdering __comp) { _S_sort(*this,__comp); }
#endif
#ifdef FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
    // chunk 767 (row 0x0082A380 CMiniMapDisplay::RemoveMarker): STLport 4.6 _list.h member template remove_if(_Predicate), declared
    // only: retail tail-jumps to the out-of-line instantiation (0x0082C770, ??$remove_if@VCIsMarkerBoundToThing@CMiniMapDisplay@@@..).
    template<class _Predicate> void remove_if(_Predicate pred);
#endif
#if defined(FABLE_RETAIL_STLP_LIST_REMOVE_IF_NOINLINE) && !defined(FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE)
    // NF67 opt-in (round 7, row 0x008A7980 CGameScriptInterface::EntityStopFollowing): the STLport 4.6 _list.c member template
    // remove_if(_Predicate) body, visible but called out of line, as retail calls list<CCountedPointer<CEventBase> >::remove_if<
    // CConvFilterToCP<CIsEventOfType> > 0x008AC890 there. Seeing that it keeps no reference to the predicate, VC7.1 packs the caller's
    // CIsEventOfType temporary into the dead argument slot (0x008A7A4D `mov dword ptr [esp+14h],3`) as retail does. Needs the list
    // erase(iterator) body (FABLE_RETAIL_STLP_LIST).
#ifdef _MSC_VER
    template<class _Predicate> __declspec(noinline) void remove_if(_Predicate __pred) {
#else
    template<class _Predicate> void remove_if(_Predicate __pred) {
#endif
        iterator __first=begin();
        iterator __last=end();
        while(__first!=__last) {
            iterator __next=__first;
            ++__next;
            if(__pred(*__first)) erase(__first);
            __first=__next;
        }
    }
#endif
};
#ifdef FABLE_RETAIL_STLP_LIST_ASSIGN_BODY
// NF78 opt-in: STLport 4.6 _list.c operator=: assign over the existing elements pairwise, then erase the surplus or insert copies of
// the rest (range insert = _M_insert_dispatch).
template<class _Tp,class _Alloc>
list<_Tp,_Alloc>& list<_Tp,_Alloc>::operator=(const list<_Tp,_Alloc>& x) {
    if(this!=&x) {
        iterator first1=begin();
        iterator last1=end();
        const_iterator first2=x.begin();
        const_iterator last2=x.end();
        while(first1!=last1 && first2!=last2)
            *first1++=*first2++;
        if(first2==last2)
            erase(first1,last1);
        else
            insert(last1,first2,last2);
    }
    return *this;
}
#endif
#ifdef FABLE_RETAIL_STLP_LIST_SORT
// Per-TU opt-in (wf9, row 0x006F3A30): STLport 4.6 _list.c _S_merge (body visible; retail calls the instance out of line, 0x006F04C0)
// and _S_sort, free templates (__fastcall in the /Gr STL TUs: the list(s) in ecx/edx, the comparator by value on the stack).
template<class _Tp,class _Alloc,class _StrictWeakOrdering>
void FABLE_STLP_FASTCALL _S_merge(list<_Tp,_Alloc>& __that,list<_Tp,_Alloc>& __x,_StrictWeakOrdering __comp) {
    typedef typename list<_Tp,_Alloc>::iterator _Literator;
    _Literator __first1=__that.begin();
    _Literator __last1=__that.end();
    _Literator __first2=__x.begin();
    _Literator __last2=__x.end();
    while(__first1!=__last1 && __first2!=__last2)
        if(__comp(*__first2,*__first1)) {
            _Literator __next=__first2;
            _List_global_inst::_Transfer(__first1._M_node,__first2._M_node,(++__next)._M_node);
            __first2=__next;
        }
        else
            ++__first1;
    if(__first2!=__last2) _List_global_inst::_Transfer(__last1._M_node,__first2._M_node,__last2._M_node);
}
template<class _Tp,class _Alloc,class _StrictWeakOrdering>
void FABLE_STLP_FASTCALL _S_sort(list<_Tp,_Alloc>& __that,_StrictWeakOrdering __comp) {
    // Do nothing if the list has length 0 or 1.
    if(__that._M_node._M_data->_M_next!=__that._M_node._M_data &&
       (__that._M_node._M_data->_M_next)->_M_next!=__that._M_node._M_data) {
        list<_Tp,_Alloc> __carry;
        list<_Tp,_Alloc> __counter[64];
        int __fill=0;
        while(!__that.empty()) {
            __carry.splice(__carry.begin(),__that,__that.begin());
            int __i=0;
            while(__i<__fill && !__counter[__i].empty()) {
                _S_merge(__counter[__i],__carry,__comp);
                __carry.swap(__counter[__i++]);
            }
            __carry.swap(__counter[__i]);
            if(__i==__fill) ++__fill;
        }
        for(int __i=1;__i<__fill;++__i)
            _S_merge(__counter[__i],__counter[__i-1],__comp);
        __that.swap(__counter[__fill-1]);
    }
}
#endif
#ifdef FABLE_RETAIL_STLP_LIST_SIZE_OWNER_MODE_PASS26
// Fidelity: scaffolding, not retail. The original list size is kept from size-mode owner0065f17b.
}
class CThingAICreature;
template<class T>class CIntelligentPointer;
#pragma optimize("s",on)
namespace std {
template<> inline size_t list<CIntelligentPointer<CThingAICreature> >::size() const {size_t result=distance(begin(),end());return result;}
#pragma optimize("",on)
#endif
// _bitset.h / _range_errors.h (no-exceptions branch)
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
#if defined(FABLE_RETAIL_MINIMAP_REGION_EXIT_MARKERS_PASS30) && defined(_MSC_VER)
    // STLport 4.6.1 _bitset.h specifies _STLP_CALL (__cdecl) on these three static members.
    static size_t __cdecl _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t __cdecl _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT __cdecl _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
#else
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
#endif
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
#ifdef FABLE_RETAIL_STLP_BITSET_SET_DECL
    // NF90 per-TU opt-in (round 7, q11188 binder row 0x004C9D60 CThing::AddTC): STLport 4.6 _bitset.h set(size_t) (the range check, then
    // _Unchecked_set), declared only. That retail TU calls the bitset<274> instance out of line (0x004CC090, landed as std::bitset::set,
    // `push type ; lea ecx,[UsingComponents] ; call` at 0x004C9F13) while it expands test() in place. No layout change.
    bitset<_Nb>& set(size_t n);
#endif
#ifdef FABLE_RETAIL_STLP_BITSET_RESET_DECL
    // NF92 per-TU opt-in (round 7, q11218 binder row 0x004C9840 CThing::RemoveTC): STLport 4.6 _bitset.h reset(size_t) (the range check, then
    // _Unchecked_reset), declared only. That retail TU calls the bitset<274> instance out of line (0x004CC0D0, `push type ; lea ecx,
    // [UsingComponents] ; call`), the twin of NF90's set() 0x004CC090. No layout change.
    bitset<_Nb>& reset(size_t n);
#endif
#if defined(FABLE_RETAIL_STLP_BITSET_RESET) && !defined(FABLE_RETAIL_STLP_BITSET_RESET_DECL)
    // NF92 per-TU opt-in (round 7, q11218 binder row 0x004C8310 CThing::~CThing): STLport 4.6 _bitset.h reset(size_t) and _Unchecked_reset
    // inline, as that retail TU expands them (the range check against 274 with the cold puts / abort block, then the word and-not).
    bitset<_Nb>& _Unchecked_reset(size_t n) { this->_M_w[this->_S_whichword(n)] &= ~this->_S_maskbit(n); return *this; }
    bitset<_Nb>& reset(size_t n) { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_reset(n); }
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
#ifdef FABLE_RETAIL_STLP_BITSET_TEST_OWNER_PASS27
// Keep the genuine bitset<274>::test body in its speed-mode owner's compile context.
template<> inline bool bitset<274>::test(size_t n) const
{
    if(n>=274) __stl_throw_out_of_range("bitset");
    return _Unchecked_test(n);
}
#endif
#ifdef FABLE_RETAIL_STLP_BITSET_TEST_SPEED_OWNER_PASS27
template<> inline bool bitset<274>::test(size_t n) const
{
    if (n >= 274)
        __stl_throw_out_of_range("bitset");
    return _Unchecked_test(n);
}
#endif
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
#ifdef FABLE_RETAIL_STLP_BINARY_COMPOSE
// Retail opt-in (chunk 1012, row 0x0049FFD0 CWorld::SetKickPos): STLport 4.6 _function_base.h logical_or and _function.h
// binary_compose / compose2 (the SGI extensions STLport keeps in namespace std). Ego_r.pdb mangles the GetPBestThing instance 0x004AC2C0
// as ...V?$binary_compose@U?$logical_or@_N@std@@VCThingFilter_HasTC@@V3@@std@@...: logical_or a struct, binary_compose a class
// holding _M_fn1 / _M_fn2 / _M_fn3 by value (SetKickPos builds it at +0x00 / +0x04 / +0x08 of its frame copy, 0x004A0083).
template<class _Tp> struct logical_or : public binary_function<_Tp,_Tp,bool> {
    bool operator()(const _Tp& x,const _Tp& y) const { return x||y; }
};
template<class _Operation1,class _Operation2,class _Operation3>
class binary_compose : public unary_function<typename _Operation2::argument_type,typename _Operation1::result_type> {
protected:
    _Operation1 _M_fn1;
    _Operation2 _M_fn2;
    _Operation3 _M_fn3;
public:
    binary_compose(const _Operation1& __x,const _Operation2& __y,const _Operation3& __z):_M_fn1(__x),_M_fn2(__y),_M_fn3(__z){}
    typename _Operation1::result_type operator()(const typename _Operation2::argument_type& __x) const { return _M_fn1(_M_fn2(__x),_M_fn3(__x)); }
};
template<class _Operation1,class _Operation2,class _Operation3>
inline binary_compose<_Operation1,_Operation2,_Operation3>
compose2(const _Operation1& __fn1,const _Operation2& __fn2,const _Operation3& __fn3) {
    return binary_compose<_Operation1,_Operation2,_Operation3>(__fn1,__fn2,__fn3);
}
#endif
#ifdef FABLE_RETAIL_STLP_SORT_INLINE
// Retail opt-in (chunk 2005, row 0x00A12EE0 CPhysicsMeshObject::GetNavigationLevelVolumes): STLport 4.6 _algo.c sort(first, last, comp)
// expanded in place (0x00A132B9..0x00A13328): the empty-range test, __introsort_loop (out of line, __fastcall: first in ecx, last in
// edx, the (T*)0, the depth __lg(n) * 2 and the comparator pushed; 0x008274B0 for pair<long, CCountedPointer<CContainmentVolume> >)
// and the inline __final_insertion_sort, whose __insertion_sort (0x008270E0) and __unguarded_insertion_sort_aux (0x00825B40) are
// out of line __fastcall instantiations. Declarations only for the three out-of-line helpers.
#ifndef __stl_threshold
#define __stl_threshold 16
#endif
template<class _Size> inline _Size __lg(_Size __n) { _Size __k; for(__k=0;__n!=1;__n>>=1) ++__k; return __k; }
template<class _RandomAccessIter,class _Tp,class _Size,class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first,_RandomAccessIter __last,_Tp*,_Size __depth_limit,_Compare __comp);
template<class _RandomAccessIter,class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp);
template<class _RandomAccessIter,class _Tp,class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first,_RandomAccessIter __last,_Tp*,_Compare __comp);
template<class _RandomAccessIter,class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp) {
    __unguarded_insertion_sort_aux(__first,__last,(typename iterator_traits<_RandomAccessIter>::value_type*)0,__comp);
}
template<class _RandomAccessIter,class _Compare>
inline void __final_insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp) {
    if(__last-__first>__stl_threshold) {
        __insertion_sort(__first,__first+__stl_threshold,__comp);
        __unguarded_insertion_sort(__first+__stl_threshold,__last,__comp);
    } else
        __insertion_sort(__first,__last,__comp);
}
template<class _RandomAccessIter,class _Compare>
#if defined(FABLE_RETAIL_STLP_SORT_INLINE_FORCE) && defined(_MSC_VER)
__forceinline
#else
inline
#endif
void sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp) {
    if(__first!=__last) {
        __introsort_loop(__first,__last,(typename iterator_traits<_RandomAccessIter>::value_type*)0,__lg(__last-__first)*2,__comp);
        __final_insertion_sort(__first,__last,__comp);
    }
}
#elif defined(FABLE_RETAIL_STLP_SORT_FASTCALL)
// Retail opt-in (chunk 796, row 0x008E18D0): the std::sort instantiations are __fastcall in retail (first in ecx, last in edx, the
// comparator pushed: 0x0076C000 / 0x0076C1A0 are bound as ??$sort@...@std@@YIX...; the landed 0x00767DA0 local model spells the
// same). A declaration only.
template<class _RandomAccessIter,class _Compare> void __fastcall sort(_RandomAccessIter,_RandomAccessIter,_Compare);
#ifdef FABLE_RETAIL_STLP_SORT_FASTCALL_BODY
// NF72 opt-in (row 0x00A84160 sort<pair<unsigned __int64, bool>*, CKeyPairCompareLess<unsigned __int64, bool> >): STLport 4.6 _algo.c
// sort(first, last, comp), the __fastcall COMDAT itself (first in ecx, last in edx, the comparator pushed, `ret 4`): the empty-range
// test, __introsort_loop (out of line, __fastcall: the (T*)0, the depth __lg(n) * 2 and the comparator pushed) and the inline
// __final_insertion_sort, whose __insertion_sort and __unguarded_insertion_sort_aux are out of line __fastcall instantiations -- the
// FABLE_RETAIL_STLP_SORT_INLINE text with sort defined __fastcall. Declarations only for the three out-of-line helpers.
#ifndef __stl_threshold
#define __stl_threshold 16
#endif
template<class _Size> inline _Size __lg(_Size __n) { _Size __k; for(__k=0;__n!=1;__n>>=1) ++__k; return __k; }
template<class _RandomAccessIter,class _Tp,class _Size,class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first,_RandomAccessIter __last,_Tp*,_Size __depth_limit,_Compare __comp);
template<class _RandomAccessIter,class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp);
template<class _RandomAccessIter,class _Tp,class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first,_RandomAccessIter __last,_Tp*,_Compare __comp);
template<class _RandomAccessIter,class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp) {
    __unguarded_insertion_sort_aux(__first,__last,(typename iterator_traits<_RandomAccessIter>::value_type*)0,__comp);
}
#ifdef FABLE_RETAIL_STLP_SORT_FINAL_INSERTION_OUTOFLINE
// NF180 nested opt-in (row 0x00CB7780 sort<pair<CCharString, CScriptInfo>*, CKeyPairCompareLess<CCharString, CScriptInfo> >): that
// sort COMDAT calls __final_insertion_sort out of line (0x00CB70E0, landed ??$__final_insertion_sort@...@std@@YIX..., __fastcall:
// first in ecx, last in edx, the comparator pushed) instead of expanding it. A declaration only.
template<class _RandomAccessIter,class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp);
#else
template<class _RandomAccessIter,class _Compare>
#if defined(_MSC_VER)
__forceinline
#else
inline
#endif
void __final_insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp) {
    if(__last-__first>__stl_threshold) {
        __insertion_sort(__first,__first+__stl_threshold,__comp);
        __unguarded_insertion_sort(__first+__stl_threshold,__last,__comp);
    } else
        __insertion_sort(__first,__last,__comp);
}
#endif
template<class _RandomAccessIter,class _Compare>
void __fastcall sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp) {
    if(__first!=__last) {
        __introsort_loop(__first,__last,(typename iterator_traits<_RandomAccessIter>::value_type*)0,__lg(__last-__first)*2,__comp);
        __final_insertion_sort(__first,__last,__comp);
    }
}
#endif
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
#ifdef FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG
// Per-TU opt-in (wf4, row 0x008F10B0 CTCTavernGameCardBase::ShuffleCards): STLport 4.6 _algo.h two-argument random_shuffle, declared
// only; retail calls the CCard** instantiation out of line as __fastcall (first ecx, last edx; 0x007D7860, landed with /Gr).
template<class _RandomAccessIter> void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter);
#endif
#ifdef FABLE_RETAIL_STLP_MEM_FUN
// Per-TU opt-in (chunk 729): STLport 4.6 _function_base.h const_mem_fun_ref_t / mem_fun_ref (Ego_r.pdb
// ??$GetAllThings@VCThingCreatureBase@@...V?$const_mem_fun_ref_t@_NVCThingCreatureBase@@@std@@...; retail 0x00603000 stores the
// 4-byte member pointer 0x005FA7F0 IsDemonDoor in the filter temporary).
template<class _Ret,class _Tp> class const_mem_fun_ref_t : public unary_function<_Tp,_Ret> {
    typedef _Ret (_Tp::*__fun_type)(void) const;
public:
    explicit const_mem_fun_ref_t(__fun_type __pf) : _M_f(__pf) {}
    _Ret operator()(const _Tp& __r) const { return (__r.*_M_f)(); }
private:
    __fun_type _M_f;
};
template<class _Ret,class _Tp> inline const_mem_fun_ref_t<_Ret,_Tp> mem_fun_ref(_Ret (_Tp::*__f)() const) {
    return const_mem_fun_ref_t<_Ret,_Tp>(__f);
}
#endif
#ifdef FABLE_RETAIL_STLP_MEM_FUN_PTR
// Per-TU opt-in (wfA2, row 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine): STLport 4.6 _function_base.h mem_fun_t / mem_fun
// (the pointer form; Ego_r.pdb ??$for_each@PAPAVCDrawBase@NPlayerGui@@V?$mem_fun_t@XVCDrawBase@NPlayerGui@@@std@@@std@@..., retail COMDAT
// 0x0043F310, called with the 4-byte member pointer = a vcall thunk; the landed ClearGui 0x00438230 models the same functor locally).
template<class _Ret,class _Tp> class mem_fun_t : public unary_function<_Tp*,_Ret> {
    typedef _Ret (_Tp::*__fun_type)(void);
public:
    explicit mem_fun_t(__fun_type __pf) : _M_f(__pf) {}
    _Ret operator()(_Tp* __p) const { return (__p->*_M_f)(); }
private:
    __fun_type _M_f;
};
template<class _Ret,class _Tp> inline mem_fun_t<_Ret,_Tp> mem_fun(_Ret (_Tp::*__f)()) {
    return mem_fun_t<_Ret,_Tp>(__f);
}
#endif
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
#ifdef FABLE_RETAIL_STLP_COPY_GENERIC_INLINE
// Per-TU opt-in (NF62, rows 0x007CDFE0 CTCBurntEffect / 0x007CEB00 CTCElectrocutedEffect constructors): STLport 4.6 _algobase.h copy() with
// its body -- `return __copy_aux(first, last, result, _BothPtrType<I,O>::_Ret());`, inline -- in place of the out-of-line declaration
// in the #else arm; a CThing** range into a back_insert_iterator<vector<CIntelligentPointer<CThing> > > then reaches the random-access __copy
// instance out of line (0x007CE1F0, __fastcall, hidden result in ecx, first in edx, the tag and the (int*)0 distance pushed).
// Needs FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22 (__copy_aux) and FABLE_RETAIL_STLP_COPY_NOINLINE (__copy kept out of line).
template<class _InputIter,class _OutputIter> inline _OutputIter copy(_InputIter first,_InputIter last,_OutputIter result) {
    return __copy_aux(first,last,result,_BothPtrType<_InputIter,_OutputIter>::_Ret());
}
#else
template<class _InputIter,class _OutputIter> _OutputIter __fastcall copy(_InputIter first,_InputIter last,_OutputIter result);
#endif
#endif
#ifdef FABLE_RETAIL_STLP_SORT_INTERNALS
// Per-TU opt-in (chunk 892, rows 0x00807250 __unguarded_partition and 0x00808AA0 __linear_insert, the first-person targeting
// sort of pair<C3DVector,long>): STLport 4.6 _algo.h sort internals and _algobase.h copy_backward / iter_swap. The retail TU is
// compiled /Gr, so the out-of-line instantiations are __fastcall (first in ecx, last in edx; Ego_r.pdb's Dinkumware counterparts
// are _Unguarded_partition / _Insertion_sort1). __copy_backward is out of line in retail (0x00806310 for pair<C3DVector,long>*,
// the tag and the (int*)0 distance pushed) with its body visible: VC7.1 then homes the empty tag temporary in the live `val`
// argument slot as retail does (0x00808AD2). __unguarded_linear_insert (0x008074A0, landed) is declared only.
template<class _ForwardIter1,class _ForwardIter2> inline void iter_swap(_ForwardIter1 a,_ForwardIter2 b) { swap(*a,*b); }
template<class _RandomAccessIter,class _BidirectionalIter,class _Distance>
#ifdef FABLE_RETAIL_STLP_SORT_COPY_BACKWARD_AUX_INLINE
// NF85 opt-in (row 0x005E0FE0, the /O2 CCharString sort TU's out-of-line copy_backward; with FABLE_RETAIL_STLP_SORT_COPY_BACKWARD_NOINLINE):
// the same STLport __copy_backward, plain inline, so that TU's copy_backward COMDAT carries the copy loop itself (retail 0x005E0FE0
// calls only CCharString::operator=); the #else arm is the repo's out-of-line __fastcall form (0x00806310 / 0x0077E170).
inline _BidirectionalIter __copy_backward(_RandomAccessIter first,_RandomAccessIter last,
#else
FABLE_STLP_NOINLINE _BidirectionalIter FABLE_STLP_FASTCALL __copy_backward(_RandomAccessIter first,_RandomAccessIter last,
#endif
                                                                           _BidirectionalIter result,const random_access_iterator_tag&,_Distance*) {
    for(_Distance n=last-first;n>0;--n) *--result=*--last;
    return result;
}
#if defined(FABLE_RETAIL_STLP_SORT_COPY_BACKWARD_NOINLINE) && defined(_MSC_VER)
// NF82 opt-in (row 0x005E1A40, the /O2 CCharString sort TU): the same STLport copy_backward, kept out of line as the __fastcall COMDAT that
// TU's __linear_insert calls (0x005E0FE0: first / last in ecx / edx, result pushed).
template<class _BI1,class _BI2> __declspec(noinline) _BI2 __fastcall copy_backward(_BI1 first,_BI1 last,_BI2 result) {
    return __copy_backward(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#else
template<class _BI1,class _BI2> inline _BI2 copy_backward(_BI1 first,_BI1 last,_BI2 result) {
    return __copy_backward(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
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
#ifdef FABLE_RETAIL_STLP_UNGUARDED_LINEAR_INSERT_BODY
// NF82 opt-in (row 0x004DD80B, the registered-TC table sort of pair<CCharString, CTCBase::CRegisteredTCInfo>): the STLport 4.6 _algo.c
// __unguarded_linear_insert body, for the row that owns the out-of-line instantiation (__fastcall: last in ecx, the by-value element
// and the comparator pushed).
void FABLE_STLP_FASTCALL __unguarded_linear_insert(_RandomAccessIter last,_Tp val,_Compare comp) {
    _RandomAccessIter next=last;
    --next;
    while(comp(val,*next)) {
        *last=*next;
        last=next;
        --next;
    }
    *last=val;
}
#else
void FABLE_STLP_FASTCALL __unguarded_linear_insert(_RandomAccessIter last,_Tp val,_Compare comp);
#endif
template<class _RandomAccessIter,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __linear_insert(_RandomAccessIter first,_RandomAccessIter last,_Tp val,_Compare comp) {
    if(comp(val,*first)) {
        copy_backward(first,last,last+1);
        *first=val;
    } else
        __unguarded_linear_insert(last,val,comp);
}
#endif
#ifdef FABLE_RETAIL_STLP_INSERTION_SORT_BODY
// NF82 opt-in (row 0x004E3686, the registered-TC table sort of pair<CCharString, CTCBase::CRegisteredTCInfo>): STLport 4.6 _algo.c
// __insertion_sort(first, last, comp), out of line in retail and __fastcall (the /Gr algorithm TU: first/last in ecx/edx, the empty
// comparator pushed, `ret 4`); each element goes through the out-of-line __linear_insert (0x004E1C8A there), declared here unless
// FABLE_RETAIL_STLP_SORT_INTERNALS carries its body.
#ifndef FABLE_RETAIL_STLP_SORT_INTERNALS
template<class _RandomAccessIter,class _Tp,class _Compare>
void __fastcall __linear_insert(_RandomAccessIter first,_RandomAccessIter last,_Tp val,_Compare comp);
#endif
template<class _RandomAccessIter,class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp) {
    if(__first==__last) return;
    for(_RandomAccessIter __i=__first+1;__i!=__last;++__i)
        __linear_insert(__first,__i,typename iterator_traits<_RandomAccessIter>::value_type(*__i),__comp);
}
#endif
#ifdef FABLE_RETAIL_STLP_UNGUARDED_INSERTION_SORT_AUX_BODY
// NF82 opt-in (row 0x004DFD60, the same sort): STLport 4.6 _algo.c __unguarded_insertion_sort_aux(first, last, _Tp*, comp), out of
// line in retail and __fastcall (first/last in ecx/edx, the (T*)0 and the comparator pushed, `ret 8`); each element goes through the
// out-of-line __unguarded_linear_insert (0x004DD80B there), declared here unless FABLE_RETAIL_STLP_SORT_INTERNALS declares it.
#ifndef FABLE_RETAIL_STLP_SORT_INTERNALS
template<class _RandomAccessIter,class _Tp,class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter last,_Tp val,_Compare comp);
#endif
template<class _RandomAccessIter,class _Tp,class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first,_RandomAccessIter __last,_Tp*,_Compare __comp) {
    for(_RandomAccessIter __i=__first;__i!=__last;++__i)
        __unguarded_linear_insert(__i,_Tp(*__i),__comp);
}
#endif
#ifdef FABLE_RETAIL_STLP_PARTIAL_SORT_BODY
// NF82 opt-in (row 0x004E6E9E, the registered-TC table sort of pair<CCharString, CTCBase::CRegisteredTCInfo>): STLport 4.6 _algo.c
// __partial_sort(first, middle, last, _Tp*, comp), out of line in retail and __fastcall (first/middle in ecx/edx, last, the (T*)0 and the
// comparator pushed, `ret 0Ch`); make_heap / sort_heap are the out-of-line __fastcall front ends of that /Os TU (0x004E36C3 /
// 0x004E41DD there) and __pop_heap is out of line (0x004E1D20), declared here unless FABLE_RETAIL_STLP_HEAP / _POP_HEAP_AUX_PASS22 carry it.
#ifdef FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP
// (nested NF82 opt-in, row 0x005E2EB0, an /O2 TU) make_heap as the STLport inline front end over the out-of-line __make_heap (0x005E1BE0
// there); with FABLE_RETAIL_STLP_HEAP the __pop_heap body is visible and expands, as that TU does.
#ifndef FABLE_RETAIL_STLP_MAKE_HEAP_BODY
template<class _RandomAccessIterator,class _Compare,class _Tp,class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first,_RandomAccessIterator __last,_Compare __comp,_Tp*,_Distance*);
#endif
template<class _RandomAccessIterator,class _Compare>
inline void make_heap(_RandomAccessIterator __first,_RandomAccessIterator __last,_Compare __comp) {
    __make_heap(__first,__last,__comp,(typename iterator_traits<_RandomAccessIterator>::value_type*)0,(ptrdiff_t*)0);
}
#else
template<class _RandomAccessIterator,class _Compare>
void __fastcall make_heap(_RandomAccessIterator __first,_RandomAccessIterator __last,_Compare __comp);
#endif
template<class _RandomAccessIterator,class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first,_RandomAccessIterator __last,_Compare __comp);
#if !defined(FABLE_RETAIL_STLP_HEAP) && !defined(FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22)
template<class _RandomAccessIterator,class _Tp,class _Compare,class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator first,_RandomAccessIterator last,_RandomAccessIterator result,_Tp val,_Compare comp,_Distance*);
#endif
template<class _RandomAccessIter,class _Tp,class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first,_RandomAccessIter __middle,_RandomAccessIter __last,_Tp*,_Compare __comp) {
    make_heap(__first,__middle,__comp);
    for(_RandomAccessIter __i=__middle;__i<__last;++__i)
        if(__comp(*__i,*__first))
            __pop_heap(__first,__middle,__i,_Tp(*__i),__comp,(ptrdiff_t*)0);
    sort_heap(__first,__middle,__comp);
}
#endif
#ifdef FABLE_RETAIL_STLP_INTROSORT_LOOP
// Per-TU opt-in (chunk 1017, row 0x006A3E00, the CVectorMap<long, CIntelligentPointer<CThing> > sort): STLport 4.6 _algo.h / _algo.c
// __median, partial_sort's front end and __introsort_loop, the loop body compiled out of line as __fastcall in the /Gr TU (first in
// ecx, last in edx, `ret 0Ch`). Retail keeps __unguarded_partition (0x006A3440) and __partial_sort (0x006A3BF0) out of line: both
// are declarations here (FABLE_RETAIL_STLP_SORT_INTERNALS carries the __unguarded_partition body for TUs that emit it).
#define __stl_threshold 16
template<class _Tp,class _Compare>
inline const _Tp& __median(const _Tp& __a,const _Tp& __b,const _Tp& __c,_Compare __comp) {
    if(__comp(__a,__b))
        if(__comp(__b,__c))
            return __b;
        else if(__comp(__a,__c))
            return __c;
        else
            return __a;
    else if(__comp(__a,__c))
        return __a;
    else if(__comp(__b,__c))
        return __c;
    else
        return __b;
}
template<class _RandomAccessIter,class _Tp,class _Compare>
_RandomAccessIter FABLE_STLP_FASTCALL __unguarded_partition(_RandomAccessIter first,_RandomAccessIter last,_Tp pivot,_Compare comp);
template<class _RandomAccessIter,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __partial_sort(_RandomAccessIter __first,_RandomAccessIter __middle,_RandomAccessIter __last,_Tp*,_Compare __comp);
template<class _RandomAccessIter,class _Compare>
inline void partial_sort(_RandomAccessIter __first,_RandomAccessIter __middle,_RandomAccessIter __last,_Compare __comp) {
    __partial_sort(__first,__middle,__last,(typename iterator_traits<_RandomAccessIter>::value_type*)0,__comp);
}
template<class _RandomAccessIter,class _Tp,class _Size,class _Compare>
void FABLE_STLP_FASTCALL __introsort_loop(_RandomAccessIter __first,_RandomAccessIter __last,_Tp*,_Size __depth_limit,_Compare __comp) {
    while(__last-__first>__stl_threshold) {
        if(__depth_limit==0) {
            partial_sort(__first,__last,__last,__comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut=
            __unguarded_partition(__first,__last,
                                  _Tp(__median(*__first,*(__first+(__last-__first)/2),*(__last-1),__comp)),
                                  __comp);
        __introsort_loop(__cut,__last,(_Tp*)0,__depth_limit,__comp);
        __last=__cut;
    }
}
#endif
#ifdef FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22
// Original STLport 4.6 _heap.c comparator overload, with its out-of-line callee.
// Native callers 005F9B1E/005F9B5A and original PDB template types establish the specializations.
template<class _RandomAccessIterator,class _Tp,class _Compare,class _Distance>
void FABLE_STLP_FASTCALL __pop_heap(_RandomAccessIterator first,_RandomAccessIterator last,
                                  _RandomAccessIterator result,_Tp val,_Compare comp,_Distance*);
template<class _RandomAccessIterator,class _Tp,class _Compare>
inline void FABLE_STLP_FASTCALL __pop_heap_aux(_RandomAccessIterator __first,
                 _RandomAccessIterator __last,_Tp*,_Compare __comp) {
    __pop_heap(__first,__last-1,__last-1,_Tp(*(__last-1)),__comp,
               (typename iterator_traits<_RandomAccessIterator>::difference_type*)0);
}
#endif
#ifdef FABLE_RETAIL_STLP_HEAP
// Per-TU opt-in (chunk 892, row 0x00BEEDA0, the decal map's vector<pair<pair<long,long>,CEngineTextureDecal> > heap sort): STLport
// 4.6 _heap.c __pop_heap, out of line in retail and __fastcall (the /Gr algorithm TU: first/last in ecx/edx, `ret 28h`), and
// __adjust_heap declared only (out of line at 0x00BEEA00).
template<class _RandomAccessIterator,class _Distance,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __adjust_heap(_RandomAccessIterator first,_Distance holeIndex,_Distance len,_Tp val,_Compare comp);
template<class _RandomAccessIterator,class _Tp,class _Compare,class _Distance>
void FABLE_STLP_FASTCALL __pop_heap(_RandomAccessIterator first,_RandomAccessIterator last,_RandomAccessIterator result,
                                    _Tp val,_Compare comp,_Distance*) {
    *result=*first;
    __adjust_heap(first,_Distance(0),_Distance(last-first),val,comp);
}
#endif
#if defined(FABLE_RETAIL_STLP_HEAP) && defined(FABLE_RETAIL_STLP_MAKE_HEAP_BODY)
// NF82 opt-in (row 0x004E1D68, the registered-TC table sort of pair<CCharString, CTCBase::CRegisteredTCInfo>): STLport 4.6 _heap.c
// __make_heap(first, last, comp, _Tp*, _Distance*), out of line in retail and __fastcall (the /Gr algorithm TU: first/last in ecx/edx,
// `ret 0Ch`), over the out-of-line __adjust_heap declared above (0x004DFDBB there).
template<class _RandomAccessIterator,class _Compare,class _Tp,class _Distance>
void FABLE_STLP_FASTCALL __make_heap(_RandomAccessIterator __first,_RandomAccessIterator __last,_Compare __comp,_Tp*,_Distance*) {
    if(__last-__first<2) return;
    _Distance __len=__last-__first;
    _Distance __parent=(__len-2)/2;
    while(true) {
        __adjust_heap(__first,__parent,__len,_Tp(*(__first+__parent)),__comp);
        if(__parent==0) return;
        __parent--;
    }
}
#endif
#ifdef FABLE_RETAIL_STLP_REMOVE_COPY_IF
// Original STLport4.6 function adaptors and stable removal-copy loop.
template<class _Predicate> class unary_negate : public unary_function<typename _Predicate::argument_type,bool> {
protected:
 _Predicate _M_pred;
public:
 explicit unary_negate(const _Predicate& pred):_M_pred(pred){}
 bool operator()(const typename _Predicate::argument_type& x)const{return !_M_pred(x);}
};
#ifdef FABLE_RETAIL_STLP_NOT1
// Original STLport4.6 _function.h lines96-101, original FableWin template calls.
template<class _Predicate> inline unary_negate<_Predicate> FABLE_FASTCALL not1(const _Predicate& pred){return unary_negate<_Predicate>(pred);}
#endif
template<class _InputIter,class _OutputIter,class _Predicate>
_OutputIter FABLE_FASTCALL remove_copy_if(_InputIter first,_InputIter last,_OutputIter result,_Predicate pred) {
 for(;first!=last;++first)if(!pred(*first)){*result=*first;++result;}
 return result;
}
#endif

#if defined(FABLE_RETAIL_STLP_DECAL_HEAP) || defined(FABLE_RETAIL_STLP_ASTAR_PUSHHEAP_PASS23)
// Original STLport4.6 _heap.c comparator overloads, ordinary source bodies.
template <class _RandomAccessIterator, class _Distance, class _Tp, 
          class _Compare>
inline
void
#ifdef FABLE_RETAIL_STLP_ASTAR_REAL_PUSH_HEAP_PASS23
// Native independent UpdateElementA21C30/PushA22460 pass ECX first, EDX holeIndex toA206F0.
FABLE_STLP_FASTCALL
#endif
__push_heap(_RandomAccessIterator __first, _Distance __holeIndex,
            _Distance __topIndex, _Tp __val, _Compare __comp)
{
  _Distance __parent = (__holeIndex - 1) / 2;
  while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
    *(__first + __holeIndex) = *(__first + __parent);
    __holeIndex = __parent;
    __parent = (__holeIndex - 1) / 2;
  }
  *(__first + __holeIndex) = __val;
}

#ifdef FABLE_RETAIL_STLP_ASTAR_REAL_PUSH_HEAP_PASS23
// Original STLport _heap.c comparator range wrapper, inlined in native queue callers.
template<class _RandomAccessIterator,class _Compare>
inline void push_heap(_RandomAccessIterator first,_RandomAccessIterator last,_Compare compare) {
 typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;
 typedef typename iterator_traits<_RandomAccessIterator>::value_type _Tp;
 __push_heap(first,_Distance((last-first)-1),_Distance(0),_Tp(*(last-1)),compare);
}
#endif

#ifdef FABLE_RETAIL_STLP_DECAL_HEAP
template <class _RandomAccessIterator, class _Distance,
          class _Tp, class _Compare>
void
__adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
              _Distance __len, _Tp __val, _Compare __comp)
{
  _Distance __topIndex = __holeIndex;
  _Distance __secondChild = 2 * __holeIndex + 2;
  while (__secondChild < __len) {
    if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
      __secondChild--;
    *(__first + __holeIndex) = *(__first + __secondChild);
    __holeIndex = __secondChild;
    __secondChild = 2 * (__secondChild + 1);
  }
  if (__secondChild == __len) {
    *(__first + __holeIndex) = *(__first + (__secondChild - 1));
    __holeIndex = __secondChild - 1;
  }
  __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
#endif



#endif

#ifdef FABLE_RETAIL_STLP_SORT_HEAP_BODY
// NF85 opt-in (rows 0x0077F190 / 0x00A41960 / 0x009B8170, the /Gr pair<CCharString, ..> sort TUs): STLport 4.6 _heap.c
// `sort_heap(first, last, comp) { while (last - first > 1) pop_heap(first, last--, comp); }`, the out-of-line __fastcall COMDAT (first in
// ecx, last in edx, the empty comparator pushed, `ret 4`), with _heap.h pop_heap and _heap.c __pop_heap_aux / __pop_heap inline over the
// out-of-line __adjust_heap (declared here unless FABLE_RETAIL_STLP_HEAP / _DECAL_HEAP carry it). The text of the landed CCharString
// instance 0x005E27F0.
#if !defined(FABLE_RETAIL_STLP_HEAP) && !defined(FABLE_RETAIL_STLP_DECAL_HEAP)
template<class _RandomAccessIterator,class _Distance,class _Tp,class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator first,_Distance holeIndex,_Distance len,_Tp val,_Compare comp);
#endif
#if !defined(FABLE_RETAIL_STLP_HEAP)
template<class _RandomAccessIterator,class _Tp,class _Compare,class _Distance>
inline void __pop_heap(_RandomAccessIterator first,_RandomAccessIterator last,_RandomAccessIterator result,_Tp val,_Compare comp,_Distance*) {
    *result=*first;
    __adjust_heap(first,_Distance(0),_Distance(last-first),val,comp);
}
#endif
#if !defined(FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22)
template<class _RandomAccessIterator,class _Tp,class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator first,_RandomAccessIterator last,_Tp*,_Compare comp) {
    __pop_heap(first,last-1,last-1,_Tp(*(last-1)),comp,(ptrdiff_t*)0);
}
#endif
template<class _RandomAccessIterator,class _Compare>
inline void pop_heap(_RandomAccessIterator first,_RandomAccessIterator last,_Compare comp) {
    __pop_heap_aux(first,last,(typename iterator_traits<_RandomAccessIterator>::value_type*)0,comp);
}
template<class _RandomAccessIterator,class _Compare>
void __fastcall sort_heap(_RandomAccessIterator first,_RandomAccessIterator last,_Compare comp) {
    while(last-first>1)
        pop_heap(first,last--,comp);
}
#endif
#if defined(FABLE_RETAIL_STLP_POP_HEAP_AUX_FASTCALL_BODY) && !defined(FABLE_RETAIL_STLP_SORT_HEAP_BODY) && !defined(FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22)
// NF85 opt-in (rows 0x009B6D80 / 0x00A41260, the /Gr pair<CCharString, ..> sort TUs): STLport 4.6 _heap.c __pop_heap_aux(first, last,
// _Tp*, comp) as the out-of-line __fastcall COMDAT (first in ecx, last in edx, the (T*)0 and the comparator pushed, `ret 8`), with
// __pop_heap inline over the out-of-line __adjust_heap (declared here unless FABLE_RETAIL_STLP_HEAP / _DECAL_HEAP carry it).
#if !defined(FABLE_RETAIL_STLP_HEAP) && !defined(FABLE_RETAIL_STLP_DECAL_HEAP)
template<class _RandomAccessIterator,class _Distance,class _Tp,class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator first,_Distance holeIndex,_Distance len,_Tp val,_Compare comp);
#endif
#if !defined(FABLE_RETAIL_STLP_HEAP)
template<class _RandomAccessIterator,class _Tp,class _Compare,class _Distance>
inline void __pop_heap(_RandomAccessIterator first,_RandomAccessIterator last,_RandomAccessIterator result,_Tp val,_Compare comp,_Distance*) {
    *result=*first;
    __adjust_heap(first,_Distance(0),_Distance(last-first),val,comp);
}
#endif
template<class _RandomAccessIterator,class _Tp,class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator first,_RandomAccessIterator last,_Tp*,_Compare comp) {
    __pop_heap(first,last-1,last-1,_Tp(*(last-1)),comp,(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_STLP_MAKE_HEAP_FRONT_BODY
// NF85 opt-in (row 0x004E36C3, the /Os registered-TC table sort; not with FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP): STLport 4.6
// _heap.c `make_heap(first, last, comp) { __make_heap(first, last, comp, _STLP_VALUE_TYPE(..), _STLP_DISTANCE_TYPE(..)); }`, the
// out-of-line __fastcall front end (`push 0 ; push 0 ; push comp ; call __make_heap ; ret 4`) over the out-of-line __make_heap
// (0x004E1D68 there; declared here unless FABLE_RETAIL_STLP_HEAP + _MAKE_HEAP_BODY give its body).
#if !(defined(FABLE_RETAIL_STLP_HEAP) && defined(FABLE_RETAIL_STLP_MAKE_HEAP_BODY))
template<class _RandomAccessIterator,class _Compare,class _Tp,class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first,_RandomAccessIterator __last,_Compare __comp,_Tp*,_Distance*);
#endif
template<class _RandomAccessIterator,class _Compare>
void __fastcall make_heap(_RandomAccessIterator __first,_RandomAccessIterator __last,_Compare __comp) {
    __make_heap(__first,__last,__comp,(typename iterator_traits<_RandomAccessIterator>::value_type*)0,(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_STLP_FINAL_INSERTION_SORT_BODY
// NF85 opt-in (row 0x004E415D, the /Os registered-TC table sort): STLport 4.6 _algo.c __final_insertion_sort(first, last, comp) as the
// out-of-line __fastcall COMDAT (first in ecx, last in edx, the comparator pushed, `ret 4`) over the out-of-line __insertion_sort
// (0x004E3686) and __unguarded_insertion_sort_aux (0x004DFD60), declared here unless FABLE_RETAIL_STLP_INSERTION_SORT_BODY /
// _UNGUARDED_INSERTION_SORT_AUX_BODY give their bodies; __unguarded_insertion_sort is the inline _algo.c forwarder.
#ifndef FABLE_RETAIL_STLP_INSERTION_SORT_BODY
template<class _RandomAccessIter,class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp);
#endif
#ifndef FABLE_RETAIL_STLP_UNGUARDED_INSERTION_SORT_AUX_BODY
template<class _RandomAccessIter,class _Tp,class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first,_RandomAccessIter __last,_Tp*,_Compare __comp);
#endif
template<class _RandomAccessIter,class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp) {
    __unguarded_insertion_sort_aux(__first,__last,(typename iterator_traits<_RandomAccessIter>::value_type*)0,__comp);
}
template<class _RandomAccessIter,class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first,_RandomAccessIter __last,_Compare __comp) {
    if(__last-__first>16) {
        __insertion_sort(__first,__first+16,__comp);
        __unguarded_insertion_sort(__first+16,__last,__comp);
    }
    else
        __insertion_sort(__first,__last,__comp);
}
#endif
#ifdef FABLE_RETAIL_GET_OPINIONS_PASS30
// The original float storage and fill constructors use the size-optimised native owners.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> FABLE_FORCEINLINE float* allocator<float>::allocate(size_t n,const void*)
{
    return n ? static_cast<float*>(malloc(n * sizeof(float))) : 0;
}
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
_Vector_base<float,allocator<float> >::_Vector_base(size_t n,const allocator<float>& a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(a,0)
{
    _M_start = _M_end_of_storage.allocate(n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + n;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
float* FABLE_FASTCALL fill_n(float* first,unsigned int n,const float& x)
{
    for (; n > 0; --n, ++first) *first = x;
    return first;
}
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
vector<float,allocator<float> >::vector(size_type n,const float& x,const allocator<float>& a)
    : _Vector_base<float,allocator<float> >(n,a)
{
    _M_finish = uninitialized_fill_n(_M_start,n,x);
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#endif
#ifdef FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY
// NF214 opt-in (2026-11-21, q6333 / q6342; rows 0x007E4B60 CTCCreditsUI::Activate, 0x006116E0 CTCInventoryMap::OpenInventory).
// Fidelity: scaffolding, not retail. With FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY those TUs compile
// vector<NUISystem::CComponent*>::_M_insert_overflow visibly (retail's caller knows the callee does not let `x` escape); with
// fill_n declared only, VC7.1 loses that knowledge and moves the caller's registers. This gives the overflow's callee
// fill_n<NUISystem::CComponent**, unsigned int, NUISystem::CComponent*> the STLport 4.6 _algobase.h body of the generic
// FABLE_RETAIL_STLP_FILL_N_FASTCALL fill_n above, visible and out of line (noinline, as retail calls 0x0048514C), a pick-any COMDAT.
// Retail kept the 23-byte copy (x re-read every pass, no frame) from a size-optimised TU, row 0x0048514C, which sets
// `#pragma optimize("s", on)` itself (no "y" change); the plain /O2 copy hoists x (30 bytes). `#pragma optimize("", on)` below returns
// to the command-line settings (VC7.1 has no push / pop for optimize), so a row that uses this opt-in must not carry a row-head optimize
// pragma before its include of this header (nf189). Needs FABLE_RETAIL_STLP_FILL_N_FASTCALL; place before any use of this fill_n.
}
namespace NUISystem { class CComponent; }
#pragma optimize("s", on)
namespace std {
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
NUISystem::CComponent** FABLE_FASTCALL fill_n(NUISystem::CComponent** first,unsigned int n,NUISystem::CComponent* const& x) { for(;n>0;--n,++first) *first=x;return first; }   // 0x0048514C
#pragma optimize("", on)
#endif
#if defined(FABLE_RETAIL_STLP_BOOL_VECTOR_SIZE_OWNERS_PASS26) && defined(_MSC_VER)
// Fidelity: scaffolding, not retail. The bool storage constructor and resize owners are
// size-built; erase at 004676BB is size-built with a frame pointer. A caller's row-head
// optimization follows this include. optimize("", on) resets the enclosing TU's mode.
#pragma optimize("s", on)
template<>
FABLE_FORCEINLINE bool* allocator<bool>::allocate(size_t n, const void*)
{
    return n ? static_cast<bool*>(malloc(n * sizeof(bool))) : 0;
}

template<>
inline _Vector_base<bool, allocator<bool> >::_Vector_base(size_t n, const allocator<bool>& a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
{
    _M_start = _M_end_of_storage.allocate(n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + n;
}

#ifdef FABLE_RETAIL_VERTEX_BLOCK_BOOL_FILL_PASS30
// Inline-budget: the size-built 00451583 owner calls the 46-byte vector base constructor
// and expands the vendor fill chain. Forced bool dispatch keeps the caller's existing expansion.
template<> FABLE_FORCEINLINE bool* fill_n<bool*,size_t,bool>(bool* first,size_t n,const bool& x)
{
    for (; n>0; --n,++first)
        *first=x;
    return first;
}
template<> FABLE_FORCEINLINE bool* __uninitialized_fill_n<bool*,size_t,bool>(bool* first,size_t n,const bool& x,const __true_type&)
{
    return std::fill_n(first,n,x);
}
template<> FABLE_FORCEINLINE bool* uninitialized_fill_n<bool*,size_t,bool>(bool* first,size_t n,const bool& x)
{
    return __uninitialized_fill_n(first,n,x,__true_type());
}
#pragma optimize("y", off)
template<> inline vector<bool, allocator<bool> >::vector(size_type n,const bool& value,const allocator<bool>& a)
    : _Vector_base<bool, allocator<bool> >(n,a)
{
    this->_M_finish=uninitialized_fill_n(this->_M_start,n,value);
}
#pragma optimize("y", on)
#endif
template<>
FABLE_FORCEINLINE bool* __copy_ptrs<bool*, bool*>(bool* first, bool* last, bool* result, const __true_type&)
{
    return static_cast<bool*>(__copy_trivial(first, last, result));
}

#pragma optimize("y", off)
template<>
inline bool* vector<bool, allocator<bool> >::erase(iterator first, iterator last)
{
    iterator result = __copy_ptrs(last, this->_M_finish, first, _IsPODType());
    _Destroy(result, this->_M_finish);
    this->_M_finish = result;
    return first;
}
#pragma optimize("y", on)
template<>
inline void vector<bool, allocator<bool> >::resize(size_type n, const bool& value)
{
    if (n < size()) erase(begin() + n, end());
    else _M_fill_insert(end(), n - size(), value);
}
#pragma optimize("", on)
#endif // FABLE_RETAIL_STLP_BOOL_VECTOR_SIZE_OWNERS_PASS26
#ifdef FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY
// NF213 opt-in (2026-11-21, q6333 / q6342; row 0x005414A0 NUISystem::CDynamicList::InsertChild, ...). Fidelity: scaffolding, not retail.
// That TU calls vector<NUISystem::CComponent*>::_M_insert_overflow 0x00485DBC out of line with the STLport stl/_vector.c body visible:
// with the body visible VC7.1 homes push_back's __true_type tag on the value slot it passes by reference (0x00541526 / 0x0054152B both
// `[esp+20h]` / `[esp+24h]` = one word); declared only, the tag takes the dead index argument word (DIFFER, two operands). A visible
// body is a pick-any COMDAT, and retail kept the 137-byte copy from a size-optimised frame-pointer TU, row 0x00485DBC (its Leaf), which
// calls the out-of-line __fastcall __copy_trivial 0x00405BA0 and fill_n 0x0048514C. The advisor pick-any rule (EXTRA_RULES 2026-11-20)
// needs the owner's body, so this explicit specialization compiles the same STLport 4.6 _vector.c __true_type body as the
// out-of-line definition above under optimize("y", off) + ("s", on), as the owner's TU did; inline (a pick-any COMDAT like the owner's) and noinline, as retail calls it. fill_n for
// this element type is declared only here (the owner's callee); needs FABLE_RETAIL_STLP_FILL_N_FASTCALL and
// FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE. Owner check: the owner row 0x00485DBC sets `#pragma optimize("y", off)` and
// `#pragma optimize("s", on)` itself. `#pragma optimize("", on)` below returns to the command-line settings (VC7.1 has no push / pop
// for optimize), so a row that uses this opt-in must not carry a row-head optimize pragma before its include of this header (nf189).
// Needs no use of the member before this point; not with FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE.
}
namespace NUISystem { class CComponent; }
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
#if defined(FABLE_RETAIL_STLP_COMPONENT_OVERFLOW_ALLOCATE_INLINE_PASS26) && defined(_MSC_VER)
// Inline-budget: 00485DBC expands the null-checked malloc through both STLport allocator layers.
template<> FABLE_FORCEINLINE NUISystem::CComponent** allocator<NUISystem::CComponent*>::allocate(size_t n, const void*)
{
    return n ? static_cast<NUISystem::CComponent**>(malloc(n * sizeof(NUISystem::CComponent*))) : 0;
}
#ifdef FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
template<> FABLE_FORCEINLINE NUISystem::CComponent** _STLP_alloc_proxy<NUISystem::CComponent**, NUISystem::CComponent*, allocator<NUISystem::CComponent*> >::allocate(size_t n)
{
    return __stl_alloc_rebind(static_cast<allocator<NUISystem::CComponent*>&>(*this), static_cast<NUISystem::CComponent**>(0)).allocate(n, 0);
}
#endif
#endif // FABLE_RETAIL_STLP_COMPONENT_OVERFLOW_ALLOCATE_INLINE_PASS26
template<> NUISystem::CComponent** FABLE_FASTCALL fill_n(NUISystem::CComponent** first,unsigned int n,NUISystem::CComponent* const& x);   // 0x0048514C
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
void vector<NUISystem::CComponent*,allocator<NUISystem::CComponent*> >::_M_insert_overflow(NUISystem::CComponent** position,NUISystem::CComponent* const& x,const __true_type&,size_t fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    NUISystem::CComponent** newStart=this->_M_end_of_storage.allocate(len);
    NUISystem::CComponent** newFinish=(NUISystem::CComponent**)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(NUISystem::CComponent**)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
#pragma optimize("", on)
#ifdef FABLE_RETAIL_STLP_COMPONENT_FILL_N_VISIBLE_PASS26
#pragma optimize("s",on)
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
NUISystem::CComponent** FABLE_FASTCALL fill_n(NUISystem::CComponent** first,unsigned int n,NUISystem::CComponent* const& x) {
    for(;n>0;--n,++first) *first=x;
    return first;
}
#pragma optimize("",on)
#endif
#endif
#ifdef FABLE_RETAIL_STLP_INVENTORY_LONG_OWNERS_PASS27
// Stock speed-built vector<long> owners. Native 00440590 selects the POD copy;
// native 00440990 performs its two copies around fill_n. Both operate on longs.
template<> inline vector<long>::iterator vector<long>::erase(iterator first,iterator last) {
    iterator result = static_cast<iterator>(__copy_trivial(last,this->_M_finish,first));
    this->_M_finish=result;
    return first;
}
template<> inline void vector<long>::_M_insert_overflow(iterator position,const long& x,const __true_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();
    const size_type len=oldSize+(max)(oldSize,fillLength);
    long* newStart=this->_M_end_of_storage.allocate(len);
    long* newFinish=(long*)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(long*)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();
    _M_set(newStart,newFinish,newStart+len);
}
#endif
#ifdef FABLE_RETAIL_STRIPIFIER_VERTEX_MAPS_PASS30
// Inline-budget: the CArray<unsigned short> constructor folds to the 90-byte vector constructor at 00a87090.
template<> FABLE_FORCEINLINE vector<unsigned short>::vector(const vector<unsigned short>& x)
    : _Vector_base<unsigned short,allocator<unsigned short> >(x.size(),x.get_allocator()) {
    this->_M_finish = __uninitialized_copy((const unsigned short*)x._M_start,
        (const unsigned short*)x._M_finish, this->_M_start, __true_type());
}
#endif
#ifdef FABLE_RETAIL_STLP_LONG_OVERFLOW_PASS30
// Inline-budget: retail calls the long overflow and expands the short one.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<> inline void vector<long>::_M_insert_overflow(iterator position,const long& x,const __true_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();
    const size_type len=oldSize+(max)(oldSize,fillLength);
    long* newStart=this->_M_end_of_storage.allocate(len);
    long* newFinish=(long*)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(long*)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();
    _M_set(newStart,newFinish,newStart+len);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
#endif
#ifdef FABLE_RETAIL_STRIPIFIER_VERTEX_MAPS_PASS30
// Inline-budget: CArray<unsigned short>'s generated assignment expands vector assignment at the folded owner.
template<> FABLE_FORCEINLINE vector<unsigned short>& vector<unsigned short>::operator=(const vector<unsigned short>& x) {
    typedef unsigned short _Tp;
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
template<> vector<long>& vector<long>::operator=(const vector<long>&);
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
// The 00485ff3 owner uses a frame pointer and optimizes for size. Preserve the
// STLport 4.6 body both for callers and for the standalone copy exported here.
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
template<> inline void vector<long, allocator<long> >::push_back(const long& x) {
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, x);
        ++this->_M_finish;
    } else {
        _M_insert_overflow(this->_M_finish, x, _IsPODType(), 1, true);
    }
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
#endif
#ifdef FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
// NF213 opt-in (2026-11-21, q6333 / q6342; rows 0x00541390, ...). Fidelity: scaffolding, not retail. A row that expands
// vector<NUISystem::CComponent*>::push_back emits the inline as a pick-any COMDAT. Retail kept the 49-byte out-of-line copy from a
// size-optimised frame-pointer TU, row 0x00486024 (Leaf ?push_back@?$vector@PAVCComponent@NUISystem@@...); the plain copy is 50 bytes
// (no frame, `[esp+..]` operands). The advisor pick-any rule (EXTRA_RULES 2026-11-20) needs the owner's body, so this explicit
// specialization compiles the same STLport 4.6 _vector.h body as the default push_back above under optimize("y", off) + ("s", on), as
// the owner's TU did; callers still expand it. Owner check: the owner row 0x00486024 sets `#pragma optimize("y", off)` and
// `#pragma optimize("s", on)` itself. `#pragma optimize("", on)` below returns to the command-line settings (VC7.1 has no push / pop
// for optimize), so a row that uses this opt-in must not carry a row-head optimize pragma before its include of this header (nf189).
// Needs no use of vector<NUISystem::CComponent*>::push_back before this point. Not with FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE /
// _NOINLINE / _FINISH_LOCAL_PASS24.
}
namespace NUISystem { class CComponent; }
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template<> inline void vector<NUISystem::CComponent*,allocator<NUISystem::CComponent*> >::push_back(NUISystem::CComponent* const& x) {
    if(this->_M_finish!=this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish,x); ++this->_M_finish;
    } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
}
#pragma optimize("", on)
#endif

#ifdef FABLE_RETAIL_OPINION_REACTION_MASK_PASS29
// Native00451583 has a frame and calls00450ccc _Vector_base before its byte fill loop.
// Inline-budget: plain uninitialized_fill_n emitted a call in the 49-byte native constructor.
// These two STLport bool fill bodies expand at the constructor; the constructor is ordinary inline.
template<> FABLE_FORCEINLINE bool* fill_n<bool*,unsigned int,bool>(bool* first,unsigned int n,const bool& x) { for(;n>0;--n,++first)*first=x;return first; }
template<> FABLE_FORCEINLINE bool* uninitialized_fill_n<bool*,unsigned int,bool>(bool* first,unsigned int n,const bool& x) {
 typedef __type_traits<bool>::is_POD_type IsPOD;
 return __uninitialized_fill_n(first,n,x,IsPOD());
}
}

#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
namespace std {
template<> inline vector<bool,allocator<bool> >::vector(unsigned int n,const bool& value,const allocator<bool>& a)
    : _Vector_base<bool,allocator<bool> >(n,a) {
    this->_M_finish=uninitialized_fill_n(this->_M_start,n,value);
}
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
namespace std {
#endif
#ifdef FABLE_RETAIL_STLP_BOOL_FILL_OWNER_PASS26
// Native00451583 has a frame and calls00450ccc _Vector_base before its byte fill loop.
// Same original STLport bodies; specialize only the bool fill helper and constructor.
template<> FABLE_FORCEINLINE bool* fill_n<bool*,unsigned int,bool>(bool* first,unsigned int n,const bool& x) { for(;n>0;--n,++first)*first=x;return first; }
template<> FABLE_FORCEINLINE bool* uninitialized_fill_n<bool*,unsigned int,bool>(bool* first,unsigned int n,const bool& x) {
 typedef __type_traits<bool>::is_POD_type IsPOD;
 return __uninitialized_fill_n(first,n,x,IsPOD());
}
}
#pragma optimize("s",on)
#pragma optimize("y",off)
namespace std {
template<> __declspec(noinline) inline vector<bool,allocator<bool> >::vector(unsigned int n,const bool& value,const allocator<bool>& a):_Vector_base<bool,allocator<bool> >(n,a) {
 this->_M_finish=uninitialized_fill_n(this->_M_start,n,value);
}
#pragma optimize("",on)
#endif

#ifdef FABLE_RETAIL_STLP_DESTROY_AUX_PAIR_LONG_CCHARSTRING_SIZE_BODY
// NF254 opt-in (2026-11-22, CODEXP27 L4 / L5; row 0x0074FCE0 vector<pair<long, CCharString> >::_M_insert_overflow, a /Gr TU).
// Fidelity: scaffolding, not retail. That TU's _M_clear expands the element destroy loop in place (0x0074FD98..0x0074FDAD), but c1
// still instantiates the range destroy __destroy_aux<pair<long, CCharString>*> (fastcall under /Gr: ??$__destroy_aux@...@std@@YIX...),
// a pick-any COMDAT the linker can keep. Retail kept the 29-byte copy (each element's ??_G called with flag 0) from a size-optimised
// TU, row 0x004DD782 (its Leaf), which sets `#pragma optimize("s", on)` itself; the plain /O2 copy expands ~pair (36 bytes). A pick-any
// template copy takes the optimize mode in force at the END of the TU (NF216), so this explicit specialization compiles the same STLport
// 4.6 _construct.h body in place under optimize("s", on), as the owner's TU did. `#pragma optimize("", on)` below returns to the
// command-line settings (VC7.1 has no push / pop for optimize), so a row that uses this opt-in must not carry a row-head optimize pragma
// before its include of this header (nf189). Needs CCharString complete (fable_string.h first) and a /Gr TU (the primary template's
// default convention then is __fastcall, as the owner's symbol is).
#pragma optimize("s", on)
template<> inline void __destroy_aux<pair<long,CCharString>*>(pair<long,CCharString>* first,pair<long,CCharString>* last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
#pragma optimize("", on)
#endif
}
#ifdef FABLE_RETAIL_HEAD_DEED_PASS29
// The native list<long> out-of-line functions use the size profile.
namespace std {
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
template<> inline _List_node<long>* list<long>::_M_create_node(const long& value) {
    _List_node<long>* node = this->_M_node.allocate(1);
    new ((void*)&node->_M_data) long(value);
    return node;
}
template<> inline list<long>::iterator list<long>::erase(iterator position) {
        _List_node_base* next_node=position._M_node->_M_next;
        _List_node_base* prev_node=position._M_node->_M_prev;
        _List_node<long>* n=(_List_node<long>*)position._M_node;
        prev_node->_M_next=next_node;
        next_node->_M_prev=prev_node;
        _Destroy(&n->_M_data);
        this->_M_node.deallocate(n,1);
        return iterator(next_node);
    }
template<> inline list<long>::iterator list<long>::insert(iterator position, const long& x) {

        _List_node<long>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
template<> inline void list<long>::push_front(const long& x) { insert(begin(),x); }
template<> inline void list<long>::pop_front() { erase(begin()); }
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
}
#endif

#ifdef FABLE_RETAIL_WATER_BYTE_RESIZE_SPELLINGS_PASS29
namespace std {
// Fidelity: scaffolding, not retail. The 00485B18 erase owner sets optimize("s", on).
// This block ends with optimize("", on). Set TU optimization pragmas after this header.
#if defined(_MSC_VER) && defined(FABLE_RETAIL_WATER_BYTE_ERASE_OWNER_PASS29)
#pragma optimize("s", on)
#endif
template<> inline vector<unsigned char>::iterator vector<unsigned char>::erase(iterator first,iterator last) {
    iterator result=std::copy(last,this->_M_finish,first);
    _Destroy(result,this->_M_finish); this->_M_finish=result; return first;
}
#if defined(_MSC_VER) && defined(FABLE_RETAIL_WATER_BYTE_ERASE_OWNER_PASS29)
#pragma optimize("", on)
#endif

// Fidelity: scaffolding, not retail. Owner 00486189 selects size optimization.
// This block ends with optimize("", on). Set any TU optimization pragma after including this header.
#if defined(_MSC_VER) && defined(FABLE_RETAIL_VECTOR_BYTE_RESIZE_VALUE_OWNER_PASS29)
#pragma optimize("s", on)
#endif
template<> inline void vector<unsigned char>::resize(size_type n,const unsigned char& x) {
    if (n < size()) erase(begin()+n,end());
    else insert(end(), n-size(), x);
}
#if defined(_MSC_VER) && defined(FABLE_RETAIL_VECTOR_BYTE_RESIZE_VALUE_OWNER_PASS29)
#pragma optimize("", on)
#endif
// Fidelity: scaffolding, not retail. Owner 004861df selects size optimization.
// This block ends with optimize("", on). Set any TU optimization pragma after including this header.
#if defined(_MSC_VER) && defined(FABLE_RETAIL_VECTOR_BYTE_RESIZE_ONE_OWNER_PASS29)
#pragma optimize("s", on)
#endif
// Inline-budget: the water Load call expands both resize overloads at 00BFDB8E..00BFDBCE.
template<> FABLE_FORCEINLINE void vector<unsigned char>::resize(size_type n) { resize(n,unsigned char()); }
#if defined(_MSC_VER) && defined(FABLE_RETAIL_VECTOR_BYTE_RESIZE_ONE_OWNER_PASS29)
#pragma optimize("", on)
#endif
}
#endif
#ifdef FABLE_RETAIL_CUTSCENE_STRING_VECTOR_DESTRUCTION_PASS30
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
namespace std {
template<> inline vector<CCharString>::~vector()
{
    _Destroy(this->_M_start, this->_M_finish);
}
}
#if defined(_MSC_VER)
#pragma optimize("s", on)
#endif
#endif
#ifdef FABLE_RETAIL_CUTSCENE_FLOAT_OVERFLOW_PASS30
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
namespace std {
template<> inline void vector<float>::_M_insert_overflow(iterator position,const float& x,const __true_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    float* newStart=this->_M_end_of_storage.allocate(len);
    float* newFinish=(float*)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(float*)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
}
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#endif
#ifdef FABLE_RETAIL_CUTSCENE_FLOAT_VECTOR_APPEND_PASS30
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
namespace std {
template<> inline void vector<float>::push_back(const float& x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, x);
        ++this->_M_finish;
    } else {
        _M_insert_overflow(this->_M_finish, x, _IsPODType(), 1, true);
    }
}
}
#if defined(_MSC_VER)
#pragma optimize("s", on)
#endif
#endif

#ifdef FABLE_RETAIL_CUTSCENE_STRING_OVERFLOW_PASS30
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
namespace std {
template<> inline void vector<CCharString>::_M_insert_overflow(iterator position,const CCharString& x,const __false_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    CCharString* newStart=this->_M_end_of_storage.allocate(len);CCharString* newFinish=newStart;
    newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
    if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
    else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
    if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
}
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#endif

#ifdef FABLE_RETAIL_CUTSCENE_STRING_APPEND_PASS30
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
namespace std {
template<> inline void vector<CCharString>::push_back(const CCharString& x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, x);
        ++this->_M_finish;
    } else {
        _M_insert_overflow(this->_M_finish, x, _IsPODType(), 1, true);
    }
}
}
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#endif

#endif
