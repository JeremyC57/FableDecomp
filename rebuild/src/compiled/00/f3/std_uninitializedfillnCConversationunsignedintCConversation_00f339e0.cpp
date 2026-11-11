// std::uninitialized_fill_n(CConversation* first, unsigned int n, CConversation const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCConversation@@IV1@@std@@YIPAVCConversation@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F339E0, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00f33810 std::vector<CConversation, std::allocator<CConversation>>::_M_fill_insert; callee 00e54ca0 CConversation::CConversation; binder ??$uninitialized_fill_n@PAVCConversation@@IV1@@std@@YIPAVCConversation@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CConversation.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "stlport/fable_containers.h"
#include "engine/CConversation.h"
template CConversation* std::uninitialized_fill_n<CConversation*,unsigned int,CConversation >(CConversation*, unsigned int, CConversation const&);
