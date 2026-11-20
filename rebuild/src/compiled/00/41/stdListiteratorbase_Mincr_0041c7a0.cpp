// std::_List_iterator_base::_M_incr() -- step the list iterator to the next node (`_M_node = _M_node->_M_next`)
// Leaf: ?_M_incr@_List_iterator_base@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C7A0, 7 bytes, thiscall: `mov eax,[ecx] ; mov edx,[eax] ; mov [ecx],edx ; ret`; the optimised
//   COMDAT the linker kept for the STLport 4.6 _list.h inline `void _M_incr() { _M_node = _M_node->_M_next; }`.
// NF146 (2026-11-20, q11818 one definition / naming): was a row-local `Iter::Step` stand-in (pack(1) Node / Iter model) defining
//   ?Step@Iter@@QAEXXZ; every landed caller (exe scan: 0x009F78F0, 0x009F7B60, 0x009F7D70, 0x009F7DC0, compiled from the working tree)
//   binds ?_M_incr@_List_iterator_base@std@@QAEXXZ through stlport/fable_containers.h. Name descriptive-by-library (STLport; both PDBs are
//   Dinkumware builds): proven by the body and the callers. The row now emits the shared header's inline body as a pick-any COMDAT through
//   the nf86 / nf88 scaffolding below (its includers emit the same COMDAT, so no nodup copy may exist).
#include "stlport/fable_containers.h"

namespace {
// Fidelity: scaffolding, not retail: takes the address of the header's inline _List_iterator_base::_M_incr so this object emits it as a
// pick-any COMDAT. Internal to this TU; nothing calls it.
void (std::_List_iterator_base::*ReferenceListIteratorIncr())()
{
    return &std::_List_iterator_base::_M_incr;
}
}
