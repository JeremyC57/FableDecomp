// CGameScriptInterface::CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, long, long) const -- draft
// Leaf: ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCScriptThing@@PBVC3DVector@@PBVCRightHandedSet@@MJJ@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CAIGAMECAMERABASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
class CMainGameComponent2 {
public:
    static long ConstantFPS;                                       // 0x01375550
    static long __fastcall GetConstantFPS() { return ConstantFPS; }
    static long __fastcall ConvertSecondsToWF(float seconds) { return (long)(GetConstantFPS() * seconds); }
};
class CThingManager {
public:
    const CThing* PeekPThingWithUID(unsigned __int64 uid) const;   // 0x0051ED60
};
enum ECameraPointType { CAMERA_POINT_SCRIPTED = 6, CAMERA_POINT_SCRIPTED_SPLINE = 7 };
class CTCCameraPointDefinitionBase : public CTCBase {
public:
    bool CutInto;                       // +0x0c
    bool CutOutOf;                      // +0x0d
    bool TestAngleBeforeActivation;     // +0x0e
    float FOV;                          // +0x10
    bool SelfTerminate;                 // +0x14
    bool HeroIsSubject;                 // +0x15
    unsigned char CoordBase[0xc];       // +0x16
    unsigned char CoordAxis[0x18];      // +0x22
    bool UsingRelativeCoords;           // +0x3a
    bool UsingRelativeOrientation;      // +0x3b
    bool Activated;                     // +0x3c
    virtual ECameraPointType GetCameraPointType() const;   // slot 28 (+0x70)
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x3f; }
    bool IsRelativeCoordTranslation() const;    // 0x0075C160
    bool IsRelativeCoordOrientation() const;    // 0x0075C170
    template <class T> bool GetPointType(T** out)
    {
        if (GetCameraPointType() == T::StatGetCameraPointType()) {
            *out = static_cast<T*>(this);
            return true;
        }
        return false;
    }
};
class CTCCameraPointScripted : public CTCCameraPointDefinitionBase {
public:
    static ECameraPointType __fastcall StatGetCameraPointType() { return CAMERA_POINT_SCRIPTED; }
    C3DVector StartPos;                 // +0x40
    C3DVector EndPos;                   // +0x4c
    C3DVector LookPos;                  // +0x58
    C3DVector LookPosEnd;               // +0x64
    unsigned __int64 ThingUID;          // +0x70
    long PAT;                           // +0x78
    float TransitionTime;               // +0x7c
    C3DVector GetLookDirection(const CRightHandedSet* axes) const;      // 0x0075CCB0
    C3DVector GetLookDirectionEnd(const CRightHandedSet* axes) const;   // 0x0075CCF0
    C3DVector GetStartPos(const C3DVector* origin, const CRightHandedSet* axes) const;   // 0x0075CD30
    C3DVector GetEndPos(const C3DVector* origin, const CRightHandedSet* axes) const;     // 0x0075CD90
};
class CTCCameraPointScriptedSpline : public CTCCameraPointDefinitionBase {
public:
    static ECameraPointType __fastcall StatGetCameraPointType() { return CAMERA_POINT_SCRIPTED_SPLINE; }
};
namespace NCameraModes {
class CBase;
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
    CIntelligentPointer<CThing> PCamPoint;          // +0x04
    CIntelligentPointer<const CThing> PThingOrigin; // +0x0c
    C3DVector CoordOrigin;                          // +0x14
    CRightHandedSet CoordAxis;                      // +0x20
    CBase* POwner;                                  // +0x38
    CGameCameraManager* PCameraManager;             // +0x3c
    bool SelfTerminate;                             // +0x40
    bool ShortenOnCollision;                        // +0x41
    long CutOutOverride;                            // +0x44
    float FOVOverride;                              // +0x48
    void SetCoordAxis(const CRightHandedSet& axis); // 0x00851D30
    void SetPCameraPoint(CThing& point);            // 0x0072A4A0
};
class CScriptModuleTransitionBetweenPoints : public CScriptModuleBase {
public:
    unsigned char _body[0x90 - 0x4c];
    CScriptModuleTransitionBetweenPoints(const C3DVector& lookFrom, const C3DVector& lookTo, const C3DVector& from, const C3DVector& to, long frames, float fov);   // 0x00852510
    CScriptModuleTransitionBetweenPoints(const CThing& lookAt, const C3DVector& from, const C3DVector& to, long frames, float fov);   // 0x00852D90
};
class CScriptModuleSpline : public CScriptModuleBase {
public:
    unsigned char _body[0x64 - 0x4c];
    CScriptModuleSpline(long startKey, long endKey);   // 0x00852710
};
}
class CGameCameraManager {
public:
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode);   // 0x00699E60
};

void CGameScriptInterface::CameraUseCameraPoint(const CScriptThing& cameraPoint, const C3DVector* pos, const CRightHandedSet* rhSet, float fov, long startKey, long endKey) const
{
    if (!cameraPoint.IsAlive())
        return;
    CThing* thing = cameraPoint.GetPThing();
    CTCCameraPointDefinitionBase* point;
    if (thing && thing->GetTC(&point)) {
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        CCountedPointer<NCameraModes::CScriptModuleBase> mode;
        CTCCameraPointScripted* scripted;
        CTCCameraPointScriptedSpline* spline;
        if (point->GetPointType(&scripted)) {
            scripted->IsRelativeCoordTranslation();
            scripted->IsRelativeCoordOrientation();
            unsigned __int64 uid = scripted->ThingUID;
            C3DVector startPos = scripted->GetStartPos(pos, rhSet);
            C3DVector endPos = scripted->GetEndPos(pos, rhSet);
            long frames = CMainGameComponent2::ConvertSecondsToWF(scripted->TransitionTime);
            float fovFraction = scripted->FOV * (1.0f / 360.0f);
            if (fov > 0.0f && fov < 180.0f)
                fovFraction = fov * (1.0f / 360.0f);
            if (uid == 0) {
                C3DVector lookDir = scripted->GetLookDirection(rhSet);
                C3DVector lookDirEnd = scripted->GetLookDirectionEnd(rhSet);
                mode.Reset(new NCameraModes::CScriptModuleTransitionBetweenPoints(lookDir + startPos, lookDirEnd + endPos, startPos, endPos, frames, fovFraction));
            } else {
                const CThing* target = World->PeekThingManager().PeekPThingWithUID(uid);
                mode.Reset(new NCameraModes::CScriptModuleTransitionBetweenPoints(*target, startPos, endPos, frames, fovFraction));
            }
        } else if (point->GetPointType(&spline)) {
            mode.Reset(new NCameraModes::CScriptModuleSpline(startKey, endKey));
        }
        if (mode.Get()) {
            mode->SelfTerminate = true;
            if (pos)
                mode->CoordOrigin = *pos;
            if (rhSet)
                mode->SetCoordAxis(*rhSet);
            mode->SetPCameraPoint(*thing);
            CGameCameraManager* manager;
            World->GetGameCamera()->GetCameraManager(&manager);
            manager->SetScriptedCameraMode(mode);
        }
    }
}
