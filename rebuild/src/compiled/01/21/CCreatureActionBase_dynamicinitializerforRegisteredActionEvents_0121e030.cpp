// CCreatureActionBase::`dynamic initializer for 'RegisteredActionEvents'' (0x013BA8BC) -- construct the static STLport map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > (0x1C-byte header node from malloc, empty, self-linked) and register its atexit destructor 0x01229280
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121E030, 60 bytes.
// agentNF164 (q4516): the static map is CCreatureActionBase::RegisteredActionEvents, the private static
//   ?RegisteredActionEvents@CCreatureActionBase@@0V?$map@VCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@4@@std@@A
//   that engine/CCreatureActionBase.h declares and the landed 0x006932B0 RegisterActionEventTypes binds. Ego_r.pdb + FableWin.pdb name
//   `dynamic initializer for 'CCreatureActionBase::RegisteredActionEvents'` (0x0100CA00) and its `dynamic atexit destructor` (0x0101B8B0).
//   Retail: `push 1Ch ; call malloc` (the 0x1C-byte node = 16-byte _Rb_tree_node_base + CCharString + 8-byte CCountedPointer), the header
//   stores into 0x013BA8BC / 0x013BA8C0, then `push 1229280h ; call _atexit`. The landed generic view CRetailTree_node0x1c g_tree_013ba8bc
//   is replaced by the definition of the static on the shared header, as the NF89 pair 0x0121F0E0 / 0x01229BE0 (the compiler emits both
//   functions from this one definition).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"

std::map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > CCreatureActionBase::RegisteredActionEvents;   // 0x013BA8BC
