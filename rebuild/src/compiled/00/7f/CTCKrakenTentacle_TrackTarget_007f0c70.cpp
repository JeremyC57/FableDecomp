// 007f0c70 CTCKrakenTentacle::TrackTarget
// Leaf: ?TrackTarget@CTCKrakenTentacle@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CTCKRAKENTENTACLE_TRACK_PASS26
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX_SQUARED
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCKraken.h"
#include "engine/CTCKrakenTentacle.h"
#include "engine/GFAngles.h"

inline CThing* CTCKraken::GetPTarget() const { return PTarget.Get(); }
inline CThing* CTCKraken::GetPCentreMarker() const { return PCentreMarker.Get(); }

void CTCKrakenTentacle::TrackTarget()
{
    FABLE_RETAIL_ASSERT(IS_THING_ALIVE(PKraken));
    CTCKraken* pTCKraken;
    if (!PKraken->GetTC(&pTCKraken)) {
        FABLE_RETAIL_ASSERT(pTCKraken);
    }
    CThing* pTarg = pTCKraken->GetPTarget();
    if (IS_THING_ALIVE(pTarg)) {
        C3DVector delta = pTarg->GetPos() - GetParentThing().GetPos();
        delta.Z = 0.0f;
        float desiredFace = delta.GetAngleXY();
        float currentFace = GetParentThing().GetPhysicsTC()->GetFacingAngleXY();
        float deltaAng = GFGetSignedAngleDifference(currentFace, desiredFace);
        bool angleInTolerence = false;
        if (fabs(deltaAng) < GetTentacleDef().TentacleStrikeAngleTolerence * (1.0f / 360.0f)) angleInTolerence = true;
        float rotSpeed = GetTentacleDef().TentacleTurnSpeed * (1.0f / 360.0f);
        if (deltaAng < 0.0f) {
            if (-rotSpeed > deltaAng)
                deltaAng = -rotSpeed;
        } else if (rotSpeed < deltaAng) {
            deltaAng = rotSpeed;
        }
        FABLE_RETAIL_ASSERT(pTCKraken->GetPCentreMarker());
        C3DVector desiredPos = pTarg->GetPos() - pTCKraken->GetPCentreMarker()->GetPos();
        desiredPos.Z = 0.0f;
        float desiredPosDelta = desiredPos.GetMagnitudeAndNormalise();
        desiredPos *= desiredPosDelta > pTCKraken->GetKrakenDef().PoolRadius ? pTCKraken->GetKrakenDef().PoolRadius : desiredPosDelta;
        C3DVector tentacleDelta = (desiredPos + pTCKraken->GetPCentreMarker()->GetPos()) - GetParentThing().GetPos();
        tentacleDelta.Z = 0.0f;
        desiredPosDelta = tentacleDelta.GetMagnitudeAndNormalise();
        if (GetTentacleDef().TentacleStrikeDistTolerence < desiredPosDelta) {
            desiredPosDelta = desiredPosDelta > GetTentacleDef().TentacleMoveSpeed ? GetTentacleDef().TentacleMoveSpeed : desiredPosDelta;
            GetParentThing().GetPhysicsTC()->SetPosition(GetParentThing().GetPos() + tentacleDelta * desiredPosDelta);
        } else CanStrike = angleInTolerence;
        GetParentThing().GetPhysicsTC()->SetFacingAngleXY(deltaAng + currentFace);
    }
}