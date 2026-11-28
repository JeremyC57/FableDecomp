#ifndef FABLE_ENGINE_CNAVIGATIONAREAINIT_H
#define FABLE_ENGINE_CNAVIGATIONAREAINIT_H
#ifdef FABLE_RETAIL_NAV_AREA_INITIALISE_PASS30
#include "engine/CNavigationAreaUpdate.h"
#include "engine/C2DBoxI.h"
// Both PDB type records name these members. Native input offsets for DetailedAreas and NavRegionSeeds are 0x44 and 0x50.
class CNavigationAreaInit : public CNavigationAreaUpdate {
public:
    C2DBoxI WorldArea;
    long MapIndex;
    std::vector<C2DBoxF> DetailedAreas;
    std::vector<CNavigationPosition> NavRegionSeeds;
};
FABLE_STATIC_ASSERT_AT(15,sizeof(CNavigationAreaInit)==0x5c);
#endif
#endif
