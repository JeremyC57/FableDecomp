#ifndef FABLE_STLPORT_COUNT_IF_H
#define FABLE_STLPORT_COUNT_IF_H
#include "stlport/fable_containers.h"
// Original STLport4.6.2 stl/_algo.h69..79 and stl/_function.h95..102.
namespace std {
#ifndef FABLE_RETAIL_STLP_COUNT_IF_CALLED_FASTCALL_PASS23
template<class _Predicate>inline unary_negate<_Predicate> not1(const _Predicate& __pred){return unary_negate<_Predicate>(__pred);}
#endif
#ifdef FABLE_RETAIL_STLP_COUNT_IF_CALLED_FASTCALL_PASS23
// Original STLport difference_type result and /Gr call convention, native0065f6a6.
template<class _InputIter,class _Predicate>
typename iterator_traits<_InputIter>::difference_type FABLE_FASTCALL count_if(_InputIter,_InputIter,_Predicate);
#else
template<class _InputIter,class _Predicate>inline typename iterator_traits<_InputIter>::difference_type count_if(_InputIter __first,_InputIter __last,_Predicate __pred){
 typename iterator_traits<_InputIter>::difference_type __n=0;
 for(;__first!=__last;++__first)if(__pred(*__first))++__n;
 return __n;
}
#endif
}
#endif
