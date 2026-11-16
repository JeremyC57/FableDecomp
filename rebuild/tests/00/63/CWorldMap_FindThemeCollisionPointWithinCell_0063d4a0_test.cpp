// Self-contained behavioral fixture, using the real shared class layouts.
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
#include <strings.h>
#define _stricmp strcasecmp
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLDMAP_CTOR
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CDRAWMAPINFO_PASS23
#define private public
#define protected public
// 0063d4a0 CWorldMap::FindThemeCollisionPointWithinCell
// Flags: none
// Leaf: ?FindThemeCollisionPointWithinCell@CWorldMap@@IBE_NPBVC3DVector@@00PAV2@1@Z
#define FABLE_RETAIL_CWORLDMAP_COLLISION_CORNERS_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CPLANE_LINE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINESEGMENT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CPlane.h"
#include "engine/CWorldMap.h"


// Original FableWin01d9e1b0; donor024a8b8c calls its fastcall thunk.
inline float FABLE_FASTCALL GFDotProduct(const C3DVector& a,const C3DVector& b)
{
    float result=a.X*b.X+a.Y*b.Y+a.Z*b.Z;
    return result;
}

bool CWorldMap::FindThemeCollisionPointWithinCell(const C3DVector* start_pos, const C3DVector* end_pos, const C3DVector* cell_pos, C3DVector* total_vector, C3DVector* collision_pos) const
{
    C3DVector corner_pos[4];
    CPlane plane;
    C3DVector vector_normal;
    C3DVector plane_normal;
    C3DVector int_cell_pos;
    vector_normal = *total_vector;
    vector_normal.Normalise();
    int_cell_pos.X = (float)(long)cell_pos->X;
    int_cell_pos.Y = (float)(long)cell_pos->Y;
    for (long c1 = 0; c1 < 4; ++c1) {
        corner_pos[c1].X = (float)LineCollisionAround4Square[c1].X + int_cell_pos.X;
        corner_pos[c1].Y = (float)LineCollisionAround4Square[c1].Y + int_cell_pos.Y;
        if (IsPosInMap(corner_pos[c1]))
            corner_pos[c1].Z = GetThemeSizeZAt(corner_pos[c1]);
        else
            corner_pos[c1].Z = 0.0f;
    }
    long base_x = (long)int_cell_pos.X;
    long base_y = (long)int_cell_pos.Y;
    if ((base_x + base_y) % 2 == 1) {
        plane.Initialise(corner_pos[0], corner_pos[1], corner_pos[2]);
        plane_normal = plane.GetNormal();
        bool facing = GFDotProduct(plane_normal,vector_normal) > 0.0f;
        if (facing
            && plane.GetIntersectionWithLine(*start_pos, vector_normal, collision_pos) == true
            && PointIsWithinLineSegment(collision_pos, start_pos, end_pos) == true
            && PointIsWithinCellTR(collision_pos, &int_cell_pos) == true)
            return true;
        plane.Initialise(corner_pos[0], corner_pos[2], corner_pos[3]);
        plane_normal = plane.GetNormal();
        facing = GFDotProduct(plane_normal,vector_normal) > 0.0f;
        if (facing
            && plane.GetIntersectionWithLine(*start_pos, vector_normal, collision_pos) == true
            && PointIsWithinLineSegment(collision_pos, start_pos, end_pos) == true
            && PointIsWithinCellBL(collision_pos, &int_cell_pos) == true)
            return true;
    } else {
        plane.Initialise(corner_pos[0], corner_pos[1], corner_pos[3]);
        plane_normal = plane.GetNormal();
        bool facing = GFDotProduct(plane_normal,vector_normal) > 0.0f;
        if (facing
            && plane.GetIntersectionWithLine(*start_pos, vector_normal, collision_pos) == true
            && PointIsWithinLineSegment(collision_pos, start_pos, end_pos) == true
            && PointIsWithinCellTL(collision_pos, &int_cell_pos) == true)
            return true;
        plane.Initialise(corner_pos[1], corner_pos[2], corner_pos[3]);
        plane_normal = plane.GetNormal();
        facing = GFDotProduct(plane_normal,vector_normal) > 0.0f;
        if (facing
            && plane.GetIntersectionWithLine(*start_pos, vector_normal, collision_pos) == true
            && PointIsWithinLineSegment(collision_pos, start_pos, end_pos) == true
            && PointIsWithinCellBR(collision_pos, &int_cell_pos) == true)
            return true;
    }
    return false;
}

