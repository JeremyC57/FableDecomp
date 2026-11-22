#define FABLE_RETAIL_CRESOURCELIST
// Leaf: ?DrawMarker@CMiniMapDisplay@@IAE_NABVCEnginePrimitivePrepareInfo@@ABVCMarkerInfo@1@JABVC2DBoxF@@E_N@Z
// Flags: (none)
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_DRAWMARKERS
#define FABLE_RETAIL_CMINIMAPDISPLAY_ADDMARKER
#define FABLE_RETAIL_CMINIMAPDISPLAY_MARKER_DRAW_PASS26
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CCAMERA_ANGLES
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CMiniMapDisplay.h"
#include "engine/ThingAlive.h"
#include "engine/CCountedPointer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
class CDataInputStream;class CDataOutputStream;
#include "engine/CGraphicDataBank.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CIEngine.h"
#include "engine/C2DExtentsF.h"
namespace NPlayerGui {C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF&);}
bool CMiniMapDisplay::DrawMarker(const CEnginePrimitivePrepareInfo& info,const CMarkerInfo& marker_info,long index,const C2DBoxF& box,unsigned char alpha,bool region_change)
{
 const CWorld& world=CMainGameComponent::Get()->PeekWorld();
 const CWorldMap& world_map=world.PeekWorldMap();
 float marker_angle=0.0f;
 bool hero_marker=false;
 C2DVector marker_offset(0.0f,0.0f);
 const CThing* pthing=marker_info.GetPThing();
 if(IS_THING_ALIVE(pthing)&&!pthing->IsInLimbo()) {
  C3DVector thing_pos=pthing->GetPos();
  if(pthing->GetTypeFlags()&0x20) hero_marker=true;
  else if(!IsThingAsPosBeingDrawn(thing_pos)) return false;
  long map_number=world_map.GetMapNumberAt(thing_pos);
  if(world_map.GetRegionNumberMapIsIn(map_number)!=world_map.GetLoadedRegion()) return false;
  if(pthing->GetPhysicsTC()) {
   if(marker_info.IsRotatedOnMap()) {
    if(hero_marker) {
     const CGameCameraManager* pcam_man;
     world.PeekGameCamera()->GetCameraManager(&pcam_man);
     marker_angle=1.0f-pcam_man->GetCamera().GetAngleXY();
    } else marker_angle=1.0f-pthing->GetPhysicsTC()->GetFacingAngleXY();
   }
   if(marker_info.IsReversedOnMap()) marker_angle+=0.5f;
  }
  marker_offset=GetDrawPosition(thing_pos,box);
  EEngine2DLayer layer=ENGINE_2D_LAYER_1;
  float scale_x=1.0f,scale_y=1.0f;
  if(!hero_marker) {
   layer=ENGINE_2D_LAYER_2;
   scale_x=box.GetWidth()/256.0f;
   scale_y=box.GetHeight()/256.0f;
  }
  if(marker_info.IsPulsingOnMap()) {
   scale_x=(MarkerFlashFade.GetFadeInOutAlpha()+0.5f)*scale_x;
   scale_y=(MarkerFlashFade.GetFadeInOutAlpha()+0.5f)*scale_y;
  }
  if(region_change)layer=ENGINE_2D_LAYER_0;
  const CGraphicDataBank& bank=CMainGameComponent::Get()->PeekEngineGraphics();
  float marker_width=(float)bank.GetGraphicFrameWidth(marker_info.GetGraphicIndex(),0)*scale_x;
  float marker_height=(float)bank.GetGraphicFrameHeight(marker_info.GetGraphicIndex(),0)*scale_y;
  marker_offset.X-=(marker_width/2.0f);
  marker_offset.Y-=(marker_height/2.0f);
  C2DBoxF marker_box(marker_offset,C2DExtentsF(marker_width,marker_height));
  CEnginePrimitive2DSprite marker_sprite(0,NPlayerGui::GFToScreen(marker_box),layer,marker_info.GetGraphicIndex(),0,
    ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,marker_angle,marker_angle,alpha,255,255,255,2,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(MarkerPrimitiveHandles[index],marker_sprite,0xc0);
 }
 return hero_marker;
}