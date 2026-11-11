// std::uninitialized_fill_n(std::pair<EReactionSpeechType,long>* first, unsigned int n, std::pair<EReactionSpeechType,long> const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAU?$pair@W4EReactionSpeechType@@J@std@@IU12@@std@@YIPAU?$pair@W4EReactionSpeechType@@J@0@PAU10@IABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00680670, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00680f70 std::vector<std::pair<EReactionSpeechType, long>, std::allocator<std::pair<EReactionSpeechType, long>>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAU?$pair@W4EReactionSpeechType@@J@std@@IU12@@std@@YIPAU?$pair@W4EReactionSpeechType@@J@0@PAU10@IABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<EReactionSpeechType,long>.
#include "stlport/fable_containers.h"
enum EReactionSpeechType { };
template std::pair<EReactionSpeechType,long>* std::uninitialized_fill_n<std::pair<EReactionSpeechType,long>*,unsigned int,std::pair<EReactionSpeechType,long> >(std::pair<EReactionSpeechType,long>*, unsigned int, std::pair<EReactionSpeechType,long> const&);
