// CCreatureAction_FireMissileWeapon::FrameUpdate() -- while the shot delay (DelayTimer) runs, count it down; at zero run the base
//   CCreatureActionBase::FrameUpdate (0x00694020, a direct non-virtual call)
// Leaf: ?FrameUpdate@CCreatureAction_FireMissileWeapon@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D5000, 23 bytes.
// NF64 (round 7, q10720): re-authored on engine/CCreatureAction_FireMissileWeapon.h (Ego_r.pdb 0x0090BCF0 / FableWin.pdb 0x02ADAB00
//   ?FrameUpdate@CCreatureAction_FireMissileWeapon@@UAEXXZ; DelayTimer per the header's member order). The base call binds the Ego_r.pdb
//   ?FrameUpdate@CCreatureActionBase@@MAEXXZ. Was an extern "C" __fastcall stand-in calling a made-up BaseFrameUpdate.
#pragma optimize("t", on)
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureAction_FireMissileWeapon.h"

void CCreatureAction_FireMissileWeapon::FrameUpdate()
{
    if (DelayTimer == 0)
        CCreatureActionBase::FrameUpdate();
    else
        --DelayTimer;
}
