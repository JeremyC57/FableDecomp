// Self-contained test of the real shared classes and full target body.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
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
// 00a17f80 CPhysicsMesh::GetPointIntersectionFromToUsingFullCollision
// Inline-budget: native00a17ff0..a1820b expands the same original HasCollidedWithTriangleAt; use its existing opt-in, whose plain/flag/depth probes are recorded for00a177a0.
// Flags: none
// Leaf: ?GetPointIntersectionFromToUsingFullCollision@CPhysicsMesh@@ABE_NABVC3DVector@@0AAVCMeshIntersectionData@@@Z
// Retail:0x00a17f80,818bytes. Identity: both original PDBs and caller00a187ac.
#define FABLE_RETAIL_CPHYSICSMESH_FULL_COLLISION_PASS23
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
#define FABLE_RETAIL_CPLANE_NORMAL_CTOR_PASS23
#define FABLE_RETAIL_CPLANE_LINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_ALL_GROUP_PASS23
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
#include "engine/CPhysicsMesh.h"
#include "engine/CPlane.h"
// Original name and fastcall signature in both PDBs; native call00a18206.
bool FABLE_FASTCALL GFIsPointInTriangle(const C3DVector&,const C3DVector&,
    const C3DVector&,const C3DVector&,const C3DVector&);

// FableWin01d9e1b0 and the original03199a42 call identify this real inline.
inline float FABLE_FASTCALL GFDotProduct(const C3DVector& a,const C3DVector& b)
{
    float result=a.X*b.X+a.Y*b.Y+a.Z*b.Z;
    return result;
}

FABLE_FORCEINLINE bool CPhysicsMesh::HasCollidedWithTriangleAt(const C3DTriangle2& face,
    const C3DVector& from,const C3DVector& direction,float length,
    C3DVector& hit,C3DVector& hitNormal) const
{
    C3DVector unused;
    const C3DVector& a=TotalPoints[face.GetA()];
    const C3DVector& b=TotalPoints[face.GetB()];
    const C3DVector& c=TotalPoints[face.GetC()];
    C3DVector normal=(b-a).Cross(c-a);
    normal.Normalise();
    float planeDistance=a.Dot(normal);
    CPlane plane(normal,planeDistance);
    float facing=GFDotProduct(normal,direction);
    if(facing>0.0f) {
        if(plane.GetIntersectionWithLine(from,direction,&hit)) {
            float along=direction.Dot(hit-from);
            if(along>-DELTA_NOTIONAL_ZERO && along<length+DELTA_NOTIONAL_ZERO) {
                if(GFIsPointInTriangle(a,b,c,-normal,hit)) {
                    hitNormal=normal;
                    return true;
                }
            }
        }
    }
    return false;
}

bool CPhysicsMesh::GetPointIntersectionFromToUsingFullCollision(const C3DVector& from,
    const C3DVector& to,CMeshIntersectionData& data) const
{
    C3DVector hit;
    C3DVector normal;
    float nearest=1e10f;
    long count=TotalFaceIndices.size();
    bool found=false;
    C3DVector direction=to-from;
    float length=direction.GetMagnitudeAndNormalise();
    for(long i=0;i<count;++i) {
        const C3DTriangle2& face=TotalFaceIndices[i];
        if(HasCollidedWithTriangleAt(face,from,direction,length,hit,normal)) {
            float distanceSquared=(hit-from).GetSquaredMagnitude();
            if(distanceSquared<nearest) {
                found=true;
                data.Normal=normal;
                data.DistanceSquared=distanceSquared;
                data.IntersectionPosition=hit;
                nearest=distanceSquared;
            }
        }
    }
    return found;
}

#undef private
#undef protected

