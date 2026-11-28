// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
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
// STLport4.6.2 excerpts adapted only for project names and native fastcall.
#endif
#ifndef FABLE_STLPORT_ALGORITHM_H
#define FABLE_STLPORT_ALGORITHM_H
// STLport 4.x (SGI) for_each + mem_fun_ref, as the landed NPlayerGui::CDrawMiniMap::ClearPrimitives 0x00644660 and
// CDrawQuickAccessIcons rows model them (chunk 867 lifts that TU-local model into a shared header): mem_fun_ref_t is the 4-byte
// functor holding the pointer-to-member; for_each is the /Gr `__fastcall` COMDAT at 0x0043F360 (hidden return pointer in ecx, the
// functor result returned by value: `lea ecx,<dead slot> ; mov edx,begin ; push pfn ; push end ; call`). Declaration only for
// for_each: the retail instantiation is a separate COMDAT, callers only relocate to it.
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_STLP_MEM_FUN_REF_ORIGINAL_PASS22
#include "stlport/fable_containers.h"
#endif
namespace std {
#ifdef FABLE_RETAIL_STLP_MEM_FUN_REF_ORIGINAL_PASS22
// Retail opt-in (Codex pass22 cursor): authentic STLport4.6.2 _function_adaptors.h433..441,746..747.
// Original empty unary_function base, member typedef and member/formal names.
template <class _Ret, class _Tp>
class mem_fun_ref_t : public unary_function<_Tp,_Ret> {
  typedef _Ret (_Tp::*__fun_type)(void);
public:
  explicit mem_fun_ref_t(__fun_type __pf) : _M_f(__pf) {}
  _Ret operator()(_Tp& __r) const { return (__r.*_M_f)(); }
private:
  __fun_type _M_f;
};
template <class _Result, class _Tp> inline mem_fun_ref_t<_Result, _Tp>
mem_fun_ref(_Result (_Tp::*__f)()) { return mem_fun_ref_t<_Result,_Tp>(__f); }
#else
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
#endif
#ifdef FABLE_RETAIL_STLP_CONST_MEM_FUN_REF
// NF90 opt-in (round 7, q11183 row 0x005BD96B NInventory::CCategory::GetFirstEmptySlot): STLport 4.6.2 _function_adaptors.h const_mem_fun_ref_t
// and its mem_fun_ref overload for a const member (FableWin.pdb ??$mem_fun_ref@_NVCItem@NInventory@@@std@@...const_mem_fun_ref_t and
// ??0?$const_mem_fun_ref_t@_NVCItem@NInventory@@@std@@QAE@P8CItem@NInventory@@BE_NXZ@Z: the find_if predicate of that function; retail pushes
// the 4-byte functor, the address of CItem::IsSlotEmpty, by value: `push 5BC46Ch`, 0x005BD974). No layout change elsewhere.
}
#include "stlport/fable_containers.h"
namespace std {
template <class _Ret, class _Tp>
class const_mem_fun_ref_t : public unary_function<_Tp,_Ret> {
  typedef _Ret (_Tp::*__fun_type)(void) const;
public:
  explicit const_mem_fun_ref_t(__fun_type __pf) : _M_f(__pf) {}
  _Ret operator()(const _Tp& __r) const { return (__r.*_M_f)(); }
private:
  __fun_type _M_f;
};
template <class _Result, class _Tp> inline const_mem_fun_ref_t<_Result, _Tp>
mem_fun_ref(_Result (_Tp::*__f)() const) { return const_mem_fun_ref_t<_Result,_Tp>(__f); }
#endif
#ifdef FABLE_RETAIL_COMBATRING_ASSIGN_PASS27
// STLport 4.6 _algo.h selects the bidirectional partition for a random-access range.
template<class It,class Predicate>
It FABLE_FASTCALL __partition(It,It,Predicate,const bidirectional_iterator_tag&);
template<class It,class Predicate>
inline It FABLE_FASTCALL partition(It first,It last,Predicate pred) {
    return __partition(first,last,pred,typename iterator_traits<It>::iterator_category());
}
#endif
#ifdef FABLE_RETAIL_STLP_FOR_EACH_BODY
// Retail opt-in (chunk 1018, row 0x0067E290 CDegradableDef::Transfer): STLport 4.6 _algo.h for_each with its body visible, which that
// TU expands in place (0x0067E37F..0x0067E398: the CSetEngineGraphicType functor's byte store in a `cmp eax,edi ; jne` loop).
template <class It, class F> inline F FABLE_FASTCALL for_each(It first, It last, F f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
#else
template <class It, class F> F FABLE_FASTCALL for_each(It first, It last, F f);   // 0x0043F360 (SGI, /Gr)
#endif
#ifdef FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
// Retail opt-in (Codex pass20): STLport4.6 _algobase.h category dispatch;
// native004b8e40 searches CCharString pointers, first/last in ecx/edx.
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
// Original STLport4.6.2 _algobase.c:98-139; tag never escapes the complete body.
template<class It,class T> inline It FABLE_FASTCALL __find(It first,It last,const T& value,const random_access_iterator_tag&){
 typename iterator_traits<It>::difference_type trip_count=(last-first)>>2;
 for(;trip_count>0;--trip_count){
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
 }
 switch(last-first){
 case 3:if(*first==value)return first;++first;
 case 2:if(*first==value)return first;++first;
 case 1:if(*first==value)return first;++first;
 case 0:default:return last;
 }
}
template<class It,class T> inline It FABLE_FASTCALL find(It first,It last,const T& value) {
#elif defined(FABLE_RETAIL_STLP_FIND_NOINLINE_BODY)
// Retail opt-in (wf15 row 0x007BF510 CTCShotDefinitionBase::OnCollision): the same STLport 4.6 _algobase.c body, called out of
// line (__find<CIntelligentPointer<const CThing>*,CIntelligentPointer<CThing> > 0x007C2980) but visible to the caller's TU, so
// VC7.1 knows it never touches the tag and homes the empty tag on a live argument slot (lea eax,[esp+28h] at 0x007BF555; a
// bare declaration homes it on the dead thing slot instead).
template<class It,class T> __declspec(noinline) It FABLE_FASTCALL __find(It first,It last,const T& value,const random_access_iterator_tag&){
 typename iterator_traits<It>::difference_type trip_count=(last-first)>>2;
 for(;trip_count>0;--trip_count){
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
 }
 switch(last-first){
 case 3:if(*first==value)return first;++first;
 case 2:if(*first==value)return first;++first;
 case 1:if(*first==value)return first;++first;
 case 0:default:return last;
 }
}
template<class It,class T> inline It find(It first,It last,const T& value) {
#else
template<class It,class T> It FABLE_FASTCALL __find(It first,It last,const T& value,const random_access_iterator_tag&);
template<class It,class T> inline It find(It first,It last,const T& value) {
#endif
    return __find(first,last,value,random_access_iterator_tag());
}
#endif
#ifdef FABLE_RETAIL_STLP_FIND_NOINLINE_BODY_FORWARDER
// Retail opt-in (wf15, row 0x005F6BD6 CTCInventoryAbilities::SetAbilityAsAvailable): STLport 4.6 _algobase.h find / _algobase.c
// __find with BOTH bodies visible in the TU but called out of line (not to be combined with FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH,
// whose FABLE_RETAIL_STLP_FIND_NOINLINE_BODY arm keeps the forwarder inline) (find<EHeroAbility*, EHeroAbility> 0x005F8150, a `__fastcall`
// forwarder, `ret 4`; __find 0x005F7ACC, `ret 8`, both landed). The caller then knows neither writes memory nor keeps &val, so
// it keeps the searched value in a register and reuses `last` after the call (0x005F6DD3..0x005F6DEC).
template<class It,class T> __declspec(noinline) It FABLE_FASTCALL __find(It first,It last,const T& value,const random_access_iterator_tag&){
 typename iterator_traits<It>::difference_type trip_count=(last-first)>>2;
 for(;trip_count>0;--trip_count){
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
 }
 switch(last-first){
 case 3:if(*first==value)return first;++first;
 case 2:if(*first==value)return first;++first;
 case 1:if(*first==value)return first;++first;
 case 0:default:return last;
 }
}
template<class It,class T> __declspec(noinline) It FABLE_FASTCALL find(It first,It last,const T& value) {
    return __find(first,last,value,random_access_iterator_tag());
}
#endif
#ifdef FABLE_RETAIL_STLP_FIND
// Retail opt-in (chunk 1008b, row 0x005F6BD6 CTCInventoryAbilities::SetAbilityAsAvailable): STLport 4.6 _algobase.h
// `find(_InputIter __first, _InputIter __last, const _Tp& __val)`, the forwarder to the category-dispatched __find (landed
// 0x005F7ACC), a `__fastcall` COMDAT in retail (find<EHeroAbility*, EHeroAbility> 0x005F8150: first in ecx, last in edx, &val on
// the stack, `ret 4`). Declaration only: callers only relocate to the instantiation.
template <class _InputIter, class _Tp> _InputIter FABLE_FASTCALL find(_InputIter __first, _InputIter __last, const _Tp& __val);
#endif
#ifdef FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_COMPARE
// STLport4.6.2 _algo.h:500..505, actual generic comparer overload. The original
// MapperPred has matching FableWin lower_bound calls; retail calls__lower_bound.
#ifdef FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_BODY
// Original STLport4.6.2 _iterator_base.h random-access distance/advance dispatch.
template<class I> inline typename iterator_traits<I>::difference_type __distance(const I& a,const I& b,const random_access_iterator_tag&){return b-a;}
#ifndef FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
template<class I> inline ptrdiff_t distance(const I& a,const I& b){return __distance(a,b,typename iterator_traits<I>::iterator_category());}
#endif
template<class I,class D> inline void __advance(I& i,D n,const random_access_iterator_tag&){i+=n;}
#ifndef FABLE_RETAIL_STLP_ADVANCE
template<class I,class D> inline void advance(I& i,D n){__advance(i,n,typename iterator_traits<I>::iterator_category());}
#endif
// Original STLport4.6.2 stl/_algobase.c:363..387, body visible as original TU.
template<class _ForwardIter,class _Tp,class _Compare,class _Distance>
_ForwardIter FABLE_FASTCALL __lower_bound(_ForwardIter first,_ForwardIter last,const _Tp& value,_Compare comp,_Distance*){
    _Distance len=distance(first,last);
    _Distance half;
    _ForwardIter middle;
    while(len>0){
        half=len>>1;
        middle=first;
        advance(middle,half);
        if(comp(*middle,value)){
            first=middle;
            ++first;
            len=len-half-1;
        }else len=half;
    }
    return first;
}
#else
template<class _ForwardIter,class _Tp,class _Compare,class _Distance>
_ForwardIter FABLE_FASTCALL __lower_bound(_ForwardIter,_ForwardIter,const _Tp&,_Compare,_Distance*);
#endif
template<class _ForwardIter,class _Tp,class _Compare>
inline _ForwardIter FABLE_FASTCALL lower_bound(_ForwardIter first,_ForwardIter last,const _Tp& value,_Compare comp){
    return __lower_bound(first,last,value,comp,(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_STLP_LOWER_BOUND
// Retail opt-in (chunk 1021, row 0x006B3E60 CEnvironment::SetWeightedThemes): STLport 4.6 _algo.h `lower_bound(first, last, val)`,
// a `__fastcall` COMDAT in retail (lower_bound<pair<long, float>*, pair<long, float> > 0x006B5A50, landed: first in ecx, last in
// edx, &val on the stack, `ret 4`). Declaration only: callers only relocate to the instantiation.
template <class _ForwardIter, class _Tp> _ForwardIter FABLE_FASTCALL lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);
#endif
#ifdef FABLE_RETAIL_STLP_FIND_IF_INPUT
// STLport 4.6.2 _algobase.c:183-197, input-iterator search and category dispatch.
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
// Modified only for this project's names and calling-convention spelling.
#ifdef FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
// NF88 nested opt-in (round 7, q11159; rows 0x005C08C0 / 0x005C0653 and the inventory callers): STLport 4.6 _algo.c random-access
// `__find_if(first, last, pred, const random_access_iterator_tag&)`, which find_if picks for pointer iterators (a better match than the
// input-iterator overload above). A `__fastcall` COMDAT in retail (0x005C0653 for const NInventory::CCategory* with CIsInventoryCategory,
// `ret 8`); declaration only unless FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS_BODY, the four-per-trip loop and the 3/2/1 remainder switch.
template<class RandomAccessIter,class Predicate>
RandomAccessIter FABLE_FASTCALL __find_if(RandomAccessIter first,RandomAccessIter last,Predicate pred,const random_access_iterator_tag&)
#ifdef FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS_BODY
{
    typename iterator_traits<RandomAccessIter>::difference_type trip_count = (last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
#else
;
#endif
#endif
#ifdef FABLE_RETAIL_STLP_FIND_IF_GUARD_BODY_PASS23
// Genuine existing STLport find_if / input-category dispatcher definitions.
// Native0066000a calls real __find_if0065f9ab; natural visibility first.

template<class Input,class Predicate>
inline Input FABLE_FASTCALL __find_if(Input first,Input last,Predicate pred,const input_iterator_tag&) {
    while(first!=last && !pred(*first)) ++first;
    return first;
}
template<class Input,class Predicate>
Input FABLE_FASTCALL find_if(Input first,Input last,Predicate pred) {
    return __find_if(first,last,pred,typename iterator_traits<Input>::iterator_category());
}

#elif defined(FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23)
template<class Input,class Predicate>
Input FABLE_FASTCALL find_if(Input first,Input last,Predicate pred);
#else
template<class Input,class Predicate>
inline Input FABLE_FASTCALL __find_if(Input first,Input last,Predicate pred,const input_iterator_tag&) {
    while(first!=last && !pred(*first)) ++first;
    return first;
}
template<class Input,class Predicate>
Input FABLE_FASTCALL find_if(Input first,Input last,Predicate pred) {
    return __find_if(first,last,pred,typename iterator_traits<Input>::iterator_category());
}
#endif // FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#endif
#ifdef FABLE_RETAIL_STLP_FIND_INPUT
// Retail opt-in (chunk 2058, row 0x00563920 CTCHeroOnlineScoreboard::DisplayScoreboardGUI): STLport 4.6 _algobase.h
// `find(first, last, val)` forwarding to the input-iterator `__find(first, last, val, const input_iterator_tag&)` (_algobase.c), both
// /Gr `__fastcall` COMDATs in retail for list<CClanMember>::iterator: find 0x00567A40 (landed, `ret 8`) and __find 0x00567240 (landed,
// `ret 0Ch`); the row calls find at one site and __find directly where find is expanded. Needs the list iterator's category typedef
// (FABLE_RETAIL_STLP_FIND_IF_INPUT in stlport/fable_containers.h).
template <class _InputIter, class _Tp>
inline _InputIter FABLE_FASTCALL __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&) {
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
template <class _InputIter, class _Tp>
inline _InputIter FABLE_FASTCALL find(_InputIter __first, _InputIter __last, const _Tp& __val) {
    return __find(__first, __last, __val, typename iterator_traits<_InputIter>::iterator_category());
}
#endif
}
#ifdef FABLE_RETAIL_STLP_STABLE_SORT_AUX_DECL
// Opt-in (B24C7/B24C9, row 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim): STLport 4.6.2 stable_sort, whose body forwards to
// __stable_sort_aux(first, last, value_type*, distance_type*, comp) (`push comp ; push 0 ; push 0 ; call`, retail 0x0070DF9F /
// 0x0070E16A and its out-of-line copy 0x00711C90), declared here; the landed __stable_sort_aux 0x00711B80 is __fastcall (Ego_r
// ??$__stable_sort_aux@PAU?$_List_iterator@..@U12@HVCAnimSetItPriorityLessThan@@@std@@YIX..: _RandomAccessIter, value type, int distance).
// STLport's spelling: _STLP_VALUE_TYPE / _STLP_DISTANCE_TYPE through iterator_traits (VC7.1 has partial specialisation).
namespace std {
template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
void FABLE_FASTCALL __stable_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Distance*, _Compare __comp);
template <class _RandomAccessIter, class _Compare>
void FABLE_FASTCALL stable_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp) {
  __stable_sort_aux(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0,
                    (typename iterator_traits<_RandomAccessIter>::difference_type*)0, __comp);
}
}
#endif
#ifdef FABLE_RETAIL_WHISPER_JUMP_FILTERS_PASS30
// STLport 4.6 _algo.c. The retail distance instantiation is at 008AC060.
namespace std {
template<class _ForwardIter, class _Predicate>
inline _ForwardIter FABLE_FASTCALL remove_if(_ForwardIter first, _ForwardIter last, _Predicate pred) {
    first = find_if(first, last, pred);
    if (first == last) return first;
    _ForwardIter next = first;
    return remove_copy_if(++next, last, first, pred);
}
}
#endif
#ifdef FABLE_RETAIL_STLP_TRANSFORM_PASS30
// 0080C74F and 0080C779 call the original 0080EC50 STLport algorithm.
namespace std {
template<class InputIter, class OutputIter, class UnaryOperation>
inline OutputIter FABLE_FASTCALL transform(InputIter first, InputIter last, OutputIter result, UnaryOperation op)
{
    for (; first != last; ++first, ++result)
        *result = op(*first);
    return result;
}
}
#endif
#endif
