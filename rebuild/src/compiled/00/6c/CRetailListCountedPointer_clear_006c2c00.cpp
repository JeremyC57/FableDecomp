// std::_List_base<CCountedPointer<CLevelLoader::CObject>, std::allocator<CCountedPointer<CLevelLoader::CObject> > >::clear() -- STLport list clear: walk from the first node to the sentinel, release each node's counted pointer (deleter and count block freed at zero), free the node, then link the sentinel to itself
// Leaf: ?clear@?$_List_base@V?$CCountedPointer@VCObject@CLevelLoader@@@@V?$allocator@V?$CCountedPointer@VCObject@CLevelLoader@@@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C2C00, 89 bytes, thiscall, `ret`; speed TU (`add esp,4` after each call).
//   Callees: the count block's deleter (CCPPointerInfo slot +4), operator delete 0x00BFE9BC (the block), free 0x00BFEA14 (the node).
// Identity (NF391 trace, NF396 re-check): its only references are the calls at 0x006C2CE3 / 0x006C2D23, the first instruction after
//   `mov esi,ecx` of the two byte-identical 25-byte list destructors 0x006C2CE0 / 0x006C2D20 (`call clear ; free(sentinel)`: the
//   STLport ~_List_base body, inlined into ~list in one of the twins; either way the call is the base's own clear()). 0x006C2D20 is
//   called by CLevelLoader's scalar deleting destructor 0x006C2E90 on the member at +0x14 = Ego_r.pdb CLevelLoader::ObjectList,
//   list<CCountedPointer<CLevelLoader::CObject> > (+20); 0x006C2CE0 has no reference. FableWin ??1CLevelLoader@@UAE@XZ
//   (level_loader.obj) destroys that list. Same 89-byte shape as the landed _List_base<CCountedPointer<T> >::clear rows 0x00669B50 / 0x009D7DB0. Both PDBs
//   are Dinkumware builds, so the spelling is the one the repo STLport emits (descriptive-by-library).
// NF396: was the stand-in CRetailList_CountedPointer::clear (a local model with a local CCountedPointer). Model: the shared
//   stlport/fable_containers.h _List_base over engine/CLevelLoader.h (CObject nested) and engine/CCountedPointer.h; the explicit
//   instantiation emits the member as its own pick-any COMDAT.
#define FABLE_RETAIL_CLEVELLOADER
#include "engine/CLevelLoader.h"

template void std::_List_base<CCountedPointer<CLevelLoader::CObject>, std::allocator<CCountedPointer<CLevelLoader::CObject> > >::clear();
