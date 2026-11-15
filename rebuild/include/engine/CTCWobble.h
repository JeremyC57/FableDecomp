#ifndef FABLE_ENGINE_CTCWOBBLE_H
#define FABLE_ENGINE_CTCWOBBLE_H
#ifdef FABLE_RETAIL_CTCWOBBLE_PASS22
// Original Ego_r layout48: CTCBase12, ParentRHSet24, timer/rotation/angle words.
#include "engine/CTCBase.h"
#include "engine/CRightHandedSet.h"
class CTCWobble : public CTCBase {
public:
    void SetWobbleParameters(float seconds,float angle);
private:
    CRightHandedSet ParentRHSet;
    long FramesOfWobbleRemaining;
    float DirectionLastRotated;
    float AngleOfWobble;
};
FABLE_STATIC_ASSERT(sizeof(CTCWobble)==0x30);
#endif
#endif