#include "engine/CWorld.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CDrawMapInfo.h"
// The fixture never inserts objects into these three containers. Specialize
// only their empty cleanup, so forward-declared element types remain genuine.
namespace boost {
template<> scoped_ptr<NThingMap::CThingMapManager>::~scoped_ptr() { assert(!ptr); }
}
namespace std {
template<> _Vector_base<CMapInfo,allocator<CMapInfo> >::~_Vector_base() { assert(!_M_start); }
template<> vector<CMapInfo>::~vector() { assert(!_M_start && !_M_finish); }
template<> _Vector_base<CRegion,allocator<CRegion> >::~_Vector_base() { assert(!_M_start); }
template<> vector<CRegion>::~vector() { assert(!_M_start && !_M_finish); }
template<> _Vector_base<CWorldMap::CPlayerPositionInfo,allocator<CWorldMap::CPlayerPositionInfo> >::~_Vector_base() { assert(!_M_start); }
template<> vector<CWorldMap::CPlayerPositionInfo>::~vector() { assert(!_M_start && !_M_finish); }
}
#undef private
#undef protected
// Unrelated lifecycle functions are test doubles, never production helpers.
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CInitBaseClass::CInitBaseClass() : Valid(false) {}
CInitBaseClass::~CInitBaseClass() {}
CGame::CGame() : CurrentGameComponent(0),Quit(false) { memset(ParameterBuffer,0,sizeof(ParameterBuffer)); }
CGame::~CGame() {}
CGameComponent::CGameComponent(CGame& game) : Quit(false),Running(false),Game(game) {}
CGameComponent::~CGameComponent() {}
void CGameComponent::Init() {}
void CGameComponent::SetQuit() { Quit=true; }
CMainGameComponent::CMainGameComponent(CGame& game,const CMainGameComponentInit&) : CGameComponent(game) {}
CMainGameComponent::~CMainGameComponent() {}
bool CMainGameComponent::Run(CGameComponent**) { abort();return false; }
void CMainGameComponent::ChangeTextureColourDepth(long) { abort(); }
void CMainGameComponent::OnPreDeviceReset() { abort(); }
bool CMainGameComponent::OnPostDeviceReset() { abort();return false; }
CMainGameComponentInit::CMainGameComponentInit() {}
CMainGameComponentInit::~CMainGameComponentInit() {}
CCharString::CCharString() : PStringData(0) {}
CCharString::CCharString(const CCharString& x) : PStringData(x.PStringData) { assert(!PStringData); }
CCharString::~CCharString() { assert(!PStringData); }
CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() { assert(!PStringData); }
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { assert(!PData); }
CIDrawWorldMap::~CIDrawWorldMap() {}
CWorldMap::CWorldMap(const CMainGameComponent& component,CWorld& world,const C2DBoxI& area,unsigned long)
 : World(world),Component(component),Granularity(1),GranularityShift(0),Area(area) {}
CWorldMap::~CWorldMap() {}
void CWorldMap::DeleteAllLevels() { abort(); }
void CWorldMap::DeleteAllRegions() { abort(); }
void CWorldMap::LoadFromFile(CAFile&, long) { abort(); }
void CWorldMap::SaveToFile(CAFile&) { abort(); }
void CWorldMap::RemoveLevel(long) { abort(); }
void CWorldMap::LoadTopologyFromFile(long, CCountedPointer<CAFile>, CASuspendableProcess&) { abort(); }
void CWorldMap::PostLoadTopology(long) { abort(); }
void CWorldMap::ForceLoad(long) { abort(); }
void CWorldMap::ForceUnload(long) { abort(); }
bool CWorldMap::IsMapLoaded(long) const { abort(); }
bool CWorldMap::IsMapUsed(long) const { abort(); }
long CWorldMap::GetGranularity() const { abort(); }
const C2DBoxI& CWorldMap::GetDimensions() const { abort(); }
CMap& CWorldMap::GetMapAt(const C2DCoordI&) { abort(); }
const CMap& CWorldMap::PeekMapAt(const C2DCoordI&) const { abort(); }
long CWorldMap::GetMapNumberAt(const C2DCoordI&) const { abort(); }
long CWorldMap::GetMapNumberAt(const C3DVector&) const { abort(); }
CMap& CWorldMap::GetMap(long) { abort(); }
const CMap& CWorldMap::PeekMap(long) const { abort(); }
void CWorldMap::GetAllMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const { abort(); }
void CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const { abort(); }
void CWorldMap::PostRegionLoad(long) { abort(); }
C2DCoordI CWorldMap::GetMapPos(long) const { abort(); }
bool CWorldMap::IsMapChangeable(long) const { abort(); }
bool CWorldMap::IsPosChangeable(const C2DCoordI&) const { abort(); }
std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> > CWorldMap::GetLoadableSections() const { abort(); }
void CWorldMap::Update() { abort(); }
void CWorldMap::HeightAreaChanged(const C2DBoxI*) { abort(); }
long CWorldMap::DrawGetFrame() const { abort(); }
long CWorldMap::DrawGetNoMaps() const { abort(); }
CDrawMapInfo CWorldMap::DrawGetMapInfo(long) const { abort(); }
C2DBoxI CWorldMap::DrawGetDimensions() const { abort(); }
long CWorldMap::DrawGetGranularity() const { abort(); }
bool CWorldMap::DrawIsPosInMap(const C3DVector&) const { abort(); }
float CWorldMap::DrawGetSizeZAt(const C3DVector&, float) const { abort(); }
C3DVector CWorldMap::DrawConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) const { abort(); }
long CWorldMap::DrawGetMyPlayerMapNumber() const { abort(); }
bool CWorldMap::DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const { abort(); }
bool CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const { abort(); }
void CWorldMap::LoadScriptBrushesFromFile(long, CCountedPointer<CAFile>) { abort(); }
long CWorldMap::AllocateNewMap() { abort(); }

