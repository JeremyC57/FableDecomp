// Complete unchanged selected production method; deterministic bodies use the actual shared class models.
#define FABLE_RETAIL_STLP_MAP
// Original Ego_d locals: xp, yp, and one block-local map.
// FTOL_SCOPE_TRAPPER is checked-build-only and generates no release operations.
// Original global: NEngineInternal::GPEngineWorldMap (Anniversary PDB and native engine forwarders).
// Leaf: ?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z
// Inline-budget: original nested GetMap bounds are expanded in retail.
// Flags: /d2inlT301
// Retail-asm: 00B43AFC fld dword ptr [esp+1Ch], 00B43B00 fistp dword ptr [esp+18h]; shared GFFloatToLongNear inside QuantiseLandscapeHeight.
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAP_ORIGINAL_PASS26
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"

namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }

float CEngineMap::PeekLandscapeHeight(long xp, long yp) const
{
    if (!MapHasDataAtPoint(xp,yp)) {
        CEngineMap* map = NEngineInternal::GPEngineWorldMap->GetMap(
            PeekWorldX() + xp,
            PeekWorldY() + yp);
        if (map && map->MapHasDataAtPoint(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY()))
            return map->PeekLandscapeHeight(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY());
        if (xp < 0)
            xp = 0;
        else if (xp > PeekWidth() - 1)
            xp = PeekWidth() - 1;
        if (yp < 0)
            yp = 0;
        else if (yp > PeekHeight() - 1)
            yp = PeekHeight() - 1;
    }
    if (GameMap)
        return QuantiseLandscapeHeight(GameMap->DrawBlockGetSizeZAt(xp, yp, 1.0f));
    return 0.0f;
}

#include "engine/C2DExtentsI.h"
#include "engine/CEngineInit.h"
#include <cstdio>
#include <cstring>
#include <cmath>
// Deterministic dependency bodies; all classes/layouts come from selected shared headers.
CCharString::CCharString() {}
CCharString::CCharString(const CCharString&) {}
CCharString::~CCharString() {}
CWideString::CWideString() {}
CWideString::~CWideString() {}
CEngineMemoryInit::CEngineMemoryInit() {}
CEngineInit::CEngineInit() {}
CEngineInit::~CEngineInit() {}
CEngineComponent::CEngineComponent() {}
CEngineComponent::~CEngineComponent() {}
void CEngineComponent::FrameStart(const CEngineComponentParams&) {}
void CEngineComponent::FrameEnd(const CEngineComponentParams&) {}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&) {return false;}
void CEngineComponent::Render(const CEngineComponentRenderParams&) {}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::WorldFrameUpdate(bool) {}
void CEngineComponent::PausedWorldFrameUpdate() {}
CCharString CEngineComponent::GetConsoleEnableFunctionName() {return CCharString();}
void CEngineComponent::OnReleaseDefaultPoolResources() {}
bool CEngineComponent::OnRestoreDefaultPoolResources() {return true;}
CEngineWorldMap::CEngineWorldMap(const CEngineInit&) : Map(0),MapArrayWidth(0),MapArrayHeight(0),MapOffsetX(0),MapOffsetY(0) {}
CEngineWorldMap::~CEngineWorldMap() {}
unsigned long CEngineWorldMap::PeekSceneFilterFlags() const {return 0;}
CEngineStaticMapFileBlockManager::~CEngineStaticMapFileBlockManager() {}
CEngineMap::CEngineMap(const CIDrawMap* map,long w,long h,long x,long y,long,CCharString,bool)
 : LandscapeMap(0),LocalDetailCacheMap(0),GameMap(map),MapWidth((unsigned short)w),MapHeight((unsigned short)h),WorldPosX((unsigned short)x),WorldPosY((unsigned short)y),EdgeHeights(0) {}