const float DELTA_NOTIONAL_ZERO=0.0000999999974737875163555145263671875f;
unsigned long GFastOneOverSqrtTable[128];
static std::vector<long> CollisionFaces;
static long FaceList[12];
static unsigned int FaceCount;
static unsigned int SegmentCalls,PlaneCalls,TriangleCalls,NormaliseCalls;
static const CPhysicsMesh* ExpectedMesh;
static const C3DVector* ExpectedFrom;
static const C3DVector* ExpectedTo;
static bool RejectPlanes,RejectTriangles;
static void InitialiseTable(){
 assert(sizeof(unsigned int)==4 && sizeof(float)==4);
 // Generate the seed table mathematically, following original initializer00a0db60.
 for(unsigned int i=0;i<128;++i){
  unsigned int bits=(i|0x1f80u)<<17;float value;
  memcpy(&value,&bits,4);float inverse=(float)(1.0/sqrt((double)value));
  memcpy(&bits,&inverse,4);GFastOneOverSqrtTable[i]=((bits+0x2000u)>>15)&0xffu;
 }
 // Original initializer00a0dbb0 patches the power-of-two seed.
 GFastOneOverSqrtTable[64]=255;
 GFastOneOverSqrtTable[64]=255;
}
void CIVCountedPointeeBase::Delete()const{abort();}
CPhysicsMesh::~CPhysicsMesh(){}
CIPhysicsFacesSegmentBase::~CIPhysicsFacesSegmentBase(){}
class FixtureSegment:public CIPhysicsFacesSegmentBase {
public:
 bool HasChildren()const{abort();}
 long Sizeof()const{abort();}
 long GetNoFaces()const{abort();}
 void GetPossibleCollisionSet(float,const C3DVector&,const C3DVector&,std::vector<long>&)const{abort();}
};
std::vector<long>& CPhysicsMesh::GetSingleInstanceCollisionSet()const{
 assert(this==ExpectedMesh);CollisionFaces._M_finish=CollisionFaces._M_start;return CollisionFaces;
}
void CIPhysicsFacesSegmentBase::GetCollidedWithTriangles(const CPhysicsMesh* mesh,float radius,
 const C3DVector& from,const C3DVector& to,std::vector<long>& faces)const{
 ++SegmentCalls;assert(mesh==ExpectedMesh && radius==0.0f && &from==ExpectedFrom && &to==ExpectedTo);
 assert(&faces==&CollisionFaces && faces._M_finish==faces._M_start);
 faces._M_start=FaceList;faces._M_finish=FaceList+FaceCount;
 faces._M_end_of_storage._M_data=FaceList+12;
}
float C3DVector::GetMagnitudeAndNormalise(){
 ++NormaliseCalls;float length=(float)sqrt((double)X*X+(double)Y*Y+(double)Z*Z);
 assert(length>0.0f);X/=length;Y/=length;Z/=length;return length;
}
bool CPlane::GetIntersectionWithLine(const C3DVector& from,const C3DVector& direction,C3DVector* out)const{
 ++PlaneCalls;assert(out);if(RejectPlanes)return false;
 double d=(double)Normal.X*direction.X+(double)Normal.Y*direction.Y+(double)Normal.Z*direction.Z;
 if(d==0.0)return false;
 double q=(Distance-(double)Normal.X*from.X-(double)Normal.Y*from.Y-(double)Normal.Z*from.Z)/d;
 out->X=(float)(from.X+q*direction.X);out->Y=(float)(from.Y+q*direction.Y);out->Z=(float)(from.Z+q*direction.Z);return true;
}
bool FABLE_FASTCALL GFIsPointInTriangle(const C3DVector& a,const C3DVector& b,const C3DVector& c,
 const C3DVector& normal,const C3DVector& point){
 ++TriangleCalls;if(RejectTriangles)return false;
 double u[3]={b.X-a.X,b.Y-a.Y,b.Z-a.Z},v[3]={c.X-a.X,c.Y-a.Y,c.Z-a.Z};
 double w[3]={point.X-a.X,point.Y-a.Y,point.Z-a.Z};
 double uu=0,vv=0,uv=0,wu=0,wv=0;
 for(int i=0;i<3;++i){uu+=u[i]*u[i];vv+=v[i]*v[i];uv+=u[i]*v[i];wu+=w[i]*u[i];wv+=w[i]*v[i];}
 assert((u[1]*v[2]-u[2]*v[1])*normal.X+(u[2]*v[0]-u[0]*v[2])*normal.Y+(u[0]*v[1]-u[1]*v[0])*normal.Z<0);
 double denom=uu*vv-uv*uv;assert(denom>0);
 double x=(wu*vv-wv*uv)/denom,y=(wv*uu-wu*uv)/denom;
 return x>=0 && y>=0 && x+y<=1;
}
static void SetVector(C3DVector& v,int axis,float u,float w,float depth){
 float a[3]={0,0,0};a[axis]=depth;a[(axis+1)%3]=u;a[(axis+2)%3]=w;v.X=a[0];v.Y=a[1];v.Z=a[2];
}
static void FillTriangles(C3DVector* points,C3DTriangle2* faces,int axis,float scale,int orientation){
 for(int i=0;i<3;++i){float z=(float)(i-1)*2.0f;
  SetVector(points[i*3],axis,0,0,z);
  SetVector(points[i*3+1],axis,2*scale,0,z);
  SetVector(points[i*3+2],axis,0,2*scale,z);
  faces[i].VertexIndex[0]=(unsigned short)(i*3);
  faces[i].VertexIndex[1]=(unsigned short)(i*3+(orientation>0?1:2));
  faces[i].VertexIndex[2]=(unsigned short)(i*3+(orientation>0?2:1));
 }
}
static float Component(const C3DVector& v,int axis){return axis==0?v.X:axis==1?v.Y:v.Z;}
int main(){
 InitialiseTable();CPhysicsMesh mesh;FixtureSegment segment;C3DVector points[9];C3DTriangle2 faces[3];
 mesh.TotalPoints._M_start=points;mesh.TotalPoints._M_finish=points+9;mesh.TotalPoints._M_end_of_storage._M_data=points+9;
 mesh.TotalFaceIndices._M_start=faces;mesh.TotalFaceIndices._M_finish=faces+3;mesh.TotalFaceIndices._M_end_of_storage._M_data=faces+3;
 mesh.RootSegment.Data=&segment;mesh.RootSegment.Info=0;ExpectedMesh=&mesh;
 unsigned int cases=0;
 for(int axis=0;axis<3;++axis)for(int orientation=-1;orientation<=1;orientation+=2)
 for(int sc=0;sc<3;++sc)for(int reverse=0;reverse<2;++reverse)
 for(int pos=0;pos<4;++pos)for(int ray=0;ray<4;++ray)for(int count=0;count<=3;++count)
 for(int reject=0;reject<3;++reject){
  float scale=sc==0?0.5f:sc==1?1.0f:2.0f;FillTriangles(points,faces,axis,scale,orientation);
  static const float transverse[4]={0.25f,0.5f,1.0f,2.5f};float u=transverse[pos]*scale,w=0.25f*scale;
  static const float starts[4]={-4.0f,4.0f,-1.0f,0.5f};static const float ends[4]={4.0f,-4.0f,1.0f,3.0f};
  C3DVector from,to;SetVector(from,axis,u,w,starts[ray]);SetVector(to,axis,u,w,ends[ray]);
  ExpectedFrom=&from;ExpectedTo=&to;FaceCount=(unsigned int)count;RejectPlanes=reject==1;RejectTriangles=reject==2;
  for(int i=0;i<count;++i)FaceList[i]=reverse?2-i:i;
  C3DTriangle2 savedFaces[3];
  for(int k=0;k<3;++k)for(int c=0;c<3;++c)savedFaces[k].VertexIndex[c]=faces[k].VertexIndex[c];
  for(int k=0;k<count;++k)for(int c=0;c<3;++c)faces[k].VertexIndex[c]=savedFaces[FaceList[k]].VertexIndex[c];
  mesh.TotalFaceIndices._M_finish=faces+count;
  SegmentCalls=PlaneCalls=TriangleCalls=NormaliseCalls=0;CMeshIntersectionData got;
  got.IntersectionPosition=C3DVector(17,19,23);got.Normal=C3DVector(29,31,37);got.DistanceSquared=41;
  // Independent axis-plane oracle: no vector helpers or triangle implementation reused.
  bool expect=false;double nearest=1e10,depth=0;float dz=ends[ray]-starts[ray];
  if(orientation*dz>0 && u>=0 && w>=0 && u+w<=2*scale && reject==0){
   for(int i=0;i<count;++i){double z=(FaceList[i]-1)*2.0;double fraction=(z-starts[ray])/dz;
    if(fraction>=0 && fraction<=1){double dist=(z-starts[ray])*(z-starts[ray]);if(dist<nearest){nearest=dist;depth=z;expect=true;}}
   }
  }
  bool actual=mesh.GetPointIntersectionFromToUsingFullCollision(from,to,got);
  assert(actual==expect && SegmentCalls==0 && NormaliseCalls==1);
  if(orientation*dz<=0 || count==0)assert(PlaneCalls==0 && TriangleCalls==0);
  if(RejectPlanes)assert(TriangleCalls==0);
  if(expect){
   assert(fabs(got.DistanceSquared-nearest)<0.0001);
   assert(fabs(Component(got.IntersectionPosition,axis)-depth)<0.0001);
   assert(fabs(Component(got.IntersectionPosition,(axis+1)%3)-u)<0.0001);
   assert(fabs(Component(got.IntersectionPosition,(axis+2)%3)-w)<0.0001);
   assert(fabs(Component(got.Normal,axis)-orientation)<0.00002);
   assert(Component(got.Normal,(axis+1)%3)==0 && Component(got.Normal,(axis+2)%3)==0);
  }else{assert(got.DistanceSquared==41 && got.IntersectionPosition.X==17 && got.IntersectionPosition.Y==19 && got.IntersectionPosition.Z==23 && got.Normal.X==29 && got.Normal.Y==31 && got.Normal.Z==37);}
  assert(Component(from,axis)==starts[ray] && Component(to,axis)==ends[ray]);++cases;
 }
 assert(cases==6912);
 // Borrowed fixture storage must not be freed by the genuine container destructors.
 mesh.TotalPoints._M_start=mesh.TotalPoints._M_finish=0;mesh.TotalPoints._M_end_of_storage._M_data=0;
 mesh.TotalFaceIndices._M_start=mesh.TotalFaceIndices._M_finish=0;mesh.TotalFaceIndices._M_end_of_storage._M_data=0;
 CollisionFaces._M_start=CollisionFaces._M_finish=0;CollisionFaces._M_end_of_storage._M_data=0;
 puts("00a17f80_TEST PASS");return 0;
}