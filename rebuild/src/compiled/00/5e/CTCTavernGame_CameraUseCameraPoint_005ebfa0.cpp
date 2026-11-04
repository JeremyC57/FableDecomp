// CTCTavernGame::CameraUseCameraPoint(const CCharString& name) -- find the world things with this script name; if the first is a live camera point, hand the camera manager a tavern-game script module that goes to the point's start position looking along its look direction
// Leaf: ?CameraUseCameraPoint@CTCTavernGame@@QAEXABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EBFA0, 327 bytes, thiscall `ret 4`. Ego_r.pdb / FableWin.pdb
//   ?CameraUseCameraPoint@CTCTavernGame@@QAEXABVCCharString@@@Z (public); the landed CTCTavernGameShoveHaPenny::EnterGame 0x007CF530 calls it.
// Callees (landed spellings): CCharString(const CCharString&) 0x0099EC30 (the filter's Name, built in the dead name argument slot),
//   CTCBase::PeekWorld 0x00686A80 then CWorld::PeekThingSearchTools (inline, [world+0x20]), CThingSearchTools::GetAllThings<CThing,
//   vector<CThing*>, CThingFilter_HasScriptName> 0x005ED860, ~CCharString 0x0099EAE0, free 0x00BFEA14 (the vector's storage),
//   CVectorMap<ETCInterfaceType,CTCBase*>::Find 0x0040F020 inside CThing::GetTC<CTCCameraPointDefinitionBase> (interface 0x3F: HasTC
//   is UsingComponents word 1 bit 31, the `jns`), GetPointType<CTCCameraPointScripted> (slot 28 GetCameraPointType == 6, result
//   unused: the point pointer stays uninitialised otherwise), CTCBase::GetWorld 0x00686A70 then CWorld::GetGameCamera (inline, +0x34)
//   and CAIGameCameraBase::GetCameraManager (slot 9), operator new 0x00BFEA1A (0x64), CTCCameraPointScripted::GetLookDirection
//   0x0075CCB0 / GetStartPos 0x0075CD30, NCameraModes::CScriptModuleTavernGame(const C3DVector&, const C3DVector&) 0x005ED240,
//   CCountedPointer<NCameraModes::CScriptModuleBase>(CScriptModuleBase*) 0x005ED270 (built in place as the by-value argument),
//   CGameCameraManager::SetScriptedCameraMode 0x00699E60.
// Model: the camera-point classes as the landed CGameScriptInterface::CameraUseCameraPoint 0x0089E190 (Ego_r.pdb members; no engine
//   header). The retail CThingSearchTools GetAllThings body (header opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY, the landed
//   0x005ED860 shape, the same TU in retail) is a noinline body: VC7.1 then knows the result vector does not escape, so the success
//   path frees the storage it re-reads without a null test, as retail does.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_CAMERAPOINT
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCTavernGame.h"

class CThingFilter_HasScriptName {
public:
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) { }
    bool operator()(const CThing& thing) const;    // 0x006A7E30
};

enum ECameraPointType { CAMERA_POINT_SCRIPTED = 6 };   // enumerator name descriptive
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
    C3DVector GetLookDirection(const CRightHandedSet* axes) const;      // 0x0075CCB0
    C3DVector GetStartPos(const C3DVector* origin, const CRightHandedSet* axes) const;   // 0x0075CD30
};
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleGoToLookInDirection : public CScriptModuleBase {   // Ego_r.pdb GoTo +0x4C, LookDirection +0x58
public:
    unsigned char _body4[0x64 - 0x4];
    CScriptModuleGoToLookInDirection(const C3DVector& goTo, const C3DVector& lookDirection);   // 0x005ED190
};
class CScriptModuleTavernGame : public CScriptModuleGoToLookInDirection {   // 0x64 bytes (operator new 0x64), vtable 0x01255FC4
public:
    CScriptModuleTavernGame(const C3DVector& goTo, const C3DVector& lookDirection);   // 0x005ED240
};
}

void CTCTavernGame::CameraUseCameraPoint(const CCharString& name)
{
    std::vector<CThing*> things;
    PeekWorld().PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_HasScriptName(name), things);
    if (things.empty())
        return;
    CThing* thing = things[0];
    CTCCameraPointDefinitionBase* point;
    if (thing && !thing->IsDead() && thing->GetTC(&point)) {
        CTCCameraPointScripted* scripted;
        point->GetPointType(&scripted);
        CGameCameraManager* manager;
        GetWorld().GetGameCamera()->GetCameraManager(&manager);
        manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
            new NCameraModes::CScriptModuleTavernGame(scripted->GetStartPos(0, 0), scripted->GetLookDirection(0))));
    }
}