// 0076e6a0 CTCWeaponTrail::DrawAddWeaponTrails
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
// Leaf: ?DrawAddWeaponTrails@CTCWeaponTrail@@QBEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
#define FABLE_RETAIL_WEAPON_TRAIL_DRAW_PASS29
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CTHINGGAMEOBJECT_WEAPONTRAIL
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBAL
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingCreatureBase.h"
#include "engine/CTCWeaponTrail.h"
#include "engine/CTCTrapBase.h"
#include "engine/CTCCarryable.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingBaseDef.h"
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CWEAPONDEF_SIZE_BODY   // NF357: GetSubDef<CWeaponDef> pick-any copy = size-mode owner 0x0042B1A6 (scaffolding, not retail)
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/ThingAlive.h"

#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/GFSqr.h"
#include "engine/GFLimit.h"
#include "engine/EEngineInterpolationSet.h"
namespace { extern long NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE; }
void CTCWeaponTrail::DrawAddWeaponTrails(const CEnginePrimitivePrepareInfo& info) const {
    if(!GetParentThing().GetPBaseDef().IsNull()) {
        C3DVector lod_centre=CMainGameComponent::Get()->GetWorld().GetGameLODCentre();
        if((GetParentThing().GetPos()-lod_centre).GetSquaredMagnitude()>GFSqr(GetParentThing().GetPBaseDef()->RenderFadeDistance)) {
            ProjectileWeaponTrailHandle.Clear();
            WeaponTrailHandles.clear();
            return;
        }
    }
    float current_time=((float)PeekWorld().GetFrame())/CMainGameComponent::GetConstantFPS();
    const CGlobalDef& glob_def=GetDefinitionManager().PeekGlobalDef();
    ProjectileWeaponTrailHandle.Clear();
    WeaponTrailHandles.clear();
    if(!ProjectileWeaponTrail.empty()) {
        if(GetParentThing().PeekBaseDef().DrawProjectileWeaponTrails) {
            long gfxweapontrail=glob_def.GraphicWeaponTrailArrow;
            const NThingShot::CTCShotDefinitionBase* ptc_thing_shot_base;
            if(GetParentThing().PeekTC(&ptc_thing_shot_base)) gfxweapontrail=ptc_thing_shot_base->GetWeaponTrailGraphic();
            float interpolate=info.Interpolate.Current.WFInterpolate;
            float skip_points=1.0f-interpolate;
            if(gfxweapontrail>0) info.Engine.SetPersistentPrimitive(ProjectileWeaponTrailHandle,CEnginePrimitiveWeaponTrail(GetParentThing().GetUniqueID(),ProjectileWeaponTrail,gfxweapontrail,0,CEnginePrimitiveWeaponTrail::WEAPON_TRAIL_TYPE_PROJECTILE,(unsigned char)glob_def.WeaponTrailArrowAlpha,skip_points,current_time,glob_def.WeaponTrailArrowAlphaFadeOffTime,0.0f,ENGINE_INTERPOLATION_SET_DEFAULT),0);
        }
    } else if(GetParentThing().PeekBaseDef().DrawWeaponTrails && NoTrails>0) {
        const CThingGameObject& game_object=*GFDebugDynamicCast(&GetParentThing(),(const CThingGameObject*)0);
        long weapon_trail_index=game_object.GetWeaponTrailGraphicIndex();
        WeaponTrailHandles.resize(NoTrails);
        for(long c1=0;c1<NoTrails;++c1) if(!Trail[c1].empty() && weapon_trail_index>0) {
            unsigned long frame=0;
            float alpha_interp=1.0f,skip_points;
            if(ActiveTrail[c1]) {
                float interpolate=info.Interpolate.Current.WFInterpolate;
                skip_points=(1.0f-interpolate)*NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE;
            } else {
                skip_points=0.0f;
                alpha_interp=1.0f-(float)InactiveTicks[c1]/glob_def.WeaponTrialNumFramesToFadeOut;
            }
            GFLimit(alpha_interp,0.0f,1.0f);
            info.Engine.SetPersistentPrimitive(WeaponTrailHandles[c1],CEnginePrimitiveWeaponTrail(GetParentThing().GetUniqueID(),Trail[c1],weapon_trail_index,frame,TrailType,(unsigned char)(glob_def.WeaponTrailAlpha*alpha_interp),skip_points,current_time,glob_def.WeaponTrailAlphaFadeOffTime,0.0f,ENGINE_INTERPOLATION_SET_DEFAULT),0);
        }
    }
}