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

#include <assert.h>
#include <stdio.h>

namespace {
    unsigned long storage[64];
    unsigned long parent[64];
    unsigned long navigator[8];
    unsigned long slots[20];
    unsigned long start[3],finish[3];
    unsigned long step;
    bool clear;
    bool FABLE_FASTCALL CheckSegment(const void* self,void*,const C3DVector& a,const C3DVector& b)
    {
        assert(self == navigator && step++ == 1);
        assert(&a == reinterpret_cast<const C3DVector*>(start));
        assert(&b == reinterpret_cast<const C3DVector*>(finish));
        return clear;
    }
}
const C3DVector& CThingPhysical::PeekPos() const
{
    assert(this == reinterpret_cast<const CThingPhysical*>(parent));
    assert(step++ == 0);
    return *reinterpret_cast<const C3DVector*>(start);
}
int main()
{
    const CTCCreatureNavigation* nav = reinterpret_cast<const CTCCreatureNavigation*>(storage);
    storage[1] = reinterpret_cast<unsigned long>(parent);
    storage[3] = reinterpret_cast<unsigned long>(navigator);
    navigator[0] = reinterpret_cast<unsigned long>(slots);
    slots[10] = reinterpret_cast<unsigned long>(&CheckSegment);
    for (unsigned int i=0; i<2; ++i) {
        step = 0;
        clear = i != 0;
        const bool result = nav->IsPosNavLineOfSightTo(*reinterpret_cast<const C3DVector*>(finish));
        assert(result == clear && step == 2);
    }
    puts("006cba20_TEST PASS");
    return 0;
}
