#ifndef FABLE_ENGINE_CABILITYTRACKDEF_H
#define FABLE_ENGINE_CABILITYTRACKDEF_H
#include "engine/CAbilityTrackLevelDef.h"
#include "engine/EHeroExperienceStatCategory.h"
#include "stlport/fable_containers.h"
// Layout: Ego_r ExperienceStat64/Abilities68/MaxedOutDescription84 -> native60/64/76.
// Native stat access multiplies index by80; RTTI123649c proves CParentDefClassBase.
class CAbilityTrackDef : public CParentDefClassBase {
public: EHeroExperienceStatCategory ExperienceStat; std::vector<CAbilityTrackLevelDef> Abilities; long MaxedOutDescription;
};
FABLE_STATIC_ASSERT(sizeof(CAbilityTrackDef)==0x50);
#endif
