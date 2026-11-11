// std::__copy(CBankFile::CRuntimeData* first, CBankFile::CRuntimeData* last, CBankFile::CRuntimeData* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCRuntimeData@CBankFile@@PAV12@H@std@@YIPAVCRuntimeData@CBankFile@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D1C30, 74 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 009d4010 std::vector<CBankFile::CRuntimeData, std::allocator<CBankFile::CRuntimeData>>::resize; binder ??$__copy@PAVCRuntimeData@CBankFile@@PAV12@H@std@@YIPAVCRuntimeData@CBankFile@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CBankFile::CRuntimeData.
#include "stlport/fable_containers.h"
class CBankFile {
public:
class CRuntimeData {
public:
    unsigned char _body[0xc];
};
};
template CBankFile::CRuntimeData* std::__copy<CBankFile::CRuntimeData*,CBankFile::CRuntimeData*,int >(CBankFile::CRuntimeData*, CBankFile::CRuntimeData*, CBankFile::CRuntimeData*, std::random_access_iterator_tag const&, int*);
