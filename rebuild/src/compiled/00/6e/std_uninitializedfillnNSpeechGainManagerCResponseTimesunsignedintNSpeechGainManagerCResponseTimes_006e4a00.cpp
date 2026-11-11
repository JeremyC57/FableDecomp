// std::uninitialized_fill_n(NSpeechGainManager::CResponseTimes* first, unsigned int n, NSpeechGainManager::CResponseTimes const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCResponseTimes@NSpeechGainManager@@IV12@@std@@YIPAVCResponseTimes@NSpeechGainManager@@PAV12@IABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E4A00, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 006e5240 source (stdvector_Mfillinsert_006e5240.cpp) names NSpeechGainManager::CResponseTimes; binder ??$uninitialized_fill_n@PAVCResponseTimes@NSpeechGainManager@@IV12@@std@@YIPAVCResponseTimes@NSpeechGainManager@@PAV12@IABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NSpeechGainManager::CResponseTimes.
#include "stlport/fable_containers.h"
#include "engine/NSpeechGainManager.h"
template NSpeechGainManager::CResponseTimes* std::uninitialized_fill_n<NSpeechGainManager::CResponseTimes*,unsigned int,NSpeechGainManager::CResponseTimes >(NSpeechGainManager::CResponseTimes*, unsigned int, NSpeechGainManager::CResponseTimes const&);
