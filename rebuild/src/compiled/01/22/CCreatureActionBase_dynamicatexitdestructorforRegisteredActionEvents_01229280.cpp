// CCreatureActionBase::`dynamic atexit destructor for 'RegisteredActionEvents'' (0x013BA8BC) -- clear the static STLport map (recursive _M_erase 0x00696900 from the root, header relinked, size zeroed) and free its header node
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229280, 84 bytes.
// agentNF164 (q4516): the static map is CCreatureActionBase::RegisteredActionEvents, the private static
//   ?RegisteredActionEvents@CCreatureActionBase@@0V?$map@VCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@4@@std@@A
//   that engine/CCreatureActionBase.h declares and the landed 0x006932B0 RegisterActionEventTypes binds. Ego_r.pdb + FableWin.pdb name
//   `dynamic initializer for 'CCreatureActionBase::RegisteredActionEvents'` (0x0100CA00) and its `dynamic atexit destructor` (0x0101B8B0).
//   Retail: the inline ~map: when the size at 0x013BA8C0 is non-zero, the out-of-line _M_erase 0x00696900 from the root, the header
//   relinked (left / right to itself, parent 0) and the size zeroed, then `push header ; call free`. The landed CRetailTree / SNode stand-in
//   (g_CParticleMorphs_Tree_Deleter_01229280, Erase_00696900) is replaced by the definition of the static on the shared header, as the
//   NF89 pair 0x0121F0E0 / 0x01229BE0 (the compiler emits both functions from this one definition); _M_erase now binds the PDB spelling
//   the landed 0x00696900 row defines.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"

std::map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > CCreatureActionBase::RegisteredActionEvents;   // 0x013BA8BC