CEngineMap::~CEngineMap() {}
CIDrawMap::~CIDrawMap() {}
long CIDrawMap::DrawGetEngineThemeAt(long,long,long) const {return 0;}
unsigned char CIDrawMap::DrawGetEngineBlendAt(long,long,long) const {return 0;}
bool CIDrawMap::DrawFindLastUnblockedPosition(const C3DVector*,const C3DVector*,C3DVector*) const {return false;}
namespace NEngineInternal { CEngineWorldMap* GPEngineWorldMap=0; }
struct TestDrawMap : CIDrawMap {
 mutable long x,y,layer;mutable int calls;float height;int marker;
 explicit TestDrawMap(int m):x(-999),y(-999),layer(-999),calls(0),height(1.0f),marker(m) {}
 virtual long DrawGetHeightMapSizeX() const {return 16;}
 virtual long DrawGetHeightMapSizeY() const {return 16;}
 virtual C2DExtentsI DrawGetHeightMapExtents() const {return C2DExtentsI(16,16);}
 virtual float DrawGetSizeZAt(const C3DVector*,float) const {return height;}
 virtual float DrawBlockGetSizeZAt(unsigned long a,unsigned long b,float scale) const {x=(long)a;y=(long)b;layer=0;++calls;return height*scale;}
 virtual long DrawGetEngineThemeAt(long a,long b,long c) const {x=a;y=b;layer=c;++calls;return marker+a+b*16+c*256;}
 virtual unsigned char DrawGetEngineBlendAt(long a,long b,long c) const {x=a;y=b;layer=c;++calls;return (unsigned char)(marker+a+b*16+c*7);}
 virtual float DrawGetSeaLevel(float) const {return 0;}
};
static int failed=0;
#define CHECK(x) do {if(!(x)){++failed;std::printf("FAIL %d\n",__LINE__);}}while(0)
static double Query(const CEngineMap& map,long x,long y,long level) {return map.PeekLandscapeHeight(x,y);}
static double Expected(const TestDrawMap& draw,long x,long y,long level) {return draw.height;}
int main() {
 CEngineInit init;CEngineWorldMap world(init);NEngineInternal::GPEngineWorldMap=&world;
 short grid[4]={0,0,0,0};world.Map=grid;world.MapArrayWidth=2;world.MapArrayHeight=2;world.MapOffsetX=64;world.MapOffsetY=96;
 TestDrawMap own(11),other(71);CEngineMap here(&own,16,16,64,96,0,CCharString(),false),there(&other,16,16,80,96,0,CCharString(),false),empty(0,16,16,64,96,0,CCharString(),false);
 world.MapList.push_back((CEngineMap*)0);world.MapList.push_back(&here);world.MapList.push_back(&there);world.MapList.push_back(&empty);
 const long inputs[]={-100,-1,0,1,15,16,30,200};
 for(unsigned i=0;i<8;++i)for(unsigned j=0;j<8;++j) {
  long x=inputs[i],y=inputs[j];long cx=x<0?0:x>15?15:x,cy=y<0?0:y>15?15:y;
  own.calls=other.calls=0;grid[0]=0;double got=Query(here,x,y,3);
  CHECK(got==Expected(own,cx,cy,3));CHECK(own.calls==1&&other.calls==0&&own.x==cx&&own.y==cy);
 }
 grid[0]=2;own.calls=other.calls=0;CHECK(Query(here,20,3,5)==Expected(other,4,3,5));CHECK(other.calls==1&&other.x==4&&other.y==3&&own.calls==0);
 // A resolved map which does not contain the translated point must fall back.
 own.calls=other.calls=0;CHECK(Query(here,31,17,1)==Expected(own,15,15,1));CHECK(own.calls==1&&other.calls==0);
 // Missing grid record, negative grid record and a record with no game map.
 const short missing[]={0,-1,3};
 for(unsigned k=0;k<3;++k){grid[0]=missing[k];own.calls=other.calls=0;CHECK(Query(here,20,2,1)==Expected(own,15,2,1));CHECK(own.calls==1&&other.calls==0);}
 grid[0]=0;own.calls=other.calls=0;CHECK(Query(empty,0,0,1)==0.0);CHECK(own.calls==0&&other.calls==0);
 grid[0]=2;CHECK(Query(empty,20,3,2)==Expected(other,4,3,2));
 // Direct helper boundary coverage also verifies the original offset subtraction.
 CHECK(world.GetMap(63,96)==0&&world.GetMap(64,95)==0&&world.GetMap(128,96)==0&&world.GetMap(64,160)==0);
 grid[0]=2;CHECK(world.GetMap(64,96)==&there);
 const float values[]={0.0f,1.0f,-1.0f,0.00390625f,0.01171875f,-0.00390625f,-0.01171875f};
 const float quantised[]={0.0f,1.0f,-1.0f,0.0f,0.015625f,0.0f,-0.015625f};
 for(unsigned k=0;k<7;++k){own.height=values[k];CHECK(Query(here,2,2,0)==quantised[k]);}
 std::puts(failed?"00b439c0_TEST FAIL":"00b439c0_TEST PASS");return failed?1:0;
}