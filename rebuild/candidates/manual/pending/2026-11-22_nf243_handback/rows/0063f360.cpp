// std::_List_base<const CThingGameObject*>::~_List_base() -- STLport list base destructor: free every node, relink the sentinel to itself, then free the sentinel
// Leaf: ??1?$_List_base@PBVCThingGameObject@@V?$allocator@PBVCThingGameObject@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063F360, 62 bytes, thiscall, plain `ret`. Callees: free 0x00BFEA14.
// Name fix (NF243, 2026-11-22, the DTOR_OUTOFLINE census): was the stand-in `void __fastcall SortTreeRecursively_00436900(TreeList*)`
//   (catalog `_global::StdList_Clear_0063f360`; "CGuiControlTreePane::SortTreeRecursively" was a label propagated across
//   byte-identical list destructors). Two sources: (a) retail 0x0063F470 is the size-optimised list<const CThingGameObject*>::~list()
//   COMDAT, a 5-byte `jmp 0x0063F360` (STLport 4.6 _list.h `~list() {}`, the clear + free in ~_List_base), the same layout as
//   0x004046B0 / 0x00404710 and 0x0042ABCA / 0x0042B6D2; (b) the only landed caller, CWorldMap::IsLineOfSight 0x0063E6F0, destroys its
//   std::list<const CThingGameObject*> local here (call at 0x0063E92A). The PDBs carry a Dinkumware list (no _List_base), so the base
//   spelling is STLport 4.6's: descriptive-by-library, as nf239 / nf241 landed list<CActionParam*> / list<CComponent*>.
// Shape: STLport 4.6 `~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }` with clear() inlined: the loop variable
//   advances before the node is freed (esi), the sentinel is re-read from `this` (edi) after every call; the element is a pointer
//   (nothing destroyed); the sentinel free is null-tested by the malloc allocator's deallocate.
#include "stlport/fable_containers.h"
class CThingGameObject;
template std::_List_base<const CThingGameObject*, std::allocator<const CThingGameObject*> >::~_List_base();
