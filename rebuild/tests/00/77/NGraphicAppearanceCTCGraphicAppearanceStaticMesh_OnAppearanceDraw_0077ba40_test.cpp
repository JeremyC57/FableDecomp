#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_INLINE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_CTOR_CALLED_PASS26
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
// Leaf: ?OnAppearanceDraw@CTCGraphicAppearanceStaticMesh@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z
// Flags: /d2inlT200
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_INTERPOLATE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_PASS22_INTERPOLATE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PASS22_PROXIMITY
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_NGRAPHICAPPEARANCE_PASS22_PRIMITIVETYPE
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearanceStaticMesh.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/CEnginePrimitiveStaticMesh.h"
#include "engine/C3DPositionInterpolator.h"
#include "engine/C3DOrientationInterpolator.h"
namespace NGlobalConsole {extern long MultipleStaticMeshesToDraw;}

#include <cstdio>
static int errors=0,creates=0,refreshes=0,draws=0,effect_queries=0,deleted=0,position_queries=0,orientation_queries=0;
#define CHECK(x) do{if(!(x)){++errors;std::printf("FAIL %d\n",__LINE__);}}while(0)
namespace NGlobalConsole {long MultipleStaticMeshesToDraw=-1;}
void CIVCountedPointeeBase::Delete() const { CHECK(false); }
struct FixtureEffect: CIVCountedPointeeBase {virtual void Delete()const{++deleted;} };
static FixtureEffect effect;static bool effect_null=false;static unsigned long effect_initial=1;
struct FixtureThing {unsigned __int64 uid;bool bullet;unsigned __int64 GetUniqueID()const{return uid;}bool IsAffectedByBulletTime()const{return bullet;}};
struct FixturePos {C3DVector GetInterpolated(float t)const{++position_queries;CHECK(t==.25f);return C3DVector(1,2,3);}};
struct FixtureOrientation {CRightHandedSet GetInterpolated(float t)const{++orientation_queries;CHECK(t==.25f);CRightHandedSet r;r.SetUp(C3DVector(0,0,1));r.SetForward(C3DVector(0,1,0));return r;}};
struct FixtureDrawInfo {float Interpolate;FixturePos*Pos;FixtureOrientation*Orientation;unsigned long ReasonsForDraw;};
namespace NGraphicAppearance {
struct FixturePrimitive {EPrimitiveType Type;long BankIndex;FixturePrimitive(EPrimitiveType t,long b):Type(t),BankIndex(b){}};
struct FixtureMesh {CIVCountedPointer<FixtureEffect> MeshEffect;FixtureMesh(unsigned __int64 uid,unsigned long bank,const C3DVector&p,const CRightHandedSet&r,float scale,bool lit,unsigned char alpha,CRGBColour tint,float fade,EEngineInterpolationSet interp,const CEnginePrimitiveThemeChannelDesc&channel,bool bullet){CHECK(uid==123&&bank==9&&p.X==1&&p.Y==2&&p.Z==3&&r.GetUp().Z==1&&r.GetForward().Y==1&&scale==6&&lit&&alpha==127&&tint.R==10&&tint.G==20&&tint.B==30&&tint.A==40&&fade==17&&(int)interp==0&&channel.ChannelIndex1==7&&bullet);}};
struct FixtureAppearance {CEngineGraphic graphic;CEnginePrimitiveThemeChannelDesc channel;FixturePrimitive stored;FixtureAppearance():stored(GAPT_STATIC_MESH,9){graphic.BankIndex=9;graphic.RenderSizeX=3;channel.ChannelIndex0=0;channel.ChannelIndex1=7;}const CEngineGraphic&GetMainGraphic()const{return graphic;}float GetScale()const{return 2;}float GetFadeDistance()const{return 17;}FixturePrimitive*AddPersistentPrimitive(const FixturePrimitive&p){++creates;CHECK(p.Type==GAPT_STATIC_MESH&&p.BankIndex==9);return &stored;}void RefreshPrimitivePos(FixturePrimitive*p,const FixturePos&,const FixtureOrientation&){++refreshes;CHECK(p==&stored);}CEnginePrimitiveThemeChannelDesc PeekLightingChannel()const{return channel;}unsigned char GetAlpha()const{return 127;}const CRGBColour GetTintColour()const{return CRGBColour(10,20,30,40);}CIVCountedPointer<FixtureEffect> GetPMeshEffect(){++effect_queries;return CIVCountedPointer<FixtureEffect>(effect_null?0:&effect);}void SetPrimitiveGraphics(FixturePrimitive*p,const FixtureMesh&m,unsigned short flags){++draws;CHECK(p==&stored&&flags==0);CHECK(effect.IVRefCount==(effect_initial));CHECK(deleted==0);}};
struct FixtureOwner {FixturePrimitive*PrimHandle;FixtureThing thing;FixtureOwner():PrimHandle(0){thing.uid=123;thing.bullet=true;}FixtureThing&GetParentThing(){return thing;}bool OnAppearanceDraw(FixtureAppearance&,const FixtureDrawInfo&);};
}
#define CTCGraphicAppearanceStaticMesh FixtureOwner
#define CTCGraphicAppearance FixtureAppearance
#define CGraphicAppearanceDrawInfo FixtureDrawInfo
#define CEnginePrimitiveStaticMesh FixtureMesh
#define CPrimitive FixturePrimitive
namespace NGraphicAppearance {
bool CTCGraphicAppearanceStaticMesh::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance,const CGraphicAppearanceDrawInfo& draw_info)
{
 const CEngineGraphic& graphic=tc_appearance.GetMainGraphic();
 if(graphic.BankIndex)
 {
  float scale=tc_appearance.GetScale()*graphic.RenderSizeX;
  float fade_dist=tc_appearance.GetFadeDistance();
  if(!PrimHandle)
   PrimHandle=tc_appearance.AddPersistentPrimitive(CPrimitive(GAPT_STATIC_MESH,graphic.BankIndex));
  else if(draw_info.ReasonsForDraw==1) {
   tc_appearance.RefreshPrimitivePos(PrimHandle,*draw_info.Pos,*draw_info.Orientation);
   return true;
  }
  CEnginePrimitiveThemeChannelDesc lighting_theme_channel=tc_appearance.PeekLightingChannel();
  if(NGlobalConsole::MultipleStaticMeshesToDraw<0 || lighting_theme_channel.ChannelIndex0==0)
  {
   bool bullet_time_affects_cloth=GetParentThing().IsAffectedByBulletTime();
   CEnginePrimitiveStaticMesh mesh_prim(GetParentThing().GetUniqueID(),graphic.BankIndex,
    draw_info.Pos->GetInterpolated(draw_info.Interpolate),draw_info.Orientation->GetInterpolated(draw_info.Interpolate),
    scale,true,tc_appearance.GetAlpha(),tc_appearance.GetTintColour(),fade_dist,(EEngineInterpolationSet)0,
    tc_appearance.PeekLightingChannel(),bullet_time_affects_cloth);
   mesh_prim.MeshEffect=tc_appearance.GetPMeshEffect();
   tc_appearance.SetPrimitiveGraphics(PrimHandle,mesh_prim,0);
  }
 }
 return true;
}
}

