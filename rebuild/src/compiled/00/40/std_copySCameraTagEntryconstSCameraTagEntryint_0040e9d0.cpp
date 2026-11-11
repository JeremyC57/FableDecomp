// std::__copy(SCameraTagEntry const* first, SCameraTagEntry const* last, SCameraTagEntry* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PBUSCameraTagEntry@@PAU1@H@std@@YIPAUSCameraTagEntry@@PBU1@0PAU1@ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040E9D0, 72 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00411b90 source (stdvector_operator_00411b90.cpp) names SCameraTagEntry; binder ??$__copy@PBUSCameraTagEntry@@PAU1@H@std@@YIPAUSCameraTagEntry@@PBU1@0PAU1@ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over SCameraTagEntry.
#include "stlport/fable_containers.h"
struct SCameraTagEntry {
    unsigned char _body[0x1c];
};
template SCameraTagEntry* std::__copy<SCameraTagEntry const*,SCameraTagEntry*,int >(SCameraTagEntry const*, SCameraTagEntry const*, SCameraTagEntry*, std::random_access_iterator_tag const&, int*);
