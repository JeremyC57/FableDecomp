// NCameraModes::CStringAndCageBase::UpdateAutoGoBehind(C3DVector& go_to) const -- swing the go-to point round the observed thing's focal point (keeping its XY distance) an eleventh of the way towards directly behind the thing each call
// Grade: RELOCATION_MATCH -- w24A: ported from local models to the shared headers, with the three ground-plane vectors written as the whole-vector expression followed by `.Z = 0.0f` (`centre = focal`, `direction = go_to - focal`, `behind = GetFacingVector() * -1.0f`). That spelling, with the DEFAULT shared C3DVector::Dot for both dot products, gives every x87 operand order, including the side-test product the local draft had swapped (`fld [behind.X] ; fmul [dir.Y]`, 0x0087224C); the draft built the vectors component by component (and needed an explicit Z, X, Y sum for the first dot).
// Leaf: ?UpdateAutoGoBehind@CStringAndCageBase@NCameraModes@@ABEXAAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00872110, 447 bytes, thiscall `ret 4` (Ego_r.pdb / FableWin.pdb name above: private const).
// Callees: CBase::GetObservedThing 0x0084F1B0, CTCGraphicAppearance::GetCameraFocalPos(float) 0x004C12D0 (the thing's +0x64
//   appearance), CTCPhysicsBase::GetFacingVector() (virtual +0x118 on the +0x60 physics TC), C3DVector::AccurateNormalise
//   0x00A14440 (the landed bound spelling; the draft called it Normalise), _CIacos 0x00BFEBA2, CBase::GetCamTools 0x0084D080
//   (result unused), C3DVector::Rotate(const C3DVector&, float) 0x00A13C60.
// Constants: 1.0f 0x0122DED8, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, 1/(2 pi) 0x01237F08, 1/11 0x01277544.
// Names: every helper is a shared-header member now -- C3DVector::CrossWithUp for the draft's "PerpendicularXY"; the draft's made-up
//   GetNegatedXY / GetLength are gone: the three flattened vectors are the full vector expression with `.Z = 0.0f` after it
//   (`direction = go_to - focal`, `behind = GetFacingVector() * -1.0f`, `centre = focal`), which VC7.1 folds to retail's stores.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_AUTOGOBEHIND
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_CAMERAFOCALPOS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include <math.h>
#pragma intrinsic(sqrt, acos)
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingGameObject.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CStringAndCageBase.h"
namespace NCameraModes {
void CStringAndCageBase::UpdateAutoGoBehind(C3DVector& go_to) const
{
    C3DVector focal = GetObservedThing()->GetTCGraphicAppearance()->GetCameraFocalPos(1.0f);
    C3DVector centre = focal;
    centre.Z = 0.0f;
    C3DVector direction = go_to - focal;
    direction.Z = 0.0f;
    float length = direction.GetMagnitude();
    direction *= 1.0f / length;
    C3DVector behind = GetObservedThing()->GetPhysicsTC()->GetFacingVector() * -1.0f;
    behind.Z = 0.0f;
    behind.AccurateNormalise();
    float dot = direction.Dot(behind);
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float angle = (float)acos(dot) * 0.15915494f;
    angle *= 0.090909094f;
    GetCamTools();
    if (behind.Dot(direction.CrossWithUp()) < 0.0f)
        angle *= -1.0f;
    direction.Rotate(C3DVector(0.0f, 0.0f, 1.0f), angle);
    go_to.X = length * direction.X + centre.X;
    go_to.Y = length * direction.Y + centre.Y;
}
}