#undef CTCGraphicAppearanceStaticMesh
#undef CTCGraphicAppearance
#undef CGraphicAppearanceDrawInfo
#undef CEnginePrimitiveStaticMesh
#undef CPrimitive
int main(){using namespace NGraphicAppearance;for(int mode=0;mode<12;++mode){FixtureOwner owner;FixtureAppearance appearance;FixturePos pos;FixtureOrientation orient;FixtureDrawInfo info;info.Interpolate=.25f;info.Pos=&pos;info.Orientation=&orient;info.ReasonsForDraw=(mode==2||mode==3)?1:2;if(mode==0)appearance.graphic.BankIndex=0;if(mode==1||mode==2||mode==6)owner.PrimHandle=&appearance.stored;if(mode==4||mode==6)appearance.channel.ChannelIndex0=2;NGlobalConsole::MultipleStaticMeshesToDraw=mode==4||mode==5||mode==6?0:-1;effect_null=mode==7||mode==8;effect_initial=mode>=9?2:1;effect.IVRefCount=effect_initial;creates=refreshes=draws=effect_queries=deleted=position_queries=orientation_queries=0;CHECK(owner.OnAppearanceDraw(appearance,info));bool refresh=mode==2;bool draw=mode!=0&&!refresh&&mode!=4&&mode!=6;CHECK(creates==((mode!=0&&mode!=1&&mode!=2&&mode!=6)?1:0));CHECK(refreshes==(refresh?1:0));CHECK(draws==(draw?1:0)&&effect_queries==draws&&position_queries==draws&&orientation_queries==draws);CHECK(deleted==((draw&&!effect_null&&effect_initial==1)?1:0));CHECK(effect.IVRefCount==effect_initial-(draw&&!effect_null?1:0));}std::puts(errors?"0077ba40_TEST FAIL":"0077ba40_TEST PASS");return errors?1:0;}