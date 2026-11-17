#pragma optimize("s",on)
// std::vector<EHeroAbility>::resize(size_type n, const EHeroAbility& x) -- STLport 4.6.2: erase the tail when shrinking, else fill-insert
//   n - size() copies of x at the end
// Leaf: ?resize@?$vector@W4EHeroAbility@@V?$allocator@W4EHeroAbility@@@std@@@std@@QAEXIABW4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F9DF8, 62 bytes, thiscall, `ret 8` (size-optimised TU).
// Identity (NF48, round 7): the callees are the out-of-line vector<EHeroAbility>::erase(iterator, iterator) 0x005F8F89 and
//   vector<EHeroAbility>::_M_fill_insert 0x005F9633 (landed); its landed callers CTCInventoryAbilities::OnSerialise 0x005F6E40 and
//   InitialiseAbilities 0x005EEDE2 bind this spelling on Ego_r.pdb's vector<EHeroAbility> members (ReceivedAbilityOrder). STLport takes
//   the value by const reference (FableWin.pdb's by-value resize is the Dinkumware form). Name descriptive-by-library. Was the generic
//   stand-in CArrayElem::resize with erase_005f8f89 / _Insert_n_005f9633 callee stand-ins.
// Shape: the shared stlport/fable_containers.h resize over the declaration-only erase(first, last)
//   (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE) and _M_fill_insert, emitted by an explicit instantiation.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/EHeroAbility.h"

template void std::vector<EHeroAbility>::resize(size_t, const EHeroAbility&);
