// NTCScriptedControl::CActionTalkBase::SetUpCamera(const CSubtitledMessage::CCameraTag&) -- place the conversation camera for one subtitle line
// Grade: DIFFER(2860v3356) -- 1269 difflines: call order and the talking-to-self jump past the normalise now match retail (goto dir_done); VC7.1 cross-jumps focus cases 1..4 into one tail and keeps offset.Z in st where retail keeps a cached 0.0 in st0 across both switches, writes each focus case out in full and reloads from.Z in every position case (0x00716115 ff.), so ours is 496 bytes short; slot layout follows
// Next idea: find the spelling that stops the focus-case cross-jump (each case retail-stores offset X/Y from st0 = 0.0 first, then Z = spilled -(zoom*zoom) * k, then at_pos through a temp), e.g. a float zero local live across the switch or per-case distinct at_pos temporaries; then the position cases with from_pos copied per case
// Leaf: ?SetUpCamera@CActionTalkBase@NTCScriptedControl@@AAEXABVCCameraTag@CSubtitledMessage@@@Z
#include <math.h>
#pragma intrinsic(sqrt)
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/GFFloatToLongNear.h"
#include "stlport/fable_containers.h"
#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"
class CThing;
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCPhysicsBase { public:                          // vtable 0x0125D2C4
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacingVector() const;          // 70 (+0x118) 0x006B1410
    unsigned char _pad04[0x34 - 4];
    float Radius;                                       // +0x34 (descriptive)
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    const C3DVector& GetPos() const;                    // 0x004C73D0
};
class CThingCreatureBase : public CThing { public:
    C3DVector GetHeadPos(bool world);                   // 0x00661B40
};
class C2DBoxI { public: long X0, Y0, X1, Y1; C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { } };
class CThingFilter_None { };
class CThingSearchTools { public:
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_None& filter, std::vector<CThingCreatureBase*>& out) const;   // 0x007197C0
};
struct CThingSearchHolder { unsigned char _pad[0x20]; CThingSearchTools* Tools; };
class CMainGameComponent { public:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0
    static long ConstantFPS;                            // 0x01375550
    unsigned char _pad[0x24];
    CThingSearchHolder* SearchHolder;                   // +0x24 (descriptive)
};
namespace NCameraModes {
class CScriptModuleBase { public:
    virtual void v00();
    unsigned char m_state[0x44 - 4];
    long  TransitionType;                               // +0x44 (descriptive)
    float TransitionTime;                               // +0x48 (descriptive)
    CScriptModuleBase();                                // 0x005ED070
};
struct CBaseIntelligentPointer { virtual void v00(); void* m_pThing; CBaseIntelligentPointer(); };   // 0x00A01B10
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00(); CIntelligentPointer() : CBaseIntelligentPointer() { } };
class CScriptModuleGoToLookAt : public CScriptModuleBase { public:
    virtual void v00();
    CIntelligentPointer m_pLookAtThing;                 // +0x4C
    C3DVector           m_goToPosition;                 // +0x54
    C3DVector           m_lookAtPosition;               // +0x60
    CScriptModuleGoToLookAt(const C3DVector& goTo, const C3DVector& lookAt)
        : CScriptModuleBase(), m_pLookAtThing(), m_goToPosition(goTo), m_lookAtPosition(lookAt) { }
};
class CScriptModuleMoveBetweenLookFromAndLookTo : public CScriptModuleBase { public:
    unsigned char _pad4c[0x80 - 0x4c];
    CScriptModuleMoveBetweenLookFromAndLookTo(const C3DVector& lookTo1, const C3DVector& lookFrom1, const C3DVector& lookTo2,
        const C3DVector& lookFrom2, long frames);   // 0x008525F0
};
}
class CGameCameraManager { public:
    void SetGameScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module, bool preload);   // 0x00697B70
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetGameCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char      _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;              // +0x34
    CGameCameraManager* GetGameCameraManager() { CGameCameraManager* manager; PCurrentGameCamera->GetGameCameraManager(&manager); return manager; }
};
#include "engine/CTCBase.h"                             // ParentThing +0x04
class CTCScriptedControl : public CTCBase { public:
    CWorld& GetWorld() const;                           // 0x00686A70
    class CActionBase { public:
        void* __vftable;                                // +0x00
        CTCScriptedControl* PTCScript;                  // +0x04
        bool Activated;                                 // +0x08
        bool Terminated;                                // +0x09
    };
};
enum EConversationCameraZoomType { CONVERSATION_CAMERA_NO_ZOOM = 0, CONVERSATION_CAMERA_SLOW_ZOOM = 1, CONVERSATION_CAMERA_FAST_ZOOM = 2 };
class CSubtitledMessage { public:
    class CCameraTag { public:
        long CameraPositionPerson;                      // +0x00
        long CameraPosition;                            // +0x04
        long CameraFocusPerson;                         // +0x08
        long CameraFocusPosition;                       // +0x0c
        EConversationCameraZoomType CameraZoomType;     // +0x10
        long CameraView;                                // +0x14
    };
};
namespace NTCScriptedControl {
class CActionTalkBase : public CTCScriptedControl::CActionBase { public:
    CScriptThing PTargetThing;                          // +0x0c
    unsigned char _pad18[0x6d - 0x18];
    bool FarApart;                                      // +0x6d (descriptive)
    unsigned char _pad6e[0x74 - 0x6e];
    C3DVector CameraFromPos;                            // +0x74 (descriptive)
    C3DVector CameraAtPos;                              // +0x80 (descriptive)
    bool CameraSetUp;                                   // +0x8c (descriptive)
    bool ParentIsDemonDoor;                             // +0x8d
private:
    void SetUpCamera(const CSubtitledMessage::CCameraTag& tag);
    void GetCameraSetupPositions(const CSubtitledMessage::CCameraTag& tag, const CThing*& fromThing, C3DVector& fromPos,
                                 const CThing*& atThing, C3DVector& atPos) const;   // 0x00712340
};
}
extern C3DVector g_CameraTorsoOffset;                   // 0x01380C38 (descriptive)

