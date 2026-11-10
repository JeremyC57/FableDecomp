// Bilinear interpolation behavior, with exactly representable values.
#include <stdio.h>
#include <string.h>
class C2DVector {public:float X,Y;C2DVector(){} C2DVector(float x,float y):X(x),Y(y){} C2DVector(const C2DVector& p):X(p.X),Y(p.Y){}};
class CFloatQuad {public:float m_corner[4];float Interpolate(float u,float v)const;};
float CFloatQuad::Interpolate(float u,float v)const
{
    C2DVector top(m_corner[0],m_corner[1]);
    C2DVector bottom(m_corner[2],m_corner[3]);
    C2DVector edge;
    edge.X=(bottom.Y-bottom.X)*u+bottom.X;
    edge.Y=(top.Y-top.X)*u+top.X;
    return(edge.Y-edge.X)*v+edge.X;
}

int main(){
 const float values[]={-4.0f,0.0f,4.0f};
 const float parameters[]={-0.5f,0.0f,0.25f,0.5f,1.0f,1.5f};
 int count=0;
 for(int a=0;a<3;++a)for(int b=0;b<3;++b)for(int c=0;c<3;++c)for(int d=0;d<3;++d){
  CFloatQuad q;q.m_corner[0]=values[a];q.m_corner[1]=values[b];q.m_corner[2]=values[c];q.m_corner[3]=values[d];
  CFloatQuad before=q;
  for(int i=0;i<6;++i)for(int j=0;j<6;++j){
   float u=parameters[i],v=parameters[j];
   float expected=values[a]*(1.0f-u)*v+values[b]*u*v+values[c]*(1.0f-u)*(1.0f-v)+values[d]*u*(1.0f-v);
   if(q.Interpolate(u,v)!=expected||memcmp(&q,&before,sizeof(q))!=0)return 1;
   ++count;
  }
 }
 if(count!=2916)return 2;
 puts("00c462e0_TEST PASS");return 0;
}