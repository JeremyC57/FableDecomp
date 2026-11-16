// Self-contained fixture: actual shared types, pasted production body.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define private public
#define protected public
// 00b202d0 NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformPosToMaintainShape
// Flags: none
// Leaf: ?TransformPosToMaintainShape@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@AAEXAAVC3DVector@@ABV4@111@Z
// Identity: both original PDB signatures and original FableWin02f5ed10 call graph.
// Original parameter and local names recovered from both module records.
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_SQUARE_GROUPS_PASS23
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_ENUM_CTOR
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
#define FABLE_RETAIL_CMATRIX3X4_PRETRANSLATE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#include "engine/CPSCSplineBase.h"
#include "engine/CMatrix3x4.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
void NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformPosToMaintainShape(
    C3DVector& dest,const C3DVector& old_start_pos,const C3DVector& new_start_pos,
    const C3DVector& old_end_pos,const C3DVector& new_end_pos)
{
    C3DVector new_start_end_vector=new_end_pos-new_start_pos;
    C3DVector old_start_end_vector=old_end_pos-old_start_pos;
    const float new_start_end_vector_mag=new_start_end_vector.GetAccurateMagnitude();
    const float old_start_end_vector_mag=old_start_end_vector.GetAccurateMagnitude();
    const float scale_factor=new_start_end_vector_mag/old_start_end_vector_mag;
    new_start_end_vector*=1.0f/new_start_end_vector_mag;
    old_start_end_vector*=1.0f/old_start_end_vector_mag;
    CMatrix3x4 transform(MATRIX34_INIT_IDENTITY);
    C3DVector normal=new_start_end_vector.Cross(old_start_end_vector);
    if(normal.IsNonZero()) {
        normal.AccurateNormalise();
        transform.SetFromVectorAndAngle(normal,(float)acos(new_start_end_vector.Dot(old_start_end_vector))*(1.0f/(2.0f*3.14159265f)));
    }
    transform.PreScale(scale_factor);
    transform.PreTranslate(-old_start_pos);
    transform.PostTranslate(new_start_pos);
    transform.TransformPoint(dest);
}

