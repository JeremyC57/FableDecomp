// CTCSinglePersonOccupiable::IsOccupied() const -- the occupier pointer is set and the occupier is not dead (IS_THING_ALIVE on POccupier)
// Leaf: ?IsOccupied@CTCSinglePersonOccupiable@@QBE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087C3F0, 39 bytes: `lea esi,[ecx+0Ch]` (POccupier), GetPItem 0x00A01B50 twice (null -> false, then
//   CThing::Dead, bit 0 of +0x91 -> false), else true; plain `ret`.
// Identity (two sources): Ego_r.pdb 0x00844840 and FableWin.pdb ?IsOccupied@CTCSinglePersonOccupiable@@QBE_NXZ; Ego_r donor order
//   SetOccupier / GetOccupier / IsOccupied / IsOfType = retail 0x0087C390 / 0x0087C3E0 / 0x0087C3F0 / 0x0087C420; POccupier (+0x0C) is the
//   Ego_r member; NF76 row 0x00970CD0 and landed 0x006A8A30 already call it under this spelling.
// NF79 (round 7, q10968): re-authored on engine/CTCSinglePersonOccupiable.h (NF76's ISOCCUPIED opt-in) with the retail CThing chain
//   (preamble as SetOccupier 0x0087C390); was the free fastcall _global::IsThingAlive_0087c3f0 over a local holder / CThing /
//   CIntelligentPointer model (catalog label generated for a repeated body shape).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSINGLEPERSONOCCUPIABLE_ISOCCUPIED
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSinglePersonOccupiable.h"
bool CTCSinglePersonOccupiable::IsOccupied() const
{
    if (POccupier.GetPItem() != 0) {
        if (!POccupier.Get()->IsDead())
            return true;
    }
    return false;
}
