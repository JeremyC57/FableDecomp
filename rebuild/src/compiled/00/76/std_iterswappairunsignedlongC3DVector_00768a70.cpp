// std::iter_swap(pair<unsigned long, C3DVector>* a, pair<unsigned long, C3DVector>* b) -- exchange two (key, vector) entries of the particle update-parameter vector map
// Leaf: ??$iter_swap@PAU?$pair@KVC3DVector@@@std@@PAU12@@std@@YIXPAU?$pair@KVC3DVector@@@0@0@Z
// Flags: /Gr
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
namespace std {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }
template <class _ForwardIter1, class _ForwardIter2> inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2) { swap(*__i1, *__i2); }
}
template void std::iter_swap<std::pair<unsigned long, C3DVector>*, std::pair<unsigned long, C3DVector>*>(std::pair<unsigned long, C3DVector>*, std::pair<unsigned long, C3DVector>*);