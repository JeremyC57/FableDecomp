// CGameScriptInterface::IsEntityInLimbo(const CScriptThing& entity) const -- true when the entity's thing is alive and in limbo
// Leaf: ?IsEntityInLimbo@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088FD60, `ret 4`; `this` is unused.
// Identity (NF431, 2026-11-25): the retail RTTI vftable placement of CGameScriptInterface (0x01260F0C) names this PDB member here;
//   Ego_r.pdb and FableWin.pdb list the symbol. FableWin's /Od body gives the shape: entity.GetPThing() (CScriptThing slot 11, +0x2c),
//   IS_THING_ALIVE<CThing*> (Dead, bit 0 of +0x91), then CThing::IsInLimbo (InLimbo, bit 5), else false.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISINLIMBO_DIRECT
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"

bool CGameScriptInterface::IsEntityInLimbo(const CScriptThing& entity) const
{
    CThing* thing = entity.GetPThing();
    if (IS_THING_ALIVE(thing))
        return thing->IsInLimbo();
    return false;
}
