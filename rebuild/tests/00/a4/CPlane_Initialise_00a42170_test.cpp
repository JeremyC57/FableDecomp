#include <cstdio>
#include <cmath>
static int gNormalises=0;
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}const C3DVector operator^(const C3DVector& o)const{return C3DVector((Y*o.Z-Z*o.Y),(Z*o.X-X*o.Z),X*o.Y-Y*o.X);}const float Dot(const C3DVector& o)const{return X*o.X+Y*o.Y+Z*o.Z;}C3DVector& AccurateNormalise(){++gNormalises;float f=1.0f/(float)std::sqrt(X*X+Y*Y+Z*Z);X*=f;Y*=f;Z*=f;return *this;}};
class CPlane{public:C3DVector Normal;float Distance;bool Initialise(const C3DVector&,const C3DVector&,const C3DVector&);};
bool CPlane::Initialise(const C3DVector& point1,const C3DVector& point2,const C3DVector& point3)
{
 C3DVector cross1;
 C3DVector cross2;
 cross1=point2-point1;
 cross2=point3-point1;
 cross1.AccurateNormalise();
 cross2.AccurateNormalise();
 Normal=cross1^cross2;
 Normal.AccurateNormalise();
 Distance=point1.Dot(Normal);
 return true;
}

static bool Check(const C3DVector& a,const C3DVector& b,const C3DVector& c){double ux=b.X-a.X,uy=b.Y-a.Y,uz=b.Z-a.Z,vx=c.X-a.X,vy=c.Y-a.Y,vz=c.Z-a.Z;double nx=uy*vz-uz*vy,ny=uz*vx-ux*vz,nz=ux*vy-uy*vx;double m=std::sqrt(nx*nx+ny*ny+nz*nz);nx/=m;ny/=m;nz/=m;double dist=nx*a.X+ny*a.Y+nz*a.Z;CPlane plane;gNormalises=0;bool result=plane.Initialise(a,b,c);double tolerance=1.0/4096.0;return result&&gNormalises==3&&std::fabs(plane.Normal.X-nx)<tolerance&&std::fabs(plane.Normal.Y-ny)<tolerance&&std::fabs(plane.Normal.Z-nz)<tolerance&&std::fabs(plane.Distance-dist)<tolerance;}
int main(){int ok=1,cases=0;for(int x=-3;x<=3;++x)for(int y=-3;y<=3;++y){C3DVector a((float)x,(float)y,2);C3DVector bs[3]={C3DVector((float)x+2,(float)y,2),C3DVector((float)x+1,(float)y+2,4),C3DVector((float)x,(float)y+2,3)};C3DVector cs[3]={C3DVector((float)x,(float)y+2,2),C3DVector((float)x-2,(float)y+1,3),C3DVector((float)x+2,(float)y-1,4)};for(int i=0;i<3;++i){ok &= Check(a,bs[i],cs[i]);ok &= Check(a,cs[i],bs[i]);cases+=2;}}
CPlane alias;alias.Normal=C3DVector(0,0,2);C3DVector b(2,0,2),c(0,2,2);ok &= alias.Initialise(alias.Normal,b,c)&&alias.Normal.Z==1&&alias.Distance==1;++cases;ok &= cases==295;std::printf(ok?"00a42170_TEST PASS\n":"00a42170_TEST FAIL\n");return ok?0:1;}