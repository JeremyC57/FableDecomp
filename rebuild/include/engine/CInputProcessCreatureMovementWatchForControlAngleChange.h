#ifndef FABLE_ENGINE_CINPUTPROCESSCREATUREMOVEMENTWATCHFORCONTROLANGLECHANGE_H
#define FABLE_ENGINE_CINPUTPROCESSCREATUREMOVEMENTWATCHFORCONTROLANGLECHANGE_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessCreatureMovementWatchForControlAngleChange : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x01237F74;
// retail sizeof 0x30 (`push 30h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessWatchForControlAngleChange +0x1E8).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessCreatureMovementWatchForControlAngleChange@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable and the members below.
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/C2DVector.h"
class CInputProcessCreatureMovementWatchForControlAngleChange : public CInputProcessGameBase {
public:
    CInputProcessCreatureMovementWatchForControlAngleChange(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
    C2DVector LastJoyDirection;                                     // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CInputProcessCreatureMovementWatchForControlAngleChange) == 0x30);
#endif // FABLE_ENGINE_CINPUTPROCESSCREATUREMOVEMENTWATCHFORCONTROLANGLECHANGE_H
