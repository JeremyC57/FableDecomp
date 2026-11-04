// std::swap(pair<C3DVector, long>& a, pair<C3DVector, long>& b) -- STLport swap of two (targeting position, value) pairs through a by-value temporary
// Leaf: ??$swap@U?$pair@VC3DVector@@J@std@@@std@@YIXAAU?$pair@VC3DVector@@J@0@0@Z
// Flags: /Gr
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
namespace std {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }
}
template void std::swap<std::pair<C3DVector, long> >(std::pair<C3DVector, long>&, std::pair<C3DVector, long>&);