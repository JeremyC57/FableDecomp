#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONFLYINGBASE_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONFLYINGBASE_H

#include "engine/CTCCreatureModeManager.h"
#include "engine/CCardinalSpline.h"

class CAnimateThing;
namespace NCreatureMode {
// Ego_r layouts and FableWin TPI 0x2B550. Retail CCardinalSpline is four bytes smaller than Ego_r's.
class CModeDragonFlyingBase : public CTCCreatureModeManager::CModeBase {
public:
    CModeDragonFlyingBase(CThing& thing);
    virtual void AnimUpdate(CAnimateThing& anim);
    virtual ~CModeDragonFlyingBase();
protected:
    CCardinalSpline FlightSpline;
    float SplinePercentageComplete;
    float LegAnimAlpha;
};

FABLE_STATIC_ASSERT(sizeof(CModeDragonFlyingBase) == 0x38);
}
#endif
