// std::_List_base<CMaintainedEvent, std::allocator<CMaintainedEvent> >::clear() -- destroy every node's CMaintainedEvent through its virtual
// destructor, free the node, re-link the head to itself
// Leaf: ?clear@?$_List_base@VCMaintainedEvent@@V?$allocator@VCMaintainedEvent@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E4710, 58 bytes, thiscall `ret`: walks from head->next to the head (re-read from `this` each
//   iteration), advances the cursor before the old node's value is destroyed through vtable slot 0 (scalar deleting destructor, flag 0:
//   the explicit destructor call), frees the node (free 0x00BFEA14, no null test), then links the head to itself.
// NF136 (2026-11-20, q3887): was the descriptive stand-in CRetailList_009e4710::clear_CRetailList_009e4710 (node value
//   CNodeValue_009e4710). Identity: STLport 4.6 _list.c _List_base::clear for list<CMaintainedEvent>, the MaintainedEventsList member
//   of CKeyboard / CMouse / CJoystick (Ego_r.pdb layouts, +0xD0C = 3340), whose landed destructors 0x00A66990 / 0x00A66F80 call it as
//   ?clear@?$_List_base@VCMaintainedEvent@@V?$allocator@VCMaintainedEvent@@@std@@@std@@QAEXXZ; CMaintainedEvent is polymorphic
//   (CBaseClass base, engine/CMaintainedEvent.h). STLport names are in no PDB (both are Dinkumware builds): descriptive-by-library.
//   Re-authored as the explicit instantiation of the stlport/fable_containers.h body.
#include "engine/CMaintainedEvent.h"
#include "stlport/fable_containers.h"

template void std::_List_base<CMaintainedEvent, std::allocator<CMaintainedEvent> >::clear();
