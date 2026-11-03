// NTCScriptedControl::CActionTalkBase::GetCameraSetupPositions(const CSubtitledMessage::CCameraTag&, const CThing*&, C3DVector&, const CThing*&, C3DVector&) const -- work out the conversation camera's from/at things and positions for a camera tag (talking to self: in front of the head; speaker/listener head points; otherwise the midpoint of the pair, pushed sideways by 1.75, 1 or 0.75 of their separation for camera view 0/1/2, at mid head height; the demon-door special case pulls the camera in front of the speaker)
// Leaf: ?GetCameraSetupPositions@CActionTalkBase@NTCScriptedControl@@ABEXABVCCameraTag@CSubtitledMessage@@AAPBVCThing@@AAVC3DVector@@12@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00712340, 1404 bytes; symbol ?GetCameraSetupPositions@CActionTalkBase@NTCScriptedControl@@ABEXABVCCameraTag@CSubtitledMessage@@AAPBVCThing@@AAVC3DVector@@12@Z (private const).
// Callees: CScriptThing::GetPThing (slot 11, +0x2c), CTCPhysicsBase::GetFacingVector (slot 70, +0x118; 0x006B1410),
// NGraphicAppearance::CTCGraphicAppearance::GetFocalPos 0x004C1060, CThing::GetPos 0x004C73D0,
// C3DVector::GetMagnitudeAndNormalise 0x00A14480. Constants 0.2/0.5/1.75/0.75/1.4/0.45/2.45/2.5 checked against retail .rdata.
// Layout: CCameraTag members are the Ego_r.pdb names (CameraPositionPerson +0, CameraPosition +4, CameraFocusPerson +8,
// CameraView +0x14); CTCScriptedControl::CActionBase::PTCScript +4, CActionTalkBase::PTargetThing +0x0c (CScriptThing,
// retail polymorphic model) and ParentIsDemonDoor read at +0x8d (Ego_r.pdb places it at +0x91: TLC layout is 4 bytes shorter
// before it). CTCBase::ParentThing +4; CThing PhysicsTC +0x60, PTCGraphicAppearance +0x64.
class CCharString;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    float GetMagnitudeAndNormalise();                   // 0x00A14480
};
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
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    C3DVector GetFocalPos(float time) const;            // 0x004C1060
};
}
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    const C3DVector& GetPos() const;                    // 0x004C73D0
};
#include "engine/CTCBase.h"                             // ParentThing +0x04
class CTCScriptedControl : public CTCBase { public:
    class CActionBase { public:
        void* __vftable;                                // +0x00
        CTCScriptedControl* PTCScript;                  // +0x04
        bool Activated;                                 // +0x08
        bool Terminated;                                // +0x09
    };
};
// Enumerators: installed-game misc_def_types.h (ghidra_out/installed_game/header_enums.csv); the CONVERSATION_* prefixes are the
// landed spelling (0x00719A70 batch hand477), descriptive.
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
class CSubtitledMessage { public:
    class CCameraTag { public:
        EConversationProtagonist CameraPositionPerson;  // +0x00
        EConversationCameraPosition CameraPosition;     // +0x04
        EConversationProtagonist CameraFocusPerson;     // +0x08
        EConversationCameraFocus CameraFocusPosition;   // +0x0c
        EConversationCameraZoomType CameraZoomType;     // +0x10
        EConversationCameraView CameraView;             // +0x14
    };
};
namespace NTCScriptedControl {
class CActionTalkBase : public CTCScriptedControl::CActionBase { public:
    CScriptThing PTargetThing;                          // +0x0c
    unsigned char _pad18[0x8d - 0x18];
    bool ParentIsDemonDoor;                             // +0x8d
private:
    void GetCameraSetupPositions(const CSubtitledMessage::CCameraTag& tag, const CThing*& fromThing, C3DVector& fromPos,
                                 const CThing*& atThing, C3DVector& atPos) const;
};
}
extern C3DVector g_CameraHeadOffset;                    // 0x01380C44 (descriptive: no PDB name found)

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
        C3DVector forward = fromThing->PhysicsTC->GetFacingVector();
        NGraphicAppearance::CTCGraphicAppearance* app = fromThing->PTCGraphicAppearance;
        C3DVector head = app->GetFocalPos(1.0f);
        fromPos = forward * 2.0f + head;
        fromPos.Z += 0.2f;
    } else if (tag.CameraPositionPerson == CONVERSATION_PROTAGONIST_SPEAKER) {
        fromThing = PTCScript->ParentThing;
        {
            NGraphicAppearance::CTCGraphicAppearance* app = fromThing->PTCGraphicAppearance;
            fromPos = app->GetFocalPos(1.0f);
        }
    } else if (tag.CameraPositionPerson == CONVERSATION_PROTAGONIST_SPEAKEE) {
        fromThing = PTargetThing.GetPThing();
        {
            NGraphicAppearance::CTCGraphicAppearance* app = fromThing->PTCGraphicAppearance;
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
        NGraphicAppearance::CTCGraphicAppearance* appA = first->PTCGraphicAppearance;
        float z = appA->GetFocalPos(1.0f).Z;
        NGraphicAppearance::CTCGraphicAppearance* appB = second->PTCGraphicAppearance;
        fromPos.Z = (appB->GetFocalPos(1.0f).Z - z) * 0.5f + z;
    }
    atThing = 0;
    if (tag.CameraFocusPerson == CONVERSATION_PROTAGONIST_SPEAKER) {
        atThing = PTCScript->ParentThing;
        {
            NGraphicAppearance::CTCGraphicAppearance* app = atThing->PTCGraphicAppearance;
            atPos = g_CameraHeadOffset + app->GetFocalPos(1.0f);
        }
    } else if (tag.CameraFocusPerson == CONVERSATION_PROTAGONIST_SPEAKEE) {
        atThing = PTargetThing.GetPThing();
        {
            NGraphicAppearance::CTCGraphicAppearance* app = atThing->PTCGraphicAppearance;
            atPos = g_CameraHeadOffset + app->GetFocalPos(1.0f);
        }
    } else {
        C3DVector half = (second->GetPos() - first->GetPos()) * 0.5f;
        atPos = half + first->GetPos();
        NGraphicAppearance::CTCGraphicAppearance* appA = first->PTCGraphicAppearance;
        float z = appA->GetFocalPos(1.0f).Z;
        NGraphicAppearance::CTCGraphicAppearance* appB = second->PTCGraphicAppearance;
        atPos.Z = (appB->GetFocalPos(1.0f).Z - z) * 0.5f + z;
    }
    if (ParentIsDemonDoor) {
        atPos.Z += 1.4f;
        C3DVector d = fromPos - atPos;
        float length = d.GetMagnitudeAndNormalise();
        C3DVector forward = me->PhysicsTC->GetFacingVector();
        if (d.Y * forward.Y + d.Z * forward.Z + forward.X * d.X < 0.45f || length < 2.45f) {
            fromPos = atPos;
            fromPos += forward * 2.5f;
        }
    }
}