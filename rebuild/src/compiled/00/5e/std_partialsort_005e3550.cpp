// std::__partial_sort<pair*, ..> (STLport 4.6 _heap.c / _algo.c, element std::pair<unsigned long, CAnimationEntry> (the animation bank CRC-keyed entry table)) -- explicit instantiation of the shared STLport template
// Leaf: ??$__partial_sort@PAU?$pair@KVCAnimationEntry@@@std@@U12@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@std@@YIXPAU?$pair@KVCAnimationEntry@@@0@000V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E3550, 117 bytes, `ret 0Ch`.
// NF95 (part 2, q11353/q11354 binder port): was a global-scope stand-in (?__partial_sort@@YI..) over a row-local model; now the std:: template
//   instantiated on the shared headers so the symbol is the retail std:: spelling (Ego_r.pdb template names; the 0x004E1D20 /
//   0x004E1D68 / 0x005E2EB0 / 0x005E27F0 recipes) and the callees bind under their std:: names (__adjust_heap self rows 0x005E1CB0 /
//   0x00A82BE0). Graded without /Gr: every heap function is declared FABLE_STLP_FASTCALL in the header.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STLP_PARTIAL_SORT_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CAnimationEntry.h"
#include "engine/CVectorMap.h"
template void std::__partial_sort(std::pair<unsigned long, CAnimationEntry>*, std::pair<unsigned long, CAnimationEntry>*, std::pair<unsigned long, CAnimationEntry>*,
 std::pair<unsigned long, CAnimationEntry>*, CKeyPairCompareLess<unsigned long, CAnimationEntry>);