void NTCScriptedControl::CActionTalkBase::SetUpCamera(const CSubtitledMessage::CCameraTag& tag)
{
    CThing* me = PTCScript->ParentThing;
    CThing* other = PTargetThing.GetPThing();
    bool talking_to_self = me == other;
    if ((other->GetPos() - me->GetPos()).GetMagnitude() > 2.0f)
        FarApart = true;
    const CThing* from_thing = 0;
    C3DVector from_pos(0.0f, 0.0f, 0.0f);
    const CThing* at_thing = 0;
    C3DVector at_pos(0.0f, 0.0f, 0.0f);
    GetCameraSetupPositions(tag, from_thing, from_pos, at_thing, at_pos);
    CameraFromPos = from_pos;
    CameraAtPos = at_pos;
    CameraSetUp = true;
    C3DVector dir;
    if (from_thing && at_thing) {
        if (from_thing != at_thing)
            dir = at_thing->GetPos() - from_thing->GetPos();
        else if (talking_to_self) {
            dir = -from_thing->PhysicsTC->GetFacingVector();
            goto dir_done;
        } else
            dir = me->GetPos() + other->GetPos();
    } else
        dir = from_pos - at_pos;
    dir *= GFFastOneOverSqrt(dir.GetSquaredMagnitude());
dir_done:
    float zoom = ((from_pos - at_pos).GetMagnitude() - 0.4f) * (1.0f / 2.6f);
    if (zoom < 0.0f)
        zoom = 0.0f;
    else if (zoom > 1.0f)
        zoom = 1.0f;
    C3DVector offset(0.0f, 0.0f, -(zoom * zoom));
    C3DVector at_start = at_pos;
    C3DVector delta = at_pos - from_pos;
    switch (tag.CameraFocusPosition) {
    case 1:
        offset.X = 0.0f; offset.Y = 0.0f; offset.Z *= 0.1f;
        at_pos = from_pos + delta * 0.1f;
        break;
    case 2:
        offset.X = 0.0f; offset.Y = 0.0f; offset.Z *= 0.25f;
        at_pos = from_pos + delta * 0.25f;
        break;
    case 3:
        offset.X = 0.0f; offset.Y = 0.0f; offset.Z *= 0.33f;
        at_pos = from_pos + delta * 0.33f;
        break;
    case 4:
        offset.X = 0.0f; offset.Y = 0.0f; offset.Z *= 0.5f;
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
        C3DVector d = at_pos - from_pos;
        at_pos.Z = d.Z * 0.75f + from_pos.Z;
    }
    at_pos += offset;
    long position = tag.CameraPosition;
    if (ParentIsDemonDoor)
        position = 11;
    C3DVector camera_pos;
    switch (position) {
    case 0: camera_pos = from_pos + dir; break;
    case 1: camera_pos = from_pos - C3DVector(dir.Y * 0.4f, -dir.X * 0.4f, 0.0f); break;
    case 2: camera_pos = from_pos + C3DVector(dir.Y * 0.4f, -dir.X * 0.4f, 0.0f); break;
    case 3: camera_pos = from_pos - C3DVector(dir.Y * 0.4f, -dir.X * 0.4f, 0.0f) - dir * 0.5f; break;
    case 4: camera_pos = from_pos + C3DVector(dir.Y * 0.4f, -dir.X * 0.4f, 0.0f) - dir * 0.5f; break;
    case 5: camera_pos = from_pos - C3DVector(dir.Y * 0.4f, -dir.X * 0.4f, 0.0f) - dir * 2.0f; break;
    case 6: camera_pos = from_pos + C3DVector(dir.Y * 0.4f, -dir.X * 0.4f, 0.0f) - dir * 2.0f; break;
    case 7: camera_pos = from_pos - C3DVector(dir.Y, -dir.X, 0.0f) + g_CameraTorsoOffset; break;
    case 8: camera_pos = from_pos + C3DVector(dir.Y, -dir.X, 0.0f) + g_CameraTorsoOffset; break;
    case 9: camera_pos = from_pos - C3DVector(dir.Y * 2.6f, -dir.X * 2.6f, 0.0f) + g_CameraTorsoOffset; break;
    case 10: camera_pos = from_pos + C3DVector(dir.Y * 2.6f, -dir.X * 2.6f, 0.0f) + g_CameraTorsoOffset; break;
    case 11:
    case 12: camera_pos = from_pos; break;
    }
    const CThingSearchTools* tools = CMainGameComponent::PMainGameComponent->SearchHolder->Tools;
    std::vector<CThingCreatureBase*> creatures;
    long y = (long)camera_pos.Y;
    long x = (long)camera_pos.X;
    tools->GetAllThingsInRoughArea(C2DBoxI(x - 2, y - 2, x + 2, y + 2), CThingFilter_None(), creatures);
    for (CThingCreatureBase** it = creatures.begin(); it != creatures.end(); ++it) {
        CThingCreatureBase* creature = *it;
        const C3DVector& pos = creature->GetPos();
        float dy = pos.Y - camera_pos.Y;
        float dx = pos.X - camera_pos.X;
        float radius = creature->PhysicsTC->Radius + 0.2f;
        if (dx * dx + dy * dy < radius * radius) {
            float head = creature->GetHeadPos(true).Z + 0.5f;
            if (head > camera_pos.Z)
                camera_pos.Z = head;
        }
    }
    CCountedPointer<NCameraModes::CScriptModuleBase> module;
    if (tag.CameraZoomType == CONVERSATION_CAMERA_NO_ZOOM) {
        module.Reset(new NCameraModes::CScriptModuleGoToLookAt(camera_pos, at_pos));
    } else {
        C3DVector from_end = at_start - dir * 0.3f;
        long frames = GFFloatToLongNear((float)CMainGameComponent::ConstantFPS * (tag.CameraZoomType == CONVERSATION_CAMERA_SLOW_ZOOM ? 15.0f : 0.8f));
        module.Reset(new NCameraModes::CScriptModuleMoveBetweenLookFromAndLookTo(at_pos, camera_pos, at_start, from_end, frames));
    }
    module->TransitionType = 1;
    module->TransitionTime = 0.2f;
    PTCScript->GetWorld().GetGameCameraManager()->SetGameScriptedCameraMode(module, true);
}
