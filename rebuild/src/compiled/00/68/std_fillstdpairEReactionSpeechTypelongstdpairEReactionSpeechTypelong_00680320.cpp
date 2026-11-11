// std::fill(std::pair<EReactionSpeechType,long>* first, std::pair<EReactionSpeechType,long>* last, std::pair<EReactionSpeechType,long> const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAU?$pair@W4EReactionSpeechType@@J@std@@U12@@std@@YIXPAU?$pair@W4EReactionSpeechType@@J@0@0ABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00680320, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00680f70 std::vector<std::pair<EReactionSpeechType, long>, std::allocator<std::pair<EReactionSpeechType, long>>>::_M_fill_insert; caller 00680f70 std::vector<std::pair<EReactionSpeechType, long>, std::allocator<std::pair<EReactionSpeechType, long>>>::_M_fill_insert; binder ??$fill@PAU?$pair@W4EReactionSpeechType@@J@std@@U12@@std@@YIXPAU?$pair@W4EReactionSpeechType@@J@0@0ABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<EReactionSpeechType,long>.
#include "stlport/fable_containers.h"
enum EReactionSpeechType { };
template void std::fill<std::pair<EReactionSpeechType,long>*,std::pair<EReactionSpeechType,long> >(std::pair<EReactionSpeechType,long>*, std::pair<EReactionSpeechType,long>*, std::pair<EReactionSpeechType,long> const&);
