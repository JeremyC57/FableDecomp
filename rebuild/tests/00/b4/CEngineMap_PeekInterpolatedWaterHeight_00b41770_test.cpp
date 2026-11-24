// Complete unchanged production source and actual shared class models.
#define FABLE_RETAIL_STLP_MAP
// Original Ego_d variable/parameter names, and original same-TU landscape member/helper.
// Leaf: ?PeekInterpolatedWaterHeight@CEngineMap@@QBEMJJJ@Z
// Flags: /d2inlT400
// Inline-budget: retail expands the original same-TU PeekLandscapeHeight; T301 calls it and T400 expands it exactly.
// Retail-asm: 00B419F4 fld dword ptr [esp+2Ch],00B419F8 fistp dword ptr [esp+30h]; shared GFFloatToLongNear.
#define FABLE_RETAIL_CENGINEMAP_PEEKWATER
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

float CEngineMap::PeekInterpolatedWaterHeight(const long mapx, const long mapy, const long distance_to_look) const
{
    long min_i = mapx - distance_to_look;
    long min_j = mapy - distance_to_look;
    long max_i = mapx + distance_to_look;
    long max_j = mapy + distance_to_look;
    if (!PeekHasWater(mapx, mapy)) {
        bool found = false;
        for (long i = min_i; i <= max_i && !found; ++i) {
            for (long j = min_j; j <= max_j && !found; ++j) {
                if (PeekHasWaterFast(i, j)) {
                    min_i = i - distance_to_look;
                    min_j = j - distance_to_look;
                    max_i = i + distance_to_look;
                    max_j = j + distance_to_look;
                    found = true;
                }
            }
        }
        if (!found)
            return 0.0f;
    }
    float total_water_depth = 0.0f;
    float points_with_water = 0.0f;
    for (long i = min_i; i <= max_i; ++i) {
        for (long j = min_j; j <= max_j; ++j) {
            float water_depth = PeekWaterDepth(i, j);
            if (water_depth > 0.001f) {
                total_water_depth += PeekLandscapeHeight(i, j) + water_depth;
                points_with_water += 1.0f;
            }
        }
    }
    return points_with_water > 0.0f ? total_water_depth / points_with_water : 0.0f;
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

static float water[64][64];static bool wet[64][64];static bool centre_wet;
static int centre_calls=0,search_calls=0,depth_calls=0;static long first_i=0,first_j=0,last_i=0,last_j=0;
static bool Valid(long x,long y){return x>=-32&&x<32&&y>=-32&&y<32;}
bool CEngineMap::PeekHasWater(long,long)const {++centre_calls;return centre_wet;}
bool CEngineMap::PeekHasWaterFast(long x,long y)const {if(!search_calls){first_i=x;first_j=y;}++search_calls;last_i=x;last_j=y;return Valid(x,y)&&wet[x+32][y+32];}
float CEngineMap::PeekWaterDepth(long x,long y)const {++depth_calls;return Valid(x,y)?water[x+32][y+32]:0;}
static int failed=0;
#define CHECK(x) do{if(!(x)){++failed;std::printf("FAIL %d\n",__LINE__);}}while(0)
static bool Near(float x,float y){return std::fabs(x-y)<0.00001f;}
static void Reset(bool centre){std::memset(water,0,sizeof(water));std::memset(wet,0,sizeof(wet));centre_wet=centre;centre_calls=search_calls=depth_calls=0;}
static void Wet(long x,long y,float depth){wet[x+32][y+32]=true;water[x+32][y+32]=depth;}
int main(){
 CEngineInit init;CEngineWorldMap world(init);NEngineInternal::GPEngineWorldMap=&world;short grid[4]={0,0,0,0};world.Map=grid;world.MapArrayWidth=2;world.MapArrayHeight=2;world.MapOffsetX=64;world.MapOffsetY=96;
 TestDrawMap own(1),other(2);CEngineMap here(&own,16,16,64,96,0,CCharString(),false),there(&other,16,16,80,96,0,CCharString(),false);
 world.MapList.push_back((CEngineMap*)0);world.MapList.push_back(&here);world.MapList.push_back(&there);
 Reset(true);Wet(2,2,2);own.calls=0;CHECK(here.PeekInterpolatedWaterHeight(2,2,0)==3);CHECK(centre_calls==1&&search_calls==0&&depth_calls==1&&own.calls==1);
 Reset(true);Wet(2,2,2);Wet(1,2,4);own.calls=0;CHECK(here.PeekInterpolatedWaterHeight(2,2,1)==4);CHECK(search_calls==0&&depth_calls==9&&own.calls==2);
 Reset(true);Wet(1,1,0.001f);Wet(1,2,0.00099f);Wet(2,2,0.00101f);own.height=1.00390625f;own.calls=0;CHECK(Near(here.PeekInterpolatedWaterHeight(2,2,1),1.00101f));CHECK(own.calls==1);own.height=1;
 Reset(true);own.calls=0;CHECK(here.PeekInterpolatedWaterHeight(2,2,1)==0);CHECK(depth_calls==9&&own.calls==0);
 Reset(false);own.calls=0;CHECK(here.PeekInterpolatedWaterHeight(3,3,1)==0);CHECK(search_calls==9&&depth_calls==0&&own.calls==0);
 Reset(false);Wet(2,2,4);Wet(1,1,2);Wet(3,3,6);CHECK(here.PeekInterpolatedWaterHeight(3,3,1)==5);CHECK(search_calls==1&&first_i==2&&first_j==2&&depth_calls==9);
 Reset(false);Wet(4,4,8);CHECK(here.PeekInterpolatedWaterHeight(3,3,1)==9);CHECK(search_calls==9&&last_i==4&&last_j==4&&depth_calls==9);
 Reset(true);Wet(-1,-1,2);own.calls=0;CHECK(here.PeekInterpolatedWaterHeight(0,0,1)==3);CHECK(own.calls==1&&own.x==0&&own.y==0);
 Reset(true);Wet(20,3,2);grid[0]=2;other.height=3;other.calls=0;CHECK(here.PeekInterpolatedWaterHeight(20,3,0)==5);CHECK(other.calls==1&&other.x==4&&other.y==3);grid[0]=0;
 Reset(true);CHECK(here.PeekInterpolatedWaterHeight(2,2,-1)==0&&depth_calls==0);
 Reset(false);CHECK(here.PeekInterpolatedWaterHeight(2,2,-1)==0&&search_calls==0&&depth_calls==0);
 std::puts(failed?"00b41770_TEST FAIL":"00b41770_TEST PASS");return failed?1:0;
}