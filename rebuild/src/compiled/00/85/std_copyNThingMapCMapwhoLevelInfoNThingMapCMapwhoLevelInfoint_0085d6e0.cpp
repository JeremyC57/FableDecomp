// std::__copy(NThingMap::CMapwhoLevelInfo* first, NThingMap::CMapwhoLevelInfo* last, NThingMap::CMapwhoLevelInfo* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCMapwhoLevelInfo@NThingMap@@PAV12@H@std@@YIPAVCMapwhoLevelInfo@NThingMap@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085D6E0, 86 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0085e870 std::vector<NThingMap::CMapwhoLevelInfo, std::allocator<NThingMap::CMapwhoLevelInfo>>::resize; binder ??$__copy@PAVCMapwhoLevelInfo@NThingMap@@PAV12@H@std@@YIPAVCMapwhoLevelInfo@NThingMap@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NThingMap::CMapwhoLevelInfo.
#include "stlport/fable_containers.h"
namespace NThingMap {
class CMapwhoLevelInfo {
public:
    unsigned char _body[0x14];
};
}
template NThingMap::CMapwhoLevelInfo* std::__copy<NThingMap::CMapwhoLevelInfo*,NThingMap::CMapwhoLevelInfo*,int >(NThingMap::CMapwhoLevelInfo*, NThingMap::CMapwhoLevelInfo*, NThingMap::CMapwhoLevelInfo*, std::random_access_iterator_tag const&, int*);
