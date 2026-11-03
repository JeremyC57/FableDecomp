// std::__uninitialized_copy<CPersistedDeadEntity*, CPersistedDeadEntity*>(first, last, result, const __false_type&) -- STLport placement copy-construct of a run of 16-byte NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity elements into raw storage, returns the end of the constructed run
// Leaf: ??$__uninitialized_copy@PAVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@PAV123@@std@@YIPAVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@PAV123@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005269B0, 62 bytes (+ int3 padding to 64), `ret 8`: /Gr STLport TU (first ecx, last edx;
//   result and the const __false_type& tag on the stack).
// Identity: T = NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity (16 bytes: UID, GamePersistent, SectionNameCRC;
//   Ego_r TPI 0x194c4): 0x00521620 SaveToGameStateBinary calls 0x005269B0 for its dead_entities push_back growth, and
//   0x0052B030 vector<CPersistedDeadEntity>::_M_insert_overflow calls both helpers. Respelled at the Codex pass 11
//   consolidation-2 landing from the descriptive CUnprovenElement_005269b0 / CUnprovenElement16_005269f0 element.
// Shape: the shared STLport helper (stlport/fable_containers.h) as an explicit specialization (it takes the primary template's
//   calling convention, YI under /Gr); `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);` -- the element's
//   implicit copy constructor copies four dwords through ebx; /O2 aligns the loop head.
#include "engine/NThingManager_CEntityRuntimePersistence.h"

typedef NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity CPersistedDeadEntity;

namespace std {
template<> CPersistedDeadEntity* __uninitialized_copy(CPersistedDeadEntity* first, CPersistedDeadEntity* last,
                                                     CPersistedDeadEntity* result, const __false_type&)
{
    CPersistedDeadEntity* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
} // namespace std