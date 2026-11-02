// NCameraModes::CPhysicsCollisionPointsObject::CPhysicsCollisionPointsObject(long,float,float,long,long,long) -- construct angular scan lists and test counts
// Leaf: ??0CPhysicsCollisionPointsObject@NCameraModes@@QAE@JMMJJJ@Z
#include "engine/CPhysicsCollisionPointsObject.h"
extern const float DELTA_NOTIONAL_ZERO;

NCameraModes::CPhysicsCollisionPointsObject::CPhysicsCollisionPointsObject(long scan_den,float s_thet,float d_thet,long red,long green,long blue)
: ScanDensity(scan_den),StartTheta(s_thet),DeltaTheta(d_thet),DebugGraphicsRed(red),DebugGraphicsGreen(green),DebugGraphicsBlue(blue)
{
    for(float theta=s_thet;theta<=0.5f+DELTA_NOTIONAL_ZERO;theta+=d_thet) {
        std::list<C3DVector> new_list;
        std::pair<float,std::list<C3DVector> > new_guy_to_add(theta,new_list);
        SmartPointList.push_back(new_guy_to_add);
    }
    std::list<std::pair<float,std::list<C3DVector> > >::iterator it=SmartPointList.begin();
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,10));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
    NumTestsPerTheta.push_back(std::pair<float,long>((it++)->first,1));
}