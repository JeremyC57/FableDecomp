#include <cstdlib>
#include <cstdio>
#include <vector>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_TOC3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TOCOORDF_PASS26
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#include "rebuild_abi.h"
#include "engine/CMatrix3x4.h"
static void check(bool ok){if(!ok)std::abort();}
static bool eq(const C3DVector& a,const C3DVector& b){return a.X==b.X && a.Y==b.Y && a.Z==b.Z;}
static C3DVector valueFor(unsigned long n){return C3DVector(float(n),float(n+2),float(-2L*long(n)));}
struct CCardinalSpline {
 std::vector<C3DVector> points;
 long GetControlPointCount()const{return long(points.size());}
 const C3DVector& GetControlPoint(long i)const{return points[i];}
};
namespace NParticleEngine {
struct CParticleSystem {
 mutable int calls;CParticleSystem():calls(0){}
 void GetInverseSystemTransformWithTranslate(CMatrix3x4& m) const{
  ++calls;m.E11=1;m.E12=0;m.E13=1;m.E21=2;m.E22=1;m.E23=-1;m.E31=0;m.E32=-2;m.E33=1;m.E41=4;m.E42=-4;m.E43=8;
 }
};
template<class K,class V> struct CParticleUpdateParameters {
 unsigned mask;mutable std::vector<K> reads;
 bool GetVectorParam(C3DVector& dest,const K& n)const{reads.push_back(n);if(!(mask&(1u<<n)))return false;dest=valueFor(n);return true;}
};
struct CPSCSplineBase {
 struct CSplinePosParamData{
  CCardinalSpline Spline;std::vector<unsigned long> SplinePosParam;CParticleSystem* PParentParticleSystem;
  bool GetClosestParameterisedControlPoints(const CParticleUpdateParameters<unsigned long,unsigned long>&,long,long,C3DVector&,C3DVector&,C3DVector&,C3DVector&);
 };
};
}
bool NParticleEngine::CPSCSplineBase::CSplinePosParamData::GetClosestParameterisedControlPoints(const CParticleUpdateParameters<unsigned long,unsigned long>& params, long start_pos_id, long end_pos_id, C3DVector& start_control_point, C3DVector& start_param_point, C3DVector& end_control_point, C3DVector& end_param_point)
{
    bool found = false;
    while (!found && start_pos_id >= 0) {
        if (SplinePosParam[start_pos_id] == 0) --start_pos_id;
        else found = true;
    }
    if (!found) return false;
    found = false;
    long control_point_count = Spline.GetControlPointCount();
    while (!found && end_pos_id < control_point_count) {
        if (SplinePosParam[end_pos_id] == 0) ++end_pos_id;
        else found = true;
    }
    if (!found) return false;
    bool ret_val = params.GetVectorParam(start_param_point, SplinePosParam[start_pos_id]);
    ret_val &= params.GetVectorParam(end_param_point, SplinePosParam[end_pos_id]);
    if (ret_val) {
        start_control_point = Spline.GetControlPoint(start_pos_id);
        end_control_point = Spline.GetControlPoint(end_pos_id);
        if (PParentParticleSystem) {
            CMatrix3x4 inv_transform;
            PParentParticleSystem->GetInverseSystemTransformWithTranslate(inv_transform);
            C3DVector pt(start_param_point);
            inv_transform.TransformPoint(pt);
            start_param_point = pt.ToC3DCoordF();
            C3DVector pt2(end_param_point);
            inv_transform.TransformPoint(pt2);
            end_param_point = pt2.ToC3DCoordF();
        }
    }
    return ret_val;
}

static C3DVector transform(const C3DVector& v){return C3DVector(v.X+2*v.Y+4,v.Y-2*v.Z-4,v.X-v.Y+v.Z+8);}
int main(){
 using namespace NParticleEngine;
 const C3DVector sentinel(-14,12,-3);
 for(unsigned present=0;present<16;++present)for(unsigned keys=0;keys<16;++keys)
 for(int first=-1;first<4;++first)for(int last=0;last<=4;++last)for(int withParent=0;withParent<2;++withParent){
  CParticleUpdateParameters<unsigned long,unsigned long> p;p.mask=keys<<1;
  CParticleSystem system;CPSCSplineBase::CSplinePosParamData obj;obj.PParentParticleSystem=withParent?&system:0;
  for(int i=0;i<4;++i){obj.SplinePosParam.push_back((present&(1u<<i))?unsigned(i+1):0);obj.Spline.points.push_back(valueFor(20+i));}
  C3DVector s=sentinel,u=sentinel,e=sentinel,v=sentinel;
  bool result=obj.GetClosestParameterisedControlPoints(p,first,last,s,u,e,v);
  int lo=-1,hi=-1;
  for(int i=0;i<4;++i)if(i<=first && (present&(1u<<i)))lo=i;
  for(int i=3;i>=0;--i)if(i>=last && (present&(1u<<i)))hi=i;
  bool pair=lo>=0 && hi>=0,one=pair && (keys&(1u<<lo))!=0,two=pair && (keys&(1u<<hi))!=0,ok=one&&two;
  check(result==ok && system.calls==int(ok&&withParent));check(p.reads.size()==unsigned(pair?2:0));
  if(pair)check(p.reads[0]==unsigned(lo+1) && p.reads[1]==unsigned(hi+1));
  check(eq(s,ok?valueFor(20+lo):sentinel) && eq(e,ok?valueFor(20+hi):sentinel));
  C3DVector a=one?valueFor(lo+1):sentinel,b=two?valueFor(hi+1):sentinel;
  if(ok&&withParent){a=transform(a);b=transform(b);}
  check(eq(u,a)&&eq(v,b));
 }
 std::puts("00b20d30_TEST PASS");return 0;
}