// CTCHighlightItem::DoMovement -- bob and rotate a highlighted object.
// Flags:
// Retail: 0077c960,502 bytes,protected thiscall with no parameters.
// Leaf: ?DoMovement@CTCHighlightItem@@IAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCHIGHLIGHTITEM
#define FABLE_RETAIL_CHIGHLIGHTITEMDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHT_BELOW
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_PEEKINFO
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_CENTER_MIN_FIRST
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CTCHighlightItem.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CHighlightItemDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include <math.h>
template<class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out)const {
 const CSubDefInfo* info=GetPSubDefInfo(T::StatGetTypeName());
 if(info){CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);out->SetDynamicCast(def);return true;}return false;
}
inline double FABLE_FASTCALL GFCos(float angle){return cos(angle*(double)6.2831855f);}
inline float FABLE_FASTCALL GFGetWeightedFractionSlowStartSlowEnd(float fraction){fraction=(float)((1.0-GFCos(fraction*0.5f))/2.0);return fraction;}
void CTCHighlightItem::DoMovement(){
 CDefPointer<const CHighlightItemDef> pdef;
 GetParentThing().PeekBaseDef().GetSubDef(&pdef);
 CTCPhysicsBase* ptc_physics=GetParentThing().GetPhysicsTC();
 C3DVector current_pos=ptc_physics->GetPosition();
 float ground_z=PeekWorldMap().GetHeightZBelowAt(StartPos,0.0f,false,CWorldMap::CThingFilterBase());
 C3DVector new_pos=current_pos;
 float bob_time_turns=CMainGameComponent::GetConstantFPS()*pdef->BobTime;
 float half_bob_time_turns=bob_time_turns*0.5f;
 float rounded_life_timer=(float)fmod((float)LifeTimer,bob_time_turns);
 float fraction;
 if(rounded_life_timer>half_bob_time_turns)fraction=1.0f-GFGetWeightedFractionSlowStartSlowEnd((rounded_life_timer-half_bob_time_turns)/half_bob_time_turns);
 else fraction=GFGetWeightedFractionSlowStartSlowEnd(rounded_life_timer/half_bob_time_turns);
 long graphic_index=GetParentThing().GetTCGraphicAppearance()->GetMainGraphic().BankIndex;
 float scale=GetParentThing().GetTCGraphicAppearance()->GetMainGraphic().RenderSizeX*GetParentThing().GetTCGraphicAppearance()->GetScale()*0.01f;
 float centre_z_offset=GetWorld().GetMeshBank()->PeekMeshInfo(graphic_index)->GetBoundingBox().GetCenter().Z*scale;
 float min_distance_from_ground=pdef->DistanceFromGround-pdef->BobDistance*0.5f;
 new_pos.Z=min_distance_from_ground+fraction*pdef->BobDistance+ground_z-centre_z_offset;
 ptc_physics->SetPosition(new_pos);
 CRightHandedSet rhs=ptc_physics->GetRHSet();
 rhs.Rotate(C3DVector(0.0f,0.0f,1.0f),pdef->RotationSpeed*(1.0f/360.0f));
 ptc_physics->SetRHSet(rhs);
 ptc_physics->SetAsMoving(true);
}