// std::_List_base<CCountedPointer<CNavRegionConnection> >::clear() -- STLport 4.6 _list.c clear: walk from the head's next node back to the head, destroying each node's counted pointer (count block release; destroy + delete at zero) and freeing the node, then re-link the head to itself
// Leaf: ?clear@?$_List_base@V?$CCountedPointer@VCNavRegionConnection@@@@V?$allocator@V?$CCountedPointer@VCNavRegionConnection@@@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80A20, 89 bytes, thiscall, plain `ret`. The head is re-read from `this` each iteration.
// Identity: the clear of CNavRegion::Connections (Ego_r.pdb list<CCountedPointer<CNavRegionConnection> >); the five landed REFS rows
//   (probe) already bind this spelling. Descriptive-by-library (no PDB holds STL instance names).
// Name fix (NF189, 2026-11-20, q4784): was the invented CRetailList_CountedPointer::clear over local CCountedPointer / SCountBlock
//   models; now the shared stlport/fable_containers.h _List_base::clear body (in-class; explicit instantiation of that member only)
//   over the shared engine/CCountedPointer.h (inline destructor: release, then both fields nulled).
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
class CNavRegionConnection;
namespace std {
template void _List_base<CCountedPointer<CNavRegionConnection>, allocator<CCountedPointer<CNavRegionConnection> > >::clear();
}
