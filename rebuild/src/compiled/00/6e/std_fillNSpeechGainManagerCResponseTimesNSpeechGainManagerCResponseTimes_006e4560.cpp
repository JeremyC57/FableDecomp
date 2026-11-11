// std::fill(NSpeechGainManager::CResponseTimes* first, NSpeechGainManager::CResponseTimes* last, NSpeechGainManager::CResponseTimes const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCResponseTimes@NSpeechGainManager@@V12@@std@@YIXPAVCResponseTimes@NSpeechGainManager@@0ABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E4560, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 006e5240 source (stdvector_Mfillinsert_006e5240.cpp) names NSpeechGainManager::CResponseTimes; binder ??$fill@PAVCResponseTimes@NSpeechGainManager@@V12@@std@@YIXPAVCResponseTimes@NSpeechGainManager@@0ABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NSpeechGainManager::CResponseTimes.
#include "stlport/fable_containers.h"
#include "engine/NSpeechGainManager.h"
template void std::fill<NSpeechGainManager::CResponseTimes*,NSpeechGainManager::CResponseTimes >(NSpeechGainManager::CResponseTimes*, NSpeechGainManager::CResponseTimes*, NSpeechGainManager::CResponseTimes const&);
