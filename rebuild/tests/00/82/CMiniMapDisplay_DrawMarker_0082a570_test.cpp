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

#include <cstdio>
#include <cmath>
static int errors=0,component_get_calls=0,mapcalls=0,regioncalls=0,clipcalls=0,drawposcalls=0,draws=0,cameracalls=0,physicscalls=0,bankcalls=0;static float exwidth=16,exheight=8,exangle=0;
#define CHECK(x) do{if(!(x)){++errors;std::printf("FAIL %d %s\n",__LINE__,#x);}}while(0)
static bool near(float a,float b){return std::fabs(a-b)<.0001f;}
struct FixtureCamera {float GetAngleXY()const{++cameracalls;return .2f;}};
struct FixtureManager {FixtureCamera camera;const FixtureCamera&GetCamera()const{return camera;}};
struct FixtureGameCamera {FixtureManager manager;bool GetCameraManager(const FixtureManager**p)const{*p=&manager;return true;}};
struct FixtureWorldMap {long region;long GetMapNumberAt(const C3DVector&p)const{++mapcalls;CHECK(p.X==3&&p.Y==4&&p.Z==5);return 7;}long GetRegionNumberMapIsIn(long n)const{++regioncalls;CHECK(n==7);return region;}long GetLoadedRegion()const{return 11;}};
struct FixtureWorld {FixtureWorldMap map;FixtureGameCamera camera;const FixtureWorldMap&PeekWorldMap()const{return map;}const FixtureGameCamera*PeekGameCamera()const{return &camera;}};
struct FixturePhysics {float GetFacingAngleXY()const{++physicscalls;return .3f;}};
struct FixtureThing {bool dead,limbo,hero,hasphys;FixturePhysics phys;C3DVector pos;FixtureThing():dead(false),limbo(false),hero(false),hasphys(true),pos(3,4,5){}bool IsDead()const{return dead;}bool IsInLimbo()const{return limbo;}const C3DVector&GetPos()const{return pos;}unsigned long GetTypeFlags()const{return hero?0x20:0;}const FixturePhysics*GetPhysicsTC()const{return hasphys?&phys:0;}};
struct FixtureMarker {const FixtureThing*thing;bool rotate,reverse,pulse;const FixtureThing*GetPThing()const{return thing;}long GetGraphicIndex()const{return 19;}bool IsRotatedOnMap()const{return rotate;}bool IsReversedOnMap()const{return reverse;}bool IsPulsingOnMap()const{return pulse;}};
struct FixtureBank {unsigned long width,height;unsigned long GetGraphicFrameWidth(unsigned long g,unsigned long f)const{++bankcalls;CHECK(g==19&&f==0);return width;}unsigned long GetGraphicFrameHeight(unsigned long g,unsigned long f)const{++bankcalls;CHECK(g==19&&f==0);return height;}};
struct FixtureComponent {FixtureWorld world;FixtureBank bank;static FixtureComponent*current;static FixtureComponent*Get(){++component_get_calls;return current;}const FixtureWorld&PeekWorld()const{return world;}const FixtureBank&PeekEngineGraphics()const{return bank;}};FixtureComponent*FixtureComponent::current=0;
struct FixtureFade {mutable int calls;FixtureFade():calls(0){}float GetFadeInOutAlpha()const{return calls++==0?.25f:.5f;}};
struct FixtureSprite {C2DBoxF box;EEngine2DLayer layer;FixtureSprite(unsigned __int64 id,const C2DBoxF&b,EEngine2DLayer l,unsigned long bank,unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interp,float a,float z,unsigned char alpha,unsigned char r,unsigned char g,unsigned char blue,unsigned long flags,const C2DVector&uv0,const C2DVector&uv1):box(b),layer(l){CHECK(id==0&&bank==19&&frame==0&&blend==ENGINE_PRIMITIVE_BLEND_ALPHA&&interp==ENGINE_INTERPOLATION_SET_0&&near(a,exangle)&&near(z,exangle)&&alpha==123&&r==255&&g==255&&blue==255&&flags==2&&uv0.X==0&&uv0.Y==0&&uv1.X==0&&uv1.Y==0);CHECK(near(box.TLX,100-exwidth*.5f)&&near(box.TLY,200-exheight*.5f)&&near(box.BRX,100+exwidth*.5f)&&near(box.BRY,200+exheight*.5f));}};
static EEngine2DLayer exlayer;
struct FixtureEngine {void SetPersistentPrimitive(int&h,const FixtureSprite&s,unsigned long f){++draws;CHECK(h==73&&s.layer==exlayer&&f==0xc0);}};
struct FixtureInfo {FixtureEngine&Engine;FixtureInfo(FixtureEngine&e):Engine(e){}};
struct FixtureMiniMap {typedef FixtureMarker CMarkerInfo;int MarkerPrimitiveHandles[4];FixtureFade MarkerFlashFade;bool clip;FixtureMiniMap():clip(true){MarkerPrimitiveHandles[2]=73;}bool IsThingAsPosBeingDrawn(const C3DVector&)const{++clipcalls;return clip;}C2DVector GetDrawPosition(const C3DVector&,const C2DBoxF&)const{++drawposcalls;return C2DVector(100,200);}bool DrawMarker(const FixtureInfo&,const FixtureMarker&,long,const C2DBoxF&,unsigned char,bool);};
namespace NPlayerGui {C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF&b){return b;}}
#define CMiniMapDisplay FixtureMiniMap
#define CThing FixtureThing
#define CWorld FixtureWorld
#define CWorldMap FixtureWorldMap
#define CMainGameComponent FixtureComponent
#define CGraphicDataBank FixtureBank
#define CGameCameraManager FixtureManager
#define CEnginePrimitivePrepareInfo FixtureInfo
#define CEnginePrimitive2DSprite FixtureSprite
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

