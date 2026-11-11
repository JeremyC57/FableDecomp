#include <cassert>
#include <cmath>
#include <cstdio>
class C3DVector { public: float X,Y,Z; };
class CMatrix3x4 { public:
 float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
 void SetFromVectorAndAngle(const C3DVector& normal,float angle);
};
inline double GFCos(float angle){return std::cos(angle*(double)6.2831855f);}
inline double GFSin(float angle){return std::sin(angle*(double)6.2831855f);}
void CMatrix3x4::SetFromVectorAndAngle(const C3DVector& normal, float angle)
{
    float cos = (float)GFCos(angle);
    float sin = (float)GFSin(angle);
    float vx2 = normal.X * normal.X;
    float vy2 = normal.Y * normal.Y;
    float vz2 = normal.Z * normal.Z;
    float vxvy = normal.X * normal.Y;
    float vxvz = normal.X * normal.Z;
    float vyvz = normal.Y * normal.Z;
    float stvx = sin * normal.X;
    float stvy = sin * normal.Y;
    float stvz = sin * normal.Z;
    E11 = vx2 + cos - vx2 * cos;
    E21 = vxvy - vxvy * cos + stvz;
    E31 = vxvz - vxvz * cos - stvy;
    E12 = vxvy - vxvy * cos - stvz;
    E22 = vy2 + cos - vy2 * cos;
    E32 = vyvz - vyvz * cos + stvx;
    E13 = vxvz - vxvz * cos + stvy;
    E23 = vyvz - vyvz * cos - stvx;
    E33 = vz2 + cos - vz2 * cos;
    E41 = 0.0f;
    E42 = 0.0f;
    E43 = 0.0f;
}

int main(){
 const float values[]={-2.0f,-1.0f,0.0f,0.5f,1.0f};
 const float turns[]={-1.0f,-0.5f,-0.25f,-0.125f,0.0f,0.125f,0.25f,0.5f,1.0f};
 unsigned cases=0;
 for(int x=0;x<5;++x)for(int y=0;y<5;++y)for(int z=0;z<5;++z)for(int a=0;a<9;++a){
  C3DVector v;v.X=values[x];v.Y=values[y];v.Z=values[z];
  CMatrix3x4 m;m.SetFromVectorAndAngle(v,turns[a]);
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
