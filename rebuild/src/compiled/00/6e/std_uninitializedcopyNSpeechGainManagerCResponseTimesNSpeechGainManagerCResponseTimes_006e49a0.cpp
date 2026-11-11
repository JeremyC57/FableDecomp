// std::__uninitialized_copy(NSpeechGainManager::CResponseTimes* first, NSpeechGainManager::CResponseTimes* last, NSpeechGainManager::CResponseTimes* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCResponseTimes@NSpeechGainManager@@PAV12@@std@@YIPAVCResponseTimes@NSpeechGainManager@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E49A0, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 006e5240 source (stdvector_Mfillinsert_006e5240.cpp) names NSpeechGainManager::CResponseTimes; binder ??$__uninitialized_copy@PAVCResponseTimes@NSpeechGainManager@@PAV12@@std@@YIPAVCResponseTimes@NSpeechGainManager@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NSpeechGainManager::CResponseTimes.
#include "stlport/fable_containers.h"
#include "engine/NSpeechGainManager.h"
template NSpeechGainManager::CResponseTimes* std::__uninitialized_copy<NSpeechGainManager::CResponseTimes*,NSpeechGainManager::CResponseTimes* >(NSpeechGainManager::CResponseTimes*, NSpeechGainManager::CResponseTimes*, NSpeechGainManager::CResponseTimes*, std::__false_type const&);
