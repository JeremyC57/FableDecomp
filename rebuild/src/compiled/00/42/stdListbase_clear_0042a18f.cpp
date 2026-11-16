// std::_List_base<NUISystem::CComponent*>::clear() -- STLport 4.6 _list.c: unlink and free every node after the sentinel, then relink
// the sentinel to itself
// Leaf: ?clear@?$_List_base@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042A18F, size TU (`pop ecx` after free 0x00BFEA14), thiscall `ret`.
// Identity (NF42, round 5): called out of line by the landed list<CComponent*> destructor 0x0042ABCA and the CTCInventory /
// CFrontEndManager slider rows (0x00583815 / 0x005838DD / 0x005839A5 / 0x00595356), which all bind this STLport spelling; the
// node frees no element (a pointer element). STLport 4.6.2 source, descriptive-by-library (both PDBs are Dinkumware builds). Was the
// placeholder `List::Clear` on local node models (manifest label CGuiControlTreePane::SortTreeRecursively, propagated); now an
// explicit instantiation of stlport/fable_containers.h's _List_base::clear.
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
namespace NUISystem { class CComponent; }
template void std::_List_base<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::clear();
