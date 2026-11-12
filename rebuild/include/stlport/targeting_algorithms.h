#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
// Retail opt-in (Codex pass21): original UpdateTargetList helper models; both PDBs and donor/native proof in row00804020.
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
// STLport4.6.2 excerpts adapted only for project names, type traits and native fastcall.
#ifndef FABLE_TARGETING_ALGORITHMS_H
#define FABLE_TARGETING_ALGORITHMS_H
#include "stlport/fable_algorithm.h"
namespace std {
template<class P> class unary_negate:public unary_function<typename P::argument_type,bool>{protected:P _M_pred;public:explicit unary_negate(const P&p):_M_pred(p){}bool operator()(const typename P::argument_type& x)const{return !_M_pred(x);}};
template<class P> inline unary_negate<P> FABLE_FASTCALL not1(const P&p){return unary_negate<P>(p);}

template<class I,class P> inline I FABLE_FASTCALL __find_if(I __first,I __last,P __pred,const random_access_iterator_tag&) {
 typename iterator_traits<I>::difference_type __trip_count=(__last-__first)>>2;
 for(;__trip_count>0;--__trip_count){
  if(__pred(*__first))return __first;++__first;
  if(__pred(*__first))return __first;++__first;
  if(__pred(*__first))return __first;++__first;
  if(__pred(*__first))return __first;++__first;
 }
 switch(__last-__first){
 case 3:if(__pred(*__first))return __first;++__first;
 case 2:if(__pred(*__first))return __first;++__first;
 case 1:if(__pred(*__first))return __first;
 case 0:default:return __last;
 }
}
template<class I,class P> inline I FABLE_FASTCALL find_if(I first,I last,P pred){return __find_if(first,last,pred,typename iterator_traits<I>::iterator_category());}
template<class I,class O,class P> inline O FABLE_FASTCALL remove_copy_if(I __first,I __last,O __result,P __pred){
 for(;__first!=__last;++__first)if(!__pred(*__first)){*__result=*__first;++__result;}
 return __result;
}
// Original STLport4.6.2 _algo.h:241-250.
template<class I,class O,class T> inline O FABLE_FASTCALL remove_copy(I first,I last,O result,const T& value){
 for(;first!=last;++first)if(!(*first==value)){*result=*first;++result;}
 return result;
}
// Original STLport4.6.2 _algo.h:281-293, including the conditional lifetime of __next.
template<class I,class P> inline I FABLE_FASTCALL remove_if(I __first,I __last,P __pred){
 __first=find_if(__first,__last,__pred);
 if(__first==__last)return __first;
 else{I __next=__first;return remove_copy_if(++__next,__last,__first,__pred);}
}
template<class I,class T> inline I FABLE_FASTCALL remove(I __first,I __last,const T& __val){
 __first=find(__first,__last,__val);
 if(__first==__last)return __first;
 else{I __next=__first;return remove_copy(++__next,__last,__first,__val);}
}
}
#endif

#endif
