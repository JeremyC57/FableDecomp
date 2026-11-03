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
