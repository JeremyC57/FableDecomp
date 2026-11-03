// std::__uninitialized_fill_n<CPersistedDeadEntity*, unsigned int, CPersistedDeadEntity>(first, n, x, const __false_type&) -- copy-construct n copies of one 16-byte NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity into raw storage; returns the end of the run
// Leaf: ??$__uninitialized_fill_n@PAVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@IV123@@std@@YIPAVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@PAV123@IABV123@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005269F0, 58 bytes, /Gr (first ecx, n edx), `ret 8` (x, the tag reference).
// Identity: T = NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity (16 bytes: UID, GamePersistent, SectionNameCRC;
//   Ego_r TPI 0x194c4): 0x00521620 SaveToGameStateBinary calls 0x005269B0 for its dead_entities push_back growth, and
//   0x0052B030 vector<CPersistedDeadEntity>::_M_insert_overflow calls both helpers. Respelled at the Codex pass 11
//   consolidation-2 landing from the descriptive CUnprovenElement_005269b0 / CUnprovenElement16_005269f0 element.
// Shape: the shared STLport helper (stlport/fable_containers.h) as an explicit specialization (YI under /Gr);
//   `for (; n > 0; --n, ++cur) _Construct(&*cur, x);` with an unsigned count (`test edx,edx ; jbe`), the placement-new null
//   test, the implicit copy inline as a four-dword block copy through ebx.
#include "engine/NThingManager_CEntityRuntimePersistence.h"

typedef NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity CPersistedDeadEntity;

namespace std {
template<> CPersistedDeadEntity* __uninitialized_fill_n(CPersistedDeadEntity* first, unsigned int n,
                                                       const CPersistedDeadEntity& x, const __false_type&)
{
    CPersistedDeadEntity* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
} // namespace std