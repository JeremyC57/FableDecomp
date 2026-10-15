#include <cstdio>

struct CThing;
struct CScriptThing {
    CThing* PImp_Data; void* PImp_Info;
    CScriptThing() : PImp_Data(0), PImp_Info(0) {}
    CScriptThing(CThing* t) : PImp_Data(t), PImp_Info(0) {}
};
struct CThing { CScriptThing GetScriptThing() { return CScriptThing(this); } };
struct C3DVector { float X, Y, Z; };

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
    CActionMoveTo a(&t, 3, false, true);
    if (a.PRouteThing.PImp_Data != &t || !a.UseRouteThing || a.PTargetThing.PImp_Data || a.UseTargetThing
        || a.MovementType != 3 || a.Proximity != 0.0f || a.FaceMovement || !a.AvoidDynamicObstacles || a.UseActionPoint
        || a.ActionPointIndex != -1 || a.CancelAllActionsOnFailure || a.IgnorePathPreferability) {
        std::printf("00905290_TEST FAIL\n");
        return 1;
    }
    std::printf("00905290_TEST PASS\n");
    return 0;
}
