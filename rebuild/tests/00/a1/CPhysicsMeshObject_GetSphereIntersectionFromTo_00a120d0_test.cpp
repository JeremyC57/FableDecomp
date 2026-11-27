
#include <stdio.h>
#include <math.h>
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator-(const C3DVector& o) const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}
 float GetSquaredMagnitude() const{return X*X+Y*Y+Z*Z;}
 C3DVector& Normalise(){float n=(float)sqrt(GetSquaredMagnitude());X/=n;Y/=n;Z/=n;return *this;}
};
struct CMatrix3x4 {
 float m[3][3],t[3];
 void SetInverse(const CMatrix3x4& b){
  for(int i=0;i<3;++i){float len=0;for(int k=0;k<3;++k)len+=b.m[k][i]*b.m[k][i];t[i]=0;for(int j=0;j<3;++j){m[i][j]=b.m[j][i]/len;t[i]-=m[i][j]*b.t[j];}}
 }
 void TransformVector(C3DVector& v) const{float x=v.X,y=v.Y,z=v.Z;v=C3DVector(m[0][0]*x+m[0][1]*y+m[0][2]*z,m[1][0]*x+m[1][1]*y+m[1][2]*z,m[2][0]*x+m[2][1]*y+m[2][2]*z);}
 void TransformPoint(const C3DVector& v,C3DVector& dst) const {dst=v;TransformVector(dst);dst.X+=t[0];dst.Y+=t[1];dst.Z+=t[2];}
 void TransformPoint(C3DVector& p) const{TransformPoint(p,p);}
};
struct CMeshIntersectionData {C3DVector IntersectionPosition,Normal;float DistanceSquared;};
static C3DVector capturedFrom,capturedTo;static bool hit;static int calls;static float radius;
struct CPhysicsMesh {
 bool Query(const C3DVector& from,const C3DVector& to,CMeshIntersectionData& d)const{++calls;capturedFrom=from;capturedTo=to;if(hit){d.IntersectionPosition=C3DVector(-1,2,3);d.Normal=C3DVector(1,-2,2);}return hit;}
 bool GetPointIntersectionFromTo(const C3DVector& a,const C3DVector& b,CMeshIntersectionData& d)const{return Query(a,b,d);}
 bool GetSphereIntersectionFromTo(float r,const C3DVector& a,const C3DVector& b,CMeshIntersectionData& d)const{radius=r;return Query(a,b,d);}
};
struct CPhysicsMeshObject { const CPhysicsMesh* Mesh;CMatrix3x4 PositionMatrix;float OO_Scale;
 float GetOneOnScale()const{return OO_Scale;}
 bool GetPointIntersectionFromTo(const C3DVector&,const C3DVector&,CMeshIntersectionData&)const;
 bool GetSphereIntersectionFromTo(float,const C3DVector&,const C3DVector&,CMeshIntersectionData&)const;
};
bool CPhysicsMeshObject::GetSphereIntersectionFromTo(float sphere_radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    CMatrix3x4 world_to_mesh;
    C3DVector ms_from, ms_to;
    float ms_sphere_radius = sphere_radius * OO_Scale;
    world_to_mesh.SetInverse(PositionMatrix);
    world_to_mesh.TransformPoint(from, ms_from);
    world_to_mesh.TransformPoint(to, ms_to);
    if (Mesh->GetSphereIntersectionFromTo(ms_sphere_radius, ms_from, ms_to, data)) {
        PositionMatrix.TransformVector(data.Normal);
        data.Normal.Normalise();
        PositionMatrix.TransformPoint(data.IntersectionPosition);
        data.DistanceSquared=(data.IntersectionPosition-from).GetSquaredMagnitude();
        return true;
    }
    return false;
}

static bool nearf(float a,float b){return fabs(a-b)<0.0002;}
static bool eq(const C3DVector& a,const C3DVector& b){return nearf(a.X,b.X)&&nearf(a.Y,b.Y)&&nearf(a.Z,b.Z);}
int main(){
 CPhysicsMesh mesh; CPhysicsMeshObject q;q.Mesh=&mesh;
 for(int r=0;r<3;++r)for(int s=0;s<8;++s)for(int t=-3;t<=3;++t)for(int k=1;k<=4;++k)for(int h=0;h<2;++h){
  CMatrix3x4& m=q.PositionMatrix;float sc[3],tr[3],a[3],b[3],hp[3],hn[3];float lp[3]={-1,2,3},ln[3]={1,-2,2};q.OO_Scale=1.f/k;
  for(int i=0;i<3;++i){sc[i]=float((s&(1<<i))?-k:k);tr[i]=float(t*(i+1));m.t[i]=tr[i];for(int j=0;j<3;++j)m.m[i][j]=0;m.m[i][(r+i)%3]=sc[i];a[i]=tr[i]+sc[i]*float((r+i)%3+1);b[i]=tr[i]+sc[i]*float(2-(r+i)%3);hp[i]=tr[i]+sc[i]*lp[(r+i)%3];hn[i]=sc[i]*ln[(r+i)%3]/(3*k);}
  C3DVector from(a[0],a[1],a[2]),to(b[0],b[1],b[2]);CMeshIntersectionData d;d.IntersectionPosition=C3DVector(9,8,7);d.Normal=C3DVector(-1,-1,-1);d.DistanceSquared=-22;hit=h!=0;calls=0;radius=-1;
  if(q.GetSphereIntersectionFromTo(3.25f,from,to,d)!=hit||calls!=1||!eq(capturedFrom,C3DVector(1,2,3))||!eq(capturedTo,C3DVector(2,1,0)))return 1;
  if(!nearf(radius,3.25f/k))return 4;
  if(!hit){if(!eq(d.IntersectionPosition,C3DVector(9,8,7))||!eq(d.Normal,C3DVector(-1,-1,-1))||d.DistanceSquared!=-22)return 2;continue;}
  float dist=0;for(int z=0;z<3;++z)dist+=(hp[z]-a[z])*(hp[z]-a[z]);if(!eq(d.IntersectionPosition,C3DVector(hp[0],hp[1],hp[2]))||!eq(d.Normal,C3DVector(hn[0],hn[1],hn[2]))||!nearf(d.DistanceSquared,dist))return 3;
 }
 puts("00a120d0_TEST PASS");return 0;
}