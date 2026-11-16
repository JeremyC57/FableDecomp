// NTCScriptedControl::CActionTalkBase::SetUpCamera(const CSubtitledMessage::CCameraTag&) -- place the conversation camera for one subtitle line
// Grade: RELOCATION_MATCH (successor 11). Levers: shared headers; same-TU GetCameraSetupPositions (0x00712340, landed text) defined above; FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY; visible default CCountedPointer::Reset (no RESET_CAMERA_DECL: c2 sees it keeps no address, one shared Reset call 0x007166BB; call target stays 0x0069C3D0); normalise inside each dir arm (tails cross-jump as retail); `delta` re-used after the focus switch; offset.Initialise in focus cases 1..4; dir.CrossWithUp() (FableWin ?CrossWithUp@C3DVector@@QBE?AV1@XZ) for the camera side; named head_pos / C2DBoxI area.
// FITTED: FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ (existing opt-in; only sum-order key) and the named locals `distance` / `camera_distance` (ordinary spellings chosen by grading: they re-key the c2 operand-sort records of the top magnitude 0x00715AB1; with neither the row differs there only). EVIDENCED: every other spelling (retail store shapes, call order, frame slots).
// Leaf: ?SetUpCamera@CActionTalkBase@NTCScriptedControl@@AAEXABVCCameraTag@CSubtitledMessage@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00715A70, 3356 bytes.
// Retail-asm: 0x0071666D / 0x00716681 `fistp dword ptr [esp+20h]` -- engine/GFFloatToLongNear.h (GFFloatToLongNear, the frame count of the CScriptModuleFullTransition camera module, one per zoom-speed arm)
#include <math.h>
#pragma intrinsic(sqrt)
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCBASE_TALKACTION_FRIEND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SCRIPTEDMODE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SETGAMESCRIPTEDMODE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEADPOS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
namespace NCameraModes { class CScriptModuleBase; }
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCScriptedControl.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/GFFloatToLongNear.h"

// Enumerators: installed-game misc_def_types.h; the CONVERSATION_* spellings are the landed ones (0x00712340, 0x00719A70).
enum EConversationProtagonist { CONVERSATION_PROTAGONIST_NONE = 0, CONVERSATION_PROTAGONIST_SPEAKER = 1, CONVERSATION_PROTAGONIST_SPEAKEE = 2 };
enum EConversationCameraPosition {
    CONVERSATION_CAMERA_IN_FRONT_OF_FACE = 0, CONVERSATION_CAMERA_LEFT_SHOULDER = 1, CONVERSATION_CAMERA_RIGHT_SHOULDER = 2,
    CONVERSATION_CAMERA_BEHIND_LEFT_SHOULDER_NEAR = 3, CONVERSATION_CAMERA_BEHIND_LEFT_SHOULDER_FAR = 4,
    CONVERSATION_CAMERA_BEHIND_RIGHT_SHOULDER_NEAR = 5, CONVERSATION_CAMERA_BEHIND_RIGHT_SHOULDER_FAR = 6,
    CONVERSATION_CAMERA_LEFT_OF_TORSO_NEAR = 7, CONVERSATION_CAMERA_LEFT_OF_TORSO_FAR = 8,
    CONVERSATION_CAMERA_RIGHT_OF_TORSO_NEAR = 9, CONVERSATION_CAMERA_RIGHT_OF_TORSO_FAR = 10,
    CONVERSATION_CAMERA_POS_LEFT = 11, CONVERSATION_CAMERA_POS_RIGHT = 12
};
enum EConversationCameraFocus {
    CONVERSATION_CAMERA_FOCUS_FACE = 0, CONVERSATION_CAMERA_FOCUS_10 = 1, CONVERSATION_CAMERA_FOCUS_25 = 2,
    CONVERSATION_CAMERA_FOCUS_33 = 3, CONVERSATION_CAMERA_FOCUS_50 = 4, CONVERSATION_CAMERA_FOCUS_HIGH = 5,
    CONVERSATION_CAMERA_FOCUS_LEVEL = 6, CONVERSATION_CAMERA_FOCUS_LOW = 7
};
enum EConversationCameraZoomType { CONVERSATION_CAMERA_NO_ZOOM = 0, CONVERSATION_CAMERA_SLOW_ZOOM = 1, CONVERSATION_CAMERA_FAST_ZOOM = 2 };
enum EConversationCameraView { CONVERSATION_CAMERA_VIEW_WIDE = 0, CONVERSATION_CAMERA_VIEW_MEDIUM = 1, CONVERSATION_CAMERA_VIEW_CLOSE = 2 };

