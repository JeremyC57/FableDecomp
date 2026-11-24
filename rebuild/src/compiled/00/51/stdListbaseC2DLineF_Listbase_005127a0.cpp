// std::_List_base<C2DLineF>::~_List_base() -- STLport list base destructor: free every node, relink the sentinel to itself, then free the sentinel
// Leaf: ??1?$_List_base@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005127A0, 62 bytes, thiscall (this in ecx, kept in edi), plain `ret`.
//   Callees: MSVCR71 free (import thunk 0x00BFEA14), for the nodes and the sentinel.
// Name fix (NF246, 2026-11-22, the DTOR_OUTOFLINE census, nf243 NOTES section 2): was `std::list<C2DLineF>::~list()` from a row-local
//   list model (its comment: "whether the out-of-line symbol is ~list or ~_List_base is not recoverable from the bytes"). Two sources:
//   (a) retail 0x00514280 is the size-optimised list<C2DLineF>::~list() COMDAT, a 5-byte `jmp 0x005127A0` (`e9 1b e5 ff ff`; `ret 4`
//   before, CC padding after), directly after the list<C2DLineF> constructor 0x00514260 (0x18-byte node: the C2DLineF 16 bytes + the
//   links), STLport 4.6 _list.h `~list() {}` with the clear + free in ~_List_base, the layout of 0x004046B0 / 0x00404710 and 0x0063F360
//   / 0x0063F470 (a second such jmp, 0x00A81C40, sits in the navigation-layer heap TU); (b) the landed pair<unsigned __int64,
//   list<C2DLineF> > helpers on stlport/fable_containers.h (__unguarded_linear_insert 0x00A81E20, iter_swap 0x00A82B10) already bind
//   this address as ~_List_base. The PDBs carry a Dinkumware list (no _List_base), so the base spelling is STLport 4.6's:
//   descriptive-by-library, as nf239 / nf241 / nf243 / nf245 landed the other list destructors. Element type: the navigation layer's
//   line lists (Ego_r CNavigationLayer::LineLists vector<list<C2DLineF> >, SwitchableLineLists CVectorMap<unsigned __int64,
//   list<C2DLineF> >), engine/C2DLineF.h (no row-local model).
// Shape: STLport 4.6 `~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }` with clear() inlined: the loop variable
//   advances before the node is freed (esi), the sentinel is re-read from `this` (edi) after every call; the element is a POD
//   (nothing destroyed); the sentinel free is null-tested by the malloc allocator's deallocate.
#include "engine/C2DLineF.h"
#include "stlport/fable_containers.h"
template std::_List_base<C2DLineF, std::allocator<C2DLineF> >::~_List_base();
