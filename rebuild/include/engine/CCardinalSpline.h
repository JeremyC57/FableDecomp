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
#ifdef FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT
 // FableWin.pdb ?GetControlPoint@CCardinalSpline@@QBEABVC3DVector@@J@Z / ?SetControlPoint@CCardinalSpline@@QAEXJABVC3DVector@@@Z,
 // expanded in retail (0x00B206B0 OffsetSplinePositions). Bodies only: no layout change, default path unchanged.
 const C3DVector& GetControlPoint(long index)const{return ControlPoints[index];}
 void SetControlPoint(long index,const C3DVector& point){ControlPoints[index]=point;}
#endif
#ifdef FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
 // agentC37 (row 0x0075D580): Ego_r.pdb 0x00BAE530 ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z (retail 0x00A567A0). Declaration only.
 void GetSplinePosition(C3DVector& position,float t)const;            // 0x00A567A0
#endif
private:
 CArray<C3DVector> ControlPoints;float Tension;bool EndPointsConnected;
};
FABLE_STATIC_ASSERT_AT(25, sizeof(CCardinalSpline)==0x14);
#endif
