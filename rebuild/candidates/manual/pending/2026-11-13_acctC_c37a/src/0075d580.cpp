// CTCCameraPointScriptedSpline::UpdateCameraParams(float index, C3DVector& pos, C3DVector& view_dir, float& fov, float& roll, bool& finished, const C3DVector* origin, const CRightHandedSet* axes) -- evaluate the camera splines at a timing index, queue the crossed key camera's pause and event, and return the next index
// Leaf: ?UpdateCameraParams@CTCCameraPointScriptedSpline@@QAEMMAAVC3DVector@@0AAM1AA_NPBV2@PBVCRightHandedSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075D580, 809 bytes, two `ret 0x20` (last instruction 0x0075D8A6).
// Name: Ego_r.pdb 0x0076D6E0 + FableWin.pdb 0x025C98E0 ?UpdateCameraParams@CTCCameraPointScriptedSpline@@QAEMMAAVC3DVector@@0AAM1AA_NPBV2@PBVCRightHandedSet@@@Z.
// Callees: CCardinalSpline::GetSplinePosition 0x00A567A0 (x5), GFLimit<float,float,float> 0x0048DAE0, C3DVector::AccurateNormalise 0x00A14440,
//   CTCCameraPointDefinitionBase::TransformPosition 0x0075C420 / TransformVector 0x0075C280, GetKeyCamera 0x0075CFE0 (x2), vector<CKeyCamera>::size
//   0x0075EE70 (x3, out of line here by the inline budget; inline inside GetKeyCamera), __ftol2 0x00BFEA70, CKeyCamera::GetEvent 0x0075C9B0,
//   CCharString::operator= 0x0099EFB0 / (const char*) 0x0099EFE0 / ~ 0x0099EAE0, CTCBase::GetWorld 0x00686A70, CThing::GetPos 0x004C73D0,
//   CMessageEventManager::AddMessage 0x00687540, list<CCharString>::push_back 0x0075F660. Global CMainGameComponent::ConstantFPS 0x01375550.
// Shape: the scalar spline gives fov in Y and roll in Z (`fld [esp+20h]` / `[esp+24h]`, 0x0075D651); the key-camera array base is loaded once
//   (`mov ebx,[edi]`, 0x0075D6C2) and kept across the later calls: VC7.1 does that only when the DEFINITION of GetKeyCamera precedes this
//   function in the TU (same text as the landed 0x0075CFE0, which also grades RELOCATION_MATCH from this TU); a named iterator local gives
//   other registers. `GetLength() != 0` is expanded where `!IsEmpty()` is called out of line by the inline budget (0x0075D804).
//   The next index lives in the dead `pos` parameter home; the pause branch copies `index` into it (0x0075D878).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_TRANSFORM
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/GFLimit.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCCameraPointScriptedSpline.h"

// CTCCameraPointScriptedSpline::GetKeyCamera 0x0075CFE0 (landed row; its definition precedes UpdateCameraParams in the retail TU, see Shape).
long CTCCameraPointScriptedSpline::GetKeyCamera(float t) const
{
    if (t >= 1.0f)
        return KeyCameras.size() - 1;
    return (long)((KeyCameras.size() - 1) * t);
}

float CTCCameraPointScriptedSpline::UpdateCameraParams(float index, C3DVector& pos, C3DVector& view_dir, float& fov, float& roll,
                                                       bool& finished, const C3DVector* origin, const CRightHandedSet* axes)
{
    if (PTimingSpline.IsNull() || PScalarSpline.IsNull() || PPositionSpline.IsNull() || PViewDirectionSpline.IsNull()
        || PAnimationSpeedSpline.IsNull())
        return index;

    C3DVector timing;
    PTimingSpline->GetSplinePosition(timing, index);
    float t = timing.X;
    GFLimit(t, 0.0f, 1.0f);
    PPositionSpline->GetSplinePosition(pos, t);
    PViewDirectionSpline->GetSplinePosition(view_dir, t);
    C3DVector speed;
    PAnimationSpeedSpline->GetSplinePosition(speed, t);
    AnimationSpeed = speed.X;
    C3DVector scalar;
    PScalarSpline->GetSplinePosition(scalar, t);
    fov = scalar.Y;
    roll = scalar.Z;
    view_dir.AccurateNormalise();
    pos = TransformPosition(pos, origin, axes);
    view_dir = TransformVector(view_dir, axes);

    long key = GetKeyCamera(index);
    float next_index = index + KeyCameras[key].DeltaIndex;
    long next_key = GetKeyCamera(next_index);
    if (next_index >= (float)NodeToEndOn / (float)(KeyCameras.size() - 1))
        next_index = (float)NodeToStartOn / (float)(KeyCameras.size() - 1);

    if (key != next_key || index == (float)NodeToStartOn / (float)(KeyCameras.size() - 1))
    {
        if (NextCrossInvalid)
            NextCrossInvalid = false;
        else
        {
            PauseFor = (float)((long)KeyCameras[next_key].GetPauseTime() * CMainGameComponent::GetConstantFPS());
            MessageToBeSent = KeyCameras[next_key].GetEvent();
        }
        if (next_key == NodeToEndOn && PauseFor == 0.0f)
            finished = true;
    }

    if (MessageToBeSent.GetLength() != 0)
    {
        GetWorld().GetMessageEventManager().AddMessage((EMessageEventType)0x2b, 10, GetParentThing().GetPos(), &GetParentThing(), 0, 0.0f, 0.0f, 0);
        MyMessages.push_back(MessageToBeSent);
        MessageToBeSent = "";
    }

    if (PauseFor > 0.0f)
    {
        PauseFor -= 1.0f;
        next_index = index;
        NextCrossInvalid = true;
    }
    return next_index;
}
