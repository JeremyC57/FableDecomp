// NBBBMaths::CCylinder::GetFirstIntersectionWithFiniteLine(C3DVector const &,C3DVector const &,C3DVector &,C3DVector &) const -- intersect a segment with the curved cylinder surface
// Leaf: ?GetFirstIntersectionWithFiniteLine@CCylinder@NBBBMaths@@QBE_NABVC3DVector@@0AAV3@1@Z
// Flags: /Gr
#include <math.h>
#pragma intrinsic(sqrt,fabs)
#include "engine/NBBBMaths_CCylinder.h"
extern const float DELTA_NOTIONAL_ZERO;
bool NBBBMaths::CCylinder::GetFirstIntersectionWithFiniteLine(const C3DVector& line_start,const C3DVector& line_end,C3DVector& intersection_pos,C3DVector& outward_normal)const{
 C3DVector cyl_ax=AxisTop-AxisBottom;
 float cyl_height=cyl_ax.GetSquaredMagnitude();
 if(!(cyl_height>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO))return false;
 cyl_height=(float)sqrt(cyl_height);cyl_ax/=cyl_height;
 C3DVector axis_x;
 axis_x.Z=0.0f;
 if((float)fabs(cyl_ax.X)>(float)fabs(cyl_ax.Y)){
  axis_x.Y=1.0f;axis_x.X=0.0f;
  if((float)fabs(cyl_ax.X)>DELTA_NOTIONAL_ZERO)axis_x.X=-cyl_ax.Y/cyl_ax.X;
 }else{
  axis_x.X=1.0f;axis_x.Y=0.0f;
  if((float)fabs(cyl_ax.Y)>DELTA_NOTIONAL_ZERO)axis_x.Y=-cyl_ax.X/cyl_ax.Y;
 }
 axis_x.AccurateNormalise();
 C3DVector axis_y=cyl_ax.Cross(axis_x);axis_y.AccurateNormalise();
 C3DVector lhs=line_start-AxisBottom;
 C3DVector lhs_prime(lhs.Dot(axis_x),lhs.Dot(axis_y),0.0f);
 C3DVector test_line=line_end-line_start;
 C3DVector test_line_prime(test_line.Dot(axis_x),test_line.Dot(axis_y),0.0f);
 float coef_a=test_line_prime.X*test_line_prime.X+test_line_prime.Y*test_line_prime.Y;
 float coef_b=2.0f*(lhs_prime.X*test_line_prime.X+lhs_prime.Y*test_line_prime.Y);
 float coef_c=lhs_prime.X*lhs_prime.X+lhs_prime.Y*lhs_prime.Y-Radius*Radius;
 float desc=coef_b*coef_b-4.0f*coef_a*coef_c;
 if(desc<0.0f)return false;
 desc=(float)sqrt(desc);
 std::vector<float> valid_lambdas;
 float lambda1=(-coef_b+desc)/(2.0f*coef_a);
 float lambda2=(-coef_b-desc)/(2.0f*coef_a);
 if(lambda1>0.0f&&lambda1<1.0f){
  C3DVector int_pos=line_start+test_line*lambda1;
  C3DVector out_line=int_pos-AxisBottom;
  float height_param=out_line.Dot(cyl_ax);
  if(height_param>0.0f&&height_param<cyl_height)valid_lambdas.push_back(lambda1);
 }
 if(lambda2>0.0f&&lambda2<1.0f){
  C3DVector int_pos=line_start+test_line*lambda2;
  C3DVector out_line=int_pos-AxisBottom;
  float height_param=out_line.Dot(cyl_ax);
  if(height_param>0.0f&&height_param<cyl_height)valid_lambdas.push_back(lambda2);
 }
 if(valid_lambdas.begin()==valid_lambdas.end())return false;
 std::sort(valid_lambdas.begin(),valid_lambdas.end(),std::less<float>());
 float lambda_to_use=*valid_lambdas.begin();
 intersection_pos=line_start+test_line*lambda_to_use;
 C3DVector out_line=intersection_pos-AxisBottom;
 outward_normal=C3DVector(out_line.Dot(axis_x),out_line.Dot(axis_y),0.0f);
 outward_normal.AccurateNormalise();
 return true;
}