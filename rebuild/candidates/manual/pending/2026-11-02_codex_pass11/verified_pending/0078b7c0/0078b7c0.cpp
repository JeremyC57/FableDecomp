// NMaths::GetRootsNumerically<CAnalyseRootsOfBowPullBackFunction>(float,float,const CAnalyseRootsOfBowPullBackFunction&,long,long) -- Find bow pullback roots numerically.
// Leaf: ??$GetRootsNumerically@VCAnalyseRootsOfBowPullBackFunction@?A0x0524c104@@@NMaths@@YI?AV?$vector@MV?$allocator@M@std@@@std@@MMABVCAnalyseRootsOfBowPullBackFunction@?A0x0524c104@@JJ@Z
// Grade: RELOCATION_MATCH -- complete1390-byte function; relocation names and behavior audited separately
// Flags: /Gr
#define FABLE_RETAIL_BOW_ROOTS
#include <stlport46/vector_roots.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
#ifdef _MSC_VER
#include <math.h>
#else
// GNU host adapter avoids mixing its private STL traits with original STLport.
extern "C" {double tanh(double);double fabs(double);}
#endif
#pragma intrinsic(tanh,fabs)
extern const float DELTA_NOTIONAL_ZERO;
inline double __fastcall GFTanh(double value){return tanh(value);}
namespace {
inline float GetBowPullBackAmountFromPullBackTimer(long pullback_timer){float tanh_weight=0.05f;return (float)GFTanh(pullback_timer*tanh_weight);}
// Layout: FableWin original class V size4; private DesiredPullBackAmount at+0,
// confirmed by native GetValue receiver load and caller functor construction.
class CAnalyseRootsOfBowPullBackFunction {
 float DesiredPullBackAmount;
public:
 CAnalyseRootsOfBowPullBackFunction(float desired_pullback_amount):DesiredPullBackAmount(desired_pullback_amount){}
 float GetValue(float x_axis)const{return DesiredPullBackAmount-GetBowPullBackAmountFromPullBackTimer((long)x_axis);}
 bool IsValid()const;
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
template std::vector<float> GetRootsNumerically<CAnalyseRootsOfBowPullBackFunction>(float,float,const CAnalyseRootsOfBowPullBackFunction&,long,long);
}
