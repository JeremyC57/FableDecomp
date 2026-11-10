// CUnprovenCombatAction_00976E70::~CUnprovenCombatAction_00976E70() -- the implicit destructor of a combat action with no members of its own: straight on to ~CCombatActionBase (a tail jump)
// Leaf: ??1CUnprovenCombatAction_00976E70@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00976E70, 5 bytes: `jmp 0x0062E900` (CCombatActionBase::~CCombatActionBase, Ego_r.pdb
//   ??1CCombatActionBase@@UAE@XZ, the binding 40+ landed rows use): the implicit destructor (no vtable store of its own; a
//   user-written empty `~X() {}` keeps a `mov [ecx],vftable` store, 11v5) with the base call turned into a jump.
// Identity (descriptive, unproven class): not called or referenced (xref.py); it is one of eight such jump destructors in the
//   Jack-of-Blades AI TU (0x00976E70, 0x00976F20, 0x00976FB0, 0x00977050, 0x009770F0, 0x00977280, 0x00977310, 0x009773B0: one
//   after each CCreatureAction_Jack* constructor + inline virtuals, JackScream's own 0x009771A0 standing in for its jump).
//   Seven of them follow the JackFlash .. JackFall classes; this first one precedes the JackFlash ctor 0x00976E80 and follows
//   CThing::GetTC<CTCJackPillar> 0x00976E20, so its class (a CCombatActionBase-derived action whose own ctor COMDAT was
//   kept from another TU, e.g. CCreatureAction_JackGetUp / CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend at
//   0x008C3810 / 0x008C27B0) is not pinned by any reference: the placeholder class names it.
// Model: the shared engine/CCombatActionBase.h over the retail CCreatureActionBase / CBaseObject chain (sizeof 0x114).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCombatActionBase.h"

class CUnprovenCombatAction_00976E70 : public CCombatActionBase {    // no members of its own; the destructor is implicit
public:
    CUnprovenCombatAction_00976E70();
};

// Emission helper: destroying the object by name makes VC7.1 emit the out-of-line implicit `??1` (the landed
// 0x008C36D0 CModeBriarRoseTeleportInvulnerable pattern); the implicit destructor stores no vtable of its own.
void DestroyOne(CUnprovenCombatAction_00976E70* action)
{
    action->CUnprovenCombatAction_00976E70::~CUnprovenCombatAction_00976E70();
}