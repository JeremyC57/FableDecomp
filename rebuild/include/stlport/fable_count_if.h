#ifndef FABLE_STLPORT_COUNT_IF_H
#define FABLE_STLPORT_COUNT_IF_H
#include "stlport/fable_containers.h"
// Original STLport4.6.2 stl/_algo.h69..79 and stl/_function.h95..102.
namespace std {
template<class _Predicate>inline unary_negate<_Predicate> not1(const _Predicate& __pred){return unary_negate<_Predicate>(__pred);}
template<class _InputIter,class _Predicate>inline typename iterator_traits<_InputIter>::difference_type count_if(_InputIter __first,_InputIter __last,_Predicate __pred){
 typename iterator_traits<_InputIter>::difference_type __n=0;
 for(;__first!=__last;++__first)if(__pred(*__first))++__n;
 return __n;
}
}
#endif
