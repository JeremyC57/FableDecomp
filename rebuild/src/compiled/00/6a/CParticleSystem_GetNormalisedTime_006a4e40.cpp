// CThingPhysical::GetHealthPercentage at 0x006A4E40.
// Leaf: ?GetHealthPercentage@CThingPhysical@@QBEMXZ
// Flags: /O2 /Oy
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTH_PERCENTAGE
#include "engine/CThingPhysical.h"

float CThingPhysical::GetHealthPercentage() const
{
    if (MaxHealth > 0.0f)
        return Health / MaxHealth;
    return 0.0f;
}
