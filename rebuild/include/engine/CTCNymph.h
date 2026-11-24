#ifndef FABLE_ENGINE_CTCNYMPH_H
#define FABLE_ENGINE_CTCNYMPH_H

#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CNymphDef.h"
#include "engine/CCardinalSpline.h"

class CThing;
class CThingCreatureBase;

// Ego_r CTCNymph, 116 bytes with a debug array in FlightSpline. The
// retail spline's twelve-byte array moves the tail four bytes earlier.
class CTCNymph : public CTCBase {
public:
    CTCNymph(CThing& thing);
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_NYMPH; }
    CThingCreatureBase* GetTarget() const { return PTarget.Get(); }
    bool GetShouldSummon();
    bool IsDecentAttackRange(const C3DVector& pos) const;
    bool HasBeenHit() const { return BeenHit; }
    void SetIsReadyToSummon(bool ready) { ReadyToSummon = ready; }
    const CNymphDef& GetNymphDef() const { return *PDef; }
    void ClearReadyToAttackFlags();
private:
    CDefPointer<const CNymphDef> PDef;
    C3DVector PointToFlyAround;
    float EffectHeightOffset;
    C3DVector InitialPos;
    CCardinalSpline FlightSpline;
    bool FlyingAround;
    CIntelligentPointer<CThing> PEffect;
    CIntelligentPointer<CThing> POldEffect;
    unsigned long LastTargetTick;
    bool CanAttack;
    bool ReadyToAttack;
    bool ReadyToSummon;
    bool HasBeenInLimbo;
    bool HasSpline;
    bool BeenHit;
    long FadeCounter;
    long TotalFadeTime;
    CIntelligentPointer<CThingCreatureBase> PTarget;
};
FABLE_STATIC_ASSERT_AT(54, sizeof(CTCNymph) == 0x70);

#endif
