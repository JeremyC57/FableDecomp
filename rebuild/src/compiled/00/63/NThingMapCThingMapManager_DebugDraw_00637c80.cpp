// 00637c80 NThingMap::CThingMapManager::DebugDraw -- draw map debug cells and a blinking position marker.
// Leaf: ?DebugDraw@CThingMapManager@NThingMap@@QBEXABVC2DCoordI@@J@Z
// Retail:00637c80,288 bytes,const thiscall.
#define FABLE_RETAIL_CTHINGMAPMANAGER
#define FABLE_RETAIL_CTHINGMAPMANAGER_DEBUGDRAW
#define FABLE_RETAIL_CTHINGMAP_GETAREA
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRENDERMANAGER2D_LOADINGSCREEN
#include "engine/C2DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CThingMapManager.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CRenderManager2D.h"
extern CRenderManager2D* g_pRenderManager2D;
extern double VIRTUAL_SCREEN_WIDTH,VIRTUAL_SCREEN_HEIGHT;
float FABLE_FASTCALL GFAXToVX(float);
float FABLE_FASTCALL GFAYToVY(float);
void NThingMap::CThingMapManager::DebugDraw(const C2DCoordI& pos,long layer) const
{
 CThingMap* map=GetThingMapAt(pos);
 C2DVector origin((float)(VIRTUAL_SCREEN_WIDTH/20.0),(float)(VIRTUAL_SCREEN_HEIGHT/20.0));
 if(map){
  map->DebugDraw(origin,layer);
  static long count=0;
  if(count%16>8){
   float halfWidth=GFAXToVX(3.0f);
   float halfHeight=GFAYToVY(3.0f);
   C2DCoordI local(pos.X-map->GetArea().GetTLPos().X,pos.Y-map->GetArea().GetTLPos().Y);
   C2DVector point(GFAXToVX((float)local.X)+origin.X,GFAYToVY((float)local.Y)+origin.Y);
   g_pRenderManager2D->Draw2DBox(C2DBoxF(point.X-halfWidth,point.Y-halfHeight,point.X+halfWidth,point.Y+halfHeight),CRGBColour(255,255,255),0);
  }
  ++count;
 }
}