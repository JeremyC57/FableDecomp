// std::__uninitialized_copy(std::pair<float,EConversationAttitude> const* first, std::pair<float,EConversationAttitude> const* last, std::pair<float,EConversationAttitude>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBU?$pair@MW4EConversationAttitude@@@std@@PAU12@@std@@YIPAU?$pair@MW4EConversationAttitude@@@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CF920, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00719470 std::vector<std::pair<float, EConversationAttitude>, std::allocator<std::pair<float, EConversationAttitude>>>::operator=; binder ??$__uninitialized_copy@PBU?$pair@MW4EConversationAttitude@@@std@@PAU12@@std@@YIPAU?$pair@MW4EConversationAttitude@@@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<float,EConversationAttitude>.
#include "stlport/fable_containers.h"
enum EConversationAttitude { };
template std::pair<float,EConversationAttitude>* std::__uninitialized_copy<std::pair<float,EConversationAttitude> const*,std::pair<float,EConversationAttitude>* >(std::pair<float,EConversationAttitude> const*, std::pair<float,EConversationAttitude> const*, std::pair<float,EConversationAttitude>*, std::__false_type const&);
