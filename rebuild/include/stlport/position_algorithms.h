#ifndef FABLE_STLPORT_POSITION_ALGORITHMS_H
#define FABLE_STLPORT_POSITION_ALGORITHMS_H
#include "stlport/fable_containers.h"
namespace std {
// Original STLport4.6 _algo.h; separate instantiated body native008ac060.
#ifdef FABLE_RETAIL_STLP_REMOVE_IF_INLINE
// Per-TU opt-in (chunk 2017, row 0x00974110 CAIStateGroup_JackOfBladesBase::Summon): STLport 4.6 _algo.h remove_if / find_if, inline;
// that TU expands remove_if (0x009742A2..0x009742FC: the out-of-line __find_if<C3DVector*, unary_negate<..>> 0x008AAC50 with the
// random_access_iterator_tag by address, then remove_copy_if 0x008AB710 from ++next). __find_if is declared only (out of line).
template<class I,class P> I FABLE_FASTCALL __find_if(I first,I last,P pred,const random_access_iterator_tag&);
template<class I,class P> inline I find_if(I first,I last,P pred) { return __find_if(first,last,pred,random_access_iterator_tag()); }
template<class I,class P> inline I remove_if(I first,I last,P pred) {
    first=find_if(first,last,pred);
    if(first==last) return first;
    else { I next=first; return remove_copy_if(++next,last,first,pred); }
}
#else
template<class I,class P>I FABLE_FASTCALL remove_if(I,I,P);
#endif
// STLport4.6 _iterator_base.h random-access distance path.
template<class I>inline ptrdiff_t FABLE_FASTCALL __distance(const I& first,const I& last,const random_access_iterator_tag&){return last-first;}
template<class I>inline ptrdiff_t FABLE_FASTCALL distance(const I& first,const I& last){return __distance(first,last,random_access_iterator_tag());}
}
#endif
