// NBBBMaths::CLinearSpline::ProcessPoints(long) -- refine corners of a polyline
// Leaf: ?ProcessPoints@CLinearSpline@NBBBMaths@@QAEXJ@Z
// Grade: DIFFER(1272v1272) -- one scheduling group: retail does the push_back capacity load/compare before point_12.Z (fld/fadd/fstp); this build between the Z add and its store
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00a58130/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 00a58130 probes in probe/00a58130.log
// Flags: /Gr
#include <math.h>
#pragma intrinsic(sqrt,acos)
#define FABLE_STLPORT_SPLINE
#define FABLE_RETAIL_VECTOR_SPLINE
#include "engine/NBBBMaths_CLinearSpline.h"
#pragma auto_inline(off)
namespace std {
template<class T,class A>void vector<T,A>::_M_insert_overflow(pointer pos,const T& x,const __false_type&,size_type fill_len,bool atend){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=start;
 finish=__uninitialized_copy(this->_M_start,pos,start,__false_type());
 if(fill_len==1){_Construct(finish,x);++finish;}else finish=__uninitialized_fill_n(finish,fill_len,x,__false_type());
 if(!atend)finish=__uninitialized_copy(pos,this->_M_finish,finish,__false_type());
 _M_clear();_M_set(start,finish,start+len);
}
}
#pragma auto_inline(on)
void NBBBMaths::CLinearSpline::ProcessPoints(long smooth_iterations){
 ProcessedPoints=ControlPoints;
 long num_pts=ProcessedPoints.size();
 if(num_pts<3)return;
 for(long smooth_counter=0;smooth_counter!=smooth_iterations;++smooth_counter){
  TVectorCntr new_points;
  num_pts=ProcessedPoints.size();
  C3DVector p_first=ProcessedPoints[0];
  for(long c1=1;c1!=num_pts-1;++c1){
   C3DVector p_second=ProcessedPoints[c1];
   C3DVector p_third=ProcessedPoints[c1+1];
   C3DVector bounding_line=p_third-p_first;
   C3DVector line0=p_first-p_second;
   C3DVector line1=p_third-p_second;
   float bl_mag=bounding_line.GetMagnitude();
   float l0_mag=line0.GetMagnitude();
   float l1_mag=line1.GetMagnitude();
   float cos_angle_limit0=-1.0f*line0.Dot(bounding_line)/(l0_mag*bl_mag);
   float cos_angle_limit1=line1.Dot(bounding_line)/(l1_mag*bl_mag);
   if(cos_angle_limit0<-1.0f)cos_angle_limit0=-1.0f;else if(cos_angle_limit0>1.0f)cos_angle_limit0=1.0f;
   if(cos_angle_limit1<-1.0f)cos_angle_limit1=-1.0f;else if(cos_angle_limit1>1.0f)cos_angle_limit1=1.0f;
   C3DVector perp=line0.Cross(line1)/(l0_mag*l1_mag);
   const float ANGLE_FRACTION=0.25f;const float LENGTH_FRACTION=0.25f;
   float angle_limit0=(float)acos(cos_angle_limit0)*0.15915493667125702f;
   line0.Rotate(perp,angle_limit0*ANGLE_FRACTION);
   float angle_limit1=(float)acos(cos_angle_limit1)*0.15915493667125702f;
   line1.Rotate(perp,-angle_limit1*ANGLE_FRACTION);
   line0*=LENGTH_FRACTION;line1*=LENGTH_FRACTION;
   C3DVector point_12=line0+p_second;C3DVector point_23=line1+p_second;
   new_points.push_back(point_12);new_points.push_back(point_23);p_first=p_second;
  }
  long c3=1;
  for(long c2=0;c2!=(long)new_points.size();){
   ProcessedPoints.insert(ProcessedPoints.begin()+c3,new_points[c2]);++c2;c3+=2;
   ProcessedPoints.insert(ProcessedPoints.begin()+c3,new_points[c2]);++c2;++c3;
  }
 }
}
