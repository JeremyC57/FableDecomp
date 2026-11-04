// NCameraModes::CPhysicsCollisionPointsObject::~CPhysicsCollisionPointsObject() -- camera collision-point scan record dtor (compiler-generated member dtors): ~NumTestsPerTheta, ~SmartPointList, ~Points
// Leaf: ??1CPhysicsCollisionPointsObject@NCameraModes@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00850330.
// Identity: Ego_r.pdb ??1CPhysicsCollisionPointsObject@NCameraModes@@QAE@XZ; three STLport lists: Points +0,
//   SmartPointList list<pair<float,list<C3DVector>>> +4, NumTestsPerTheta list<pair<float,long>> +8.
// Shape: empty user body; the shared stlport list header reproduces retail's inline/out-of-line split.
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"

namespace NCameraModes {
class CPhysicsCollisionPointsObject {
public:
    CPhysicsCollisionPointsObject(long scanDensity, float startTheta, float deltaTheta, long red, long green, long blue);
    ~CPhysicsCollisionPointsObject();
    void GetPointsList();
    long GetNumberOfTests(float theta) const;
    std::list<C3DVector>                                Points;             // +0x00
    std::list<std::pair<float, std::list<C3DVector> > > SmartPointList;     // +0x04
    std::list<std::pair<float, long> >                  NumTestsPerTheta;   // +0x08
    long                                                ScanDensity;        // +0x0c
    float                                               StartTheta;         // +0x10
    float                                               DeltaTheta;         // +0x14
    long                                                DebugGraphicsRed;   // +0x18
    long                                                DebugGraphicsGreen; // +0x1c
    long                                                DebugGraphicsBlue;  // +0x20
};

CPhysicsCollisionPointsObject::~CPhysicsCollisionPointsObject()
{
}
}