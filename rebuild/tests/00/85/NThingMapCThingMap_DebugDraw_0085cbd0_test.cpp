#define FABLE_RETAIL_STLP_VECTOR
// Original named locals and box-from-vector/extents helper; full shared owner.
// Leaf: ?DebugDraw@CThingMap@NThingMap@@QAEXABVC2DVector@@J@Z
// Flags: (none)
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CRENDERMANAGER2D_LOADINGSCREEN
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CRenderManager.h"
extern double VIRTUAL_SCREEN_WIDTH;
extern CRenderManager* GPRenderManager;
float FABLE_FASTCALL GFXToY(float x);


#include <cstdio>
#include <cmath>
static int failures=0,calls=0;
#define CHECK(x) do{if(!(x)){++failures;std::printf("FAIL %d\n",__LINE__);}}while(0)
struct Draw{C2DBoxF box;CRGBColour colour;};static Draw draws[32];
struct FixtureRenderer{bool Draw2DBox(const C2DBoxF&b,const CRGBColour&c,unsigned long flags){CHECK(flags==0&&calls<32);draws[calls].box=b;draws[calls++].colour=c;return true;}};
static FixtureRenderer renderer;static FixtureRenderer*FixtureGlobal=&renderer;
double VIRTUAL_SCREEN_WIDTH=100;
float FABLE_FASTCALL GFXToY(float x){return x*.5f;}
namespace NThingMap {
long MaxLayers=2;
const CLayerInfo LayerInfo[]={{-1,0,CRGBColour(1,2,3,4),CRGBColour(5,6,7,8)},{1,0,CRGBColour(9,10,11,12),CRGBColour(13,14,15,16)}};
CThingMapCell::CThingMapCell():PFirstNode(0){}
CMapwhoLevelInfo::CMapwhoLevelInfo(){}
CMapwhoLevelInfo::CMapwhoLevelInfo(const CMapwhoLevelInfo&o):Size(o.Size),Extents(o.Extents),ScaleShift(o.ScaleShift),NoThings(o.NoThings){}
struct FixtureMap{std::vector<CMapwhoLevelInfo>MapwhoLevelInfo;std::vector<std::vector<CThingMapCell> >Cells;FixtureMap():MapwhoLevelInfo(2),Cells(2){MapwhoLevelInfo[0].Size=-1;MapwhoLevelInfo[0].Extents.X=MapwhoLevelInfo[0].Extents.Y=1;Cells[0].resize(1);MapwhoLevelInfo[1].Size=1;MapwhoLevelInfo[1].Extents.X=3;MapwhoLevelInfo[1].Extents.Y=2;Cells[1].resize(6);}const CThingMapCell&PeekCell(unsigned char layer,unsigned long x,unsigned long y)const{return Cells[layer][MapwhoLevelInfo[layer].Extents.X*y+x];}void DebugDraw(const C2DVector&,long);};
}
#define CThingMap FixtureMap
#define GPRenderManager FixtureGlobal
void NThingMap::CThingMap::DebugDraw(const C2DVector& win_tl,long layer)
{
 float map_draw_size=VIRTUAL_SCREEN_WIDTH*0.4;
 long c1=layer;
 if(layer<0||layer>=MaxLayers)return;
 float pix_stepsizex=map_draw_size/MapwhoLevelInfo[c1].Extents.X;
 float pix_stepsizey=GFXToY(pix_stepsizex);
 C2DVector tlpos=win_tl;
 if(MapwhoLevelInfo[c1].Size==-1) {
  if(PeekCell((unsigned char)c1,0,0).GetPFirstNode())
   GPRenderManager->Draw2DBox(C2DBoxF(0,0,map_draw_size,map_draw_size),LayerInfo[c1].UsedColour,0);
  else GPRenderManager->Draw2DBox(C2DBoxF(0,0,map_draw_size,map_draw_size),LayerInfo[c1].UnusedColour,0);
 } else {
  for(long cy=0;cy<MapwhoLevelInfo[c1].Extents.Y;++cy) {
   for(long cx=0;cx<MapwhoLevelInfo[c1].Extents.X;++cx) {
    if(PeekCell((unsigned char)c1,cx,cy).GetPFirstNode())
     GPRenderManager->Draw2DBox(C2DBoxF(tlpos,C2DExtentsF(pix_stepsizex,pix_stepsizey)),LayerInfo[c1].UsedColour,0);
    else GPRenderManager->Draw2DBox(C2DBoxF(tlpos,C2DExtentsF(pix_stepsizex,pix_stepsizey)),LayerInfo[c1].UnusedColour,0);
    tlpos.X+=pix_stepsizex;
   }
   tlpos.Y+=pix_stepsizey;tlpos.X=win_tl.X;
  }
 }
}

#undef CThingMap
#undef GPRenderManager
static bool Near(float x,float y){return std::fabs(x-y)<.00001f;}
int main(){NThingMap::FixtureMap map;C2DVector origin(7,11);calls=0;map.DebugDraw(origin,-1);map.DebugDraw(origin,2);CHECK(calls==0);
 for(int wet=0;wet<2;++wet){map.Cells[0][0].PFirstNode=wet?(NThingMap::CMapwhoNode*)1:0;calls=0;map.DebugDraw(origin,0);CHECK(calls==1);CHECK(draws[0].box.TLX==0&&draws[0].box.TLY==0&&draws[0].box.BRX==40&&draws[0].box.BRY==40);CHECK(draws[0].colour.R==(wet?1:5));}
 for(int pattern=0;pattern<4;++pattern){for(int c=0;c<6;++c)map.Cells[1][c].PFirstNode=((c+pattern)%3)?(NThingMap::CMapwhoNode*)1:0;calls=0;map.DebugDraw(origin,1);CHECK(calls==6);for(int c=0;c<6;++c){float x=7+(c%3)*(40.0f/3),y=11+(c/3)*(20.0f/3);CHECK(Near(draws[c].box.TLX,x)&&Near(draws[c].box.TLY,y)&&Near(draws[c].box.BRX,x+40.0f/3)&&Near(draws[c].box.BRY,y+20.0f/3));CHECK(draws[c].colour.R==(((c+pattern)%3)?9:13));}}
 map.MapwhoLevelInfo[1].Extents.Y=0;calls=0;map.DebugDraw(origin,1);CHECK(calls==0);std::puts(failures?"0085cbd0_TEST FAIL":"0085cbd0_TEST PASS");return failures?1:0;}