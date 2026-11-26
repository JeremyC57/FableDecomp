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
#ifdef FABLE_RETAIL_COMBAT_INPUT_PASS26
class CInputEvent;
class CProcessedInput;
#endif
class CInputProcessCombat : public CInputProcessGameBase {
#ifdef FABLE_RETAIL_COMBAT_INPUT_PASS26
private:
    void ProcessCombatInput(const CInputEvent& event,CProcessedInput& processed_input);
#endif
public:
    CInputProcessCombat(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init), LastPositionTrackerFrameUpdate(0), NumberOfFramesAttackHeld(0) { }
    CJoystickPositionTracker PositionTracker;                       // +0x28
    long LastPositionTrackerFrameUpdate;                            // +0x50
    long NumberOfFramesAttackHeld;                                  // +0x54
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
    // Opt-in (NF477, r467d / r459a): the retail virtual list, declaration only. Retail vftable 0x012380B4, 9 counted function slots
    // (RTTI locator at vftable - 4, then consecutive slots at .text function starts); listed: each slot whose target differs from the
    // base class table, named by the base slot it fills (order from retail, spellings from Ego_r.pdb / FableWin.pdb).
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);           // slot 4, 0x0068FB90
public:
#endif
};
FABLE_STATIC_ASSERT_AT(19, sizeof(CInputProcessCombat) == 0x58);
#endif // FABLE_ENGINE_CINPUTPROCESSCOMBAT_H
