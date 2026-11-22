#ifndef FABLE_ENGINE_CTCKRAKENTENTACLE_H
#define FABLE_ENGINE_CTCKRAKENTENTACLE_H
#ifdef FABLE_RETAIL_CTCKRAKENTENTACLE_TRACK_PASS26
#include "engine/CTCBase.h"
#include "engine/CKrakenTentacleDef.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
// Original layout from Ego_r, corroborated by native ReturnToDesiredPos and TrackTarget.
class CTCKrakenTentacle : public CTCBase {
public:
    const CKrakenTentacleDef& GetTentacleDef() const { return *PTentacleDef; }
private:
    void TrackTarget();
    CDefPointer<const CKrakenTentacleDef> PTentacleDef;
    CIntelligentPointer<CThing> PKraken, PObstruction, PRippleEffect;
    bool CanStrike, Tracking, IsStriking;
    long QueuedStrikeTick, LastStrikeTick, TickToRise;
    C3DVector DesiredPos;
    float FacingAngle;
    CCharString DummyName;
    bool DoneDeathStuff;
};
#endif
#endif
