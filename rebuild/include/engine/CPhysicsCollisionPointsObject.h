#ifndef FABLE_ENGINE_CPHYSICSCOLLISIONPOINTSOBJECT_H
#define FABLE_ENGINE_CPHYSICSCOLLISIONPOINTSOBJECT_H
// Retail opt-in (Codex pass11): Ego_r TPI0x4c5d8 and native constructor008fb170.
// Three STLport lists are4 bytes each; subsequent six fields occupy12..32, total36.
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_SCAN_PASS30
#include "engine/GFSqr.h"
#endif
#include "stlport/fable_containers.h"
namespace NCameraModes {
class CPhysicsCollisionPointsObject {
public:
    std::list<C3DVector> Points;
    std::list<std::pair<float,std::list<C3DVector> > > SmartPointList;
    std::list<std::pair<float,long> > NumTestsPerTheta;
    long ScanDensity;
    float StartTheta;
    float DeltaTheta;
    long DebugGraphicsRed,DebugGraphicsGreen,DebugGraphicsBlue;
    CPhysicsCollisionPointsObject(long,float,float,long,long,long);
#ifdef FABLE_RETAIL_CPHYSICSCOLLISIONPOINTSOBJECT_POINTS
    // Retail opt-in (Codex pass19): both donor name/order and native008fa9f0.
    void GetPointsList();
#ifdef FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_SCAN_PASS30
    // FableWin 02B1F250, offset local and promoted 14.3f. TLC loads all three as floats.
    long GetNumberOfTests(float theta) const
    {
        float offset = theta - 0.25f;
        return (long)((1.0f - GFSqr(offset) * 14.3f) * ScanDensity);
    }
#else
    long GetNumberOfTests(float) const;
#endif
#endif
};
}
#ifdef _MSC_VER
typedef char FABLE_STATIC_ASSERT_CPhysicsCollisionPointsObject[(sizeof(NCameraModes::CPhysicsCollisionPointsObject)==36)?1:-1];
#endif
#endif
