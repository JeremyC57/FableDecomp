#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct C3DVector{float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector&p){X=p.X;Y=p.Y;Z=p.Z;}C3DVector ToC3DCoordF()const{return *this;}C3DVector ToC3DVector()const{return *this;}};
struct CMatrix3x4{float m[12];CMatrix3x4(){for(int n=0;n<12;++n)m[n]=0;m[0]=m[4]=m[8]=1;}void TransformPoint(C3DVector&v)const{float xx=v.X,yy=v.Y,zz=v.Z;v.X=xx*m[0]+yy*m[3]+zz*m[6]+m[9];v.Y=xx*m[1]+yy*m[4]+zz*m[7]+m[10];v.Z=xx*m[2]+yy*m[5]+zz*m[8]+m[11];}};
struct CCardinalSpline{std::vector<C3DVector>points;long GetControlPointCount()const{return(long)points.size();}void SetControlPoint(long i,const C3DVector&p){points[i]=p;}};
namespace NParticleEngine{
template<class K,class V>struct CParticleUpdateParameters{std::map<K,C3DVector>values;mutable int lookups;CParticleUpdateParameters():lookups(0){}bool GetVectorParam(C3DVector&dest,const K&id)const{++lookups;if(id){typename std::map<K,C3DVector>::const_iterator found=values.find(id);if(found!=values.end()){dest=found->second;return true;}}return false;}};
struct CParticleSystem{CMatrix3x4 transform;mutable int calls;CParticleSystem():calls(0){}void GetInverseSystemTransformWithTranslate(CMatrix3x4&out)const{out=transform;++calls;}};
struct CPSCSplineBase{struct CSplinePosParamData{const CParticleSystem*PParentParticleSystem;const CCardinalSpline&Spline;bool MaintainSplineShape;std::vector<unsigned long>SplinePosParam;CSplinePosParamData(const CCardinalSpline&s):PParentParticleSystem(0),Spline(s),MaintainSplineShape(false){}void Update(const CParticleUpdateParameters<unsigned long,unsigned long>&params,CCardinalSpline&spline);};};
}

void NParticleEngine::CPSCSplineBase::CSplinePosParamData::Update(const CParticleUpdateParameters<unsigned long,unsigned long>& params,CCardinalSpline& spline)
{
 if(!MaintainSplineShape) {
  for(long c_i=0;c_i<Spline.GetControlPointCount();++c_i) {
   C3DVector temp;
   if(params.GetVectorParam(temp,SplinePosParam[c_i])) {
    C3DVector pt=temp.ToC3DCoordF();
    if(PParentParticleSystem) {
     CMatrix3x4 inv_transform;
     PParentParticleSystem->GetInverseSystemTransformWithTranslate(inv_transform);
     inv_transform.TransformPoint(pt);
    }
    spline.SetControlPoint(c_i,pt.ToC3DVector());
   }
  }
 }
}
static void check(bool p,const char*s){if(!p){printf("FAIL %s\n",s);exit(1);}}
static bool eq(const C3DVector&a,const C3DVector&b){return fabs(a.X-b.X)<0.00003&&fabs(a.Y-b.Y)<0.00003&&fabs(a.Z-b.Z)<0.00003;}
using namespace NParticleEngine;
int main(){
 {CCardinalSpline shape,out;CPSCSplineBase::CSplinePosParamData d(shape);CParticleUpdateParameters<unsigned long,unsigned long>p;d.Update(p,out);check(p.lookups==0,"empty source");}
 {CCardinalSpline shape,out;shape.points.resize(2);out.points.resize(2,C3DVector(8,9,10));CPSCSplineBase::CSplinePosParamData d(shape);CParticleUpdateParameters<unsigned long,unsigned long>p;d.MaintainSplineShape=true;d.Update(p,out);check(p.lookups==0&&eq(out.points[0],C3DVector(8,9,10)),"maintain shape skips all parameters");}
 {CCardinalSpline shape,out;shape.points.resize(3);out.points.resize(4,C3DVector(8,9,10));CPSCSplineBase::CSplinePosParamData d(shape);d.SplinePosParam.push_back(0);d.SplinePosParam.push_back(9);d.SplinePosParam.push_back(1);CParticleUpdateParameters<unsigned long,unsigned long>p;p.values[0]=C3DVector(99,99,99);p.values[1]=C3DVector(1,2,3);d.Update(p,out);check(p.lookups==3&&eq(out.points[0],C3DVector(8,9,10))&&eq(out.points[1],C3DVector(8,9,10)),"zero and missing parameters leave outputs");check(eq(out.points[2],C3DVector(1,2,3))&&eq(out.points[3],C3DVector(8,9,10)),"no parent copies and uses source count");check(eq(p.values[1],C3DVector(1,2,3)),"parameters unchanged");}
 {CCardinalSpline shape,out;shape.points.resize(2);out.points.resize(2);CPSCSplineBase::CSplinePosParamData d(shape);d.SplinePosParam.push_back(1);d.SplinePosParam.push_back(1);CParticleUpdateParameters<unsigned long,unsigned long>p;p.values[1]=C3DVector(1,2,3);CParticleSystem system;system.transform.m[0]=2;system.transform.m[1]=3;system.transform.m[2]=4;system.transform.m[3]=5;system.transform.m[4]=6;system.transform.m[5]=7;system.transform.m[6]=8;system.transform.m[7]=9;system.transform.m[8]=10;system.transform.m[9]=11;system.transform.m[10]=12;system.transform.m[11]=13;d.PParentParticleSystem=&system;d.Update(p,out);check(system.calls==2&&eq(out.points[0],C3DVector(47,54,61))&&eq(out.points[1],out.points[0]),"all affine components, duplicate keys, fresh transform each point");check(eq(p.values[1],C3DVector(1,2,3)),"transformed copy does not mutate parameter");}
 puts("00b20ba0_TEST PASS");return 0;
}