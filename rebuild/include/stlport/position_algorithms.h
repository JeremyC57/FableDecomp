#ifndef FABLE_STLPORT_POSITION_ALGORITHMS_H
#define FABLE_STLPORT_POSITION_ALGORITHMS_H
#include "stlport/fable_containers.h"
namespace std {
// Original STLport4.6 _algo.h; separate instantiated body native008ac060.
template<class I,class P>I FABLE_FASTCALL remove_if(I,I,P);
// STLport4.6 _iterator_base.h random-access distance path.
template<class I>inline ptrdiff_t FABLE_FASTCALL __distance(const I& first,const I& last,const random_access_iterator_tag&){return last-first;}
template<class I>inline ptrdiff_t FABLE_FASTCALL distance(const I& first,const I& last){return __distance(first,last,random_access_iterator_tag());}
}
#endif
