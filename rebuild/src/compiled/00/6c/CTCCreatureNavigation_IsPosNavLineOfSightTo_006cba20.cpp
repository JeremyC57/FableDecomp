// CTCCreatureNavigation::IsPosNavLineOfSightTo asks the active navigator about the segment from its creature.
// Leaf: ?IsPosNavLineOfSightTo@CTCCreatureNavigation@@QBE_NABVC3DVector@@@Z
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
#define FABLE_RETAIL_CNAVIGATORBASE
#include "fable_string.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CNavigatorBase.h"

bool CTCCreatureNavigation::IsPosNavLineOfSightTo(const C3DVector& pos) const
{
    return PNavigator->IsNavLineOfSightClear(GetParentCreature().PeekPos(), pos);
}