const C2DCoordI CWorldMap::LineCollisionAround4Square[4]={C2DCoordI(0,1),C2DCoordI(1,1),C2DCoordI(1,0),C2DCoordI(0,0)};
unsigned long GFastOneOverSqrtTable[128];
static int CellX,CellY,InMapMask,HeightPattern,ReadCount,HeightCount,PlaneCount,LineCount;
static int TriangleOrder[2],PredicateOrder[2],PredicateCount;
static C3DVector ExpectedStart,ExpectedEnd;
static const CWorldMap* ExpectedMap;
static bool RejectLine,RejectSegment,RejectCell;
static int CornerIndex(float x,float y) {
 int dx=(int)x-CellX,dy=(int)y-CellY;
 assert((dx==0 || dx==1) && (dy==0 || dy==1));
 return dy?(dx?1:0):(dx?2:3);
}
static float CornerHeight(int i) { return (float)(((HeightPattern >> i)&1)?2:0); }
bool CWorldMap::IsPosInMap(const C3DVector& p) const {
 assert(this==ExpectedMap);int i=CornerIndex(p.X,p.Y);assert(i==ReadCount++);
 return (InMapMask & (1<<i))!=0;
}
float CWorldMap::GetThemeSizeZAt(const C3DVector& p) const {
 assert(this==ExpectedMap);int i=CornerIndex(p.X,p.Y);assert(InMapMask&(1<<i));++HeightCount;return CornerHeight(i);
}
bool CPlane::Initialise(const C3DVector& a,const C3DVector& b,const C3DVector& c) {
 assert(PlaneCount<2);int ia=CornerIndex(a.X,a.Y),ib=CornerIndex(b.X,b.Y),ic=CornerIndex(c.X,c.Y);
 int code=ia*16+ib*4+ic;TriangleOrder[PlaneCount++]=code;
 const C3DVector* v[3]={&a,&b,&c};int ix[3]={ia,ib,ic};
 for(int i=0;i<3;++i)assert(v[i]->Z==((InMapMask&(1<<ix[i]))?CornerHeight(ix[i]):0.0f));
 float ux=b.X-a.X,uy=b.Y-a.Y,uz=b.Z-a.Z,vx=c.X-a.X,vy=c.Y-a.Y,vz=c.Z-a.Z;
 Normal.X=uy*vz-uz*vy;Normal.Y=uz*vx-ux*vz;Normal.Z=ux*vy-uy*vx;
 // An unscaled plane equation has exactly the same intersection contract.
 Distance=Normal.X*a.X+Normal.Y*a.Y+Normal.Z*a.Z;return true;
}
bool CPlane::GetIntersectionWithLine(const C3DVector& point,const C3DVector& direction,C3DVector* out) const {
 ++LineCount;assert(point.X==ExpectedStart.X && point.Y==ExpectedStart.Y && point.Z==ExpectedStart.Z);
 float square=direction.X*direction.X+direction.Y*direction.Y+direction.Z*direction.Z;
 assert(square>0.995f && square<1.005f);
 if(RejectLine)return false;
 float divisor=Normal.X*direction.X+Normal.Y*direction.Y+Normal.Z*direction.Z;
 assert(divisor>0.0f);
 float t=(Distance-(Normal.X*point.X+Normal.Y*point.Y+Normal.Z*point.Z))/divisor;
 out->X=point.X+t*direction.X;out->Y=point.Y+t*direction.Y;out->Z=point.Z+t*direction.Z;return true;
}
bool CWorldMap::PointIsWithinLineSegment(const C3DVector* p,const C3DVector* a,const C3DVector* b) const {
 assert(this==ExpectedMap);assert(a->X==ExpectedStart.X && b->X==ExpectedEnd.X);
 return !RejectSegment && p->X >= (a->X<b->X?a->X:b->X) && p->X <= (a->X>b->X?a->X:b->X)
   && p->Y >= (a->Y<b->Y?a->Y:b->Y) && p->Y <= (a->Y>b->Y?a->Y:b->Y);
}
static bool CellTest(const C3DVector* p,const C3DVector* c,int kind) {
 assert((int)c->X==CellX && (int)c->Y==CellY);assert(PredicateCount<2);PredicateOrder[PredicateCount++]=kind;
 if(RejectCell || (long)p->X!=CellX || (long)p->Y!=CellY)return false;
 float x=p->X-CellX,y=p->Y-CellY;
 if(kind==0)return x+y>1.0f;
 if(kind==1)return x+y<=1.0f;
 if(kind==2)return x<y;
 return x>=y;
}
bool CWorldMap::PointIsWithinCellTR(const C3DVector* p,const C3DVector* c) const { return CellTest(p,c,0); }
bool CWorldMap::PointIsWithinCellBL(const C3DVector* p,const C3DVector* c) const { return CellTest(p,c,1); }
bool CWorldMap::PointIsWithinCellTL(const C3DVector* p,const C3DVector* c) const { return CellTest(p,c,2); }
bool CWorldMap::PointIsWithinCellBR(const C3DVector* p,const C3DVector* c) const { return CellTest(p,c,3); }
static bool Reference(double x,double y,int sign) {
 // Vertical rays meet both triangles' planes. Acceptance is the cell's
 // half-open integer bounds and strict/non-strict diagonal partition.
 if(sign>0 || RejectLine || RejectSegment || RejectCell)return false;
 return (long)x==CellX && (long)y==CellY;
}
static double Surface(double x,double y) {
 double h[4];for(int i=0;i<4;++i)h[i]=(InMapMask&(1<<i))?CornerHeight(i):0;
 if((CellX+CellY)%2==1) {
  if(x+y>1)return (1-x)*h[0]+(x+y-1)*h[1]+(1-y)*h[2];
  return y*h[0]+x*h[2]+(1-x-y)*h[3];
 }
 if(x<y)return (y-x)*h[0]+x*h[1]+(1-y)*h[3];
 return y*h[1]+(x-y)*h[2]+(1-x)*h[3];
}
static bool ObliqueReference(const C3DVector& from,const C3DVector& end,const C3DVector& direction,double out[3]) {
 // Independent affine-height equations, derived from three corner samples.
 const int odd[2][3]={{0,1,2},{0,2,3}},even[2][3]={{0,1,3},{1,2,3}};
 const int (*tri)[3]=((CellX+CellY)%2==1)?odd:even;
 const double x[4]={0,1,1,0},y[4]={1,1,0,0};double h[4];
 for(int k=0;k<4;++k)h[k]=(InMapMask&(1<<k))?CornerHeight(k):0;
 for(int t=0;t<2;++t) {
  int i=tri[t][0],j=tri[t][1],k=tri[t][2];
  double u=x[j]-x[i],v=y[j]-y[i],w=x[k]-x[i],z=y[k]-y[i],det=u*z-v*w;
  double a=((h[j]-h[i])*z-(h[k]-h[i])*v)/det;
  double b=(u*(h[k]-h[i])-w*(h[j]-h[i]))/det;
  double c=h[i]-a*x[i]-b*y[i];
  double denom=direction.Z-a*direction.X-b*direction.Y;
  if(denom>=0)continue;
  double distance=(a*(from.X-CellX)+b*(from.Y-CellY)+c-from.Z)/denom;
  double px=from.X+distance*direction.X,py=from.Y+distance*direction.Y;
  if(px<(from.X<end.X?from.X:end.X) || px>(from.X>end.X?from.X:end.X)
   ||py<(from.Y<end.Y?from.Y:end.Y) || py>(from.Y>end.Y?from.Y:end.Y))continue;
  if((long)px!=CellX || (long)py!=CellY)continue;
  double dx=px-CellX,dy=py-CellY;
  bool first=((CellX+CellY)%2==1)?dx+dy>1:dx<dy;
  if(first!=(t==0))continue;
  out[0]=px;out[1]=py;out[2]=from.Z+distance*direction.Z;return true;
 }
 return false;
}
int main() {
 for(unsigned int i=0;i<128;++i) {
  unsigned int bits=(i|0x1f80u)<<17;float value;memcpy(&value,&bits,4);
  float inverse=(float)(1.0/sqrt((double)value));memcpy(&bits,&inverse,4);
  GFastOneOverSqrtTable[i]=((bits+0x2000u)>>15)&0xffu;
 }
 // Native initializer00a0dbb0 fixes the exact-power-of-two seed.
 GFastOneOverSqrtTable[64]=255;
 CGame game;CMainGameComponentInit init;CMainGameComponent component(game,init);CWorld world;
 C2DBoxI area;memset(&area,0,sizeof(area));CWorldMap map(component,world,area,0);ExpectedMap=&map;
 unsigned int cases=0;
 for(CellX=0;CellX<2;++CellX)for(CellY=0;CellY<2;++CellY)
 for(InMapMask=0;InMapMask<16;++InMapMask)for(HeightPattern=0;HeightPattern<16;++HeightPattern)
 for(int ix=0;ix<5;++ix)for(int iy=0;iy<5;++iy)for(int sign=-1;sign<=1;sign+=2)
 for(int rejection=0;rejection<4;++rejection) {
  RejectLine=rejection==1;RejectSegment=rejection==2;RejectCell=rejection==3;
  C3DVector cell((float)CellX+0.25f,(float)CellY+0.75f,0.0f);
  float x=(float)CellX+(float)ix*0.25f,y=(float)CellY+(float)iy*0.25f;
  C3DVector from(x,y,4.0f),to(x,y,-4.0f),direction(0.0f,0.0f,(float)sign*8.0f),hit(17,19,23);
  ExpectedStart=from;ExpectedEnd=to;ReadCount=HeightCount=PlaneCount=LineCount=PredicateCount=0;
  bool got=map.FindThemeCollisionPointWithinCell(&from,&to,&cell,&direction,&hit);
  assert(got==Reference(x,y,sign));assert(ReadCount==4);
  int expectedReads=0;for(int j=0;j<4;++j)if(InMapMask&(1<<j))++expectedReads;
  assert(HeightCount==expectedReads);assert(PlaneCount>=1 && PlaneCount<=2);
  bool odd=(CellX+CellY)%2==1;assert(TriangleOrder[0]==(odd?6:7));
  if(PlaneCount==2)assert(TriangleOrder[1]==(odd?11:27));
  if(got){assert(fabs(hit.X-x)<0.00001 && fabs(hit.Y-y)<0.00001);assert(fabs(hit.Z-Surface(x-CellX,y-CellY))<0.00002);}
  if(sign>0 || RejectLine){assert(hit.X==17 && hit.Y==19 && hit.Z==23);}
  assert(direction.X==0 && direction.Y==0 && direction.Z==(float)sign*8.0f);
  ++cases;
 }
 assert(cases==204800u);
 RejectLine=RejectSegment=RejectCell=false;
 unsigned int oblique=0;
 for(CellX=0;CellX<2;++CellX)for(CellY=0;CellY<2;++CellY)
 for(InMapMask=0;InMapMask<16;++InMapMask)for(HeightPattern=0;HeightPattern<16;HeightPattern+=3)
 for(int ray=0;ray<8;++ray) {
  C3DVector from(CellX+0.375f,CellY+0.625f,4.0f);
  C3DVector direction((ray&1)?0.5f:-0.25f,(ray&2)?0.25f:-0.5f,(ray&4)?-8.0f:8.0f);
  C3DVector to(from.X+direction.X,from.Y+direction.Y,from.Z+direction.Z);
  C3DVector cell((float)CellX,(float)CellY,0),hit(17,19,23);
  double expected[3];bool wanted=ObliqueReference(from,to,direction,expected);
  ExpectedStart=from;ExpectedEnd=to;ReadCount=HeightCount=PlaneCount=LineCount=PredicateCount=0;
  bool got=map.FindThemeCollisionPointWithinCell(&from,&to,&cell,&direction,&hit);
  assert(got==wanted);assert(ReadCount==4);
  if(got)assert(fabs(hit.X-expected[0])<0.00002 && fabs(hit.Y-expected[1])<0.00002 && fabs(hit.Z-expected[2])<0.00002);
  ++oblique;
 }
 assert(oblique==3072u);
 puts("0063d4a0_TEST PASS");return 0;
}