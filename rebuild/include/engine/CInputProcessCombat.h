#ifndef FABLE_ENGINE_CINPUTPROCESSCOMBAT_H
#define FABLE_ENGINE_CINPUTPROCESSCOMBAT_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessCombat : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x012380B4;
// retail sizeof 0x58 (`push 58h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessCombat +0x164).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessCombat@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable and the members below.
// PositionTracker's constructor is out of line (0x00A0E0A0, Ego_r.pdb / FableWin.pdb ??0CJoystickPositionTracker@@QAE@XZ); LastPositionTrackerFrameUpdate and NumberOfFramesAttackHeld start at 0 (+0x50 / +0x54 behind the 0x28-byte STLport deque).
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CJoystickPositionTracker.h"
class CInputProcessCombat : public CInputProcessGameBase {
public:
    CInputProcessCombat(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init), LastPositionTrackerFrameUpdate(0), NumberOfFramesAttackHeld(0) { }
    CJoystickPositionTracker PositionTracker;                       // +0x28
    long LastPositionTrackerFrameUpdate;                            // +0x50
    long NumberOfFramesAttackHeld;                                  // +0x54
};
FABLE_STATIC_ASSERT(sizeof(CInputProcessCombat) == 0x58);
#endif // FABLE_ENGINE_CINPUTPROCESSCOMBAT_H
