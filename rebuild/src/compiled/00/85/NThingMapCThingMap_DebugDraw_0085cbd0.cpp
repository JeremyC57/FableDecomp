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