// CGameScriptInterface::CameraDoConversation(const CScriptThing& speaker, const CScriptThing& listener, ECameraOp op, bool) const --
//   frame a two-shot of speaker and listener: per op (0..3 over-the-shoulder at 0.75 / 0.25 turn, 4 speaker / 5 listener
//   close-up) place the camera half a unit behind one head along the flattened facing, rotate the facing, step forward,
//   then ScriptMoveToPosAndLookAtPos(pos, lookAt, 0) / ScriptStart / ScriptMoveToPosAndLookAtPos(mid, lookAt, 6) /
//   ScriptSetRotationSpeed(0) / ScriptStart on the game camera
// Grade: DIFFER(1612v1564) -- case bodies, call order, slots and frame match; the residue is cross-jumping: retail merges
//   cases 0, 1 and 3 before scheduling ("call GetFacingVector; push 0; push angle; jmp common", case 3 into case 1 at the
//   facing call; 4 and 5 likewise), ours merges only after the scheduler hoists the X*X / Y*Y loads above the pushes, so
//   each case keeps its own Normalise/Rotate copy (+48 bytes). Function-scope shared locals reproduce retail's merge points
//   (attempts c/h) but add assignment temporaries (frame 0xEC); inline helpers, rvalue refs, a this-expression Normalise
//   and GetSquaredMagnitude move nothing. 11 attempts; last three moved nothing.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088EDC0, 1564 bytes, thiscall, `ret 0x10`.
// Callees: CScriptThing::GetPThing, CThing::GetTCGraphicAppearance / CTCGraphicAppearance::GetFocalPos 0x004C1060,
//   CTCPhysicsBase::GetFacingVector() (vtable slot 70, +0x118), C3DVector::Rotate 0x00A14260, CAIGameCameraBase script calls.
// Leaf: ?CameraDoConversation@CGameScriptInterface@@UBEXABVCScriptThing@@0W4ECameraOp@@_N@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

namespace NGraphicAppearance {
class CTCGraphicAppearance : public CTCBase {
public:
    C3DVector GetFocalPos(float interpolation) const;   // 0x004C1060
};
}
enum ECameraOp { ECameraOp_Force32 = 0x7fffffff };

void CGameScriptInterface::CameraDoConversation(const CScriptThing& speaker, const CScriptThing& listener, ECameraOp op, bool unknown) const
{
    if (!speaker.IsAlive())
        return;
    if (!listener.IsAlive())
        return;
    if (!(speaker.GetPThing()->GetTypeFlags() & 8))
        return;
    if (!(listener.GetPThing()->GetTypeFlags() & 8))
        return;
    CThing* speakerThing = speaker.GetPThing();
    CThing* listenerThing = listener.GetPThing();
    switch (op) {
    case 2: {
        C3DVector pos = speakerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector lookAt = listenerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector dir = speakerThing->GetPhysicsTC()->GetFacingVector();
        dir.Z = 0.0f;
        dir.Normalise();
        pos -= dir * 0.5f;
        dir.Rotate(0.75f, 0.0f);
        pos += dir * 0.5f;
        C3DVector mid = pos + (lookAt - pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(pos, lookAt, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(mid, lookAt, 6.0f);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case 3: {
        C3DVector pos = speakerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector lookAt = listenerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector dir = speakerThing->GetPhysicsTC()->GetFacingVector();
        dir.Z = 0.0f;
        dir.Normalise();
        pos -= dir * 0.5f;
        dir.Rotate(0.25f, 0.0f);
        pos += dir * 0.5f;
        C3DVector mid = pos + (lookAt - pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(pos, lookAt, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(mid, lookAt, 6.0f);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case 0: {
        C3DVector pos = listenerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector lookAt = speakerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector dir = listenerThing->GetPhysicsTC()->GetFacingVector();
        dir.Z = 0.0f;
        dir.Normalise();
        pos -= dir * 0.5f;
        dir.Rotate(0.75f, 0.0f);
        pos += dir * 0.5f;
        C3DVector mid = pos + (lookAt - pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(pos, lookAt, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(mid, lookAt, 6.0f);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case 1: {
        C3DVector pos = listenerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector lookAt = speakerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector dir = listenerThing->GetPhysicsTC()->GetFacingVector();
        dir.Z = 0.0f;
        dir.Normalise();
        pos -= dir * 0.5f;
        dir.Rotate(0.25f, 0.0f);
        pos += dir * 0.5f;
        C3DVector mid = pos + (lookAt - pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(pos, lookAt, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(mid, lookAt, 6.0f);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case 4: {
        C3DVector pos = speakerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector lookAt = speakerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector dir = speakerThing->GetPhysicsTC()->GetFacingVector();
        dir.Z = 0.0f;
        dir.Normalise();
        pos += dir * 0.75f;
        C3DVector mid = pos + (lookAt - pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(pos, lookAt, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(mid, lookAt, 6.0f);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case 5: {
        C3DVector pos = listenerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector lookAt = listenerThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector dir = listenerThing->GetPhysicsTC()->GetFacingVector();
        dir.Z = 0.0f;
        dir.Normalise();
        pos += dir * 0.75f;
        C3DVector mid = pos + (lookAt - pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(pos, lookAt, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(mid, lookAt, 6.0f);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    }
}
