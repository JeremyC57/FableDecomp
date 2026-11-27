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
#ifdef FABLE_RETAIL_SCRIPTED_SPLINE_UPDATE_PASS29
 CCardinalSpline(const CArray<C3DVector>& points,float tension=0.5f,bool end_points_connected=false)
   : ControlPoints(points),Tension(tension),EndPointsConnected(end_points_connected){}
 void DeleteControlPoint(long index){if(index>=0 && static_cast<unsigned long>(index)<ControlPoints.size()) ControlPoints.erase(ControlPoints.begin()+index);}
#endif
#ifdef FABLE_RETAIL_CPSCSPLINEBASE_SERIAL_PASS29
 bool GetEndPointsConnected() const { return EndPointsConnected; }
 void SetEndPointsConnected(bool value) { EndPointsConnected=value; }
#endif
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
#ifdef FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT
 // Retail opt-in (w24B, row 0x007DDA30 CTCCreatureFlyInCircles::QueueSwoop): FableWin.pdb ?DeleteAllControlPoints@CCardinalSpline@@QAEXXZ
 // (debug COMDAT 0x027636F0, first used by QueueSwoop 0x027633E0) and Ego_r.pdb / FableWin.pdb ?AppendControlPoint@CCardinalSpline@@QAEJABVC3DVector@@@Z
 // (retail's unreferenced out-of-line copy 0x0075F980, landed: push_back then `size() - 1`), both expanded in QueueSwoop: the clear
 // is ControlPoints.erase(begin, end) through the random-access __copy 0x0074CA20 (0x007DDB37..0x007DDB4C), each append the inline
 // push_back with the _M_insert_overflow 0x0066E370 call (0x007DDB6E..0x007DDBA4 and twice more). Bodies only: no layout change.
 void DeleteAllControlPoints(){ControlPoints.clear();}
#ifdef FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT_FORCEINLINE
 // Per-TU inline opt-in (same row): QueueSwoop expands all three AppendControlPoint calls in place past VC7.1's caller-size inline
 // budget, which calls a plain-inline copy (502v642).
 FABLE_FORCEINLINE
#endif
 long AppendControlPoint(const C3DVector& point){ControlPoints.push_back(point);return ControlPoints.size()-1;}
#endif
private:
 CArray<C3DVector> ControlPoints;float Tension;bool EndPointsConnected;
};
FABLE_STATIC_ASSERT_AT(25, sizeof(CCardinalSpline)==0x14);
#endif
