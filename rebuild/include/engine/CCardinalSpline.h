#ifndef FABLE_ENGINE_CCARDINALSPLINE_H
#define FABLE_ENGINE_CCARDINALSPLINE_H
#include "engine/CArray.h"
#include "engine/C3DVector.h"
// Layout: both PDBs prove private ControlPoints/Tension/EndPointsConnected;
// native new0x14 and accesses at+0/+0c/+10 reflect the12-byte STLport array.
// Donor01BBE05D and02F4B5C0.. getter group prove these original inline bodies.
class CCardinalSpline {
public:
 CCardinalSpline():Tension(0.5f),EndPointsConnected(false){}
 float GetTension()const{return Tension;}
 void SetTension(const float t){Tension=t;}
 long GetControlPointCount()const{return ControlPoints.size();}
 CArray<C3DVector> GetControlPoints()const{return ControlPoints;}
 void SetControlPoints(const CArray<C3DVector>& points){ControlPoints=points;}
private:
 CArray<C3DVector> ControlPoints;float Tension;bool EndPointsConnected;
};
FABLE_STATIC_ASSERT(sizeof(CCardinalSpline)==0x14);
#endif
