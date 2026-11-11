// std::uninitialized_fill_n(CSubtitledMessage* first, unsigned int n, CSubtitledMessage const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCSubtitledMessage@@IV1@@std@@YIPAVCSubtitledMessage@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00719E50, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0071a2f0 std::vector<CSubtitledMessage, std::allocator<CSubtitledMessage>>::_M_fill_insert; callee 004d1fe0 CSubtitledMessage::CSubtitledMessage; binder ??$uninitialized_fill_n@PAVCSubtitledMessage@@IV1@@std@@YIPAVCSubtitledMessage@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CSubtitledMessage.
#include "stlport/fable_containers.h"
class CSubtitledMessage {
public:
    unsigned char _body[0x44];
    CSubtitledMessage(const CSubtitledMessage& other);           // 0x004D1FE0
};
template CSubtitledMessage* std::uninitialized_fill_n<CSubtitledMessage*,unsigned int,CSubtitledMessage >(CSubtitledMessage*, unsigned int, CSubtitledMessage const&);
