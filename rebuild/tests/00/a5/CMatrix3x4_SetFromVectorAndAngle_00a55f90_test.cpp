#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include <cassert>
#include <cmath>
#include <cstdio>

#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
#include "engine/CMatrix3x4.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"

// The selected implementation is reproduced verbatim for this standalone fixture.
void CMatrix3x4::SetFromVectorAndAngle(const C3DVector& normal, float angle)
{
    float cos = GFCos(angle);
    float sin = GFSin(angle);
    const C3DVector& axis = normal;
    float vx2 = axis.X * axis.X;
    float vy2 = axis.Y * axis.Y;
    float vz2 = axis.Z * axis.Z;
    float vxvy = axis.X * axis.Y;
    float vxvz = axis.X * axis.Z;
    float vyvz = axis.Y * axis.Z;
    float stvx = sin * axis.X;
    float stvy = sin * axis.Y;
    float stvz = sin * axis.Z;
    E11 = (vx2 + cos) - vx2 * cos;
    E21 = vxvy - vxvy * cos + stvz;
    E31 = vxvz - vxvz * cos - stvy;
    E12 = vxvy - vxvy * cos - stvz;
    E22 = (vy2 + cos) - vy2 * cos;
    E32 = vyvz - vyvz * cos + stvx;
    E13 = vxvz - vxvz * cos + stvy;
    E23 = vyvz - vyvz * cos - stvx;
    E33 = vz2 + cos - vz2 * cos;
    E41 = 0.0f;
    E42 = 0.0f;
    E43 = 0.0f;
}

const double TWO_PI = (double)6.2831855f;

// Compare the implementation with a separate matrix form of Rodrigues' formula.
int main(){
 const float values[]={-2.0f,-1.0f,0.0f,0.5f,1.0f};
 const float turns[]={-1.0f,-0.5f,-0.25f,-0.125f,0.0f,0.125f,0.25f,0.5f,1.0f};
 unsigned cases=0;
 for(int x=0;x<5;++x)for(int y=0;y<5;++y)for(int z=0;z<5;++z)for(int a=0;a<9;++a){
  C3DVector v;v.X=values[x];v.Y=values[y];v.Z=values[z];
  CMatrix3x4 m;
  m.E41 = 91.0f;
  m.E42 = -32.0f;
  m.E43 = 2.0f;
  m.SetFromVectorAndAngle(v,turns[a]);
  const float actual[3][3]={{m.E11,m.E12,m.E13},{m.E21,m.E22,m.E23},{m.E31,m.E32,m.E33}};
  const double axis[3]={v.X,v.Y,v.Z};
  const double skew[3][3]={{0,-v.Z,v.Y},{v.Z,0,-v.X},{-v.Y,v.X,0}};
  const double c=std::cos((double)turns[a]*(double)6.2831855f);
  const double sn=std::sin((double)turns[a]*(double)6.2831855f);
  for(int r=0;r<3;++r)for(int col=0;col<3;++col){
   const double expected=(r==col?c:0)+(1-c)*axis[r]*axis[col]+sn*skew[r][col];
   assert(std::fabs(actual[r][col]-expected)<0.00001);
  }
  assert(m.E41==0&&m.E42==0&&m.E43==0);
  assert(v.X==values[x]&&v.Y==values[y]&&v.Z==values[z]);++cases;
 }
 assert(cases==1125);std::puts("00a55f90_TEST PASS");return 0;
}