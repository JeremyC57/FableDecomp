#ifndef FABLE_ENGINE_CAISTATEGROUP_GOTOBED_H
#define FABLE_ENGINE_CAISTATEGROUP_GOTOBED_H
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRightHandedSet.h"
// Retail opt-in (Codex pass 17): Ego_r complete 96-byte class; native IsRunnable
// reads PBed +14h. FableWin donor preserves the original two success arms.
#ifdef FABLE_RETAIL_CAISTATEGROUP_GOTOBED
class CAIStateGroup_GoToBed : public CAIStateGroupBase {
public:
    virtual void Update();
    virtual bool IsRunnable();
    virtual bool IsStillValid() const;
    virtual void OnReplace();
    virtual void InitStateGroup(const CWorld&,CThingAICreature&);
    virtual bool InitCreaturePosition();
    bool IsInSleepTime() const;
protected:
    bool FindFreeBed(bool);
    bool FindFreeBanditBed();
    CIntelligentPointer<CThing> PBed;
    bool IsBedSleepPointMarker;
    long SleepStartTimeVariation;
    long SleepEndTimeVariation;
    bool BedToRightOfCreature;
    long ActionPointIndex;
    CCharString BedSide;
    bool PlayEmergencyGetOutOfBed;
    long BedGetOutOfBedFastAnim;
    C3DVector BedPos;
    CRightHandedSet Rhset;
    bool IsHeroTrespassingPerceived() const;
    bool IsUnreportedCrimeKnown() const;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_GoToBed)==96);
#endif
#endif