class CSubtitledMessage { public:                                               // members as the landed 0x00712340 / 0x00716DE0 (Ego_r.pdb)
    class CCameraTag { public:
        EConversationProtagonist CameraPositionPerson;  // +0x00
        EConversationCameraPosition CameraPosition;     // +0x04
        EConversationProtagonist CameraFocusPerson;     // +0x08
        EConversationCameraFocus CameraFocusPosition;   // +0x0c
        EConversationCameraZoomType CameraZoomType;     // +0x10
        EConversationCameraView CameraView;             // +0x14
    };
};

namespace NCameraModes {                                                        // no engine header; layout = Ego_r.pdb CScriptModuleBase (0x4C)
class CScriptModuleBase { public:
    CScriptModuleBase();                                                        // 0x005ED070
    virtual ~CScriptModuleBase();
    CIntelligentPointer<CThing> PCamPoint;                                      // +0x04
    CIntelligentPointer<const CThing> PThingOrigin;                             // +0x0C
    C3DVector CoordOrigin;                                                      // +0x14
    float CoordAxis[6];                                                         // +0x20 CRightHandedSet
    void* POwner;                                                               // +0x38 CBase*
    CGameCameraManager* PCameraManager;                                         // +0x3C
    bool SelfTerminate;                                                         // +0x40
    bool ShortenOnCollision;                                                    // +0x41
    long CutOutOverride;                                                        // +0x44
    float FOVOverride;                                                          // +0x48
};
class CScriptModuleGoToLookAt : public CScriptModuleBase { public:              // vtable 0x012657D8; ctor text = landed 0x007187C0
    CScriptModuleGoToLookAt(const C3DVector& goTo, const C3DVector& lookAt)
        : CScriptModuleBase(), m_pLookAtThing(), m_goToPosition(goTo), m_lookAtPosition(lookAt) { }
    virtual ~CScriptModuleGoToLookAt();
    CIntelligentPointer<CThing> m_pLookAtThing;                                 // +0x4C
    C3DVector m_goToPosition;                                                   // +0x54
    C3DVector m_lookAtPosition;                                                 // +0x60
};
class CScriptModuleFullTransition : public CScriptModuleBase { public:              // Ego_r 0x00827260 / FableWin 0x0283C7F0 ??0CScriptModuleFullTransition@NCameraModes@@QAE@ABVC3DVector@@000J@Z
    CScriptModuleFullTransition(const C3DVector& lookTo1, const C3DVector& lookFrom1, const C3DVector& lookTo2,
                                              const C3DVector& lookFrom2, long frames);   // 0x008525F0
    unsigned char _pad4c[0x80 - 0x4c];
};
}

namespace NTCScriptedControl {
class CActionTalkBase : public CTCScriptedControl::CActionBase {               // member names as the landed 0x00716DE0 / 0x00716B30
protected:
    CScriptThing PTargetThing;                                                  // +0x0C
    unsigned char _pad18[0x6D - 0x18];
    bool FarEnoughAwayForFacingCamera;                                          // +0x6D
    unsigned char _pad6e[0x74 - 0x6E];
    C3DVector ThingLookingFromPos, ThingLookingAtPos;                           // +0x74 / +0x80
    bool ThingLookingPosSet, ParentIsDemonDoor;                                 // +0x8C / +0x8D
private:
    CGameCameraManager& GetCameraManager() const                                // Ego_r ?GetCameraManager@..@@ABEAAVCGameCameraManager@@XZ (as 0x00716B30)
    {
        CGameCameraManager* manager;
        PTCScript->GetWorld().GetGameCamera()->GetCameraManager(&manager);
        return *manager;
    }
    void GetCameraSetupPositions(const CSubtitledMessage::CCameraTag& tag, const CThing*& fromThing, C3DVector& fromPos,
                                 const CThing*& atThing, C3DVector& atPos) const;   // 0x00712340 (same TU; text = landed row)
    void SetUpCamera(const CSubtitledMessage::CCameraTag& tag);
};
}
extern C3DVector g_CameraHeadOffset;                    // 0x01380C44 (descriptive, as landed 0x00712340)
extern C3DVector g_CameraTorsoOffset;                   // 0x01380C38 (descriptive)

