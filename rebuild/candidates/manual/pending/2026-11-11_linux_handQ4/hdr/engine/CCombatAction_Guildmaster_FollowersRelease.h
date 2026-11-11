#ifndef FABLE_ENGINE_CCOMBATACTION_GUILDMASTER_FOLLOWERSRELEASE_H
#define FABLE_ENGINE_CCOMBATACTION_GUILDMASTER_FOLLOWERSRELEASE_H
// New header (chunk Q3, rows 0x008E20F0 / 0x008E24F0): the guildmaster's "release" combat actions, Ego_r.pdb
// ??0CCombatAction_Guildmaster_ShieldFollowersRelease@@QAE@AAVCThingCreatureBase@@@Z (vtable 0x01286B04) and
// ??0CCombatAction_Guildmaster_SpeedUpFollowersRelease@@QAE@AAVCThingCreatureBase@@@Z. Retail expands both constructors in the
// mode FrameUpdates (0x008E247B..0x008E24C4: CCharString "SWORD_SWING", GetSoundPair 0x008561A0, CCombatActionBase(creature, 150,
// sound) 0x00858030, then the vtable store); the implicit destructor is ~CCombatActionBase 0x0062E900. No data members of their own.
// Needs FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR (engine/CCombatActionBase.h) and engine/CSoundPair.h.
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
class CCombatAction_Guildmaster_ShieldFollowersRelease : public CCombatActionBase {
public:
    CCombatAction_Guildmaster_ShieldFollowersRelease(CThingCreatureBase& creature)
        : CCombatActionBase(creature, 150, GetSoundPair(creature, "SWORD_SWING")) { }
};
class CCombatAction_Guildmaster_SpeedUpFollowersRelease : public CCombatActionBase {
public:
    CCombatAction_Guildmaster_SpeedUpFollowersRelease(CThingCreatureBase& creature)
        : CCombatActionBase(creature, 150, GetSoundPair(creature, "SWORD_SWING")) { }
};
#endif