#undef CMiniMapDisplay
#undef CThing
#undef CWorld
#undef CWorldMap
#undef CMainGameComponent
#undef CGraphicDataBank
#undef CGameCameraManager
#undef CEnginePrimitivePrepareInfo
#undef CEnginePrimitive2DSprite
int main(){for(int mode=0;mode<24;++mode){FixtureComponent component;component.world.map.region=mode==5?12:11;component.bank.width=mode==13?0:16;component.bank.height=mode==13?0:8;FixtureComponent::current=&component;FixtureThing thing;thing.dead=mode==1;thing.limbo=mode==2;thing.hero=mode==4||mode==8||(mode>=16&&(mode&1));thing.hasphys=mode!=6;FixtureMarker marker;marker.thing=mode==0?0:&thing;marker.rotate=mode==8||mode==9||mode==10||(mode>=16&&(mode&2));marker.reverse=mode==7||mode==10||(mode>=16&&(mode&4));marker.pulse=mode==11||(mode>=16&&(mode&8));FixtureMiniMap map;map.clip=mode!=3&&mode!=4;bool change=mode==12||(mode>=16&&(mode&4));FixtureEngine engine;FixtureInfo info(engine);C2DBoxF box(0,0,256,128);bool visible=mode!=0&&mode!=1&&mode!=2&&mode!=3&&mode!=5;exwidth=(float)component.bank.width*(thing.hero?1.0f:box.GetWidth()/256.0f);exheight=(float)component.bank.height*(thing.hero?1.0f:box.GetHeight()/256.0f);if(marker.pulse){exwidth*=.75f;exheight*=1.0f;}exangle=thing.hasphys&&marker.rotate?(thing.hero?.8f:.7f):0;if(thing.hasphys&&marker.reverse)exangle+=.5f;exlayer=change?ENGINE_2D_LAYER_0:thing.hero?ENGINE_2D_LAYER_1:ENGINE_2D_LAYER_2;component_get_calls=mapcalls=regioncalls=clipcalls=drawposcalls=draws=cameracalls=physicscalls=bankcalls=0;bool result=map.DrawMarker(info,marker,2,box,123,change);CHECK(result==(visible&&thing.hero));CHECK(draws==(visible?1:0)&&drawposcalls==draws&&bankcalls==draws*2&&component_get_calls==1+draws);CHECK(cameracalls==((visible&&thing.hasphys&&thing.hero&&marker.rotate)?1:0));CHECK(physicscalls==((visible&&thing.hasphys&&!thing.hero&&marker.rotate)?1:0));CHECK(map.MarkerFlashFade.calls==((visible&&marker.pulse)?2:0));}std::puts(errors?"0082a570_TEST FAIL":"0082a570_TEST PASS");return errors?1:0;}