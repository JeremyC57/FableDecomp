#ifndef FABLE_ENGINE_CTCWOBBLE_H
#define FABLE_ENGINE_CTCWOBBLE_H
#ifdef FABLE_RETAIL_CTCWOBBLE_PASS22
// Original Ego_r layout48: CTCBase12, ParentRHSet24, timer/rotation/angle words.
#include "engine/CTCBase.h"
#include "engine/CRightHandedSet.h"
class CTCWobble : public CTCBase {
public:
    void SetWobbleParameters(float seconds,float angle);
#ifdef FABLE_RETAIL_CTCWOBBLE_ONINITIALACTIVATE
    // hand2c opt-in (row 0x007E2DF0): Ego_r.pdb ?OnInitialActivate@CTCWobble@@UAEXXZ, the CTCBase slot 6 override (CTCWobble vtable
    // 0x0123DF34). Declaration only; no layout or vtable change.
    virtual void OnInitialActivate();                                    // 0x007E2DF0
#endif
private:
    CRightHandedSet ParentRHSet;
    long FramesOfWobbleRemaining;
    float DirectionLastRotated;
    float AngleOfWobble;
};
FABLE_STATIC_ASSERT(sizeof(CTCWobble)==0x30);
#endif
#endif
