// NMaths::GetRootsNumerically<CCotMethod>(float range_start, float range_end, const CCotMethod& func, long num_roots, long num_asymptotes) -- bracket and refine projectile angle roots
// Leaf: ??$GetRootsNumerically@VCCotMethod@?A0x
// Grade: DIFFER(1520v1522) -- same x87 allocation as sibling 0x0087AA10 in the refinement loop's right-bracket GetValue (retail fld st(1) / fdivp st(2) / faddp st(2) + fstp st(0); VC7.1 RTM fxch / fdivp st(1) / faddp st(1)): 2 bytes short, every later branch shifts. 12 shapes (pair ctor/make_pair, iterator reads, term order, cot as cos/sin, 1.0f, named quotients, statement order, accuracy scope, for(;;), member locals) leave it; anonymous-namespace leaf misses (?A0x hash path-dependent) Regraded unchanged on the landed repo headers (f01).
// Flags: /Gr
#include "stlport/fable_containers.h"
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
extern "C" { double __cdecl sin(double);double __cdecl cos(double);double __cdecl tan(double);double __cdecl fabs(double); }
#pragma intrinsic(sin,cos,tan,fabs)
extern const double TWO_PI=6.283185482025146484375;
extern const float DELTA_NOTIONAL_ZERO;
inline double __fastcall GFSin(float angle){return sin(angle*TWO_PI);}
inline double __fastcall GFCos(float angle){return cos(angle*TWO_PI);}
inline double __fastcall GFTan(float angle){return tan(angle*TWO_PI);}
namespace {
// Layout: original donor class, private float Tx+0, Ty+4, Rad+8, Ki+12; size16.
class CCotMethod {
private:
 float Tx,Ty,Rad,Ki;
public:
 CCotMethod(float t_x,float t_y,float rad,float ki):Tx(t_x),Ty(t_y),Rad(rad),Ki(ki){}
 bool IsValid()const;
 float GetValue(float x_axis)const {return (Tx/Rad-GFSin(x_axis))/(Ty/Rad-GFCos(x_axis))+1.0/GFTan(x_axis+Ki);}
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
template std::vector<float> GetRootsNumerically<CCotMethod>(float,float,const CCotMethod&,long,long);
}
