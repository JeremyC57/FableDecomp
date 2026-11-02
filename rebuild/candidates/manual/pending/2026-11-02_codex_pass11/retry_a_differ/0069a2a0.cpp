// CGameCameraManager::UpdateTransitions(void) -- select requested script, manual, or automatic camera mode
// Leaf: ?UpdateTransitions@CGameCameraManager@@AAEXXZ
// Grade: DIFFER(1267v1452) -- inline budget/header: retail inlines the concrete camera-mode constructors and counted-pointer conversions; this build calls them; frame 0x20 vs 0x10
// Context: grades against the Codex row context $S/codex11_dl/x/rows/0069a2a0/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = retry11a probe p11; 0069a2a0 probes in probe/0069a2a0.log
#define FABLE_RETAIL_CAMERA_TRANSITIONS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_TYPED_GETTC
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
#include "engine/CThing.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCCameraPointGeneralCase.h"
#include "engine/NCameraModes_CScriptedControl.h"
#include "engine/NCameraModes_CSnapToPoint.h"
#include "engine/NCameraModes_CFixedDolly.h"
#include "engine/NCameraModes_CTrack.h"
#include "engine/NCameraModes_CDead.h"
#include "engine/NCameraModes_CStringAndCage.h"
namespace { extern CDefPointer<const CCameraModeDef> DEFAULT_CAMERA_MODE_DEF; }
void CGameCameraManager::UpdateTransitions() {
 if(!PPendingScriptModule.IsNull()) {
  CCountedPointer<NCameraModes::CBase> mode;
  mode.Reset(new NCameraModes::CScriptedControl(*this,DEFAULT_CAMERA_MODE_DEF,PPendingScriptModule));
  SetCameraMode(mode);return;
 }
 if(!PCurrentScriptModule.IsNull()) return;
 if(!BlockModeChange && !PendingMode.IsNull()) {SetCameraMode(PendingMode);return;}
 if(UsingCameraManually || !NewAutoMode) return;
 NewAutoMode=false;
 if(AvailableCamPoints.begin()!=AvailableCamPoints.end()) {
  {
   const CTCCameraPointDefinitionBase* point;
   AvailableCamPoints.front()->PeekTC(&point);
   if(point->IsFixedPoint()) {
    const CThing& cameraPoint=*AvailableCamPoints.front().Get();
    SetCameraMode(new NCameraModes::CSnapToPoint(*this,DEFAULT_CAMERA_MODE_DEF,cameraPoint));
    return;
   }
  }
  {
   const CTCCameraPointDefinitionBase* point;
   AvailableCamPoints.front()->PeekTC(&point);
   if(point->IsFixedDolly()) {
    const CThing& cameraPoint=*AvailableCamPoints.front().Get();
    SetCameraMode(new NCameraModes::CFixedDolly(*this,DEFAULT_CAMERA_MODE_DEF,cameraPoint));
    return;
   }
  }
  {
   const CTCCameraPointDefinitionBase* point;
   AvailableCamPoints.front()->PeekTC(&point);
   if(point->IsTrack()) {
    const CThing& cameraPoint=*AvailableCamPoints.front().Get();
    SetCameraMode(new NCameraModes::CTrack(*this,DEFAULT_CAMERA_MODE_DEF,cameraPoint));
    return;
   }
  }
  {
   const CTCCameraPointDefinitionBase* point;
   AvailableCamPoints.front()->PeekTC(&point);
   if(point->IsDead()) {
    const CThing& cameraPoint=*AvailableCamPoints.front().Get();
    SetCameraMode(new NCameraModes::CDead(*this,DEFAULT_CAMERA_MODE_DEF,cameraPoint));
    return;
   }
  }
  {
   const CTCCameraPointDefinitionBase* point;
   AvailableCamPoints.front()->PeekTC(&point);
   if(point->IsGeneralCase()) {
    const CTCCameraPointGeneralCase* general;
    AvailableCamPoints.front()->PeekTC(&general);
    CCountedPointer<NCameraModes::CBase> mode;
    mode.Reset(new NCameraModes::CStringAndCage(*this,DEFAULT_CAMERA_MODE_DEF,
     general->GetStringLength(),general->GetCageRadius(),general->GetHeightOffset(),
     general->IsAutoGoBehind(),general->GetAutoGoBehindTime(),general->GetFOV()*(1.0f/360.0f)));
    SetCameraMode(mode);
   }
  }
 } else {
  PushCurrentManualMode();
  UsingCameraManually=false;
  SetCameraMode(PendingMode);
 }
}
