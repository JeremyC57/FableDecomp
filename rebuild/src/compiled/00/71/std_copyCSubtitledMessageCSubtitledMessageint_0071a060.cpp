// std::__copy(CSubtitledMessage* first, CSubtitledMessage* last, CSubtitledMessage* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCSubtitledMessage@@PAV1@H@std@@YIPAVCSubtitledMessage@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A060, 67 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0071a110 std::vector<CSubtitledMessage, std::allocator<CSubtitledMessage>>::erase; callee 00719e80 CSubtitledMessage::operator=; binder ??$__copy@PAVCSubtitledMessage@@PAV1@H@std@@YIPAVCSubtitledMessage@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CSubtitledMessage.
#include "stlport/fable_containers.h"
class CSubtitledMessage {
public:
    unsigned char _body[0x44];
    CSubtitledMessage& operator=(const CSubtitledMessage& other);   // 0x00719E80
};
template CSubtitledMessage* std::__copy<CSubtitledMessage*,CSubtitledMessage*,int >(CSubtitledMessage*, CSubtitledMessage*, CSubtitledMessage*, std::random_access_iterator_tag const&, int*);
