// std::__uninitialized_copy(std::pair<EReactionSpeechType,long>* first, std::pair<EReactionSpeechType,long>* last, std::pair<EReactionSpeechType,long>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@W4EReactionSpeechType@@J@std@@PAU12@@std@@YIPAU?$pair@W4EReactionSpeechType@@J@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006805B0, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00680f70 std::vector<std::pair<EReactionSpeechType, long>, std::allocator<std::pair<EReactionSpeechType, long>>>::_M_fill_insert; caller 00680f70 std::vector<std::pair<EReactionSpeechType, long>, std::allocator<std::pair<EReactionSpeechType, long>>>::_M_fill_insert; binder ??$__uninitialized_copy@PAU?$pair@W4EReactionSpeechType@@J@std@@PAU12@@std@@YIPAU?$pair@W4EReactionSpeechType@@J@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<EReactionSpeechType,long>.
#include "stlport/fable_containers.h"
enum EReactionSpeechType { };
template std::pair<EReactionSpeechType,long>* std::__uninitialized_copy<std::pair<EReactionSpeechType,long>*,std::pair<EReactionSpeechType,long>* >(std::pair<EReactionSpeechType,long>*, std::pair<EReactionSpeechType,long>*, std::pair<EReactionSpeechType,long>*, std::__false_type const&);
