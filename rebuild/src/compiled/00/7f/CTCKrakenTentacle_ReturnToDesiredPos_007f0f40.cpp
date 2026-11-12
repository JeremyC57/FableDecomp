// CTCKrakenTentacle::ReturnToDesiredPos(bool immediately) -- while the kraken is alive, move the tentacle's parent thing towards
// the kraken's dummy object named DummyName (raised by the def's TentacleHeightOffset when the dummy is found), and turn it to the
// kraken's facing plus FacingAngle; unless `immediately`, the turn is clamped to TentacleTurnSpeed/360 and the step to TentacleMoveSpeed.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F0F40, length 559 bytes (private, Ego_r/FableWin ?ReturnToDesiredPos@CTCKrakenTentacle@@AAEX_N@Z).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x5: alive test twice, the unused `kraken` local, the appearance, the physics),
//   CThing::GetPos 0x004C73D0, NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0, C3DVector::GetMagnitudeAndNormalise
//   0x00A14480, GFGetSignedAngleDifference 0x00A441E0 (__fastcall, floats on the stack), CTCPhysicsBase virtuals GetFacingAngleXY (+0xFC),
//   SetPosition (+0x78), SetFacingAngleXY (+0x108). Data: DELTA_NOTIONAL_ZERO 0x0129BA3C, 1/360 0x01238E00, 0.0f 0x0122DEDC.
// Shape: `dir` is assigned from the subtraction (temp at esp+3Ch dword-copied into dir); the kraken's facing is its own local
//   (krakenAngle) and the signed difference a new local `angle` -- VC7.1 then shares krakenAngle's frame word with angle and puts dist
//   below it as retail does (one `angle` variable swaps the dist/angle slots); IsNonZero in its `squared +=` Z, Y, X form.
// Layout: local CTCKrakenTentacle model (no shared header exists; PDB offsets), PTentacleDef kept as the def pointer word (CDefPointer).
// Leaf: ?ReturnToDesiredPos@CTCKrakenTentacle@@AAEX_N@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
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
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CKrakenTentacleDef.h"
#include "engine/GFAngles.h"

class CTCKrakenTentacle : public CTCBase {
public:
    const CKrakenTentacleDef* PTentacleDef;                 // +0x0c (CDefPointer<const CKrakenTentacleDef>)
    CIntelligentPointer<CThing> PKraken;                    // +0x10
    CIntelligentPointer<CThing> PObstruction;               // +0x18
    CIntelligentPointer<CThing> PRippleEffect;              // +0x20
    bool CanStrike;                                         // +0x28
    bool Tracking;                                          // +0x29
    bool IsStriking;                                        // +0x2a
    long QueuedStrikeTick;                                  // +0x2c
    long LastStrikeTick;                                    // +0x30
    long TickToRise;                                        // +0x34
    C3DVector DesiredPos;                                   // +0x38
    float FacingAngle;                                      // +0x44
    CCharString DummyName;                                  // +0x48
    bool DoneDeathStuff;                                    // +0x4c
private:
    void ReturnToDesiredPos(bool immediately);
};

void CTCKrakenTentacle::ReturnToDesiredPos(bool immediately)
{
    if (!IS_THING_ALIVE(PKraken))
        return;
    C3DVector current = ParentThing->GetPos();
    C3DVector desired = current;
    CThing* kraken = PKraken.Get();
    const CKrakenTentacleDef* def = PTentacleDef;
    CRightHandedSet orientation;
    if (PKraken->GetTCGraphicAppearance()->GetDummyObject(DummyName, 0, 0, desired, orientation, 1.0f))
        desired.Z += def->TentacleHeightOffset;
    C3DVector dir;
    dir = desired - current;
    float dist;
    if (dir.IsNonZero())
        dist = dir.GetMagnitudeAndNormalise();
    else
        dist = 0.0f;
    float krakenAngle = PKraken->GetPhysicsTC()->GetFacingAngleXY();
    float facing = ParentThing->GetPhysicsTC()->GetFacingAngleXY();
    krakenAngle += FacingAngle;
    float angle = GFGetSignedAngleDifference(facing, krakenAngle);
    if (!immediately) {
        float turn = def->TentacleTurnSpeed * (1.0f / 360.0f);
        if (angle < 0.0f) {
            if (-turn > angle)
                angle = -turn;
        } else if (turn < angle)
            angle = turn;
        if (!(dist < def->TentacleMoveSpeed))
            dist = def->TentacleMoveSpeed;
    }
    ParentThing->GetPhysicsTC()->SetPosition(dir * dist + current);
    ParentThing->GetPhysicsTC()->SetFacingAngleXY(angle + facing);
}