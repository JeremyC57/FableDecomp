#include <stdio.h>
#include <string.h>
#include <new>
// 00ac6d50 QuadricOptimiserInternals::CQuadric::EvaluateError
// Flags:
// Leaf: ?EvaluateError@CQuadric@QuadricOptimiserInternals@@QBENABVC3DVector@@@Z
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;};
#pragma pack(pop)
namespace QuadricOptimiserInternals{
// Layout: original donor88bytes, double AA0/AB8/AC16/AD24/BB32/BC40/BD48/
// CC56/CD64/DD72/Weight80. No engine header exists for this nested class.
class CQuadric{private:double AA,AB,AC,AD,BB,BC,BD,CC,CD,DD,Weight;
public:
 double EvaluateError(const C3DVector&)const;
};
double CQuadric::EvaluateError(const C3DVector&v)const{
 float x=v.X,y=v.Y,z=v.Z;
 return (z*z)*CC+(y*z*2.0f)*BC+(y*y)*BB+(x*z*2.0f)*AC+(x*y*2.0f)*AB
       +(z*2.0f)*CD+(y*2.0f)*BD+(x*2.0f)*AD+(x*x)*AA+DD;
}
}

static double reference(const QuadricOptimiserInternals::CQuadric&q,const C3DVector&v){
 double coeff[11];memcpy(coeff,&q,sizeof(coeff));
 const double matrix[4][4]={{coeff[0],coeff[1],coeff[2],coeff[3]},{coeff[1],coeff[4],coeff[5],coeff[6]},{coeff[2],coeff[5],coeff[7],coeff[8]},{coeff[3],coeff[6],coeff[8],coeff[9]}};
 const double p[4]={v.X,v.Y,v.Z,1.0};double value=0.0;
 for(unsigned int i=0;i<4;++i)for(unsigned int j=0;j<4;++j)value+=p[i]*matrix[i][j]*p[j];return value;
}
int main(){
 typedef QuadricOptimiserInternals::CQuadric Quadric;
 if(sizeof(Quadric)!=88||sizeof(C3DVector)!=12)return 1;
 union Storage{double align;unsigned char bytes[88+32];}quadStorage;
 union VectorStorage{double align;unsigned char bytes[12+32];}vectorStorage;
 unsigned int cases=0;
 for(unsigned int pattern=0;pattern<8;++pattern)for(unsigned int seed=0;seed<256;++seed)for(unsigned int point=0;point<64;++point){
  memset(quadStorage.bytes,pattern*31,sizeof(quadStorage.bytes));memset(vectorStorage.bytes,pattern*31,sizeof(vectorStorage.bytes));
  Quadric*q=new(quadStorage.bytes+16)Quadric;C3DVector*v=new(vectorStorage.bytes+16)C3DVector;
  double coeff[11];coeff[0]=((int)(seed%9)-4)*0.5;coeff[1]=((int)((seed/3)%9)-4)*0.25;coeff[2]=((int)((seed/5)%9)-4)*0.5;coeff[3]=((int)((seed/7)%9)-4)*0.25;
  coeff[4]=((int)((seed/11)%9)-4)*0.5;coeff[5]=((int)((seed/13)%9)-4)*0.25;coeff[6]=((int)((seed/17)%9)-4)*0.5;
  coeff[7]=((int)((seed/19)%9)-4)*0.25;coeff[8]=((int)((seed/23)%9)-4)*0.5;coeff[9]=((int)((seed/29)%9)-4)*0.25;coeff[10]=(double)(seed*point);
  memcpy(q,coeff,sizeof(coeff));
  v->X=((int)(point%4)-2)*0.5f;v->Y=((int)((point/4)%4)-2)*0.25f;v->Z=((int)((point/16)%4)-2)*0.5f;
  unsigned char beforeQuad[88],beforeVector[12];memcpy(beforeQuad,q,88);memcpy(beforeVector,v,12);
  double value=q->EvaluateError(*v);if(value!=reference(*q,*v))return 2;
  if(memcmp(beforeQuad,q,88)||memcmp(beforeVector,v,12))return 3;
  for(unsigned int i=0;i<16;++i)if(quadStorage.bytes[i]!=pattern*31||quadStorage.bytes[104+i]!=pattern*31||vectorStorage.bytes[i]!=pattern*31||vectorStorage.bytes[28+i]!=pattern*31)return 4;
  ++cases;
 }
 // Isolate every coefficient and demonstrate that Weight does not enter the form.
 for(unsigned int coefficient=0;coefficient<11;++coefficient)for(unsigned int point=0;point<64;++point){
  Quadric q;double coeff[11]={0};coeff[coefficient]=1;memcpy(&q,coeff,sizeof(coeff));
  C3DVector v;v.X=(float)((int)(point%4)-2);v.Y=(float)((int)((point/4)%4)-2);v.Z=(float)((int)((point/16)%4)-2);
  if(q.EvaluateError(v)!=reference(q,v))return 5;++cases;
 }
 if(cases!=131776)return 6;puts("00ac6d50_TEST PASS");return 0;
}