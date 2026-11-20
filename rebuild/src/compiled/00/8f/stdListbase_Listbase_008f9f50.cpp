// std::_List_base<LHNetworkLib::LSocket*>::~_List_base() -- STLport list base destructor: clear (free every node), re-link the sentinel to
//   itself, then free the sentinel node
// Leaf: ??1?$_List_base@PAVLSocket@LHNetworkLib@@V?$allocator@PAVLSocket@LHNetworkLib@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F9F50 (clear expanded, then `if (sentinel) free(sentinel)`); reached by the 5-byte `jmp` COMDAT
//   0x008FA060 (~list<LSocket*>, the empty derived destructor), by the GlobalList atexit thunk 0x0122A200 (`jmp`) and by
//   LSocketServer::~LSocketServer 0x0090AA40 (call + tail jmp 0x0090AC01).
// Name fix (NF202, 2026-11-20, q12252 / q12257): the July row named it CGuiControlTreePane::SortTreeRecursively (a label propagated
//   across byte twins; Leaf `SortTreeRecursively`, symbol ?SortTreeRecursively_00436900@@YIXPAUTreeList@@@Z over TreeList / TreeNode
//   stand-ins). The body is STLport _List_base::~_List_base (the ~list thunk 0x008FA060 jumps here; FableWin.pdb has no ??1?$_List_base
//   for any T, so STL instance names are descriptive-by-library: the repo STLport header spelling). Ported onto stlport/fable_containers.h
//   + engine/LSocket.h exactly as the GlobalList atexit row 0x0122A200 (its _List_base COMDAT grades RELOCATION_MATCH here, NF193),
//   explicit instantiation.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/LSocket.h"
template std::_List_base<LHNetworkLib::LSocket*, std::allocator<LHNetworkLib::LSocket*> >::~_List_base();
