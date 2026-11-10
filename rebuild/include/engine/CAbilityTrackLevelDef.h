#ifndef FABLE_ENGINE_CABILITYTRACKLEVELDEF_H
#define FABLE_ENGINE_CABILITYTRACKLEVELDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/EHeroAbility.h"
// Layout: Ego_r ExperienceCost/AbilityUnlocked/Description at64/68/72, native60/64/68.
// RTTI1236504 proves CParentDefClassBase; the parent is4 bytes smaller in retail.
class CAbilityTrackLevelDef : public CParentDefClassBase {
public: long ExperienceCost; EHeroAbility AbilityUnlocked; long Description;
};
FABLE_STATIC_ASSERT(sizeof(CAbilityTrackLevelDef)==0x48);
#endif
