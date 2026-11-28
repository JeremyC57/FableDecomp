// Retail 0x006A4DD0, 20 bytes.
// Leaf: ?GetMapSizeZBelowAt@CThingPhysical@@QBEMABVC3DVector@@@Z
#define FABLE_RETAIL_CREATURE_NAV_FLEE_COVER_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CWORLDMAP
#include "fable_string.h"
#include "engine/CThingPhysical.h"
#include "engine/CWorldMap.h"

float CThingPhysical::GetMapSizeZBelowAt(const C3DVector& pos) const
{
    return PeekWorldMap().GetGroundSizeZAt(pos);
}

#include <assert.h>
#include <stdio.h>

namespace {
    unsigned long world_words[4];
    unsigned long thing_words[4];
    unsigned long position_words[3];
    const CThing* expected_thing;
    const C3DVector* expected_position;
    unsigned long step;
    float height;
}

const CWorldMap& CThing::PeekWorldMap() const
{
    assert(this == expected_thing && step++ == 0);
    return *reinterpret_cast<const CWorldMap*>(world_words);
}

float CWorldMap::GetGroundSizeZAt(const C3DVector& pos) const
{
    assert(this == reinterpret_cast<const CWorldMap*>(world_words));
    assert(&pos == expected_position && step++ == 1);
    return height;
}

int main()
{
    const CThingPhysical* thing = reinterpret_cast<const CThingPhysical*>(thing_words);
    expected_thing = thing;
    expected_position = reinterpret_cast<const C3DVector*>(position_words);
    const float values[] = {-230.125f, -0.25f, 0.0f, 0.5f, 99.125f};
    for (int i = 0; i < 5; ++i) {
        step = 0;
        height = values[i];
        float result = thing->GetMapSizeZBelowAt(*expected_position);
        assert(result == values[i] && step == 2);
    }
    puts("006a4dd0_TEST PASS");
    return 0;
}
