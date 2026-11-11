// std::__uninitialized_copy(CSubtitledMessage* first, CSubtitledMessage* last, CSubtitledMessage* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCSubtitledMessage@@PAV1@@std@@YIPAVCSubtitledMessage@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2100, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 004d2760 std::vector<CSubtitledMessage, std::allocator<CSubtitledMessage>>::_M_insert_overflow; caller 0071a2f0 std::vector<CSubtitledMessage, std::allocator<CSubtitledMessage>>::_M_fill_insert; caller 0071a2f0 std::vector<CSubtitledMessage, std::allocator<CSubtitledMessage>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVCSubtitledMessage@@PAV1@@std@@YIPAVCSubtitledMessage@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CSubtitledMessage.
#include "stlport/fable_containers.h"
class CSubtitledMessage {
public:
    unsigned char _body[0x44];
    CSubtitledMessage(const CSubtitledMessage& other);           // 0x004D1FE0
};
template CSubtitledMessage* std::__uninitialized_copy<CSubtitledMessage*,CSubtitledMessage* >(CSubtitledMessage*, CSubtitledMessage*, CSubtitledMessage*, std::__false_type const&);
