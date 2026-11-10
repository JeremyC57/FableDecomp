// CTCWeatherVane::FrameUpdate() -- turn the vane to face the wind: normalise the environment's wind direction, convert its XY heading to a fraction of a turn wrapped into [0, 1), and set the owner's facing angle to it less the vane's initial offset
// Leaf: ?FrameUpdate@CTCWeatherVane@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00732D40, 185 bytes (192 with padding), `ret`.
// Identity: Ego_r.pdb ?FrameUpdate@CTCWeatherVane@@UAEXXZ; it follows CTCWeatherVane::OnInitialActivate 0x00732D00 (landed; it sets
//   InitialOffsetXY +0x0c). The catalog's "CViewBase_UpdateLookDirection" is an autoname. CTCWeatherVane: CTCBase + InitialOffsetXY (0x10).
// Callees: CTCBase::GetWorld 0x00686A70, CEnvironment::PeekWindDirection (Ego_r ?PeekWindDirection@CEnvironment@@UBEABVC3DVector@@XZ,
//   virtual slot 2), _CIfmod 0x00BFEB28 (fmod(x, 1.0), double 1.0 @ 0x01237F00), CTCPhysicsBase::SetFacingAngleXY(float) (virtual slot
//   66, +0x108). Constants: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC, 1/(2*pi) = 0.159155f @ 0x01237F08.
// Layout: engine/CTCBase.h (ParentThing +0x04), engine/CThing.h (PhysicsTC +0x60), engine/CWorld.h (PEnvironment +0x18 through the
//   FableWin.pdb inline GetEnvironment()), engine/CEnvironment.h (FABLE_RETAIL_CENVIRONMENT_VIRTUALS: the retail vtable 0x0125D538,
//   PeekWindDirection slot 2) and engine/CTCPhysicsBase.h (FABLE_RETAIL_CTCPHYSICSBASE: SetFacingAngleXY at +0x108). CTCWeatherVane
//   has no shared header: modelled from Ego_r.pdb (0x10 bytes).
// Shape: GetWorld().GetEnvironment() (inline accessor: the environment goes straight into ecx for the virtual call, 0x00732D5A);
//   the wind copied into a local and divided by its length through the shared C3DVector operator/ into a second vector
//   (`direction = wind / wind.GetMagnitude()`: the reciprocal, retail's z,y,x square order and the Y*inv operand order
//   `fxch st(1) ; fmul st(1)` at 0x00732D8A; an in-place normalise gives either the x,z,y squares or inv*Y); atan2 is the inline
//   `fpatan`; the [0,1) wrap is the landed WrapAngle model (0x007FAF60 / 0x00661540).
#pragma intrinsic(atan2, fmod, sqrt)
#define FABLE_RETAIL_CTCBASE
#include "engine/C3DVector.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"

FABLE_FORCEINLINE float WrapAngle(float angle)      // landed model 0x007FAF60 / 0x00661540
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#include "engine/CEnvironment.h"
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#include "engine/CWorld.h"
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"

class CTCWeatherVane : public CTCBase {      // Ego_r.pdb, 0x10 bytes
public:
    virtual void FrameUpdate();                 // slot 9 (vtable from 0x00732D00's TU)
private:
    float InitialOffsetXY;                      // +0x0c
};

void CTCWeatherVane::FrameUpdate()
{
    CTCPhysicsBase* physics = GetParentThing().GetPhysicsTC();
    if (physics) {
        C3DVector wind = GetWorld().GetEnvironment().PeekWindDirection();
        C3DVector direction = wind / wind.GetMagnitude();
        float angle = WrapAngle((float)atan2(direction.X, direction.Y) * 0.15915494f);
        physics->SetFacingAngleXY(angle - InitialOffsetXY);
    }
}