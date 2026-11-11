// std::fill(CSubtitledMessage* first, CSubtitledMessage* last, CSubtitledMessage const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCSubtitledMessage@@V1@@std@@YIXPAVCSubtitledMessage@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A030, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0071a2f0 std::vector<CSubtitledMessage, std::allocator<CSubtitledMessage>>::_M_fill_insert; callee 00719e80 CSubtitledMessage::operator=; binder ??$fill@PAVCSubtitledMessage@@V1@@std@@YIXPAVCSubtitledMessage@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CSubtitledMessage.
#include "stlport/fable_containers.h"
class CSubtitledMessage {
public:
    unsigned char _body[0x44];
    CSubtitledMessage& operator=(const CSubtitledMessage& other);   // 0x00719E80
};
template void std::fill<CSubtitledMessage*,CSubtitledMessage >(CSubtitledMessage*, CSubtitledMessage*, CSubtitledMessage const&);
