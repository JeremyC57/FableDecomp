// CThingObject::GetAlpha() const -- the object's draw alpha: 0xFF unless its definition has property bit 13 (0x2000); then 0..40
//   by how closely its facing heading matches the sun's (40 when aligned, falling to 0 at a quarter turn, 0 beyond)
// Leaf: ?GetAlpha@CThingObject@@IBEEXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702E00, 488 bytes, __fastcall `ret`, returns unsigned char. No direct call and no vtable or
//   data reference (an out-of-line COMDAT the linker kept).
// Identity: Ego_r.pdb ?GetAlpha@CThingObject@@IBEEXZ 0x005522B0 sits between DrawGetEditDrawInfo 0x005521B0 and GetProperties
//   0x005524F0, as retail 0x00702E00 sits between the landed DrawGetEditDrawInfo 0x00702D10 and GetProperties 0x00702FF0; FableWin.pdb
//   has the same order (0x01D859B0 / 0x01D85F30 / ... / 0x01D86120) with ?GFGetUnsignedAngleDifference@@YIMMM@Z 0x01D86050 right
//   after GetAlpha -- the angle helper this body expands. Return type E (unsigned char) and const match the bytes. The catalog's
//   CViewBase_GetRotationDegrees and the draft's CThingHeadingGauge::GetHeadingAlignment were descriptive; replaced.
// Callees: CTCPhysicsBase::GetFacingVector (slot 70, +0x118, C3DVector by hidden pointer) on the +0x60 physics component,
//   CThing::PeekWorld 0x004C7AB0, CEnvironment::PeekSunVector (slot 4, +0x10) on the world's +0x18 environment, _CIfmod 0x00BFEB28 (x4),
//   __ftol2 0x00BFEA70.
// Shape: both headings through the inline C3DVector::GetAngleXY (atan2(X, Y) in turns, wrapped into [0, 1)); GFGetUnsignedAngleDifference
//   wraps both again (the sun's first), takes fabs and folds above half a turn. The definition test reads PDef in place
//   (`mov eax,[esi+0D0h] ; mov ecx,[eax+90h] ; test ch,20h`), not the virtual GetProperties. Retail expands all four wraps (four
//   _CIfmod calls): GET_ROUNDED_ANGLE and GFGetUnsignedAngleDifference are forced inline (GFAngles.h inline-budget devices; with plain
//   `inline` both stay out-of-line calls, 316 / 350 bytes v 488).
// Header: engine/CThingObject.h (FABLE_RETAIL_CTHINGOBJECT_GETALPHA, FABLE_RETAIL_CTHINGOBJECT_DEFINITION), engine/GFAngles.h
//   (FABLE_RETAIL_GFANGLES_UNSIGNED_DIFFERENCE, FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE), engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES), engine/CTCPhysicsBase.h,
//   engine/CWorld.h (FABLE_RETAIL_CWORLD_PEEKENVIRONMENT), engine/CEnvironment.h (FABLE_RETAIL_CENVIRONMENT_VIRTUALS).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CTHINGOBJECT_DEFINITION
#define FABLE_RETAIL_CTHINGOBJECT_GETALPHA
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_GFANGLES_UNSIGNED_DIFFERENCE
#define FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CWORLD_PEEKENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#include "fable_string.h"
#include "engine/GFAngles.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CEnvironment.h"
#include "engine/CWorld.h"

unsigned char CThingObject::GetAlpha() const
{
    if (PeekDefinition().Property & 0x2000) {
        float facing_angle = GetPhysicsTC()->GetFacingVector().GetAngleXY();
        float sun_angle = PeekWorld().PeekEnvironment().PeekSunVector().GetAngleXY();
        float difference = GFGetUnsignedAngleDifference(sun_angle, facing_angle);
        if (difference > 0.25f)
            return 0;
        return (unsigned char)((1.0f - difference * 4.0f) * 40.0f);
    }
    return 0xff;
}