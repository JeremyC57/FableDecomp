#ifndef FABLE_ENGINE_CTHINGDRAWINFORMATION_H
#define FABLE_ENGINE_CTHINGDRAWINFORMATION_H

#ifdef FABLE_RETAIL_CTHINGPHYSICAL_DEBUGDRAW_PASS24
#include "fable_string.h"
#include "engine/CRGBColour.h"
class CThingDrawInformation {
public:
    CThingDrawInformation();
    CCharString DrawString;
    float RelativeSize;
    float BarPercentage;
    CRGBColour Colour;
    bool BarOn;
    bool StringOn;
};
FABLE_STATIC_ASSERT(sizeof(CThingDrawInformation) == 0x14);
#endif
#endif
