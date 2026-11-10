#ifndef FABLE_ENGINE_CANIMATETHINGUPDATE_H
#define FABLE_ENGINE_CANIMATETHINGUPDATE_H
// CAnimateThingUpdate (new shared header, chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate): the argument of
// CAnimateThing::Update 0x0070E8B0. Ego_r.pdb layout (12 bytes): TimePassed +0x00, DistanceMoved +0x04, PercentageComplete +0x08.
// Constructors (FableWin.pdb ??0CAnimateThingUpdate@@QAE@XZ / @MM@Z / @MMM@Z) are inline; the (time, distance) one leaves the
// percentage at 0 (retail 0x005B4031: `fdivr ; fstp [u] ; fldz ; fstp [u+4] ; fldz ; fstp [u+8]`).
#include "rebuild_abi.h"
class CAnimateThingUpdate {
public:
    CAnimateThingUpdate() : TimePassed(0.0f), DistanceMoved(0.0f), PercentageComplete(0.0f) { }
    CAnimateThingUpdate(float timePassed, float distanceMoved)
        : TimePassed(timePassed), DistanceMoved(distanceMoved), PercentageComplete(0.0f) { }
    CAnimateThingUpdate(float timePassed, float distanceMoved, float percentageComplete)
        : TimePassed(timePassed), DistanceMoved(distanceMoved), PercentageComplete(percentageComplete) { }
    float TimePassed;                                        // +0x00
    float DistanceMoved;                                     // +0x04
    float PercentageComplete;                                // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CAnimateThingUpdate) == 0xc);
#endif // FABLE_ENGINE_CANIMATETHINGUPDATE_H
