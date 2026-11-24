// 0064e978 NPlayerGui::CDrawReceivedItems::Draw
// Leaf: ?Draw@CDrawReceivedItems@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_HIGHLIGHT_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWRECEIVEDITEMS_PASS23
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_GLOBALUPDATE
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_DRAW_ACCESSORS_PASS23
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GENERATED_COPY_PASS23
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR
#define FABLE_RETAIL_C3DANIMATIONSTATE
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_GENERATED_SPECIALS_PASS23
#define FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_GENERATED_DTOR_PASS23
#define FABLE_RETAIL_C3DANIMATIONSTATE_ORIGINAL_CTORS_PASS23
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_C3DANIMATIONSTATEALPHA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_C3DANIMATIONSTATEBASE_VIRTUALS
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_EFFECT_ENTRY_PASS23
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CENGINEPRIMITIVE2DANIMATINGMESH_IMPLICIT_DTOR_PASS23
#define FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED
#include "engine/EEnginePrimitiveType.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/NPlayerGui_CDrawReceivedItems.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingObject.h"
#include "engine/CTCHeroReceiveItems.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/GFGetInterpolatedValue.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/CEnginePrimitive2DStaticMesh.h"
#include "engine/CEnginePrimitive2DAnimatingMesh.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/C3DAnimationState.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CMeshInfo.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;
// Genuine body composition from original FableWin01e1dfa4/01e1decf.
// Plain inline context only; native callers retain out-of-line calls.
inline C3DAnimationInterpolator::C3DAnimationInterpolator(const C3DAnimationBlendState& a,const C3DAnimationBlendState& b)
 :StartState(a),EndState(b){}
inline CEnginePrimitive2DAnimatingMesh::CEnginePrimitive2DAnimatingMesh(unsigned __int64 id,unsigned long mesh,const C3DAnimationBlendState& anim,const C2DBoxF& box,const C3DBoundingSphere& sphere,const C3DOrientationInterpolator& orientation,EEngineInterpolationSet interpolation,unsigned char alpha,CRGBColour tint)
 :CEnginePrimitive(ENGINE_PRIMITIVE_2D_ANIMATING_MESH,id,interpolation),ClipBox(box),MeshIndex(mesh),Alpha(alpha),Orientation(orientation),Anim(anim,anim),BoundingSphere(sphere),TintColour(tint){}
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
void NPlayerGui::CDrawReceivedItems::Draw(const CEnginePrimitivePrepareInfo& info)
{
 const CThingPlayerCreature* phero=GetPlayer().PeekPControlledCreature();
 if(IS_THING_ALIVE(phero)){
 const CTCHeroReceiveItems* ptc_receive_items;
 if(phero->PeekTC(&ptc_receive_items)&&ptc_receive_items->IsDisplayingItems()){
  Drawing=true;
  PrimitiveHandles.resize(2);
  C2DExtentsI screen_extents=GPDisplayManager->GetRenderTargetDimensions();
  C2DBoxF background_box(0.0f,0.0f,(float)screen_extents.X,(float)screen_extents.Y);
  const CRGBColour& colour=CPlayerGui::PeekGuiDef().ContainerItemDisplayBackgroundColour;
  CEnginePrimitive2DSprite background_sprite(0,GFToScreen(background_box),ENGINE_2D_LAYER_7,0,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,colour.A,colour.R,colour.G,colour.B,0,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(PrimitiveHandles[0],background_sprite,0xc0);
  CThingObject* pthing=ptc_receive_items->GetPThingObject();
  if(IS_THING_ALIVE(pthing)){
   float interpolate=info.Interpolate.PausedCurrent.WFInterpolate;
   const C2DBoxF& old_area=ptc_receive_items->GetOldAreaItemTaking();
   const C2DBoxF& new_area=ptc_receive_items->GetCurrentAreaItemTaking();
   C2DBoxF clip_box;
   clip_box.TLX=GFGetInterpolatedValue(old_area.TLX,new_area.TLX,interpolate);
   clip_box.TLY=GFGetInterpolatedValue(old_area.TLY,new_area.TLY,interpolate);
   clip_box.BRX=GFGetInterpolatedValue(old_area.BRX,new_area.BRX,interpolate);
   clip_box.BRY=GFGetInterpolatedValue(old_area.BRY,new_area.BRY,interpolate);
   C3DOrientationInterpolator orientation(ptc_receive_items->GetOldRHS(),ptc_receive_items->GetCurrentRHS());
   std::vector<NGraphicAppearance::CMeshInfo> meshes;
   pthing->GetTCGraphicAppearance()->GetAllMeshes(meshes);
   if(pthing->GetTCGraphicAppearance()->GetMainGraphic().Type==5){
    CEnginePrimitive2DStaticMesh item_mesh(0,meshes[0].Index,clip_box,orientation,ENGINE_INTERPOLATION_SET_1,255,CRGBColour(255,255,255,255));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1],item_mesh,0xc0);
   }else if(pthing->GetTCGraphicAppearance()->GetMainGraphic().Type==4){
    CDefPointer<const CThingObjectDef> pobject_def;
    CMainGameComponent::Get()->PeekDefinitionManager().GetDef(pthing->GetDefGlobalIndex(),&pobject_def);
    CDefPointer<const CAppearanceDef> pappearance_def;
    pobject_def->GetSubDef(&pappearance_def);
    C3DAnimationBlendState anim_blend;
    anim_blend.SetBaseAnimation(C3DAnimationState(pappearance_def->Animation.GetAnim("DEFAULT",0,CAnimFilterNoGroup()).BankIndex,0.0f));
    C3DBoundingSphere bounding_sphere(C3DVector(0,0,0),1.0f);
    CEnginePrimitive2DAnimatingMesh item_mesh(0,meshes[0].Index,anim_blend,clip_box,bounding_sphere,orientation,ENGINE_INTERPOLATION_SET_1,255,CRGBColour(255,255,255,255));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1],item_mesh,0xc0);
   }
  }
 }
}
}