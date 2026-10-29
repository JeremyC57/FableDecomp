// Behaviour test for 007e71d0: body copied from src/007e71d0.cpp (CActionMoveTo ctor), the two root ctors stubbed.
#include <new>
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_base = 0, g_scriptBase = 0;
struct CBase { virtual void v00() {} CBase() { ++g_base; } };
class CBaseClass {
public:  virtual void v00() {} CBaseClass() { ++g_scriptBase; } };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
typedef long EMovementType;
class CTCBase;
template <class T> class CCountedPointer { public: T* Object; void* Count; CCountedPointer() : Object(0), Count(0) { } };
class CScriptThing : public CBaseClass { public:
    virtual void v00() {}
    CCountedPointer<CScriptThing> PImp;
    CScriptThing() : CBaseClass(), PImp() { }
};
struct __declspec(novtable) CAction : CBase {
    void* PTCScript; bool Activated; bool Terminated;
    CAction() : CBase(), PTCScript(0), Activated(false), Terminated(false) { }
};
struct CActionMoveTo : CAction {
    C3DVector     Pos;
    EMovementType MovementType;
    bool          FaceMovement;
    float         Proximity;
    CScriptThing  PTargetThing;
    bool          UseTargetThing;
    CTCBase*      PTCOldPhysics;
    CScriptThing  PRouteThing;
    bool          UseRouteThing, AvoidDynamicObstacles, PreviousPushableByHero, UseActionPoint;
    long          ActionPointIndex;
    bool          GotActionPointIndex, UseBackupProximityForActionPointIfRouteBlocked, CancelAllActionsOnFailure, IgnorePathPreferability;
    virtual void v00() {}
    CActionMoveTo(const C3DVector& pos, EMovementType movementType, float proximity, bool faceMovement, bool avoidDynamicObstacles);
};
CActionMoveTo::CActionMoveTo(const C3DVector& pos, EMovementType movementType, float proximity, bool faceMovement,
                             bool avoidDynamicObstacles)
    : CAction()
    , Pos(pos)
    , MovementType(movementType)
    , FaceMovement(faceMovement)
    , Proximity(proximity)
    , PTargetThing()
    , UseTargetThing(false)
    , PTCOldPhysics(0)
    , PRouteThing()
    , UseRouteThing(false)
    , AvoidDynamicObstacles(avoidDynamicObstacles)
    , PreviousPushableByHero(false)
    , UseActionPoint(false)
    , ActionPointIndex(-1)
    , GotActionPointIndex(false)
    , UseBackupProximityForActionPointIfRouteBlocked(false)
    , CancelAllActionsOnFailure(false)
    , IgnorePathPreferability(false)
{
}
int main()
{
    unsigned char raw[sizeof(CActionMoveTo)]; memset(raw, 0xAA, sizeof(raw));
    C3DVector pos(1.5f, -2.0f, 3.25f);
    CActionMoveTo* a = new (raw) CActionMoveTo(pos, 7, 0.75f, true, true);
    CHECK(sizeof(CActionMoveTo) == 0x50 && sizeof(CScriptThing) == 12);
    CHECK(g_base == 1 && g_scriptBase == 2);
    CHECK(a->PTCScript == 0 && !a->Activated && !a->Terminated);
    CHECK(a->Pos.X == 1.5f && a->Pos.Y == -2.0f && a->Pos.Z == 3.25f);
    CHECK(a->MovementType == 7 && a->FaceMovement == true && a->Proximity == 0.75f);
    CHECK(a->PTargetThing.PImp.Object == 0 && a->PTargetThing.PImp.Count == 0 && !a->UseTargetThing && a->PTCOldPhysics == 0);
    CHECK(a->PRouteThing.PImp.Object == 0 && a->PRouteThing.PImp.Count == 0 && !a->UseRouteThing);
    CHECK(a->AvoidDynamicObstacles == true && !a->PreviousPushableByHero && !a->UseActionPoint && a->ActionPointIndex == -1);
    CHECK(!a->GotActionPointIndex && !a->UseBackupProximityForActionPointIfRouteBlocked && !a->CancelAllActionsOnFailure && !a->IgnorePathPreferability);
    CActionMoveTo* b = new (raw) CActionMoveTo(pos, 1, 0.0f, false, false);
    CHECK(b->FaceMovement == false && b->AvoidDynamicObstacles == false && b->MovementType == 1);
    if (fails == 0) printf("007e71d0_TEST PASS\n"); else printf("007e71d0_TEST FAIL\n");
    return fails;
}