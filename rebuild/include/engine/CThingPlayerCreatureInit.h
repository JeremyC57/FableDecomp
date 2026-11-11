#ifndef FABLE_ENGINE_CTHINGPLAYERCREATUREINIT_H
#define FABLE_ENGINE_CTHINGPLAYERCREATUREINIT_H
// NEW (agentNF11, row 0x006A9DD0 CThingPlayerCreature::ConstructFromParams): the player-creature construction parameters.
// Ego_r.pdb CThingPlayerCreatureInit (struct_layouts_egor.tsv, 12 bytes, no STL member: the same offsets in retail; the row reads
// OriginalThing [init+0], PreviousThing [init+4], PlaceAtEndOfList byte [init+8], 0x006A9E16..0x006A9E22).
#include "rebuild_abi.h"
class CThingPhysical;
class CThingPlayerCreatureInit {
public:
    CThingPhysical* OriginalThing;                                       // +0x00
    CThingPhysical* PreviousThing;                                       // +0x04
    bool            PlaceAtEndOfList;                                    // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreatureInit) == 12);
#endif
