// CGameScriptInterface::CameraUseCameraPoint(const CScriptThing& cameraPoint, const C3DVector* pos, const CRightHandedSet* rhSet, float fov, long startKey, long endKey) const -- script call: when the thing is a live camera point (camera-point TC, interface 0x3F), build its scripted camera mode (a transition between the point's transformed start/end positions, looking along its look directions or at the thing with ThingUID; or a spline between two key cameras), anchor it at pos / rhSet, bind it to the camera point and hand it to the camera manager
// Leaf: ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCScriptThing@@PBVC3DVector@@PBVCRightHandedSet@@MJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089E190, 803 bytes, thiscall `ret 18h`, no EH frame. Identity: retail CGameScriptInterface
//   vtable 0x01260F0C slot 411 (engine/CGameScriptInterface.h); the slot-413 overload 0x00891070 (landed) forwards to it.
// Callees (PDB spellings): CScriptThing::IsAlive / GetPThing (slots 75 / 11), CVectorMap<ETCInterfaceType,CTCBase*>::Find
//   0x0040F020 (inside CThing::GetTC<CTCCameraPointDefinitionBase>, interface 0x3F: the HasTC bit is UsingComponents word 1 bit 31,
//   the `jns`), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, NProgressDisplay::DisplayProgress(const
//   CCharString&, float, bool, bool) 0x009E9F40 (__fastcall), CTCCameraPointDefinitionBase::GetCameraPointType (slot 28, +0x70)
//   inside GetPointType<CTCCameraPointScripted> (type 6: 0x0075EDF0 `mov eax,6`) / <CTCCameraPointScriptedSpline> (type 7:
//   0x0075FBF0), IsRelativeCoordTranslation 0x0075C160 / IsRelativeCoordOrientation 0x0075C170 (results unused), CTCCameraPointScripted::
//   GetStartPos 0x0075CD30 / GetEndPos 0x0075CD90 / GetLookDirection 0x0075CCB0 / GetLookDirectionEnd 0x0075CCF0, __ftol2 (the inline
//   CMainGameComponent::ConvertSecondsToWF over ConstantFPS 0x01375550), CWorld::PeekThingManager (slot 4) + CThingManager::PeekPThingWithUID
//   0x0051ED60, operator new 0x00BFEA1A, NCameraModes::CScriptModuleTransitionBetweenPoints(const C3DVector&, const C3DVector&, const
//   C3DVector&, const C3DVector&, long, float) 0x00852510 / (const CThing&, const C3DVector&, const C3DVector&, long, float) 0x00852D90,
//   NCameraModes::CScriptModuleSpline(long, long) 0x00852710, CCountedPointer<CScriptModuleBase>::Reset 0x0069C3D0 (the engine header's
//   inline Reset, emitted out of line), CScriptModuleBase::SetCoordAxis 0x00851D30 / SetPCameraPoint 0x0072A4A0, CAIGameCameraBase::
//   GetCameraManager (slot 9), CGameCameraManager::SetScriptedCameraMode 0x00699E60 (the counted-pointer copy built in place),
//   ~CCountedPointer<CScriptModuleBase> 0x005ED2C0.
// Names: Ego_r.pdb ??0CScriptModuleTransitionBetweenPoints@NCameraModes@@QAE@ABVC3DVector@@000JM@Z / @ABVCThing@@ABVC3DVector@@1JM@Z
//   (argument shapes 4 vectors + long + float, thing + 2 vectors + long + float; reloc_bindings has the second as an invented
//   CScriptModuleMoveBetweenLookingAt, NAME_FIXES), ?IsRelativeCoordTranslation / ?IsRelativeCoordOrientation@CTCCameraPointDefinitionBase@@QBE_NXZ
//   (Ego_r 0x0076AB80 / 0x0076AB90, PDB UsingRelativeCoords +0x3A / UsingRelativeOrientation +0x3B = the bytes those 9-byte bodies test;
//   landed under the placeholder CASuspendableProcess::CheckTermination, NAME_FIXES), ?GetLookDirection / ?GetLookDirectionEnd@CTCCameraPointScripted@@QBE?AVC3DVector@@PBVCRightHandedSet@@@Z
//   (LookPos +0x58 / LookPosEnd +0x64; the second landed as CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedScreenFilterParameters).
//   The ECameraPointType enumerator names and the parameter names are not in the name sets (descriptive).
// Layout: CTCCameraPointDefinitionBase / CTCCameraPointScripted / NCameraModes::CScriptModuleBase are the Ego_r.pdb members (no STL
//   containers, so the offsets hold in retail: FOV +0x10, ThingUID +0x70, TransitionTime +0x7C; CoordOrigin +0x14, SelfTerminate +0x40).
//   No engine header exists for them (header debt): they are declared here.
// Shape: the TC-map key and the manager pointer live in dead argument slots; the two start/end sums are `start + look`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGMANAGER_PEEK
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WF
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "fable_game.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40 ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z
}
enum ECameraPointType { CAMERA_POINT_SCRIPTED = 6, CAMERA_POINT_SCRIPTED_SPLINE = 7 };   // enumerator names descriptive
class CTCCameraPointDefinitionBase : public CTCBase {
public:
    bool CutInto;                                   // +0x0c
    bool CutOutOf;                                  // +0x0d
    bool TestAngleBeforeActivation;                 // +0x0e
    float FOV;                                      // +0x10
    bool SelfTerminate;                             // +0x14
    bool HeroIsSubject;                             // +0x15
    unsigned char CoordBase[0xc];                   // +0x16 C3DVector (byte-aligned in Ego_r.pdb)
    unsigned char CoordAxis[0x18];                  // +0x22 CRightHandedSet
    bool UsingRelativeCoords;                       // +0x3a
    bool UsingRelativeOrientation;                  // +0x3b
    bool Activated;                                 // +0x3c
    virtual ECameraPointType GetCameraPointType() const;   // slot 28 (+0x70)
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x3f; }
    bool IsRelativeCoordTranslation() const;        // 0x0075C160
    bool IsRelativeCoordOrientation() const;        // 0x0075C170
    template <class T> bool GetPointType(T** out)   // Ego_r ??$GetPointType@V..@@@CTCCameraPointDefinitionBase@@QAE_NPAPAV..@@@Z
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
    C3DVector StartPos;                             // +0x40
    C3DVector EndPos;                               // +0x4c
    C3DVector LookPos;                              // +0x58
    C3DVector LookPosEnd;                           // +0x64
    unsigned __int64 ThingUID;                      // +0x70
    long PAT;                                       // +0x78 EEditorPointAcquisionType
    float TransitionTime;                           // +0x7c
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
class CScriptModuleTransitionBetweenPoints : public CScriptModuleBase {   // 0x90 bytes (operator new 0x90)
public:
    unsigned char _body4c[0x90 - 0x4c];
    CScriptModuleTransitionBetweenPoints(const C3DVector& lookFrom, const C3DVector& lookTo, const C3DVector& from, const C3DVector& to, long frames, float fov);   // 0x00852510
    CScriptModuleTransitionBetweenPoints(const CThing& lookAt, const C3DVector& from, const C3DVector& to, long frames, float fov);   // 0x00852D90
};
class CScriptModuleSpline : public CScriptModuleBase {                    // 0x64 bytes (Ego_r.pdb sizeof 100)
public:
    unsigned char _body4c[0x64 - 0x4c];
    CScriptModuleSpline(long startKey, long endKey);   // 0x00852710
};
}

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
            long frames = CMainGameComponent::ConvertSecondsToWF(scripted->TransitionTime);
            float fovFraction = scripted->FOV * (1.0f / 360.0f);
            if (fov > 0.0f && fov < 180.0f)
                fovFraction = fov * (1.0f / 360.0f);
            if (uid == 0) {
                C3DVector lookDir = scripted->GetLookDirection(rhSet);
                C3DVector lookDirEnd = scripted->GetLookDirectionEnd(rhSet);
                mode.Reset(new NCameraModes::CScriptModuleTransitionBetweenPoints(startPos + lookDir, endPos + lookDirEnd, startPos, endPos, frames, fovFraction));
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
