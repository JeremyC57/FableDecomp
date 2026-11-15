// 00c29dc0 CSoundEventGroup::GetPositionFlag
// Flags: none
// Leaf: ?GetPositionFlag@CSoundEventGroup@@UAE_NPAUCSoundEventManager@@@Z
// Identity: descriptive; original names absent, native virtual slot and typed caller proved.
// Extent: whole8-byte function C29DC0..C29DC8; assigned232-byte span includes six siblings.
// Evidence: member table012B3674 slot8 C21330 returns position flag; callerC11573 forwards manager and consumes AL.
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#include "libpk/CSoundEventGroup.h"
bool CSoundEventGroup::GetPositionFlag(CSoundEventManager* manager)
{
    return First->GetPositionFlag(manager);
}