#ifndef FABLE_ENGINE_CTCASSASSINRUSH_H
#define FABLE_ENGINE_CTCASSASSINRUSH_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#pragma pack(push,1)
class CTCAssassinRush : public CTCBase {
    CIntelligentPointer<CThing> PTarget;
    CDefPointer<const CSpecialAbilitiesAssassinRushDef> PDef;
    CIntelligentPointer<CThing> AssassinTrail, AssassinDisplacementTrail;
    long TrailDelay;
    bool StartTrailDelay;
    C3DVector PositionStartedFrom;
    CIntelligentPointer<CThing> CollidedThing;
    bool StartedMoving, JustReleased;
    unsigned char _pad43;
    long SlowTimeLength;
    bool DirectionOverride;
    C3DVector DirectionOverrideVector, BehindTargetPos;
    unsigned char _pad61[3];
    std::vector<CIntelligentPointer<CThing> > ItemAlphaList;
    float DashDistance;
    C3DVector LocalRelativeVectorForCamera, RelativeVectorForCamera;
    long SpellLevel;
    CCountedPointer<CCreatureActionBase> FinalAction;
    void* RadialBlurFadeKey;
    bool UseCollision, IsZTargetting, ScheduleForCancel, FakeCollideWithSomething;
    long RushDelayFrameCount;
    bool CompletedExitModes;
    unsigned char _padA5[3];
    void CleanUpEffects();
    void FinishMove(CThingCreatureBase&);
    void SetupFacingDirection(C3DVector&);
    C3DVector GetValidTeleportPos(const CTCPhysicsBase*);
    void RushForwards();
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CTCAssassinRush)==0xa8);
#endif
