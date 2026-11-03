// std::vector<NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity>::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- STLport growth insert for the 16-byte CPersistedDeadEntity: allocate size + max(size, fill_len) elements, copy the head out of line, construct x once (or fill out of line fill_len times), copy the tail unless atend, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052B030, 207 bytes, thiscall, `ret 14h` (five arguments).
// Identity: STLport 4.6 vector<T>::_M_insert_overflow(pos, x, const __false_type&, fill_len, atend) (the landed
//   0x0084F5E0 shape). T is CPersistedDeadEntity: its copy helper 0x005269B0 is the
//   __uninitialized_copy<CPersistedDeadEntity*> that row 0x00521620 (SaveToGameStateBinary) calls for its dead_entities
//   push_back growth (16-byte CPersistedDeadEntity, Ego_r TPI 0x194c4), and one instantiation calls its own helpers, so
//   the fill helper 0x005269F0 is __uninitialized_fill_n<CPersistedDeadEntity*, unsigned int, CPersistedDeadEntity>.
//   Respelled at the Codex pass 11 consolidation landing from the descriptive CUnprovenElement_005269b0 /
//   CUnprovenElement16_005269f0 element (0x0052B030 was the only binder of either spelling).
// Shape: the shared STLport vector (stlport/fable_containers.h, through NThingManager_CEntityRuntimePersistence.h) with
//   this member's explicit specialization; the two helpers are explicit specializations with bodies in the TU under
//   `#pragma auto_inline(off)` (noinline), so the const __false_type& tag is homed in the live `atend` slot. /Gr: the
//   helpers are YI (an explicit specialization takes the primary template's calling convention); the member stays thiscall.
#include "engine/NThingManager_CEntityRuntimePersistence.h"

typedef NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity CPersistedDeadEntity;

namespace std {
#pragma auto_inline(off)
template<> __declspec(noinline) CPersistedDeadEntity* __uninitialized_copy(CPersistedDeadEntity* first, CPersistedDeadEntity* last,
                                                                           CPersistedDeadEntity* result, const __false_type&)
{                                                          // 0x005269B0 (same-TU body)
    CPersistedDeadEntity* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template<> __declspec(noinline) CPersistedDeadEntity* __uninitialized_fill_n(CPersistedDeadEntity* first, unsigned int n,
                                                                             const CPersistedDeadEntity& x, const __false_type&)
{                                                          // 0x005269F0 (same-TU body)
    CPersistedDeadEntity* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
#pragma auto_inline(on)
template<> void vector<CPersistedDeadEntity>::_M_insert_overflow(iterator position, const CPersistedDeadEntity& x,
                                                                 const __false_type&, size_type fillLength, bool atEnd)
{
    const size_type oldSize = size();
    const size_type len = oldSize + (max)(oldSize, fillLength);
    CPersistedDeadEntity* newStart = this->_M_end_of_storage.allocate(len);
    CPersistedDeadEntity* newFinish = newStart;
    newFinish = __uninitialized_copy(this->_M_start, position, newStart, __false_type());
    if (fillLength == 1) {
        _Construct(newFinish, x);
        ++newFinish;
    } else
        newFinish = __uninitialized_fill_n(newFinish, fillLength, x, __false_type());
    if (!atEnd)
        newFinish = __uninitialized_copy(position, this->_M_finish, newFinish, __false_type());
    _M_clear();
    _M_set(newStart, newFinish, newStart + len);
}
} // namespace std