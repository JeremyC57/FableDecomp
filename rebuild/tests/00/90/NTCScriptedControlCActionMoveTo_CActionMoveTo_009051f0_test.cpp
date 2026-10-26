#include <cstdio>

class CThing;
class CScriptThing { public:
    CThing* PImp_Data; void* PImp_Info;
    CScriptThing() : PImp_Data(0), PImp_Info(0) {}
    CScriptThing(CThing* t) : PImp_Data(t), PImp_Info(0) {}
};
class CThing { public: CScriptThing GetScriptThing() { return CScriptThing(this); } };
class C3DVector { public: float X, Y, Z; };

struct CActionBase {
    void* PTCScript; bool Activated; bool Terminated;
    CActionBase() : PTCScript(0), Activated(false), Terminated(false) {}
};

struct CActionMoveTo : CActionBase {
    C3DVector Pos; long MovementType; bool FaceMovement; float Proximity; CScriptThing PTargetThing; bool UseTargetThing;
    void* PTCOldPhysics; CScriptThing PRouteThing; bool UseRouteThing; bool AvoidDynamicObstacles;
    bool PreviousPushableByHero; bool UseActionPoint; long ActionPointIndex; bool GotActionPointIndex;
    bool UseBackupProximityForActionPointIfRouteBlocked; bool CancelAllActionsOnFailure; bool IgnorePathPreferability;
    CActionMoveTo(CThing* target, long movementType, float proximity, bool faceMovement,
                  bool avoidDynamicObstacles, bool useActionPoint, bool useBackupProximity,
                  bool cancelAllActionsOnFailure, bool ignorePathPreferability)
        : CActionBase(), MovementType(movementType), FaceMovement(faceMovement), Proximity(proximity),
          PTargetThing(target->GetScriptThing()), UseTargetThing(true), PTCOldPhysics(0), PRouteThing(),
          UseRouteThing(false), AvoidDynamicObstacles(avoidDynamicObstacles), PreviousPushableByHero(false),
          UseActionPoint(useActionPoint), ActionPointIndex(-1), GotActionPointIndex(false),
          UseBackupProximityForActionPointIfRouteBlocked(useBackupProximity),
          CancelAllActionsOnFailure(cancelAllActionsOnFailure), IgnorePathPreferability(ignorePathPreferability) {}
    CActionMoveTo(CThing* routeThing, long movementType, bool faceMovement, bool avoidDynamicObstacles)
        : CActionBase(), MovementType(movementType), FaceMovement(faceMovement), Proximity(0.0f), PTargetThing(),
          UseTargetThing(false), PTCOldPhysics(0), PRouteThing(routeThing->GetScriptThing()), UseRouteThing(true),
          AvoidDynamicObstacles(avoidDynamicObstacles), PreviousPushableByHero(false), UseActionPoint(false),
          ActionPointIndex(-1), GotActionPointIndex(false), UseBackupProximityForActionPointIfRouteBlocked(false),
          CancelAllActionsOnFailure(false), IgnorePathPreferability(false) {}
};

int main()
{
    CThing t;
    CActionMoveTo a(&t, 2, 1.5f, true, false, true, false, true, false);
    if (a.PTargetThing.PImp_Data != &t || !a.UseTargetThing || a.PRouteThing.PImp_Data || a.UseRouteThing
        || a.MovementType != 2 || a.Proximity != 1.5f || !a.FaceMovement || a.AvoidDynamicObstacles || !a.UseActionPoint
        || a.UseBackupProximityForActionPointIfRouteBlocked || !a.CancelAllActionsOnFailure || a.IgnorePathPreferability
        || a.ActionPointIndex != -1 || a.GotActionPointIndex || a.PreviousPushableByHero || a.PTCOldPhysics) {
        std::printf("009051f0_TEST FAIL\n");
        return 1;
    }
    std::printf("009051f0_TEST PASS\n");
    return 0;
}