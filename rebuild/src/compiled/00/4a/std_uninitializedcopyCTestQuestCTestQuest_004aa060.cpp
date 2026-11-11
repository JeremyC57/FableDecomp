// std::__uninitialized_copy(CTestQuest* first, CTestQuest* last, CTestQuest* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCTestQuest@@PAV1@@std@@YIPAVCTestQuest@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AA060, 112 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 004adb50 std::vector<CTestQuest>::_M_insert_overflow (non-const iterators); body: 28-byte element, six CCharString copy ctors (0x0099EC30) + one dword = Ego_r CTestQuest
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTestQuest.
#include "stlport/fable_containers.h"
#include "engine/CTestQuest.h"
template CTestQuest* std::__uninitialized_copy<CTestQuest*,CTestQuest* >(CTestQuest*, CTestQuest*, CTestQuest*, std::__false_type const&);
