#ifndef FABLE_ENGINE_CHEROSOULSCREATUREDEF_H
#define FABLE_ENGINE_CHEROSOULSCREATUREDEF_H
// Chunk 789 (new header). Ego_r.pdb CHeroSoulsCreatureDef : CSubDefClassBase, 0x34 bytes: CreatureType +0x28, Location +0x2c,
// NumCreatures +0x30 (ghidra_out/struct_layouts_egor.tsv; base padding 0x00..0x27). Retail agrees: WatchForRoundsInArena
// 0x00D7FE90 steps the Creatures vector by 0x34 and reads +0x28 (CreateCreature type), +0x2c (GetThingWithScriptName /
// GetAllThingsWithScriptName name) and +0x30 (count). Members only (no landed member function uses this header yet).
#include "engine/CSubDefClassBase.h"
#include "fable_string.h"
class CHeroSoulsCreatureDef : public CSubDefClassBase {
public:
    CHeroSoulsCreatureDef();
    CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef&);
    virtual ~CHeroSoulsCreatureDef();
    CCharString CreatureType;                                            // +0x28
    CCharString Location;                                                // +0x2c
    long NumCreatures;                                                   // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CHeroSoulsCreatureDef) == 0x34);
#endif