void NTCScriptedControl::CActionTalkBase::GetCameraSetupPositions(const CSubtitledMessage::CCameraTag& tag, const CThing*& fromThing,
    C3DVector& fromPos, const CThing*& atThing, C3DVector& atPos) const
{
    CThing* me = PTCScript->ParentThing;
    CThing* other = PTargetThing.GetPThing();
    bool talkingToSelf = me == other;
    const CThing* first = 0;
    const CThing* second = 0;
    if (tag.CameraPosition == CONVERSATION_CAMERA_POS_LEFT) {
        first = me;
        second = other;
    } else if (tag.CameraPosition == CONVERSATION_CAMERA_POS_RIGHT) {
        first = other;
        second = me;
    }
    fromThing = 0;
    if (talkingToSelf) {
        fromThing = PTCScript->ParentThing;
        C3DVector forward = fromThing->GetPhysicsTC()->GetFacingVector();
        const NGraphicAppearance::CTCGraphicAppearance* app = fromThing->GetTCGraphicAppearance();
        C3DVector head = app->GetFocalPos(1.0f);
        fromPos = forward * 2.0f + head;
        fromPos.Z += 0.2f;
    } else if (tag.CameraPositionPerson == CONVERSATION_PROTAGONIST_SPEAKER) {
        fromThing = PTCScript->ParentThing;
        {
            const NGraphicAppearance::CTCGraphicAppearance* app = fromThing->GetTCGraphicAppearance();
            fromPos = app->GetFocalPos(1.0f);
        }
    } else if (tag.CameraPositionPerson == CONVERSATION_PROTAGONIST_SPEAKEE) {
        fromThing = PTargetThing.GetPThing();
        {
            const NGraphicAppearance::CTCGraphicAppearance* app = fromThing->GetTCGraphicAppearance();
            fromPos = app->GetFocalPos(1.0f);
        }
    } else {
        C3DVector half = (second->GetPos() - first->GetPos()) * 0.5f;
        fromPos = half + first->GetPos();
        switch (tag.CameraView) {
        case CONVERSATION_CAMERA_VIEW_WIDE: {
            C3DVector d = second->GetPos() - first->GetPos();
            fromPos += C3DVector(d.Y, -d.X, 0.0f) * 1.75f;
            break;
        }
        case CONVERSATION_CAMERA_VIEW_MEDIUM: {
            C3DVector d = second->GetPos() - first->GetPos();
            fromPos += C3DVector(d.Y, -d.X, 0.0f);
            break;
        }
        case CONVERSATION_CAMERA_VIEW_CLOSE: {
            C3DVector d = second->GetPos() - first->GetPos();
            fromPos += C3DVector(d.Y, -d.X, 0.0f) * 0.75f;
            break;
        }
        }
        const NGraphicAppearance::CTCGraphicAppearance* appA = first->GetTCGraphicAppearance();
        float z = appA->GetFocalPos(1.0f).Z;
        const NGraphicAppearance::CTCGraphicAppearance* appB = second->GetTCGraphicAppearance();
        fromPos.Z = (appB->GetFocalPos(1.0f).Z - z) * 0.5f + z;
    }
    atThing = 0;
    if (tag.CameraFocusPerson == CONVERSATION_PROTAGONIST_SPEAKER) {
        atThing = PTCScript->ParentThing;
        {
            const NGraphicAppearance::CTCGraphicAppearance* app = atThing->GetTCGraphicAppearance();
            atPos = g_CameraHeadOffset + app->GetFocalPos(1.0f);
        }
    } else if (tag.CameraFocusPerson == CONVERSATION_PROTAGONIST_SPEAKEE) {
        atThing = PTargetThing.GetPThing();
        {
            const NGraphicAppearance::CTCGraphicAppearance* app = atThing->GetTCGraphicAppearance();
            atPos = g_CameraHeadOffset + app->GetFocalPos(1.0f);
        }
    } else {
        C3DVector half = (second->GetPos() - first->GetPos()) * 0.5f;
        atPos = half + first->GetPos();
        const NGraphicAppearance::CTCGraphicAppearance* appA = first->GetTCGraphicAppearance();
        float z = appA->GetFocalPos(1.0f).Z;
        const NGraphicAppearance::CTCGraphicAppearance* appB = second->GetTCGraphicAppearance();
        atPos.Z = (appB->GetFocalPos(1.0f).Z - z) * 0.5f + z;
    }
    if (ParentIsDemonDoor) {
        atPos.Z += 1.4f;
        C3DVector d = fromPos - atPos;
        float length = d.GetMagnitudeAndNormalise();
        C3DVector forward = me->GetPhysicsTC()->GetFacingVector();
        if (d.Y * forward.Y + d.Z * forward.Z + forward.X * d.X < 0.45f || length < 2.45f) {
            fromPos = atPos;
            fromPos += forward * 2.5f;
        }
    }
}

