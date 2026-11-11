#include <cstdio>
#include <cmath>
#include <cassert>
class C3DVector { public: float X,Y,Z; C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} };
class C4DVector { public: float X,Y,Z,W; C3DVector ToC3DVector()const{return C3DVector(X,Y,Z);} };
class CMatrix4x4 { public: float Data[4][4]; float& operator()(int row,int column){return Data[row][column];} };
inline float GFSqr(float value){return value*value;}
class CQuaternion {public:float X,Y,Z,W;
 void ToMatrix(CMatrix4x4& dest,const C3DVector& translation)const {
  float d=GFSqr(X)+GFSqr(Y)+GFSqr(Z)+GFSqr(W);
  float s=(d!=0.0f)?2.0f/d:1.0f;
  float xs=X*s,ys=Y*s,zs=Z*s;
  float wx=W*xs,wy=W*ys,wz=W*zs;
  float xx=X*xs,xy=X*ys,xz=X*zs;
  float yy=Y*ys,yz=Y*zs,zz=Z*zs;
  dest(0,0)=1.0f-(yy+zz);dest(0,1)=xy-wz;dest(0,2)=xz+wy;dest(0,3)=0.0f;
  dest(1,0)=xy+wz;dest(1,1)=1.0f-(xx+zz);dest(1,2)=yz-wx;dest(1,3)=0.0f;
  dest(2,0)=xz-wy;dest(2,1)=yz+wx;dest(2,2)=1.0f-(xx+yy);dest(2,3)=0.0f;
  dest(3,0)=translation.X;dest(3,1)=translation.Y;dest(3,2)=translation.Z;dest(3,3)=1.0f;
 }
};
class C3DKeyframe {public:CQuaternion Rotation;C4DVector Position,Scaling;
 C3DVector GetPosition()const{return Position.ToC3DVector();}
 void ToMatrix(CMatrix4x4& matrix,bool applyScaling)const;
};
void C3DKeyframe::ToMatrix(CMatrix4x4& matrix,bool applyScaling)const
{
 Rotation.ToMatrix(matrix,GetPosition());
}

int main(){
 const float vals[]={-2.0f,-1.0f,0.0f,0.5f,1.0f,2.0f};
 unsigned int cases=0;
 for(int i=0;i<6;++i)for(int j=0;j<6;++j)for(int k=0;k<6;++k)for(int w=0;w<6;++w)
 for(int pos=0;pos<8;++pos)for(int scaled=0;scaled<2;++scaled){
  C3DKeyframe f;f.Rotation.X=vals[i];f.Rotation.Y=vals[j];f.Rotation.Z=vals[k];f.Rotation.W=vals[w];
  f.Position.X=(pos&1)?4.0f:-4.0f;f.Position.Y=(pos&2)?2.0f:-2.0f;f.Position.Z=(pos&4)?8.0f:-8.0f;f.Position.W=99.0f;
  f.Scaling.X=2.0f;f.Scaling.Y=3.0f;f.Scaling.Z=4.0f;f.Scaling.W=5.0f;
  CMatrix4x4 m;for(int r=0;r<4;++r)for(int c=0;c<4;++c)m.Data[r][c]=123.0f;
  f.ToMatrix(m,scaled!=0);
  double v[3]={vals[i],vals[j],vals[k]},scalar=vals[w];double norm=scalar*scalar;
  for(int n=0;n<3;++n)norm+=v[n]*v[n];double scale=norm!=0.0?2.0/norm:1.0;
  double skew[3][3]={{0,-v[2],v[1]},{v[2],0,-v[0]},{-v[1],v[0],0}};
  for(int r=0;r<3;++r)for(int c=0;c<3;++c){
   double expected=(r==c?1.0:0.0)+scale*(v[r]*v[c]+scalar*skew[r][c]);
   if(r==c)expected-=scale*(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
   assert(std::fabs(m.Data[r][c]-expected)<0.00001);
  }
  assert(m.Data[0][3]==0&&m.Data[1][3]==0&&m.Data[2][3]==0&&m.Data[3][3]==1);
  assert(m.Data[3][0]==f.Position.X&&m.Data[3][1]==f.Position.Y&&m.Data[3][2]==f.Position.Z);
  assert(f.Rotation.X==vals[i]&&f.Rotation.Y==vals[j]&&f.Rotation.Z==vals[k]&&f.Rotation.W==vals[w]);
  assert(f.Scaling.X==2&&f.Scaling.Y==3&&f.Scaling.Z==4&&f.Scaling.W==5&&f.Position.W==99);++cases;
 }
 assert(cases==20736);std::puts("00aa39a0_TEST PASS");return 0;
}