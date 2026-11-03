#ifndef FABLE_ENGINE_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_H
#define FABLE_ENGINE_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_H
#include "engine/CCombatActionBase.h"
// Original TPI0x3bfa3; native setters at134/135 and ctor size148 prove full shape.
class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif
public:
    virtual ~CCombatAction_GenericStrikeResponseBase();
    void SetMoveOnHitOverride(bool b) { MoveOnHitOverride=b; }
    void SetCreateDustEffect(bool b) { CreateDustEffect=b; }
protected:
    unsigned char PThingThatHit[8],PointOfHit[12],DirectionOfHit[12];
    bool MoveOnHitOverride,CreateDustEffect;
    long FrameForDust;
    unsigned char DustTravelDirection[12];
};
FABLE_STATIC_ASSERT(sizeof(CCombatAction_GenericStrikeResponseBase)==0x148);
#endif
