// std::uninitialized_fill_n(std::pair<unsigned long,CDefString>* first, unsigned int n, std::pair<unsigned long,CDefString> const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAU?$pair@KVCDefString@@@std@@IU12@@std@@YIPAU?$pair@KVCDefString@@@0@PAU10@IABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00470BCD, 35 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00470a9c std::vector<std::pair<unsigned long, CDefString>, std::allocator<std::pair<unsigned long, CDefString>>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAU?$pair@KVCDefString@@@std@@IU12@@std@@YIPAU?$pair@KVCDefString@@@0@PAU10@IABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<unsigned long,CDefString>.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "stlport/fable_containers.h"
#include "engine/CDefString.h"
template std::pair<unsigned long,CDefString>* std::uninitialized_fill_n<std::pair<unsigned long,CDefString>*,unsigned int,std::pair<unsigned long,CDefString> >(std::pair<unsigned long,CDefString>*, unsigned int, std::pair<unsigned long,CDefString> const&);
