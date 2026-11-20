// std::_List_iterator_base::operator!=(const _List_iterator_base& y) const -- true when the two list iterators hold different nodes
// Leaf: ??9_List_iterator_base@std@@QBE_NABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C7C0, thiscall `ret 4`: compares this->_M_node with y._M_node; the optimised COMDAT the linker
//   kept for the STLport 4.6 _list.h inline `bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }`.
// NF146 (2026-11-20, q11818 one definition / naming): was a row-local `CP::NotEqual(const CP*)` stand-in defining ?NotEqual@CP@@QBE_NPBU1@@Z
//   (catalog label "_Tree_const_iterator<...>::operator!=" propagated); every landed caller (exe scan: 0x0041E6D3, 0x009F7B60, 0x009F7D70,
//   0x009F7DC0, compiled from the working tree) binds ??9_List_iterator_base@std@@QBE_NABU01@@Z through stlport/fable_containers.h (0x0041E6D3
//   through FABLE_RETAIL_STLP_LIST_NOINLINE_NE, whose header comment names this COMDAT). Name descriptive-by-library (STLport; both PDBs are
//   Dinkumware builds): proven by the body and the callers. The row emits the shared header's inline body as a pick-any COMDAT through the
//   nf86 / nf88 scaffolding below.
#include "stlport/fable_containers.h"

namespace {
// Fidelity: scaffolding, not retail: takes the address of the header's inline _List_iterator_base::operator!= so this object emits it as
// a pick-any COMDAT. Internal to this TU; nothing calls it.
bool (std::_List_iterator_base::*ReferenceListIteratorNotEqual())(const std::_List_iterator_base&) const
{
    return &std::_List_iterator_base::operator!=;
}
}
