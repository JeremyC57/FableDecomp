// NMaths::GetRootsNumerically<CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint>(float, float, const F&, long, long) -- the roots of the ellipse closest-point function Ca*cos + Cb*sin + Cc*cos*sin (angles in turns) in [range_start, range_end): step the widened range in ever finer equal steps until enough sign changes bracket roots, bisect each bracket to 0.001, keep the roots whose both sides converged, then drop those outside the range
// Grade: (in progress)
// Leaf: ??$GetRootsNumerically@VCVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint@?A0x
// Flags: /Gr
#include "stlport/fable_containers.h"
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
extern "C" { double __cdecl sin(double);double __cdecl cos(double);double __cdecl tan(double);double __cdecl fabs(double); }
#pragma intrinsic(sin,cos,tan,fabs)
const double TWO_PI=6.283185482025146484375;             // 0x0124F2B8 ?TWO_PI@@3NB (ESTABLISHED_SPELLINGS.tsv): a namespace-scope const (internal linkage, decorated name)
extern const float DELTA_NOTIONAL_ZERO;
inline double __fastcall GFSin(float angle){return sin(angle*TWO_PI);}
inline double __fastcall GFCos(float angle){return cos(angle*TWO_PI);}
namespace {
class CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint {
private:
 float Ca,Cb,Cc;
public:
 CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint(float coef_a,float coef_b,float coef_c):Ca(coef_a),Cb(coef_b),Cc(coef_c){}
 bool IsValid()const;
 float GetValue(float x_axis)const {
  float cos_x=(float)GFCos(x_axis);
  float sin_x=(float)GFSin(x_axis);
  return Ca*cos_x+Cb*sin_x+(Cc*cos_x)*sin_x;
 }
};
}
namespace NMaths {
template<class F> std::vector<float> GetRootsNumerically(float range_start,float range_end,const F& func,long num_roots,long num_asymptotes) {
 float mod_range_start=range_start-DELTA_NOTIONAL_ZERO;
 float mod_range_end=range_end+DELTA_NOTIONAL_ZERO;
 std::vector<std::pair<float,float> > roots_between;
 long num_steps=(num_roots+num_asymptotes)-1;
 long found_roots;
 do {
  ++num_steps;roots_between.clear();found_roots=0;
  float step=(mod_range_end-mod_range_start)/num_steps;float r_start=mod_range_start;float r_end=r_start+step;float val_a=func.GetValue(mod_range_start);
  for(long c1=0;c1!=num_steps;++c1){
   float val_b=func.GetValue(r_end);
   if((val_a>=0.0f && val_b<=0.0f)||(val_a<=0.0f && val_b>=0.0f)){
    ++found_roots;roots_between.push_back(std::make_pair(r_start,r_end));
   }
   val_a=val_b;r_start=r_end;r_end+=step;
  }
 }while(found_roots<(num_roots+num_asymptotes));
 std::vector<float> roots;
 for(std::vector<std::pair<float,float> >::iterator it=roots_between.begin();it!=roots_between.end();++it){
  const std::pair<float,float>& roots_pair=*it;
  float r_start=roots_pair.first,r_end=roots_pair.second;
  const float accuracy=0.001f;
  float val_a=func.GetValue(r_start);
  float val_b=func.GetValue(r_end);
  std::pair<bool,float> val_a_converging;
  val_a_converging.first=true;
  val_a_converging.second=val_a;
  std::pair<bool,float> val_b_converging;
  val_b_converging.first=true;
  val_b_converging.second=val_b;
  while(r_end-r_start>accuracy){
   float r_mid=(r_start+r_end)*0.5f;val_b=func.GetValue(r_mid);
   if(val_a<=0.0f){if(val_b<=0.0f){val_a=val_b;r_start=r_mid;if(!((float)fabs(val_b)>(float)fabs(val_a_converging.second)+DELTA_NOTIONAL_ZERO))val_a_converging.first=true;else val_a_converging.first=false;val_a_converging.second=val_b;}else{r_end=r_mid;if(!((float)fabs(val_b)>(float)fabs(val_b_converging.second)+DELTA_NOTIONAL_ZERO))val_b_converging.first=true;else val_b_converging.first=false;val_b_converging.second=val_b;}}
   else{if(val_b>=0.0f){val_a=val_b;r_start=r_mid;if(!((float)fabs(val_b)>(float)fabs(val_a_converging.second)+DELTA_NOTIONAL_ZERO))val_a_converging.first=true;else val_a_converging.first=false;val_a_converging.second=val_b;}else{r_end=r_mid;if(!((float)fabs(val_b)>(float)fabs(val_b_converging.second)+DELTA_NOTIONAL_ZERO))val_b_converging.first=true;else val_b_converging.first=false;val_b_converging.second=val_b;}}
  }
  if(val_a_converging.first && val_b_converging.first)roots.push_back(r_start);
 }
 for(std::vector<float>::iterator it=roots.begin();it!=roots.end();){
  float root=*it;
  if(!(root>=range_start && root<range_end))it=roots.erase(it);else ++it;
 }
 return roots;
}
template std::vector<float> GetRootsNumerically<CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint>(float,float,const CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint&,long,long);
}