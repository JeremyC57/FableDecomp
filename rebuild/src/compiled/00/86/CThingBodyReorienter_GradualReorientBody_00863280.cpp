// CThingBodyReorienter::GradualReorientBody() -- turn the body towards the final XY facing angle (angles in turns): wrap the remaining turn into [0, 1), take the short way round, snap and finish when it is within reach, else turn by at most half a turn this frame
// Leaf: ?GradualReorientBody@CThingBodyReorienter@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00863280, 290 bytes (+14 int3), `ret`.
// Callees: the body's physics component (Body +0x24 -> PhysicsTC +0x60) virtual slots 63 (+0xfc, GetFacingAngleXY, float)
//   and 66 (+0x108, SetFacingAngleXY(float)); _CIfmod @ 0x00BFEB28 (the fmod intrinsic, divisor the double 1.0
//   @ 0x01237F00). Globals: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 0.5f 0x0122F59C, -0.5f 0x01238798, 1e-4f 0x0129BA3C (an
//   extern const: `eps + 0.5f` stays a run-time add).
// Layout: Ego_r.pdb CThingBodyReorienter (0x2C): BodyReorientationDone +0, StillCheckingForNewBodyReorientationAngle +8,
//   HaveFinalReorientationAngle +9, FinalReorientationAngleXY +0xC; retail Body +0x24 (PDB +0x28; the STLport vector
//   before it is 12 bytes).
// Shape: the wrap is `if (d < 0 || d >= 1) { d = fmod(d, 1.0); if (d < 0) d += 1; }` through the frame float; the short
//   way is `delta = difference; if (difference > 0.5f) delta = difference - 1.0f;` (the ternary keeps the difference on
//   the x87 stack; this form homes it in its frame float and reloads it, as retail); the finish test compares |delta| with eps + 0.5; the clamp is an if/else-if
//   over -0.5 / 0.5 and the new angle `current + delta`.
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl fabs(double x);
// Headers: engine/CThing.h (GetPhysicsTC, PhysicsTC +0x60) and engine/CTCPhysicsBase.h (retail: GetFacingAngleXY slot 63,
//   SetFacingAngleXY slot 66); CThingBodyReorienter has no engine header (local class with the Ego_r.pdb layout).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#pragma intrinsic(fmod, fabs)
extern const float DELTA_NOTIONAL_ZERO;              // 0x0129BA3C (1e-4f; `eps + 0.5f` stays a run-time add)
enum ERotationDirection { };
class CThingBodyReorienter {
public:
    void UpdateBodyReorienation();
private:
    void GradualReorientBody();
    bool  BodyReorientationDone;                     // +0x00
    bool  ReorientingBody;                           // +0x01
    long  ReorientationCount;                        // +0x04
    bool  StillCheckingForNewBodyReorientationAngle; // +0x08
    bool  HaveFinalReorientationAngle;               // +0x09
    float FinalReorientationAngleXY;                 // +0x0c
    ERotationDirection RotationDirection;            // +0x10
    bool  HasPossibleIntersectingThings;             // +0x14
    std::vector<CIntelligentPointer<CThing> > PossibleIntersectingThings;   // +0x18
    CThing* Body;                                    // +0x24 (PDB +0x28: the STLport vector is 12 bytes)
};

void CThingBodyReorienter::GradualReorientBody()
{
    if (HaveFinalReorientationAngle) {
        float current = Body->GetPhysicsTC()->GetFacingAngleXY();
        float difference = FinalReorientationAngleXY - current;
        if (difference < 0.0f || difference >= 1.0f) {
            difference = (float)fmod((double)difference, 1.0);
            if (difference < 0.0f)
                difference += 1.0f;
        }
        float delta = difference;
        if (difference > 0.5f)
            delta = difference - 1.0f;
        if (fabs(delta) < DELTA_NOTIONAL_ZERO + 0.5f) {
            HaveFinalReorientationAngle = false;
            Body->GetPhysicsTC()->SetFacingAngleXY(FinalReorientationAngleXY);
            if (!StillCheckingForNewBodyReorientationAngle)
                BodyReorientationDone = true;
            return;
        }
        if (delta < -0.5f)
            delta = -0.5f;
        else if (delta > 0.5f)
            delta = 0.5f;
        Body->GetPhysicsTC()->SetFacingAngleXY(current + delta);
    }
}