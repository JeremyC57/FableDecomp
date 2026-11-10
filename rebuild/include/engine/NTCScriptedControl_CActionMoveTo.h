#ifndef FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONMOVETO_H
#define FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONMOVETO_H
// Retail opt-in (Codex pass18): Ego_r TPI0x2246b, FableWin constructor and native00905290, size0x50.
#include "engine/CTCScriptedControl.h"
#include "engine/CScriptThing.h"
#include "engine/EMovementType.h"
#include "engine/C3DVector.h"
namespace NTCScriptedControl {
class CActionMoveTo:public CTCScriptedControl::CActionBase {
public:
 CActionMoveTo(const CThing& route,EMovementType movement_type,bool face_movement,bool avoid_dynamic_obstacles);
 virtual void Update();
 virtual CCharString GetActionName()const;
private:
 C3DVector Pos;
 EMovementType MovementType;
 bool FaceMovement;
 float Proximity;
 CScriptThing PTargetThing;
 bool UseTargetThing;
 CTCBase* PTCOldPhysics;
 CScriptThing PRouteThing;
 bool UseRouteThing;
 bool AvoidDynamicObstacles;
 bool PreviousPushableByHero;
 bool UseActionPoint;
 long ActionPointIndex;
 bool GotActionPointIndex;
 bool UseBackupProximityForActionPointIfRouteBlocked;
 bool CancelAllActionsOnFailure;
 bool IgnorePathPreferability;
};
}
FABLE_STATIC_ASSERT(sizeof(NTCScriptedControl::CActionMoveTo)==0x50);
#endif
