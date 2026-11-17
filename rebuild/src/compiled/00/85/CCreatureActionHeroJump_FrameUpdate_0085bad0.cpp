// CCreatureActionHeroJump::FrameUpdate() -- when the jump's frames running come within 7 frames of its total time, read the acting
//   creature (out-of-line CBaseIntelligentPointer::GetPItem, result unused), then run the base CCombatActionBase::FrameUpdate (tail jmp)
// Leaf: ?FrameUpdate@CCreatureActionHeroJump@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085BAD0, virtual slot 56 of vtable 0x012766A4 (RTTI .?AVCCreatureActionHeroJump@@).
// NF75 (round 7, land1117fu q10916): was the free __fastcall stand-in CCreatureActionHeroJump_FrameUpdate(CCreatureActionHeroJump*) on a
//   local int-field model calling the stand-in ?Base_FrameStep@@YIXPAVCCreatureActionHeroJump@@@Z. Now the PDB virtual
//   ?FrameUpdate@CCreatureActionHeroJump@@UAEXXZ (Ego_r.pdb / FableWin.pdb) of a class on the shared engine/CCombatActionBase.h (no
//   engine/ header exists for CCreatureActionHeroJump); its base call binds Ego_r.pdb ?FrameUpdate@CCombatActionBase@@UAEXXZ (0x00859D80,
//   overlay opt-in FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE). The fields are CCreatureActionBase's (Ego_r.pdb, retail offsets =
//   PDB: PCreature +0x08, TotalTime +0x10, FramesRunning +0x1C; engine/CCreatureActionBase.h FULL_MEMBERS).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NCreatureActionEvents.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"

class CCreatureActionHeroJump : public CCombatActionBase {
public:
    virtual void FrameUpdate();
};

void CCreatureActionHeroJump::FrameUpdate()
{
    if (FramesRunning + 7 >= TotalTime)
        PCreature.GetPItem();
    CCombatActionBase::FrameUpdate();
}
