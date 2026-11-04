// std::swap(pair<unsigned long, C3DVector>& a, pair<unsigned long, C3DVector>& b) -- STLport swap of two (key, vector) entries through a by-value temporary
// Leaf: ??$swap@U?$pair@KVC3DVector@@@std@@@std@@YIXAAU?$pair@KVC3DVector@@@0@0@Z
// Flags: /Gr
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
namespace std {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }
}
template void std::swap<std::pair<unsigned long, C3DVector> >(std::pair<unsigned long, C3DVector>&, std::pair<unsigned long, C3DVector>&);