void NTCScriptedControl::CActionTalkBase::SetUpCamera(const CSubtitledMessage::CCameraTag& tag)
{
    CThing* me = PTCScript->ParentThing;
    CThing* other = PTargetThing.GetPThing();
    bool talking_to_self = me == other;
    float distance = (me->GetPos() - other->GetPos()).GetMagnitude();
    if (distance > 2.0f)
        FarEnoughAwayForFacingCamera = true;
    const CThing* from_thing = 0;
    C3DVector from_pos(0.0f, 0.0f, 0.0f);
    const CThing* at_thing = 0;
    C3DVector at_pos(0.0f, 0.0f, 0.0f);
    GetCameraSetupPositions(tag, from_thing, from_pos, at_thing, at_pos);
    ThingLookingFromPos = from_pos;
    ThingLookingAtPos = at_pos;
    ThingLookingPosSet = true;
    C3DVector dir;
    if (from_thing && at_thing) {
        if (from_thing != at_thing) {
            dir = at_thing->GetPos() - from_thing->GetPos();
            dir *= GFFastOneOverSqrt(dir.GetSquaredMagnitude());
        } else if (talking_to_self)
            dir = -from_thing->GetPhysicsTC()->GetFacingVector();
        else {
            dir = other->GetPos() + me->GetPos();
            dir *= GFFastOneOverSqrt(dir.GetSquaredMagnitude());
        }
    } else {
        dir = from_pos - at_pos;
        dir *= GFFastOneOverSqrt(dir.GetSquaredMagnitude());
    }
    float camera_distance = (from_pos - at_pos).GetMagnitude();
    float zoom = (camera_distance - 0.4f) * (1.0f / 2.6f);
    if (zoom < 0.0f)
        zoom = 0.0f;
    else if (zoom > 1.0f)
        zoom = 1.0f;
    float height = -(zoom * zoom);
    C3DVector offset(0.0f, 0.0f, height);
    C3DVector at_start = at_pos;
        C3DVector delta = at_pos - from_pos;
        switch (tag.CameraFocusPosition) {
        case 1:
            offset.Initialise(0.0f, 0.0f, height * 0.1f);
            at_pos = from_pos + delta * 0.1f;
            break;
        case 2:
            offset.Initialise(0.0f, 0.0f, height * 0.25f);
            at_pos = from_pos + delta * 0.25f;
            break;
        case 3:
            offset.Initialise(0.0f, 0.0f, height * 0.33f);
            at_pos = from_pos + delta * 0.33f;
            break;
        case 4:
            offset.Initialise(0.0f, 0.0f, height * 0.5f);
            at_pos = from_pos + delta * 0.5f;
            break;
        case 5:
            from_pos.Z += 0.75f;
            offset = C3DVector(0.0f, 0.0f, 0.0f);
            break;
        case 6:
            offset = C3DVector(0.0f, 0.0f, 0.0f);
            break;
        case 7:
            from_pos.Z -= 0.75f;
            offset = C3DVector(0.0f, 0.0f, 0.0f);
            break;
        }
    if (offset.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO && from_pos.Z > at_pos.Z) {
        delta = at_pos - from_pos;
        at_pos.Z = delta.Z * 0.75f + from_pos.Z;
    }
    at_pos += offset;
    long position = tag.CameraPosition;
    if (ParentIsDemonDoor)
        position = CONVERSATION_CAMERA_POS_LEFT;
    C3DVector camera_pos;
    switch (position) {
    case CONVERSATION_CAMERA_IN_FRONT_OF_FACE: camera_pos = from_pos + dir; break;
    case CONVERSATION_CAMERA_LEFT_SHOULDER: camera_pos = from_pos - dir.CrossWithUp() * 0.4f; break;
    case CONVERSATION_CAMERA_RIGHT_SHOULDER: camera_pos = from_pos + dir.CrossWithUp() * 0.4f; break;
    case CONVERSATION_CAMERA_BEHIND_LEFT_SHOULDER_NEAR: camera_pos = from_pos - dir.CrossWithUp() * 0.4f - dir * 0.5f; break;
    case CONVERSATION_CAMERA_BEHIND_RIGHT_SHOULDER_NEAR: camera_pos = from_pos + dir.CrossWithUp() * 0.4f - dir * 0.5f; break;
    case CONVERSATION_CAMERA_BEHIND_LEFT_SHOULDER_FAR: camera_pos = from_pos - dir.CrossWithUp() * 0.4f - dir * 2.0f; break;
    case CONVERSATION_CAMERA_BEHIND_RIGHT_SHOULDER_FAR: camera_pos = from_pos + dir.CrossWithUp() * 0.4f - dir * 2.0f; break;
    case CONVERSATION_CAMERA_LEFT_OF_TORSO_NEAR: camera_pos = from_pos - dir.CrossWithUp() + g_CameraTorsoOffset; break;
    case CONVERSATION_CAMERA_RIGHT_OF_TORSO_NEAR: camera_pos = from_pos + dir.CrossWithUp() + g_CameraTorsoOffset; break;
    case CONVERSATION_CAMERA_LEFT_OF_TORSO_FAR: camera_pos = from_pos - dir.CrossWithUp() * 2.6f + g_CameraTorsoOffset; break;
    case CONVERSATION_CAMERA_RIGHT_OF_TORSO_FAR: camera_pos = from_pos + dir.CrossWithUp() * 2.6f + g_CameraTorsoOffset; break;
    case CONVERSATION_CAMERA_POS_LEFT:
    case CONVERSATION_CAMERA_POS_RIGHT: camera_pos = from_pos; break;
    }
    const CThingSearchTools& tools = CMainGameComponent::Get()->GetWorld().GetThingSearchTools();
    std::vector<CThingCreatureBase*> creatures;
    C2DBoxI area(C2DCoordI((long)camera_pos.X, (long)camera_pos.Y), 2);
    tools.GetAllThingsInRoughArea<CThingCreatureBase>(area, CThingFilter_None(), creatures);
    for (CThingCreatureBase** it = creatures.begin(); it != creatures.end(); ++it) {
        CThingCreatureBase* creature = *it;
        const C3DVector& pos = creature->GetPos();
        float dy = pos.Y - camera_pos.Y;
        float dx = pos.X - camera_pos.X;
        float radius = creature->GetPhysicsTC()->Radius + 0.2f;
        if (dx * dx + dy * dy < radius * radius) {
            C3DVector head_pos = creature->GetHeadPos(true);
            float head = head_pos.Z + 0.5f;
            if (head > camera_pos.Z)
                camera_pos.Z = head;
        }
    }
    CCountedPointer<NCameraModes::CScriptModuleBase> module;
    if (tag.CameraZoomType == CONVERSATION_CAMERA_NO_ZOOM) {
        module.Reset(new NCameraModes::CScriptModuleGoToLookAt(camera_pos, at_pos));
    } else {
        C3DVector from_end = at_start - dir * 0.3f;
        long frames;
        if (tag.CameraZoomType == CONVERSATION_CAMERA_SLOW_ZOOM)
            frames = GFFloatToLongNear((float)CMainGameComponent::GetConstantFPS() * 15.0f);
        else
            frames = GFFloatToLongNear((float)CMainGameComponent::GetConstantFPS() * 0.8f);
        module.Reset(new NCameraModes::CScriptModuleFullTransition(at_pos, camera_pos, at_start, from_end, frames));
    }
    module->CutOutOverride = 1;
    module->FOVOverride = 0.2f;
    GetCameraManager().SetGameScriptedCameraMode(module, true);
}