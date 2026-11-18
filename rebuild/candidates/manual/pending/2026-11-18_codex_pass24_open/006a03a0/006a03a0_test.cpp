#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>

typedef std::string CCharString;
const CCharString ACTION_POINT("Use");
struct C2DCoordI;
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    C2DCoordI ToC2DCoordI() const;
};
struct C2DVector {
    float X,Y;
    C2DVector(float x,float y):X(x),Y(y) {}
    C2DVector(const C3DVector& v):X(v.X),Y(v.Y) {}
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X-v.X,Y-v.Y); }
    float GetSquaredMagnitude() const { return X*X+Y*Y; }
};
struct C2DCoordI {
    long X,Y;
    C2DCoordI(long x,long y):X(x),Y(y) {}
    explicit C2DCoordI(const C3DVector& v):X((long)v.X),Y((long)v.Y) {}
};
C2DCoordI C3DVector::ToC2DCoordI() const { return C2DCoordI(*this); }
float GFGet2DDistance(const C2DCoordI& a,const C2DCoordI& b)
{
    return (float)sqrt(double((a.X-b.X)*(a.X-b.X)+(a.Y-b.Y)*(a.Y-b.Y)));
}
struct CRightHandedSet { int Turn; };
namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    bool HasAnimations,Invalid;
    std::vector<long> Variants;
    CTCGraphicAppearance():HasAnimations(true),Invalid(false) {}
    void GetDummyObjectNamesStartingWith(std::vector<CCharString>& out,const CCharString& prefix)
    {
        Invalid=prefix!="Use";
        if(HasAnimations) {
            out.push_back("B");out.push_back("A");out.push_back("A");
        }
    }
    bool GetDummyObject(const CCharString& name,long variant,unsigned long selected,C3DVector& pos,CRightHandedSet& orientation,float scale)
    {
        Variants.push_back(variant);
        Invalid=Invalid||selected!=0||scale!=1||(name!="A"&&name!="B");
        pos=(name=="B")?C3DVector(3,1,1):(variant?C3DVector(1,1,1):C3DVector(5,1,1));
        orientation.Turn=(int)variant;return true;
    }
};
}
struct CPhysics {
    unsigned Approaches;
    CPhysics():Approaches(0) {}
    void GetBoundingInfo(C3DVector& pos,float& radius) { ++Approaches; pos=C3DVector(5,5,10); radius=3; }
};
struct CThing {
    C3DVector Position;
    NGraphicAppearance::CTCGraphicAppearance* PGraphicAppearance;
    CPhysics* PPhysics;
    CThing():PGraphicAppearance(0),PPhysics(0) {}
    const C3DVector& GetPos() const { return Position; }
    static float GetPersonalSpaceRadius() { return 1; }
    CPhysics* GetPhysicsTC() const { return PPhysics; }
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return PGraphicAppearance; }
};
struct CBaseIntelligentPointer {
    CThing* Item;
    CThing* operator->() const { return Item; }
};
struct CCreatureActionBase {
    CBaseIntelligentPointer PCreature;
    int TotalTime,ActionTime;
    unsigned Initialises;
    CCreatureActionBase():TotalTime(1),ActionTime(1),Initialises(0) {}
    void Initialise() { ++Initialises; }

};
struct CCreatureAction_UseObject : CCreatureActionBase {
    CBaseIntelligentPointer PUsedThing;
    CRightHandedSet Dummy;
    C3DVector DummyPos;
    float Proximity;
    int PreviousPhysicsComponent;
    bool UsingDummy,MovingToDummy,TurningToDummy,AllowTurn;
    CCreatureAction_UseObject(CThing& creature,CThing& object)
        :Proximity(0),PreviousPhysicsComponent(33),UsingDummy(false),MovingToDummy(true),TurningToDummy(true),AllowTurn(true)
    { PCreature.Item=&creature;PUsedThing.Item=&object; }
    void Initialise();
};

void CCreatureAction_UseObject::Initialise()
{
    CCreatureActionBase::Initialise();
    TotalTime = -9;
    ActionTime = -9;
    std::vector<CCharString> dummy_object_names;
    NGraphicAppearance::CTCGraphicAppearance* ptc_graph = PUsedThing->GetTCGraphicAppearance();
    bool init_success = false;
    if (ptc_graph) {
        ptc_graph->GetDummyObjectNamesStartingWith(dummy_object_names, ACTION_POINT);
        std::sort(dummy_object_names.begin(), dummy_object_names.end());
        if (!dummy_object_names.empty()) {
            long index = 0;
            ptc_graph->GetDummyObject(dummy_object_names[0], index, 0, DummyPos, Dummy, 1.0f);
            for (long c_name = 1; c_name < static_cast<long>(dummy_object_names.size()); ++c_name) {
                if (dummy_object_names[c_name] == dummy_object_names[c_name - 1])
                    ++index;
                else
                    index = 0;
                C3DVector pos;
                CRightHandedSet rhset;
                ptc_graph->GetDummyObject(dummy_object_names[c_name], index, 0, pos, rhset, 1.0f);
                if ((C2DVector(pos) - C2DVector(PCreature->GetPos())).GetSquaredMagnitude() <
                    (C2DVector(DummyPos) - C2DVector(PCreature->GetPos())).GetSquaredMagnitude()) {
                    DummyPos = pos;
                    Dummy = rhset;
                }
            }
            init_success = true;
        }
    }
    if (!init_success) {
        PUsedThing->GetPhysicsTC()->GetBoundingInfo(DummyPos, Proximity);
        Proximity += PCreature->GetPersonalSpaceRadius() * 2.0f;
    }
    UsingDummy = true;
    if (GFGet2DDistance(DummyPos.ToC2DCoordI(), PCreature->GetPos().ToC2DCoordI()) <
        PCreature->GetPersonalSpaceRadius() * 2.0f) {
        if (fabs(DummyPos.Z - PCreature->GetPos().Z) < 2.5f) {
            MovingToDummy = false;
            if (Proximity == 0.0f)
                TurningToDummy = AllowTurn;
        } else {
            MovingToDummy = true;
            TurningToDummy = false;
        }
    } else {
        MovingToDummy = true;
        TurningToDummy = false;
    }
    MovingToDummy = false;
    TurningToDummy = false;
    PreviousPhysicsComponent = 0;
}

int main()
{
    CThing creature,object;
    NGraphicAppearance::CTCGraphicAppearance appearance;
    CPhysics physics;
    object.PGraphicAppearance=&appearance;object.PPhysics=&physics;
    CCreatureAction_UseObject action(creature,object);
    action.Initialise();
    if(appearance.Invalid || appearance.Variants.size()!=3 || appearance.Variants[0]!=0 || appearance.Variants[1]!=1 ||
        appearance.Variants[2]!=0 || action.DummyPos.X!=1 || action.DummyPos.Y!=1 || action.Dummy.Turn!=1 ||
        !action.UsingDummy || action.MovingToDummy || action.TurningToDummy || action.PreviousPhysicsComponent || action.TotalTime!=-9 || action.ActionTime!=-9 ||
        action.Initialises!=1 || physics.Approaches)
        return 1;
    appearance.HasAnimations=false;
    action.Initialise();
    if(physics.Approaches!=1 || action.Proximity!=5 || action.DummyPos.Z!=10 || action.Initialises!=2)
        return 2;
    object.PGraphicAppearance=0;
    action.Initialise();
    if(physics.Approaches!=2 || action.Proximity!=5 || !action.UsingDummy || action.Initialises!=3)
        return 3;
    puts("006a03a0_TEST PASS");
    return 0;
}
