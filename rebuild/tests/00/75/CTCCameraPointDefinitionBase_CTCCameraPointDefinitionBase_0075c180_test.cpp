// Behaviour test for CTCCameraPointDefinitionBase's constructor @ 0x0075C180: FOV 72, self-terminating, identity axes.
#include <cstdio>
struct CThing { long id; };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CRightHandedSet { C3DVector Up, Forward; CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(1.0f, 0.0f, 0.0f) {} };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* owner) : ParentThing(owner) {} virtual ~CTCBase() {} };
struct CTCCameraPointDefinitionBase : CTCBase {
    bool CutInto, CutOutOf, TestAngleBeforeActivation;
    float FOV;
    bool SelfTerminate, HeroIsSubject;
    C3DVector CoordBase;
    CRightHandedSet CoordAxis;
    bool UsingRelativeCoords, UsingRelativeOrientation, Activated;
    CTCCameraPointDefinitionBase(CThing* owner)
        : CTCBase(owner), CutInto(false), CutOutOf(false), TestAngleBeforeActivation(false),
          SelfTerminate(true), HeroIsSubject(false), CoordBase(0.0f, 0.0f, 0.0f),
          UsingRelativeCoords(false), UsingRelativeOrientation(false), Activated(false)
    { FOV = 72.0f; }
};
int main()
{
    CThing t; t.id = 3;
    CTCCameraPointDefinitionBase c(&t);
    if (c.FOV != 72.0f || !c.SelfTerminate || c.CutInto || c.CutOutOf || c.TestAngleBeforeActivation || c.HeroIsSubject ||
        c.CoordBase.X != 0.0f || c.CoordAxis.Up.Z != 1.0f || c.CoordAxis.Up.X != 0.0f || c.CoordAxis.Forward.X != 1.0f ||
        c.CoordAxis.Forward.Z != 0.0f || c.UsingRelativeCoords || c.UsingRelativeOrientation || c.Activated)
    { std::printf("0075c180_TEST FAIL\n"); return 1; }
    std::printf("0075c180_TEST PASS\n");
    return 0;
}