#undef private
#undef protected
// Real callee test doubles. No substitute engine class is defined here.
const float DELTA_NOTIONAL_ZERO=0.0001f;
static unsigned int normalizeCalls=0,rotationCalls=0;
C3DVector& C3DVector::AccurateNormalise() {
 ++normalizeCalls;
 double m=sqrt((double)X*X+(double)Y*Y+(double)Z*Z);
 assert(m>0.0); X=(float)(X/m);Y=(float)(Y/m);Z=(float)(Z/m);return *this;
}
void CMatrix3x4::SetFromVectorAndAngle(const C3DVector& axis,float angle) {
 ++rotationCalls;
 double x=axis.X,y=axis.Y,z=axis.Z;
 assert(fabs(x*x+y*y+z*z-1.0)<0.00001);
 double r=(double)angle*(2.0*3.14159265358979323846);
 double c=cos(r),s=sin(r),t=1.0-c;
 E11=(float)(c+x*x*t); E12=(float)(x*y*t-z*s); E13=(float)(x*z*t+y*s);
 E21=(float)(x*y*t+z*s); E22=(float)(c+y*y*t); E23=(float)(y*z*t-x*s);
 E31=(float)(x*z*t-y*s); E32=(float)(y*z*t+x*s); E33=(float)(c+z*z*t);
 E41=E42=E43=0.0f;
}
namespace NParticleEngine {
CPSCSplineBase::CSplinePosParamData::CSplinePosParamData(const CCardinalSpline& spline)
 : PParentParticleSystem(0),Spline(spline),MaintainSplineShape(true) {}
}
static void referenceTransform(const C3DVector& point,const C3DVector& a,const C3DVector& b,
 const C3DVector& endA,const C3DVector& endB,double result[3],bool& rotated) {
 double u[3]={endA.X-a.X,endA.Y-a.Y,endA.Z-a.Z};
 double v[3]={endB.X-b.X,endB.Y-b.Y,endB.Z-b.Z};
 double p[3]={point.X-a.X,point.Y-a.Y,point.Z-a.Z};
 double origin[3]={b.X,b.Y,b.Z};
 double m=sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2]);
 double n=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
 assert(m>0.0&&n>0.0);
 int j;for(j=0;j<3;++j){u[j]/=m;v[j]/=n;}
 double axis[3]={u[1]*v[2]-u[2]*v[1],u[2]*v[0]-u[0]*v[2],u[0]*v[1]-u[1]*v[0]};
 double sine=sqrt(axis[0]*axis[0]+axis[1]*axis[1]+axis[2]*axis[2]);
 rotated=sine>(double)DELTA_NOTIONAL_ZERO;
 if(rotated){
  for(j=0;j<3;++j)axis[j]/=sine;
  double cosine=u[0]*v[0]+u[1]*v[1]+u[2]*v[2];
  double projection=axis[0]*p[0]+axis[1]*p[1]+axis[2]*p[2];
  double cross[3]={axis[1]*p[2]-axis[2]*p[1],axis[2]*p[0]-axis[0]*p[2],axis[0]*p[1]-axis[1]*p[0]};
  for(j=0;j<3;++j)result[j]=origin[j]+n/m*(cosine*p[j]+sine*cross[j]+(1.0-cosine)*projection*axis[j]);
 }else{
  // Original deliberately leaves identity for parallel OR antiparallel directions.
  for(j=0;j<3;++j)result[j]=origin[j]+n/m*p[j];
 }
}
static bool closeValue(double actual,double expected) {
 return fabs(actual-expected)<=0.0002*(1.0+fabs(expected));
}
int main() {
 CCardinalSpline spline;
 NParticleEngine::CPSCSplineBase::CSplinePosParamData data(spline);
 static const float directions[10][3]={{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1},{1,1,0},{0,1,1},{1,0,1},{1,1,1}};
 static const float positions[7][3]={{0,0,0},{1,0,0},{0,1,0},{0,0,1},{-2,1,4},{0.5f,-0.25f,2},{4,-2,-1}};
 static const float scales[3]={0.5f,1.0f,4.0f};
 unsigned int cases=0;
 for(int i=0;i<10;++i)for(int k=0;k<10;++k)for(int si=0;si<3;++si)for(int sk=0;sk<3;++sk)
 for(int shift=0;shift<4;++shift)for(int q=0;q<7;++q)for(int alias=0;alias<5;++alias){
  C3DVector a((float)shift,-(float)shift,0.5f*(float)shift);
  C3DVector b(-0.5f*(float)shift,2.0f*(float)shift,-(float)shift);
  C3DVector ae(a.X+directions[i][0]*scales[si],a.Y+directions[i][1]*scales[si],a.Z+directions[i][2]*scales[si]);
  C3DVector be(b.X+directions[k][0]*scales[sk],b.Y+directions[k][1]*scales[sk],b.Z+directions[k][2]*scales[sk]);
  C3DVector p(positions[q][0],positions[q][1],positions[q][2]);
  C3DVector* dest=&p;if(alias==1)dest=&a;else if(alias==2)dest=&b;else if(alias==3)dest=&ae;else if(alias==4)dest=&be;
  C3DVector inputs[4]={a,b,ae,be};
  double expected[3];bool rotated;
  referenceTransform(*dest,a,b,ae,be,expected,rotated);
  normalizeCalls=rotationCalls=0;
  data.TransformPosToMaintainShape(*dest,a,b,ae,be);
  assert(normalizeCalls==(rotated?1U:0U));assert(rotationCalls==normalizeCalls);
  assert(closeValue(dest->X,expected[0])&&closeValue(dest->Y,expected[1])&&closeValue(dest->Z,expected[2]));
  C3DVector* after[4]={&a,&b,&ae,&be};
  for(int f=0;f<4;++f)if(after[f]!=dest)assert(after[f]->X==inputs[f].X&&after[f]->Y==inputs[f].Y&&after[f]->Z==inputs[f].Z);
  assert(data.PParentParticleSystem==0&&&data.Spline==&spline&&data.MaintainSplineShape&&data.SplinePosParam.empty());
  ++cases;
 }
 assert(cases==126000);
 puts("00b202d0_TEST PASS");return 0;
}