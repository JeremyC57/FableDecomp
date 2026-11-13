#ifndef FABLE_STLPORT_ALGORITHM_H
#define FABLE_STLPORT_ALGORITHM_H
// STLport 4.x (SGI) for_each + mem_fun_ref, as the landed NPlayerGui::CDrawMiniMap::ClearPrimitives 0x00644660 and
// CDrawQuickAccessIcons rows model them (chunk 867 lifts that TU-local model into a shared header): mem_fun_ref_t is the 4-byte
// functor holding the pointer-to-member; for_each is the /Gr `__fastcall` COMDAT at 0x0043F360 (hidden return pointer in ecx, the
// functor result returned by value: `lea ecx,<dead slot> ; mov edx,begin ; push pfn ; push end ; call`). Declaration only for
// for_each: the retail instantiation is a separate COMDAT, callers only relocate to it.
#include "rebuild_abi.h"
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
template<class It,class T> It FABLE_FASTCALL __find(It first,It last,const T& value,const random_access_iterator_tag&);
template<class It,class T> inline It find(It first,It last,const T& value) {
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
#endif
